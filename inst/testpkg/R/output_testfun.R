# Creating functions to check each set of matrix inputs.

###############################

.check_output_mat <- function(FUN, ..., hdf5.out, cxxfun) { 
    for (i in 1:3) { 
        test.mat <- FUN(...)
        
        out <- .Call(cxxfun, test.mat, i)
        if (hdf5.out) { 
            testthat::expect_s4_class(out[[1]], "HDF5Matrix")
            testthat::expect_equal(out[[1]]@seed@first_val, as.vector(out[[1]][1,1]))
            testthat::expect_identical(dim(out[[1]]), dim(test.mat))
            out[[1]] <- as.matrix(out[[1]])
        }      
        ref <- as.matrix(test.mat)
        testthat::expect_identical(ref, out[[1]])

        # Checking the flipped matrix. Here, we extract values by row/column from the filled output matrix, flip
        # the values and refill the output matrix. This checks whether the getters of the output matrix work as expected.
        if (i==1L) {
            ref <- ref[nrow(ref):1,,drop=FALSE]
        } else if (i==2L) {
            ref <- ref[,ncol(ref):1,drop=FALSE]
        } else if (i==3L) {
            ref <- ref[nrow(ref):1,ncol(ref):1,drop=FALSE]
        }
        testthat::expect_identical(ref, out[[2]])
    }
    return(invisible(NULL))
}
   
check_integer_output_mat <- function(FUN, ..., hdf5.out) {
    .check_output_mat(FUN=FUN, ..., hdf5.out=hdf5.out, cxxfun=cxx_test_integer_output)
} 

check_numeric_output_mat <- function(FUN, ..., hdf5.out) {
    .check_output_mat(FUN=FUN, ..., hdf5.out=hdf5.out, cxxfun=cxx_test_numeric_output)
} 

check_logical_output_mat <- function(FUN, ..., hdf5.out) {
    .check_output_mat(FUN=FUN, ..., hdf5.out=hdf5.out, cxxfun=cxx_test_logical_output)
} 

check_character_output_mat <- function(FUN, ..., hdf5.out) {
    .check_output_mat(FUN=FUN, ..., hdf5.out=hdf5.out, cxxfun=cxx_test_character_output)
} 

###############################

.check_output_slice <- function(FUN, ..., by.row, by.col, class.out, cxxfun, fill) { 
    rx <- range(by.row)
    ry <- range(by.col)

    for (it in 1:2) {
        test.mat <- FUN(...)      
        out <- .Call(cxxfun, test.mat, it, rx, ry)
        if (!is.na(class.out)) {
            testthat::expect_s4_class(out[[1]], class.out) 
            out[[1]] <- as.matrix(out[[1]])
        }

        # Checking the standard fill.
        test <- out[[1]]
        ref <- as.matrix(test.mat)
        testthat::expect_identical(ref[by.row, by.col], test[by.row, by.col]) 
        test[by.row,by.col] <- fill
        testthat::expect_true(all(test==fill))

        # Checking the flipped fill.
        flipped <- out[[2]]
        dimnames(ref) <- NULL
        if (it==1L) {
            ref[,-by.col] <- fill
            testthat::expect_identical(ref[rev(by.row),], flipped[by.row,])
        } else {
            ref[-by.row,] <- fill
            testthat::expect_identical(ref[,rev(by.col)], flipped[,by.col])
        }
        flipped[by.row,by.col] <- fill
        testthat::expect_true(all(flipped==fill))
    }
    return(invisible(NULL))
}


check_integer_output_slice <- function(FUN, ..., by.row, by.col, hdf5.out) {
    .check_output_slice(FUN=FUN, ..., by.row=by.row, by.col=by.col, 
                        class.out=ifelse(hdf5.out, "HDF5Matrix", NA_character_),
                        cxxfun=cxx_test_integer_output_slice, fill=0L)
} 

check_numeric_output_slice <- function(FUN, ..., by.row, by.col, hdf5.out) {
    .check_output_slice(FUN=FUN, ..., by.row=by.row, by.col=by.col, 
                        class.out=ifelse(hdf5.out, "HDF5Matrix", NA_character_),
                        cxxfun=cxx_test_numeric_output_slice, fill=0)
} 

check_logical_output_slice <- function(FUN, ..., by.row, by.col, hdf5.out) {
    .check_output_slice(FUN=FUN, ..., by.row=by.row, by.col=by.col, 
                        class.out=ifelse(hdf5.out, "HDF5Matrix", NA_character_),
                        cxxfun=cxx_test_logical_output_slice, fill=FALSE)
} 

check_character_output_slice <- function(FUN, ..., by.row, by.col, hdf5.out) {
    .check_output_slice(FUN=FUN, ..., by.row=by.row, by.col=by.col, 
                        class.out=ifelse(hdf5.out, "HDF5Matrix", NA_character_),
                        cxxfun=cxx_test_character_output_slice, fill="")
} 

###############################

check_sparse_numeric_output <- function(FUN, ...) {
    for (mode in 1:3) {
        current <- FUN(...)
        if (mode==1L) {
            dim <- ncol(current)
        } else {
            dim <- nrow(current)
        }

        for (type in 1:2) {
            if (type==1L) {
                o <- seq_len(dim)
            } else {
                o <- sample(dim)
            }

            out <- .Call(cxx_test_sparse_numeric_output, current, mode, o-1L)
            testthat::expect_s4_class(out[[1]], "dgCMatrix")
            ref <- as.matrix(out[[1]])
            dimnames(ref) <- NULL

            if (mode==1L) { 
                testthat::expect_identical(out[[1]][,o], current)
                testthat::expect_identical(out[[2]][nrow(current):1,,drop=FALSE], ref)
            } else if (mode==2L) {
                testthat::expect_identical(out[[1]][o,], current)
                testthat::expect_identical(out[[2]][,ncol(current):1,drop=FALSE], ref)
            } else {
                testthat::expect_identical(out[[1]], current) # no reordering 
                testthat::expect_identical(out[[2]][nrow(current):1,ncol(current):1,drop=FALSE], ref)
                break
            }
        }
    }
    return(invisible(NULL))
}

check_sparse_numeric_output_slice <- function(FUN, ..., by.row, by.col) {
   .check_output_slice(FUN, ..., by.row=by.row, by.col=by.col, class.out="dgCMatrix",
                       cxxfun=cxx_test_sparse_numeric_output_slice, fill=0) 
}


###############################

.check_execution_order <- function(FUN, cxxfun, type) {
    # Checking that the output function in '.Call' does not overwrite the 
    # underlying HDF5 file and change the values of other HDF5Matrix objects. 
    mats <- list(FUN(), FUN(), FUN())
    ref.mats <- lapply(mats, as.matrix)
    new.mats <- lapply(mats, function(x) .Call(cxxfun, x, 1L)[[1]]) 

    for (i in seq_along(mats)) { 
        out <- new.mats[[i]]
        testthat::expect_s4_class(out, "HDF5Matrix")
        ref <- ref.mats[[i]]
        testthat::expect_identical(ref, as.matrix(out))
        original <- mats[[i]]
        testthat::expect_identical(ref, as.matrix(original))
        testthat::expect_true(original@seed@file!=out@seed@file)
    }

    # Checking that the old and realized files are in the log.
    testthat::expect_message(log <- HDF5Array::showHDF5DumpLog())
    for (mode in c(TRUE, FALSE)) {
        for (i in seq_along(mats)) {
            if (mode) {
                current <- new.mats[[i]]
            } else {
                current <- mats[[i]]
            }   

            j <- which(log$name==current@seed@name & log$file==current@seed@file)
            testthat::expect_true(length(j)==1L)
            testthat::expect_identical(type, log$type[j])
            testthat::expect_identical(sprintf("%ix%i", nrow(current), ncol(current)), log$dims[j])
        }
    }
    return(invisible(NULL))
}

check_integer_order <- function(FUN, cxxfun) {
    .check_execution_order(FUN, cxxfun=cxx_test_integer_output, type="integer")
}

check_numeric_order <- function(FUN, cxxfun) {
    .check_execution_order(FUN, cxxfun=cxx_test_numeric_output, type="double")
}

check_logical_order <- function(FUN, cxxfun) {
    .check_execution_order(FUN, cxxfun=cxx_test_logical_output, type="logical")
}

check_character_order <- function(FUN, cxxfun) {
    .check_execution_order(FUN, cxxfun=cxx_test_character_output, type="character")
}

###############################

.check_converted_output <- function(FUN, ..., hdf5.out, cxxfun, rfun) { 
    for (i in 1:3) { 
        test.mat <- FUN(...)
        
        out <- .Call(cxxfun, test.mat, i)
        if (hdf5.out) { 
            testthat::expect_s4_class(out[[1]], "HDF5Matrix")
            out[[1]] <- as.matrix(out[[1]])
        }
        ref <- as.matrix(test.mat)
        testthat::expect_identical(rfun(ref), out[[1]])
            
        # With flipping.
        if (i==1L) {
            ref <- ref[nrow(ref):1,,drop=FALSE]
        } else if (i==2L) {
            ref <- ref[,ncol(ref):1,drop=FALSE]
        } else if (i==3L) {
            ref <- ref[nrow(ref):1,ncol(ref):1,drop=FALSE]
        }
        testthat::expect_identical(rfun(ref), out[[2]])
    }
    return(invisible(NULL))
}

check_numeric_converted_output  <- function(FUN, ..., hdf5.out) {
    .check_converted_output(FUN=FUN, ..., cxxfun=cxx_test_numeric_to_integer_output, hdf5.out=hdf5.out, 
                            rfun=function(x) {
                                storage.mode(x) <- "integer" 
                                return(x)
                            })
}

check_integer_converted_output <- function(FUN, ..., hdf5.out) {
    .check_converted_output(FUN=FUN, ..., cxxfun=cxx_test_integer_to_numeric_output, hdf5.out=hdf5.out, 
                            rfun=function(x) { 
                                storage.mode(x) <- "double"
                                return(x)
                            })
}

check_logical_converted_output <- function(FUN, ..., hdf5.out) {
    .check_converted_output(FUN=FUN, ..., cxxfun=cxx_test_logical_to_numeric_output, hdf5.out=hdf5.out, 
                            rfun=function(x) { 
                                storage.mode(x) <- "double"
                                return(x)
                            })
    .check_converted_output(FUN=FUN, ..., cxxfun=cxx_test_logical_to_integer_output, hdf5.out=hdf5.out, 
                            rfun=function(x) { 
                                storage.mode(x) <- "integer"
                                return(x)
                            })
}


###############################

.check_edge_output_errors <- function(x, cxxfun) {
    testthat::expect_true(.Call(cxxfun, x, 0L))

    testthat::expect_error(.Call(cxxfun, x, 1L), "row index out of range")
    testthat::expect_error(.Call(cxxfun, x, -1L), "column index out of range")        
    testthat::expect_error(.Call(cxxfun, x, 2L), "column start index is greater than column end index")
    testthat::expect_error(.Call(cxxfun, x, -2L), "row start index is greater than row end index")
    testthat::expect_error(.Call(cxxfun, x, 3L), "column end index out of range")
    testthat::expect_error(.Call(cxxfun, x, -3L), "row end index out of range")
    
    testthat::expect_error(.Call(cxxfun, x, 4L), "row index out of range")
    testthat::expect_error(.Call(cxxfun, x, -4L), "column index out of range")        
    testthat::expect_error(.Call(cxxfun, x, 5L), "column start index is greater than column end index")
    testthat::expect_error(.Call(cxxfun, x, -5L), "row start index is greater than row end index")
    testthat::expect_error(.Call(cxxfun, x, 6L), "column end index out of range")
    testthat::expect_error(.Call(cxxfun, x, -6L), "row end index out of range")

    return(invisible(NULL))
}

check_integer_edge_output_errors <- function(FUN, ...) {
    .check_edge_output_errors(FUN(...), cxxfun=cxx_test_integer_edge_output)
}

check_logical_edge_output_errors <- function(FUN, ...) {
    .check_edge_output_errors(FUN(...), cxxfun=cxx_test_logical_edge_output)
}

check_numeric_edge_output_errors <- function(FUN, ...) {
    .check_edge_output_errors(FUN(...), cxxfun=cxx_test_numeric_edge_output)
}

check_character_edge_output_errors <- function(FUN, ...) {
    .check_edge_output_errors(FUN(...), cxxfun=cxx_test_character_edge_output)
}

