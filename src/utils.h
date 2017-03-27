#ifndef UTILS_H
#define UTILS_H

#include "beachmat.h"

const char* get_class(SEXP);

SEXP get_safe_slot(SEXP, const char*);

void throw_custom_error(const char*, const char*, const char*);

template <class T>
struct index_orderer { 
    index_orderer(const T* host) : ptr(host) {}
    bool operator()(const int& l, const int& r) const { return ptr[l] < ptr[r]; }
    const T* ptr;
};

#endif
