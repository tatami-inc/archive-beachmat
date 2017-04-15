# Checks for proper execution of pkgconfig.

expect_output(pkgconfig('PKG_CPPFLAGS'))
expect_output(pkgconfig('PKG_LIBS'))

