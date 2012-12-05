AC_INIT([hkl], [0.0], [], [hkl], [http://github.com/hkl/hkl])


AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES([Makefile src/Makefile])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC

AC_CHECK_LIB(dl, dlopen, LIBDL="-ldl")

AC_OUTPUT

AM_PROG_CC_C_O
