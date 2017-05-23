#ifndef BEACHMAT_NUMERIC_MATRIX_H
#define BEACHMAT_NUMERIC_MATRIX_H

#include "LIN_matrix.h"
#include "Output_matrix.h"

namespace beachmat { 

/* Virtual base class for numeric matrices. */

typedef lin_matrix<double> numeric_matrix;

/* Simple numeric matrix */

typedef simple_lin_matrix<double, Rcpp::NumericVector> simple_numeric_matrix;

/* dgeMatrix */

typedef dense_lin_matrix<double, Rcpp::NumericVector> dense_numeric_matrix;

/* dgCMatrix */

typedef Csparse_lin_matrix<double, Rcpp::NumericVector> Csparse_numeric_matrix;

/* dspMatrix */

typedef Psymm_lin_matrix<double, Rcpp::NumericVector> Psymm_numeric_matrix;

/* HDF5Matrix */

typedef HDF5_lin_matrix<double, REALSXP> HDF5_numeric_matrix;

/* Dispatcher */

std::unique_ptr<numeric_matrix> create_numeric_matrix(const Rcpp::RObject&);

/***************************************************
 * Virtual base class for output numeric matrices. *
 ***************************************************/

typedef lin_output<double> numeric_output;

/* Simple output numeric matrix */

typedef simple_lin_output<double, Rcpp::NumericVector> simple_numeric_output;

/* HDF5 output numeric matrix */

class HDF5_numeric_output : public HDF5_lin_output<double, Rcpp::NumericVector> {
public:
    HDF5_numeric_output(int, int);
    ~HDF5_numeric_output();
    void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);
    void fill_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);
    void fill_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);
    std::unique_ptr<numeric_output> clone() const;

};

/* Output dispatchers */

std::unique_ptr<numeric_output> create_numeric_output(int, int, bool=true);

std::unique_ptr<numeric_output> create_numeric_output(int, int, const Rcpp::RObject&, bool);

}

#endif
