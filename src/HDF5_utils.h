#ifndef BEACHMAT_HDF5_UTILS_H
#define BEACHMAT_HDF5_UTILS_H

#include "beachmat.h"
#include "utils.h"

namespace beachmat { 

size_t get_cache_size_hard_limit();

void calc_HDF5_chunk_cache_settings (const size_t, const size_t, const H5::DSetCreatPropList&, const H5::DataType&,
        bool&, bool&, bool&, bool&, bool&, bool&,
        H5::FileAccPropList&, H5::FileAccPropList&);

void reopen_HDF5_file_by_dim(const std::string&, const std::string&, 
        H5::H5File&, H5::DataSet&, const unsigned&, const H5::FileAccPropList&,
        bool&, const bool&, const bool&, const bool&);

void HDF5_select_row(const size_t&, const size_t&, const size_t&,
        hsize_t*, hsize_t*, 
        H5::DataSpace&, H5::DataSpace&);

void HDF5_select_col(const size_t&, const size_t&, const size_t&,
        hsize_t*, hsize_t*, 
        H5::DataSpace&, H5::DataSpace&);

void HDF5_select_one(const size_t&, const size_t&,
        hsize_t*, hsize_t*, 
        H5::DataSpace& hspace);

H5::DataType set_HDF5_data_type (int, const H5::DataSet&);

H5::DataType set_HDF5_data_type (int, size_t);

void initialize_HDF5_size_arrays (const size_t&, const size_t&,
        hsize_t*, hsize_t*, hsize_t*, 
        hsize_t*, H5::DataSpace&);

}

#endif
