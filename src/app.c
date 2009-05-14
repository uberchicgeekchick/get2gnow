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

#include <config.h>
#include <sys/stat.h>
#include <string.h>

#include <canberra-gtk.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <libnotify/notify.h>
#include <gdk/gdkkeysyms.h>
#include <libsexy/sexy.h>

#include "debug.h"
#include "gconfig.h"
#include "gtkbuilder.h"
#ifdef HAVE_GNOME_KEYRING
#include "keyring.h"
#endif

#include "main.h"
#include "about.h"
#include "services-dialog.h"
#include "popup-dialog.h"
#include "app.h"
#include "images.h"
#include "preferences.h"
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
#include "tweet-view.h"

#define	GET_PRIV(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_APP, AppPriv ))

struct _AppPriv {
	/* Main widgets */
	GtkWindow		*window;
	
	GtkMenuBar		*menubar;
        GtkMenuItem		*menu_accounts;
        GtkMenuItem		*menu_tweets;
	GtkMenuItem		*menu_friends;
	GtkMenuItem		*menu_timelines;
	GtkMenuItem		*menu_help;
	
	TweetList		*listview;
	GtkStatusbar		*statusbar;
	
	/* Widgets that are enabled when we are connected/disconnected */
	GList			*widgets_connected;
	GList			*widgets_disconnected;
	
	/* Timeline menu items */
	GSList			*group;
	GtkRadioMenuItem	*timeline_public;
	GtkRadioMenuItem	*timeline_friends;
	GtkRadioMenuItem	*timeline_mine;
	GtkRadioMenuItem	*timeline_dm;
	GtkRadioMenuItem	*timeline_mentions;
	GtkRadioMenuItem	*timeline_favorites;
	GtkRadioMenuItem	*timeline_replies;
	
	GtkAction		*friends_menu_friends_manager;
	GtkAction		*friends_menu_timelines;
	GtkAction		*friends_menu_follow;
	GtkAction		*friends_menu_unfollow;
	GtkAction		*friends_menu_block;
	GtkAction		*friends_menu_tweets;
	GtkAction		*friends_menu_profile;
	
	/* GtkTreeView */
	GtkTreeView		*accounts_treeview;
	GtkListStore		*accounts_liststore;
	
	/* user, status, & update widgets.
	 * Actually they're in the TweetView.
	 * The main-window's GtkVBox contains them.
	 */
	GtkHBox			*expand_box;
	GtkVBox			*tweet_view_vbox;
	TweetView		*tweet_view;
	
	/* Status Icon */
	GtkStatusIcon		*status_icon;
	
	/* Status Icon Popup Menu */
	GtkWidget		*popup_menu;
	GtkToggleAction		*popup_menu_show_app;
	
	/* Misc */
	guint			size_timeout_id;
};


#define	DEBUG_DOMAINS	"App:UI:Tweets"
#define	GtkBuilderUI	"main-window.ui"


static void app_class_init(AppClass *klass);
static void app_init(App *app);
static void app_finalize(GObject *object);
static void app_disconnect(void);
static void app_setup(void);
static void main_window_destroy_cb(GtkWidget *window, App *app); 
static gboolean main_window_delete_event_cb(GtkWidget *window, GdkEvent *event, App *app);
static void app_set_radio_group(App *app, GtkBuilder *ui); 
static void app_quit_cb(GtkWidget *window, App *app); 
static void app_services_cb(GtkWidget *window, App *app); 
static void app_select_service(GtkMenuItem *item, App *app);
static void app_preferences_cb(GtkWidget *window, App *app); 

/* Handles all timelines */
static void app_timeline_cb(GtkRadioMenuItem *item, App *app);
static void app_accounts_treeview_fill(void);


static void app_about_cb(GtkWidget *window, App *app); 
static void app_help_contents_cb(GtkWidget *widget, App *app); 
static void app_status_icon_activate_cb(GtkStatusIcon *status_icon, App *app); 
static void app_status_icon_popup_menu_cb(GtkStatusIcon *status_icon, guint button, guint activate_time, App *app);


static void friends_menu_request(GtkAction *item, App *app);


static void app_connection_items_setup(GtkBuilder *ui); 
static gboolean app_connect(void);
static void app_set_default_timeline(App *app, gchar *timeline);
static void app_retrieve_default_timeline(void);
static void app_status_icon_create_menu(void);
static void app_status_icon_create(void);
static void app_check_dir(void);
static void app_toggle_visibility(void);
static gboolean configure_event_timeout_cb(GtkWidget *widget);
static gboolean app_window_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, App *app);


static App  *app=NULL;
static AppPriv *app_priv=NULL;

G_DEFINE_TYPE(App, app, G_TYPE_OBJECT);

static void app_class_init(AppClass *klass){
	GObjectClass *object_class=G_OBJECT_CLASS(klass);
	object_class->finalize=app_finalize;
	g_type_class_add_private(object_class, sizeof(AppPriv));
}



static void app_init(App *singleton_app){
	app=singleton_app;
	
	app_priv=GET_PRIV(app);
	app_priv->widgets_connected=NULL;
	app_priv->widgets_disconnected=NULL;
	app_priv->group=NULL;
	unset_selected_tweet();
}

static void app_finalize(GObject *object){
	App	       *app;
	
	app=APP(object);
	app_priv=GET_PRIV(app);
	
	if(app_priv->size_timeout_id)
		g_source_remove(app_priv->size_timeout_id);
		
	g_list_free(app_priv->widgets_connected);
	g_list_free(app_priv->widgets_disconnected);
	g_slist_free(app_priv->group);
	
	G_OBJECT_CLASS(app_parent_class)->finalize(object);
}



static void app_disconnect(void){
	GtkListStore *store=tweet_list_get_store();
	gtk_list_store_clear(store);
	network_logout();
	app_state_on_connection(FALSE);
}

static void app_setup(void){
	GtkBuilder	*ui;
	GtkWidget	*scrolled_window;
	
	debug(DEBUG_DOMAINS, "Starting %s...", PACKAGE_NAME);
	
	
	/* Set up interface */
	debug(DEBUG_DOMAINS, "Initialising interface");
	ui=gtkbuilder_get_file(
				GtkBuilderUI,
					"main_window", &app_priv->window,
					
					"main_menubar", &app_priv->menubar,
					"accounts", &app_priv->menu_accounts,
					"tweets", &app_priv->menu_tweets,
					
					"timelines", &app_priv->menu_timelines,
					"view_public_timeline", &app_priv->timeline_public,
					"view_friends_timeline", &app_priv->timeline_friends,
					"view_my_timeline", &app_priv->timeline_mine,
					"view_direct_messages", &app_priv->timeline_dm,
					"view_mentions", &app_priv->timeline_mentions,
					"view_replies", &app_priv->timeline_replies,
					"view_favorites_timeline", &app_priv->timeline_favorites,
					
					"friends", &app_priv->menu_friends,
					"friends_menu_friends_manager", &app_priv->friends_menu_friends_manager,
					"friends_menu_timelines", &app_priv->friends_menu_timelines,
					"friends_menu_follow", &app_priv->friends_menu_follow,
					"friends_menu_unfollow", &app_priv->friends_menu_unfollow,
					"friends_menu_block", &app_priv->friends_menu_block,
					"friends_menu_tweets", &app_priv->friends_menu_tweets,
					"friends_menu_profile", &app_priv->friends_menu_profile,
					
					"help", &app_priv->menu_help,
					/* end menu items. */
					
					"accounts_treeview", &app_priv->accounts_treeview,
					"accounts_liststore", &app_priv->accounts_treeview,
					
					"main_scrolledwindow", &scrolled_window,
					
					"expand_box", &app_priv->expand_box,
					"tweet_view_vbox", &app_priv->tweet_view_vbox,
					
					"main_statusbar", &app_priv->statusbar,
				NULL
	);
	g_signal_connect_after(app_priv->window, "key-press-event", G_CALLBACK(tweets_hotkey), NULL);
	
	/* Set group for menu radio actions */
	app_set_radio_group(app, ui);
	
	/* Connect the signals */
	gtkbuilder_connect( ui, app,
					"main_window", "destroy", main_window_destroy_cb,
					"main_window", "delete_event", main_window_delete_event_cb,
					"main_window", "configure_event", app_window_configure_event_cb,
					
					"services_connect", "activate", app_connect,
					"services_disconnect", "activate", app_disconnect,
					"services", "activate", app_services_cb,
					"select_service", "activate", app_select_service,
					"preferences", "activate", app_preferences_cb,
					"quit", "activate", app_quit_cb,
					
					"tweets_new_tweet", "activate", tweets_new_tweet,
				 	"tweets_reply", "activate", tweets_reply,
					"tweets_retweet", "activate", tweets_retweet,
					"tweets_save_fave", "activate", tweets_save_fave,
					"tweets_new_dm", "activate", tweet_view_new_dm,
					
					"timeline_refresh", "activate", app_refresh_timeline,
					"view_public_timeline", "activate", app_timeline_cb,
					"view_friends_timeline", "activate", app_timeline_cb,
					"view_my_timeline", "activate", app_timeline_cb,
					"view_direct_messages", "activate", app_timeline_cb,
					"view_mentions", "activate", app_timeline_cb,
					"view_replies", "activate", app_timeline_cb,
					"view_favorites_timeline", "activate", app_timeline_cb,
					
					"friends_menu_friends_manager", "activate", friends_menu_request,
					"friends_menu_timelines", "activate", friends_menu_request,
					"friends_menu_follow", "activate", friends_menu_request,
					"friends_menu_unfollow", "activate", friends_menu_request,
					"friends_menu_block", "activate", friends_menu_request,
					"friends_menu_tweets", "activate", friends_menu_request,
					"friends_menu_profile", "activate", friends_menu_request,
					
					"help_contents", "activate", app_help_contents_cb,
					"help_about", "activate", app_about_cb,
				NULL
	);
	
	/* Set up connected related widgets */
	app_connection_items_setup(ui);
	g_object_unref(ui);
	
	/* Set-up the notification area */
	debug(DEBUG_DOMAINS, "Configuring notification area widget...");
	app_status_icon_create_menu();
	app_status_icon_create();
	
	/* Set-up list view */
	app_priv->listview=tweet_list_new();
	gtk_widget_show(GTK_WIDGET(app_priv->listview));
	gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(app_priv->listview));
	
	/* Expand tweet area used to view & send tweets & dm.  */
	if(!gconfig_if_bool(PREFS_UI_TWEET_VIEW_USE_DIALOG)){
		app_priv->tweet_view=tweet_view_new(NULL);
		gtk_widget_reparent(GTK_WIDGET(app_priv->tweet_view->tweet_view_embed), GTK_WIDGET(app_priv->tweet_view_vbox));
		gtk_widget_show(GTK_WIDGET(app_priv->tweet_view->tweet_view_embed));
	}else{
		app_priv->tweet_view=tweet_view_new(app_priv->window);
		gtk_widget_hide(GTK_WIDGET(app_priv->expand_box));
	}
	
	/* Set the main window geometry */ 	 
	geometry_load();
	
	/* Initial status of widgets */
	app_state_on_connection(FALSE);
	
	/* Check get2gnow directory and images cache */
	app_check_dir();
	
	/* Ok, set the window state based on the gconf value */
	if(!gconfig_if_bool(PREFS_UI_HIDDEN))
		gtk_widget_show(GTK_WIDGET(app_priv->window));
	else
		gtk_widget_hide(GTK_WIDGET(app_priv->window));
	
	app_connect();
	
	app_accounts_treeview_fill();
}

void app_tweet_view_set_embed(GtkCheckButton *check_button, gpointer user_data){
	gboolean dont_embed=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button));
	debug(DEBUG_DOMAINS, "Embed TweetView changed:\t[%s].", (dont_embed ?"floating" :"embed" ) );
	if(gconfig_if_bool(PREFS_UI_TWEET_VIEW_USE_DIALOG)==dont_embed)
		return;
	
	gconfig_set_bool(PREFS_UI_TWEET_VIEW_USE_DIALOG, dont_embed);
	
	if(dont_embed){
		if(gtk_widget_get_parent(GTK_WIDGET(app_priv->tweet_view->tweet_view_embed))==GTK_WIDGET(app_priv->tweet_view->tweet_view))
			return;
		
		debug(DEBUG_DOMAINS, "Displaying TweetView as a stand alone dialog & setting TweetView's parent window..");
		gtk_widget_reparent(GTK_WIDGET(app_priv->tweet_view->tweet_view_embed), GTK_WIDGET(app_priv->tweet_view->tweet_view));
		gtk_window_present(GTK_WINDOW(app_priv->tweet_view->tweet_view));
		g_object_add_weak_pointer(G_OBJECT(app_priv->tweet_view->tweet_view),(gpointer) &app_priv->tweet_view->tweet_view);
		gtk_window_set_transient_for(GTK_WINDOW(app_priv->tweet_view->tweet_view), app_priv->window);
		gtk_widget_hide(GTK_WIDGET(app_priv->expand_box));
	}else{
		if(gtk_widget_get_parent(GTK_WIDGET(app_priv->tweet_view->tweet_view_embed))==GTK_WIDGET(app_priv->tweet_view_vbox))
			return;
		
		debug(DEBUG_DOMAINS, "Embeding TweetView's into %s main window.", PACKAGE_NAME);
		gtk_widget_reparent(GTK_WIDGET(app_priv->tweet_view->tweet_view_embed), GTK_WIDGET(app_priv->tweet_view_vbox));
		gtk_widget_show(GTK_WIDGET(app_priv->expand_box));
		gtk_widget_show(GTK_WIDGET(app_priv->tweet_view_vbox));
		gtk_widget_show(GTK_WIDGET(app_priv->tweet_view->tweet_view_embed));
		gtk_widget_hide(GTK_WIDGET(app_priv->tweet_view->tweet_view));
	}
	tweet_view_set_embed_toggle_and_image();
	geometry_load();
}//app_tweet_view_embed

GtkWindow *app_get_window(void){
	return app_priv->window;
}//app_get_window

GtkMenuBar *app_get_main_menu(void){
	return app_priv->menubar;
}//app_get_main_menu

static void app_accounts_treeview_fill(void){
}//app_accounts_treeview_fill

GtkMenuItem *app_get_menu(const gchar *menu){
	if((g_str_equal(menu, "accounts")) ) return GET_PRIV(app)->menu_accounts;
	if((g_str_equal(menu, "tweets")) ) return GET_PRIV(app)->menu_tweets;
	if((g_str_equal(menu, "friends")) ) return GET_PRIV(app)->menu_friends;
	if((g_str_equal(menu, "timelines")) ) return GET_PRIV(app)->menu_timelines;
	if((g_str_equal(menu, "help")) ) return GET_PRIV(app)->menu_help;
	//if((g_str_equal(menu, "")) ) return GET_PRIV(app)->menu_;
	return NULL;
}//app_get_menu

TweetList *app_get_tweet_list(void){
	return GET_PRIV(app)->listview;
}//app_get_tweet_list

static void main_window_destroy_cb(GtkWidget *window, App *app){
	unset_selected_tweet();
	/* Add any clean-up code above this function call */
	gtk_widget_destroy( GTK_WIDGET(GET_PRIV(app)->window) );
}

gboolean app_has_status_icon(void){
	return gtk_status_icon_is_embedded(app_priv->status_icon);
}//app_has_status_icon

static gboolean main_window_delete_event_cb(GtkWidget *window, GdkEvent *event, App *app){
	if(app_has_status_icon()) {
		hint_show(PREFS_HINTS_CLOSE_MAIN_WINDOW,
						  _("get2gnow is still running, it is just hidden."),
						  _("Click on the notification area icon to show get2gnow."),
						   GTK_WINDOW(app_get_window()),
						   NULL, NULL);
		
		app_set_visibility(FALSE);
		return TRUE;
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
	)) return TRUE;
	
	return FALSE;
}

static void app_set_radio_group(App *app, GtkBuilder *ui){
	GtkRadioMenuItem *item;
	
	const gchar *radio_actions[]={
		"view_friends_timeline",
		"view_mentions",
		"view_direct_messages",
		"view_favorites_timeline",
		"view_my_timeline",
		"view_public_timeline",
	};
	
	
	for(int i=0; i < G_N_ELEMENTS(radio_actions); i++) {
		item=GTK_RADIO_MENU_ITEM(gtk_builder_get_object(ui, radio_actions[i]));
		app_priv->group=gtk_radio_menu_item_get_group(item);
	}
}


static void app_toggle_visibility(void){
	gboolean       visible;
	
	visible=window_get_is_visible(GTK_WINDOW(app_priv->window));
	
	if(visible && gtk_status_icon_is_embedded(app_priv->status_icon)) {
		geometry_save();
		
		gtk_widget_hide(GTK_WIDGET(app_priv->window));
		
		if(app_priv->size_timeout_id){
			g_source_remove(app_priv->size_timeout_id);
			app_priv->size_timeout_id=0;
		}
	} else {
		geometry_load();
		window_present(GTK_WINDOW(app_priv->window), TRUE);
		
		if(gconfig_if_bool(PREFS_UI_TWEET_VIEW_USE_DIALOG))
			window_present(GTK_WINDOW(app_priv->tweet_view->tweet_view), TRUE);
	}
	/* Save the window visibility state */
	gconfig_set_bool(PREFS_UI_HIDDEN, visible);
}

void app_set_visibility(gboolean visible){
	GtkWindow *window=app_get_window();
	gconfig_set_bool(PREFS_UI_HIDDEN, !visible);
	
	if(!visible)
		gtk_widget_hide(GTK_WIDGET(window));
	else{
		window_present(GTK_WINDOW(window), TRUE);
		if(gconfig_if_bool(PREFS_UI_TWEET_VIEW_USE_DIALOG))
			window_present(GTK_WINDOW(app_priv->tweet_view->tweet_view), TRUE);
	}
}

static void app_quit_cb(GtkWidget  *widget, App  *app){
	gtk_main_quit();
}

void app_refresh_timeline(GtkWidget *window, App *app){
	network_refresh();
}

static void app_timeline_cb(GtkRadioMenuItem *item, App *app){
	debug(DEBUG_DOMAINS, "Switching timelines. MenuItem selected: %s", gtk_menu_item_get_label(GTK_MENU_ITEM(item)) );
	
	if(GTK_CHECK_MENU_ITEM(app_priv->timeline_public)->active)
		return network_get_timeline(API_TIMELINE_PUBLIC);
		
	if(GTK_CHECK_MENU_ITEM(app_priv->timeline_mine)->active)
		return network_get_user_timeline(current_service, NULL);
		
	if(GTK_CHECK_MENU_ITEM(app_priv->timeline_favorites)->active)
		return network_get_timeline(API_FAVORITES);
		
	if(GTK_CHECK_MENU_ITEM(app_priv->timeline_dm)->active)
		return network_get_timeline(API_DIRECT_MESSAGES);
		
	if(GTK_CHECK_MENU_ITEM(app_priv->timeline_mentions)->active) 
		return network_get_timeline(API_MENTIONS);
	
	if(GTK_CHECK_MENU_ITEM(app_priv->timeline_replies)->active)
		return network_get_timeline(API_REPLIES);
	
	if(GTK_CHECK_MENU_ITEM(app_priv->timeline_friends)->active) 
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


static void app_services_cb(GtkWidget *widget, App *app){
	services_dialog_show(GTK_WINDOW(app_priv->window));
}

static void app_select_service(GtkMenuItem *item, App *app){
	popup_select_service( GET_PRIV(app)->window );
}//app_select_service

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
		
	
	show=window_get_is_visible(GTK_WINDOW(app_priv->window));
	
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
	
	
	app_priv->popup_menu_show_app=gtk_toggle_action_new("tray_show_app",
								   _("_Show "),
								   NULL,
								   NULL);
	g_signal_connect(G_OBJECT(app_priv->popup_menu_show_app),
					  "toggled", G_CALLBACK(app_show_hide_cb),
					  app);
					
	new_msg=gtk_action_new("tray_new_message", _("_New Tweet"), NULL, "gtk-new");
	g_signal_connect(G_OBJECT(new_msg), "activate", G_CALLBACK(tweets_new_tweet), app);
	
	new_dm=gtk_action_new("tray_new_dm", _("New _DM"), NULL, "gtk-jump-to");
	g_signal_connect(G_OBJECT(new_dm), "activate", G_CALLBACK(tweet_view_new_dm), app);
	
	about=gtk_action_new("tray_about", _("_About"), NULL, "gtk-about");
	g_signal_connect(G_OBJECT(new_dm), "activate", G_CALLBACK(app_about_cb), app);
	
	quit=gtk_action_new("tray_quit", _("_Quit"), NULL, "gtk-quit");
	g_signal_connect(G_OBJECT(quit), "activate", G_CALLBACK(app_quit_cb), app);
	
	app_priv->popup_menu=gtk_menu_new();
	w=gtk_action_create_menu_item(GTK_ACTION(app_priv->popup_menu_show_app));
	gtk_menu_shell_append(GTK_MENU_SHELL(app_priv->popup_menu), w);
	w=gtk_separator_menu_item_new();
	gtk_widget_show(w);
	gtk_menu_shell_append(GTK_MENU_SHELL(app_priv->popup_menu), w);
	w=gtk_action_create_menu_item(new_msg);
	gtk_menu_shell_append(GTK_MENU_SHELL(app_priv->popup_menu), w);
	w=gtk_action_create_menu_item(quit);
	gtk_menu_shell_append(GTK_MENU_SHELL(app_priv->popup_menu), w);
}

static void app_status_icon_create(void){
	app_priv->status_icon=gtk_status_icon_new_from_icon_name("get2gnow");
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

void app_create(void){
	g_object_new(TYPE_APP, NULL);
	app_setup();
}

App *app_get(void){
	g_assert(app != NULL);
	return app;
}
 
static gboolean configure_event_timeout_cb(GtkWidget *widget){
	geometry_save();
	
	app_priv->size_timeout_id=0;
	
	return FALSE;
}

static gboolean app_window_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, App *app){
	if(app_priv->size_timeout_id)
		g_source_remove(app_priv->size_timeout_id);
	
	app_priv->size_timeout_id=g_timeout_add(
						500,
						(GSourceFunc) configure_event_timeout_cb,
						widget
	);
	
	return FALSE;
}

static gboolean app_connect(void){
	debug(DEBUG_DOMAINS, "Logging into online services.");
	if(!online_services_login(online_services))
		return FALSE;
	
	app_retrieve_default_timeline();
	return TRUE;
}//connect

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
	
	debug(DEBUG_DOMAINS, "Selecting default timeline's radio button: %s", timeline);
	/* TODO: Make this actually work as in like change the radio button.
	 * NOTE: I think I'm going to have to convert these into a GtkRadioMenuItem
	 * */
	
	if((g_str_equal(timeline, API_TIMELINE_FRIENDS)) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_friends), TRUE);
	
	if((g_str_equal(timeline, API_MENTIONS)) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_mentions), TRUE);
	
	if((g_str_equal(timeline, API_DIRECT_MESSAGES)) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_dm), TRUE);
	
	if((g_str_equal(timeline, API_FAVORITES)) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_favorites), TRUE);
	
	if((g_str_equal(timeline, API_TIMELINE_MY)) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_mine), TRUE);
	
	if((g_str_equal(timeline, API_TIMELINE_PUBLIC)) )
		return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM( app_priv->timeline_public), TRUE);
	
	/* Let's fallback to friends timeline */
	return gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(app_priv->timeline_friends), TRUE);
}

/* Function to retrieve the users default timeline */
static void app_retrieve_default_timeline(void){
	gchar         *timeline=NULL;
	
	gconfig_get_string(PREFS_TWEETS_HOME_TIMELINE, &timeline);
	
	if(G_STR_EMPTY(timeline))
		timeline=g_strdup(API_TIMELINE_FRIENDS);
	
	app_set_default_timeline(app, timeline);
	debug(DEBUG_DOMAINS, "Retriving default timeline: %s", timeline);
	network_get_timeline(timeline);
	g_free(timeline);
}

static void
app_check_dir(void)
{
	gchar    *file;
	
	file=g_build_filename(g_get_home_dir(), ".gnome2", CACHE_IMAGES, NULL);
	
	if(!g_file_test(file, G_FILE_TEST_EXISTS|G_FILE_TEST_IS_DIR)) {
		debug(DEBUG_DOMAINS, "Making directory: %s", file);
		g_mkdir_with_parents(file, S_IRUSR|S_IWUSR|S_IXUSR);
	}
	
	g_free(file);
}


static void app_connection_items_setup(GtkBuilder *ui){
	GList         *list;
	gint           i;
	
	const gchar   *widgets_connected[]={
		"services_disconnect",
		"tweets",
		"timelines",
		"friends",
		"expand_box",
	};
	
	const gchar   *widgets_disconnected[]={
		"services_connect",
	};
	
	for(i=0, list=NULL; i < G_N_ELEMENTS(widgets_connected); i++)
		list=g_list_prepend(list,(gtk_builder_get_object(ui, widgets_connected[i])) );
	app_priv->widgets_connected=list;
	
	for(i=0, list=NULL; i < G_N_ELEMENTS(widgets_disconnected); i++)
		list=g_list_prepend(list,(gtk_builder_get_object(ui, widgets_disconnected[i])) );
	app_priv->widgets_disconnected=list;
}

void app_state_on_connection(gboolean connected){
	if(!( app_priv && app_priv->widgets_connected && app_priv->widgets_disconnected ))
		return;
	GList         *l;
	
	for(l=app_priv->widgets_connected; l; l=l->next)
		gtk_widget_set_sensitive( GTK_WIDGET(l->data), connected );
	
	for(l=app_priv->widgets_disconnected; l; l=l->next)
		gtk_widget_set_sensitive( GTK_WIDGET(l->data), !connected );
	
	g_list_free(l);
	tweet_view_sexy_select();
}

void app_statusbar_printf(const gchar *msg, ...){
	if(!( app_priv && app_priv->statusbar && GTK_IS_STATUSBAR(app_priv->statusbar) ))
		return;
	
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
	if(!( app_priv && app_priv->statusbar && GTK_IS_STATUSBAR(app_priv->statusbar) ))
		return;
	
	/* starts displaying the status bar message for 5 seconds before returning to the system default */
	gtk_statusbar_pop(GTK_STATUSBAR(app_priv->statusbar), 1);
	gtk_statusbar_push(GTK_STATUSBAR(app_priv->statusbar), 1, message );
}


void app_notify_sound(gboolean force){
	if(!( force && gconfig_if_bool(PREFS_UI_SOUND) ))
		return;
	
	gtk_widget_error_bell(GTK_WIDGET(app_priv->tweet_view->sexy_entry));
}

gboolean app_notify_on_timeout(gpointer tweet){
	if(tweet) {
		app_notify(tweet);
		g_free(tweet);
	}
	return FALSE;
}

void app_notify(gchar *msg){
	if(!gconfig_if_bool(PREFS_UI_NOTIFICATION))
		return;
	
	NotifyNotification *notification;
	GError             *error=NULL;
	
	notification=notify_notification_new(PACKAGE_NAME, msg, PACKAGE_TARNAME, NULL);
	
	notify_notification_set_timeout(notification, 8 * 1000);
	notify_notification_show(notification, &error);
	g_object_unref(G_OBJECT(notification));
	
	if(!error)
		return;
	
	debug(DEBUG_DOMAINS, "Error displaying notification: %s", error->message);
	g_error_free(error);
}

