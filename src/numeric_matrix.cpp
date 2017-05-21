#include "numeric_matrix.h"

namespace beachmat { 

/* Column-sparse numeric methods. */

Csparse_numeric_matrix::Csparse_numeric_matrix(const Rcpp::RObject& incoming) : Csparse_lin_matrix<double, Rcpp::NumericVector>(incoming, 0) {}

Csparse_numeric_matrix::~Csparse_numeric_matrix() {}

std::unique_ptr<numeric_matrix> Csparse_numeric_matrix::clone() const {
    return std::unique_ptr<numeric_matrix>(new Csparse_numeric_matrix(*this));
}

/* HDF5 numeric methods. */

HDF5_numeric_matrix::HDF5_numeric_matrix(const Rcpp::RObject& incoming) : 
    HDF5_lin_matrix<double>(incoming, REALSXP, H5T_FLOAT, H5::PredType::NATIVE_DOUBLE) {}

HDF5_numeric_matrix::~HDF5_numeric_matrix() {}

void HDF5_numeric_matrix::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, &(*out), start, end);
}

void HDF5_numeric_matrix::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, &(*out), start, end);
}

std::unique_ptr<numeric_matrix> HDF5_numeric_matrix::clone() const {
    return std::unique_ptr<numeric_matrix>(new HDF5_numeric_matrix(*this));
}
    
HDF5_numeric_output::HDF5_numeric_output(int nr, int nc) : 
    HDF5_output<double, Rcpp::NumericVector, numeric_zero>(nr, nc, H5::PredType::NATIVE_DOUBLE) {}

HDF5_numeric_output::~HDF5_numeric_output() {}

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
