dnl ######################################################################
dnl check the correct type for the 3rd argument to yp_order()
AC_DEFUN(AC_TYPE_YP_ORDER_OUTORDER,
[
AC_CACHE_CHECK(pointer type of 3rd argument to yp_order(),
ac_cv_yp_order_outorder,
[
# select the correct type
case "${host_os}" in
changequote(<<, >>)dnl
	solaris2* | svr4* | sysv4* | sunos5* | hpux* | aix4.[3-9]* )
		ac_cv_yp_order_outorder="unsigned long" ;;
changequote([, ])dnl
	osf* )
		# DU4 man page is wrong, headers are right
		ac_cv_yp_order_outorder="unsigned int" ;;
	* )
		ac_cv_yp_order_outorder=int ;;
esac
])
AC_DEFINE_UNQUOTED(YP_ORDER_OUTORDER_TYPE, $ac_cv_yp_order_outorder)
])
dnl ======================================================================
