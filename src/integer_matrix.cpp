#include "integer_matrix.h"

namespace beachmat {

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

/* Output dispatch definition */

std::shared_ptr<integer_output> create_integer_output(int nrow, int ncol, bool basic) {
    if (basic) { 
        return std::shared_ptr<integer_output>(new simple_integer_output(nrow, ncol));
    } 
    return std::shared_ptr<integer_output>(new HDF5_integer_output(nrow, ncol));
}

std::shared_ptr<integer_output> create_integer_output(int nrow, int ncol, const Rcpp::RObject& incoming, bool simplify) {
    return create_integer_output(nrow, ncol, (simplify || !incoming.isS4()));
}

}
