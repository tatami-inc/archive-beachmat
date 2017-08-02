#include "character_matrix.h"

namespace beachmat {

/* Methods for the virtual class. */

character_matrix::character_matrix() {}

character_matrix::~character_matrix() {}

void character_matrix::get_col(size_t c, Rcpp::StringVector::iterator out) { 
    get_col(c, out, 0, get_nrow());
}

void character_matrix::get_row(size_t r, Rcpp::StringVector::iterator out) { 
    get_row(r, out, 0, get_ncol());
}

Rcpp::StringVector::iterator character_matrix::get_const_col(size_t c, Rcpp::StringVector::iterator work) {
    return get_const_col(c, work, 0, get_nrow());
}

Rcpp::StringVector::iterator character_matrix::get_const_col(size_t c, Rcpp::StringVector::iterator work, size_t first, size_t last) {
    get_col(c, work, first, last);
    return work;
}

/* Methods for the simple character matrix. */

simple_character_matrix::simple_character_matrix(const Rcpp::RObject& incoming) : mat(incoming) {}

simple_character_matrix::~simple_character_matrix() {}

size_t simple_character_matrix::get_nrow() const {
    return mat.get_nrow();
}

size_t simple_character_matrix::get_ncol() const {
    return mat.get_ncol();
}

void simple_character_matrix::get_row(size_t r, Rcpp::StringVector::iterator out, size_t first, size_t last) { 
    mat.get_row(r, out, first, last);
}

void simple_character_matrix::get_col(size_t c, Rcpp::StringVector::iterator out, size_t first, size_t last) { 
    mat.get_col(c, out, first, last);
}

Rcpp::String simple_character_matrix::get(size_t r, size_t c) {
    return mat.get(r, c);
}

Rcpp::StringVector::iterator simple_character_matrix::get_const_col(size_t c, Rcpp::StringVector::iterator work, size_t first, size_t last) {
    return mat.get_const_col(c, work, first, last);
}

std::unique_ptr<character_matrix> simple_character_matrix::clone() const {
    return std::unique_ptr<character_matrix>(new simple_character_matrix(*this));
}

Rcpp::RObject simple_character_matrix::yield() const {
    return mat.yield();
}

matrix_type simple_character_matrix::get_matrix_type() const {
    return mat.get_matrix_type();
}

/* Methods for the Rle character matrix. */

Rle_character_matrix::Rle_character_matrix(const Rcpp::RObject& incoming) : mat(incoming) {}

Rle_character_matrix::~Rle_character_matrix() {}

size_t Rle_character_matrix::get_nrow() const {
    return mat.get_nrow();
}

size_t Rle_character_matrix::get_ncol() const {
    return mat.get_ncol();
}

void Rle_character_matrix::get_row(size_t r, Rcpp::StringVector::iterator out, size_t first, size_t last) { 
    mat.get_row(r, out, first, last);
}

void Rle_character_matrix::get_col(size_t c, Rcpp::StringVector::iterator out, size_t first, size_t last) { 
    mat.get_col(c, out, first, last);
}

Rcpp::String Rle_character_matrix::get(size_t r, size_t c) {
    return mat.get(r, c);
}

std::unique_ptr<character_matrix> Rle_character_matrix::clone() const {
    return std::unique_ptr<character_matrix>(new Rle_character_matrix(*this));
}

Rcpp::RObject Rle_character_matrix::yield() const {
    return mat.yield();
}

matrix_type Rle_character_matrix::get_matrix_type() const {
    return mat.get_matrix_type();
}

/* Methods for the HDF5 character matrix. */

HDF5_character_matrix::HDF5_character_matrix(const Rcpp::RObject& incoming) : mat(incoming) {
    const H5::DataType& str_type=mat.get_datatype();
    if (!str_type.isVariableStr()) { 
        bufsize=str_type.getSize(); 
        row_buf.resize(bufsize*(mat.get_ncol()));
        col_buf.resize(bufsize*(mat.get_nrow()));
        one_buf.resize(bufsize);
    } else {
        throw std::runtime_error("variable-length strings not supported for HDF5_character_matrix");
    }
}

HDF5_character_matrix::~HDF5_character_matrix() {}

size_t HDF5_character_matrix::get_nrow() const {
    return mat.get_nrow();
}

size_t HDF5_character_matrix::get_ncol() const {
    return mat.get_ncol();
}

void HDF5_character_matrix::get_row(size_t r, Rcpp::StringVector::iterator out, size_t first, size_t last) { 
    char* ref=row_buf.data();
    mat.extract_row(r, ref, first, last);
    for (size_t c=first; c<last; ++c, ref+=bufsize, ++out) {
        (*out)=ref; 
    }
    return;
} 

void HDF5_character_matrix::get_col(size_t c, Rcpp::StringVector::iterator out, size_t first, size_t last) { 
    char* ref=col_buf.data();
    mat.extract_col(c, ref, first, last);
    for (size_t r=first; r<last; ++r, ref+=bufsize, ++out) {
        (*out)=ref; 
    }
    return;
}
 
Rcpp::String HDF5_character_matrix::get(size_t r, size_t c) { 
    char* ref=one_buf.data();
    mat.extract_one(r, c, ref);
    return ref;
}

std::unique_ptr<character_matrix> HDF5_character_matrix::clone() const {
    return std::unique_ptr<character_matrix>(new HDF5_character_matrix(*this));
}
 
Rcpp::RObject HDF5_character_matrix::yield() const {
    return mat.yield();
}

matrix_type HDF5_character_matrix::get_matrix_type() const {
    return mat.get_matrix_type();
}

/* Dispatch definition */

std::unique_ptr<character_matrix> create_character_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) { 
        std::string ctype=get_class(incoming);
        if (ctype=="HDF5Matrix") {
            return std::unique_ptr<character_matrix>(new HDF5_character_matrix(incoming));
        } else if (ctype=="RleMatrix") { 
            return std::unique_ptr<character_matrix>(new Rle_character_matrix(incoming));
        } else if (ctype=="DelayedMatrix") { 
            if (is_pristine_delayed_array(incoming)) { 
                return create_character_matrix(get_safe_slot(incoming, "seed"));
            } else {
                return create_character_matrix(realize_delayed_array(incoming));
            }
        }
        std::stringstream err;
        err << "unsupported class '" << ctype << "' for character_matrix";
        throw std::runtime_error(err.str().c_str());
    } 
    return std::unique_ptr<character_matrix>(new simple_character_matrix(incoming));
}

}
