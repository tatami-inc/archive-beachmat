#include "character_matrix.h"

namespace beachmat {

/* Methods for the virtual class. */

character_output::character_output() {}

character_output::~character_output() {}

void character_output::get_col(size_t c, Rcpp::StringVector::iterator out) { 
    get_col(c, out, 0, get_nrow());
}

void character_output::get_row(size_t r, Rcpp::StringVector::iterator out) { 
    get_row(r, out, 0, get_ncol());
}

void character_output::fill_col(size_t c, Rcpp::StringVector::iterator out) { 
    fill_col(c, out, 0, get_nrow());
}

void character_output::fill_row(size_t r, Rcpp::StringVector::iterator out) { 
    fill_row(r, out, 0, get_ncol());
}

/* Methods for the simple character matrix. */

simple_character_output::simple_character_output(size_t nr, size_t nc) : mat(nr, nc) {}

simple_character_output::~simple_character_output() {}

size_t simple_character_output::get_nrow() const {
    return mat.get_nrow();
}

size_t simple_character_output::get_ncol() const {
    return mat.get_ncol();
}

void simple_character_output::get_row(size_t r, Rcpp::StringVector::iterator out, size_t start, size_t end) { 
    mat.get_row(r, out, start, end);
}

void simple_character_output::get_col(size_t c, Rcpp::StringVector::iterator out, size_t start, size_t end) { 
    mat.get_col(c, out, start, end);
}

Rcpp::String simple_character_output::get(size_t r, size_t c) {
    return mat.get(r, c);
}

void simple_character_output::fill_row(size_t r, Rcpp::StringVector::iterator in, size_t start, size_t end) { 
    mat.fill_row(r, in, start, end);
}

void simple_character_output::fill_col(size_t c, Rcpp::StringVector::iterator in, size_t start, size_t end) { 
    mat.fill_col(c, in, start, end);
}

void simple_character_output::fill(size_t r, size_t c, Rcpp::String in) {
    mat.fill(r, c, in);
    return;
}

Rcpp::RObject simple_character_output::yield() {
    return mat.yield();
}

std::unique_ptr<character_output> simple_character_output::clone() const {
    return std::unique_ptr<character_output>(new simple_character_output(*this));
}

matrix_type simple_character_output::get_matrix_type() const {
    return mat.get_matrix_type();
}

/* Methods for the HDF5 character matrix. */

template<>
char HDF5_output<char, STRSXP>::get_empty() const { return '\0'; }

HDF5_character_output::HDF5_character_output(size_t nr, size_t nc, size_t strlen, size_t chunk_nr, size_t chunk_nc, int compress) :
        bufsize(strlen+1), mat(nr, nc, chunk_nr, chunk_nc, compress, bufsize), col_buf(bufsize*nr), row_buf(bufsize*nc), one_buf(bufsize) {}

HDF5_character_output::~HDF5_character_output() {}

size_t HDF5_character_output::get_nrow() const {
    return mat.get_nrow();
}

size_t HDF5_character_output::get_ncol() const {
    return mat.get_ncol();
}

void HDF5_character_output::get_row(size_t r, Rcpp::StringVector::iterator out, size_t start, size_t end) { 
    char* ref=row_buf.data();
    mat.extract_row(r, ref, start, end);
    for (size_t c=start; c<end; ++c, ref+=bufsize, ++out) {
        (*out)=ref; 
    }
    return;
} 

void HDF5_character_output::get_col(size_t c, Rcpp::StringVector::iterator out, size_t start, size_t end) { 
    char* ref=col_buf.data();
    mat.extract_col(c, ref, start, end);
    for (size_t r=start; r<end; ++r, ref+=bufsize, ++out) {
        (*out)=ref; 
    }
    return;
}
 
Rcpp::String HDF5_character_output::get(size_t r, size_t c) { 
    char* ref=one_buf.data();
    mat.extract_one(r, c, ref);
    return ref;
}

void HDF5_character_output::fill_row(size_t r, Rcpp::StringVector::iterator in, size_t start, size_t end) { 
    if (mat.get_ncol() + start >= end) { // ensure they can fit in 'row_buf'; if not, it should trigger an error in insert_row().
        char* ref=row_buf.data();
        for (size_t c=start; c<end; ++c, ref+=bufsize, ++in) {
            std::strncpy(ref, Rcpp::String(*in).get_cstring(), bufsize-1);
            ref[bufsize-1]='\0'; // strncpy only pads up to just before the last position.
        }
    }
    mat.insert_row(r, row_buf.data(), start, end);
    return;
} 

void HDF5_character_output::fill_col(size_t c, Rcpp::StringVector::iterator in, size_t start, size_t end) { 
    if (mat.get_nrow() + start >= end) { // ensure they can fit in 'col_buf'.
        char* ref=col_buf.data();
        for (size_t r=start; r<end; ++r, ref+=bufsize, ++in) {
            std::strncpy(ref, Rcpp::String(*in).get_cstring(), bufsize-1);
            ref[bufsize-1]='\0';
        }
    }
    mat.insert_col(c, col_buf.data(), start, end);
    return;
}
 
void HDF5_character_output::fill(size_t r, size_t c, Rcpp::String in) { 
    char* ref=one_buf.data();
    std::strncpy(ref, in.get_cstring(), bufsize-1);
    ref[bufsize-1]='\0';
    mat.insert_one(r, c, ref);
    return;
}

Rcpp::RObject HDF5_character_output::yield() {
    return mat.yield();
}

std::unique_ptr<character_output> HDF5_character_output::clone() const {
    return std::unique_ptr<character_output>(new HDF5_character_output(*this));
}
 
matrix_type HDF5_character_output::get_matrix_type() const {
    return mat.get_matrix_type();
}

/* Dispatch definition */

std::unique_ptr<character_output> create_character_output(int nrow, int ncol, const output_param& param) {
    switch (param.get_mode()) {
        case SIMPLE:
            return std::unique_ptr<character_output>(new simple_character_output(nrow, ncol));
        case HDF5:
            return std::unique_ptr<character_output>(new HDF5_character_output(nrow, ncol,
                        param.get_strlen(), param.get_chunk_nrow(), param.get_chunk_ncol(), param.get_compression()));
        default:
            throw std::runtime_error("unsupported output mode for character matrices");
    }
}

}
