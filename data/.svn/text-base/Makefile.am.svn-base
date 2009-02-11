desktopdir 	 = $(datadir)/applications
desktop_in_files = twitux.desktop.in.in
desktop_DATA 	 = $(desktop_in_files:.desktop.in.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

schemasdir	 = $(GCONF_SCHEMA_FILE_DIR)
schemas_in_files = twitux.schemas.in
schemas_DATA 	 = $(schemas_in_files:.schemas.in=.schemas)
@INTLTOOL_SCHEMAS_RULE@

gladedir = $(datadir)/twitux
glade_DATA = 				\
	add_friend_dlg.xml		\
	account_dlg.xml			\
	main_window.xml			\
	prefs_dlg.xml			\
	send_message_dlg.xml		\
	spell_dlg.xml			\
	lists_dlg.xml

icondir = $(datadir)/icons/hicolor/48x48/apps
icon_DATA = twitux.png
svgicondir = $(datadir)/icons/hicolor/scalable/apps
svgicon_DATA = twitux.svg
	
EXTRA_DIST =			\
	$(glade_DATA)		\
	$(icon_DATA)		\
	$(svgicon_DATA)		\
	$(desktop_in_files)	\
	$(schemas_in_files)
	
DISTCLEANFILES =		\
	$(desktop_DATA)		\
	$(schemas_DATA)
	
gtk_update_icon_cache = gtk-update-icon-cache -f -t $(datadir)/icons/hicolor

install-data-hook: update-icon-cache
uninstall-hook: update-icon-cache
update-icon-cache:
	@-if test -z "$(DESTDIR)"; then \
		echo "Updating Gtk icon cache."; \
		$(gtk_update_icon_cache); \
	else \
		echo "*** Icon cache not updated.  After (un)install, run this:"; \
		echo "***   $(gtk_update_icon_cache)"; \
	fi
	
if GCONF_SCHEMAS_INSTALL
install-data-local:
	        if test -z "$(DESTDIR)" ; then \
	                for p in $(schemas_DATA) ; do \
	                        GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(top_builddir)/data/$$p ; \
	                done \
	        fi
else
install-data-local:
endif
	
