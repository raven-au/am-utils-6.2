dnl ######################################################################
dnl Initial settings for LDFLAGS (-L options)
dnl NOTE: this is for configuration as well as compilations!
AC_DEFUN(AC_OPT_LDFLAGS,
[AC_MSG_CHECKING(for configuration/compilation (-L) library flags)
AC_ARG_ENABLE(ldflags,
[  --enable-ldflags[=ARG]  configure/compile with ARG (-L) library flags],
[if test "$enableval" = ""; then
  # ignore if empty
  AC_MSG_RESULT(none)
else
  # use supplied options
  LDFLAGS="$LDFLAGS $enableval"
  export LDFLAGS
  AC_MSG_RESULT($enableval)
fi], [
  # default is to have no additional flags
  AC_MSG_RESULT(none)
])
])
dnl ======================================================================
