
#ifdef HAVE_RPCSVC_MOUNT_H
# include <rpcsvc/mount.h>
#endif /* HAVE_RPCSVC_MOUNT_H */

#ifdef HAVE_MOUNT_H
# include <mount.h>
#endif /* HAVE_MOUNT_H */

#ifdef HAVE_NFS_NFS_GFS_H
# include <nfs/nfs_gfs.h>
#endif /* HAVE_NFS_NFS_GFS_H */

#ifdef HAVE_NFS_MOUNT_H
# include <nfs/mount.h>
#endif /* HAVE_NFS_MOUNT_H */

#ifdef HAVE_SYS_FS_NFS_CLNT_H
# include <sys/fs/nfs_clnt.h>
#endif /* HAVE_SYS_FS_NFS_CLNT_H */

# ifndef _LINUX_NFS_H
#  define _LINUX_NFS_H
# endif /* not _LINUX_NFS_H */
# ifndef _LINUX_NFS2_H
#  define _LINUX_NFS2_H
# endif /* not _LINUX_NFS2_H */
# ifndef _LINUX_NFS3_H
#  define _LINUX_NFS3_H
# endif /* not _LINUX_NFS3_H */
# ifndef _LINUX_NFS_FS_H
#  define _LINUX_NFS_FS_H
# endif /* not _LINUX_NFS_FS_H */
# ifndef _LINUX_IN_H
#  define _LINUX_IN_H
# endif /* not _LINUX_IN_H */
# ifndef __KERNEL__
#  define __KERNEL__
# endif /* __KERNEL__ */
# include <linux/nfs_mount.h>
# undef __KERNEL__
#endif /* HAVE_LINUX_NFS_MOUNT_H */
