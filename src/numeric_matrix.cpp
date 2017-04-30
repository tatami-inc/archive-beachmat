#include "numeric_matrix.h"

/* Methods for a HDF5 matrix. */

#ifdef BEACHMAT_USE_HDF5

HDF5_numeric_matrix::HDF5_numeric_matrix(const Rcpp::RObject& incoming) : HDF5_matrix<double, H5::PredType::NATIVE_DOUBLE>(incoming) { 
    const Rcpp::RObject& h5_seed=incoming.slot("seed");
    const Rcpp::RObject& firstval=get_safe_slot(h5_seed, "first_val");
    if (firstval.sexp_type()!=REALSXP) {
        throw_custom_error("'first_val' slot in a ", get_class(h5_seed), " object should be double-precision");
    }
    if (hdata.getTypeClass()!=H5T_FLOAT) { 
        throw std::runtime_error("data type in HDF5 file is not floating-point");
    }
    return;
}

HDF5_numeric_matrix::~HDF5_numeric_matrix() {}

#endif

/* Dispatch definition */

std::shared_ptr<numeric_matrix> create_numeric_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) {
        std::string ctype=get_class(incoming);
        if (ctype=="dgeMatrix") { 
            return std::shared_ptr<numeric_matrix>(new dense_numeric_matrix(incoming));
        } else if (ctype=="dgCMatrix") { 
            return std::shared_ptr<numeric_matrix>(new Csparse_numeric_matrix(incoming));
        } else if (ctype=="dgTMatrix") {
            throw std::runtime_error("dgTMatrix not supported, convert to dgCMatrix");
        } else if (ctype=="dspMatrix") {
            return std::shared_ptr<numeric_matrix>(new Psymm_numeric_matrix(incoming));
        } else if (ctype=="HDF5Matrix") { 
#ifdef BEACHMAT_USE_HDF5
            return std::shared_ptr<numeric_matrix>(new HDF5_numeric_matrix(incoming));
#else  
            throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
        }
        throw_custom_error("unsupported class '", ctype, "' for numeric_matrix");
    } 
    return std::shared_ptr<numeric_matrix>(new simple_numeric_matrix(incoming));
}


