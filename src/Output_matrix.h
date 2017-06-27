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

    matrix_type get_matrix_type() const;
private:
    V data;    
};

template<typename T, class V>
class sparse_output : public any_matrix {
public:
    sparse_output(size_t, size_t);
    ~sparse_output();

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

    matrix_type get_matrix_type() const;
private:
    typedef std::pair<size_t, T> data_pair;
    std::vector<std::deque<data_pair> > data;

    T get_empty() const;
    template <class Iter>
    Iter find_matching_row(Iter, Iter, const data_pair&);
};


template<typename T, int RTYPE>
class HDF5_output : public any_matrix {
public:
    HDF5_output(size_t, size_t, size_t=0, size_t=0, int=-1);
    ~HDF5_output();
    
    void insert_row(size_t, const T*, size_t, size_t);
    template<typename X>
    void insert_row(size_t, const X*, const H5::DataType&, size_t, size_t);

    void insert_col(size_t, const T*, size_t, size_t);
    template<typename X>
    void insert_col(size_t, const X*, const H5::DataType&, size_t, size_t);

    void insert_one(size_t, size_t, T);

    void extract_col(size_t, T*, size_t, size_t);
    template<typename X>
    void extract_col(size_t, X*, const H5::DataType&, size_t, size_t);

    void extract_row(size_t, T*, size_t, size_t);
    template<typename X>
    void extract_row(size_t, X*, const H5::DataType&, size_t, size_t);

    T extract_one(size_t, size_t);

    Rcpp::RObject yield();

    matrix_type get_matrix_type() const;
protected:
    std::string fname, dname;

    H5::H5File hfile;
    H5::DataSet hdata;
    H5::DataSpace hspace, rowspace, colspace, onespace;
    hsize_t h5_start[2], col_count[2], row_count[2], one_count[2], zero_start[1];

    H5::DataType default_type;
    void select_row(size_t, size_t, size_t);
    void select_col(size_t, size_t, size_t);
    void select_one(size_t, size_t);

    T get_empty() const;

    bool onrow, oncol;
    bool rowokay, colokay;
    bool largerrow, largercol;
    H5::FileAccPropList rowlist, collist;
};

#include "Output_methods.h"

}

#endif
