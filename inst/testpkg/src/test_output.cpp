#include "beachtest.h"
#include "template_outfun.h"

/* Realized output functions. */

SEXP test_integer_output(SEXP in, SEXP mode, SEXP order) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    auto optr=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::IntegerVector>(ptr.get(), optr.get(), optr2.get(), mode, order);
    END_RCPP
}

SEXP test_logical_output(SEXP in, SEXP mode, SEXP order) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::LogicalVector>(ptr.get(), optr.get(), optr2.get(), mode, order);
    END_RCPP
}

SEXP test_numeric_output(SEXP in, SEXP mode, SEXP order) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::NumericVector>(ptr.get(), optr.get(), optr2.get(), mode, order);
    END_RCPP
}

SEXP test_character_output(SEXP in, SEXP mode, SEXP order) {
    BEGIN_RCPP
    auto ptr=beachmat::create_character_matrix(in);
    beachmat::output_param op(in);
    op.set_strlen(10);
    auto optr=beachmat::create_character_output(ptr->get_nrow(), ptr->get_ncol(), op);
    auto optr2=beachmat::create_character_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::StringVector>(ptr.get(), optr.get(), optr2.get(), mode, order);
    END_RCPP
}

/* Realized output slice functions. */

SEXP test_integer_output_slice(SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    auto optr=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out_slice<Rcpp::IntegerVector>(ptr.get(), optr.get(), optr2.get(), mode, rx, cx);
    END_RCPP
}

SEXP test_logical_output_slice(SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out_slice<Rcpp::LogicalVector>(ptr.get(), optr.get(), optr2.get(), mode, rx, cx);
    END_RCPP
}

SEXP test_numeric_output_slice(SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out_slice<Rcpp::NumericVector>(ptr.get(), optr.get(), optr2.get(), mode, rx, cx);
    END_RCPP
}

SEXP test_character_output_slice(SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_character_matrix(in);
    beachmat::output_param op(in);
    op.set_strlen(10);
    auto optr=beachmat::create_character_output(ptr->get_nrow(), ptr->get_ncol(), op);
    auto optr2=beachmat::create_character_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out_slice<Rcpp::StringVector>(ptr.get(), optr.get(), optr2.get(), mode, rx, cx);
    END_RCPP
}

/* Conversion functions. */

SEXP test_integer_to_logical_output(SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::IntegerVector>(ptr.get(), optr.get(), optr2.get(), mode);
    END_RCPP
}

SEXP test_integer_to_numeric_output(SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::IntegerVector>(ptr.get(), optr.get(), optr2.get(), mode);
    END_RCPP
}

SEXP test_logical_to_integer_output(SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    auto optr=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::LogicalVector>(ptr.get(), optr.get(), optr2.get(), mode);
    END_RCPP
}

SEXP test_logical_to_numeric_output(SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::LogicalVector>(ptr.get(), optr.get(), optr2.get(), mode);
    END_RCPP
}

SEXP test_numeric_to_logical_output(SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::NumericVector>(ptr.get(), optr.get(), optr2.get(), mode);
    END_RCPP
}

SEXP test_numeric_to_integer_output(SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    auto optr=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::NumericVector>(ptr.get(), optr.get(), optr2.get(), mode);
    END_RCPP
}

/* Edge case error checking. */

SEXP test_integer_edge_output (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    auto optr=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    output_edge<Rcpp::IntegerVector>(optr.get(), mode);
    return Rf_ScalarLogical(1);
    END_RCPP
}

SEXP test_logical_edge_output (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    output_edge<Rcpp::LogicalVector>(optr.get(), mode);
    return Rf_ScalarLogical(1);
    END_RCPP
}

SEXP test_numeric_edge_output (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    output_edge<Rcpp::NumericVector>(optr.get(), mode);
    return Rf_ScalarLogical(1);
    END_RCPP
}

SEXP test_character_edge_output (SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_character_matrix(in);
    beachmat::output_param op(in);
    op.set_strlen(10);
    auto optr=beachmat::create_character_output(ptr->get_nrow(), ptr->get_ncol(), op);
    output_edge<Rcpp::StringVector>(optr.get(), mode);
    return Rf_ScalarLogical(1);
    END_RCPP
}

/* Sparse output. */

SEXP test_sparse_numeric_output(SEXP in, SEXP mode, SEXP order) { 
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in); // should be a sparse matrix.
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in, false, true)); // a sparse matrix as output.
    auto optr2=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::NumericVector>(ptr.get(), optr.get(), optr2.get(), mode, order);
    END_RCPP
}

SEXP test_sparse_numeric_output_slice(SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in); // should be a sparse matrix.
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in, false, true)); // a sparse matrix as output.
    auto optr2=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out_slice<Rcpp::NumericVector>(ptr.get(), optr.get(), optr2.get(), mode, rx, cx);
    END_RCPP
}

SEXP test_sparse_logical_output(SEXP in, SEXP mode, SEXP order) { 
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in); // should be a sparse matrix.
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in, false, true)); // a sparse matrix as output.
    auto optr2=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::LogicalVector>(ptr.get(), optr.get(), optr2.get(), mode, order);
    END_RCPP
}

SEXP test_sparse_logical_output_slice(SEXP in, SEXP mode, SEXP rx, SEXP cx) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in); // should be a sparse matrix.
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in, false, true)); // a sparse matrix as output.
    auto optr2=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out_slice<Rcpp::LogicalVector>(ptr.get(), optr.get(), optr2.get(), mode, rx, cx);
    END_RCPP
}

/* Checking output type selection. */

SEXP select_output_by_sexp (SEXP incoming, SEXP simplify, SEXP preserve_zero) {
    beachmat::output_param op(incoming, Rf_asLogical(simplify), Rf_asLogical(preserve_zero));
    return Rcpp::IntegerVector::create(op.get_mode());
}

SEXP select_output_by_mode (SEXP incoming, SEXP simplify, SEXP preserve_zero) {
    beachmat::matrix_type mode;
    
    Rcpp::StringVector requested(incoming);
    std::string thingy=Rcpp::as<std::string>(requested[0]);
    if (thingy=="simple") {
        mode=beachmat::SIMPLE;
    } else if (thingy=="HDF5") {
        mode=beachmat::HDF5;
    } else if (thingy=="sparse") {
        mode=beachmat::SPARSE;
    } else if (thingy=="RLE") {
        mode=beachmat::RLE;
    } else if (thingy=="dense") {
        mode=beachmat::DENSE;
    } else if (thingy=="Psymm") {
        mode=beachmat::PSYMM;
    }

    beachmat::output_param op(mode, Rf_asLogical(simplify), Rf_asLogical(preserve_zero));
    return Rcpp::IntegerVector::create(op.get_mode());
}

SEXP get_all_modes () {
    return Rcpp::IntegerVector::create(Rcpp::Named("simple")=beachmat::SIMPLE,
                                       Rcpp::Named("HDF5")=beachmat::HDF5,
                                       Rcpp::Named("sparse")=beachmat::SPARSE,
                                       Rcpp::Named("RLE")=beachmat::RLE,
                                       Rcpp::Named("dense")=beachmat::DENSE,
                                       Rcpp::Named("Psymm")=beachmat::PSYMM);
}

