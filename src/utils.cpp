#include "utils.h"

const char* get_class(SEXP incoming) {
    SEXP ctype=getAttrib(incoming, R_ClassSymbol);
    if (!isString(ctype) || LENGTH(ctype)!=1) {
        throw std::runtime_error("class definition should be a string");
    }
    return CHAR(STRING_ELT(ctype, 0));
}

