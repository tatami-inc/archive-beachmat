#ifndef NUMERIC_MATRIX_H
#define NUMERIC_MATRIX_H

#include "matrix.h"

/* Virtual base class for numeric matrices. */

typedef any_matrix<double> numeric_matrix;

/* Simple numeric matrix */

typedef simple_matrix<double, Rcpp::NumericVector> simple_numeric_matrix;

/* dgeMatrix */

typedef dense_matrix<double, Rcpp::NumericVector> dense_numeric_matrix;

/* dgCMatrix */

extern constexpr double numeric_zero=0;
typedef Csparse_matrix<double, Rcpp::NumericVector, numeric_zero> Csparse_numeric_matrix;

/* dspMatrix */

typedef Psymm_matrix<double, Rcpp::NumericVector> Psymm_numeric_matrix;

/* HDF5Matrix */

#ifdef BEACHMAT_USE_HDF5

class HDF5_numeric_matrix : public HDF5_matrix<double, H5::PredType::NATIVE_DOUBLE> { 
public:
    HDF5_numeric_matrix(const Rcpp::RObject&);
    ~HDF5_numeric_matrix();
};

#endif

/* Dispatcher */

std::shared_ptr<numeric_matrix> create_numeric_matrix(const Rcpp::RObject&);

#endif
