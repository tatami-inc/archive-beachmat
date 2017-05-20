# Creating functions to check each set of matrix inputs.

.check_output_mat <- function(FUN, ..., hdf5.out, cxxfun) { 
    for (flip in c(TRUE, FALSE)) { 
        # When flip=TRUE, we fill the output matrix based on the input matrix,
        # extract values by row/column from the filled output matrix, flip
        # the values and refill the output matrix. This checks whether the
        # getters of the output matrix work as expected.

        for (i in 1:3) { 
            test.mat <- FUN(...)

            out <- .Call(cxxfun, test.mat, i, flip)
            if (hdf5.out) { 
                testthat::expect_s4_class(out, "HDF5Matrix")
                out <- as.matrix(out)
            }
            
            ref <- as.matrix(test.mat)
            if (flip) {
                if (i==1L) {
                    ref <- ref[nrow(ref):1,,drop=FALSE]
                } else if (i==2L) {
                    ref <- ref[,ncol(ref):1,drop=FALSE]
                }
            }

            testthat::expect_identical(ref, out)
        }
    }
    return(invisible(NULL))
}
   
.check_output_slice <- function(FUN, ..., by.row, by.col, hdf5.out, cxxfun, fill) { 
    rx <- range(by.row)
    ry <- range(by.col)

    for (flip in c(TRUE, FALSE)) { # Again, flipping to check getters in *_output.
        for (it in 1:2) {
            test.mat <- FUN(...)

            out <- .Call(cxxfun, test.mat, it, rx, ry, flip)
            if (hdf5.out) { 
                testthat::expect_s4_class(out, "HDF5Matrix")
                out <- as.matrix(out)
            }
            
            ref <- as.matrix(test.mat)
            if (!flip) { 
                testthat::expect_identical(ref[by.row, by.col], out[by.row, by.col]) 
            } else {
                if (it==1L) {
                    ref[,-by.col] <- fill
                    testthat::expect_identical(ref[rev(by.row),], out[by.row,])
                } else {
                    ref[-by.row,] <- fill
                    testthat::expect_identical(ref[,rev(by.col)], out[,by.col])
                }
            }

            # Checking that everywhere else is blank. 
            out[by.row,by.col] <- fill
            testthat::expect_true(all(out==fill))
        }
    }
    return(invisible(NULL))
}

check_integer_output_mat <- function(FUN, ..., hdf5.out) {
    .check_output_mat(FUN=FUN, ..., hdf5.out=hdf5.out, cxxfun=cxx_test_integer_output)
} 

check_integer_output_slice <- function(FUN, ..., by.row, by.col, hdf5.out) {
    .check_output_slice(FUN=FUN, ..., by.row=by.row, by.col=by.col, hdf5.out=hdf5.out, 
                        cxxfun=cxx_test_integer_output_slice, fill=0L)
} 

check_numeric_output_mat <- function(FUN, ..., hdf5.out) {
    .check_output_mat(FUN=FUN, ..., hdf5.out=hdf5.out, cxxfun=cxx_test_numeric_output)
} 

check_numeric_output_slice <- function(FUN, ..., by.row, by.col, hdf5.out) {
    .check_output_slice(FUN=FUN, ..., by.row=by.row, by.col=by.col, hdf5.out=hdf5.out, 
                        cxxfun=cxx_test_numeric_output_slice, fill=0)
} 

check_logical_output_mat <- function(FUN, ..., hdf5.out) {
    .check_output_mat(FUN=FUN, ..., hdf5.out=hdf5.out, cxxfun=cxx_test_logical_output)
} 

check_logical_output_slice <- function(FUN, ..., by.row, by.col, hdf5.out) {
    .check_output_slice(FUN=FUN, ..., by.row=by.row, by.col=by.col, hdf5.out=hdf5.out, 
                        cxxfun=cxx_test_logical_output_slice, fill=FALSE)
} 

.check_execution_order <- function(FUN, cxxfun, type) {
    # Checking that the output function in '.Call' does not overwrite the 
    # underlying HDF5 file and change the values of other HDF5Matrix objects. 
    mats <- list(FUN(), FUN(), FUN())
    ref.mats <- lapply(mats, as.matrix)
    new.mats <- lapply(mats, function(x) .Call(cxxfun, x, 1L, FALSE)) 

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

