dnl ######################################################################
dnl check the correct network transport type to use
AC_DEFUN(AC_CHECK_NETWORK_TRANSPORT_TYPE,
[
AC_CACHE_CHECK(network transport type,
ac_cv_transport_type,
[
# select the correct type
case "${host_os_name}" in
	solaris2* | sunos5* | hpux1[[12]]* )
		ac_cv_transport_type=tli ;;
	* )
		ac_cv_transport_type=sockets ;;
esac
])
am_utils_link_files=${am_utils_link_files}libamu/transputil.c:conf/transp/transp_${ac_cv_transport_type}.c" "
dnl XXX: remove the next two lines after porting to autoconf-2.14
am_utils_link_files_src=${am_utils_link_files_src}conf/transp/transp_${ac_cv_transport_type}.c" "
am_utils_link_files_dst=${am_utils_link_files_dst}libamu/transputil.c" "

# append transport utilities object to LIBOBJS for automatic compilation
LIBOBJS="$LIBOBJS transputil.o"
if test $ac_cv_transport_type = tli
then
  AC_DEFINE(HAVE_TRANSPORT_TYPE_TLI)
fi
])
dnl ======================================================================
