dnl ######################################################################
dnl New versions of the cache functions which also dynamically evaluate the
dnl cache-id field, so that it may contain shell variables to expand
dnl dynamically for the creation of $ac_cv_* variables on the fly.
dnl In addition, this function allows you to call COMMANDS which generate
dnl output on the command line, because it prints its own AC_MSG_CHECKING
dnl after COMMANDS are run.
dnl
dnl ======================================================================
dnl AC_CACHE_CHECK_DYNAMIC(MESSAGE, CACHE-ID, COMMANDS)
define(AC_CACHE_CHECK_DYNAMIC,
[
ac_tmp=`echo $2`
if eval "test \"`echo '$''{'$ac_tmp'+set}'`\" = set"; then
  AC_MSG_CHECKING([$1])
dnl XXX: use the next line for autoconf-2.14
dnl  echo $ECHO_N "(cached) $ECHO_C" 1>&AC_FD_MSG
  echo $ac_n "(cached) $ac_c" 1>&AC_FD_MSG
else
  $3
  AC_MSG_CHECKING([$1])
fi
ac_tmp_val=`eval eval "echo '$''{'$ac_tmp'}'"`
AC_MSG_RESULT($ac_tmp_val)
])
dnl ======================================================================
