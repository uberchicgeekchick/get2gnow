SUBDIRS = po data

if ENABLE_SK
SUBDIRS += help
endif

SUBDIRS += src

INTLTOOL = 			\
	intltool-extract.in 	\
	intltool-merge.in 	\
	intltool-update.in

EXTRA_DIST = 			\
	ChangeLog               \
	README                  \
	AUTHORS			\
	NEWS			\
	get2gnow.spec		\
	xmldocs.make		\
	omf.make		\
	gnome-doc-utils.make	\
	$(INTLTOOL)

DISTCLEANFILES = 		\
	intltool-extract	\
	intltool-merge		\
	intltool-update		\
	get2gnow.spec		\
	gnome-doc-utils.make

DISTCHECK_CONFIGURE_FLAGS =	\
	--disable-scrollkeeper

all: config.h get2gnow.spec
	$(MAKE) $(AM_MAKEFLAGS) all-recursive

get2gnow.spec: $(top_builddir)/config.status $(top_builddir)/get2gnow.spec.in
	cd $(top_builddir) && $(SHELL) ./config.status $@

# Workaround broken scrollkeeper that doesn't remove its files on
# uninstall.
distuninstallcheck_listfiles = find . -type f -print | grep -v '^\./var/scrollkeeper'

uninstall-debug:
	-gconftool-2 --recursive-unset /schemas/get2gnow-debug
	-gconftool-2 --recursive-unset /apps/get2gnow-debug
	-test -d "~/.gnome2/get2gnow-debug" || rm -rf ~/.gnome2/get2gnow-debug ;
uninstall-gconf:
	-gconftool-2 --recursive-unset /schemas/get2gnow
	-gconftool-2 --recursive-unset /apps/get2gnow
uninstall-cache-dir:
	-test -d "~/.gnome2/get2gnow" || rm -rf ~/.gnome2/get2gnow ;
uninstall: uninstall-debug uninstall-gconf uninstall-cache-dir uninstall-recursive uninstall-am
distclean: distclean-recursive distclean-am

