#include "character_matrix.h"
#include "hdf5.h"

namespace beachmat {

/* Methods for the HDF5 character matrix. */

HDF5_character_matrix::HDF5_character_matrix(const Rcpp::RObject& incoming) : HDF5_matrix<Rcpp::String, Rcpp::StringVector, H5T_STRING, H5::PredType::C_S1>(incoming), 
    str_type(hdata) {
    if (!H5Tis_variable_str(str_type.getId())) {
        auto bufsize=str_type.getSize(); 
        row_buf.resize(bufsize*(this->ncol));
        col_buf.resize(bufsize*(this->nrow));
    } else {
        throw std::runtime_error("variable-length strings not supported for HDF5_character_matrix");
    }
}

HDF5_character_matrix::~HDF5_character_matrix() {}

std::shared_ptr<character_matrix> HDF5_character_matrix::clone() const {
    return std::shared_ptr<character_matrix>(new HDF5_character_matrix(*this));
}
    
void HDF5_character_matrix::get_row(int r, Rcpp::StringVector::iterator out, int start, int end) { 
    select_row(r, start, end);

    char* ref=row_buf.data();
    hdata.read(ref, str_type, rowspace, hspace);
    auto bufsize=str_type.getSize(); 

    for (int c=start; c<end; ++c, ref+=bufsize, ++out) {
        (*out)=ref; 
    }
    return;
} 

void HDF5_character_matrix::get_col(int c, Rcpp::StringVector::iterator out, int start, int end) { 
    select_col(c, start, end);

    char* ref=col_buf.data();
    hdata.read(ref, str_type, colspace, hspace);
    auto bufsize=str_type.getSize(); 

    for (int r=start; r<end; ++r, ref+=bufsize, ++out) {
        (*out)=ref; 
    }
    return;
}
 
Rcpp::String HDF5_character_matrix::get(int r, int c) { 
    select_one(r, c);
    std::string out;
    hdata.read(out, str_type, onespace, hspace);
    return out;
}

/* Dispatch definition */

std::shared_ptr<character_matrix> create_character_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) { 
        std::string ctype=get_class(incoming);
        if (ctype=="HDF5Matrix" || ctype=="DelayedMatrix") { 
#ifdef BEACHMAT_USE_HDF5
            return std::shared_ptr<character_matrix>(new HDF5_character_matrix(incoming));
#else
            throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
        }
        std::stringstream err;
        err << "unsupported class '" << ctype << "' for character_matrix";
        throw std::runtime_error(err.str().c_str());
    } 
    return std::shared_ptr<character_matrix>(new simple_character_matrix(incoming));
}

}
