#ifndef TEMPLATE_METHODS_H 
#define TEMPLATE_METHODS_H 

/* Methods for the virtual base matrix. */

template<typename T, class V>
any_matrix<T, V>::any_matrix() : nrow(0), ncol(0) {}

template<typename T, class V>
any_matrix<T, V>::~any_matrix() {}

template<typename T, class V>
int any_matrix<T, V>::get_nrow() const { return nrow; }

template<typename T, class V>
int any_matrix<T, V>::get_ncol() const { return ncol; }

template<typename T, class V>
void any_matrix<T, V>::get_row(int r, typename V::iterator out) {
    get_row(r, out, 0, ncol);
    return;
}

template<typename T, class V>
void any_matrix<T, V>::get_col(int c, typename V::iterator out) {
    get_col(c, out, 0, nrow);
    return;
}

template<typename T, class V>
void any_matrix<T, V>::fill_dims(const Rcpp::RObject& dims) {
    Rcpp::IntegerVector d;
    if (dims.sexp_type()!=d.sexp_type() || (d=dims).size()!=2) { 
        throw std::runtime_error("matrix dimensions should be an integer vector of length 2");
    }
    nrow=d[0];
    ncol=d[1];
    if (nrow<0 || ncol<0) { throw std::runtime_error("dimensions should be non-negative"); }
    return;
}

/* Methods for a simple matrix. */

template<typename T, class V>
simple_matrix<T, V>::simple_matrix(const Rcpp::RObject& incoming) { 
    if (!incoming.hasAttribute("dim")) { 
        throw std::runtime_error("matrix object should have 'dim' attribute"); 
    }
    this->fill_dims(incoming.attr("dim"));
    const int& NC=this->ncol; 

    if (incoming.sexp_type()!=mat.sexp_type()) { 
        throw_custom_error("matrix should be ", translate_type(mat.sexp_type()), "");
    }
    mat=incoming;
    if (mat.size()!=(this->nrow)*NC) {
        throw std::runtime_error("length of matrix is inconsistent with its dimensions"); 
    }
    return;
}

template<typename T, class V>
simple_matrix<T, V>::~simple_matrix () {}

template<typename T, class V>
std::unique_ptr<any_matrix<T, V> > simple_matrix<T, V>::clone() const {
    return std::unique_ptr<any_matrix<T, V> >(new simple_matrix<T, V>(*this));
}

template<typename T, class V>
int simple_matrix<T, V>::get_index(int r, int c) const { return r + c*(this->nrow); }

template<typename T, class V>
void simple_matrix<T, V>::get_row(int r, typename V::iterator out, int start, int end) {
    const int& NR=this->nrow;
    auto src=mat.begin()+start*NR+r;
    for (int col=start; col<end; ++col, src+=NR, ++out) { (*out)=(*src); }
    return;
}

template<typename T, class V>
void simple_matrix<T, V>::get_col(int c, typename V::iterator out, int start, int end) {
    auto src=mat.begin() + c*(this->nrow);
    std::copy(src+start, src+end, out);
    return;
}

template<typename T, class V>
T simple_matrix<T, V>::get(int r, int c) {
    return mat[get_index(r, c)];
}

/* Methods for a dense Matrix. */

template <typename T, class V>
dense_matrix<T, V>::dense_matrix(const Rcpp::RObject& incoming) { 
    std::string ctype=check_Matrix_class(incoming, "geMatrix");
    this->fill_dims(incoming.attr("Dim"));
    const int& NC=this->ncol;
    
    Rcpp::RObject temp=get_safe_slot(incoming, "x"); 
    if (temp.sexp_type()!=x.sexp_type()) { 
        std::stringstream err;
        err << "'x' slot in a " << get_class(incoming) << " object should be " << translate_type(x.sexp_type());
        throw std::runtime_error(err.str().c_str());
    }
    x=temp;
    if (x.size()!=(this->nrow)*NC) {
        throw_custom_error("length of 'x' in a ", ctype, " object is inconsistent with its dimensions"); 
    }
    return;
}

template <typename T, class V>
dense_matrix<T, V>::~dense_matrix() {}

template<typename T, class V>
std::unique_ptr<any_matrix<T, V> > dense_matrix<T, V>::clone() const {
    return std::unique_ptr<any_matrix<T, V> >(new dense_matrix<T, V>(*this));
}

template <typename T, class V>
int dense_matrix<T, V>::get_index(int r, int c) const { return r + c*(this->nrow); }

template <typename T, class V>
void dense_matrix<T, V>::get_row(int r, typename V::iterator out, int start, int end) {
    const int& NR=this->nrow;
    auto src=x.begin()+start*NR+r;
    for (int col=start; col<end; ++col, src+=NR, ++out) { (*out)=*src; }
    return;
}

template <typename T, class V>
void dense_matrix<T, V>::get_col(int c, typename V::iterator out, int start, int end) {
    auto src=x.begin() + c*(this->nrow);
    std::copy(src+start, src+end, out);
    return;
}

template <typename T, class V>
T dense_matrix<T, V>::get(int r, int c) {
    return x[get_index(r, c)];
}

/* Methods for a Csparse matrix. */

template <typename T, class V, const T& Z>
Csparse_matrix<T, V, Z>::Csparse_matrix(const Rcpp::RObject& incoming) : nx(0), currow(0), curstart(0), curend(this->ncol) {
    std::string ctype=check_Matrix_class(incoming, "gCMatrix");  
    this->fill_dims(get_safe_slot(incoming, "Dim"));
    const int& NC=this->ncol;
    const int& NR=this->nrow;

    Rcpp::RObject temp_i=get_safe_slot(incoming, "i");
    if (temp_i.sexp_type()!=INTSXP) { throw_custom_error("'i' slot in a ", ctype, " object should be integer"); }
    i=temp_i;

    Rcpp::RObject temp_p=get_safe_slot(incoming, "p");
    if (temp_p.sexp_type()!=INTSXP) { throw_custom_error("'p' slot in a ", ctype, " object should be integer"); }
    p=temp_p;

    Rcpp::RObject temp_x=get_safe_slot(incoming, "x");
    if (temp_x.sexp_type()!=x.sexp_type()) { 
        std::stringstream err;
        err << "'x' slot in a " << get_class(incoming) << " object should be " << translate_type(x.sexp_type());
        throw std::runtime_error(err.str().c_str());
    }
    x=temp_x;
    nx=x.size();

    if (nx!=i.size()) { throw_custom_error("'x' and 'i' slots in a ", ctype, " object should have the same length"); }
    if (NC+1!=p.size()) { throw_custom_error("length of 'p' slot in a ", ctype, " object should be equal to 'ncol+1'"); }
    if (p[0]!=0) { throw_custom_error("first element of 'p' in a ", ctype, " object should be 0"); }
    if (p[NC]!=nx) { throw_custom_error("last element of 'p' in a ", ctype, " object should be 'length(x)'"); }

    // Checking all the indices.
    indices.resize(NC);
    auto pIt=p.begin();
    for (int px=0; px<NC; ++px) {
        const int& current=(indices[px]=*pIt); 
        if (current > *(++pIt)) { throw_custom_error("'p' slot in a ", ctype, " object should be sorted"); }
    }
    pIt=p.begin();
    for (int px=0; px<NC; ++px) {
        int left=*pIt;
        int right=*(++pIt)-1;
        auto iIt=i.begin()+left;

        for (int ix=left; ix<right; ++ix) {
            const int& current=*iIt;
            if (current > *(++iIt)) {
                throw_custom_error("'i' in each column of a ", ctype, " object should be sorted");
            }
        }
    }
    for (auto iIt=i.begin(); iIt!=i.end(); ++iIt) {
        const int& curi=*iIt;
        if (curi<0 || curi>=NR) {
            throw_custom_error("'i' slot in a ", ctype, " object should contain elements in [0, nrow)");
        }
    }

    return;
}

template <typename T, class V, const T& Z>
Csparse_matrix<T, V, Z>::~Csparse_matrix () {}

template<typename T, class V, const T& Z>
std::unique_ptr<any_matrix<T, V> > Csparse_matrix<T, V, Z>::clone() const {
    return std::unique_ptr<any_matrix<T, V> >(new Csparse_matrix<T, V, Z>(*this));
}

template <typename T, class V, const T& Z>
int Csparse_matrix<T, V, Z>::get_index(int r, int c) const {
    auto iend=i.begin() + p[c+1];
    auto loc=std::lower_bound(i.begin() + p[c], iend, r);
    if (loc!=iend && *loc==r) { 
        return loc - i.begin();
    } else {
        return nx;
    }
}

template <typename T, class V, const T& Z>
void Csparse_matrix<T, V, Z>::update_indices(int r, int start, int end) {
    /* If left/right slice are not equal to what is stored, we reset the indices,
     * so that the code below will know to recompute them. It's too much effort
     * to try to figure out exactly which columns need recomputing; just do them all.
     */
    if (start!=curstart || end!=curend) {
        curstart=start;
        curend=end;
        Rcpp::IntegerVector::iterator pIt=p.begin()+start;
        for (int px=start; px<end; ++px, ++pIt) {
            indices[px]=*pIt; 
        }
        currow=0;
    }

    /* entry of 'indices' for each column should contain the index of the first
     * element with row number not less than 'r'. If no such element exists, it
     * will contain the index of the first element of the next column.
     */
    if (r==currow) { 
        return; 
    } 

    Rcpp::IntegerVector::iterator pIt=p.begin()+start;
    if (r==currow+1) {
        ++pIt; // points to the first-past-the-end element, at any given 'c'.
        for (int c=start; c<end; ++c, ++pIt) {
            int& curdex=indices[c];
            if (curdex!=*pIt && i[curdex] < r) { 
                ++curdex;
            }
        }
    } else if (r+1==currow) {
        for (int c=start; c<end; ++c, ++pIt) {
            int& curdex=indices[c];
            if (curdex!=*pIt && i[curdex-1] >= r) { 
                --curdex;
            }
        }

    } else { 
        Rcpp::IntegerVector::iterator istart=i.begin(), loc;
        if (r > currow) {
            ++pIt; // points to the first-past-the-end element, at any given 'c'.
            for (int c=start; c<end; ++c, ++pIt) { 
                int& curdex=indices[c];
                loc=std::lower_bound(istart + curdex, istart + *pIt, r);
                curdex=loc - istart;
            }
        } else { 
            for (int c=start; c<end; ++c, ++pIt) {
                int& curdex=indices[c];
                loc=std::lower_bound(istart + *pIt, istart + curdex, r);
                curdex=loc - istart;
            }
        }
    }

    currow=r;
    return;
}

template <typename T, class V, const T& Z>
void Csparse_matrix<T, V, Z>::get_row(int r, typename V::iterator out, int start, int end) {
    update_indices(r, start, end);
    std::fill(out, out+end-start, Z);

    auto pIt=p.begin()+start+1; // Points to first-past-the-end for each 'c'.
    for (int c=start; c<end; ++c, ++pIt, ++out) { 
        const auto& idex=indices[c];
        if (idex!=*pIt && i[idex]==r) { (*out)=x[idex]; }
    } 
    return;  
}

template <typename T, class V, const T& Z>
void Csparse_matrix<T, V, Z>::get_col(int c, typename V::iterator out, int start, int end) {
    const int& pstart=p[c];
    auto iIt=i.begin()+pstart, 
         eIt=i.begin()+p[c+1]; 
    auto xIt=x.begin()+pstart;

    if (start) { // Jumping ahead if non-zero.
        auto new_iIt=std::lower_bound(iIt, eIt, start);
        xIt+=(new_iIt-iIt);
        iIt=new_iIt;
    } 
    if (end!=(this->nrow)) { // Jumping to last element.
        eIt=std::lower_bound(iIt, eIt, end);
    }

    std::fill(out, out+end-start, Z);
    for (; iIt!=eIt; ++iIt, ++xIt) {
        *(out + *iIt - start)=*xIt;
    }
    return;
}

template <typename T, class V, const T& Z>
T Csparse_matrix<T, V, Z>::get(int r, int c) {
    const int index=get_index(r, c);
    return (index!=nx ? x[index] : Z);
}

/* Methods for a Psymm matrix. */

template <typename T, class V>
Psymm_matrix<T, V>::Psymm_matrix(const Rcpp::RObject& incoming) : upper(true) { 
    std::string ctype=check_Matrix_class(incoming, "spMatrix");  
    this->fill_dims(get_safe_slot(incoming, "Dim"));
    const int& NR=this->nrow;
    if (NR!=(this->ncol)) { throw_custom_error("'nrow' and 'ncol' should be equal for a ", ctype, " object"); }

    Rcpp::RObject temp=get_safe_slot(incoming, "x");
    if (temp.sexp_type()!=x.sexp_type()) { 
        std::stringstream err;
        err << "'x' slot in a " << get_class(incoming) << " object should be " << translate_type(x.sexp_type());
        throw std::runtime_error(err.str().c_str());
    }
    x=temp;
    if (x.size()!=(NR*(NR+1))/2) { throw_custom_error("length of 'x' in a ", ctype, " object is inconsistent with its dimensions"); }
   
    // Checking up/lo slot. 
    Rcpp::RObject ul=get_safe_slot(incoming, "uplo");
    std::string uplo;
    try {
        uplo=make_to_string(ul);
        if (uplo.length()!=1) { throw; }
    } catch (...) { 
        throw_custom_error("'uplo' slot in a ", ctype, " object should be a string of length 1"); 
    }

    switch (uplo[0]) { 
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

template <typename T, class V>
Psymm_matrix<T, V>::~Psymm_matrix () {}

template<typename T, class V>
std::unique_ptr<any_matrix<T, V> > Psymm_matrix<T, V>::clone() const {
    return std::unique_ptr<any_matrix<T, V> >(new Psymm_matrix<T, V>(*this));
}

template <typename T, class V>
int Psymm_matrix<T, V>::get_index(int r, int c) const {
    if (upper) {
        if (c > r) { 
            return (c*(c+1))/2 + r;            
        } else {
            return (r*(r+1))/2 + c;            
        }
    } else {
        if (r > c) { 
            return (this->nrow) * c - (c * (c - 1))/2 + r - c;
        } else {
            return (this->nrow) * r - (r * (r - 1))/2 + c - r;
        }
    }
}

template <typename T, class V>
void Psymm_matrix<T, V>::get_col (int c, typename V::iterator out, int start, int end) {
    auto xIt=x.begin();
    if (upper) {
        xIt+=(c*(c+1))/2;
        if (start < c) {
            if (end <= c) { 
                std::copy(xIt+start, xIt+end, out);
            } else {
                std::copy(xIt+start, xIt+c, out);
                out+=c - start;
                for (int i=c; i<end; ++i, ++out) {
                    (*out)=*(xIt+c);
                    xIt+=i+1;
                }
            }
        } else {
            int diff=start - c;
            xIt+=c*diff+(diff*(diff+1))/2;
            for (int i=start; i<end; ++i, ++out) {
                (*out)=*(xIt+c);
                xIt+=i+1;
            }
        }
    } else {
        const int& NR=this->nrow;
        if (start < c) { 
            xIt+=NR*start - (start*(start-1))/2;
            if (end <= c) {
                for (int i=start; i<end; ++i, ++out) {
                    (*out)=*(xIt+c-i);
                    xIt+=NR-i;
                }
            } else {
                for (int i=start; i<c; ++i, ++out) {
                    (*out)=*(xIt+c-i);
                    xIt+=NR-i;
                }
                std::copy(xIt, xIt+end-c, out);
            }
        } else {
            xIt+=NR*c - (c*(c-1))/2;
            std::copy(xIt + start - c, xIt+end - c, out);
        }
    }
    return;
}

template <typename T, class V>
void Psymm_matrix<T, V>::get_row (int r, typename V::iterator out, int start, int end) {
    get_col(r, out, start, end);
    return;
}

template <typename T, class V>
T Psymm_matrix<T, V>::get(int r, int c) {
    return x[get_index(r, c)];
}

/* Methods for a HDF5 matrix. */

#ifdef BEACHMAT_USE_HDF5

template<typename T, class V, H5T_class_t HTC, const H5::PredType& HPT>
HDF5_matrix<T, V, HTC, HPT>::HDF5_matrix(const Rcpp::RObject& incoming) : realized(R_NilValue) { 
    std::string ctype=get_class(incoming);
    if (incoming.isS4()) {
        if (ctype=="DelayedMatrix") { 
            Rcpp::Environment delayenv("package:DelayedArray");
            Rcpp::Function realfun=delayenv["realize"];
            realized=realfun(incoming, "HDF5Array");
        } else if (ctype=="HDF5Matrix") {
            realized=incoming;
        }
    }
    if (realized==R_NilValue) {
        throw std::runtime_error("matrix should be a HDF5Matrix or DelayedMatrix object");
    }

    const Rcpp::RObject& h5_seed=get_safe_slot(realized, "seed");
    std::string stype=get_class(h5_seed);
    if (!h5_seed.isS4() || stype!="HDF5ArraySeed") {
        throw_custom_error("'seed' slot in a ", ctype, " object should be a HDF5ArraySeed object");
    }

    // Checking first value.
    const Rcpp::RObject& firstval=get_safe_slot(h5_seed, "first_val");
    V temp;
    if (firstval.sexp_type()!=temp.sexp_type()) { 
        std::stringstream err;
        err << "'first_val' slot in a " << get_class(h5_seed) << " object should be " << translate_type(temp.sexp_type());
        throw std::runtime_error(err.str().c_str());
    }

    // Checking dimensions.
    if (!h5_seed.hasAttribute("dim")) { 
        throw_custom_error("", stype, " object should have 'dim' attribute"); 
    }
    this->fill_dims(h5_seed.attr("dim"));
    const int& NC=this->ncol;
    const int& NR=this->nrow;

    std::string fname;
    try {
        fname=make_to_string(get_safe_slot(h5_seed, "file"));
    } catch (...) { 
        throw_custom_error("'file' slot in a ", stype, " object should be a string");
    }

    std::string dataset;
    try {
        dataset=make_to_string(get_safe_slot(h5_seed, "name"));
    } catch (...) { 
        throw_custom_error("'name' slot in a ", stype, " object should be a string");
    }
    
    // Setting up the HDF5 accessors.
    hfile.openFile(H5std_string(fname), H5F_ACC_RDONLY);
    hdata = hfile.openDataSet(H5std_string(dataset));
    if (hdata.getTypeClass()!=HTC) {
        std::stringstream err;
        err << "data type in HDF5 file is not ";
        if (HTC==H5T_FLOAT) { err << "floating-point"; }
        else if (HTC==H5T_INTEGER) { err << "integer"; }
        else { err << "the specified type"; }
        throw std::runtime_error(err.str().c_str());
    }

    hspace = hdata.getSpace();
    if (hspace.getSimpleExtentNdims()!=2) {
        throw std::runtime_error("data in HDF5 file is not a two-dimensional array");
    }

    hsize_t dims_out[2];
    hspace.getSimpleExtentDims(dims_out, NULL);
    if (dims_out[1]!=NR || dims_out[0]!=NC) { 
        throw_custom_error("dimensions in HDF5 file do not equal dimensions in the ", ctype, " object");
    }

    h5_start[0]=0;
    h5_start[1]=0;
    col_count[0]=1;
    col_count[1]=NR;
    row_count[0]=NC;
    row_count[1]=1;

    zero_start[0]=0;
    one_count[0]=1;
    one_count[1]=1;
    onespace.setExtentSimple(1, one_count);
    onespace.selectAll();
    return;
}

template<typename T, class V, H5T_class_t HTC, const H5::PredType& HPT>
HDF5_matrix<T, V, HTC, HPT>::~HDF5_matrix() {}

template<typename T, class V, H5T_class_t HTC, const H5::PredType& HPT>
std::unique_ptr<any_matrix<T, V> > HDF5_matrix<T, V, HTC, HPT>::clone() const {
    return std::unique_ptr<any_matrix<T, V> >(new HDF5_matrix<T, V, HTC, HPT>(*this));
}

template<typename T, class V, H5T_class_t HTC, const H5::PredType& HPT>
void HDF5_matrix<T, V, HTC, HPT>::select_row(int r, int start, int end) {
    row_count[0] = end-start;
    rowspace.setExtentSimple(1, row_count);
    rowspace.selectAll();
    h5_start[0] = start;
    h5_start[1] = r;
    hspace.selectHyperslab(H5S_SELECT_SET, row_count, h5_start);
    return;
}

template<typename T, class V, H5T_class_t HTC, const H5::PredType& HPT>
void HDF5_matrix<T, V, HTC, HPT>::get_row(int r, typename V::iterator out, int start, int end) { 
    select_row(r, start, end);
    hdata.read(&(*out), HPT, rowspace, hspace);
    return;
} 

template<typename T, class V, H5T_class_t HTC, const H5::PredType& HPT>
void HDF5_matrix<T, V, HTC, HPT>::select_col(int c, int start, int end) {
    col_count[1] = end-start;
    colspace.setExtentSimple(1, col_count+1);
    colspace.selectAll();
    h5_start[0] = c;
    h5_start[1] = start;
    hspace.selectHyperslab(H5S_SELECT_SET, col_count, h5_start);
    return;
}

template<typename T, class V, H5T_class_t HTC, const H5::PredType& HPT>
void HDF5_matrix<T, V, HTC, HPT>::get_col(int c, typename V::iterator out, int start, int end) { 
    select_col(c, start, end);
    hdata.read(&(*out), HPT, colspace, hspace);
    return;
}

template<typename T, class V, H5T_class_t HTC, const H5::PredType& HPT>
void HDF5_matrix<T, V, HTC, HPT>::select_one(int r, int c) {
    h5_start[0]=c;
    h5_start[1]=r;
    hspace.selectHyperslab( H5S_SELECT_SET, one_count, h5_start);
    return;
}
 
template<typename T, class V, H5T_class_t HTC, const H5::PredType& HPT>
T HDF5_matrix<T, V, HTC, HPT>::get(int r, int c) { 
    select_one(r, c);
    T out;
    hdata.read(&out, HPT, onespace, hspace);
    return out;
}

#endif

#endif
