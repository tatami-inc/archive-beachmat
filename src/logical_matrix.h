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

typedef Csparse_lin_matrix<int, Rcpp::LogicalVector> Csparse_logical_matrix;

/* lspMatrix */

typedef Psymm_lin_matrix<int, Rcpp::LogicalVector> Psymm_logical_matrix;

/* HDF5Matrix */

typedef HDF5_lin_matrix<int, LGLSXP> HDF5_logical_matrix;

/* Dispatcher */

std::unique_ptr<logical_matrix> create_logical_matrix(const Rcpp::RObject&);

/***************************************************
 * Virtual base class for output logical matrices. *
 ***************************************************/

typedef lin_output<int> logical_output;

/* Simple output logical matrix */

typedef simple_lin_output<int, Rcpp::LogicalVector> simple_logical_output;

/* HDF5 output logical matrix */

class HDF5_logical_output : public HDF5_lin_output<int, Rcpp::LogicalVector> {
public:
    HDF5_logical_output(int, int);
    ~HDF5_logical_output();
    void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void fill_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void fill_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    std::unique_ptr<logical_output> clone() const;
};

/* Output dispatchers */

std::unique_ptr<logical_output> create_logical_output(int, int, bool=true);

std::unique_ptr<logical_output> create_logical_output(int, int, const Rcpp::RObject&, bool);

}

#endif
