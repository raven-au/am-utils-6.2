dnl ######################################################################
dnl Compile a program with NFS headers to try and find a feature.
dnl The headers part are fixed.  Only three arguments are allowed:
dnl [$1] is the program to compile (2nd arg to AC_TRY_COMPILE)
dnl [$2] action to take if the program compiled (3rd arg to AC_TRY_COMPILE)
dnl [$3] action to take if program did not compile (4rd arg to AC_TRY_COMPILE)
AC_DEFUN(AC_TRY_COMPILE_NFS,
[# try to compile a program which may have a definition for a structure
AC_TRY_COMPILE(
[
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */
#ifdef HAVE_SYS_ERRNO_H
# include <sys/errno.h>
#endif /* HAVE_SYS_ERRNO_H */
#ifdef HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif /* HAVE_SYS_PARAM_H */
#ifdef HAVE_SYS_UCRED_H
# include <sys/ucred.h>
#endif /* HAVE_SYS_UCRED_H */

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else /* not TIME_WITH_SYS_TIME */
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else /* not HAVE_SYS_TIME_H */
#  include <time.h>
# endif /* not HAVE_SYS_TIME_H */
#endif /* not TIME_WITH_SYS_TIME */

#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif /* HAVE_NETINET_IN_H */

/*
 * Some operating systems must define these variables to get
 * NFS definitions included.
 */
#ifndef NFSCLIENT
# define NFSCLIENT
#endif /* NFSCLIENT */
#ifndef NFS
# define NFS
#endif /* NFS */

#ifdef HAVE_SYS_MOUNT_H
# ifndef NFSCLIENT
#  define NFSCLIENT
# endif /* not NFSCLIENT */
# ifndef PCFS
#  define PCFS
# endif /* not PCFS */
# ifndef LOFS
#  define LOFS
# endif /* not LOFS */
# ifndef RFS
#  define RFS
# endif /* not RFS */
# ifndef MSDOSFS
#  define MSDOSFS
# endif /* not MSDOSFS */
# ifndef MFS
#  define MFS
# endif /* not MFS */
# ifndef CD9660
#  define CD9660
# endif /* not CD9660 */
# ifndef NFS
#  define NFS
# endif /* not NFS */
# include <sys/mount.h>
#endif /* HAVE_SYS_MOUNT_H */

#ifdef HAVE_SYS_VMOUNT_H
# include <sys/vmount.h>
#endif /* HAVE_SYS_VMOUNT_H */

#ifdef HAVE_LINUX_FS_H
/*
 * There's a conflict of definitions on redhat alpha linux between
 * <netinet/in.h> and <linux/fs.h>.
 * Also a conflict in definitions of ntohl/htonl in RH-5.1 sparc64
 * between <netinet/in.h> and <linux/byteorder/generic.h> (2.1 kernels).
 */
# ifdef HAVE_SOCKETBITS_H
#  define _LINUX_SOCKET_H
#  undef BLKFLSBUF
#  undef BLKGETSIZE
#  undef BLKRAGET
#  undef BLKRASET
#  undef BLKROGET
#  undef BLKROSET
#  undef BLKRRPART
#  undef MS_MGC_VAL
#  undef MS_RMT_MASK
# endif /* HAVE_SOCKETBITS_H */
# ifdef HAVE_LINUX_POSIX_TYPES_H
#  include <linux/posix_types.h>
# endif /* HAVE_LINUX_POSIX_TYPES_H */
# ifndef _LINUX_BYTEORDER_GENERIC_H
#  define _LINUX_BYTEORDER_GENERIC_H
# endif /* _LINUX_BYTEORDER_GENERIC_H */
# include <linux/fs.h>
#endif /* HAVE_LINUX_FS_H */

#ifdef HAVE_NFS_MOUNT_H
# include <nfs/mount.h>
#endif /* HAVE_NFS_MOUNT_H */

#ifdef HAVE_RPC_RPC_H
# include <rpc/rpc.h>
#endif /* HAVE_RPC_RPC_H */

#ifdef HAVE_RPC_TYPES_H
# include <rpc/types.h>
#endif /* HAVE_RPC_TYPES_H */

/* Prevent multiple inclusion on Ultrix 4 */
#if defined(HAVE_RPC_XDR_H) && !defined(__XDR_HEADER__)
# include <rpc/xdr.h>
#endif /* defined(HAVE_RPC_XDR_H) && !defined(__XDR_HEADER__) */

/* ALWAYS INCLUDE AM-UTILS' SPECIFIC NFS PROTOCOL HEADER! */
#include AMU_NFS_PROTOCOL_HEADER

#ifdef HAVE_RPCSVC_MOUNT_H
# include <rpcsvc/mount.h>
#endif /* HAVE_RPCSVC_MOUNT_H */

#ifdef HAVE_SYS_FS_NFS_CLNT_H
# include <sys/fs/nfs_clnt.h>
#endif /* HAVE_SYS_FS_NFS_CLNT_H */

#ifdef HAVE_LINUX_NFS_MOUNT_H
# include <linux/nfs_mount.h>
#endif /* HAVE_LINUX_NFS_MOUNT_H */
#ifdef HAVE_NFS_NFS_GFS_H
# include <nfs/nfs_gfs.h>
#endif /* HAVE_NFS_NFS_GFS_H */
#ifdef HAVE_NFS_NFS_MOUNT_H_off
 /* broken on netxtep3 (includes non-existing headers) */
# include <nfs/nfs_mount.h>
#endif /* HAVE_NFS_NFS_MOUNT_H */
], [$1], [$2], [$3])
])
dnl ======================================================================
