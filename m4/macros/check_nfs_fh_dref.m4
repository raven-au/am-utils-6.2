dnl ######################################################################
dnl check the correct way to dereference the address part of the nfs fhandle
AC_DEFUN(AC_CHECK_NFS_FH_DREF,
[
AC_CACHE_CHECK(nfs file-handle address dereferencing style,
ac_cv_nfs_fh_dref_style,
[
# select the correct nfs address dereferencing style
case "${host_os}" in
	hpux9* | hpux10* )
		ac_cv_nfs_fh_dref_style=hpux ;;
	sunos3* )
		ac_cv_nfs_fh_dref_style=sunos3 ;;
	sunos4* | solaris1* )
		ac_cv_nfs_fh_dref_style=sunos4 ;;
	svr4* | sysv4* | solaris* | sunos* | hpux* )
		ac_cv_nfs_fh_dref_style=svr4 ;;
	bsd44* | bsdi2* )
		ac_cv_nfs_fh_dref_style=bsd44 ;;
	# bsdi3, freebsd-2.2, netbsd, etc. changed the type of the
	# filehandle in nfs_args from nfsv2fh_t to u_char.
	freebsd[[2-4]]* | freebsdelf[[2-4]]* | bsdi* | netbsd* | openbsd* )
		ac_cv_nfs_fh_dref_style=freebsd22 ;;
	# matches freebsd5 and newer
	freebsd* | freebsdelf* )
		ac_cv_nfs_fh_dref_style=bsd44 ;;
	aix3* | aix4.[[01]]* )
		ac_cv_nfs_fh_dref_style=aix3 ;;
	aix* )
		ac_cv_nfs_fh_dref_style=aix42 ;;
	irix* )
		ac_cv_nfs_fh_dref_style=irix ;;
	linux* )
		ac_cv_nfs_fh_dref_style=linux ;;
	isc3 )
		ac_cv_nfs_fh_dref_style=isc3 ;;
	osf2* )
		ac_cv_nfs_fh_dref_style=osf2 ;;
	osf* )
		ac_cv_nfs_fh_dref_style=osf4 ;;
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
