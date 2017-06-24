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

/* Methods for the sparse output matrix. */

template<typename T, class V>
sparse_output<T, V>::sparse_output(size_t nr, size_t nc) : any_matrix(nr, nc), data(nc) {}

template<typename T, class V>
sparse_output<T, V>::~sparse_output() {}

template<typename T, class V>
template<class Iter>
void sparse_output<T, V>::fill_col(size_t c, Iter in, size_t start, size_t end) {
    check_colargs(c, start, end);
    std::deque<data_pair>& current=data[c];
    std::deque<data_pair> new_set;

    // Filling in all elements before start.
    auto cIt=current.begin(); 
    while (cIt!=current.end() && cIt->first < start) {
        new_set.push_back(*cIt);
        ++cIt;
    }
   
    // Filling in all non-empty elements. 
    for (size_t index=start; index<end; ++index, ++in) {
        if ((*in)!=get_empty()) { 
            new_set.push_back(data_pair(index, *in));
        }
    } 

    // Jumping to the end.
    while (cIt!=current.end() && cIt->first < end) {
        ++cIt;
    }

    // Filling in remaining elements.
    while (cIt!=current.end()) {
        new_set.push_back(*cIt);
        ++cIt;
    }

    current.swap(new_set);
    return;
}

template<typename T, class V>
template <class Iter>
Iter sparse_output<T, V>::find_matching_row(Iter begin, Iter end, const data_pair& incoming) {
    return std::lower_bound(begin, end, incoming, 
            [](const data_pair& lhs, const data_pair& rhs) -> bool { return lhs.first < rhs.first; }); // Using a lambda to only compare first value.
}

template<typename T, class V>
template<class Iter>
void sparse_output<T, V>::fill_row(size_t r, Iter in, size_t start, size_t end) {
    check_rowargs(r, start, end);
    for (size_t c=start; c<end; ++c, ++in) {
        if ((*in)==get_empty()) { continue; }

        std::deque<data_pair>& current=data[c];
        if (current.size()) {
            if (r < current.front().first) {
                current.push_front(data_pair(r, *in));
            } else if (r==current.front().first) {
                current.front().second=*in;
            } else if (r > current.back().first) {
                current.push_back(data_pair(r, *in));
            } else if (r==current.back().first) {
                current.back().second=*in;
            } else {
                data_pair incoming(r, *in);
                auto insert_loc=find_matching_row(current.begin(), current.end(), incoming);
                if (insert_loc!=current.end() && insert_loc->first==r) { 
                    insert_loc->second=*in;
                } else {
                    current.insert(insert_loc, incoming);
                }
            }
        } else {
            current.push_back(data_pair(r, *in));
        }
    }
    return;
}

template<typename T, class V>
void sparse_output<T, V>::fill(size_t r, size_t c, T in) {
    check_oneargs(r, c);
    fill_row(r, &in, c, c+1);
    return;
}

template<typename T, class V>
template<class Iter>
void sparse_output<T, V>::get_row(size_t r, Iter out, size_t start, size_t end) {
    check_rowargs(r, start, end);
    std::fill(out, out+end-start, get_empty());

    for (size_t col=start; col<end; ++col, ++out) {
        const std::deque<data_pair>& current=data[col];
        if (current.empty() || r>current.back().first || r<current.front().first) {
            continue; 
        }
        if (r==current.back().first) { 
            (*out)=current.back().second;
        } else if (r==current.front().first) {
            (*out)=current.front().second;
        } else {
            auto loc=find_matching_row(current.begin(), current.end(), data_pair(r, *out)); // Equivalent to get_empty(), due to fill.
            if (loc!=current.end() && loc->first==r) { 
                (*out)=loc->second;
            }
        }
    }
    return;
}

template<typename T, class V>
template<class Iter>
void sparse_output<T, V>::get_col(size_t c, Iter out, size_t start, size_t end) {
    check_colargs(c, start, end);
    const std::deque<data_pair>& current=data[c];

    // Jumping forwards.
    auto cIt=current.begin();
    if (start) {
        cIt=find_matching_row(current.begin(), current.end(), data_pair(start, get_empty()));
    }
    
    std::fill(out, out+end-start, get_empty());
    while (cIt!=current.end() && cIt->first < end) { 
        *(out + (cIt->first - start)) = cIt->second;
        ++cIt;
    }
    return;
}

template<typename T, class V>
T sparse_output<T, V>::get(size_t r, size_t c) {
    check_oneargs(r, c);
    const std::deque<data_pair>& current=data[c];
    auto cIt=find_matching_row(current.begin(), current.end(), data_pair(r, get_empty()));
    if (cIt!=current.end() && cIt->first==r) {
        return cIt->second;
    } else {
        return get_empty();
    }
}

template<typename T, class V>
Rcpp::RObject sparse_output<T, V>::yield() {
    const int RTYPE=V().sexp_type();
    std::string classname;
    switch (RTYPE) { 
        case LGLSXP:
            classname="lgCMatrix";
            break;
        case REALSXP:
            classname="dgCMatrix";
            break;
        default:
            std::stringstream err;
            err << "unsupported sexptype '" << RTYPE << "' for sparse output";
            throw std::runtime_error(err.str().c_str());
    }
    Rcpp::S4 mat(classname);

    // Setting dimensions.
    if (!mat.hasSlot("Dim")) {
        throw_custom_error("missing 'Dim' slot in ", classname, " object");
    }
    mat.slot("Dim") = Rcpp::IntegerVector::create(this->nrow, this->ncol);

    // Setting 'p'.
    if (!mat.hasSlot("p")) {
        throw_custom_error("missing 'p' slot in ", classname, " object");
    }
    Rcpp::IntegerVector p(this->ncol+1, 0);
    auto pIt=p.begin()+1;
    size_t total_size=0;
    for (auto dIt=data.begin(); dIt!=data.end(); ++dIt, ++pIt) { 
        total_size+=dIt->size();
        (*pIt)=total_size;
    }
    mat.slot("p")=p;

    // Setting 'i' and 'x'.
    Rcpp::IntegerVector i(total_size);
    V x(total_size);
    if (!mat.hasSlot("i")) {
        throw_custom_error("missing 'i' slot in ", classname, " object");
    }
    if (!mat.hasSlot("x")) {
        throw_custom_error("missing 'x' slot in ", classname, " object");
    }
    auto xIt=x.begin();
    auto iIt=i.begin();
    for (size_t c=0; c<this->ncol; ++c) {
        auto current=data[c];
        for (auto cIt=current.begin(); cIt!=current.end(); ++cIt, ++xIt, ++iIt) {
            (*iIt)=cIt->first;
            (*xIt)=cIt->second;
        }
    }
    mat.slot("i")=i;
    mat.slot("x")=x;

    return SEXP(mat);
}

/* Methods for HDF5 output matrix. */

template<typename T, class V>
HDF5_output<T, V>::HDF5_output (size_t nr, size_t nc, size_t chunk_nr, size_t chunk_nc, int compress) : any_matrix(nr, nc), 
        onrow(true), oncol(true), rowokay(false), colokay(false), largerrow(false), largercol(false), // assuming contiguous.
        rowlist(H5::FileAccPropList::DEFAULT.getId()), collist(H5::FileAccPropList::DEFAULT.getId()) {

    // Pulling out settings.
    const Rcpp::Environment env=Rcpp::Environment::namespace_env("beachmat");
    Rcpp::Function fun=env["setupHDF5Array"];
    const int RTYPE=V().sexp_type();
    Rcpp::List collected=fun(Rcpp::IntegerVector::create(this->nrow, this->ncol), Rcpp::StringVector(translate_type(RTYPE)),
                             Rcpp::IntegerVector::create(chunk_nr, chunk_nc), compress);

    if (collected.size()!=4) { 
        throw std::runtime_error("output of setupHDF5Array should be a list of four elements");
    }
    fname=make_to_string(collected[0]);
    dname=make_to_string(collected[1]);
    Rcpp::IntegerVector r_chunks=collected[2];
    if (r_chunks.size()!=2) { 
        throw std::runtime_error("chunk dimensions should be an integer vector of length 2");
    }
    chunk_nr=r_chunks[0];
    chunk_nc=r_chunks[1];
    Rcpp::IntegerVector r_compress=collected[3];
    if (r_compress.size()!=1) {
        throw std::runtime_error("compression should be an integer scalar");
    }
    compress=r_compress[0];

    // Opening the file.
    hfile.openFile(fname, H5F_ACC_RDWR);

    // Type setting.
    switch (RTYPE) {
        case REALSXP:
            default_type=H5::DataType(H5::PredType::NATIVE_DOUBLE);
            break;
        case INTSXP: case LGLSXP:
            default_type=H5::DataType(H5::PredType::NATIVE_INT32);
            break;
        default:
            std::stringstream err;
            err << "unsupported sexptype '" << RTYPE << "' for HDF5 output";
            throw std::runtime_error(err.str().c_str());
    }

    // Creating the data set.
    H5::DSetCreatPropList plist;
    const T empty=get_empty();
    plist.setFillValue(default_type, &empty);
        
    // Setting the chunk dimensions if not contiguous.
    if (compress>0) {
        std::vector<hsize_t> chunk_dims(2);
        chunk_dims[0]=chunk_nc; // flipping them, as rhdf5 internally transposes it.
        chunk_dims[1]=chunk_nr;
        plist.setChunk(2, chunk_dims.data());
        plist.setDeflate(compress);
    }

    std::vector<hsize_t> dims(2);
    dims[0]=this->ncol; // Setting the dimensions (0 is column, 1 is row; internally transposed).
    dims[1]=this->nrow; 

    hspace.setExtentSimple(2, dims.data());
    hdata=hfile.createDataSet(dname, default_type, hspace, plist); 

    // Setting other values.
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

    // Setting the chunk cache parameters.
    calc_HDF5_chunk_cache_settings(this->nrow, this->ncol, hdata.getCreatePlist(), default_type, 
            onrow, oncol, rowokay, colokay, largerrow, largercol, rowlist, collist);
    return;
}

template<typename T, class V>
HDF5_output<T, V>::~HDF5_output() {}

template<typename T, class V>
void HDF5_output<T, V>::select_col(size_t c, size_t start, size_t end) {
    check_colargs(c, start, end);
    if (oncol || (onrow && largerrow)) {
        ; // Don't do anything, it's okay.
    } else if (!colokay) {
        std::stringstream err;
        err << "cache size limit (" << get_cache_size_hard_limit() << ") exceeded for column access, repack the file";
        throw std::runtime_error(err.str().c_str());
    } else {
        hdata.close();
        hfile.close();
        hfile.openFile(fname.c_str(), H5F_ACC_RDWR, collist);
        hdata = hfile.openDataSet(dname.c_str());
        oncol=true;
    }

    col_count[1]=end-start;
    colspace.setExtentSimple(1, col_count+1);
    colspace.selectAll();
    h5_start[0]=c;
    h5_start[1]=start;
    hspace.selectHyperslab(H5S_SELECT_SET, col_count, h5_start);
    return;
}

template<typename T, class V>
template<typename X>
void HDF5_output<T, V>::fill_col(size_t c, const X* in, const H5::DataType& HDT, size_t start, size_t end) {
    select_col(c, start, end);
    hdata.write(in, HDT, colspace, hspace);
    return;
}

template<typename T, class V>
void HDF5_output<T, V>::fill_col(size_t c, const T* in, size_t start, size_t end) {
    fill_col(c, in, default_type, start, end);
    return;
}

template<typename T, class V>
void HDF5_output<T, V>::select_row(size_t r, size_t start, size_t end) {
    check_rowargs(r, start, end);
    if (onrow || (oncol && largercol)) {
        ; // Don't do anything, it's okay.
    } else if (!rowokay) {
        std::stringstream err;
        err << "cache size limit (" << get_cache_size_hard_limit() << ") exceeded for row access, repack the file";
        throw std::runtime_error(err.str().c_str());
    } else {
        hdata.close();
        hfile.close();
        hfile.openFile(fname.c_str(), H5F_ACC_RDWR, rowlist);
        hdata = hfile.openDataSet(dname.c_str());
        onrow=true;
    }

    row_count[0] = end-start;
    rowspace.setExtentSimple(1, row_count);
    rowspace.selectAll();
    h5_start[0] = start;
    h5_start[1] = r;
    hspace.selectHyperslab(H5S_SELECT_SET, row_count, h5_start);
    return;
}

template<typename T, class V>
template<typename X>
void HDF5_output<T, V>::fill_row(size_t c, const X* in, const H5::DataType& HDT, size_t start, size_t end) {
    select_row(c, start, end);
    hdata.write(in, HDT, rowspace, hspace);
    return;
}

template<typename T, class V>
void HDF5_output<T, V>::fill_row(size_t c, const T* in, size_t start, size_t end) {
    fill_row(c, in, default_type, start, end);
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
    hdata.write(&in, default_type, onespace, hspace);
    return;
}

template<typename T, class V>
template<typename X>
void HDF5_output<T, V>::get_row(size_t r, X* out, const H5::DataType& HDT, size_t start, size_t end) { 
    select_row(r, start, end);
    hdata.read(out, HDT, rowspace, hspace);
    return;
} 

template<typename T, class V>
void HDF5_output<T, V>::get_row(size_t r, T* out, size_t start, size_t end) { 
    get_row(r, out, default_type, start, end);
    return;
} 

template<typename T, class V>
template<typename X>
void HDF5_output<T, V>::get_col(size_t c, X* out, const H5::DataType& HDT, size_t start, size_t end) { 
    select_col(c, start, end);
    hdata.read(out, HDT, colspace, hspace);
    return;
}

template<typename T, class V>
void HDF5_output<T, V>::get_col(size_t c, T* out, size_t start, size_t end) { 
    get_col(c, out, default_type, start, end);
    return;
}

template<typename T, class V>
T HDF5_output<T, V>::get(size_t r, size_t c) { 
    select_one(r, c);
    T out;
    hdata.read(&out, default_type, onespace, hspace);
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

