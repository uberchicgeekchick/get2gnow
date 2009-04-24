C_STD_MODE=gnu99
AM_CFLAGS=						\
	-I$(top_srcdir)                          	\
	-I${top_builddir}				\
	-DBUILDDIR=\""${abs_top_builddir}"\"		\
	-DPREFIX="\"$(prefix)"\"			\
	-DSYSCONFDIR=\""$(sysconfdir)"\"		\
	-DDATADIR=\""$(datadir)"\"              	\
	-DLOCALEDIR=\""$(localedir)"\"			\
	-DLIBDIR=\""$(libdir)"\"			\
	$(get2gnow_CFLAGS)			\
	$(ENCHANT_CFLAGS)				\
	$(WARN_CFLAGS)					\
	-std=$(C_STD_MODE)

bin_PROGRAMS = get2gnow

if HAVE_GNOME_KEYRING
keyring_sources = keyring.c keyring.h
endif

get2gnow_SOURCES = 				\
	$(keyring_sources)				\
	main.c			main.h			\
	gconf.c			gconf.h			\
	debug.c			debug.h			\
	gtkbuilder.c		gtkbuilder.h		\
	tree-view.c 		tree-view.h		\
	about.c			about.h			\
	accounts-dialog.c 	accounts-dialog.h	\
	profile-viewer.c	profile-viewer.h	\
	popup-dialog.c 		popup-dialog.h		\
	ipc.c			ipc.h			\
	app.c			app.h			\
	geometry.c		geometry.h		\
	hint.c			hint.h			\
	label.c			label.h			\
	oauth.c			oauth.h			\
	network.c		network.h		\
	timer.c			timer.h			\
	parser.c		parser.h		\
	preferences.c		preferences.h		\
	ui-utils.c		ui-utils.h		\
	following-viewer.c	following-viewer.h	\
	friend-manager.c	friend-manager.h	\
	tweets.c		tweets.h		\
	users.c			users.h			\
	images.c		images.h		\
	tweet-list.c		tweet-list.h


get2gnow_LDADD=			\
	$(get2gnow_LIBS)	\
	$(ENCHANT_LIBS)
