/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright(c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the RPL
 *
 * For more information or to find the latest release, visit our
 * website at: http://uberChicGeekChick.Com/?projects=get2gnow
 *
 * Writen by an uberChick, other uberChicks please meet me & others @:
 * 	http://uberChicks.Net/
 *
 * I'm also disabled. I live with a progressive neuro-muscular disease.
 * DYT1+ Early-Onset Generalized Dystonia, a type of Generalized Dystonia.
 * 	http://Dystonia-DREAMS.Org/
 *
 *
 *
 * Unless explicitly acquired and licensed from Licensor under another
 * license, the contents of this file are subject to the Reciprocal Public
 * License("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
 * and You may not copy or use this file in either source code or executable
 * form, except in compliance with the terms and conditions of the RPL.
 *
 * All software distributed under the RPL is provided strictly on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, AND
 * LICENSOR HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, QUIET ENJOYMENT, OR NON-INFRINGEMENT. See the RPL for specific
 * language governing rights and limitations under the RPL.
 *
 * The User-Visible Attribution Notice below, when provided, must appear in each
 * user-visible display as defined in Section 6.4(d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright(C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright(C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright(C) of the User the data is connected to.
 * Users may lisences their data under the terms of an OSI approved or Creative Commons
 * license.  Users must be allowed to select their choice of license for each piece of data
 * on an individual bases and cannot be blanketly applied to all of the Users.  The User may
 * select a default license for their data.  All of the Software's data pertaining to each
 * User must be fully accessible, exportable, and deletable to that User.
 */

#define _GNU_SOURCE
#define _THREAD_SAFE

#include <sys/stat.h>
#include <string.h>

#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <gdk/gdkkeysyms.h>
#include <libsexy/sexy.h>

#include "config.h"
#include "program.h"

#include "gconfig.h"
#include "gtkbuilder.h"

#include "about.h"
#include "online-services.typedefs.h"
#include "online-services.rest-uris.defines.h"
#include "online-services.typedefs.h"
#include "online-services.types.h"
#include "online-service.h"
#include "online-services.h"
#include "online-service-request.h"
#include "online-services-dialog.h"
#include "main-window.h"

#include "best-friends.h"

#include "images.h"
#include "preferences.defines.h"
#include "ui-utils.h"
#include "geometry.h"

#include "preferences.h"

#include "hotkeys.h"
#include "tabs.h"

#include "uberchick-tree-view.h"
#include "update-viewer.h"

#include "profile-manager.h"
#include "friends-manager.h"
#include "following-viewer.h"

#include "uberchick-sexy-entry-completion.h"


#define	GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_MAIN_WINDOW, MainWindowPrivate))

struct _MainWindowPrivate {
	/* Misc */
	guint				size_timeout_id;
	guint				timeout_id_status_bar_message_default;
	gboolean			connected;
	
	/* Status Icon Stuff. */
	GtkStatusIcon			*status_icon;
	GtkWidget			*popup_menu;
	GtkToggleAction			*popup_menu_show_main_window;
	
	/* Main widgets */
	GtkWindow			*window;
	GtkVBox				*main_vbox;
	GtkWindow			*update_viewer_window;
	
	GtkMenuBar			*menubar;
	GtkMenuItem			*network_menu_item;
        GtkMenu				*menu_network;
	
	/* Tools / Edit menu items. */
	GtkMenuItem			*edit_menu_item;
        GtkMenu				*menu_edit;
	
	GtkMenuItem			*view_menu_item;
        GtkMenu				*menu_view;
	
	GtkCheckMenuItem		*view_menu_uber_compact_view_check_menu;
	
	GtkCheckMenuItem		*view_toolbar_main_check_menu_item;
	GtkCheckMenuItem		*view_toolbar_tabs_check_menu_item;
	GtkCheckMenuItem		*view_best_friends_check_menu_item;
	
	GtkCheckMenuItem		*view_update_viewer_floating_check_menu_item;
	GtkCheckMenuItem		*view_update_viewer_compact_view_check_menu_item;
	
	GtkCheckMenuItem		*view_menu_detailed_update_column_check_menu_item;
	GtkCheckMenuItem		*view_menu_from_colums_check_menu_item;
	GtkCheckMenuItem		*view_menu_rcpt_colums_check_menu_item;
	
	/* This is the 'Tools' menu. */
	GtkMenuItem		*online_service_request_menu_item;
	GtkMenu			*menu_online_service_request;
	GList			*online_service_request_popup_image_menu_items;
	
	/* Timeline menu items */
	GtkMenuItem			*tabs_menu_item;
	GtkMenu				*menu_tabs;
	GList				*tabs_menu_widgets;
	GtkCheckMenuItem		*timeline_homepage;
	GtkCheckMenuItem		*timeline_mine;
	GtkCheckMenuItem		*timeline_dm;
	GtkCheckMenuItem		*timeline_favorites;
	GtkCheckMenuItem		*timeline_replies;
	GtkCheckMenuItem		*timeline_retweets_to_me;
	GtkCheckMenuItem		*timeline_retweets_of_me;
	GtkCheckMenuItem		*timeline_public;
	
	GtkMenuItem			*help_menu_item;
	GtkMenu				*menu_help;
	
	GtkHandleBox			*main_window_handlebox;
	GtkToolbar			*main_toolbar;
	
	UberChickSexyEntryCompletion	*sexy_search_entry_completion;
	
	GtkFrame			*search_frame;
	GtkAlignment			*search_entry_alignment;
	GtkLabel			*search_label;
	GtkComboBoxEntry		*search_history_combo_box_entry;
	SexySpellEntry			*sexy_search_entry;
	gint				sexy_position;
	gint				search_history_total;
	gint				search_history_maximum;
	GtkEntryCompletion		*search_history_completion;
	GtkListStore			*search_history_list_store;
	GtkTreeModel			*search_history_tree_model;
	GtkButton			*search_button;
	
	GtkToolButton			*preferences_tool_button;
	GtkToolButton			*accounts_tool_button;
	GtkToolButton			*exit_tool_button;
	
	/* Widgets that are enabled when we are connected/disconnected */
	GList				*widgets_connected;
	GList				*widgets_disconnected;
	GList				*selected_update_image_menu_items;
	
	GtkHPaned			*uberchick_tree_view_hpaned;
	GtkVBox				*best_friends_vbox;
	
	/* user, status, & update widgets.
	 * Actually they're in the UpdateViewer.
	 * The main-window's GtkVBox contains them.
	 */
	GtkVPaned			*main_vpaned;
	GtkHBox				*expand_box;
	GtkVBox				*update_viewer_vbox;
	
	/*These are or part of 'UpdateViewer'.*/
	UpdateViewer			*update_viewer;
	GtkHBox				*update_viewer_embed;
	
	GtkStatusbar			*statusbar;
	gchar				*statusbar_default_message;
};

typedef enum{
	GSTRING_SEARCH_PHRASE = 0,
} PeviousSearches;

#define	DEBUG_DOMAINS	"UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Updates:Requests:Users:Start-Up:main-window.c"
#include "debug.h"

#define	GTK_BUILDER_UI_FILENAME	"main-window"


static void main_window_class_init(MainWindowClass *klass);
static void main_window_init(MainWindow *main_window);
static void main_window_finalize(GObject *object);

static gboolean main_window_statusbar_display(const gchar *message);
static void main_window_statusbar_timeouts_free(void);
static void main_window_statusbar_timer_remove(void);

static void main_window_setup(void);
static void main_window_view_setup(MainWindowPrivate *m_w_p);

static void main_window_destroy_cb(GtkWidget *window, MainWindow *main_window); 
static gboolean main_window_delete_event_cb(GtkWidget *window, GdkEvent *event, MainWindow *main_window);
static void main_window_exit(GtkWidget *window, MainWindow *main_window); 

static gint main_window_search_history_validate_maxium(MainWindowPrivate *m_w_p);
static void main_window_sexy_search_setup(MainWindowPrivate *m_w_p);
static void main_window_search_history_load(MainWindowPrivate *m_w_p);

static void main_window_search_history_add(MainWindowPrivate *m_w_p, const gchar *update, gint list_store_index);
#define main_window_search_history_restore(m_w_p, update)			\
		main_window_search_history_add(m_w_p, update, -3)
#define main_window_search_history_apppend(m_w_p, update)			\
		main_window_search_history_add(m_w_p, update, -2)
#define main_window_search_history_prepend(m_w_p, update)			\
		main_window_search_history_add(m_w_p, update, -1)
#define main_window_search_history_insert(m_w_p, update, list_store_index)	\
		main_window_search_history_add(m_w_p, update, list_store_index)

static gboolean main_window_search_history_is_unique(MainWindowPrivate *m_w_p, const gchar *search_phrase);
static void main_window_search_history_remove(MainWindowPrivate *m_w_p, gint list_store_index);
static void main_window_search_history_rotate(MainWindowPrivate *m_w_p);

static void main_window_search_history_selected(GtkComboBoxEntry *search_history_combo_box_entry, MainWindow *main_window);
static void main_window_search_submitted(GtkWidget *search_widget, MainWindow *main_window);

static void main_window_services_cb(GtkWidget *window, MainWindow *main_window); 
static void main_window_select_service(GtkMenuItem *item, MainWindow *main_window);
static void main_window_preferences_cb(GtkWidget *window, MainWindow *main_window); 

static void main_window_tabs_menu_widgets_setup(MainWindowPrivate *m_w_p);
static void main_window_tabs_menu_timeline_selected(GtkCheckMenuItem *selected_tab, MainWindowPrivate *m_w_p);

static void main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(GtkCheckMenuItem *check_menu_item, const gchar *gconfig_key, gboolean active, GtkWidget *widget);
static void main_window_view_menu_option_toggled(GtkCheckMenuItem *check_menu_item);
static void main_window_compact_view_toggled(gboolean checked);
static void main_window_compact_timelines_toggled(gboolean checked);

static void main_window_online_service_request_popup_menu_setup(GtkBuilder *ui);
static void main_menu_online_service_request_menu_process(GtkImageMenuItem *item, MainWindow *main_window);

static void main_window_about_cb(GtkWidget *window, MainWindow *main_window); 
static void main_window_help_contents_cb(GtkWidget *widget, MainWindow *main_window); 

static void main_window_connection_items_setup(GtkBuilder *ui);
static void main_window_selected_update_widgets_setup(GtkBuilder *ui);
static void main_window_login(void);
static void main_window_reconnect(GtkMenuItem *item, MainWindow *main_window);

static void main_window_status_icon_create(void);
static void main_window_status_icon_create_menu(void);
static void main_window_status_icon_activate_cb(GtkStatusIcon *status_icon, MainWindow *main_window); 
static void main_window_status_icon_popup_menu_cb(GtkStatusIcon *status_icon, guint button, guint activate_time, MainWindow *main_window);

static void main_window_set_visibility(gboolean visible);
static void main_window_toggle_visibility(void);

static gboolean configure_event_timeout_cb(GtkWidget *widget);
static gboolean main_window_window_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, MainWindow *main_window);

static MainWindow  *main_window=NULL;
static guint statusbar_inital_context_id=0;

G_DEFINE_TYPE(MainWindow, main_window, G_TYPE_OBJECT);

static void main_window_class_init(MainWindowClass *klass){
	GObjectClass *object_class=G_OBJECT_CLASS(klass);
	object_class->finalize=main_window_finalize;
	g_type_class_add_private(object_class, sizeof(MainWindowPrivate));
}/*main_window_class_init(GET_CLASS(main_window));*/

static void main_window_init(MainWindow *singleton_main_window){
	main_window=singleton_main_window;
	
	main_window->private=GET_PRIVATE(main_window);
	main_window->private->sexy_search_entry=NULL;
	main_window->private->sexy_position=-1;
	main_window->private->search_history_completion=NULL;
	main_window->private->online_service_request_popup_image_menu_items=NULL;
	main_window->private->widgets_connected=NULL;
	main_window->private->widgets_disconnected=NULL;
	main_window->private->search_history_total=0;
	main_window->private->search_history_maximum=0;
	main_window->private->statusbar_default_message=g_strdup("");
	online_service_request_unset_selected_update();
}/*main_window_init(main_window);*/

static void main_window_finalize(GObject *object){
	MainWindow	       *main_window;
	
	main_window=MAIN_WINDOW(object);
	main_window->private=GET_PRIVATE(main_window);
	
	gtk_list_store_clear(main_window->private->search_history_list_store);
	
	gtk_widget_destroy(GTK_WIDGET(main_window->private->sexy_search_entry));
	gtk_widget_destroy(GTK_WIDGET(main_window->private->search_history_completion));
	
	program_timeout_remove(&main_window->private->size_timeout_id, _("main window configuration"));
	
	main_window_statusbar_timeouts_free();
	
	uber_list_free(main_window->private->online_service_request_popup_image_menu_items);
	uber_list_free(main_window->private->tabs_menu_widgets);
	uber_list_free(main_window->private->widgets_connected);
	uber_list_free(main_window->private->widgets_disconnected);
	uber_list_free(main_window->private->selected_update_image_menu_items);

	best_friends_clean_up();
	
	tabs_destroy();
	uber_free(main_window->private->statusbar_default_message);
	
	G_OBJECT_CLASS(main_window_parent_class)->finalize(object);
}/*main_window_finalize(main_window);*/

static void main_window_setup(void){
	debug("Starting %s", PACKAGE_NAME);
	GtkNotebook *tabs_notebook=NULL;
	
	/* Set up interface */
	debug("Initialising interface");
	GtkBuilder *ui=gtkbuilder_get_file(
				GTK_BUILDER_UI_FILENAME,
					"main_window", &main_window->private->window,
					"main_vbox", &main_window->private->main_vbox,
					
					"main_menubar", &main_window->private->menubar,
					"network_menu_item", &main_window->private->network_menu_item,
					"connections_menu", &main_window->private->menu_network,
					
					"tabs_menu_item", &main_window->private->tabs_menu_item,
					"tabs_menu", &main_window->private->menu_tabs,
					"tabs_menu_hompage_timeline_check_menu_item", &main_window->private->timeline_homepage,
					"tabs_menu_my_timeline_check_menu_item", &main_window->private->timeline_mine,
					"tabs_menu_direct_messages_check_menu_item", &main_window->private->timeline_dm,
					"tabs_menu_replies_check_menu_item", &main_window->private->timeline_replies,
					"tabs_menu_favorites_timeline_check_menu_item", &main_window->private->timeline_favorites,
					"tabs_menu_retweets_to_me_timeline_check_menu_item", &main_window->private->timeline_retweets_to_me,
					"tabs_menu_retweets_of_me_timeline_check_menu_item", &main_window->private->timeline_retweets_of_me,
					"tabs_menu_public_timeline_check_menu_item", &main_window->private->timeline_public,
					
					"edit_menu_item", &main_window->private->edit_menu_item,
					"edit_menu", &main_window->private->menu_edit,
					
					"view_menu_item", &main_window->private->view_menu_item,
					"view_menu", &main_window->private->menu_view,
					
					"view_menu_uber_compact_view_check_menu", &main_window->private->view_menu_uber_compact_view_check_menu,
					"view_toolbar_main_check_menu_item", &main_window->private->view_toolbar_main_check_menu_item,
					"view_toolbar_tabs_check_menu_item", &main_window->private->view_toolbar_tabs_check_menu_item,
					
					"view_best_friends_check_menu_item", &main_window->private->view_best_friends_check_menu_item,
					
					"view_update_viewer_floating_check_menu_item", &main_window->private->view_update_viewer_floating_check_menu_item,
					"view_update_viewer_compact_view_check_menu_item", &main_window->private->view_update_viewer_compact_view_check_menu_item,
					
					"view_menu_detailed_update_column_check_menu_item", &main_window->private->view_menu_detailed_update_column_check_menu_item,
					"view_menu_from_colums_check_menu_item", &main_window->private->view_menu_from_colums_check_menu_item,
					"view_menu_rcpt_colums_check_menu_item", &main_window->private->view_menu_rcpt_colums_check_menu_item,
					
					"online_service_request_menu_item", &main_window->private->online_service_request_menu_item,
					"online_service_request_menu", &main_window->private->menu_online_service_request,
					
					"help_menu_item", &main_window->private->help_menu_item,
					"help_menu", &main_window->private->menu_help,
					/* end menu items. */
					
					/* start:main toolbar */
					"main_window_handlebox", &main_window->private->main_window_handlebox,
					"main_toolbar", &main_window->private->main_toolbar,
					
					"search_frame", &main_window->private->search_frame,
					"search_entry_alignment", &main_window->private->search_entry_alignment,
                              		"search_label", &main_window->private->search_label,
					"search_history_combo_box_entry", &main_window->private->search_history_combo_box_entry,
					"search_history_list_store", &main_window->private->search_history_list_store,
					"search_button", &main_window->private->search_button,
					
					"preferences_tool_button", &main_window->private->preferences_tool_button,
					"accounts_tool_button", &main_window->private->accounts_tool_button,
					"main_window_main_tool_bar_exit_tool_button", &main_window->private->exit_tool_button,
					
					/* the major UI elements. */
					"main_vpaned", &main_window->private->main_vpaned,
					"best_friends_vbox", &main_window->private->best_friends_vbox,
					
					/*tabs_notebook is used to contain get2gnow's timeline tabs.*/
					"uberchick_tree_view_hpaned", &main_window->private->uberchick_tree_view_hpaned,
					"tabs_notebook", &tabs_notebook,
					
					/*container elements for the UpdateViewer - when its embed.*/
					"expand_box", &main_window->private->expand_box,
					"update_viewer_vbox", &main_window->private->update_viewer_vbox,
					
					/* the...well duh, LOL you figure it out.*/
					"main_statusbar", &main_window->private->statusbar,
				NULL
	);
	
	tabs_init(tabs_notebook);
	main_window_tabs_menu_widgets_setup(main_window->private);
	main_window_sexy_search_setup(main_window->private);
	
	/* Connect the signals */
	gtkbuilder_connect( ui, main_window,
					"main_window", "destroy", main_window_destroy_cb,
					"main_window", "delete_event", main_window_delete_event_cb,
					"main_window", "configure_event", main_window_window_configure_event_cb,
					
					"services_connect", "activate", main_window_reconnect,
					"services_disconnect", "activate", main_window_disconnect,
					"quit", "activate", main_window_exit,
					
					"accounts_image_menu_item", "activate", main_window_services_cb,
					"select_service_image_menu_item", "activate", main_window_select_service,
					"preferences", "activate", main_window_preferences_cb,
					
					"new_update", "activate", update_viewer_new_update,
					"new_dm", "activate", update_viewer_new_dm,
					"selected_update_reply_image_menu_item", "activate", online_service_request_selected_update_reply,
					"selected_update_forward_update_image_menu_item", "activate", online_service_request_selected_update_forward,
					"selected_update_save_fave_image_menu_item", "activate", online_service_request_selected_update_fave,
					"selected_update_author_view_profile_image_menu_item", "activate", online_service_request_selected_update_view_profile,
					"selected_update_author_view_unread_updates_image_menu_item", "activate", online_service_request_selected_update_view_updates_new,
					"selected_update_author_view_updates_image_menu_item", "activate", online_service_request_selected_update_view_updates,
					"selected_update_author_best_friend_image_menu_item", "activate", online_service_request_selected_update_best_friend_add,
					"selected_update_author_follow_image_menu_item", "activate", online_service_request_selected_update_follow,
					"selected_update_author_unfollow_image_menu_item", "activate", online_service_request_selected_update_unfollow,
					"selected_update_author_block_image_menu_item", "activate", online_service_request_selected_update_block,
					
					"help_contents", "activate", main_window_help_contents_cb,
					"help_about", "activate", main_window_about_cb,
					
					"search_history_combo_box_entry", "changed", main_window_search_history_selected,
					"search_button", "clicked", main_window_search_submitted,
					
					"accounts_tool_button", "clicked", main_window_services_cb,
					"select_service_tool_button", "clicked", main_window_select_service,
					"preferences_tool_button", "clicked", main_window_preferences_cb,
					"main_window_main_tool_bar_exit_tool_button", "clicked", main_window_exit,
				NULL
	);
	
	gchar *window_title=g_strdup_printf("%s", _(GETTEXT_PACKAGE));
	gtk_window_set_title(GTK_WINDOW(main_window->private->window), window_title);
	uber_free(window_title);
	
	gtkbuilder_connect_after(ui, main_window,
					/*"main_window", "grab-focus", update_viewer_sexy_select,*/
					"main_window", "focus-in-event", update_viewer_sexy_select,
				NULL
	);
	
	main_window_online_service_request_popup_menu_setup(ui);
	best_friends_setup(ui);
	main_window_view_setup(main_window->private);
	
	/* Set up connected related widgets */
	main_window_connection_items_setup(ui);
	main_window_selected_update_widgets_setup(ui);
	uber_object_unref(ui);
	
	main_window_status_icon_create();
	
	/* Expand tweet area used to view & send tweets & dm.  */
	if(!gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE))
		main_window->private->update_viewer=update_viewer_new(NULL);
	else
		main_window->private->update_viewer=update_viewer_new(main_window->private->window);
	
	/*update_viewer stuff.*/
	main_window->private->update_viewer_embed=update_viewer_get_embed();
	main_window->private->update_viewer_window=update_viewer_get_window();
	
	if(!gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE)){
		gtk_widget_reparent(GTK_WIDGET(main_window->private->update_viewer_embed), GTK_WIDGET(main_window->private->update_viewer_vbox));
		gtk_widget_show(GTK_WIDGET(main_window->private->update_viewer_embed));
	}else
		gtk_widget_hide(GTK_WIDGET(main_window->private->expand_box));
	/* Set the main window geometry */
	geometry_load();
	
	/* Initial status of widgets */
	main_window_state_on_connection(FALSE);
	best_friends_buttons_set_sensitive();

	/* Ok, set the window state based on the gconf value */
	if(!gconfig_if_bool(MAIN_WINDOW_UI_HIDDEN, FALSE))
		gtk_widget_show(GTK_WIDGET(main_window->private->window));
	else
		gtk_widget_hide(GTK_WIDGET(main_window->private->window));
	
	main_window_login();
}/*main_window_setup();*/

static void main_window_view_setup(MainWindowPrivate *m_w_p){
	/* Best Friends stuff. */
	main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(m_w_p->view_best_friends_check_menu_item, MAIN_WINDOW_BEST_FRIENDS_HIDE_VBOX, TRUE, (GtkWidget *)m_w_p->best_friends_vbox);
	
	/* Main Toolbar stuff. */
	main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(m_w_p->view_toolbar_main_check_menu_item, MAIN_WINDOW_MAIN_TOOLBAR_HIDE, TRUE, (GtkWidget *)m_w_p->main_window_handlebox);
	
	/* Tabs Toolbars stuff. */
	main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(m_w_p->view_toolbar_tabs_check_menu_item, TIMELINE_SEXY_TREE_VIEW_TOOLBAR_VISIBILITY, TRUE, NULL);
	
	/* Timeline Columns. */
	main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(m_w_p->view_menu_detailed_update_column_check_menu_item, CONCATENATED_UPDATES, FALSE, NULL);
	main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(m_w_p->view_menu_from_colums_check_menu_item, TIMELINE_SEXY_TREE_VIEW_FROM_COLUMN_VISIBILITY, TRUE, NULL);
	main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(m_w_p->view_menu_rcpt_colums_check_menu_item, TIMELINE_SEXY_TREE_VIEW_RCPT_COLUMN_VISIBILITY, TRUE, NULL);
	
	gtk_check_menu_item_set_active(m_w_p->view_update_viewer_floating_check_menu_item, gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE));
	g_signal_connect_after(m_w_p->view_update_viewer_floating_check_menu_item, "toggled", (GCallback)update_viewer_emulate_embed_toggle_via_check_menu_item, NULL);
	
	gtk_check_menu_item_set_active(m_w_p->view_update_viewer_compact_view_check_menu_item, gconfig_if_bool(PREFS_UPDATE_VIEWER_COMPACT, FALSE));
	g_signal_connect_after(m_w_p->view_update_viewer_compact_view_check_menu_item, "toggled", (GCallback)update_viewer_emulate_compact_view_toggle, NULL);
	
	/* Over all hidden/visible UI widgets. */
	main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(m_w_p->view_menu_uber_compact_view_check_menu, COMPACT_VIEW, FALSE, NULL);
}/*main_window_view_setup(main_window->private);*/

static void main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(GtkCheckMenuItem *check_menu_item, const gchar *gconfig_key, gboolean active, GtkWidget *widget){
	gboolean hide=!gconfig_if_bool(gconfig_key, active);
	gtk_check_menu_item_set_active(check_menu_item, !hide);
	
	g_object_set_data_full((GObject *)check_menu_item, "gconfig_key", g_strdup(gconfig_key), g_free);
	g_object_set_data_full((GObject *)check_menu_item, "default_value", g_strdup((active ?"TRUE" :"FALSE")), g_free);
	
	if(widget){
		g_object_set_data((GObject *)check_menu_item, "widget", widget);
		if(hide) gtk_widget_hide(widget);
	}
	
	g_signal_connect_after(check_menu_item, "toggled", (GCallback)main_window_view_menu_option_toggled, NULL);
}/*main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(main_window->private->view_best_friends_check_menu_item, MAIN_WINDOW_BEST_FRIENDS_HIDE_VBOX, TRUE|FALSE, (GtkWidget *)main_window->private->best_friends_vbox);*/

static void main_window_view_menu_option_toggled(GtkCheckMenuItem *check_menu_item){
	/* Any widget's visiblity state is stored as !visibility.  I am going to switch this.
	 * I'm only currently doing this because gconf's preferences currently indicate if something hidden.
	 * TODO: depricate this - mainly by updating how the MainWindow handles its 'auto-hide in system tray' state.
	 * e.g. /ui/hide/best_friends & /ui/hide/main_window.
 .	 * 	This is handled by MainWindow's methods: 'main_window_toggle_visibility' & 'main_window_timeout'.
	 */
	const gchar *gconfig_key=g_object_get_data((GObject *)check_menu_item, "gconfig_key");
	const gchar *gconfig_default=g_object_get_data((GObject *)check_menu_item, "default_value");
	gboolean active=gtk_check_menu_item_get_active(check_menu_item);
	if(active==gconfig_if_bool(gconfig_key, (g_str_equal(gconfig_default, "FALSE") ?FALSE :TRUE)))
		return;
	
	gconfig_set_bool(gconfig_key, active);
	GtkWidget *widget=NULL;
	if((widget=g_object_get_data((GObject *)check_menu_item, "widget"))){
		if(!active)
			gtk_widget_hide(widget);
		else
			gtk_widget_show(widget);
	}
	
	if(check_menu_item==main_window->private->view_toolbar_tabs_check_menu_item)
		tabs_toggle_toolbars();
	
	else if(check_menu_item==main_window->private->view_menu_from_colums_check_menu_item)
		tabs_toggle_from_columns();
	
	else if(check_menu_item==main_window->private->view_menu_rcpt_colums_check_menu_item)
		tabs_toggle_rcpt_columns();
	
	else if(check_menu_item==main_window->private->view_menu_detailed_update_column_check_menu_item)
		main_window_compact_timelines_toggled(active);
	
	else if(check_menu_item==main_window->private->view_menu_uber_compact_view_check_menu)
		main_window_compact_view_toggled(active);
	
	update_viewer_sexy_select(update_viewer_sexy_entry_get_widget());
}/*main_window_view_menu_option_toggled(check_menu_item);*/

static void main_window_compact_view_toggled(gboolean checked){
	gtk_check_menu_item_set_active(main_window->private->view_toolbar_main_check_menu_item, !checked);
	gtk_check_menu_item_set_active(main_window->private->view_toolbar_tabs_check_menu_item, !checked);
	gtk_check_menu_item_set_active(main_window->private->view_best_friends_check_menu_item, !checked);
	gtk_check_menu_item_set_active(main_window->private->view_menu_detailed_update_column_check_menu_item, checked);
	gtk_check_menu_item_set_active(main_window->private->view_update_viewer_compact_view_check_menu_item, checked);
}/*main_window_view_compact_view_toggled(TRUE|FALSE);*/

static void main_window_compact_timelines_toggled(gboolean checked){
	tabs_toggle_view();
	
	gtk_check_menu_item_set_active(main_window->private->view_menu_from_colums_check_menu_item, !checked);
	gtk_check_menu_item_set_active(main_window->private->view_menu_rcpt_colums_check_menu_item, !checked);
}/*main_window_compact_timelines_toggled(TRUE|FALSE);*/

void main_window_compact_ui(GtkToggleButton *toggle_button){
	main_window_compact_view_toggled(gtk_toggle_button_get_active(toggle_button));
}/*main_window_compact_ui();*/

void main_window_concatenate_timeline_columns(GtkToggleButton *toggle_button){
	main_window_compact_timelines_toggled(gtk_toggle_button_get_active(toggle_button));
}/*main_window_concatenate_timeline_columns();*/

void main_window_update_viewer_set_embed(GtkToggleButton *toggle_button, gpointer user_data){
	gboolean use_update_viewer_dialog=gtk_toggle_button_get_active(toggle_button);
	if(use_update_viewer_dialog==gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE)) return;
	
	debug("UpdateViewer changed:\t[%s]",(use_update_viewer_dialog?_("floating"):_("embed")));
	geometry_save();
	gconfig_set_bool(PREFS_UPDATE_VIEWER_DIALOG, use_update_viewer_dialog);
	
	if(use_update_viewer_dialog){
		if(gtk_widget_get_parent(GTK_WIDGET(main_window->private->update_viewer_embed))==GTK_WIDGET(main_window->private->update_viewer_window))
			return;
		
		debug("Displaying UpdateViewer as a stand alone dialog & setting UpdateViewer's parent window");
		gtk_widget_reparent(GTK_WIDGET(main_window->private->update_viewer_embed), GTK_WIDGET(main_window->private->update_viewer_window));
		window_present(GTK_WINDOW(main_window->private->update_viewer_window), TRUE);
		g_object_add_weak_pointer(G_OBJECT(main_window->private->update_viewer_window),(gpointer)&main_window->private->update_viewer_window);
		gtk_window_set_transient_for(GTK_WINDOW(main_window->private->update_viewer_window), main_window->private->window);
		gtk_widget_hide(GTK_WIDGET(main_window->private->expand_box));
	}else{
		if(gtk_widget_get_parent(GTK_WIDGET(main_window->private->update_viewer_embed))==GTK_WIDGET(main_window->private->update_viewer_vbox))
			return;
		
		debug("Embeding UpdateViewer's into %s main window", PACKAGE_NAME);
		gtk_widget_reparent(GTK_WIDGET(main_window->private->update_viewer_embed), GTK_WIDGET(main_window->private->update_viewer_vbox));
		gtk_widget_show(GTK_WIDGET(main_window->private->expand_box));
		gtk_widget_show(GTK_WIDGET(main_window->private->update_viewer_vbox));
		gtk_widget_show(GTK_WIDGET(main_window->private->update_viewer_embed));
		gtk_widget_hide(GTK_WIDGET(main_window->private->update_viewer_window));
	}
	
	update_viewer_set_embed_toggle_and_image();
	
	debug("Setting MainWindow's embed state indicator, in its 'View' menu, to %s window", (use_update_viewer_dialog ?"embed UpdateViewer into its main" :"split UpdateViewer off into a floating"));
	gtk_check_menu_item_set_active(main_window->private->view_update_viewer_floating_check_menu_item, use_update_viewer_dialog);
	
	geometry_load();
}/*main_window_update_viewer_embed*/

GtkWindow *main_window_get_window(void){
	return main_window->private->window;
}/*main_window_get_window*/

GtkPaned *main_window_get_uberchick_tree_view_paned(void){
	return GTK_PANED(main_window->private->uberchick_tree_view_hpaned);
}/*main_window_get_uberchick_tree_view_paned();*/

GtkPaned *main_window_get_main_paned(void){
	return GTK_PANED(main_window->private->main_vpaned);
}/*main_window_get_main_paned();*/

GtkMenuBar *main_menu_get_main_menubar(void){
	return main_window->private->menubar;
}/*main_menu_get_main_menubar();*/

GtkMenuItem *main_window_get_menu_item(const gchar *menu){
	if((g_str_equal(menu, "network"))) return GET_PRIVATE(main_window)->network_menu_item;
	if((g_str_equal(menu, "tabs"))) return GET_PRIVATE(main_window)->tabs_menu_item;
	if((g_str_equal(menu, "edit"))) return GET_PRIVATE(main_window)->edit_menu_item;
	if((g_str_equal(menu, "view"))) return GET_PRIVATE(main_window)->view_menu_item;
	if((g_str_equal(menu, "online_service_request"))) return GET_PRIVATE(main_window)->online_service_request_menu_item;
	if((g_str_equal(menu, "help"))) return GET_PRIVATE(main_window)->help_menu_item;
	return NULL;
}/*main_window_get_menu_item("tabs");*/

GtkMenu *main_window_get_menu(const gchar *menu){
	if((g_str_equal(menu, "network"))) return GET_PRIVATE(main_window)->menu_network;
	if((g_str_equal(menu, "tabs"))) return GET_PRIVATE(main_window)->menu_tabs;
	if((g_str_equal(menu, "edit"))) return GET_PRIVATE(main_window)->menu_edit;
	if((g_str_equal(menu, "view"))) return GET_PRIVATE(main_window)->menu_view;
	if((g_str_equal(menu, "online_service_request"))) return GET_PRIVATE(main_window)->menu_online_service_request;
	if((g_str_equal(menu, "help"))) return GET_PRIVATE(main_window)->menu_help;
	return NULL;
}/*main_window_get_menu("tabs");*/

static void main_window_destroy_cb(GtkWidget *window, MainWindow *main_window){
	online_service_request_unset_selected_update();
	/* Add any clean-up code above this function call */
	gtk_widget_destroy(GTK_WIDGET(GET_PRIVATE(main_window)->window));
}

static gboolean main_window_delete_event_cb(GtkWidget *window, GdkEvent *event, MainWindow *main_window){
	if(main_window_status_icon_is_embedded()) {
		online_service_request_popup_confirmation_dialog(PREFS_HINTS_HIDE_MAIN_WINDOW,
						_("get2gnow is still running, it is just hidden."),
						_("Click on the notification area icon to show get2gnow."),
						NULL, NULL
		);
		
		main_window_set_visibility(FALSE);
		return TRUE;
	}
	
	if((online_service_request_popup_confirmation_dialog(
				PREFS_HINTS_CLOSE_MAIN_WINDOW,
				_("You were about to quit!"),
				_(
					"Since no system or notification tray has been "
					"found, this action would normally quit get2gnow."
				),
				NULL, NULL
	))) return TRUE;
	
	return FALSE;
}

static void main_window_toggle_visibility(void){
	gboolean       visible;
	
	visible=window_get_is_visible(GTK_WINDOW(main_window->private->window));
	
	if(visible && main_window_status_icon_is_embedded()) {
		geometry_save();
		
		gtk_widget_hide(GTK_WIDGET(main_window->private->window));
		
		program_timeout_remove(&main_window->private->size_timeout_id, _("main window configuration"));
	} else {
		geometry_load();
		window_present(GTK_WINDOW(main_window->private->window), TRUE);
		
		if(gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE))
			window_present(GTK_WINDOW(main_window->private->update_viewer_window), TRUE);
	}
	/* Save the window visibility state */
	gconfig_set_bool(MAIN_WINDOW_UI_HIDDEN, visible);
}

static void main_window_set_visibility(gboolean visible){
	gconfig_set_bool(MAIN_WINDOW_UI_HIDDEN, !visible);
	
	if(!visible)
		gtk_widget_hide(GTK_WIDGET(main_window->private->window));
	else{
		window_present(GTK_WINDOW(main_window->private->window), TRUE);
		if(gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE))
			window_present(GTK_WINDOW(main_window->private->update_viewer_window), TRUE);
	}
}

static void main_window_exit(GtkWidget  *widget, MainWindow  *main_window){
	gtk_main_quit();
}

static void main_window_tabs_menu_widgets_setup(MainWindowPrivate *m_w_p){
	m_w_p->tabs_menu_widgets=g_list_append(m_w_p->tabs_menu_widgets, m_w_p->timeline_mine);
	g_object_set_data_full(G_OBJECT(m_w_p->timeline_mine), "timeline", API_TIMELINE_MINE, NULL);
	
	m_w_p->tabs_menu_widgets=g_list_append(m_w_p->tabs_menu_widgets, m_w_p->timeline_favorites);
	g_object_set_data_full(G_OBJECT(m_w_p->timeline_favorites), "timeline", API_FAVORITES, NULL);
	
	m_w_p->tabs_menu_widgets=g_list_append(m_w_p->tabs_menu_widgets, m_w_p->timeline_dm);
	g_object_set_data_full(G_OBJECT(m_w_p->timeline_dm), "timeline", API_DIRECT_MESSAGES, NULL);
	
	m_w_p->tabs_menu_widgets=g_list_append(m_w_p->tabs_menu_widgets, m_w_p->timeline_replies);
	g_object_set_data_full(G_OBJECT(m_w_p->timeline_replies), "timeline", API_REPLIES, NULL);
	
	m_w_p->tabs_menu_widgets=g_list_append(m_w_p->tabs_menu_widgets, m_w_p->timeline_homepage);
	g_object_set_data_full(G_OBJECT(m_w_p->timeline_homepage), "timeline", API_TIMELINE_HOMEPAGE, NULL);
	
	m_w_p->tabs_menu_widgets=g_list_append(m_w_p->tabs_menu_widgets, m_w_p->timeline_retweets_to_me);
	g_object_set_data_full(G_OBJECT(m_w_p->timeline_retweets_to_me), "timeline", API_RETWEETED_TO_ME, NULL);
	
	m_w_p->tabs_menu_widgets=g_list_append(m_w_p->tabs_menu_widgets, m_w_p->timeline_retweets_of_me);
	g_object_set_data_full(G_OBJECT(m_w_p->timeline_retweets_of_me), "timeline", API_RETWEETS_OF_ME, NULL);
	
	m_w_p->tabs_menu_widgets=g_list_append(m_w_p->tabs_menu_widgets, m_w_p->timeline_public);
	g_object_set_data_full(G_OBJECT(m_w_p->timeline_public), "timeline", API_TIMELINE_PUBLIC, NULL);
	
	GList *tabs=NULL;
	for(tabs=m_w_p->tabs_menu_widgets; tabs; tabs=tabs->next)
		g_signal_connect(GTK_CHECK_MENU_ITEM(tabs->data), "toggled", G_CALLBACK(main_window_tabs_menu_timeline_selected), m_w_p);
	g_list_free(tabs);
}/*main_window_tabs_menu_widgets_setup(main_window);*/

static void main_window_tabs_menu_timeline_selected(GtkCheckMenuItem *selected_tab, MainWindowPrivate *m_w_p){
	if(!(main_window && main_window->private && main_window->private->tabs_menu_widgets)) return;
	GList *tab_menu_items=NULL;
	for(tab_menu_items=m_w_p->tabs_menu_widgets; tab_menu_items; tab_menu_items=tab_menu_items->next){
		GtkCheckMenuItem *check_menu_item=(GtkCheckMenuItem *)tab_menu_items->data;
		if(check_menu_item!=selected_tab) continue;
		const gchar *timeline=g_object_get_data(G_OBJECT(check_menu_item), "timeline");
		debug("Opening a timeline. GtkCheckMenuItem selected: %s; timeline: %s", gtk_menu_item_get_label(GTK_MENU_ITEM(check_menu_item)), timeline);
		if(!gtk_check_menu_item_get_active(check_menu_item))
			tabs_close_timeline(timeline);
		else
			tabs_open_timeline(timeline, NULL);
	}
	g_list_free(tab_menu_items);
}/*main_window_tabs_menu_timeline_selected(selected_tab, main_window->private);*/

/*
 * Function to set the default
 * timeline in the menu.
 */
gboolean main_window_tabs_menu_set_active(const gchar *timeline_to_open, gboolean open){
	if(!(main_window && main_window->private && main_window->private->tabs_menu_widgets)) return FALSE;
	/* This shouldn't happen, but just in case */
	if(G_STR_EMPTY(timeline_to_open)){
		debug("**ERROR:** Cannot open an empty or NULL timeline");
		return FALSE;
	}
	
	MainWindowPrivate *m_w_p=main_window->private;
	GList *tab_menu_items=NULL;
	gboolean tab_found=FALSE;
	for(tab_menu_items=m_w_p->tabs_menu_widgets; tab_menu_items; tab_menu_items=tab_menu_items->next){
		GtkCheckMenuItem *check_menu_item=(GtkCheckMenuItem *)tab_menu_items->data;
		const gchar *timeline=g_object_get_data(G_OBJECT(check_menu_item), "timeline");
		if(!g_str_equal(timeline, timeline_to_open)) continue;
		debug("%s timeline tab: %s. GtkCheckMenuItem selected: %s", (open ?_("Opening") :_("Closing")), timeline, gtk_menu_item_get_label(GTK_MENU_ITEM(check_menu_item)));
		gtk_check_menu_item_set_active(check_menu_item, open);
		tab_found=TRUE;
	}
	g_list_free(tab_menu_items);
	return tab_found;
}/*main_window_tabs_menu_set_active(API_TIMELINE_HOMEPAGE, TRUE);*/

/* Function to retrieve the users default timeline */
gboolean main_window_tabs_init(void){
	gboolean open_home_page=TRUE;
	gchar *timeline=NULL;
	gconfig_get_string(PREFS_UPDATES_HOME_TIMELINE, &timeline);
	online_services_refresh();
	if(G_STR_EMPTY(timeline)){
		uber_free(timeline);
		open_home_page=FALSE;
	}else
		debug("Retrived default timeline: %s.  Auto-loading timeline tabs", timeline);
	
	/* TODO: Remove this check by version 010102a00						*
	 *	I'm depricating support for							*
	 *	<friends_timeline.xml> in place of <homepage.xml>				*
	 *	I'm doing this now; before <friends_timeline.xml> is depricated.		*
	 *	Twitter already has this planned and I imagine identi.ca will do so too.	*/
	gboolean open_old_homepage=FALSE;
	if((gconfig_get_bool(PREFS_AUTOLOAD_FOLLOWING, &open_old_homepage)) && open_old_homepage){
		gconfig_set_bool(PREFS_AUTOLOAD_FOLLOWING, FALSE);
		gconfig_rm_rf(PREFS_AUTOLOAD_FOLLOWING);
		if(!gconfig_if_bool(PREFS_AUTOLOAD_HOMEPAGE, TRUE))
			gconfig_set_bool(PREFS_AUTOLOAD_HOMEPAGE, TRUE);
	}
	
	if(gconfig_if_bool(PREFS_AUTOLOAD_HOMEPAGE, TRUE)){
		debug("Preparing auto-monitor for My Friends' Updates");
		main_window_tabs_menu_set_active(API_TIMELINE_HOMEPAGE, TRUE);
		if(timeline && open_home_page && g_str_equal(timeline, API_TIMELINE_HOMEPAGE))
			open_home_page=FALSE;
	}
	
	if(gconfig_if_bool(PREFS_AUTOLOAD_REPLIES, TRUE)){
		debug("Preparing auto-monitor for Replies");
		main_window_tabs_menu_set_active(API_REPLIES, TRUE);
		if(timeline && open_home_page && g_str_equal(timeline, API_REPLIES))
			open_home_page=FALSE;
	}
	
	if(gconfig_if_bool(PREFS_AUTOLOAD_DMS, TRUE)){
		debug("Preparing auto-monitor for DMs");
		main_window_tabs_menu_set_active(API_DIRECT_MESSAGES, TRUE);
		if(timeline && open_home_page && g_str_equal(timeline, API_DIRECT_MESSAGES))
			open_home_page=FALSE;
	}
	
	if(timeline && open_home_page){
		debug("Retriving default timeline: %s", timeline);
		main_window_tabs_menu_set_active(timeline, TRUE);
	}
	
	if(timeline)
		uber_free(timeline);
	
	tabs_view_page(0);
	return FALSE;
}/*main_window_tabs_init();*/

static void main_window_online_service_request_popup_menu_setup(GtkBuilder *ui){
	struct {
		const char	*gtk_image_menu_item_name;
		GPointerFunc	func;
		gpointer	user_data;
	}
	
	online_service_request_popup_image_menu_items[]={
		{ "online_service_request_popup_manage_profile_image_menu_item",
			(GPointerFunc)profile_manager_show,
			main_window->private->window },
		{ "online_service_request_popup_friends_manager_image_menu_item",
			(GPointerFunc)friends_manager_show,
			main_window->private->window },
		{ "online_service_request_popup_following_viewer_image_menu_item",
			(GPointerFunc)following_viewer_show,
			(gpointer)main_window->private->window },
		{ "online_service_request_popup_follow_image_menu_item",
			(GPointerFunc)online_service_request_popup_follow,
			NULL },
		{ "online_service_request_popup_unfollow_image_menu_item",
			(GPointerFunc)online_service_request_popup_unfollow,
			NULL },
		{ "online_service_request_popup_block_image_menu_item",
			(GPointerFunc)online_service_request_popup_block,
			NULL },
		{ "online_service_request_popup_unread_updates_image_menu_item",
			(GPointerFunc)online_service_request_popup_updates_new,
			NULL },
		{ "online_service_request_popup_updates_image_menu_item",
			(GPointerFunc)online_service_request_popup_updates,
			NULL },
		{ "online_service_request_popup_view_profile_image_menu_item",
			(GPointerFunc)online_service_request_popup_view_profile,
			NULL },
		{ "online_service_request_popup_forwards_image_menu_item",
			(GPointerFunc)online_service_request_popup_forwards,
			NULL },
		{ "online_service_request_popup_menu_best_friends_add_image_menu_item",
			(GPointerFunc)online_service_request_popup_best_friend_add,
			NULL },
		{ "online_service_request_popup_menu_best_friends_drop_image_menu_item",
			(GPointerFunc)online_service_request_popup_best_friend_drop,
			NULL },
	};
	
	GList *list=NULL;
	for(gint i=0; i < G_N_ELEMENTS(online_service_request_popup_image_menu_items); i++){
		debug("Setting up and connecting OnlineServicRequestPopup: %s", online_service_request_popup_image_menu_items[i].gtk_image_menu_item_name);
		GtkImageMenuItem *item=(GtkImageMenuItem *)gtk_builder_get_object(ui, online_service_request_popup_image_menu_items[i].gtk_image_menu_item_name);
		g_object_set_data_full(G_OBJECT(item), "gtk_image_menu_item_name", g_strdup(online_service_request_popup_image_menu_items[i].gtk_image_menu_item_name), g_free);
		g_object_set_data(G_OBJECT(item), "func", online_service_request_popup_image_menu_items[i].func);
		g_object_set_data(G_OBJECT(item), "user_data", online_service_request_popup_image_menu_items[i].user_data);
		g_signal_connect(item, "activate", G_CALLBACK(main_menu_online_service_request_menu_process), NULL);
		list=g_list_append(list, item);
	}
	main_window->private->online_service_request_popup_image_menu_items=list;
	
}/*main_window_online_service_request_popup_menu_setup(ui);*/


static void main_menu_online_service_request_menu_process(GtkImageMenuItem *item, MainWindow *main_window){
	const gchar *gtk_image_menu_item_name=g_object_get_data(G_OBJECT(item), "gtk_image_menu_item_name");
	debug("Attempting to run OnlineServicRequestPopup: %s", gtk_image_menu_item_name);
	GPointerFunc	func=g_object_get_data(G_OBJECT(item), "func");
	gpointer	user_data=g_object_get_data(G_OBJECT(item), "user_data");
	func(user_data);
}



/*BEGIN:UBERCHICK_SEXY_ENTRY_COMPLETION_COMBO_BOX*/

static void main_window_sexy_search_setup(MainWindowPrivate *m_w_p){
	m_w_p->search_history_tree_model=gtk_combo_box_get_model((GtkComboBox *)m_w_p->search_history_combo_box_entry);
	m_w_p->sexy_search_entry=(SexySpellEntry *)sexy_spell_entry_new();
	m_w_p->sexy_search_entry=g_object_ref_sink(m_w_p->sexy_search_entry);
	gtk_container_remove(GTK_CONTAINER(m_w_p->search_history_combo_box_entry), gtk_bin_get_child(GTK_BIN(m_w_p->search_history_combo_box_entry)));
	gtk_container_add(GTK_CONTAINER(m_w_p->search_history_combo_box_entry), GTK_WIDGET(m_w_p->sexy_search_entry));
	m_w_p->search_history_completion=gtk_entry_completion_new();
	gtk_entry_completion_set_model(m_w_p->search_history_completion, m_w_p->search_history_tree_model);
	gtk_entry_set_completion(GTK_ENTRY(m_w_p->sexy_search_entry), m_w_p->search_history_completion);
	gtk_combo_box_entry_set_text_column(m_w_p->search_history_combo_box_entry, GSTRING_SEARCH_PHRASE);
	gtk_entry_completion_set_text_column(m_w_p->search_history_completion, GSTRING_SEARCH_PHRASE);
	main_window_search_history_load(m_w_p);
	gtk_widget_show(GTK_WIDGET(m_w_p->sexy_search_entry));
	
	g_signal_connect_after(m_w_p->sexy_search_entry, "key-press-event", G_CALLBACK(hotkey_pressed), NULL);
	g_signal_connect(m_w_p->sexy_search_entry, "activate", G_CALLBACK(main_window_search_submitted), main_window);
}/*main_window_sexy_search_setup(main_window->private);*/

static gint main_window_search_history_validate_maxium(MainWindowPrivate *m_w_p){
	if(!m_w_p) return 0;
	gconfig_get_int_or_default(PREFS_SEARCH_HISTORY_MAXIMUM, &m_w_p->search_history_maximum, 50);
	if(m_w_p->search_history_maximum < 5)
		gconfig_set_int(PREFS_SEARCH_HISTORY_MAXIMUM, (m_w_p->search_history_maximum=5));
	else if(m_w_p->search_history_maximum > 100)
		gconfig_set_int(PREFS_SEARCH_HISTORY_MAXIMUM, (m_w_p->search_history_maximum=100));
	return m_w_p->search_history_maximum;
}/*main_window_search_history_validate_maxium(main_window->private);*/

static void main_window_search_history_load(MainWindowPrivate *m_w_p){
	gchar *previous_search=NULL, *search_history_gconf_path=NULL;
	main_window_search_history_validate_maxium(m_w_p);
	for(m_w_p->search_history_total=0; m_w_p->search_history_total<=m_w_p->search_history_maximum; m_w_p->search_history_total++){
		if(!((gconfig_get_string((search_history_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "search", m_w_p->search_history_total)), &previous_search)) && G_STR_N_EMPTY(previous_search))) break;
		
		main_window_search_history_restore(m_w_p, previous_search);
		uber_free(previous_search);
		uber_free(search_history_gconf_path);
	}

	main_window_search_history_prepend(m_w_p, "[new search]");
	if(previous_search) uber_free(previous_search);
	if(search_history_gconf_path) uber_free(search_history_gconf_path);
}/*main_window_search_history_load(main_window->private);*/

static void main_window_search_history_add(MainWindowPrivate *m_w_p, const gchar *search_phrase, gint list_store_index){
	if(G_STR_EMPTY(search_phrase)) return;
	
	if(
		G_STR_N_EMPTY(search_phrase)
		&&
		g_str_n_equal(search_phrase, "[new search]")
		&&
		gconfig_if_bool(PREFS_SEARCH_HISTORY_UNIQUE_ONLY, TRUE)
		&&
		!main_window_search_history_is_unique(m_w_p, search_phrase)
	){
		debug("Update being sent: %s is already in the search history's and will not be stored again", search_phrase);
		return;
	}
	
	main_window_search_history_validate_maxium(m_w_p);
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	if(list_store_index<-1)
		gtk_list_store_append(m_w_p->search_history_list_store, iter);
	else if(list_store_index==-1)
		gtk_list_store_prepend(m_w_p->search_history_list_store, iter);
	else if(list_store_index>-1)
		gtk_list_store_insert(m_w_p->search_history_list_store, iter, (list_store_index<m_w_p->search_history_maximum ?list_store_index :m_w_p->search_history_maximum));
	gtk_list_store_set(
			m_w_p->search_history_list_store, iter,
				GSTRING_SEARCH_PHRASE, g_strdup(search_phrase),
			-1
	);
	uber_free(iter);
	
	if(list_store_index==-3 || g_str_equal(search_phrase, "[new search]")) return;
	
	main_window_search_history_remove(m_w_p, 1);
	main_window_search_history_add(m_w_p, "[new search]", list_store_index);
	
	if(m_w_p->search_history_total<m_w_p->search_history_maximum)
		m_w_p->search_history_total++;
	else
		for(; m_w_p->search_history_total>=m_w_p->search_history_maximum; m_w_p->search_history_total--)
			main_window_search_history_remove(m_w_p, m_w_p->search_history_total);
	
	main_window_search_history_rotate(m_w_p);
	gchar *search_history_gconf_path=NULL;
	gconfig_set_string((search_history_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "search", 0)), search_phrase);
	uber_free(search_history_gconf_path);
}/*main_window_search_history_add(main_window->private, GTK_ENTRY(update_viewer->sexy_search_entry)->text, -3 to prepend w/o saving update into gconfig|-2 to prepend|-1 to append|>0 to instert at this index);*/

static gboolean main_window_search_history_is_unique(MainWindowPrivate *m_w_p, const gchar *search_phrase){
	if(G_STR_EMPTY(search_phrase)) return TRUE;
	
	if(g_str_equal(search_phrase, "[new search]"))
		return TRUE;
	
	debug("Checking uniqueness of search_phrase.  New search_phrase:\t[%s]\n", search_phrase);
	gboolean uniq=TRUE;
	gchar *search_phrase_at_index=NULL;
	for(gint i=1; i<=m_w_p->search_history_total && uniq; i++) {
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(m_w_p->search_history_tree_model, iter, path)){
			debug("Checking search_phrase, at index: %d, failed to get valid iter for the list store", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gtk_tree_model_get(
				m_w_p->search_history_tree_model, iter,
					GSTRING_SEARCH_PHRASE, &search_phrase_at_index,
				-1
		);
		
		if(!strcmp(search_phrase, search_phrase_at_index)){
			debug("Search phrase is not unique.  Duplicate search_phrase found at index: %d", i);
			debug("\tComparing new search_phrase: [%s]", search_phrase);
			debug("\tAgainst old search_phrase: [%s]", search_phrase_at_index);
			uniq=FALSE;
		}
		
		uber_free(search_phrase_at_index);
		gtk_tree_path_free(path);
		uber_free(iter);
	}
	return uniq;
}/*main_window_search_history_is_unique(main_window->private, search_phrase);*/

static void main_window_search_history_remove(MainWindowPrivate *m_w_p, gint list_store_index){
	if(!(list_store_index > 0 && list_store_index <= m_w_p->search_history_total)) return;
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	GtkTreePath *path=gtk_tree_path_new_from_indices(list_store_index, -1);
	debug("Removing saved update %d.  Total update saved: %d", list_store_index, m_w_p->search_history_total);
	if(!gtk_tree_model_get_iter(m_w_p->search_history_tree_model, iter, path))
		debug("Removing saved update, at index: %d, failed to get valid iter for the list store", list_store_index);
	else{
		debug("Removing iter at index: %d", list_store_index);
		gtk_list_store_remove(m_w_p->search_history_list_store, iter);
	}
	gtk_tree_path_free(path);
	uber_free(iter);
}/*main_window_search_history_remove(main_window->private, m_w_p->search_history_total);*/

static void main_window_search_history_rotate(MainWindowPrivate *m_w_p){
	gchar *search_history_previous_search=NULL, *search_history_gconf_path=NULL;
	for(gint i=m_w_p->search_history_maximum; i>=0; i--){
		if((gconfig_get_string((search_history_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "search", i-1)), &search_history_previous_search)) && G_STR_N_EMPTY(search_history_previous_search)){
			uber_free(search_history_gconf_path);
			search_history_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "search", i);
			gconfig_set_string(search_history_gconf_path, search_history_previous_search);
		}
		if(search_history_previous_search) uber_free(search_history_previous_search);
		uber_free(search_history_gconf_path);
	}
}/*main_window_search_history_rotate(main_window->private);*/

static void main_window_search_history_selected(GtkComboBoxEntry *search_history_combo_box_entry, MainWindow *main_window){
	GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
	if(!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(search_history_combo_box_entry), iter)){
		uber_free(iter);
		return;
	}
	
	gchar	*selected_search_phrase=NULL;
	gtk_tree_model_get(
				gtk_combo_box_get_model(GTK_COMBO_BOX(search_history_combo_box_entry)), iter,
					GSTRING_SEARCH_PHRASE, &selected_search_phrase,
				-1
	);
	
	if(!(G_STR_N_EMPTY(selected_search_phrase) && g_str_n_equal(selected_search_phrase, "[new search]"))){
		uber_free(selected_search_phrase);
		uber_free(iter);
		return;
	}
	
	main_window_sexy_search_entry_set(selected_search_phrase, FALSE);
	uber_free(selected_search_phrase);
	uber_free(iter);
}/*main_window_search_history_selected(main_window->private->search_history_combo_box_entry, main_window);*/

void main_window_sexy_search_entry_set(const gchar *search_phrase, gboolean auto_submit){
	gtk_entry_set_text(GTK_ENTRY(main_window->private->sexy_search_entry), (search_phrase==NULL ?(gchar *)"" :search_phrase));
	main_window_sexy_search_entry_select();
	if(auto_submit) main_window_search_submitted(GTK_WIDGET(main_window->private->sexy_search_entry), main_window);
}/*main_window_sexy_search_entry_set();*/

static void main_window_search_submitted(GtkWidget *search_widget, MainWindow *main_window){
	gchar *search_phrase=GTK_ENTRY(main_window->private->sexy_search_entry)->text;
	if(G_STR_EMPTY(search_phrase)) return;
	main_window_search_history_prepend(main_window->private, search_phrase);
	debug("Searching for %s", search_phrase);
	gchar *search_timeline=NULL;
	if(search_phrase[0]!='!'){
		search_phrase=g_uri_escape_string(search_phrase, NULL, TRUE);
		search_timeline=g_strdup_printf(API_TIMELINE_SEARCH_SUBMIT, search_phrase);
	}else{
		search_phrase=g_uri_escape_string(&search_phrase[1], NULL, TRUE);
		search_timeline=g_strdup_printf(API_STATUSNET_GROUPS_SEARCH_SUBMIT, search_phrase);
	}
	tabs_open_timeline(search_timeline, NULL);
	uber_free(search_phrase);
	uber_free(search_timeline);
}/*main_window_search_submitted(main_window->private->search_button, main_window->private);*/

SexySpellEntry *main_window_sexy_search_entry_get_widget(void){
	if(! main_window->private->sexy_search_entry) return NULL;
	return main_window->private->sexy_search_entry;
}/*main_window_sexy_search_entry_get_widget();*/

void main_window_sexy_search_entry_select(void){
	if(gtk_widget_has_focus(GTK_WIDGET(main_window->private->sexy_search_entry))) return;
	if(gtk_widget_has_focus(update_viewer_sexy_entry_get_widget())) return;
	gtk_widget_grab_focus(GTK_WIDGET(main_window->private->sexy_search_entry));
	gint sexy_position=-1;
	if(main_window->private->sexy_position > 0 && main_window->private->sexy_position <= gtk_entry_get_text_length((GtkEntry *)main_window->private->sexy_search_entry))
		sexy_position=main_window->private->sexy_position;
	gtk_entry_set_position(GTK_ENTRY(main_window->private->sexy_search_entry), sexy_position);
}/*main_window_sexy_search_entry_select();*/

void main_window_show_search_history(void){
	g_signal_emit_by_name(main_window->private->search_history_combo_box_entry, "popup");
}/*main_window_show_search_history();*/

void main_window_hide_search_history(void){
	g_signal_emit_by_name(main_window->private->search_history_combo_box_entry, "popdown");
}/*main_window_hide_search_history();*/


/*END:UBERCHICK_SEXY_ENTRY_COMPLETION_COMBO_BOX*/




static void main_window_services_cb(GtkWidget *widget, MainWindow *main_window){
	online_services_dialog_show(main_window->private->window);
}

static void main_window_select_service(GtkMenuItem *item, MainWindow *main_window){
	online_service_request_popup_select_service();
}/*main_window_select_service*/

static void main_window_preferences_cb(GtkWidget *widget, MainWindow *main_window){
	preferences_dialog_show(GTK_WINDOW(main_window->private->window));
}

static void main_window_about_cb(GtkWidget *widget, MainWindow *main_window){
	about_dialog_new(GTK_WINDOW(main_window->private->window));
}

static void main_window_help_contents_cb(GtkWidget *widget, MainWindow *main_window){
	help_show(GTK_WINDOW(main_window->private->window));
}

static void main_window_show_hide_cb(GtkWidget *widget, MainWindow *main_window){
	main_window_toggle_visibility();
}

GtkStatusIcon *main_window_status_icon_get(void){
	if(!main_window->private->status_icon)
		main_window_status_icon_create();
	
	return main_window->private->status_icon;
}/*main_window_status_icon_get();*/

gboolean main_window_status_icon_is_embedded(void){
	return gtk_status_icon_is_embedded(main_window->private->status_icon);
}/*main_window_status_icon_is_embedded();*/

static void main_window_status_icon_activate_cb(GtkStatusIcon *status_icon, MainWindow *main_window){
	main_window_toggle_visibility();
}

static void main_window_status_icon_popup_menu_cb(GtkStatusIcon *status_icon, guint button, guint activate_time, MainWindow *main_window){
	gboolean show=window_get_is_visible(GTK_WINDOW(main_window->private->window));
	
	g_signal_handlers_block_by_func(main_window->private->popup_menu_show_main_window, main_window_show_hide_cb, main_window);
	
	gtk_toggle_action_set_active(main_window->private->popup_menu_show_main_window, show);
	
	g_signal_handlers_unblock_by_func(main_window->private->popup_menu_show_main_window, main_window_show_hide_cb, main_window);
	
	gtk_menu_popup(GTK_MENU(main_window->private->popup_menu), NULL, NULL, gtk_status_icon_position_menu, main_window->private->status_icon, button, activate_time);
}

static void main_window_status_icon_create_menu(void){
	GtkWidget *w=NULL;
	GtkAction *action=NULL;
	
	main_window->private->popup_menu=gtk_menu_new();
	main_window->private->popup_menu_show_main_window=gtk_toggle_action_new("tray_show_app", _("_Show "), _("Show, or hide, get2gnow's main window"), NULL);
	g_signal_connect(G_OBJECT(main_window->private->popup_menu_show_main_window), "toggled", G_CALLBACK(main_window_show_hide_cb), main_window);
	w=gtk_action_create_menu_item(GTK_ACTION(main_window->private->popup_menu_show_main_window));
	gtk_widget_show(w);
	gtk_menu_shell_append(GTK_MENU_SHELL(main_window->private->popup_menu), w);
	
	
	w=gtk_separator_menu_item_new();
	gtk_widget_show(w);
	gtk_menu_shell_append(GTK_MENU_SHELL(main_window->private->popup_menu), w);
	
	action=gtk_action_new("tray_new_message", _("_New Update"), _("Start a new update"), "gtk-new");
	g_signal_connect(G_OBJECT(action), "activate", G_CALLBACK(update_viewer_new_update), main_window);
	w=gtk_action_create_menu_item(action);
	gtk_menu_shell_append(GTK_MENU_SHELL(main_window->private->popup_menu), w);
	
	action=gtk_action_new("tray_new_dm", _("New _DM"), _("Send a direct message to one of your followers"), "gtk-jump-to");
	g_signal_connect(G_OBJECT(action), "activate", G_CALLBACK(update_viewer_new_dm), main_window);
	w=gtk_action_create_menu_item(action);
	gtk_menu_shell_append(GTK_MENU_SHELL(main_window->private->popup_menu), w);
	
	action=gtk_action_new("tray_about", _("_About"), _("View information about get2gnow"), "gtk-about");
	g_signal_connect(G_OBJECT(action), "activate", G_CALLBACK(main_window_about_cb), main_window);
	w=gtk_action_create_menu_item(action);
	gtk_menu_shell_append(GTK_MENU_SHELL(main_window->private->popup_menu), w);
	
	action=gtk_action_new("tray_quit", _("_Quit"), _("Quit get2gnow"), "gtk-quit");
	g_signal_connect(G_OBJECT(action), "activate", G_CALLBACK(main_window_exit), main_window);
	w=gtk_action_create_menu_item(action);
	gtk_menu_shell_append(GTK_MENU_SHELL(main_window->private->popup_menu), w);
}

static void main_window_status_icon_create(void){
	/* Set-up the system notification area icon and its popup menu. */
	debug("Configuring system notification icon");
	main_window_status_icon_create_menu();
	
	main_window->private->status_icon=gtk_status_icon_new_from_icon_name(_(GETTEXT_PACKAGE));
	g_signal_connect(main_window->private->status_icon, "activate", G_CALLBACK(main_window_status_icon_activate_cb), main_window);
	
	g_signal_connect(main_window->private->status_icon, "popup_menu", G_CALLBACK(main_window_status_icon_popup_menu_cb), main_window);
	
	gtk_status_icon_set_visible(main_window->private->status_icon, TRUE);
}

void main_window_create(void){
	g_object_new(TYPE_MAIN_WINDOW, NULL);
	main_window_setup();
}

MainWindow *main_window_get(void){
	g_assert(main_window != NULL);
	return main_window;
}
 
static gboolean configure_event_timeout_cb(GtkWidget *widget){
	geometry_save();
	
	main_window->private->size_timeout_id=0;
	
	return FALSE;
}

static gboolean main_window_window_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, MainWindow *main_window){
	program_timeout_remove(&main_window->private->size_timeout_id, _("main window configuration"));
	
	main_window->private->size_timeout_id=g_timeout_add_seconds(1, (GSourceFunc)configure_event_timeout_cb, widget);
	
	return FALSE;
}

static void main_window_login(void){
	debug("Logging into online services");
	if(!online_services_login()){
		online_services_dialog_show(main_window->private->window);
		return;
	}
	
	main_window_tabs_init();
}/*main_window_login*/

static void main_window_reconnect(GtkMenuItem *item, MainWindow *main_window){
	if(!(online_services_reconnect()))
		return;
	
	main_window_tabs_init();
}/*main_window_reconnect*/

void main_window_disconnect(void){
	main_window_state_on_connection(FALSE);
	online_services_disconnect();
}/*main_window_disconnect*/

static void main_window_selected_update_widgets_setup(GtkBuilder *ui){
	const gchar *selected_update_buttons[]={
		"selected_update_submenu_image_menu_item",
		"selected_update_reply_image_menu_item",
		"selected_update_forward_update_image_menu_item",
		"selected_update_save_fave_image_menu_item",
		"selected_update_author_view_profile_image_menu_item",
		"selected_update_author_view_unread_updates_image_menu_item",
		"selected_update_author_view_updates_image_menu_item",
		"selected_update_author_best_friend_image_menu_item",
		"selected_update_author_follow_image_menu_item",
		"selected_update_author_unfollow_image_menu_item",
		"selected_update_author_block_image_menu_item",
	};
	
	GList *list=NULL;
	for(int i=0; i < G_N_ELEMENTS(selected_update_buttons); i++)
		list=g_list_append(list, (gtk_builder_get_object(ui, selected_update_buttons[i])));
	main_window->private->selected_update_image_menu_items=list;
}/*main_window_selected_update_widgets_setup(ui);*/

static void main_window_connection_items_setup(GtkBuilder *ui){
	GList         *list;
	gint           i;
	
	const gchar   *widgets_connected[]={
		"services_disconnect",
		"compose_image_menu_item",
		"tabs_menu_item",
		"online_service_request_menu_item",
		"expand_box",
	};
	
	const gchar   *widgets_disconnected[]={
		"services_connect",
	};
	
	for(i=0, list=NULL; i < G_N_ELEMENTS(widgets_connected); i++)
		list=g_list_prepend(list, (gtk_builder_get_object(ui, widgets_connected[i])));
	main_window->private->widgets_connected=list;
	
	for(i=0, list=NULL; i < G_N_ELEMENTS(widgets_disconnected); i++)
		list=g_list_prepend(list, (gtk_builder_get_object(ui, widgets_disconnected[i])));
	main_window->private->widgets_disconnected=list;
}/*main_window_connection_items_setup(ui);*/

void main_window_state_on_connection(gboolean connected){
	if(!(main_window->private && main_window->private->widgets_connected && main_window->private->widgets_disconnected))
		return;
	
	if(main_window->private->connected==connected) return;
	main_window->private->connected=connected;
	
	if(!connected){
		update_viewer_new_update();
		main_window_statusbar_timeouts_free();
		online_service_request_unset_selected_update();
		main_window_set_statusbar_default_message(_("You're currently not connected to any Online Services."));
		tabs_stop();
	}else{
		update_viewer_new_update();
		tabs_refresh();
	}
	
	
	GList         *l;
	for(l=main_window->private->widgets_connected; l; l=l->next)
		gtk_widget_set_sensitive(GTK_WIDGET(l->data), connected);
	g_list_free(l);
	
	for(l=main_window->private->widgets_disconnected; l; l=l->next)
		gtk_widget_set_sensitive(GTK_WIDGET(l->data), !connected);
	g_list_free(l);
	
	if(connected) update_viewer_sexy_select(update_viewer_sexy_entry_get_widget());
}

void main_window_selected_update_image_menu_items_show(gboolean selected_update){
	if(!(main_window->private && main_window->private->selected_update_image_menu_items)) return;
	
	GList *selected_update_image_menu_items;
	for(selected_update_image_menu_items=main_window->private->selected_update_image_menu_items; selected_update_image_menu_items; selected_update_image_menu_items=selected_update_image_menu_items->next)
		gtk_widget_set_sensitive(GTK_WIDGET(selected_update_image_menu_items->data), selected_update);
	g_list_free(selected_update_image_menu_items);
	update_viewer_sexy_select(update_viewer_sexy_entry_get_widget());
}/*main_window_selected_update_image_menu_items_show(TRUE|FALSE);*/

const gchar *main_window_set_statusbar_default_message(const gchar *default_message){
	if(g_str_equal(main_window->private->statusbar_default_message, default_message))
		return main_window->private->statusbar_default_message;
	
	uber_free(main_window->private->statusbar_default_message);
	main_window->private->statusbar_default_message=g_strdup(default_message);
	if(!statusbar_inital_context_id)
		statusbar_inital_context_id=gtk_statusbar_push(main_window->private->statusbar, 1, main_window->private->statusbar_default_message);
	return main_window->private->statusbar_default_message;
}/*main_window_set_statusbar_default_message(_("Default message."));*/

void main_window_statusbar_printf(const gchar *msg, ...){
	if(!(main_window->private && main_window->private->statusbar && GTK_IS_STATUSBAR(main_window->private->statusbar)))
		return;
	
	gchar *message=NULL;
	va_list words_and_more;
	
	va_start(words_and_more, msg);
	message=g_strdup_vprintf(msg, words_and_more);
	va_end(words_and_more);
	
	main_window_set_statusbar_msg(message);
	g_free(message);
}/*main_window_statusbar_printf("__format", ...);*/

static GSList *statusbar_timers=NULL;
static guint statusbar_messages_total=0;
void main_window_set_statusbar_msg(gchar *message){
	static guint8 statusbar_messages_timeout=5;
	if(!(main_window->private && main_window->private->statusbar && GTK_IS_STATUSBAR(main_window->private->statusbar)))
		return;
	
	if(G_STR_N_EMPTY(message) && !g_str_equal(message, main_window->private->statusbar_default_message))
		statusbar_timers=g_slist_append(statusbar_timers, GINT_TO_POINTER(g_timeout_add_seconds_full(G_PRIORITY_DEFAULT, ++statusbar_messages_total+statusbar_messages_timeout, (GSourceFunc)main_window_statusbar_display, g_strdup(message), g_free)));
	
	if(main_window->private->timeout_id_status_bar_message_default)
		program_timeout_remove(&main_window->private->timeout_id_status_bar_message_default, _("status bar default message"));
	
	main_window->private->timeout_id_status_bar_message_default=g_timeout_add_seconds_full(G_PRIORITY_DEFAULT, ++statusbar_messages_total+statusbar_messages_timeout, (GSourceFunc)main_window_statusbar_display, g_strdup(main_window->private->statusbar_default_message), g_free);
}/*main_window_set_statusbar_msg("Message...");*/

static gboolean main_window_statusbar_display(const gchar *message){
	if(statusbar_inital_context_id){
		gtk_statusbar_remove(main_window->private->statusbar, 1, statusbar_inital_context_id);
		statusbar_inital_context_id=0;
	}
	gtk_statusbar_pop(GTK_STATUSBAR(main_window->private->statusbar), 1);
	gtk_statusbar_push(GTK_STATUSBAR(main_window->private->statusbar), 1, (G_STR_N_EMPTY(message) ?message :main_window->private->statusbar_default_message));
	
	if(statusbar_messages_total && statusbar_timers && statusbar_timers->data && !g_str_equal(message, main_window->private->statusbar_default_message))
		statusbar_messages_total--;
	statusbar_messages_total--;
	
	return FALSE;
}/*main_window_set_statusbar_display(gpointer);*/

static void main_window_statusbar_timeouts_free(void){
	if(statusbar_timers)
		for(statusbar_timers=g_slist_nth(statusbar_timers, 0); statusbar_messages_total; --statusbar_messages_total, statusbar_timers=g_slist_nth(statusbar_timers, 0))
			main_window_statusbar_timer_remove();
	
	uber_slist_free(statusbar_timers);
	if(statusbar_messages_total) statusbar_messages_total=0;
	
	program_timeout_remove(&main_window->private->timeout_id_status_bar_message_default, _("statusbar default message"));
}/*main_window_statusbar_timeouts_free();*/

static void main_window_statusbar_timer_remove(void){
	if(!(statusbar_timers && statusbar_timers->data)) return;
	
	gpointer timer_pointer=statusbar_timers->data;
	guint timer=GPOINTER_TO_INT(timer_pointer);
	program_timeout_remove(&timer, _("status bar message"));
	statusbar_timers=g_slist_remove(statusbar_timers, timer_pointer);
	if(statusbar_messages_total) statusbar_messages_total--;
}/*main_window_statusbar_timer_remove();*/


