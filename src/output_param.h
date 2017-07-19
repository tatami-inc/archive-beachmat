#ifndef BEACHMAT_OUTPUT_PARAM_H
#define BEACHMAT_OUTPUT_PARAM_H

#include "utils.h"

namespace beachmat {

class output_param {
public:
    output_param(matrix_type);
    output_param(const Rcpp::RObject&, bool=false, bool=false);
    output_param(matrix_type, bool, bool=false);
    
    matrix_type get_mode() const;

    void set_chunk_dim(size_t, size_t);
    size_t get_chunk_nrow() const;
    size_t get_chunk_ncol() const;
    void optimize_chunk_dims(size_t, size_t);

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
