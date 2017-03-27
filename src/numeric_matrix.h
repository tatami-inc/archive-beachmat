#include "matrix.h"

/* Virtual base class for numeric matrices. */

class numeric_matrix : public virtual any_matrix {
public:
    numeric_matrix();
    virtual ~numeric_matrix();

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
};

/* Simple numeric matrix */

class simple_numeric_matrix : public simple_matrix, public numeric_matrix {
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

class Csparse_numeric_matrix : public Csparse_matrix, public numeric_matrix {
public:    
    Csparse_numeric_matrix(SEXP);
    ~Csparse_numeric_matrix();

    const double * get_row(int);
    const double * get_col(int);
    double get(int, int);
    
protected:
    const double * xptr;
    double* row_ptr, * col_ptr;
};

/* HDF5Matrix */

class HDF5_numeric_matrix : public HDF5_matrix, public numeric_matrix {
public:
    HDF5_numeric_matrix(SEXP);
    ~HDF5_numeric_matrix();

    const double * get_row(int);
    const double * get_col(int);
    double get(int, int);

protected:
    double* row_ptr, * col_ptr;
};

/* Dispatcher */

std::shared_ptr<numeric_matrix> create_numeric_matrix(SEXP);


