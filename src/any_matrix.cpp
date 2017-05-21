#include "any_matrix.h"

any_matrix::any_matrix() : nrow(0), ncol(0) {}

any_matrix::~any_matrix() {}

size_t any_matrix::get_nrow() const { return nrow; }

size_t any_matrix::get_ncol() const { return ncol; }

void any_matrix::fill_dims(const Rcpp::RObject& dims) {
    Rcpp::IntegerVector d;
    if (dims.sexp_type()!=d.sexp_type() || (d=dims).size()!=2) { 
        throw std::runtime_error("matrix dimensions should be an integer vector of length 2");
    }
    if (d[0]<0 || d[1]<0) { throw std::runtime_error("dimensions should be non-negative"); }
    nrow=d[0];
    ncol=d[1];
    return;
}


