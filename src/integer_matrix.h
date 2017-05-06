#ifndef BEACHMAT_INTEGER_MATRIX_H
#define BEACHMAT_INTEGER_MATRIX_H

#include "matrix.h"
#include "output.h"

namespace beachmat {

/********************************************
 * Virtual base class for integer matrices. *
 ********************************************/

typedef any_matrix<int, Rcpp::IntegerVector> integer_matrix;

/* Simple integer matrix */

typedef simple_matrix<int, Rcpp::IntegerVector> simple_integer_matrix;

/* HDF5Matrix */

#ifdef BEACHMAT_USE_HDF5

typedef HDF5_matrix<int, Rcpp::IntegerVector, H5T_INTEGER, H5::PredType::NATIVE_INT32> HDF5_integer_matrix;

#endif

/* Dispatcher */

std::unique_ptr<integer_matrix> create_integer_matrix(const Rcpp::RObject&);

/***************************************************
 * Virtual base class for output integer matrices. *
 ***************************************************/

typedef output_matrix<int, Rcpp::IntegerVector> integer_output;

/* Simple output integer matrix */

typedef simple_output<int, Rcpp::IntegerVector> simple_integer_output;

/* HDF5 output integer matrix */

#ifdef BEACHMAT_USE_HDF5

typedef HDF5_output<int, Rcpp::IntegerVector, H5::PredType::NATIVE_INT32, integer_zero> HDF5_integer_output;

#endif

/* Output dispatchers */

std::unique_ptr<integer_output> create_integer_output(int, int, bool=true);

std::unique_ptr<integer_output> create_integer_output(int, int, const Rcpp::RObject&, bool);

}

#endif
