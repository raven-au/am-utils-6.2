dnl ######################################################################
dnl check for type of rpcvers_t (usually in <rpc/types.h>)
AC_DEFUN([AMU_TYPE_RPCVERS_T],
[AC_CHECK_TYPE(rpcvers_t, unsigned long)])
dnl ======================================================================
