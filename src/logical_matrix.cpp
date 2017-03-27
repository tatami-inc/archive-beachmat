#include "logical_matrix.h"

/* Methods for the base class. */

logical_matrix::logical_matrix() {}

logical_matrix::~logical_matrix() {}

/* Methods for a simple matrix. */

simple_logical_matrix::simple_logical_matrix(SEXP incoming) : simple_matrix(incoming), simple_ptr(NULL), row_ptr(NULL) {
    if (!isLogical(incoming)) { 
        throw std::runtime_error("matrix should be logical");
    }
    simple_ptr=LOGICAL(incoming);

    try {
        row_ptr=new int[ncol];
    } catch (std::exception& e) {
        delete [] row_ptr;
        throw; 
    }
    return;
}

simple_logical_matrix::~simple_logical_matrix () {
    delete [] row_ptr;
    return;
}

const int* simple_logical_matrix::get_row(int r) {
    for (int col=0; col<ncol; ++col) { row_ptr[col]=get(r, col); }
    return row_ptr; 
}

const int* simple_logical_matrix::get_col(int c) {
    return simple_ptr + c*nrow;
}

int simple_logical_matrix::get(int r, int c) {
    return simple_ptr[get_index(r, c)];
}

/* Methods for a dense Matrix. */

dense_logical_matrix::dense_logical_matrix(SEXP incoming) : dense_matrix(incoming), dense_ptr(NULL), row_ptr(NULL) { 
    SEXP x=get_safe_slot(incoming, "x");
    if (!isLogical(x)) { throw_custom_error("'x' slot in a ", get_class(incoming), " object should be logical"); }
    dense_ptr=LOGICAL(x);

    try {
        row_ptr=new int[ncol];
    } catch (std::exception& e) {
        delete [] row_ptr;
        throw; 
    }
    return;
}

dense_logical_matrix::~dense_logical_matrix() {
    delete [] row_ptr;
}

const int* dense_logical_matrix::get_row(int r) {
    for (int col=0; col<ncol; ++col) { row_ptr[col]=get(r, col); }
    return row_ptr; 
}

const int* dense_logical_matrix::get_col(int c) {
    return dense_ptr + c*nrow;
}

int dense_logical_matrix::get(int r, int c) {
    return dense_ptr[get_index(r, c)];
}

/* Methods for a Csparse matrix. */

Csparse_logical_matrix::Csparse_logical_matrix(SEXP incoming) : Csparse_matrix(incoming), xptr(NULL), row_ptr(NULL), col_ptr(NULL) {
    SEXP x=get_safe_slot(incoming, "x");
    if (!isLogical(x)) { throw_custom_error("'x' slot in a ", get_class(incoming), " object should be logical"); }
    xptr=LOGICAL(x);

    try {
        col_ptr=new int[nrow];
        row_ptr=new int[ncol];
    } catch (std::exception& e) {
        delete [] col_ptr;
        delete [] row_ptr;
        throw; 
    }
    return;
}

Csparse_logical_matrix::~Csparse_logical_matrix () {
    delete [] col_ptr;
    delete [] row_ptr;
    return;
}

const int* Csparse_logical_matrix::get_row(int r) {
    std::fill(row_ptr, row_ptr+ncol, 0);
    for (int col=0; col<ncol; ++col) { 
        if (pptr[col]!=pptr[col+1]) { row_ptr[col]=get(r, col); }
    }
    return row_ptr; 
}

const int* Csparse_logical_matrix::get_col(int c) {
    const int& start=pptr[c];
    const int& end=pptr[c+1];
    std::fill(col_ptr, col_ptr+nrow, 0);
    for (int ix=start; ix<end; ++ix) {
        col_ptr[iptr[ix]]=xptr[ix];
    }
    return col_ptr;
}

int Csparse_logical_matrix::get(int r, int c) {
    const int index=get_index(r, c);
    return (index!=nx ? xptr[index] : 0);
}

/* Methods for a Tsparse matrix. */

Tsparse_logical_matrix::Tsparse_logical_matrix(SEXP incoming) : Tsparse_matrix(incoming), xptr(NULL), row_ptr(NULL), col_ptr(NULL) {
    SEXP x=get_safe_slot(incoming, "x");
    if (!isLogical(x)) { throw_custom_error("'x' slot in a ", get_class(incoming), " object should be logical"); }
    xptr=LOGICAL(x);

    try {
        col_ptr=new int[nrow];
        row_ptr=new int[ncol];
    } catch (std::exception& e) {
        delete [] col_ptr;
        delete [] row_ptr;
        throw; 
    }
    return;
}

Tsparse_logical_matrix::~Tsparse_logical_matrix () {
    delete [] col_ptr;
    delete [] row_ptr;
    return;
}

const int* Tsparse_logical_matrix::get_row(int r) {
    std::fill(row_ptr, row_ptr+ncol, 0);
    for (int col=0; col<ncol; ++col) { 
        if (pptr[col]!=pptr[col+1]) { row_ptr[col]=get(r, col); }
    }
    return row_ptr; 
}

const int* Tsparse_logical_matrix::get_col(int c) {
    const int& start=pptr[c];
    const int& end=pptr[c+1];
    std::fill(col_ptr, col_ptr+nrow, 0);
    for (int ix=start; ix<end; ++ix) {
        col_ptr[iptr2[ix]]=xptr[order[ix]];
    }
    return col_ptr;
}

int Tsparse_logical_matrix::get(int r, int c) {
    const int index=get_index(r, c);
    return (index!=nx ? xptr[index] : 0);
}

/* Methods for a HDF5 matrix. */

HDF5_logical_matrix::HDF5_logical_matrix(SEXP incoming) : HDF5_matrix(incoming), row_ptr(NULL), col_ptr(NULL) {
    SEXP h5_seed=get_safe_slot(incoming, "seed"); 
    SEXP firstval=get_safe_slot(h5_seed, "first_val");
    if (!isLogical(firstval)) { 
        throw_custom_error("'first_val' slot in a ", get_class(h5_seed), " object should be logical");
    }
    if (hdata.getTypeClass()!=H5T_INTEGER) { 
        throw std::runtime_error("data type in HDF5 file is not integer");
    }

    try {
        col_ptr=new int[nrow];
        row_ptr=new int[ncol];
    } catch (std::exception& e) {
        delete [] col_ptr;
        delete [] row_ptr;
        throw; 
    }
    return;
}

HDF5_logical_matrix::~HDF5_logical_matrix( ){ 
    delete [] col_ptr;
    delete [] row_ptr;
    return;
}

const int * HDF5_logical_matrix::get_row(int r) { 
    set_row(r);
    hdata.read(row_ptr, H5::PredType::NATIVE_INT32, rowspace, hspace);
    return row_ptr;
} 

const int * HDF5_logical_matrix::get_col(int c) { 
    set_col(c);
    hdata.read(col_ptr, H5::PredType::NATIVE_INT32, colspace, hspace);
    return col_ptr; 
}

int HDF5_logical_matrix::get(int r, int c) { 
    set_one(r, c);
    int out;
    hdata.read(&out, H5::PredType::NATIVE_INT32, onespace, hspace);
    return out; 
}

/* Dispatch definition */

std::shared_ptr<logical_matrix> create_logical_matrix(SEXP incoming) { 
    if (IS_S4_OBJECT(incoming)) {
        const char* ctype=get_class(incoming);
        if (std::strcmp(ctype, "lgeMatrix")==0) { 
            return std::shared_ptr<logical_matrix>(new dense_logical_matrix(incoming));
        } else if (std::strcmp(ctype, "lgCMatrix")==0) { 
            return std::shared_ptr<logical_matrix>(new Csparse_logical_matrix(incoming));
        } else if (std::strcmp(ctype, "lgTMatrix")==0) {
            return std::shared_ptr<logical_matrix>(new Tsparse_logical_matrix(incoming));
        } else if (std::strcmp(ctype, "HDF5Matrix")==0) { 
            return std::shared_ptr<logical_matrix>(new HDF5_logical_matrix(incoming));
        }
        std::stringstream err;
        err << "unsupported class '" << ctype << "' for logical_matrix";
        throw std::runtime_error(err.str().c_str());
    } 
    return std::shared_ptr<logical_matrix>(new simple_logical_matrix(incoming));
}

