dnl ######################################################################
dnl check the autofs flavor
AC_DEFUN(AC_CHECK_AUTOFS_STYLE,
[
AC_CACHE_CHECK(autofs style,
ac_cv_autofs_style,
[
# select the correct style to mount(2) a filesystem
case "${host_os_name}" in
       solaris2.5* )
               ac_cv_autofs_style=solaris1 ;;
       solaris2.[67]* )
               ac_cv_autofs_style=solaris2 ;;
#       solaris2.8* )
#	       ac_cv_autofs_style=solaris3 ;;
#       irix* )
#	       ac_cv_autofs_style=solaris1 ;;
       linux* )
               ac_cv_autofs_style=linux ;;
       * )
               ac_cv_autofs_style=default ;;
esac
])
# only make a link and include the file name if needed.
if test "$ac_cv_autofs_style" != default
then
  am_utils_autofs_style=$srcdir"/conf/autofs/autofs_"$ac_cv_autofs_style".h"
  am_utils_link_files_src=${am_utils_link_files_src}conf/autofs/autofs_${ac_cv_autofs_style}.c" "
  am_utils_link_files_dst=${am_utils_link_files_dst}amd/ops_autofs.c" "
else
  am_utils_autofs_style="/dev/null"
fi
AC_SUBST_FILE(am_utils_autofs_style)
])
dnl ======================================================================
