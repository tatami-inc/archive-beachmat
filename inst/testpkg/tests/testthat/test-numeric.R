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

test.mat <- matrix(as.double(rpois(150, lambda=5)), 15, 10) # slices
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_numeric_slice, test.mat, 1L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_numeric_slice, test.mat, 2L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_numeric_slice, test.mat, 1L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_numeric_slice, test.mat, 2L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_numeric_slice, test.mat, 1L, c(6L, 8L), c(1L, 5L))) 
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_numeric_slice, test.mat, 2L, c(6L, 8L), c(1L, 5L)))
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_numeric_slice, test.mat, 1L, c(6L, 8L), c(6L, 8L))) 
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_numeric_slice, test.mat, 2L, c(6L, 8L), c(6L, 8L)))

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

test.mat <- matrix(as.double(rpois(150, lambda=5)), 15, 10) # slices
A <- Matrix(test.mat)
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_numeric_slice, A, 2L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_numeric_slice, A, 2L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(6L, 8L), c(1L, 5L))) 
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_numeric_slice, A, 2L, c(6L, 8L), c(1L, 5L)))
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(6L, 8L), c(6L, 8L))) 
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_numeric_slice, A, 2L, c(6L, 8L), c(6L, 8L)))

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

A <- rsparsematrix(nrow=15, 10, density=0.1) # slices
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_numeric_slice, A, 2L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_numeric_slice, A, 2L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(6L, 8L), c(1L, 5L))) 
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_numeric_slice, A, 2L, c(6L, 8L), c(1L, 5L)))
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(6L, 8L), c(6L, 8L))) 
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_numeric_slice, A, 2L, c(6L, 8L), c(6L, 8L)))

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

A <- pack(forceSymmetric(matrix(as.double(rpois(100, lambda=5)), 20, 20))) # slices (mode=1L fills by column, so only testing row slices).
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat[1:5,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(1L, 5L), c(1L, 20L)))
expect_identical(test.mat[1:10,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(1L, 10L), c(1L, 20L)))
expect_identical(test.mat[1:20,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(1L, 20L), c(1L, 20L)))
expect_identical(test.mat[5:10,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(5L, 10L), c(1L, 20L)))
expect_identical(test.mat[5:15,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(5L, 15L), c(1L, 20L)))
expect_identical(test.mat[5:20,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(5L, 20L), c(1L, 20L)))
expect_identical(test.mat[10:15,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(10L, 15L), c(1L, 20L)))
expect_identical(test.mat[10:20,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(10L, 20L), c(1L, 20L)))

A <- pack(forceSymmetric(matrix(as.double(rpois(100, lambda=5)), 20, 20), "L")) # more slices
test.mat <- as.matrix(A)
dimnames(test.mat) <- NULL
expect_identical(test.mat[1:5,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(1L, 5L), c(1L, 20L)))
expect_identical(test.mat[1:10,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(1L, 10L), c(1L, 20L)))
expect_identical(test.mat[1:20,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(1L, 20L), c(1L, 20L)))
expect_identical(test.mat[5:10,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(5L, 10L), c(1L, 20L)))
expect_identical(test.mat[5:15,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(5L, 15L), c(1L, 20L)))
expect_identical(test.mat[5:20,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(5L, 20L), c(1L, 20L)))
expect_identical(test.mat[10:15,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(10L, 15L), c(1L, 20L)))
expect_identical(test.mat[10:20,], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(10L, 20L), c(1L, 20L)))

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

test.mat <- matrix(as.double(rpois(150, lambda=5)), 15, 10) # slices
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_numeric_slice, A, 2L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_numeric_slice, A, 2L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(6L, 8L), c(1L, 5L))) 
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_numeric_slice, A, 2L, c(6L, 8L), c(1L, 5L)))
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_numeric_slice, A, 1L, c(6L, 8L), c(6L, 8L))) 
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_numeric_slice, A, 2L, c(6L, 8L), c(6L, 8L)))

expect_identical("double-precision", .Call(beachtest:::cxx_test_type_check, A))

B <- A[1:10,] # Testing delayed operations
resolved <- as.matrix(B)
expect_s4_class(B, "DelayedMatrix")
expect_identical(resolved, .Call(beachtest:::cxx_test_numeric_access, B, 1L))

B <- A + 1
resolved <- as.matrix(B)
expect_s4_class(B, "DelayedMatrix")
expect_identical(resolved, .Call(beachtest:::cxx_test_numeric_access, B, 1L))

expect_identical("double-precision", .Call(beachtest:::cxx_test_type_check, B))
B <- A > 0
expect_identical("logical", .Call(beachtest:::cxx_test_type_check, B)) # Proper type check!

}

# Testing simple numeric output:

set.seed(12345)
test.mat <- matrix(rnorm(150), 15, 10)
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_output, test.mat, 1L, FALSE))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_output, test.mat, 2L, FALSE))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_output, test.mat, 3L, FALSE))

flip.rows <- nrow(test.mat):1 # Refilling, to test getters.
flip.cols <- ncol(test.mat):1
expect_identical(test.mat[flip.rows,], .Call(beachtest:::cxx_test_numeric_output, test.mat, 1L, TRUE))
expect_identical(test.mat[,flip.cols], .Call(beachtest:::cxx_test_numeric_output, test.mat, 2L, TRUE))
expect_identical(test.mat, .Call(beachtest:::cxx_test_numeric_output, test.mat, 3L, TRUE))

test.mat <- matrix(rnorm(150), 15, 10) # slices
chosen.rows <- 1:12
chosen.cols <- 3:7

stuff <- .Call(beachtest:::cxx_test_numeric_output_slice, test.mat, 1L, range(chosen.rows), range(chosen.cols), FALSE)
expect_identical(test.mat[chosen.rows, chosen.cols], stuff[chosen.rows, chosen.cols]) 
tmp <- stuff
tmp[chosen.rows, chosen.cols] <- 0L
expect_true(all(tmp==0L))
restuff <- .Call(beachtest:::cxx_test_numeric_output_slice, test.mat, 1L, range(chosen.rows), range(chosen.cols), TRUE)
expect_identical(stuff[rev(chosen.rows),], restuff[chosen.rows,])

stuff <- .Call(beachtest:::cxx_test_numeric_output_slice, test.mat, 2L, range(chosen.rows), range(chosen.cols), FALSE)
expect_identical(test.mat[chosen.rows, chosen.cols], stuff[chosen.rows, chosen.cols]) 
tmp <- stuff
tmp[chosen.rows, chosen.cols] <- 0L
expect_true(all(tmp==0L))
restuff <- .Call(beachtest:::cxx_test_numeric_output_slice, test.mat, 2L, range(chosen.rows), range(chosen.cols), TRUE)
expect_identical(stuff[,rev(chosen.cols)], restuff[,chosen.cols])

# Testing HDF5 numeric output:

if (beachmat:::use.hdf5) { 

library(HDF5Array)
test.mat <- matrix(rnorm(150), 15, 10)
A <- as(test.mat, "HDF5Array")

B <- .Call(beachtest:::cxx_test_numeric_output, A, 1L, FALSE)
expect_s4_class(B, "HDF5Matrix")
expect_identical(test.mat, as.matrix(B))
C <- .Call(beachtest:::cxx_test_numeric_output, A, 1L, TRUE)
expect_identical(test.mat[flip.rows,], as.matrix(C))

B <- .Call(beachtest:::cxx_test_numeric_output, A, 2L, FALSE)
expect_s4_class(B, "HDF5Matrix")
expect_identical(test.mat, as.matrix(B))
C <- .Call(beachtest:::cxx_test_numeric_output, A, 2L, TRUE)
expect_identical(test.mat[,flip.cols], as.matrix(C))

B <- .Call(beachtest:::cxx_test_numeric_output, A, 3L, FALSE)
expect_s4_class(B, "HDF5Matrix")
expect_identical(test.mat, as.matrix(B))
C <- .Call(beachtest:::cxx_test_numeric_output, A, 3L, TRUE)
expect_identical(test.mat, as.matrix(C))

test.mat <- matrix(rnorm(150), 15, 10) # slices
A <- as(test.mat, "HDF5Array")
chosen.rows <- 1:2
chosen.cols <- 2:10

stuff <- .Call(beachtest:::cxx_test_numeric_output_slice, A, 1L, range(chosen.rows), range(chosen.cols), FALSE)
expect_s4_class(stuff, "HDF5Matrix")
simple.mat <- as.matrix(stuff)
expect_identical(test.mat[chosen.rows, chosen.cols], simple.mat[chosen.rows, chosen.cols]) 
simple.mat[chosen.rows, chosen.cols] <- 0
expect_true(all(simple.mat==0))
restuff <- .Call(beachtest:::cxx_test_numeric_output_slice, test.mat, 1L, range(chosen.rows), range(chosen.cols), TRUE)
expect_identical(test.mat[rev(chosen.rows),chosen.cols], as.matrix(restuff[chosen.rows,chosen.cols]))

stuff <- .Call(beachtest:::cxx_test_numeric_output_slice, A, 2L, range(chosen.rows), range(chosen.cols), FALSE)
expect_s4_class(stuff, "HDF5Matrix")
simple.mat <- as.matrix(stuff)
expect_identical(test.mat[chosen.rows, chosen.cols], simple.mat[chosen.rows, chosen.cols])
simple.mat[chosen.rows, chosen.cols] <- 0
expect_true(all(simple.mat==0))
restuff <- .Call(beachtest:::cxx_test_numeric_output_slice, test.mat, 2L, range(chosen.rows), range(chosen.cols), TRUE)
expect_identical(test.mat[chosen.rows,rev(chosen.cols)], as.matrix(restuff[chosen.rows,chosen.cols]))

}
