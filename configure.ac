AC_INIT("DESCRIPTION")

# Using same compilation settings as used for R.
: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
    echo "could not determine R_HOME"
    exit 1
fi
CC=`"${R_HOME}/bin/R" CMD config CC`
CFLAGS=`"${R_HOME}/bin/R" CMD config CFLAGS`
CPPFLAGS=`"${R_HOME}/bin/R" CMD config CPPFLAGS`
CXX=`"${R_HOME}/bin/R" CMD config CXX`
CXXFLAGS=`"${R_HOME}/bin/R" CMD config CXXFLAGS`
LDFLAGS=`"${R_HOME}/bin/R" CMD config LDFLAGS`
AC_LANG(C++)

# Checking that the HDF5 header exists.
AC_CHECK_HEADER(H5Cpp.h, [use_hdf=yes], [use_hdf=no])
if test "$use_hdf" = yes; then
    DEF_HDF_CPP="#define BEACHMAT_USE_HDF5"
    DEF_HDF_R="TRUE"

    # Checking for HDF5-linkable libraries.
    SAVED_LIBS=$LIBS
    LIBS="-lhdf5_cpp $LIBS"
    AC_LINK_IFELSE(
        [AC_LANG_PROGRAM([#include <H5Cpp.h>], 
                         [H5::H5File dummy])],
        [], 
        [AC_MSG_ERROR([unable to find libhdf5_cpp])]
    )
    LIBS=$SAVED_LIBS
    
    PKG_LIBS=-lhdf5_cpp
else
    DEF_HDF_R="FALSE"
    DEF_HDF_CPP=
    PKG_LIBS=    
fi

AC_SUBST([DEF_HDF_R])
AC_SUBST([DEF_HDF_CPP])
AC_SUBST([PKG_LIBS])
AC_OUTPUT([src/Makevars src/beachmat.h:src/beachmat.in R/use_hdf.R:R/use_hdf.in])
