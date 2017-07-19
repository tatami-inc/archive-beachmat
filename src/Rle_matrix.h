#ifndef BEACHMAT_RLE_MATRIX_H
#define BEACHMAT_RLE_MATRIX_H

#include "beachmat.h"
#include "utils.h"
#include "any_matrix.h"

namespace beachmat {

/*** Class definition ***/

template<typename T, class V>
class Rle_matrix : public any_matrix {
public:
    Rle_matrix(const Rcpp::RObject&);
    ~Rle_matrix();

    T get(size_t, size_t);

    template<class Iter>
    void get_row(size_t, Iter, size_t, size_t); 

    template<class Iter>
    void get_col(size_t, Iter, size_t, size_t); 

    matrix_type get_matrix_type () const;
private:
    std::deque<V> runvalues;
    std::vector<size_t> chunkdex, coldex;
    std::vector<std::deque<size_t> > cumrow;

    void initialize_solid_rle(const Rcpp::RObject&);
    void initialize_chunked_rle(const Rcpp::RObject&);
    size_t parse_rle(const Rcpp::IntegerVector&, size_t);

    size_t cache_row, cache_start, cache_end;
    std::vector<size_t> cache_indices;
    void update_indices(size_t r, size_t first, size_t last);
};

/*** Constructor definitions ***/

template<typename T, class V>
Rle_matrix<T, V>::Rle_matrix(const Rcpp::RObject& incoming) {
    std::string ctype=get_class(incoming);
    if (!incoming.isS4() || ctype!="RleMatrix") { 
        throw std::runtime_error("matrix should be a RleMatrix object");
    }

    const Rcpp::RObject& rle_seed=get_safe_slot(incoming, "seed");
    std::string stype=get_class(rle_seed);
    if (!rle_seed.isS4() || (stype!="ChunkedRleArraySeed" && stype!="SolidRleArraySeed")) {
        throw_custom_error("'seed' slot in a ", ctype, " object should be a Chunked/SolidRleArraySeed object");
    }

    // Setting the dimensions.
    this->fill_dims(get_safe_slot(rle_seed, "DIM"));

    /* The chunkdex vector contains the index of the chunk of "runvalues" containing the Rle for each column.
     * The coldex vector contains the index on the corresponding chunk of "runvalues" where each column starts.
     * The cumrow vector contains the cumulative row for each column, taken by adding all the RLE lengths within a column.
     * Each cumulative row corresponds to a value in a chunk of "runvalues".
     */
    const size_t& NC=this->ncol;
    chunkdex.resize(NC);
    coldex.resize(NC + 1);
    cumrow.resize(NC);
 
    // Figuring out which _type_ of Rle to set up.
    if (stype=="SolidRleArraySeed") { 
        initialize_solid_rle(rle_seed);
    } else if (stype=="ChunkedRleArraySeed") {
        initialize_chunked_rle(rle_seed);
    }  
  
    // Setting up the cached indices.
    cache_row=0;
    cache_start=0;
    cache_end=0;
    cache_indices.resize(this->ncol);
    return;
}

template<typename T, class V>
void Rle_matrix<T, V>::initialize_solid_rle(const Rcpp::RObject& rle_seed) {
    const Rcpp::RObject& rle=get_safe_slot(rle_seed, "rle");
    std::string rtype=get_class(rle);
    if (!rle.isS4() || rtype!="Rle") {
        throw_custom_error("'rle' slot in a ", get_class(rle_seed), " object should be a Rle object");
    }

    Rcpp::RObject temp_rv=get_safe_slot(rle, "values");
    if (temp_rv.sexp_type()!=V().sexp_type()) { 
        std::stringstream err;
        err << "'values' slot in a " << get_class(rle) << " object should be " << translate_type(V().sexp_type());
        throw std::runtime_error(err.str().c_str());
    }
    runvalues.push_back(V(temp_rv));

    const Rcpp::RObject& temp_rl=get_safe_slot(rle, "lengths");
    if (temp_rl.sexp_type()!=INTSXP) { 
        throw_custom_error("'lengths' slot in a ", rtype, " object should be integer"); 
    }
    Rcpp::IntegerVector runlength(temp_rl);
    if (runlength.size()!=runvalues.back().size()) { 
        throw std::runtime_error("run value and length vectors should be of the same length"); 
    }

    // In this case, the entire data set is a single chunk, so the chunk index is trivially set to all-zeroes.
    size_t last_col=parse_rle(runlength, 0);
    if (last_col!=(this->ncol)) {
        throw std::runtime_error("length of RLE is not consistent with matrix dimensions");
    }
    return;
}

template<typename T, class V>
Rle_matrix<T, V>::~Rle_matrix() {}

/*** Internal Rle parsing functions. ***/

template<typename T, class V>
void Rle_matrix<T, V>::initialize_chunked_rle(const Rcpp::RObject& rle_seed) {
    const Rcpp::RObject& chunkenv=get_safe_slot(rle_seed, "chunks");
    if (chunkenv.sexp_type()!=ENVSXP) {
        throw_custom_error("'chunks' slot in a ", get_class(rle_seed), " objects should be an environment");
    }
    Rcpp::Environment chunks(chunkenv);

    const int expected_type=V().sexp_type();
    size_t col=0;

    // Looping across all Rles contained within the chunk.
    Rcpp::StringVector objects=chunks.ls(false);
    for (auto&& o : objects) {
        Rcpp::RObject rle=chunks.get(Rcpp::as<std::string>(o));
        std::string rtype=get_class(rle);
        if (!rle.isS4() || rtype!="Rle") {
            throw_custom_error("'rle' slot in a ", get_class(rle_seed), " object should be a Rle object");
        }

        Rcpp::RObject temp_rv=get_safe_slot(rle, "values");
        const int observed_type=temp_rv.sexp_type();
        if (observed_type!=expected_type && !(observed_type==RAWSXP && expected_type==INTSXP)) { 
            std::stringstream err;
            err << "'values' slot in a " << get_class(rle) << " object should be " << translate_type(expected_type);
            throw std::runtime_error(err.str().c_str());
        }
        runvalues.push_back(V(temp_rv)); // Coercing RawVector to IntegerVector for simplicity.

        // Pulling out the lengths and parsing them.
        const Rcpp::RObject& temp_rl=get_safe_slot(rle, "lengths");
        if (temp_rl.sexp_type()!=INTSXP) { 
            throw_custom_error("'lengths' slot in a ", rtype, " object should be integer"); 
        }
        Rcpp::IntegerVector runlength(temp_rl);
        if (runlength.size()!=runvalues.back().size()) { 
            throw std::runtime_error("run value and length vectors should be of the same length"); 
        }

        const size_t last_col=parse_rle(runlength, col);
        while (col < last_col) {
            chunkdex[col]=runvalues.size()-1;
            ++col;
        }
    }

    if (col!=this->ncol) { 
        throw std::runtime_error("length of RLE is not consistent with matrix dimensions");
    }
    return;
}

template<typename T, class V>
size_t Rle_matrix<T, V>::parse_rle(const Rcpp::IntegerVector& runlength, size_t startcol) {
    const size_t& NC=this->ncol;
    const size_t& NR=this->nrow;

    std::deque<size_t> tmp_holder;
    coldex[startcol]=0; 
    size_t col=startcol, row=0, counter=0;

    for (auto rlIt=runlength.begin(); rlIt!=runlength.end(); ++rlIt, ++counter) {
        row+=*rlIt;
        if (row >= NR) { 
            // Some work required to account for cases where a single RLE entry runs over into the next column(s).
            while (row >= NR) {
                tmp_holder.push_back(NR);
                row -= NR;
                if (col==NC) {
                    throw std::runtime_error("length of RLE is inconsistent with matrix dimensions");
                }
                cumrow[col].swap(tmp_holder);
                ++col;
                coldex[col]=counter; // Current value is the start of the next column.
            }

            if (row > 0) { 
                if (col==NC) {
                    throw std::runtime_error("length of RLE is inconsistent with matrix dimensions");
                }
                tmp_holder.push_back(row);
            } else {
                ++(coldex[col]); // Next value is the start of the next column, if the run expires at the end of the last column.
            }
        } else {
            tmp_holder.push_back(row);
        }
    }
    if (row!=0) {
        throw std::runtime_error("RLE did not reach the end of the column");
    }
    return col;
}

/*** Getter methods ***/

template<typename T, class V>
template<class Iter>
void Rle_matrix<T, V>::get_col(size_t c, Iter out, size_t first, size_t last) {
    check_colargs(c, first, last);

    const auto& curcol=cumrow[c];
    auto rvIt=runvalues[chunkdex[c]].begin() + coldex[c];
    auto ccIt=curcol.begin();
    if (first) {
        auto tmp_ccIt=std::upper_bound(ccIt, curcol.end(), first); // jump to first.
        rvIt+=tmp_ccIt - ccIt;
        ccIt=tmp_ccIt;
    }

    size_t current_row=first;
    while (current_row < last) {
        const size_t n_to_add=std::min(last, *ccIt) - current_row;
        std::fill(out, out+n_to_add, *rvIt);
        current_row=*ccIt;
        ++ccIt;
        ++rvIt;
        out+=n_to_add;
    }

    return;
}

template<typename T, class V>
void Rle_matrix<T, V>::update_indices(size_t r, size_t first, size_t last) {
    if (cache_start!=first|| cache_end!=last) {
        // Regenerate if they don't match; too much effort to keep track of which ones are valid.
        cache_start=first;
        cache_end=last;
        std::fill(cache_indices.begin() + first, cache_indices.begin() + last, 0);
        cache_row=0;
    }

    /* Index for column 'c' should hold the point at cumrow[c] that is greater than 'r'.
     * We use an upper bound because a cumulative row of 1 corresponds to a row index of 0.
     */
    if (r==cache_row) {
        return;
    }
    if (r==cache_row+1) {
        for (size_t c=first; c<last; ++c) {
            size_t& curIndex=cache_indices[c];
            if (cumrow[c][curIndex] <= r) {
                ++curIndex;
            }            
        }        
    } else if (r+1==cache_row) {
        for (size_t c=first; c<last; ++c) {
            size_t& curIndex=cache_indices[c];
            if (curIndex) {
                if (cumrow[c][curIndex-1] > r) {
                    --curIndex;
                }
            }        
        }
    } else if (r > cache_row) {
        for (size_t c=first; c<last; ++c) {
            const auto& curcol=cumrow[c];
            size_t& curIndex=cache_indices[c];
            auto rdIt=std::upper_bound(curcol.begin() + curIndex, curcol.end(), r);
            curIndex=rdIt - curcol.begin();            
        }
    } else if (r < cache_row) {
        for (size_t c=first; c<last; ++c) {
            const auto& curcol=cumrow[c];
            size_t& curIndex=cache_indices[c];
            auto rdIt=std::upper_bound(curcol.begin(), curcol.begin() + curIndex, r);
            curIndex=rdIt - curcol.begin();            
        }
    }

    cache_row=r;
    return;
}

template<typename T, class V>
template<class Iter>
void Rle_matrix<T, V>::get_row(size_t r, Iter out, size_t first, size_t last) {
    check_rowargs(r, first, last);
    update_indices(r, first, last);
    for (size_t c=first; c<last; ++c, ++out) {
        (*out)=*(runvalues[chunkdex[c]].begin() + cache_indices[c] + coldex[c]);
    }
    return;
}

template<typename T, class V>
T Rle_matrix<T, V>::get(size_t r, size_t c) {
    check_oneargs(r, c);
    const auto& curcol=cumrow[c];
    size_t extra=std::upper_bound(curcol.begin(), curcol.end(), r) - curcol.begin();
    return *(runvalues[chunkdex[c]].begin() + coldex[c] + extra);
}

template<typename T, class V>
matrix_type Rle_matrix<T, V>::get_matrix_type() const {
    return RLE;
}

}

#endif
