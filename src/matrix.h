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

    /* Fills an array with values in the specified row.
     *
     * @param r The row index (0-based).
     * @param out An array of T's of length 'nrow'.
     * @return Void. 'out' is filled with values in row 'r'.
     */
    virtual void fill_row(int, T*)=0;

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

/* Simple matrix */

template<typename T, class V>
class simple_matrix : public any_matrix<T> {
public:    
    simple_matrix(const Rcpp::RObject&);
    ~simple_matrix();

    T get(int, int);
    void fill_row(int, T*);
    void fill_col(int, T*);
protected:
    int get_index(int, int) const;   
    V mat;
};

/* dense Matrix */

template<typename T, class V>
class dense_matrix : public any_matrix<T> {
public:    
    dense_matrix(const Rcpp::RObject&);
    ~dense_matrix();

    T get(int, int);
    void fill_row(int, T*);
    void fill_col(int, T*);
protected:
    int get_index(int, int) const;   
    V x;
};

/* column-major sparse Matrix */

template<typename T, class V, const T& Z>
class Csparse_matrix : public any_matrix<T> {
public:    
    Csparse_matrix(const Rcpp::RObject&);
    ~Csparse_matrix();

    T get(int, int);
    void fill_row(int, T*);
    void fill_col(int, T*);
protected:
    Rcpp::IntegerVector i, p;
    V x;
    int nx;
    int get_index(int, int) const;

    int currow;
    std::vector<int> indices;
    void update_indices(int);
};

/* symmetric packed Matrix */

template<typename T, class V>
class Psymm_matrix : public any_matrix<T> {
public:    
    Psymm_matrix(const Rcpp::RObject&);
    ~Psymm_matrix();

    T get(int, int);   
    void fill_row(int, T*);
    void fill_col(int, T*);
protected:
    V x;
    bool upper;
    int get_index(int, int) const;
};

/* HDF5Matrix */

#ifdef BEACHMAT_USE_HDF5

template<typename T, int RTYPE, H5T_class_t HTC, const H5::PredType& HPT>
class HDF5_matrix : public any_matrix<T> {
public:
    HDF5_matrix(const Rcpp::RObject&);
    ~HDF5_matrix();

    T get(int, int);
    void fill_row(int, T*);
    void fill_col(int, T*);
protected:
    H5::H5File hfile;
    H5::DataSet hdata;
    H5::DataSpace hspace, rowspace, colspace, onespace;
    hsize_t offset[2], rows_out[2], cols_out[2], one_out[2];
};

#endif

#include "template_methods.h"

#endif

/* Comments (Aaron Lun):
 *
 * (i) If any class has pointers to SEXP data, the data that each pointer points to should be contained within a RObject that is also a member of the class. 
 * This ensures that the data is PROTECTed for the lifetime of the class instance. 
 * Otherwise, one could imagine a situation where the class is instantiated from an RObject; the RObject is destroyed; and garbage collection occurs.
 * This would invalidate the pointers in the class instance.
 *
 * (ii) We return pointers rather than std::vectors to maintain high performance for column-major access of base matrices.
 * For these matrices, we can currently return a pointer for direct access to the column-wise arrays.
 * In contrast, if we were forced to return std::vectors, we would have to do a copy to transfer data ownership to the std::vector container.
 * Note that this choice does require some care by the user, as existing pointers are not guaranteed to be valid after another get_* call.
 *
 * (iii) It would be nice to allow the fill_* methods to take any random access iterator; however, virtual methods cannot be templated.
 * We could add a template argument to the entire class, but this would only allow it to take either a random access iterator or a pointer (not both).
 * Overloading seems like the best approach, but would be a pain if I have to rewrite every fill_* method.
 */


