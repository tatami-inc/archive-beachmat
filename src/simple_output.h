#ifndef BEACHMAT_SIMPLE_OUTPUT_H
#define BEACHMAT_SIMPLE_OUTPUT_H

#include "beachmat.h"
#include "utils.h"
#include "any_matrix.h"

namespace beachmat {

/*** Class definition ***/

template<typename T, class V>
class simple_output : public any_matrix {
public:
    simple_output(size_t, size_t);
    ~simple_output();

    template <class Iter>
    void set_row(size_t, Iter, size_t, size_t);
    template <class Iter>
    void set_col(size_t, Iter, size_t, size_t);
    void set(size_t, size_t, T);

    template <class Iter>
    void get_col(size_t, Iter, size_t, size_t);
    template <class Iter>
    void get_row(size_t, Iter, size_t, size_t);
    T get(size_t, size_t);

    Rcpp::RObject yield();

    matrix_type get_matrix_type() const;
private:
    V data;    
};

/*** Constructor definition ***/

template<typename T, class V>
simple_output<T, V>::simple_output(size_t nr, size_t nc) : any_matrix(nr, nc) { 
    (this->data)=V(nr*nc);
    return; 
}

template<typename T, class V>
simple_output<T, V>::~simple_output() {}

/*** Setter methods ***/

template<typename T, class V>
template<class Iter>
void simple_output<T, V>::set_col(size_t c, Iter in, size_t start, size_t end) {
    check_colargs(c, start, end);
    std::copy(in, in + end - start, data.begin()+c*(this->nrow)+start); 
    return;
}

template<typename T, class V>
template<class Iter>
void simple_output<T, V>::set_row(size_t r, Iter in, size_t start, size_t end) {
    check_rowargs(r, start, end);
    const size_t& NR=this->nrow;
    auto mIt=data.begin() + r + start * NR;
    for (size_t c=start; c<end; ++c, mIt+=NR, ++in) {
        (*mIt)=*in;        
    }
    return;
}

template<typename T, class V>
void simple_output<T, V>::set(size_t r, size_t c, T in) {
    check_oneargs(r, c);
    data[r + (this->nrow)*c]=in;
    return;
}

/*** Getter methods ***/

template<typename T, class V>
template<class Iter>
void simple_output<T, V>::get_row(size_t r, Iter out, size_t start, size_t end) {
    check_rowargs(r, start, end);
    const size_t& NR=this->nrow;
    auto src=data.begin()+start*NR+r;
    for (size_t col=start; col<end; ++col, src+=NR, ++out) { (*out)=(*src); }
    return;
}

template<typename T, class V>
template<class Iter>
void simple_output<T, V>::get_col(size_t c, Iter out, size_t start, size_t end) {
    check_colargs(c, start, end);
    auto src=data.begin() + c*(this->nrow);
    std::copy(src+start, src+end, out);
    return;
}

template<typename T, class V>
T simple_output<T, V>::get(size_t r, size_t c) {
    check_oneargs(r, c);
    return data[c*(this->nrow)+r];
}

/*** Output function ***/

template<typename T, class V>
Rcpp::RObject simple_output<T, V>::yield() {
    Rcpp::RObject out(SEXP(this->data));
    out.attr("dim") = Rcpp::IntegerVector::create(this->nrow, this->ncol); 
    return out;
}

template<typename T, class V>
matrix_type simple_output<T, V>::get_matrix_type() const {
    return SIMPLE;
}

}

#endif
