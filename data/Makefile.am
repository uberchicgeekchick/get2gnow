desktopdir 	 = $(datadir)/applications
desktop_in_files = @PACKAGE_NAME@.desktop.in
desktop_DATA 	 = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@



schemasdir	 = $(GCONF_SCHEMA_FILE_DIR)
schemas_in_files = @PACKAGE_NAME@.schemas.in
schemas_DATA 	 = $(schemas_in_files:.schemas.in=.schemas)
@INTLTOOL_SCHEMAS_RULE@



gtkbuilderdir = $(datadir)/@PACKAGE_NAME@
gtkbuilder_DATA =		 				\
			online-services-dialog.ui		\
			online-services-request-popup.ui	\
			preferences.ui				\
			profile-manager.ui			\
			user-profile-viewer.ui			\


sexy_DATA =							\
			main-window.ui				\
			uberchick-tree-view.ui			\
			update-viewer.ui			\
			friends-manager.ui			\
			following-viewer.ui			\


un_sexy_DATA =							\
			main-window.in.ui			\
			uberchick-tree-view.in.ui		\
			update-viewer.in.ui			\
			friends-manager.in.ui			\
			following-viewer.in.ui			\



svgicondir = $(datadir)/icons/hicolor/scalable/apps
svgicon_DATA = @PACKAGE_NAME@.svg

EXTRA_DIST =							\
			$(sexy_DATA)				\
			$(svgicon_DATA)				\
			$(gtkbuilder_DATA)			\
			$(desktop_in_files)			\
			$(schemas_in_files)			\

	
DISTCLEANFILES =						\
			$(sexy_DATA)				\
			$(desktop_DATA)				\
			$(schemas_DATA)				\


@gtk_update_icon_cache@
#gtk_update_icon_cache=gtk-update-icon-cache -f -t $(datadir)/icons/hicolor


main-window.ui: sexy_DATA
following-viewer.ui: sexy_DATA
friends-manager.ui: sexy_DATA
uberchick-tree-view.ui: sexy_DATA
sexy_DATA: $(un_sexy_DATA)
	@eol='$$'; \
	list='$(un_sexy_DATA)'; for p in $$list; do \
		if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
		f=`echo $$d$$p | sed 's/\.in//'`; \
		if ! test -f "$$f" || test "$$d$$p" -nt "$$f"; then \
			cp -f "$$d$$p" "$$f"; \
			echo "ex -E -n -s -X --noplugin '+1,$$eols/\(class="\)Gtk\(TreeView"\)/\1Sexy\2/g' '+wq' '$$f';"; \
			ex -E -n -s -X --noplugin '+1,$$s/\(class="\)Gtk\(TreeView"\)/\1Sexy\2/g' "+wq" "$$f"; \
			if test "$$f" == "main-window.ui"; then \
				echo "ex -E -n -s -X --noplugin '+1,$$s/^\([\t\ ]\+<object class=\"Gtk[VH]Separator\".*\n.*\n.*\n.*\n.*<packing>\n\)\(.*<property name=\"\)\(expand\)\(\">\)[^<]\+\(<\/property>\)\(\n.*<property name=\"fill\">[^<]*<\/property>\)\?/\1\2\3\4True\5\r\2fill\4True\5/' '+wq' '$$f';"; \
				ex -E -n -s -X --noplugin '+1,$$s/^\([\t\ ]\+<object class="Gtk[VH]Separator".*\n.*\n.*\n.*\n.*<packing>\n\)\(.*<property name="\)\(expand\)\(">\)[^<]\+\(<\/property>\)\(\n.*<property name="fill">[^<]*<\/property>\)\?/\1\2\3\4True\5\r\2fill\4True\5/' "+wq" "$$f"; \
			fi; \
		fi; \
	done
clean-sexy-data:
	@list='$(sexy_DATA)'; for p in $$list; do \
		if test -f "$$p"; then d=; else d="$(subdir)/"; fi; \
		if test -f $$d$$p; then \
			echo "rm '$$d$$p'"; \
			rm "$$d$$p"; \
		fi; \
	done
clean: clean-sexy-data clean-am

install-data-hook: update-icon-cache
uninstall-hook: update-icon-cache
uninstall-gconf-data:
	gconftool-2 --recursive-unset /schemas/apps/@PACKAGE_NAME@
	gconftool-2 --recursive-unset /apps/@PACKAGE_NAME@
uninstall-data: uninstall-gconf-data uninstall-data-am
install-data: install-data-am
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

