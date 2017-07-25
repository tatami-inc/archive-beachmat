#ifndef BEACHMAT_LIN_OUTFUN_H
#define BEACHMAT_LIN_OUTFUN_H

namespace beachmat { 

/****************************************
 * Defining the common output interface. 
 ****************************************/

template<typename T>
lin_output<T>::lin_output() {}

template<typename T>
lin_output<T>::~lin_output() {}

template<typename T>
void lin_output<T>::get_col(size_t c, Rcpp::IntegerVector::iterator out) {
    get_col(c, out, 0, get_nrow());
    return;
}

template<typename T>
void lin_output<T>::get_col(size_t c, Rcpp::NumericVector::iterator out) {
    get_col(c, out, 0, get_nrow());
    return;
}

template<typename T>
void lin_output<T>::get_row(size_t r, Rcpp::IntegerVector::iterator out) {
    get_row(r, out, 0, get_ncol());
    return;
}

template<typename T>
void lin_output<T>::get_row(size_t r, Rcpp::NumericVector::iterator out) {
    get_row(r, out, 0, get_ncol());
    return;
}

template<typename T>
void lin_output<T>::set_col(size_t c, Rcpp::IntegerVector::iterator out) {
    set_col(c, out, 0, get_nrow());
    return;
}

template<typename T>
void lin_output<T>::set_col(size_t c, Rcpp::NumericVector::iterator out) {
    set_col(c, out, 0, get_nrow());
    return;
}

template<typename T>
void lin_output<T>::set_row(size_t r, Rcpp::IntegerVector::iterator out) {
    set_row(r, out, 0, get_ncol());
    return;
}

template<typename T>
void lin_output<T>::set_row(size_t r, Rcpp::NumericVector::iterator out) {
    set_row(r, out, 0, get_ncol());
    return;
}

/* Defining the simple output interface. */ 

template<typename T, class V>
simple_lin_output<T, V>::simple_lin_output(size_t nr, size_t nc) : mat(nr, nc) {}

template<typename T, class V>
simple_lin_output<T, V>::~simple_lin_output() {}

template<typename T, class V>
size_t simple_lin_output<T, V>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T, class V>
size_t simple_lin_output<T, V>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T, class V>
void simple_lin_output<T, V>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.get_col(c, out, first, last);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.get_col(c, out, first, last);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.get_row(r, out, first, last);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.get_row(r, out, first, last);
    return;
}

template<typename T, class V>
T simple_lin_output<T, V>::get(size_t r, size_t c) {
    return mat.get(r, c);
}

template<typename T, class V>
void simple_lin_output<T, V>::set_col(size_t c, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.set_col(c, out, first, last);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::set_col(size_t c, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.set_col(c, out, first, last);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::set_row(size_t r, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.set_row(r, out, first, last);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::set_row(size_t r, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.set_row(r, out, first, last);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::set(size_t r, size_t c, T in) {
    mat.set(r, c, in);
    return;
}

template<typename T, class V>
Rcpp::RObject simple_lin_output<T, V>::yield() {
    return mat.yield();
}

template<typename T, class V>
std::unique_ptr<lin_output<T> > simple_lin_output<T, V>::clone() const {
    return std::unique_ptr<lin_output<T> >(new simple_lin_output<T, V>(*this));
}

template<typename T, class V>
matrix_type simple_lin_output<T, V>::get_matrix_type() const {
    return mat.get_matrix_type();
}


/* Defining the sparse output interface. */ 

template<typename T, class V>
sparse_lin_output<T, V>::sparse_lin_output(size_t nr, size_t nc) : mat(nr, nc) {}

template<typename T, class V>
sparse_lin_output<T, V>::~sparse_lin_output() {}

template<typename T, class V>
size_t sparse_lin_output<T, V>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T, class V>
size_t sparse_lin_output<T, V>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T, class V>
void sparse_lin_output<T, V>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.get_col(c, out, first, last);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.get_col(c, out, first, last);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.get_row(r, out, first, last);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.get_row(r, out, first, last);
    return;
}

template<typename T, class V>
T sparse_lin_output<T, V>::get(size_t r, size_t c) {
    return mat.get(r, c);
}

template<typename T, class V>
void sparse_lin_output<T, V>::set_col(size_t c, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.set_col(c, out, first, last);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::set_col(size_t c, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.set_col(c, out, first, last);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::set_row(size_t r, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.set_row(r, out, first, last);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::set_row(size_t r, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.set_row(r, out, first, last);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::set(size_t r, size_t c, T in) {
    mat.set(r, c, in);
    return;
}

template<typename T, class V>
Rcpp::RObject sparse_lin_output<T, V>::yield() {
    return mat.yield();
}

template<typename T, class V>
std::unique_ptr<lin_output<T> > sparse_lin_output<T, V>::clone() const {
    return std::unique_ptr<lin_output<T> >(new sparse_lin_output<T, V>(*this));
}

template<typename T, class V>
matrix_type sparse_lin_output<T, V>::get_matrix_type() const {
    return mat.get_matrix_type();
}

/* Defining the HDF5 output interface. */

template<typename T, int RTYPE>
HDF5_lin_output<T, RTYPE>::HDF5_lin_output(size_t nr, size_t nc, size_t chunk_nr, size_t chunk_nc, int compress) : 
    mat(nr, nc, chunk_nr, chunk_nc, compress) {}

template<typename T, int RTYPE>
HDF5_lin_output<T, RTYPE>::~HDF5_lin_output() {}

template<typename T, int RTYPE>
size_t HDF5_lin_output<T, RTYPE>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T, int RTYPE>
size_t HDF5_lin_output<T, RTYPE>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T, int RTYPE>
T HDF5_lin_output<T, RTYPE>::get(size_t r, size_t c) {
    T out;
    mat.extract_one(r, c, &out);
    return out;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::set(size_t r, size_t c, T in) {
    mat.insert_one(r, c, &in);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.extract_row(r, &(*out), H5::PredType::NATIVE_INT32, first, last);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.extract_row(r, &(*out), H5::PredType::NATIVE_DOUBLE, first, last);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.extract_col(c, &(*out), H5::PredType::NATIVE_INT32, first, last);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.extract_col(c, &(*out), H5::PredType::NATIVE_DOUBLE, first, last);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::set_row(size_t r, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.insert_row(r, &(*out), H5::PredType::NATIVE_INT32, first, last);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::set_row(size_t r, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.insert_row(r, &(*out), H5::PredType::NATIVE_DOUBLE, first, last);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::set_col(size_t c, Rcpp::IntegerVector::iterator out, size_t first, size_t last) {
    mat.insert_col(c, &(*out), H5::PredType::NATIVE_INT32, first, last);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::set_col(size_t c, Rcpp::NumericVector::iterator out, size_t first, size_t last) {
    mat.insert_col(c, &(*out), H5::PredType::NATIVE_DOUBLE, first, last);
    return;
}

template<typename T, int RTYPE>
Rcpp::RObject HDF5_lin_output<T, RTYPE>::yield() {
    return mat.yield();
}

template<typename T, int RTYPE>
std::unique_ptr<lin_output<T> > HDF5_lin_output<T, RTYPE>::clone() const {
    return std::unique_ptr<lin_output<T> >(new HDF5_lin_output<T, RTYPE>(*this));
}

template<typename T, int RTYPE>
matrix_type HDF5_lin_output<T, RTYPE>::get_matrix_type() const {
    return mat.get_matrix_type();
}

}

#endif
