dnl ######################################################################
dnl usage AC_CHECK_LIB_FUNCS(lib, func1 func2 ...)
dnl check if any of funcN exist in lib, and define/use that library
AC_DEFUN(AC_CHECK_LIB_FUNCS,
[
# make variable out of library name and functions
ac_safe=`echo ac_cv_have_lib_$1_$2 | tr ' ' '_'`
AC_CACHE_CHECK_DYNAMIC(libray $1 for functions: $2,
$ac_safe,
[
eval "$ac_safe=notfound"
# iterate over all functions
for ac_libfunc_tmp in $2
do
  AC_CHECK_LIB($1, $ac_libfunc_tmp, eval "$ac_safe=found", eval "$ac_safe=notfound")
  if test "`eval echo '$''{'$ac_safe'}'`" != notfound
  then
    break
  fi
done
])
# if cache value found, define HAVE_LIBlib and add it to $LIBS
if test "`eval echo '$''{'$ac_safe'}'`" != notfound
then
  changequote(<<, >>)dnl
  ac_tr_lib=HAVE_LIB`echo $1 | sed -e 's/[^a-zA-Z0-9_]/_/g' -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`
  changequote([, ])dnl
  AC_DEFINE_UNQUOTED($ac_tr_lib)
  LIBS="-l$1 $LIBS"
fi
])
dnl ======================================================================
