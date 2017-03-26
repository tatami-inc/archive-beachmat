#include "beachmat.h"
#include "H5Cpp.h"

/* Virtual base class for numeric matrices. */

class numeric_matrix {
public:
    numeric_matrix();
    virtual ~numeric_matrix();

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
     * @return An array of doubles of length 'ncol', containing all values in row 'r'.
     */
    virtual const double* get_row(int)=0;

    /* Returns values in the specified column.
     *
     * @param c The column index (0-based).
     * @return An array of doubles of length 'nrow', containing all values in column 'c'.
     */
    virtual const double* get_col(int)=0;

    /* Returns values in the specified cell.
     *
     * @param r The row index (0-based).
     * @param c The column index (0-based).
     * @return Double, the value of the matrix at row 'r' and column 'c'.
     */
    virtual double get(int, int)=0;
protected:
    int nrow, ncol;
};

/* Simple numeric matrix */

class simple_numeric_matrix : public numeric_matrix {
public:    
    simple_numeric_matrix(SEXP);
    ~simple_numeric_matrix();

    const double* get_row(int);
    const double* get_col(int);
    double get(int, int);

protected:
    const double* simple_ptr;
    double * row_ptr;
};

/* dgCMatrix */

class Csparse_numeric_matrix : public numeric_matrix {
public:    
    Csparse_numeric_matrix(SEXP);
    ~Csparse_numeric_matrix();

    const double * get_row(int);
    const double * get_col(int);
    double get(int, int);
    
protected:
    const int * iptr, * pptr;
    const double * xptr;
    int nx;
    double* row_ptr, * col_ptr;
};

/* HDF5Matrix */

class HDF5_numeric_matrix : public numeric_matrix {
public:
    HDF5_numeric_matrix(SEXP);
    ~HDF5_numeric_matrix();

    const double * get_row(int);
    const double * get_col(int);
    double get(int, int);

protected:
    H5::H5File hfile;
    H5::DataSet hdata;
    H5::DataSpace hspace, rowspace, colspace, onespace;
    
    hsize_t offset[2], rows_out[2], cols_out[2], one_out[2];
    double* row_ptr, * col_ptr;
};

/* Dispatcher */

std::shared_ptr<numeric_matrix> create_numeric_matrix(SEXP);


