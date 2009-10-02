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

#include <canberra-gtk.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <libnotify/notify.h>
#include <gdk/gdkkeysyms.h>
#include <libsexy/sexy.h>

#include "config.h"
#include "program.h"

#include "gconfig.h"
#include "gtkbuilder.h"
#include "parser.h"

#include "about.h"
#include "online-services-typedefs.h"
#include "online-service-request.h"
#include "online-service.types.h"
#include "online-service.h"
#include "online-services.h"
#include "online-services-dialog.h"
#include "main-window.h"

#include "users.types.h"
#include "users.h"

#include "images.h"
#include "preferences.h"
#include "ui-utils.h"
#include "geometry.h"

#include "label.h"
#include "preferences.h"

#include "tweets.h"
#include "tweet-lists.h"
#include "tweet-list.h"

#include "friends-manager.h"
#include "following-viewer.h"
#include "control-panel.h"


#define	GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE(( obj), TYPE_MAIN_WINDOW, MainWindowPrivate ))

struct _MainWindowPrivate {
	/* Misc */
	guint			size_timeout_id;
	guint			timeout_id_status_bar;
	
	/* Status Icon Stuff. */
	GtkStatusIcon		*status_icon;
	GtkWidget		*popup_menu;
	GtkToggleAction		*popup_menu_show_main_window;
	
	/* Main widgets */
	GtkWindow		*window;
	GtkVBox			*main_vbox;
	GtkWindow		*control_panel_window;
	
	GtkMenuBar		*menubar;
        GtkMenuItem		*menu_network;
	
	/* Tools / Edit menu items. */
        GtkMenuItem		*menu_edit;
	
        GtkMenuItem		*menu_view;
	GtkCheckMenuItem	*view_toolbar_main_check_menu_item;
	GtkCheckMenuItem	*view_toolbar_tabs_check_menu_item;
	GtkCheckMenuItem	*view_best_friends_check_menu_item;
	GtkCheckMenuItem	*view_control_panel_floating_check_menu_item;
	GtkCheckMenuItem	*view_control_panel_compact_view_check_menu_item;
	
	/* This is the 'Tools' menu. */
	GtkMenuItem		*menu_online_service_request;
	GtkImageMenuItem	*online_service_request_menu_friends_manager;
	GtkImageMenuItem	*online_service_request_menu_following_viewer;
	GtkImageMenuItem	*online_service_request_menu_follow;
	GtkImageMenuItem	*online_service_request_menu_unfollow;
	GtkImageMenuItem	*online_service_request_menu_block;
	GtkImageMenuItem	*online_service_request_menu_updates;
	GtkImageMenuItem	*online_service_request_menu_profile;
	GtkImageMenuItem	*online_service_request_menu_best_friend;
	
	/* Timeline menu items */
	GtkMenuItem		*menu_tabs;
	GtkCheckMenuItem	*timeline_public;
	GtkCheckMenuItem	*timeline_friends;
	GtkCheckMenuItem	*timeline_mine;
	GtkCheckMenuItem	*timeline_dm;
	GtkCheckMenuItem	*timeline_favorites;
	GtkCheckMenuItem	*timeline_replies;
								
	GtkMenuItem		*menu_help;
	
	GtkHandleBox		*main_window_handlebox;
	GtkToolbar		*main_toolbar;
	
	/* 'New Tabs' Timeline menu items */
	GtkMenu			*tabs_menu;
	
	GtkComboBoxEntry	*search_combo_box_entry;
	GtkListStore		*search_list_store;
	GtkTreeModel		*search_tree_model;
	GtkButton		*search_button;
	
	GtkToolButton		*preferences_tool_button;
	GtkToolButton		*accounts_tool_button;
	GtkToolButton		*exit_tool_button;
	
	/* Widgets that are enabled when we are connected/disconnected */
	GList			*widgets_connected;
	GList			*widgets_disconnected;
	GList			*selected_update_image_menu_items;
	
	/* OnlineServices "best friends" stuff. */
	GtkHPaned		*tweet_list_hpaned;
	GList			*best_friends_buttons;
	GtkVBox			*best_friends_vbox;
	GtkScrolledWindow	*best_friends_scrolled_window;
	
	SexyTreeView		*best_friends_sexy_tree_view;
	GtkTreeViewColumn	*best_friends_user_name_tree_view_column;
	GtkCellRendererText	*best_friends_user_name_cell_renderer_text;
	GtkTreeViewColumn	*best_friends_online_service_tree_view_column;
	GtkCellRendererText	*best_friends_online_service_cell_renderer_text;
	
	GtkListStore		*best_friends_list_store;
	GtkTreeModel		*best_friends_tree_model;
	GtkTreeModel		*best_friends_tree_model_sort;
	
	GtkButton		*best_friends_add_button;
	GtkButton		*best_friends_drop_button;
	GtkButton		*best_friends_refresh_button;
	
	GtkButton		*best_friends_view_updates_new_button;
	GtkButton		*best_friends_view_profile_button;
	GtkButton		*best_friends_send_at_message_button;
	GtkButton		*best_friends_send_dm_button;
	GtkButton		*best_friends_unfollow_button;
	GtkButton		*best_friends_view_updates_button;
	
	/* user, status, & update widgets.
	 * Actually they're in the ControlPanel.
	 * The main-window's GtkVBox contains them.
	 */
	GtkVPaned		*tweet_vpaned;
	GtkHBox			*expand_box;
	GtkVBox			*control_panel_vbox;
	
	/*These are or part of 'ControlPanel'.*/
	ControlPanel		*control_panel;
	GtkHBox			*control_panel_embed;
	
	gchar			*tabs_to_right_align;
	GtkStatusbar		*statusbar;
};

/* UI & notification gconf values. */
#define MAIN_WINDOW_UI_HIDDEN			GCONF_PATH "/ui/hide/main_window"
#define MAIN_WINDOW_BEST_FRIENDS_HIDE_VBOX	GCONF_PATH "/ui/hide/best_friends"

#define MAIN_WINDOW_MAIN_TOOLBAR_HIDE		GCONF_PATH "/ui/hide/main_toolbar"

#define PREFS_HINTS_HIDE_MAIN_WINDOW		GCONF_PATH "/popup_confirmation_dialog/disabled_when/closing/main_window"
#define PREFS_HINTS_CLOSE_MAIN_WINDOW		GCONF_PATH "/popup_confirmation_dialog/disabled_when/hiding/main_window"

#define	DEBUG_DOMAINS	"UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Tweets:Requests:Users:Authentication:Preferences:Settings:Setup:Start-Up:main-window.c"
#include "debug.h"

#define	GtkBuilderUI	"main-window"


static void main_window_class_init(MainWindowClass *klass);
static void main_window_init(MainWindow *main_window);
static void main_window_finalize(GObject *object);

static gboolean main_window_statusbar_display(const gchar *message);

static void main_window_setup(void);
static void main_window_best_friends_resized(GtkScrolledWindow *best_friends_scrolled_window, GtkAllocation *allocation, MainWindow *main_window);
static void main_window_best_friends_setup(GtkBuilder *ui);
static void main_window_best_friends_list_store_validate(GtkButton *best_friends_refresh_button, MainWindow *main_window );
static void main_window_best_friends_buttons_set_sensitive(void);
static void main_window_best_friends_tree_view_row_activated(GtkTreeView *best_friends_tree_view, MainWindow *main_window);
static void main_window_best_friends_button_clicked(GtkButton *button);
static void main_window_view_setup(void);

static void main_window_destroy_cb(GtkWidget *window, MainWindow *main_window); 
static gboolean main_window_delete_event_cb(GtkWidget *window, GdkEvent *event, MainWindow *main_window);
static void main_window_exit(GtkWidget *window, MainWindow *main_window); 
static void main_window_services_cb(GtkWidget *window, MainWindow *main_window); 
static void main_window_select_service(GtkMenuItem *item, MainWindow *main_window);
static void main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(GtkCheckMenuItem *check_menu_item, const gchar *gconfig_key, GtkWidget *widget);
static void main_window_view_menu_option_toggled(GtkCheckMenuItem *check_menu_item);
static void main_window_preferences_cb(GtkWidget *window, MainWindow *main_window); 

static void main_window_about_cb(GtkWidget *window, MainWindow *main_window); 
static void main_window_help_contents_cb(GtkWidget *widget, MainWindow *main_window); 
static void main_window_status_icon_activate_cb(GtkStatusIcon *status_icon, MainWindow *main_window); 
static void main_window_status_icon_popup_menu_cb(GtkStatusIcon *status_icon, guint button, guint activate_time, MainWindow *main_window);

static void main_window_tabs_menu_timeline_selected(GtkRadioMenuItem *item, MainWindow *main_window);
static void main_window_tabs_menu_set_active(MainWindow *main_window, gchar *timeline);

static void online_service_request_menu_process(GtkImageMenuItem *item, MainWindow *main_window);

static void main_window_connection_items_setup(GtkBuilder *ui);
static void main_window_selected_update_widgets_setup(GtkBuilder *ui);
static void main_window_login(void);
static void main_window_reconnect(GtkMenuItem *item, MainWindow *main_window);

static void main_window_status_icon_create_menu(void);
static void main_window_status_icon_create(void);

static void main_window_set_visibility(gboolean visible);
static void main_window_toggle_visibility(void);

static gboolean configure_event_timeout_cb(GtkWidget *widget);
static gboolean main_window_window_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, MainWindow *main_window);


static MainWindow  *main_window=NULL;
//static MainWindowPriv *main_window->private=NULL;

G_DEFINE_TYPE(MainWindow, main_window, G_TYPE_OBJECT);

static void main_window_class_init(MainWindowClass *klass){
	GObjectClass *object_class=G_OBJECT_CLASS(klass);
	object_class->finalize=main_window_finalize;
	g_type_class_add_private(object_class, sizeof(MainWindowPrivate));
}/*main_window_class_init( GET_CLASS(main_window) );*/

static void main_window_init(MainWindow *singleton_main_window){
	main_window=singleton_main_window;
	
	main_window->private=GET_PRIVATE(main_window);
	main_window->private->widgets_connected=NULL;
	main_window->private->widgets_disconnected=NULL;
	online_service_request_unset_selected_update();
	main_window->private->tabs_to_right_align=g_strdup("\t");
}/*main_window_init(main_window);*/

static void main_window_finalize(GObject *object){
	MainWindow	       *main_window;
	
	main_window=MAIN_WINDOW(object);
	main_window->private=GET_PRIVATE(main_window);
	
	program_timeout_remove(&main_window->private->size_timeout_id, _("main window configuration"));
	program_timeout_remove(&main_window->private->timeout_id_status_bar, _("status bar message"));
	
	g_list_free(main_window->private->widgets_connected);
	g_list_free(main_window->private->widgets_disconnected);
	tweet_lists_destroy();
	if(main_window->private->best_friends_tree_model_sort)
		g_object_unref(main_window->private->best_friends_tree_model_sort);
	
	G_OBJECT_CLASS(main_window_parent_class)->finalize(object);
}/*main_window_finalize(main_window);*/

static void main_window_setup(void){
	GtkBuilder	*ui;
	
	debug("Starting %s...", PACKAGE_NAME);
	GtkNotebook	*tweet_lists_notebook;
	
	
	/* Set up interface */
	debug("Initialising interface");
	ui=gtkbuilder_get_file(
				GtkBuilderUI,
					"main_window", &main_window->private->window,
					"main_vbox", &main_window->private->main_vbox,
					
					"main_menubar", &main_window->private->menubar,
					"network", &main_window->private->menu_network,
					"edit", &main_window->private->menu_edit,
					
					"tabs", &main_window->private->menu_tabs,
					"tabs_public_timeline", &main_window->private->timeline_public,
					"tabs_friends_timeline", &main_window->private->timeline_friends,
					"tabs_my_timeline", &main_window->private->timeline_mine,
					"tabs_direct_messages", &main_window->private->timeline_dm,
					"tabs_replies", &main_window->private->timeline_replies,
					"tabs_favorites_timeline", &main_window->private->timeline_favorites,
					
					"view", &main_window->private->menu_view,
					"view_toolbar_main_check_menu_item", &main_window->private->view_toolbar_main_check_menu_item,
					"view_toolbar_tabs_check_menu_item", &main_window->private->view_toolbar_tabs_check_menu_item,
					"view_best_friends_check_menu_item", &main_window->private->view_best_friends_check_menu_item,
					"view_control_panel_floating_check_menu_item", &main_window->private->view_control_panel_floating_check_menu_item,
					"view_control_panel_compact_view_check_menu_item", &main_window->private->view_control_panel_compact_view_check_menu_item,
					
					"online_service_request", &main_window->private->menu_online_service_request,
					"online_service_request_menu_friends_manager", &main_window->private->online_service_request_menu_friends_manager,
					"online_service_request_menu_following_viewer", &main_window->private->online_service_request_menu_following_viewer,
					"online_service_request_menu_follow", &main_window->private->online_service_request_menu_follow,
					"online_service_request_menu_unfollow", &main_window->private->online_service_request_menu_unfollow,
					"online_service_request_menu_block", &main_window->private->online_service_request_menu_block,
					"online_service_request_menu_updates", &main_window->private->online_service_request_menu_updates,
					"online_service_request_menu_profile", &main_window->private->online_service_request_menu_profile,
					
					"help", &main_window->private->menu_help,
					/* end menu items. */
					
					/* start:main toolbar */
					"main_window_handlebox", &main_window->private->main_window_handlebox,
					"main_toolbar", &main_window->private->main_toolbar,
					
					"search_combo_box_entry", &main_window->private->search_combo_box_entry,
					"search_list_store", &main_window->private->search_list_store,
					"search_button", &main_window->private->search_button,
					
					"preferences_tool_button", &main_window->private->preferences_tool_button,
					"accounts_tool_button", &main_window->private->accounts_tool_button,
					"main_window_main_tool_bar_exit_tool_button", &main_window->private->exit_tool_button,
					
					"tweet_list_hpaned", &main_window->private->tweet_list_hpaned,
					"best_friends_vbox", &main_window->private->best_friends_vbox,
					"best_friends_scrolled_window", &main_window->private->best_friends_scrolled_window,
					
					"best_friends_sexy_tree_view", &main_window->private->best_friends_sexy_tree_view,
					"best_friends_user_name_tree_view_column", &main_window->private->best_friends_user_name_tree_view_column,
					"best_friends_user_name_cell_renderer_text", &main_window->private->best_friends_user_name_cell_renderer_text,
					"best_friends_online_service_tree_view_column", &main_window->private->best_friends_online_service_tree_view_column,
					"best_friends_online_service_cell_renderer_text", &main_window->private->best_friends_online_service_cell_renderer_text,
					
					"best_friends_list_store", &main_window->private->best_friends_list_store,
					"best_friends_add_button", &main_window->private->best_friends_add_button,
					"best_friends_drop_button", &main_window->private->best_friends_drop_button,
					"best_friends_refresh_button", &main_window->private->best_friends_refresh_button,
					"best_friends_view_updates_new_button", &main_window->private->best_friends_view_updates_new_button,
					"best_friends_view_profile_button", &main_window->private->best_friends_view_profile_button,
					"best_friends_send_at_message_button", &main_window->private->best_friends_send_at_message_button,
					"best_friends_send_dm_button", &main_window->private->best_friends_send_dm_button,
					"best_friends_unfollow_button", &main_window->private->best_friends_unfollow_button,
					"best_friends_view_updates_button", &main_window->private->best_friends_view_updates_button,
					
					/*tweet_lists_notebook is used to contain get2gnow's timeline tabs.*/
					"tweet_lists_notebook", &tweet_lists_notebook,
					
					"tweet_vpaned", &main_window->private->tweet_vpaned,
					"expand_box", &main_window->private->expand_box,
					"control_panel_vbox", &main_window->private->control_panel_vbox,
					
					"main_statusbar", &main_window->private->statusbar,
				NULL
	);
	
	tweet_lists_init(tweet_lists_notebook);
	
	/* Connect the signals */
	gtkbuilder_connect( ui, main_window,
					"main_window", "destroy", main_window_destroy_cb,
					"main_window", "delete_event", main_window_delete_event_cb,
					"main_window", "configure_event", main_window_window_configure_event_cb,
					"main_window", "grab-focus", control_panel_sexy_select,
					
					"services_connect", "activate", main_window_reconnect,
					"services_disconnect", "activate", main_window_disconnect,
					"accounts_image_menu_item", "activate", main_window_services_cb,
					"select_service_image_menu_item", "activate", main_window_select_service,
					"preferences", "activate", main_window_preferences_cb,
					"quit", "activate", main_window_exit,
					
					"tweets_new_tweet", "activate", tweets_new_tweet,
					"tweets_new_dm", "activate", control_panel_new_dm,
					"selected_update_reply_image_menu_item", "activate", online_service_request_selected_update_reply,
					"selected_update_retweet_image_menu_item", "activate", online_service_request_selected_update_retweet,
					"selected_update_save_fave_image_menu_item", "activate", online_service_request_selected_update_save_fave,
					"selected_update_author_view_profile_image_menu_item", "activate", online_service_request_selected_update_view_profile,
					"selected_update_author_view_updates_image_menu_item", "activate", online_service_request_selected_update_view_updates,
					"selected_update_author_best_friend_image_menu_item", "activate", online_service_request_selected_update_best_friend_add,
					"selected_update_author_follow_image_menu_item", "activate", online_service_request_selected_update_follow,
					"selected_update_author_unfollow_image_menu_item", "activate", online_service_request_selected_update_unfollow,
					"selected_update_author_block_image_menu_item", "activate", online_service_request_selected_update_block,
					
					"tabs_public_timeline", "activate", main_window_tabs_menu_timeline_selected,
					"tabs_friends_timeline", "activate", main_window_tabs_menu_timeline_selected,
					"tabs_my_timeline", "activate", main_window_tabs_menu_timeline_selected,
					"tabs_direct_messages", "activate", main_window_tabs_menu_timeline_selected,
					"tabs_replies", "activate", main_window_tabs_menu_timeline_selected,
					"tabs_favorites_timeline", "activate", main_window_tabs_menu_timeline_selected,
					
					"online_service_request_menu_friends_manager", "activate", online_service_request_menu_process,
					"online_service_request_menu_following_viewer", "activate", online_service_request_menu_process,
					"online_service_request_menu_follow", "activate", online_service_request_menu_process,
					"online_service_request_menu_unfollow", "activate", online_service_request_menu_process,
					"online_service_request_menu_block", "activate", online_service_request_menu_process,
					"online_service_request_menu_updates", "activate", online_service_request_menu_process,
					"online_service_request_menu_profile", "activate", online_service_request_menu_process,
					
					"help_contents", "activate", main_window_help_contents_cb,
					"help_about", "activate", main_window_about_cb,
					
					"accounts_tool_button", "clicked", main_window_services_cb,
					"select_service_tool_button", "clicked", main_window_select_service,
					"preferences_tool_button", "clicked", main_window_preferences_cb,
					"main_window_main_tool_bar_exit_tool_button", "clicked", main_window_exit,
					
					"best_friends_scrolled_window", "size-allocate", main_window_best_friends_resized,
					
					"best_friends_sexy_tree_view", "cursor-changed", main_window_best_friends_buttons_set_sensitive,
					"best_friends_sexy_tree_view", "row-activated", main_window_best_friends_tree_view_row_activated,
					
					"best_friends_add_button", "clicked", online_service_request_popup_best_friend_add,
					"best_friends_drop_button", "clicked", main_window_best_friends_button_clicked,
					
					"best_friends_refresh_button", "clicked", main_window_best_friends_list_store_validate,
					
					"best_friends_view_updates_new_button", "clicked", main_window_best_friends_button_clicked,
					"best_friends_view_profile_button", "clicked", main_window_best_friends_button_clicked,
					"best_friends_send_at_message_button", "clicked", main_window_best_friends_button_clicked,
					"best_friends_send_dm_button", "clicked", main_window_best_friends_button_clicked,
					"best_friends_unfollow_button", "clicked", main_window_best_friends_button_clicked,
					"best_friends_view_updates_button", "clicked", main_window_best_friends_button_clicked,
				NULL
	);
	
	/* TODO: fix online_service_best_friends_list_store_validate(); */
	gtk_widget_hide( GTK_WIDGET( main_window->private->best_friends_refresh_button ) );
	
	/*main_window->private->search_tree_model=gtk_combo_box_get_model( (GtkComboBox *)main_window->private->search_combo_box_entry ));*/
	g_signal_connect_after( main_window->private->window, "event-after", (GCallback)control_panel_sexy_select, NULL );
	
	main_window_best_friends_setup(ui);
	main_window_view_setup();
	
	/* Set up connected related widgets */
	main_window_connection_items_setup(ui);
	main_window_selected_update_widgets_setup(ui);
	g_object_unref(ui);
	
	/* Set-up the notification area */
	debug("Configuring notification area widget...");
	main_window_status_icon_create_menu();
	main_window_status_icon_create();
	
	/* Expand tweet area used to view & send tweets & dm.  */
	if(!gconfig_if_bool(PREFS_CONTROL_PANEL_DIALOG, FALSE))
		main_window->private->control_panel=control_panel_new(NULL);
	else
		main_window->private->control_panel=control_panel_new(main_window->private->window);
	
	/*control_panel stuff.*/
	main_window->private->control_panel_embed=control_panel_get_embed();
	main_window->private->control_panel_window=control_panel_get_window();
	
	if(!gconfig_if_bool(PREFS_CONTROL_PANEL_DIALOG, FALSE)){
		gtk_widget_reparent(GTK_WIDGET( main_window->private->control_panel_embed), GTK_WIDGET(main_window->private->control_panel_vbox));
		gtk_widget_show(GTK_WIDGET( main_window->private->control_panel_embed));
	}else
		gtk_widget_hide(GTK_WIDGET( main_window->private->expand_box));
	/* Set the main window geometry */
	geometry_load();
	
	/* Initial status of widgets */
	main_window_state_on_connection(FALSE);
	main_window_best_friends_buttons_set_sensitive();

	/* Ok, set the window state based on the gconf value */
	if(!gconfig_if_bool(MAIN_WINDOW_UI_HIDDEN, FALSE))
		gtk_widget_show(GTK_WIDGET( main_window->private->window));
	else
		gtk_widget_hide(GTK_WIDGET( main_window->private->window));
	
	main_window_login();
}/*main_window_setup();*/

static void main_window_best_friends_resized(GtkScrolledWindow *best_friends_scrolled_window, GtkAllocation *allocation, MainWindow *main_window){
	return;
	g_object_set(main_window->private->best_friends_user_name_cell_renderer_text, "wrap-width", ((gtk_tree_view_column_get_width(main_window->private->best_friends_user_name_tree_view_column))-10), NULL);
	g_object_set(main_window->private->best_friends_online_service_cell_renderer_text, "wrap-width", ((gtk_tree_view_column_get_width(main_window->private->best_friends_online_service_tree_view_column))-10), NULL);
}/*main_window_best_friends_resized(widget, allocation, main_window);*/

static void main_window_best_friends_list_store_validate(GtkButton *best_friends_refresh_button, MainWindow *main_window ){
	/* TODO: Add a confirmation dialog that warn of how long this may take. */
	online_services_best_friends_list_store_validate(main_window->private->best_friends_list_store);
}/*main_window_best_friends_list_store_validate(button, main_window);*/

static void main_window_best_friends_setup(GtkBuilder *ui){
	const gchar *best_friends_buttons[]={
		"best_friends_view_updates_new_button",
		"best_friends_view_profile_button",
		"best_friends_send_at_message_button",
		"best_friends_send_dm_button",
		"best_friends_unfollow_button",
		"best_friends_view_updates_button",
	};
	
	GList *list=NULL;
	for(int i=0; i < G_N_ELEMENTS(best_friends_buttons); i++)
		list=g_list_append(list, (gtk_builder_get_object(ui, best_friends_buttons[i])) );
	main_window->private->best_friends_buttons=list;
	
	main_window->private->best_friends_tree_model=gtk_tree_view_get_model( (GtkTreeView *)main_window->private->best_friends_sexy_tree_view );
	main_window->private->best_friends_tree_model_sort=gtk_tree_model_sort_new_with_model(main_window->private->best_friends_tree_model);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(main_window->private->best_friends_tree_model_sort), BestFriendOnlineServiceGUID, GTK_SORT_DESCENDING);
	
	online_services_best_friends_list_store_fill(main_window->private->best_friends_list_store);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(main_window->private->best_friends_sexy_tree_view), main_window->private->best_friends_tree_model_sort);
	sexy_tree_view_set_tooltip_label_column(main_window->private->best_friends_sexy_tree_view, BestFriendUserName);
}/*main_window_best_friends_setup(ui);*/

static void main_window_best_friends_buttons_set_sensitive(void){
	OnlineService *service=NULL;
	gchar *user=NULL;
	gchar *user_name=NULL;
	gboolean sensitive;
	if(!((main_window_best_friends_get_selected( &service, &user, &user_name)) && service && G_STR_N_EMPTY(user) && G_STR_N_EMPTY(user_name) )){
		sensitive=FALSE;
		debug("No best friend is selected.  Best Friend buttons will be disabled.");
	}else{
		sensitive=TRUE;
		debug("Selected best friend: %s, on <%s>.  Enabling best friend controls.", user, service->guid );
		uber_free(user);
		uber_free(user_name);
	}
	
	GList *buttons=NULL;
	for(buttons=main_window->private->best_friends_buttons; buttons; buttons=buttons->next)
		gtk_widget_set_sensitive( (GtkWidget *)buttons->data, sensitive );
	g_list_free(buttons);
	
	control_panel_sexy_select();
}/*main_window_best_friends_buttons_set_sensitive();*/

gboolean main_window_best_friends_get_selected(OnlineService **service, gchar **user, gchar **user_name ){
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	gboolean found=FALSE;
	OnlineService *selected_service=NULL;
	gchar *selected_user=NULL;
	gchar *selected_user_name=NULL;
	GtkTreeSelection *sel=gtk_tree_view_get_selection( (GtkTreeView *)main_window->private->best_friends_sexy_tree_view );
	if(gtk_tree_selection_get_selected(sel, &main_window->private->best_friends_tree_model_sort, iter))
		gtk_tree_model_get(
				main_window->private->best_friends_tree_model_sort, iter,
					BestFriendOnlineService, &selected_service,
					BestFriendUser, &selected_user,
					BestFriendUserName, &selected_user_name,
				-1
		);
	if(!( selected_service && selected_service->connected && G_STR_N_EMPTY(selected_user) && G_STR_N_EMPTY(selected_user_name) )){
		*service=NULL;
		*user=NULL;
		*user_name=NULL;
	}else{
		found=TRUE;
		if( g_str_has_prefix(selected_user_name, "<b>") )
			online_services_best_friends_list_store_mark_as_read(selected_service, selected_user, main_window->private->best_friends_list_store);
		*user=g_strdup(selected_user);
		*user_name=g_strdup(selected_user_name);
		*service=selected_service;
	}
	
	uber_free(iter);
	return found;
}/*main_window_best_friends_get_selected();*/

static void main_window_best_friends_tree_view_row_activated(GtkTreeView *best_friends_tree_view, MainWindow *main_window){
	OnlineService *service=NULL;
	gchar *user=NULL;
	gchar *user_name=NULL;
	if(!((main_window_best_friends_get_selected( &service, &user, &user_name)) && service && G_STR_N_EMPTY(user) && G_STR_N_EMPTY(user_name) )){
		debug("Cannot load best friends' updates.  Invalid OnlineService or empty user_name.");
		control_panel_sexy_select();
		return;
	}
	
	if(!g_str_has_prefix(user_name, "<b>"))
		online_service_request_view_updates(service, main_window->private->window, user);
	else
		online_service_request_view_updates_new(service, main_window->private->window, user);
	uber_free(user);
	uber_free(user_name);
	control_panel_sexy_select();
}/*main_window_best_friends_tree_view_row_activated(tree_view, path, column, main_window);*/

static void main_window_best_friends_button_clicked(GtkButton *button){
	OnlineService *service=NULL;
	gchar *user=NULL;
	gchar *user_name=NULL;
	if(!((main_window_best_friends_get_selected( &service, &user, &user_name)) && service && G_STR_N_EMPTY(user) && G_STR_N_EMPTY(user_name) )){
		if(button==main_window->private->best_friends_drop_button){
			online_service_request_popup_best_friend_drop();
			return;
		}
		debug("Cannot load best friends request.  Invalid OnlineService or empty user_name.");
		control_panel_sexy_select();
		return;
	}
	
	if(button==main_window->private->best_friends_view_updates_new_button){
		online_service_request_view_updates_new(service, main_window->private->window, user);
		control_panel_sexy_select();
		uber_free(user_name);
		uber_free(user);
		return;
	}
	
	if(button==main_window->private->best_friends_drop_button){
		online_service_best_friends_drop(service, main_window->private->window, user);
		control_panel_sexy_select();
		uber_free(user_name);
		uber_free(user);
		return;
	}
	
	if(button==main_window->private->best_friends_unfollow_button){
		online_service_request_unfollow(service, main_window->private->window, user);
		control_panel_sexy_select();
		uber_free(user_name);
		uber_free(user);
		return;
	}
	
	if(button==main_window->private->best_friends_view_profile_button){
		online_service_request_view_profile(service, main_window->private->window, user);
		control_panel_sexy_select();
		uber_free(user_name);
		uber_free(user);
		return;
	}
	
	if(button==main_window->private->best_friends_send_at_message_button){
		gchar *at_string=NULL;
		if(!( (gconfig_if_bool(PREFS_TWEETS_NO_PROFILE_LINK, TRUE)) && online_services_has_connected(1) ))
			at_string=g_strdup_printf("@%s (http://%s/%s) ", user, service->uri, user );
		else
			at_string=g_strdup_printf("@%s ", user);
		
		control_panel_sexy_prefix_string(at_string);
		control_panel_sexy_select();
		uber_free(at_string);
		uber_free(user_name);
		uber_free(user);
		return;
	}
	
	if(button==main_window->private->best_friends_send_dm_button){
		control_panel_best_friends_start_dm(service, user);
		control_panel_sexy_select();
		uber_free(user_name);
		uber_free(user);
		return;
	}
	
	if( button==main_window->private->best_friends_view_updates_button){
		online_service_request_view_updates(service, main_window->private->window, user);
		control_panel_sexy_select();
		uber_free(user_name);
		uber_free(user);
		return;
	}
	
}/*main_window_best_friends_button_clicked(button);*/

static void main_window_view_setup(void){
	/* Best Friends stuff. */
	main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key( main_window->private->view_best_friends_check_menu_item, MAIN_WINDOW_BEST_FRIENDS_HIDE_VBOX, (GtkWidget *)main_window->private->best_friends_vbox );
	
	/* Main Toolbar stuff. */
	main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key( main_window->private->view_toolbar_main_check_menu_item, MAIN_WINDOW_MAIN_TOOLBAR_HIDE, (GtkWidget *)main_window->private->main_window_handlebox );
	
	/* Tabs Toolbars stuff. */
	main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(main_window->private->view_toolbar_tabs_check_menu_item, UPDATE_VEWER_TOOLBAR_VISIBILITY, NULL);
	
	gtk_check_menu_item_set_active( main_window->private->view_control_panel_floating_check_menu_item, gconfig_if_bool(PREFS_CONTROL_PANEL_DIALOG, FALSE) );
	g_signal_connect_after( main_window->private->view_control_panel_floating_check_menu_item, "toggled", (GCallback)contol_panel_emulate_embed_toggle, NULL );
	
	gtk_check_menu_item_set_active(main_window->private->view_control_panel_compact_view_check_menu_item, gconfig_if_bool( PREFS_CONTROL_PANEL_COMPACT, TRUE) );
	g_signal_connect_after( main_window->private->view_control_panel_compact_view_check_menu_item, "toggled", (GCallback)contol_panel_emulate_compact_view_toggle, NULL );
}/*main_window_view_setup();*/

static void main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key(GtkCheckMenuItem *check_menu_item, const gchar *gconfig_key, GtkWidget *widget){
	gboolean hide=gconfig_if_bool(gconfig_key, FALSE);
	gtk_check_menu_item_set_active(check_menu_item, !hide);
	
	g_object_set_data_full( (GObject *)check_menu_item, "gconfig_key", g_strdup(gconfig_key), g_free );
	
	if(widget){
		g_object_set_data( (GObject *)check_menu_item, "widget", widget );
		if(hide) gtk_widget_hide(widget);
	}
	
	g_signal_connect_after( check_menu_item, "toggled", (GCallback)main_window_view_menu_option_toggled, NULL );
}/*main_window_bind_widget_visibility_to_check_menu_item_and_gconfig_key( main_window->private->view_best_friends_check_menu_item, MAIN_WINDOW_BEST_FRIENDS_HIDE_VBOX, (GtkWidget *)main_window->private->best_friends_vbox );*/

static void main_window_view_menu_option_toggled(GtkCheckMenuItem *check_menu_item){
	/* Any widget's visiblity state is stored as !visibility.  I am going to switch this.
	 * I'm only currently doing this because gconf's preferences currently indicate if something hidden.
	 * TODO: depricate this - mainly by updating how the MainWindow handles its 'auto-hide in system tray' state.
	 * e.g. /ui/hide/best_friends & /ui/hide/main_window.
 .	 * 	This is handled by MainWindow's methods: 'main_window_toggle_visibility' & 'main_window_timeout'.
	 */
	const gchar *gconfig_key=g_object_get_data( (GObject *)check_menu_item, "gconfig_key");
	gboolean hide=!gtk_check_menu_item_get_active(check_menu_item);
	gconfig_set_bool(gconfig_key, hide);
	GtkWidget *widget=NULL;
	if( (widget=g_object_get_data( (GObject *)check_menu_item, "widget")) ){
		if(hide)
			gtk_widget_hide(widget);
		else
			gtk_widget_show(widget);
	}
	if(check_menu_item==main_window->private->view_toolbar_tabs_check_menu_item) tweet_lists_toggle_toolbars();
}/*main_window_view_menu_option_toggled(check_menu_item);*/

void main_window_control_panel_set_embed(GtkToggleButton *toggle_button, gpointer user_data){
	gboolean use_control_panel_dialog=gtk_toggle_button_get_active(toggle_button);
	if(gconfig_if_bool(PREFS_CONTROL_PANEL_DIALOG, FALSE)==use_control_panel_dialog)
		return;
	
	debug("TweetView changed:\t[%s].",(use_control_panel_dialog?_( "floating"):_("embed")) );
	geometry_save();
	gconfig_set_bool(PREFS_CONTROL_PANEL_DIALOG, use_control_panel_dialog);
	
	if(use_control_panel_dialog){
		if(gtk_widget_get_parent( GTK_WIDGET( main_window->private->control_panel_embed))==GTK_WIDGET(main_window->private->control_panel))
			return;
		
		debug("Displaying TweetView as a stand alone dialog & setting TweetView's parent window..");
		gtk_widget_reparent(GTK_WIDGET( main_window->private->control_panel_embed), GTK_WIDGET(main_window->private->control_panel_window));
		window_present(GTK_WINDOW( main_window->private->control_panel_window), TRUE);
		g_object_add_weak_pointer(G_OBJECT( main_window->private->control_panel_window),(gpointer)&main_window->private->control_panel_window);
		gtk_window_set_transient_for(GTK_WINDOW( main_window->private->control_panel_window), main_window->private->window);
		gtk_widget_hide(GTK_WIDGET( main_window->private->expand_box));
	}else{
		if(gtk_widget_get_parent( GTK_WIDGET( main_window->private->control_panel_embed))==GTK_WIDGET(main_window->private->control_panel_vbox))
			return;
		
		debug("Embeding TweetView's into %s main window.", PACKAGE_NAME);
		gtk_widget_reparent(GTK_WIDGET( main_window->private->control_panel_embed), GTK_WIDGET(main_window->private->control_panel_vbox));
		gtk_widget_show(GTK_WIDGET( main_window->private->expand_box));
		gtk_widget_show(GTK_WIDGET( main_window->private->control_panel_vbox));
		gtk_widget_show(GTK_WIDGET( main_window->private->control_panel_embed));
		gtk_widget_hide(GTK_WIDGET( main_window->private->control_panel_window));
	}
	
	control_panel_set_embed_toggle_and_image();
	
	debug("Setting MainWindow's embed state indicator, in its 'View' menu, to %s window.", (use_control_panel_dialog ?"embed ControlPanel into its main" :"split ControlPanel off into a floating" ) );
	gtk_check_menu_item_set_active(main_window->private->view_control_panel_floating_check_menu_item, !use_control_panel_dialog);
	
	geometry_load();
}/*main_window_control_panel_embed*/

GtkWindow *main_window_get_window(void){
	return main_window->private->window;
}/*main_window_get_window*/

GtkPaned *main_window_get_tweet_list_paned(void){
	return GTK_PANED(main_window->private->tweet_list_hpaned);
}/*main_window_get_tweet_paned();*/

GtkPaned *main_window_get_tweet_paned(void){
	return GTK_PANED(main_window->private->tweet_vpaned);
}/*main_window_get_tweet_paned();*/

GtkMenuItem *main_window_get_menu(const gchar *menu){
	if((g_str_equal(menu, "network")) ) return GET_PRIVATE(main_window)->menu_network;
	if((g_str_equal(menu, "tabs")) ) return GET_PRIVATE(main_window)->menu_tabs;
	if((g_str_equal(menu, "edit")) ) return GET_PRIVATE(main_window)->menu_edit;
	if((g_str_equal(menu, "view")) ) return GET_PRIVATE(main_window)->menu_view;
	if((g_str_equal(menu, "online_service_request")) ) return GET_PRIVATE(main_window)->menu_online_service_request;
	if((g_str_equal(menu, "help")) ) return GET_PRIVATE(main_window)->menu_help;
	return NULL;
}/*main_window_get_menu*/

GtkListStore *main_window_get_best_friends_list_store(void){
	return main_window->private->best_friends_list_store;
}/*main_window_get_best_friends_list_store();*/

GtkTreeModel *main_window_get_best_friends_tree_model(void){
	return main_window->private->best_friends_tree_model_sort;
}/*main_window_get_best_friends_tree_model();*/

static void main_window_destroy_cb(GtkWidget *window, MainWindow *main_window){
	online_service_request_unset_selected_update();
	/* Add any clean-up code above this function call */
	online_services_best_friends_list_store_free(main_window->private->best_friends_list_store);
	gtk_widget_destroy(GTK_WIDGET( GET_PRIVATE( main_window)->window) );
}

static gboolean main_window_delete_event_cb(GtkWidget *window, GdkEvent *event, MainWindow *main_window){
	if(gtk_status_icon_is_embedded( main_window->private->status_icon)) {
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
	
	visible=window_get_is_visible(GTK_WINDOW( main_window->private->window));
	
	if(visible && gtk_status_icon_is_embedded(main_window->private->status_icon)) {
		geometry_save();
		
		gtk_widget_hide(GTK_WIDGET( main_window->private->window));
		
		program_timeout_remove(&main_window->private->size_timeout_id, _("main window configuration"));
	} else {
		geometry_load();
		window_present(GTK_WINDOW( main_window->private->window), TRUE);
		
		if(gconfig_if_bool(PREFS_CONTROL_PANEL_DIALOG, FALSE))
			window_present(GTK_WINDOW( main_window->private->control_panel_window), TRUE);
	}
	/* Save the window visibility state */
	gconfig_set_bool(MAIN_WINDOW_UI_HIDDEN, visible);
}

static void main_window_set_visibility(gboolean visible){
	gconfig_set_bool(MAIN_WINDOW_UI_HIDDEN, !visible);
	
	if(!visible)
		gtk_widget_hide(GTK_WIDGET( main_window->private->window));
	else{
		window_present(GTK_WINDOW( main_window->private->window), TRUE);
		if(gconfig_if_bool(PREFS_CONTROL_PANEL_DIALOG, FALSE))
			window_present(GTK_WINDOW( main_window->private->control_panel_window), TRUE);
	}
}

static void main_window_exit(GtkWidget  *widget, MainWindow  *main_window){
	gtk_main_quit();
}

static void main_window_tabs_menu_timeline_selected(GtkRadioMenuItem *item, MainWindow *main_window){
	gboolean timeline_found=FALSE;
	debug("Switching timelines. MenuItem selected: %s", gtk_menu_item_get_label(GTK_MENU_ITEM( item)) );
	
	if( main_window->private->timeline_public->active){
		tweet_lists_get_timeline(API_TIMELINE_PUBLIC, NULL);
		timeline_found=TRUE;
	}
	
	if( main_window->private->timeline_mine->active){
		tweet_lists_get_timeline(API_TIMELINE_MINE, NULL);
		timeline_found=TRUE;
	}
	
	if( main_window->private->timeline_favorites->active){
		tweet_lists_get_timeline(API_FAVORITES, NULL);
		timeline_found=TRUE;
	}
	
	if( main_window->private->timeline_dm->active){
		tweet_lists_get_timeline(API_DIRECT_MESSAGES, NULL);
		timeline_found=TRUE;
	}
	
	if( main_window->private->timeline_replies->active){
		tweet_lists_get_timeline(API_REPLIES, NULL);
		timeline_found=TRUE;
	}
	
	if( main_window->private->timeline_friends->active){
		tweet_lists_get_timeline(API_TIMELINE_FRIENDS, NULL);
		timeline_found=TRUE;
	}
	
	/* just in case, fall back to friends timeline */
	if(!timeline_found)
		gtk_check_menu_item_set_active(main_window->private->timeline_friends,TRUE);
}/*main_window_tabs_menu_timeline_selected(item, main_window);*/


static void online_service_request_menu_process(GtkImageMenuItem *item, MainWindow *main_window){
	if(item == main_window->private->online_service_request_menu_friends_manager)
		return friends_manager_show(GTK_WINDOW( main_window->private->window) );
	
	if(item == main_window->private->online_service_request_menu_following_viewer)
		return following_viewer_show(GTK_WINDOW( main_window->private->window) );
	
	if(item == main_window->private->online_service_request_menu_profile)
		return online_service_request_popup_friend_profile();
	
	if(item == main_window->private->online_service_request_menu_updates)
		return online_service_request_popup_friend_updates();
	
	if(item == main_window->private->online_service_request_menu_best_friend)
		return online_service_request_popup_best_friend_add();
	
	if(item == main_window->private->online_service_request_menu_follow)
		return online_service_request_popup_friend_follow();
	
	if(item == main_window->private->online_service_request_menu_unfollow)
		return online_service_request_popup_friend_unfollow();
	
	if(item == main_window->private->online_service_request_menu_block)
		return online_service_request_popup_friend_block();
}


static void main_window_services_cb(GtkWidget *widget, MainWindow *main_window){
	online_services_dialog_show(GTK_WINDOW( main_window->private->window));
}

static void main_window_select_service(GtkMenuItem *item, MainWindow *main_window){
	online_service_request_popup_select_service();
}/*main_window_select_service*/

static void main_window_preferences_cb(GtkWidget *widget, MainWindow *main_window){
	preferences_dialog_show(GTK_WINDOW( main_window->private->window));
}

static void main_window_about_cb(GtkWidget *widget, MainWindow *main_window){
	about_dialog_new(GTK_WINDOW( main_window->private->window));
}

static void main_window_help_contents_cb(GtkWidget *widget, MainWindow *main_window){
	help_show(GTK_WINDOW( main_window->private->window));
}

static void main_window_show_hide_cb(GtkWidget *widget, MainWindow *main_window){
	main_window_toggle_visibility();
}

static void main_window_status_icon_activate_cb(GtkStatusIcon *status_icon, MainWindow *main_window){
	main_window_toggle_visibility();
}

static void main_window_status_icon_popup_menu_cb(GtkStatusIcon *status_icon, guint button, guint activate_time, MainWindow *main_window){
	gboolean       show=window_get_is_visible(GTK_WINDOW( main_window->private->window));
	
	g_signal_handlers_block_by_func(main_window->private->popup_menu_show_main_window,
									 main_window_show_hide_cb, main_window);
									
	gtk_toggle_action_set_active(main_window->private->popup_menu_show_main_window, show);
	
	g_signal_handlers_unblock_by_func(main_window->private->popup_menu_show_main_window,
									   main_window_show_hide_cb, main_window);
									
	gtk_menu_popup(GTK_MENU( main_window->private->popup_menu),
					NULL, NULL,
					gtk_status_icon_position_menu,
					main_window->private->status_icon,
					button,
					activate_time);
}

static void main_window_status_icon_create_menu(void){
	GtkAction       *new_msg, *new_dm, *about, *quit;
	GtkWidget       *w;
	
	
	main_window->private->popup_menu_show_main_window=gtk_toggle_action_new("tray_show_app", _("_Show "), NULL, NULL);
	g_signal_connect(G_OBJECT( main_window->private->popup_menu_show_main_window), "toggled", G_CALLBACK(main_window_show_hide_cb), main_window);
					
	new_msg=gtk_action_new("tray_new_message", _("_New Tweet"), NULL, "gtk-new");
	g_signal_connect(G_OBJECT( new_msg), "activate", G_CALLBACK(tweets_new_tweet), main_window);
	
	new_dm=gtk_action_new("tray_new_dm", _("New _DM"), NULL, "gtk-jump-to");
	g_signal_connect(G_OBJECT( new_dm), "activate", G_CALLBACK(control_panel_new_dm), main_window);
	
	about=gtk_action_new("tray_about", _("_About"), NULL, "gtk-about");
	g_signal_connect(G_OBJECT( new_dm), "activate", G_CALLBACK(main_window_about_cb), main_window);
	
	quit=gtk_action_new("tray_quit", _("_Quit"), NULL, "gtk-quit");
	g_signal_connect(G_OBJECT( quit), "activate", G_CALLBACK(main_window_exit), main_window);
	
	main_window->private->popup_menu=gtk_menu_new();
	w=gtk_action_create_menu_item(GTK_ACTION( main_window->private->popup_menu_show_main_window));
	gtk_menu_shell_append(GTK_MENU_SHELL( main_window->private->popup_menu), w);
	w=gtk_separator_menu_item_new();
	gtk_widget_show(w);
	gtk_menu_shell_append(GTK_MENU_SHELL( main_window->private->popup_menu), w);
	w=gtk_action_create_menu_item(new_msg);
	gtk_menu_shell_append(GTK_MENU_SHELL( main_window->private->popup_menu), w);
	w=gtk_action_create_menu_item(quit);
	gtk_menu_shell_append(GTK_MENU_SHELL( main_window->private->popup_menu), w);
}

static void main_window_status_icon_create(void){
	main_window->private->status_icon=gtk_status_icon_new_from_icon_name("get2gnow");
	g_signal_connect(main_window->private->status_icon,
					  "activate",
					  G_CALLBACK(main_window_status_icon_activate_cb),
					  main_window);
					
	g_signal_connect(main_window->private->status_icon,
					  "popup_menu",
					  G_CALLBACK(main_window_status_icon_popup_menu_cb),
					  main_window);
					
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
	
	main_window->private->size_timeout_id=g_timeout_add(500,(GSourceFunc)configure_event_timeout_cb, widget);
	
	return FALSE;
}

static void main_window_login(void){
	debug("Logging into online services.");
	if(!online_services_login()){
		online_services_dialog_show(main_window->private->window);
		return;
	}
	
	main_window_tabs_init();
}/*main_window_login*/

static void main_window_reconnect(GtkMenuItem *item, MainWindow *main_window){
	if(!( online_services_relogin()))
		return;
	
	main_window_tabs_init();
}/*main_window_reconnect*/

void main_window_disconnect(void){
	online_services_disconnect();
}/*main_window_disconnect*/

/*
 * Function to set the default
 * timeline in the menu.
 */
static void main_window_tabs_menu_set_active(MainWindow *main_window, gchar *timeline){
	/* This shouldn't happen, but just in case */
	if(G_STR_EMPTY( timeline)) {
		debug("**ERROR:** Default timeline in not set.  Selecting 'timeline_friends' by default.");
		return gtk_check_menu_item_set_active( main_window->private->timeline_friends, TRUE);
	}
	
	gboolean timeline_found=FALSE;
	debug("Selecting default timeline's radio button: %s", timeline);
	
	if((g_str_equal(timeline, API_TIMELINE_FRIENDS)) ){
		gtk_check_menu_item_set_active( main_window->private->timeline_friends, TRUE);
		timeline_found=TRUE;
	}
	
	if((g_str_equal(timeline, API_REPLIES)) ){
		gtk_check_menu_item_set_active( main_window->private->timeline_replies, TRUE);
		timeline_found=TRUE;
	}
	
	if((g_str_equal(timeline, API_DIRECT_MESSAGES)) ){
		gtk_check_menu_item_set_active( main_window->private->timeline_dm, TRUE);
		timeline_found=TRUE;
	}
	
	if((g_str_equal(timeline, API_FAVORITES)) ){
		gtk_check_menu_item_set_active( main_window->private->timeline_favorites, TRUE);
		timeline_found=TRUE;
	}
	
	if((g_str_equal(timeline, API_TIMELINE_MINE)) ){
		gtk_check_menu_item_set_active( main_window->private->timeline_mine, TRUE);
		timeline_found=TRUE;
	}
	
	if((g_str_equal(timeline, API_TIMELINE_PUBLIC)) ){
		gtk_check_menu_item_set_active( main_window->private->timeline_public, TRUE);
		timeline_found=TRUE;
	}
	
	/* Let's fallback to friends timeline */
	if(!timeline_found)
		gtk_check_menu_item_set_active( main_window->private->timeline_friends, TRUE);
}

/* Function to retrieve the users default timeline */
void main_window_tabs_init(void){
	gboolean open_home_page=TRUE;
	gchar *timeline=NULL;
	gconfig_get_string(PREFS_TWEETS_HOME_TIMELINE, &timeline);
	if(G_STR_EMPTY( timeline)){
		if(timeline) uber_free(timeline);
		timeline=g_strdup(API_TIMELINE_FRIENDS);
	}
	debug("Retrived default timeline: %s.  Loading timeline tabs.", timeline);
	
	if(gconfig_if_bool(PREFS_AUTOLOAD_FOLLOWING, TRUE)){
		debug("Preparing auto-monitor for My Friends' Tweets.");
		main_window_tabs_menu_set_active(main_window, API_TIMELINE_FRIENDS);
		if(open_home_page && g_str_equal(timeline, API_TIMELINE_FRIENDS))
			open_home_page=FALSE;
	}
	
	if(gconfig_if_bool(PREFS_AUTOLOAD_REPLIES, TRUE)){
		debug("Preparing auto-monitor for Replies.");
		main_window_tabs_menu_set_active(main_window, API_REPLIES);
		if(open_home_page && g_str_equal(timeline, API_REPLIES))
			open_home_page=FALSE;
	}
	
	if(gconfig_if_bool(PREFS_AUTOLOAD_DMS, TRUE)){
		debug("Preparing auto-monitor for DMs.");
		main_window_tabs_menu_set_active(main_window, API_DIRECT_MESSAGES);
		if(open_home_page && g_str_equal(timeline, API_DIRECT_MESSAGES))
			open_home_page=FALSE;
	}
	
	if(open_home_page){
		debug("Retriving default timeline: %s", timeline);
		tweet_lists_get_timeline(timeline, NULL);
	}
	main_window_tabs_menu_set_active(main_window, timeline);
	uber_free(timeline);
}/*main_window_tabs_init();*/

static void main_window_selected_update_widgets_setup(GtkBuilder *ui){
	const gchar *selected_update_buttons[]={
		"selected_update",
		"selected_update_reply_image_menu_item",
		"selected_update_retweet_image_menu_item",
		"selected_update_save_fave_image_menu_item",
		"selected_update_author_view_profile_image_menu_item",
		"selected_update_author_view_updates_image_menu_item",
		"selected_update_author_best_friend_image_menu_item",
		"selected_update_author_follow_image_menu_item",
		"selected_update_author_unfollow_image_menu_item",
		"selected_update_author_block_image_menu_item",
	};
	
	GList *list=NULL;
	for(int i=0; i < G_N_ELEMENTS(selected_update_buttons); i++)
		list=g_list_append(list, (gtk_builder_get_object(ui, selected_update_buttons[i])) );
	main_window->private->selected_update_image_menu_items=list;
}/*main_window_selected_update_widgets_setup(ui);*/

static void main_window_connection_items_setup(GtkBuilder *ui){
	GList         *list;
	gint           i;
	
	const gchar   *widgets_connected[]={
		"services_disconnect",
		"compose",
		"tabs",
		"online_service_request",
		"expand_box",
	};
	
	const gchar   *widgets_disconnected[]={
		"services_connect",
	};
	
	for(i=0, list=NULL; i < G_N_ELEMENTS(widgets_connected); i++)
		list=g_list_prepend(list, (gtk_builder_get_object(ui, widgets_connected[i])) );
	main_window->private->widgets_connected=list;
	
	for(i=0, list=NULL; i < G_N_ELEMENTS(widgets_disconnected); i++)
		list=g_list_prepend(list, (gtk_builder_get_object(ui, widgets_disconnected[i])) );
	main_window->private->widgets_disconnected=list;
}/*main_window_connection_items_setup(ui);*/

void main_window_state_on_connection(gboolean connected){
	if(!(main_window->private && main_window->private->widgets_connected && main_window->private->widgets_disconnected))
		return;
	
	GList         *l;
	if(!connected){
		online_service_request_unset_selected_update();
		tweet_lists_stop();
	} else
		tweet_lists_refresh();
	
	for(l=main_window->private->widgets_connected; l; l=l->next)
		gtk_widget_set_sensitive(GTK_WIDGET( l->data), connected );
	g_list_free(l);
	
	for(l=main_window->private->widgets_disconnected; l; l=l->next)
		gtk_widget_set_sensitive(GTK_WIDGET( l->data), !connected );
	g_list_free(l);
	
	if(connected) control_panel_sexy_select();
}

void main_window_selected_update_image_menu_items_show(gboolean selected_update){
	if(!(main_window->private && main_window->private->selected_update_image_menu_items )) return;
	
	GList *selected_update_image_menu_items;
	for(selected_update_image_menu_items=main_window->private->selected_update_image_menu_items; selected_update_image_menu_items; selected_update_image_menu_items=selected_update_image_menu_items->next)
		gtk_widget_set_sensitive(GTK_WIDGET( selected_update_image_menu_items->data), selected_update);
	g_list_free(selected_update_image_menu_items);
	control_panel_sexy_select();
}/*main_window_selected_update_image_menu_items_show(TRUE|FALSE);*/

void main_window_statusbar_printf(const gchar *msg, ...){
	if(!(main_window->private && main_window->private->statusbar && GTK_IS_STATUSBAR( main_window->private->statusbar) ))
		return;
	
	gchar *message=NULL;
	va_list words_and_more;
	
	va_start(words_and_more, msg);
	message=g_strdup_vprintf(msg, words_and_more);
	va_end(words_and_more);
	
	main_window_set_statusbar_msg(message);
	g_free(message);
}/*main_window_statusbar_printf("__format", ...);*/

static guint statusbar_messages=0;
void main_window_set_statusbar_msg(gchar *message){
	
	if(!(main_window->private && main_window->private->statusbar && GTK_IS_STATUSBAR( main_window->private->statusbar) ))
		return;
	
	if(G_STR_N_EMPTY( message)){
		statusbar_messages++;
		g_timeout_add_seconds_full(G_PRIORITY_DEFAULT, statusbar_messages,(GSourceFunc)main_window_statusbar_display, g_strdup(message), g_free);
	}
	
	program_timeout_remove(&main_window->private->timeout_id_status_bar, _("status bar message"));
	main_window->private->timeout_id_status_bar=g_timeout_add_seconds_full(G_PRIORITY_DEFAULT,(statusbar_messages>=5?statusbar_messages:5),(GSourceFunc)main_window_statusbar_display, g_strdup(STATUSBAR_DEFAULT), g_free);
}/*main_window_set_statusbar_msg("Message...");*/

static gboolean main_window_statusbar_display(const gchar *message){
	gtk_statusbar_pop(GTK_STATUSBAR( main_window->private->statusbar), 1);
	gtk_statusbar_push(GTK_STATUSBAR( main_window->private->statusbar), 1,(G_STR_N_EMPTY( message) ?message :STATUSBAR_DEFAULT ) );
	
	if(statusbar_messages) statusbar_messages--;
	
	return FALSE;
}/*main_window_set_statusbar_display(gpointer);*/


gboolean main_window_notify_on_timeout(gpointer data){
	UserStatus *status=(UserStatus *)data;
	if(!(status && G_STR_N_EMPTY(status->notification) )){
		return FALSE;
	}
	
	NotifyNotification *notify_notification;
	GError             *error=NULL;
	
	if(!gtk_status_icon_is_embedded( main_window->private->status_icon))
		notify_notification=notify_notification_new(PACKAGE_TARNAME, status->notification, PACKAGE_TARNAME, NULL);
	else
		notify_notification=notify_notification_new_with_status_icon(PACKAGE_TARNAME, status->notification, PACKAGE_TARNAME, main_window->private->status_icon);
	
	notify_notification_set_timeout(notify_notification, 10000);
	
	if(gconfig_if_bool(PREFS_NOTIFY_BEEP, TRUE))
		tweets_beep();
	
	notify_notification_show(notify_notification, &error);
	
	g_object_unref(G_OBJECT( notify_notification));
	
	if(error){
		debug("Error displaying status->notification: %s.", error->message);
		g_error_free(error);
	}
	return FALSE;
}/*main_window_notify_on_timeout - only used as a callback to g_timer_add_seconds*/

const gchar *main_window_tabs_to_right_align(void){
	static int tab_count=0;
	gint w=0, h=0, test_tab_count=0;
	gtk_window_get_size(main_window->private->window, &w, &h);
	if(!((( w>0 && h>0)) && ((test_tab_count=w/100) >1) )) return main_window->private->tabs_to_right_align;
	if(tab_count==test_tab_count && G_STR_N_EMPTY(main_window->private->tabs_to_right_align) ) return main_window->private->tabs_to_right_align;
	
	tab_count=test_tab_count;
	gchar *tabs_to_right_align=NULL, *tabs_to_right_align_swap=NULL;
	if(main_window->private->tabs_to_right_align) uber_free(main_window->private->tabs_to_right_align);
	
	for(int i=0; i<=tab_count; i++){
		if(tabs_to_right_align){
			if(tabs_to_right_align_swap) uber_free(tabs_to_right_align_swap);
			tabs_to_right_align_swap=tabs_to_right_align;
		}
		tabs_to_right_align=g_strdup_printf("%s\t", (tabs_to_right_align_swap ?tabs_to_right_align_swap :"") );
	}
	
	if(tabs_to_right_align_swap) uber_free(tabs_to_right_align_swap);
	main_window->private->tabs_to_right_align=tabs_to_right_align;
	
	return main_window->private->tabs_to_right_align;
}/*main_window_tabs_to_right_align();*/

