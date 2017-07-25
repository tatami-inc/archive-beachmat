#include "R_ext/Rdynload.h"
#include "R_ext/Visibility.h"
#include "beachtest.h"

#define REGISTER(x, i) {#x, (DL_FUNC) &x, i}

extern "C" {

static const R_CallMethodDef all_call_entries[] = {
    // Standard access.
    REGISTER(test_numeric_access, 3),
    REGISTER(test_integer_access, 3),
    REGISTER(test_logical_access, 3),
    REGISTER(test_character_access, 3),

    REGISTER(test_numeric_slice, 4),
    REGISTER(test_integer_slice, 4),
    REGISTER(test_logical_slice, 4),
    REGISTER(test_character_slice, 4),

    // Const access.
    REGISTER(test_numeric_const_access, 1),
    REGISTER(test_integer_const_access, 1),
    REGISTER(test_logical_const_access, 1),
    REGISTER(test_character_const_access, 1),

    REGISTER(test_numeric_const_slice, 2),
    REGISTER(test_integer_const_slice, 2),
    REGISTER(test_logical_const_slice, 2),
    REGISTER(test_character_const_slice, 2),

    // Non-zero access.
    REGISTER(test_numeric_nonzero_access, 2),
    REGISTER(test_integer_nonzero_access, 2),
    REGISTER(test_logical_nonzero_access, 2),

    REGISTER(test_numeric_nonzero_slice, 4),
    REGISTER(test_integer_nonzero_slice, 4),
    REGISTER(test_logical_nonzero_slice, 4),

    // Sparse access.
    REGISTER(test_sparse_numeric_slice, 2),

    // Type checks.
    REGISTER(test_type_check, 1),
    REGISTER(test_numeric_to_logical, 2), 
    REGISTER(test_numeric_to_integer, 2),
    REGISTER(test_integer_to_logical, 2),
    REGISTER(test_integer_to_numeric, 2),
    REGISTER(test_logical_to_integer, 2),
    REGISTER(test_logical_to_numeric, 2),

    // Edge cases.
    REGISTER(test_integer_edge, 2),
    REGISTER(test_numeric_edge, 2),
    REGISTER(test_logical_edge, 2),
    REGISTER(test_character_edge, 2),

    // Output tests.
    REGISTER(test_integer_output, 3),
    REGISTER(test_numeric_output, 3),
    REGISTER(test_logical_output, 3),
    REGISTER(test_character_output, 3),

    REGISTER(test_integer_output_slice, 4),
    REGISTER(test_numeric_output_slice, 4),
    REGISTER(test_logical_output_slice, 4),
    REGISTER(test_character_output_slice, 4),

    REGISTER(test_sparse_numeric_output, 3),
    REGISTER(test_sparse_numeric_output_slice, 4),
    REGISTER(test_sparse_logical_output, 3),
    REGISTER(test_sparse_logical_output_slice, 4),

    // Output type tests.
    REGISTER(test_numeric_to_logical_output, 2), 
    REGISTER(test_numeric_to_integer_output, 2),
    REGISTER(test_integer_to_logical_output, 2),
    REGISTER(test_integer_to_numeric_output, 2),
    REGISTER(test_logical_to_integer_output, 2),
    REGISTER(test_logical_to_numeric_output, 2),

    // Output mode tests.
    REGISTER(select_output_by_sexp, 3),
    REGISTER(select_output_by_mode, 3),
    REGISTER(get_all_modes, 0),

    // Output edge cases.
    REGISTER(test_integer_edge_output, 2),
    REGISTER(test_numeric_edge_output, 2),
    REGISTER(test_logical_edge_output, 2),
    REGISTER(test_character_edge_output, 2),

    {NULL, NULL, 0}
};

void attribute_visible R_init_beachtest(DllInfo *dll) {
    R_registerRoutines(dll, NULL, all_call_entries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
    R_forceSymbols(dll, TRUE);
}

}

