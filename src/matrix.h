#ifndef BEACHMAT_MATRIX_H
#define BEACHMAT_MATRIX_H

#include "beachmat.h"
#include "utils.h"

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
    size_t get_nrow() const;

    /* Returns the number of columns in the matrix.
     * 
     * @return Integer, number of columns.
     */
    size_t get_ncol() const;

    /* Fills an array with values in the specified row.
     *
     * @param r The row index (0-based).
     * @param out An iterator pointing to an Rcpp::Vector of T's of length 'ncol'.
     * @return Void. 'out' is filled with values in row 'r'.
     */
    void get_row(size_t, typename V::iterator);

    /* Fills an array with values in the specified row.
     *
     * @param r The row index (0-based).
     * @param out An iterator pointing to an Rcpp::Vector of T's of length 'end-start'.
     * @param start The index of the first column to store.
     * @param end One past the index of the last column to store. 
     * @return Void. 'out' is filled with values in row 'r', from 'start' to 'end-1'.
     */
    virtual void get_row(size_t, typename V::iterator, size_t, size_t)=0;

    /* Fills an array with values in the specified column.
     *
     * @param c The column index (0-based).
     * @param out An iterator pointing to an Rcpp::Vector of T's of length 'nrow'.
     * @return Void. 'out' is filled with values in column 'c'.
     */
    void get_col(size_t, typename V::iterator);

    /* Fills an array with values in the specified column.
     *
     * @param c The column index (0-based).
     * @param out An iterator pointing to an Rcpp::Vector of T's of length 'end-start'.
     * @param start The index of the first column to store.
     * @param end One past the index of the last column to store. 
     * @return Void. 'out' is filled with values in column 'c', from 'start' to 'end-1'.
     */
    virtual void get_col(size_t, typename V::iterator, size_t, size_t)=0;

    /* Returns values in the specified cell.
     *
     * @param r The row index (0-based).
     * @param c The column index (0-based).
     * @return T, the value of the matrix at row 'r' and column 'c'.
     */
    virtual T get(size_t, size_t)=0;

    /* Returns a deep copy of itself. 
     *
     * @ return A unique pointer to a deep copy of itself.
     */
    virtual std::unique_ptr<any_matrix<T, V> > clone() const=0;

protected:
    size_t nrow, ncol;
    void fill_dims(const Rcpp::RObject&);
};

/* Simple matrix */

template<typename T, class V>
class simple_matrix : public any_matrix<T, V> {
public:    
    simple_matrix(const Rcpp::RObject&);
    ~simple_matrix();
    std::unique_ptr<any_matrix<T, V> > clone() const;

    T get(size_t, size_t);
    void get_row(size_t, typename V::iterator, size_t, size_t);
    void get_col(size_t, typename V::iterator, size_t, size_t);
protected:
    size_t get_index(size_t, size_t) const;   
    V mat;
};

/* dense Matrix */

template<typename T, class V>
class dense_matrix : public any_matrix<T, V> {
public:    
    dense_matrix(const Rcpp::RObject&);
    ~dense_matrix();
    std::unique_ptr<any_matrix<T, V> > clone() const;

    T get(size_t, size_t);
    void get_row(size_t, typename V::iterator, size_t, size_t);
    void get_col(size_t, typename V::iterator, size_t, size_t);
protected:
    size_t get_index(size_t, size_t) const;   
    V x;
};

/* column-major sparse Matrix */

template<typename T, class V, const T& Z>
class Csparse_matrix : public any_matrix<T, V> {
public:    
    Csparse_matrix(const Rcpp::RObject&);
    ~Csparse_matrix();
    std::unique_ptr<any_matrix<T, V> > clone() const;

    T get(size_t, size_t);
    void get_row(size_t, typename V::iterator, size_t, size_t);
    void get_col(size_t, typename V::iterator, size_t, size_t);
protected:
    Rcpp::IntegerVector i, p;
    V x;
    size_t nx;
    size_t get_index(size_t, size_t) const;

    size_t currow, curstart, curend;
    std::vector<int> indices; // Left as 'int' to simplify comparisons with 'i' and 'p'.
    void update_indices(size_t, size_t, size_t);
};

/* symmetric packed Matrix */

template<typename T, class V>
class Psymm_matrix : public any_matrix<T, V> {
public:    
    Psymm_matrix(const Rcpp::RObject&);
    ~Psymm_matrix();
    std::unique_ptr<any_matrix<T, V> > clone() const;

    T get(size_t, size_t);   
    void get_row(size_t, typename V::iterator, size_t, size_t);
    void get_col(size_t, typename V::iterator, size_t, size_t);
protected:
    V x;
    bool upper;
    size_t get_index(size_t, size_t) const;
};

/* HDF5Matrix */

#ifdef BEACHMAT_USE_HDF5

template<typename T, class V>
class HDF5_matrix : public any_matrix<T, V> {
public:
    HDF5_matrix(const Rcpp::RObject&, const H5T_class_t&, const H5::PredType&);
    ~HDF5_matrix();
    std::unique_ptr<any_matrix<T, V> > clone() const;

    T get(size_t, size_t);
    void get_row(size_t, typename V::iterator, size_t, size_t);
    void get_col(size_t, typename V::iterator, size_t, size_t);
protected:
    Rcpp::RObject realized;

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

#include "template_methods.h"

}

#endif

