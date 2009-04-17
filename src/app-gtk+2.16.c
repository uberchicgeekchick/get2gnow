/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors: Kaity G. B. <uberChick@uberChicGeekChick.Com>
 */

#include <config.h>
#include <sys/stat.h>
#include <string.h>

#include <canberra-gtk.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <libnotify/notify.h>
#include <gdk/gdkkeysyms.h>

#include "debug.h"
#include "gconf.h"
#include "gtkbuilder.h"
#ifdef HAVE_GNOME_KEYRING
#include "keyring.h"
#endif

#include "send-message-dialog.h"
#include "main.h"
#include "about.h"
#include "accounts-dialog.h"
#include "popup-dialog.h"
#include "app.h"
#include "geometry.h"
#include "hint.h"
#include "label.h"
#include "network.h"
#include "preferences.h"
#include "tweets.h"
#include "friends-manager.h"
#include "following-viewer.h"
#include "ui-utils.h"
#include "tweet-list.h"

#define GET_PRIV(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_APP, AppPriv ))

struct _AppPriv {
	/* Main widgets */
	GtkWindow		*window;
	
	GtkMenuBar		*menubar;
        GtkMenuItem		*menu_services;
        GtkMenuItem		*menu_tweets;
	GtkMenuItem		*menu_friends;
	GtkMenuItem		*menu_timelines;
	GtkMenuItem		*menu_help;
	
	TweetList		*listview;
	GtkStatusbar		*statusbar;
	
	/* Widgets that are enabled when we are connected/disconnected */
	GList			*widgets_connected;
	GList			*widgets_disconnected;
	
	/* Widgets that are enabled when we a tweet is selected */
	GList			*widgets_tweet_selected;
	
	/* Timeline menu items */
	GSList			*group;
	GtkRadioMenuItem	*timeline_combined;
	GtkRadioMenuItem	*timeline_public;
	GtkRadioMenuItem	*timeline_friends;
	GtkRadioMenuItem	*timeline_mine;
	GtkRadioMenuItem	*timeline_dm;
	GtkRadioMenuItem	*timeline_replies;
	GtkRadioMenuItem	*timeline_favorites;
	
	GtkAction		*friends_menu_friends_manager;
	GtkAction		*friends_menu_timelines;
	GtkAction		*friends_menu_follow;
	GtkAction		*friends_menu_unfollow;
	GtkAction		*friends_menu_block;
	GtkAction		*friends_menu_tweets;
	GtkAction		*friends_menu_profile;

	/* Status Icon */
	GtkStatusIcon		*status_icon;
	
	/* Status Icon Popup Menu */
	GtkWidget		*popup_menu;
	GtkToggleAction		*popup_menu_show_app;
	
	/* Account related data */
	char			*username;
	char			*password;
	
	/* Misc */
	guint			size_timeout_id;
	
	/* Expand messages widgets */
	GtkWidget		*expand_box;
	GtkWidget		*expand_image;
	GtkWidget		*expand_title;
	GtkWidget		*expand_label;
	
	/* Buttons for viewing details about the user of the curren selected/extended Twees. */
	GtkButton		*expanded_tweet_view_users_profile_button;
	GtkButton		*expanded_tweet_view_users_timeline_button;
	
	/* Buttons for viewing details about the user of the curren selected/extended Twees. */
	GtkButton		*expanded_tweet_user_follow_button;
	GtkButton		*expanded_tweet_user_unfollow_button;
	GtkButton		*expanded_tweet_user_block_button;

	/* Buttons for stuff to do with the selected & extend tweet. */
	GtkButton		*expanded_tweet_reply_button;
	GtkButton		*expanded_tweet_retweet_button;
	GtkButton		*expanded_tweet_dm_button;
	GtkButton		*expanded_tweet_make_fave_button;
};

#define	GtkBuilderUI		"main-window.ui"

#define	DEBUG_DOMAIN	"App"
#define	DEBUG_QUIT

#define	TYPE_TWITTER	"twitter"

static void app_class_init (AppClass *klass);
static void app_init (App *app);
static void app_finalize(GObject *object);
static void disconnect(App *app);
static void app_setup(void);
static void main_window_destroy_cb(GtkWidget *window, App *app); 
static gboolean main_window_delete_event_cb(GtkWidget *window, GdkEvent *event, App *app);
static void app_set_radio_group(App *app, GtkBuilder *ui); 
static void app_connect_cb(GtkWidget *window, App *app); 
static void app_disconnect_cb(GtkWidget *window, App *app); 
static void app_quit_cb(GtkWidget *window, App *app); 
static void app_accounts_cb(GtkWidget *window, App *app); 
static void app_preferences_cb(GtkWidget *window, App *app); 

/* Handles all timelines */
static void app_timeline_cb(GtkRadioMenuItem *item, App *app);


static void app_about_cb(GtkWidget *window, App *app); 
static void app_help_contents_cb(GtkWidget *widget, App *app); 
static void app_status_icon_activate_cb(GtkStatusIcon *status_icon, App *app); 
static void app_status_icon_popup_menu_cb(GtkStatusIcon *status_icon, guint button, guint activate_time, App *app);


static void friends_menu_request(GtkAction *item, App *app);


static void app_connection_items_setup(App *app, GtkBuilder *ui); 
static void app_login(App *app);
static void app_set_default_timeline(App *app, gchar *timeline);
static void app_retrieve_default_timeline(void);
static void app_status_icon_create_menu(void);
static void app_status_icon_create(void);
static void app_check_dir(void);
static void app_toggle_visibility(void);
static gboolean configure_event_timeout_cb(GtkWidget *widget);
static gboolean app_window_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, App *app);


static App  *app = NULL;
AppPriv *app_priv=NULL;

G_DEFINE_TYPE(App, app, G_TYPE_OBJECT);

static void app_class_init(AppClass *klass){
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = app_finalize;
	g_type_class_add_private(object_class, sizeof(AppPriv));
}



static void app_init(App *singleton_app){
	//AppPriv      *priv;	
	app = singleton_app;

	app_priv=GET_PRIV(app);
	app_priv->widgets_connected = NULL;
	app_priv->widgets_disconnected = NULL;
	app_priv->widgets_tweet_selected=NULL;
	app_priv->group = NULL;
	unset_selected_tweet();
}

static void app_finalize(GObject *object){
	App	       *app;
	//AppPriv      *priv=GET_PRIV(app);
	
	app = APP(object);
	app_priv = GET_PRIV(app);

	if(app_priv->size_timeout_id)
		g_source_remove(app_priv->size_timeout_id);

	g_list_free(app_priv->widgets_connected);
	g_list_free(app_priv->widgets_disconnected);
	g_slist_free(app_priv->group);

	conf_shutdown();
	
	G_OBJECT_CLASS(app_parent_class)->finalize(object);
}



static void disconnect(App *app){
	GtkListStore *store=tweet_list_get_store();
	gtk_list_store_clear(store);
	network_logout();
	app_state_on_connection(FALSE);
}

static void app_setup(void){
	Conf		*conf;
	GtkBuilder	*ui;
	GtkWidget	*scrolled_window;
	GtkWidget	*expand_vbox;
	gboolean	login;
	gboolean	hidden;

	debug(DEBUG_DOMAIN, "Beginning....");
	
	
	/* Set up interface */
	debug(DEBUG_DOMAIN, "Initialising interface");
	ui = gtkbuilder_get_file(
				GtkBuilderUI,
					"main_window", &app_priv->window,
					
					"main_menubar", &app_priv->menubar,
					"services", &app_priv->menu_services,
					"tweets", &app_priv->menu_tweets,
					"friends", &app_priv->menu_friends,
					"timelines", &app_priv->menu_timelines,
					"help", &app_priv->menu_help,

					"main_scrolledwindow", &scrolled_window,
					"main_statusbar", &app_priv->statusbar,
					
					"view_combined_timeline", &app_priv->timeline_combined,
					"view_public_timeline", &app_priv->timeline_public,
					"view_friends_timeline", &app_priv->timeline_friends,
					"view_my_timeline", &app_priv->timeline_mine,
					"view_direct_messages", &app_priv->timeline_dm,
					"view_direct_replies", &app_priv->timeline_replies,
					"view_favorites_timeline", &app_priv->timeline_favorites,
					
					"friends_menu_friends_manager", &app_priv->friends_menu_friends_manager,
					"friends_menu_timelines", &app_priv->friends_menu_timelines,
					"friends_menu_follow", &app_priv->friends_menu_follow,
					"friends_menu_unfollow", &app_priv->friends_menu_unfollow,
					"friends_menu_block", &app_priv->friends_menu_block,
					"friends_menu_tweets", &app_priv->friends_menu_tweets,
					"friends_menu_profile", &app_priv->friends_menu_profile,

					"expand_box", &app_priv->expand_box,
					"expand_vbox", &expand_vbox,
					"expand_image", &app_priv->expand_image,
					
					"expanded_tweet_dm_button", &app_priv->expanded_tweet_dm_button,
					"expanded_tweet_view_users_profile_button", &app_priv->expanded_tweet_view_users_timeline_button,
					"expanded_tweet_view_users_timeline_button", &app_priv->expanded_tweet_view_users_timeline_button,
					
					"expanded_tweet_user_follow_button", &app_priv->expanded_tweet_user_follow_button,
					"expanded_tweet_user_unfollow_button", &app_priv->expanded_tweet_user_unfollow_button,
					"expanded_tweet_user_block_button", &app_priv->expanded_tweet_user_block_button,
					
					"expanded_tweet_reply_button", &app_priv->expanded_tweet_reply_button, 
					"expanded_tweet_retweet_button", &app_priv->expanded_tweet_retweet_button,
					"expanded_tweet_make_fave_button", &app_priv->expanded_tweet_make_fave_button,
				NULL
	);
	g_signal_connect(app_priv->window, "key-press-event", G_CALLBACK(tweets_hotkey), NULL);

	/* Grab the conf object */
	conf = conf_get();

	/* Set group for menu radio actions */
	app_set_radio_group(app, ui);
	
	/* Connect the signals */
	gtkbuilder_connect( ui, app,
				"main_window", "destroy", main_window_destroy_cb,
				"main_window", "delete_event", main_window_delete_event_cb,
				"main_window", "configure_event", app_window_configure_event_cb,
				
				"twitter_connect", "activate", app_connect_cb,
				"twitter_disconnect", "activate", app_disconnect_cb,
				"accounts", "activate", app_accounts_cb,
				"preferences", "activate", app_preferences_cb,
				"quit", "activate", app_quit_cb,
				
				"tweets_new_tweet", "activate", tweets_new_tweet,
				"tweets_reply", "activate", tweets_reply,
				"tweets_retweet", "activate", tweets_retweet,
				"tweets_save_fave", "activate", tweets_save_fave,
				"tweets_new_dm", "activate", tweets_new_dm,
				
				"twitter_refresh", "activate", app_refresh_timeline,
				"view_combined_timeline", "group-changed", app_timeline_cb,
				"view_public_timeline", "group-changed", app_timeline_cb,
				"view_friends_timeline", "group-changed", app_timeline_cb,
				"view_my_timeline", "group-changed", app_timeline_cb,
				"view_direct_messages", "group-changed", app_timeline_cb,
				"view_direct_replies", "group-changed", app_timeline_cb,
				"view_favorites_timeline", "group-changed", app_timeline_cb,
				
				"friends_menu_friends_manager", "activate", friends_menu_request,
				"friends_menu_timelines", "activate", friends_menu_request,
				"friends_menu_follow", "activate", friends_menu_request,
				"friends_menu_unfollow", "activate", friends_menu_request,
				"friends_menu_block", "activate", friends_menu_request,
				"friends_menu_tweets", "activate", friends_menu_request,
				"friends_menu_profile", "activate", friends_menu_request,
				
				"help_contents", "activate", app_help_contents_cb,
				"help_about", "activate", app_about_cb,
				
				"expanded_tweet_dm_button", "clicked", tweets_new_dm,
				"expanded_tweet_view_users_profile_button", "clicked", tweets_user_view_profile,
				"expanded_tweet_view_users_timeline_button", "clicked", tweets_user_view_tweets,
				
				"expanded_tweet_user_follow_button", "clicked", tweets_user_follow,
				"expanded_tweet_user_unfollow_button", "clicked", tweets_user_unfollow,
				"expanded_tweet_user_block_button", "clicked", tweets_user_block,
				
				"expanded_tweet_reply_button", "clicked", tweets_reply,
				"expanded_tweet_retweet_button", "clicked", tweets_retweet,
				"expanded_tweet_make_fave_button", "clicked", tweets_save_fave,
			NULL
	);

	/* Set up connected related widgets */
	app_connection_items_setup(app, ui);
	g_object_unref(ui);

	/* Let's hide the main window, while we are setting up the ui */
	gtk_widget_hide(GTK_WIDGET(app_priv->window));

	/* Set-up the notification area */
	debug(DEBUG_DOMAIN,
			"Configuring notification area widget...");
	app_status_icon_create_menu();
	app_status_icon_create();
	
	/* Set the main window geometry */ 	 
	geometry_load_for_main_window(app_priv->window);

	/* Set-up list view */
	app_priv->listview=tweet_list_new();
	gtk_widget_show(GTK_WIDGET(app_priv->listview));
	gtk_container_add(GTK_CONTAINER(scrolled_window),
					   GTK_WIDGET(app_priv->listview));
	
	app_priv->expand_title=label_new();
	gtk_widget_show(GTK_WIDGET(app_priv->expand_title));
	gtk_box_pack_start(
				GTK_BOX(expand_vbox),
				GTK_WIDGET(app_priv->expand_title),
				TRUE, TRUE, 0
	);
	/* Set-up expand messages panel */
	app_priv->expand_label=label_new();
	gtk_widget_show(GTK_WIDGET(app_priv->expand_label));
	gtk_box_pack_end(GTK_BOX(expand_vbox),
					   GTK_WIDGET(app_priv->expand_label),
					   TRUE, TRUE, 0);
	//gtk_widget_hide(GTK_WIDGET(app_priv->expand_box));

	/* Initial status of widgets */
	app_state_on_connection(FALSE);

	/* Check get2gnow directory and images cache */
	app_check_dir();
	
	/* Get the gconf value for whether the window should be hidden on start-up */
	conf_get_bool(conf,
						  PREFS_UI_MAIN_WINDOW_HIDDEN,
						  &hidden);
	
	/* Ok, set the window state based on the gconf value */				  
	if(!hidden)
		gtk_widget_show(GTK_WIDGET(app_priv->window));
	else
		gtk_widget_hide(GTK_WIDGET(app_priv->window));

	/*Check to see if we should automatically login */
	conf_get_bool(conf, PREFS_AUTH_AUTO_LOGIN, &login);
	
	if(login) 
		app_login(app);
	
	unset_selected_tweet();
	tweets_show_submenu_entries((gboolean)FALSE);
}

static void main_window_destroy_cb(GtkWidget *window, App *app){
	unset_selected_tweet();
	/* Add any clean-up code above this function call */
	//gtk_main_quit();
	gtk_widget_destroy( GTK_WIDGET(app_get_window()) );
}

static gboolean main_window_delete_event_cb(GtkWidget *window, GdkEvent *event, App *app){
	if(gtk_status_icon_is_embedded(app_priv->status_icon)) {
		hint_show(PREFS_HINTS_CLOSE_MAIN_WINDOW,
						  _("get2gnow is still running, it is just hidden."),
						  _("Click on the notification area icon to show get2gnow."),
						   GTK_WINDOW(app_get_window()),
						   NULL, NULL);
		
		app_set_visibility(FALSE);
		return (gboolean)TRUE;
	}
	
	if((hint_dialog_show(
				PREFS_HINTS_CLOSE_MAIN_WINDOW,
				_("You were about to quit!"),
				_(
					"Since no system or notification tray has been "
					"found, this action would normally quit get2gnow.\n\n"
					"This is just a reminder, from now on, get2gnow will "
					"quit when performing this action unless you uncheck "
					"the option below."
				),
				GTK_WINDOW(app_get_window()),
				NULL, NULL)
	)) return (gboolean)TRUE;

	return (gboolean)FALSE;
}

static void app_set_radio_group(App  *app, GtkBuilder *ui){
	GtkRadioMenuItem *item;

	const gchar     *radio_actions[] = {
		"view_combined_timeline",
		"view_friends_timeline",
		"view_direct_replies",
		"view_direct_messages",
		"view_favorites_timeline",
		"view_my_timeline",
		"view_public_timeline",
	};

	
	for(int i = 0; i < G_N_ELEMENTS(radio_actions); i++) {
		item=GTK_RADIO_MENU_ITEM(gtk_builder_get_object(ui, radio_actions[i]));
		app_priv->group=gtk_radio_menu_item_get_group(item);
	}
}


static void app_toggle_visibility(void){
		gboolean       visible;

	
	visible = window_get_is_visible(GTK_WINDOW(app_priv->window));

	if(visible && gtk_status_icon_is_embedded(app_priv->status_icon)) {
		gint x, y, w, h;

		gtk_window_get_size(GTK_WINDOW(app_priv->window), &w, &h);
		gtk_window_get_position(GTK_WINDOW(app_priv->window), &x, &y);
		gtk_widget_hide(GTK_WIDGET(app_priv->window));

		geometry_save_for_main_window(x, y, w, h);

		if(app_priv->size_timeout_id) {
			g_source_remove(app_priv->size_timeout_id);
			app_priv->size_timeout_id = 0;
		}
	} else {
		geometry_load_for_main_window(app_priv->window);
		window_present(GTK_WINDOW(app_priv->window), TRUE);
	}
	/* Save the window visibility state */
	conf_set_bool(conf_get(),
						  PREFS_UI_MAIN_WINDOW_HIDDEN,
						  visible);
}

void
app_set_visibility(gboolean visible)
{
	GtkWindow *window=app_get_window();
	conf_set_bool(conf_get(), PREFS_UI_MAIN_WINDOW_HIDDEN, !visible);

	if(!visible)
		gtk_widget_hide(GTK_WIDGET(window));
	else
		window_present(GTK_WINDOW(window), TRUE);
}

static void app_connect_cb(GtkWidget *widget, App *app){
	app_login(app);
}

static void app_disconnect_cb(GtkWidget *widget, App *app){
	disconnect(app);
}


static void app_quit_cb(GtkWidget  *widget, App  *app){
	gtk_main_quit();
	//gtk_widget_destroy(app_priv->window);
}

void app_refresh_timeline(GtkWidget *window, App *app){
	tweets_show_submenu_entries((gboolean)FALSE);
	network_refresh();
}

static void app_timeline_cb(GtkRadioMenuItem *item, App *app){
	if(app_priv->timeline_combined==item)
		return network_get_combined_timeline();
	
	if(app_priv->timeline_public==item)
		return network_get_timeline(API_TIMELINE_PUBLIC);

	if(app_priv->timeline_mine==item)
		return network_get_user_timeline(NULL);

	if(app_priv->timeline_favorites==item)
		return network_get_timeline(API_FAVORITES);

	if(app_priv->timeline_dm==item)
		return network_get_timeline(API_DIRECT_MESSAGES);

	if(app_priv->timeline_replies==item) 
		return network_get_timeline(API_REPLIES);
	
	if(app_priv->timeline_friends==item) 
		return network_get_timeline(API_TIMELINE_FRIENDS);
	
	/* just in case, fall back to friends timeline */
	network_get_timeline(API_TIMELINE_FRIENDS);
}


static void friends_menu_request(GtkAction *action, App *app){
	if(action == app_priv->friends_menu_friends_manager)
		return friends_manager_show(GTK_WINDOW(app_priv->window));
	
	if(action == app_priv->friends_menu_timelines)
		return following_viewer_show(GTK_WINDOW(app_priv->window));
	
	if(action == app_priv->friends_menu_follow)
		return popup_friend_follow( GTK_WINDOW(app_priv->window) );
	
	if(action == app_priv->friends_menu_unfollow)
		return popup_friend_unfollow( GTK_WINDOW(app_priv->window) );
	
	if(action == app_priv->friends_menu_block)
		return popup_friend_block( GTK_WINDOW(app_priv->window) );
	
	if(action == app_priv->friends_menu_profile)
		return popup_friend_profile( GTK_WINDOW(app_priv->window) );
	
	if(action == app_priv->friends_menu_tweets)
		return popup_friend_tweets( GTK_WINDOW(app_priv->window) );
}


static void app_accounts_cb(GtkWidget *widget, App *app){
	accounts_dialog_show(GTK_WINDOW(app_priv->window));
}

static void app_preferences_cb(GtkWidget *widget, App *app){
	preferences_dialog_show(GTK_WINDOW(app_priv->window));
}

static void app_about_cb(GtkWidget *widget, App *app){
	about_dialog_new(GTK_WINDOW(app_priv->window));
}

static void app_help_contents_cb(GtkWidget *widget, App *app){
	help_show(GTK_WINDOW(app_priv->window));
}

static void app_show_hide_cb( GtkWidget *widget, App *app ){
	app_toggle_visibility();
}

static void app_status_icon_activate_cb(GtkStatusIcon *status_icon, App *app){
	app_toggle_visibility();
}

static void
app_status_icon_popup_menu_cb(GtkStatusIcon *status_icon,
							   guint          button,
							   guint          activate_time,
							   App     *app)
{
		gboolean       show;

	
	show = window_get_is_visible(GTK_WINDOW(app_priv->window));

	g_signal_handlers_block_by_func(app_priv->popup_menu_show_app,
									 app_show_hide_cb, app);

	gtk_toggle_action_set_active(app_priv->popup_menu_show_app, show);

	g_signal_handlers_unblock_by_func(app_priv->popup_menu_show_app,
									   app_show_hide_cb, app);

	gtk_menu_popup(GTK_MENU(app_priv->popup_menu),
					NULL, NULL,
					gtk_status_icon_position_menu,
					app_priv->status_icon,
					button,
					activate_time);
}

static void
app_status_icon_create_menu(void)
{
	GtkAction       *new_msg, *new_dm, *about, *quit;
	GtkWidget       *w;

	
	app_priv->popup_menu_show_app = gtk_toggle_action_new("tray_show_app",
													   _("_Show "),
													   NULL,
													   NULL);
	g_signal_connect(G_OBJECT(app_priv->popup_menu_show_app),
					  "toggled", G_CALLBACK(app_show_hide_cb),
					  app);

	new_msg = gtk_action_new("tray_new_message", _("_New Tweet"), NULL, "gtk-new");
	g_signal_connect(G_OBJECT(new_msg), "activate", G_CALLBACK(tweets_new_tweet), app);
	
	new_dm = gtk_action_new("tray_new_dm", _("New _DM"), NULL, "gtk-leave-fullscreen");
	g_signal_connect(G_OBJECT(new_dm), "activate", G_CALLBACK(tweets_new_dm), app);
	
	about = gtk_action_new("tray_about", _("_About"), NULL, "gtk-about");
	g_signal_connect(G_OBJECT(new_dm), "activate", G_CALLBACK(app_about_cb), app);
	
	quit = gtk_action_new("tray_quit", _("_Quit"), NULL, "gtk-quit");
	g_signal_connect(G_OBJECT(quit), "activate", G_CALLBACK(app_quit_cb), app);

	app_priv->popup_menu = gtk_menu_new();
	w = gtk_action_create_menu_item(GTK_ACTION(app_priv->popup_menu_show_app));
	gtk_menu_shell_append(GTK_MENU_SHELL(app_priv->popup_menu), w);
	w = gtk_separator_menu_item_new();
	gtk_widget_show(w);
	gtk_menu_shell_append(GTK_MENU_SHELL(app_priv->popup_menu), w);
	w = gtk_action_create_menu_item(new_msg);
	gtk_menu_shell_append(GTK_MENU_SHELL(app_priv->popup_menu), w);
	w = gtk_action_create_menu_item(quit);
	gtk_menu_shell_append(GTK_MENU_SHELL(app_priv->popup_menu), w);
}

static void
app_status_icon_create(void)
{
	
	
	app_priv->status_icon = gtk_status_icon_new_from_icon_name("get2gnow");
	g_signal_connect(app_priv->status_icon,
					  "activate",
					  G_CALLBACK(app_status_icon_activate_cb),
					  app);

	g_signal_connect(app_priv->status_icon,
					  "popup_menu",
					  G_CALLBACK(app_status_icon_popup_menu_cb),
					  app);

	gtk_status_icon_set_visible(app_priv->status_icon, TRUE);
}

void
app_create(void)
{
	g_object_new(TYPE_APP, NULL);
	app_setup();
}

App *app_get(void){
	g_assert(app != NULL);
	return app;
}
 
static gboolean configure_event_timeout_cb(GtkWidget *widget){
	gint           x, y, w, h;

	
	gtk_window_get_size(GTK_WINDOW(widget), &w, &h);
	gtk_window_get_position(GTK_WINDOW(widget), &x, &y);

	geometry_save_for_main_window(x, y, w, h);

	app_priv->size_timeout_id = 0;

	
	return FALSE;
}

static gboolean
app_window_configure_event_cb(GtkWidget         *widget,
							   GdkEventConfigure *event,
							   App         *app)
{
	
	
	if(app_priv->size_timeout_id)
		g_source_remove(app_priv->size_timeout_id);

	app_priv->size_timeout_id = g_timeout_add(500,
					(GSourceFunc) configure_event_timeout_cb,
					   widget);

return FALSE;
}

static void
request_username_password(App *a)
{
		Conf    *conf;

	
	conf = conf_get();

	g_free(app_priv->username);
	app_priv->username = NULL;
	conf_get_string(conf,
				PREFS_AUTH_USER_ID,
				&app_priv->username);
	g_free(app_priv->password);
#ifdef HAVE_GNOME_KEYRING
	app_priv->password = NULL;
	if(G_STR_EMPTY(app_priv->username))
		app_priv->password = NULL;
	else if(!(keyring_get_password(app_priv->username, &app_priv->password)))
		app_priv->password = NULL;
#else
	conf_get_string(conf, PREFS_AUTH_PASSWORD, &app_priv->password);
#endif
}

static void app_login(App *a){
	request_username_password(a);

	if(G_STR_EMPTY(app_priv->username) || G_STR_EMPTY(app_priv->password))
		return app_accounts_cb(NULL, a);
	
	network_login(app_priv->username, app_priv->password);
	app_retrieve_default_timeline();
}//app_login

/*
 * Function to set the default
 * timeline in the menu.
 */
static void app_set_default_timeline(App *app, gchar *timeline){
	/* This shouldn't happen, but just in case */
	if(G_STR_EMPTY(timeline)) {
		g_warning("Default timeline in not set");
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_friends), TRUE);
	}
	
	debug(DEBUG_DOMAIN, "Selecting default timeline's radio button: %s", timeline);
	/* TODO: Make this actually work as in like change the radio button.
	 * NOTE: I think I'm going to have to convert these into a GtkRadioMenuItem
	 * */
	
	if( (g_str_equal(timeline, "combined")) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_combined), TRUE);
		
	if( (g_str_equal(timeline, API_TIMELINE_FRIENDS)) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_friends), TRUE);
	
	if( (g_str_equal(timeline, API_REPLIES)) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_replies), TRUE);
	
	if( (g_str_equal(timeline, API_DIRECT_MESSAGES)) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_dm), TRUE);
	
	if( (g_str_equal(timeline, API_FAVORITES)) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_favorites), TRUE);
	
	if( (g_str_equal(timeline, API_TIMELINE_MY)) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_mine), TRUE);
	
	if( (g_str_equal(timeline, API_TIMELINE_PUBLIC)) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM( app_priv->timeline_public), TRUE);
	
	/* Let's fallback to friends timeline */
	return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_friends), TRUE);
}

/* Function to retrieve the users default timeline */
static void app_retrieve_default_timeline(void){
	gchar         *timeline=NULL;

	conf_get_string(conf_get(), PREFS_TWEETS_HOME_TIMELINE, &timeline);

	if(G_STR_EMPTY(timeline))
		timeline=g_strdup(API_TIMELINE_FRIENDS);
	
	app_set_default_timeline(app, timeline);
	debug(DEBUG_DOMAIN, "Retriving default timeline: %s", timeline);
	network_get_timeline(timeline);
	g_free(timeline);
}

static void
app_check_dir(void)
{
	gchar    *file;

	file = g_build_filename(g_get_home_dir(), ".gnome2", CACHE_IMAGES, NULL);

	if(!g_file_test(file, G_FILE_TEST_EXISTS|G_FILE_TEST_IS_DIR)) {
		debug(DEBUG_DOMAIN, "Making directory: %s", file);
		g_mkdir_with_parents(file, S_IRUSR|S_IWUSR|S_IXUSR);
	}

	g_free(file);
}


GList *app_get_widgets_tweet_selected(void){
	return app_priv->widgets_tweet_selected;
}//get_widgets_tweet_selected


static void app_connection_items_setup(App *app, GtkBuilder *ui){
	GList         *list;
	gint           i;

	const gchar   *widgets_connected[] = {
		"twitter_disconnect",
		"tweets",
		"timelines",
		"friends",
	};

	const gchar   *widgets_disconnected[] = {
		"twitter_connect"
	};

	const gchar *widgets_tweet_selected[]={
		"tweets_save_fave",
		"tweets_reply",
		"tweets_retweet"
	};
	
	for(i = 0, list = NULL; i < G_N_ELEMENTS(widgets_tweet_selected); i++)
		list=g_list_prepend(list, (gtk_builder_get_object(ui, widgets_tweet_selected[i])) );
	app_priv->widgets_tweet_selected=list;
			
	for(i = 0, list = NULL; i < G_N_ELEMENTS(widgets_connected); i++)
		list=g_list_prepend(list, (gtk_builder_get_object(ui, widgets_connected[i])) );
	app_priv->widgets_connected=list;
	
	for(i = 0, list = NULL; i < G_N_ELEMENTS(widgets_disconnected); i++)
		list=g_list_prepend(list, (gtk_builder_get_object(ui, widgets_disconnected[i])) );
	app_priv->widgets_disconnected=list;
}

void
app_state_on_connection(gboolean connected)
{
	GList         *l;
	
		
	for(l = app_priv->widgets_connected; l; l = l->next)
		g_object_set(l->data, "sensitive", connected, NULL);
	
	for(l = app_priv->widgets_disconnected; l; l = l->next)
		g_object_set(l->data, "sensitive", !connected, NULL);
	
	g_list_free(l);
}

GtkWindow *app_get_window(void){
	return app_priv->window;
}//app_get_window

GtkMenuBar *app_get_main_menu(void){
	return app_priv->menubar;
}//app_get_main_menu

GtkMenuItem *app_get_menu(const gchar *menu){
	if( (g_str_equal(menu, "services")) ) return GET_PRIV(app)->menu_services;
	if( (g_str_equal(menu, "tweets")) ) return GET_PRIV(app)->menu_tweets;
	if( (g_str_equal(menu, "friends")) ) return GET_PRIV(app)->menu_friends;
	if( (g_str_equal(menu, "timelines")) ) return GET_PRIV(app)->menu_timelines;
	if( (g_str_equal(menu, "help")) ) return GET_PRIV(app)->menu_help;
	//if( (g_str_equal(menu, "")) ) return GET_PRIV(app)->menu_;
	return NULL;
}//app_get_menu


TweetList *app_get_tweet_list(void){
	return GET_PRIV(app)->listview;
}//app_get_tweet_list


void app_statusbar_printf(const gchar *msg, ...){
	gchar *message=NULL;
	va_list words_and_more;
	
	va_start(words_and_more, msg);
	message=g_strdup_vprintf(msg, words_and_more);
	va_end(words_and_more);
	
	app_set_statusbar_msg(message);
	g_free(message);
}//app_update_status_bar


void app_set_statusbar_msg(gchar *message){
	/* Avoid some warnings */
	if(!app_priv->statusbar || !GTK_IS_STATUSBAR(app_priv->statusbar))
		return;
	
	/* starts displaying the status bar message for 5 seconds before returning to the system default */
	gtk_statusbar_pop(GTK_STATUSBAR(app_priv->statusbar), 1);
	gtk_statusbar_push(GTK_STATUSBAR(app_priv->statusbar), 1, message );

	return;
	/* TODO:
	 * I want this to display the custom message for a short time
	 * Then return to the systeme default */

	/* conext ID will be always 1 */
	gtk_statusbar_pop(GTK_STATUSBAR(app_priv->statusbar), 1);
	/* returns the status bar message to the default message */
	gtk_statusbar_push(GTK_STATUSBAR(app_priv->statusbar), 1, TWEETS_RETURN_MODIFIERS_STATUSBAR_MSG);
}


void
app_notify_sound(void)
{
	gboolean sound;

	conf_get_bool(conf_get(),
						  PREFS_UI_SOUND,
						  &sound);

	if(!sound) return;
	ca_context_play(
				(ca_gtk_context_get()),
				0,
				CA_PROP_APPLICATION_NAME, g_get_application_name(),
				CA_PROP_EVENT_ID, "message-new-instant",
				CA_PROP_EVENT_DESCRIPTION, _("New tweet received"),
				NULL
	);
}

void
app_notify(gchar *msg)
{
	gboolean notify;

	conf_get_bool(conf_get(),
						  PREFS_UI_NOTIFICATION,
						  &notify);

	if(!notify)
		return;
	NotifyNotification *notification;
	GError             *error = NULL;

	notification=notify_notification_new(PACKAGE_NAME, msg, PACKAGE_TARNAME, NULL);

	notify_notification_set_timeout(notification, 8 * 1000);
	notify_notification_show(notification, &error);
	g_object_unref(G_OBJECT(notification));

	if(!error)
		return;
	
	debug(DEBUG_DOMAIN, "Error displaying notification: %s", error->message);
	g_error_free(error);
}

void app_set_image(const gchar *image_filename, GtkTreeIter  iter){
	GdkPixbuf *pixbuf;
	if( !(pixbuf=images_get_pixbuf_from_filename( image_filename )) )
		return;

	gtk_list_store_set((tweet_list_get_store()) , &iter, PIXBUF_AVATAR, pixbuf, -1);
	g_object_unref(pixbuf);
}//app_set_image

void app_expand_message(const gchar *user_name, const gchar *user_nick, const gchar *date, const gchar *tweet, GdkPixbuf *pixbuf){
	gchar *title_text=g_strdup_printf("<b>%s ( @%s )</b> - %s", user_nick, user_name, date);
	label_set_text(LABEL(app_priv->expand_label), tweet);
	label_set_text(LABEL(app_priv->expand_title), title_text);
	g_free(title_text);
	
	if(pixbuf) {
		GdkPixbuf *resized=NULL;
		resized=images_expand_pixbuf( pixbuf );
		gtk_image_set_from_pixbuf(GTK_IMAGE(app_priv->expand_image), resized);
		if(resized)
			g_object_unref(resized);
	}
	
	gtk_widget_show(app_priv->expand_box);
}
