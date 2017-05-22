#ifndef BEACHMAT_ANY_MATRIX_H
#define BEACHMAT_ANY_MATRIX_H

#include "beachmat.h"

/* Virtual base class for matrices. */

class any_matrix {
public:
    any_matrix();
    any_matrix(size_t, size_t);
    virtual ~any_matrix();
    size_t get_nrow() const;
    size_t get_ncol() const;
protected:
    size_t nrow, ncol;
    void fill_dims(const Rcpp::RObject&);
    void check_rowargs(size_t, size_t, size_t) const;
    void check_colargs(size_t, size_t, size_t) const;
    void check_oneargs(size_t, size_t) const;
};

#endif
