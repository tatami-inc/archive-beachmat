#ifndef MATRIX_H
#define MATRIX_H

#include "beachmat.h"
#include "utils.h"

#ifdef BEACHMAT_USE_HDF5
#include "H5Cpp.h"
#endif

namespace beachmat { 

/* Virtual base class for matrices. */

template<typename T, class V>
class any_matrix {
public:
    any_matrix();
    virtual ~any_matrix();

    /* Returns the number of rows in the matrix.
     *
     * @return Integer, number of rows.
     */
    int get_nrow() const;

    /* Returns the number of columns in the matrix.
     * 
     * @return Integer, number of columns.
     */
    int get_ncol() const;

    /* Fills an array with values in the specified row.
     *
     * @param r The row index (0-based).
     * @param out An iterator pointing to an Rcpp::Vector of T's of length 'ncol'.
     * @return Void. 'out' is filled with values in row 'r'.
     */
    void get_row(int, typename V::iterator);

    /* Fills an array with values in the specified row.
     *
     * @param r The row index (0-based).
     * @param out An iterator pointing to an Rcpp::Vector of T's of length 'end-start'.
     * @param start The index of the first column to store.
     * @param end One past the index of the last column to store. 
     * @return Void. 'out' is filled with values in row 'r', from 'start' to 'end-1'.
     */
    virtual void get_row(int, typename V::iterator, int, int)=0;

    /* Fills an array with values in the specified column.
     *
     * @param c The column index (0-based).
     * @param out An iterator pointing to an Rcpp::Vector of T's of length 'nrow'.
     * @return Void. 'out' is filled with values in column 'c'.
     */
    void get_col(int, typename V::iterator);

    /* Fills an array with values in the specified column.
     *
     * @param c The column index (0-based).
     * @param out An iterator pointing to an Rcpp::Vector of T's of length 'end-start'.
     * @param start The index of the first column to store.
     * @param end One past the index of the last column to store. 
     * @return Void. 'out' is filled with values in column 'c', from 'start' to 'end-1'.
     */
    virtual void get_col(int, typename V::iterator, int, int)=0;

    /* Returns values in the specified cell.
     *
     * @param r The row index (0-based).
     * @param c The column index (0-based).
     * @return T, the value of the matrix at row 'r' and column 'c'.
     */
    virtual T get(int, int)=0;

protected:
    int nrow, ncol;
    void fill_dims(const Rcpp::RObject&);
};

/* Simple matrix */

template<typename T, class V>
class simple_matrix : public any_matrix<T, V> {
public:    
    simple_matrix(const Rcpp::RObject&);
    ~simple_matrix();

    T get(int, int);
    void get_row(int, typename V::iterator, int, int);
    void get_col(int, typename V::iterator, int, int);
protected:
    int get_index(int, int) const;   
    V mat;
};

/* dense Matrix */

template<typename T, class V>
class dense_matrix : public any_matrix<T, V> {
public:    
    dense_matrix(const Rcpp::RObject&);
    ~dense_matrix();

    T get(int, int);
    void get_row(int, typename V::iterator, int, int);
    void get_col(int, typename V::iterator, int, int);
protected:
    int get_index(int, int) const;   
    V x;
};

/* column-major sparse Matrix */

template<typename T, class V, const T& Z>
class Csparse_matrix : public any_matrix<T, V> {
public:    
    Csparse_matrix(const Rcpp::RObject&);
    ~Csparse_matrix();

    T get(int, int);
    void get_row(int, typename V::iterator, int, int);
    void get_col(int, typename V::iterator, int, int);
protected:
    Rcpp::IntegerVector i, p;
    V x;
    int nx;
    int get_index(int, int) const;

    int currow, curstart, curend;
    std::vector<int> indices;
    void update_indices(int, int, int);
};

/* symmetric packed Matrix */

template<typename T, class V>
class Psymm_matrix : public any_matrix<T, V> {
public:    
    Psymm_matrix(const Rcpp::RObject&);
    ~Psymm_matrix();

    T get(int, int);   
    void get_row(int, typename V::iterator, int, int);
    void get_col(int, typename V::iterator, int, int);
protected:
    V x;
    bool upper;
    int get_index(int, int) const;
};

/* HDF5Matrix */

#ifdef BEACHMAT_USE_HDF5

template<typename T, class V, H5T_class_t HTC, const H5::PredType& HPT>
class HDF5_matrix : public any_matrix<T, V> {
public:
    HDF5_matrix(const Rcpp::RObject&);
    ~HDF5_matrix();

    T get(int, int);
    void get_row(int, typename V::iterator, int, int);
    void get_col(int, typename V::iterator, int, int);
protected:
    H5::H5File hfile;
    H5::DataSet hdata;
    H5::DataSpace hspace, rowspace, colspace, onespace;
    hsize_t h5_start[2], col_count[2], row_count[2], one_count[2], zero_start[1];

    void select_row(int, int, int);
    void select_col(int, int, int);
    void select_one(int, int);
};

#endif

#include "template_methods.h"

}

#endif

