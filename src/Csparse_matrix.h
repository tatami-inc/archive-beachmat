/*** Class definition ***/

template<typename T, class V>
class Csparse_matrix : public any_matrix {
public:    
    Csparse_matrix(const Rcpp::RObject&);
    ~Csparse_matrix();

    T get(size_t, size_t);

    template <class Iter>
    void get_row(size_t, Iter, size_t, size_t);

    template <class Iter>
    void get_col(size_t, Iter, size_t, size_t);

    template<class Iter>
    size_t get_nonzero_row(size_t, Rcpp::IntegerVector::iterator, Iter, size_t, size_t);

    template<class Iter>
    size_t get_nonzero_col(size_t, Rcpp::IntegerVector::iterator, Iter, size_t, size_t);

    matrix_type get_matrix_type () const;
protected:
    Rcpp::IntegerVector i, p;
    V x;

    size_t currow, curstart, curend;
    std::vector<int> indices; // Left as 'int' to simplify comparisons with 'i' and 'p'.
    void update_indices(size_t, size_t, size_t);

    T get_empty() const; // Specialized function for each realization (easy to extend for non-int/double).
};

/*** Constructor definition ***/

template <typename T, class V>
Csparse_matrix<T, V>::Csparse_matrix(const Rcpp::RObject& incoming) : currow(0), curstart(0), curend(this->ncol) {
    std::string ctype=check_Matrix_class(incoming, "gCMatrix");  
    this->fill_dims(get_safe_slot(incoming, "Dim"));
    const size_t& NC=this->ncol;
    const size_t& NR=this->nrow;

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

    if (x.size()!=i.size()) { throw_custom_error("'x' and 'i' slots in a ", ctype, " object should have the same length"); }
    if (NC+1!=p.size()) { throw_custom_error("length of 'p' slot in a ", ctype, " object should be equal to 'ncol+1'"); }
    if (p[0]!=0) { throw_custom_error("first element of 'p' in a ", ctype, " object should be 0"); }
    if (p[NC]!=x.size()) { throw_custom_error("last element of 'p' in a ", ctype, " object should be 'length(x)'"); }

    // Checking all the indices.
    indices.resize(NC);
    auto pIt=p.begin();
    for (size_t px=0; px<NC; ++px) {
        if (*pIt < 0) { throw_custom_error("'p' slot in a ", ctype, " object should contain non-negative values"); }
        const int& current=(indices[px]=*pIt); 
        if (current > *(++pIt)) { throw_custom_error("'p' slot in a ", ctype, " object should be sorted"); }
    }

    pIt=p.begin();
    for (size_t px=0; px<NC; ++px) {
        int left=*pIt; // Integers as that's R's storage type. 
        int right=*(++pIt)-1; // Not checking the last element, as this is the start of the next column.
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

template <typename T, class V>
Csparse_matrix<T, V>::~Csparse_matrix () {}

/*** Getter functions ***/

template <typename T, class V>
T Csparse_matrix<T, V>::get(size_t r, size_t c) {
    check_oneargs(r, c);
    auto iend=i.begin() + p[c+1];
    auto loc=std::lower_bound(i.begin() + p[c], iend, r);
    if (loc!=iend && *loc==r) { 
        return x[loc - i.begin()];
    } else {
        return get_empty();
    }
}

template <typename T, class V>
void Csparse_matrix<T, V>::update_indices(size_t r, size_t first, size_t last) {
    /* If left/right slice are not equal to what is stored, we reset the indices,
     * so that the code below will know to recompute them. It's too much effort
     * to try to figure out exactly which columns need recomputing; just do them all.
     */
    if (first!=curstart || last!=curend) {
        curstart=first;
        curend=last;
        Rcpp::IntegerVector::iterator pIt=p.begin()+first;
        for (size_t px=first; px<last; ++px, ++pIt) {
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

    Rcpp::IntegerVector::iterator pIt=p.begin()+first;
    if (r==currow+1) {
        ++pIt; // points to the first-past-the-end element, at any given 'c'.
        for (size_t c=first; c<last; ++c, ++pIt) {
            int& curdex=indices[c];
            if (curdex!=*pIt && i[curdex] < r) { 
                ++curdex;
            }
        }
    } else if (r+1==currow) {
        for (size_t c=first; c<last; ++c, ++pIt) {
            int& curdex=indices[c];
            if (curdex!=*pIt && i[curdex-1] >= r) { 
                --curdex;
            }
        }

    } else { 
        Rcpp::IntegerVector::iterator istart=i.begin(), loc;
        if (r > currow) {
            ++pIt; // points to the first-past-the-end element, at any given 'c'.
            for (size_t c=first; c<last; ++c, ++pIt) { 
                int& curdex=indices[c];
                loc=std::lower_bound(istart + curdex, istart + *pIt, r);
                curdex=loc - istart;
            }
        } else { 
            for (size_t c=first; c<last; ++c, ++pIt) {
                int& curdex=indices[c];
                loc=std::lower_bound(istart + *pIt, istart + curdex, r);
                curdex=loc - istart;
            }
        }
    }

    currow=r;
    return;
}

template <typename T, class V>
template <class Iter>
void Csparse_matrix<T, V>::get_row(size_t r, Iter out, size_t first, size_t last) {
    check_rowargs(r, first, last);
    update_indices(r, first, last);
    std::fill(out, out+last-first, get_empty());

    auto pIt=p.begin()+first+1; // Points to first-past-the-end for each 'c'.
    for (size_t c=first; c<last; ++c, ++pIt, ++out) { 
        const int& idex=indices[c];
        if (idex!=*pIt && i[idex]==r) { (*out)=x[idex]; }
    } 
    return;  
}

template <typename T, class V>
template <class Iter>
void Csparse_matrix<T, V>::get_col(size_t c, Iter out, size_t first, size_t last) {
    check_colargs(c, first, last);
    const int& pstart=p[c]; 
    auto iIt=i.begin()+pstart, 
         eIt=i.begin()+p[c+1]; 
    auto xIt=x.begin()+pstart;

    if (first) { // Jumping ahead if non-zero.
        auto new_iIt=std::lower_bound(iIt, eIt, first);
        xIt+=(new_iIt-iIt);
        iIt=new_iIt;
    } 
    if (last!=(this->nrow)) { // Jumping to last element.
        eIt=std::lower_bound(iIt, eIt, last);
    }

    std::fill(out, out+last-first, get_empty());
    for (; iIt!=eIt; ++iIt, ++xIt) {
        *(out + (*iIt - int(first)))=*xIt;
    }
    return;
}

template<typename T, class V>
template<class Iter>
size_t Csparse_matrix<T, V>::get_nonzero_row(size_t r, Rcpp::IntegerVector::iterator index, Iter val, size_t first, size_t last) {
    check_rowargs(r, first, last);
    update_indices(r, first, last);

    auto pIt=p.begin()+first+1; // Points to first-past-the-end for each 'c'.
    size_t nzero=0;
    for (size_t c=first; c<last; ++c, ++pIt) { 
        const int& idex=indices[c];
        if (idex!=*pIt && i[idex]==r) { 
            ++nzero;
            (*index)=c;
            (*val)=x[idex];
            ++index;
            ++val;
        }
    } 

    return nzero;
}

template<typename T, class V>
template<class Iter>
size_t Csparse_matrix<T, V>::get_nonzero_col(size_t c, Rcpp::IntegerVector::iterator index, Iter val, size_t first, size_t last) {
    check_colargs(c, first, last);
    const int& pstart=p[c]; 
    auto iIt=i.begin()+pstart, 
         eIt=i.begin()+p[c+1]; 
    auto xIt=x.begin()+pstart;

    if (first) { // Jumping ahead if non-zero.
        auto new_iIt=std::lower_bound(iIt, eIt, first);
        xIt+=(new_iIt-iIt);
        iIt=new_iIt;
    } 
    if (last!=(this->nrow)) { // Jumping to last element.
        eIt=std::lower_bound(iIt, eIt, last);
    }

    size_t nzero=eIt-iIt;

    std::copy(iIt, iIt+nzero, index);
    std::copy(xIt, xIt+nzero, val);
    return nzero;
}

template<typename T, class V>
matrix_type Csparse_matrix<T, V>::get_matrix_type() const {
    return SPARSE;
}
