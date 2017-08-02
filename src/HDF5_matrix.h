#ifndef BEACHMAT_HDF5_MATRIX_H
#define BEACHMAT_HDF5_MATRIX_H

#include "beachmat.h"
#include "any_matrix.h"
#include "HDF5_utils.h"

namespace beachmat {

/*** Class definition ***/

template<typename T, int RTYPE>
class HDF5_matrix : public any_matrix {
public:
    HDF5_matrix(const Rcpp::RObject&);
    ~HDF5_matrix();

    void extract_row(size_t, T*, size_t, size_t);
    template<typename X>
    void extract_row(size_t, X*, const H5::DataType&, size_t, size_t);

    void extract_col(size_t, T*, size_t, size_t);
    template<typename X>
    void extract_col(size_t, X*, const H5::DataType&, size_t, size_t);
    
    void extract_one(size_t, size_t, T*); // Use of pointer is a bit circuitous, but necessary for character access.
    template<typename X>
    void extract_one(size_t, size_t, X*, const H5::DataType&);  

    const H5::DataType& get_datatype() const;

    Rcpp::RObject yield() const;
    matrix_type get_matrix_type() const;
protected:
    Rcpp::RObject original;
    std::string filename, dataname;

    H5::H5File hfile;
    H5::DataSet hdata;
    H5::DataSpace hspace, rowspace, colspace, onespace;
    hsize_t h5_start[2], col_count[2], row_count[2], one_count[2];

    H5::DataType default_type;

    bool onrow, oncol;
    bool rowokay, colokay;
    bool largerrow, largercol;
    H5::FileAccPropList rowlist, collist;
};

/*** Constructor definition ***/

template<typename T, int RTYPE>
HDF5_matrix<T, RTYPE>::HDF5_matrix(const Rcpp::RObject& incoming) : original(incoming),
        rowlist(H5::FileAccPropList::DEFAULT.getId()), collist(H5::FileAccPropList::DEFAULT.getId()) {

    std::string ctype=get_class(incoming);
    if (!incoming.isS4() || ctype!="HDF5Matrix") {
        throw std::runtime_error("matrix should be a HDF5Matrix or DelayedMatrix object");
    }

    const Rcpp::RObject& h5_seed=get_safe_slot(incoming, "seed");
    std::string stype=get_class(h5_seed);
    if (!h5_seed.isS4() || stype!="HDF5ArraySeed") {
        throw_custom_error("'seed' slot in a ", ctype, " object should be a HDF5ArraySeed object");
    }

    // Checking first value.
    const Rcpp::RObject& firstval=get_safe_slot(h5_seed, "first_val");
    if (firstval.sexp_type()!=RTYPE) { 
        std::stringstream err;
        err << "'first_val' slot in a " << get_class(h5_seed) << " object should be " << translate_type(RTYPE);
        throw std::runtime_error(err.str().c_str());
    }

    // Checking dimensions.
    if (!h5_seed.hasAttribute("dim")) { 
        throw_custom_error("", stype, " object should have 'dim' attribute"); 
    }
    this->fill_dims(h5_seed.attr("dim"));
    const size_t& NC=this->ncol;
    const size_t& NR=this->nrow;

    // Checking names.
    try {
        filename=make_to_string(get_safe_slot(h5_seed, "file"));
    } catch (...) { 
        throw_custom_error("'file' slot in a ", stype, " object should be a string");
    }
    try {
        dataname=make_to_string(get_safe_slot(h5_seed, "name"));
    } catch (...) { 
        throw_custom_error("'name' slot in a ", stype, " object should be a string");
    }
    
    // Setting up the HDF5 accessors.
    hfile.openFile(filename.c_str(), H5F_ACC_RDONLY);
    hdata = hfile.openDataSet(dataname.c_str());
    default_type=set_HDF5_data_type(RTYPE, hdata);

    hspace = hdata.getSpace();
    if (hspace.getSimpleExtentNdims()!=2) {
        throw std::runtime_error("data in HDF5 file is not a two-dimensional array");
    }

    hsize_t dims_out[2];
    hspace.getSimpleExtentDims(dims_out, NULL);
    if (dims_out[1]!=NR || dims_out[0]!=NC) { 
        throw_custom_error("dimensions in HDF5 file do not equal dimensions in the ", ctype, " object");
    }

    // Setting up the hsize_t[2] arrays.
    initialize_HDF5_size_arrays(NR, NC, 
            h5_start, col_count, row_count, 
            one_count, onespace);

    // Setting the chunk cache parameters.
    calc_HDF5_chunk_cache_settings(this->nrow, this->ncol, hdata.getCreatePlist(), default_type, 
            onrow, oncol, rowokay, colokay, largerrow, largercol, rowlist, collist);
    return;
}

template<typename T, int RTYPE>
HDF5_matrix<T, RTYPE>::~HDF5_matrix() {}

/*** Getter functions ***/

template<typename T, int RTYPE>
template<typename X>
void HDF5_matrix<T, RTYPE>::extract_row(size_t r, X* out, const H5::DataType& HDT, size_t first, size_t last) { 
    check_rowargs(r, first, last);
    reopen_HDF5_file_by_dim(filename, dataname, 
            hfile, hdata, H5F_ACC_RDONLY, rowlist, 
            onrow, oncol, largercol, rowokay);
    HDF5_select_row(r, first, last, row_count, h5_start, rowspace, hspace);
    hdata.read(out, HDT, rowspace, hspace);
    return;
}

template<typename T, int RTYPE>
void HDF5_matrix<T, RTYPE>::extract_row(size_t r, T* out, size_t first, size_t last) { 
    extract_row(r, out, default_type, first, last);
    return;
}

template<typename T, int RTYPE>
template<typename X>
void HDF5_matrix<T, RTYPE>::extract_col(size_t c, X* out, const H5::DataType& HDT, size_t first, size_t last) { 
    check_colargs(c, first, last);
    reopen_HDF5_file_by_dim(filename, dataname, 
            hfile, hdata, H5F_ACC_RDONLY, collist, 
            oncol, onrow, largerrow, colokay);
    HDF5_select_col(c, first, last, col_count, h5_start, colspace, hspace);
    hdata.read(out, HDT, colspace, hspace);
    return;
}
    
template<typename T, int RTYPE>
void HDF5_matrix<T, RTYPE>::extract_col(size_t c, T* out, size_t first, size_t last) { 
    extract_col(c, out, default_type, first, last);
    return;
}

template<typename T, int RTYPE>
template<typename X>
void HDF5_matrix<T, RTYPE>::extract_one(size_t r, size_t c, X* out, const H5::DataType& HDT) { 
    check_oneargs(r, c);
    HDF5_select_one(r, c, one_count, h5_start, hspace);
    hdata.read(out, HDT, onespace, hspace);
    return;
}

template<typename T, int RTYPE>
void HDF5_matrix<T, RTYPE>::extract_one(size_t r, size_t c, T* out) { 
    extract_one(r, c, out, default_type);
    return;
}

template<typename T, int RTYPE>
const H5::DataType& HDF5_matrix<T, RTYPE>::get_datatype() const { 
    return default_type;
}

template<typename T, int RTYPE>
Rcpp::RObject HDF5_matrix<T, RTYPE>::yield() const {
    return original;
}

template<typename T, int RTYPE>
matrix_type HDF5_matrix<T, RTYPE>::get_matrix_type() const {
    return HDF5;
}

}

#endif
