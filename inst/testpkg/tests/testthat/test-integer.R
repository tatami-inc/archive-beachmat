# This tests the ability of the API to properly access integer matrices of different types.
# library(testthat); source("test-integer.R")

#######################################################

# Testing simple matrices:

set.seed(12345)
sFUN <- function(nr=15, nc=10, lambda=5) {
    matrix(rpois(nr*nc, lambda=lambda), nr, nc)
}

test_that("Simple integer matrix input is okay", {
    beachtest:::check_integer_mat(sFUN)
    beachtest:::check_integer_mat(sFUN, nr=5, nc=30)
    beachtest:::check_integer_mat(sFUN, nr=30, nc=5)
    
    beachtest:::check_integer_slice(sFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Testing const and non-zero options.
    beachtest:::check_integer_const_mat(sFUN)
    beachtest:::check_integer_const_slice(sFUN, by.row=list(1:5, 6:8))

    beachtest:::check_integer_nonzero_mat(sFUN)
    beachtest:::check_integer_nonzero_slice(sFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    beachtest:::check_type(sFUN, expected="integer")
})

# Testing RLE matrices:

set.seed(23456)
library(DelayedArray)
rFUN <- function(nr=15, nc=10, lambda=1, chunk.ncols=NULL) {
    x <- sFUN(nr, nc, lambda=lambda)
    rle <- Rle(x)
    if (!is.null(chunk.ncols)) {
        chunksize <- chunk.ncols*nrow(x)
    } else {
        chunksize <- NULL
    }
    RleArray(rle, dim(x), chunksize=chunksize)
}

test_that("RLE integer matrix input is okay", {
    beachtest:::check_integer_mat(rFUN)
    beachtest:::check_integer_mat(rFUN, nr=5, nc=30)
    beachtest:::check_integer_mat(rFUN, nr=30, nc=5)

    beachtest:::check_integer_mat(rFUN, lambda=0.1)
    beachtest:::check_integer_mat(rFUN, nr=5, nc=30, lambda=0.1)
    beachtest:::check_integer_mat(rFUN, nr=30, nc=5, lambda=0.1)

    beachtest:::check_integer_slice(rFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
    beachtest:::check_integer_slice(rFUN, lambda=0.1, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
 
    beachtest:::check_integer_const_mat(rFUN)
    beachtest:::check_integer_const_slice(rFUN, by.row=list(1:5, 6:8))

    beachtest:::check_integer_nonzero_mat(rFUN)
    beachtest:::check_integer_nonzero_slice(rFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Repeating the test with chunks.
    beachtest:::check_integer_mat(rFUN, chunk.ncols=3)
    beachtest:::check_integer_mat(rFUN, nr=5, nc=30, chunk.ncols=5)
    beachtest:::check_integer_mat(rFUN, nr=30, nc=5, chunk.ncols=2)

    beachtest:::check_integer_mat(rFUN, lambda=0.1, chunk.ncols=3)
    beachtest:::check_integer_mat(rFUN, nr=5, nc=30, lambda=0.1, chunk.ncols=5)
    beachtest:::check_integer_mat(rFUN, nr=30, nc=5, lambda=0.1, chunk.ncols=2)

    beachtest:::check_integer_mat(rFUN, lambda=1000, chunk.ncols=3) # Breaking out of the RAW type.
    beachtest:::check_integer_mat(rFUN, nr=5, nc=30, lambda=1000, chunk.ncols=5)
    beachtest:::check_integer_mat(rFUN, nr=30, nc=5, lambda=1000, chunk.ncols=2)
    
    beachtest:::check_integer_slice(rFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8), chunk.ncols=2)
    beachtest:::check_integer_slice(rFUN, lambda=0.1, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8), chunk.ncols=2)
 
    beachtest:::check_integer_const_mat(rFUN, chunk.ncols=2)
    beachtest:::check_integer_const_slice(rFUN, chunk.ncols=2, by.row=list(1:5, 6:8))

    beachtest:::check_integer_nonzero_mat(rFUN, chunk.ncols=2)
    beachtest:::check_integer_nonzero_slice(rFUN, chunk.ncols=2, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))
 
    # Checking for type. 
    beachtest:::check_type(rFUN, expected="integer")
    beachtest:::check_type(rFUN, chunk.ncols=2, expected="integer")
})

# Testing HDF5 matrices:

set.seed(34567)
library(HDF5Array)
hFUN <- function(nr=15, nc=10) {
    as(sFUN(nr, nc), "HDF5Array")
}

test_that("HDF5 integer matrix input is okay", {
    expect_s4_class(hFUN(), "HDF5Matrix")

    beachtest:::check_integer_mat(hFUN)
    beachtest:::check_integer_mat(hFUN, nr=5, nc=30)
    beachtest:::check_integer_mat(hFUN, nr=30, nc=5)
    
    beachtest:::check_integer_slice(hFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    # Checking const and non-zero options.
    beachtest:::check_integer_const_mat(hFUN)
    beachtest:::check_integer_const_slice(hFUN, by.row=list(1:5, 6:8))

    beachtest:::check_integer_nonzero_mat(hFUN)
    beachtest:::check_integer_nonzero_slice(hFUN, by.row=list(1:5, 6:8), by.col=list(1:5, 6:8))

    beachtest:::check_type(hFUN, expected="integer")
})

# Testing delayed operations:

sub_hFUN <- function() {
    A <- hFUN(15, 10)    
    A[1:10,]
}

add_hFUN <- function() {
    hFUN(15, 10) + 1L
}

test_that("Delayed integer matrix input is okay", {
    expect_s4_class(sub_hFUN(), "DelayedMatrix")
    beachtest:::check_integer_mat(sub_hFUN) 
    beachtest:::check_type(sub_hFUN, expected="integer")

    expect_s4_class(add_hFUN(), "DelayedMatrix")
    beachtest:::check_integer_mat(add_hFUN)
    beachtest:::check_type(add_hFUN, expected="integer")
    
    expect_identical("double", .Call(beachtest:::cxx_test_type_check, hFUN()+1)) # Proper type check!
})

#######################################################

# Testing conversions.

test_that("Integer matrix input conversions are okay", {
    beachtest:::check_integer_conversion(sFUN)

    beachtest:::check_integer_conversion(hFUN)
})

# Testing errors.

test_that("Integer matrix input error generation is okay", {
    beachtest:::check_integer_edge_errors(sFUN)

    beachtest:::check_integer_edge_errors(hFUN)
})

#######################################################

# Testing simple integer output:

set.seed(12345)

test_that("Simple integer matrix output is okay", {
    beachtest:::check_integer_output_mat(sFUN, hdf5.out=FALSE)

    beachtest:::check_integer_output_slice(sFUN, by.row=2:11, by.col=4:8, hdf5.out=FALSE)
})

# Testing HDF5 integer output:

test_that("HDF5 integer matrix output is okay", {
    beachtest:::check_integer_output_mat(hFUN, hdf5.out=TRUE)

    beachtest:::check_integer_output_slice(hFUN, by.row=5:15, by.col=8:10, hdf5.out=TRUE)

    beachtest:::check_integer_order(hFUN)
})

# Testing conversions:

test_that("Integer matrix output conversions are okay", {
    beachtest:::check_integer_converted_output(sFUN, hdf5.out=FALSE)

    beachtest:::check_integer_converted_output(hFUN, hdf5.out=TRUE)
})

# Testing for errors:

test_that("Integer matrix output error generation is okay", {
    beachtest:::check_integer_edge_output_errors(sFUN)

    beachtest:::check_integer_edge_output_errors(hFUN)
})

#######################################################

