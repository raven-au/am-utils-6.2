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
 * $Id: amfs_auto.c,v 1.62 2003/07/30 06:56:05 ib42 Exp $
 *
 */

/*
 * Automount file system
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* HAVE_CONFIG_H */
#include <am_defs.h>
#include <amd.h>

/****************************************************************************
 *** MACROS                                                               ***
 ****************************************************************************/


/****************************************************************************
 *** STRUCTURES                                                           ***
 ****************************************************************************/


/****************************************************************************
 *** FORWARD DEFINITIONS                                                  ***
 ****************************************************************************/
static int amfs_auto_mount(am_node *mp, mntfs *mf);


/****************************************************************************
 *** OPS STRUCTURES                                                       ***
 ****************************************************************************/
am_ops amfs_auto_ops =
{
  "auto",
  amfs_generic_match,
  0,				/* amfs_auto_init */
  amfs_auto_mount,
  amfs_generic_umount,
  amfs_generic_lookup_child,
  amfs_generic_mount_child,
  amfs_generic_readdir,
  0,				/* amfs_auto_readlink */
  amfs_generic_mounted,
  0,				/* amfs_auto_umounted */
  amfs_generic_find_srvr,
  FS_AMQINFO | FS_DIRECTORY,
#ifdef HAVE_FS_AUTOFS
  AUTOFS_AUTO_FS_FLAGS,
#endif /* HAVE_FS_AUTOFS */
};


/****************************************************************************
 *** FUNCTIONS                                                             ***
 ****************************************************************************/
/*
 * Mount a sub-mount
 */
static int
amfs_auto_mount(am_node *mp, mntfs *mf)
{
  /*
   * Pseudo-directories are used to provide some structure
   * to the automounted directories instead
   * of putting them all in the top-level automount directory.
   *
   * Here, just increment the parent's link count.
   */
  mp->am_parent->am_fattr.na_nlink++;

  /*
   * Info field of . means use parent's info field.
   * Historical - not documented.
   */
  if (mf->mf_info[0] == '.' && mf->mf_info[1] == '\0')
    mf->mf_info = strealloc(mf->mf_info, mp->am_parent->am_mnt->mf_info);

  /*
   * Compute prefix:
   *
   * If there is an option prefix then use that else
   * If the parent had a prefix then use that with name
   *      of this node appended else
   * Use the name of this node.
   *
   * That means if you want no prefix you must say so
   * in the map.
   */
  if (mf->mf_fo->opt_pref) {
    /* allow pref:=null to set a real null prefix */
    if (STREQ(mf->mf_fo->opt_pref, "null")) {
      mp->am_pref = strdup("");
    } else {
      /*
       * the prefix specified as an option
       */
      mp->am_pref = strdup(mf->mf_fo->opt_pref);
    }
  } else {
    /*
     * else the parent's prefix
     * followed by the name
     * followed by /
     */
    char *ppref = mp->am_parent->am_pref;
    if (ppref == 0)
      ppref = "";
    mp->am_pref = str3cat((char *) 0, ppref, mp->am_name, "/");
  }

#ifdef HAVE_FS_AUTOFS
  if (mf->mf_flags & MFF_IS_AUTOFS) {
    char opts[256];
    int error;

    autofs_get_opts(opts, mp->am_autofs_fh);

    /* now do the mount */
    error = amfs_mount(mp, mf, opts);
    if (error) {
      errno = error;
      plog(XLOG_FATAL, "amfs_auto_mount: amfs_mount failed: %m");
      return error;
    }
  }
#endif /* HAVE_FS_AUTOFS */

  /*
   * Attach a map cache
   */
  amfs_mkcacheref(mf);

  return 0;
}
