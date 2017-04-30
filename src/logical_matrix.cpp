#include "logical_matrix.h"

namespace beachmat {

/* Dispatch definition */

std::shared_ptr<logical_matrix> create_logical_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) {
        std::string ctype=get_class(incoming);
        if (ctype=="lgeMatrix") { 
            return std::shared_ptr<logical_matrix>(new dense_logical_matrix(incoming));
        } else if (ctype=="lgCMatrix") { 
            return std::shared_ptr<logical_matrix>(new Csparse_logical_matrix(incoming));
        } else if (ctype=="lgTMatrix") {
            throw std::runtime_error("lgTMatrix not supported, convert to lgCMatrix");
        } else if (ctype=="lspMatrix") {
            return std::shared_ptr<logical_matrix>(new Psymm_logical_matrix(incoming));
        } else if (ctype=="HDF5Matrix") { 
#ifdef BEACHMAT_USE_HDF5            
            return std::shared_ptr<logical_matrix>(new HDF5_logical_matrix(incoming));
#else
            throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
        }
        throw_custom_error("unsupported class '", ctype, "' for logical_matrix");
    } 
    return std::shared_ptr<logical_matrix>(new simple_logical_matrix(incoming));
}

}
