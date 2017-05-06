#ifndef BEACHTEST_TEMPLATE_INFUN_H
#define BEACHTEST_TEMPLATE_INFUN_H

/* This function tests the get_row/get_col methods, and that they properly
 * call the get_row/get_col methods of the derived classes (along with the
 * correct arguments to the overloaded virtual methods). 
 */

template <class T, class O, class M>  // M is automatically deduced.
O fill_up (M& ptr, const Rcpp::IntegerVector& mode) {
    if (mode.size()!=1) { 
        throw std::runtime_error("'mode' should be an integer scalar"); 
    }
    const int Mode=mode[0];
    const int& nrows=ptr->get_nrow();
    const int& ncols=ptr->get_ncol();
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
O fill_up_slice (M& ptr, const Rcpp::IntegerVector& mode, const Rcpp::IntegerVector& rows, const Rcpp::IntegerVector& cols) {

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

#endif