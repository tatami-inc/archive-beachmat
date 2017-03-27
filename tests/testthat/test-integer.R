# This tests the ability of the API to properly access integer matrices of different types.
# library(beachmat); library(testthat); source("test-integer.R")

# Testing simple matrices:

set.seed(12345)
test.mat <- matrix(rpois(150, lambda=5), 15, 10)
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, test.mat, 3L))

test.mat <- matrix(rpois(150, lambda=5), 5, 30)
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, test.mat, 3L))

test.mat <- matrix(rpois(150, lambda=5), 30, 5)
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, test.mat, 3L))

# Testing HDF5 matrices:

set.seed(34567)
library(HDF5Array)

test.mat <- matrix(rpois(150, lambda=5), 15, 10)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, A, 3L))

test.mat <- matrix(rpois(150, lambda=5), 6, 25)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, A, 3L))

test.mat <- matrix(rpois(150, lambda=5), 25, 6)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, A, 1L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, A, 2L))
expect_identical(test.mat, .Call(beachmat:::cxx_test_integer_access, A, 3L))

