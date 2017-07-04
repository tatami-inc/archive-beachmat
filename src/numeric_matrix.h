#ifndef BEACHMAT_NUMERIC_MATRIX_H
#define BEACHMAT_NUMERIC_MATRIX_H

#include "LIN_matrix.h"
#include "LIN_output.h"

namespace beachmat { 

/* Virtual base class for numeric matrices. */

typedef lin_matrix<double, Rcpp::NumericVector> numeric_matrix;

/* Simple numeric matrix */

typedef simple_lin_matrix<double, Rcpp::NumericVector> simple_numeric_matrix;

/* dgeMatrix */

typedef dense_lin_matrix<double, Rcpp::NumericVector> dense_numeric_matrix;

/* dgCMatrix */

typedef Csparse_lin_matrix<double, Rcpp::NumericVector> Csparse_numeric_matrix;

/* dspMatrix */

typedef Psymm_lin_matrix<double, Rcpp::NumericVector> Psymm_numeric_matrix;

/* RleMatrix */

typedef Rle_lin_matrix<double, Rcpp::NumericVector> Rle_numeric_matrix;

/* HDF5Matrix */

typedef HDF5_lin_matrix<double, Rcpp::NumericVector, REALSXP> HDF5_numeric_matrix;

/* Dispatcher */

std::unique_ptr<numeric_matrix> create_numeric_matrix(const Rcpp::RObject&);

/***************************************************
 * Virtual base class for output numeric matrices. *
 ***************************************************/

typedef lin_output<double> numeric_output;

/* Simple output numeric matrix */

typedef simple_lin_output<double, Rcpp::NumericVector> simple_numeric_output;

/* Sparse output numeric matrix */

typedef sparse_lin_output<double, Rcpp::NumericVector> sparse_numeric_output;

/* HDF5 output numeric matrix */

typedef HDF5_lin_output<double, REALSXP> HDF5_numeric_output;

/* Output dispatchers */

std::unique_ptr<numeric_output> create_numeric_output(int, int, const output_param&);

}

#endif
