#include "character_matrix.h"

namespace beachmat {

#ifdef BEACHMAT_USE_HDF5

/* Methods for the HDF5 character matrix. */

HDF5_character_matrix::HDF5_character_matrix(const Rcpp::RObject& incoming) : 
        HDF5_matrix<Rcpp::String, Rcpp::StringVector>(incoming, H5T_STRING, H5::PredType::NATIVE_INT32), 
        str_type(hdata) {
    // H5::PredType::NATIVE_INT32 just to give it something to work with.
    // Not really necessary here, as we override get* methods anyway, so it never actually gets used. 
            
    if (!str_type.isVariableStr()) { 
        auto bufsize=str_type.getSize(); 
        row_buf.resize(bufsize*(this->ncol));
        col_buf.resize(bufsize*(this->nrow));
    } else {
        throw std::runtime_error("variable-length strings not supported for HDF5_character_matrix");
    }
}

HDF5_character_matrix::~HDF5_character_matrix() {}

std::unique_ptr<character_matrix> HDF5_character_matrix::clone() const {
    return std::unique_ptr<character_matrix>(new HDF5_character_matrix(*this));
}
    
void HDF5_character_matrix::get_row(size_t r, Rcpp::StringVector::iterator out, size_t start, size_t end) { 
    select_row(r, start, end);

    char* ref=row_buf.data();
    hdata.read(ref, str_type, rowspace, hspace);
    auto bufsize=str_type.getSize(); 

    for (size_t c=start; c<end; ++c, ref+=bufsize, ++out) {
        (*out)=ref; 
    }
    return;
} 

void HDF5_character_matrix::get_col(size_t c, Rcpp::StringVector::iterator out, size_t start, size_t end) { 
    select_col(c, start, end);

    char* ref=col_buf.data();
    hdata.read(ref, str_type, colspace, hspace);
    auto bufsize=str_type.getSize(); 

    for (size_t r=start; r<end; ++r, ref+=bufsize, ++out) {
        (*out)=ref; 
    }
    return;
}
 
Rcpp::String HDF5_character_matrix::get(size_t r, size_t c) { 
    select_one(r, c);
    std::string out;
    hdata.read(out, str_type, onespace, hspace);
    return out;
}

#endif

/* Dispatch definition */

std::unique_ptr<character_matrix> create_character_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) { 
        std::string ctype=get_class(incoming);
        if (ctype=="HDF5Matrix" || ctype=="DelayedMatrix") { 
#ifdef BEACHMAT_USE_HDF5
            return std::unique_ptr<character_matrix>(new HDF5_character_matrix(incoming));
#else
            throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
        }
        std::stringstream err;
        err << "unsupported class '" << ctype << "' for character_matrix";
        throw std::runtime_error(err.str().c_str());
    } 
    return std::unique_ptr<character_matrix>(new simple_character_matrix(incoming));
}

}
