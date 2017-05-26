#include "beachtest.h"
#include "template_outfun.h"

/* Realized output functions. */

SEXP test_integer_output(SEXP in, SEXP mode, SEXP redo) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    auto optr=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    return pump_out<Rcpp::IntegerVector>(ptr.get(), optr.get(), mode, redo);
    END_RCPP
}

SEXP test_logical_output(SEXP in, SEXP mode, SEXP redo) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    return pump_out<Rcpp::LogicalVector>(ptr.get(), optr.get(), mode, redo);
    END_RCPP
}

SEXP test_numeric_output(SEXP in, SEXP mode, SEXP redo) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    return pump_out<Rcpp::NumericVector>(ptr.get(), optr.get(), mode, redo);
    END_RCPP
}

/* Realized output slice functions. */

SEXP test_integer_output_slice(SEXP in, SEXP mode, SEXP rx, SEXP cx, SEXP redo) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    auto optr=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    return pump_out_slice<Rcpp::IntegerVector>(ptr.get(), optr.get(), mode, rx, cx, redo);
    END_RCPP
}

SEXP test_logical_output_slice(SEXP in, SEXP mode, SEXP rx, SEXP cx, SEXP redo) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    return pump_out_slice<Rcpp::LogicalVector>(ptr.get(), optr.get(), mode, rx, cx, redo);
    END_RCPP
}

SEXP test_numeric_output_slice(SEXP in, SEXP mode, SEXP rx, SEXP cx, SEXP redo) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    return pump_out_slice<Rcpp::NumericVector>(ptr.get(), optr.get(), mode, rx, cx, redo);
    END_RCPP
}

/* Conversion functions. */

SEXP test_integer_to_logical_output(SEXP in, SEXP mode, SEXP redo) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    return pump_out<Rcpp::IntegerVector>(ptr.get(), optr.get(), mode, redo);
    END_RCPP
}

SEXP test_integer_to_numeric_output(SEXP in, SEXP mode, SEXP redo) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    return pump_out<Rcpp::IntegerVector>(ptr.get(), optr.get(), mode, redo);
    END_RCPP
}

SEXP test_logical_to_integer_output(SEXP in, SEXP mode, SEXP redo) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    auto optr=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    return pump_out<Rcpp::LogicalVector>(ptr.get(), optr.get(), mode, redo);
    END_RCPP
}

SEXP test_logical_to_numeric_output(SEXP in, SEXP mode, SEXP redo) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    return pump_out<Rcpp::LogicalVector>(ptr.get(), optr.get(), mode, redo);
    END_RCPP
}

SEXP test_numeric_to_logical_output(SEXP in, SEXP mode, SEXP redo) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    return pump_out<Rcpp::NumericVector>(ptr.get(), optr.get(), mode, redo);
    END_RCPP
}

SEXP test_numeric_to_integer_output(SEXP in, SEXP mode, SEXP redo) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    auto optr=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    return pump_out<Rcpp::NumericVector>(ptr.get(), optr.get(), mode, redo);
    END_RCPP
}

/* Edge case error checking. */

SEXP test_integer_edge_output (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    auto optr=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    output_edge<Rcpp::IntegerVector>(optr.get(), mode);
    return Rf_ScalarLogical(1);
    END_RCPP
}

SEXP test_logical_edge_output (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    output_edge<Rcpp::LogicalVector>(optr.get(), mode);
    return Rf_ScalarLogical(1);
    END_RCPP
}

SEXP test_numeric_edge_output (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), in, false);
    output_edge<Rcpp::NumericVector>(optr.get(), mode);
    return Rf_ScalarLogical(1);
    END_RCPP
}

