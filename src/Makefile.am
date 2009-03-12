AM_CPPFLAGS=						\
	-I$(top_srcdir)                          	\
	-DPREFIX="\"$(prefix)"\"			\
	-DSYSCONFDIR=\""$(sysconfdir)"\"		\
	-DDATADIR=\""$(datadir)"\"              	\
	-DLOCALEDIR=\""$(localedir)"\"			\
	-DLIBDIR=\""$(libdir)"\"			\
	$(TWITUX_CFLAGS)				\
	$(ENCHANT_CFLAGS)				\
	$(WARN_CFLAGS)

bin_PROGRAMS = greet-tweet-know

if HAVE_DBUS
dbus_sources = dbus.c dbus.h
endif

if HAVE_GNOME_KEYRING
keyring_sources = keyring.c keyring.h
endif

greet_tweet_know_SOURCES = 				\
	$(dbus_sources)					\
	$(keyring_sources)				\
	main.c			main.h			\
	gconf.c			gconf.h			\
	debug.c			debug.h			\
	paths.c 		paths.h			\
	glade.c			glade.h			\
	about.c			about.h			\
	account-dialog.c 	account-dialog.h	\
	add-dialog.c 		add-dialog.h		\
	app.c			app.h			\
	geometry.c		geometry.h		\
	hint.c			hint.h			\
	label.c			label.h			\
	network.c		network.h		\
	parser.c		parser.h		\
	preferences.c		preferences.h		\
	send-message-dialog.c	send-message-dialog.h	\
	spell.c			spell.h			\
	spell-dialog.c		spell-dialog.h		\
	ui-utils.c		ui-utils.h		\
	lists-dialog.c		lists-dialog.h		\
	tweet-list.c		tweet-list.h


greet_tweet_know_LDADD=						\
	$(TWITUX_LIBS)					\
	$(ENCHANT_LIBS)
