# This tests the ability of the API to efficiently access sparse matrices by row.
# library(testthat); source("test-sparse.R")

check_numeric_indexing <- function(FUN, ...) { 
    A <- FUN(...)
    test.mat <- as.matrix(A)
    dimnames(test.mat) <- NULL
    
    testthat::expect_identical(test.mat, .Call(beachtest:::cxx_test_sparse_numeric, A, 1:nrows)) # forward
    testthat::expect_identical(test.mat[nrows:1,], .Call(beachtest:::cxx_test_sparse_numeric, A, nrows:1)) # reverse
    for (it in 1:3) {
        o <- sample(nrows) # random directions
        testthat::expect_identical(test.mat[o,], .Call(beachtest:::cxx_test_sparse_numeric, A, o))
    }
    
    return(invisible(NULL))
}

set.seed(23456)
nrows <- 1000

library(Matrix)
test_that("Sparse numeric indexing is okay", {
    check_numeric_indexing(FUN=rsparsematrix, nrow=nrows, ncol=10, density=0.1)
    check_numeric_indexing(FUN=rsparsematrix, nrow=nrows, ncol=20, density=0.1)
    check_numeric_indexing(FUN=rsparsematrix, nrow=nrows, ncol=20, density=0.2)
})

# Repeating for RLE matrices.

set.seed(34567)
library(DelayedArray)
rFUN <- function(nr=15, nc=10, density=0.2) {
    as(as.matrix(rsparsematrix(nr, nc, density)), "RleArray")
}

test_that("RLE numeric indexing is okay", {
    check_numeric_indexing(FUN=rFUN, nr=nrows, nc=10, density=0.1)
    check_numeric_indexing(FUN=rFUN, nr=nrows, nc=20, density=0.1)
    check_numeric_indexing(FUN=rFUN, nr=nrows, nc=20, density=0.2)
})

# Checking random column slices behave correctly.

check_col_slices <- function(FUN, ...) { 
    A <- FUN(...)

    test.mat <- as.matrix(A)
    dimnames(test.mat) <- NULL
    slice.start <- sample(ncol(A), nrow(A), replace=TRUE)
    slice.end <- pmin(ncol(A), slice.start + sample(10, nrow(A), replace=TRUE))
    
    out <- .Call(beachtest:::cxx_test_sparse_numeric_slice, A, cbind(slice.start, slice.end))
    ref <- vector('list', nrow(A))
    for (x in seq_along(ref)) { 
        ref[[x]] <- as.vector(A[x,slice.start[x]:slice.end[x]])
    }
    expect_identical(out, ref)
}

test_that("Sparse numeric indexing with slices is okay", {
    check_col_slices(FUN=rsparsematrix, nrow=100, ncol=20, density=0.2)
    check_col_slices(FUN=rsparsematrix, nrow=100, ncol=20, density=0.1)
    check_col_slices(FUN=rsparsematrix, nrow=100, ncol=50, density=0.2)
})

# Repeating with RLE matrix.

test_that("RLE numeric indexing with slices is okay", {
    check_col_slices(FUN=rFUN, nr=100, nc=20, density=0.2)
    check_col_slices(FUN=rFUN, nr=100, nc=20, density=0.1)
    check_col_slices(FUN=rFUN, nr=100, nc=50, density=0.2)
})

# # Benchmarking with a huge matrix.
# library(microbenchmark); library(beachmat); library(Matrix); library(beachtest)
# nrows <- 10000
# A <- rsparsematrix(nrow=nrows, 200, density=0.1)
# f <- 1:nrows
# r <- nrows:1
# o <- sample(nrows)
# microbenchmark(.Call(beachtest:::cxx_test_sparse_numeric, A, f),
#                .Call(beachtest:::cxx_test_sparse_numeric, A, r),
#                .Call(beachtest:::cxx_test_sparse_numeric, A, o))


