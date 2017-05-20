# This tests the ability of the API to efficiently access sparse matrices by row.
# library(testthat); source("test-sparse.R")

library(Matrix)
check_numeric_indexing <- function(nr, nc, d) {
    A <- rsparsematrix(nrow=nr, ncol=nc, density=d)
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

test_that("Sparse numeric indexing is okay", {
    check_numeric_indexing(nr=nrows, nc=10, d=0.1)
    check_numeric_indexing(nr=nrows, nc=20, d=0.1)
    check_numeric_indexing(nr=nrows, nc=20, d=0.2)
})

# Checking random column slices behave correctly.

A <- rsparsematrix(nrow=100, 20, density=0.2)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
slice.start <- sample(ncol(A), nrow(A), replace=TRUE)
slice.end <- pmin(ncol(A), slice.start + sample(10, nrow(A), replace=TRUE))

out <- .Call(beachtest:::cxx_test_sparse_numeric_slice, A, cbind(slice.start, slice.end))
ref <- vector('list', nrow(A))
for (x in seq_along(ref)) { 
     ref[[x]] <- A[x,slice.start[x]:slice.end[x]]
}

test_that("Sparse numeric indexing with slices is okay", {
    expect_identical(out, ref)
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


