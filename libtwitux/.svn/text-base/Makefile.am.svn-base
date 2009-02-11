AM_CPPFLAGS =                                           \
	-I.                                             \
	-I$(top_srcdir)                                 \
	-DDATADIR=\""$(datadir)"\"              	\
	-DLOCALEDIR=\""$(datadir)/locale"\"		\
	$(LIBTWITUX_CFLAGS) $(WARN_CFLAGS)

noinst_LTLIBRARIES = libtwitux.la

if HAVE_GNOME_KEYRING
keyring_impl =						\
	twitux-keyring.c	twitux-keyring.h
endif

libtwitux_la_SOURCES =             			\
	twitux-conf.c		twitux-conf.h		\
	twitux-debug.c		twitux-debug.h		\
	twitux-paths.c 		twitux-paths.h		\
	twitux-xml.c		twitux-xml.h		\
	$(keyring_impl)

libtwitux_la_LIBADD =              			\
	$(LIBTWITUX_LIBS)

libtwituxincludedir = $(includedir)/twitux/


