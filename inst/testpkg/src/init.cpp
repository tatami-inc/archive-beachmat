#include "R_ext/Rdynload.h"
#include "R_ext/Visibility.h"
#include "beachtest.h"

#define REGISTER(x, i) {#x, (DL_FUNC) &x, i}

extern "C" {

static const R_CallMethodDef all_call_entries[] = {
    REGISTER(test_numeric_access, 2),
    REGISTER(test_integer_access, 2),
    REGISTER(test_logical_access, 2),
    REGISTER(test_character_access, 2),

    REGISTER(test_numeric_slice, 4),
    REGISTER(test_integer_slice, 4),
    REGISTER(test_logical_slice, 4),
    REGISTER(test_character_slice, 4),

    REGISTER(test_sparse_numeric, 2),
    REGISTER(test_sparse_numeric_slice, 2),

    REGISTER(test_integer_output, 3),
    REGISTER(test_numeric_output, 3),
    REGISTER(test_logical_output, 3),
    REGISTER(test_integer_output_slice, 5),
    REGISTER(test_numeric_output_slice, 5),
    REGISTER(test_logical_output_slice, 5),

    REGISTER(test_type_check, 1),
    REGISTER(test_numeric_to_logical, 2), 
    REGISTER(test_numeric_to_integer, 2),
    REGISTER(test_integer_to_logical, 2),
    REGISTER(test_integer_to_numeric, 2),
    REGISTER(test_logical_to_integer, 2),
    REGISTER(test_logical_to_numeric, 2),

    {NULL, NULL, 0}
};

void attribute_visible R_init_beachtest(DllInfo *dll) {
    R_registerRoutines(dll, NULL, all_call_entries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
    R_forceSymbols(dll, TRUE);
}

}

