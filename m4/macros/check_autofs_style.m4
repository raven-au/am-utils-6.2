dnl ######################################################################
dnl check the autofs flavor
AC_DEFUN(AC_CHECK_AUTOFS_STYLE,
[
AC_CACHE_CHECK(autofs style,
ac_cv_autofs_style,
[
# select the correct style to mount(2) a filesystem
case "${host_os_name}" in
       solaris2* )
               ac_cv_autofs_style=solaris2 ;;
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
else
  am_utils_autofs_style="/dev/null"
fi
AC_SUBST_FILE(am_utils_autofs_style)
])
dnl ======================================================================
