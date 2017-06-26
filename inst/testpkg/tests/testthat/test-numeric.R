# This tests the ability of the API to properly access numeric matrices of different types.
# library(testthat); source("test-numeric.R")

#######################################################

# Testing simple matrices:

set.seed(12345)
sFUN <- function(nr=15, nc=10) {
    matrix(rnorm(nr*nc), nr, nc)
}

test_that("Simple numeric matrix input is okay", {
    beachtest:::check_numeric_mat(sFUN)
    beachtest:::check_numeric_mat(sFUN, nr=5, nc=30)
    beachtest:::check_numeric_mat(sFUN, nr=30, nc=5)

    beachtest:::check_numeric_slice(sFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Testing const and non-zero options.   
    beachtest:::check_numeric_const_mat(sFUN)
    beachtest:::check_numeric_const_slice(sFUN, by.row=list(1:5, 6:8))
    
    beachtest:::check_numeric_nonzero_mat(sFUN)
    beachtest:::check_numeric_nonzero_slice(sFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    beachtest:::check_type(sFUN, expected="double")
})

# Testing dense matrices:

set.seed(13579)
library(Matrix)
dFUN <- function(nr=15, nc=10) {
    Matrix(sFUN(nr, nc), sparse=FALSE, doDiag=FALSE)
}

test_that("Dense numeric matrix input is okay", {
    expect_s4_class(dFUN(), "dgeMatrix")

    beachtest:::check_numeric_mat(dFUN)
    beachtest:::check_numeric_mat(dFUN, nr=5, nc=30)
    beachtest:::check_numeric_mat(dFUN, nr=30, nc=5)

    beachtest:::check_numeric_slice(dFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Testing const and non-zero options.   
    beachtest:::check_numeric_const_mat(dFUN)
    beachtest:::check_numeric_const_slice(dFUN, by.row=list(1:5, 6:8))
    
    beachtest:::check_numeric_nonzero_mat(dFUN)
    beachtest:::check_numeric_nonzero_slice(dFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8)) 

    beachtest:::check_type(dFUN, expected="double")
})

# Testing sparse matrices (dgCMatrix):

set.seed(23456)
csFUN <- function(nr=15, nc=10, d=0.1) {
    rsparsematrix(nrow=nr, ncol=nc, density=d)
}

test_that("Sparse numeric matrix input is okay", {
    expect_s4_class(csFUN(), "dgCMatrix")
    
    beachtest:::check_numeric_mat(csFUN)
    beachtest:::check_numeric_mat(csFUN, nr=5, nc=30)
    beachtest:::check_numeric_mat(csFUN, nr=30, nc=5)
    
    beachtest:::check_numeric_mat(csFUN, d=0.2)
    beachtest:::check_numeric_mat(csFUN, nr=5, nc=30, d=0.2)
    beachtest:::check_numeric_mat(csFUN, nr=30, nc=5, d=0.2)
    
    beachtest:::check_numeric_slice(csFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Testing const and non-zero options.   
    beachtest:::check_numeric_const_mat(csFUN)
    beachtest:::check_numeric_const_slice(csFUN, by.row=list(1:5, 6:8))
    
    beachtest:::check_numeric_nonzero_mat(csFUN)
    beachtest:::check_numeric_nonzero_slice(csFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
   
    beachtest:::check_type(csFUN, expected="double")
})

# Testing dense symmetric matrices (dspMatrix):

set.seed(45678)
spFUN <- function(nr=10, mode="U") {
    pack(forceSymmetric(sFUN(nr, nr), uplo=mode))
}

test_that("Symmetric numeric matrix input is okay", {
    expect_s4_class(spFUN(), "dspMatrix")

    beachtest:::check_numeric_mat(spFUN)
    beachtest:::check_numeric_mat(spFUN, nr=5)
    beachtest:::check_numeric_mat(spFUN, nr=30)
    
    beachtest:::check_numeric_mat(spFUN, mode="L")
    beachtest:::check_numeric_mat(spFUN, nr=5, mode="L")
    beachtest:::check_numeric_mat(spFUN, nr=30, mode="L")
    
    beachtest:::check_numeric_slice(spFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
    beachtest:::check_numeric_slice(spFUN, mode="L", by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
    
    # Testing const and non-zero options.   
    beachtest:::check_numeric_const_mat(spFUN)
    beachtest:::check_numeric_const_mat(spFUN, mode="L")
    beachtest:::check_numeric_const_slice(spFUN, by.row=list(1:5, 6:8))
    beachtest:::check_numeric_const_slice(spFUN, mode="L", by.row=list(1:5, 6:8))
    
    beachtest:::check_numeric_nonzero_mat(spFUN)
    beachtest:::check_numeric_nonzero_mat(spFUN, mode="L")
    beachtest:::check_numeric_nonzero_slice(spFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
    beachtest:::check_numeric_nonzero_slice(spFUN, mode="L", by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
 
    beachtest:::check_type(spFUN, expected="double")
    beachtest:::check_type(spFUN, mode="L", expected="double")
})

# Testing Rle matrices:

set.seed(23456)
library(DelayedArray)
rFUN <- function(nr=15, nc=10, density=0.2, chunk.ncols=NULL) {
    x <- as.matrix(csFUN(nr, nc, density))
    rle <- Rle(x)
    if (!is.null(chunk.ncols)) {
        chunksize <- chunk.ncols*nrow(x)
    } else {
        chunksize <- NULL
    }
    RleArray(rle, dim(x), chunksize=chunksize)
}

test_that("RLE numeric matrix input is okay", {
    beachtest:::check_numeric_mat(rFUN)
    beachtest:::check_numeric_mat(rFUN, nr=5, nc=30)
    beachtest:::check_numeric_mat(rFUN, nr=30, nc=5)

    beachtest:::check_numeric_mat(rFUN, density=0.1)
    beachtest:::check_numeric_mat(rFUN, nr=5, nc=30, density=0.1)
    beachtest:::check_numeric_mat(rFUN, nr=30, nc=5, density=0.1)
    
    beachtest:::check_numeric_slice(rFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
    beachtest:::check_numeric_slice(rFUN, density=0.1, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    beachtest:::check_numeric_const_mat(rFUN)
    beachtest:::check_numeric_const_slice(rFUN, by.row=list(1:5, 6:8))
    
    beachtest:::check_numeric_nonzero_mat(rFUN)
    beachtest:::check_numeric_nonzero_slice(rFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Testing chunk settings.
    beachtest:::check_numeric_mat(rFUN, chunk.ncols=3)
    beachtest:::check_numeric_mat(rFUN, nr=5, nc=30, chunk.ncols=5)
    beachtest:::check_numeric_mat(rFUN, nr=30, nc=5, chunk.ncols=2)

    beachtest:::check_numeric_mat(rFUN, density=0.1, chunk.ncols=3)
    beachtest:::check_numeric_mat(rFUN, nr=5, nc=30, density=0.1, chunk.ncols=5)
    beachtest:::check_numeric_mat(rFUN, nr=30, nc=5, density=0.1, chunk.ncols=2)
    
    beachtest:::check_numeric_slice(rFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8), chunk.ncols=2)
    beachtest:::check_numeric_slice(rFUN, density=0.1, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8), chunk.ncols=2)
 
    beachtest:::check_numeric_const_mat(rFUN, chunk.ncols=2)
    beachtest:::check_numeric_const_slice(rFUN, chunk.ncols=2, by.row=list(1:5, 6:8))
    
    beachtest:::check_numeric_nonzero_mat(rFUN, chunk.ncols=2)
    beachtest:::check_numeric_nonzero_slice(rFUN, chunk.ncols=2, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
  
    # Checking type.
    beachtest:::check_type(rFUN, expected="double")
    beachtest:::check_type(rFUN, chunk.ncols=2, expected="double")
})

# Testing HDF5 matrices:

set.seed(34567)
library(HDF5Array)
hFUN <- function(nr=15, nc=10) {
    as(sFUN(nr, nc), "HDF5Array")
}

test_that("HDF5 numeric matrix input is okay", {
    expect_s4_class(hFUN(), "HDF5Matrix")

    beachtest:::check_numeric_mat(hFUN)
    beachtest:::check_numeric_mat(hFUN, nr=5, nc=30)
    beachtest:::check_numeric_mat(hFUN, nr=30, nc=5)
    
    beachtest:::check_numeric_slice(hFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Checking const and non-zero options.
    beachtest:::check_numeric_const_mat(hFUN)
    beachtest:::check_numeric_const_slice(hFUN, by.row=list(1:5, 6:8))
    
    beachtest:::check_numeric_nonzero_mat(hFUN)
    beachtest:::check_numeric_nonzero_slice(hFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    beachtest:::check_type(hFUN, expected="double")
})

# Testing delayed operations

sub_hFUN <- function() {
    A <- hFUN(15, 10)    
    A[1:10,]
}

add_hFUN <- function() {
    hFUN(15, 10) + 1
}

test_that("Delayed numeric matrix input is okay", {
    expect_s4_class(sub_hFUN(), "DelayedMatrix")
    beachtest:::check_numeric_mat(sub_hFUN) 
    beachtest:::check_type(sub_hFUN, expected="double")
    
    expect_s4_class(add_hFUN(), "DelayedMatrix")
    beachtest:::check_numeric_mat(add_hFUN) 
    beachtest:::check_type(add_hFUN, expected="double")
    
    expect_identical("logical", .Call(beachtest:::cxx_test_type_check, hFUN() > 0)) # Proper type check!
})

#######################################################

# Testing conversions.

test_that("Numeric matrix input conversions are okay", {
    beachtest:::check_numeric_conversion(sFUN)

    beachtest:::check_numeric_conversion(dFUN)

    beachtest:::check_numeric_conversion(csFUN)

    beachtest:::check_numeric_conversion(spFUN)

    beachtest:::check_numeric_conversion(hFUN)
})

# Testing error generation.

test_that("Numeric matrix input error generation is okay", {
    beachtest:::check_numeric_edge_errors(sFUN)

    beachtest:::check_numeric_edge_errors(dFUN)

    beachtest:::check_numeric_edge_errors(csFUN)

    beachtest:::check_numeric_edge_errors(spFUN)

    beachtest:::check_numeric_edge_errors(hFUN)
})

#######################################################

# Testing simple numeric output:

set.seed(12345)

test_that("Simple numeric matrix output is okay", {
    beachtest:::check_numeric_output_mat(sFUN, hdf5.out=FALSE)
    
    beachtest:::check_numeric_output_slice(sFUN, by.row=1:12, by.col=3:7, hdf5.out=FALSE)  
})

# Testing HDF5 sparse output:

test_that("Sparse numeric matrix output is okay", {
    beachtest:::check_sparse_numeric_output(csFUN)
    beachtest:::check_sparse_numeric_output(csFUN, d=0.2)
    beachtest:::check_sparse_numeric_output(csFUN, d=0.5)

    beachtest:::check_sparse_numeric_output_slice(csFUN, by.row=1:5, by.col=7:9)
    beachtest:::check_sparse_numeric_output_slice(csFUN, by.row=1, by.col=2:8, d=0.2)
    beachtest:::check_sparse_numeric_output_slice(csFUN, by.row=3:9, by.col=5, d=0.5)
})

# Testing HDF5 numeric output:

test_that("HDF5 numeric matrix output is okay", {
    beachtest:::check_numeric_output_mat(hFUN, hdf5.out=TRUE)
    
    beachtest:::check_numeric_output_slice(hFUN, by.row=1:2, by.col=2:10, hdf5.out=TRUE)

    beachtest:::check_numeric_order(hFUN)
})

# Testing conversions:

test_that("Numeric matrix output conversions are okay", {
    beachtest:::check_numeric_converted_output(sFUN, hdf5.out=FALSE)
    
    beachtest:::check_numeric_converted_output(hFUN, hdf5.out=TRUE)
})

# Testing for errors:

test_that("Numeric matrix output error generation is okay", {
    beachtest:::check_numeric_edge_output_errors(sFUN)
    
    beachtest:::check_numeric_edge_output_errors(hFUN)
})

#######################################################

