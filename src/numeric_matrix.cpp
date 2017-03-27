#include "numeric_matrix.h"
#include "utils.h"

/* Methods for the base class. */

numeric_matrix::numeric_matrix() {}

numeric_matrix::~numeric_matrix() {}

/* Methods for a simple matrix. */

simple_numeric_matrix::simple_numeric_matrix(SEXP incoming) : simple_matrix(incoming), simple_ptr(NULL), row_ptr(NULL) {
    if (!isReal(incoming)) { 
        throw std::runtime_error("matrix should be double-precision");
    }
    simple_ptr=REAL(incoming);

    try {
        row_ptr=new double[ncol];
    } catch (std::exception& e) {
        delete [] row_ptr;
        throw; 
    }
    return;
}

simple_numeric_matrix::~simple_numeric_matrix () {
    delete [] row_ptr;
    return;
}

const double* simple_numeric_matrix::get_row(int r) {
    for (int col=0; col<ncol; ++col) { row_ptr[col]=get(r, col); }
    return row_ptr; 
}

const double* simple_numeric_matrix::get_col(int c) {
    return simple_ptr + c*nrow;
}

double simple_numeric_matrix::get(int r, int c) {
    return simple_ptr[get_index(r, c)];
}

/* Methods for a dense Matrix. */

dense_numeric_matrix::dense_numeric_matrix(SEXP incoming) : dense_matrix(incoming), dense_ptr(NULL), row_ptr(NULL) { 
    SEXP x=R_do_slot(incoming, install("x"));
    if (!isReal(x)) { throw std::runtime_error("'x' slot in a numeric dgeMatrix should be double-precision"); }
    dense_ptr=REAL(x);

    try {
        row_ptr=new double[ncol];
    } catch (std::exception& e) {
        delete [] row_ptr;
        throw; 
    }
    return;
}

dense_numeric_matrix::~dense_numeric_matrix() {
    delete [] row_ptr;
}

const double* dense_numeric_matrix::get_row(int r) {
    for (int col=0; col<ncol; ++col) { row_ptr[col]=get(r, col); }
    return row_ptr; 
}

const double* dense_numeric_matrix::get_col(int c) {
    return dense_ptr + c*nrow;
}

double dense_numeric_matrix::get(int r, int c) {
    return dense_ptr[get_index(r, c)];
}

/* Methods for a Csparse matrix. */

Csparse_numeric_matrix::Csparse_numeric_matrix(SEXP incoming) : Csparse_matrix(incoming), xptr(NULL), row_ptr(NULL), col_ptr(NULL) {
    SEXP x=R_do_slot(incoming, install("x"));
    if (!isReal(x)) { throw std::runtime_error("'x' slot in a numeric dgCMatrix should be double-precision"); }
    xptr=REAL(x);

    try {
        col_ptr=new double[nrow];
        row_ptr=new double[ncol];
    } catch (std::exception& e) {
        delete [] col_ptr;
        delete [] row_ptr;
        throw; 
    }
    return;
}

Csparse_numeric_matrix::~Csparse_numeric_matrix () {
    delete [] col_ptr;
    delete [] row_ptr;
    return;
}

const double* Csparse_numeric_matrix::get_row(int r) {
    std::fill(row_ptr, row_ptr+ncol, 0);
    for (int col=0; col<ncol; ++col) { 
        if (pptr[col]!=pptr[col+1]) { row_ptr[col]=get(r, col); }
    }
    return row_ptr; 
}

const double* Csparse_numeric_matrix::get_col(int c) {
    const int& start=pptr[c];
    const int& end=pptr[c+1];
    std::fill(col_ptr, col_ptr+nrow, 0);
    for (int ix=start; ix<end; ++ix) {
        col_ptr[iptr[ix]]=xptr[ix];
    }
    return col_ptr;
}

double Csparse_numeric_matrix::get(int r, int c) {
    const int index=get_index(r, c);
    return (index!=nx ? xptr[index] : 0);
}

/* Methods for a Tsparse matrix. */

Tsparse_numeric_matrix::Tsparse_numeric_matrix(SEXP incoming) : Tsparse_matrix(incoming), xptr(NULL), row_ptr(NULL), col_ptr(NULL) {
    SEXP x=R_do_slot(incoming, install("x"));
    if (!isReal(x)) { throw std::runtime_error("'x' slot in a numeric dgTMatrix should be double-precision"); }
    xptr=REAL(x);

    try {
        col_ptr=new double[nrow];
        row_ptr=new double[ncol];
    } catch (std::exception& e) {
        delete [] col_ptr;
        delete [] row_ptr;
        throw; 
    }
    return;
}

Tsparse_numeric_matrix::~Tsparse_numeric_matrix () {
    delete [] col_ptr;
    delete [] row_ptr;
    return;
}

const double* Tsparse_numeric_matrix::get_row(int r) {
    std::fill(row_ptr, row_ptr+ncol, 0);
    for (int col=0; col<ncol; ++col) { 
        if (pptr[col]!=pptr[col+1]) { row_ptr[col]=get(r, col); }
    }
    return row_ptr; 
}

const double* Tsparse_numeric_matrix::get_col(int c) {
    const int& start=pptr[c];
    const int& end=pptr[c+1];
    std::fill(col_ptr, col_ptr+nrow, 0);
    for (int ix=start; ix<end; ++ix) {
        col_ptr[iptr2[ix]]=xptr[order[ix]];
    }
    return col_ptr;
}

double Tsparse_numeric_matrix::get(int r, int c) {
    const int index=get_index(r, c);
    return (index!=nx ? xptr[index] : 0);
}

/* Methods for a HDF5 matrix. */

HDF5_numeric_matrix::HDF5_numeric_matrix(SEXP incoming) : HDF5_matrix(incoming), row_ptr(NULL), col_ptr(NULL) {
    SEXP h5_seed=R_do_slot(incoming, install("seed")); 
    SEXP firstval=R_do_slot(h5_seed, install("first_val"));
    if (!isReal(firstval)) { 
        throw std::runtime_error("'first_val' should be double-precision");
    }
    
    try {
        col_ptr=new double[nrow];
        row_ptr=new double[ncol];
    } catch (std::exception& e) {
        delete [] col_ptr;
        delete [] row_ptr;
        throw; 
    }
    return;
}

HDF5_numeric_matrix::~HDF5_numeric_matrix( ){ 
    delete [] col_ptr;
    delete [] row_ptr;
    return;
}

const double * HDF5_numeric_matrix::get_row(int r) { 
    set_row(r);
    hdata.read(row_ptr, H5::PredType::NATIVE_DOUBLE, rowspace, hspace);
    return row_ptr;
} 

const double * HDF5_numeric_matrix::get_col(int c) { 
    set_col(c);
    hdata.read(col_ptr, H5::PredType::NATIVE_DOUBLE, colspace, hspace);
    return col_ptr; 
}

double HDF5_numeric_matrix::get(int r, int c) { 
    set_one(r, c);
    double out;
    hdata.read(&out, H5::PredType::NATIVE_DOUBLE, onespace, hspace);
    return out; 
}

/* Dispatch definition */

std::shared_ptr<numeric_matrix> create_numeric_matrix(SEXP incoming) { 
    if (IS_S4_OBJECT(incoming)) {
        const char* ctype=get_class(incoming);
        if (std::strcmp(ctype, "dgeMatrix")==0) { 
            return std::shared_ptr<numeric_matrix>(new dense_numeric_matrix(incoming));
        } else if (std::strcmp(ctype, "dgCMatrix")==0) { 
            return std::shared_ptr<numeric_matrix>(new Csparse_numeric_matrix(incoming));
        } else if (std::strcmp(ctype, "dgTMatrix")==0) {
            return std::shared_ptr<numeric_matrix>(new Tsparse_numeric_matrix(incoming));
        } else if (std::strcmp(ctype, "HDF5Matrix")==0) { 
            return std::shared_ptr<numeric_matrix>(new HDF5_numeric_matrix(incoming));
        }
    } else {
        return std::shared_ptr<numeric_matrix>(new simple_numeric_matrix(incoming));
    }
    throw std::runtime_error("unsupported data type for numeric_matrix");
}


