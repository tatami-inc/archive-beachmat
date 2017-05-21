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

/* Methods for the simple character matrix. */

simple_character_matrix::simple_character_matrix(const Rcpp::RObject& incoming) : mat(incoming) {}

simple_character_matrix::~simple_character_matrix() {}

size_t simple_character_matrix::get_nrow() const {
    return mat.get_nrow();
}

size_t simple_character_matrix::get_ncol() const {
    return mat.get_ncol();
}

void simple_character_matrix::get_row(size_t r, Rcpp::StringVector::iterator out, size_t start, size_t end) { 
    mat.get_row(r, out, start, end);
}

void simple_character_matrix::get_col(size_t c, Rcpp::StringVector::iterator out, size_t start, size_t end) { 
    mat.get_col(c, out, start, end);
}

Rcpp::String simple_character_matrix::get(size_t r, size_t c) {
    return mat.get(r, c);
}

std::unique_ptr<character_matrix> simple_character_matrix::clone() const {
    return std::unique_ptr<character_matrix>(new simple_character_matrix(*this));
}

/* Methods for the HDF5 character matrix. */

HDF5_character_matrix::HDF5_character_matrix(const Rcpp::RObject& incoming) : 
        mat(incoming, STRSXP, H5T_STRING, H5::PredType::NATIVE_INT32), str_type(mat.hdata) {
    // H5::PredType::NATIVE_INT32 just to give it something to work with.
    // Not really necessary here, as we override get* methods anyway, so it never actually gets used. 
            
    if (!str_type.isVariableStr()) { 
        auto bufsize=str_type.getSize(); 
        row_buf.resize(bufsize*(mat.get_ncol()));
        col_buf.resize(bufsize*(mat.get_nrow()));
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

void HDF5_character_matrix::get_row(size_t r, Rcpp::StringVector::iterator out, size_t start, size_t end) { 
    mat.select_row(r, start, end);

    char* ref=row_buf.data();
    mat.hdata.read(ref, str_type, mat.rowspace, mat.hspace);
    auto bufsize=str_type.getSize(); 

    for (size_t c=start; c<end; ++c, ref+=bufsize, ++out) {
        (*out)=ref; 
    }
    return;
} 

void HDF5_character_matrix::get_col(size_t c, Rcpp::StringVector::iterator out, size_t start, size_t end) { 
    mat.select_col(c, start, end);

    char* ref=col_buf.data();
    mat.hdata.read(ref, str_type, mat.colspace, mat.hspace);
    auto bufsize=str_type.getSize(); 

    for (size_t r=start; r<end; ++r, ref+=bufsize, ++out) {
        (*out)=ref; 
    }
    return;
}
 
Rcpp::String HDF5_character_matrix::get(size_t r, size_t c) { 
    mat.select_one(r, c);
    std::string out;
    mat.hdata.read(out, str_type, mat.onespace, mat.hspace);
    return out;
}

std::unique_ptr<character_matrix> HDF5_character_matrix::clone() const {
    return std::unique_ptr<character_matrix>(new HDF5_character_matrix(*this));
}
   
/* Dispatch definition */

std::unique_ptr<character_matrix> create_character_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) { 
        std::string ctype=get_class(incoming);
        if (ctype=="HDF5Matrix" || ctype=="DelayedMatrix") { 
            return std::unique_ptr<character_matrix>(new HDF5_character_matrix(incoming));
        }
        std::stringstream err;
        err << "unsupported class '" << ctype << "' for character_matrix";
        throw std::runtime_error(err.str().c_str());
    } 
    return std::unique_ptr<character_matrix>(new simple_character_matrix(incoming));
}

}
