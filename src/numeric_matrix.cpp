#include "numeric_matrix.h"

namespace beachmat { 

/* Csparse logical input methods. */

template<>
double Csparse_matrix<double, Rcpp::NumericVector>::get_empty() const { return 0; }

/* HDF5 integer input methods. */

template<>
void HDF5_numeric_matrix::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, coltmp.data(), start, end);
    std::copy(coltmp.begin(), coltmp.begin() + end - start, out);
    return;
}

template<>
void HDF5_numeric_matrix::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, &(*out), start, end);
    return;
}

template<>
void HDF5_numeric_matrix::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, rowtmp.data(), start, end);
    std::copy(rowtmp.begin(), rowtmp.begin() + end - start, out);
    return;
}

template<>
void HDF5_numeric_matrix::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, &(*out), start, end);
    return;
}

/* HDF5 numeric output methods. */

template<>
double HDF5_output<double, Rcpp::NumericVector>::get_empty() const { return 0; }

template<> 
void HDF5_numeric_output::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, rowtmp.data(), start, end);
    std::copy(rowtmp.begin(), rowtmp.begin() + end - start, out);
    return;
}

template<>
void HDF5_numeric_output::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, &(*out), start, end);
    return;
}

template<> 
void HDF5_numeric_output::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, coltmp.data(), start, end);
    std::copy(coltmp.begin(), coltmp.begin() + end - start, out);
    return;
}

template<>
void HDF5_numeric_output::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, &(*out), start, end);
    return;
}

template<> 
void HDF5_numeric_output::fill_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    std::copy(out, out + end - start, rowtmp.begin());
    mat.fill_row(r, rowtmp.data(), start, end);
    return;
}

template<>
void HDF5_numeric_output::fill_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.fill_row(r, &(*out), start, end);
    return;
}

template<> 
void HDF5_numeric_output::fill_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    std::copy(out, out + end - start, coltmp.begin());
    mat.fill_col(c, coltmp.data(), start, end);
    return;
}

template<>
void HDF5_numeric_output::fill_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.fill_col(c, &(*out), start, end);
    return;
}

/* Dispatch definition */

std::unique_ptr<numeric_matrix> create_numeric_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) {
        std::string ctype=get_class(incoming);
        if (ctype=="dgeMatrix") { 
            return std::unique_ptr<numeric_matrix>(new dense_numeric_matrix(incoming));
        } else if (ctype=="dgCMatrix") { 
            return std::unique_ptr<numeric_matrix>(new Csparse_numeric_matrix(incoming));
        } else if (ctype=="dgTMatrix") {
            throw std::runtime_error("dgTMatrix not supported, convert to dgCMatrix");
        } else if (ctype=="dspMatrix") {
            return std::unique_ptr<numeric_matrix>(new Psymm_numeric_matrix(incoming));
        } else if (ctype=="HDF5Matrix" || ctype=="DelayedMatrix") { 
#ifdef BEACHMAT_USE_HDF5
            return std::unique_ptr<numeric_matrix>(new HDF5_numeric_matrix(incoming));
#else  
            throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
        }
        throw_custom_error("unsupported class '", ctype, "' for numeric_matrix");
    } 
    return std::unique_ptr<numeric_matrix>(new simple_numeric_matrix(incoming));
}

/* Output dispatch definition */

std::unique_ptr<numeric_output> create_numeric_output(int nrow, int ncol, bool basic) {
    if (basic) { 
        return std::unique_ptr<numeric_output>(new simple_numeric_output(nrow, ncol));
    } 
#ifdef BEACHMAT_USE_HDF5
    return std::unique_ptr<numeric_output>(new HDF5_numeric_output(nrow, ncol));
#else
    throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
}

std::unique_ptr<numeric_output> create_numeric_output(int nrow, int ncol, const Rcpp::RObject& incoming, bool simplify) {
    return create_numeric_output(nrow, ncol, (simplify || !incoming.isS4()));
}

}
