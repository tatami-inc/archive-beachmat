#ifndef BEACHMAT_LIN_MATRIX_H
#define BEACHMAT_LIN_MATRIX_H

#include "Input_matrix.h"

namespace beachmat { 

/* Virtual base class for LIN (logical/integer/numeric) matrices. */

template<typename T>
class lin_matrix {
public:
    lin_matrix();
    virtual ~lin_matrix();
    
    virtual size_t get_nrow() const=0;
    virtual size_t get_ncol() const=0;

    void get_row(size_t, Rcpp::IntegerVector::iterator);
    void get_row(size_t, Rcpp::NumericVector::iterator);

    virtual void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t)=0;
    virtual void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t)=0;

    void get_col(size_t, Rcpp::IntegerVector::iterator);
    void get_col(size_t, Rcpp::NumericVector::iterator);

    virtual void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t)=0;
    virtual void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t)=0;

    virtual T get(size_t, size_t)=0;

    virtual std::unique_ptr<lin_matrix<T> > clone() const=0;
};

/* Simple LIN matrix */

template<typename T, class V>
class simple_lin_matrix : public lin_matrix<T> {
public:    
    simple_lin_matrix(const Rcpp::RObject&);
    ~simple_lin_matrix();
    
    size_t get_nrow() const;
    size_t get_ncol() const;

    void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    T get(size_t, size_t);

    std::unique_ptr<lin_matrix<T> > clone() const;
private:
    simple_matrix<T, V> mat;
};

/* Dense LIN Matrix */

template<typename T, class V>
class dense_lin_matrix : public lin_matrix<T> {
public:    
    dense_lin_matrix(const Rcpp::RObject&);
    ~dense_lin_matrix();

    size_t get_nrow() const;
    size_t get_ncol() const;

    void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    T get(size_t, size_t);

    std::unique_ptr<lin_matrix<T> > clone() const;
private:
    dense_matrix<T, V> mat;
};

/* Column-major sparse LIN Matrix */

template<typename T, class V>
class Csparse_lin_matrix : public lin_matrix<T> {
public:    
    Csparse_lin_matrix(const Rcpp::RObject&, T);
    ~Csparse_lin_matrix();

    size_t get_nrow() const;
    size_t get_ncol() const;

    void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    T get(size_t, size_t);

    std::unique_ptr<lin_matrix<T> > clone() const;
private:
    Csparse_matrix<T, V> mat;
};

/* symmetric packed Matrix */

template<typename T, class V>
class Psymm_lin_matrix : public lin_matrix<T> {
public:    
    Psymm_lin_matrix(const Rcpp::RObject&);
    ~Psymm_lin_matrix();

    size_t get_nrow() const;
    size_t get_ncol() const;

    void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    T get(size_t, size_t);

    std::unique_ptr<lin_matrix<T> > clone() const;
private:
    Psymm_matrix<T, V> mat;
};

/* HDF5Matrix of LINs */

template<typename T>
class HDF5_lin_matrix : public lin_matrix<T> {
public:
    HDF5_lin_matrix(const Rcpp::RObject&, int, const H5T_class_t&, const H5::PredType&);
    ~HDF5_lin_matrix();

    size_t get_nrow() const;
    size_t get_ncol() const;

    void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    T get(size_t, size_t);

    std::unique_ptr<lin_matrix<T> > clone() const;
protected:
    HDF5_matrix<T> mat;
    std::vector<T> rowtmp, coltmp;
};

#include "LIN_methods.h"

}

#endif

