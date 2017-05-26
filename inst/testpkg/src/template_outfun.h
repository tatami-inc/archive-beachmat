#ifndef TEMPLATE_TESTFUN_H
#define TEMPLATE_TESTFUN_H

/* This function tests the output fill_row/fill_col methods; that they
 * call the fill_row/fill_col methods of the derived classes (along with the
 * correct arguments to the overloaded virtual methods). It also checks that
 * the get_row/get_col methods of the derived classes are working properly, 
 * by extracting rows/columns and and re-filling the output matrix.
 */

template <class T, class M, class O>  // M, O are automatically deduced.
Rcpp::RObject pump_out(M ptr, O optr, const Rcpp::IntegerVector& mode, const Rcpp::LogicalVector& refill) {
    if (mode.size()!=1) { 
        throw std::runtime_error("'mode' should be an integer scalar"); 
    }
    if (refill.size()!=1) {
        throw std::runtime_error("'refill' should be a logical scalar");
    }
    const int Mode=mode[0];
    const size_t& nrows=ptr->get_nrow();
    const size_t& ncols=ptr->get_ncol();
    const bool redo=refill[0];

    if (Mode==1) { 
        // By column.
        T target(nrows);
        for (int c=0; c<ncols; ++c) {
            ptr->get_col(c, target.begin());
            optr->fill_col(c, target.begin());
            if (redo) {
                std::fill(target.begin(), target.end(), 0); // Wiping, to test that target is actually re-filled properly.
                optr->get_col(c, target.begin());
                std::reverse(target.begin(), target.end()); // Reversing the order, to keep it interesting.
                optr->fill_col(c, target.begin());
            }
        }
    } else if (Mode==2) { 
        // By row.
        T target(ncols);
        for (int r=0; r<nrows; ++r) {
            ptr->get_row(r, target.begin());
            optr->fill_row(r, target.begin());                
            if (redo) {
                std::fill(target.begin(), target.end(), 0);  // Wiping.
                optr->get_row(r, target.begin());
                std::reverse(target.begin(), target.end()); // Reversing.
                optr->fill_row(r, target.begin());
            }
        }
    } else if (Mode==3) {
        // By cell.
        for (int c=0; c<ncols; ++c){ 
            for (int r=0; r<nrows; ++r) {
                optr->fill(r, c, ptr->get(r, c));
                if (redo) {
                    optr->fill(r, c, optr->get(r, c));
                }
            }
        }
    } else { 
        throw std::runtime_error("'mode' should be in [1,3]"); 
    }

    return optr->yield();
}

/* This function tests the fill_row/fill_col methods, and that they properly
 * call the fill_row/fill_col methods of the derived classes with slices.
 */

template <class T, class M, class O>  
Rcpp::RObject pump_out_slice (M ptr, O optr, const Rcpp::IntegerVector& mode, 
        const Rcpp::IntegerVector& rows, const Rcpp::IntegerVector& cols,  const Rcpp::LogicalVector& refill) {

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

    if (refill.size()!=1) {
        throw std::runtime_error("'refill' should be a logical scalar");
    }
    const bool redo=refill[0];

    if (Mode==1) { 
        // By column.
        T target(nrows);
        for (int c=0; c<ncols; ++c) {
            ptr->get_col(c+cstart, target.begin(), rstart, rend);
            optr->fill_col(c+cstart, target.begin(), rstart, rend);
            if (redo) {
                std::fill(target.begin(), target.end(), 0); // Wiping, to test that target is actually re-filled properly.
                optr->get_col(c+cstart, target.begin(), rstart, rend);
                std::reverse(target.begin(), target.end()); // Reversing the order, to keep it interesting.
                optr->fill_col(c+cstart, target.begin(), rstart, rend);
            }
        }
    } else if (Mode==2) { 
        // By row.
        T target(ncols);
        for (int r=0; r<nrows; ++r) {
            ptr->get_row(r+rstart, target.begin(), cstart, cend);
            optr->fill_row(r+rstart, target.begin(), cstart, cend);
            if (redo) {
                std::fill(target.begin(), target.end(), 0); // Wiping.
                optr->get_row(r+rstart, target.begin(), cstart, cend);
                std::reverse(target.begin(), target.end()); // Reversing.
                optr->fill_row(r+rstart, target.begin(), cstart, cend);
            }
        }
    } else { 
        throw std::runtime_error("'mode' should be in [1,2]"); 
    }

    return optr->yield();
}

/* This function tests the edge cases and error triggers. */

template <class T, class M>  
void output_edge (M ptr, const Rcpp::IntegerVector& mode) {
    if (mode.size()!=1) { 
        throw std::runtime_error("'mode' should be an integer scalar"); 
    }
    const int Mode=mode[0];

    T stuff;
    if (Mode==0) {
        ptr->get_row(0, stuff.begin(), 0, 0); // Should not break.
        ptr->get_col(0, stuff.begin(), 0, 0); 
        ptr->fill_row(0, stuff.begin(), 0, 0); 
        ptr->fill_col(0, stuff.begin(), 0, 0); 
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
    } else if (Mode==4) {
        ptr->fill_row(-1, stuff.begin()); // break!
    } else if (Mode==-4) {
        ptr->fill_col(-1, stuff.begin()); // break!
    } else if (Mode==5) {
        ptr->fill_row(0, stuff.begin(), 1, 0); // break!
    } else if (Mode==-5) {
        ptr->fill_col(0, stuff.begin(), 1, 0); // break!
    } else if (Mode==6) {
        ptr->fill_row(0, stuff.begin(), 0, -1); // break!
    } else if (Mode==-6) {
        ptr->fill_col(0, stuff.begin(), 0, -1); // break!
    }
   
    return;
}


#endif
