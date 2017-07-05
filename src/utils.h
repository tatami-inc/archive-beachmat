#ifndef BEACHMAT_UTILS_H
#define BEACHMAT_UTILS_H

#include "beachmat.h"

namespace beachmat { 

std::string make_to_string(const Rcpp::RObject&);

void throw_custom_error(const std::string&, const std::string&, const std::string&);

// Class checking.

std::string get_class(const Rcpp::RObject&);

Rcpp::RObject get_safe_slot(const Rcpp::RObject&, const std::string&);

std::string check_Matrix_class (const Rcpp::RObject&, const std::string&);

// Type checking.

std::string translate_type(int);

int find_sexp_type (const Rcpp::RObject&);

// HDF5 utilities

size_t get_cache_size_hard_limit();

void calc_HDF5_chunk_cache_settings (const size_t, const size_t, const H5::DSetCreatPropList&, const H5::DataType&,
        bool&, bool&, bool&, bool&, bool&, bool&,
        H5::FileAccPropList&, H5::FileAccPropList&);

// Delayed Array conversion utilities.

bool is_pristine_delayed_array(const Rcpp::RObject&);

Rcpp::RObject realize_delayed_array(const Rcpp::RObject&);

// Output utilities.

enum matrix_type { SIMPLE, HDF5, SPARSE, RLE, PSYMM, DENSE };

class output_param {
public:
    output_param(matrix_type);
    output_param(const Rcpp::RObject&, bool=false, bool=false);
    
    matrix_type get_mode() const;

    void set_chunk_dim(size_t, size_t);
    size_t get_chunk_nrow() const;
    size_t get_chunk_ncol() const;

    int get_compression () const;
    void set_compression (int);

    void set_strlen(size_t);
    size_t get_strlen() const;

    static const size_t DEFAULT_CHUNKDIM=0; // This will trigger use of global chunk settings.
    static const int DEFAULT_COMPRESS=-1; // This will trigger use of global compression settings.
    static const size_t DEFAULT_STRLEN=10;
private:
    matrix_type mode;
    size_t chunk_nr, chunk_nc;
    int compress;
    size_t strlen;
};

extern const output_param SIMPLE_PARAM;
extern const output_param HDF5_PARAM;
extern const output_param SPARSE_PARAM;

}

#endif
