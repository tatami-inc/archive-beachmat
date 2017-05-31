#ifndef BEACHTEST_H
#define BEACHTEST_H

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

SEXP test_numeric_to_logical (SEXP, SEXP);

SEXP test_numeric_to_integer (SEXP, SEXP);

SEXP test_integer_to_logical (SEXP, SEXP);

SEXP test_integer_to_numeric (SEXP, SEXP);

SEXP test_logical_to_integer (SEXP, SEXP);

SEXP test_logical_to_numeric (SEXP, SEXP);

SEXP test_integer_edge (SEXP, SEXP);

SEXP test_logical_edge (SEXP, SEXP);

SEXP test_numeric_edge (SEXP, SEXP);

SEXP test_character_edge (SEXP, SEXP);

// Output functions.

SEXP test_integer_output(SEXP, SEXP);

SEXP test_integer_output_slice(SEXP, SEXP, SEXP, SEXP);

SEXP test_numeric_output(SEXP, SEXP);

SEXP test_numeric_output_slice(SEXP, SEXP, SEXP, SEXP);

SEXP test_logical_output(SEXP, SEXP);

SEXP test_logical_output_slice(SEXP, SEXP, SEXP, SEXP);

SEXP test_numeric_to_logical_output (SEXP, SEXP);

SEXP test_numeric_to_integer_output (SEXP, SEXP);

SEXP test_integer_to_logical_output (SEXP, SEXP);

SEXP test_integer_to_numeric_output (SEXP, SEXP);

SEXP test_logical_to_integer_output (SEXP, SEXP);

SEXP test_logical_to_numeric_output (SEXP, SEXP);

SEXP test_integer_edge_output (SEXP, SEXP);

SEXP test_logical_edge_output (SEXP, SEXP);

SEXP test_numeric_edge_output (SEXP, SEXP);

SEXP test_sparse_numeric_output (SEXP, SEXP, SEXP);

}

#endif
