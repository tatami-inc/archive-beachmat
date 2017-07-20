# Assorted tests that don't fit anywhere else.
# library(testthat); source("test-misc.R")

test_that("Output mode choices are okay", {
    expect_identical(beachtest:::check_output_mode("simple", simplify=TRUE, preserve.zero=FALSE), "simple")
    expect_identical(beachtest:::check_output_mode("sparse", simplify=TRUE, preserve.zero=FALSE), "simple")
    expect_identical(beachtest:::check_output_mode("RLE", simplify=TRUE, preserve.zero=FALSE), "simple")
    expect_identical(beachtest:::check_output_mode("Psymm", simplify=TRUE, preserve.zero=FALSE), "simple")
    expect_identical(beachtest:::check_output_mode("sparse", simplify=FALSE, preserve.zero=TRUE), "sparse")
    expect_identical(beachtest:::check_output_mode("sparse", simplify=FALSE, preserve.zero=FALSE), "HDF5")
    expect_identical(beachtest:::check_output_mode("RLE", simplify=FALSE, preserve.zero=FALSE), "HDF5")
    expect_identical(beachtest:::check_output_mode("Psymm", simplify=FALSE, preserve.zero=FALSE), "HDF5")
    expect_identical(beachtest:::check_output_mode("HDF5", simplify=FALSE, preserve.zero=FALSE), "HDF5")
})


