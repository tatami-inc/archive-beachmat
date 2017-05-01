#ifndef INTEGER_MATRIX_H
#define INTEGER_MATRIX_H

#include "matrix.h"

namespace beachmat {

/* Virtual base class for integer matrices. */

typedef any_matrix<int, Rcpp::IntegerVector> integer_matrix;

/* Simple integer matrix */

typedef simple_matrix<int, Rcpp::IntegerVector> simple_integer_matrix;

/* HDF5Matrix */

#ifdef BEACHMAT_USE_HDF5

typedef HDF5_matrix<int, Rcpp::IntegerVector, H5T_INTEGER, H5::PredType::NATIVE_INT32> HDF5_integer_matrix;

#endif

/* Dispatcher */

std::shared_ptr<integer_matrix> create_integer_matrix(const Rcpp::RObject&);

}

#endif
