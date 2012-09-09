CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -std=c99 -D_POSIX_SOURCE -O3 -g
LDFLAGS =
AM_YFLAGS = -d -v
AM_LFLAGS =

bin_PROGRAMS = hklc hkl

hklc_SOURCES = y.tab.y lex.yy.l hkl_string.c hklc.c

hkl_SOURCES = hkl_string.c hkl_test.c
