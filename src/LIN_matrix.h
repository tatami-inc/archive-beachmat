#ifndef BEACHMAT_LIN_MATRIX_H
#define BEACHMAT_LIN_MATRIX_H

#include "Input_matrix.h"

namespace beachmat { 

/***************************************************************** 
 * Virtual base class for LIN (logical/integer/numeric) matrices. 
 *****************************************************************/

template<typename T, class V>
class lin_matrix {
public:
    lin_matrix();
    virtual ~lin_matrix();
    
    virtual size_t get_nrow() const=0;
    virtual size_t get_ncol() const=0;

    void get_row(size_t, Rcpp::IntegerVector::iterator);
    void get_row(size_t, Rcpp::NumericVector::iterator);

    /* We can't add a LogicalVector::iterator method because IntegerVector::iterator==LogicalVector::iterator
     * under the hood in Rcpp. The compiler then complains that overloading is not possible. Thus, for all 
     * references here to LogicalVector, we will consider the use of IntegerVector in its place.
     */

    virtual void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t)=0;
    virtual void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t)=0;

    void get_col(size_t, Rcpp::IntegerVector::iterator);
    void get_col(size_t, Rcpp::NumericVector::iterator);

    virtual void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t)=0;
    virtual void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t)=0;

    virtual T get(size_t, size_t)=0;

    typename V::const_iterator get_const_col(size_t, typename V::iterator);
    virtual typename V::const_iterator get_const_col(size_t, typename V::iterator, size_t, size_t);

    size_t get_nonzero_col(size_t, Rcpp::IntegerVector::iterator, Rcpp::IntegerVector::iterator);
    size_t get_nonzero_col(size_t, Rcpp::IntegerVector::iterator, Rcpp::NumericVector::iterator);

    virtual size_t get_nonzero_col(size_t, Rcpp::IntegerVector::iterator, Rcpp::IntegerVector::iterator, size_t, size_t);
    virtual size_t get_nonzero_col(size_t, Rcpp::IntegerVector::iterator, Rcpp::NumericVector::iterator, size_t, size_t);

    size_t get_nonzero_row(size_t, Rcpp::IntegerVector::iterator, Rcpp::IntegerVector::iterator);
    size_t get_nonzero_row(size_t, Rcpp::IntegerVector::iterator, Rcpp::NumericVector::iterator);

    virtual size_t get_nonzero_row(size_t, Rcpp::IntegerVector::iterator, Rcpp::IntegerVector::iterator, size_t, size_t);
    virtual size_t get_nonzero_row(size_t, Rcpp::IntegerVector::iterator, Rcpp::NumericVector::iterator, size_t, size_t);

    virtual std::unique_ptr<lin_matrix<T, V> > clone() const=0;

    virtual matrix_type get_matrix_type() const=0;
};

/* Various flavours of a LIN matrix */

template <typename T, class V, class M>
class advanced_lin_matrix : public lin_matrix<T, V> {
public:    
    advanced_lin_matrix(const Rcpp::RObject&);
    ~advanced_lin_matrix();
    
    size_t get_nrow() const;
    size_t get_ncol() const;

    void get_col(size_t,  Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_col(size_t,  Rcpp::NumericVector::iterator, size_t, size_t);

    void get_row(size_t,  Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_row(size_t,  Rcpp::NumericVector::iterator, size_t, size_t);

    T get(size_t, size_t);

    std::unique_ptr<lin_matrix<T, V> > clone() const;

    matrix_type get_matrix_type() const;
protected:
    M mat;
};

template <typename T, class V>
class simple_lin_matrix : public advanced_lin_matrix<T, V, simple_matrix<T, V> > {
public:
    simple_lin_matrix(const Rcpp::RObject&);
    ~simple_lin_matrix();
    
    typename V::const_iterator get_const_col(size_t, typename V::iterator, size_t, size_t);

    std::unique_ptr<lin_matrix<T, V> > clone() const;
};

template <typename T, class V>
class dense_lin_matrix : public advanced_lin_matrix<T, V, dense_matrix<T, V> > {
public:
    dense_lin_matrix(const Rcpp::RObject&);
    ~dense_lin_matrix();
    
    typename V::const_iterator get_const_col(size_t, typename V::iterator, size_t, size_t);

    std::unique_ptr<lin_matrix<T, V> > clone() const;
};

template <typename T, class V>
class Csparse_lin_matrix : public advanced_lin_matrix<T, V, Csparse_matrix<T, V> > {
public:
    Csparse_lin_matrix(const Rcpp::RObject&);
    ~Csparse_lin_matrix();

    virtual size_t get_nonzero_col(size_t, Rcpp::IntegerVector::iterator, Rcpp::IntegerVector::iterator, size_t, size_t);
    virtual size_t get_nonzero_col(size_t, Rcpp::IntegerVector::iterator, Rcpp::NumericVector::iterator, size_t, size_t);

    virtual size_t get_nonzero_row(size_t, Rcpp::IntegerVector::iterator, Rcpp::IntegerVector::iterator, size_t, size_t);
    virtual size_t get_nonzero_row(size_t, Rcpp::IntegerVector::iterator, Rcpp::NumericVector::iterator, size_t, size_t);

    std::unique_ptr<lin_matrix<T, V> > clone() const;
};

template <typename T, class V>
using Psymm_lin_matrix=advanced_lin_matrix<T, V, Psymm_matrix<T, V> >;

template <typename T, class V>
using Rle_lin_matrix=advanced_lin_matrix<T, V, Rle_matrix<T, V> >;

/* HDF5Matrix of LINs */

template<typename T, class V, int RTYPE>
class HDF5_lin_matrix : public lin_matrix<T, V> {
public:
    HDF5_lin_matrix(const Rcpp::RObject&);
    ~HDF5_lin_matrix();

    size_t get_nrow() const;
    size_t get_ncol() const;

    void get_col(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    void get_row(size_t, Rcpp::IntegerVector::iterator, size_t, size_t);
    void get_row(size_t, Rcpp::NumericVector::iterator, size_t, size_t);

    T get(size_t, size_t);

    std::unique_ptr<lin_matrix<T, V> > clone() const;

    matrix_type get_matrix_type() const;
protected:
    HDF5_matrix<T, RTYPE> mat;
};

}

#include "LIN_methods.h"

#endif

