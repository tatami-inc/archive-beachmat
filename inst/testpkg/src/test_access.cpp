#include "beachmat/numeric_matrix.h"
#include "beachmat/integer_matrix.h"
#include "beachmat/logical_matrix.h"
#include "beachmat/character_matrix.h"

template <typename T, typename M, class O>  // Only T needs to be specified, rest is automatically deduced.
void fill_up (M& ptr, O& output, const Rcpp::IntegerVector& mode) {
    if (mode.size()!=1) { 
        throw std::runtime_error("'mode' should be an integer scalar"); 
    }
    const int Mode=mode[0];
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();

    if (Mode==1) { 
        // By column.
        std::vector<T> target(nrows);
        for (int c=0; c<ncols; ++c) {
            ptr->get_col(c, target.data());
            for (int r=0; r<nrows; ++r) {
                output[c * nrows + r]=target[r];
            }
        }
    } else if (Mode==2) { 
        // By row.
        std::vector<T> target(ncols);
        for (int r=0; r<nrows; ++r) {
            ptr->get_row(r, target.data());
            for (int c=0; c<ncols; ++c) {
                output[c * nrows + r]=target[c];
            }
        }
    } else if (Mode==3) {
        // By cell.
        for (int c=0; c<ncols; ++c){ 
            for (int r=0; r<nrows; ++r) {
                output[c * nrows + r]=ptr->get(r, c);
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
    auto ptr=beachmat::create_numeric_matrix(in);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();

    Rcpp::NumericMatrix output(nrows, ncols);
    fill_up<double>(ptr, output, mode);
    return output;
    END_RCPP
}

SEXP test_integer_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();

    Rcpp::IntegerMatrix output(nrows, ncols);
    fill_up<int>(ptr, output, mode);
    return output;
    END_RCPP
}

SEXP test_logical_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();

    Rcpp::LogicalMatrix output(nrows, ncols);
    fill_up<int>(ptr, output, mode);
    return output;
    END_RCPP
}

SEXP test_character_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_character_matrix(in);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();

    Rcpp::CharacterMatrix output(nrows, ncols);
    fill_up<Rcpp::String>(ptr, output, mode);
    return output;
    END_RCPP
}

SEXP test_sparse_numeric(SEXP in, SEXP rorder) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();
    
    Rcpp::IntegerVector ro(rorder);
    if (ro.size()!=nrows) {
        throw std::runtime_error("'rorder' should have length equal to number of rows");
    }

    Rcpp::NumericMatrix output(nrows, ncols);
    double* optr=REAL(output.get__());
        
    // By row, in the specified order.
    std::vector<double> target(ncols);
    for (int r=0; r<nrows; ++r) {
        int currow=ro[r]-1;
        ptr->get_row(currow, target.data());
        for (int c=0; c<ncols; ++c) {
            optr[c * nrows + r]=target[c];
        }
    }

    return output;
    END_RCPP
}

SEXP test_type_check(SEXP in) {
    BEGIN_RCPP
    std::string out=beachmat::translate_type(beachmat::find_sexp_type(in));
    return Rf_mkString(out.c_str());
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
    REGISTER(test_character_access, 2),
    REGISTER(test_sparse_numeric, 2),
    REGISTER(test_type_check, 1),
    {NULL, NULL, 0}
};

void attribute_visible R_init_beachtest(DllInfo *dll) {
    R_registerRoutines(dll, NULL, all_call_entries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
    R_forceSymbols(dll, TRUE);
}

}

