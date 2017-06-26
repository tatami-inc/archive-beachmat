#ifndef BEACHMAT_INPUT_MATRIX_H
#define BEACHMAT_INPUT_MATRIX_H

#include "beachmat.h"
#include "any_matrix.h"
#include "utils.h"

namespace beachmat { 

/* Simple matrix */

template<typename T, class V>
class simple_matrix : public any_matrix {
public:    
    simple_matrix(const Rcpp::RObject&);
    ~simple_matrix();

    T get(size_t, size_t);

    template <class Iter>
    void get_row(size_t, Iter, size_t, size_t);

    template <class Iter>
    void get_col(size_t, Iter, size_t, size_t);

    typename V::iterator get_const_col(size_t, typename V::iterator, size_t, size_t);
private:
    V mat;
};

/* dense Matrix */

template<typename T, class V>
class dense_matrix : public any_matrix {
public:    
    dense_matrix(const Rcpp::RObject&);
    ~dense_matrix();

    T get(size_t, size_t);

    template <class Iter>
    void get_row(size_t, Iter, size_t, size_t);

    template <class Iter>
    void get_col(size_t, Iter, size_t, size_t);

    typename V::iterator get_const_col(size_t, typename V::iterator, size_t, size_t);
protected:
    V x;
};

/* column-major sparse Matrix */

template<typename T, class V>
class Csparse_matrix : public any_matrix {
public:    
    Csparse_matrix(const Rcpp::RObject&);
    ~Csparse_matrix();

    T get(size_t, size_t);

    template <class Iter>
    void get_row(size_t, Iter, size_t, size_t);

    template <class Iter>
    void get_col(size_t, Iter, size_t, size_t);

    typename V::iterator get_const_col(size_t, typename V::iterator, size_t, size_t);
protected:
    Rcpp::IntegerVector i, p;
    V x;

    size_t currow, curstart, curend;
    std::vector<int> indices; // Left as 'int' to simplify comparisons with 'i' and 'p'.
    void update_indices(size_t, size_t, size_t);

    T get_empty() const; // Specialized function for each realization (easy to extend for non-int/double).
};

/* symmetric packed Matrix */

template<typename T, class V>
class Psymm_matrix : public any_matrix {
public:    
    Psymm_matrix(const Rcpp::RObject&);
    ~Psymm_matrix();

    T get(size_t, size_t);   

    template <class Iter>
    void get_row(size_t, Iter, size_t, size_t);

    template <class Iter>
    void get_col(size_t, Iter, size_t, size_t);

    typename V::iterator get_const_col(size_t, typename V::iterator, size_t, size_t);
protected:
    V x;
    bool upper;

    template <class Iter>
    void get_rowcol(size_t, Iter, size_t, size_t);

    size_t get_index(size_t, size_t) const;
};

/* Run length encoding-based matrix */

#include "Rle_matrix.h"

/* HDF5Matrix */

template<typename T, int RTYPE>
class HDF5_matrix : public any_matrix {
public:
    HDF5_matrix(const Rcpp::RObject&);
    ~HDF5_matrix();

    void extract_row(size_t, T*, size_t, size_t);
    template<typename X>
    void extract_row(size_t, X*, const H5::DataType&, size_t, size_t);

    void extract_col(size_t, T*, size_t, size_t);
    template<typename X>
    void extract_col(size_t, X*, const H5::DataType&, size_t, size_t);
    
    void extract_one(size_t, size_t, T*); // Use of pointer is a bit circuitous, but necessary for character access.
    template<typename X>
    void extract_one(size_t, size_t, X*, const H5::DataType&);  

    const H5::DataType& get_datatype() const;
protected:
    std::string filename, dataname;

    H5::H5File hfile;
    H5::DataSet hdata;
    H5::DataSpace hspace, rowspace, colspace, onespace;
    hsize_t h5_start[2], col_count[2], row_count[2], one_count[2], zero_start[1];

    H5::DataType default_type;
    H5T_class_t set_types();

    bool onrow, oncol;
    bool rowokay, colokay;
    bool largerrow, largercol;
    H5::FileAccPropList rowlist, collist;
};

#include "Input_methods.h"

}

#endif

