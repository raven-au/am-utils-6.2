dnl ######################################################################
dnl Do we want to compile with HESIOD support
AC_DEFUN(AMU_WITH_ADDON,
AC_MSG_CHECKING([if $1 is wanted])
[AC_ARG_WITH($1,
[  --with-$1		  enable `echo $1|tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'` support (by default, enabled)],
[
if test "$withval" = "yes"; then
  with_$1=yes
elif test "$withval" = "no"; then
  with_$1=no
else
  AC_MSG_ERROR(please use \"yes\" or \"no\" with --with-$1)
fi
],[
with_$1=yes
])
if test "$with_$1" = "yes"
then
  AC_MSG_RESULT([yes, will enable if all libraries are found])
else
  AC_MSG_RESULT([no])
fi
])

