#ifndef BEACHMAT_UTILS_H
#define BEACHMAT_UTILS_H

#include "beachmat.h"

namespace beachmat { 

std::string make_to_string(const Rcpp::RObject&);

std::string get_class(const Rcpp::RObject&);

Rcpp::RObject get_safe_slot(const Rcpp::RObject&, const std::string&);

void throw_custom_error(const std::string&, const std::string&, const std::string&);

std::string translate_type(int);

std::string check_Matrix_class (const Rcpp::RObject&, const std::string&);

H5T_class_t set_HDF5_types (int, H5::DataType&);

int find_sexp_type (const Rcpp::RObject&);

}

#endif
