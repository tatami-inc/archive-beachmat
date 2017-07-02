# Checks for proper execution of pkgconfig.

test_that("pkgconfig reports some output", {
    expect_output(pkgconfig('PKG_CPPFLAGS'))
    expect_output(pkgconfig('PKG_LIBS'))
})

