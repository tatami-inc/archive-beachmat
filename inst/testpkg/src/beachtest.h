#include "Rcpp.h"
#include "beachmat/numeric_matrix.h"
#include "beachmat/integer_matrix.h"
#include "beachmat/logical_matrix.h"
#include "beachmat/character_matrix.h"

extern "C" { 

// Input functions.

SEXP test_numeric_access (SEXP, SEXP);

SEXP test_integer_access (SEXP, SEXP);

SEXP test_logical_access (SEXP, SEXP);

SEXP test_character_access (SEXP, SEXP);

SEXP test_numeric_slice (SEXP, SEXP, SEXP, SEXP);

SEXP test_integer_slice (SEXP, SEXP, SEXP, SEXP);

SEXP test_logical_slice (SEXP, SEXP, SEXP, SEXP);

SEXP test_character_slice (SEXP, SEXP, SEXP, SEXP);

SEXP test_sparse_numeric(SEXP, SEXP);

SEXP test_sparse_numeric_slice(SEXP, SEXP);

SEXP test_type_check(SEXP);

// Output functions.

SEXP test_integer_output(SEXP, SEXP);

SEXP test_integer_output_slice(SEXP, SEXP, SEXP, SEXP);

SEXP test_numeric_output(SEXP, SEXP);

SEXP test_numeric_output_slice(SEXP, SEXP, SEXP, SEXP);

SEXP test_logical_output(SEXP, SEXP);

SEXP test_logical_output_slice(SEXP, SEXP, SEXP, SEXP);

}


