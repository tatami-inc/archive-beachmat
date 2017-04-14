# This tests the ability of the API to properly access logical matrices of different types.
# library(beachmat); library(testthat); source("test-logical.R")

# Testing simple matrices:

set.seed(12345)
test.mat <- matrix(rbinom(150, 1, 0.5)==0, 15, 10)
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, test.mat, 3L))

test.mat <- matrix(rbinom(150, 1, 0.5)==0, 5, 30)
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, test.mat, 3L))

test.mat <- matrix(rbinom(150, 1, 0.5)==0, 30, 5)
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, test.mat, 3L))

# Testing dense matrices:

set.seed(13579)
library(Matrix)
test.mat <- matrix(rbinom(150, 1, 0.5)==0, 15, 10)
A <- Matrix(test.mat)
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

test.mat <- matrix(rbinom(150, 1, 0.5)==0, 5, 30)
A <- Matrix(test.mat)
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

test.mat <- matrix(rbinom(150, 1, 0.5)==0, 30, 5)
A <- Matrix(test.mat)
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

# Testing sparse matrices (dgCMatrix):

set.seed(23456)
A <- rsparsematrix(nrow=15, 10, density=0.1)!=0
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

A <- rsparsematrix(nrow=15, 10, density=0.2)!=0
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

A <- rsparsematrix(nrow=30, 5, density=0.1)!=0
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

A <- rsparsematrix(nrow=30, 5, density=0.2)!=0
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

A <- rsparsematrix(nrow=5, 30, density=0.1)!=0
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

A <- rsparsematrix(nrow=5, 30, density=0.2)!=0
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

# Testing dense symmetric matrices (lspMatrix):

set.seed(45678)
A <- pack(forceSymmetric(matrix(rbinom(100, 1, 0.5)==0, 10, 10)))
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

A <- pack(forceSymmetric(matrix(rbinom(400, 1, 0.5)==0, 20, 20), "L"))
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))


# Testing HDF5 matrices:

set.seed(34567)
library(HDF5Array)

test.mat <- matrix(rbinom(150, 1, 0.5)==0, 15, 10)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

test.mat <- matrix(rbinom(150, 1, 0.5)==0, 6, 25)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

test.mat <- matrix(rbinom(150, 1, 0.5)==0, 25, 6)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_logical_access, A, 3L))

