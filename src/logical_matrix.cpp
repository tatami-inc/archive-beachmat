#include "logical_matrix.h"

/* Methods for the base class. */

logical_matrix::logical_matrix() {}

logical_matrix::~logical_matrix() {}

/* Methods for a simple matrix. */

simple_logical_matrix::simple_logical_matrix(const Rcpp::RObject& incoming) : simple_matrix(incoming), simple_ptr(NULL), 
        row_data(ncol), row_ptr(row_data.data()) {

    if (obj.sexp_type()!=LGLSXP) {
        throw std::runtime_error("matrix should be logical");
    }
    simple_ptr=LOGICAL(obj.get__());
    return;
}

simple_logical_matrix::~simple_logical_matrix () {}

const int* simple_logical_matrix::get_row(int r) {
    return get_row_inside<int>(simple_ptr, r, row_ptr); 
}

const int* simple_logical_matrix::get_col(int c) {
    return get_col_inside<int>(simple_ptr, c);
}

int simple_logical_matrix::get(int r, int c) {
    return simple_ptr[get_index(r, c)];
}

/* Methods for a dense Matrix. */

const int* check_Matrix_logical(const Rcpp::RObject& x, const Rcpp::RObject& incoming) { 
    if (x.sexp_type()!=LGLSXP) { 
        throw_custom_error("'x' slot in a ", get_class(incoming), " object should be logical"); 
    }
    return LOGICAL(x.get__());
}

dense_logical_matrix::dense_logical_matrix(const Rcpp::RObject& incoming) : dense_matrix(incoming), dense_ptr(NULL), 
            row_data(ncol), row_ptr(row_data.data()) {

    dense_ptr=check_Matrix_logical(obj_x, incoming);
    return;
}

dense_logical_matrix::~dense_logical_matrix() {}

const int* dense_logical_matrix::get_row(int r) {
    return get_row_inside<int>(dense_ptr, r, row_ptr); 
}

const int* dense_logical_matrix::get_col(int c) {
    return get_col_inside<int>(dense_ptr, c);
}

int dense_logical_matrix::get(int r, int c) {
    return dense_ptr[get_index(r, c)];
}

/* Methods for a Csparse matrix. */

Csparse_logical_matrix::Csparse_logical_matrix(const Rcpp::RObject& incoming) : Csparse_matrix(incoming), xptr(NULL), 
        row_data(ncol), col_data(nrow), row_ptr(row_data.data()), col_ptr(col_data.data()) {
    
    xptr=check_Matrix_logical(obj_x, incoming);
    return;
}

Csparse_logical_matrix::~Csparse_logical_matrix () {}

const int* Csparse_logical_matrix::get_row(int r) {
    return get_row_inside<int>(xptr, r, row_ptr, 0);
}

const int* Csparse_logical_matrix::get_col(int c) {
    return get_col_inside<int>(xptr, c, col_ptr, 0);
}

int Csparse_logical_matrix::get(int r, int c) {
    return get_one_inside<int>(xptr, r, c, 0);
}

/* Methods for a Psymm matrix. */

Psymm_logical_matrix::Psymm_logical_matrix(const Rcpp::RObject& incoming) : Psymm_matrix(incoming), xptr(NULL), 
        out_data(nrow), out_ptr(out_data.data()) {

    xptr=check_Matrix_logical(obj_x, incoming);
    return;
}

Psymm_logical_matrix::~Psymm_logical_matrix () {}

const int* Psymm_logical_matrix::get_col (int c) {
    return get_rowcol_inside<int>(xptr, c, out_ptr);
}

const int* Psymm_logical_matrix::get_row (int r) {
    return get_rowcol_inside<int>(xptr, r, out_ptr);
}

int Psymm_logical_matrix::get(int r, int c) {
    return xptr[get_index(r, c)];
}

/* Methods for a HDF5 matrix. */

HDF5_logical_matrix::HDF5_logical_matrix(const Rcpp::RObject& incoming) : HDF5_matrix(incoming), 
        row_data(ncol), col_data(nrow), row_ptr(row_data.data()), col_ptr(col_data.data()) {

    const Rcpp::RObject& h5_seed=incoming.slot("seed");
    const Rcpp::RObject& firstval=get_safe_slot(h5_seed, "first_val");
    if (firstval.sexp_type()!=LGLSXP) { 
        throw_custom_error("'first_val' slot in a ", get_class(h5_seed), " object should be logical");
    }
    if (hdata.getTypeClass()!=H5T_INTEGER) { 
        throw std::runtime_error("data type in HDF5 file is not integer");
    }
    return;
}

HDF5_logical_matrix::~HDF5_logical_matrix() {}

const int * HDF5_logical_matrix::get_row(int r) { 
    return get_row_inside<int>(r, row_ptr, H5::PredType::NATIVE_INT32);
} 

const int * HDF5_logical_matrix::get_col(int c) { 
    return get_col_inside<int>(c, col_ptr, H5::PredType::NATIVE_INT32);
}

int HDF5_logical_matrix::get(int r, int c) { 
    return get_one_inside<int>(r, c, H5::PredType::NATIVE_INT32);
}

/* Dispatch definition */

std::shared_ptr<logical_matrix> create_logical_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) {
        std::string ctype=get_class(incoming);
        if (ctype=="lgeMatrix") { 
            return std::shared_ptr<logical_matrix>(new dense_logical_matrix(incoming));
        } else if (ctype=="lgCMatrix") { 
            return std::shared_ptr<logical_matrix>(new Csparse_logical_matrix(incoming));
        } else if (ctype=="lgTMatrix") {
            throw std::runtime_error("lgTMatrix not supported, convert to lgCMatrix");
        } else if (ctype=="lspMatrix") {
            return std::shared_ptr<logical_matrix>(new Psymm_logical_matrix(incoming));
        } else if (ctype=="HDF5Matrix") { 
            return std::shared_ptr<logical_matrix>(new HDF5_logical_matrix(incoming));
        }
        throw_custom_error("unsupported class '", ctype, "' for logical_matrix");
    } 
    return std::shared_ptr<logical_matrix>(new simple_logical_matrix(incoming));
}


