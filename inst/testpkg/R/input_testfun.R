# Creating functions to check each set of matrix inputs.

###############################

.check_mat <- function(FUN, ..., cxxfun, nmodes=3) {
    for (it in seq_len(nmodes)) {
        test.mat <- FUN(...)
        ref <- as.matrix(test.mat)
        dimnames(ref) <- NULL
        testthat::expect_identical(ref, .Call(cxxfun, test.mat, it))
    }
    return(invisible(NULL))
}

check_integer_mat <- function(FUN, ...) {
    .check_mat(FUN=FUN, ..., cxxfun=cxx_test_integer_access)
}

check_character_mat <- function(FUN, ...) {
    .check_mat(FUN=FUN, ..., cxxfun=cxx_test_character_access)
}

check_numeric_mat <- function(FUN, ...) {
    .check_mat(FUN=FUN, ..., cxxfun=cxx_test_numeric_access)
}

check_logical_mat <- function(FUN, ...) {
    .check_mat(FUN=FUN, ..., cxxfun=cxx_test_logical_access)
}

###############################

.check_slices <- function(FUN, ..., by.row, by.col, cxxfun) {
    for (x in by.row) {
        rx <- range(x)

        for (y in by.col) {
            ry <- range(y)
            test.mat <- FUN(...) 
            ref <- as.matrix(test.mat[x, y, drop=FALSE])
            dimnames(ref) <- NULL

            for (i in 1:2) {
                testthat::expect_identical(ref, .Call(cxxfun, test.mat, i, rx, ry))
            }
        }
    }
    return(invisible(NULL))
}

check_integer_slice <- function(FUN, ..., by.row, by.col) {
    .check_slices(FUN=FUN, ..., by.row=by.row, by.col=by.col, cxxfun=cxx_test_integer_slice)
}

check_character_slice <- function(FUN, ..., by.row, by.col) {
    .check_slices(FUN=FUN, ..., by.row=by.row, by.col=by.col, cxxfun=cxx_test_character_slice)
}

check_numeric_slice <- function(FUN, ..., by.row, by.col) {
    .check_slices(FUN=FUN, ..., by.row=by.row, by.col=by.col, cxxfun=cxx_test_numeric_slice)
}

check_logical_slice <- function(FUN, ..., by.row, by.col) {
    .check_slices(FUN=FUN, ..., by.row=by.row, by.col=by.col, cxxfun=cxx_test_logical_slice)
}

###############################

.check_const_mat <- function(FUN, ..., cxxfun) {
    test.mat <- FUN(...)
    ref <- as.matrix(test.mat)
    dimnames(ref) <- NULL
    testthat::expect_identical(ref, .Call(cxxfun, test.mat))
    return(invisible(NULL))
}

check_integer_const_mat <- function(FUN, ...) {
    .check_const_mat(FUN=FUN, ..., cxxfun=cxx_test_integer_const_access)
}

check_character_const_mat <- function(FUN, ...) {
    .check_const_mat(FUN=FUN, ..., cxxfun=cxx_test_character_const_access)
}

check_numeric_const_mat <- function(FUN, ...) {
    .check_const_mat(FUN=FUN, ..., cxxfun=cxx_test_numeric_const_access)
}

check_logical_const_mat <- function(FUN, ...) {
    .check_const_mat(FUN=FUN, ..., cxxfun=cxx_test_logical_const_access)
}

.check_const_slices <- function(FUN, ..., by.row, cxxfun) {
    for (x in by.row) {
        rx <- range(x)
        test.mat <- FUN(...)
        ref <- as.matrix(test.mat)
        dimnames(ref) <- NULL
        testthat::expect_identical(ref[x,,drop=FALSE], .Call(cxxfun, test.mat, rx))
    }
    return(invisible(NULL))
}

check_integer_const_slice <- function(FUN, ..., by.row) {
    .check_const_slices(FUN=FUN, ..., by.row=by.row, cxxfun=cxx_test_integer_const_slice)
}

check_character_const_slice <- function(FUN, ..., by.row) {
    .check_const_slices(FUN=FUN, ..., by.row=by.row, cxxfun=cxx_test_character_const_slice)
}

check_numeric_const_slice <- function(FUN, ..., by.row) {
    .check_const_slices(FUN=FUN, ..., by.row=by.row, cxxfun=cxx_test_numeric_const_slice)
}

check_logical_const_slice <- function(FUN, ..., by.row) {
    .check_const_slices(FUN=FUN, ..., by.row=by.row, cxxfun=cxx_test_logical_const_slice)
}

###############################

check_integer_nonzero_mat <- function(FUN, ...) {
    .check_mat(FUN=FUN, ..., cxxfun=cxx_test_integer_nonzero_access, nmodes=2)
}

check_numeric_nonzero_mat <- function(FUN, ...) {
    .check_mat(FUN=FUN, ..., cxxfun=cxx_test_numeric_nonzero_access, nmodes=2)
}

check_logical_nonzero_mat <- function(FUN, ...) {
    .check_mat(FUN=FUN, ..., cxxfun=cxx_test_logical_nonzero_access, nmodes=2)
}

check_integer_nonzero_slice <- function(FUN, ..., by.row, by.col) {
    .check_slices(FUN=FUN, ..., by.row=by.row, by.col=by.col, cxxfun=cxx_test_integer_nonzero_slice)
}

check_numeric_nonzero_slice <- function(FUN, ..., by.row, by.col) {
    .check_slices(FUN=FUN, ..., by.row=by.row, by.col=by.col, cxxfun=cxx_test_numeric_nonzero_slice)
}

check_logical_nonzero_slice <- function(FUN, ..., by.row, by.col) {
    .check_slices(FUN=FUN, ..., by.row=by.row, by.col=by.col, cxxfun=cxx_test_logical_nonzero_slice)
}

###############################

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

###############################

.check_edge_errors <- function(x, cxxfun) {
    testthat::expect_true(.Call(cxxfun, x, 0L))
    testthat::expect_error(.Call(cxxfun, x, 1L), "row index out of range")
    testthat::expect_error(.Call(cxxfun, x, -1L), "column index out of range")
    testthat::expect_error(.Call(cxxfun, x, 2L), "column start index is greater than column end index")
    testthat::expect_error(.Call(cxxfun, x, -2L), "row start index is greater than row end index")
    testthat::expect_error(.Call(cxxfun, x, 3L), "column end index out of range")
    testthat::expect_error(.Call(cxxfun, x, -3L), "row end index out of range")
    return(invisible(NULL))
}

check_integer_edge_errors <- function(FUN, ...) {
    .check_edge_errors(FUN(...), cxxfun=cxx_test_integer_edge)
}

check_logical_edge_errors <- function(FUN, ...) {
    .check_edge_errors(FUN(...), cxxfun=cxx_test_logical_edge)
}

check_numeric_edge_errors <- function(FUN, ...) {
    .check_edge_errors(FUN(...), cxxfun=cxx_test_numeric_edge)
}

check_character_edge_errors <- function(FUN, ...) {
    .check_edge_errors(FUN(...), cxxfun=cxx_test_character_edge)
}
