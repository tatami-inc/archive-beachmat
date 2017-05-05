# This tests the ability of the API to properly access integer matrices of different types.
# library(beachtest); library(testthat); source("test-integer.R")

##############################################
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

##############################################
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

expect_identical("integer", .Call(beachtest:::cxx_test_type_check, A))

B <- A[1:10,] # Testing delayed operations
resolved <- as.matrix(B)
expect_s4_class(B, "DelayedMatrix")
expect_identical(resolved, .Call(beachtest:::cxx_test_integer_access, B, 1L))

B <- A + 1L
resolved <- as.matrix(B)
expect_s4_class(B, "DelayedMatrix")
expect_identical(resolved, .Call(beachtest:::cxx_test_integer_access, B, 1L))

expect_identical("integer", .Call(beachtest:::cxx_test_type_check, B))
B <- A + 1
expect_identical("double-precision", .Call(beachtest:::cxx_test_type_check, B)) # Proper type check!

}

##############################################
# Testing bigmemory matrices:

library(bigmemory)
test.mat <- matrix(rpois(150, lambda=5), 15, 10)
A <- as.big.matrix(test.mat, type="integer")
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 3L))

test.mat <- matrix(rpois(150, lambda=5), 5, 30)
A <- as.big.matrix(test.mat, type="integer")
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 3L))

test.mat <- matrix(rpois(150, lambda=5), 30, 5)
A <- as.big.matrix(test.mat, type="integer")
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 3L))

test.mat <- matrix(rpois(150, lambda=5), 15, 10) # slices
A <- as.big.matrix(test.mat, type="integer")
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_integer_slice, A, 1L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_integer_slice, A, 2L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_integer_slice, A, 1L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_integer_slice, A, 2L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_integer_slice, A, 1L, c(6L, 8L), c(1L, 5L))) 
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_integer_slice, A, 2L, c(6L, 8L), c(1L, 5L)))
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_integer_slice, A, 1L, c(6L, 8L), c(6L, 8L))) 
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_integer_slice, A, 2L, c(6L, 8L), c(6L, 8L)))

test.mat <- matrix(rpois(150, lambda=5), 30, 5)
A <- as.big.matrix(test.mat, type="integer", separated=TRUE)
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_access, A, 3L))

test.mat <- matrix(rpois(150, lambda=5), 15, 10) # slices
A <- as.big.matrix(test.mat, type="integer", separated=TRUE)
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_integer_slice, A, 1L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_integer_slice, A, 2L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_integer_slice, A, 1L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_integer_slice, A, 2L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_integer_slice, A, 1L, c(6L, 8L), c(1L, 5L))) 
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_integer_slice, A, 2L, c(6L, 8L), c(1L, 5L)))
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_integer_slice, A, 1L, c(6L, 8L), c(6L, 8L))) 
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_integer_slice, A, 2L, c(6L, 8L), c(6L, 8L)))

expect_identical("integer", .Call(beachtest:::cxx_test_type_check, A))

##############################################
##############################################
##############################################
# Testing simple integer output:

set.seed(12345)
test.mat <- matrix(rpois(150, lambda=5), 15, 10)
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_output, test.mat, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_output, test.mat, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_output, test.mat, 3L))

test.mat <- matrix(rpois(150, lambda=5), 15, 10) # slices
chosen.rows <- 2:11
chosen.cols <- 4:8
stuff <- .Call(beachtest:::cxx_test_integer_output_slice, test.mat, 1L, range(chosen.rows), range(chosen.cols))
expect_identical(test.mat[chosen.rows, chosen.cols], stuff[chosen.rows, chosen.cols]) 
stuff[chosen.rows, chosen.cols] <- 0L
expect_true(all(stuff==0L))

stuff <- .Call(beachtest:::cxx_test_integer_output_slice, test.mat, 2L, range(chosen.rows), range(chosen.cols))
expect_identical(test.mat[chosen.rows, chosen.cols], stuff[chosen.rows, chosen.cols]) 
stuff[chosen.rows, chosen.cols] <- 0L
expect_true(all(stuff==0L))

# Testing HDF5 integer output:

if (beachmat:::use.hdf5) { 

library(HDF5Array)
test.mat <- matrix(rpois(150, lambda=5), 15, 10)
A <- as(test.mat, "HDF5Array")
B <- .Call(beachtest:::cxx_test_integer_output, A, 1L)
expect_s4_class(B, "HDF5Matrix")
expect_identical(test.mat, as.matrix(B))

B <- .Call(beachtest:::cxx_test_integer_output, A, 2L)
expect_s4_class(B, "HDF5Matrix")
expect_identical(test.mat, as.matrix(B))

B <- .Call(beachtest:::cxx_test_integer_output, A, 3L)
expect_s4_class(B, "HDF5Matrix")
expect_identical(test.mat, as.matrix(B))

test.mat <- matrix(rpois(150, lambda=5), 15, 10) # slices
A <- as(test.mat, "HDF5Array")
chosen.rows <- 5:15
chosen.cols <- 8:10
stuff <- .Call(beachtest:::cxx_test_integer_output_slice, A, 1L, range(chosen.rows), range(chosen.cols))
expect_s4_class(stuff, "HDF5Matrix")
simple.mat <- as.matrix(stuff)
expect_identical(test.mat[chosen.rows, chosen.cols], simple.mat[chosen.rows, chosen.cols]) 
simple.mat[chosen.rows, chosen.cols] <- 0L
expect_true(all(simple.mat==0L))

stuff <- .Call(beachtest:::cxx_test_integer_output_slice, A, 2L, range(chosen.rows), range(chosen.cols))
expect_s4_class(stuff, "HDF5Matrix")
simple.mat <- as.matrix(stuff)
expect_identical(test.mat[chosen.rows, chosen.cols], simple.mat[chosen.rows, chosen.cols])
simple.mat[chosen.rows, chosen.cols] <- 0L
expect_true(all(simple.mat==0L))

}
