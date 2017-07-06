#include "HDF5_utils.h"

namespace beachmat {

/*******************************************
 ******** HDF5 dual-use functions **********
 *******************************************/

/* HDF5 utilities. */

size_t get_cache_size_hard_limit () {
    return 2000000000;
}

/* This function computes the chunk cache settings for a HDF5 file
 * of a given dimension. It takes a bunch of HDF5_matrix/output 
 * members and modifies them by reference.
 */

void calc_HDF5_chunk_cache_settings (const size_t total_nrows, const size_t total_ncols, 
        const H5::DSetCreatPropList& cparms, const H5::DataType& default_type,
        bool& onrow, bool& oncol, bool& rowokay, bool& colokay, bool& largerrow, bool& largercol,
        H5::FileAccPropList& rowlist, H5::FileAccPropList& collist) {

    if (cparms.getLayout()!=H5D_CHUNKED) {
        // If contiguous, setting the flags to avoid reopening the file.
        onrow=true;
        oncol=true;
        rowokay=false;
        colokay=false;
        largerrow=false;
        largercol=false;
        return;
    }
    
    /* Setting up the chunk cache specification. */
    hsize_t chunk_dims[2];
    cparms.getChunk(2, chunk_dims);
    const size_t chunk_nrows=chunk_dims[1];
    const size_t chunk_ncols=chunk_dims[0];
    const size_t num_chunks_per_row=std::ceil(double(total_ncols)/chunk_ncols); // per row needs to divide by column dimensions.
    const size_t num_chunks_per_col=std::ceil(double(total_nrows)/chunk_nrows); 

    /* Everything is transposed, so hash indices are filled column-major. 
     * Here, we computing the lowest multiple of # row-chunks that is greater than # col-chunks, plus 1.
     * This ensures that two chunks in the same row/column do not have the same hash index.
     */
    const size_t nslots = std::ceil(double(num_chunks_per_row)/num_chunks_per_col) * num_chunks_per_col + 1; 

    /* Computing the size of the cache required to store all chunks in each row or column.
     * The approach used below avoids overflow from computing eachchunk*num_Xchunks.
     */
    const size_t eachchunk=default_type.getSize() * chunk_nrows * chunk_ncols;
    const size_t nchunks_in_cache=get_cache_size_hard_limit()/eachchunk;
    rowokay=nchunks_in_cache >= num_chunks_per_row; 
    colokay=nchunks_in_cache >= num_chunks_per_col; 

    const size_t eachrow=eachchunk * num_chunks_per_row; 
    const size_t eachcol=eachchunk * num_chunks_per_col;
    largercol=eachcol >= eachrow;
    largerrow=eachrow >= eachcol;

    /* The first argument is ignored, according to https://support.hdfgroup.org/HDF5/doc/RM/RM_H5P.html.
     * Setting w0 to 0 to evict the last used chunk; no need to worry about full vs partial reads here.
     */
    rowlist.setCache(10000, nslots, eachrow, 0);
    collist.setCache(10000, nslots, eachcol, 0);

    // File is not opened on either row or column yet.
    onrow=false; 
    oncol=false;
    return;
}

/* This function reopens the file with the chunk cache optimized for the
 * specified dimension. It needs a lot of internal components from the 
 * HDF5_matrix and HDF5_output objects to do so, including flags for the
 * other dimension (e.g., 'dim=>col' and 'other=>row' for column access).
 */

void reopen_HDF5_file_by_dim(const std::string& filename, const std::string& dataname, 
        H5::H5File& hfile, H5::DataSet& hdata, const unsigned& openmode, const H5::FileAccPropList& dimlist,
        bool& ondim, const bool& onother, const bool& largerother, const bool& dimokay) {
    if (ondim || (onother && largerother)) {
        ; // Don't do anything, it's okay.
    } else if (!dimokay) {
        std::stringstream err;
        err << "cache size limit (" << get_cache_size_hard_limit() << ") exceeded for dim access, repack the file";
        throw std::runtime_error(err.str().c_str());
    } else {
        hdata.close();
        hfile.close();
        hfile.openFile(filename.c_str(), openmode, dimlist);
        hdata = hfile.openDataSet(dataname.c_str());
        ondim=true;
    }
}

/* These functions set the rowspace and dataspace elements according to
 * the requested data access profile. We have column, row and single access.
 */

void HDF5_select_row(const size_t& r, const size_t& start, const size_t& end,
        hsize_t* row_count, hsize_t* h5_start, 
        H5::DataSpace& rowspace, H5::DataSpace& hspace) {
    row_count[0] = end-start;
    rowspace.setExtentSimple(1, row_count);
    rowspace.selectAll();
    h5_start[0] = start;
    h5_start[1] = r;
    hspace.selectHyperslab(H5S_SELECT_SET, row_count, h5_start);
    return;
}

void HDF5_select_col(const size_t& c, const size_t& start, const size_t& end,
        hsize_t* col_count, hsize_t* h5_start, 
        H5::DataSpace& colspace, H5::DataSpace& hspace) {
    col_count[1] = end-start;
    colspace.setExtentSimple(1, col_count+1);
    colspace.selectAll();
    h5_start[0] = c;
    h5_start[1] = start;
    hspace.selectHyperslab(H5S_SELECT_SET, col_count, h5_start);
    return;
}

void HDF5_select_one(const size_t& r, const size_t& c,
        hsize_t* one_count, hsize_t* h5_start, 
        H5::DataSpace& hspace) {
    h5_start[0]=c;
    h5_start[1]=r;  
    hspace.selectHyperslab(H5S_SELECT_SET, one_count, h5_start);
    return;
}

/* This function initializes the various hsize_t* arrays
 * that are present in the HDF5_matrix/output objects.
 */

void initialize_HDF5_size_arrays (const size_t& NR, const size_t& NC,
        hsize_t* h5_start, hsize_t* col_count, hsize_t* row_count, 
        hsize_t* one_count, H5::DataSpace& onespace) {
    h5_start[0]=0;
    h5_start[1]=0;

    col_count[0]=1;
    col_count[1]=NR;
    row_count[0]=NC;
    row_count[1]=1;

    one_count[0]=1;
    one_count[1]=1;
    onespace.setExtentSimple(1, one_count);
    onespace.selectAll();
    return;
}

/* These overloaded functions return an output DataType for a 
 * given RTYPE. The second one also checks that the dataset is
 * of an appropriate class. Technically, I could put it in the 
 * input header, but it lives here with its overloaded sibling.
 */

H5::DataType set_HDF5_data_type (int RTYPE, size_t strlen) { 
    switch (RTYPE) {
        case REALSXP:
            return H5::DataType(H5::PredType::NATIVE_DOUBLE);
        case INTSXP: 
            return H5::DataType(H5::PredType::NATIVE_INT32);
        case LGLSXP:
            return H5::DataType(H5::PredType::NATIVE_INT32);
        case STRSXP:
            return H5::StrType(0, strlen);
    }
    std::stringstream err;
    err << "unsupported sexptype '" << RTYPE << "'";
    throw std::runtime_error(err.str().c_str());
}

H5::DataType set_HDF5_data_type (int RTYPE, const H5::DataSet& hdata) {
    auto curtype=hdata.getTypeClass();
    switch (RTYPE) {
        case REALSXP:
            if (curtype!=H5T_FLOAT) { 
                throw std::runtime_error("data type in HDF5 file is not double");
            }
            break;
        case INTSXP: 
            if (curtype!=H5T_INTEGER) { 
                throw std::runtime_error("data type in HDF5 file is not integer");
            }
            break;
        case LGLSXP:
            if (curtype!=H5T_INTEGER) { 
                throw std::runtime_error("data type in HDF5 file is not logical");
            }
            break;
        case STRSXP:
            if (curtype!=H5T_STRING) { 
                throw std::runtime_error("data type in HDF5 file is not character");
            }
            return H5::StrType(hdata);
    }
    return set_HDF5_data_type(RTYPE, 0);
}

}
