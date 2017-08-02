#ifndef BEACHMAT_DENSE_MATRIX_H
#define BEACHMAT_DENSE_MATRIX_H

#include "beachmat.h"
#include "utils.h"
#include "any_matrix.h"

namespace beachmat { 

/*** Class definition ***/

template<typename T, class V>
class dense_matrix : public any_matrix {
public:    
    dense_matrix(const Rcpp::RObject&);
    ~dense_matrix();

    T get(size_t, size_t);

    template <class Iter>
    void get_row(size_t, Iter, size_t, size_t);

    template <class Iter>
    void get_col(size_t, Iter, size_t, size_t);

    typename V::iterator get_const_col(size_t, typename V::iterator, size_t, size_t);

    Rcpp::RObject yield() const;
    matrix_type get_matrix_type () const;
protected:
    Rcpp::RObject original;
    V x;
};

/*** Constructor definitions ***/

template <typename T, class V>
dense_matrix<T, V>::dense_matrix(const Rcpp::RObject& incoming) : original(incoming) { 
    std::string ctype=check_Matrix_class(incoming, "geMatrix");
    this->fill_dims(incoming.attr("Dim"));
    const size_t& NC=this->ncol;
    
    Rcpp::RObject temp=get_safe_slot(incoming, "x"); 
    if (temp.sexp_type()!=x.sexp_type()) { 
        std::stringstream err;
        err << "'x' slot in a " << get_class(incoming) << " object should be " << translate_type(x.sexp_type());
        throw std::runtime_error(err.str().c_str());
    }
    x=temp;
    if (x.size()!=(this->nrow)*NC) {
        throw_custom_error("length of 'x' in a ", ctype, " object is inconsistent with its dimensions"); 
    }
    return;
}

template <typename T, class V>
dense_matrix<T, V>::~dense_matrix() {}

/*** Getter functions ***/

template <typename T, class V>
T dense_matrix<T, V>::get(size_t r, size_t c) { 
    check_oneargs(r, c);
    return x[r + c*(this->nrow)]; 
}

template <typename T, class V>
template <class Iter>
void dense_matrix<T, V>::get_row(size_t r, Iter out, size_t first, size_t last) {
    check_rowargs(r, first, last);
    const size_t& NR=this->nrow;
    auto src=x.begin()+first*NR+r;
    for (size_t col=first; col<last; ++col, src+=NR, ++out) { (*out)=*src; }
    return;
}

template <typename T, class V>
template <class Iter>
void dense_matrix<T, V>::get_col(size_t c, Iter out, size_t first, size_t last) {
    check_colargs(c, first, last);
    auto src=x.begin() + c*(this->nrow);
    std::copy(src+first, src+last, out);
    return;
}

template<typename T, class V>
typename V::iterator dense_matrix<T, V>::get_const_col(size_t c, typename V::iterator work, size_t first, size_t last) {
    return x.begin() + first + c*(this->nrow);
}

template<typename T, class V>
Rcpp::RObject dense_matrix<T, V>::yield() const {
    return original;
}

template<typename T, class V>
matrix_type dense_matrix<T, V>::get_matrix_type() const {
    return DENSE;
}

}

#endif
