#ifndef BEACHMAT_OUTPUT_H
#define BEACHMAT_OUTPUT_H

#include "matrix.h"

namespace beachmat {

template<typename T, class V>
class output_matrix {
public:
    output_matrix(int, int);
    virtual ~output_matrix();
    output_matrix(output_matrix<T, V> const&) = delete;
    output_matrix<T, V>& operator=(output_matrix<T, V> const&) = delete;

    void fill_row(int, typename V::iterator);
    void fill_col(int, typename V::iterator);
    virtual void fill_row(int, typename V::iterator, int, int)=0;
    virtual void fill_col(int, typename V::iterator, int, int)=0;
    virtual void fill(int, int, T)=0;
    virtual Rcpp::RObject yield()=0;

    // Methods copied from any_matrix.
    int get_nrow() const;
    int get_ncol() const;
    void get_row(int, typename V::iterator);
    void get_col(int, typename V::iterator);
    virtual void get_col(int, typename V::iterator, int, int)=0;
    virtual void get_row(int, typename V::iterator, int, int)=0;
    virtual T get(int, int)=0;
protected:
    int nrow, ncol;
};

template<typename T, class V>
class simple_output : public output_matrix<T, V> {
public:
    simple_output(int, int);
    ~simple_output();

    void fill_row(int, typename V::iterator, int, int);
    void fill_col(int, typename V::iterator, int, int);
    void fill(int, int, T);
    Rcpp::RObject yield();

    void get_col(int, typename V::iterator, int, int);
    void get_row(int, typename V::iterator, int, int);
    T get(int, int);
private:
    V data;    
};

template<typename T, class V, const H5::PredType& HPT, const T& FILL>
class HDF5_output : public output_matrix<T, V> {
public:
    HDF5_output(int, int);
    ~HDF5_output();
    
    void fill_row(int, typename V::iterator, int, int);
    void fill_col(int, typename V::iterator, int, int);
    void fill(int, int, T);
    Rcpp::RObject yield();

    void get_col(int, typename V::iterator, int, int);
    void get_row(int, typename V::iterator, int, int);
    T get(int, int);
protected:
    std::string fname, dname;

    H5::H5File hfile;
    H5::DataSet hdata;
    H5::DataSpace hspace, rowspace, colspace, onespace;
    hsize_t h5_start[2], col_count[2], row_count[2], one_count[2], zero_start[1];
    
    void select_row(int, int, int);
    void select_col(int, int, int);
    void select_one(int, int);
};

#include "template_output.h"

}

#endif
