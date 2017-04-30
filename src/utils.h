#ifndef UTILS_H
#define UTILS_H

#include "beachmat.h"

std::string make_to_string(const Rcpp::RObject&);

std::string get_class(const Rcpp::RObject&);

Rcpp::RObject get_safe_slot(const Rcpp::RObject&, const std::string&);

void throw_custom_error(const std::string&, const std::string&, const std::string&);

std::string translate_type(int);

std::string check_Matrix_class (const Rcpp::RObject&, const std::string&);

template <class T>
struct index_orderer { 
    index_orderer(const T* host) : ptr(host) {}
    bool operator()(const int& l, const int& r) const { return ptr[l] < ptr[r]; }
    const T* ptr;
};

#endif
