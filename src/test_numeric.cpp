#include "numeric_matrix.h"

extern "C" {

SEXP test_numeric_access (SEXP in, SEXP mode) try {
    auto ptr=create_numeric_matrix(in);
    if (!isInteger(mode) || LENGTH(mode)!=1) {
        throw std::runtime_error("'mode' should be an integer scalar"); 
    }
    const int M=asInteger(mode);
    if (M<1 || M>3) { throw std::runtime_error("'mode' should be in [1,3]"); }
    
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();
    SEXP output=PROTECT(allocMatrix(REALSXP, nrows, ncols));
    try { 
        double * optr=REAL(output);

        if (M==1) { 
            // By column.
            const double* cptr=NULL;
            for (int c=0; c<ncols; ++c) {
                cptr=ptr->get_col(c);
                std::copy(cptr, cptr+nrows, optr+c*nrows);
            }
        } else if (M==2) { 
            // By row.
            const double* rptr=NULL;
            for (int r=0; r<nrows; ++r) {
                rptr=ptr->get_row(r);
                for (int c=0; c<ncols; ++c) {
                    optr[c * nrows + r]=rptr[c];
                }
            }
        } else {
            // By cell.
            for (int c=0; c<ncols; ++c){ 
                for (int r=0; r<nrows; ++r) {
                    optr[c * nrows + r]=ptr->get(r, c);
                }
            }
        }
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
