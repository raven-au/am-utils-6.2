dnl ######################################################################
dnl Initial settings for LIBS (-l options)
dnl NOTE: this is for configuration as well as compilations!
AC_DEFUN(AC_OPT_LIBS,
[AC_MSG_CHECKING(for configuration/compilation (-l) library flags)
AC_ARG_ENABLE(libs,
[  --enable-libs[=ARG]     configure/compile with ARG (-l) library flags],
[if test "$enableval" = ""; then
  # ignore if empty
  AC_MSG_RESULT(none)
else
  # use supplied options
  LIBS="$LIBS $enableval"
  export LIBS
  AC_MSG_RESULT($enableval)
fi], [
  # default is to have no additional flags
  AC_MSG_RESULT(none)
])
])
dnl ======================================================================
