/*
 * Copyright (c) 1997-2005 Erez Zadok
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
 * $Id: umount_linux.c,v 1.6 2005/07/20 03:32:30 ezk Exp $
 *
 */

/*
 * Linux method of unmounting filesystems: if umount(2) failed with EIO or
 * ESTALE, try umount2(2) with MNT_FORCE+MNT_DETACH.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* HAVE_CONFIG_H */
#include <am_defs.h>
#include <amu.h>


int
umount_fs(char *mntdir, const char *mnttabname, int unmount_flags)
{
  struct stat dummy;
  mntlist *mlist, *mp, *mp_save = 0;
  int error = 0;

  mp = mlist = read_mtab(mntdir, mnttabname);

  /*
   * Search the mount table looking for
   * the correct (ie last) matching entry
   */
  while (mp) {
    if (STREQ(mp->mnt->mnt_dir, mntdir))
      mp_save = mp;
    mp = mp->mnext;
  }

  if (mp_save) {
    dlog("Trying unmount(%s)", mp_save->mnt->mnt_dir);

#ifdef MOUNT_TABLE_ON_FILE
    /*
     * This unmount may hang leaving this process with an exclusive lock on
     * /etc/mtab. Therefore it is necessary to unlock mtab, do the unmount,
     * then lock mtab (again) and reread it and finally update it.
     */
    unlock_mntlist();
#endif /* MOUNT_TABLE_ON_FILE */

#if defined(HAVE_UMOUNT2) && defined(MNT2_GEN_OPT_DETACH)
    /*
     * If user asked to try forced unmounts, then do a quick check to see if
     * the mount point is hung badly.  If so, then try to detach it by
     * force; if the latter works, we're done.
     *
     * XXX: the stat() below may hang this unmount attempt of a toplvl
     * mount.  In that case, you may have to kill -9 the Amd process.  A
     * better way to handle this would be to check mtab for an old amd
     * process, send a kill -0 to it to see if the Amd process is alive, and
     * only do the forced unmount if the older Amd process died.
     */
    if ((unmount_flags & AMU_UMOUNT_DETACH) &&
	(stat(mp_save->mnt->mnt_dir, &dummy) < 0 &&
	  (errno == ESTALE || errno == EIO))) {
      plog(XLOG_INFO, "Forcibly unmounting stale/bad %s (%m)",
	   mp_save->mnt->mnt_dir);
      error = umount2(mp_save->mnt->mnt_dir, MNT2_GEN_OPT_DETACH);
    } else
#endif /* defined(HAVE_UMOUNT2) && defined(MNT2_GEN_OPT_DETACH) */
      error = UNMOUNT_TRAP(mp_save->mnt);
    if (error < 0) {
      plog(XLOG_WARNING, "unmount(%s) failed: %m", mp_save->mnt->mnt_dir);
      switch ((error = errno)) {
      case EINVAL:
      case ENOTBLK:
	plog(XLOG_WARNING, "unmount: %s is not mounted", mp_save->mnt->mnt_dir);
	error = 0;		/* Not really an error */
	break;

      case ENOENT:
	/*
	 * This could happen if the kernel insists on following symlinks
	 * when we try to unmount a direct mountpoint. We need to propagate
	 * the error up so that the top layers know it failed and don't
	 * try to rmdir() the mountpoint or other silly things.
	 */
	plog(XLOG_ERROR, "mount point %s: %m", mp_save->mnt->mnt_dir);
	break;

#if defined(HAVE_UMOUNT2) && defined(MNT2_GEN_OPT_DETACH)
      case EBUSY:
	/*
	 * Caller determines if forced unmounts should be used now (for
	 * EBUSY).  If caller asked to force an unmount, *and* the above
	 * "trivial" unmount attempt failed with EBUSY, then try to force
	 * the unmount.
	 */
	if (unmount_flags & AMU_UMOUNT_FORCE) {
	  error = umount2(mp_save->mnt->mnt_dir, MNT2_GEN_OPT_FORCE);
	  if (error < 0) {
	    plog(XLOG_WARNING, "%s: unmount/detach: %m",
		 mp_save->mnt->mnt_dir);
	    error = errno;
	  }
	}
	break;
#endif /* defined(HAVE_UMOUNT2) && defined(MNT2_GEN_OPT_DETACH) */

      default:
	dlog("%s: unmount: %m", mp_save->mnt->mnt_dir);
	break;
      }
    } else {
      dlog("unmount(%s) succeeded", mp_save->mnt->mnt_dir);
    }
    dlog("Finished unmount(%s)", mp_save->mnt->mnt_dir);

    /*
     * If we are successful or there was an ENOENT, remove
     * the mount entry from the mtab file.
     */
    if (!error || error == ENOENT) {
#ifdef HAVE_LOOP_DEVICE
      /* look for loop=/dev/loopX in mnt_opts */
      char *opt, *xopts=NULL;
      char loopstr[] = "loop=";
      char *loopdev;
      xopts = strdup(mp_save->mnt->mnt_opts); /* b/c strtok is destructive */
      for (opt = strtok(xopts, ","); opt; opt = strtok(NULL, ","))
	if (NSTREQ(opt, loopstr, sizeof(loopstr) - 1)) {
	  loopdev = opt + sizeof(loopstr) - 1;
	  if (delete_loop_device(loopdev) < 0)
	    plog(XLOG_WARNING, "unmount() failed to release loop device %s: %m", loopdev);
	  else
	    plog(XLOG_INFO, "unmount() released loop device %s OK", loopdev);
	  break;
	}
      if (xopts)
	XFREE(xopts);
#endif /* HAVE_LOOP_DEVICE */

#ifdef MOUNT_TABLE_ON_FILE
      free_mntlist(mlist);
      mp = mlist = read_mtab(mntdir, mnttabname);

      /*
       * Search the mount table looking for
       * the correct (ie last) matching entry
       */
      mp_save = 0;
      while (mp) {
	if (STREQ(mp->mnt->mnt_dir, mntdir))
	  mp_save = mp;
	mp = mp->mnext;
      }

      if (mp_save) {
	mnt_free(mp_save->mnt);
	mp_save->mnt = 0;
	rewrite_mtab(mlist, mnttabname);
      }
#endif /* MOUNT_TABLE_ON_FILE */
    }

  } else {

    plog(XLOG_ERROR, "Couldn't find how to unmount %s", mntdir);
    /*
     * Assume it is already unmounted
     */
    error = 0;
  } /* end of "if (mp_save)" statement */

  free_mntlist(mlist);

  return error;
}
