#include "numeric_matrix.h"

namespace beachmat { 

/* Csparse numeric input methods. */

template<>
double Csparse_matrix<double, Rcpp::NumericVector>::get_empty() const { return 0; }

/* Sparse numeric output methods. */

template<>
double sparse_output<double, Rcpp::NumericVector>::get_empty() const { return 0; }

/* HDF5 numeric output methods. */

template<>
double HDF5_output<double, Rcpp::NumericVector>::get_empty() const { return 0; }

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
            return std::unique_ptr<numeric_matrix>(new HDF5_numeric_matrix(incoming));
        } else if (ctype=="RleMatrix") {
            return std::unique_ptr<numeric_matrix>(new Rle_numeric_matrix(incoming));
        }
        throw_custom_error("unsupported class '", ctype, "' for numeric_matrix");
    } 
    return std::unique_ptr<numeric_matrix>(new simple_numeric_matrix(incoming));
}

/* Output dispatch definition */

std::unique_ptr<numeric_output> create_numeric_output(int nrow, int ncol, output_mode mode) {
    switch (mode) {
        case BASIC:
            return std::unique_ptr<numeric_output>(new simple_numeric_output(nrow, ncol));
        case SPARSE:
            return std::unique_ptr<numeric_output>(new sparse_numeric_output(nrow, ncol));
        case HDF5:
            return std::unique_ptr<numeric_output>(new HDF5_numeric_output(nrow, ncol));
        default:
            throw std::runtime_error("unsupported output mode for numeric matrices");
    }
}

std::unique_ptr<numeric_output> create_numeric_output(int nrow, int ncol, const Rcpp::RObject& incoming, bool simplify, bool preserve_zero) {
    return create_numeric_output(nrow, ncol, choose_output_mode(incoming, simplify, preserve_zero));
}

}
