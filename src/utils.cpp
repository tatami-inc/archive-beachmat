#include "utils.h"

const char* get_class(SEXP incoming) {
    SEXP ctype=getAttrib(incoming, R_ClassSymbol);
    if (!isString(ctype) || LENGTH(ctype)!=1) {
        throw std::runtime_error("class definition should be a string");
    }
    return CHAR(STRING_ELT(ctype, 0));
}

void fill_dims(int& nrow, int& ncol, SEXP dims) {
    if (!isInteger(dims) || LENGTH(dims)!=2) { 
        throw std::runtime_error("matrix dimensions should be an integer vector of length 2");
    }
    nrow=INTEGER(dims)[0];
    ncol=INTEGER(dims)[1];
    return;
}
