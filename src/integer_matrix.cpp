#include "integer_matrix.h"

/* Methods for a HDF5 matrix. */

#ifdef BEACHMAT_USE_HDF5

HDF5_integer_matrix::HDF5_integer_matrix(const Rcpp::RObject& incoming) : HDF5_matrix<int, H5::PredType::NATIVE_INT32>(incoming) {
    const Rcpp::RObject& h5_seed=incoming.slot("seed");
    const Rcpp::RObject& firstval=get_safe_slot(h5_seed, "first_val");
    if (firstval.sexp_type()!=INTSXP) {
        throw_custom_error("'first_val' slot in a ", get_class(h5_seed), " object should be integer");
    }
    if (hdata.getTypeClass()!=H5T_INTEGER) { 
        throw std::runtime_error("data type in HDF5 file is not integer");
    }
    return;
}

HDF5_integer_matrix::~HDF5_integer_matrix() {}

#endif

/* Dispatch definition */

std::shared_ptr<integer_matrix> create_integer_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) { 
        std::string ctype=get_class(incoming);
        if (ctype=="HDF5Matrix") { 
#ifdef BEACHMAT_USE_HDF5
            return std::shared_ptr<integer_matrix>(new HDF5_integer_matrix(incoming));
#else
            throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
        }
        std::stringstream err;
        err << "unsupported class '" << ctype << "' for integer_matrix";
        throw std::runtime_error(err.str().c_str());
    } 
    return std::shared_ptr<integer_matrix>(new simple_integer_matrix(incoming));
}

