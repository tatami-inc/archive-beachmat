#ifndef BEACHMAT_LIN_MATRIX_H
#define BEACHMAT_LIN_MATRIX_H

#include "Input_matrix.h"
#include "Output_matrix.h"

namespace beachmat { 

/***************************************************************** 
 * Virtual base class for LIN (logical/integer/numeric) matrices. 
 *****************************************************************/

template<typename T>
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
    Csparse_lin_matrix(const Rcpp::RObject&);
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

template<typename T, int RTYPE>
class HDF5_lin_matrix : public lin_matrix<T> {
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

    std::unique_ptr<lin_matrix<T> > clone() const;
protected:
    HDF5_matrix<T, RTYPE> mat;
    std::vector<T> rowtmp, coltmp;
};

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
private:
    sparse_output<T, V> mat;
};

/* HDF5 LIN output */

template<typename T, class V>
class HDF5_lin_output : public lin_output<T> {
public:
    HDF5_lin_output(size_t, size_t);
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
protected:
    HDF5_output<T, V> mat;
    std::vector<T> rowtmp, coltmp;
};

#include "LIN_methods.h"

}

#endif

