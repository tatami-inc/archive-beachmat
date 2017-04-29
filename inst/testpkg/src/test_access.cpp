#include "numeric_matrix.h"
#include "integer_matrix.h"
#include "logical_matrix.h"

template <typename M, typename T> 
void fill_up (M& ptr, T* optr, const Rcpp::IntegerVector& mode) {
    if (mode.size()!=1) { 
        throw std::runtime_error("'mode' should be an integer scalar"); 
    }
    const int Mode=mode[0];
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

SEXP test_numeric_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=create_numeric_matrix(in);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();

    Rcpp::NumericMatrix output(nrows, ncols);
    double* optr=REAL(output.get__());
    fill_up(ptr, optr, mode);
    return output;
    END_RCPP
}

SEXP test_integer_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=create_integer_matrix(in);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();

    Rcpp::IntegerMatrix output(nrows, ncols);
    int* optr=INTEGER(output.get__());
    fill_up(ptr, optr, mode);
    return output;
    END_RCPP
}

SEXP test_logical_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=create_logical_matrix(in);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();

    Rcpp::LogicalMatrix output(nrows, ncols);
    int* optr=INTEGER(output.get__());
    fill_up(ptr, optr, mode);
    return output;
    END_RCPP
}

SEXP test_sparse_numeric(SEXP in, SEXP rorder) {
    BEGIN_RCPP
    auto ptr=create_numeric_matrix(in);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();
    
    Rcpp::IntegerVector ro(rorder);
    if (ro.size()!=nrows) {
        throw std::runtime_error("'rorder' should have length equal to number of rows");
    }

    Rcpp::NumericMatrix output(nrows, ncols);
    double* optr=REAL(output.get__());
        
    // By row, in the specified order.
    const double* rptr;
    for (int r=0; r<nrows; ++r) {
        int currow=ro[r]-1;
        rptr=ptr->get_row(currow);
        for (int c=0; c<ncols; ++c) {
            optr[c * nrows + r]=rptr[c];
        }
    }

    return output;
    END_RCPP
}

}

#include "R_ext/Rdynload.h"
#include "R_ext/Visibility.h"
#define REGISTER(x, i) {#x, (DL_FUNC) &x, i}

extern "C" {

static const R_CallMethodDef all_call_entries[] = {
    REGISTER(test_numeric_access, 2),
    REGISTER(test_integer_access, 2),
    REGISTER(test_logical_access, 2),
    REGISTER(test_sparse_numeric, 2),
    {NULL, NULL, 0}
};

void attribute_visible R_init_beachtest(DllInfo *dll) {
    R_registerRoutines(dll, NULL, all_call_entries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
    R_forceSymbols(dll, TRUE);
}

}

