dnl ######################################################################
dnl Initial settings for CPPFLAGS (-I options)
dnl NOTE: this is for configuration as well as compilations!
AC_DEFUN(AC_OPT_CPPFLAGS,
[AC_MSG_CHECKING(for configuration/compilation (-I) preprocessor flags)
AC_ARG_ENABLE(cppflags,
[  --enable-cppflags[=ARG] configure/compile with ARG (-I) preprocessor flags],
[if test "$enableval" = ""; then
  # ignore if empty
  AC_MSG_RESULT(none)
else
  # use supplied options
  CPPFLAGS="$CPPFLAGS $enableval"
  export CPPFLAGS
  AC_MSG_RESULT($enableval)
fi], [
  # default is to have no additional flags
  AC_MSG_RESULT(none)
])
])
dnl ======================================================================
