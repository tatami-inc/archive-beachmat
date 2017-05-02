# This tests the ability of the API to efficiently access sparse matrices by row.
 library(beachtest); library(testthat); #source("test-sparse.R")

set.seed(23456)
nrows <- 1000

library(Matrix)
A <- rsparsematrix(nrow=nrows, 10, density=0.1)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL

expect_identical(test.mat, .Call(beachtest:::cxx_test_sparse_numeric, A, 1:nrows)) # forward
expect_identical(test.mat[nrows:1,], .Call(beachtest:::cxx_test_sparse_numeric, A, nrows:1)) # reverse
o <- sample(nrows) # random directions
expect_identical(test.mat[o,], .Call(beachtest:::cxx_test_sparse_numeric, A, o))
o <- sample(nrows)
expect_identical(test.mat[o,], .Call(beachtest:::cxx_test_sparse_numeric, A, o))
o <- sample(nrows)
expect_identical(test.mat[o,], .Call(beachtest:::cxx_test_sparse_numeric, A, o))

A <- rsparsematrix(nrow=nrows, 20, density=0.1)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL

expect_identical(test.mat, .Call(beachtest:::cxx_test_sparse_numeric, A, 1:nrows)) # forward
expect_identical(test.mat[nrows:1,], .Call(beachtest:::cxx_test_sparse_numeric, A, nrows:1)) # reverse
o <- sample(nrows) # random directions
expect_identical(test.mat[o,], .Call(beachtest:::cxx_test_sparse_numeric, A, o))
o <- sample(nrows)
expect_identical(test.mat[o,], .Call(beachtest:::cxx_test_sparse_numeric, A, o))
o <- sample(nrows)
expect_identical(test.mat[o,], .Call(beachtest:::cxx_test_sparse_numeric, A, o))

A <- rsparsematrix(nrow=nrows, 20, density=0.2)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL

expect_identical(test.mat, .Call(beachtest:::cxx_test_sparse_numeric, A, 1:nrows)) # forward
expect_identical(test.mat[nrows:1,], .Call(beachtest:::cxx_test_sparse_numeric, A, nrows:1)) # reverse
o <- sample(nrows) # random directions
expect_identical(test.mat[o,], .Call(beachtest:::cxx_test_sparse_numeric, A, o))
o <- sample(nrows)
expect_identical(test.mat[o,], .Call(beachtest:::cxx_test_sparse_numeric, A, o))
o <- sample(nrows)
expect_identical(test.mat[o,], .Call(beachtest:::cxx_test_sparse_numeric, A, o))

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
expect_identical(out, ref)

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


