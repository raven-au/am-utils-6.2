dnl ######################################################################
dnl check the correct way to dereference the address part of the nfs fhandle
AC_DEFUN(AC_CHECK_NFS_FH_DREF,
[
AC_CACHE_CHECK(nfs file-handle address dereferencing style,
ac_cv_nfs_fh_dref_style,
[
# select the correct nfs address dereferencing style
case "${host_os}" in
	svr4* | sysv4* |solaris2* | sunos5* | hpux11* )
		ac_cv_nfs_fh_dref_style=svr4 ;;
	sunos4* )
		ac_cv_nfs_fh_dref_style=sunos4 ;;
	sunos3* )
		ac_cv_nfs_fh_dref_style=sunos3 ;;
changequote(<<, >>)dnl
	# bsdi3, freebsd-2.2, netbsd, etc. changed the type of the
	# filehandle in nfs_args from nfsv2fh_t to u_char.
	freebsd2.[2-9]* | freebsd[3-4]* | bsdi[3-4]* | netbsd* | openbsd* )
		ac_cv_nfs_fh_dref_style=freebsd22 ;;
	aix4.[2-9]* )
		ac_cv_nfs_fh_dref_style=aix42 ;;
changequote([, ])dnl
	bsd44* | bsdi2* | freebsd*  )
		ac_cv_nfs_fh_dref_style=bsd44 ;;
	hpux* )
		ac_cv_nfs_fh_dref_style=hpux ;;
	irix* )
		ac_cv_nfs_fh_dref_style=irix ;;
	linux* )
		ac_cv_nfs_fh_dref_style=linux ;;
	aix* )
		ac_cv_nfs_fh_dref_style=aix3 ;;
	isc3 )
		ac_cv_nfs_fh_dref_style=isc3 ;;
	osf4* )
		ac_cv_nfs_fh_dref_style=osf4 ;;
	osf* )
		ac_cv_nfs_fh_dref_style=osf2 ;;
	nextstep* )
		ac_cv_nfs_fh_dref_style=nextstep ;;
	* )
		ac_cv_nfs_fh_dref_style=default ;;
esac
])
am_utils_nfs_fh_dref=$srcdir"/conf/fh_dref/fh_dref_"$ac_cv_nfs_fh_dref_style".h"
AC_SUBST_FILE(am_utils_nfs_fh_dref)
])
dnl ======================================================================
