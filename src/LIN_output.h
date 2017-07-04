#ifndef BEACHMAT_LIN_OUTPUT_H
#define BEACHMAT_LIN_OUTPUT_H

#include "Output_matrix.h"

namespace beachmat { 

/************************************************************************
 * Virtual base class for LIN (logical/integer/numeric) output matrices.
 ************************************************************************/

template<typename T>
class lin_output {
public:
    lin_output();
    virtual ~lin_output();
    
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

    void fill_row(size_t, Rcpp::IntegerVector::iterator);
    void fill_row(size_t, Rcpp::NumericVector::iterator);

    virtual void fill_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t)=0;
    virtual void fill_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t)=0;

    void fill_col(size_t, Rcpp::IntegerVector::iterator);
    void fill_col(size_t, Rcpp::NumericVector::iterator);

    virtual void fill_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t)=0;
    virtual void fill_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t)=0;

    virtual void fill(size_t, size_t, T)=0;

    virtual Rcpp::RObject yield()=0;

    virtual std::unique_ptr<lin_output<T> > clone() const=0;

    virtual matrix_type get_matrix_type() const=0;
};

/* Simple LIN output */

template<typename T, class V>
class simple_lin_output : public lin_output<T> {
public:
    simple_lin_output(size_t, size_t);
    ~simple_lin_output();

    size_t get_nrow() const;
    size_t get_ncol() const;

    void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    T get(size_t, size_t);

    void fill_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void fill_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void fill_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void fill_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void fill(size_t, size_t, T);

    Rcpp::RObject yield();

    std::unique_ptr<lin_output<T> > clone() const;

    matrix_type get_matrix_type() const;
private:
    simple_output<T, V> mat;
};

/* Sparse LIN output */

template<typename T, class V>
class sparse_lin_output : public lin_output<T> {
public:
    sparse_lin_output(size_t, size_t);
    ~sparse_lin_output();

    size_t get_nrow() const;
    size_t get_ncol() const;

    void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    T get(size_t, size_t);

    void fill_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void fill_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void fill_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void fill_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void fill(size_t, size_t, T);

    Rcpp::RObject yield();

    std::unique_ptr<lin_output<T> > clone() const;

    matrix_type get_matrix_type() const;
private:
    Csparse_output<T, V> mat;
};

/* HDF5 LIN output */

template<typename T, int RTYPE>
class HDF5_lin_output : public lin_output<T> {
public:
    HDF5_lin_output(size_t, size_t);
    HDF5_lin_output(size_t, size_t, size_t, size_t, int);
    ~HDF5_lin_output();

    size_t get_nrow() const;
    size_t get_ncol() const;

    void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    T get(size_t, size_t);

    void fill_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void fill_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void fill_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void fill_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void fill(size_t, size_t, T);

    Rcpp::RObject yield();

    std::unique_ptr<lin_output<T> > clone() const;

    matrix_type get_matrix_type() const;
protected:
    HDF5_output<T, RTYPE> mat;
};

#include "LIN_outfun.h"

}

#endif

