#ifndef BEACHMAT_PSYMM_MATRIX_H
#define BEACHMAT_PSYMM_MATRIX_H

#include "beachmat.h"
#include "utils.h"
#include "any_matrix.h"

namespace beachmat {

/* Class definition */

template<typename T, class V>
class Psymm_matrix : public any_matrix {
public:    
    Psymm_matrix(const Rcpp::RObject&);
    ~Psymm_matrix();

    T get(size_t, size_t);   

    template <class Iter>
    void get_row(size_t, Iter, size_t, size_t);

    template <class Iter>
    void get_col(size_t, Iter, size_t, size_t);

    matrix_type get_matrix_type () const;
protected:
    V x;
    bool upper;

    template <class Iter>
    void get_rowcol(size_t, Iter, size_t, size_t);

    size_t get_index(size_t, size_t) const;
};

/*** Constructor definition ***/

template <typename T, class V>
Psymm_matrix<T, V>::Psymm_matrix(const Rcpp::RObject& incoming) : upper(true) { 
    std::string ctype=check_Matrix_class(incoming, "spMatrix");  
    this->fill_dims(get_safe_slot(incoming, "Dim"));
    const size_t& NR=this->nrow;
    if (NR!=(this->ncol)) { throw_custom_error("'nrow' and 'ncol' should be equal for a ", ctype, " object"); }

    Rcpp::RObject temp=get_safe_slot(incoming, "x");
    if (temp.sexp_type()!=x.sexp_type()) { 
        std::stringstream err;
        err << "'x' slot in a " << get_class(incoming) << " object should be " << translate_type(x.sexp_type());
        throw std::runtime_error(err.str().c_str());
    }
    x=temp;
    if (x.size()!=(NR*(NR+1))/2) { throw_custom_error("length of 'x' in a ", ctype, " object is inconsistent with its dimensions"); }
   
    // Checking up/lo slot. 
    Rcpp::RObject ul=get_safe_slot(incoming, "uplo");
    std::string uplo;
    try {
        uplo=make_to_string(ul);
        if (uplo.length()!=1) { throw; }
    } catch (...) { 
        throw_custom_error("'uplo' slot in a ", ctype, " object should be a string of length 1"); 
    }

    switch (uplo[0]) { 
        case 'U':
            upper=true;
            break;
        case 'L':
            upper=false;
            break;
        default:
            throw_custom_error("'uplo' slot in a ", ctype, " object should be either 'U' or 'L'");
    }
    return;
}

template <typename T, class V>
Psymm_matrix<T, V>::~Psymm_matrix () {}

/*** Getter methods ***/

template <typename T, class V>
size_t Psymm_matrix<T, V>::get_index(size_t r, size_t c) const {
    check_oneargs(r, c);
    if (upper) {
        if (c > r) { 
            return (c*(c+1))/2 + r;            
        } else {
            return (r*(r+1))/2 + c;            
        }
    } else {
        if (r > c) { 
            return (this->nrow) * c - (c * (c - 1))/2 + r - c;
        } else {
            return (this->nrow) * r - (r * (r - 1))/2 + c - r;
        }
    }
}

template <typename T, class V>
T Psymm_matrix<T, V>::get(size_t r, size_t c) {
    return x[get_index(r, c)];
}

template <typename T, class V>
template <class Iter>
void Psymm_matrix<T, V>::get_rowcol (size_t c, Iter out, size_t first, size_t last) {
    auto xIt=x.begin();
    if (upper) {
        xIt+=(c*(c+1))/2;
        if (first < c) {
            if (last <= c) { 
                std::copy(xIt+first, xIt+last, out);
            } else {
                std::copy(xIt+first, xIt+c, out);
                out+=c - first;
                for (size_t i=c; i<last; ++i, ++out) {
                    (*out)=*(xIt+c);
                    xIt+=i+1;
                }
            }
        } else {
            size_t diff=first - c;
            xIt+=c*diff+(diff*(diff+1))/2;
            for (size_t i=first; i<last; ++i, ++out) {
                (*out)=*(xIt+c);
                xIt+=i+1;
            }
        }
    } else {
        const size_t& NR=this->nrow;
        if (first < c) { 
            xIt+=NR*first - (first*(first-1))/2;
            if (last <= c) {
                for (size_t i=first; i<last; ++i, ++out) {
                    (*out)=*(xIt+c-i);
                    xIt+=NR-i;
                }
            } else {
                for (size_t i=first; i<c; ++i, ++out) {
                    (*out)=*(xIt+c-i);
                    xIt+=NR-i;
                }
                std::copy(xIt, xIt+last-c, out);
            }
        } else {
            xIt+=NR*c - (c*(c-1))/2;
            std::copy(xIt + first - c, xIt+last - c, out);
        }
    }
    return;
}

template <typename T, class V>
template <class Iter>
void Psymm_matrix<T, V>::get_col (size_t c, Iter out, size_t first, size_t last) {
    check_colargs(c, first, last); // Split up to ensure we get the right error messages.
    get_rowcol(c, out, first, last);
    return;
}

template <typename T, class V>
template <class Iter>
void Psymm_matrix<T, V>::get_row (size_t r, Iter out, size_t first, size_t last) {
    check_rowargs(r, first, last);
    get_rowcol(r, out, first, last);
    return;
}

template<typename T, class V>
matrix_type Psymm_matrix<T, V>::get_matrix_type() const {
    return PSYMM;
}

}

#endif
