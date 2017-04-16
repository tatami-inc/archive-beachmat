#include "matrix.h"

/* Methods for the virtual base matrix. */

any_matrix::any_matrix() : nrow(0), ncol(0) {}

any_matrix::~any_matrix() {}

int any_matrix::get_nrow() const { return nrow; }

int any_matrix::get_ncol() const { return ncol; }

void any_matrix::fill_dims(const Rcpp::RObject& dims) {
    if (dims.sexp_type()!=INTSXP || Rf_length(dims.get__())!=2) { 
        throw std::runtime_error("matrix dimensions should be an integer vector of length 2");
    }
    Rcpp::IntegerVector d(dims);
    nrow=d[0];
    ncol=d[1];
    if (nrow<0 || ncol<0) { throw std::runtime_error("dimensions should be non-negative"); }
    return;
}

/* If any class has pointers as data members, the data that each pointer points to should be 
 * contained within a RObject that is also a member of the class. This ensures that the 
 * data is PROTECTed for the lifetime of the class instance. Otherwise, one could imagine 
 * a situation where the class is instantiated from an RObject; the RObject is destroyed;
 * and garbage collection occurs, such that the pointers in the class instance are invalid.
 */

/* Methods for the virtual simple matrix. */

simple_matrix::simple_matrix(const Rcpp::RObject& incoming) : obj(incoming) {
    if (!obj.hasAttribute("dim")) { 
        throw std::runtime_error("matrix object should have 'dim' attribute"); 
    }
    fill_dims(obj.attr("dim"));
    if (Rf_length(obj.get__())!=nrow*ncol) { 
        throw std::runtime_error("length of matrix is inconsistent with its dimensions"); 
    }
    return;
}

simple_matrix::~simple_matrix() {}

int simple_matrix::get_index(int r, int c) const { return r + c*nrow; }

/* Methods for the virtual *geMatrix. */

std::string check_Matrix_class (const Rcpp::RObject& mat, const std::string& expected) {
    std::string mattype=get_class(mat);
    if (!mat.isS4() || mattype.empty() || mattype.substr(1)!=expected) {
        throw_custom_error("matrix should be a *", expected, " object");
    }
    return mattype;
}

dense_matrix::dense_matrix (const Rcpp::RObject& incoming) {
    std::string ctype=check_Matrix_class(incoming, "geMatrix");
    fill_dims(incoming.attr("Dim"));
    obj_x=get_safe_slot(incoming, "x"); 
    if (Rf_length(obj_x.get__())!=nrow*ncol) { 
        throw_custom_error("length of 'x' in a ", ctype, " object is inconsistent with its dimensions"); 
    }
    return;
}

dense_matrix::~dense_matrix() {}

int dense_matrix::get_index(int r, int c) const { return r + c*nrow; }

/* Methods for the virtual *gCMatrix. */

Csparse_matrix::Csparse_matrix(const Rcpp::RObject& incoming) : iptr(NULL), pptr(NULL), nx(0) {
    std::string ctype=check_Matrix_class(incoming, "gCMatrix");  
    fill_dims(get_safe_slot(incoming, "Dim"));

    obj_i=get_safe_slot(incoming, "i");
    if (obj_i.sexp_type()!=INTSXP) { throw_custom_error("'i' slot in a ", ctype, " object should be integer"); }
    iptr=INTEGER(obj_i.get__());

    obj_p=get_safe_slot(incoming, "p");
    if (obj_p.sexp_type()!=INTSXP) { throw_custom_error("'p' slot in a ", ctype, " object should be integer"); }
    pptr=INTEGER(obj_p.get__());

    obj_x=get_safe_slot(incoming, "x");
    nx=Rf_length(obj_x.get__());
    if (nx!=Rf_length(obj_i.get__())) { throw_custom_error("'x' and 'i' slots in a ", ctype, " object should have the same length"); }
    if (ncol+1!=Rf_length(obj_p.get__())) { throw_custom_error("length of 'p' slot in a ", ctype, " object should be equal to 'ncol+1'"); }
    if (pptr[0]!=0) { throw_custom_error("first element of 'p' in a ", ctype, " object should be 0"); }
    if (pptr[ncol]!=nx) { throw_custom_error("last element of 'p' in a ", ctype, " object should be 'length(x)'"); }

    // Checking all the indices.
    int px, ix;
    for (px=1; px<=ncol; ++px) {
        if (pptr[px] < pptr[px-1]) { throw_custom_error("'p' slot in a ", ctype, " object should be sorted"); }
    }
    for (px=0; px<ncol; ++px) { 
        for (ix=pptr[px]+1; ix<pptr[px+1]; ++ix) {
            if (iptr[ix]<iptr[ix-1]) { 
                throw_custom_error("'i' in each column of a ", ctype, " object should be sorted");
            }
        }
    }
    for (ix=0; ix<nx; ++ix) {
        if (iptr[ix]<0 || iptr[ix]>=nrow) {
            throw_custom_error("'i' slot in a ", ctype, " object should contain elements in [0, nrow)");
        }
    }

    return;
}

Csparse_matrix::~Csparse_matrix() {} 

int Csparse_matrix::get_index(int r, int c) const {
    const int* iend=iptr + pptr[c+1];
    const int* loc=std::lower_bound(iptr + pptr[c], iend, r);
    if (loc!=iend && *loc==r) { 
        return loc - iptr;
    } else {
        return nx;
    }
}

/* Methods for the virtual *spMatrix. */

Psymm_matrix::Psymm_matrix(SEXP incoming) : upper(true) {
    std::string ctype=check_Matrix_class(incoming, "spMatrix");  
    fill_dims(get_safe_slot(incoming, "Dim"));
    if (nrow!=ncol) { throw_custom_error("'nrow' and 'ncol' should be equal for a ", ctype, " object"); }

    obj_x=get_safe_slot(incoming, "x");
    if ((nrow*(nrow+1))/2!=Rf_length(obj_x.get__())) { throw_custom_error("length of 'x' in a ", ctype, " object is inconsistent with its dimensions"); }
    
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
    return;
}

Psymm_matrix::~Psymm_matrix() {}

int Psymm_matrix::get_index(int r, int c) const {
    if (upper) {
        if (c > r) { 
            return (c*(c+1))/2 + r;            
        } else {
            return (r*(r+1))/2 + c;            
        }
    } else {
        if (r > c) { 
            return nrow * c - (c * (c - 1))/2 + r - c;
        } else {
            return nrow * r - (r * (r - 1))/2 + c - r;
        }
    }
}

/* Methods for the virtual HDF5Matrix. */

#ifdef BEACHMAT_USE_HDF5

HDF5_matrix::HDF5_matrix(const Rcpp::RObject& incoming) {
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
    fill_dims(h5_seed.attr("dim"));

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
    if (dims_out[1]!=nrow || dims_out[0]!=ncol) { 
        throw_custom_error("dimensions in HDF5 file do not equal dimensions in the ", ctype, " object");
    }

    offset[0]=0;
    offset[1]=0;

    rows_out[0]=ncol;
    rows_out[1]=1;
    rowspace=H5::DataSpace(2, rows_out);
    rowspace.selectHyperslab(H5S_SELECT_SET, rows_out, offset);

    cols_out[0]=1;
    cols_out[1]=nrow;
    colspace=H5::DataSpace(2, cols_out);
    colspace.selectHyperslab(H5S_SELECT_SET, cols_out, offset);

    one_out[0]=1;
    one_out[1]=1;
    onespace=H5::DataSpace(2, one_out);
    onespace.selectHyperslab(H5S_SELECT_SET, one_out, offset);

    return;
}

HDF5_matrix::~HDF5_matrix() {}

void HDF5_matrix::set_row(int r) {
    offset[0] = 0;
    offset[1] = r;
    hspace.selectHyperslab(H5S_SELECT_SET, rows_out, offset);
    return;
}

void HDF5_matrix::set_col(int c) {
    offset[0] = c;
    offset[1] = 0;
    hspace.selectHyperslab(H5S_SELECT_SET, cols_out, offset);
    return;
}

void HDF5_matrix::set_one(int r, int c) { 
    offset[0]=c;
    offset[1]=r;
    hspace.selectHyperslab( H5S_SELECT_SET, one_out, offset);
    return;
}

#endif
