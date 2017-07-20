#include "beachtest.h"
#include "template_infun.h"

/* Realized access functions. */

SEXP test_numeric_access (SEXP in, SEXP mode, SEXP order) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    return fill_up<Rcpp::NumericVector, Rcpp::NumericMatrix>(ptr.get(), mode, order);
    END_RCPP
}

SEXP test_integer_access (SEXP in, SEXP mode, SEXP order) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    return fill_up<Rcpp::IntegerVector, Rcpp::IntegerMatrix>(ptr.get(), mode, order);
    END_RCPP
}

SEXP test_logical_access (SEXP in, SEXP mode, SEXP order) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    return fill_up<Rcpp::LogicalVector, Rcpp::LogicalMatrix>(ptr.get(), mode, order);
    END_RCPP
}

SEXP test_character_access (SEXP in, SEXP mode, SEXP order) {
    BEGIN_RCPP
    auto ptr=beachmat::create_character_matrix(in);
    return fill_up<Rcpp::StringVector, Rcpp::StringMatrix>(ptr.get(), mode, order);
    END_RCPP
}

/* Realized slice-access functions. */

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

/* Const access functions. */

SEXP test_numeric_const_access (SEXP in) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    return fill_up_const<Rcpp::NumericVector, Rcpp::NumericMatrix>(ptr.get());
    END_RCPP
}

SEXP test_integer_const_access (SEXP in) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    return fill_up_const<Rcpp::IntegerVector, Rcpp::IntegerMatrix>(ptr.get());
    END_RCPP
}

SEXP test_logical_const_access (SEXP in) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    return fill_up_const<Rcpp::LogicalVector, Rcpp::LogicalMatrix>(ptr.get());
    END_RCPP
}

SEXP test_character_const_access (SEXP in) {
    BEGIN_RCPP
    auto ptr=beachmat::create_character_matrix(in);
    return fill_up_const<Rcpp::StringVector, Rcpp::StringMatrix>(ptr.get());
    END_RCPP
}

/* Realized const slice-access functions. */

SEXP test_numeric_const_slice (SEXP in, SEXP rx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    return fill_up_const_slice<Rcpp::NumericVector, Rcpp::NumericMatrix>(ptr.get(), rx);
    END_RCPP
}

SEXP test_integer_const_slice (SEXP in, SEXP rx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    return fill_up_const_slice<Rcpp::IntegerVector, Rcpp::IntegerMatrix>(ptr.get(), rx);
    END_RCPP
}

SEXP test_logical_const_slice (SEXP in, SEXP rx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    return fill_up_const_slice<Rcpp::LogicalVector, Rcpp::LogicalMatrix>(ptr.get(), rx);
    END_RCPP
}

SEXP test_character_const_slice (SEXP in, SEXP rx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_character_matrix(in);
    return fill_up_const_slice<Rcpp::StringVector, Rcpp::StringMatrix>(ptr.get(), rx);
    END_RCPP
}

/* Realized non-zero access functions. */

SEXP test_numeric_nonzero_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    return fill_up_nonzero<Rcpp::NumericVector, Rcpp::NumericMatrix>(ptr.get(), mode);
    END_RCPP
}

SEXP test_integer_nonzero_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    return fill_up_nonzero<Rcpp::IntegerVector, Rcpp::IntegerMatrix>(ptr.get(), mode);
    END_RCPP
}

SEXP test_logical_nonzero_access (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    return fill_up_nonzero<Rcpp::LogicalVector, Rcpp::LogicalMatrix>(ptr.get(), mode);
    END_RCPP
}

/* Realized non-zero slice functions. */

SEXP test_numeric_nonzero_slice (SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    return fill_up_nonzero_slice<Rcpp::NumericVector, Rcpp::NumericMatrix>(ptr.get(), mode, rx, cx);
    END_RCPP
}

SEXP test_integer_nonzero_slice (SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    return fill_up_nonzero_slice<Rcpp::IntegerVector, Rcpp::IntegerMatrix>(ptr.get(), mode, rx, cx);
    END_RCPP
}

SEXP test_logical_nonzero_slice (SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    return fill_up_nonzero_slice<Rcpp::LogicalVector, Rcpp::LogicalMatrix>(ptr.get(), mode, rx, cx);
    END_RCPP
}

/* Sparse access functions. */

SEXP test_sparse_numeric_slice(SEXP in, SEXP Inx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    const int& nrows=ptr->get_nrow();
    
    Rcpp::IntegerMatrix inx(Inx);
    if (inx.nrow()!=nrows) {
        throw std::runtime_error("'Inx' and input matrix should have same number of rows");
    }
    Rcpp::List output(nrows);
        
    // By row, using the requested column indices (check update_indices resets properly).
    for (int r=0; r<nrows; ++r) {
        int start=inx(r, 0)-1, end=inx(r, 1);
        Rcpp::NumericVector target(end-start);
        ptr->get_row(r, target.begin(), start, end);
        output[r]=target;
    }

    return output;
    END_RCPP
}

/* Type check and conversion functions. */

SEXP test_type_check(SEXP in) {
    BEGIN_RCPP
    std::string out=beachmat::translate_type(beachmat::find_sexp_type(in));
    return Rf_mkString(out.c_str());
    END_RCPP
}

SEXP test_numeric_to_logical (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    return fill_up<Rcpp::LogicalVector, Rcpp::LogicalMatrix>(ptr.get(), mode);
    END_RCPP
}

SEXP test_numeric_to_integer (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    return fill_up<Rcpp::IntegerVector, Rcpp::IntegerMatrix>(ptr.get(), mode);
    END_RCPP
}

SEXP test_logical_to_numeric (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    return fill_up<Rcpp::NumericVector, Rcpp::NumericMatrix>(ptr.get(), mode);
    END_RCPP
}

SEXP test_logical_to_integer (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    return fill_up<Rcpp::IntegerVector, Rcpp::IntegerMatrix>(ptr.get(), mode);
    END_RCPP
}

SEXP test_integer_to_numeric (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    return fill_up<Rcpp::NumericVector, Rcpp::NumericMatrix>(ptr.get(), mode);
    END_RCPP
}

SEXP test_integer_to_logical (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    return fill_up<Rcpp::LogicalVector, Rcpp::LogicalMatrix>(ptr.get(), mode);
    END_RCPP
}

/* Edge case error checking. */

SEXP test_integer_edge (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    input_edge<Rcpp::IntegerVector>(ptr.get(), mode);
    return Rf_ScalarLogical(1);
    END_RCPP
}

SEXP test_logical_edge (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    input_edge<Rcpp::LogicalVector>(ptr.get(), mode);
    return Rf_ScalarLogical(1);
    END_RCPP
}

SEXP test_numeric_edge (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    input_edge<Rcpp::NumericVector>(ptr.get(), mode);
    return Rf_ScalarLogical(1);
    END_RCPP
}

SEXP test_character_edge (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_character_matrix(in);
    input_edge<Rcpp::StringVector>(ptr.get(), mode);
    return Rf_ScalarLogical(1);
    END_RCPP
}

