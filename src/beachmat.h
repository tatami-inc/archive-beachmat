#ifndef BEACHMAT_H
#define BEACHMAT_H

#include "R.h"
#include "Rinternals.h"
#include "R_ext/Rdynload.h"
#include "R_ext/Visibility.h"

#include <vector>
#include <algorithm>
#include <cstring>
#include <memory>
#include <stdexcept>

extern "C" {

SEXP test_numeric_access(SEXP, SEXP);

}

#endif
