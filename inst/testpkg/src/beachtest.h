#ifndef BEACHTEST_H
#define BEACHTEST_H

#include "Rcpp.h"
#include "beachmat/numeric_matrix.h"
#include "beachmat/integer_matrix.h"
#include "beachmat/logical_matrix.h"
#include "beachmat/character_matrix.h"

extern "C" { 

// Standard access.

SEXP test_numeric_access (SEXP, SEXP, SEXP);

SEXP test_integer_access (SEXP, SEXP, SEXP);

SEXP test_logical_access (SEXP, SEXP, SEXP);

SEXP test_character_access (SEXP, SEXP, SEXP);

SEXP test_numeric_slice (SEXP, SEXP, SEXP, SEXP);

SEXP test_integer_slice (SEXP, SEXP, SEXP, SEXP);

SEXP test_logical_slice (SEXP, SEXP, SEXP, SEXP);

SEXP test_character_slice (SEXP, SEXP, SEXP, SEXP);

// Const access.

SEXP test_numeric_const_access (SEXP);

SEXP test_integer_const_access (SEXP);

SEXP test_logical_const_access (SEXP);

SEXP test_character_const_access (SEXP);

SEXP test_numeric_const_slice (SEXP, SEXP);

SEXP test_integer_const_slice (SEXP, SEXP);

SEXP test_logical_const_slice (SEXP, SEXP);

SEXP test_character_const_slice (SEXP, SEXP);

// Non-zero access.

SEXP test_numeric_nonzero_access (SEXP, SEXP);

SEXP test_integer_nonzero_access (SEXP, SEXP);

SEXP test_logical_nonzero_access (SEXP, SEXP);

SEXP test_numeric_nonzero_slice (SEXP, SEXP, SEXP,SEXP);
                                                 
SEXP test_integer_nonzero_slice (SEXP, SEXP, SEXP,SEXP);
                                                 
SEXP test_logical_nonzero_slice (SEXP, SEXP, SEXP,SEXP);

// Sparse access.

SEXP test_sparse_numeric_slice(SEXP, SEXP);

// Type checks.

SEXP test_type_check(SEXP);

SEXP test_numeric_to_logical (SEXP, SEXP);

SEXP test_numeric_to_integer (SEXP, SEXP);

SEXP test_integer_to_logical (SEXP, SEXP);

SEXP test_integer_to_numeric (SEXP, SEXP);

SEXP test_logical_to_integer (SEXP, SEXP);

SEXP test_logical_to_numeric (SEXP, SEXP);

// Edge cases.

SEXP test_integer_edge (SEXP, SEXP);

SEXP test_logical_edge (SEXP, SEXP);

SEXP test_numeric_edge (SEXP, SEXP);

SEXP test_character_edge (SEXP, SEXP);

// Output functions.

SEXP test_integer_output(SEXP, SEXP, SEXP);

SEXP test_integer_output_slice(SEXP, SEXP, SEXP, SEXP);

SEXP test_numeric_output(SEXP, SEXP, SEXP);

SEXP test_numeric_output_slice(SEXP, SEXP, SEXP, SEXP);

SEXP test_logical_output(SEXP, SEXP, SEXP);

SEXP test_logical_output_slice(SEXP, SEXP, SEXP, SEXP);

SEXP test_character_output(SEXP, SEXP, SEXP);

SEXP test_character_output_slice(SEXP, SEXP, SEXP, SEXP);

SEXP test_sparse_numeric_output (SEXP, SEXP, SEXP);

SEXP test_sparse_numeric_output_slice (SEXP, SEXP, SEXP, SEXP);

SEXP test_sparse_logical_output (SEXP, SEXP, SEXP);

SEXP test_sparse_logical_output_slice (SEXP, SEXP, SEXP, SEXP);

// Output type checks.

SEXP test_numeric_to_logical_output (SEXP, SEXP);

SEXP test_numeric_to_integer_output (SEXP, SEXP);

SEXP test_integer_to_logical_output (SEXP, SEXP);

SEXP test_integer_to_numeric_output (SEXP, SEXP);

SEXP test_logical_to_integer_output (SEXP, SEXP);

SEXP test_logical_to_numeric_output (SEXP, SEXP);

// Output mode checks.

SEXP select_output_by_sexp (SEXP, SEXP, SEXP);

SEXP select_output_by_mode (SEXP, SEXP, SEXP);

SEXP get_all_modes();
    
// Output edge cases.

SEXP test_integer_edge_output (SEXP, SEXP);

SEXP test_logical_edge_output (SEXP, SEXP);

SEXP test_numeric_edge_output (SEXP, SEXP);

SEXP test_character_edge_output (SEXP, SEXP);

}

#endif
