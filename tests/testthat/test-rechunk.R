# This checks that the rechunking function is working.

library(HDF5Array)
test_that("rechunking is working", {
    set.seed(1000)
    A <- writeHDF5Array(matrix(runif(5000), nrow=100, ncol=50), chunk=c(10, 10))
    byrow <- rechunkByMargins(A, byrow=TRUE)
    bycol <- rechunkByMargins(A, byrow=FALSE)
    ref <- as.matrix(A)
    expect_identical(ref, as.matrix(byrow))
    expect_identical(ref, as.matrix(bycol))

    B <- writeHDF5Array(matrix(runif(5000), nrow=100, ncol=50), chunk=c(100, 1))
    byrow <- rechunkByMargins(B, byrow=TRUE)
    bycol <- rechunkByMargins(B, byrow=FALSE)
    ref <- as.matrix(B)
    expect_identical(ref, as.matrix(byrow))
    expect_identical(ref, as.matrix(bycol))

    C <- writeHDF5Array(matrix(runif(5000), nrow=100, ncol=50), chunk=c(1, 50))
    byrow <- rechunkByMargins(C, byrow=TRUE)
    bycol <- rechunkByMargins(C, byrow=FALSE)
    ref <- as.matrix(C)
    expect_identical(ref, as.matrix(byrow))
    expect_identical(ref, as.matrix(bycol))

    # Checking for contiguous input.
    D <- writeHDF5Array(matrix(runif(5000), nrow=100, ncol=50), level=0)
    byrow <- rechunkByMargins(D, byrow=TRUE)
    bycol <- rechunkByMargins(D, byrow=FALSE)
    ref <- as.matrix(D)
    expect_identical(ref, as.matrix(byrow))
    expect_identical(ref, as.matrix(bycol))
    expect_error(rechunkByMargins(D, outlevel=0), "compression level of 0 implies a contiguous layout")
})
