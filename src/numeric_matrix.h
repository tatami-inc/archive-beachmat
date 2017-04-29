#ifndef NUMERIC_MATRIX_H
#define NUMERIC_MATRIX_H

#include "matrix.h"

/* Virtual base class for numeric matrices. */

typedef any_matrix<double> numeric_matrix;

/* Simple numeric matrix */

class simple_numeric_matrix : public simple_matrix<double> {
public:    
    simple_numeric_matrix(const Rcpp::RObject&);
    ~simple_numeric_matrix();
};

/* dgeMatrix */

class dense_numeric_matrix : public dense_matrix<double> {
public:    
    dense_numeric_matrix(const Rcpp::RObject&);
    ~dense_numeric_matrix();
};

/* dgCMatrix */

extern constexpr double numeric_zero=0;

class Csparse_numeric_matrix : public Csparse_matrix<double, numeric_zero> {
public: 
    Csparse_numeric_matrix(const Rcpp::RObject&);
    ~Csparse_numeric_matrix();
};

/* dspMatrix */

class Psymm_numeric_matrix : public Psymm_matrix<double> {
public:    
    Psymm_numeric_matrix(const Rcpp::RObject&);
    ~Psymm_numeric_matrix();
};

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
