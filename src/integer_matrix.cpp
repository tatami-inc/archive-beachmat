#include "integer_matrix.h"

namespace beachmat {

/* HDF5 integer input methods. */

template<>
void HDF5_integer_matrix::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, &(*out), start, end);
    return;
}

template<>
void HDF5_integer_matrix::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, coltmp.data(), start, end);
    std::copy(coltmp.begin(), coltmp.begin() + end - start, out);
    return;
}

template<>
void HDF5_integer_matrix::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, &(*out), start, end);
    return;
}

template<>
void HDF5_integer_matrix::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, rowtmp.data(), start, end);
    std::copy(rowtmp.begin(), rowtmp.begin() + end - start, out);
    return;
}

/* HDF5 integer output methods. */

template<>
int HDF5_output<int, Rcpp::IntegerVector>::get_empty() const { return 0; }

template<> 
void HDF5_integer_output::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, &(*out), start, end);
    return;
}

template<>
void HDF5_integer_output::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, rowtmp.data(), start, end);
    std::copy(rowtmp.begin(), rowtmp.begin() + end - start, out);
    return;
}

template<> 
void HDF5_integer_output::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, &(*out), start, end);
    return;
}

template<>
void HDF5_integer_output::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, coltmp.data(), start, end);
    std::copy(coltmp.begin(), coltmp.begin() + end - start, out);
    return;
}

template<> 
void HDF5_integer_output::fill_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_row(r, &(*out), start, end);
    return;
}

template<>
void HDF5_integer_output::fill_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    std::copy(out, out + end - start, rowtmp.begin());
    mat.fill_row(r, rowtmp.data(), start, end);
    return;
}

template<> 
void HDF5_integer_output::fill_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_col(c, &(*out), start, end);
    return;
}

template<>
void HDF5_integer_output::fill_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    std::copy(out, out + end - start, coltmp.begin());
    mat.fill_col(c, coltmp.data(), start, end);
    return;
}

/* Dispatch definition */

std::unique_ptr<integer_matrix> create_integer_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) { 
        std::string ctype=get_class(incoming);
        if (ctype=="HDF5Matrix" || ctype=="DelayedMatrix") { 
            return std::unique_ptr<integer_matrix>(new HDF5_integer_matrix(incoming));
        }
        std::stringstream err;
        err << "unsupported class '" << ctype << "' for integer_matrix";
        throw std::runtime_error(err.str().c_str());
    } 
    return std::unique_ptr<integer_matrix>(new simple_integer_matrix(incoming));
}

/* Output dispatch definition */

std::unique_ptr<integer_output> create_integer_output(int nrow, int ncol, output_mode mode) {
    switch (mode) {
        case BASIC:
            return std::unique_ptr<integer_output>(new simple_integer_output(nrow, ncol));
        case HDF5:
            return std::unique_ptr<integer_output>(new HDF5_integer_output(nrow, ncol));
        default:
            throw std::runtime_error("unsupported output mode for integer matrices");
    }
}

std::unique_ptr<integer_output> create_integer_output(int nrow, int ncol, const Rcpp::RObject& incoming, bool simplify) {
    return create_integer_output(nrow, ncol, choose_output_mode(incoming, simplify, false));
}

}
