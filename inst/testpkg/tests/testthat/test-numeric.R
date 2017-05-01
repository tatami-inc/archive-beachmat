# This tests the ability of the API to properly access numeric matrices of different types.
# library(beachtest); library(testthat); source("test-numeric.R")

# Testing simple matrices:

set.seed(12345)
test.mat <- matrix(as.double(rpois(150, lambda=5)), 15, 10)
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, test.mat, 3L))

test.mat <- matrix(as.double(rpois(150, lambda=5)), 5, 30)
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, test.mat, 3L))

test.mat <- matrix(as.double(rpois(150, lambda=5)), 30, 5)
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, test.mat, 3L))

expect_identical("double-precision", .Call(beachtest:::cxx_test_type_check, test.mat))

# Testing dense matrices:

set.seed(13579)
library(Matrix)
test.mat <- matrix(as.double(rpois(150, lambda=5)), 15, 10)
A <- Matrix(test.mat)
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

test.mat <- matrix(as.double(rpois(150, lambda=5)), 5, 30)
A <- Matrix(test.mat)
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

test.mat <- matrix(as.double(rpois(150, lambda=5)), 30, 5)
A <- Matrix(test.mat)
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

expect_identical("double-precision", .Call(beachtest:::cxx_test_type_check, A))

# Testing sparse matrices (dgCMatrix):

set.seed(23456)
A <- rsparsematrix(nrow=15, 10, density=0.1)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

A <- rsparsematrix(nrow=15, 10, density=0.2)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

A <- rsparsematrix(nrow=30, 5, density=0.1)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

A <- rsparsematrix(nrow=30, 5, density=0.2)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

A <- rsparsematrix(nrow=5, 30, density=0.1)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

A <- rsparsematrix(nrow=5, 30, density=0.2)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

expect_identical("double-precision", .Call(beachtest:::cxx_test_type_check, A))

# Testing dense symmetric matrices (dspMatrix):

set.seed(45678)
A <- pack(forceSymmetric(matrix(as.double(rpois(100, lambda=5)), 10, 10)))
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

A <- pack(forceSymmetric(matrix(as.double(rpois(100, lambda=5)), 20, 20), "L"))
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

expect_identical("double-precision", .Call(beachtest:::cxx_test_type_check, A))

# Testing HDF5 matrices:

if (beachmat:::use.hdf5) { 

set.seed(34567)
library(HDF5Array)

test.mat <- matrix(as.double(rpois(150, lambda=5)), 15, 10)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

test.mat <- matrix(as.double(rpois(150, lambda=5)), 6, 25)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

test.mat <- matrix(as.double(rpois(150, lambda=5)), 25, 6)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_access, A, 3L))

expect_identical("double-precision", .Call(beachtest:::cxx_test_type_check, A))

}
