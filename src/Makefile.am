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
	$(get2gnow_CFLAGS)				\
	$(WARN_CFLAGS)					\
	-std=$(C_STD_MODE)

bin_PROGRAMS = get2gnow

if HAVE_GNOME_KEYRING
keyring_sources = keyring.c keyring.h
endif

get2gnow_SOURCES = 					\
	$(keyring_sources)				\
	program.c			program.h			\
	main.c			main.h			\
	gconfig.c		gconfig.h		\
	debug.c			debug.h			\
	cache.c			cache.h			\
	gtkbuilder.c		gtkbuilder.h		\
	tree-view.c 		tree-view.h		\
	about.c			about.h			\
	services-dialog.c	services-dialog.h	\
	profile-viewer.c	profile-viewer.h	\
	popup-dialog.c 		popup-dialog.h		\
	ipc.c			ipc.h			\
	app.c			app.h			\
	geometry.c		geometry.h		\
	hint.c			hint.h			\
	label.c			label.h			\
	online-services.c	online-services.h	\
	network.c		network.h		\
	timer.c			timer.h			\
	parser.c		parser.h		\
	preferences.c		preferences.h		\
	ui-utils.c		ui-utils.h		\
	following-viewer.c	following-viewer.h	\
	friend-manager.c	friend-manager.h	\
	tweet-view.c		tweet-view.h		\
	tweets.c		tweets.h		\
	users.c			users.h			\
	images.c		images.h		\
	tweet-list.c		tweet-list.h


get2gnow_LDADD=			\
	$(get2gnow_LIBS)
