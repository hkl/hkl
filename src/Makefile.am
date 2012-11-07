AM_CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -std=c99 -D_POSIX_SOURCE -O0 -g
AM_LDFLAGS =
AM_YFLAGS = -d -v
AM_LFLAGS =

bin_PROGRAMS = hkl testbed

hkl_SOURCES = y.tab.y lex.yy.l hkl_string.c hkl_hash.c hkl_tree.c hkl.c \
hkl_deque.c hklr.c hklr_object.c hklr_expression.c hklr_statement.c \
hkl_value.c hkl_list.c \
expression/hklr_op_size.c \
expression/hklr_op_plus.c \
expression/hklr_op_equal.c \
statement/hklr_statement_puts.c \
statement/hklr_statement_assign.c

testbed_SOURCES = hkl_tree.c hkl_string.c hkl_test.c hklr.c hklr_object.c \
hkl_deque.c hkl_hash.c hkl_list.c hkl_value.c test/hash.c test/traversal.c
