#ifndef BEACHMAT_CHARACTER_MATRIX_H
#define BEACHMAT_CHARACTER_MATRIX_H

#include "Input_matrix.h"

namespace beachmat { 

/* Virtual base class for character matrices. */

class character_matrix {
public:    
    character_matrix();
    virtual ~character_matrix();
    
    virtual size_t get_nrow() const=0;
    virtual size_t get_ncol() const=0;
    
    void get_row(size_t, Rcpp::StringVector::iterator); 
    virtual void get_row(size_t, Rcpp::StringVector::iterator, size_t, size_t)=0;

    void get_col(size_t, Rcpp::StringVector::iterator);
    virtual void get_col(size_t, Rcpp::StringVector::iterator, size_t, size_t)=0;

    virtual Rcpp::String get(size_t, size_t)=0;

    Rcpp::StringVector::iterator get_const_col(size_t, Rcpp::StringVector::iterator);
    virtual Rcpp::StringVector::iterator get_const_col(size_t, Rcpp::StringVector::iterator, size_t, size_t);

    virtual std::unique_ptr<character_matrix> clone() const=0;

    virtual matrix_type get_matrix_type() const=0;
};

/* Simple character matrix */

class simple_character_matrix : public character_matrix {
public:
    simple_character_matrix(const Rcpp::RObject& incoming);
    ~simple_character_matrix();
  
    size_t get_nrow() const;
    size_t get_ncol() const;
 
    void get_row(size_t, Rcpp::StringVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::StringVector::iterator, size_t, size_t);

    Rcpp::String get(size_t, size_t);

    Rcpp::StringVector::iterator get_const_col(size_t, Rcpp::StringVector::iterator, size_t, size_t);

    std::unique_ptr<character_matrix> clone() const;
    
    matrix_type get_matrix_type() const;
private:
    simple_matrix<Rcpp::String, Rcpp::StringVector> mat;
};

/* RLE character matrix */

class Rle_character_matrix : public character_matrix {
public:
    Rle_character_matrix(const Rcpp::RObject& incoming);
    ~Rle_character_matrix();
  
    size_t get_nrow() const;
    size_t get_ncol() const;
 
    void get_row(size_t, Rcpp::StringVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::StringVector::iterator, size_t, size_t);

    Rcpp::String get(size_t, size_t);

    std::unique_ptr<character_matrix> clone() const;

    matrix_type get_matrix_type() const;
private:
    Rle_matrix<Rcpp::String, Rcpp::StringVector> mat;
};

/* HDF5Matrix */

class HDF5_character_matrix : public character_matrix {
public:    
    HDF5_character_matrix(const Rcpp::RObject&);
    ~HDF5_character_matrix();

    size_t get_nrow() const;
    size_t get_ncol() const;
 
    void get_row(size_t, Rcpp::StringVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::StringVector::iterator, size_t, size_t);

    Rcpp::String get(size_t, size_t);

    std::unique_ptr<character_matrix> clone() const;

    matrix_type get_matrix_type() const;
protected:
    HDF5_matrix<char, STRSXP> mat; 
    size_t bufsize;
    std::vector<char> row_buf, col_buf, one_buf;
};

/* Dispatcher */

std::unique_ptr<character_matrix> create_character_matrix(const Rcpp::RObject&);

}

/* Collected output definitions */

#include "character_output.h"

#endif
