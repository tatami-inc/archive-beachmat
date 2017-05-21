#include "integer_matrix.h"

namespace beachmat {

/* HDF5 integer methods. */

HDF5_integer_matrix::HDF5_integer_matrix(const Rcpp::RObject& incoming) : 
    HDF5_lin_matrix(incoming, INTSXP, H5T_INTEGER, H5::PredType::NATIVE_INT32) {}

HDF5_integer_matrix::~HDF5_integer_matrix() {}

void HDF5_integer_matrix::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, &(*out), HPT, start, end);
}

void HDF5_integer_matrix::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, &(*out), HPT, start, end);
}

std::unique_ptr<integer_matrix> HDF5_integer_matrix::clone() const {
    return std::unique_ptr<integer_matrix>(new HDF5_integer_matrix(*this));
}

HDF5_integer_output::HDF5_integer_output(int nr, int nc) : 
    HDF5_output<int, Rcpp::IntegerVector, integer_zero>(nr, nc, H5::PredType::NATIVE_INT32) {}

HDF5_integer_output::~HDF5_integer_output() {}

/* Dispatch definition */

std::unique_ptr<integer_matrix> create_integer_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) { 
        std::string ctype=get_class(incoming);
        if (ctype=="HDF5Matrix" || ctype=="DelayedMatrix") { 
#ifdef BEACHMAT_USE_HDF5
            return std::unique_ptr<integer_matrix>(new HDF5_integer_matrix(incoming));
#else
            throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
        }
        std::stringstream err;
        err << "unsupported class '" << ctype << "' for integer_matrix";
        throw std::runtime_error(err.str().c_str());
    } 
    return std::unique_ptr<integer_matrix>(new simple_integer_matrix(incoming));
}

/* Output dispatch definition */

std::unique_ptr<integer_output> create_integer_output(int nrow, int ncol, bool basic) {
    if (basic) { 
        return std::unique_ptr<integer_output>(new simple_integer_output(nrow, ncol));
    } 
#ifdef BEACHMAT_USE_HDF5
    return std::unique_ptr<integer_output>(new HDF5_integer_output(nrow, ncol));
#else
    throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
}

std::unique_ptr<integer_output> create_integer_output(int nrow, int ncol, const Rcpp::RObject& incoming, bool simplify) {
    return create_integer_output(nrow, ncol, (simplify || !incoming.isS4()));
}

}
