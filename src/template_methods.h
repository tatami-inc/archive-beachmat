#ifndef TEMPLATE_METHODS_H
#define TEMPLATE_METHODS_H

/* Simple matrix templated methods */

template<typename T> 
const T* simple_matrix::get_row_inside(const T* in, int r, T* out) {
    for (int col=0; col<ncol; ++col) { out[col]=in[get_index(r, col)]; }
    return out;
}

template<typename T>
const T* simple_matrix::get_col_inside(const T* in, int c) {
    return in + c*nrow;
}

/* Dense matrix templated methods */

template<typename T> 
const T* dense_matrix::get_row_inside(const T* in, int r, T* out) {
    for (int col=0; col<ncol; ++col) { out[col]=in[get_index(r, col)]; }
    return out;
}

template<typename T>
const T* dense_matrix::get_col_inside(const T* in, int c) {
    return in + c*nrow;
}

/* Csparse templated methods */

template<typename T>
const T* Csparse_matrix::get_row_inside(const T* in, int r, T* out, T zero) { 
    std::fill(out, out+ncol, zero);
    for (int col=0; col<ncol; ++col) {  
        if (pptr[col]!=pptr[col+1]) { out[col]=get_one_inside(in, r, col, zero); } 
    } 
    return out;  
} 

template<typename T> 
const T* Csparse_matrix::get_col_inside(const T* in, int c, T* out, T zero) { 
    const int& start=pptr[c]; 
    const int& end=pptr[c+1]; 
    std::fill(out, out+nrow, zero); 
    for (int ix=start; ix<end; ++ix) {
        out[iptr[ix]]=in[ix];
    }
    return out;
}

template<typename T>
T Csparse_matrix::get_one_inside(const T* in, int r, int c, T zero) {
    const int index=get_index(r, c);
    return (index!=nx ? in[index] : zero);
}

/* Psymm templated methods */

template<typename T>
const T* Psymm_matrix::get_rowcol_inside (const T* in, int rc, T* out) {
    if (upper) {
        int start=(rc*(rc+1))/2;
        std::copy(in+start, in+start+rc, out);
        for (int i=rc; i<ncol; ++i) {
            out[i]=in[start+rc];
            start+=i+1;
        }
    } else {
        int start=0;
        for (int i=0; i<rc; ++i) {
            out[i]=in[rc-i + start];
            start+=nrow-i;
        }
        std::copy(in+start, in+start+nrow-rc, out+rc);
    }
    return out;
}

/* HDF5Matrix templated methods */

template<typename T>
const T* HDF5_matrix::get_row_inside(int r, T* out, const H5::PredType& S) {
    set_row(r);
    hdata.read(out, S, rowspace, hspace);
    return out;
}

template<typename T>
const T* HDF5_matrix::get_col_inside(int c, T* out, const H5::PredType& S) {
    set_col(c);
    hdata.read(out, S, colspace, hspace);
    return out;
}

template<typename T>
T HDF5_matrix::get_one_inside(int r, int c, const H5::PredType& S) {
    set_one(r, c);
    T out;
    hdata.read(&out, S, onespace, hspace);
    return out;
}

#endif
