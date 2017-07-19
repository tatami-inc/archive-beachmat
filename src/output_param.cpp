#include "output_param.h"

namespace beachmat {

output_param::output_param (matrix_type m) : mode(m), chunk_nr(DEFAULT_CHUNKDIM), chunk_nc(DEFAULT_CHUNKDIM), 
    compress(DEFAULT_COMPRESS), strlen(DEFAULT_STRLEN) {}

output_param::output_param (const Rcpp::RObject& in, bool simplify, bool preserve_zero) : output_param(SIMPLE) { 
    if (!in.isS4()) {
        return;
    } 

    auto curclass=get_class(in);
    if (curclass=="HDF5Array" || curclass=="DelayedArray") { 
        mode=HDF5;
        return;
    }

    if (simplify) {
        return;
    }

    if (preserve_zero && !curclass.empty() && curclass.substr(1)=="gCMatrix") {
        mode=SPARSE;
        return;
    } 

    mode=HDF5;
    return;
}

output_param::output_param(matrix_type m, bool simplify, bool preserve_zero) : output_param(m) {
    switch (mode) {
        case SIMPLE: case HDF5: // keeping to the two extremes.
            break;
        case SPARSE:
            if (preserve_zero) { break; } // keeping sparse, if preserve_zero is true.
        default:
            mode=(simplify ? SIMPLE : HDF5); // going to the relevant extreme.
    }
    return; 
}

matrix_type output_param::get_mode() const {
    return mode;
}

void output_param::set_chunk_dim(size_t cnr, size_t cnc) {
    chunk_nr=cnr;
    chunk_nc=cnc;
    return;
}

size_t output_param::get_chunk_nrow() const {
    return chunk_nr;
}
        
size_t output_param::get_chunk_ncol() const {
    return chunk_nc;
}

int output_param::get_compression() const {
    return compress;
}

void output_param::set_compression(int c) {
    compress=c;
    return;
}

void output_param::set_strlen(size_t s) {
    strlen=s;
    return;
}

size_t output_param::get_strlen() const {
    return strlen;
}

const output_param SIMPLE_PARAM(SIMPLE);
const output_param SPARSE_PARAM(SPARSE);
const output_param HDF5_PARAM(HDF5);

}
