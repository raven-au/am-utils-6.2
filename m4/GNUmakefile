#
# GNUmakefile for remaking configuration files.
# Used by am-utils maintainers only!
#
# Erez Zadok <ezk@cs.columbia.edu>
#

AUTOCONF=autoconf --localdir=./aux ./aux/configure.in
AUTOHEADER=autoheader --localdir=./aux ./aux/configure.in
#AUTOMAKE=./aux/automake --include-deps --amdir=./aux/amdir
#AUTOMAKE=automake --include-deps --no-intermediates --localdir=./aux
AUTOMAKE=automake --include-deps --localdir=./aux
MK_ACLOCAL=./mk-aclocal

CUTWARNMSG="warning: AC_TRY_RUN called without default to allow cross compilin"
AMFILES=../Makefile.am $(wildcard ../*/Makefile.am)
INAMFILES=$(AMFILES:.am=.in)
ACFILES=$(wildcard macros/*.m4 macros/HEADER macros/TRAILER aux/aclocal.m4)

LOG=/tmp/amu.log

TOPDIR=..
VPATH=

all: testdir ${TOPDIR}/configure config.h.in ${TOPDIR}/Makefile.in stamp-h.in GNUmakefile

config: all $(TOPDIR)/buildall aux_conf.h.in
	(cd ${TOPDIR} && ./buildall -c)

dconfig: all $(TOPDIR)/buildall aux_conf.h.in
	(cd ${TOPDIR} && ./buildall -C)

qconfig: all $(TOPDIR)/buildall aux_conf.h.in
	(cd ${TOPDIR} && ./buildall -q)

build: all $(TOPDIR)/buildall aux_conf.h.in
	(cd ${TOPDIR} && ./buildall -b)

world: all $(TOPDIR)/buildall aux_conf.h.in
	(cd ${TOPDIR} && ./buildall)

testdir: ${TOPDIR}/amd/amd.c

${TOPDIR}/configure: configure.in aclocal.m4
	-(cd ${TOPDIR} && ${AUTOCONF} > configure.new 2> ${LOG})
	-@egrep -v ${CUTWARNMSG} ${LOG} > ${LOG}.new ; mv ${LOG}.new ${LOG}
	@if test -s ${LOG}; then cat ${LOG}; exit 2; fi
	-mv ../configure ../configure.old
	mv ../configure.new ../configure
	rm -f ../configure.old
	chmod a+rx $@

config.h.in: configure.in acconfig.h
	-(cd ${TOPDIR} && ${AUTOHEADER} > ./aux/config.h.in 2> ${LOG})
	-@egrep -v ${CUTWARNMSG} ${LOG} > ${LOG}.new ; mv ${LOG}.new ${LOG}
	@if test -s ${LOG}; then cat ${LOG}; exit 2; fi

${TOPDIR}/Makefile.in: ${AMFILES} configure.in aclocal.m4
	(cd ${TOPDIR} && ${AUTOMAKE})
	@rm -f ${LOG}

aclocal.m4: ${ACFILES} configure.in
	rm -f $@
	${MK_ACLOCAL} > acinclude.m4
	aclocal

stamp-h.in: ${AMFILES} config.h.in aclocal.m4 aux_conf.h.in
	echo timestamp > $@

clean:
	rm -f ${TOPDIR}/configure config.h.in $(INAMFILES) aclocal.m4 stamp.h.in

