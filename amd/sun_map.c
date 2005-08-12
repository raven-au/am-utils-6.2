/*
 * Copyright (c) 1997-2005 Erez Zadok
 * Copyright (c) 2005 Daniel P. Ottavio
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
 * File: am-utils/amd/sun_map.c
 *
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* HAVE_CONFIG_H */
#include <am_defs.h>
#include <amd.h>
#include <sun_map.h>



/*
 * Add a data pointer to the end of the list.
 */
void
sun_list_add(struct sun_list *list, qelem *item)
{
  if (list->last == NULL) {
    list->last = item;
    list->first = item;
    item->q_back = NULL;
  }
  else {
    list->last->q_forw = item;
    item->q_back = list->last;
    list->last = item;
  }

  item->q_forw = NULL;
}


/*
 * Sun2Amd conversion routines
 */

/*
 * A set of string Sun fstypes.
 */
#define SUN_NFS_TYPE     "nfs"
#define SUN_HSFS_TYPE    "hsfs" /* CD fs */
#define SUN_AUTOFS_TYPE  "autofs"
#define SUN_CACHEFS_TYPE "cachefs"


/*
 * Convert the list of Sun mount options to Amd mount options.  The
 * result is concatenated to dest.
 */
static void
sun_opts2amd(char *dest, size_t destlen, const struct sun_opt *opt_list)
{
  const struct sun_opt *opt;

  xstrlcat(dest,"opts:=", destlen);

  /* Iterate through each option and append it to the buffer. */
  for(opt = opt_list; opt != NULL; opt = NEXT(struct sun_opt, opt)) {
    xstrlcat(dest, opt->str, destlen);
    if(NEXT(struct sun_opt, opt) != NULL) {
      xstrlcat(dest, ",", destlen);
    }
  }
  xstrlcat(dest, ";", destlen);
}


/*
 * Convert the list of Sun mount locations to a list of Amd mount
 * locations.  The result is concatenated to dest.
 */
static void
sun_locations2amd(char *dest,
		  size_t destlen,
		  const struct sun_location *local_list)
{
  const struct sun_location *local;
  const struct sun_host *host;

  for (local = local_list;
       local != NULL;
       local = NEXT(struct sun_location,local)) {
    /*
     * Check to see if the list of hosts is empty.  Some mount types
     * i.e cd-rom may have mount location with no host.
     */
    if (local->host_list != NULL) {
      /* Write each host that belongs to this location. */
      for (host = local->host_list;
	   host != NULL;
	   host = NEXT(struct sun_host, host)) {
	xstrlcat(dest, "rhost:=", destlen);
	xstrlcat(dest, host->name, destlen);
	xstrlcat(dest, ";", destlen);
	xstrlcat(dest, "rfs:=", destlen);
	xstrlcat(dest, local->path, destlen);
	xstrlcat(dest, ";", destlen);
	if (NEXT(struct sun_host, host) != NULL) {
	  /* add a space to seperate each host listing */
	  xstrlcat(dest, " ", destlen);
	}
      }
    }
    else {
      /* no host location */
      xstrlcat(dest,"fs:=", destlen);
      xstrlcat(dest,local->path, destlen);
    }
  }
}


/*
 * Convert a Sun NFS automount entry to an Amd.  The result is concatenated
 * into dest.
 */
static void
sun_nfs2amd(char *dest, size_t destlen, const struct sun_entry *s_entry)
{
  /*
   * If the Sun entry has muliple mount points we use type:=auto along
   * with auto entries.  For a single mount point just use type:=nfs.
   */
  if (s_entry->mountpt_list == NULL) {
    /* single NFS mount point */
    xstrlcat(dest, "type:=nfs;", destlen);
    if (s_entry->location_list != NULL) {
      /* write out the list of mountpoint locations */
      sun_locations2amd(dest, destlen, s_entry->location_list);
    }
  }
}


/*
 * Convert the sun_entry into an Amd equivalent string.
 *
 * return - Adm entry string on success, null on error.
 */
char *
sun_entry2amd(const char *a_entry)
{
  char *retval = NULL;
  char line_buff[INFO_MAX_LINE_LEN];
  struct sun_entry *s_entry;

  /* Parse the sun entry line. */
  s_entry = sun_map_parse_read(a_entry);
  if (s_entry == NULL) {
    plog(XLOG_ERROR,"could not parse Sun style map");
    goto err;
  }

  memset(line_buff, 0, sizeof(line_buff));

  if (s_entry->opt_list != NULL) {
    /* write the mount options to the buffer  */
    sun_opts2amd(line_buff, sizeof(line_buff), s_entry->opt_list);
  }

  /* Check the fstype. */
  if (s_entry->fstype != NULL) {
    if (NSTREQ(s_entry->fstype, SUN_NFS_TYPE, strlen(SUN_NFS_TYPE))) {
      /* NFS Type */
      sun_nfs2amd(line_buff, sizeof(line_buff), s_entry);
    }
    else if (NSTREQ(s_entry->fstype, SUN_HSFS_TYPE, strlen(SUN_HSFS_TYPE))) {
      /* HSFS Type (CD fs) */

    }
    else if (NSTREQ(s_entry->fstype, SUN_AUTOFS_TYPE, strlen(SUN_AUTOFS_TYPE))) {
      /* AutoFS Type */

    }
    else if (NSTREQ(s_entry->fstype, SUN_CACHEFS_TYPE, strlen(SUN_CACHEFS_TYPE))) {
      /* CacheFS Type */

    }
    else {
      plog(XLOG_ERROR, "Sun fstype %s is currently not supported by Amd.",
	   s_entry->fstype);
      goto err;
    }
  }
  else {
    plog(XLOG_INFO, "No SUN fstype specified defaulting to NFS.");
  }


 err:
  /*
   * XXX: For now this function will just pass back a copy of the sun
   * entry.
   */
  if (retval == NULL) {
    retval = strdup(a_entry);
  }
  return retval;
}
