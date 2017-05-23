#ifndef BEACHMAT_OUTPUT_MATRIX_H
#define BEACHMAT_OUTPUT_MATRIX_H

#include "beachmat.h"
#include "utils.h"

namespace beachmat {

template<typename T, class V>
class simple_output : public any_matrix {
public:
    simple_output(size_t, size_t);
    ~simple_output();

    template <class Iter>
    void fill_row(size_t, Iter, size_t, size_t);
    template <class Iter>
    void fill_col(size_t, Iter, size_t, size_t);
    void fill(size_t, size_t, T);

    template <class Iter>
    void get_col(size_t, Iter, size_t, size_t);
    template <class Iter>
    void get_row(size_t, Iter, size_t, size_t);
    T get(size_t, size_t);

    Rcpp::RObject yield();
private:
    V data;    
};

template<typename T, class V>
class HDF5_output : public any_matrix {
public:
    HDF5_output(size_t, size_t, const H5::DataType&, T);
    ~HDF5_output();
    
    void fill_row(size_t, T*, size_t, size_t);
    void fill_col(size_t, T*, size_t, size_t);
    void fill(size_t, size_t, T);

    void get_col(size_t, T*, size_t, size_t);
    void get_row(size_t, T*, size_t, size_t);
    T get(size_t, size_t);

    Rcpp::RObject yield();
protected:
    std::string fname, dname;

    H5::H5File hfile;
    H5::DataSet hdata;
    H5::DataSpace hspace, rowspace, colspace, onespace;
    hsize_t h5_start[2], col_count[2], row_count[2], one_count[2], zero_start[1];

    H5::DataType HDT;
    const T FILL;
    void select_row(size_t, size_t, size_t);
    void select_col(size_t, size_t, size_t);
    void select_one(size_t, size_t);
};

#include "Output_methods.h"

}

#endif
