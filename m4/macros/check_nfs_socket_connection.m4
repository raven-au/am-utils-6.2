dnl ######################################################################
dnl check if need to turn on, off, or leave alone the NFS "noconn" option
AC_DEFUN(AC_CHECK_NFS_SOCKET_CONNECTION,
[
AC_CACHE_CHECK(if to turn on/off noconn option,
ac_cv_nfs_socket_connection,
[
# set default to no-change
ac_cv_nfs_socket_connection=none
# select the correct style
case "${host_os}" in
changequote(<<, >>)dnl
	openbsd2.[2-9]* | freebsd[3-4]* | freebsdelf[3-4]* )
			ac_cv_nfs_socket_connection=conn ;;
changequote([, ])dnl
	openbsd* )
			ac_cv_nfs_socket_connection=noconn ;;
esac
])
# set correct value
case "$ac_cv_nfs_socket_connection" in
	noconn )	AC_DEFINE(USE_UNCONNECTED_NFS_SOCKETS) ;;
	conn )		AC_DEFINE(USE_CONNECTED_NFS_SOCKETS) ;;
esac
])
dnl ======================================================================
