#ifndef INTEGER_MATRIX_H
#define INTEGER_MATRIX_H

#include "matrix.h"

/* Virtual base class for integer matrices. */

typedef any_matrix<int> integer_matrix;

/* Simple integer matrix */

typedef simple_matrix<int, Rcpp::IntegerVector> simple_integer_matrix;

/* HDF5Matrix */

#ifdef BEACHMAT_USE_HDF5

class HDF5_integer_matrix : public HDF5_matrix<int, H5::PredType::NATIVE_INT32> {
public:
    HDF5_integer_matrix(const Rcpp::RObject&);
    ~HDF5_integer_matrix();
};

#endif

/* Dispatcher */

std::shared_ptr<integer_matrix> create_integer_matrix(const Rcpp::RObject&);

#endif
