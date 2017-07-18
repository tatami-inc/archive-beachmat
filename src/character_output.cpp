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

void character_output::set_col(size_t c, Rcpp::StringVector::iterator out) { 
    set_col(c, out, 0, get_nrow());
}

void character_output::set_row(size_t r, Rcpp::StringVector::iterator out) { 
    set_row(r, out, 0, get_ncol());
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

void simple_character_output::get_row(size_t r, Rcpp::StringVector::iterator out, size_t first, size_t last) { 
    mat.get_row(r, out, first, last);
}

void simple_character_output::get_col(size_t c, Rcpp::StringVector::iterator out, size_t first, size_t last) { 
    mat.get_col(c, out, first, last);
}

Rcpp::String simple_character_output::get(size_t r, size_t c) {
    return mat.get(r, c);
}

void simple_character_output::set_row(size_t r, Rcpp::StringVector::iterator in, size_t first, size_t last) { 
    mat.set_row(r, in, first, last);
}

void simple_character_output::set_col(size_t c, Rcpp::StringVector::iterator in, size_t first, size_t last) { 
    mat.set_col(c, in, first, last);
}

void simple_character_output::set(size_t r, size_t c, Rcpp::String in) {
    mat.set(r, c, in);
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

/* Methods for the HDF5 output matrix. */

template<>
char HDF5_output<char, STRSXP>::get_empty() const { return '\0'; }

template<>
Rcpp::RObject HDF5_output<char, STRSXP>::get_firstval() {
    std::vector<char> first(default_type.getSize());
    extract_one(0, 0, first.data());
    return Rcpp::StringVector::create(Rcpp::String(first.data()));
}

/* Methods for the HDF5 character matrix. */

HDF5_character_output::HDF5_character_output(size_t nr, size_t nc, size_t strlen, size_t chunk_nr, size_t chunk_nc, int compress) :
        bufsize(strlen+1), mat(nr, nc, chunk_nr, chunk_nc, compress, bufsize), 
        row_buf(bufsize*nc), col_buf(bufsize*nr), one_buf(bufsize) {}

HDF5_character_output::~HDF5_character_output() {}

size_t HDF5_character_output::get_nrow() const {
    return mat.get_nrow();
}

size_t HDF5_character_output::get_ncol() const {
    return mat.get_ncol();
}

void HDF5_character_output::get_row(size_t r, Rcpp::StringVector::iterator out, size_t first, size_t last) { 
    char* ref=row_buf.data();
    mat.extract_row(r, ref, first, last);
    for (size_t c=first; c<last; ++c, ref+=bufsize, ++out) {
        (*out)=ref; 
    }
    return;
} 

void HDF5_character_output::get_col(size_t c, Rcpp::StringVector::iterator out, size_t first, size_t last) { 
    char* ref=col_buf.data();
    mat.extract_col(c, ref, first, last);
    for (size_t r=first; r<last; ++r, ref+=bufsize, ++out) {
        (*out)=ref; 
    }
    return;
}
 
Rcpp::String HDF5_character_output::get(size_t r, size_t c) { 
    char* ref=one_buf.data();
    mat.extract_one(r, c, ref);
    return ref;
}

void HDF5_character_output::set_row(size_t r, Rcpp::StringVector::iterator in, size_t first, size_t last) { 
    if (mat.get_ncol() + first >= last) { // ensure they can fit in 'row_buf'; if not, it should trigger an error in insert_row().
        char* ref=row_buf.data();
        for (size_t c=first; c<last; ++c, ref+=bufsize, ++in) {
            std::strncpy(ref, Rcpp::String(*in).get_cstring(), bufsize-1);
            ref[bufsize-1]='\0'; // strncpy only pads up to just before the last position.
        }
    }
    mat.insert_row(r, row_buf.data(), first, last);
    return;
} 

void HDF5_character_output::set_col(size_t c, Rcpp::StringVector::iterator in, size_t first, size_t last) { 
    if (mat.get_nrow() + first >= last) { // ensure they can fit in 'col_buf'.
        char* ref=col_buf.data();
        for (size_t r=first; r<last; ++r, ref+=bufsize, ++in) {
            std::strncpy(ref, Rcpp::String(*in).get_cstring(), bufsize-1);
            ref[bufsize-1]='\0';
        }
    }
    mat.insert_col(c, col_buf.data(), first, last);
    return;
}
 
void HDF5_character_output::set(size_t r, size_t c, Rcpp::String in) { 
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
