#include "beachmat.h"
#include "output_param.h"
#include "functions.h"

SEXP find_chunks (SEXP indim) {
    BEGIN_RCPP
    Rcpp::IntegerVector incoming(indim);
    if (incoming.size()!=2 || incoming[0] < 0 || incoming[1] < 0) { 
        throw std::runtime_error("dimensions must be a vector of 2 positive integers");
    }
    beachmat::output_param op(beachmat::HDF5);
    op.optimize_chunk_dims(incoming[0], incoming[1]);
    return Rcpp::IntegerVector::create(op.get_chunk_nrow(), op.get_chunk_ncol());
    END_RCPP
}
