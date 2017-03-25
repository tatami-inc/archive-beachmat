# This tests the ability of the API to properly access numeric matrices of different types.
# library(beachmat); library(testthat); source("test-numeric.R")

# Testing simple matrices:

set.seed(12345)
test.mat <- matrix(as.double(rpois(150, lambda=5)), 15, 10)
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, test.mat, 3L))

test.mat <- matrix(as.double(rpois(150, lambda=5)), 5, 30)
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, test.mat, 3L))

test.mat <- matrix(as.double(rpois(150, lambda=5)), 30, 5)
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, test.mat, 3L))

# Testing sparse matrices:

library(beachmat); library(testthat);

set.seed(12345)
library(Matrix)
A <- rsparsematrix(nrow=15, 10, density=0.1)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 3L))

A <- rsparsematrix(nrow=15, 10, density=0.2)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 3L))

A <- rsparsematrix(nrow=30, 5, density=0.1)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 3L))

A <- rsparsematrix(nrow=30, 5, density=0.2)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 3L))

A <- rsparsematrix(nrow=5, 30, density=0.1)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 3L))

A <- rsparsematrix(nrow=5, 30, density=0.2)
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_numeric_access, A, 3L))

