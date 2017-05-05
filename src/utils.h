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

Rcpp::RObject realize_DelayedMatrix(const Rcpp::RObject&);

int find_sexp_type (const Rcpp::RObject&);

extern const double numeric_zero;

extern const int integer_zero;

extern const int logical_false;

}

#endif
