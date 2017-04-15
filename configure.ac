AC_INIT("DESCRIPTION")

AC_LANG(C++)
AC_CHECK_HEADER(H5Cpp.h, [use_hdf=yes], [use_hdf=no])

if test "$use_hdf" = yes; then
    PKG_CPPFLAGS=-DUSE_HDF

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
    PKG_CPPFLAGS=
    PKG_LIBS=    
fi

AC_SUBST(PKG_CPPFLAGS)
AC_SUBST(PKG_LIBS)
AC_OUTPUT(src/Makevars)       
