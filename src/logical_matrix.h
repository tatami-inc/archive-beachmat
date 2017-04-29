#ifndef LOGICAL_MATRIX_H
#define LOGICAL_MATRIX_H

#include "matrix.h"

/* Virtual base class for logical matrices. */

typedef any_matrix<int> logical_matrix;

/* Simple logical matrix */

class simple_logical_matrix : public simple_matrix<int> {
public:    
    simple_logical_matrix(const Rcpp::RObject&);
    ~simple_logical_matrix();
};

/* lgeMatrix */

class dense_logical_matrix : public dense_matrix<int> {
public:    
    dense_logical_matrix(const Rcpp::RObject&);
    ~dense_logical_matrix();
};

/* lgCMatrix */

extern constexpr int logical_false=0;

class Csparse_logical_matrix : public Csparse_matrix<int, logical_false> {
public:    
    Csparse_logical_matrix(const Rcpp::RObject&);
    ~Csparse_logical_matrix();
};

/* lspMatrix */

class Psymm_logical_matrix : public Psymm_matrix<int> {
public:    
    Psymm_logical_matrix(const Rcpp::RObject&);
    ~Psymm_logical_matrix();
};

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
