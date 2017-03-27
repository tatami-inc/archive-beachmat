#ifndef UTILS_H
#define UTILS_H

#include "beachmat.h"

const char* get_class(SEXP);

template <class T>
struct index_orderer { 
    index_orderer(const T* host) : ptr(host) {}
    bool operator()(const int& l, const int& r) const { return ptr[l] < ptr[r]; }
    const T* ptr;
};

#endif
