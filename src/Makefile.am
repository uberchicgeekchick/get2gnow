UBERCHICK_CFLAGS=-std=gnu99 -g -ggdb
AM_CFLAGS=									\
		$(UBERCHICK_CFLAGS)						\
		-I$(top_srcdir)                        			  	\
		-I${top_builddir}						\
		-DBUILDDIR=\""${abs_top_builddir}"\"				\
		-DPREFIX="\"$(prefix)"\"					\
		-DSYSCONFDIR=\""$(sysconfdir)"\"				\
		-DDATADIR=\""$(datadir)"\"              			\
		-DPKGDATADIR=\""$(pkgdatadir)"\"				\
		-DLOCALEDIR=\""$(localedir)"\"					\
		-DLIBDIR=\""$(libdir)"\"					\
		-DLIBSOUP_VERSION=\""`pkg-config --modversion libsoup-2.4`"\"	\
		$(WARN_CFLAGS)							\
		$(debug_CFLAGS)							\
		$(get2gnow_CFLAGS)

bin_PROGRAMS = get2gnow

if HAVE_GNOME_KEYRING
keyring_sources = keyring.c keyring.h
endif

get2gnow_SOURCES = 			online-services-typedefs.h	\
	$(keyring_sources)						\
	program.c			program.h			\
	main.c				main.h				\
	online-services.c		online-services.h		\
	online-service.c		online-service.h		\
	online-service-wrapper.c	online-service-wrapper.h	\
	online-service-request.c	online-service-request.h	\
	online-services-dialog.c	online-services-dialog.h	\
	users-glists.c			users-glists.h			\
	network.c			network.h			\
	timer.c				timer.h				\
	proxy.c				proxy.h				\
	gconfig.c			gconfig.h			\
	debug.c				debug.h				\
	cache.c				cache.h				\
	gtkbuilder.c			gtkbuilder.h			\
	about.c				about.h				\
	ipc.c				ipc.h				\
	main-window.c			main-window.h			\
	geometry.c			geometry.h			\
	label.c				label.h				\
	parser.c			parser.h			\
	groups.c			groups.h			\
	searches.c			searches.h			\
	preferences.c			preferences.h			\
	ui-utils.c			ui-utils.h			\
	following-viewer.c		following-viewer.h		\
	friend-manager.c		friend-manager.h		\
	control-panel.c			control-panel.h			\
	tweets.c			tweets.h			\
	users.c				users.h				\
	images.c			images.h			\
	tweet-list.c			tweet-list.h


get2gnow_LDADD=								\
	$(get2gnow_LIBS)
