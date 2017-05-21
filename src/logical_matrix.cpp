#include "logical_matrix.h"

namespace beachmat {

/* Column-sparse logical methods. */

Csparse_logical_matrix::Csparse_logical_matrix(const Rcpp::RObject& incoming) : Csparse_lin_matrix<int, Rcpp::LogicalVector>(incoming, 0) {}

Csparse_logical_matrix::~Csparse_logical_matrix() {}

std::unique_ptr<logical_matrix> Csparse_logical_matrix::clone() const {
    return std::unique_ptr<logical_matrix>(new Csparse_logical_matrix(*this));
}

/* HDF5 logical methods. */

HDF5_logical_matrix::HDF5_logical_matrix(const Rcpp::RObject& incoming) : 
    HDF5_lin_matrix<int>(incoming, LGLSXP, H5T_INTEGER, H5::PredType::NATIVE_INT32) {}

HDF5_logical_matrix::~HDF5_logical_matrix() {}

void HDF5_logical_matrix::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, &(*out), HPT, start, end);
}

void HDF5_logical_matrix::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, &(*out), HPT, start, end);
}

std::unique_ptr<logical_matrix> HDF5_logical_matrix::clone() const {
    return std::unique_ptr<logical_matrix>(new HDF5_logical_matrix(*this));
}

HDF5_logical_output::HDF5_logical_output(int nr, int nc) : HDF5_output<int, Rcpp::LogicalVector, logical_false>(nr, nc, H5::PredType::NATIVE_INT32) {
    H5::StrType str_type(0, H5T_VARIABLE);
    H5::DataSpace att_space(1, one_count);
    H5::Attribute att = hdata.createAttribute("storage.mode", str_type, att_space);
    att.write(str_type, std::string("logical"));
    return;
}

HDF5_logical_output::~HDF5_logical_output() {}

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
