AC_PREREQ([2.69])

AC_INIT([c-cgi-framework], [v0.0.0], [b.heden@gmail.com])

AC_PROG_CC

AC_CHECK_HEADERS([stdio.h stdlib.h string.h stdarg.h])

AC_TYPE_SIZE_T

AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([ setenv getenv strcmp strcasecmp strdup snprintf])

AC_ARG_ENABLE(
    [testing],
    AC_HELP_STRING([--enable-testing],[Enable testing]),
    [CFLAGS="${CFLAGS} -O0 -ggdb3 -g -fprofile-arcs -ftest-coverage"]
)

AC_OUTPUT([
    Makefile
    lib/Makefile
    cgis/Makefile
    test/Makefile
])
