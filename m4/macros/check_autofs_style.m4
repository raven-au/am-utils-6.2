dnl ######################################################################
dnl check the autofs flavor
AC_DEFUN(AC_CHECK_AUTOFS_STYLE,
[
AC_CACHE_CHECK(autofs style,
ac_cv_autofs_style,
[
# select the correct style to mount(2) a filesystem
case "${host_os}" in
       solaris2.5* )
               ac_cv_autofs_style=solaris_v1 ;;
       # Solaris 8 uses the AutoFS V3 protocol, but it's very similar to V2,
       # so use one style for both.
       solaris2.6* | solaris2.7* | solaris2.8* )
               ac_cv_autofs_style=solaris_v2_v3 ;;
#       irix* )
#	       ac_cv_autofs_style=solaris_v1 ;;
       linux* )
               ac_cv_autofs_style=linux ;;
       * )
               ac_cv_autofs_style=default ;;
esac
])
# always make a link and include the file name, otherwise on systems where
# autofs support has not been ported yet check_fs_{headers, mntent}.m4 add
# ops_autofs.o to AMD_FS_OBJS, but there's no way to build it.
am_utils_autofs_style=$srcdir"/conf/autofs/autofs_"$ac_cv_autofs_style".h"
am_utils_link_files_src=${am_utils_link_files_src}conf/autofs/autofs_${ac_cv_autofs_style}.c" "
am_utils_link_files_dst=${am_utils_link_files_dst}amd/ops_autofs.c" "
AC_SUBST_FILE(am_utils_autofs_style)
])
dnl ======================================================================
