/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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
 * License ("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
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
 * user-visible display as defined in Section 6.4 (d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright (C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright (C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright (C) of the User the data is connected to.
 * Users may lisences their data under the terms of an OSI approved or Creative Commons
 * license.  Users must be allowed to select their choice of license for each piece of data
 * on an individual bases and cannot be blanketly applied to all of the Users.  The User may
 * select a default license for their data.  All of the Software's data pertaining to each
 * User must be fully accessible, exportable, and deletable to that User.
 */
/********************************************************************************
 *                      My art, code, & programming.                            *
 ********************************************************************************/
#define _GNU_SOURCE
#define _THREAD_SAFE



/********************************************************************************
 * Engine, event, ai, & graphics toolkit, & etc headers.                        *
 *         e.g. #include <gtk/gtk.h>                                            *
 ********************************************************************************/
#include <gtk/gtk.h>

/*******************************************************************************
 * keyboard & mouse event structs, enums, & defines.                           *
 *          /usr/include/gtk-2.0/gdk/gdkevents.h                               *
 *******************************************************************************/
#include <gdk/gdk.h>
#include <gdk/gdkevents.h>
#include <gdk/gdkkeys.h>
#include <gdk/gdktypes.h>
#include <gdk/gdkkeysyms.h>

/********************************************************************************
 *      Project, system & library headers, e.g. #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/

#include <libsexy/sexy.h>

#include "config.h"
#include "program.h"

#include "tweet-list.h"

#include "images.h"

#include "online-services.defines.h"
#include "online-services-typedefs.h"
#include "online-service-request.h"
#include "online-service.types.h"
#include "online-service.h"
#include "network.h"

#include "gtkbuilder.h"
#include "gconfig.h"
#include "preferences.h"

#include "label.h"
#include "main-window.h"
#include "tweet-lists.h"

#include "parser.h"
#include "tweets.h"
#include "control-panel.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
#define	GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_TWEET_LIST, TweetListPrivate))

typedef struct _TimelineLabels TimelineLabels;

struct _TimelineLabels{
	UpdateMonitor	monitoring;
	const gchar	*timeline;
	const gchar	*tab_label_string;
	const gchar	*menu_label_string;
};

TimelineLabels TimelineLabelsList[]={
	{BestFriends,	API_TIMELINE_USER,		N_("%s_%s's %s%s Updates"),	N_("%s%s's %s%s Updates")},
	{Users,		API_TIMELINE_USER,		N_("%s_%s's %s%s Updates"),	N_("%s%s's %s%s Updates")},
	{Tweets,	API_TIMELINE_FRIENDS,		N_("My _Friends' Updates"),	N_("My Friends' Updates")},
	{Replies,	API_REPLIES,			N_("@ _Replies"),		N_("@ Replies")},
	/*{Replies,	API_MENTIONS,			N_("@ _Mentions"),		N_("@ Mentions")},*/
	{DMs,		API_DIRECT_MESSAGES,		N_("My DMs _Inbox"),		N_("My DMs Inbox")},
	{Faves,		API_FAVORITES,			N_("My Star_'d Updates"),	N_("My Star'd Updates")},
	{Searches,	API_TIMELINE_SEARCH,		N_("_Search Results"),		N_("Search Results")},
	{Groups,	API_TIMELINE_GROUP,		N_("_Group Discussions"),	N_("Group Discussions")},
	{Timelines,	API_TIMELINE_PUBLIC,		N_("_Global Updates"),		N_("Public Updates")},
	{Archive,	API_TIMELINE_MINE,		N_("_My Updates"),		N_("My Updates")},
	{None,		NULL,				N_("Unknow_n timeline"),	N_("Unknown timeline")}
};

struct _TweetListPrivate {
	guint			timeout_id;
	gint			page;
	OnlineService		*service;
	gchar			*user;
	
	UpdateMonitor		monitoring;
	const gchar		*monitoring_string;
	gint8			has_loaded;
	
	gint			minutes;
	gulong			reload;
	
	gchar			*timeline;
	gchar			*tab_label_string;
	gchar			*menu_label_string;
	
	gint			index;
	
	guint			total;
	gboolean		unread;
	gboolean		loading;
	
	guint			connected_online_services;
	gdouble			maximum;
	gdouble			minimum;
	
	GtkLabel		*tab_label;
	GtkLabel		*menu_label;
	
	GtkVBox			*vbox;
	
	GtkHBox			*hbox;
	
	GtkHandleBox		*handlebox;
	GtkToolbar		*toolbar;
	GtkToolButton		*refresh_tool_button;
	
	GtkToolItem		*progress_bar_custom_tool_button;
	GtkProgressBar		*progress_bar;
	GtkAdjustment		*progress_bar_adjustment;
	
	GtkSpinButton		*max_tweets_spin_button;
	GtkAdjustment		*max_tweets_adjustment;
	
	GtkToolButton		*close_tool_button;
	GtkToggleToolButton	*stop_toggle_tool_button;
	
	GtkHSeparator		*hseparator;
	
	/* These make up the core of TweetView */
	GtkScrolledWindow	*scrolled_window;
	
	SexyTreeView		*sexy_tree_view;
	
	GtkTreeViewColumn	*pixbuf_avatar_tree_view_column;
	GtkCellRendererPixbuf	*pixbuf_avatar_cell_renderer_pixbuf;
	
	GtkTreeViewColumn	*string_tweet_tree_view_column;
	GtkCellRendererText	*string_tweet_cell_renderer_text;
	
	GtkTreeViewColumn	*created_ago_str_tree_view_column;
	GtkCellRendererText	*created_ago_str_cell_renderer_text;
	
	GtkTreeViewColumn	*string_from_tree_view_column;
	GtkCellRendererText	*string_from_cell_renderer_text;
	
	GtkTreeViewColumn	*string_rcpt_tree_view_column;
	GtkCellRendererText	*string_rcpt_cell_renderer_text;
	
	GtkListStore		*list_store;
	GtkTreeModel		*tree_model;
	GtkTreeModel		*tree_model_sort;
};



/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define DEBUG_DOMAINS "OnlineServices:UI:GtkBuilder:GtkBuildable:Networking:Tweets:Requests:Notification:Settings:Setup:Start-Up:TweetList.c"
#include "debug.h"

#define GtkBuilderUI "tweet-list"

#define	MINIMUM_TWEETS	20.00
#define	MAXIMUM_TWEETS	100.00


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static void tweet_list_class_init(TweetListClass *klass);
static void tweet_list_init(TweetList *tweet_list);
static void tweet_list_finalize(TweetList *tweet_list);

static void tweet_list_setup(TweetList *tweet_list);
static float tweet_list_prepare_reload(TweetList *tweet_list);

static void tweet_list_set_timeline_label(TweetList *tweet_list, const gchar *timeline);

static void tweet_list_refresh_clicked(GtkButton *tweet_list_refresh_tool_button, TweetList *tweet_list);

static void tweet_list_stop_toggle_setup(TweetList *tweet_list);
static void tweet_list_stop_toggle_tool_button_toggled(GtkToggleToolButton *stop_toggle_tool_button, TweetList *tweet_list);

static void tweet_list_set_adjustment(TweetList *tweet_list);

static void tweet_list_update_age(TweetList *tweet_list, gint delete_older_then);
static void tweet_list_clean_up(TweetList *tweet_list);
static void tweet_list_set_selected_index(TweetList *tweet_list);
static void tweet_list_check_maximum_updates(TweetList *tweet_list);
static void tweet_list_check_inbox(TweetList *tweet_list);

static void tweet_list_grab_focus_cb(GtkWidget *widget, TweetList *tweet_list);
static void tweet_list_set_sexy_tooltip(SexyTreeView *sexy_tree_view, GtkTreePath *path, GtkTreeViewColumn *column, TweetList *tweet_list);
static void tweet_list_changed_cb(SexyTreeView *tweet_list_sexy_tree_view, TweetList *tweet_list);
static void tweet_list_size_cb(GtkWidget *widget, GtkAllocation *allocation, TweetList *tweet_list);

static void tweet_list_clear(TweetList *tweet_list);

static void tweet_list_move(TweetList *tweet_list, GdkEventKey *event);

static void tweet_list_index_validate(TweetList *tweet_list);
static void tweet_list_index_select(TweetList *tweet_list);
static void tweet_list_index_scroll_to(TweetList *tweet_list);

static void tweet_list_scroll_to_top(TweetList *tweet_list);


G_DEFINE_TYPE(TweetList, tweet_list, SEXY_TYPE_TREE_VIEW);


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
/* BEGIN: GObject core methods. */
static void tweet_list_class_init(TweetListClass *klass){
	GObjectClass	*object_class=G_OBJECT_CLASS(klass);
	
	object_class->finalize=(GObjectFinalizeFunc)tweet_list_finalize;
	
	g_type_class_add_private(object_class, sizeof(TweetListPrivate));
}/* tweet_list_class_init */

static void tweet_list_init(TweetList *tweet_list){
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	this->service=NULL;
	this->user=NULL;
	
	this->has_loaded=-1;
	this->unread=TRUE;
	this->loading=FALSE;
	
	this->minutes=0;
	this->reload=0;
	
	this->connected_online_services=this->timeout_id=this->index=this->total=0;
	this->maximum=this->minimum=0.0;
	this->timeline=this->tab_label_string=this->menu_label_string=NULL;
	this->tab_label=this->menu_label=NULL;
	this->vbox=NULL;
	this->tree_model_sort=NULL;
	tweet_list_setup(tweet_list);
	
	/*
	g_object_set(tweet_list, "rules-hint", TRUE, "reorderable", TRUE, "headers-visible", FALSE, NULL);
	g_signal_connect(tweet_list, "size-allocate", G_CALLBACK(tweet_list_size_cb), tweet_list);
	g_signal_connect(tweet_list, "cursor-changed", G_CALLBACK(tweet_list_changed_cb), tweet_list);
	g_signal_connect(tweet_list, "row-activated", G_CALLBACK(online_service_request_selected_update_reply), tweet_list);
	*/
	g_signal_connect(tweet_list, "grab-focus", G_CALLBACK(tweet_list_grab_focus_cb), tweet_list);
	g_signal_connect(tweet_list, "key-press-event", G_CALLBACK(tweets_hotkey), tweet_list);
}/* tweet_list_init */

TweetList *tweet_list_new(const gchar *timeline, OnlineService *service){
	if(G_STR_EMPTY(timeline)) {
		debug("**ERROR:** Cannot create tweet view for an empty timeline.");
		return NULL;
	}
	TweetList *tweet_list=g_object_new(TYPE_TWEET_LIST, NULL);
	
	debug("Creating new TweetView for timeline: %s.", timeline);
	if(service && service->connected )
		GET_PRIVATE(tweet_list)->service=service;
	
	tweet_list_set_timeline_label(tweet_list, timeline);
	tweet_list_stop_toggle_setup(tweet_list);
	
	tweet_list_start(tweet_list);
	
	gtk_widget_show_all(GTK_WIDGET(GET_PRIVATE(tweet_list)->vbox));
	
	tweet_list=g_object_ref_sink(tweet_list);
	
	if(gconfig_if_bool( UPDATE_VEWER_TOOLBAR_VISIBILITY, FALSE) )
		tweet_list_toggle_toolbar(tweet_list);
	
	return tweet_list;
}/*tweet_list_new(timeline);*/

static void tweet_list_finalize(TweetList *tweet_list){
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(this->timeout_id && G_STR_N_EMPTY(this->timeline))
		program_timeout_remove(&this->timeout_id, g_strrstr(this->timeline, "/"));
	
	if(this->user) uber_free(this->user);
	if(this->timeline) uber_free(this->timeline);
	if(this->tab_label_string) uber_free(this->tab_label_string);
	if(this->menu_label_string) uber_free(this->menu_label_string);
	
	if(this->tab_label) gtk_widget_destroy(GTK_WIDGET(this->tab_label));
	if(this->menu_label) gtk_widget_destroy(GTK_WIDGET(this->menu_label));
	if(this->vbox) gtk_widget_destroy(GTK_WIDGET(this->vbox));
	if(this->tree_model_sort) g_object_unref(this->tree_model_sort);
	
	G_OBJECT_CLASS(tweet_list_parent_class)->finalize(G_OBJECT(tweet_list));
}/* tweet_list_finalized */

const gchar *monitoring_to_string(UpdateMonitor monitoring){
	switch(monitoring){
		case None: return _("None");
		case DMs: return _("DMs");
		case Replies: return _("Replies");
		case Faves: _("Faves");
		case BestFriends: return _("BestFriends");
		case Tweets: return _("Tweets");
		case Searches: return _("Searches");
		case Groups: return _("Groups");
		case Timelines: return _("Timelines");
		case Users: return _("Users");
		case Archive: return _("Archive");
		default: return _("Unknown UpdateMonitor");
	}
}/*monitoring_to_string(monitoring);*/

/*BEGIN: Custom TweetList methods.*/
/**
 *@returns: -1, 0, 1 if the timeline hasn't loaded, is loading, or is reloading.
 */
gint8 tweet_list_has_loaded(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return 0;
	return GET_PRIVATE(tweet_list)->has_loaded;
}/*tweet_list_has_loaded(tweet_list);*/

const gchar *tweet_list_get_timeline(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIVATE(tweet_list)->timeline;
}/*tweet_list_get_timeline(tweet_list);*/

GtkVBox *tweet_list_get_child(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIVATE(tweet_list)->vbox;
}/*tweet_list_get_child(tweet_list);*/

GtkLabel *tweet_list_get_tab(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIVATE(tweet_list)->tab_label;
}/*tweet_list_get_label(TweetList *tweet_list);*/

GtkLabel *tweet_list_get_menu(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIVATE(tweet_list)->menu_label;
}/*tweet_list_get_label(TweetList *tweet_list);*/

GtkListStore *tweet_list_get_list_store(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIVATE(tweet_list)->list_store;
}/*tweet_list_get_list_store(tweet_list);*/

GtkTreeModel *tweet_list_get_tree_model(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIVATE(tweet_list)->tree_model;
}/*tweet_list_get_tree_model(tweet_list);*/

gint tweet_list_get_page(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return -1;
	return GET_PRIVATE(tweet_list)->page;
}/*tweet_list_get_page(tweet_list);*/

UpdateMonitor tweet_list_get_monitoring(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return None;
	return GET_PRIVATE(tweet_list)->monitoring;
}/*tweet_list_get_monitoring(tweet_list);*/

const gchar *tweet_list_get_monitoring_string(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIVATE(tweet_list)->monitoring_string;
}/*tweet_list_get_monitoring_string(TweetList *tweet_list);*/

guint tweet_list_get_notify_delay(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return 1*10;
	return (GET_PRIVATE(tweet_list)->page+1)*10;
}/*tweet_list_get_notify_delay(tweet_list);*/

gint tweet_list_get_total(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return 0;
	return GET_PRIVATE(tweet_list)->total;
}/*tweet_list_get_total(tweet_list);*/

void tweet_list_set_page(TweetList *tweet_list, gint page){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	GET_PRIVATE(tweet_list)->page=page;
}/*tweet_list_set_page(tweet_list, 0);*/

void tweet_list_start(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	gtk_progress_bar_set_fraction(this->progress_bar, tweet_list_prepare_reload(tweet_list) );
	
	if(!gtk_toggle_tool_button_get_active(this->stop_toggle_tool_button)){
		debug("Creating timeout to reload %s.", this->menu_label_string);
		this->timeout_id=g_timeout_add(this->reload, (GSourceFunc)tweet_list_refresh, tweet_list);
	}
	tweet_list_clean_up(tweet_list);
	tweet_list_update_age(tweet_list, 0);
	tweet_list_set_adjustment(tweet_list);
	if(this->loading) return;
	if(this->minutes){
		this->loading=TRUE;
		if(!this->service){
			online_services_request(QUEUE, this->timeline, NULL, network_display_timeline, tweet_list, (gpointer)this->monitoring);
		}else{
			online_service_request(this->service, QUEUE, this->timeline, NULL, network_display_timeline, tweet_list, (gpointer)this->monitoring);
		}
	}
}/*tweet_list_start(TweetList *tweet_list);*/

static float tweet_list_prepare_reload(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return 0.0;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(gtk_progress_bar_get_fraction(this->progress_bar)==0.0){
		this->minutes=0;
		this->reload=(this->monitoring+this->page+1)*5000;
		return 0.0;
	}
	
	gint minutes=0;
	gconfig_get_int(PREFS_TWEETS_RELOAD_TIMELINES, &minutes);
	if(this->minutes==minutes && this->has_loaded > 1 ) return 0.0;
	this->minutes=minutes;
	/* With multiple timeline support timeline reloading interval shouldn't be less than 5 minutes */
	if(this->minutes < 5)
		gconfig_set_int(PREFS_TWEETS_RELOAD_TIMELINES, (this->minutes=minutes=5) );
	
	switch(this->monitoring){
		case None:	default:
			debug("TweetList(s) monitoring %s does not auto-reload.", this->monitoring_string);
			return 1.0;
		case Archive:	case Timelines:
			if(minutes<15) minutes=15;
			else minutes+=8;
			break;
		case Replies:
			minutes+=1;
			break;
		case DMs:
			minutes+=3;
			break;
		case BestFriends:	case Users:
			minutes+=12;
			break;
		case Faves:
			minutes+=5;
			break;
		case Searches: case Groups:
			minutes+=7;
			break;
		case Tweets:
			break;
	}
	
	if(this->page) minutes+=this->page;
	if(this->has_loaded < 2) this->has_loaded++;
	this->reload=minutes*60000;
	
	return 1.0;
}/*tweet_list_prepare_reload(tweet_list);*/

static void tweet_list_set_adjustment(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	guint connected_online_services=online_services_has_connected(0);
	if(connected_online_services==this->connected_online_services)	return;
	
	this->connected_online_services=connected_online_services;
	
	this->minimum=connected_online_services*MINIMUM_TWEETS;
	gtk_adjustment_set_lower(this->max_tweets_adjustment, this->minimum);
	gtk_adjustment_set_lower(this->progress_bar_adjustment, this->maximum);
	
	this->maximum=connected_online_services*MAXIMUM_TWEETS;
	gtk_adjustment_set_upper(this->max_tweets_adjustment, this->maximum);
	gtk_adjustment_set_upper(this->progress_bar_adjustment, this->maximum);
	
	gdouble updates=0.0, max_updates=gtk_adjustment_get_value(this->max_tweets_adjustment);
	if(!max_updates)
		if(this->monitoring==DMs || this->monitoring==Replies || this->monitoring==Faves)
			updates=this->maximum;
		else
			updates=this->minimum;
	else if(max_updates > this->maximum)
		updates=this->maximum;
	else if(max_updates < this->minimum)
		updates=this->minimum;
	else
		return;
	
	gtk_adjustment_set_value(this->max_tweets_adjustment, updates);
	gtk_adjustment_set_value(this->progress_bar_adjustment, updates);
	
	gtk_spin_button_set_value(this->max_tweets_spin_button, gtk_adjustment_get_value(this->max_tweets_adjustment));
	
	if(!(this->monitoring==DMs || this->monitoring==Replies || this->monitoring==Faves))
		return;
	
	gtk_widget_set_sensitive(GTK_WIDGET(this->max_tweets_spin_button), FALSE);
	gtk_widget_hide(GTK_WIDGET(this->max_tweets_spin_button));
}/*tweet_list_set_adjustment(tweet_list);*/

static void tweet_list_clean_up(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(!this->total) return;
	switch(this->monitoring){
		case	DMs:	case Replies:
		case	Faves:	case BestFriends:
		case Users:
			tweet_list_check_inbox(tweet_list);
			break;
		
		case	Tweets:	case Timelines:
		case	Searches:	case Groups:
		case	Archive:
			tweet_list_check_maximum_updates(tweet_list);
		
		case None: default:
			return;
	}
}/*tweet_list_clean_up(tweet_list);*/

static void tweet_list_check_maximum_updates(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(!this->total) return;
	gdouble max_updates=gtk_spin_button_get_value(this->max_tweets_spin_button);
	if(max_updates > this->maximum)
		max_updates=this->maximum;
	else if(max_updates < this->minimum)
		max_updates=this->minimum;
	if(this->total <= max_updates)	return;
	
	debug("Cleaning up TweetList for %s.  TweetList's total updates: %d.  Maximum allowed updates: %f", this->timeline, this->total, max_updates);
	if(!this->index){
		debug("Moving focus to TweetList's top since no iter is currently selected selected.");
		tweet_list_scroll_to_top(tweet_list);
	}
	for(gint i=this->total-1; i>max_updates; i--){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter(this->tree_model, iter, path)))
			debug("Removing iter at index: %d failed.  Unable to retrieve iter from path.", i);
		else{
			debug("Removing iter at index: %d", i);
			if(i==this->index){
				this->index=0;
				debug("Moving focus to TweetList's top since the currently selected iter is being removed.");
				tweet_list_scroll_to_top(tweet_list);
			}
			gtk_list_store_remove(this->list_store, iter);
			this->total--;
		}
		gtk_tree_path_free(path);
		uber_free(iter);
	}
}/*tweet_check_maximum_updates(tweet_list);*/

static void tweet_list_set_selected_index(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(!this->total) return;
	
	debug("Updating Selected Index for updates appearing in the TweetList for %s.  TweetList's total updates: %d.", this->timeline, this->total);
	if(!this->index){
		debug("Moving focus to TweetList's top since no iter is currently selected selected.");
		tweet_list_scroll_to_top(tweet_list);
	}
	for(guint i=0; i<=this->total; i++){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter(this->tree_model, iter, path)))
			debug("Removing iter at index: %d failed.  Unable to retrieve iter from path.", i);
		else{
			debug("Removing iter at index: %d", i);
			gtk_list_store_set(this->list_store, iter, GUINT_SELECTED_INDEX, i, -1);
		}
		gtk_tree_path_free(path);
		uber_free(iter);
	}
}/*tweet_list_set_selected_index(tweet_list);*/

static void tweet_list_check_inbox(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(!this->total) return;
	
	gint update_expiration=0;
	switch(this->monitoring){
		case DMs:
			/*By default Direct Messages from the last 4 weeks are loaded.*/
			gconfig_get_int_or_default(PREFS_TWEETS_ARCHIVE_DMS, &update_expiration, 2419200);
			break;
		
		case Replies:
			/*By default Replies, & @ Mentions, from the last 7 days are loaded.*/
			gconfig_get_int_or_default(PREFS_TWEETS_ARCHIVE_REPLIES, &update_expiration, 604800);
			break;
		
		case Faves:
			/*By default "My Favorite/Started Updates" from the last 4 weeks are loaded.*/
			gconfig_get_int_or_default(PREFS_TWEETS_ARCHIVE_FAVES, &update_expiration, 2419200);
			break;
		
		case BestFriends:
			/*By default "Best Friends' Updates" from the last 24 hrs are loaded.*/
			gconfig_get_int_or_default(PREFS_TWEETS_ARCHIVE_BEST_FRIENDS, &update_expiration, 86400);
			break;
		
		case Users:
			update_expiration=86400;
		
		case	None:	case	Tweets:
		case	Searches:	case Groups:
		case	Timelines:	case	Archive:
			return;
	}
	
	tweet_list_update_age(tweet_list, update_expiration);
}/*tweet_list_check_inbox(tweet_list);*/

static void tweet_list_update_age(TweetList *tweet_list, gint delete_older_then){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(!this->total) return;
	
	gint 		created_ago=0;
	gchar		*created_at_str=NULL, *created_how_long_ago=NULL;
	OnlineService	*service=NULL;
	
	for(gint i=0; i<=this->total; i++){
		created_ago=0;
		created_at_str=NULL;
		created_how_long_ago=NULL;
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter(this->tree_model, iter, path))){
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gtk_tree_model_get(
					this->tree_model, iter,
						STRING_CREATED_AT, &created_at_str,
						ONLINE_SERVICE, &service,
					-1
		);
		
		created_how_long_ago=parser_convert_time(created_at_str, &created_ago);
		if(delete_older_then > 0 && created_ago > 0 && created_ago > delete_older_then){
			debug( "Removing <%s>'s expired %s.  Oldest %s allowed: [%i] it was posted %i.", service->guid, this->monitoring_string, this->monitoring_string, delete_older_then, created_ago );
			gtk_list_store_remove(this->list_store, iter);
		}else
			gtk_list_store_set(
					this->list_store, iter,
						STRING_CREATED_AGO, created_how_long_ago,
							/*(seconds|minutes|hours|day) ago.*/
						GINT_CREATED_AGO, created_ago,
							/*How old the post is, in seconds, for sorting.*/
					-1
			);
		uber_free(created_how_long_ago);
		uber_free(created_at_str);
		gtk_tree_path_free(path);
		uber_free(iter);
		service=NULL;
	}
}/*tweet_list_update_age(tweet_list, 0);*/

static void tweet_list_refresh_clicked(GtkButton *tweet_list_refresh_tool_button, TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(this->has_loaded < 2) this->has_loaded=2;
	tweet_list_clear(tweet_list);
	tweet_list_refresh(tweet_list);
}/*tweet_list_refresh_clicked(tweet_list);*/

gboolean tweet_list_refresh(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return FALSE;
	tweet_list_stop(tweet_list);
	tweet_list_start(tweet_list);
	return FALSE;
}/*tweet_list_refresh(tweet_list);*/

void tweet_list_complete(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(this->loading) this->loading=FALSE;
	if(!this->connected_online_services)	return;
	
	tweet_list_set_selected_index(tweet_list);	
	if(gconfig_if_bool( SCROLL_TO_TOP_WITH_NEW_UPDATES, TRUE ))
		tweet_list_scroll_to_top(tweet_list);
	
	gtk_progress_bar_set_fraction(this->progress_bar, 1.0);
}/*tweet_list_complete(tweet_list);*/

static void tweet_list_stop_toggle_tool_button_toggled(GtkToggleToolButton *stop_toggle_tool_button, TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	tweet_list_stop_toggle_setup(tweet_list);
	if(GET_PRIVATE(tweet_list)->has_loaded)
		tweet_list_refresh(tweet_list);
}/*tweet_list_stop_toggle_tool_button_toggled(stop_toggle_tool_button, tweet_list);*/

static void tweet_list_stop_toggle_setup(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(!gtk_toggle_tool_button_get_active(this->stop_toggle_tool_button)){
		gtk_tool_button_set_label(GTK_TOOL_BUTTON(this->stop_toggle_tool_button), "<span weight=\"bold\">Disable auto_-update.</span>");
		gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(this->stop_toggle_tool_button), "gtk-media-pause");
		gtk_widget_set_tooltip_markup(GTK_WIDGET(this->stop_toggle_tool_button), "<span weight=\"bold\">Stop auto-reloading these updates.</span>");
	}else{
		gtk_tool_button_set_label(GTK_TOOL_BUTTON(this->stop_toggle_tool_button), "<span weight=\"light\">Enable auto_-update.</span>");
		gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(this->stop_toggle_tool_button), "gtk-media-play");
		gtk_widget_set_tooltip_markup(GTK_WIDGET(this->stop_toggle_tool_button), "<span weight=\"bold\">Start auto-reloading these updates.</span>");
	}
}/*tweet_list_stop_toggle_setup(tweet_list);*/

void tweet_list_stop(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(this->timeout_id && G_STR_N_EMPTY(this->timeline))
		program_timeout_remove(&this->timeout_id, g_strrstr(this->timeline, "/"));
}/*tweet_list_stop(tweet_list);*/

static void tweet_list_close(GtkToolButton *tweet_list_close_tool_button, TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	tweet_lists_close_page(this->page);
}/*tweet_list_close(tweet_list_close_tool_button, tweet_list);*/

static void tweet_list_set_timeline_label(TweetList *tweet_list, const gchar *timeline){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) && G_STR_N_EMPTY(timeline) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	TimelineLabels *timeline_labels=NULL;
	this->timeline=g_strdup(timeline);
	for(timeline_labels=TimelineLabelsList; timeline_labels->timeline; timeline_labels++){
		if(g_str_has_prefix(this->timeline, timeline_labels->timeline) || g_str_equal(this->timeline, timeline_labels->timeline) ){
			this->monitoring=timeline_labels->monitoring;
			this->tab_label_string=g_strdup(timeline_labels->tab_label_string);
			this->menu_label_string=g_strdup(timeline_labels->menu_label_string);
			break;
		}
		
		if(timeline_labels->monitoring!=BestFriends && timeline_labels->monitoring!=Users )
			continue;
		
		
		if(!g_str_has_prefix(this->timeline, "/statuses/user_timeline/"))
			continue;
		
		/* This checks for 'monitoring of 'BestFriends' updates 1st & than 'Users' updates but no others. */
		gchar **feed_info=g_strsplit_set(timeline, "/.", -1);
		gchar **user_info=g_strsplit_set(feed_info[3], ".", -1);
		const gchar *user_name=user_info[0];
		if(G_STR_EMPTY(user_name)){
			g_strfreev(feed_info);
			g_strfreev(user_info);
			continue;
		}
		
		this->user=g_strdup(user_name);
		gboolean user_is_best_friend=online_services_is_user_best_friend(this->service, this->user);
		gboolean viewing_newest_updates=TRUE;
		if(!g_strrstr(this->timeline, "?since_id="))
			viewing_newest_updates=FALSE;
		if(!(this->service && viewing_newest_updates && user_is_best_friend ))
			timeline_labels++;
		
		this->timeline=g_strdup_printf(timeline_labels->timeline, this->user);
		this->tab_label_string=g_strdup_printf(timeline_labels->tab_label_string, (user_is_best_friend?"My Best Friend: " :""), this->user, (viewing_newest_updates?"Newest ":""), this->service->uri );
		this->menu_label_string=g_strdup_printf(timeline_labels->menu_label_string, (user_is_best_friend?"My Best Friend: " :""), this->user, (viewing_newest_updates?"Newest ":""), this->service->uri );
		this->monitoring=timeline_labels->monitoring;
		g_strfreev(feed_info);
		g_strfreev(user_info);
		break;
	}
	if(!this->menu_label_string){
		debug("**ERROR:** Unable to determine timeline label to use.");
		this->tab_label_string=g_strdup(timeline_labels->tab_label_string);
		this->menu_label_string=g_strdup(timeline_labels->menu_label_string);
	}
	tweet_list_mark_as_read(tweet_list);
	if( (this->monitoring==Archive) || (this->monitoring==Users) || (this->monitoring==Faves) )
		gtk_toggle_tool_button_set_active(this->stop_toggle_tool_button, TRUE);
	else
		gtk_toggle_tool_button_set_active(this->stop_toggle_tool_button, FALSE);
	
	this->monitoring_string=monitoring_to_string(this->monitoring);
}/*tweet_list_set_timeline_label(tweet_list, timeline);*/

guint tweet_list_increment(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return -1;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	gtk_progress_bar_pulse(this->progress_bar);
	if(this->index) this->index++;
	return (this->total++);
}/*tweet_list_increment(tweet_list);*/

static void tweet_list_setup(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	GtkBuilder *gtk_builder_ui=gtkbuilder_get_file(
							GtkBuilderUI,
								"tweet_list_tab_label", &this->tab_label,
								
								"tweet_list_menu_label", &this->menu_label,
								
								"tweet_list_vbox", &this->vbox,
								"tweet_list_hbox", &this->hbox,
								
								"tweet_list_handlebox", &this->handlebox,
								"tweet_list_toolbar", &this->toolbar,
								"tweet_list_refresh_tool_button", &this->refresh_tool_button,
								"tweet_list_close_tool_button", &this->close_tool_button,

								"tweet_list_progress_bar_custom_tool_button", &this->progress_bar_custom_tool_button,
								"tweet_list_progress_bar", &this->progress_bar,
								"tweet_list_progress_bar_adjustment", &this->progress_bar_adjustment,
								
								"tweet_list_max_tweets_spin_button", &this->max_tweets_spin_button,
								"tweet_list_max_tweets_adjustment", &this->max_tweets_adjustment,
								
								"tweet_list_stop_toggle_tool_button", &this->stop_toggle_tool_button,
								
								"tweet_list_scrolled_window", &this->scrolled_window,
								
								"tweet_list_hseparator", &this->hseparator,
								
								/* These make up the core of TweetView */
								"tweet_list_sexy_tree_view", &this->sexy_tree_view,
								
								"tweet_list_list_store", &this->list_store,
								
								"tweet_list_pixbuf_avatar_tree_view_column", &this->pixbuf_avatar_tree_view_column,
								"tweet_list_pixbuf_avatar_cell_renderer_pixbuf", &this->pixbuf_avatar_cell_renderer_pixbuf,
								
								"tweet_list_string_tweet_tree_view_column", &this->string_tweet_tree_view_column,
								"tweet_list_string_tweet_cell_renderer_text", &this->string_tweet_cell_renderer_text,
								
								"tweet_list_string_from_tree_view_column", &this->string_from_tree_view_column,
								"tweet_list_string_from_cell_renderer_text", &this->string_from_cell_renderer_text,
								
								"tweet_list_string_rcpt_tree_view_column", &this->string_rcpt_tree_view_column,
								"tweet_list_string_rcpt_cell_renderer_text", &this->string_rcpt_cell_renderer_text,
								
								"tweet_list_created_ago_str_tree_view_column", &this->created_ago_str_tree_view_column,
								"tweet_list_created_ago_str_cell_renderer_text", &this->created_ago_str_cell_renderer_text,
							NULL
	);
	
	gtkbuilder_connect(gtk_builder_ui, tweet_list,
								"tweet_list_refresh_tool_button", "clicked", tweet_list_refresh_clicked,
								"tweet_list_stop_toggle_tool_button", "toggled", tweet_list_stop_toggle_tool_button_toggled,
								"tweet_list_close_tool_button", "clicked", tweet_list_close,
								
								"tweet_list_scrolled_window", "grab-focus", tweet_list_grab_focus_cb,
								"tweet_list_scrolled_window", "size-allocate", tweet_list_size_cb,
								
								"tweet_list_sexy_tree_view", "grab-focus", tweet_list_grab_focus_cb,
								
								"tweet_list_sexy_tree_view", "get-tooltip", tweet_list_set_sexy_tooltip,
								
								"tweet_list_sexy_tree_view", "cursor-changed", tweet_list_changed_cb,
								"tweet_list_sexy_tree_view", "size-allocate", tweet_list_size_cb,
								"tweet_list_sexy_tree_view", "key-press-event", tweets_hotkey,
							NULL
	);
	
	this->tree_model=GTK_TREE_MODEL(this->list_store);
	this->tree_model_sort=gtk_tree_model_sort_new_with_model(this->tree_model);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(this->tree_model_sort), GINT_CREATED_AGO, GTK_SORT_ASCENDING);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(this->sexy_tree_view), this->tree_model_sort);
	sexy_tree_view_set_tooltip_label_column(this->sexy_tree_view, STRING_SEXY_TWEET);
	
	tweet_list_set_adjustment(tweet_list);
}/*tweet_list_setup(tweet_list);*/

void tweet_list_key_pressed(TweetList *tweet_list, GdkEventKey *event){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	if(event->keyval!=GDK_Return && event->keyval!=GDK_KP_Enter) return tweet_list_move(tweet_list, event);
	
	switch(event->state){
		case GDK_CONTROL_MASK:
			control_panel_sexy_insert_char('\n');
			return;
		case GDK_MOD1_MASK:
			online_service_request_selected_update_retweet();
			return;
		case GDK_SHIFT_MASK:
			control_panel_new_dm();
			return;
		default:
			online_service_request_selected_update_reply();
			return;
	}//switch
}/*tweet_list_key_pressed(widget, event);*/

static void tweet_list_move(TweetList *tweet_list, GdkEventKey *event){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	switch(event->keyval){
		case GDK_Tab: case GDK_KP_Tab:
			this->index=0;
			tweet_list_index_select(tweet_list);
			return;
		case GDK_Home: case GDK_KP_Home:
		case GDK_Begin: case GDK_Escape:
			this->index=0;
			break;
		case GDK_End: case GDK_KP_End:
			this->index=this->total-1;
			break;
		case GDK_Up: case GDK_KP_Up: case GDK_KP_Prior:
			this->index--;
			break;
		case GDK_Down: case GDK_KP_Down: case GDK_KP_Next:
			this->index++;
			break;
		case GDK_Page_Up:
			this->index-=3; break;
		case GDK_Page_Down:
			this->index+=3; break;
		default:
			return;
	}//switch
	
	switch(event->state){
		case GDK_CONTROL_MASK|GDK_MOD1_MASK:
			tweet_list_index_select(tweet_list);
			return;
		default:
			tweet_list_index_scroll_to(tweet_list);
			return;
	}
}/* tweet_list_move(tweet_list, event); */

static void tweet_list_index_validate(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(this->index<0) {
		control_panel_beep();
		this->index=0;
	}else if(this->index>=this->total){
		control_panel_beep();
		this->index=this->total-1;
	}
}/*tweet_list_index_validate(tweet_list);*/

static void tweet_list_index_select(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	tweet_list_index_validate(tweet_list);
	
	debug("Selecting update %d, out of %d total updates.", this->index, this->total);
	
	GtkTreePath *path=gtk_tree_path_new_from_indices(this->index, -1);
	if(GTK_IS_TREE_VIEW(GTK_TREE_VIEW(this->sexy_tree_view)))
		gtk_tree_view_set_cursor(GTK_TREE_VIEW(this->sexy_tree_view), path, NULL, FALSE);
	gtk_tree_path_free(path);
	
	tweet_list_mark_as_read(tweet_list);
	
	control_panel_sexy_select();
}/*tweet_list_index_select(tweet_list);*/

static void tweet_list_index_scroll_to(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	tweet_list_index_validate(tweet_list);
	
	debug("Scrolling to update %d, out of %d total updates.", this->index, this->total);
	GtkTreePath *path=gtk_tree_path_new_from_indices(this->index, -1);
	if(GTK_IS_TREE_VIEW(GTK_TREE_VIEW(this->sexy_tree_view)))
		gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(this->sexy_tree_view), path, NULL, FALSE, 0.0, 0.0);
	gtk_tree_path_free(path);
	
	tweet_list_mark_as_read(tweet_list);
	
	control_panel_sexy_select();
}/*tweet_list_index_scroll_to(tweet_list);*/

static void tweet_list_scroll_to_top(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(!( GTK_TREE_VIEW(this->sexy_tree_view) && this->total )) return;
	
	GtkTreePath *path=gtk_tree_path_new_from_indices(0, -1);
	if(GTK_IS_TREE_VIEW(GTK_TREE_VIEW(this->sexy_tree_view)))
		gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(this->sexy_tree_view), path, NULL, FALSE, 0.0, 0.0);
	gtk_tree_path_free(path);
}/*tweet_list_scroll_to_top(tweet_list);*/

static void tweet_list_clear(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	debug("Re-setting tweet_list_index.");
	gtk_list_store_clear(this->list_store);
	gtk_progress_bar_set_fraction(this->progress_bar, 1.0);
	this->has_loaded=0;
	if(this->index){
		this->index=0;
		tweet_list_scroll_to_top(tweet_list);
	}
	this->total=0;
}/*tweet_list_clear(tweet_list);*/

static void tweet_list_grab_focus_cb(GtkWidget *widget, TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	tweet_list_mark_as_read(tweet_list);
}/*tweet_list_grab_focus_cb(widget, event, tweet_list);*/

void tweet_list_toggle_toolbar(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	gtk_widget_toggle_visibility( GTK_WIDGET( GET_PRIVATE(tweet_list)->handlebox ) );
}/*tweet_list_toggle_toolbar();*/

void tweet_list_mark_as_read(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(!this->unread)	return;
	
	this->unread=FALSE;
	
	gtk_label_set_markup_with_mnemonic(this->tab_label, this->tab_label_string);
	gtk_label_set_markup_with_mnemonic(this->menu_label, this->menu_label_string);
}/*tweet_list_mark_as_read(tweet_list);*/

void tweet_list_mark_as_unread(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	if(this->unread) return;
	
	this->unread=TRUE;
	
	control_panel_beep();
	
	gchar *label_markup=g_markup_printf_escaped("<span weight=\"ultrabold\">*%s*</span>", this->tab_label_string);
	gtk_label_set_markup_with_mnemonic(this->tab_label, label_markup);
	uber_free(label_markup);
	
	label_markup=g_markup_printf_escaped("<span weight=\"ultrabold\">*%s*</span>", this->menu_label_string);
	gtk_label_set_markup_with_mnemonic(this->menu_label, label_markup);
	uber_free(label_markup);
}/*tweet_list_mark_as_unread(tweet_list);*/

static void tweet_list_set_sexy_tooltip(SexyTreeView *sexy_tree_view, GtkTreePath *path, GtkTreeViewColumn *column, TweetList *tweet_list){
}/*static void tweet_list_set_sexy_tooltip(sexy_tree_view, path, column, tweet_list);*/

static void tweet_list_changed_cb(SexyTreeView *tweet_list_sexy_tree_view, TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	tweet_list_mark_as_read(tweet_list);
	
	GtkTreeSelection	*sel;
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	if(!((sel=gtk_tree_view_get_selection(GTK_TREE_VIEW(this->sexy_tree_view))) && gtk_tree_selection_get_selected(sel, &this->tree_model_sort, iter) )){
		g_free(iter);
		control_panel_sexy_select();
		return;
	}
	
	
	gdouble		tweet_id, user_id;
	OnlineService	*service=NULL;
	GdkPixbuf	*pixbuf;
	gchar		*user_name, *user_nick, *date, *sexy_tweet, *text_tweet;
	
	gtk_tree_model_get(
				GTK_TREE_MODEL(this->tree_model_sort), iter,
					GDOUBLE_TWEET_ID, &tweet_id,
					GDOUBLE_USER_ID, &user_id,
					STRING_NICK, &user_nick,
					STRING_TEXT, &text_tweet,
					STRING_SEXY_TWEET, &sexy_tweet,
					STRING_CREATED_AGO, &date,
					STRING_USER, &user_name,
					PIXBUF_AVATAR, &pixbuf,
					ONLINE_SERVICE, &service,
					GUINT_SELECTED_INDEX, &this->index,
				-1
	);
	
	debug("Displaying tweet: #%d, update ID: %f from <%s>.", this->index, tweet_id, service->guid);
	
	control_panel_view_selected_update(service, tweet_id, user_id, user_name, user_nick, date, sexy_tweet, text_tweet, pixbuf);
	
	g_free(user_name);
	g_free(sexy_tweet);
	g_free(text_tweet);
	g_free(date);
	g_free(user_nick);
	if(pixbuf) g_object_unref(pixbuf);
	g_free(iter);
	
	control_panel_sexy_select();
}/*tweet_list_changed_cb(tweet_list_sexy_tree_view, tweet_list);*/

static void tweet_list_size_cb(GtkWidget *widget, GtkAllocation *allocation, TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	g_object_set(this->string_tweet_cell_renderer_text, "wrap-width", ((gtk_tree_view_column_get_width(this->string_tweet_tree_view_column))-10), NULL);
	g_object_set(this->string_rcpt_cell_renderer_text, "wrap-width", ((gtk_tree_view_column_get_width(this->string_rcpt_tree_view_column))-10), NULL);
	g_object_set(this->string_from_cell_renderer_text, "wrap-width", ((gtk_tree_view_column_get_width(this->string_from_tree_view_column))-10), NULL);
	g_object_set(this->created_ago_str_cell_renderer_text, "wrap-width", ((gtk_tree_view_column_get_width(this->created_ago_str_tree_view_column))-10), NULL);
}/*tweet_list_size_cb(widget, allocation, tweet_list);*/

void tweet_list_set_image(TweetList *tweet_list, const gchar *image_filename, GtkTreeIter *iter){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPrivate *this=GET_PRIVATE(tweet_list);
	
	GdkPixbuf *pixbuf;
	if(!(pixbuf=images_get_pixbuf_from_filename((gchar *)image_filename)))
		return;
	
	debug("Adding image: '%s' to control_panel.", image_filename);
	gtk_list_store_set(this->list_store, iter, PIXBUF_AVATAR, pixbuf, -1);
	g_object_unref(pixbuf);
}/* tweet_list_set_image */


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

