#include "numeric_matrix.h"

/* Methods for the base class. */

numeric_matrix::numeric_matrix() {}

numeric_matrix::~numeric_matrix() {}

/* Methods for a simple matrix. */

simple_numeric_matrix::simple_numeric_matrix(const Rcpp::RObject& incoming) : simple_matrix(incoming), simple_ptr(NULL), 
        row_data(ncol), row_ptr(row_data.data()) {

    if (obj.sexp_type()!=REALSXP) { 
        throw std::runtime_error("matrix should be double-precision");
    }
    simple_ptr=REAL(obj.get__());
    return;
}

simple_numeric_matrix::~simple_numeric_matrix () {}

const double* simple_numeric_matrix::get_row(int r) {
    return get_row_inside<double>(simple_ptr, r, row_ptr); 
}

const double* simple_numeric_matrix::get_col(int c) {
    return get_col_inside<double>(simple_ptr, c);
}

double simple_numeric_matrix::get(int r, int c) {
    return simple_ptr[get_index(r, c)];
}

/* Methods for a dense Matrix. */

const double* check_Matrix_numeric(const Rcpp::RObject& x, const Rcpp::RObject& incoming) { 
    if (x.sexp_type()!=REALSXP) { 
        throw_custom_error("'x' slot in a ", get_class(incoming), " object should be double-precision"); 
    }
    return REAL(x.get__());
}

dense_numeric_matrix::dense_numeric_matrix(const Rcpp::RObject& incoming) : dense_matrix(incoming), dense_ptr(NULL), 
        row_data(ncol), row_ptr(row_data.data()) {
            
    dense_ptr=check_Matrix_numeric(obj_x, incoming);
    return;
}

dense_numeric_matrix::~dense_numeric_matrix() {}

const double* dense_numeric_matrix::get_row(int r) {
    return get_row_inside<double>(dense_ptr, r, row_ptr); 
}

const double* dense_numeric_matrix::get_col(int c) {
    return get_col_inside<double>(dense_ptr, c);
}

double dense_numeric_matrix::get(int r, int c) {
    return dense_ptr[get_index(r, c)];
}

/* Methods for a Csparse matrix. */

Csparse_numeric_matrix::Csparse_numeric_matrix(const Rcpp::RObject& incoming) : Csparse_matrix(incoming), xptr(NULL), 
        row_data(ncol), col_data(nrow), row_ptr(row_data.data()), col_ptr(col_data.data()) {

    xptr=check_Matrix_numeric(obj_x, incoming);
    return;
}

Csparse_numeric_matrix::~Csparse_numeric_matrix () {}

const double* Csparse_numeric_matrix::get_row(int r) {
    return get_row_inside<double>(xptr, r, row_ptr, 0);
}

const double* Csparse_numeric_matrix::get_col(int c) {
    return get_col_inside<double>(xptr, c, col_ptr, 0);
}

double Csparse_numeric_matrix::get(int r, int c) {
    return get_one_inside<double>(xptr, r, c, 0);
}

/* Methods for a Psymm matrix. */

Psymm_numeric_matrix::Psymm_numeric_matrix(const Rcpp::RObject& incoming) : Psymm_matrix(incoming), xptr(NULL), 
        out_data(nrow), out_ptr(out_data.data()) {
            
    xptr=check_Matrix_numeric(obj_x, incoming);
    return;
}

Psymm_numeric_matrix::~Psymm_numeric_matrix () {}

const double* Psymm_numeric_matrix::get_col (int c) {
    return get_rowcol_inside<double>(xptr, c, out_ptr);
}

const double* Psymm_numeric_matrix::get_row (int r) {
    return get_rowcol_inside<double>(xptr, r, out_ptr);
}

double Psymm_numeric_matrix::get(int r, int c) {
    return xptr[get_index(r, c)];
}

/* Methods for a HDF5 matrix. */

HDF5_numeric_matrix::HDF5_numeric_matrix(const Rcpp::RObject& incoming) : HDF5_matrix(incoming), 
        row_data(ncol), col_data(nrow), row_ptr(row_data.data()), col_ptr(col_data.data()) {

    const Rcpp::RObject& h5_seed=incoming.slot("seed");
    const Rcpp::RObject& firstval=get_safe_slot(h5_seed, "first_val");
    if (firstval.sexp_type()!=REALSXP) {
        throw_custom_error("'first_val' slot in a ", get_class(h5_seed), " object should be double-precision");
    }
    if (hdata.getTypeClass()!=H5T_FLOAT) { 
        throw std::runtime_error("data type in HDF5 file is not floating-point");
    }
    return;
}

HDF5_numeric_matrix::~HDF5_numeric_matrix() {}

const double * HDF5_numeric_matrix::get_row(int r) { 
    return get_row_inside<double>(r, row_ptr, H5::PredType::NATIVE_DOUBLE);
} 

const double * HDF5_numeric_matrix::get_col(int c) { 
    return get_col_inside<double>(c, col_ptr, H5::PredType::NATIVE_DOUBLE);
}

double HDF5_numeric_matrix::get(int r, int c) { 
    return get_one_inside<double>(r, c, H5::PredType::NATIVE_DOUBLE);
}

/* Dispatch definition */

std::shared_ptr<numeric_matrix> create_numeric_matrix(const Rcpp::RObject& incoming) { 
    if (incoming.isS4()) {
        std::string ctype=get_class(incoming);
        if (ctype=="dgeMatrix") { 
            return std::shared_ptr<numeric_matrix>(new dense_numeric_matrix(incoming));
        } else if (ctype=="dgCMatrix") { 
            return std::shared_ptr<numeric_matrix>(new Csparse_numeric_matrix(incoming));
        } else if (ctype=="dgTMatrix") {
            throw std::runtime_error("dgTMatrix not supported, convert to dgCMatrix");
        } else if (ctype=="dspMatrix") {
            return std::shared_ptr<numeric_matrix>(new Psymm_numeric_matrix(incoming));
        } else if (ctype=="HDF5Matrix") { 
            return std::shared_ptr<numeric_matrix>(new HDF5_numeric_matrix(incoming));
        }
        throw_custom_error("unsupported class '", ctype, "' for numeric_matrix");
    } 
    return std::shared_ptr<numeric_matrix>(new simple_numeric_matrix(incoming));
}


