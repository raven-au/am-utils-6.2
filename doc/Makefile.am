## Process this file with automake to produce Makefile.in

# Package: am-utils
# Level: Makefile for doc/ directory
# Author: Erez Zadok

@SET_MAKE@

info_TEXINFOS = am-utils.texi

EXTRA_DIST = hlfsd.ps

# force generation of 'letter' format PS and embed Type-3 fonts
# (our European allies may object...)
DVIPS = dvips -t letter -Pcmz

# Force version.texi to be rebuilt regardless of maintainer mode
$(srcdir)/version.texi: am-utils.texi $(top_srcdir)/configure.in
	@(set `$(SHELL) $(srcdir)/mdate-sh $(srcdir)/am-utils.texi`; \
	echo "@set UPDATED $$1 $$2 $$3"; \
	echo "@set UPDATED-MONTH $$2 $$3"; \
	echo "@set EDITION $(VERSION)"; \
	echo "@set VERSION $(VERSION)") > vti.tmp
	@cmp -s vti.tmp $(srcdir)/version.texi \
	  || (echo "Updating $(srcdir)/version.texi"; \
	      cp vti.tmp $(srcdir)/version.texi)
	-@rm -f vti.tmp

# include dvi/ps in distribution
dist-hook: am-utils.dvi am-utils.ps
	cp -p $^ $(distdir)

# a special target for making Postscript files
ps: am-utils.dvi am-utils.ps

am-utils-clean: clean maintainer-clean-aminfo maintainer-clean-vti

# personal rule for Erez Zadok to generate and install html docs
alldocs: ps install-NEWS install-INSTALL install-ChangeLog install-AUTHORS install-tasks install-autofs install-y2k install-BUGS install-html install-ps

install-html: $(HOME)/html/am-utils/am-utils_1.html
$(HOME)/html/am-utils/am-utils_1.html: am-utils.texi
	texi2html -frames -Verbose -expand info -number -split chapter $<
	mv *.html $(HOME)/html/am-utils
	chmod -R a+r $(HOME)/html/am-utils

install-ps: $(HOME)/html/am-utils/am-utils.ps
$(HOME)/html/am-utils/am-utils.ps: am-utils.ps
	install -c -m 644 $< $@
	(cd $(HOME)/html/research && ${MAKE} ${MFLAGS})

install-NEWS: $(HOME)/html/am-utils/NEWS.txt
$(HOME)/html/am-utils/NEWS.txt: $(top_srcdir)/NEWS
	install -c -m 644 $< $@

install-INSTALL: $(HOME)/html/am-utils/INSTALL.txt
$(HOME)/html/am-utils/INSTALL.txt: $(top_srcdir)/INSTALL
	install -c -m 644 $< $@

install-ChangeLog: $(HOME)/html/am-utils/ChangeLog.txt
$(HOME)/html/am-utils/ChangeLog.txt: $(top_srcdir)/ChangeLog
	install -c -m 644 $< $@

install-AUTHORS: $(HOME)/html/am-utils/AUTHORS.txt
$(HOME)/html/am-utils/AUTHORS.txt: $(top_srcdir)/AUTHORS
	install -c -m 644 $< $@

install-tasks: $(HOME)/html/am-utils/tasks.txt
$(HOME)/html/am-utils/tasks.txt: $(top_srcdir)/tasks
	install -c -m 644 $< $@

install-autofs: $(HOME)/html/am-utils/autofs.txt
$(HOME)/html/am-utils/autofs.txt: $(top_srcdir)/README.autofs
	install -c -m 644 $< $@

install-y2k: $(HOME)/html/am-utils/y2k.txt
$(HOME)/html/am-utils/y2k.txt: $(top_srcdir)/README.y2k
	install -c -m 644 $< $@

install-BUGS: $(HOME)/html/am-utils/BUGS.txt
$(HOME)/html/am-utils/BUGS.txt: $(top_srcdir)/BUGS
	install -c -m 644 $< $@
