dnl ######################################################################
dnl Find NFS-specific mount(2) options (hex numbers)
dnl Usage: AC_CHECK_MNT2_NFS_OPT(<fs>)
dnl Check if there is an entry for NFSMNT_<fs> in sys/mntent.h or
dnl mntent.h, then define MNT2_NFS_OPT_<fs> to the hex number.
AC_DEFUN(AC_CHECK_MNT2_NFS_OPT,
[
# what name to give to the fs
ac_fs_name=$1
# store variable name of fs
ac_upcase_fs_name=`echo $ac_fs_name | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
ac_safe=MNT2_NFS_OPT_$ac_upcase_fs_name
# check for cache and set it if needed
AC_CACHE_CHECK_DYNAMIC(for NFS-specific mount(2) option $ac_fs_name,
ac_cv_mnt2_nfs_opt_$ac_fs_name,
[
# undefine by default
eval "ac_cv_mnt2_nfs_opt_$ac_fs_name=notfound"
value=notfound

# first try NFSMNT_* (most systems)
if test "$value" = notfound
then
AC_EXPAND_CPP_HEX(
AC_MOUNT_HEADERS
, NFSMNT_$ac_upcase_fs_name)
fi

# next try NFS_MOUNT_* (linux)
if test "$value" = notfound
then
AC_EXPAND_CPP_HEX(
AC_MOUNT_HEADERS
, NFS_MOUNT_$ac_upcase_fs_name)
fi

# set cache variable to value
eval "ac_cv_mnt2_nfs_opt_$ac_fs_name=$value"
])
# outside cache check, if ok, define macro
ac_tmp=`eval echo '$''{ac_cv_mnt2_nfs_opt_'$ac_fs_name'}'`
if test "${ac_tmp}" != notfound
then
  AC_DEFINE_UNQUOTED($ac_safe, $ac_tmp)
fi
])
dnl ======================================================================
