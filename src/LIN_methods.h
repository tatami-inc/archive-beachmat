#ifndef BEACHMAT_LIN_METHODS_H
#define BEACHMAT_LIN_METHODS_H

/* Defining the common interface. */

template<typename T>
lin_matrix<T>::lin_matrix() {}

template<typename T>
lin_matrix<T>::~lin_matrix() {}

template<typename T>
void lin_matrix<T>::get_col(size_t c, Rcpp::IntegerVector::iterator out) {
    get_col(c, out, 0, get_nrow());
    return;
}

template<typename T>
void lin_matrix<T>::get_col(size_t c, Rcpp::NumericVector::iterator out) {
    get_col(c, out, 0, get_nrow());
    return;
}

template<typename T>
void lin_matrix<T>::get_row(size_t r, Rcpp::IntegerVector::iterator out) {
    get_row(r, out, 0, get_ncol());
    return;
}

template<typename T>
void lin_matrix<T>::get_row(size_t r, Rcpp::NumericVector::iterator out) {
    get_row(r, out, 0, get_ncol());
    return;
}

/* Defining the simple interface. */

template<typename T, class V>
simple_lin_matrix<T, V>::simple_lin_matrix(const Rcpp::RObject& incoming) : mat(incoming) {}

template<typename T, class V>
simple_lin_matrix<T, V>::~simple_lin_matrix() {}

template<typename T, class V>
size_t simple_lin_matrix<T, V>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T, class V>
size_t simple_lin_matrix<T, V>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T, class V>
void simple_lin_matrix<T, V>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void simple_lin_matrix<T, V>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void simple_lin_matrix<T, V>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
void simple_lin_matrix<T, V>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
T simple_lin_matrix<T, V>::get(size_t r, size_t c) {
    return mat.get(r, c);
}

template<typename T, class V>
std::unique_ptr<lin_matrix<T> > simple_lin_matrix<T, V>::clone() const {
    return std::unique_ptr<lin_matrix<T> >(new simple_lin_matrix<T, V>(*this));
}

/* Defining the dense interface. */

template<typename T, class V>
dense_lin_matrix<T, V>::dense_lin_matrix(const Rcpp::RObject& incoming) : mat(incoming) {}

template<typename T, class V>
dense_lin_matrix<T, V>::~dense_lin_matrix() {}

template<typename T, class V>
size_t dense_lin_matrix<T, V>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T, class V>
size_t dense_lin_matrix<T, V>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T, class V>
void dense_lin_matrix<T, V>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void dense_lin_matrix<T, V>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void dense_lin_matrix<T, V>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
void dense_lin_matrix<T, V>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
T dense_lin_matrix<T, V>::get(size_t r, size_t c) {
    return mat.get(r, c);
}

template<typename T, class V>
std::unique_ptr<lin_matrix<T> > dense_lin_matrix<T, V>::clone() const {
    return std::unique_ptr<lin_matrix<T> >(new dense_lin_matrix<T, V>(*this));
}

/* Defining the column-sparse interface. */

template<typename T, class V>
Csparse_lin_matrix<T, V>::Csparse_lin_matrix(const Rcpp::RObject& incoming, T fill) : mat(incoming, fill) {}

template<typename T, class V>
Csparse_lin_matrix<T, V>::~Csparse_lin_matrix() {}

template<typename T, class V>
size_t Csparse_lin_matrix<T, V>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T, class V>
size_t Csparse_lin_matrix<T, V>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T, class V>
void Csparse_lin_matrix<T, V>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void Csparse_lin_matrix<T, V>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void Csparse_lin_matrix<T, V>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
void Csparse_lin_matrix<T, V>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
T Csparse_lin_matrix<T, V>::get(size_t r, size_t c) {
    return mat.get(r, c);
}

template<typename T, class V>
std::unique_ptr<lin_matrix<T> > Csparse_lin_matrix<T, V>::clone() const {
    return std::unique_ptr<lin_matrix<T> >(new Csparse_lin_matrix<T, V>(*this));
}

/* Defining the packed symmetric interface. */

template<typename T, class V>
Psymm_lin_matrix<T, V>::Psymm_lin_matrix(const Rcpp::RObject& incoming) : mat(incoming) {}

template<typename T, class V>
Psymm_lin_matrix<T, V>::~Psymm_lin_matrix() {}

template<typename T, class V>
size_t Psymm_lin_matrix<T, V>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T, class V>
size_t Psymm_lin_matrix<T, V>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T, class V>
void Psymm_lin_matrix<T, V>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void Psymm_lin_matrix<T, V>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void Psymm_lin_matrix<T, V>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
void Psymm_lin_matrix<T, V>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
T Psymm_lin_matrix<T, V>::get(size_t r, size_t c) {
    return mat.get(r, c);
}

template<typename T, class V>
std::unique_ptr<lin_matrix<T> > Psymm_lin_matrix<T, V>::clone() const {
    return std::unique_ptr<lin_matrix<T> >(new Psymm_lin_matrix<T, V>(*this));
}

/* Defining the HDF5 interface. */

template<typename T>
HDF5_lin_matrix<T>::HDF5_lin_matrix(const Rcpp::RObject& incoming, int expected, const H5T_class_t& hct, const H5::PredType& hpt) : 
    mat(incoming, expected, hct), HPT(hpt), rowtmp(mat.get_ncol()), coltmp(mat.get_nrow()) {}

template<typename T>
HDF5_lin_matrix<T>::~HDF5_lin_matrix() {}

template<typename T>
size_t HDF5_lin_matrix<T>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T>
size_t HDF5_lin_matrix<T>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T>
void HDF5_lin_matrix<T>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, coltmp.data(), HPT, start, end);
    std::copy(coltmp.begin(), coltmp.end(), out);
    return;
}

template<typename T>
void HDF5_lin_matrix<T>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, coltmp.data(), HPT, start, end);
    std::copy(coltmp.begin(), coltmp.end(), out);
    return;
}

template<typename T>
void HDF5_lin_matrix<T>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, rowtmp.data(), HPT, start, end);
    std::copy(rowtmp.begin(), rowtmp.end(), out);
    return;
}

template<typename T>
void HDF5_lin_matrix<T>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, rowtmp.data(), HPT, start, end);
    std::copy(rowtmp.begin(), rowtmp.end(), out);
    return;
}

template<typename T>
T HDF5_lin_matrix<T>::get(size_t r, size_t c) {
    T out;
    mat.extract_one(r, c, &out, HPT);
    return out;
}

template<typename T>
std::unique_ptr<lin_matrix<T> > HDF5_lin_matrix<T>::clone() const {
    return std::unique_ptr<lin_matrix<T> >(new HDF5_lin_matrix<T>(*this));
}

#endif
