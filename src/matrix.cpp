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
    if (nrow<0 || ncol<0) { throw std::runtime_error("dimensions should be non-negative"); }
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

/* Methods for the virtual *geMatrix. */

dense_matrix::dense_matrix (SEXP incoming) {
    if (!IS_S4_OBJECT(incoming) || std::strcmp(get_class(incoming)+1, "geMatrix")!=0) {
        throw std::runtime_error("matrix should be a *geMatrix object");
    }
    fill_dims(get_safe_slot(incoming, "Dim"));
    return;
}

dense_matrix::~dense_matrix() {}

int dense_matrix::get_index(int r, int c) const { return r + c*nrow; }

/* Methods for the virtual *gCMatrix. */

Csparse_matrix::Csparse_matrix(SEXP incoming) : iptr(NULL), pptr(NULL), nx(0) {
    const char* ctype=get_class(incoming);
    if (!IS_S4_OBJECT(incoming) || std::strcmp(ctype+1, "gCMatrix")!=0) {
        throw std::runtime_error("matrix should be a *gCMatrix object");
    }
    
    fill_dims(get_safe_slot(incoming, "Dim"));

    SEXP i=get_safe_slot(incoming, "i");
    if (!isInteger(i)) { throw_custom_error("'i' slot in a ", ctype, " object should be integer"); }
    iptr=INTEGER(i);

    SEXP p=get_safe_slot(incoming, "p");
    if (!isInteger(p)) { throw_custom_error("'p' slot in a ", ctype, " object should be integer"); }
    pptr=INTEGER(p);

    nx=LENGTH(get_safe_slot(incoming, "x"));
    if (nx!=LENGTH(i)) { throw_custom_error("'x' and 'i' slots in a ", ctype, " object should have the same length"); }
    if (ncol+1!=LENGTH(p)) { throw_custom_error("length of 'p' slot in a ", ctype, " object should be equal to 'ncol+1'"); }
    if (pptr[0]!=0) { throw_custom_error("first element of 'p' in a ", ctype, " object should be 0"); }
    if (pptr[ncol]!=nx) { throw_custom_error("last element of 'p' in a ", ctype, " object should be 'length(x)'"); }

    // Checking all the indices.
    int px, ix;
    for (px=1; px<=ncol; ++px) {
        if (pptr[px] < pptr[px-1]) { throw_custom_error("'p' in a ", ctype, " object should be sorted"); }
    }
    for (px=0; px<ncol; ++px) { 
        for (ix=pptr[px]+1; ix<pptr[px+1]; ++ix) {
            if (iptr[ix]<iptr[ix-1]) { 
                throw_custom_error("'i' in each column of a ", ctype, " object should be sorted");
            }
        }
    }
    for (ix=0; ix<nx; ++ix) {
        if (iptr[ix]<0 || iptr[ix]>=nrow) {
            throw_custom_error("'i' slot in a ", ctype, " object should contain elements in [0, nrow)");
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

/* Methods for the virtual *gTMatrix. */

Tsparse_matrix::Tsparse_matrix(SEXP incoming) : iptr(NULL), jptr(NULL), nx(0), order(NULL), pptr(NULL), iptr2(NULL) {
    const char* ctype=get_class(incoming);
    if (!IS_S4_OBJECT(incoming) || std::strcmp(ctype+1, "gTMatrix")!=0) {
        throw std::runtime_error("matrix should be a *gTMatrix object");
    }
    
    fill_dims(get_safe_slot(incoming, "Dim"));

    SEXP i=get_safe_slot(incoming, "i");
    if (!isInteger(i)) { throw_custom_error("'i' slot in a ", ctype, " object should be integer"); }
    iptr=INTEGER(i);

    SEXP j=get_safe_slot(incoming, "j");
    if (!isInteger(i)) { throw_custom_error("'j' slot in a ", ctype, " object should be integer"); }
    jptr=INTEGER(j);

    nx=LENGTH(get_safe_slot(incoming, "x"));
    if (nx!=LENGTH(i) || LENGTH(j)!=nx) { throw_custom_error("'x', 'i' and 'j' slots in a ", ctype, " object should have the same length"); }
    for (int i=0; i<nx; ++i) {
        if (iptr[i] < 0 || iptr[i]>=nrow) { throw_custom_error("'i' slot of a ", ctype, " object should contain elements in [0, nrow)"); }
        if (jptr[i] < 0 || jptr[i]>=ncol) { throw_custom_error("'j' slot of a ", ctype, " object should contain elements in [0, ncol)"); }
    }
   
    try {
        // Adding column-major indexing, mimicking *gCMatrix for rapid column-access. 
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

/* Methods for the virtual *spMatrix. */

Psymm_matrix::Psymm_matrix(SEXP incoming) : upper(true) {
    const char* ctype=get_class(incoming);
    if (!IS_S4_OBJECT(incoming) || std::strcmp(ctype+1, "spMatrix")!=0) {
        throw std::runtime_error("matrix should be a *spMatrix object");
    }
    
    fill_dims(get_safe_slot(incoming, "Dim"));
    if (nrow!=ncol) { throw_custom_error("'nrow' and 'ncol' should be equal for a ", ctype, "object"); }
    
    SEXP ul=get_safe_slot(incoming, "uplo");
    if (!isString(ul) || LENGTH(ul)!=1) { throw_custom_error("'uplo' slot in a ", ctype, " object should be a string"); }
    switch (*(CHAR(STRING_ELT(ul, 0)))) {
        case 'U':
            upper=true;
            break;
        case 'L':
            upper=false;
            break;
        default:
            throw_custom_error("'uplo' slot in a ", ctype, " object should be either 'U' or 'L'");
    }
    return;
}

Psymm_matrix::~Psymm_matrix() {}

int Psymm_matrix::get_index(int r, int c) const {
    if (upper) {
        if (c > r) { 
            return (c*(c+1))/2 + r;            
        } else {
            return (r*(r+1))/2 + c;            
        }
    } else {
        if (r > c) { 
            return nrow * c - (c * (c - 1))/2 + r - c;
        } else {
            return nrow * r - (r * (r - 1))/2 + c - r;
        }
    }
}

/* Methods for the virtual HDF5Matrix. */

HDF5_matrix::HDF5_matrix(SEXP incoming) {
    const char* ctype=get_class(incoming);
    if (!IS_S4_OBJECT(incoming) || std::strcmp(ctype, "HDF5Matrix")!=0) {
        throw std::runtime_error("matrix should be a HDF5Matrix object");
    }

    SEXP h5_seed=get_safe_slot(incoming, "seed");
    const char *stype=get_class(h5_seed);
    if (!IS_S4_OBJECT(h5_seed) || std::strcmp(stype, "HDF5ArraySeed")!=0) {
        throw_custom_error("'seed' slot in a ", ctype, " object should be a HDF5ArraySeed object");
    }
    fill_dims(getAttrib(h5_seed, R_DimSymbol));

    SEXP filename=get_safe_slot(h5_seed, "file");
    if (!isString(filename) || LENGTH(filename)!=1) { 
        throw_custom_error("'file' slot in a ", stype, " object should be a string");
    }
    const char* fname=CHAR(STRING_ELT(filename, 0));

    SEXP dataname=get_safe_slot(h5_seed, "name");
    if (!isString(dataname) || LENGTH(dataname)!=1) { 
        throw_custom_error("'name' slot in a ", stype, " object should be a string");
    }
    const char* dataset=CHAR(STRING_ELT(dataname, 0));
    
    // Setting up the HDF5 accessors.
    hfile.openFile(H5std_string(fname), H5F_ACC_RDONLY);
    hdata = hfile.openDataSet(H5std_string(dataset));
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
    rowspace.selectHyperslab(H5S_SELECT_SET, rows_out, offset);

    cols_out[0]=1;
    cols_out[1]=nrow;
    colspace=H5::DataSpace(2, cols_out);
    colspace.selectHyperslab(H5S_SELECT_SET, cols_out, offset);

    one_out[0]=1;
    one_out[1]=1;
    onespace=H5::DataSpace(2, one_out);
    onespace.selectHyperslab(H5S_SELECT_SET, one_out, offset);

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

