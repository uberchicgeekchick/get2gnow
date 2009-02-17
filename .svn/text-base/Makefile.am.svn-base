SUBDIRS = data libtwitux src po

if ENABLE_SK
SUBDIRS += help
endif

EXTRA_DIST = 			\
	ChangeLog               \
	README                  \
	AUTHORS			\
	NEWS			\
	xmldocs.make		\
	omf.make		\
	gnome-doc-utils.make

DISTCLEANFILES = 		\
	gnome-doc-utils.make

DISTCHECK_CONFIGURE_FLAGS =	\
	--disable-scrollkeeper

# Workaround broken scrollkeeper that doesn't remove its files on
# uninstall.
distuninstallcheck_listfiles = find . -type f -print | grep -v '^\./var/scrollkeeper'
