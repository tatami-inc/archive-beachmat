#ifndef BEACHMAT_CHARACTER_OUTPUT_H
#define BEACHMAT_CHARACTER_OUTPUT_H

#include "Output_matrix.h"

namespace beachmat { 

/* Virtual base class for character matrices. */

class character_output {
public:    
    character_output();
    virtual ~character_output();
    
    virtual size_t get_nrow() const=0;
    virtual size_t get_ncol() const=0;

    // Getters    
    void get_row(size_t, Rcpp::StringVector::iterator); 
    virtual void get_row(size_t, Rcpp::StringVector::iterator, size_t, size_t)=0;

    void get_col(size_t, Rcpp::StringVector::iterator);
    virtual void get_col(size_t, Rcpp::StringVector::iterator, size_t, size_t)=0;

    virtual Rcpp::String get(size_t, size_t)=0;

    // Setters
    void fill_row(size_t, Rcpp::StringVector::iterator); 
    virtual void fill_row(size_t, Rcpp::StringVector::iterator, size_t, size_t)=0;

    void fill_col(size_t, Rcpp::StringVector::iterator);
    virtual void fill_col(size_t, Rcpp::StringVector::iterator, size_t, size_t)=0;

    virtual void fill(size_t, size_t, Rcpp::String)=0;

    // Other stuff.
    virtual Rcpp::RObject yield()=0;

    virtual std::unique_ptr<character_output> clone() const=0;

    virtual matrix_type get_matrix_type() const=0;
};

/* Simple character matrix */

class simple_character_output : public character_output {
public:
    simple_character_output(size_t, size_t);
    ~simple_character_output();
  
    size_t get_nrow() const;
    size_t get_ncol() const;
 
    void get_row(size_t, Rcpp::StringVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::StringVector::iterator, size_t, size_t);

    Rcpp::String get(size_t, size_t);

    void fill_row(size_t, Rcpp::StringVector::iterator, size_t, size_t);
    void fill_col(size_t, Rcpp::StringVector::iterator, size_t, size_t);
    
    void fill(size_t, size_t, Rcpp::String);

    Rcpp::RObject yield();

    std::unique_ptr<character_output> clone() const;
    
    matrix_type get_matrix_type() const;
private:
    simple_output<Rcpp::String, Rcpp::StringVector> mat;
};

/* HDF5Matrix */

class HDF5_character_output : public character_output {
public:   
    HDF5_character_output(size_t, size_t, 
            size_t=output_param::DEFAULT_STRLEN, 
            size_t=output_param::DEFAULT_CHUNKDIM, 
            size_t=output_param::DEFAULT_CHUNKDIM, 
            int=output_param::DEFAULT_COMPRESS);
    ~HDF5_character_output();

    size_t get_nrow() const;
    size_t get_ncol() const;
 
    void get_row(size_t, Rcpp::StringVector::iterator, size_t, size_t);
    void get_col(size_t, Rcpp::StringVector::iterator, size_t, size_t);

    Rcpp::String get(size_t, size_t);

    void fill_row(size_t, Rcpp::StringVector::iterator, size_t, size_t);
    void fill_col(size_t, Rcpp::StringVector::iterator, size_t, size_t);

    void fill(size_t, size_t, Rcpp::String);

    Rcpp::RObject yield();

    std::unique_ptr<character_output> clone() const;

    matrix_type get_matrix_type() const;
protected:
    HDF5_output<char, STRSXP> mat; 
    size_t bufsize;
    std::vector<char> row_buf, col_buf, one_buf;
};

/* Dispatcher */

std::unique_ptr<character_output> create_character_output(const Rcpp::RObject&);

}

#endif