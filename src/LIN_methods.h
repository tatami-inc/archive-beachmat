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
Csparse_lin_matrix<T, V>::Csparse_lin_matrix(const Rcpp::RObject& incoming) : mat(incoming) {}

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

/* Defining the Rle interface. */

template<typename T, class V>
Rle_lin_matrix<T, V>::Rle_lin_matrix(const Rcpp::RObject& incoming) : mat(incoming) {}

template<typename T, class V>
Rle_lin_matrix<T, V>::~Rle_lin_matrix() {}

template<typename T, class V>
size_t Rle_lin_matrix<T, V>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T, class V>
size_t Rle_lin_matrix<T, V>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T, class V>
void Rle_lin_matrix<T, V>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void Rle_lin_matrix<T, V>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void Rle_lin_matrix<T, V>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
void Rle_lin_matrix<T, V>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
T Rle_lin_matrix<T, V>::get(size_t r, size_t c) {
    return mat.get(r, c);
}

template<typename T, class V>
std::unique_ptr<lin_matrix<T> > Rle_lin_matrix<T, V>::clone() const {
    return std::unique_ptr<lin_matrix<T> >(new Rle_lin_matrix<T, V>(*this));
}

/* Defining the HDF5 interface. */

template<typename T, int RTYPE>
HDF5_lin_matrix<T, RTYPE>::HDF5_lin_matrix(const Rcpp::RObject& incoming) : mat(incoming) {}

template<typename T, int RTYPE>
HDF5_lin_matrix<T, RTYPE>::~HDF5_lin_matrix() {}

template<typename T, int RTYPE>
size_t HDF5_lin_matrix<T, RTYPE>::get_nrow() const {
    return mat.get_nrow();
}

template<typename T, int RTYPE>
size_t HDF5_lin_matrix<T, RTYPE>::get_ncol() const {
    return mat.get_ncol();
}

template<typename T, int RTYPE>
void HDF5_lin_matrix<T, RTYPE>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, &(*out), H5::PredType::NATIVE_INT32, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_matrix<T, RTYPE>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, &(*out), H5::PredType::NATIVE_DOUBLE, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_matrix<T, RTYPE>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, &(*out), H5::PredType::NATIVE_INT32, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_matrix<T, RTYPE>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, &(*out), H5::PredType::NATIVE_DOUBLE, start, end);
    return;
}

template<typename T, int RTYPE>
T HDF5_lin_matrix<T, RTYPE>::get(size_t r, size_t c) {
    T out;
    mat.extract_one(r, c, &out);
    return out; 
}

template<typename T, int RTYPE>
std::unique_ptr<lin_matrix<T> > HDF5_lin_matrix<T, RTYPE>::clone() const {
    return std::unique_ptr<lin_matrix<T> >(new HDF5_lin_matrix<T, RTYPE>(*this));
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
void sparse_lin_output<T, V>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V>
T sparse_lin_output<T, V>::get(size_t r, size_t c) {
    return mat.get(r, c);
}

template<typename T, class V>
void sparse_lin_output<T, V>::fill_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_col(c, out, start, end);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::fill_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.fill_col(c, out, start, end);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::fill_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_row(r, out, start, end);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::fill_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.fill_row(r, out, start, end);
    return;
}

template<typename T, class V>
void sparse_lin_output<T, V>::fill(size_t r, size_t c, T in) {
    mat.fill(r, c, in);
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

/* Defining the HDF5 output interface. */

template<typename T, class V>
HDF5_lin_output<T, V>::HDF5_lin_output(size_t nr, size_t nc) : mat(nr, nc) {}

template<typename T, class V>
HDF5_lin_output<T, V>::HDF5_lin_output(size_t nr, size_t nc, size_t chunk_nr, size_t chunk_nc, int compress) : 
    mat(nr, nc, chunk_nr, chunk_nc, compress) {}

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
T HDF5_lin_output<T, V>::get(size_t r, size_t c) {
    return mat.get(r, c);
}

template<typename T, class V>
void HDF5_lin_output<T, V>::fill(size_t r, size_t c, T in) {
    mat.fill(r, c, in);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, &(*out), H5::PredType::NATIVE_INT32, start, end);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, &(*out), H5::PredType::NATIVE_DOUBLE, start, end);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, &(*out), H5::PredType::NATIVE_INT32, start, end);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, &(*out), H5::PredType::NATIVE_DOUBLE, start, end);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::fill_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_row(r, &(*out), H5::PredType::NATIVE_INT32, start, end);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::fill_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.fill_row(r, &(*out), H5::PredType::NATIVE_DOUBLE, start, end);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::fill_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.fill_col(c, &(*out), H5::PredType::NATIVE_INT32, start, end);
    return;
}

template<typename T, class V>
void HDF5_lin_output<T, V>::fill_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.fill_col(c, &(*out), H5::PredType::NATIVE_DOUBLE, start, end);
    return;
}

template<typename T, class V>
Rcpp::RObject HDF5_lin_output<T, V>::yield() {
    return mat.yield();
}

template<typename T, class V>
std::unique_ptr<lin_output<T> > HDF5_lin_output<T, V>::clone() const {
    return std::unique_ptr<lin_output<T> >(new HDF5_lin_output<T, V>(*this));
}

#endif
