/*
 * Copyright (c) 1999-2000 Ion Badulescu
 * Copyright (c) 1997-2000 Erez Zadok
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
 *      %W% (Berkeley) %G%
 *
 * $Id: autofs_linux.c,v 1.1 2000/11/28 06:36:50 ib42 Exp $
 *
 */

/*
 * Automounter filesystem
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* HAVE_CONFIG_H */
#include <am_defs.h>
#include <amd.h>

/*
 * KLUDGE: wrap whole file in HAVE_FS_AUTOFS, because
 * not all systems with an automounter file system are supported
 * by am-utils yet...
 */

#ifdef HAVE_FS_AUTOFS

/*
 * MACROS:
 */

/* FIXME: 256 below should be replaced with a system-specific
   value for max filedescriptors */
#define AUTOFS_MAX_FDS 256
#define AUTOFS_MIN_PROTO_VERSION 3
#define AUTOFS_MAX_PROTO_VERSION 4


/*
 * STRUCTURES:
 */

/*
 * VARIABLES:
 */

static am_node *hash[AUTOFS_MAX_FDS];
static int list[AUTOFS_MAX_FDS];
static int numfds = 0;


static void hash_init(void)
{
  int i;

  for (i = 0 ; i < AUTOFS_MAX_FDS; i++)
    hash[i] = 0, list[i] = -1;
}

static void hash_insert(int fd, am_node *mp)
{
  if (hash[fd] != 0)
    plog(XLOG_ERROR, "file descriptor %d already in the hash", fd);

  hash[fd] = mp;
  list[numfds] = fd;
  numfds++;
}

static void hash_delete(int fd)
{
  int i;

  if (hash[fd] == 0)
    plog(XLOG_WARNING, "file descriptor %d not in the hash", fd);

  hash[fd] = 0;
  numfds--;
  for (i = 0; i < numfds; i++)
    if (list[i] == fd) {
      list[i] = list[numfds];
      break;
    }
}

autofs_fh_t *
autofs_get_fh(am_node *mp)
{
  int fds[2];
  autofs_fh_t *fh;

  plog(XLOG_DEBUG, "autofs_get_fh for %s", mp->am_path);
  /* XXX not fully implemented??? */
  if (pipe(fds) < 0)
    return 0;
  fh = ALLOC(autofs_fh_t);
  fh->fd = fds[0];
  fh->kernelfd = fds[1];
  fh->ioctlfd = -1;
  fh->waiting = 0;

  hash_insert(fh->fd, mp);

  return fh;
}

void
autofs_mounted(mntfs *mf)
{
  autofs_fh_t *fh = mf->mf_autofs_fh;

  close(fh->kernelfd);
  fh->ioctlfd = open(mf->mf_mount, O_RDONLY);
  /* Get autofs protocol version */
  if (ioctl(fh->ioctlfd, AUTOFS_IOC_PROTOVER, &fh->version) < 0) {
    plog(XLOG_ERROR, "AUTOFS_IOC_PROTOVER: %s", strerror(errno));
    fh->version = AUTOFS_MIN_PROTO_VERSION;
    plog(XLOG_ERROR, "autofs: assuming protocol version %d", fh->version);
  } else
    plog(XLOG_INFO, "autofs: using protocol version %d", fh->version);
}

void
autofs_release_fh(autofs_fh_t *fh)
{
  if (fh) {
    hash_delete(fh->fd);
    /*
     * if a mount succeeded, the kernel fd was closed on
     * the amd side, so it might have been reused.
     * we set it to -1 after closing it, to avoid the problem.
     */
    if (fh->kernelfd >= 0)
      close(fh->kernelfd);

    if (fh->ioctlfd >= 0) {
      /*
       * Tell the kernel we're catatonic
       */
      ioctl(fh->ioctlfd, AUTOFS_IOC_CATATONIC, 0);
      close(fh->ioctlfd);
    }
#if 0
    if (fh->fd >= 0)
      close(fh->fd);
#endif

    XFREE(fh);
  }
}

static int
autofs_get_pkt(int fd, struct autofs_packet_missing *pkt)
{
  char *buf = (char *) pkt;
  int bytes = sizeof(*pkt);
  int i;

  do {
    i = read(fd, buf, bytes);

    if (i <= 0)
      break;

    buf += i;
    bytes -= i;
  } while (bytes);

  return bytes;
}

void
autofs_add_fdset(fd_set *readfds)
{
  int i;
  for (i = 0; i < numfds; i++)
    FD_SET(list[i], readfds);
}

int
autofs_handle_fdset(fd_set *readfds, int nsel)
{
  int i, error;
  struct autofs_packet_missing pkt;
  autofs_fh_t *fh;
  am_node *mp, *ap;
  mntfs *mf;

  for (i = 0; nsel && i < numfds; i++) {
    if (!FD_ISSET(list[i], readfds))
      continue;

    nsel--;
    FD_CLR(list[i], readfds);
    mp = hash[list[i]];
    mf = mp->am_mnt;
    fh = mf->mf_autofs_fh;

    if (autofs_get_pkt(fh->fd, &pkt))
      continue;

    fh->wait_queue_token = pkt.wait_queue_token;
    fh->waiting = 1;

    amuDebug(D_TRACE)
      plog(XLOG_DEBUG, "\tlookuppn(%s, %s)", mp->am_path, pkt.name);
    ap = mf->mf_ops->lookuppn(mp, pkt.name, &error, VLOOK_CREATE);

    /* some of the rest can be done in amfs_auto_cont */

    if (ap == 0) {
      if (error < 0) {
	dlog("Not sending autofs reply");
	return 0;
      }
      autofs_lookup_failed(mp, pkt.name);
    } else {
      /*
       * XXX: EXPERIMENTAL! Delay unmount of what was looked up.  This
       * should reduce the chance for race condition between unmounting an
       * entry synchronously, and re-mounting it asynchronously.
       */
      if (ap->am_ttl < mp->am_ttl)
	ap->am_ttl = mp->am_ttl;
      autofs_mount_succeeded(ap);
    }
  }
  return nsel;
}

int
create_autofs_service(void)
{
  hash_init();
  return 0;
}

int
autofs_link_mount(am_node *mp)
{
  plog(XLOG_INFO, "autofs: symlinking %s -> %s", mp->am_path, mp->am_link);
  return symlink(mp->am_link, mp->am_path);
}

static int
autofs_link_umount(am_node *mp)
{
  plog(XLOG_INFO, "autofs: deleting symlink %s", mp->am_path);
  return unlink(mp->am_path);
}

int
autofs_umount_succeeded(am_node *mp)
{
  if (mp->am_link)
    return autofs_link_umount(mp);

  plog(XLOG_INFO, "autofs: removing mountpoint directory %s", mp->am_path);
  return rmdir(mp->am_path);
}

void
autofs_mount_succeeded(am_node *mp)
{
  autofs_fh_t *fh = mp->am_parent->am_mnt->mf_autofs_fh;

  /* sanity check */
  if (fh->waiting == 0)
    return;

  plog(XLOG_INFO, "autofs: mounting %s succeeded", mp->am_path);
  if ( ioctl(fh->ioctlfd, AUTOFS_IOC_READY, fh->wait_queue_token) < 0 )
    plog(XLOG_ERROR, "AUTOFS_IOC_READY: %s", strerror(errno));
  fh->waiting = 0;
}

void
autofs_mount_failed(am_node *mp)
{
  autofs_fh_t *fh = mp->am_parent->am_mnt->mf_autofs_fh;

  /* sanity check */
  if (fh->waiting == 0)
    return;

  rmdir(mp->am_path);

  plog(XLOG_INFO, "autofs: mounting %s failed", mp->am_path);
  if ( ioctl(fh->ioctlfd, AUTOFS_IOC_FAIL, fh->wait_queue_token) < 0 )
    syslog(XLOG_ERROR, "AUTOFS_IOC_FAIL: %s", strerror(errno));

  fh->waiting = 0;
}

void
autofs_lookup_failed(am_node *mp, char *name)
{
  autofs_fh_t *fh = mp->am_mnt->mf_autofs_fh;

  /* sanity check */
  if (fh->waiting == 0)
    return;

  plog(XLOG_INFO, "autofs: lookup of %s failed", name);
  if ( ioctl(fh->ioctlfd, AUTOFS_IOC_FAIL, fh->wait_queue_token) < 0 )
    syslog(XLOG_ERROR, "AUTOFS_IOC_FAIL: %s", strerror(errno));

  fh->waiting = 0;
}

void
autofs_get_opts(char *opts, autofs_fh_t *fh)
{
  sprintf(opts, "fd=%d,pgrp=%ld,minproto=%d,maxproto=%d",
	  fh->kernelfd, get_server_pid(),
	  AUTOFS_MIN_PROTO_VERSION, AUTOFS_MAX_PROTO_VERSION);
}

#endif /* HAVE_FS_AUTOFS */
