dnl ######################################################################
dnl Which options to add to CFLAGS for compilation?
dnl NOTE: this is only for final compiltions, not for configure tests)
AC_DEFUN(AC_OPT_AMU_CFLAGS,
[AC_MSG_CHECKING(for additional C option compilation flags)
AC_ARG_ENABLE(am-cflags,
[  --enable-am-cflags[=ARG]
                          compile package with ARG additional C flags],
[if test "$enableval" = ""; then
  AMU_CFLAGS=""
  AC_SUBST(AMU_CFLAGS)
  AC_MSG_RESULT(none)
else
  # check if user supplied configure option without argument
  if test "$enableval" = "yes"; then
    AMU_CFLAGS=""
    AC_SUBST(AMU_CFLAGS)
    AC_MSG_RESULT(none)
  else
    # user supplied a cflags option to configure
    AMU_CFLAGS="$enableval"
    AC_SUBST(AMU_CFLAGS)
    AC_MSG_RESULT($enableval)
  fi
fi], [
  # default is to have no additional C flags
  AMU_CFLAGS=""
  AC_SUBST(AMU_CFLAGS)
  AC_MSG_RESULT(none)
])
])
dnl ======================================================================
