#ifndef BEACHMAT_LOGICAL_MATRIX_H
#define BEACHMAT_LOGICAL_MATRIX_H

#include "matrix.h"
#include "output.h"

namespace beachmat {

/********************************************
 * Virtual base class for logical matrices. *
 ********************************************/

typedef any_matrix<int, Rcpp::LogicalVector> logical_matrix;

/* Simple logical matrix */

typedef simple_matrix<int, Rcpp::LogicalVector> simple_logical_matrix;

/* lgeMatrix */

typedef dense_matrix<int, Rcpp::LogicalVector> dense_logical_matrix;

/* lgCMatrix */

typedef Csparse_matrix<int, Rcpp::LogicalVector, logical_false> Csparse_logical_matrix;

/* lspMatrix */

typedef Psymm_matrix<int, Rcpp::LogicalVector> Psymm_logical_matrix;

/* HDF5Matrix */

#ifdef BEACHMAT_USE_HDF5

class HDF5_logical_matrix : public HDF5_matrix<int, Rcpp::LogicalVector> {
public:
    HDF5_logical_matrix(const Rcpp::RObject&);
    ~HDF5_logical_matrix();
};

#endif

/* Dispatcher */

std::unique_ptr<logical_matrix> create_logical_matrix(const Rcpp::RObject&);

/***************************************************
 * Virtual base class for output logical matrices. *
 ***************************************************/

typedef output_matrix<int, Rcpp::LogicalVector> logical_output;

/* Simple output logical matrix */

typedef simple_output<int, Rcpp::LogicalVector> simple_logical_output;

/* HDF5 output logical matrix */

#ifdef BEACHMAT_USE_HDF5

class HDF5_logical_output : public HDF5_output<int, Rcpp::LogicalVector, logical_false> {
public:
    HDF5_logical_output(int, int);
    ~HDF5_logical_output();
};

#endif

/* Output dispatchers */

std::unique_ptr<logical_output> create_logical_output(int, int, bool=true);

std::unique_ptr<logical_output> create_logical_output(int, int, const Rcpp::RObject&, bool);

}

#endif
