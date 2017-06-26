#ifndef BEACHTEST_TEMPLATE_INFUN_H
#define BEACHTEST_TEMPLATE_INFUN_H

/* This function tests the get_row/get_col methods, and that they properly
 * call the get_row/get_col methods of the derived classes (along with the
 * correct arguments to the overloaded virtual methods). 
 */

template <class T, class O, class M>  // M is automatically deduced.
O fill_up (M ptr, const Rcpp::IntegerVector& mode) {
    if (mode.size()!=1) { 
        throw std::runtime_error("'mode' should be an integer scalar"); 
    }
    const int Mode=mode[0];
    const size_t& nrows=ptr->get_nrow();
    const size_t& ncols=ptr->get_ncol();
    O output(nrows, ncols);

    if (Mode==1) { 
        // By column.
        T target(nrows);
        for (int c=0; c<ncols; ++c) {
            ptr->get_col(c, target.begin());
            for (int r=0; r<nrows; ++r) {
                output[c * nrows + r]=target[r];
            }
        }
    } else if (Mode==2) { 
        // By row.
        T target(ncols);
        for (int r=0; r<nrows; ++r) {
            ptr->get_row(r, target.begin());
            for (int c=0; c<ncols; ++c) {
                output[c * nrows + r]=target[c];
            }
        }
    } else if (Mode==3) {
        // By cell.
        for (int c=0; c<ncols; ++c){ 
            for (int r=0; r<nrows; ++r) {
                output[c * nrows + r]=ptr->get(r, c);
            }
        }
    } else { 
        throw std::runtime_error("'mode' should be in [1,3]"); 
    }

    return output;
}

/* This function tests the get_row/get_col methods, and that they properly
 * call the get_row/get_col methods of the derived classes with slices.
 */

template <class T, class O, class M>  
O fill_up_slice (M ptr, const Rcpp::IntegerVector& mode, const Rcpp::IntegerVector& rows, const Rcpp::IntegerVector& cols) {

    if (mode.size()!=1) { 
        throw std::runtime_error("'mode' should be an integer scalar"); 
    }
    const int Mode=mode[0];

    if (rows.size()!=2) { 
        throw std::runtime_error("'rows' should be an integer vector of length 2"); 
    }
    const int rstart=rows[0]-1, rend=rows[1];
    const int nrows=rend-rstart;    

    if (cols.size()!=2) { 
        throw std::runtime_error("'cols' should be an integer vector of length 2"); 
    }
    const int cstart=cols[0]-1, cend=cols[1];
    const int ncols=cend-cstart;    

    O output(nrows, ncols);
    if (Mode==1) { 
        // By column.
        T target(nrows);
        for (int c=0; c<ncols; ++c) {
            ptr->get_col(c+cstart, target.begin(), rstart, rend);
            for (int r=0; r<nrows; ++r) {
                output[c * nrows + r]=target[r];
            }
        }
    } else if (Mode==2) { 
        // By row.
        T target(ncols);
        for (int r=0; r<nrows; ++r) {
            ptr->get_row(r+rstart, target.begin(), cstart, cend);
            for (int c=0; c<ncols; ++c) {
                output[c * nrows + r]=target[c];
            }
        }
    } else { 
        throw std::runtime_error("'mode' should be in [1,2]"); 
    }

    return output;
}

/* This function tests the get_const_col methods, with or
 * without the use of slices.
 */

template <class T, class O, class M>  
O fill_up_const (M ptr) {
    const size_t& nrows=ptr->get_nrow();
    const size_t& ncols=ptr->get_ncol();
    O output(nrows, ncols);

    T target(nrows);
    for (int c=0; c<ncols; ++c) {
        auto it=ptr->get_const_col(c, target.begin());
        for (int r=0; r<nrows; ++r, ++it) {
            output[c * nrows + r]=*it;
        }
    }
    return output;
}

template <class T, class O, class M>  
O fill_up_const_slice (M ptr, const Rcpp::IntegerVector& rows) {
    if (rows.size()!=2) { 
        throw std::runtime_error("'rows' should be an integer vector of length 2"); 
    }
    const int rstart=rows[0]-1, rend=rows[1];
    const int nrows=rend-rstart;    

    const size_t& ncols=ptr->get_ncol();
    O output(nrows, ncols);

    T target(nrows);
    for (int c=0; c<ncols; ++c) {
        auto it=ptr->get_const_col(c, target.begin(), rstart, rend);
        for (int r=0; r<nrows; ++r, ++it) {
            output[c * nrows + r]=*it;
        }
    }
    return output;
}

/* This tests the behaviour of the non-zero filling-up without slices. */

template <class T, class O, class M>  
O fill_up_nonzero (M ptr, const Rcpp::IntegerVector& mode) {
    if (mode.size()!=1) { 
        throw std::runtime_error("'mode' should be an integer scalar"); 
    }
    const int Mode=mode[0];
    const size_t& nrows=ptr->get_nrow();
    const size_t& ncols=ptr->get_ncol();
    O output(nrows, ncols);

    if (Mode==1) { 
        // By column.
        Rcpp::IntegerVector indices(nrows);
        T target(nrows);
        for (int c=0; c<ncols; ++c) {
            auto iIt=indices.begin(); 
            auto tIt=target.begin();
            size_t num=ptr->get_nonzero_col(c, iIt, tIt);
            for (size_t x=0; x<num; ++x, ++iIt, ++tIt) {
                output[c * nrows + *iIt]=*tIt;
            }
        }
    } else if (Mode==2) { 
        // By row.
        Rcpp::IntegerVector indices(ncols);
        T target(ncols);
        for (int r=0; r<nrows; ++r) {
            auto iIt=indices.begin(); 
            auto tIt=target.begin();
            size_t num=ptr->get_nonzero_row(r, iIt, tIt);
            for (size_t x=0; x<num; ++x, ++iIt, ++tIt) {
                output[nrows * (*iIt) + r]=*tIt;
            }
        }
    } 
    return output;
}

/* This tests the behaviour of the non-zero filling-up with slices. */

template <class T, class O, class M>  
O fill_up_nonzero_slice (M ptr, const Rcpp::IntegerVector& mode, const Rcpp::IntegerVector& rows, const Rcpp::IntegerVector& cols) {
    if (mode.size()!=1) { 
        throw std::runtime_error("'mode' should be an integer scalar"); 
    }
    const int Mode=mode[0];

    if (rows.size()!=2) { 
        throw std::runtime_error("'rows' should be an integer vector of length 2"); 
    }
    const int rstart=rows[0]-1, rend=rows[1];
    const int nrows=rend-rstart;    

    if (cols.size()!=2) { 
        throw std::runtime_error("'cols' should be an integer vector of length 2"); 
    }
    const int cstart=cols[0]-1, cend=cols[1];
    const int ncols=cend-cstart;    

    O output(nrows, ncols);
    if (Mode==1) { 
        // By column.
        Rcpp::IntegerVector indices(nrows);
        T target(nrows);
        for (int c=0; c<ncols; ++c) {
            auto iIt=indices.begin(); 
            auto tIt=target.begin();
            size_t num=ptr->get_nonzero_col(c+cstart, iIt, tIt, rstart, rend);
            for (size_t x=0; x<num; ++x, ++iIt, ++tIt) {
                output[c * nrows + (*iIt - rstart)]=*tIt;
            }
        }
    } else if (Mode==2) { 
        // By row.
        Rcpp::IntegerVector indices(ncols);
        T target(ncols);
        for (int r=0; r<nrows; ++r) {
            auto iIt=indices.begin(); 
            auto tIt=target.begin();
            size_t num=ptr->get_nonzero_row(r+rstart, iIt, tIt, cstart, cend);
            for (size_t x=0; x<num; ++x, ++iIt, ++tIt) {
                output[nrows * (*iIt - cstart) + r]=*tIt;
            }
        }
    } else { 
        throw std::runtime_error("'mode' should be in [1,2]"); 
    }
    return output;
}

/* This function tests the edge cases and error triggers. */

template <class T, class M>  
void input_edge (M ptr, const Rcpp::IntegerVector& mode) {
    if (mode.size()!=1) { 
        throw std::runtime_error("'mode' should be an integer scalar"); 
    }
    const int Mode=mode[0];

    T stuff;
    if (Mode==0) {
        ptr->get_row(0, stuff.begin(), 0, 0); // Should not break.
        ptr->get_col(0, stuff.begin(), 0, 0); 
    } else if (Mode==1) {
        ptr->get_row(-1, stuff.begin()); // break!
    } else if (Mode==-1) {
        ptr->get_col(-1, stuff.begin()); // break!
    } else if (Mode==2) {
        ptr->get_row(0, stuff.begin(), 1, 0); // break!
    } else if (Mode==-2) {
        ptr->get_col(0, stuff.begin(), 1, 0); // break!
    } else if (Mode==3) {
        ptr->get_row(0, stuff.begin(), 0, -1); // break!
    } else if (Mode==-3) {
        ptr->get_col(0, stuff.begin(), 0, -1); // break!
    }
   
    return;
}

#endif
