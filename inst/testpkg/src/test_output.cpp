#include "beachtest.h"
#include "template_outfun.h"

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

