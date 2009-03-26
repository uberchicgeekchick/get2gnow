AM_CPPFLAGS=						\
	-I$(top_srcdir)                          	\
	-DPREFIX="\"$(prefix)"\"			\
	-DSYSCONFDIR=\""$(sysconfdir)"\"		\
	-DDATADIR=\""$(datadir)"\"              	\
	-DLOCALEDIR=\""$(localedir)"\"			\
	-DLIBDIR=\""$(libdir)"\"			\
	$(GREET_TWEET_KNOW_CFLAGS)				\
	$(ENCHANT_CFLAGS)				\
	$(WARN_CFLAGS)

bin_PROGRAMS = greet-tweet-know

if HAVE_GNOME_KEYRING
keyring_sources = keyring.c keyring.h
endif

greet_tweet_know_SOURCES = 				\
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
	friend-manager.c	friend-manager.h	\
	followers-dialog.c	followers-dialog.h	\
	tweets.c		tweets.h		\
	tweet-list.c		tweet-list.h


greet_tweet_know_LDADD=						\
	$(GREET_TWEET_KNOW_LIBS)					\
	$(ENCHANT_LIBS)
