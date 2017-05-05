#ifndef TEMPLATE_OUTPUT_H
#define TEMPLATE_OUTPUT_H

/* Methods for the base output class. */

template<typename T, class V>
output_matrix<T, V>::output_matrix(int nr, int nc) : nrow(nr), ncol(nc) {}

template<typename T, class V>
output_matrix<T, V>::~output_matrix() {}

template<typename T, class V>
void output_matrix<T, V>::fill_row(int r, typename V::iterator in) {
    fill_row(r, in, 0, this->ncol);
    return;
}

template<typename T, class V>
void output_matrix<T, V>::fill_col(int c, typename V::iterator in) {
    fill_col(c, in, 0, this->nrow);
    return;
}

template<typename T, class V>
int output_matrix<T, V>::get_nrow() const { 
    return this->nrow;
}

template<typename T, class V>
int output_matrix<T, V>::get_ncol() const { 
    return this->ncol;
}

/* Methods for the simple output matrix. */

template<typename T, class V>
simple_output<T, V>::simple_output(int nr, int nc) : output_matrix<T, V>(nr, nc) { 
    (this->data)=V(nr*nc);
    return; 
}

template<typename T, class V>
simple_output<T, V>::~simple_output() {}

template<typename T, class V>
void simple_output<T, V>::fill_col(int c, typename V::iterator in, int start, int end) {
    std::copy(in, in + end - start, (this->data).begin()+c*(this->nrow)+start); 
    return;
}

template<typename T, class V>
void simple_output<T, V>::fill_row(int r, typename V::iterator in, int start, int end) {
    const int& NR=this->nrow;
    auto mIt=(this->data).begin() + r + start * NR;
    for (int c=start; c<end; ++c, mIt+=NR, ++in) {
        (*mIt)=*in;        
    }
    return;
}

template<typename T, class V>
void simple_output<T, V>::fill(int r, int c, T in) {
    (this->data)[r + (this->nrow)*c]=in;
    return;
}

template<typename T, class V>
Rcpp::RObject simple_output<T, V>::yield() {
    Rcpp::RObject out(SEXP(this->data));
    out.attr("dim") = Rcpp::IntegerVector::create(this->nrow, this->ncol); 
    return out;
}

/* Methods for HDF5 output matrix. */

template<typename T, class V, const H5::PredType& HPT, const T& FILL>
HDF5_output<T, V, HPT, FILL>::HDF5_output (int nr, int nc) : output_matrix<T, V>(nr, nc) {
    // Initializing file name (not using getHDF5DumpName, to avoid file handle conflicts
    // and problems with concurrent read/write).
    Rcpp::Environment baseenv("package:base");
    Rcpp::Function tempfun=baseenv["tempfile"];
    fname = Rcpp::as<std::string>(tempfun()) + ".h5";
    hfile=H5::H5File(fname, H5F_ACC_TRUNC);
   
    H5::DSetCreatPropList plist;
    plist.setFillValue(HPT, &FILL);
    
    dname="/HDF5ArrayAUTO00001";
    hsize_t dims[2];
    dims[0]=this->ncol; // Setting the dimensions (0 is column, 1 is row; internally transposed).
    dims[1]=this->nrow; 
    hspace=H5::DataSpace(2, dims);
    hdata=H5::DataSet(hfile.createDataSet(dname, HPT, hspace, plist)); // Creating the data set.

    h5_start[0]=0;
    h5_start[1]=0;
    col_count[0]=1;
    col_count[1]=this->nrow;
    row_count[0]=this->ncol;
    row_count[1]=1;
    rowspace=H5::DataSpace(1, row_count);
    colspace=H5::DataSpace(1, col_count+1);
    
    zero_start[0]=0;
    one_count[0]=1;
    one_count[1]=1;
    onespace=H5::DataSpace(1, one_count);
    onespace.selectAll();

    // Specifying it's logical, if that's the case.
    V tmp;
    if (tmp.sexp_type()==LGLSXP) {
        H5::StrType str_type(0, H5T_VARIABLE);
        H5::DataSpace att_space(1, one_count);
        H5::Attribute att = hdata.createAttribute("storage.mode", str_type, att_space);
        att.write(str_type, std::string("logical"));
    }

    return;
}

template<typename T, class V, const H5::PredType& HPT, const T& FILL>
HDF5_output<T, V, HPT, FILL>::~HDF5_output() {}

template<typename T, class V, const H5::PredType& HPT, const T& FILL>
void HDF5_output<T, V, HPT, FILL>::select_col(int c, int start, int end) {
    col_count[1]=end-start;
    colspace.setExtentSimple(1, col_count+1);
    colspace.selectAll();
    h5_start[0]=c;
    h5_start[1]=start;
    hspace.selectHyperslab(H5S_SELECT_SET, col_count, h5_start);
    return;
}

template<typename T, class V, const H5::PredType& HPT, const T& FILL>
void HDF5_output<T, V, HPT, FILL>::fill_col(int c, typename V::iterator in, int start, int end) {
    select_col(c, start, end);
    hdata.write(&(*in), HPT, colspace, hspace);
    return;
}

template<typename T, class V, const H5::PredType& HPT, const T& FILL>
void HDF5_output<T, V, HPT, FILL>::select_row(int r, int start, int end) {
    row_count[0] = end-start;
    rowspace.setExtentSimple(1, row_count);
    rowspace.selectAll();
    h5_start[0] = start;
    h5_start[1] = r;
    hspace.selectHyperslab(H5S_SELECT_SET, row_count, h5_start);
    return;
}

template<typename T, class V, const H5::PredType& HPT, const T& FILL>
void HDF5_output<T, V, HPT, FILL>::fill_row(int c, typename V::iterator in, int start, int end) {
    select_row(c, start, end);
    hdata.write(&(*in), HPT, rowspace, hspace);
    return;
}

template<typename T, class V, const H5::PredType& HPT, const T& FILL>
void HDF5_output<T, V, HPT, FILL>::select_one(int r, int c) {
    h5_start[0]=c;
    h5_start[1]=r;
    hspace.selectHyperslab(H5S_SELECT_SET, one_count, h5_start);
    return;
}

template<typename T, class V, const H5::PredType& HPT, const T& FILL>
void HDF5_output<T, V, HPT, FILL>::fill(int r, int c, T in) {
    select_one(r, c);
    hdata.write(&in, HPT, onespace, hspace);
    return;
}

template<typename T, class V, const H5::PredType& HPT, const T& FILL>
T HDF5_output<T, V, HPT, FILL>::get_first() {
    T thing;
    if (this->nrow && this->ncol) {
        select_one(0, 0);
        hdata.read(&thing, HPT, onespace, hspace);
    }
    return thing;
}

template<typename T, class V, const H5::PredType& HPT, const T& FILL>
Rcpp::RObject HDF5_output<T, V, HPT, FILL>::yield() {
    std::string seedclass="HDF5ArraySeed";
    Rcpp::S4 h5seed(seedclass);

    // Assigning to slots.
    if (!h5seed.hasSlot("file")) {
        throw_custom_error("missing 'file' slot in ", seedclass, " object");
    }
    h5seed.slot("file") = fname;
    if (!h5seed.hasSlot("name")) {
        throw_custom_error("missing 'name' slot in ", seedclass, " object");
    }
    h5seed.slot("name") = dname;
    if (!h5seed.hasSlot("dim")) {
        throw_custom_error("missing 'dim' slot in ", seedclass, " object");
    }
    h5seed.slot("dim") = Rcpp::IntegerVector::create(this->nrow, this->ncol);
    if (!h5seed.hasSlot("first_val")) {
        throw_custom_error("missing 'first_val' slot in ", seedclass, " object");
    }
    h5seed.slot("first_val") = V::create(get_first());

    // Assigning the seed to the HDF5Matrix.
    std::string matclass="HDF5Matrix";
    Rcpp::S4 h5mat(matclass);
    if (!h5mat.hasSlot("seed")) {
        throw_custom_error("missing 'seed' slot in ", matclass, " object");
    }
    h5mat.slot("seed") = h5seed;

    return SEXP(h5mat);
}

#endif

