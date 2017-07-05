/*** Class definition ***/

template<typename T, int RTYPE>
class HDF5_output : public any_matrix {
public:
    HDF5_output(size_t, size_t, 
            size_t=output_param::DEFAULT_CHUNKDIM, 
            size_t=output_param::DEFAULT_CHUNKDIM, 
            int=output_param::DEFAULT_COMPRESS, 
            size_t=output_param::DEFAULT_STRLEN);
    ~HDF5_output();
    
    void insert_row(size_t, const T*, size_t, size_t);
    template<typename X>
    void insert_row(size_t, const X*, const H5::DataType&, size_t, size_t);

    void insert_col(size_t, const T*, size_t, size_t);
    template<typename X>
    void insert_col(size_t, const X*, const H5::DataType&, size_t, size_t);

    void insert_one(size_t, size_t, T*);

    void extract_col(size_t, T*, size_t, size_t);
    template<typename X>
    void extract_col(size_t, X*, const H5::DataType&, size_t, size_t);

    void extract_row(size_t, T*, size_t, size_t);
    template<typename X>
    void extract_row(size_t, X*, const H5::DataType&, size_t, size_t);

    void extract_one(size_t, size_t, T*);

    Rcpp::RObject yield();

    matrix_type get_matrix_type() const;
protected:
    std::string fname, dname;

    H5::H5File hfile;
    H5::DataSet hdata;
    H5::DataSpace hspace, rowspace, colspace, onespace;
    hsize_t h5_start[2], col_count[2], row_count[2], one_count[2], zero_start[1];

    H5::DataType default_type;
    void select_row(size_t, size_t, size_t);
    void select_col(size_t, size_t, size_t);
    void select_one(size_t, size_t);

    T get_empty() const;

    bool onrow, oncol;
    bool rowokay, colokay;
    bool largerrow, largercol;
    H5::FileAccPropList rowlist, collist;
};

/*** Constructor definition ***/

template<typename T, int RTYPE>
HDF5_output<T, RTYPE>::HDF5_output (size_t nr, size_t nc, size_t chunk_nr, size_t chunk_nc, int compress, size_t len) : any_matrix(nr, nc), 
        rowlist(H5::FileAccPropList::DEFAULT.getId()), collist(H5::FileAccPropList::DEFAULT.getId()) {

    // Pulling out settings.
    const Rcpp::Environment env=Rcpp::Environment::namespace_env("beachmat");
    Rcpp::Function fun=env["setupHDF5Array"];
    Rcpp::List collected=fun(Rcpp::IntegerVector::create(this->nrow, this->ncol), Rcpp::StringVector(translate_type(RTYPE)),
                             Rcpp::IntegerVector::create(chunk_nr, chunk_nc), compress);

    if (collected.size()!=4) { 
        throw std::runtime_error("output of setupHDF5Array should be a list of four elements");
    }
    fname=make_to_string(collected[0]);
    dname=make_to_string(collected[1]);
    Rcpp::IntegerVector r_chunks=collected[2];
    if (r_chunks.size()!=2) { 
        throw std::runtime_error("chunk dimensions should be an integer vector of length 2");
    }
    chunk_nr=r_chunks[0];
    chunk_nc=r_chunks[1];
    Rcpp::IntegerVector r_compress=collected[3];
    if (r_compress.size()!=1) {
        throw std::runtime_error("compression should be an integer scalar");
    }
    compress=r_compress[0];

    // Opening the file.
    hfile.openFile(fname, H5F_ACC_RDWR);

    // Type setting.
    switch (RTYPE) {
        case REALSXP:
            default_type=H5::DataType(H5::PredType::NATIVE_DOUBLE);
            break;
        case INTSXP: case LGLSXP:
            default_type=H5::DataType(H5::PredType::NATIVE_INT32);
            break;
        case STRSXP:
            default_type=H5::StrType(0, len);
            break;
        default:
            std::stringstream err;
            err << "unsupported sexptype '" << RTYPE << "' for HDF5 output";
            throw std::runtime_error(err.str().c_str());
    }

    // Creating the data set.
    H5::DSetCreatPropList plist;
    const T empty=get_empty();
    plist.setFillValue(default_type, &empty);
        
    // Setting the chunk dimensions if not contiguous.
    if (compress>0) {
        std::vector<hsize_t> chunk_dims(2);
        chunk_dims[0]=chunk_nc; // flipping them, as rhdf5 internally transposes it.
        chunk_dims[1]=chunk_nr;
        plist.setLayout(H5D_CHUNKED);
        plist.setChunk(2, chunk_dims.data());
        plist.setDeflate(compress);
    } else {
        plist.setLayout(H5D_CONTIGUOUS);
    }

    std::vector<hsize_t> dims(2);
    dims[0]=this->ncol; // Setting the dimensions (0 is column, 1 is row; internally transposed).
    dims[1]=this->nrow; 

    hspace.setExtentSimple(2, dims.data());
    hdata=hfile.createDataSet(dname, default_type, hspace, plist); 

    // Setting other values.
    h5_start[0]=0;
    h5_start[1]=0;
    col_count[0]=1;
    col_count[1]=this->nrow;
    row_count[0]=this->ncol;
    row_count[1]=1;
    
    zero_start[0]=0;
    one_count[0]=1;
    one_count[1]=1;
    onespace=H5::DataSpace(1, one_count);
    onespace.selectAll();

    // Setting logical attributes.
    if (RTYPE==LGLSXP) {
        H5::StrType str_type(0, H5T_VARIABLE);
        H5::DataSpace att_space(1, one_count);
        H5::Attribute att = hdata.createAttribute("storage.mode", str_type, att_space);
        att.write(str_type, std::string("logical"));
    }

    // Setting the chunk cache parameters.
    calc_HDF5_chunk_cache_settings(this->nrow, this->ncol, hdata.getCreatePlist(), default_type, 
            onrow, oncol, rowokay, colokay, largerrow, largercol, rowlist, collist);
    return;
}

template<typename T, int RTYPE>
HDF5_output<T, RTYPE>::~HDF5_output() {}

/*** Setter methods ***/

template<typename T, int RTYPE>
void HDF5_output<T, RTYPE>::select_col(size_t c, size_t start, size_t end) {
    check_colargs(c, start, end);
    if (oncol || (onrow && largerrow)) {
        ; // Don't do anything, it's okay.
    } else if (!colokay) {
        std::stringstream err;
        err << "cache size limit (" << get_cache_size_hard_limit() << ") exceeded for column access, repack the file";
        throw std::runtime_error(err.str().c_str());
    } else {
        hdata.close();
        hfile.close();
        hfile.openFile(fname.c_str(), H5F_ACC_RDWR, collist);
        hdata = hfile.openDataSet(dname.c_str());
        oncol=true;
    }

    col_count[1]=end-start;
    colspace.setExtentSimple(1, col_count+1);
    colspace.selectAll();
    h5_start[0]=c;
    h5_start[1]=start;
    hspace.selectHyperslab(H5S_SELECT_SET, col_count, h5_start);
    return;
}

template<typename T, int RTYPE>
template<typename X>
void HDF5_output<T, RTYPE>::insert_col(size_t c, const X* in, const H5::DataType& HDT, size_t start, size_t end) {
    select_col(c, start, end);
    hdata.write(in, HDT, colspace, hspace);
    return;
}

template<typename T, int RTYPE>
void HDF5_output<T, RTYPE>::insert_col(size_t c, const T* in, size_t start, size_t end) {
    insert_col(c, in, default_type, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_output<T, RTYPE>::select_row(size_t r, size_t start, size_t end) {
    check_rowargs(r, start, end);
    if (onrow || (oncol && largercol)) {
        ; // Don't do anything, it's okay.
    } else if (!rowokay) {
        std::stringstream err;
        err << "cache size limit (" << get_cache_size_hard_limit() << ") exceeded for row access, repack the file";
        throw std::runtime_error(err.str().c_str());
    } else {
        hdata.close();
        hfile.close();
        hfile.openFile(fname.c_str(), H5F_ACC_RDWR, rowlist);
        hdata = hfile.openDataSet(dname.c_str());
        onrow=true;
    }

    row_count[0] = end-start;
    rowspace.setExtentSimple(1, row_count);
    rowspace.selectAll();
    h5_start[0] = start;
    h5_start[1] = r;
    hspace.selectHyperslab(H5S_SELECT_SET, row_count, h5_start);
    return;
}

template<typename T, int RTYPE>
template<typename X>
void HDF5_output<T, RTYPE>::insert_row(size_t c, const X* in, const H5::DataType& HDT, size_t start, size_t end) {
    select_row(c, start, end);
    hdata.write(in, HDT, rowspace, hspace);
    return;
}

template<typename T, int RTYPE>
void HDF5_output<T, RTYPE>::insert_row(size_t c, const T* in, size_t start, size_t end) {
    insert_row(c, in, default_type, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_output<T, RTYPE>::select_one(size_t r, size_t c) {
    check_oneargs(r, c);
    h5_start[0]=c;
    h5_start[1]=r;
    hspace.selectHyperslab(H5S_SELECT_SET, one_count, h5_start);
    return;
}

template<typename T, int RTYPE>
void HDF5_output<T, RTYPE>::insert_one(size_t r, size_t c, T* in) {
    select_one(r, c);
    hdata.write(in, default_type, onespace, hspace);
    return;
}

/*** Getter methods ***/

template<typename T, int RTYPE>
template<typename X>
void HDF5_output<T, RTYPE>::extract_row(size_t r, X* out, const H5::DataType& HDT, size_t start, size_t end) { 
    select_row(r, start, end);
    hdata.read(out, HDT, rowspace, hspace);
    return;
} 

template<typename T, int RTYPE>
void HDF5_output<T, RTYPE>::extract_row(size_t r, T* out, size_t start, size_t end) { 
    extract_row(r, out, default_type, start, end);
    return;
} 

template<typename T, int RTYPE>
template<typename X>
void HDF5_output<T, RTYPE>::extract_col(size_t c, X* out, const H5::DataType& HDT, size_t start, size_t end) { 
    select_col(c, start, end);
    hdata.read(out, HDT, colspace, hspace);
    return;
}

template<typename T, int RTYPE>
void HDF5_output<T, RTYPE>::extract_col(size_t c, T* out, size_t start, size_t end) { 
    extract_col(c, out, default_type, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_output<T, RTYPE>::extract_one(size_t r, size_t c, T* out) { 
    select_one(r, c);
    hdata.read(out, default_type, onespace, hspace);
    return;
}

// get_empty() defined for each realized class separately.

/*** Output function ***/

template<typename T, int RTYPE>
Rcpp::RObject HDF5_output<T, RTYPE>::yield() {
    std::string seedclass="HDF5ArraySeed";
    Rcpp::S4 h5seed(seedclass);

    // Assigning to slots.
    if (!h5seed.hasSlot("file")) {
        throw_custom_error("missing 'file' slot in ", seedclass, " object");
    }
    h5seed.slot("file") = fname;
    if (!h5seed.hasSlot("name")) {
        throw_custom_error("missing 'name' slot in ", seedclass, " object");
    }
    h5seed.slot("name") = dname;
    if (!h5seed.hasSlot("dim")) {
        throw_custom_error("missing 'dim' slot in ", seedclass, " object");
    }
    h5seed.slot("dim") = Rcpp::IntegerVector::create(this->nrow, this->ncol);
    if (!h5seed.hasSlot("first_val")) {
        throw_custom_error("missing 'first_val' slot in ", seedclass, " object");
    }
    if (this->nrow && this->ncol) { 
        T first;
        extract_one(0, 0, &first);
        h5seed.slot("first_val") = first;
    } else {
        h5seed.slot("first_val") = Rcpp::Vector<RTYPE>(0); // empty vector.
    }

    // Assigning the seed to the HDF5Matrix.
    std::string matclass="HDF5Matrix";
    Rcpp::S4 h5mat(matclass);
    if (!h5mat.hasSlot("seed")) {
        throw_custom_error("missing 'seed' slot in ", matclass, " object");
    }
    h5mat.slot("seed") = h5seed;

    return SEXP(h5mat);
}

template<typename T, int RTYPE>
matrix_type HDF5_output<T, RTYPE>::get_matrix_type() const {
    return HDF5;
}

