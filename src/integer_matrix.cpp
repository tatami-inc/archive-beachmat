#include "integer_matrix.h"

/* Methods for the base class. */

integer_matrix::integer_matrix() {}

integer_matrix::~integer_matrix() {}

/* Methods for a simple matrix. */

simple_integer_matrix::simple_integer_matrix(SEXP incoming) : simple_matrix(incoming), simple_ptr(NULL), 
        row_data(ncol), row_ptr(row_data.data()) { 

    if (!isInteger(incoming)) { 
        throw std::runtime_error("matrix should be integer");
    }
    simple_ptr=INTEGER(incoming);
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

HDF5_integer_matrix::HDF5_integer_matrix(SEXP incoming) : HDF5_matrix(incoming), 
        row_data(ncol), col_data(nrow), row_ptr(row_data.data()), col_ptr(col_data.data()) {

    SEXP h5_seed=R_do_slot(incoming, install("seed")); 
    SEXP firstval=R_do_slot(h5_seed, install("first_val"));
    if (!isInteger(firstval)) { 
        throw std::runtime_error("'first_val' should be integer");
    }
    if (hdata.getTypeClass()!=H5T_INTEGER) { 
        throw std::runtime_error("data type in HDF5 file is not integer");
    }

    col_data.resize(nrow);
    col_ptr=col_data.data();
    row_data.resize(ncol);
    row_ptr=row_data.data();
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

/* Dispatch definition */

std::shared_ptr<integer_matrix> create_integer_matrix(SEXP incoming) { 
    if (IS_S4_OBJECT(incoming)) {
        const char* ctype=get_class(incoming);
        if (std::strcmp(ctype, "HDF5Matrix")==0) { 
            return std::shared_ptr<integer_matrix>(new HDF5_integer_matrix(incoming));
        }
        std::stringstream err;
        err << "unsupported class '" << ctype << "' for integer_matrix";
        throw std::runtime_error(err.str().c_str());
    } 
    return std::shared_ptr<integer_matrix>(new simple_integer_matrix(incoming));
}

