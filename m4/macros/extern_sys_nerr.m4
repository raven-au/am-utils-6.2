dnl ######################################################################
dnl find if "extern int sys_nerr" exists in headers
AC_DEFUN(AMU_EXTERN_SYS_NERR,
[
AC_CACHE_CHECK(if external definition for sys_nerr exists,
ac_cv_extern_sys_nerr,
[
# try to compile program that uses the variable
AC_TRY_COMPILE(
[
#ifdef HAVE_STDIO_H
# include <stdio.h>
#endif /* HAVE_STDIO_H */
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif /* HAVE_STDLIB_H */
#ifdef HAVE_SYS_ERRNO_H
# include <sys/errno.h>
#endif /* HAVE_SYS_ERRNO_H */
#ifdef HAVE_ERRNO_H
# include <errno.h>
#endif /* HAVE_ERRNO_H */
],
[
int i = sys_nerr;
], ac_cv_extern_sys_nerr=yes, ac_cv_extern_sys_nerr=no)
])
if test "$ac_cv_extern_sys_nerr" = yes
then
  AC_DEFINE(HAVE_EXTERN_SYS_NERR)
fi
])
dnl ======================================================================
