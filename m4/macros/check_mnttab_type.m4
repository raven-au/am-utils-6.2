dnl ######################################################################
dnl check the string type of the name of a filesystem mount table entry.
dnl Usage: AC_CHECK_MNTTAB_TYPE(<fs>, [fssymbol])
dnl Check if there is an entry for MNTTYPE_<fs> in sys/mntent.h and mntent.h
dnl define MNTTAB_TYPE_<fs> to the string name (e.g., "nfs").  If <fssymbol>
dnl exist, then define MNTTAB_TYPE_<fssymbol> instead.  If <fssymbol> is
dnl defined, then <fs> can be a list of fs strings to look for.
dnl If no symbols have been defined, but the filesystem has been found
dnl earlier, then set the mount-table type to "<fs>" anyway...
AC_DEFUN([AMU_CHECK_MNTTAB_TYPE],
[
# find what name to give to the fs
if test -n "$2"
then
  ac_fs_name=$2
else
  ac_fs_name=$1
fi
# store variable name of fs
ac_upcase_fs_name=`echo $ac_fs_name | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
ac_safe=MNTTAB_TYPE_$ac_upcase_fs_name
# check for cache and set it if needed
AMU_CACHE_CHECK_DYNAMIC(for mnttab name for $ac_fs_name filesystem,
ac_cv_mnttab_type_$ac_fs_name,
[
# undefine by default
eval "ac_cv_mnttab_type_$ac_fs_name=notfound"
# and look to see if it was found
for ac_fs_tmp in $1
do
  if test "$ac_fs_tmp" = "nfs3" -a "$ac_cv_fs_nfs3" = "yes" -a "$ac_cv_header_linux_nfs_h" = "yes"
  then
    eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$ac_cv_mnttab_type_nfs\\\""
    break
  fi

  ac_upcase_fs_symbol=`echo $ac_fs_tmp | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ' | tr -d '.'`

  # first look for MNTTYPE_*
  AC_EGREP_CPP(yes,
  AMU_MOUNT_HEADERS(
  [
#ifdef MNTTYPE_$ac_upcase_fs_symbol
    yes
#endif /* MNTTYPE_$ac_upcase_fs_symbol */
  ]),
  [ eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$ac_fs_tmp\\\""
  ])
  # check if need to terminate "for" loop
  if test "`eval echo '$''{ac_cv_mnttab_type_'$ac_fs_name'}'`" != notfound
  then
    break
  fi

  # look for a loadable filesystem module (linux)
  if test -f /lib/modules/$host_os_version/fs/$ac_fs_tmp.ko
  then
    eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$ac_fs_tmp\\\""
    break
  fi
  if test -f /lib/modules/$host_os_version/fs/$ac_fs_tmp.o
  then
    eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$ac_fs_tmp\\\""
    break
  fi

  # look for a loadable filesystem module (linux 2.4+)
  if test -f /lib/modules/$host_os_version/kernel/fs/$ac_fs_tmp/$ac_fs_tmp.ko
  then
    eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$ac_fs_tmp\\\""
    break
  fi
  if test -f /lib/modules/$host_os_version/kernel/fs/$ac_fs_tmp/$ac_fs_tmp.o
  then
    eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$ac_fs_tmp\\\""
    break
  fi

  # look for a loadable filesystem module (linux redhat-5.1)
  if test -f /lib/modules/preferred/fs/$ac_fs_tmp.ko
  then
    eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$ac_fs_tmp\\\""
    break
  fi
  if test -f /lib/modules/preferred/fs/$ac_fs_tmp.o
  then
    eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$ac_fs_tmp\\\""
    break
  fi

  # next look for statically compiled filesystem (linux)
  if egrep "[[^a-zA-Z0-9_]]$ac_fs_tmp$" /proc/filesystems >/dev/null 2>&1
  then
    eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$ac_fs_tmp\\\""
    break
  fi

  # then try to run a program that derefences a static array (bsd44)
  AMU_EXPAND_RUN_STRING(
  AMU_MOUNT_HEADERS(
  [
#ifndef INITMOUNTNAMES
# error INITMOUNTNAMES not defined
#endif /* not INITMOUNTNAMES */
  ]),
  [
  char const *namelist[] = INITMOUNTNAMES;
  if (argc > 1)
    printf("\"%s\"", namelist[MOUNT_$ac_upcase_fs_symbol]);
  ], [ eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$value\\\""
  ])
  # check if need to terminate "for" loop
  if test "`eval echo '$''{ac_cv_mnttab_type_'$ac_fs_name'}'`" != notfound
  then
    break
  fi

  # then try to run a program that derefences a static array (bsd44)
  AMU_EXPAND_RUN_STRING(
  AMU_MOUNT_HEADERS(
  [
  ]),
  [
  if (argc > 1)
    printf("\"%s\"", MOUNT_$ac_upcase_fs_symbol);
  ], [ eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$value\\\""
  ])
  # check if need to terminate "for" loop
  if test "`eval echo '$''{ac_cv_mnttab_type_'$ac_fs_name'}'`" != notfound
  then
    break
  fi

  # finally run a test program for bsdi3
  AC_TRY_RUN(
  [
#include <sys/param.h>
#include <sys/mount.h>
main()
{
  int i;
  struct vfsconf vf;
  i = getvfsbyname("$ac_fs_tmp", &vf);
  if (i < 0)
    exit(1);
  else
    exit(0);
}
  ], [eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$ac_fs_tmp\\\""
      break
     ]
  )

done

# check if not defined, yet the filesystem is defined
if test "`eval echo '$''{ac_cv_mnttab_type_'$ac_fs_name'}'`" = notfound
then
# this should test if $ac_cv_fs_<fsname> is "yes"
  if test "`eval echo '$''{ac_cv_fs_'$ac_fs_name'}'`" = yes ||
    test "`eval echo '$''{ac_cv_fs_header_'$ac_fs_name'}'`" = yes
  then
    eval "ac_cv_mnttab_type_$ac_fs_name=\\\"$ac_fs_name\\\""
  fi
fi
])
# check if need to define variable
ac_tmp=`eval echo '$''{ac_cv_mnttab_type_'$ac_fs_name'}'`
if test "$ac_tmp" != notfound
then
  AC_DEFINE_UNQUOTED($ac_safe, $ac_tmp)
fi
])
dnl ======================================================================
