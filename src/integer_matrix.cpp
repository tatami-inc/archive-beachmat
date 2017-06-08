#include "integer_matrix.h"

namespace beachmat {

/* HDF5 integer output methods. */

template<>
int HDF5_output<int, Rcpp::IntegerVector>::get_empty() const { return 0; }

/* Dispatch definition */

std::unique_ptr<integer_matrix> create_integer_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) { 
        std::string ctype=get_class(incoming);
        if (ctype=="HDF5Matrix" || ctype=="DelayedMatrix") { 
            return std::unique_ptr<integer_matrix>(new HDF5_integer_matrix(incoming));
        } else if (ctype=="RleMatrix") {
            return std::unique_ptr<integer_matrix>(new Rle_integer_matrix(incoming));
        }
        std::stringstream err;
        err << "unsupported class '" << ctype << "' for integer_matrix";
        throw std::runtime_error(err.str().c_str());
    } 
    return std::unique_ptr<integer_matrix>(new simple_integer_matrix(incoming));
}

/* Output dispatch definition */

std::unique_ptr<integer_output> create_integer_output(int nrow, int ncol, output_mode mode) {
    switch (mode) {
        case BASIC:
            return std::unique_ptr<integer_output>(new simple_integer_output(nrow, ncol));
        case HDF5:
            return std::unique_ptr<integer_output>(new HDF5_integer_output(nrow, ncol));
        default:
            throw std::runtime_error("unsupported output mode for integer matrices");
    }
}

std::unique_ptr<integer_output> create_integer_output(int nrow, int ncol, const Rcpp::RObject& incoming, bool simplify) {
    return create_integer_output(nrow, ncol, choose_output_mode(incoming, simplify, false));
}

}
