#include "matrix.h"

/* Virtual base class for logical matrices. */

class logical_matrix : public virtual any_matrix {
public:
    logical_matrix();
    virtual ~logical_matrix();

    /* Returns values in the specified row.
     *
     * @param r The row index (0-based).
     * @return An array of ints of length 'ncol', containing all values in row 'r'.
     */
    virtual const int* get_row(int)=0;

    /* Returns values in the specified column.
     *
     * @param c The column index (0-based).
     * @return An array of ints of length 'nrow', containing all values in column 'c'.
     */
    virtual const int* get_col(int)=0;

    /* Returns values in the specified cell.
     *
     * @param r The row index (0-based).
     * @param c The column index (0-based).
     * @return Double, the value of the matrix at row 'r' and column 'c'.
     */
    virtual int get(int, int)=0;
};

/* Simple logical matrix */

class simple_logical_matrix : public simple_matrix, public logical_matrix {
public:    
    simple_logical_matrix(SEXP);
    ~simple_logical_matrix();

    const int* get_row(int);
    const int* get_col(int);
    int get(int, int);

protected:
    const int* simple_ptr;
    int * row_ptr;
};

/* dgeMatrix */

class dense_logical_matrix : public dense_matrix, public logical_matrix {
public:    
    dense_logical_matrix(SEXP);
    ~dense_logical_matrix();

    const int* get_row(int);
    const int* get_col(int);
    int get(int, int);

protected:
    const int* dense_ptr;
    int * row_ptr;
};

/* dgCMatrix */

class Csparse_logical_matrix : public Csparse_matrix, public logical_matrix {
public:    
    Csparse_logical_matrix(SEXP);
    ~Csparse_logical_matrix();

    const int * get_row(int);
    const int * get_col(int);
    int get(int, int);
    
protected:
    const int * xptr;
    int* row_ptr, * col_ptr;
};

/* dgTMatrix */

class Tsparse_logical_matrix : public Tsparse_matrix, public logical_matrix {
public:    
    Tsparse_logical_matrix(SEXP);
    ~Tsparse_logical_matrix();

    const int * get_row(int);
    const int * get_col(int);
    int get(int, int);
    
protected:
    const int * xptr;
    int* row_ptr, * col_ptr;
};

/* HDF5Matrix */

class HDF5_logical_matrix : public HDF5_matrix, public logical_matrix {
public:
    HDF5_logical_matrix(SEXP);
    ~HDF5_logical_matrix();

    const int * get_row(int);
    const int * get_col(int);
    int get(int, int);

protected:
    int* row_ptr, * col_ptr;
};

/* Dispatcher */

std::shared_ptr<logical_matrix> create_logical_matrix(SEXP);


