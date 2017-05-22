#ifndef BEACHMAT_LIN_METHODS_H
#define BEACHMAT_LIN_METHODS_H

/****************************************
 * Defining the common input interface. 
 ****************************************/

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
    std::copy(coltmp.begin(), coltmp.begin() + end - start, out);
    return;
}

template<typename T>
void HDF5_lin_matrix<T>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, coltmp.data(), HPT, start, end);
    std::copy(coltmp.begin(), coltmp.begin() + end - start, out);
    return;
}

template<typename T>
void HDF5_lin_matrix<T>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, rowtmp.data(), HPT, start, end);
    std::copy(rowtmp.begin(), rowtmp.begin() + end - start, out);
    return;
}

template<typename T>
void HDF5_lin_matrix<T>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, rowtmp.data(), HPT, start, end);
    std::copy(rowtmp.begin(), rowtmp.begin() + end - start, out);
    return;
}

template<typename T>
T HDF5_lin_matrix<T>::get(size_t r, size_t c) {
    T out;
    mat.extract_one(r, c, &out, HPT);
    return out;
}

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
void lin_output<T>::fill_col(size_t c, Rcpp::IntegerVector::iterator out) {
    fill_col(c, out, 0, get_nrow());
    return;
}

template<typename T>
void lin_output<T>::fill_col(size_t c, Rcpp::NumericVector::iterator out) {
    fill_col(c, out, 0, get_nrow());
    return;
}

template<typename T>
void lin_output<T>::fill_row(size_t r, Rcpp::IntegerVector::iterator out) {
    fill_row(r, out, 0, get_ncol());
    return;
}

template<typename T>
void lin_output<T>::fill_row(size_t r, Rcpp::NumericVector::iterator out) {
    fill_row(r, out, 0, get_ncol());
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
void simple_lin_output<T, V>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
T simple_lin_output<T, V>::get(size_t r, size_t c) {
    return mat.get(r, c);
}

template<typename T, class V>
void simple_lin_output<T, V>::fill_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_col(c, out, start, end);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::fill_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.fill_col(c, out, start, end);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::fill_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_row(r, out, start, end);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::fill_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.fill_row(r, out, start, end);
    return;
}

template<typename T, class V>
void simple_lin_output<T, V>::fill(size_t r, size_t c, T in) {
    mat.fill(r, c, in);
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

/* Defining the HDF5 output interface. */

template<typename T, class V>
HDF5_lin_output<T, V>::HDF5_lin_output(size_t nr, size_t nc, const H5::DataType& hpt, T fill) : mat(nr, nc, hpt, fill), 
    HPT(hpt), rowtmp(mat.get_ncol()), coltmp(mat.get_nrow()) {}

template<typename T, class V>
HDF5_lin_output<T, V>::~HDF5_lin_output() {}

template<typename T, class V>
size_t HDF5_lin_output<T, V>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T, class V>
size_t HDF5_lin_output<T, V>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T, class V>
void HDF5_lin_output<T, V>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, coltmp.data(), HPT, start, end);
    std::copy(coltmp.begin(), coltmp.begin() + end - start, out);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, coltmp.data(), HPT, start, end);
    std::copy(coltmp.begin(), coltmp.begin() + end - start, out);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, rowtmp.data(), HPT, start, end);
    std::copy(rowtmp.begin(), rowtmp.begin() + end - start, out);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, rowtmp.data(), HPT, start, end);
    std::copy(rowtmp.begin(), rowtmp.begin() + end - start, out);
    return;
}

template<typename T, class V>
T HDF5_lin_output<T, V>::get(size_t r, size_t c) {
    return mat.get(r, c, HPT);
}

template<typename T, class V>
void HDF5_lin_output<T, V>::fill_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_col(c, coltmp.data(), HPT, start, end);
    std::copy(coltmp.begin(), coltmp.begin() + end - start, out);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::fill_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.fill_col(c, coltmp.data(), HPT, start, end);
    std::copy(coltmp.begin(), coltmp.begin() + end - start, out);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::fill_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_row(r, rowtmp.data(), HPT, start, end);
    std::copy(rowtmp.begin(), rowtmp.begin() + end - start, out);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::fill_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.fill_row(r, rowtmp.data(), HPT, start, end);
    std::copy(rowtmp.begin(), rowtmp.begin() + end - start, out);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::fill(size_t r, size_t c, T in) {
    mat.fill(r, c, in, HPT);
    return;
}

template<typename T, class V>
Rcpp::RObject HDF5_lin_output<T, V>::yield() {
    return mat.yield(HPT);
}

#endif
