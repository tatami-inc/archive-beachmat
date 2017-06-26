# This tests the ability of the API to properly access logical matrices of different types.
# library(testthat); source("test-logical.R")

#######################################################

# Testing simple matrices:

set.seed(12345)
sFUN <- function(nr=15, nc=10) {
    matrix(rbinom(nr*nc, 1, 0.5)==0, nr, nc)
}

test_that("Simple logical matrix input is okay", {
    beachtest:::check_logical_mat(sFUN)
    beachtest:::check_logical_mat(sFUN, nr=5, nc=30)
    beachtest:::check_logical_mat(sFUN, nr=30, nc=5)
    
    beachtest:::check_logical_slice(sFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Testing const and non-zero options.   
    beachtest:::check_logical_const_mat(sFUN)
    beachtest:::check_logical_const_slice(sFUN, by.row=list(1:5, 6:8))
    
    beachtest:::check_logical_nonzero_mat(sFUN)
    beachtest:::check_logical_nonzero_slice(sFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    beachtest:::check_type(sFUN, expected="logical")
})

# Testing dense matrices:

library(Matrix)
set.seed(13579)
dFUN <- function(nr=15, nc=10) {
    Matrix(sFUN(nr, nc), sparse=FALSE, doDiag=FALSE)
}

test_that("Dense logical matrix input is okay", {
    expect_s4_class(dFUN(), "lgeMatrix")
    
    beachtest:::check_logical_mat(dFUN)
    beachtest:::check_logical_mat(dFUN, nr=5, nc=30)
    beachtest:::check_logical_mat(dFUN, nr=30, nc=5)
    
    beachtest:::check_logical_slice(dFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
      
    # Testing const and non-zero options.   
    beachtest:::check_logical_const_mat(dFUN)
    beachtest:::check_logical_const_slice(dFUN, by.row=list(1:5, 6:8))
    
    beachtest:::check_logical_nonzero_mat(dFUN)
    beachtest:::check_logical_nonzero_slice(dFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8)) 

    beachtest:::check_type(dFUN, expected="logical")
})

# Testing sparse matrices (lgCMatrix):

set.seed(23456)
csFUN <- function(nr=15, nc=10, d=0.1) {
    rsparsematrix(nrow=nr, ncol=nc, density=d)!=0
}

test_that("Sparse logical matrix input is okay", {
    expect_s4_class(csFUN(), "lgCMatrix")

    beachtest:::check_logical_mat(csFUN)
    beachtest:::check_logical_mat(csFUN, nr=5, nc=30)
    beachtest:::check_logical_mat(csFUN, nr=30, nc=5)
    
    beachtest:::check_logical_mat(csFUN, d=0.2)
    beachtest:::check_logical_mat(csFUN, nr=5, nc=30, d=0.2)
    beachtest:::check_logical_mat(csFUN, nr=30, nc=5, d=0.2)
    
    beachtest:::check_logical_slice(csFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Testing const and non-zero options.   
    beachtest:::check_logical_const_mat(csFUN)
    beachtest:::check_logical_const_slice(csFUN, by.row=list(1:5, 6:8))
    
    beachtest:::check_logical_nonzero_mat(csFUN)
    beachtest:::check_logical_nonzero_slice(csFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    beachtest:::check_type(csFUN, expected="logical")
})

# Testing dense symmetric matrices (lspMatrix):

set.seed(45678)
spFUN <- function(nr=10, mode="U") {
    pack(forceSymmetric(sFUN(nr, nr), uplo=mode))
}

test_that("Symmetric logical matrix input is okay", {
    expect_s4_class(spFUN(), "lspMatrix")
    
    beachtest:::check_logical_mat(spFUN)
    beachtest:::check_logical_mat(spFUN, nr=5)
    beachtest:::check_logical_mat(spFUN, nr=30)
    
    beachtest:::check_logical_mat(spFUN, mode="L")
    beachtest:::check_logical_mat(spFUN, nr=5, mode="L")
    beachtest:::check_logical_mat(spFUN, nr=30, mode="L")
    
    beachtest:::check_logical_slice(spFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
    beachtest:::check_logical_slice(spFUN, mode="L", by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Testing const and non-zero options.   
    beachtest:::check_logical_const_mat(spFUN)
    beachtest:::check_logical_const_mat(spFUN, mode="L")
    beachtest:::check_logical_const_slice(spFUN, by.row=list(1:5, 6:8))
    beachtest:::check_logical_const_slice(spFUN, mode="L", by.row=list(1:5, 6:8))
    
    beachtest:::check_logical_nonzero_mat(spFUN)
    beachtest:::check_logical_nonzero_mat(spFUN, mode="L")
    beachtest:::check_logical_nonzero_slice(spFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
    beachtest:::check_logical_nonzero_slice(spFUN, mode="L", by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    beachtest:::check_type(spFUN, expected="logical")
    beachtest:::check_type(spFUN, mode="L", expected="logical")
})

# Testing RLE matrices:

set.seed(23456)
library(DelayedArray)
rFUN <- function(nr=15, nc=10, chunk.ncols=NULL) {
    x <- sFUN(nr, nc)
    rle <- Rle(x)
    if (!is.null(chunk.ncols)) {
        chunksize <- chunk.ncols*nrow(x)
    } else {
        chunksize <- NULL
    }
    RleArray(rle, dim(x), chunksize=chunksize)
}

test_that("RLE logical matrix input is okay", {
    beachtest:::check_logical_mat(rFUN)
    beachtest:::check_logical_mat(rFUN, nr=5, nc=30)
    beachtest:::check_logical_mat(rFUN, nr=30, nc=5)
    
    beachtest:::check_logical_slice(rFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    beachtest:::check_logical_const_mat(rFUN)
    beachtest:::check_logical_const_slice(rFUN, by.row=list(1:5, 6:8))
    
    beachtest:::check_logical_nonzero_mat(rFUN)
    beachtest:::check_logical_nonzero_slice(rFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Testing chunks.
    beachtest:::check_logical_mat(rFUN, chunk.ncol=3)
    beachtest:::check_logical_mat(rFUN, nr=5, nc=30, chunk.ncol=5)
    beachtest:::check_logical_mat(rFUN, nr=30, nc=5, chunk.ncol=2)
    
    beachtest:::check_logical_slice(rFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8), chunk.ncol=2)

    beachtest:::check_logical_const_mat(rFUN, chunk.ncols=2)
    beachtest:::check_logical_const_slice(rFUN, chunk.ncols=2, by.row=list(1:5, 6:8))
    
    beachtest:::check_logical_nonzero_mat(rFUN, chunk.ncols=2)
    beachtest:::check_logical_nonzero_slice(rFUN, chunk.ncols=2, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Checking type.
    beachtest:::check_type(rFUN, expected="logical")
    beachtest:::check_type(rFUN, chunk.ncol=2, expected="logical")
})

# Testing HDF5 matrices:

set.seed(34567)
library(HDF5Array)
hFUN <- function(nr=15, nc=10) {
    as(sFUN(nr, nc), "HDF5Array")
}

test_that("HDF5 logical matrix input is okay", {
    expect_s4_class(hFUN(), "HDF5Matrix")
    
    beachtest:::check_logical_mat(hFUN)
    beachtest:::check_logical_mat(hFUN, nr=5, nc=30)
    beachtest:::check_logical_mat(hFUN, nr=30, nc=5)
    
    beachtest:::check_logical_slice(hFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

   # Checking const and non-zero options.
    beachtest:::check_logical_const_mat(hFUN)
    beachtest:::check_logical_const_slice(hFUN, by.row=list(1:5, 6:8))
    
    beachtest:::check_logical_nonzero_mat(hFUN)
    beachtest:::check_logical_nonzero_slice(hFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    beachtest:::check_type(hFUN, expected="logical")
})

# Testing delayed operations

sub_hFUN <- function() {
    A <- hFUN(15, 10)    
    A[1:10,]
}

alt_hFUN <- function() {
    !hFUN(15, 10)
}

test_that("Delayed logical matrix input is okay", {
    expect_s4_class(sub_hFUN(), "DelayedMatrix")
    beachtest:::check_logical_mat(sub_hFUN) 
    beachtest:::check_type(sub_hFUN, expected="logical")
    
    expect_s4_class(alt_hFUN(), "DelayedMatrix")
    beachtest:::check_logical_mat(alt_hFUN) 
    beachtest:::check_type(alt_hFUN, expected="logical")
    
    expect_identical("integer", .Call(beachtest:::cxx_test_type_check, hFUN()+1L)) # Proper type check!
})

#######################################################

# Testing conversions.

test_that("Logical matrix input conversions are okay", {
    beachtest:::check_logical_conversion(sFUN)

    beachtest:::check_logical_conversion(dFUN)

    beachtest:::check_logical_conversion(csFUN)

    beachtest:::check_logical_conversion(spFUN)

    beachtest:::check_logical_conversion(hFUN)
})

# Testing error generation.

test_that("Logical matrix input error generation is okay", {
    beachtest:::check_logical_edge_errors(sFUN)

    beachtest:::check_logical_edge_errors(dFUN)

    beachtest:::check_logical_edge_errors(csFUN)

    beachtest:::check_logical_edge_errors(spFUN)

    beachtest:::check_logical_edge_errors(hFUN)
})

#######################################################

# Testing simple logical output:

set.seed(12345)

test_that("Simple logical matrix output is okay", {
    beachtest:::check_logical_output_mat(sFUN, hdf5.out=FALSE)

    beachtest:::check_logical_output_slice(sFUN, by.row=10:13, by.col=2:5, hdf5.out=FALSE)
})

# Testing HDF5 logical output:

test_that("HDF5 logical matrix output is okay", {
    beachtest:::check_logical_output_mat(hFUN, hdf5.out=TRUE)

    beachtest:::check_logical_output_slice(hFUN, by.row=12:15, by.col=1:5, hdf5.out=TRUE)

    beachtest:::check_logical_order(hFUN)
})

# Testing conversions:

test_that("Logical matrix output conversions are okay", {
    beachtest:::check_logical_converted_output(sFUN, hdf5.out=FALSE)

    beachtest:::check_logical_converted_output(hFUN, hdf5.out=TRUE)
})

# Testing for errors:

test_that("Logical matrix output error generation is okay", {
    beachtest:::check_logical_edge_output_errors(sFUN)

    beachtest:::check_logical_edge_output_errors(hFUN)
})

#######################################################

