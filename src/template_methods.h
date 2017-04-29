#ifndef TEMPLATE_METHODS_H 
#define TEMPLATE_METHODS_H 

/* Methods for the virtual base matrix. */

template<typename T>
any_matrix<T>::any_matrix() : nrow(0), ncol(0) {}

template<typename T>
any_matrix<T>::~any_matrix() {}

template<typename T>
int any_matrix<T>::get_nrow() const { return nrow; }

template<typename T>
int any_matrix<T>::get_ncol() const { return ncol; }

template<typename T>
void any_matrix<T>::fill_dims(const Rcpp::RObject& dims) {
    if (dims.sexp_type()!=INTSXP || Rf_length(dims.get__())!=2) { 
        throw std::runtime_error("matrix dimensions should be an integer vector of length 2");
    }
    Rcpp::IntegerVector d(dims);
    nrow=d[0];
    ncol=d[1];
    if (nrow<0 || ncol<0) { throw std::runtime_error("dimensions should be non-negative"); }
    return;
}

/* Methods for a simple matrix. */

template<typename T>
simple_matrix<T>::simple_matrix(const Rcpp::RObject& incoming) : obj(incoming), simple_ptr(NULL) { 
    if (!obj.hasAttribute("dim")) { 
        throw std::runtime_error("matrix object should have 'dim' attribute"); 
    }

    this->fill_dims(obj.attr("dim"));
    const int& NC=this->ncol; 

    if (Rf_length(obj.get__())!=(this->nrow)*NC) {
        throw std::runtime_error("length of matrix is inconsistent with its dimensions"); 
    }

    row_data.resize(NC);
    row_ptr=row_data.data();
    return;
}

template<typename T>
simple_matrix<T>::~simple_matrix () {}

template<typename T>
int simple_matrix<T>::get_index(int r, int c) const { return r + c*(this->nrow); }

template<typename T>
void simple_matrix<T>::fill_row(int r, T* out) {
    for (int col=0; col<(this->ncol); ++col) { out[col]=simple_ptr[get_index(r, col)]; }
    return;
}

template<typename T>
const T* simple_matrix<T>::get_row(int r) {
    fill_row(r, row_ptr);
    return row_ptr;
}

template<typename T>
void simple_matrix<T>::fill_col(int c, T* out) {
    const T* src=get_col(c);
    std::copy(src, src+(this->nrow), out);
    return;
}

template<typename T>
const T* simple_matrix<T>::get_col(int c) {
    return simple_ptr + c*(this->nrow);
}

template<typename T>
T simple_matrix<T>::get(int r, int c) {
    return simple_ptr[get_index(r, c)];
}

/* Methods for a dense Matrix. */

template <typename T>
dense_matrix<T>::dense_matrix(const Rcpp::RObject& incoming) : dense_ptr(NULL){ 
    std::string ctype=check_Matrix_class(incoming, "geMatrix");
    this->fill_dims(incoming.attr("Dim"));
    const int& NC=this->ncol;
    
    obj_x=get_safe_slot(incoming, "x"); 
    if (Rf_length(obj_x.get__())!=(this->nrow)*NC) {
        throw_custom_error("length of 'x' in a ", ctype, " object is inconsistent with its dimensions"); 
    }

    row_data.resize(NC);
    row_ptr=row_data.data();
    return;
}

template <typename T>
dense_matrix<T>::~dense_matrix() {}

template <typename T>
int dense_matrix<T>::get_index(int r, int c) const { return r + c*(this->nrow); }

template <typename T>
void dense_matrix<T>::fill_row(int r, T* out) {
    for (int col=0; col<(this->ncol); ++col) { out[col]=dense_ptr[get_index(r, col)]; }
    return;
}

template <typename T>
const T* dense_matrix<T>::get_row(int r) {
    fill_row(r, row_ptr);
    return row_ptr;
}

template <typename T>
void dense_matrix<T>::fill_col(int c, T* out) {
    const T* src=get_col(c);
    std::copy(src, src+(this->nrow), out);
    return;
}

template <typename T>
const T* dense_matrix<T>::get_col(int c) {
    return dense_ptr + c*(this->nrow);
}

template <typename T>
T dense_matrix<T>::get(int r, int c) {
    return dense_ptr[get_index(r, c)];
}

/* Methods for a Csparse matrix. */

template <typename T, const T& Z>
Csparse_matrix<T, Z>::Csparse_matrix(const Rcpp::RObject& incoming) : iptr(NULL), pptr(NULL), nx(0), xptr(NULL) {
    std::string ctype=check_Matrix_class(incoming, "gCMatrix");  
    this->fill_dims(get_safe_slot(incoming, "Dim"));
    const int& NC=this->ncol;
    const int& NR=this->nrow;

    obj_i=get_safe_slot(incoming, "i");
    if (obj_i.sexp_type()!=INTSXP) { throw_custom_error("'i' slot in a ", ctype, " object should be integer"); }
    iptr=INTEGER(obj_i.get__());

    obj_p=get_safe_slot(incoming, "p");
    if (obj_p.sexp_type()!=INTSXP) { throw_custom_error("'p' slot in a ", ctype, " object should be integer"); }
    pptr=INTEGER(obj_p.get__());

    obj_x=get_safe_slot(incoming, "x");
    nx=Rf_length(obj_x.get__());
    if (nx!=Rf_length(obj_i.get__())) { throw_custom_error("'x' and 'i' slots in a ", ctype, " object should have the same length"); }
    if (NC+1!=Rf_length(obj_p.get__())) { throw_custom_error("length of 'p' slot in a ", ctype, " object should be equal to 'ncol+1'"); }
    if (pptr[0]!=0) { throw_custom_error("first element of 'p' in a ", ctype, " object should be 0"); }
    if (pptr[NC]!=nx) { throw_custom_error("last element of 'p' in a ", ctype, " object should be 'length(x)'"); }

    // Checking all the indices.
    int px, ix;
    for (px=1; px<=(NC); ++px) {
        if (pptr[px] < pptr[px-1]) { throw_custom_error("'p' slot in a ", ctype, " object should be sorted"); }
    }
    for (px=0; px<(NC); ++px) { 
        for (ix=pptr[px]+1; ix<pptr[px+1]; ++ix) {
            if (iptr[ix]<iptr[ix-1]) { 
                throw_custom_error("'i' in each column of a ", ctype, " object should be sorted");
            }
        }
    }
    for (ix=0; ix<nx; ++ix) {
        if (iptr[ix]<0 || iptr[ix]>=NR) {
            throw_custom_error("'i' slot in a ", ctype, " object should contain elements in [0, nrow)");
        }
    }
   
    // Resizing the output vectors.
    row_data.resize(NC);
    col_data.resize(NR);
    row_ptr=row_data.data();
    col_ptr=col_data.data();
    return;
}

template <typename T, const T& Z>
Csparse_matrix<T, Z>::~Csparse_matrix () {}

template <typename T, const T& Z>
int Csparse_matrix<T, Z>::get_index(int r, int c) const {
    const int* iend=iptr + pptr[c+1];
    const int* loc=std::lower_bound(iptr + pptr[c], iend, r);
    if (loc!=iend && *loc==r) { 
        return loc - iptr;
    } else {
        return nx;
    }
}

template <typename T, const T& Z>
void Csparse_matrix<T, Z>::fill_row(int r, T* out) {
    const int& NC=this->ncol;
    std::fill(out, out+NC, Z);
    for (int col=0; col<NC; ++col) {  
        if (pptr[col]!=pptr[col+1]) { out[col]=get(r, col); } 
    } 
    return;  
}

template <typename T, const T& Z>
const T* Csparse_matrix<T, Z>::get_row(int r) {
    fill_row(r, row_ptr);
    return row_ptr;  
}

template <typename T, const T& Z>
void Csparse_matrix<T, Z>::fill_col(int c, T* out) {
    const int& start=pptr[c]; 
    const int& end=pptr[c+1]; 
    std::fill(out, out+(this->nrow), Z); 
    for (int ix=start; ix<end; ++ix) {
        out[iptr[ix]]=xptr[ix];
    }
    return;
}

template <typename T, const T& Z>
const T* Csparse_matrix<T, Z>::get_col(int c) {
    fill_col(c, col_ptr);
    return col_ptr;
}

template <typename T, const T& Z>
T Csparse_matrix<T, Z>::get(int r, int c) {
    const int index=get_index(r, c);
    return (index!=nx ? xptr[index] : Z);
}

/* Methods for a Psymm matrix. */

template <typename T>
Psymm_matrix<T>::Psymm_matrix(const Rcpp::RObject& incoming) : upper(true), xptr(NULL) { 
    std::string ctype=check_Matrix_class(incoming, "spMatrix");  
    this->fill_dims(get_safe_slot(incoming, "Dim"));
    const int& NR=this->nrow;
    if (NR!=(this->ncol)) { throw_custom_error("'nrow' and 'ncol' should be equal for a ", ctype, " object"); }

    obj_x=get_safe_slot(incoming, "x");
    if ((NR*(NR+1))/2!=Rf_length(obj_x.get__())) { throw_custom_error("length of 'x' in a ", ctype, " object is inconsistent with its dimensions"); }
    
    Rcpp::RObject ul=get_safe_slot(incoming, "uplo");
    std::string uplo;
    try {
        uplo=make_to_string(ul);
        if (uplo.length()!=1) { throw; }
    } catch (...) { 
        throw_custom_error("'uplo' slot in a ", ctype, " object should be a string of length 1"); 
    }

    switch (uplo[0]) { 
        case 'U':
            upper=true;
            break;
        case 'L':
            upper=false;
            break;
        default:
            throw_custom_error("'uplo' slot in a ", ctype, " object should be either 'U' or 'L'");
    }

    // Resizing elements.
    out_data.resize(NR);
    out_ptr=out_data.data();
    return;
}

template <typename T>
Psymm_matrix<T>::~Psymm_matrix () {}

template <typename T>
int Psymm_matrix<T>::get_index(int r, int c) const {
    if (upper) {
        if (c > r) { 
            return (c*(c+1))/2 + r;            
        } else {
            return (r*(r+1))/2 + c;            
        }
    } else {
        if (r > c) { 
            return (this->nrow) * c - (c * (c - 1))/2 + r - c;
        } else {
            return (this->nrow) * r - (r * (r - 1))/2 + c - r;
        }
    }
}

template <typename T>
void Psymm_matrix<T>::fill_col (int c, T* out) {
    if (upper) {
        int start=(c*(c+1))/2;
        std::copy(xptr+start, xptr+start+c, out);
        for (int i=c; i<(this->ncol); ++i) {
            out[i]=xptr[start+c];
            start+=i+1;
        }
    } else {
        const int& NR=this->nrow;
        int start=0;
        for (int i=0; i<c; ++i) {
            out[i]=xptr[c-i + start];
            start+=NR-i;
        }
        std::copy(xptr+start, xptr+start+NR-c, out+c);
    }
    return;
}

template <typename T>
const T* Psymm_matrix<T>::get_col (int c) {
    fill_col(c, out_ptr);
    return out_ptr;
}

template <typename T>
void Psymm_matrix<T>::fill_row (int r, T* out) {
    fill_col(r, out);
    return;
}

template <typename T>
const T* Psymm_matrix<T>::get_row (int r) {
    return get_col(r);
}

template <typename T>
T Psymm_matrix<T>::get(int r, int c) {
    return xptr[get_index(r, c)];
}

/* Methods for a HDF5 matrix. */

#ifdef BEACHMAT_USE_HDF5

template<typename T, const H5::PredType& H>
HDF5_matrix<T, H>::HDF5_matrix(const Rcpp::RObject& incoming) {
    std::string ctype=get_class(incoming);
    if (!incoming.isS4() || ctype!="HDF5Matrix") {
        throw std::runtime_error("matrix should be a HDF5Matrix object");
    }

    const Rcpp::RObject& h5_seed=get_safe_slot(incoming, "seed");
    std::string stype=get_class(h5_seed);
    if (!h5_seed.isS4() || stype!="HDF5ArraySeed") {
        throw_custom_error("'seed' slot in a ", ctype, " object should be a HDF5ArraySeed object");
    }

    if (!h5_seed.hasAttribute("dim")) { 
        throw_custom_error("", stype, " object should have 'dim' attribute"); 
    }
    this->fill_dims(h5_seed.attr("dim"));
    const int& NC=this->ncol;
    const int& NR=this->nrow;

    std::string fname;
    try {
        fname=make_to_string(get_safe_slot(h5_seed, "file"));
    } catch (...) { 
        throw_custom_error("'file' slot in a ", stype, " object should be a string");
    }

    std::string dataset;
    try {
        dataset=make_to_string(get_safe_slot(h5_seed, "name"));
    } catch (...) { 
        throw_custom_error("'name' slot in a ", stype, " object should be a string");
    }
    
    // Setting up the HDF5 accessors.
    hfile.openFile(H5std_string(fname), H5F_ACC_RDONLY);
    hdata = hfile.openDataSet(H5std_string(dataset));
    hspace = hdata.getSpace();
    if (hspace.getSimpleExtentNdims()!=2) {
        throw std::runtime_error("data in HDF5 file is not a two-dimensional array");
    }
    hsize_t dims_out[2];
    hspace.getSimpleExtentDims(dims_out, NULL);
    if (dims_out[1]!=NR || dims_out[0]!=NC) { 
        throw_custom_error("dimensions in HDF5 file do not equal dimensions in the ", ctype, " object");
    }

    offset[0]=0;
    offset[1]=0;

    rows_out[0]=NC;
    rows_out[1]=1;
    rowspace=H5::DataSpace(2, rows_out);
    rowspace.selectHyperslab(H5S_SELECT_SET, rows_out, offset);

    cols_out[0]=1;
    cols_out[1]=NR;
    colspace=H5::DataSpace(2, cols_out);
    colspace.selectHyperslab(H5S_SELECT_SET, cols_out, offset);

    one_out[0]=1;
    one_out[1]=1;
    onespace=H5::DataSpace(2, one_out);
    onespace.selectHyperslab(H5S_SELECT_SET, one_out, offset);

    // Resizing the output vectors.
    row_data.resize(NC);
    col_data.resize(NR);
    row_ptr=row_data.data();
    col_ptr=col_data.data();
    return;
}

template<typename T, const H5::PredType& H>
HDF5_matrix<T, H>::~HDF5_matrix() {}

template<typename T, const H5::PredType& H>
void HDF5_matrix<T, H>::fill_row(int r, T* out) { 
    offset[0] = 0;
    offset[1] = r;
    hspace.selectHyperslab(H5S_SELECT_SET, rows_out, offset);
    hdata.read(out, H, rowspace, hspace);
    return;
} 

template<typename T, const H5::PredType& H>
const T * HDF5_matrix<T, H>::get_row(int r) { 
    fill_row(r, row_ptr);
    return row_ptr;
}

template<typename T, const H5::PredType& H>
void HDF5_matrix<T, H>::fill_col(int c, T* out) { 
    offset[0] = c;
    offset[1] = 0;
    hspace.selectHyperslab(H5S_SELECT_SET, cols_out, offset);
    hdata.read(out, H, colspace, hspace);
    return;
}
 
template<typename T, const H5::PredType& H>
const T * HDF5_matrix<T, H>::get_col(int c) { 
    fill_col(c, col_ptr);
    return col_ptr;
}

template<typename T, const H5::PredType& H>
T HDF5_matrix<T, H>::get(int r, int c) { 
    offset[0]=c;
    offset[1]=r;
    hspace.selectHyperslab( H5S_SELECT_SET, one_out, offset);
    T out;
    hdata.read(&out, H, onespace, hspace);
    return out;
}

#endif

#endif
