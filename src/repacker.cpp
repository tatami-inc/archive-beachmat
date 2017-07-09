#include "beachmat.h"
#include "functions.h"

/********************* A rechunking class ************************/

template<typename T, bool use_size>
class rechunker {
public: 
    rechunker(const std::string& input_file, const std::string& input_data, 
              const std::string& output_file, const std::string& output_data,
              int compress, size_t cs, bool br) : 
        ihfile(H5std_string(input_file), H5F_ACC_RDONLY),
        ihdata(ihfile.openDataSet(H5std_string(input_data))),
        HDT(ihdata.getDataType()),
        ohfile(H5std_string(output_file), H5F_ACC_RDWR),
        chunksize(cs), byrow(br)
    {
        // Setting up the input structures.
        H5::DataSpace ihspace=ihdata.getSpace(); 
        if (ihspace.getSimpleExtentNdims()!=2){ 
            throw std::runtime_error("rechunking is not supported for arrays");
        }
        ihspace.getSimpleExtentDims(dims);
       
        H5::DSetCreatPropList cparms = ihdata.getCreatePlist();
        if (cparms.getLayout()==H5D_CONTIGUOUS) {
            // Contiguous is treated as column-wise chunks.
            chunk_dims[0]=1;
            chunk_dims[1]=nrows();
        } else {
            cparms.getChunk(2, chunk_dims);
        }
        
        // Specifying the output chunk size.
        H5::DSetCreatPropList oparms; 
        if (byrow) { 
            if (chunksize > ncols()) { chunksize=ncols(); }
            out_chunk_ncols()=chunksize;
            out_chunk_nrows()=1;
        } else {
            if (chunksize > nrows()) { chunksize=nrows(); }
            out_chunk_ncols()=1;
            out_chunk_nrows()=chunksize;
        }
        oparms.setLayout(H5D_CHUNKED);
        oparms.setChunk(2, out_chunk_dims);
        oparms.setDeflate(compress);

        /* Holding one chunk in memory (the incompletely read and written one between iterations).
         * No need for fancy nslots calculations, and we evict fully read chunks first (not that
         * it really matters, because there's only one chunk being held in memory).
         */
        H5::FileAccPropList inputlist(ihfile.getAccessPlist().getId());
        const size_t cache_size=chunk_ncols()*chunk_nrows()*HDT.getSize();
        inputlist.setCache(0, 1, cache_size, 1); 

        ihdata.close();
        ihfile.close();
        ihfile.openFile(H5std_string(input_file), H5F_ACC_RDONLY, inputlist);
        ihdata=ihfile.openDataSet(H5std_string(input_data));

        // Creating the output data set.
        H5::DataSpace ohspace(2, dims);
        ohdata=ohfile.createDataSet(output_data, HDT, ohspace, oparms); 

        // Setting up the data space and the storage space.
        mat_space.setExtentSimple(2, dims);

        hsize_t store_dims[2];
        if (byrow) {
            store_dims[0]=chunksize; // store_dims, NOT store_counts!
            store_dims[1]=chunk_nrows();
        } else {
            store_dims[0]=chunk_ncols();
            store_dims[1]=chunksize;
        }
        store_space.setExtentSimple(2, store_dims);
        store_rowpos()=0;
        store_colpos()=0;

        size_t store_size=store_dims[0]*store_dims[1]; // store_dims, NOT store_counts!
        if (use_size) { store_size *= HDT.getSize(); }
        storage.resize(store_size);
        return;
    }

    void execute() {
        if (byrow) {
            fill_by_row();
        } else {
            fill_by_col();
        }
        return;
    }

    Rcpp::IntegerVector get_chunk_dims() {
        return Rcpp::IntegerVector::create(out_chunk_nrows(), out_chunk_ncols());
    }
private:
    H5::H5File ihfile;
    H5::DataSet ihdata;
    H5::DataSpace ihspace;
    const H5::DataType HDT;

    hsize_t dims[2];    
    hsize_t chunk_dims[2];
    
    H5::DataSpace mat_space;
    hsize_t mat_offset[2];
    hsize_t mat_count[2];

    H5::H5File ohfile;
    H5::DataSet ohdata;
    hsize_t out_chunk_dims[2];
    size_t chunksize;
    
    H5::DataSpace store_space;
    hsize_t store_offset[2];
    hsize_t store_count[2];
    std::vector<T> storage;

    bool byrow;

    // Convenience getters.
    const hsize_t& chunk_ncols () { return chunk_dims[0]; }
    const hsize_t& chunk_nrows () { return chunk_dims[1]; }

    const hsize_t& ncols () const { return dims[0]; }
    const hsize_t& nrows () const { return dims[1]; }

    hsize_t& out_chunk_ncols () { return out_chunk_dims[0]; }
    hsize_t& out_chunk_nrows () { return out_chunk_dims[1]; }

    hsize_t& query_colpos () { return mat_offset[0]; }
    hsize_t& query_rowpos () { return mat_offset[1]; }
    hsize_t& query_ncols () { return mat_count[0]; }
    hsize_t& query_nrows () { return mat_count[1]; }

    hsize_t& store_colpos () { return store_offset[0]; }
    hsize_t& store_rowpos () { return store_offset[1]; }
    hsize_t& store_ncols () { return store_count[0]; }
    hsize_t& store_nrows () { return store_count[1]; }

    /* Filling for row-based chunks. The idea is to read/write blocks of X*Y, where
     * X is the number of rows in the input chunks and Y is the size of the output
     * chunk. This is repeated across the columns of the input matrix, and then
     * the function jumps to the next "X" rows. This approach ensures that half-read
     * input chunks in the cache are also written.
     */
    void fill_by_row() {
        size_t currentrow=0, currentcol=0;
    
        // Outer loop across rows.
        while (currentrow < nrows()) { 
            currentcol=0;
            query_rowpos()=currentrow;
            const size_t nextrow=currentrow+chunk_nrows();
            if (nextrow > nrows()) { 
                query_nrows()=nrows() - currentrow;
            } else {
                query_nrows()=chunk_nrows();
            }
            store_nrows()=query_nrows();

            // Middle loop across columns. 
            while (currentcol < ncols()) {
                query_colpos()=currentcol; 
                const size_t nextcol=currentcol+chunksize;
                if (nextcol > ncols()) { 
                    query_ncols()=ncols() - currentcol;
                } else {
                    query_ncols()=chunksize;
                }
                store_ncols()=query_ncols();

                // Actually reading and writing.
                store_space.selectHyperslab(H5S_SELECT_SET, store_count, store_offset);
                mat_space.selectHyperslab(H5S_SELECT_SET, mat_count, mat_offset);
                ihdata.read(storage.data(), HDT, store_space, mat_space);
                ohdata.write(storage.data(), HDT, store_space, mat_space);
                
                currentcol=nextcol;
            }
            currentrow=nextrow;
        }
        return;
    }

    /* Filling for column-based chunks. The idea is to read/write blocks of X*Y, where
     * X is the size of the output chunk and Y is the number of columns in the input 
     * chunk. This is repeated across the rows of the input matrix, and then
     * the function jumps to the next "Y" columns. 
     */
    void fill_by_col() {
        size_t currentcol=0, currentrow=0;
    
        // Outer loop across columns.
        while (currentcol < ncols()) { 
            currentrow=0;
            query_colpos()=currentcol;
            const size_t nextcol=currentcol+chunk_ncols();
            if (nextcol > ncols()) { 
                query_ncols()=ncols() - currentcol;
            } else {
                query_ncols()=chunk_ncols();
            }
            store_ncols()=query_ncols();

            // Middle loop across rows.
            while (currentrow < nrows()) {
                query_rowpos()=currentrow; 
                const size_t nextrow=currentrow+chunksize;
                if (nextrow > nrows()) { 
                    query_nrows()=nrows() - currentrow;
                } else {
                    query_nrows()=chunksize;
                }
                store_nrows()=query_nrows();

                // Actually reading and writing.
                store_space.selectHyperslab(H5S_SELECT_SET, store_count, store_offset);
                mat_space.selectHyperslab(H5S_SELECT_SET, mat_count, mat_offset);
                ihdata.read(storage.data(), HDT, store_space, mat_space);
                ohdata.write(storage.data(), HDT, store_space, mat_space);
                
                currentrow=nextrow;
            }
            currentcol=nextcol;
        }
        return;
    }

};

/************************** Secondary templated functions *********************/

template <typename T, bool use_size> 
SEXP rechunk(Rcpp::StringVector ifile, Rcpp::StringVector idata, 
        Rcpp::StringVector ofile, Rcpp::StringVector odata, 
        Rcpp::IntegerVector olevel, Rcpp::IntegerVector nelements, Rcpp::LogicalVector byrow) {

    if (ifile.size()!=1 || idata.size()!=1 || ofile.size()!=1 || odata.size()!=1) {
        throw std::runtime_error("file and dataset names must be strings");
    }
    if (olevel.size()!=1) {
        throw std::runtime_error("compression level should be an integer scalar");
    }
    if (nelements.size()!=1) {
        throw std::runtime_error("chunk size should be an integer scalar");
    }
    if (byrow.size()!=1) {
        throw std::runtime_error("byrow should be a logical scalar");
    }

    rechunker<T, use_size> repacker(Rcpp::as<std::string>(ifile[0]), Rcpp::as<std::string>(idata[0]),
            Rcpp::as<std::string>(ofile[0]), Rcpp::as<std::string>(odata[0]), 
            olevel[0], nelements[0], byrow[0]);
    repacker.execute();
    return repacker.get_chunk_dims();
}

/************************** The actual R-visible functions *********************/

SEXP rechunk_matrix(SEXP inname, SEXP indata, SEXP intype, SEXP outname, SEXP outdata, SEXP outlevel, SEXP longdim, SEXP byrow) {
    BEGIN_RCPP
    // Figuring out the type.
    Rcpp::StringVector type(intype);
    if (type.size()!=1) {
        throw std::runtime_error("type should be a string");
    }
    const std::string choice=Rcpp::as<std::string>(type[0]);

    // Dispatching.
    if (choice=="double") {
        return rechunk<double, false>(inname, indata, outname, outdata, outlevel, longdim, byrow);
    } else if (choice=="integer" || choice=="logical") { 
        return rechunk<int, false>(inname, indata, outname, outdata, outlevel, longdim, byrow);
    } else if (choice=="character") {
        return rechunk<char, true>(inname, indata, outname, outdata, outlevel, longdim, byrow);
    }
    throw std::runtime_error("unsupported data type");
    END_RCPP
}
