#ifdef HAVE_FS_AUTOFS
typedef autofs_args_t autofs_fh_t;

#define NEED_AUTOFS_SPACE_HACK

static inline char *autofs_strdup_space_hack(char *s)
{
  /*
   * autofs hack: append a space to the directory name
   * to stop the kernel->daemon recursion.
   *
   * Returns malloc'ed space which needs to be freed by the caller.
   */
  char *tmp = malloc(strlen(s) + 2);
  strcpy(tmp, s);
  strcat(tmp, " ");
  return tmp;
}

#define AUTOFS_AUTO_FS_FLAGS	(FS_AMQINFO | FS_DIRECTORY | FS_AUTOFS)
#define AUTOFS_DIRECT_FS_FLAGS	(FS_DIRECT | FS_MKMNT | FS_NOTIMEOUT | FS_BACKGROUND | FS_AMQINFO | FS_DIRECTORY | FS_AUTOFS)
#define AUTOFS_ERROR_FS_FLAGS	(FS_DISCARD)
#define AUTOFS_HOST_FS_FLAGS	(FS_MKMNT | FS_BACKGROUND | FS_AMQINFO)
#define AUTOFS_INHERIT_FS_FLAGS	(FS_DISCARD)
#define AUTOFS_LINK_FS_FLAGS	(FS_MBACKGROUND)
#define AUTOFS_LINKX_FS_FLAGS	(FS_MBACKGROUND)
#define AUTOFS_NFSL_FS_FLAGS	(FS_MKMNT | FS_BACKGROUND | FS_AMQINFO)
#define AUTOFS_NFSX_FS_FLAGS	(/* FS_UBACKGROUND| */ FS_AMQINFO)
#define AUTOFS_PROGRAM_FS_FLAGS	(FS_BACKGROUND | FS_AMQINFO)
#define AUTOFS_ROOT_FS_FLAGS	(FS_NOTIMEOUT | FS_AMQINFO | FS_DIRECTORY)
#define AUTOFS_TOPLVL_FS_FLAGS	(FS_MKMNT | FS_NOTIMEOUT | FS_BACKGROUND | FS_AMQINFO | FS_DIRECTORY | FS_AUTOFS)
#define AUTOFS_UNION_FS_FLAGS	(FS_MKMNT | FS_NOTIMEOUT | FS_BACKGROUND | FS_AMQINFO | FS_DIRECTORY)

#define AUTOFS_CACHEFS_FS_FLAGS	(FS_MKMNT | FS_NOTIMEOUT | FS_UBACKGROUND | FS_AMQINFO)
#define AUTOFS_CDFS_FS_FLAGS	(FS_MKMNT | FS_UBACKGROUND | FS_AMQINFO)
#define AUTOFS_EFS_FS_FLAGS	(FS_MKMNT | FS_NOTIMEOUT | FS_UBACKGROUND | FS_AMQINFO)
#define AUTOFS_LOFS_FS_FLAGS	(FS_MKMNT | FS_NOTIMEOUT | FS_UBACKGROUND | FS_AMQINFO)
#define AUTOFS_NFS_FS_FLAGS	(FS_MKMNT | FS_BACKGROUND | FS_AMQINFO)
#define AUTOFS_PCFS_FS_FLAGS	(FS_MKMNT | FS_UBACKGROUND | FS_AMQINFO)
#define AUTOFS_UFS_FS_FLAGS	(FS_MKMNT | FS_NOTIMEOUT | FS_UBACKGROUND | FS_AMQINFO)
#define AUTOFS_XFS_FS_FLAGS	(FS_MKMNT | FS_NOTIMEOUT | FS_UBACKGROUND | FS_AMQINFO)
#endif /* HAVE_FS_AUTOFS */
