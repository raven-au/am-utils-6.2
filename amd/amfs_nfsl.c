/*
 * Copyright (c) 1997-2003 Erez Zadok
 * Copyright (c) 1990 Jan-Simon Pendry
 * Copyright (c) 1990 Imperial College of Science, Technology & Medicine
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Jan-Simon Pendry at Imperial College, London.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgment:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 * $Id: amfs_nfsl.c,v 1.17 2003/07/30 06:56:06 ib42 Exp $
 *
 */

/*
 * NFSL: Network file system with local existence check.  If the local
 * path denoted by $rfs exists, it behaves as type:=link.
 *
 * Example:
 *	pkg	type:=nfsl;rhost:=jonny;rfs:=/n/johnny/src/pkg;fs:=${rfs}
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* HAVE_CONFIG_H */
#include <am_defs.h>
#include <amd.h>


/* forward declarations */
static char *amfs_nfsl_match(am_opts *fo);
static int amfs_nfsl_init(mntfs *mf);
static int amfs_nfsl_mount(am_node *mp, mntfs *mf);
static int amfs_nfsl_umount(am_node *mp, mntfs *mf);
static void amfs_nfsl_umounted(mntfs *mf);
static fserver *amfs_nfsl_ffserver(mntfs *mf);

/*
 * NFS-Link operations
 */
am_ops amfs_nfsl_ops =
{
  "nfsl",			/* name of file system */
  amfs_nfsl_match,		/* match */
  amfs_nfsl_init,		/* initialize */
  amfs_nfsl_mount,		/* mount vnode */
  amfs_nfsl_umount,		/* unmount vnode */
  amfs_error_lookup_child,	/* lookup path-name */
  amfs_error_mount_child,
  amfs_error_readdir,		/* read directory */
  0,				/* read link */
  0,				/* after-mount extra actions */
  amfs_nfsl_umounted,		/* after-umount extra actions */
  amfs_nfsl_ffserver,		/* find a file server */
  FS_MKMNT | FS_BACKGROUND | FS_AMQINFO,	/* nfs_fs_flags */
#ifdef HAVE_FS_AUTOFS
  AUTOFS_NFSL_FS_FLAGS,
#endif /* HAVE_FS_AUTOFS */
};


/*
 * Check that f/s has all needed fields.
 * Returns: matched string if found, NULL otherwise.
 */
static char *
amfs_nfsl_match(am_opts *fo)
{
  char *cp = fo->opt_fs;
  char *ho = fo->opt_rhost;
  struct stat stb;

  if (!cp || !ho) {
    plog(XLOG_USER, "amfs_nfsl: host $fs and $rhost must be specified");
    return NULL;
  }

  /*
   * If this host is not the same as $rhost, or if link does not exist,
   * perform nfs_match(), same as for type:=nfs.
   * If link value exists (or same host), then perform amfs_link_match(),
   * same as for linkx.
   */
  if (!STRCEQ(ho, am_get_hostname())) {
    plog(XLOG_INFO, "amfs_nfsl: \"%s\" is not local host, using type:=nfs", ho);
    return nfs_ops.fs_match(fo);
  } else if (lstat(cp, &stb) < 0) {
    plog(XLOG_INFO, "amfs_nfsl: \"%s\" does not exist, using type:=nfs", cp);
    return nfs_ops.fs_match(fo);
  } else {
    plog(XLOG_INFO, "amfs_nfsl: \"%s\" exists, using type:=link", cp);
    return amfs_link_ops.fs_match(fo);
  }
}


/*
 * Initialize.
 * Returns: 0 if OK, non-zero (errno) if failed.
 */
static int
amfs_nfsl_init(mntfs *mf)
{
  int ret = 0;
  /*
   * If a link, do nothing (same as type:=link).
   * If non-link, do nfs_init (same as type:=nfs).
   */
  if (mf->mf_flags & MFF_NFSLINK) {
    if (amfs_link_ops.fs_init)
      ret = amfs_link_ops.fs_init(mf);
  } else {
    if (nfs_ops.fs_init)
      ret = nfs_ops.fs_init(mf);
  }
  return ret;
}


/*
 * Mount vfs.
 * Returns: 0 if OK, non-zero (errno) if failed.
 */
static int
amfs_nfsl_mount(am_node *mp, mntfs *mf)
{
  /*
   * If a link, do run amfs_link_fmount() (same as type:=link)
   * If non-link, do nfs_fmount (same as type:=nfs).
   */
  if (mf->mf_flags & MFF_NFSLINK) {
    return amfs_link_ops.mount_fs(mp, mf);
  } else {
    return nfs_ops.mount_fs(mp, mf);
  }
}


/*
 * Unmount VFS.
 * Returns: 0 if OK, non-zero (errno) if failed.
 */
static int
amfs_nfsl_umount(am_node *mp, mntfs *mf)
{
  /*
   * If a link, do run amfs_link_umount() (same as type:=link)
   * If non-link, do nfs_umount (same as type:=nfs).
   */
  if (mf->mf_flags & MFF_NFSLINK) {
    return amfs_link_ops.umount_fs(mp, mf);
  } else {
    return amfs_link_ops.umount_fs(mp, mf);
  }
}


/*
 * Async unmount callback function.
 * After the base umount() succeeds, we may want to take extra actions,
 * such as informing remote mount daemons that we've unmounted them.
 * See amfs_auto_umounted(), host_umounted(), nfs_umounted().
 */
static void
amfs_nfsl_umounted(mntfs *mf)
{
  /*
   * If a link, do nothing (same as type:=link)
   * If non-link, do nfs_umount (same as type:=nfs).
   */
  if (mf->mf_flags & MFF_NFSLINK) {
    if (amfs_link_ops.umounted)
      amfs_link_ops.umounted(mf);
  } else {
    if (nfs_ops.umounted)
      nfs_ops.umounted(mf);
    /*
     * MUST remove mount point directories, because if they remain
     * behind, the next nfsl access will think they are a link
     * type file system, and not NFS! (when it performs link target
     * existence test)
     */
    if (mf->mf_flags & MFF_MKMNT) {
      rmdirs(mf->mf_mount);
      mf->mf_flags &= ~MFF_MKMNT;
    }
  }
}


/*
 * Find a file server.
 * Returns: fserver of found server, or NULL if not found.
 */
static fserver *
amfs_nfsl_ffserver(mntfs *mf)
{
  char *cp = mf->mf_fo->opt_fs;
  char *ho = mf->mf_fo->opt_rhost;
  struct stat stb;

  /*
   * If this host is not the same as $rhost, or if link does not exist,
   * perform find_nfs_srvr(), same as for type:=nfs.
   * If link value exists (or same host), then perform
   * amfs_generic_find_srvr(), same as for linkx.
   */
  if (!STRCEQ(ho, am_get_hostname()) || lstat(cp, &stb) < 0) {
    return find_nfs_srvr(mf);
  } else {
    mf->mf_flags |= MFF_NFSLINK;
    return amfs_generic_find_srvr(mf);
  }
}
