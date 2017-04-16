#include "integer_matrix.h"

/* Methods for the base class. */

integer_matrix::integer_matrix() {}

integer_matrix::~integer_matrix() {}

/* Methods for a simple matrix. */

simple_integer_matrix::simple_integer_matrix(const Rcpp::RObject& incoming) : simple_matrix(incoming), simple_ptr(NULL), 
        row_data(ncol), row_ptr(row_data.data()) { 

    if (obj.sexp_type()!=INTSXP) { 
        throw std::runtime_error("matrix should be integer");
    }
    simple_ptr=INTEGER(obj.get__());
    return;
}

simple_integer_matrix::~simple_integer_matrix () {}

const int* simple_integer_matrix::get_row(int r) {
    return get_row_inside<int>(simple_ptr, r, row_ptr);
}

const int* simple_integer_matrix::get_col(int c) {
    return get_col_inside<int>(simple_ptr, c);
}

int simple_integer_matrix::get(int r, int c) {
    return simple_ptr[get_index(r, c)];
}

/* Methods for a HDF5 matrix. */

#ifdef BEACHMAT_USE_HDF5

HDF5_integer_matrix::HDF5_integer_matrix(const Rcpp::RObject& incoming) : HDF5_matrix(incoming), 
        row_data(ncol), col_data(nrow), row_ptr(row_data.data()), col_ptr(col_data.data()) {

    const Rcpp::RObject& h5_seed=incoming.slot("seed");
    const Rcpp::RObject& firstval=get_safe_slot(h5_seed, "first_val");
    if (firstval.sexp_type()!=INTSXP) {
        throw_custom_error("'first_val' slot in a ", get_class(h5_seed), " object should be integer");
    }
    if (hdata.getTypeClass()!=H5T_INTEGER) { 
        throw std::runtime_error("data type in HDF5 file is not integer");
    }
    return;
}

HDF5_integer_matrix::~HDF5_integer_matrix() {}

const int * HDF5_integer_matrix::get_row(int r) { 
    return get_row_inside<int>(r, row_ptr, H5::PredType::NATIVE_INT32);
} 

const int * HDF5_integer_matrix::get_col(int c) { 
    return get_col_inside<int>(c, col_ptr, H5::PredType::NATIVE_INT32);
}

int HDF5_integer_matrix::get(int r, int c) { 
    return get_one_inside<int>(r, c, H5::PredType::NATIVE_INT32);
}

#endif

/* Dispatch definition */

std::shared_ptr<integer_matrix> create_integer_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) { 
        std::string ctype=get_class(incoming);
        if (ctype=="HDF5Matrix") { 
#ifdef BEACHMAT_USE_HDF5
            return std::shared_ptr<integer_matrix>(new HDF5_integer_matrix(incoming));
#else
            throw std::runtime_error("'beachmat' not compiled with HDF5 support");
#endif            
        }
        std::stringstream err;
        err << "unsupported class '" << ctype << "' for integer_matrix";
        throw std::runtime_error(err.str().c_str());
    } 
    return std::shared_ptr<integer_matrix>(new simple_integer_matrix(incoming));
}

