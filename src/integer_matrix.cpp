#include "integer_matrix.h"

/* Methods for the base class. */

integer_matrix::integer_matrix() {}

integer_matrix::~integer_matrix() {}

/* Methods for a simple matrix. */

simple_integer_matrix::simple_integer_matrix(SEXP incoming) : simple_matrix(incoming), simple_ptr(NULL), row_ptr(NULL) {
    if (!isInteger(incoming)) { 
        throw std::runtime_error("matrix should be integer");
    }
    simple_ptr=INTEGER(incoming);

    try {
        row_ptr=new int[ncol];
    } catch (std::exception& e) {
        delete [] row_ptr;
        throw; 
    }
    return;
}

simple_integer_matrix::~simple_integer_matrix () {
    delete [] row_ptr;
    return;
}

const int* simple_integer_matrix::get_row(int r) {
    for (int col=0; col<ncol; ++col) { row_ptr[col]=get(r, col); }
    return row_ptr; 
}

const int* simple_integer_matrix::get_col(int c) {
    return simple_ptr + c*nrow;
}

int simple_integer_matrix::get(int r, int c) {
    return simple_ptr[get_index(r, c)];
}

/* Methods for a HDF5 matrix. */

HDF5_integer_matrix::HDF5_integer_matrix(SEXP incoming) : HDF5_matrix(incoming), row_ptr(NULL), col_ptr(NULL) {
    SEXP h5_seed=R_do_slot(incoming, install("seed")); 
    SEXP firstval=R_do_slot(h5_seed, install("first_val"));
    if (!isInteger(firstval)) { 
        throw std::runtime_error("'first_val' should be integer");
    }
    if (hdata.getTypeClass()!=H5T_INTEGER) { 
        throw std::runtime_error("data type in HDF5 file is not integer");
    }

    try {
        col_ptr=new int[nrow];
        row_ptr=new int[ncol];
    } catch (std::exception& e) {
        delete [] col_ptr;
        delete [] row_ptr;
        throw; 
    }
    return;
}

HDF5_integer_matrix::~HDF5_integer_matrix( ){ 
    delete [] col_ptr;
    delete [] row_ptr;
    return;
}

const int * HDF5_integer_matrix::get_row(int r) { 
    set_row(r);
    hdata.read(row_ptr, H5::PredType::NATIVE_INT32, rowspace, hspace);
    return row_ptr;
} 

const int * HDF5_integer_matrix::get_col(int c) { 
    set_col(c);
    hdata.read(col_ptr, H5::PredType::NATIVE_INT32, colspace, hspace);
    return col_ptr; 
}

int HDF5_integer_matrix::get(int r, int c) { 
    set_one(r, c);
    int out;
    hdata.read(&out, H5::PredType::NATIVE_INT32, onespace, hspace);
    return out; 
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

