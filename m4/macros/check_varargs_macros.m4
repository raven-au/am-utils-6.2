dnl ######################################################################
dnl check if CPP can handle variable-length argument macros
AC_DEFUN([AMU_VARARGS_MACROS],
[
AC_CACHE_CHECK(if pre-processor can handle variable-length macros,
ac_cv_varargs_macros,
[
# try C99 style
AC_PREPROC_IFELSE(
[
#define foo(format, ...)    bar(format, __VA_ARGS__)
], ac_cv_varargs_macros=c99,
# else try gcc style
AC_PREPROC_IFELSE(
[
#define foo(format, args...)   bar(format, args)
], ac_cv_varargs_macros=gcc, ac_cv_varargs_macros=none))
])
if test "$ac_cv_varargs_macros" = c99
then
  AC_DEFINE(HAVE_C99_VARARGS_MACROS, 1,
	 [System supports C99-style variable-length argument macros])
else
  if test "$ac_cv_varargs_macros" = gcc
  then
    AC_DEFINE(HAVE_GCC_VARARGS_MACROS, 1,
	 [System supports GCC-style variable-length argument macros])
  fi
fi
])
dnl ======================================================================
