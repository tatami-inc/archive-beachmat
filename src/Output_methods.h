#ifndef BEACHMAT_OUTPUT_METHODS_H
#define BEACHMAT_OUTPUT_METHODS_H

/* Methods for the simple output matrix. */

template<typename T, class V>
simple_output<T, V>::simple_output(size_t nr, size_t nc) : any_matrix(nr, nc) { 
    (this->data)=V(nr*nc);
    return; 
}

template<typename T, class V>
simple_output<T, V>::~simple_output() {}

template<typename T, class V>
template<class Iter>
void simple_output<T, V>::fill_col(size_t c, Iter in, size_t start, size_t end) {
    check_colargs(c, start, end);
    std::copy(in, in + end - start, data.begin()+c*(this->nrow)+start); 
    return;
}

template<typename T, class V>
template<class Iter>
void simple_output<T, V>::fill_row(size_t r, Iter in, size_t start, size_t end) {
    check_rowargs(r, start, end);
    const size_t& NR=this->nrow;
    auto mIt=data.begin() + r + start * NR;
    for (size_t c=start; c<end; ++c, mIt+=NR, ++in) {
        (*mIt)=*in;        
    }
    return;
}

template<typename T, class V>
void simple_output<T, V>::fill(size_t r, size_t c, T in) {
    check_oneargs(r, c);
    data[r + (this->nrow)*c]=in;
    return;
}

template<typename T, class V>
template<class Iter>
void simple_output<T, V>::get_row(size_t r, Iter out, size_t start, size_t end) {
    check_rowargs(r, start, end);
    const size_t& NR=this->nrow;
    auto src=data.begin()+start*NR+r;
    for (size_t col=start; col<end; ++col, src+=NR, ++out) { (*out)=(*src); }
    return;
}

template<typename T, class V>
template<class Iter>
void simple_output<T, V>::get_col(size_t c, Iter out, size_t start, size_t end) {
    check_colargs(c, start, end);
    auto src=data.begin() + c*(this->nrow);
    std::copy(src+start, src+end, out);
    return;
}

template<typename T, class V>
T simple_output<T, V>::get(size_t r, size_t c) {
    check_oneargs(r, c);
    return data[c*(this->nrow)+r];
}

template<typename T, class V>
Rcpp::RObject simple_output<T, V>::yield() {
    Rcpp::RObject out(SEXP(this->data));
    out.attr("dim") = Rcpp::IntegerVector::create(this->nrow, this->ncol); 
    return out;
}

/* Methods for HDF5 output matrix. */

template<typename T, class V>
HDF5_output<T, V>::HDF5_output (size_t nr, size_t nc) : any_matrix(nr, nc) {
    // File opening.
    Rcpp::Environment hdf5env("package:HDF5Array");
    Rcpp::Function filenamefun=hdf5env["getHDF5DumpFile"];
    fname=make_to_string(filenamefun(Rcpp::Named("for.use", Rcpp::LogicalVector::create(1))));
    hfile.openFile(fname, H5F_ACC_RDWR);

    // Type setting.
    const int RTYPE=V().sexp_type();
    switch (RTYPE) {
        case REALSXP:
            HDT=H5::DataType(H5::PredType::NATIVE_DOUBLE);
            break;
        case INTSXP: case LGLSXP:
            HDT=H5::DataType(H5::PredType::NATIVE_INT32);
            break;
        default:
            std::stringstream err;
            err << "unsupported sexptype '" << RTYPE << "'";
            throw std::runtime_error(err.str().c_str());
    }

    // Creating the data set.
    H5::DSetCreatPropList plist;
    const T empty=get_empty();
    plist.setFillValue(HDT, &empty);
    hsize_t dims[2];
    dims[0]=this->ncol; // Setting the dimensions (0 is column, 1 is row; internally transposed).
    dims[1]=this->nrow; 
    hspace.setExtentSimple(2, dims);

    Rcpp::Function datanamefun=hdf5env["getHDF5DumpName"];
    dname=make_to_string(datanamefun(Rcpp::Named("for.use", Rcpp::LogicalVector::create(1))));
    hdata=hfile.createDataSet(dname, HDT, hspace, plist); 

    // Adding to the log file.
    Rcpp::Function appendfun=hdf5env["appendDatasetCreationToHDF5DumpLog"];
    appendfun(Rcpp::StringVector(fname), 
            Rcpp::StringVector(dname), 
            Rcpp::IntegerVector::create(this->nrow, this->ncol),
            Rcpp::StringVector(translate_type(RTYPE)));

    h5_start[0]=0;
    h5_start[1]=0;
    col_count[0]=1;
    col_count[1]=this->nrow;
    row_count[0]=this->ncol;
    row_count[1]=1;
    
    zero_start[0]=0;
    one_count[0]=1;
    one_count[1]=1;
    onespace=H5::DataSpace(1, one_count);
    onespace.selectAll();

    // Setting logical attributes.
    if (V().sexp_type()==LGLSXP) {
        H5::StrType str_type(0, H5T_VARIABLE);
        H5::DataSpace att_space(1, one_count);
        H5::Attribute att = hdata.createAttribute("storage.mode", str_type, att_space);
        att.write(str_type, std::string("logical"));
    }

    return;
}

template<typename T, class V>
HDF5_output<T, V>::~HDF5_output() {}

template<typename T, class V>
void HDF5_output<T, V>::select_col(size_t c, size_t start, size_t end) {
    check_colargs(c, start, end);
    col_count[1]=end-start;
    colspace.setExtentSimple(1, col_count+1);
    colspace.selectAll();
    h5_start[0]=c;
    h5_start[1]=start;
    hspace.selectHyperslab(H5S_SELECT_SET, col_count, h5_start);
    return;
}

template<typename T, class V>
void HDF5_output<T, V>::fill_col(size_t c, T* in, size_t start, size_t end) {
    select_col(c, start, end);
    hdata.write(in, HDT, colspace, hspace);
    return;
}

template<typename T, class V>
void HDF5_output<T, V>::select_row(size_t r, size_t start, size_t end) {
    check_rowargs(r, start, end);
    row_count[0] = end-start;
    rowspace.setExtentSimple(1, row_count);
    rowspace.selectAll();
    h5_start[0] = start;
    h5_start[1] = r;
    hspace.selectHyperslab(H5S_SELECT_SET, row_count, h5_start);
    return;
}

template<typename T, class V>
void HDF5_output<T, V>::fill_row(size_t c, T* in, size_t start, size_t end) {
    select_row(c, start, end);
    hdata.write(in, HDT, rowspace, hspace);
    return;
}

template<typename T, class V>
void HDF5_output<T, V>::select_one(size_t r, size_t c) {
    check_oneargs(r, c);
    h5_start[0]=c;
    h5_start[1]=r;
    hspace.selectHyperslab(H5S_SELECT_SET, one_count, h5_start);
    return;
}

template<typename T, class V>
void HDF5_output<T, V>::fill(size_t r, size_t c, T in) {
    select_one(r, c);
    hdata.write(&in, HDT, onespace, hspace);
    return;
}

template<typename T, class V>
void HDF5_output<T, V>::get_row(size_t r, T* out, size_t start, size_t end) { 
    select_row(r, start, end);
    hdata.read(out, HDT, rowspace, hspace);
    return;
} 

template<typename T, class V>
void HDF5_output<T, V>::get_col(size_t c, T* out, size_t start, size_t end) { 
    select_col(c, start, end);
    hdata.read(out, HDT, colspace, hspace);
    return;
}

template<typename T, class V>
T HDF5_output<T, V>::get(size_t r, size_t c) { 
    select_one(r, c);
    T out;
    hdata.read(&out, HDT, onespace, hspace);
    return out;
}

// get_empty() defined for each realized class separately.

template<typename T, class V>
Rcpp::RObject HDF5_output<T, V>::yield() {
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
    if (this->nrow && this->ncol) { 
        h5seed.slot("first_val") = get(0, 0);
    } else {
        h5seed.slot("first_val") = V(0); // empty vector.
    }

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

