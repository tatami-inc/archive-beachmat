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

class Csparse_numeric_matrix : public Csparse_lin_matrix<double, Rcpp::NumericVector> {
public:
    Csparse_numeric_matrix(const Rcpp::RObject&);
    ~Csparse_numeric_matrix();
    std::unique_ptr<numeric_matrix> clone() const;
};

/* dspMatrix */

typedef Psymm_lin_matrix<double, Rcpp::NumericVector> Psymm_numeric_matrix;

/* HDF5Matrix */

class HDF5_numeric_matrix : public HDF5_lin_matrix<double> {
public:
    HDF5_numeric_matrix(const Rcpp::RObject&);
    ~HDF5_numeric_matrix();
    void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);
    std::unique_ptr<numeric_matrix> clone() const;
};

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

class HDF5_numeric_output : public HDF5_output<double, Rcpp::NumericVector, numeric_zero> {
public:
    HDF5_numeric_output(int, int);
    ~HDF5_numeric_output();
};

#endif

/* Output dispatchers */

std::unique_ptr<numeric_output> create_numeric_output(int, int, bool=true);

std::unique_ptr<numeric_output> create_numeric_output(int, int, const Rcpp::RObject&, bool);

}

#endif
