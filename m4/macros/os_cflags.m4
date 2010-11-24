dnl ######################################################################
dnl Specify additional compile options based on the OS and the compiler
AC_DEFUN([AMU_OS_CFLAGS],
[
AC_CACHE_CHECK(additional compiler flags,
ac_cv_os_cflags,
[
case "${host_os}" in
	irix6* )
		case "${CC}" in
			cc )
				# do not use 64-bit compiler
				ac_cv_os_cflags="-n32 -mips3 -Wl,-woff,84"
				;;
		esac
		;;
	osf[[1-3]]* )
		# get the right version of struct sockaddr
		case "${CC}" in
			cc )
				ac_cv_os_cflags="-std -D_SOCKADDR_LEN -D_NO_PROTO"
				;;
			* )
				ac_cv_os_cflags="-D_SOCKADDR_LEN -D_NO_PROTO"
				;;
		esac
		;;
	osf* )
		# get the right version of struct sockaddr
		case "${CC}" in
			cc )
				ac_cv_os_cflags="-std -D_SOCKADDR_LEN"
				;;
			* )
				ac_cv_os_cflags="-D_SOCKADDR_LEN"
				;;
		esac
		;;
	aix[[1-3]]* )
		ac_cv_os_cflags="" ;;
	aix4.[[0-2]]* )
		# turn on additional headers
		ac_cv_os_cflags="-D_XOPEN_EXTENDED_SOURCE"
		;;
	aix5.3* )
		# avoid circular dependencies in yp headers, and more
		ac_cv_os_cflags="-DHAVE_BAD_HEADERS -D_XOPEN_EXTENDED_SOURCE -D_USE_IRS -D_MSGQSUPPORT"
		;;
	aix* )
		# avoid circular dependencies in yp headers
		ac_cv_os_cflags="-DHAVE_BAD_HEADERS -D_XOPEN_EXTENDED_SOURCE -D_USE_IRS"
		;;
	OFF-sunos4* )
		# make sure passing whole structures is handled in gcc
		case "${CC}" in
			gcc )
				ac_cv_os_cflags="-fpcc-struct-return"
				;;
		esac
		;;
	sunos[[34]]* | solaris1* | solaris2.[[0-5]] | sunos5.[[0-5]] | solaris2.5.* | sunos5.5.* )
		ac_cv_os_cflags="" ;;
	solaris2* | sunos5* )
		# turn on 64-bit file offset interface
		case "${CC}" in
			* )
				ac_cv_os_cflags="-D_LARGEFILE64_SOURCE"
				;;
		esac
		;;
	hpux* )
		# use Ansi compiler on HPUX
		case "${CC}" in
			cc )
				ac_cv_os_cflags="-Ae"
				;;
		esac
		;;
	darwin* | macosx* | rhapsody* )
		ac_cv_os_cflags="-D_P1003_1B_VISIBLE"
		;;
	linux* )
		ac_cv_os_cflags="-D_GNU_SOURCE"
		;;
	* )
		ac_cv_os_cflags=""
		;;
esac
])
AMU_CFLAGS="$AMU_CFLAGS $ac_cv_os_cflags"
# use same flags for configuring, so it matches what we do at compile time
CFLAGS="$CFLAGS $ac_cv_os_cflags"
export CFLAGS
])
dnl ======================================================================
