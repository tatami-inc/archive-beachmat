#include "beachtest.h"
#include "template_outfun.h"

/* Realized output functions. */

SEXP test_integer_output(SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_integer_matrix(in);
    auto optr=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_integer_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::IntegerVector>(ptr.get(), optr.get(), optr2.get(), mode);
    END_RCPP
}

SEXP test_logical_output(SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_logical_matrix(in);
    auto optr=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_logical_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::LogicalVector>(ptr.get(), optr.get(), optr2.get(), mode);
    END_RCPP
}

SEXP test_numeric_output(SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in);
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in));
    auto optr2=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::NumericVector>(ptr.get(), optr.get(), optr2.get(), mode);
    END_RCPP
}

SEXP test_character_output(SEXP in, SEXP mode) {
    BEGIN_RCPP
    auto ptr=beachmat::create_character_matrix(in);
    beachmat::output_param op(in);
    op.set_strlen(10);
    auto optr=beachmat::create_character_output(ptr->get_nrow(), ptr->get_ncol(), op);
    auto optr2=beachmat::create_character_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);
    return pump_out<Rcpp::StringVector>(ptr.get(), optr.get(), optr2.get(), mode);
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

SEXP test_sparse_numeric_output(SEXP in, SEXP mode, SEXP rorder) {
    BEGIN_RCPP
    auto ptr=beachmat::create_numeric_matrix(in); // should be a sparse matrix.
    auto optr=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::output_param(in, false, true)); // a sparse matrix as output.
    auto optr2=beachmat::create_numeric_output(ptr->get_nrow(), ptr->get_ncol(), beachmat::SIMPLE_PARAM);

    Rcpp::IntegerVector M(mode);
    if (M.size()!=1) { 
        throw std::runtime_error("mode should be an integer scalar");
    } 
    Rcpp::IntegerVector order(rorder);
    const size_t& nrows=ptr->get_nrow();
    const size_t& ncols=ptr->get_ncol();

    if (M[0]==1) {
        if (order.size()!=ncols) {
            throw std::runtime_error("order should be of length equal to the number of columns");
        }
        Rcpp::NumericVector target(nrows);
        auto oIt=order.begin();
        for (size_t c=0; c<ncols; ++c, ++oIt) {
            ptr->get_col(c, target.begin());
            optr->set_col(*oIt, target.begin());
   
            // Wiping, reversing and refilling. 
            std::fill(target.begin(), target.end(), 0); 
            optr->get_col(*oIt, target.begin());
            std::reverse(target.begin(), target.end()); 
            optr2->set_col(*oIt, target.begin());
        }
    } else if (M[0]==2) {
        if (order.size()!=nrows){ 
            throw std::runtime_error("order should be of length equal to the number of rows");
        }
        Rcpp::NumericVector target(ncols);
        auto oIt=order.begin();
        for (size_t r=0; r<nrows; ++r, ++oIt) {
            ptr->get_row(r, target.begin());
            optr->set_row(*oIt, target.begin());
            
            // Wiping, reversing and refilling. 
            std::fill(target.begin(), target.end(), 0); 
            optr->get_row(*oIt, target.begin());
            std::reverse(target.begin(), target.end()); 
            optr2->set_row(*oIt, target.begin());
        }
    } else {
        // By cell.
        for (size_t c=0; c<ncols; ++c){ 
            for (size_t r=0; r<nrows; ++r) {
                optr->set(r, c, ptr->get(r, c));
                optr2->set(nrows-r-1, ncols-c-1, optr->get(r, c));
            }
        }
    }

    Rcpp::List output(2);
    output[0]=optr->yield();
    output[1]=optr2->yield();
    return output;
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
