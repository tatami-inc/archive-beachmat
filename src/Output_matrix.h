#ifndef BEACHMAT_OUTPUT_MATRIX_H
#define BEACHMAT_OUTPUT_MATRIX_H

#include "beachmat.h"
#include "utils.h"

namespace beachmat {

template<typename T, class V>
class output_matrix {
public:
    output_matrix(size_t, size_t);
    virtual ~output_matrix();

    void fill_row(size_t, typename V::iterator);
    void fill_col(size_t, typename V::iterator);
    virtual void fill_row(size_t, typename V::iterator, size_t, size_t)=0;
    virtual void fill_col(size_t, typename V::iterator, size_t, size_t)=0;
    virtual void fill(size_t, size_t, T)=0;
    virtual Rcpp::RObject yield()=0;

    // Methods copied from any_matrix.
    size_t get_nrow() const;
    size_t get_ncol() const;
    void get_row(size_t, typename V::iterator);
    void get_col(size_t, typename V::iterator);
    virtual void get_col(size_t, typename V::iterator, size_t, size_t)=0;
    virtual void get_row(size_t, typename V::iterator, size_t, size_t)=0;
    virtual T get(size_t, size_t)=0;
    virtual std::unique_ptr<output_matrix<T, V> > clone() const=0;
protected:
    size_t nrow, ncol;
};

template<typename T, class V>
class simple_output : public output_matrix<T, V> {
public:
    simple_output(size_t, size_t);
    ~simple_output();

    void fill_row(size_t, typename V::iterator, size_t, size_t);
    void fill_col(size_t, typename V::iterator, size_t, size_t);
    void fill(size_t, size_t, T);
    Rcpp::RObject yield();

    void get_col(size_t, typename V::iterator, size_t, size_t);
    void get_row(size_t, typename V::iterator, size_t, size_t);
    T get(size_t, size_t);
    std::unique_ptr<output_matrix<T, V> > clone() const;
private:
    V data;    
};

#ifdef BEACHMAT_USE_HDF5

template<typename T, class V, const T& FILL>
class HDF5_output : public output_matrix<T, V> {
public:
    HDF5_output(size_t, size_t, const H5::PredType&);
    ~HDF5_output();
    
    void fill_row(size_t, typename V::iterator, size_t, size_t);
    void fill_col(size_t, typename V::iterator, size_t, size_t);
    void fill(size_t, size_t, T);
    Rcpp::RObject yield();

    void get_col(size_t, typename V::iterator, size_t, size_t);
    void get_row(size_t, typename V::iterator, size_t, size_t);
    T get(size_t, size_t);
    std::unique_ptr<output_matrix<T, V> > clone() const;
protected:
    std::string fname, dname;

    H5::H5File hfile;
    H5::DataSet hdata;
    H5::DataSpace hspace, rowspace, colspace, onespace;
    hsize_t h5_start[2], col_count[2], row_count[2], one_count[2], zero_start[1];
   
    const H5::PredType& HPT; 
    void select_row(size_t, size_t, size_t);
    void select_col(size_t, size_t, size_t);
    void select_one(size_t, size_t);
};

#endif

#include "Output_methods.h"

}

#endif
