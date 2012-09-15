CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -std=c99 -D_POSIX_SOURCE -O3 -g
LDFLAGS =
AM_YFLAGS = -d -v
AM_LFLAGS =

bin_PROGRAMS = hklc hkl

hklc_SOURCES = y.tab.y lex.yy.l hkl_string.c hklc.c hkl_assert_stmt.c hkl_assign_stmt.c hkl_expression.c hkl_include_stmt.c hkl_puts_stmt.c hkl_return_stmt.c hkl_variable.c

hkl_SOURCES = hkl_string.c hkl_test.c
