dnl ######################################################################
dnl Find if struct nfs_mount_data exists anywhere in typical headers
AC_DEFUN(AC_STRUCT_NFS_MOUNT_DATA,
[
dnl make sure this is called before macros which depend on it
AC_BEFORE([$0], [AC_TYPE_NFS_FH])
AC_BEFORE([$0], [AC_STRUCT_FIELD_NFS_FH])
AC_CACHE_CHECK(for struct nfs_mount_data,
ac_cv_have_struct_nfs_mount_data,
[
# try to compile a program which may have a definition for the structure
AC_TRY_COMPILE_NFS(
[ struct nfs_mount_data nmd;
], ac_cv_have_struct_nfs_mount_data=yes, ac_cv_have_struct_nfs_mount_data=no)
])
if test "$ac_cv_have_struct_nfs_mount_data" = yes
then
  AC_DEFINE(HAVE_STRUCT_NFS_MOUNT_DATA)
  AC_DEFINE(nfs_args_t, struct nfs_mount_data)
fi
])
dnl ======================================================================
