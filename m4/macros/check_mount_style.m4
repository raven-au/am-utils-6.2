dnl ######################################################################
dnl check style of mounting filesystems
AC_DEFUN(AC_CHECK_MOUNT_STYLE,
[
AC_CACHE_CHECK(style of mounting filesystems,
ac_cv_style_mount,
[
# select the correct style for mounting filesystems
case "${host_os_name}" in
	svr4* | sysv4* | solaris2* | sunos5* | aoi* | hpux1[[12]]* )
			ac_cv_style_mount=svr4 ;;
	bsdi3* | bsdi4* )
			ac_cv_style_mount=bsdi3 ;;
	aix* )
			ac_cv_style_mount=aix ;;
	hpux* )
			ac_cv_style_mount=hpux ;;
	irix6* )
			ac_cv_style_mount=irix6 ;;
	irix5* )
			ac_cv_style_mount=irix5 ;;
	isc3* )
			ac_cv_style_mount=isc3 ;;
	linux* )
			ac_cv_style_mount=linux ;;
	mach3* )
			ac_cv_style_mount=mach3 ;;
	stellix* )
			ac_cv_style_mount=stellix ;;
	* )	# no style needed.  Use default filesystem calls ala BSD
			ac_cv_style_mount=default ;;
esac
])
am_utils_mount_style_file="mountutil.c"
am_utils_link_files=${am_utils_link_files}libamu/${am_utils_mount_style_file}:conf/mount/mount_${ac_cv_style_mount}.c" "
# append mount utilities object to LIBOBJS for automatic compilation
LIBOBJS="$LIBOBJS mountutil.o"
])
dnl ======================================================================
