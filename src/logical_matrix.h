#ifndef LOGICAL_MATRIX_H
#define LOGICAL_MATRIX_H

#include "matrix.h"

/* Virtual base class for logical matrices. */

typedef any_matrix<int> logical_matrix;

/* Simple logical matrix */

typedef simple_matrix<int, Rcpp::LogicalVector> simple_logical_matrix;

/* lgeMatrix */

typedef dense_matrix<int, Rcpp::LogicalVector> dense_logical_matrix;

/* lgCMatrix */

extern constexpr int logical_false=0;
typedef Csparse_matrix<int, Rcpp::LogicalVector, logical_false> Csparse_logical_matrix;

/* lspMatrix */

typedef Psymm_matrix<int, Rcpp::LogicalVector> Psymm_logical_matrix;

/* HDF5Matrix */

#ifdef BEACHMAT_USE_HDF5

class HDF5_logical_matrix : public HDF5_matrix<int, H5::PredType::NATIVE_INT32> {
public:
    HDF5_logical_matrix(const Rcpp::RObject&);
    ~HDF5_logical_matrix();
};

#endif

/* Dispatcher */

std::shared_ptr<logical_matrix> create_logical_matrix(const Rcpp::RObject&);

#endif
