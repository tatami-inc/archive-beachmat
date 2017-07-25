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

void output_param::optimize_chunk_dims(size_t nr, size_t nc) {
    if (!nr || !nc) {
        chunk_nr=0;
        chunk_nc=0;
        return;
    }

    // Computes the optimal chunk dimensions given the total number of rows/columns.
    // This is a heuristic approach assuming that all values are continuous (but
    // taking the ceiling to err on the side of conservativeness).
    double dnc=nc, dnr=nr;
    size_t cachesize;
    if (nr > nc) {
        cachesize=std::ceil(dnr*std::sqrt(dnc));
    } else {
        cachesize=std::ceil(dnc*std::sqrt(dnr));
    }
    chunk_nr=std::ceil(cachesize/dnc); 
    chunk_nc=std::ceil(cachesize/dnr);

    // Further work to guarantee that it involves fewer reads.
    chunk_nr=std::max(chunk_nr, size_t(std::ceil(dnc/chunk_nc)));
    chunk_nc=std::max(chunk_nc, size_t(std::ceil(dnr/chunk_nr)));
    return;
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
