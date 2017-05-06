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

# Testing simple integer output:

set.seed(12345)
test.mat <- matrix(rpois(150, lambda=5), 15, 10)
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_output, test.mat, 1L, FALSE))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_output, test.mat, 2L, FALSE))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_output, test.mat, 3L, FALSE))

flip.rows <- nrow(test.mat):1 # Refilling, to test getters.
flip.cols <- ncol(test.mat):1
expect_identical(test.mat[flip.rows,], .Call(beachtest:::cxx_test_integer_output, test.mat, 1L, TRUE))
expect_identical(test.mat[,flip.cols], .Call(beachtest:::cxx_test_integer_output, test.mat, 2L, TRUE))
expect_identical(test.mat, .Call(beachtest:::cxx_test_integer_output, test.mat, 3L, TRUE))

test.mat <- matrix(rpois(150, lambda=5), 15, 10) # slices
chosen.rows <- 2:11
chosen.cols <- 4:8

stuff <- .Call(beachtest:::cxx_test_integer_output_slice, test.mat, 1L, range(chosen.rows), range(chosen.cols), FALSE)
expect_identical(test.mat[chosen.rows, chosen.cols], stuff[chosen.rows, chosen.cols]) 
tmp <- stuff
tmp[chosen.rows, chosen.cols] <- 0L
expect_true(all(tmp==0L))
restuff <- .Call(beachtest:::cxx_test_integer_output_slice, test.mat, 1L, range(chosen.rows), range(chosen.cols), TRUE)
expect_identical(stuff[rev(chosen.rows),], restuff[chosen.rows,])

stuff <- .Call(beachtest:::cxx_test_integer_output_slice, test.mat, 2L, range(chosen.rows), range(chosen.cols), FALSE)
expect_identical(test.mat[chosen.rows, chosen.cols], stuff[chosen.rows, chosen.cols]) 
tmp <- stuff
tmp[chosen.rows, chosen.cols] <- 0L
expect_true(all(tmp==0L))
restuff <- .Call(beachtest:::cxx_test_integer_output_slice, test.mat, 2L, range(chosen.rows), range(chosen.cols), TRUE)
expect_identical(stuff[,rev(chosen.cols)], restuff[,chosen.cols])

# Testing HDF5 integer output:

if (beachmat:::use.hdf5) { 

library(HDF5Array)
test.mat <- matrix(rpois(150, lambda=5), 15, 10)
A <- as(test.mat, "HDF5Array")

B <- .Call(beachtest:::cxx_test_integer_output, A, 1L, FALSE)
expect_s4_class(B, "HDF5Matrix")
expect_identical(test.mat, as.matrix(B))
C <- .Call(beachtest:::cxx_test_integer_output, A, 1L, TRUE)
expect_identical(test.mat[flip.rows,], as.matrix(C))

B <- .Call(beachtest:::cxx_test_integer_output, A, 2L, FALSE)
expect_s4_class(B, "HDF5Matrix")
expect_identical(test.mat, as.matrix(B))
C <- .Call(beachtest:::cxx_test_integer_output, A, 2L, TRUE)
expect_identical(test.mat[,flip.cols], as.matrix(C))

B <- .Call(beachtest:::cxx_test_integer_output, A, 3L, FALSE)
expect_s4_class(B, "HDF5Matrix")
expect_identical(test.mat, as.matrix(B))
C <- .Call(beachtest:::cxx_test_integer_output, A, 3L, TRUE)
expect_identical(test.mat, as.matrix(C))

test.mat <- matrix(rpois(150, lambda=5), 15, 10) # slices
A <- as(test.mat, "HDF5Array")
chosen.rows <- 5:15
chosen.cols <- 8:10

stuff <- .Call(beachtest:::cxx_test_integer_output_slice, A, 1L, range(chosen.rows), range(chosen.cols), FALSE)
expect_s4_class(stuff, "HDF5Matrix")
simple.mat <- as.matrix(stuff)
expect_identical(test.mat[chosen.rows, chosen.cols], simple.mat[chosen.rows, chosen.cols]) 
simple.mat[chosen.rows, chosen.cols] <- 0L
expect_true(all(simple.mat==0L))
restuff <- .Call(beachtest:::cxx_test_integer_output_slice, test.mat, 1L, range(chosen.rows), range(chosen.cols), TRUE)
expect_identical(test.mat[rev(chosen.rows),chosen.cols], as.matrix(restuff[chosen.rows,chosen.cols]))

stuff <- .Call(beachtest:::cxx_test_integer_output_slice, A, 2L, range(chosen.rows), range(chosen.cols), FALSE)
expect_s4_class(stuff, "HDF5Matrix")
simple.mat <- as.matrix(stuff)
expect_identical(test.mat[chosen.rows, chosen.cols], simple.mat[chosen.rows, chosen.cols])
simple.mat[chosen.rows, chosen.cols] <- 0L
expect_true(all(simple.mat==0L))
restuff <- .Call(beachtest:::cxx_test_integer_output_slice, test.mat, 2L, range(chosen.rows), range(chosen.cols), TRUE)
expect_identical(test.mat[chosen.rows,rev(chosen.cols)], as.matrix(restuff[chosen.rows,chosen.cols]))

}
