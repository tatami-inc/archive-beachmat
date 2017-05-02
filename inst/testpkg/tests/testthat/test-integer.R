# This tests the ability of the API to properly access integer matrices of different types.
# library(beachtest); library(testthat); source("test-integer.R")

# Testing simple matrices:

set.seed(12345)
test.mat <- matrix(rpois(150, lambda=5), 15, 10)
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, test.mat, 3L))

test.mat <- matrix(rpois(150, lambda=5), 5, 30)
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, test.mat, 3L))

test.mat <- matrix(rpois(150, lambda=5), 30, 5)
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, test.mat, 3L))

test.mat <- matrix(rpois(150, lambda=5), 15, 10) # slices
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_integer_slice, test.mat, 1L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_integer_slice, test.mat, 2L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_integer_slice, test.mat, 1L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_integer_slice, test.mat, 2L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_integer_slice, test.mat, 1L, c(6L, 8L), c(1L, 5L))) 
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_integer_slice, test.mat, 2L, c(6L, 8L), c(1L, 5L)))
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_integer_slice, test.mat, 1L, c(6L, 8L), c(6L, 8L))) 
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_integer_slice, test.mat, 2L, c(6L, 8L), c(6L, 8L)))

expect_identical("integer", .Call(beachtest:::cxx_test_type_check, test.mat))

# Testing HDF5 matrices:

if (beachmat:::use.hdf5) { 

set.seed(34567)
library(HDF5Array)

test.mat <- matrix(rpois(150, lambda=5), 15, 10)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 3L))

test.mat <- matrix(rpois(150, lambda=5), 6, 25)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 3L))

test.mat <- matrix(rpois(150, lambda=5), 25, 6)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 3L))

test.mat <- matrix(rpois(150, lambda=5), 15, 10)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_integer_slice, A, 1L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_integer_slice, A, 2L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_integer_slice, A, 1L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_integer_slice, A, 2L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_integer_slice, A, 1L, c(6L, 8L), c(1L, 5L))) 
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_integer_slice, A, 2L, c(6L, 8L), c(1L, 5L)))
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_integer_slice, A, 1L, c(6L, 8L), c(6L, 8L))) 
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_integer_slice, A, 2L, c(6L, 8L), c(6L, 8L)))

expect_identical("integer", .Call(beachtest:::cxx_test_type_check, test.mat))

}
