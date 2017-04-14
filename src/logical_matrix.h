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
    simple_logical_matrix(const Rcpp::RObject&);
    ~simple_logical_matrix();

    const int* get_row(int);
    const int* get_col(int);
    int get(int, int);

protected:
    const int* simple_ptr;
    std::vector<int> row_data;
    int * row_ptr;
};

/* lgeMatrix */

class dense_logical_matrix : public dense_matrix, public logical_matrix {
public:    
    dense_logical_matrix(const Rcpp::RObject&);
    ~dense_logical_matrix();

    const int* get_row(int);
    const int* get_col(int);
    int get(int, int);

protected:
    const int* dense_ptr;
    std::vector<int> row_data;
    int * row_ptr;
};

/* lgCMatrix */

class Csparse_logical_matrix : public Csparse_matrix, public logical_matrix {
public:    
    Csparse_logical_matrix(const Rcpp::RObject&);
    ~Csparse_logical_matrix();

    const int * get_row(int);
    const int * get_col(int);
    int get(int, int);
    
protected:
    const int * xptr;
    std::vector<int> row_data, col_data;
    int* row_ptr, * col_ptr;
};

/* lspMatrix */

class Psymm_logical_matrix : public Psymm_matrix, public logical_matrix {
public:    
    Psymm_logical_matrix(const Rcpp::RObject&);
    ~Psymm_logical_matrix();
    
    const int * get_row(int);
    const int * get_col(int);
    int get(int, int);
    
protected:
    const int * xptr;
    std::vector<int> out_data;
    int* out_ptr;
};

/* HDF5Matrix */

class HDF5_logical_matrix : public HDF5_matrix, public logical_matrix {
public:
    HDF5_logical_matrix(const Rcpp::RObject&);
    ~HDF5_logical_matrix();

    const int * get_row(int);
    const int * get_col(int);
    int get(int, int);

protected:
    std::vector<int> row_data, col_data;
    int* row_ptr, * col_ptr;
};

/* Dispatcher */

std::shared_ptr<logical_matrix> create_logical_matrix(const Rcpp::RObject&);


