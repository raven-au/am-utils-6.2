dnl ######################################################################
dnl check the correct type for the 6th argument to recvfrom()
AC_DEFUN(AC_TYPE_RECVFROM_FROMLEN,
[
AC_CACHE_CHECK(non-pointer type of 6th (fromlen) argument to recvfrom(),
ac_cv_recvfrom_fromlen,
[
# select the correct type
case "${host}" in
changequote(<<, >>)dnl
	*-aix4.* )
		ac_cv_recvfrom_fromlen="size_t" ;;
changequote([, ])dnl
	* )
		ac_cv_recvfrom_fromlen="int" ;;
esac
])
AC_DEFINE_UNQUOTED(RECVFROM_FROMLEN_TYPE, $ac_cv_recvfrom_fromlen)
])
dnl ======================================================================
