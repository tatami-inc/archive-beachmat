# This tests the ability of the API to properly access character matrices of different types.
# library(beachtest); library(testthat); source("test-character.R")

genwords <- function(n = 5000) {
    all.choices <- c(rep("", 4), LETTERS) # to get variable length strings.
    a <- do.call(paste0, replicate(5, sample(all.choices, n, TRUE), FALSE))
    paste0(a, sprintf("%04d", sample(9999, n, TRUE)), sample(LETTERS, n, TRUE))
}

# Testing simple matrices:

set.seed(12345)
test.mat <- matrix(genwords(150), 15, 10)
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, test.mat, 3L))

test.mat <- matrix(genwords(150), 5, 30)
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, test.mat, 3L))

test.mat <- matrix(genwords(150), 30, 5)
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, test.mat, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, test.mat, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, test.mat, 3L))

test.mat <- matrix(genwords(150), 15, 10) # slices
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_character_slice, test.mat, 1L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_character_slice, test.mat, 2L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_character_slice, test.mat, 1L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_character_slice, test.mat, 2L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_character_slice, test.mat, 1L, c(6L, 8L), c(1L, 5L))) 
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_character_slice, test.mat, 2L, c(6L, 8L), c(1L, 5L)))
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_character_slice, test.mat, 1L, c(6L, 8L), c(6L, 8L))) 
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_character_slice, test.mat, 2L, c(6L, 8L), c(6L, 8L)))

expect_identical("character", .Call(beachtest:::cxx_test_type_check, test.mat))

# Testing HDF5 matrices:

if (beachmat:::use.hdf5) { 

set.seed(34567)
library(HDF5Array)

test.mat <- matrix(genwords(150), 15, 10)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, A, 3L))

test.mat <- matrix(genwords(150), 6, 25)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, A, 3L))

test.mat <- matrix(genwords(150), 25, 6)
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, A, 1L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, A, 2L))
expect_identical(test.mat, .Call(beachtest:::cxx_test_character_access, A, 3L))

test.mat <- matrix(genwords(150), 15, 10) # slices
A <- as(test.mat, "HDF5Array")
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_character_slice, A, 1L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,1:5], .Call(beachtest:::cxx_test_character_slice, A, 2L, c(1L, 5L), c(1L, 5L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_character_slice, A, 1L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[1:5,6:8], .Call(beachtest:::cxx_test_character_slice, A, 2L, c(1L, 5L), c(6L, 8L)))
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_character_slice, A, 1L, c(6L, 8L), c(1L, 5L))) 
expect_identical(test.mat[6:8,1:5], .Call(beachtest:::cxx_test_character_slice, A, 2L, c(6L, 8L), c(1L, 5L)))
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_character_slice, A, 1L, c(6L, 8L), c(6L, 8L))) 
expect_identical(test.mat[6:8,6:8], .Call(beachtest:::cxx_test_character_slice, A, 2L, c(6L, 8L), c(6L, 8L)))

expect_identical("character", .Call(beachtest:::cxx_test_type_check, test.mat))

}
