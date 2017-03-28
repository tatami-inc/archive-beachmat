#ifndef BEACHMAT_H
#define BEACHMAT_H

#include <vector>
#include <algorithm>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <sstream>

#include "R.h"
#include "Rinternals.h"
#include "R_ext/Rdynload.h"
#include "R_ext/Visibility.h"

extern "C" {

SEXP test_numeric_access(SEXP, SEXP);

SEXP test_integer_access(SEXP, SEXP);

SEXP test_logical_access(SEXP, SEXP);

}

#endif
