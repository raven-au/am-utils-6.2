dnl ######################################################################
dnl check if the mount table is kept in a file or in the kernel.
AC_DEFUN(AC_CHECK_MNTTAB_LOCATION,
[
AC_CACHE_CHECK_DYNAMIC(where mount table is kept,
ac_cv_mnttab_location,
[
# assume location is on file
ac_cv_mnttab_location=file
AC_CHECK_FUNCS(mntctl getmntinfo getmountent,
ac_cv_mnttab_location=kernel)
])
if test "$ac_cv_mnttab_location" = file
then
 AC_DEFINE(MOUNT_TABLE_ON_FILE)
fi
])
dnl ======================================================================
