dnl ######################################################################
dnl check for external definition for a function (not external variables)
dnl Usage AC_CHECK_EXTERN(extern)
dnl Checks for external definition for "extern" that is delimited on the
dnl left and the right by a character that is not a valid symbol character.
dnl
dnl Note that $pattern below is very carefully crafted to match any system
dnl external defintion, with __P posix prototypes, with or without an extern
dnl word, etc.  Think twice before changing this.
AC_DEFUN(AC_CHECK_EXTERN,
[
# store variable name for external definition
ac_upcase_extern_name=`echo $1 | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
ac_safe=HAVE_EXTERN_$ac_upcase_extern_name
# check for cached value and set it if needed
AC_CACHE_CHECK_DYNAMIC(external function definition for $1,
ac_cv_extern_$1,
[
changequote(<<, >>)dnl
# the old pattern assumed that the complete external definition is on one
# line but on some systems it is split over several lines, so only match
# beginning of the extern definition including the opening parenthesis.
#pattern="(extern)?.*[^a-zA-Z0-9_]$1[^a-zA-Z0-9_]?.*\(.*\).*;"
pattern="(extern)?.*[^a-zA-Z0-9_]$1[^a-zA-Z0-9_]?.*\("
changequote([, ])dnl
AC_EGREP_CPP(${pattern},
[
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */
#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif /* HAVE_SYS_WAIT_H */
#ifdef HAVE_STDIO_H
# include <stdio.h>
#endif /* HAVE_STDIO_H */
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif /* HAVE_STDLIB_H */
#if HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */
#if HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */
#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif /* HAVE_STRINGS_H */
#ifdef HAVE_NETDB_H
# include <netdb.h>
#endif /* HAVE_NETDB_H */
#ifdef HAVE_CLUSTER_H
# include <cluster.h>
#endif /* HAVE_CLUSTER_H */
#ifdef HAVE_RPC_RPC_H
/*
 * Turn on PORTMAP, so that additional header files would get included
 * and the important definition for UDPMSGSIZE is included too.
 */
# ifndef PORTMAP
#  define PORTMAP
# endif /* not PORTMAP */
# include <rpc/rpc.h>
# ifndef XDRPROC_T_TYPE
typedef bool_t (*xdrproc_t) __P ((XDR *, __ptr_t, ...));
# endif /* not XDRPROC_T_TYPE */
#endif /* HAVE_RPC_RPC_H */

], eval "ac_cv_extern_$1=yes", eval "ac_cv_extern_$1=no")
])
# check if need to define variable
if test "`eval echo '$''{ac_cv_extern_'$1'}'`" = yes
then
  AC_DEFINE_UNQUOTED($ac_safe)
fi
])
dnl ======================================================================
