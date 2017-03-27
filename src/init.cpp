#include "beachmat.h"
#define REGISTER(x, i) {#x, (DL_FUNC) &x, i}

extern "C" {

static const R_CallMethodDef all_call_entries[] = {
    REGISTER(test_numeric_access, 2),
    REGISTER(test_integer_access, 2),
    REGISTER(test_logical_access, 2),
    {NULL, NULL, 0}
};

void attribute_visible R_init_beachmat(DllInfo *dll) {
    R_registerRoutines(dll, NULL, all_call_entries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
    R_forceSymbols(dll, TRUE);
}

}

