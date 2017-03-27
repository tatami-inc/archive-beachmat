#include "numeric_matrix.h"
#include "logical_matrix.h"
#include "integer_matrix.h"

template <typename M, typename T> 
void fill_up (M& ptr, T* optr, SEXP mode) {
    if (!isInteger(mode) || LENGTH(mode)!=1) {
        throw std::runtime_error("'mode' should be an integer scalar"); 
    }
    const int Mode=asInteger(mode);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();

    if (Mode==1) { 
        // By column.
        const T* cptr=NULL;
        for (int c=0; c<ncols; ++c) {
            cptr=ptr->get_col(c);
            std::copy(cptr, cptr+nrows, optr+c*nrows);
        }
    } else if (Mode==2) { 
        // By row.
        const T* rptr=NULL;
        for (int r=0; r<nrows; ++r) {
            rptr=ptr->get_row(r);
            for (int c=0; c<ncols; ++c) {
                optr[c * nrows + r]=rptr[c];
            }
        }
    } else if (Mode==3) {
        // By cell.
        for (int c=0; c<ncols; ++c){ 
            for (int r=0; r<nrows; ++r) {
                optr[c * nrows + r]=ptr->get(r, c);
            }
        }
    } else { 
        throw std::runtime_error("'mode' should be in [1,3]"); 
    }

    return;
}

extern "C" {

SEXP test_numeric_access (SEXP in, SEXP mode) try {
    auto ptr=create_numeric_matrix(in);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();
    SEXP output=PROTECT(allocMatrix(REALSXP, nrows, ncols));
    try { 
        double * optr=REAL(output);
        fill_up(ptr, optr, mode);
    } catch (std::exception& e) {
        UNPROTECT(1);
        throw;
    }

    UNPROTECT(1);
    return output;
} catch (std::exception& e) {
    return mkString(e.what());
}

SEXP test_integer_access (SEXP in, SEXP mode) try {
    auto ptr=create_integer_matrix(in);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();
    SEXP output=PROTECT(allocMatrix(INTSXP, nrows, ncols));
    try { 
        int* optr=INTEGER(output);
        fill_up(ptr, optr, mode);
    } catch (std::exception& e) {
        UNPROTECT(1);
        throw;
    }

    UNPROTECT(1);
    return output;
} catch (std::exception& e) {
    return mkString(e.what());
}

SEXP test_logical_access (SEXP in, SEXP mode) try {
    auto ptr=create_logical_matrix(in);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();
    SEXP output=PROTECT(allocMatrix(LGLSXP, nrows, ncols));
    try { 
        int* optr=LOGICAL(output);
        fill_up(ptr, optr, mode);
    } catch (std::exception& e) {
        UNPROTECT(1);
        throw;
    }

    UNPROTECT(1);
    return output;
} catch (std::exception& e) {
    return mkString(e.what());
}

}
