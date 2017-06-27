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
typename V::const_iterator lin_matrix<T, V>::get_const_col(size_t c, typename V::iterator work, size_t start, size_t end) {
    get_col(c, work, start, end);
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
size_t zero_hunter(Rcpp::IntegerVector::iterator index, Iter val, size_t start, size_t end) {
    size_t nzero=0;
    Iter src=val;
    const T zero=0;

    for (size_t x=start; x<end; ++x, ++val) {
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
size_t lin_matrix<T, V>::get_nonzero_row(size_t r, Rcpp::IntegerVector::iterator index, Rcpp::IntegerVector::iterator val, size_t start, size_t end) {
    get_row(r, val, start, end);
    return zero_hunter<int>(index, val, start, end);
}

template<typename T, class V>
size_t lin_matrix<T, V>::get_nonzero_row(size_t r, Rcpp::IntegerVector::iterator index, Rcpp::NumericVector::iterator val, size_t start, size_t end) {
    get_row(r, val, start, end);
    return zero_hunter<double>(index, val, start, end);
}

template<typename T, class V>
size_t lin_matrix<T, V>::get_nonzero_col(size_t c, Rcpp::IntegerVector::iterator index, Rcpp::IntegerVector::iterator val, size_t start, size_t end) {
    get_col(c, val, start, end);
    return zero_hunter<int>(index, val, start, end);
}

template<typename T, class V>
size_t lin_matrix<T, V>::get_nonzero_col(size_t c, Rcpp::IntegerVector::iterator index, Rcpp::NumericVector::iterator val, size_t start, size_t end) {
    get_col(c, val, start, end);
    return zero_hunter<double>(index, val, start, end);
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
void advanced_lin_matrix<T, V, M>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V, class M>
void advanced_lin_matrix<T, V, M>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_col(c, out, start, end);
    return;
}

template<typename T, class V, class M>
void advanced_lin_matrix<T, V, M>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
    return;
}

template<typename T, class V, class M>
void advanced_lin_matrix<T, V, M>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.get_row(r, out, start, end);
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
typename V::const_iterator simple_lin_matrix<T, V>::get_const_col(size_t c, typename V::iterator work, size_t start, size_t end) {
    return this->mat.get_const_col(c, work, start, end);
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
typename V::const_iterator dense_lin_matrix<T, V>::get_const_col(size_t c, typename V::iterator work, size_t start, size_t end) {
    return this->mat.get_const_col(c, work, start, end);
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
size_t Csparse_lin_matrix<T, V>::get_nonzero_col(size_t c, Rcpp::IntegerVector::iterator dex, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    return this->mat.get_nonzero_col(c, dex, out, start, end);
}

template <typename T, class V>
size_t Csparse_lin_matrix<T, V>::get_nonzero_col(size_t c, Rcpp::IntegerVector::iterator dex, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    return this->mat.get_nonzero_col(c, dex, out, start, end);
}

template <typename T, class V>
size_t Csparse_lin_matrix<T, V>::get_nonzero_row(size_t r, Rcpp::IntegerVector::iterator dex, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    return this->mat.get_nonzero_row(r, dex, out, start, end);
}

template <typename T, class V>
size_t Csparse_lin_matrix<T, V>::get_nonzero_row(size_t r, Rcpp::IntegerVector::iterator dex, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    return this->mat.get_nonzero_row(r, dex, out, start, end);
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
void HDF5_lin_matrix<T, V, RTYPE>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, &(*out), H5::PredType::NATIVE_INT32, start, end);
    return;
}

template<typename T, class V, int RTYPE>
void HDF5_lin_matrix<T, V, RTYPE>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, &(*out), H5::PredType::NATIVE_DOUBLE, start, end);
    return;
}

template<typename T, class V, int RTYPE>
void HDF5_lin_matrix<T, V, RTYPE>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, &(*out), H5::PredType::NATIVE_INT32, start, end);
    return;
}

template<typename T, class V, int RTYPE>
void HDF5_lin_matrix<T, V, RTYPE>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, &(*out), H5::PredType::NATIVE_DOUBLE, start, end);
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

template<typename T, class V>
matrix_type sparse_lin_output<T, V>::get_matrix_type() const {
    return mat.get_matrix_type();
}

/* Defining the HDF5 output interface. */

template<typename T, int RTYPE>
HDF5_lin_output<T, RTYPE>::HDF5_lin_output(size_t nr, size_t nc) : mat(nr, nc) {}

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
    return mat.extract_one(r, c);
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::fill(size_t r, size_t c, T in) {
    mat.insert_one(r, c, in);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::get_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, &(*out), H5::PredType::NATIVE_INT32, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::get_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_row(r, &(*out), H5::PredType::NATIVE_DOUBLE, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::get_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, &(*out), H5::PredType::NATIVE_INT32, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::get_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.extract_col(c, &(*out), H5::PredType::NATIVE_DOUBLE, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::fill_row(size_t r, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.insert_row(r, &(*out), H5::PredType::NATIVE_INT32, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::fill_row(size_t r, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.insert_row(r, &(*out), H5::PredType::NATIVE_DOUBLE, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::fill_col(size_t c, Rcpp::IntegerVector::iterator out, size_t start, size_t end) {
    mat.insert_col(c, &(*out), H5::PredType::NATIVE_INT32, start, end);
    return;
}

template<typename T, int RTYPE>
void HDF5_lin_output<T, RTYPE>::fill_col(size_t c, Rcpp::NumericVector::iterator out, size_t start, size_t end) {
    mat.insert_col(c, &(*out), H5::PredType::NATIVE_DOUBLE, start, end);
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

#endif
