#ifndef MATRIX_H
#define MATRIX_H

#include "beachmat.h"
#include "H5Cpp.h"
#include "utils.h"

/* A virtual base matrix class */

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

protected:
    int nrow, ncol;
    void fill_dims(SEXP);
};

/* A virtual simple matrix class */

class simple_matrix : public virtual any_matrix {
public:   
   simple_matrix(SEXP);
   ~simple_matrix();
protected:
    int get_index(int, int) const;   
}; 

/* A virtual dgeMatrix class */

class dense_matrix : public virtual any_matrix {
public:
    dense_matrix(SEXP);
    ~dense_matrix();
protected:
    int get_index(int, int) const;   
};

/* A virtual dgCMatrix class */

class Csparse_matrix : public virtual any_matrix {
public:
    Csparse_matrix(SEXP);
    ~Csparse_matrix();   
protected:
    const int * iptr, * pptr;
    int nx;
    int get_index(int, int) const;   
};

/* A virtual dgTMatrix class */

class Tsparse_matrix : public virtual any_matrix {
public:
    Tsparse_matrix(SEXP);
    ~Tsparse_matrix();   
protected:
    const int * iptr, * jptr;
    int nx;
    int* order, *pptr, *iptr2;
    int get_index(int, int) const;   
};

/* A virtual HDF5Matrix class */

class HDF5_matrix : public virtual any_matrix {
public:
    HDF5_matrix(SEXP);
    ~HDF5_matrix();
protected:
    H5::H5File hfile;
    H5::DataSet hdata;
    H5::DataSpace hspace, rowspace, colspace, onespace;
    
    hsize_t offset[2], rows_out[2], cols_out[2], one_out[2];
    void set_row(int);
    void set_col(int);
    void set_one(int, int);
};

#endif