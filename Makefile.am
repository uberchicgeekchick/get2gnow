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

# Workaround broken scrollkeeper that doesn't remove its files on
# uninstall.
distuninstallcheck_listfiles = find . -type f -print | grep -v '^\./var/scrollkeeper'

clean: clean-recursive
	gconftool-2 --recursive-unset /apps/get2gnow/debug
	if( -d "~/.gnome2/get2gnow/debug" ) rm -r ~/.gnome2/get2gnow/debug ;

