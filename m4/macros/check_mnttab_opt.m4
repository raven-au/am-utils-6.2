dnl ######################################################################
dnl check the string type of the name of a filesystem mount table entry
dnl option.
dnl Usage: AC_CHECK_MNTTAB_OPT(<fs>)
dnl Check if there is an entry for MNTOPT_<fs> in sys/mntent.h or mntent.h
dnl define MNTTAB_OPT_<fs> to the string name (e.g., "ro").
AC_DEFUN(AC_CHECK_MNTTAB_OPT,
[
# what name to give to the fs
ac_fs_name=$1
# store variable name of fs
ac_upcase_fs_name=`echo $ac_fs_name | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
ac_safe=MNTTAB_OPT_$ac_upcase_fs_name
# check for cache and set it if needed
AC_CACHE_CHECK_DYNAMIC(for mount table option $ac_fs_name,
ac_cv_mnttab_opt_$ac_fs_name,
[
# undefine by default
eval "ac_cv_mnttab_opt_$ac_fs_name=notfound"
# and look to see if it was found
AC_EXPAND_CPP_STRING(
AC_MOUNT_HEADERS
, MNTOPT_$ac_upcase_fs_name)
# set cache variable to value
if test "${value}" != notfound
then
  eval "ac_cv_mnttab_opt_$ac_fs_name=\\\"$value\\\""
else
  eval "ac_cv_mnttab_opt_$ac_fs_name=$value"
fi
])
# outside cache check, if ok, define macro
ac_tmp=`eval echo '$''{ac_cv_mnttab_opt_'$ac_fs_name'}'`
if test "${ac_tmp}" != notfound
then
  AC_DEFINE_UNQUOTED($ac_safe, $ac_tmp)
fi
])
dnl ======================================================================
