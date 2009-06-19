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


/********************************************************************************
 *      Project, system & library headers, eg #include <gdk/gdkkeysyms.h>       *
 ********************************************************************************/
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <libsexy/sexy.h>

#include "config.h"
#include "main.h"
#include "tweet-list.h"

#include "images.h"

#include "online-services-typedefs.h"
#include "online-service-request.h"
#include "network.h"

#include "gtkbuilder.h"
#include "gconfig.h"
#include "preferences.h"

#include "label.h"
#include "main-window.h"

#include "tweets.h"
#include "tweet-view.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
#define	GET_PRIV(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_TWEET_LIST, TweetListPriv))

typedef struct _TimelineLabels TimelineLabels;

struct _TimelineLabels{
	TweetLists	tweet_list;
	const gchar	*timeline;
	const gchar	*timeline_tab_label;
	const gchar	*timeline_menu_label;
};

TimelineLabels TimelineLabelsList[]={
	{Tweets,	API_TIMELINE_FRIENDS,	N_("My _Friends' Updates"),	N_("My_Friends' Updates")},
	{Replies,	API_REPLIES,		N_("@ _Replies"),		N_("@ Replies")},
	{Replies,	API_MENTIONS,		N_("@ _Mentions"),		N_("@ Mentions")},
	{DMs,		API_DIRECT_MESSAGES,	N_("My _DMs Inbox"),		N_("My DMs Inbox")},
	{Timelines,	API_TIMELINE_PUBLIC,	N_("Public Updates"),		N_("Public Updates")},
	{Users,		API_TIMELINE_USER,	N_("%s_'s Updates"),		N_("%s's Updates")},
	{Archive,	API_FAVORITES,		N_("My _Star'd Updates"),	N_("My Star'd Updates")},
	{Archive,	API_TIMELINE_MINE,	N_("_My Tweets"),		N_("My Tweets")},
	{None,		NULL,			N_("Unknown timeline"),		N_("Unknown timeline")}
};

struct _TweetListPriv {
	guint			timeout_id;
	gint			page;
	
	TweetLists		tweet_list;
	gchar			*timeline;
	gchar			*timeline_tab_label;
	gchar			*timeline_menu_label;
	
	gint			index;
	guint			total;
	
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
	
	GtkTreeView		*timeline_tree_view;
	
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

#define GtkBuilderUI "tweet-list.ui"

#define	MINIMUM_TWEETS	20.00
#define	MAXIMUM_TWEETS	100.00


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static void tweet_list_class_init(TweetListClass *klass);
static void tweet_list_init(TweetList *tweet);
static void tweet_list_finalize(GObject *object);

static void tweet_list_setup(TweetList *tweet_list);
static void tweet_list_setup(TweetList *tweet_list);

static void tweet_list_set_timeline_label(TweetList *tweet_list, const gchar *timeline);

static void tweet_list_stop_toggled(GtkToggleToolButton *tweet_list_stop_toggle_tool_button, TweetList *tweet_list);
static void tweet_list_refresh_clicked(GtkButton *tweet_list_refresh_tool_button, TweetList *tweet_list);

static void tweet_list_focus_in_event_cb(GtkWidget *widget, GdkEventFocus *event, TweetList *tweet_list);
static void tweet_list_size_cb(GtkWidget *widget, GtkAllocation *allocation, TweetList *tweet_list);
static void tweet_list_changed_cb(GtkTreeView *tweet_list_tree_view, TweetList *tweet_list);
static void tweet_list_clear(TweetList *tweet_list);

static void tweet_list_move(TweetList *tweet_list, GdkEventKey *event);
static void tweet_list_goto_index(TweetList *tweet_list);
static void tweet_list_scroll_to_top(TweetList *tweet_list);


G_DEFINE_TYPE(TweetList, tweet_list, SEXY_TYPE_TREE_VIEW);


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
/* BEGIN: GObject core methods. */
static void tweet_list_class_init(TweetListClass *klass){
	GObjectClass	*object_class=G_OBJECT_CLASS(klass);
	
	object_class->finalize=tweet_list_finalize;
	
	g_type_class_add_private(object_class, sizeof(TweetListPriv));
}/* tweet_list_class_init */

static void tweet_list_init(TweetList *tweet_list){
	TweetListPriv *this=GET_PRIV(tweet_list);
		
	tweet_list_setup(tweet_list);
	this->index=this->total=0;
	g_object_set(tweet_list, "rules-hint", TRUE, "reorderable", TRUE, "headers-visible", FALSE, NULL);
	g_signal_connect(tweet_list, "size-allocate", G_CALLBACK(tweet_list_size_cb), tweet_list);
	g_signal_connect(this->timeline_tree_view, "size-allocate", G_CALLBACK(tweet_list_size_cb), tweet_list);
	g_signal_connect(tweet_list, "cursor-changed", G_CALLBACK(tweet_list_changed_cb), tweet_list);
	g_signal_connect(tweet_list, "row-activated", G_CALLBACK(selected_tweet_reply), tweet_list);
	g_signal_connect(tweet_list, "key-press-event", G_CALLBACK(tweets_hotkey), tweet_list);
}/* tweet_list_init */

TweetList *tweet_list_new(const gchar *timeline){
	TweetList *tweet_list=g_object_new(TYPE_TWEET_LIST, NULL);
	debug("Creating new TweetView for timeline: %s.", timeline);
	tweet_list_set_timeline_label(tweet_list, timeline);
	tweet_list_start(tweet_list);
	return tweet_list;
}/*tweet_list_new(timeline);*/

static void tweet_list_finalize( GObject *object ){
	TweetList *tweet_list=TWEET_LIST(object);
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	program_timeout_remove(&this->timeout_id, g_strrstr(this->timeline, "/"));
	
	if(this->timeline) uber_free(this->timeline);
	if(this->timeline_tab_label) uber_free(this->timeline_tab_label);
	if(this->timeline_menu_label) uber_free(this->timeline_menu_label);

	if(this->tree_model_sort) g_object_unref(this->tree_model_sort);
	if(this->vbox) gtk_widget_destroy(GTK_WIDGET(this->vbox));
}/* tweet_list_finalized */


/*BEGIN: Custom TweetList methods.*/
const gchar *tweet_list_get_timeline(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIV(tweet_list)->timeline;
}/*tweet_list_get_timeline(tweet_list);*/

GtkVBox *tweet_list_get_child(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIV(tweet_list)->vbox;
}/*tweet_list_get_child(tweet_list);*/

GtkLabel *tweet_list_get_tab(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIV(tweet_list)->tab_label;
}/*tweet_list_get_label(TweetList *tweet_list);*/

GtkLabel *tweet_list_get_menu(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIV(tweet_list)->menu_label;
}/*tweet_list_get_label(TweetList *tweet_list);*/

GtkListStore *tweet_list_get_list_store(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIV(tweet_list)->list_store;
}/*tweet_list_get_list_store(tweet_list);*/

GtkTreeModel *tweet_list_get_tree_model(TweetList *tweet_list){
	if(!( tweet_list && IS_TWEET_LIST(tweet_list) )) return NULL;
	return GET_PRIV(tweet_list)->tree_model;
}/*tweet_list_get_tree_model(tweet_list);*/

gint tweet_list_get_page(TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) ))	return -1;
	return GET_PRIV(tweet_list)->page;
}/*tweet_list_get_page(tweet_list);*/

gint tweet_list_get_total(TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) ))	return 0;
	return GET_PRIV(tweet_list)->total;
}/*tweet_list_get_total(tweet_list);*/

void tweet_list_set_page(TweetList *tweet_list, gint page){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	GET_PRIV(tweet_list)->page=page;
}/*tweet_list_set_page(tweet_list, 0);*/

void tweet_list_start(TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	gint minutes=0;
	gconfig_get_int(PREFS_TWEETS_RELOAD_TIMELINES, &minutes);
	/* With multiple timeline support timeline reloading interval shouldn't be less than 5 minutes */
	if(minutes < 5) minutes=5;
	switch(this->tweet_list){
		case None:	default:
			debug("unsupport tweet-list type");
			return;
		case Archive:	case Users:	case Timelines:
			if(minutes<15) minutes=15;
			else minutes+=6;
			break;
		case Replies:
			minutes+=2;
			break;
		case DMs:
			minutes+=8;
			break;
		case Tweets:
			break;
	}
	
	if(!gtk_toggle_tool_button_get_active(this->stop_toggle_tool_button)){
		debug("Creating timeout to reload %s.", this->timeline_menu_label);
		guint reload=minutes*60*1000;
		this->timeout_id=g_timeout_add(reload, (GSourceFunc)tweet_list_refresh, tweet_list);
	}
	if(this->total) tweet_list_scroll_to_top(tweet_list);
	
	gtk_progress_bar_set_fraction(this->progress_bar, 0.0);
	
	online_services_request(online_services, QUEUE, this->timeline, NULL, network_display_timeline, tweet_list, (gpointer)this->tweet_list);
}/*tweet_list_start(TweetList *tweet_list);*/

static void tweet_list_refresh_clicked(GtkButton *tweet_list_refresh_tool_button, TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	if(this->total) online_services_update_ids_reset(online_services);
	tweet_list_clear(tweet_list);
	tweet_list_refresh(tweet_list);
}/*tweet_list_refresh_clicked(tweet_list);*/

gboolean tweet_list_refresh(TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return FALSE;
	tweet_list_stop(tweet_list);
	tweet_list_start(tweet_list);
	return FALSE;
}/*tweet_list_refresh(tweet_list);*/

void tweet_list_complete(TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	gtk_progress_bar_set_fraction(this->progress_bar, 1.0);
}/*tweet_list_complete(tweet_list);*/

static void tweet_list_stop_toggled(GtkToggleToolButton *tweet_list_stop_toggle_tool_button, TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	tweet_list_stop(tweet_list);
	if(gtk_toggle_tool_button_get_active(this->stop_toggle_tool_button))
		tweet_list_start(tweet_list);
}/*tweet_list_auto_refresh_toggle(tweet_list_stop_toggle_tool_button, tweet_list);*/

void tweet_list_stop(TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	program_timeout_remove(&this->timeout_id, g_strrstr(this->timeline, "/"));
}/*tweet_list_stop(tweet_list);*/

static void tweet_list_close(GtkToolButton *tweet_list_close_tool_button, TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) ))	return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	main_window_tweet_lists_close_page(this->page);
}/*tweet_list_close(tweet_list_close_tool_button, tweet_list);*/

static void tweet_list_set_timeline_label(TweetList *tweet_list, const gchar *timeline){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	TimelineLabels *timeline_labels=TimelineLabelsList;
	while(timeline_labels->timeline){
		if(g_str_has_prefix(timeline, timeline_labels->timeline )){
			this->tweet_list=timeline_labels->tweet_list;
			this->timeline=g_strdup(timeline);
			if(!g_str_has_prefix(timeline, "/statuses/user_timeline/")){
				this->timeline_tab_label=g_strdup(timeline_labels->timeline_tab_label);
				this->timeline_menu_label=g_strdup(timeline_labels->timeline_menu_label);
				break;
			}
			gchar *user=NULL;
			gchar *feed=g_strrstr(timeline, "/");
			feed++;
			g_strlcpy(user, feed, g_utf8_strlen(feed, -1)-4);
			this->timeline_tab_label=g_strdup_printf(timeline_labels->timeline_tab_label, user);
			this->timeline_menu_label=g_strdup_printf(timeline_labels->timeline_menu_label, user);
			break;
			gchar **feed_data=g_strsplit_set(timeline, "/.", -1);
			this->timeline_tab_label=g_strdup_printf(timeline_labels->timeline_tab_label, feed_data[2]);
			this->timeline_menu_label=g_strdup_printf(timeline_labels->timeline_menu_label, feed_data[2]);
			g_strfreev(feed_data);
			break;
		}
		timeline_labels++;
	}
	if(!this->timeline_menu_label){
		debug("**ERROR: Unable to determine timeline label to use**");
		this->timeline_tab_label=g_strdup(timeline_labels->timeline_tab_label);
		this->timeline_menu_label=g_strdup(timeline_labels->timeline_menu_label);
	}
	gtk_label_set_markup_with_mnemonic(this->tab_label, this->timeline_tab_label);
	gtk_label_set_markup_with_mnemonic(this->menu_label, this->timeline_menu_label);
	if( (this->tweet_list==Archive) || (this->tweet_list==Users) )
		gtk_toggle_tool_button_set_active(this->stop_toggle_tool_button, FALSE);
}/*tweet_list_set_timeline_label(timeline, tweet_list);*/

void tweet_list_increment(TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	gchar *label_markup=g_markup_printf_escaped("<span weight=\"ultrabold\">*%s*</span>", this->timeline_tab_label);
	gtk_label_set_markup_with_mnemonic(this->tab_label, label_markup);
	uber_free(label_markup);
	
	label_markup=g_markup_printf_escaped("<span weight=\"ultrabold\">*%s*</span>", this->timeline_menu_label);
	gtk_label_set_markup_with_mnemonic(this->menu_label, label_markup);
	uber_free(label_markup);
	
	gdouble max_updates=gtk_adjustment_get_value(this->max_tweets_adjustment);
	if(max_updates < MINIMUM_TWEETS)
		max_updates=MINIMUM_TWEETS;
	else if(max_updates > MAXIMUM_TWEETS)
		max_updates=MAXIMUM_TWEETS;
	gtk_progress_bar_pulse(this->progress_bar);
	if(this->total <= max_updates){
		this->total++;
		return;
	}
	this->total++;
	return;
	
	for(gint i=this->total; i>=max_updates; i--){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		if( (gtk_tree_model_iter_nth_child(this->tree_model, iter, NULL, i)) ){
			gtk_list_store_remove(this->list_store, iter);
			if(i<this->total) this->total--;
		}
		uber_free(iter);
	}
}/*tweet_list_increment(tweet_list);*/

static void tweet_list_setup(TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
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
								"tweet_list_timeline_tree_view", &this->timeline_tree_view,
								
								"tweet_list_list_store", &this->list_store,
								
								"tweet_list_pixbuf_avatar_tree_view_column", &this->pixbuf_avatar_tree_view_column,
								"tweet_list_pixbuf_avatar_cell_renderer_pixbuf", &this->pixbuf_avatar_cell_renderer_pixbuf,
								
								"tweet_list_string_tweet_tree_view_column", &this->string_tweet_tree_view_column,
								"tweet_list_string_tweet_cell_renderer_text", &this->string_tweet_cell_renderer_text,
								
								"tweet_list_created_ago_str_tree_view_column", &this->created_ago_str_tree_view_column,
								"tweet_list_created_ago_str_cell_renderer_text", &this->created_ago_str_cell_renderer_text,
								
								"tweet_list_string_from_tree_view_column", &this->string_from_tree_view_column,
								"tweet_list_string_from_cell_renderer_text", &this->string_from_cell_renderer_text,
								
								"tweet_list_string_rcpt_tree_view_column", &this->string_rcpt_tree_view_column,
								"tweet_list_string_rcpt_cell_renderer_text", &this->string_rcpt_cell_renderer_text,
								
							NULL
	);
	
	gtkbuilder_connect(gtk_builder_ui, tweet_list,
								"tweet_list_refresh_tool_button", "clicked", tweet_list_refresh_clicked,
								"tweet_list_stop_toggle_tool_button", "toggled", tweet_list_stop_toggled,
								"tweet_list_close_tool_button", "clicked", tweet_list_close,
								
								"tweet_list_tab_label", "button-press-event", tweet_list_focus_in_event_cb,
								"tweet_list_tab_label", "focus-in-event", tweet_list_focus_in_event_cb,
								"tweet_list_menu_label", "focus-in-event", tweet_list_focus_in_event_cb,
								
								"tweet_list_scrolled_window", "focus-in-event", tweet_list_focus_in_event_cb,
								"tweet_list_scrolled_window", "size-allocate", tweet_list_size_cb,
								
								"tweet_list_timeline_tree_view", "cursor-changed", tweet_list_changed_cb,
								"tweet_list_timeline_tree_view", "size-allocate", tweet_list_size_cb,
								"tweet_list_timeline_tree_view", "row-activated", selected_tweet_reply,
								"tweet_list_timeline_tree_view", "key-press-event", tweets_hotkey,
			NULL
	);
	
	this->tree_model=GTK_TREE_MODEL(this->list_store);
	this->tree_model_sort=gtk_tree_model_sort_new_with_model(this->tree_model);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(this->tree_model_sort), ULONG_CREATED_AT, GTK_SORT_ASCENDING);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(tweet_list), this->tree_model_sort);
	gtk_tree_view_set_model(this->timeline_tree_view, this->tree_model_sort);
	sexy_tree_view_set_tooltip_label_column( SEXY_TREE_VIEW(tweet_list), STRING_SEXY_TWEET);
	
	gtk_adjustment_set_value(this->max_tweets_adjustment, MINIMUM_TWEETS);
	gtk_spin_button_set_value(this->max_tweets_spin_button, MINIMUM_TWEETS);
	
	gtk_widget_show_all(GTK_WIDGET(this->vbox));
	gtk_widget_show_all(GTK_WIDGET(tweet_list));
}/*tweet_list_setup(tweet_list);*/

void tweet_list_key_pressed(TweetList *tweet_list, GdkEventKey *event){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	if(event->keyval!=GDK_Return && event->keyval!=GDK_KP_Enter) return tweet_list_move(tweet_list, event);
	
	switch(event->state){
		case GDK_CONTROL_MASK:
			tweet_view_sexy_insert_char('\n');
			return;
		case GDK_MOD1_MASK:
			selected_tweet_retweet();
			return;
		case GDK_SHIFT_MASK:
			tweets_new_dm();
			return;
		default:
			selected_tweet_reply();
			return;
	}//switch
}/*tweet_list_key_pressed(widget, event);*/

static void tweet_list_move(TweetList *tweet_list, GdkEventKey *event){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	switch(event->keyval){
		case GDK_Tab: case GDK_KP_Tab:
		case GDK_Home: case GDK_KP_Home:
		case GDK_Begin: case GDK_Escape:
			this->index=0;
			break;
		case GDK_Up: case GDK_KP_Up: case GDK_KP_Prior:
			this->index--;
			break;
		case GDK_Down: case GDK_KP_Down: case GDK_KP_Next:
			this->index++;
			break;
		case GDK_End: case GDK_KP_End:
			this->index=this->total-1;
			break;
		case GDK_Page_Up:
			this->index-=9; break;
		case GDK_Page_Down:
			this->index+=9; break;
		default:
			return;
	}//switch
	
	tweet_list_goto_index(tweet_list);
}/* tweet_list_move */

static void tweet_list_goto_index(TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	if(this->index<0) {
		tweets_beep();
		this->index=0;
	}else if(this->index>=this->total){
		tweets_beep();
		this->index=this->total-1;
	}
	
	debug("Selecting tweet %d, maximum tweets are: %d.", this->index, this->total);
	GtkTreePath *path=gtk_tree_path_new_from_indices(this->index, -1);
	/*gtk_tree_view_set_cursor( GTK_TREE_VIEW(tweet_list), path, NULL, FALSE );*/
	gtk_tree_view_set_cursor(this->timeline_tree_view, path, NULL, FALSE);
	gtk_tree_path_free(path);
	
	tweet_view_sexy_select();
}/*tweet_list_goto_index();*/

static void tweet_list_scroll_to_top(TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
		
	if(!(GTK_TREE_VIEW(this->timeline_tree_view))){
		debug("**ERROR:** TweetList cannot be cast to GtkTreeView.  Unable to move to top.");
		return;
	}
	GtkTreePath *path=gtk_tree_path_new_from_indices(0, -1);
	if(GTK_IS_TREE_VIEW(GTK_TREE_VIEW(this->timeline_tree_view)))
		gtk_tree_view_scroll_to_cell(this->timeline_tree_view, path, NULL, FALSE, 0.0, 0.0);
	gtk_tree_path_free(path);
}/* tweet_list_scroll_to_top */

static void tweet_list_clear(TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	debug("Re-setting tweet_list_index.");
	gtk_list_store_clear(this->list_store);
	this->index=0;
	this->total=0;
}/* tweet_list_refreshed */

static void tweet_list_focus_in_event_cb(GtkWidget *widget, GdkEventFocus *event, TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	gtk_label_set_markup_with_mnemonic(this->tab_label, this->timeline_tab_label);
	gtk_label_set_markup_with_mnemonic(this->menu_label, this->timeline_menu_label);
}/*tweet_list_focus_in_event_cb(widget, event, tweet_list);*/

static void tweet_list_changed_cb(GtkTreeView *tweet_list_tree_view, TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	GtkTreeSelection	*sel;
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	if(!((sel=gtk_tree_view_get_selection(this->timeline_tree_view)) && gtk_tree_selection_get_selected(sel, &this->tree_model_sort, iter) )){
		g_free(iter);
		tweet_view_sexy_select();
		return;
	}
	
	
	main_window_set_statusbar_msg(NULL);
	
	gulong		tweet_id, user_id;
	OnlineService	*service=NULL;
	GdkPixbuf	*pixbuf;
	gchar		*user_name, *user_nick, *date, *sexy_tweet, *text_tweet;

	gtk_tree_model_get(
				GTK_TREE_MODEL(this->tree_model_sort), iter,
					ULONG_TWEET_ID, &tweet_id,
					ULONG_USER_ID, &user_id,
					STRING_NICK, &user_nick,
					STRING_TWEET, &text_tweet,
					STRING_SEXY_TWEET, &sexy_tweet,
					STRING_CREATED_AGO, &date,
					STRING_USER, &user_name,
					PIXBUF_AVATAR, &pixbuf,
					ONLINE_SERVICE, &service,
				-1
	);
	
	debug("Displaying tweet: #%lu from '%s'.", tweet_id, online_service_get_key(service));
	tweet_view_show_tweet(service, tweet_id, user_id, user_name, user_nick, date, sexy_tweet, text_tweet, pixbuf);
	
	g_free(user_name);
	g_free(sexy_tweet);
	g_free(text_tweet);
	g_free(date);
	g_free(user_nick);
	if(pixbuf) g_object_unref(pixbuf);
	g_free(iter);
	
	tweet_view_sexy_select();
}

static void tweet_list_size_cb(GtkWidget *widget, GtkAllocation *allocation, TweetList *tweet_list){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	g_object_set(this->string_tweet_cell_renderer_text, "wrap-width", ((gtk_tree_view_column_get_width(this->string_tweet_tree_view_column))-10), NULL);
}

void tweet_list_set_image(TweetList *tweet_list, const gchar *image_filename, GtkTreeIter *iter){
	if(!(tweet_list && IS_TWEET_LIST(tweet_list) )) return;
	TweetListPriv *this=GET_PRIV(tweet_list);
	
	GdkPixbuf *pixbuf;
	if(!(pixbuf=images_get_pixbuf_from_filename((gchar *)image_filename)))
		return;
	
	debug("Adding image: '%s' to tweet_view.", image_filename);
	gtk_list_store_set(this->list_store , iter, PIXBUF_AVATAR, pixbuf, -1);
	g_object_unref(pixbuf);
}/* tweet_list_set_image */


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

