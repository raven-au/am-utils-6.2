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
 * $Id: autofs_solaris2.c,v 1.1 2000/11/28 06:36:51 ib42 Exp $
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

/* forward declarations */
bool_t xdr_postmountreq(XDR *xdrs, postmountreq *objp);
bool_t xdr_postmountres(XDR *xdrs, postmountres *objp);
bool_t xdr_umntrequest(XDR *xdrs, umntrequest *objp);
bool_t xdr_umntres(XDR *xdrs, umntres *objp);
bool_t xdr_postumntreq(XDR *xdrs, postumntreq *objp);
bool_t xdr_postumntres(XDR *xdrs, postumntres *objp);
bool_t xdr_autofs_lookupargs(XDR *xdrs, autofs_lookupargs *objp);
bool_t xdr_autofs_mountres(XDR *xdrs, autofs_mountres *objp);
bool_t xdr_autofs_lookupres(XDR *xdrs, autofs_lookupres *objp);


/*
 * AUTOFS XDR FUNCTIONS:
 */
bool_t
xdr_autofs_stat(XDR *xdrs, autofs_stat *objp)
{
  if (!xdr_enum(xdrs, (enum_t *)objp))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_autofs_action(XDR *xdrs, autofs_action *objp)
{
  if (!xdr_enum(xdrs, (enum_t *)objp))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_linka(XDR *xdrs, linka *objp)
{
  if (!xdr_string(xdrs, &objp->dir, AUTOFS_MAXPATHLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->link, AUTOFS_MAXPATHLEN))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_autofs_netbuf(XDR *xdrs, struct netbuf *objp)
{
  bool_t dummy;

  if (!xdr_u_long(xdrs, (u_long *) &objp->maxlen))
    return (FALSE);
  dummy = xdr_bytes(xdrs, (char **)&(objp->buf),
		    (u_int *)&(objp->len), objp->maxlen);
  return (dummy);
}

bool_t
xdr_autofs_args(XDR *xdrs, autofs_args *objp)
{
  if (!xdr_autofs_netbuf(xdrs, &objp->addr))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->path, AUTOFS_MAXPATHLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->opts, AUTOFS_MAXOPTSLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->map, AUTOFS_MAXPATHLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->subdir, AUTOFS_MAXPATHLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->key, AUTOFS_MAXCOMPONENTLEN))
    return (FALSE);
  if (!xdr_int(xdrs, &objp->mount_to))
    return (FALSE);
  if (!xdr_int(xdrs, &objp->rpc_to))
    return (FALSE);
  if (!xdr_int(xdrs, &objp->direct))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_mounta(XDR *xdrs, struct mounta *objp)
{
  if (!xdr_string(xdrs, &objp->spec, AUTOFS_MAXPATHLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->dir, AUTOFS_MAXPATHLEN))
    return (FALSE);
  if (!xdr_int(xdrs, &objp->flags))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->fstype, AUTOFS_MAXCOMPONENTLEN))
    return (FALSE);
  if (!xdr_pointer(xdrs, (char **)&objp->dataptr, sizeof (autofs_args),
		   (XDRPROC_T_TYPE) xdr_autofs_args))
    return (FALSE);
  if (!xdr_int(xdrs, &objp->datalen))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_action_list_entry(XDR *xdrs, action_list_entry *objp)
{
  if (!xdr_autofs_action(xdrs, &objp->action))
    return (FALSE);
  switch (objp->action) {
  case AUTOFS_MOUNT_RQ:
    if (!xdr_mounta(xdrs, &objp->action_list_entry_u.mounta))
      return (FALSE);
    break;
  case AUTOFS_LINK_RQ:
    if (!xdr_linka(xdrs, &objp->action_list_entry_u.linka))
      return (FALSE);
    break;
  default:
    break;
  }
  return (TRUE);
}

bool_t
xdr_action_list(XDR *xdrs, action_list *objp)
{
  if (!xdr_action_list_entry(xdrs, &objp->action))
    return (FALSE);
  if (!xdr_pointer(xdrs, (char **)&objp->next, sizeof (action_list),
		   (XDRPROC_T_TYPE) xdr_action_list))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_postmountreq(XDR *xdrs, postmountreq *objp)
{
  if (!xdr_string(xdrs, &objp->special, AUTOFS_MAXPATHLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->mountp, AUTOFS_MAXPATHLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->fstype, AUTOFS_MAXCOMPONENTLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->mntopts, AUTOFS_MAXOPTSLEN))
    return (FALSE);
  if (!xdr_dev_t(xdrs, &objp->devid))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_postmountres(XDR *xdrs, postmountres *objp)
{
  if (!xdr_int(xdrs, &objp->status))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_umntrequest(XDR *xdrs, umntrequest *objp)
{
  amuDebug(D_XDRTRACE)
    plog(XLOG_DEBUG, "xdr_umntrequest:");

  if (!xdr_bool_t(xdrs, &objp->isdirect))
    return (FALSE);
  if (!xdr_dev_t(xdrs, &objp->devid))
    return (FALSE);
  if (!xdr_dev_t(xdrs, &objp->rdevid))
    return (FALSE);
  if (!xdr_pointer(xdrs, (char **) &objp->next, sizeof(umntrequest),
		   (XDRPROC_T_TYPE) xdr_umntrequest))
    return (FALSE);

  return (TRUE);
}


bool_t
xdr_umntres(XDR *xdrs, umntres *objp)
{
  amuDebug(D_XDRTRACE)
    plog(XLOG_DEBUG, "xdr_mntres:");

  if (!xdr_int(xdrs, &objp->status))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_postumntreq(XDR *xdrs, postumntreq *objp)
{
  if (!xdr_dev_t(xdrs, &objp->devid))
    return (FALSE);
  if (!xdr_dev_t(xdrs, &objp->rdevid))
    return (FALSE);
  if (!xdr_pointer(xdrs, (char **)&objp->next,
		   sizeof (struct postumntreq),
		   (XDRPROC_T_TYPE) xdr_postumntreq))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_postumntres(XDR *xdrs, postumntres *objp)
{
  if (!xdr_int(xdrs, &objp->status))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_autofs_res(XDR *xdrs, autofs_res *objp)
{
  if (!xdr_enum(xdrs, (enum_t *)objp))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_autofs_lookupargs(XDR *xdrs, autofs_lookupargs *objp)
{
  amuDebug(D_XDRTRACE)
    plog(XLOG_DEBUG, "xdr_mntrequest:");

  if (!xdr_string(xdrs, &objp->map, AUTOFS_MAXPATHLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->path, AUTOFS_MAXPATHLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->name, AUTOFS_MAXCOMPONENTLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->subdir, AUTOFS_MAXPATHLEN))
    return (FALSE);
  if (!xdr_string(xdrs, &objp->opts, AUTOFS_MAXOPTSLEN))
    return (FALSE);
  if (!xdr_bool_t(xdrs, &objp->isdirect))
    return (FALSE);
  return (TRUE);
}


bool_t
xdr_mount_result_type(XDR *xdrs, mount_result_type *objp)
{
  if (!xdr_autofs_stat(xdrs, &objp->status))
    return (FALSE);
  switch (objp->status) {
  case AUTOFS_ACTION:
    if (!xdr_pointer(xdrs,
		     (char **)&objp->mount_result_type_u.list,
		     sizeof (action_list), (XDRPROC_T_TYPE) xdr_action_list))
      return (FALSE);
    break;
  case AUTOFS_DONE:
    if (!xdr_int(xdrs, &objp->mount_result_type_u.error))
      return (FALSE);
    break;
  }
  return (TRUE);
}

bool_t
xdr_autofs_mountres(XDR *xdrs, autofs_mountres *objp)
{
  amuDebug(D_XDRTRACE)
    plog(XLOG_DEBUG, "xdr_mntres:");

  if (!xdr_mount_result_type(xdrs, &objp->mr_type))
    return (FALSE);
  if (!xdr_int(xdrs, &objp->mr_verbose))
    return (FALSE);

  return (TRUE);
}

bool_t
xdr_lookup_result_type(XDR *xdrs, lookup_result_type *objp)
{
  if (!xdr_autofs_action(xdrs, &objp->action))
    return (FALSE);
  switch (objp->action) {
  case AUTOFS_LINK_RQ:
    if (!xdr_linka(xdrs, &objp->lookup_result_type_u.lt_linka))
      return (FALSE);
    break;
  default:
    break;
  }
  return (TRUE);
}

bool_t
xdr_autofs_lookupres(XDR *xdrs, autofs_lookupres *objp)
{
  if (!xdr_autofs_res(xdrs, &objp->lu_res))
    return (FALSE);
  if (!xdr_lookup_result_type(xdrs, &objp->lu_type))
    return (FALSE);
  if (!xdr_int(xdrs, &objp->lu_verbose))
    return (FALSE);
  return (TRUE);
}

#if 0
bool_t
xdr_autofs_rddirargs(XDR *xdrs, autofs_rddirargs *objp)
{
  if (!xdr_string(xdrs, &objp->rda_map, AUTOFS_MAXPATHLEN))
    return (FALSE);
  if (!xdr_u_long(xdrs, &objp->rda_offset))
    return (FALSE);
  if (!xdr_u_long(xdrs, &objp->rda_count))
    return (FALSE);
  return (TRUE);
}

bool_t
xdr_autofs_rddirres(XDR *xdrs, autofs_rddirres *objp)
{
  if (!xdr_enum(xdrs, (enum_t *)&objp->rd_status))
    return (FALSE);
  if (objp->rd_status != AUTOFS_OK)
    return (TRUE);
  if (xdrs->x_op == XDR_ENCODE)
    return (xdr_autofs_putrddirres(
				   xdrs, (struct autofsrddir *)&objp->rd_rddir,
				   objp->rd_bufsize));
  else if (xdrs->x_op == XDR_DECODE)
    return (xdr_autofs_getrddirres(xdrs,
				   (struct autofsrddir *)&objp->rd_rddir));
  else return (FALSE);
}
#endif


/* XXX not implemented */
static int
autofs_lookup_2_req(autofs_lookupargs *m,
		    autofs_lookupres *res,
		    struct authunix_parms *cred)
{
  int err = AUTOFS_OK;

  dlog("LOOKUP REQUEST: name=%s[%s] map=%s opts=%s path=%s direct=%d\n",
       m->name, m->subdir, m->map, m->opts,
       m->path, m->isdirect);

  res->lu_res = err;
  res->lu_verbose = 1;

  dlog("LOOKUP REPLY    : status=%d\n", res->lu_res);
  return 0;
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
static int
autofs_mount_2_req(autofs_lookupargs *m,
		   autofs_mountres *res,
		   struct authunix_parms *cred)
{
  int err = AUTOFS_OK;
  am_node *mp, *ap;
  mntfs *mf;

  dlog("MOUNT REQUEST: name=%s[%s] map=%s opts=%s path=%s direct=%d\n",
       m->name, m->subdir, m->map, m->opts,
       m->path, m->isdirect);

  mp = find_ap(m->path);
  if (!mp) {
    plog(XLOG_ERROR, "map %s not found", m->path);
    err = AUTOFS_NOENT;
    goto out;
  }

  mf = mp->am_mnt;
  ap = mf->mf_ops->lookuppn(mp, m->name, &err, VLOOK_CREATE);
  if (ap == NULL) {
    if (err < 0) {
      /* we're working on it */
      err = 0;
      res->mr_type.status = AUTOFS_ACTION;
      res->mr_type.mount_result_type_u.list = NULL;
      goto out2;
    }
    err = AUTOFS_NOENT;
  }

out:
  res->mr_type.status = AUTOFS_DONE;
  res->mr_type.mount_result_type_u.error = err;
out2:
  res->mr_verbose = 1;

  switch (res->mr_type.status) {
  case AUTOFS_ACTION:
    dlog("MOUNT REPLY    : status=%d, AUTOFS_ACTION\n",
	 err);
    break;
  case AUTOFS_DONE:
    dlog("MOUNT REPLY    : status=%d, AUTOFS_DONE\n",
	 err);
    break;
  default:
    dlog("MOUNT REPLY    : status=%d, UNKNOWN\n",
	 err);
  }

  if (err) {
    if (m->isdirect) {
      /* direct mount */
      plog(XLOG_ERROR, "mount of %s failed", m->path);
    } else {
      /* indirect mount */
      plog(XLOG_ERROR, "mount of %s/%s failed", m->path, m->name);
    }
  }
  return 0;
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
static int
autofs_postmount_2_req(postmountreq *req,
		       postmountres *res,
		       struct authunix_parms *cred)
{
  dlog("POSTMOUNT REQUEST: %s\tdev=%lx\tspecial=%s %s\n",
       req->mountp, req->devid, req->special, req->mntopts);

  res->status = 0;

  dlog("POSTMOUNT REPLY  : status=%d\n", res->status);
  return 0;
}

/* XXX not implemented */
static int
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
  return 0;
}

/* XXX not implemented */
static int
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
  return 0;
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
  int ret;

  bool_t (*xdr_argument)();
  bool_t (*xdr_result)();
  int (*local)();
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
  ret = (*local) (&argument, &result, rqstp->rq_clntcred);

  /* send reply only if the RPC method returned 0 */
  if (!ret) {
    if (!svc_sendreply(transp,
		       (XDRPROC_T_TYPE) xdr_result,
		       (SVC_IN_ARG_TYPE) &result)) {
      svcerr_systemerr(transp);
    }
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

#endif /* HAVE_FS_AUTOFS */
