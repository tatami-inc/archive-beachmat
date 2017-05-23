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

/* HDF5 logical output methods. */

HDF5_logical_output::HDF5_logical_output(int nr, int nc) : HDF5_lin_output(nr, nc, H5::PredType::NATIVE_INT32, 0) {}

HDF5_logical_output::~HDF5_logical_output() {}

void HDF5_logical_output::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, &(*out), start, end);
}

void HDF5_logical_output::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, &(*out), start, end);
}

void HDF5_logical_output::fill_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_row(r, &(*out), start, end);
}

void HDF5_logical_output::fill_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_col(c, &(*out), start, end);
}

std::unique_ptr<logical_output> HDF5_logical_output::clone() const {
    return std::unique_ptr<logical_output>(new HDF5_logical_output(*this));
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
#ifdef BEACHMAT_USE_HDF5            
            return std::unique_ptr<logical_matrix>(new HDF5_logical_matrix(incoming));
#else
            throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
        }
        throw_custom_error("unsupported class '", ctype, "' for logical_matrix");
    } 
    return std::unique_ptr<logical_matrix>(new simple_logical_matrix(incoming));
}

/* Output dispatch definition */

std::unique_ptr<logical_output> create_logical_output(int nrow, int ncol, bool basic) {
    if (basic) { 
        return std::unique_ptr<logical_output>(new simple_logical_output(nrow, ncol));
    } 
#ifdef BEACHMAT_USE_HDF5
    return std::unique_ptr<logical_output>(new HDF5_logical_output(nrow, ncol));
#else
    throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
}

std::unique_ptr<logical_output> create_logical_output(int nrow, int ncol, const Rcpp::RObject& incoming, bool simplify) {
    return create_logical_output(nrow, ncol, (simplify || !incoming.isS4()));
}

}
