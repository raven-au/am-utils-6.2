dnl ######################################################################
dnl check style of unmounting filesystems
AC_DEFUN(AC_CHECK_UMOUNT_STYLE,
[
AC_CACHE_CHECK(style of unmounting filesystems,
ac_cv_style_umount,
[
# select the correct style for unmounting filesystems
case "${host_os_name}" in
	bsd44* | bsdi* | freebsd* | netbsd* | openbsd* | darwin* | rhapsody* )
			ac_cv_style_umount=bsd44 ;;
	osf* )
			ac_cv_style_umount=osf ;;
	* )
			ac_cv_style_umount=default ;;
esac
])
am_utils_umount_style_file="umount_fs.c"
am_utils_link_files=${am_utils_link_files}libamu/${am_utils_umount_style_file}:conf/umount/umount_${ac_cv_style_umount}.c" "
dnl XXX: remove the next two lines after porting to autoconf-2.14
am_utils_link_files_src=${am_utils_link_files_src}conf/umount/umount_${ac_cv_style_umount}.c" "
am_utils_link_files_dst=${am_utils_link_files_dst}libamu/${am_utils_umount_style_file}" "

# append un-mount utilities object to LIBOBJS for automatic compilation
LIBOBJS="$LIBOBJS umount_fs.o"
])
dnl ======================================================================
