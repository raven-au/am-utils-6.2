dnl ######################################################################
dnl check for type of time_t (usually in <sys/types.h>)
AC_DEFUN(AC_TYPE_TIME_T,
[AC_CHECK_TYPE(time_t, long)])
dnl ======================================================================
