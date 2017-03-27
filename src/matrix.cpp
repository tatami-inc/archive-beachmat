#include "matrix.h"

/* Methods for the virtual base matrix. */

any_matrix::any_matrix() : nrow(0), ncol(0) {}

any_matrix::~any_matrix() {}

int any_matrix::get_nrow() const { return nrow; }

int any_matrix::get_ncol() const { return ncol; }

void any_matrix::fill_dims(SEXP dims) {
    if (!isInteger(dims) || LENGTH(dims)!=2) { 
        throw std::runtime_error("matrix dimensions should be an integer vector of length 2");
    }
    nrow=INTEGER(dims)[0];
    ncol=INTEGER(dims)[1];
    return;
}

/* Methods for the virtual simple matrix. */

simple_matrix::simple_matrix(SEXP incoming) {
    fill_dims(getAttrib(incoming, R_DimSymbol));
    if (LENGTH(incoming)!=nrow*ncol) { throw std::runtime_error("length of matrix is inconsistent with its dimensions"); }
    return;
}

simple_matrix::~simple_matrix() {}

int simple_matrix::get_index(int r, int c) const { return r + c*nrow; }

/* Methods for the virtual dgeMatrix. */

dense_matrix::dense_matrix (SEXP incoming) {
    SEXP dimslot = install("Dim");
    if (!R_has_slot(incoming, dimslot)) { throw std::runtime_error("no 'Dim' slot in the dgeMatrix object"); }
    fill_dims(R_do_slot(incoming, dimslot));

    SEXP xslot = install("x");
    if (!R_has_slot(incoming, xslot)) { throw std::runtime_error("no 'x' slot in the dgeMatrix object"); }
    SEXP x=R_do_slot(incoming, xslot);
    if (LENGTH(x)!=nrow*ncol) { throw std::runtime_error("length of 'x' is not consistent with dgeMatrix dimensions"); }

    return;
}

dense_matrix::~dense_matrix() {}

int dense_matrix::get_index(int r, int c) const { return r + c*nrow; }

/* Methods for the virtual dgCMatrix. */

Csparse_matrix::Csparse_matrix(SEXP incoming) : iptr(NULL), pptr(NULL), nx(0) {
    if (!IS_S4_OBJECT(incoming) || std::strcmp(get_class(incoming), "dgCMatrix")!=0) {
        throw std::runtime_error("matrix should be a dgCMatrix object");
    }
    
    SEXP dimslot = install("Dim");
    if (!R_has_slot(incoming, dimslot)) { throw std::runtime_error("no 'Dim' slot in the dgCMatrix object"); }
    fill_dims(R_do_slot(incoming, dimslot));

    SEXP islot = install("i");
    if (!R_has_slot(incoming, islot)) { throw std::runtime_error("no 'i' slot in the dgCMatrix object"); }
    SEXP i=R_do_slot(incoming, islot);
    if (!isInteger(i)) { throw std::runtime_error("'i' slot in a dgCMatrix should be integer"); }
    iptr=INTEGER(i);

    SEXP pslot = install("p");
    if (!R_has_slot(incoming, pslot)) { throw std::runtime_error("no 'p' slot in the dgCMatrix object"); }
    SEXP p=R_do_slot(incoming, pslot);
    if (!isInteger(p)) { throw std::runtime_error("'p' slot in a dgCMatrix should be integer"); }
    pptr=INTEGER(p);

    SEXP xslot = install("x");
    if (!R_has_slot(incoming, xslot)) { throw std::runtime_error("no 'x' slot in the dgCMatrix object"); }
    SEXP x=R_do_slot(incoming, xslot);
    nx=LENGTH(x);

    if (nx!=LENGTH(i)) { throw std::runtime_error("'x' and 'i' slots in a dgCMatrix should have the same length"); }
    if (ncol+1!=LENGTH(p)) { throw std::runtime_error("length of 'p' slot in a dgCMatrix should be equal to 'ncol+1'"); }
    if (pptr[ncol]!=nx || pptr[0]!=0) { throw std::runtime_error("first and last elements of 'p' should be 0 and 'length(x)', respectively"); }

    // Checking all the indices.
    int px, ix;
    for (px=1; px<=ncol; ++px) {
        if (pptr[px] < pptr[px-1]) { throw std::runtime_error("'p' is not sorted"); }
    }
    for (px=0; px<ncol; ++px) { 
        for (ix=pptr[px]+1; ix<pptr[px+1]; ++ix) {
            if (iptr[ix]<iptr[ix-1]) { 
                throw std::runtime_error("'i' in each column of a dgCMatrix should be sorted");
            }
        }
    }
    for (ix=0; ix<nx; ++ix) {
        if (iptr[ix]<0 || iptr[ix]>=nrow) {
            throw std::runtime_error("'i' indices out of range for dgCMatrix");
        }
    }

    return;
}

Csparse_matrix::~Csparse_matrix() {} 

int Csparse_matrix::get_index(int r, int c) const {
    const int* iend=iptr + pptr[c+1];
    const int* loc=std::lower_bound(iptr + pptr[c], iend, r);
    if (loc!=iend && *loc==r) { 
        return loc - iptr;
    } else {
        return nx;
    }
}

/* Methods for dgTMatrix. */

Tsparse_matrix::Tsparse_matrix(SEXP incoming) : iptr(NULL), jptr(NULL), nx(0), order(NULL), pptr(NULL), iptr2(NULL) {
    if (!IS_S4_OBJECT(incoming) || std::strcmp(get_class(incoming), "dgTMatrix")!=0) {
        throw std::runtime_error("matrix should be a dgTMatrix object");
    }
    
    SEXP dimslot = install("Dim");
    if (!R_has_slot(incoming, dimslot)) { throw std::runtime_error("no 'Dim' slot in the dgTMatrix object"); }
    fill_dims(R_do_slot(incoming, dimslot));

    SEXP islot = install("i");
    if (!R_has_slot(incoming, islot)) { throw std::runtime_error("no 'i' slot in the dgTMatrix object"); }
    SEXP i=R_do_slot(incoming, islot);
    if (!isInteger(i)) { throw std::runtime_error("'i' slot in a dgTMatrix should be integer"); }
    iptr=INTEGER(i);

    SEXP jslot = install("j");
    if (!R_has_slot(incoming, jslot)) { throw std::runtime_error("no 'p' slot in the dgTMatrix object"); }
    SEXP j=R_do_slot(incoming, jslot);
    if (!isInteger(j)) { throw std::runtime_error("'j' slot in a dgTMatrix should be integer"); }
    jptr=INTEGER(j);

    SEXP xslot = install("x");
    if (!R_has_slot(incoming, xslot)) { throw std::runtime_error("no 'x' slot in the dgTMatrix object"); }
    SEXP x=R_do_slot(incoming, xslot);
    nx=LENGTH(x);

    if (nx!=LENGTH(i) || LENGTH(j)!=nx) { throw std::runtime_error("'x', 'i' and 'j' slots in a dgTMatrix should have the same length"); }
    for (int i=0; i<nx; ++i) {
        if (iptr[i] < 0 || iptr[i]>=nrow) { throw std::runtime_error("'i' slot of a dgTMatrix should contain elements in [0, nrow)"); }
        if (jptr[i] < 0 || jptr[i]>=ncol) { throw std::runtime_error("'j' slot of a dgTMatrix should contain elements in [0, ncol)"); }
    }
   
    try {
        // Adding column-major indexing as in dgCMatrix.   
        int ix;
        order=new int[nx];
        for (ix=0; ix<nx; ++ix) {
            order[ix]=ix;
        }
        index_orderer<int> jorder(jptr);
        std::sort(order, order+nx, jorder);

        pptr=new int[ncol+1];
        std::fill(pptr, pptr+ncol+1, 0);
        for (ix=0; ix<nx; ++ix) {
            ++(pptr[jptr[ix]+1]);
        }
        for (ix=1; ix<=ncol; ++ix){ 
            pptr[ix]+=pptr[ix-1];
        }

        index_orderer<int> iorder(iptr);
        for (int px=0; px<ncol; ++px) { 
            std::sort(order + pptr[px], order + pptr[px+1], iorder);
        }

        iptr2=new int[nx];
        for (ix=0; ix<nx; ++ix) {
            iptr2[ix]=iptr[order[ix]];
        }
    } catch (std::exception& e) {
        delete [] order;
        delete [] pptr;
        delete [] iptr2;
    }
    return;
}

Tsparse_matrix::~Tsparse_matrix() {
    delete [] order;
    delete [] pptr;
    delete [] iptr2;
    return;
} 

int Tsparse_matrix::get_index(int r, int c) const {
    int* iend=iptr2 + pptr[c+1];
    const int* loc=std::lower_bound(iptr2 + pptr[c], iend, r);
    if (loc!=iend && *loc==r) { 
        return order[loc - iptr2];
    } else {
        return nx;
    }
}

/* Methods for the virtual HDF5Matrix. */

HDF5_matrix::HDF5_matrix(SEXP incoming) {
    if (!IS_S4_OBJECT(incoming) || std::strcmp(get_class(incoming), "HDF5Matrix")!=0) {
        throw std::runtime_error("matrix should be a HDF5Matrix object");
    }

    SEXP seedslot=install("seed");
    if (!R_has_slot(incoming, seedslot)) { throw std::runtime_error("no 'seed' slot in the HDF5Matrix object"); }
    SEXP h5_seed=R_do_slot(incoming, seedslot);
    if (!IS_S4_OBJECT(h5_seed) || std::strcmp(get_class(h5_seed), "HDF5ArraySeed")!=0) {
        throw std::runtime_error("'seed' should be a HDF5ArraySeed object");
    }
    fill_dims(getAttrib(h5_seed, R_DimSymbol));

    SEXP fileslot=install("file");
    if (!R_has_slot(h5_seed, fileslot)) { throw std::runtime_error("no 'file' slot in the HDF5ArraySeed object"); }
    SEXP filename=R_do_slot(h5_seed, fileslot);
    if (!isString(filename) || LENGTH(filename)!=1) { 
        throw std::runtime_error("'file' should be a string");
    }
    const char* fname=CHAR(STRING_ELT(filename, 0));

    SEXP nameslot=install("name");
    if (!R_has_slot(h5_seed, nameslot)) { throw std::runtime_error("no 'name' slot in the HDF5ArraySeed object"); }
    SEXP dataname=R_do_slot(h5_seed, nameslot);
    if (!isString(dataname) || LENGTH(dataname)!=1) { 
        throw std::runtime_error("'name' should be a string");
    }
    const char* dataset=CHAR(STRING_ELT(dataname, 0));
    
    // Setting up the HDF5 accessors.
    hfile.openFile(H5std_string(fname), H5F_ACC_RDONLY);
    hdata = hfile.openDataSet(H5std_string(dataset));
    if (hdata.getTypeClass()!=H5T_FLOAT) { 
        throw std::runtime_error("data type in HDF5 file is not a float");
    }
    hspace = hdata.getSpace();
    if (hspace.getSimpleExtentNdims()!=2) {
        throw std::runtime_error("data in HDF5 file is not a two-dimensional array");
    }
    hsize_t dims_out[2];
    hspace.getSimpleExtentDims(dims_out, NULL);
    if (dims_out[1]!=nrow || dims_out[0]!=ncol) { 
        throw std::runtime_error("dimensions in HDF5 file do not equal dimensions in HDF5Matrix");
    }

    offset[0]=0;
    offset[1]=0;

    rows_out[0]=ncol;
    rows_out[1]=1;
    rowspace=H5::DataSpace(2, rows_out);
    rowspace.selectHyperslab( H5S_SELECT_SET, rows_out, offset);

    cols_out[0]=1;
    cols_out[1]=nrow;
    colspace=H5::DataSpace(2, cols_out);
    colspace.selectHyperslab( H5S_SELECT_SET, cols_out, offset);

    one_out[0]=1;
    one_out[1]=1;
    onespace=H5::DataSpace(2, one_out);
    onespace.selectHyperslab( H5S_SELECT_SET, one_out, offset);

    return;
}

HDF5_matrix::~HDF5_matrix() {}

void HDF5_matrix::set_row(int r) {
    offset[0] = 0;
    offset[1] = r;
    hspace.selectHyperslab(H5S_SELECT_SET, rows_out, offset);
    return;
}

void HDF5_matrix::set_col(int c) {
    offset[0] = c;
    offset[1] = 0;
    hspace.selectHyperslab(H5S_SELECT_SET, cols_out, offset);
    return;
}

void HDF5_matrix::set_one(int r, int c) { 
    offset[0]=c;
    offset[1]=r;
    hspace.selectHyperslab( H5S_SELECT_SET, one_out, offset);
    return;
}

