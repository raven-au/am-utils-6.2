#ifdef HAVE_FS_AUTOFS

struct autofs_pending_mount {
  unsigned long wait_queue_token;	/* Associated kernel wait token */
  char *name;
  struct autofs_pending_mount *next;
};

struct autofs_pending_umount {
  unsigned long wait_queue_token;	/* Associated kernel wait token */
  char *name;
  struct autofs_pending_umount *next;
};

typedef struct {
  int fd;
  int kernelfd;
  int ioctlfd;
  int version;
  struct autofs_pending_mount *pending_mounts;
  struct autofs_pending_umount *pending_umounts;
} autofs_fh_t;

#ifndef HAVE_LINUX_AUTO_FS4_H
union autofs_packet_union {
	struct autofs_packet_hdr hdr;
	struct autofs_packet_missing missing;
	struct autofs_packet_expire expire;
};
#endif /* not HAVE_LINUX_AUTO_FS4_H */

#define AUTOFS_AUTO_FS_FLAGS	(FS_AMQINFO | FS_DIRECTORY | FS_AUTOFS | FS_ON_AUTOFS)
#define AUTOFS_DIRECT_FS_FLAGS	(FS_DIRECT | FS_MKMNT | FS_NOTIMEOUT | FS_BACKGROUND | FS_AMQINFO)
#define AUTOFS_ERROR_FS_FLAGS	(FS_DISCARD)
#define AUTOFS_HOST_FS_FLAGS	(FS_MKMNT | FS_BACKGROUND | FS_AMQINFO)
#define AUTOFS_INHERIT_FS_FLAGS	(FS_DISCARD)
#define AUTOFS_LINK_FS_FLAGS	(FS_MBACKGROUND | FS_ON_AUTOFS)
#define AUTOFS_LINKX_FS_FLAGS	(FS_MBACKGROUND | FS_ON_AUTOFS)
#define AUTOFS_NFSL_FS_FLAGS	(FS_BACKGROUND | FS_AMQINFO | FS_ON_AUTOFS)
#define AUTOFS_NFSX_FS_FLAGS	(/* FS_UBACKGROUND| */ FS_AMQINFO | FS_ON_AUTOFS)
#define AUTOFS_PROGRAM_FS_FLAGS	(FS_BACKGROUND | FS_AMQINFO)
#define AUTOFS_ROOT_FS_FLAGS	(FS_NOTIMEOUT | FS_AMQINFO | FS_DIRECTORY)
#define AUTOFS_TOPLVL_FS_FLAGS	(FS_MKMNT | FS_NOTIMEOUT | FS_BACKGROUND | FS_AMQINFO | FS_DIRECTORY | FS_AUTOFS)
#define AUTOFS_UNION_FS_FLAGS	(FS_NOTIMEOUT | FS_BACKGROUND | FS_AMQINFO | FS_DIRECTORY | FS_ON_AUTOFS)

#define AUTOFS_CACHEFS_FS_FLAGS	(FS_NOTIMEOUT | FS_UBACKGROUND | FS_AMQINFO | FS_ON_AUTOFS)
#define AUTOFS_CDFS_FS_FLAGS	(FS_UBACKGROUND | FS_AMQINFO | FS_ON_AUTOFS)
#define AUTOFS_EFS_FS_FLAGS	(FS_NOTIMEOUT | FS_UBACKGROUND | FS_AMQINFO | FS_ON_AUTOFS)
#define AUTOFS_LOFS_FS_FLAGS	(FS_NOTIMEOUT | FS_UBACKGROUND | FS_AMQINFO | FS_ON_AUTOFS)
#define AUTOFS_NFS_FS_FLAGS	(FS_BACKGROUND | FS_AMQINFO | FS_ON_AUTOFS)
#define AUTOFS_PCFS_FS_FLAGS	(FS_UBACKGROUND | FS_AMQINFO | FS_ON_AUTOFS)
#define AUTOFS_UFS_FS_FLAGS	(FS_NOTIMEOUT | FS_UBACKGROUND | FS_AMQINFO | FS_ON_AUTOFS)
#define AUTOFS_XFS_FS_FLAGS	(FS_NOTIMEOUT | FS_UBACKGROUND | FS_AMQINFO | FS_ON_AUTOFS)
#endif /* HAVE_FS_AUTOFS */
