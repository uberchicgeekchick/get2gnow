UBERCHICK_CFLAGS=-std=gnu99 -g3 -ggdb -Wno-parentheses -ffast-math
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
if ENABLE_ALPHA_UI
get2gnow_ui_sources = uberchick-label.alpha.c
else
get2gnow_ui_sources = uberchick-label.c
endif

get2gnow_SOURCES = 													\
	about.c						about.h								\
	best-friends.c					best-friends.h							\
	cache.c						cache.h								\
	debug.c						debug.h								\
	following-viewer.c				following-viewer.h						\
	friend-manager.c				friend-manager.h						\
	gconfig.c					gconfig.h							\
	geometry.c					geometry.h							\
	groups.c					groups.h							\
	gtkbuilder.c					gtkbuilder.h							\
	hotkeys.c					hotkeys.h							\
	images.c					images.h							\
	ipc.c						ipc.h								\
	$(keyring_sources)												\
	main.c						main.h								\
	main-window.c					main-window.h							\
	network.c					network.h							\
	online-services.typedefs.h			online-services.rest-uri.defines.h				\
	online-service.c				online-service.typedefs.h		online-service.h	\
	online-service-request.c			online-service-request.h					\
	online-services.c				online-services.h						\
	online-services-dialog.c			online-services-dialog.h					\
	online-service-wrapper.c			online-service-wrapper.h					\
	parser.c					parser.h							\
	preferences.c					preferences.h							\
	profile-manager.c				profile-manager.h						\
	program.c					program.h							\
	proxy.c						proxy.h								\
	searches.c					searches.h							\
	tabs.c						tabs.h								\
	timer.c						timer.h								\
	$(get2gnow_ui_sources)				uberchick-label.h						\
	uberchick-sexy-entry-completion.c		uberchick-sexy-entry-completion.h				\
	uberchick-tree-view.c		uberchick-tree-view.macros.h		uberchick-tree-view.h			\
	ui-utils.c					ui-utils.h							\
	update-ids.c					update-ids.h							\
	update-viewer.c					update-viewer.h							\
	users-glists.c							users-glists.h					\
	users.c				users.types.h			users.h						\
	user-profile-viewer.c				user-profile-viewer.h						\
	www.c						www.h								\


get2gnow_LDADD=								\
	$(get2gnow_LIBS)
