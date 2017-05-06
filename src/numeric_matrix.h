#ifndef BEACHMAT_NUMERIC_MATRIX_H
#define BEACHMAT_NUMERIC_MATRIX_H

#include "matrix.h"
#include "output.h"

namespace beachmat { 

/* Virtual base class for numeric matrices. */

typedef any_matrix<double, Rcpp::NumericVector> numeric_matrix;

/* Simple numeric matrix */

typedef simple_matrix<double, Rcpp::NumericVector> simple_numeric_matrix;

/* dgeMatrix */

typedef dense_matrix<double, Rcpp::NumericVector> dense_numeric_matrix;

/* dgCMatrix */

typedef Csparse_matrix<double, Rcpp::NumericVector, numeric_zero> Csparse_numeric_matrix;

/* dspMatrix */

typedef Psymm_matrix<double, Rcpp::NumericVector> Psymm_numeric_matrix;

/* HDF5Matrix */

#ifdef BEACHMAT_USE_HDF5

typedef HDF5_matrix<double, Rcpp::NumericVector, H5T_FLOAT, H5::PredType::NATIVE_DOUBLE> HDF5_numeric_matrix; 

#endif

/* Dispatcher */

std::unique_ptr<numeric_matrix> create_numeric_matrix(const Rcpp::RObject&);

/***************************************************
 * Virtual base class for output numeric matrices. *
 ***************************************************/

typedef output_matrix<double, Rcpp::NumericVector> numeric_output;

/* Simple output numeric matrix */

typedef simple_output<double, Rcpp::NumericVector> simple_numeric_output;

/* HDF5 output numeric matrix */

#ifdef BEACHMAT_USE_HDF5

typedef HDF5_output<double, Rcpp::NumericVector, H5::PredType::NATIVE_DOUBLE, numeric_zero> HDF5_numeric_output;

#endif

/* Output dispatchers */

std::unique_ptr<numeric_output> create_numeric_output(int, int, bool=true);

std::unique_ptr<numeric_output> create_numeric_output(int, int, const Rcpp::RObject&, bool);

}

#endif
