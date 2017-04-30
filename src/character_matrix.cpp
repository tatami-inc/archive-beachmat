#include "character_matrix.h"
#include "hdf5.h"

namespace beachmat {

/* Methods for the HDF5 character matrix. */

HDF5_character_matrix::HDF5_character_matrix(const Rcpp::RObject& incoming) : HDF5_matrix<const char*, STRSXP, H5T_STRING, H5::PredType::C_S1>(incoming), 
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

void HDF5_character_matrix::get_row(int r, const char** out) { 
    offset[0] = 0;
    offset[1] = r;
    hspace.selectHyperslab(H5S_SELECT_SET, rows_out, offset);

    char* ref=row_buf.data();
    hdata.read(ref, str_type, rowspace, hspace);
    auto bufsize=str_type.getSize(); 

    const int& NC=this->ncol;
    for (int c=0; c<NC; ++c, ref+=bufsize) {
        out[c]=CHAR(Rf_mkChar(ref)); // store in R's cache.
    }
    return;
} 

void HDF5_character_matrix::get_col(int c, const char** out) { 
    offset[0] = c;
    offset[1] = 0;
    hspace.selectHyperslab(H5S_SELECT_SET, cols_out, offset);

    char* ref=col_buf.data();
    hdata.read(ref, str_type, colspace, hspace);
    auto bufsize=str_type.getSize(); 

    const int& NR=this->nrow;
    for (int r=0; r<NR; ++r, ref+=bufsize) {
        out[r]=CHAR(Rf_mkChar(ref)); // store in R's cache.
    }
    return;
}
 
const char* HDF5_character_matrix::get(int r, int c) { 
    offset[0]=c;
    offset[1]=r;
    hspace.selectHyperslab(H5S_SELECT_SET, one_out, offset);
    std::string out;
    hdata.read(out, str_type, onespace, hspace);
    return CHAR(Rf_mkChar(out.c_str())); // store in R's cache.
}

/* Dispatch definition */

std::shared_ptr<character_matrix> create_character_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) { 
        std::string ctype=get_class(incoming);
        if (ctype=="HDF5Matrix") { 
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
