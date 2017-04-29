#ifndef MATRIX_H
#define MATRIX_H

#include "beachmat.h"
#include "utils.h"

#ifdef BEACHMAT_USE_HDF5
#include "H5Cpp.h"
#endif

/* Virtual base class for matrices. */

template<typename T>
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

    /* Returns values in the specified row.
     *
     * @param r The row index (0-based).
     * @return An array of T's of length 'ncol', containing all values in row 'r'.
     */
    virtual const T* get_row(int)=0;

    /* Fills an array with values in the specified row.
     *
     * @param r The row index (0-based).
     * @param out An array of T's of length 'nrow'.
     * @return Void. 'out' is filled with values in row 'r'.
     */
    virtual void fill_row(int, T*)=0;

    /* Returns values in the specified column.
     *
     * @param c The column index (0-based).
     * @return An array of T's of length 'nrow', containing all values in column 'c'.
     */
    virtual const T* get_col(int)=0;

    /* Fills an array with values in the specified column.
     *
     * @param c The column index (0-based).
     * @param out An array of T's of length 'nrow'.
     * @return Void. 'out' is filled with values in column 'c'.
     */
    virtual void fill_col(int, T*)=0;

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

/* If any class has pointers as data members, the data that each pointer points to should be 
 * contained within a RObject that is also a member of the class. This ensures that the 
 * data is PROTECTed for the lifetime of the class instance. Otherwise, one could imagine 
 * a situation where the class is instantiated from an RObject; the RObject is destroyed;
 * and garbage collection occurs, such that the pointers in the class instance are invalid.
 */

/* Simple matrix */

template<typename T>
class simple_matrix : public any_matrix<T> {
public:    
    simple_matrix(const Rcpp::RObject&);
    ~simple_matrix();

    const T* get_row(int);
    const T* get_col(int);
    T get(int, int);

    void fill_row(int, T*);
    void fill_col(int, T*);
protected:
    Rcpp::RObject obj;
    int get_index(int, int) const;   

    const T* simple_ptr;
    std::vector<T> row_data;
    T* row_ptr;
};

/* dense Matrix */

template<typename T>
class dense_matrix : public any_matrix<T> {
public:    
    dense_matrix(const Rcpp::RObject&);
    ~dense_matrix();

    const T* get_row(int);
    const T* get_col(int);
    T get(int, int);

    void fill_row(int, T*);
    void fill_col(int, T*);
protected:
    Rcpp::RObject obj_x;
    int get_index(int, int) const;   

    const T* dense_ptr;
    std::vector<T> row_data;
    T * row_ptr;
};

/* Column-major sparse Matrix */

template<typename T, const T& Z>
class Csparse_matrix : public any_matrix<T> {
public:    
    Csparse_matrix(const Rcpp::RObject&);
    ~Csparse_matrix();

    const T * get_row(int);
    const T * get_col(int);
    T get(int, int);

    void fill_row(int, T*);
    void fill_col(int, T*);
protected:
    Rcpp::RObject obj_i, obj_p, obj_x;
    const int * iptr, * pptr;
    int nx;
    int get_index(int, int) const;   

    const T * xptr;
    std::vector<T> row_data, col_data;
    T* row_ptr, * col_ptr;
};

/* symmetric packed Matrix */

template<typename T>
class Psymm_matrix : public any_matrix<T> {
public:    
    Psymm_matrix(const Rcpp::RObject&);
    ~Psymm_matrix();

    const T * get_row(int);
    const T * get_col(int);
    T get(int, int);
    
    void fill_row(int, T*);
    void fill_col(int, T*);
protected:
    Rcpp::RObject obj_x;
    bool upper;
    int get_index(int, int) const;
    
    const T * xptr;
    std::vector<T> out_data;
    T* out_ptr;
};

/* HDF5Matrix */

#ifdef BEACHMAT_USE_HDF5

template<typename T, const H5::PredType& H>
class HDF5_matrix : public any_matrix<T> {
public:
    HDF5_matrix(const Rcpp::RObject&);
    ~HDF5_matrix();

    const T * get_row(int);
    const T * get_col(int);
    T get(int, int);

    void fill_row(int, T*);
    void fill_col(int, T*);
protected:
    H5::H5File hfile;
    H5::DataSet hdata;
    H5::DataSpace hspace, rowspace, colspace, onespace;
    
    hsize_t offset[2], rows_out[2], cols_out[2], one_out[2];
    void set_row(int);
    void set_col(int);
    void set_one(int, int);

    std::vector<T> row_data, col_data;
    T * row_ptr, * col_ptr;
};

#endif

#include "template_methods.h"

#endif
