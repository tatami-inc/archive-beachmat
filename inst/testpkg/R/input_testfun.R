# Creating functions to check each set of matrix inputs.

.check_mat <- function(FUN, ..., cxxfun) {
    for (it in 1:3) {
        test.mat <- FUN(...)
        ref <- as.matrix(test.mat)
        dimnames(ref) <- NULL
        testthat::expect_identical(ref, .Call(cxxfun, test.mat, it))
    }
    return(invisible(NULL))
}

.check_slices <- function(FUN, ..., by.row, by.col, cxxfun) {
    for (x in by.row) {
        rx <- range(x)
        for (y in by.col) {
            ry <- range(y)
            test.mat <- FUN(...) 
            ref <- as.matrix(test.mat[x, y, drop=FALSE])
            dimnames(ref) <- NULL
            testthat::expect_identical(ref, .Call(cxxfun, test.mat, 1L, rx, ry))
            testthat::expect_identical(ref, .Call(cxxfun, test.mat, 2L, rx, ry))
        }
    }
    return(invisible(NULL))
}

check_integer_mat <- function(FUN, ...) {
    .check_mat(FUN=FUN, ..., cxxfun=cxx_test_integer_access)
}

check_integer_slice <- function(FUN, ..., by.row, by.col) {
    .check_slices(FUN=FUN, ..., by.row=by.row, by.col=by.col, cxxfun=cxx_test_integer_slice)
}

check_character_mat <- function(FUN, ...) {
    .check_mat(FUN=FUN, ..., cxxfun=cxx_test_character_access)
}

check_character_slice <- function(FUN, ..., by.row, by.col) {
    .check_slices(FUN=FUN, ..., by.row=by.row, by.col=by.col, cxxfun=cxx_test_character_slice)
}

check_numeric_mat <- function(FUN, ...) {
    .check_mat(FUN=FUN, ..., cxxfun=cxx_test_numeric_access)
}

check_numeric_slice <- function(FUN, ..., by.row, by.col) {
    .check_slices(FUN=FUN, ..., by.row=by.row, by.col=by.col, cxxfun=cxx_test_numeric_slice)
}

check_logical_mat <- function(FUN, ...) {
    .check_mat(FUN=FUN, ..., cxxfun=cxx_test_logical_access)
}

check_logical_slice <- function(FUN, ..., by.row, by.col) {
    .check_slices(FUN=FUN, ..., by.row=by.row, by.col=by.col, cxxfun=cxx_test_logical_slice)
}

check_type <- function(FUN, ..., expected) {
    testthat::expect_identical(expected, .Call(cxx_test_type_check, FUN(...)))
}

.check_conversion <- function(FUN, ..., cxxfun, rfun) {
    for (it in 1:2) {
        test.mat <- FUN(...)
        ref <- as.matrix(test.mat)
        dimnames(ref) <- NULL
        testthat::expect_identical(rfun(ref), .Call(cxxfun, test.mat, it))
    }
    return(invisible(NULL))
}

check_numeric_conversion  <- function(FUN, ...) {
    .check_conversion(FUN=FUN, ..., cxxfun=cxx_test_numeric_to_integer,
                      rfun=function(x) {
                          storage.mode(x) <- "integer" 
                          return(x)
                      })
}

check_integer_conversion <- function(FUN, ...) {
    .check_conversion(FUN=FUN, ..., cxxfun=cxx_test_integer_to_numeric, 
                      rfun=function(x) { 
                          storage.mode(x) <- "double"
                          return(x)
                      })
}

check_logical_conversion <- function(FUN, ...) {
    .check_conversion(FUN=FUN, ..., cxxfun=cxx_test_logical_to_numeric, 
                      rfun=function(x) { 
                          storage.mode(x) <- "double"
                          return(x)
                      })
    .check_conversion(FUN=FUN, ..., cxxfun=cxx_test_logical_to_integer, 
                      rfun=function(x) { 
                          storage.mode(x) <- "integer"
                          return(x)
                      })
}


