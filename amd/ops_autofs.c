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
 * $Id: ops_autofs.c,v 1.15 2000/11/05 13:03:09 ib42 Exp $
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
#ifndef AUTOFS_NULL
# define AUTOFS_NULL	NULLPROC
#endif /* not AUTOFS_NULL */

/*
 * STRUCTURES:
 */

/*
 * VARIABLES:
 */

int amd_use_autofs = 0;

#if 0
#ifdef __sun__

/* forward declarations */
static int mount_autofs(char *dir, char *opts);
static int autofs_mount_1_svc(struct mntrequest *mr, struct mntres *result, struct authunix_parms *cred);
static int autofs_unmount_1_svc(struct umntrequest *ur, struct umntres *result, struct authunix_parms *cred);

/****************************************************************************
 *** VARIABLES                                                            ***
 ****************************************************************************/

/****************************************************************************
 *** FUNCTIONS                                                            ***
 ****************************************************************************/

/*
 * Mount an automounter directory.
 * The automounter is connected into the system
 * as a user-level NFS server.  mount_autofs constructs
 * the necessary NFS parameters to be given to the
 * kernel so that it will talk back to us.
 */
static int
mount_autofs(char *dir, char *opts)
{
  char fs_hostname[MAXHOSTNAMELEN + MAXPATHLEN + 1];
  char *map_opt, buf[MAXHOSTNAMELEN];
  int retry, error, flags;
  struct utsname utsname;
  mntent_t mnt;
  autofs_args_t autofs_args;
  MTYPE_TYPE type = MOUNT_TYPE_AUTOFS;

  memset((voidp) &autofs_args, 0, sizeof(autofs_args)); /* Paranoid */

  memset((voidp) &mnt, 0, sizeof(mnt));
  mnt.mnt_dir = dir;
  mnt.mnt_fsname = pid_fsname;
  mnt.mnt_opts = opts;
  mnt.mnt_type = type;

  retry = hasmntval(&mnt, "retry");
  if (retry <= 0)
    retry = 2;			/* XXX */

  /*
   * SET MOUNT ARGS
   */
  if (uname(&utsname) < 0) {
    strcpy(buf, "localhost.autofs");
  } else {
    strcpy(buf, utsname.nodename);
    strcat(buf, ".autofs");
  }
#ifdef HAVE_FIELD_AUTOFS_ARGS_T_ADDR
  autofs_args.addr.buf = buf;
  autofs_args.addr.len = strlen(autofs_args.addr.buf);
  autofs_args.addr.maxlen = autofs_args.addr.len;
#endif /* HAVE_FIELD_AUTOFS_ARGS_T_ADDR */

  autofs_args.path = dir;
  autofs_args.opts = opts;

  map_opt = hasmntopt(&mnt, "map");
  if (map_opt) {
    map_opt += sizeof("map="); /* skip the "map=" */
    if (map_opt == NULL) {
      plog(XLOG_WARNING, "map= has a null map name. reset to amd.unknown");
      map_opt = "amd.unknown";
    }
  }
  autofs_args.map = map_opt;

  /* XXX: these I set arbitrarily... */
  autofs_args.mount_to = 300;
  autofs_args.rpc_to = 60;
  autofs_args.direct = 0;

  /*
   * Make a ``hostname'' string for the kernel
   */
  sprintf(fs_hostname, "pid%ld@%s:%s",
	  get_server_pid(), am_get_hostname(), dir);

  /*
   * Most kernels have a name length restriction.
   */
  if (strlen(fs_hostname) >= MAXHOSTNAMELEN)
    strcpy(fs_hostname + MAXHOSTNAMELEN - 3, "..");

  /*
   * Finally we can compute the mount flags set above.
   */
  flags = compute_mount_flags(&mnt);

  /*
   * This is it!  Here we try to mount amd on its mount points.
   */
  error = mount_fs(&mnt, flags, (caddr_t) &autofs_args, retry, type, 0, NULL, mnttab_file_name);
  return error;
}


/****************************************************************************/
/* autofs program dispatcher */
static void
autofs_program_1(struct svc_req *rqstp, SVCXPRT *transp)
{
  int ret;
  union {
    mntrequest autofs_mount_1_arg;
    umntrequest autofs_umount_1_arg;
  } argument;
  union {
    mntres mount_res;
    umntres umount_res;
  } result;

  bool_t (*xdr_argument)(), (*xdr_result)();
  int (*local)();

  switch (rqstp->rq_proc) {

  case AUTOFS_NULL:
    svc_sendreply(transp,
		  (XDRPROC_T_TYPE) xdr_void,
		  (SVC_IN_ARG_TYPE) NULL);
    return;

  case AUTOFS_MOUNT:
    xdr_argument = xdr_mntrequest;
    xdr_result = xdr_mntres;
    local = (int (*)()) autofs_mount_1_svc;
    break;

  case AUTOFS_UNMOUNT:
    xdr_argument = xdr_umntrequest;
    xdr_result = xdr_umntres;
    local = (int (*)()) autofs_unmount_1_svc;
    break;

  default:
    svcerr_noproc(transp);
    return;
  }

  memset((char *) &argument, 0, sizeof(argument));
  if (!svc_getargs(transp,
		   (XDRPROC_T_TYPE) xdr_argument,
		   (SVC_IN_ARG_TYPE) &argument)) {
    plog(XLOG_ERROR,
	 "AUTOFS xdr decode failed for %d %d %d",
	 (int) rqstp->rq_prog, (int) rqstp->rq_vers, (int) rqstp->rq_proc);
    svcerr_decode(transp);
    return;
  }

  ret = (*local) (&argument, &result, rqstp);
  if (!svc_sendreply(transp,
		     (XDRPROC_T_TYPE) xdr_result,
		     (SVC_IN_ARG_TYPE) &result)) {
    svcerr_systemerr(transp);
  }

  if (!svc_freeargs(transp,
		    (XDRPROC_T_TYPE) xdr_argument,
		    (SVC_IN_ARG_TYPE) &argument)) {
    plog(XLOG_FATAL, "unable to free rpc arguments in autofs_program_1");
    going_down(1);
  }
}


static int
autofs_mount_1_svc(struct mntrequest *mr, struct mntres *result, struct authunix_parms *cred)
{
  int err = 0;
  am_node *anp, *anp2;

  plog(XLOG_INFO, "XXX: autofs_mount_1_svc: %s:%s:%s:%s",
       mr->map, mr->name, mr->opts, mr->path);

  /* look for map (eg. "/home") */
  anp = find_ap(mr->path);
  if (!anp) {
    plog(XLOG_ERROR, "map %s not found", mr->path);
    err = ENOENT;
    goto out;
  }
  /* turn on autofs in map flags */
  if (!(anp->am_flags & AMF_AUTOFS)) {
    plog(XLOG_INFO, "turning on AMF_AUTOFS for node %s", mr->path);
    anp->am_flags |= AMF_AUTOFS;
  }

  /*
   * Look for (and create if needed) the new node.
   *
   * If an error occurred, return it.  If a -1 was returned, that indicates
   * that a mount is in progress, so sleep a while (while the backgrounded
   * mount is happening), and then signal the autofs to retry the mount.
   *
   * There's something I don't understand.  I was thinking that this code
   * here is the one which will succeed eventually and will send an RPC
   * reply to the kernel, but apparently that happens somewhere else, not
   * here.  It works though, just that I don't know how.  Arg. -Erez.
   * */
  err = 0;
  anp2 = autofs_lookuppn(anp, mr->name, &err, VLOOK_CREATE);
  if (!anp2) {
    if (err == -1) {		/* then tell autofs to retry */
      sleep(1);
      err = EAGAIN;
    }
    goto out;
  }

out:
  result->status = err;
  return err;
}


static int
autofs_unmount_1_svc(struct umntrequest *ur, struct umntres *result, struct authunix_parms *cred)
{
  int err = 0;

  plog(XLOG_INFO, "XXX: autofs_unmount_1_svc: %d:%lu:%lu:0x%lx",
       ur->isdirect, (unsigned long) ur->devid, (unsigned long) ur->rdevid,
       (unsigned long) ur->next);

  err = EINVAL;			/* XXX: not implemented yet */
  goto out;

out:
  result->status = err;
  return err;
}
#endif  /* __sun__ */
#endif /* 0 */


/************************************************************************/
#ifdef __sun__

/* XXX not implemented */
static void
autofs_lookup_2_req(autofs_lookupargs *m,
		    autofs_lookupres *res,
		    struct authunix_parms *cred)
{
  dlog("LOOKUP REQUEST: name=%s[%s] map=%s opts=%s path=%s direct=%d\n",
       m->name, m->subdir, m->map, m->opts,
       m->path, m->isdirect);

  res->lu_res = AUTOFS_NOENT;
  res->lu_verbose = 1;

  dlog("LOOKUP REPLY    : status=%d\n", res->lu_res);
}


static void
autofs_lookup_2_free(autofs_lookupres *res)
{
  struct linka link;

  if ((res->lu_res == AUTOFS_OK) &&
      (res->lu_type.action == AUTOFS_LINK_RQ)) {
    /*
     * Free link information
     */
    link = res->lu_type.lookup_result_type_u.lt_linka;
    if (link.dir)
      XFREE(link.dir);
    if (link.link)
      XFREE(link.link);
  }
}


/* XXX not implemented */
static void
autofs_mount_2_req(autofs_lookupargs *m,
		   autofs_mountres *res,
		   struct authunix_parms *cred)
{
  int status;

  dlog("MOUNT REQUEST: name=%s[%s] map=%s opts=%s path=%s direct=%d\n",
       m->name, m->subdir, m->map, m->opts,
       m->path, m->isdirect);

  status = EACCES;

  res->mr_type.status = AUTOFS_DONE;
  res->mr_type.mount_result_type_u.error = status;
  res->mr_verbose = 1;

  switch (res->mr_type.status) {
  case AUTOFS_ACTION:
    dlog("MOUNT REPLY    : status=%d, AUTOFS_ACTION\n",
	 status);
    break;
  case AUTOFS_DONE:
    dlog("MOUNT REPLY    : status=%d, AUTOFS_DONE\n",
	 status);
    break;
  default:
    dlog("MOUNT REPLY    : status=%d, UNKNOWN\n",
	 status);
  }

  if (status) {
    if (m->isdirect) {
      /* direct mount */
      plog(XLOG_ERROR, "mount of %s failed", m->path);
    } else {
      /* indirect mount */
      plog(XLOG_ERROR, "mount of %s/%s failed", m->path, m->name);
    }
  }
}


/* XXX not implemented */
static void
autofs_mount_2_free(struct autofs_mountres *res)
{
  if (res->mr_type.status == AUTOFS_ACTION) {
    dlog("freeing action list\n");
    /*     free_action_list(res->mr_type.mount_result_type_u.list); */
  }
}

/* XXX not implemented */
static void
autofs_postmount_2_req(postmountreq *req,
		       postmountres *res,
		       struct authunix_parms *cred)
{
  dlog("POSTMOUNT REQUEST: %s\tdev=%lx\tspecial=%s %s\n",
       req->mountp, req->devid, req->special, req->mntopts);

  res->status = 0;

  dlog("POSTMOUNT REPLY  : status=%d\n", res->status);
}

/* XXX not implemented */
static void
autofs_unmount_2_req(umntrequest *m,
		     umntres *res,
		     struct authunix_parms *cred)
{
  umntrequest *ul;

  dlog("UNMOUNT REQUEST: \n");
  for (ul = m; ul; ul = ul->next)
    dlog("  dev=%lx rdev=%lx %s\n",
	 ul->devid,
	 ul->rdevid,
	 ul->isdirect ? "direct" : "indirect");

  res->status = 1;

  dlog("UNMOUNT REPLY: status=%d\n", res->status);
}

/* XXX not implemented */
static void
autofs_postunmount_2_req(postumntreq *req,
			 postumntres *res,
			 struct authunix_parms *cred)
{
  postumntreq *ul;

  dlog("POSTUNMOUNT REQUEST: \n");
  for (ul = req; ul; ul = ul->next)
    dlog("  dev=%lx rdev=%lx\n",
	 ul->devid,
	 ul->rdevid);

  res->status = 1;

  dlog("POSTUNMOUNT REPLY  : status=%d\n", res->status);
}

/****************************************************************************/
/* autofs program dispatcher */
static void
autofs_program_2(struct svc_req *rqstp, SVCXPRT *transp)
{
  union {
    autofs_lookupargs autofs_mount_2_arg;
    autofs_lookupargs autofs_lookup_2_arg;
    umntrequest autofs_umount_2_arg;
    autofs_rddirargs autofs_readdir_2_arg;
    postmountreq autofs_postmount_2_arg;
    postumntreq autofs_postumnt_2_arg;
  } argument;

  union {
    autofs_mountres mount_res;
    autofs_lookupres lookup_res;
    umntres umount_res;
    autofs_rddirres readdir_res;
    postumntres postumnt_res;
    postmountres postmnt_res;
  } result;

  bool_t (*xdr_argument)();
  bool_t (*xdr_result)();
  void (*local)();
  void (*local_free)() = NULL;

  switch (rqstp->rq_proc) {

  case AUTOFS_NULL:
    svc_sendreply(transp,
		  (XDRPROC_T_TYPE) xdr_void,
		  (SVC_IN_ARG_TYPE) NULL);
    return;

  case AUTOFS_LOOKUP:
    xdr_argument = xdr_autofs_lookupargs;
    xdr_result = xdr_autofs_lookupres;
    local = autofs_lookup_2_req;
    local_free = autofs_lookup_2_free;
    break;

  case AUTOFS_MOUNT:
    xdr_argument = xdr_autofs_lookupargs;
    xdr_result = xdr_autofs_mountres;
    local = autofs_mount_2_req;
    local_free = autofs_mount_2_free;
    break;

  case AUTOFS_UNMOUNT:
    xdr_argument = xdr_umntrequest;
    xdr_result = xdr_umntres;
    local = autofs_unmount_2_req;
    break;

  case AUTOFS_POSTUNMOUNT:
    xdr_argument = xdr_postumntreq;
    xdr_result = xdr_postumntres;
    local = autofs_postunmount_2_req;
    break;

  case AUTOFS_POSTMOUNT:
    xdr_argument = xdr_postmountreq;
    xdr_result = xdr_postmountres;
    local = autofs_postmount_2_req;
    break;

#ifdef not_yet
  case AUTOFS_READDIR:
    xdr_argument = xdr_autofs_rddirargs;
    xdr_result = xdr_autofs_rddirres;
    local = autofs_readdir_2_req;
    local_free = autofs_readdir_2_free;
    break;
#endif

  default:
    svcerr_noproc(transp);
    return;
  }

  memset((char *) &argument, 0, sizeof(argument));
  if (!svc_getargs(transp,
		   (XDRPROC_T_TYPE) xdr_argument,
		   (SVC_IN_ARG_TYPE) &argument)) {
    plog(XLOG_ERROR,
	 "AUTOFS xdr decode failed for %d %d %d",
	 (int) rqstp->rq_prog, (int) rqstp->rq_vers, (int) rqstp->rq_proc);
    svcerr_decode(transp);
    return;
  }

  memset((char *)&result, 0, sizeof (result));
  (*local) (&argument, &result, rqstp->rq_clntcred);

  if (!svc_sendreply(transp,
		     (XDRPROC_T_TYPE) xdr_result,
		     (SVC_IN_ARG_TYPE) &result)) {
    svcerr_systemerr(transp);
  }

  if (!svc_freeargs(transp,
		    (XDRPROC_T_TYPE) xdr_argument,
		    (SVC_IN_ARG_TYPE) &argument)) {
    plog(XLOG_FATAL, "unable to free rpc arguments in autofs_program_2");
  }

  if (local_free)
    (*local_free)(&result);
}


autofs_fh_t *
autofs_get_fh(am_node *mp)
{
  autofs_fh_t *fh;
  char buf[MAXHOSTNAMELEN];
  mntfs *mf = mp->am_mnt;
  struct utsname utsname;

  plog(XLOG_DEBUG, "autofs_get_fh for %s", mp->am_path);
  fh = ALLOC(autofs_fh_t);
  memset((voidp) fh, 0, sizeof(autofs_fh_t)); /* Paranoid */

  /*
   * SET MOUNT ARGS
   */
  if (uname(&utsname) < 0) {
    strcpy(buf, "localhost.autofs");
  } else {
    strcpy(buf, utsname.nodename);
    strcat(buf, ".autofs");
  }
#ifdef HAVE_FIELD_AUTOFS_ARGS_T_ADDR
  fh->addr.buf = strdup(buf);
  fh->addr.len = fh->addr.maxlen = strlen(buf);
#endif /* HAVE_FIELD_AUTOFS_ARGS_T_ADDR */

  fh->direct = (mf->mf_ops == &amfs_direct_ops);
  fh->rpc_to = 60;			/* XXX: arbitrary */
  fh->mount_to = mp->am_timeo;
  fh->path = mp->am_path;
  fh->opts = "";			/* XXX: arbitrary */
  fh->map = mf->mf_info;
  fh->subdir = "";
  if (fh->direct)
    fh->key = mp->am_name;
  else
    fh->key = "";

  return fh;
}

void
autofs_mounted(mntfs *mf)
{
  /* nothing */
}

void
autofs_release_fh(autofs_fh_t *fh)
{
  /* XXX not fully implemented */
  if (fh) {
    free(fh->addr.buf);
    XFREE(fh);
  }
}


void
autofs_add_fdset(fd_set *readfds)
{
  /* nothing to do */
}

int
autofs_handle_fdset(fd_set *readfds, int nsel)
{
  /* nothing to do */
  return nsel;
}

/*
 * find the IP address that can be used to connect autofs service to.
 */
static int
get_autofs_address(struct netconfig *ncp, struct t_bind *tbp)
{
  int ret;
  struct nd_addrlist *addrs = (struct nd_addrlist *) NULL;
  struct nd_hostserv service;

  service.h_host = HOST_SELF;
  service.h_serv = "autofs";

  ret = netdir_getbyname(ncp, &service, &addrs);

  if (ret) {
    plog(XLOG_FATAL, "get_autofs_address: cannot get local host address: %s", netdir_sperror());
    goto out;
  }

  /*
   * XXX: there may be more more than one address for this local
   * host.  Maybe something can be done with those.
   */
  tbp->addr.len = addrs->n_addrs->len;
  tbp->addr.maxlen = addrs->n_addrs->len;
  memcpy(tbp->addr.buf, addrs->n_addrs->buf, addrs->n_addrs->len);
  tbp->qlen = 8;		/* arbitrary? who cares really */

  /* all OK */
  netdir_free((voidp) addrs, ND_ADDRLIST);

out:
  return ret;
}


#include <rpc/nettype.h>
/*
 * Create the autofs service for amd
 */
int
create_autofs_service(void)
{
  struct t_bind *tbp = 0;
  int fd = -1, err = 1;		/* assume failed */
  SVCXPRT *autofs_xprt = NULL;
  struct netconfig *autofs_ncp;
  char *conftype = "ticotsord";

  autofs_ncp = getnetconfigent(conftype);
  if (autofs_ncp == NULL) {
    plog(XLOG_ERROR, "create_autofs_service: cannot getnetconfigent for %s", conftype);
    goto out;
  }

  fd = t_open(autofs_ncp->nc_device, O_RDWR, NULL);
  if (fd < 0) {
    plog(XLOG_ERROR, "create_autofs_service: t_open failed (%s)",
	 t_errlist[t_errno]);
    goto out;
  }

  tbp = (struct t_bind *) t_alloc(fd, T_BIND, T_ADDR);
  if (!tbp) {
    plog(XLOG_ERROR, "create_autofs_service: t_alloca failed");
    goto out;
  }

  if (get_autofs_address(autofs_ncp, tbp) != 0) {
    plog(XLOG_ERROR, "create_autofs_service: get_autofs_address failed");
    goto out;
  }

  autofs_xprt = svc_tli_create(fd, autofs_ncp, tbp, 0, 0);
  if (autofs_xprt == NULL) {
    plog(XLOG_ERROR, "cannot create autofs tli service for amd");
    goto out;
  }

  rpcb_unset(AUTOFS_PROG, AUTOFS_VERS, autofs_ncp);
  if (svc_reg(autofs_xprt, AUTOFS_PROG, AUTOFS_VERS, autofs_program_2, autofs_ncp) == FALSE) {
    plog(XLOG_ERROR, "could not register amd AUTOFS service");
    goto out;
  }
  err = 0;
  goto really_out;

out:
  if (autofs_ncp)
    freenetconfigent(autofs_ncp);
  if (autofs_xprt)
    SVC_DESTROY(autofs_xprt);
  else {
    if (fd > 0)
      t_close(fd);
  }

really_out:
  if (tbp)
    t_free((char *) tbp, T_BIND);

  dlog("create_autofs_service: returning %d\n", err);
  return err;
}

/* XXX not implemented */
int
autofs_link_mount(am_node *mp)
{
  return EACCES;
}

/* XXX not implemented */
int
autofs_umount_succeeded(am_node *mp)
{
  plog(XLOG_INFO, "autofs: unmounting %s succeeded", mp->am_path);
  return 0;
}

/* XXX not implemented */
void
autofs_mount_succeeded(am_node *mp)
{
  plog(XLOG_INFO, "autofs: mounting %s succeeded", mp->am_path);
}

void
autofs_mount_failed(am_node *mp)
{
  plog(XLOG_INFO, "autofs: mounting %s failed", mp->am_path);
}

void
autofs_lookup_failed(am_node *mp, char *name)
{
  plog(XLOG_INFO, "autofs: lookup of %s failed", name);
}

void
autofs_get_opts(char *opts, autofs_fh_t *fh)
{
  sprintf(opts, "%sdirect",
	  fh->direct ? "" : "in");
}
#endif

#ifdef __linux__

/* FIXME: 256 below should be replaced with a system-specific
   value for max filedescriptors */
#define AUTOFS_MAX_FDS 256
#define AUTOFS_MIN_PROTO_VERSION 3
#define AUTOFS_MAX_PROTO_VERSION 4

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

#endif /* __linux__ */
#endif /* HAVE_FS_AUTOFS */
