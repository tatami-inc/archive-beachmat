#ifndef CHARACTER_MATRIX_H
#define CHARACTER_MATRIX_H

#include "matrix.h"

namespace beachmat { 

/* Virtual base class for character matrices. */

typedef any_matrix<const char*> character_matrix;

/* Simple character matrix */

typedef simple_matrix<const char*, Rcpp::StringVector> simple_character_matrix;

// This is possible as Rcpp will automatically convert CHARSXP to const char* via CHAR().

/* HDF5Matrix */

#ifdef BEACHMAT_USE_HDF5

class HDF5_character_matrix : public HDF5_matrix<const char*, STRSXP, H5T_STRING, H5::PredType::C_S1> {
public:    
    HDF5_character_matrix(const Rcpp::RObject&);
    ~HDF5_character_matrix();

    const char* get(int, int);
    void get_row(int, const char**);
    void get_col(int, const char**);
protected:
    H5::StrType str_type;
    std::vector<char> row_buf, col_buf;
};

// H5::PredType::C_S1 not really necessary here, as we override get* methods anyway, so it never actually gets used. 

#endif

/* Dispatcher */

std::shared_ptr<character_matrix> create_character_matrix(const Rcpp::RObject&);

}

#endif
