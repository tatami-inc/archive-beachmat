# This checks that the calculation of the best dimensions is working.

check.vals <- function(dims) {
    cdims <- getBestChunkDims(dims)
    N.per.row <- dims[2]/cdims[2]
    N.per.col <- dims[1]/cdims[1]
    expect_true(dims[1] >= N.per.row)
    expect_true(dims[2] >= N.per.col)
    return(cdims)
}

test_that("getBestChunkDims is working", {
    dims <- c(343, 3659)
    check.vals(dims)

    dims <- c(50000, 1000)
    check.vals(dims)

    dims <- c(2200, 50000)
    check.vals(dims)

    dims <- c(3942, 542941)
    check.vals(dims)

    dims <- c(124, 4342941)
    check.vals(dims)

    dims <- c(100, 23121)
    check.vals(dims)

    dims <- c(0, 2312)
    expect_true(all(getBestChunkDims(dims)==0L))

    dims <- c(123, 0)
    expect_true(all(getBestChunkDims(dims)==0L))
})
