#include "utils.h"

const char* get_class(SEXP incoming) {
    SEXP ctype=getAttrib(incoming, R_ClassSymbol);
    if (!isString(ctype) || LENGTH(ctype)!=1) {
        throw std::runtime_error("class definition should be a string");
    }
    return CHAR(STRING_ELT(ctype, 0));
}

SEXP get_safe_slot(SEXP incoming, const char* slotname) {
    SEXP dimslot = install(slotname);
    if (!R_has_slot(incoming, dimslot)) { 
        std::stringstream err;
        err << "no '" << slotname << "' slot in the " << get_class(incoming) << " object";
        throw std::runtime_error(err.str().c_str()); 
    }
    return R_do_slot(incoming, dimslot);
}

void throw_custom_error(const char* left, const char* classname, const char* right) {
    std::stringstream err;
    err << left << classname << right;
    throw std::runtime_error(err.str().c_str());
}


