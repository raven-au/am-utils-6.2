dnl ######################################################################
dnl Do you want to turn on "amq -M" code (security hole for IP sproofing!)
AC_DEFUN(AC_OPT_AMQ_MOUNT,
[AC_MSG_CHECKING(for amq -M remote mount code)
AC_ARG_ENABLE(amq-mount,
[  --enable-amq-mount      enable amq -M remote mount code],
[if test "$enableval" = yes; then
  AC_MSG_RESULT(yes)
  AC_DEFINE(ENABLE_AMQ_MOUNT)
else
  AC_MSG_RESULT(no)
fi], [
  # default is to not include this code because it is insecure
  AC_MSG_RESULT(no)
])])
dnl ======================================================================
