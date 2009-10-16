UBERCHICK_CFLAGS=-std=gnu99 -g -ggdb -Wno-parentheses
AM_CFLAGS=									\
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
		$(get2gnow_CFLAGS)						\
		$(UBERCHICK_CFLAGS)						\

bin_PROGRAMS = get2gnow

if HAVE_GNOME_KEYRING
keyring_sources = keyring.c keyring.h
endif

get2gnow_SOURCES = 										\
	about.c						about.h					\
	best-friends.c					best-friends.h				\
	control-panel.c					control-panel.h				\
	cache.c						cache.h					\
	debug.c						debug.h					\
	following-viewer.c				following-viewer.h			\
	friend-manager.c				friend-manager.h			\
	gconfig.c					gconfig.h				\
	geometry.c					geometry.h				\
	groups.c					groups.h				\
	gtkbuilder.c					gtkbuilder.h				\
	hotkeys.c					hotkeys.h				\
	images.c					images.h				\
	ipc.c						ipc.h					\
	$(keyring_sources)									\
	label.c						label.h					\
	main.c						main.h					\
	main-window.c					main-window.h				\
	network.c					network.h				\
	online-service.c				online-service.types.h		online-service.h		\
	online-service-request.c			online-service-request.h		\
	online-services.c				online-services.h			\
	online-services-dialog.c			online-services-dialog.h		\
	online-services-typedefs.h								\
	online-service-wrapper.c			online-service-wrapper.h		\
	parser.c					parser.h				\
	preferences.c					preferences.h				\
	program.c					program.h				\
	proxy.c						proxy.h					\
	searches.c					searches.h				\
	tabs.c						tabs.h					\
	timer.c						timer.h					\
	ui-utils.c					ui-utils.h				\
	update-viewer.c					update-viewer.h				\
	users-glists.c							users-glists.h		\
	users.c				users.types.h			users.h


get2gnow_LDADD=								\
	$(get2gnow_LIBS)
