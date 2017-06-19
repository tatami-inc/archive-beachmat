#include "utils.h"

namespace beachmat {

std::string make_to_string(const Rcpp::RObject& str) {
    if (str.sexp_type()!=STRSXP || Rf_length(str.get__())!=1) {
        throw std::runtime_error("input RObject should contain a single string");
    }
    return Rcpp::as<std::vector<std::string> >(str).front();
}

void throw_custom_error(const std::string& left, const std::string& classname, const std::string& right) {
    std::stringstream err;
    err << left << classname << right;
    throw std::runtime_error(err.str().c_str());
}

/* Class checks. */

std::string get_class(const Rcpp::RObject& incoming) {
    if (!incoming.isObject()) {
        throw std::runtime_error("object has no class attribute");
    }
    return make_to_string(incoming.attr("class"));
}

Rcpp::RObject get_safe_slot(const Rcpp::RObject& incoming, const std::string& slotname) {
    if (!incoming.hasSlot(slotname)) { 
        std::stringstream err;
        err << "no '" << slotname << "' slot in the " << get_class(incoming) << " object";
        throw std::runtime_error(err.str().c_str()); 
    }
    return incoming.slot(slotname);
}

std::string check_Matrix_class (const Rcpp::RObject& mat, const std::string& expected) {
    std::string mattype=get_class(mat);
    if (!mat.isS4() || mattype.empty() || mattype.substr(1)!=expected) {
        throw_custom_error("matrix should be a *", expected, " object");
    }
    return mattype;
}


/* Type checks */

std::string translate_type(int sexp_type) {
    std::string should_be;
    switch(sexp_type) {
        case REALSXP:
            should_be="double";
            break;
        case INTSXP:
            should_be="integer";
            break;
        case LGLSXP:
            should_be="logical";
            break;
        case STRSXP:
            should_be="character";
            break;
        default:
            std::stringstream err;
            err << "unsupported sexptype '" << sexp_type << "'";
            throw std::runtime_error(err.str().c_str());
    }
    return should_be;
}

int reverse_translate_type (const std::string& curtype) {
    if (curtype=="logical") {
        return LGLSXP;
    } else if (curtype=="character") {
        return STRSXP;
    } else if (curtype=="integer") {
        return INTSXP;
    } else if (curtype=="double") {
        return REALSXP;
    }
    std::stringstream err;
    err << "unsupported type'" << curtype << "'";
    throw std::runtime_error(err.str().c_str());
}

int find_sexp_type (const Rcpp::RObject& incoming) {
    if (incoming.isObject()) {
        const std::string classname=get_class(incoming);
        if (classname=="DelayedMatrix") {
            Rcpp::Environment delayenv("package:DelayedArray");
            Rcpp::Function typefun=delayenv["type"];
            std::string curtype=Rcpp::as<std::string>(typefun(incoming));
            return reverse_translate_type(curtype);
            
        } else if (classname=="HDF5Matrix") {
            Rcpp::RObject h5seed=get_safe_slot(incoming, "seed");
            Rcpp::RObject first_val=get_safe_slot(h5seed, "first_val");
            return first_val.sexp_type();

        } else if (classname=="RleMatrix") {
            Rcpp::RObject rleseed=get_safe_slot(incoming, "seed");
            std::string rclass=get_class(rleseed);
            if (rclass=="SolidRleArraySeed") {
                Rcpp::RObject rle=get_safe_slot(rleseed, "rle");
                return get_safe_slot(rle, "values").sexp_type();
            } else if (rclass=="ChunkedRleArraySeed") {
                std::string curtype=Rcpp::as<std::string>(get_safe_slot(rleseed, "type"));
                return reverse_translate_type(curtype);               
            }

        } else if (classname.length()==9 && classname.substr(3)=="Matrix") {
            if (classname[0]=='d') {
                return REALSXP;
            } else if (classname[0]=='l') {
                return LGLSXP;
            }

        }
        throw_custom_error("unknown SEXP type for ", classname, " object");
    }
    return incoming.sexp_type();
}

/* HDF5 utilities. */

size_t get_cache_size_hard_limit () {
    return 2000000000;
}

void calc_HDF5_chunk_cache_settings (const size_t total_nrows, const size_t total_ncols, 
        const H5::DSetCreatPropList& cparms, const H5::DataType& default_type,
        bool& onrow, bool& oncol, bool& rowokay, bool& colokay, bool& largerrow, bool& largercol,
        H5::FileAccPropList& rowlist, H5::FileAccPropList& collist) {

    /* Setting up the chunk cache specification. */
    if (cparms.getLayout()!=H5D_CHUNKED) {
        return;
    }

    hsize_t chunk_dims[2];
    cparms.getChunk(2, chunk_dims);
    const size_t chunk_nrows=chunk_dims[1];
    const size_t chunk_ncols=chunk_dims[0];
    const size_t num_rowchunks=std::ceil(double(total_nrows)/chunk_nrows); // taking the ceiling.
    const size_t num_colchunks=std::ceil(double(total_ncols)/chunk_ncols); 

    // Everything is transposed, so hash indices are filled column-major.
    // Computing the lowest multiple of # row-chunks that is greater than # col-chunks, plus 1.
    const size_t nslots = std::ceil(double(num_colchunks)/num_rowchunks) * num_rowchunks + 1; 

    // Computing the size of the cache required to store all chunks in each row or column.
    const size_t eachchunk=default_type.getSize() * chunk_nrows * chunk_ncols;
    const size_t nchunks_in_cache=get_cache_size_hard_limit()/eachchunk;
    colokay=nchunks_in_cache >= num_colchunks; // This way avoids overflow from eachchunk*num_Xchunks.
    rowokay=nchunks_in_cache >= num_rowchunks;

    const size_t eachrow=eachchunk * num_colchunks; // Need number of chunks in each column, when accessing each row!
    const size_t eachcol=eachchunk * num_rowchunks;
    largercol=eachcol >= eachrow;
    largerrow=eachrow >= eachcol;

    // The first argument is ignored, according to https://support.hdfgroup.org/HDF5/doc/RM/RM_H5P.html.
    // Setting w0 to 0 to evict the last used chunk; no need to worry about full vs partial reads here.
    rowlist.setCache(10000, nslots, eachrow, 0);
    collist.setCache(10000, nslots, eachcol, 0);

    // File is not opened on either row or column yet.
    onrow=false; 
    oncol=false;
    return;
}

/* DelayedArray utilities. */

bool is_pristine_delayed_array(const Rcpp::RObject& in) {
    const Rcpp::Environment env=Rcpp::Environment::namespace_env("DelayedArray");
    Rcpp::Function fun=env["is_pristine"];
    Rcpp::LogicalVector out=fun(in);
    if (out.size()!=1) {
        throw std::runtime_error("pristine check should return a logical scalar");
    }
    return out[0];
}

Rcpp::RObject realize_delayed_array(const Rcpp::RObject& in) {
    Rcpp::Environment delayenv("package:DelayedArray");
    Rcpp::Function realfun=delayenv["realize"];
    return realfun(in);
}

/* Output functions. */

output_mode choose_output_mode(const Rcpp::RObject& in, bool simplify, bool preserve_zero) {
    if (!in.isS4()) {
        return SIMPLE;
    } 
    auto curclass=get_class(in);
    if (curclass=="HDF5Array" || curclass=="DelayedArray") { 
        return HDF5;
    }           
    if (simplify) {
        return SIMPLE;
    }
    if (preserve_zero && !curclass.empty() && curclass.substr(1)=="gCMatrix") {
        return SPARSE;
    } 
    return HDF5;
}

}
