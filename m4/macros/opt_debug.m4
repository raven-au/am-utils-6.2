dnl ######################################################################
dnl Debugging: "yes" means general, "mem" means general and memory debugging,
dnl and "no" means none.
AC_DEFUN(AC_OPT_DEBUG,
[AC_MSG_CHECKING(for debugging options)
AC_ARG_ENABLE(debug,
[  --enable-debug[=ARG]    enable debugging (yes/mem/no)],
[
if test "$enableval" = yes; then
  AC_MSG_RESULT(yes)
  AC_DEFINE(DEBUG)
elif test "$enableval" = mem; then
  AC_MSG_RESULT(mem)
  AC_DEFINE(DEBUG)
  AC_DEFINE(DEBUG_MEM)
  AC_CHECK_LIB(mapmalloc, malloc_verify)
  AC_CHECK_LIB(malloc, mallinfo)
else
  AC_MSG_RESULT(no)
fi
],
[
  # default is no debugging
  AC_MSG_RESULT(no)
])
])
dnl ======================================================================
