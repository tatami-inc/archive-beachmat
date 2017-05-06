#include "beachtest.h"
#include "template_infun.h"

SEXP test_numeric_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    return fill_up<Rcpp::NumericVector, Rcpp::NumericMatrix>(ptr.get(), mode);
    END_RCPP
}

SEXP test_integer_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    return fill_up<Rcpp::IntegerVector, Rcpp::IntegerMatrix>(ptr.get(), mode);
    END_RCPP
}

SEXP test_logical_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    return fill_up<Rcpp::LogicalVector, Rcpp::LogicalMatrix>(ptr.get(), mode);
    END_RCPP
}

SEXP test_character_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_character_matrix(in);
    return fill_up<Rcpp::StringVector, Rcpp::StringMatrix>(ptr.get(), mode);
    END_RCPP
}

SEXP test_numeric_slice (SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    return fill_up_slice<Rcpp::NumericVector, Rcpp::NumericMatrix>(ptr.get(), mode, rx, cx);
    END_RCPP
}

SEXP test_integer_slice (SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    return fill_up_slice<Rcpp::IntegerVector, Rcpp::IntegerMatrix>(ptr.get(), mode, rx, cx);
    END_RCPP
}

SEXP test_logical_slice (SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    return fill_up_slice<Rcpp::LogicalVector, Rcpp::LogicalMatrix>(ptr.get(), mode, rx, cx);
    END_RCPP
}

SEXP test_character_slice (SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_character_matrix(in);
    return fill_up_slice<Rcpp::StringVector, Rcpp::StringMatrix>(ptr.get(), mode, rx, cx);
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
        
    // By row, in the specified order.
    Rcpp::NumericVector target(ncols);
    for (int r=0; r<nrows; ++r) {
        int currow=ro[r]-1;
        ptr->get_row(currow, target.begin());
        for (int c=0; c<ncols; ++c) {
            output[c * nrows + r]=target[c];
        }
    }

    return output;
    END_RCPP
}

SEXP test_sparse_numeric_slice(SEXP in, SEXP Inx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    const int& nrows=ptr->get_nrow();
    
    Rcpp::IntegerMatrix inx(Inx);
    if (inx.nrow()!=nrows) {
        throw std::runtime_error("'rorder' should have length equal to number of rows");
    }
    Rcpp::List output(nrows);
        
    // By row, using the requested column indices (check update_indices works properly).
    for (int r=0; r<nrows; ++r) {
        int start=inx(r, 0)-1, end=inx(r, 1);
        Rcpp::NumericVector target(end-start);
        ptr->get_row(r, target.begin(), start, end);
        output[r]=target;
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


