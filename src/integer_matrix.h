#ifndef BEACHMAT_INTEGER_MATRIX_H
#define BEACHMAT_INTEGER_MATRIX_H

#include "LIN_matrix.h"
#include "LIN_output.h"

namespace beachmat {

/********************************************
 * Virtual base class for integer matrices. *
 ********************************************/

typedef lin_matrix<int, Rcpp::IntegerVector> integer_matrix;

/* Simple integer matrix */

typedef simple_lin_matrix<int, Rcpp::IntegerVector> simple_integer_matrix;

/* RleMatrix */

typedef Rle_lin_matrix<int, Rcpp::IntegerVector> Rle_integer_matrix;

/* HDF5Matrix */

typedef HDF5_lin_matrix<int, Rcpp::IntegerVector, INTSXP> HDF5_integer_matrix;

/* Dispatcher */

std::unique_ptr<integer_matrix> create_integer_matrix(const Rcpp::RObject&);

/***************************************************
 * Virtual base class for output integer matrices. *
 ***************************************************/

typedef lin_output<int> integer_output;

/* Simple output integer matrix */

typedef simple_lin_output<int, Rcpp::IntegerVector> simple_integer_output;

/* HDF5 output integer matrix */

typedef HDF5_lin_output<int, INTSXP> HDF5_integer_output;

/* Output dispatchers */

std::unique_ptr<integer_output> create_integer_output(int, int, const output_param&);

}

#endif
