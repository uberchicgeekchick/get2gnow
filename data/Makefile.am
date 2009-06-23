desktopdir 	 = $(datadir)/applications
desktop_in_files = get2gnow.desktop.in
desktop_DATA 	 = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

schemasdir	 = $(GCONF_SCHEMA_FILE_DIR)
schemas_in_files = get2gnow.schemas.in
schemas_DATA 	 = $(schemas_in_files:.schemas.in=.schemas)
@INTLTOOL_SCHEMAS_RULE@

gtkbuilderdir = $(datadir)/get2gnow
gtkbuilder_DATA = 				\
	online-services-dialog.ui		\
	following-viewer.ui			\
	friends-manager.ui			\
	main-window.ui				\
	tweet-view.ui				\
	online-services-request-popup.ui	\
	preferences.ui				\
	user-profile-viewer.ui

tweet_list_DATA = tweet-list.ui
tweet_list_in_DATA = tweet-list.in.ui

svgicondir = $(datadir)/icons/hicolor/scalable/apps
svgicon_DATA = get2gnow.svg
	
EXTRA_DIST =			\
	$(tweet_list_DATA)	\
	$(gtkbuilder_DATA)	\
	$(svgicon_DATA)		\
	$(desktop_in_files)	\
	$(schemas_in_files)
	
DISTCLEANFILES =		\
	$(tweet_list_DATA)	\
	$(desktop_DATA)		\
	$(schemas_DATA)


tweet-list.ui: $(tweet_list_in_DATA)
	ex -n -s -E '+1,$$s/\(class="\)Gtk\(TreeView"\)/\1Sexy\2/g' '+wq! $(top_builddir)/data/$(tweet_list_DATA)' '$(top_builddir)/data/$(tweet_list_in_DATA)'

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

