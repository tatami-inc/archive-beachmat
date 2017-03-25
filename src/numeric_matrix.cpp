#include "numeric_matrix.h"
#include "utils.h"

/* Methods for the base class. */

numeric_matrix::numeric_matrix() : nrow(0), ncol(0) {}

numeric_matrix::~numeric_matrix() {}

int numeric_matrix::get_nrow() const { return nrow; }

int numeric_matrix::get_ncol() const { return ncol; }

/* Methods for a simple matrix. */

simple_numeric_matrix::simple_numeric_matrix(SEXP incoming) : row_ptr(NULL) {
    if (!isReal(incoming)) { 
        throw std::runtime_error("matrix should be double-precision");
    }
    simple_ptr=REAL(incoming);
    fill_dims(nrow, ncol, getAttrib(incoming, R_DimSymbol));

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
    for (int col=0; col<ncol; ++col) {
        row_ptr[col]=simple_ptr[r+col*nrow];
    }
    return row_ptr; 
}

const double* simple_numeric_matrix::get_col(int c) {
    return simple_ptr + c*nrow;
}

double simple_numeric_matrix::get(int r, int c) const {
    return simple_ptr[r + c * nrow];
}

/* Methods for a Csparse matrix. */

Csparse_numeric_matrix::Csparse_numeric_matrix(SEXP incoming) : iptr(NULL), pptr(NULL), xptr(NULL), row_ptr(NULL), col_ptr(NULL) {
   
    if (!IS_S4_OBJECT(incoming) || std::strcmp(get_class(incoming), "dgCMatrix")!=0) {
        throw std::runtime_error("matrix should be a dgCMatrix object");
    }

    fill_dims(nrow, ncol, R_do_slot(incoming, install("Dim")));
    SEXP i=R_do_slot(incoming, install("i"));
    if (!isInteger(i)) { throw std::runtime_error("'i' slot in a dgCMatrix should be integer"); }
    iptr=INTEGER(i);
    SEXP p=R_do_slot(incoming, install("p"));
    if (!isInteger(p)) { throw std::runtime_error("'p' slot in a dgCMatrix should be integer"); }
    pptr=INTEGER(p);
    SEXP x=R_do_slot(incoming, install("x"));
    if (!isReal(x)) { throw std::runtime_error("'x' slot in a numeric dgCMatrix should be double-precision"); }
    xptr=REAL(x);

    nx=LENGTH(x);
    if (nx!=LENGTH(i)) { throw std::runtime_error("'x' and 'i' slots in a dgCMatrix should have the same length"); }
    if (ncol+1!=LENGTH(p)) { throw std::runtime_error("length of 'p' slot in a dgCMatrix should be equal to 'ncol+1'"); }
    if (pptr[ncol]!=nx || pptr[0]!=0) { throw std::runtime_error("first and last elements of 'p' should be 0 and 'length(x)', respectively"); }
    for (int i=1; i<=ncol; ++i) {
        if (pptr[i] < pptr[i-1]) { throw std::runtime_error("'p' is not sorted"); }
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

double Csparse_numeric_matrix::get(int r, int c) const {
    const int* istart=iptr + pptr[c];
    const int* iend=iptr + pptr[c+1];
    if (istart!=iend) { 
        const int* loc=std::lower_bound(istart, iend, r);
        if (loc!=iend && *loc==r) {
            return xptr[loc-iptr];
        }
    }
    return 0;
}

/* Dispatch definition */

std::shared_ptr<numeric_matrix> create_numeric_matrix(SEXP incoming) { 
    if (IS_S4_OBJECT(incoming)) {
        const char* ctype=get_class(incoming);
        if (std::strcmp(ctype, "dgCMatrix")==0) { 
            return std::shared_ptr<numeric_matrix>(new Csparse_numeric_matrix(incoming));
        }
    } else {
        return std::shared_ptr<numeric_matrix>(new simple_numeric_matrix(incoming));
    }
    throw std::runtime_error("unsupported data type for numeric_matrix");
}



