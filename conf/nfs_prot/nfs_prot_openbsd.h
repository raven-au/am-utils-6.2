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
 * $Id: nfs_prot_openbsd.h,v 1.11 2003/10/16 05:03:27 ezk Exp $
 *
 */

#ifndef _AMU_NFS_PROT_H
#define _AMU_NFS_PROT_H

#ifdef HAVE_RPCSVC_NFS_PROT_H
# include <rpcsvc/nfs_prot.h>
#endif /* HAVE_RPCSVC_NFS_PROT_H */
#ifdef HAVE_NFS_RPCV2_H
# include <nfs/rpcv2.h>
#endif /* HAVE_NFS_RPCV2_H */

#ifndef NFS_NPROCS
# define NFS_NPROCS	26
#endif /* not NFS_NPROCS */
#ifdef HAVE_NFS_NFS_H
# include <nfs/nfs.h>
#endif /* HAVE_NFS_NFS_H */

#ifdef	HAVE_UFS_UFS_UFSMOUNT_H
# ifdef HAVE_UFS_UFS_EXTATTR_H
/*
 * Need to define _KERNEL to include protected definition of struct
 * ufs_extattr_per_mount, which is used in struct ufsmount in
 * <ufs/ufs/ufsmount.h>, but is NOT protected by _KERNEL there.
 */
#  define _KERNEL
#  include <ufs/ufs/extattr.h>
#  undef _KERNEL
# endif /* HAVE_UFS_UFS_EXTATTR_H */
# ifndef MAXQUOTAS
#  define MAXQUOTAS     2
# endif /* not MAXQUOTAS */
/*
 * XXX: fake struct nextexport: trying to include proper headers here is too
 * difficult: those headers drag many other headers, etc.
 */
struct netexport { int this_is_SO_wrong; };
# include <ufs/ufs/ufsmount.h>
#endif	/* HAVE_UFS_UFS_UFSMOUNT_H */


/*
 * MACROS:
 */
#define	dr_drok_u	diropres
#define ca_where	where
#define da_fhandle	dir
#define da_name		name
#define dl_entries	entries
#define dl_eof		eof
#define dr_status	status
#define dr_u		diropres_u
#define drok_attributes	attributes
#define drok_fhandle	file
#define fh_data		data
#define la_fhandle	from
#define la_to		to
#define na_atime	atime
#define na_ctime	ctime
#define na_fileid	fileid
#define na_fsid		fsid
#define na_mode		mode
#define na_mtime	mtime
#define na_nlink	nlink
#define na_size		size
#define na_type		type
#define na_uid		uid
#define na_gid		gid
#define ne_cookie	cookie
#define ne_fileid	fileid
#define ne_name		name
#define ne_nextentry	nextentry
#define ns_attr_u	attributes
#define ns_status	status
#define ns_u		attrstat_u
#define nt_seconds	seconds
#define nt_useconds	useconds
#define rda_cookie	cookie
#define rda_count	count
#define rda_fhandle	dir
#define rdr_reply_u	reply
#define rdr_status	status
#define rdr_u		readdirres_u
#define rlr_data_u	data
#define rlr_status	status
#define rlr_u		readlinkres_u
#define rna_from	from
#define rna_to		to
#define rr_status	status
#define sag_fhandle	file
#define sfr_reply_u	reply
#define sfr_status	status
#define sfr_u		statfsres_u
#define sfrok_bavail	bavail
#define sfrok_bfree	bfree
#define sfrok_blocks	blocks
#define sfrok_bsize	bsize
#define sfrok_tsize	tsize
#define sla_from	from
#define wra_fhandle	file


/*
 * TYPEDEFS:
 */
typedef attrstat nfsattrstat;
typedef createargs nfscreateargs;
typedef dirlist nfsdirlist;
typedef diropargs nfsdiropargs;
typedef diropres nfsdiropres;
typedef entry nfsentry;
typedef fattr nfsfattr;
typedef ftype nfsftype;
typedef linkargs nfslinkargs;
typedef readargs nfsreadargs;
typedef readdirargs nfsreaddirargs;
typedef readdirres nfsreaddirres;
typedef readlinkres nfsreadlinkres;
typedef readres nfsreadres;
typedef renameargs nfsrenameargs;
typedef sattrargs nfssattrargs;
typedef statfsokres nfsstatfsokres;
typedef statfsres nfsstatfsres;
typedef symlinkargs nfssymlinkargs;
typedef writeargs nfswriteargs;


/*
 * OpenBSD 2.2 has NFS V3, but it is undefined in the header files.
 * so I define everything that's needed for NFS V3 here.
 */
#ifdef NFSMNT_NFSV3

/*
 * turn of this function so it cannot be used!  OpenBSD seems to
 * require that NFS sockets be connected.
 * See m4/macros/check_nfs_socket_connection.m4.
 */

# define MOUNT_NFS3 MOUNT_NFS
# define MNTOPT_NFS3 "nfs"

#define FHSIZE3 64		/* size in bytes of a file handle (v3) */
#define	NFS3_FHSIZE 64
#define	MOUNTVERS3 ((unsigned long)(3))
#define	NFS_V3 ((unsigned long)(3))

typedef struct {
  u_int fhandle3_len;
  char *fhandle3_val;
} fhandle3;

enum mountstat3 {
  MNT3_OK = 0,
  MNT3ERR_PERM = 1,
  MNT3ERR_NOENT = 2,
  MNT3ERR_IO = 5,
  MNT3ERR_ACCES = 13,
  MNT3ERR_NOTDIR = 20,
  MNT3ERR_INVAL = 22,
  MNT3ERR_NAMETOOLONG = 63,
  MNT3ERR_NOTSUPP = 10004,
  MNT3ERR_SERVERFAULT = 10006
};
typedef enum mountstat3 mountstat3;

struct mountres3_ok {
  fhandle3 fhandle;
  struct {
    u_int auth_flavors_len;
    int *auth_flavors_val;
  } auth_flavors;
};
typedef struct mountres3_ok mountres3_ok;

struct mountres3 {
  mountstat3 fhs_status;
  union {
    mountres3_ok mountinfo;
  } mountres3_u;
};
typedef struct mountres3 mountres3;

struct nfs_fh3 {
  u_int fh3_length;
  union nfs_fh3_u {
    struct nfs_fh3_i {
      fhandle_t fh3_i;
    } nfs_fh3_i;
    char data[NFS3_FHSIZE];
  } fh3_u;
};
typedef struct nfs_fh3 nfs_fh3;

#define NFSPROC3_LOOKUP ((u_long) 3)
enum nfsstat3 {
	NFS3_OK = 0,
	NFS3ERR_PERM = 1,
	NFS3ERR_NOENT = 2,
	NFS3ERR_IO = 5,
	NFS3ERR_NXIO = 6,
	NFS3ERR_ACCES = 13,
	NFS3ERR_EXIST = 17,
	NFS3ERR_XDEV = 18,
	NFS3ERR_NODEV = 19,
	NFS3ERR_NOTDIR = 20,
	NFS3ERR_ISDIR = 21,
	NFS3ERR_INVAL = 22,
	NFS3ERR_FBIG = 27,
	NFS3ERR_NOSPC = 28,
	NFS3ERR_ROFS = 30,
	NFS3ERR_MLINK = 31,
	NFS3ERR_NAMETOOLONG = 63,
	NFS3ERR_NOTEMPTY = 66,
	NFS3ERR_DQUOT = 69,
	NFS3ERR_STALE = 70,
	NFS3ERR_REMOTE = 71,
	NFS3ERR_BADHANDLE = 10001,
	NFS3ERR_NOT_SYNC = 10002,
	NFS3ERR_BAD_COOKIE = 10003,
	NFS3ERR_NOTSUPP = 10004,
	NFS3ERR_TOOSMALL = 10005,
	NFS3ERR_SERVERFAULT = 10006,
	NFS3ERR_BADTYPE = 10007,
	NFS3ERR_JUKEBOX = 10008
};
typedef enum nfsstat3 nfsstat3;

typedef char *filename3;

struct diropargs3 {
	am_nfs_fh3 dir;
	filename3 name;
};
typedef struct diropargs3 diropargs3;

struct LOOKUP3args {
	diropargs3 what;
};
typedef struct LOOKUP3args LOOKUP3args;

struct LOOKUP3resok {
	am_nfs_fh3 object;
#if 0
	post_op_attr obj_attributes;
	post_op_attr dir_attributes;
#endif
};
typedef struct LOOKUP3resok LOOKUP3resok;

struct LOOKUP3resfail {
#if 0
	post_op_attr dir_attributes;
#endif
};
typedef struct LOOKUP3resfail LOOKUP3resfail;

struct LOOKUP3res {
	nfsstat3 status;
	union {
		LOOKUP3resok ok;
		LOOKUP3resfail fail;
	} res_u;
};
typedef struct LOOKUP3res LOOKUP3res;

#endif /* NFSMNT_NFSV3 */

#endif /* not _AMU_NFS_PROT_H */
