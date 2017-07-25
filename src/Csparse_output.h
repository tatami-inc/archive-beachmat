#ifndef BEACHMAT_CSPARSE_OUTPUT_H
#define BEACHMAT_CSPARSE_OUTPUT_H

#include "beachmat.h"
#include "utils.h"
#include "any_matrix.h"

namespace beachmat { 

/*** Class definition ***/

template<typename T, class V>
class Csparse_output : public any_matrix {
public:
    Csparse_output(size_t, size_t);
    ~Csparse_output();

    template <class Iter>
    void set_row(size_t, Iter, size_t, size_t);
    template <class Iter>
    void set_col(size_t, Iter, size_t, size_t);
    void set(size_t, size_t, T);

    template <class Iter>
    void get_col(size_t, Iter, size_t, size_t);
    template <class Iter>
    void get_row(size_t, Iter, size_t, size_t);
    T get(size_t, size_t);

    Rcpp::RObject yield();

    matrix_type get_matrix_type() const;
private:
    typedef std::pair<size_t, T> data_pair;
    std::vector<std::deque<data_pair> > data;

    T get_empty() const;
    template <class Iter>
    Iter find_matching_row(Iter, Iter, const data_pair&);
};

/*** Constructor definition ***/

template<typename T, class V>
Csparse_output<T, V>::Csparse_output(size_t nr, size_t nc) : any_matrix(nr, nc), data(nc) {}

template<typename T, class V>
Csparse_output<T, V>::~Csparse_output() {}

/*** Setter methods ***/

template<typename T, class V>
template<class Iter>
void Csparse_output<T, V>::set_col(size_t c, Iter in, size_t first, size_t last) {
    check_colargs(c, first, last);
    std::deque<data_pair>& current=data[c];
    std::deque<data_pair> new_set;

    // Filling in all elements before start.
    auto cIt=current.begin(); 
    while (cIt!=current.end() && cIt->first < first) {
        new_set.push_back(*cIt);
        ++cIt;
    }
   
    // Filling in all non-empty elements. 
    for (size_t index=first; index<last; ++index, ++in) {
        if ((*in)!=get_empty()) { 
            new_set.push_back(data_pair(index, *in));
        }
    } 

    // Jumping to the end.
    while (cIt!=current.end() && cIt->first < last) {
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
Iter Csparse_output<T, V>::find_matching_row(Iter begin, Iter end, const data_pair& incoming) {
    return std::lower_bound(begin, end, incoming, 
            [](const data_pair& lhs, const data_pair& rhs) -> bool { return lhs.first < rhs.first; }); // Using a lambda to only compare first value.
}

template<typename T, class V>
template<class Iter>
void Csparse_output<T, V>::set_row(size_t r, Iter in, size_t first, size_t last) {
    check_rowargs(r, first, last);
    for (size_t c=first; c<last; ++c, ++in) {
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
void Csparse_output<T, V>::set(size_t r, size_t c, T in) {
    check_oneargs(r, c);
    set_row(r, &in, c, c+1);
    return;
}

/*** Getter methods ***/

template<typename T, class V>
template<class Iter>
void Csparse_output<T, V>::get_row(size_t r, Iter out, size_t first, size_t last) {
    check_rowargs(r, first, last);
    std::fill(out, out+last-first, get_empty());

    for (size_t col=first; col<last; ++col, ++out) {
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
void Csparse_output<T, V>::get_col(size_t c, Iter out, size_t first, size_t last) {
    check_colargs(c, first, last);
    const std::deque<data_pair>& current=data[c];

    // Jumping forwards.
    auto cIt=current.begin();
    if (first) {
        cIt=find_matching_row(current.begin(), current.end(), data_pair(first, get_empty()));
    }
    
    std::fill(out, out+last-first, get_empty());
    while (cIt!=current.end() && cIt->first < last) { 
        *(out + (cIt->first - first)) = cIt->second;
        ++cIt;
    }
    return;
}

template<typename T, class V>
T Csparse_output<T, V>::get(size_t r, size_t c) {
    check_oneargs(r, c);
    const std::deque<data_pair>& current=data[c];
    auto cIt=find_matching_row(current.begin(), current.end(), data_pair(r, get_empty()));
    if (cIt!=current.end() && cIt->first==r) {
        return cIt->second;
    } else {
        return get_empty();
    }
}

/*** Output function ***/

template<typename T, class V>
Rcpp::RObject Csparse_output<T, V>::yield() {
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

template<typename T, class V>
matrix_type Csparse_output<T, V>::get_matrix_type() const {
    return SPARSE;
}

}

#endif
