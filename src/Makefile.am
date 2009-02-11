AM_CPPFLAGS=						\
	-I$(top_srcdir)                          	\
	-DPREFIX="\"$(prefix)"\"			\
	-DSYSCONFDIR=\""$(sysconfdir)"\"		\
	-DDATADIR=\""$(datadir)"\"              	\
	-DLIBDIR=\""$(libdir)"\"			\
	$(TWITUX_CFLAGS)				\
	$(ENCHANT_CFLAGS)				\
	$(WARN_CFLAGS)

bin_PROGRAMS = twitux

if HAVE_DBUS
dbus_sources = twitux-dbus.c twitux-dbus.h
endif

twitux_SOURCES = 							\
	$(dbus_sources)							\
	twitux.h							\
	twitux-main.c							\
	twitux-about.c			twitux-about.h			\
	twitux-account-dialog.c 	twitux-account-dialog.h		\
	twitux-add-dialog.c 		twitux-add-dialog.h		\
	twitux-app.c			twitux-app.h			\
	twitux-geometry.c		twitux-geometry.h		\
	twitux-hint.c			twitux-hint.h			\
	twitux-label.c			twitux-label.h			\
	twitux-network.c		twitux-network.h		\
	twitux-parser.c			twitux-parser.h			\
	twitux-preferences.c		twitux-preferences.h		\
	twitux-send-message-dialog.c	twitux-send-message-dialog.h	\
	twitux-spell.c			twitux-spell.h			\
	twitux-spell-dialog.c		twitux-spell-dialog.h		\
	twitux-ui-utils.c		twitux-ui-utils.h		\
	twitux-lists-dialog.c		twitux-lists-dialog.h		\
	twitux-tweet-list.c		twitux-tweet-list.h

twitux_LDADD=						\
	$(top_builddir)/libtwitux/libtwitux.la		\
	$(TWITUX_LIBS)					\
	$(ENCHANT_LIBS)
