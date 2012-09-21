AM_CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -std=c99 -D_POSIX_SOURCE -O0 -g
AM_LDFLAGS =
AM_YFLAGS = -d -v
AM_LFLAGS =

bin_PROGRAMS = hkl testbed

hkl_SOURCES = y.tab.y lex.yy.l hkl_string.c hklc.c hkl_assert_stmt.c hkl_assign_stmt.c hkl_expression.c hkl_include_stmt.c hkl_puts_stmt.c hkl_return_stmt.c hkl_variable.c

testbed_SOURCES = hkl_string.c hkl_test.c hkl_hash.c hkl_tree.c
