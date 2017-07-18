#ifndef BEACHMAT_LIN_METHODS_H
#define BEACHMAT_LIN_METHODS_H

/****************************************
 * Defining the common input interface. 
 ****************************************/

template<typename T, class V>
lin_matrix<T, V>::lin_matrix() {}

template<typename T, class V>
lin_matrix<T, V>::~lin_matrix() {}

template<typename T, class V>
void lin_matrix<T, V>::get_col(size_t c, Rcpp::IntegerVector::iterator out) {
    get_col(c, out, 0, get_nrow());
    return;
}

template<typename T, class V>
void lin_matrix<T, V>::get_col(size_t c, Rcpp::NumericVector::iterator out) {
    get_col(c, out, 0, get_nrow());
    return;
}

template<typename T, class V>
void lin_matrix<T, V>::get_row(size_t r, Rcpp::IntegerVector::iterator out) {
    get_row(r, out, 0, get_ncol());
    return;
}

template<typename T, class V>
void lin_matrix<T, V>::get_row(size_t r, Rcpp::NumericVector::iterator out) {
    get_row(r, out, 0, get_ncol());
    return;
}

template<typename T, class V>
typename V::const_iterator lin_matrix<T, V>::get_const_col(size_t c, typename V::iterator work) {
    return get_const_col(c, work, 0, get_nrow());
}

template<typename T, class V>
typename V::const_iterator lin_matrix<T, V>::get_const_col(size_t c, typename V::iterator work, size_t first, size_t last) {
    get_col(c, work, first, last);
    return work;
}

template<typename T, class V>
size_t lin_matrix<T, V>::get_nonzero_col(size_t c, Rcpp::IntegerVector::iterator dex, Rcpp::IntegerVector::iterator out) {
    return get_nonzero_col(c, dex, out, 0, get_nrow());
}

template<typename T, class V>
size_t lin_matrix<T, V>::get_nonzero_col(size_t c, Rcpp::IntegerVector::iterator dex, Rcpp::NumericVector::iterator out) {
    return get_nonzero_col(c, dex, out, 0, get_nrow());
}

template<typename T, class V>
size_t lin_matrix<T, V>::get_nonzero_row(size_t r, Rcpp::IntegerVector::iterator dex, Rcpp::IntegerVector::iterator out) {
    return get_nonzero_row(r, dex, out, 0, get_ncol());
}

template<typename T, class V>
size_t lin_matrix<T, V>::get_nonzero_row(size_t r, Rcpp::IntegerVector::iterator dex, Rcpp::NumericVector::iterator out) {
    return get_nonzero_row(r, dex, out, 0, get_ncol());
}

template<class T, class Iter>
size_t zero_hunter(Rcpp::IntegerVector::iterator index, Iter val, size_t first, size_t last) {
    size_t nzero=0;
    Iter src=val;
    const T zero=0;

    for (size_t x=first; x<last; ++x, ++val) {
        if (*val!=zero) {
            ++nzero;
            (*index)=x;
            (*src)=(*val);
            ++src;
            ++index;
        }   
    }   
    return nzero;
}

template<typename T, class V>
size_t lin_matrix<T, V>::get_nonzero_row(size_t r, Rcpp::IntegerVector::iterator index, Rcpp::IntegerVector::iterator val, size_t first, size_t last) {
    get_row(r, val, first, last);
    return zero_hunter<int>(index, val, first, last);
}

template<typename T, class V>
size_t lin_matrix<T, V>::get_nonzero_row(size_t r, Rcpp::IntegerVector::iterator index, Rcpp::NumericVector::iterator val, size_t first, size_t last) {
    get_row(r, val, first, last);
    return zero_hunter<double>(index, val, first, last);
}

template<typename T, class V>
size_t lin_matrix<T, V>::get_nonzero_col(size_t c, Rcpp::IntegerVector::iterator index, Rcpp::IntegerVector::iterator val, size_t first, size_t last) {
    get_col(c, val, first, last);
    return zero_hunter<int>(index, val, first, last);
}

template<typename T, class V>
size_t lin_matrix<T, V>::get_nonzero_col(size_t c, Rcpp::IntegerVector::iterator index, Rcpp::NumericVector::iterator val, size_t first, size_t last) {
    get_col(c, val, first, last);
    return zero_hunter<double>(index, val, first, last);
}

/* Defining the advanced interface. */

template<typename T, class V, class M>
advanced_lin_matrix<T, V, M>::advanced_lin_matrix(const Rcpp::RObject& incoming) : mat(incoming) {}

template<typename T, class V, class M>
advanced_lin_matrix<T, V, M>::~advanced_lin_matrix() {}

template<typename T, class V, class M>
size_t advanced_lin_matrix<T, V, M>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T, class V, class M>
size_t advanced_lin_matrix<T, V, M>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T, class V, class M>
void advanced_lin_matrix<T, V, M>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.get_col(c, out, first, last);
    return;
}

template<typename T, class V, class M>
void advanced_lin_matrix<T, V, M>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.get_col(c, out, first, last);
    return;
}

template<typename T, class V, class M>
void advanced_lin_matrix<T, V, M>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.get_row(r, out, first, last);
    return;
}

template<typename T, class V, class M>
void advanced_lin_matrix<T, V, M>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.get_row(r, out, first, last);
    return;
}

template<typename T, class V, class M>
T advanced_lin_matrix<T, V, M>::get(size_t r, size_t c) {
    return mat.get(r, c);
}

template<typename T, class V, class M>
std::unique_ptr<lin_matrix<T, V> > advanced_lin_matrix<T, V, M>::clone() const {
    return std::unique_ptr<lin_matrix<T, V> >(new advanced_lin_matrix<T, V, M>(*this));
}

template<typename T, class V, class M> 
matrix_type advanced_lin_matrix<T, V, M>::get_matrix_type() const {
    return mat.get_matrix_type();
}

/* Defining specific interface for simple/dense matrices. */

template <typename T, class V>
simple_lin_matrix<T, V>::simple_lin_matrix(const Rcpp::RObject& in) : advanced_lin_matrix<T, V, simple_matrix<T, V> >(in) {}

template <typename T, class V>
simple_lin_matrix<T, V>::~simple_lin_matrix() {} 

template <typename T, class V>
typename V::const_iterator simple_lin_matrix<T, V>::get_const_col(size_t c, typename V::iterator work, size_t first, size_t last) {
    return this->mat.get_const_col(c, work, first, last);
}

template <typename T, class V>
std::unique_ptr<lin_matrix<T, V> > simple_lin_matrix<T, V>::clone() const {
    return std::unique_ptr<lin_matrix<T, V> >(new simple_lin_matrix<T, V>(*this));
}

template <typename T, class V>
dense_lin_matrix<T, V>::dense_lin_matrix(const Rcpp::RObject& in) : advanced_lin_matrix<T, V, dense_matrix<T, V> >(in) {}

template <typename T, class V>
dense_lin_matrix<T, V>::~dense_lin_matrix() {} 

template <typename T, class V>
typename V::const_iterator dense_lin_matrix<T, V>::get_const_col(size_t c, typename V::iterator work, size_t first, size_t last) {
    return this->mat.get_const_col(c, work, first, last);
}

template <typename T, class V>
std::unique_ptr<lin_matrix<T, V> > dense_lin_matrix<T, V>::clone() const {
    return std::unique_ptr<lin_matrix<T, V> >(new dense_lin_matrix<T, V>(*this));
}

/* Defining specific interface for sparse matrices. */

template <typename T, class V>
Csparse_lin_matrix<T, V>::Csparse_lin_matrix(const Rcpp::RObject& in) : advanced_lin_matrix<T, V, Csparse_matrix<T, V> >(in) {}

template <typename T, class V>
Csparse_lin_matrix<T, V>::~Csparse_lin_matrix() {} 

template <typename T, class V>
size_t Csparse_lin_matrix<T, V>::get_nonzero_col(size_t c, Rcpp::IntegerVector::iterator dex, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    return this->mat.get_nonzero_col(c, dex, out, first, last);
}

template <typename T, class V>
size_t Csparse_lin_matrix<T, V>::get_nonzero_col(size_t c, Rcpp::IntegerVector::iterator dex, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    return this->mat.get_nonzero_col(c, dex, out, first, last);
}

template <typename T, class V>
size_t Csparse_lin_matrix<T, V>::get_nonzero_row(size_t r, Rcpp::IntegerVector::iterator dex, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    return this->mat.get_nonzero_row(r, dex, out, first, last);
}

template <typename T, class V>
size_t Csparse_lin_matrix<T, V>::get_nonzero_row(size_t r, Rcpp::IntegerVector::iterator dex, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    return this->mat.get_nonzero_row(r, dex, out, first, last);
}

template <typename T, class V>
std::unique_ptr<lin_matrix<T, V> > Csparse_lin_matrix<T, V>::clone() const {
    return std::unique_ptr<lin_matrix<T, V> >(new Csparse_lin_matrix<T, V>(*this));
}

/* Defining the HDF5 interface. */

template<typename T, class V, int RTYPE>
HDF5_lin_matrix<T, V, RTYPE>::HDF5_lin_matrix(const Rcpp::RObject& incoming) : mat(incoming) {}

template<typename T, class V, int RTYPE>
HDF5_lin_matrix<T, V, RTYPE>::~HDF5_lin_matrix() {}

template<typename T, class V, int RTYPE>
size_t HDF5_lin_matrix<T, V, RTYPE>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T, class V, int RTYPE>
size_t HDF5_lin_matrix<T, V, RTYPE>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T, class V, int RTYPE>
void HDF5_lin_matrix<T, V, RTYPE>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.extract_col(c, &(*out), H5::PredType::NATIVE_INT32, first, last);
    return;
}

template<typename T, class V, int RTYPE>
void HDF5_lin_matrix<T, V, RTYPE>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.extract_col(c, &(*out), H5::PredType::NATIVE_DOUBLE, first, last);
    return;
}

template<typename T, class V, int RTYPE>
void HDF5_lin_matrix<T, V, RTYPE>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.extract_row(r, &(*out), H5::PredType::NATIVE_INT32, first, last);
    return;
}

template<typename T, class V, int RTYPE>
void HDF5_lin_matrix<T, V, RTYPE>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.extract_row(r, &(*out), H5::PredType::NATIVE_DOUBLE, first, last);
    return;
}

template<typename T, class V, int RTYPE>
T HDF5_lin_matrix<T, V, RTYPE>::get(size_t r, size_t c) {
    T out;
    mat.extract_one(r, c, &out);
    return out; 
}

template<typename T, class V, int RTYPE>
std::unique_ptr<lin_matrix<T, V> > HDF5_lin_matrix<T, V, RTYPE>::clone() const {
    return std::unique_ptr<lin_matrix<T, V> >(new HDF5_lin_matrix<T, V, RTYPE>(*this));
}

template<typename T, class V, int RTYPE>
matrix_type HDF5_lin_matrix<T, V, RTYPE>::get_matrix_type() const {
    return mat.get_matrix_type();
}

#endif
