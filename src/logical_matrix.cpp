#include "logical_matrix.h"

namespace beachmat {

/* Csparse logical input methods. */

template<>
int Csparse_matrix<int, Rcpp::LogicalVector>::get_empty() const { return 0; }

/* HDF5 logical input methods. */

template<>
void HDF5_logical_matrix::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, &(*out), start, end);
    return;
}

template<>
void HDF5_logical_matrix::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, coltmp.data(), start, end);
    std::copy(coltmp.begin(), coltmp.begin() + end - start, out);
    return;
}

template<>
void HDF5_logical_matrix::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, &(*out), start, end);
    return;
}

template<>
void HDF5_logical_matrix::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, rowtmp.data(), start, end);
    std::copy(rowtmp.begin(), rowtmp.begin() + end - start, out);
    return;
}

/* Sparse numeric output methods. */

template<>
int sparse_output<int, Rcpp::LogicalVector>::get_empty() const { return 0; }

/* HDF5 logical output methods. */

template<>
int HDF5_output<int, Rcpp::LogicalVector>::get_empty() const { return 0; }

template<> 
void HDF5_logical_output::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, &(*out), start, end);
    return;
}

template<>
void HDF5_logical_output::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, rowtmp.data(), start, end);
    std::copy(rowtmp.begin(), rowtmp.begin() + end - start, out);
    return;
}

template<> 
void HDF5_logical_output::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, &(*out), start, end);
    return;
}

template<>
void HDF5_logical_output::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, coltmp.data(), start, end);
    std::copy(coltmp.begin(), coltmp.begin() + end - start, out);
    return;
}

template<> 
void HDF5_logical_output::fill_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_row(r, &(*out), start, end);
    return;
}

template<>
void HDF5_logical_output::fill_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    std::copy(out, out + end - start, rowtmp.begin());
    mat.fill_row(r, rowtmp.data(), start, end);
    return;
}

template<> 
void HDF5_logical_output::fill_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_col(c, &(*out), start, end);
    return;
}

template<>
void HDF5_logical_output::fill_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    std::copy(out, out + end - start, coltmp.begin());
    mat.fill_col(c, coltmp.data(), start, end);
    return;
}

/* Dispatch definition */

std::unique_ptr<logical_matrix> create_logical_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) {
        std::string ctype=get_class(incoming);
        if (ctype=="lgeMatrix") { 
            return std::unique_ptr<logical_matrix>(new dense_logical_matrix(incoming));
        } else if (ctype=="lgCMatrix") { 
            return std::unique_ptr<logical_matrix>(new Csparse_logical_matrix(incoming));
        } else if (ctype=="lgTMatrix") {
            throw std::runtime_error("lgTMatrix not supported, convert to lgCMatrix");
        } else if (ctype=="lspMatrix") {
            return std::unique_ptr<logical_matrix>(new Psymm_logical_matrix(incoming));
        } else if (ctype=="HDF5Matrix" || ctype=="DelayedMatrix") { 
            return std::unique_ptr<logical_matrix>(new HDF5_logical_matrix(incoming));
        }
        throw_custom_error("unsupported class '", ctype, "' for logical_matrix");
    } 
    return std::unique_ptr<logical_matrix>(new simple_logical_matrix(incoming));
}

/* Output dispatch definition */

std::unique_ptr<logical_output> create_logical_output(int nrow, int ncol, output_mode mode) {
    switch (mode) {
        case BASIC:
            return std::unique_ptr<logical_output>(new simple_logical_output(nrow, ncol));
        case SPARSE:
            return std::unique_ptr<logical_output>(new sparse_logical_output(nrow, ncol));
        case HDF5:
            return std::unique_ptr<logical_output>(new HDF5_logical_output(nrow, ncol));
        default:
            throw std::runtime_error("unsupported output mode for logical matrices");
    }
}

std::unique_ptr<logical_output> create_logical_output(int nrow, int ncol, const Rcpp::RObject& incoming, bool simplify, bool preserve_zero) {
    return create_logical_output(nrow, ncol, choose_output_mode(incoming, simplify, preserve_zero && get_class(incoming)=="lgCMatrix"));
}

}
