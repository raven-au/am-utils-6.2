dnl my version is similar to the one from Autoconf 2.12, but I also
dnl define HAVE_BAD_MEMCMP so that I can smarter things to avoid
dnl linkage conflicts with bad memcmp versions that are in libc.
AC_DEFUN(AMU_FUNC_BAD_MEMCMP,
[AC_CACHE_CHECK(for 8-bit clean memcmp, ac_cv_func_memcmp_clean,
[AC_TRY_RUN([
main()
{
  char c0 = 0x40, c1 = 0x80, c2 = 0x81;
  exit(memcmp(&c0, &c2, 1) < 0 && memcmp(&c1, &c2, 1) < 0 ? 0 : 1);
}
], ac_cv_func_memcmp_clean=yes, ac_cv_func_memcmp_clean=no,
ac_cv_func_memcmp_clean=no)])
if test $ac_cv_func_memcmp_clean = no
then
  LIBOBJS="$LIBOBJS memcmp.o"
  AC_DEFINE(HAVE_BAD_MEMCMP)
fi
dnl AC_SUBST(LIBOBJS)dnl
])
