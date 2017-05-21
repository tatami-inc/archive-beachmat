#ifndef BEACHMAT_LOGICAL_MATRIX_H
#define BEACHMAT_LOGICAL_MATRIX_H

#include "LIN_matrix.h"
#include "Output_matrix.h"

namespace beachmat {

/********************************************
 * Virtual base class for logical matrices. *
 ********************************************/

typedef lin_matrix<int> logical_matrix;

/* Simple logical matrix */

typedef simple_lin_matrix<int, Rcpp::LogicalVector> simple_logical_matrix;

/* lgeMatrix */

typedef dense_lin_matrix<int, Rcpp::LogicalVector> dense_logical_matrix;

/* lgCMatrix */

class Csparse_logical_matrix : public Csparse_lin_matrix<int, Rcpp::LogicalVector> {
public:
    Csparse_logical_matrix(const Rcpp::RObject&);
    ~Csparse_logical_matrix();
    std::unique_ptr<logical_matrix> clone() const;
};

/* lspMatrix */

typedef Psymm_lin_matrix<int, Rcpp::LogicalVector> Psymm_logical_matrix;

/* HDF5Matrix */

class HDF5_logical_matrix : public HDF5_lin_matrix<int> {
public:
    HDF5_logical_matrix(const Rcpp::RObject&);
    ~HDF5_logical_matrix();
    void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    std::unique_ptr<logical_matrix> clone() const;
};

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
