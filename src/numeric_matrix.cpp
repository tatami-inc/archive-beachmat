#include "numeric_matrix.h"

namespace beachmat { 

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
        } else if (ctype=="HDF5Matrix" || ctype=="DelayedMatrix") { 
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

/* Output dispatch definition */

std::shared_ptr<numeric_output> create_numeric_output(int nrow, int ncol, bool basic) {
    if (basic) { 
        return std::shared_ptr<numeric_output>(new simple_numeric_output(nrow, ncol));
    } 
#ifdef BEACHMAT_USE_HDF5
    return std::shared_ptr<numeric_output>(new HDF5_numeric_output(nrow, ncol));
#else
    throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
}

std::shared_ptr<numeric_output> create_numeric_output(int nrow, int ncol, const Rcpp::RObject& incoming, bool simplify) {
    return create_numeric_output(nrow, ncol, (simplify || !incoming.isS4()));
}

}
