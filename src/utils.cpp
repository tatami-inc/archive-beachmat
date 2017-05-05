#include "utils.h"

namespace beachmat {

std::string make_to_string(const Rcpp::RObject& str) {
    if (str.sexp_type()!=STRSXP || Rf_length(str.get__())!=1) {
        throw std::runtime_error("input RObject should contain a single string");
    }
    return Rcpp::as<std::vector<std::string> >(str).front();
}

std::string get_class(const Rcpp::RObject& incoming) {
    if (!incoming.isObject()) {
        throw std::runtime_error("object has no class attribute");
    }
    return make_to_string(incoming.attr("class"));
}

Rcpp::RObject get_safe_slot(const Rcpp::RObject& incoming, const std::string& slotname) {
    if (!incoming.hasSlot(slotname)) { 
        std::stringstream err;
        err << "no '" << slotname << "' slot in the " << get_class(incoming) << " object";
        throw std::runtime_error(err.str().c_str()); 
    }
    return incoming.slot(slotname);
}

void throw_custom_error(const std::string& left, const std::string& classname, const std::string& right) {
    std::stringstream err;
    err << left << classname << right;
    throw std::runtime_error(err.str().c_str());
}

std::string translate_type(int sexp_type) {
    std::string should_be;
    switch(sexp_type) {
        case REALSXP:
            should_be="double-precision";
            break;
        case INTSXP:
            should_be="integer";
            break;
        case LGLSXP:
            should_be="logical";
            break;
        case STRSXP:
            should_be="character";
            break;
        default:
            std::stringstream err;
            err << "unsupported sexptype '" << sexp_type << "'";
            throw std::runtime_error(err.str().c_str());
    }
    return should_be;
}

std::string check_Matrix_class (const Rcpp::RObject& mat, const std::string& expected) {
    std::string mattype=get_class(mat);
    if (!mat.isS4() || mattype.empty() || mattype.substr(1)!=expected) {
        throw_custom_error("matrix should be a *", expected, " object");
    }
    return mattype;
}

int find_sexp_type (const Rcpp::RObject& incoming) {
    if (incoming.isObject()) {
        const std::string classname=get_class(incoming);
        if (classname=="HDF5Matrix") {
            Rcpp::RObject h5seed=get_safe_slot(incoming, "seed");
            Rcpp::RObject first_val=get_safe_slot(h5seed, "first_val");
            return first_val.sexp_type();
        } else if (classname=="big.matrix") {
            Rcpp::RObject address=get_bigmemory_address(incoming);
            Rcpp::XPtr<BigMatrix> xptr(address);
            switch(xptr->matrix_type()) { 
                case 4:
                    return INTSXP;
                case 8:
                    return REALSXP;
            }
        } else if (classname.length()==9 && classname.substr(3)=="Matrix") {
            switch(classname[0]) { 
                case 'd':
                    return REALSXP;
                case 'l':
                    return LGLSXP;
            }
        }
        throw_custom_error("unknown SEXP type for ", classname, " object");
    }
    return incoming.sexp_type();
}

const double numeric_zero=0;
                               
const int integer_zero=0;
                               
const int logical_false=0;

Rcpp::RObject get_bigmemory_address(const Rcpp::RObject& incoming) {
    std::string ctype=get_class(incoming);
    if (!incoming.isS4() || ctype!="big.matrix") {
        throw std::runtime_error("matrix should be a big.matrix object");
    }
    return get_safe_slot(incoming, "address");
}

}

