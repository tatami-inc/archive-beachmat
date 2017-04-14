#include "matrix.h"

/* Virtual base class for integer matrices. */

class integer_matrix : public virtual any_matrix {
public:
    integer_matrix();
    virtual ~integer_matrix();

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

/* Simple integer matrix */

class simple_integer_matrix : public simple_matrix, public integer_matrix {
public:    
    simple_integer_matrix(const Rcpp::RObject&);
    ~simple_integer_matrix();

    const int* get_row(int);
    const int* get_col(int);
    int get(int, int);

protected:
    const int* simple_ptr;
    std::vector<int> row_data;
    int* row_ptr;
};

/* HDF5Matrix */

class HDF5_integer_matrix : public HDF5_matrix, public integer_matrix {
public:
    HDF5_integer_matrix(const Rcpp::RObject&);
    ~HDF5_integer_matrix();

    const int * get_row(int);
    const int * get_col(int);
    int get(int, int);

protected:
    std::vector<int> row_data, col_data;
    int* row_ptr, * col_ptr;
};

/* Dispatcher */

std::shared_ptr<integer_matrix> create_integer_matrix(const Rcpp::RObject&);


