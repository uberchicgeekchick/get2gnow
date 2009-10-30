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

#include "images.h"

#include "online-services.defines.h"
#include "online-services-typedefs.h"
#include "online-service-request.h"
#include "online-service.types.h"
#include "online-service.h"

#include "best-friends.h"
#include "network.h"

#include "gtkbuilder.h"
#include "gconfig.h"
#include "preferences.h"

#include "label.h"
#include "main-window.h"
#include "tabs.h"

#include "parser.h"
#include "hotkeys.h"
#include "update-viewer.h"

#include "timelines-sexy-tree-view.macros.h"
#include "timelines-sexy-tree-view.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
#define	GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_TIMELINES_SEXY_TREE_VIEW, TimelinesSexyTreeViewPrivate))

typedef struct _TimelineLabels TimelineLabels;

struct _TimelineLabels{
	UpdateMonitor	monitoring;
	const gchar	*timeline;
	const gchar	*tab_label_string;
	const gchar	*menu_label_string;
};

TimelineLabels TimelineLabelsList[]={
	{Updates,	API_TIMELINE_FRIENDS,		N_("My _Friends' Updates"),	N_("My Friends' Updates")},
	{Replies,	API_REPLIES,			N_("@ _Replies"),		N_("@ Replies")},
	/*{Replies,	API_MENTIONS,			N_("@ _Mentions"),		N_("@ Mentions")},*/
	{DMs,		API_DIRECT_MESSAGES,		N_("My DMs _Inbox"),		N_("My DMs Inbox")},
	{BestFriends,	API_TIMELINE_USER,		N_("%s_%s's %s%s Updates"),	N_("%s%s's %s%s Updates")},
	{Users,		API_TIMELINE_USER,		N_("%s_%s's %s%s Updates"),	N_("%s%s's %s%s Updates")},
	{Faves,		API_FAVORITES,			N_("My Star_'d Updates"),	N_("My Star'd Updates")},
	{Searches,	API_TIMELINE_SEARCH,		N_("_Search Results"),		N_("Search Results")},
	{Groups,	API_TIMELINE_GROUP,		N_("_Group Discussions"),	N_("Group Discussions")},
	{Timelines,	API_TIMELINE_PUBLIC,		N_("_Global Updates"),		N_("Public Updates")},
	{Archive,	API_TIMELINE_MINE,		N_("_My Updates"),		N_("My Updates")},
	{None,		NULL,				N_("Unknow_n timeline"),	N_("Unknown timeline")}
};

struct _TimelinesSexyTreeViewPrivate {
	guint			timeout_id;
	gint			page;
	OnlineService		*service;
	gchar			*user;
	
	UpdateMonitor		monitoring;
	gchar			*monitoring_string;
	gint8			has_loaded;
	
	gint			minutes;
	gulong			reload;
	
	gchar			*timeline;
	gchar			*tab_label_string;
	gchar			*menu_label_string;
	
	gint			index;
	
	gint			total;
	gint			list_store_index;
	
	gboolean		unread;
	guint			unread_updates;
	
	guint			connected_online_services;
	gdouble			max_updates;
	gchar			*max_updates_str;
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
	
	GtkSpinButton		*max_updates_spin_button;
	GtkAdjustment		*max_updates_adjustment;
	
	GtkToolButton		*close_tool_button;
	GtkToggleToolButton	*stop_toggle_tool_button;
	
	GtkHSeparator		*hseparator;
	
	/* These make up the core of TweetView */
	GtkScrolledWindow	*scrolled_window;
	
	SexyTreeView		*sexy_tree_view;
	
	GtkTreeViewColumn	*pixbuf_avatar_tree_view_column;
	GtkCellRendererPixbuf	*pixbuf_avatar_cell_renderer_pixbuf;
	
	GtkTreeViewColumn	*string_update_tree_view_column;
	GtkCellRendererText	*string_update_cell_renderer_text;
	
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
#define DEBUG_DOMAINS "OnlineServices:UI:GtkBuilder:GtkBuildable:Networking:Updates:Requests:Notification:Settings:Setup:Start-Up:timelines-sexy-tree-view.c"
#include "debug.h"

#define GtkBuilderUI "timelines-sexy-tree-view"

#define	MINIMUM_UPDATES	20.00
#define	MAXIMUM_UPDATES	100.00


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static void timelines_sexy_tree_view_class_init(TimelinesSexyTreeViewClass *klass);
static void timelines_sexy_tree_view_init(TimelinesSexyTreeView *timelines_sexy_tree_view);
static void timelines_sexy_tree_view_finalize(TimelinesSexyTreeView *timelines_sexy_tree_view);

static void timelines_sexy_tree_view_create_gui(TimelinesSexyTreeView *timelines_sexy_tree_view);
static float timelines_sexy_tree_view_prepare_reload(TimelinesSexyTreeView *timelines_sexy_tree_view);

static void timelines_sexy_tree_view_set_timeline_label(TimelinesSexyTreeView *timelines_sexy_tree_view, const gchar *timeline);

static void timelines_sexy_tree_view_refresh_clicked(GtkButton *timelines_sexy_tree_view_refresh_tool_button, TimelinesSexyTreeView *timelines_sexy_tree_view);
static void timelines_sexy_tree_view_clear(TimelinesSexyTreeView *timelines_sexy_tree_view);

static void timelines_sexy_tree_view_stop_toggle_setup(TimelinesSexyTreeView *timelines_sexy_tree_view);
static void timelines_sexy_tree_view_stop_toggle_tool_button_toggled(GtkToggleToolButton *stop_toggle_tool_button, TimelinesSexyTreeView *timelines_sexy_tree_view);

static void timelines_sexy_tree_view_set_maximum_updates(GtkSpinButton *max_updates_spin_button, TimelinesSexyTreeView *timelines_sexy_tree_view);
static void timelines_sexy_tree_view_set_adjustment(TimelinesSexyTreeView *timelines_sexy_tree_view);

const gchar *timelines_sexy_tree_view_list_store_column_to_string(TimelinesSexyTreeViewListStoreColumn timelines_sexy_tree_view_list_store_column);
static void timelines_sexy_tree_view_modifiy_updates_list_store( TimelinesSexyTreeView *timelines_sexy_tree_view, TimelinesSexyTreeViewListStoreColumn timelines_sexy_tree_view_list_store_column, gpointer value, gboolean list_store_remove );

static gboolean timelines_sexy_tree_view_notification(TimelinesSexyTreeView *timelines_sexy_tree_view);
static void timelines_sexy_tree_view_grab_focus_cb(GtkWidget *widget, TimelinesSexyTreeView *timelines_sexy_tree_view);
static void timelines_sexy_tree_view_increment_unread(TimelinesSexyTreeView *timelines_sexy_tree_view);
static void timelines_sexy_tree_view_mark_as_unread(TimelinesSexyTreeView *timelines_sexy_tree_view);

static void timelines_sexy_tree_view_check_updates(TimelinesSexyTreeView *timelines_sexy_tree_view);
static void timelines_sexy_tree_view_update_age(TimelinesSexyTreeView *timelines_sexy_tree_view, gint expiration);
static void timelines_sexy_tree_view_check_maximum_updates(TimelinesSexyTreeView *timelines_sexy_tree_view);
static void timelines_sexy_tree_view_check_inbox(TimelinesSexyTreeView *timelines_sexy_tree_view);

static void timelines_sexy_tree_view_find_selected_update_index(TimelinesSexyTreeView *timelines_sexy_tree_view, OnlineService *service0, const gchar *user_name, gdouble update_id);

static void timelines_sexy_tree_view_set_sexy_tooltip(SexyTreeView *sexy_tree_view, GtkTreePath *path, GtkTreeViewColumn *column, TimelinesSexyTreeView *timelines_sexy_tree_view);
static void timelines_sexy_tree_view_update_selected(SexyTreeView *timelines_sexy_tree_view_sexy_tree_view, TimelinesSexyTreeView *timelines_sexy_tree_view);
static void timelines_sexy_tree_view_resize_cb(GtkWidget *widget, GtkAllocation *allocation, TimelinesSexyTreeView *timelines_sexy_tree_view);

static void timelines_sexy_tree_view_move(TimelinesSexyTreeView *timelines_sexy_tree_view, GdkEventKey *event);

static gboolean timelines_sexy_tree_view_index_validate(TimelinesSexyTreeView *timelines_sexy_tree_view, gint *index);
static gboolean timelines_sexy_tree_view_goto(TimelinesSexyTreeView *timelines_sexy_tree_view, gint goto_index, gboolean select);
static gboolean timelines_sexy_tree_view_goto_index(TimelinesSexyTreeView *timelines_sexy_tree_view, gboolean select);
static gboolean timelines_sexy_tree_view_index_select(TimelinesSexyTreeView *timelines_sexy_tree_view);
static gboolean timelines_sexy_tree_view_index_scroll_to(TimelinesSexyTreeView *timelines_sexy_tree_view);
static gboolean timelines_sexy_tree_view_scroll_to_top(TimelinesSexyTreeView *timelines_sexy_tree_view);

G_DEFINE_TYPE(TimelinesSexyTreeView, timelines_sexy_tree_view, SEXY_TYPE_TREE_VIEW);


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
/* BEGIN: GObject core methods. */
static void timelines_sexy_tree_view_class_init(TimelinesSexyTreeViewClass *klass){
	GObjectClass	*object_class=G_OBJECT_CLASS(klass);
	
	object_class->finalize=(GObjectFinalizeFunc)timelines_sexy_tree_view_finalize;
	
	g_type_class_add_private(object_class, sizeof(TimelinesSexyTreeViewPrivate));
}/* timelines_sexy_tree_view_class_init */

static void timelines_sexy_tree_view_init(TimelinesSexyTreeView *timelines_sexy_tree_view){
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	this->service=NULL;
	this->user=NULL;
	
	this->has_loaded=-1;
	this->unread_updates=0;
	this->unread=FALSE;
	
	this->minutes=0;
	this->reload=0;
	
	this->connected_online_services=this->timeout_id=this->total=0;
	this->index=-1;
	this->list_store_index=-1;
	this->max_updates_str=gdouble_to_str((this->max_updates=0.0) );
	this->minimum=MINIMUM_UPDATES;
	this->maximum=MAXIMUM_UPDATES;
	
	this->timeline=this->tab_label_string=this->menu_label_string=NULL;
	this->tab_label=this->menu_label=NULL;
	this->vbox=NULL;
	this->tree_model_sort=NULL;
	timelines_sexy_tree_view_create_gui(timelines_sexy_tree_view);
	
	g_object_set(timelines_sexy_tree_view, "rules-hint", TRUE, "reorderable", TRUE, "headers-visible", TRUE, NULL);
	g_signal_connect(timelines_sexy_tree_view, "grab-focus", G_CALLBACK(timelines_sexy_tree_view_grab_focus_cb), timelines_sexy_tree_view);
	g_signal_connect(timelines_sexy_tree_view, "key-press-event", G_CALLBACK(hotkey_pressed), timelines_sexy_tree_view);
}/* timelines_sexy_tree_view_init */

TimelinesSexyTreeView *timelines_sexy_tree_view_new(const gchar *timeline, OnlineService *service){
	if(G_STR_EMPTY(timeline)) {
		debug("**ERROR:** Cannot create update view for an empty timeline.");
		return NULL;
	}
	TimelinesSexyTreeView *timelines_sexy_tree_view=g_object_new(TYPE_TIMELINES_SEXY_TREE_VIEW, NULL);
	
	debug("Creating new TweetView for timeline: %s.", timeline);
	if(service && service->connected )
		GET_PRIVATE(timelines_sexy_tree_view)->service=service;
	
	timelines_sexy_tree_view=g_object_ref_sink(timelines_sexy_tree_view);
	
	timelines_sexy_tree_view_set_timeline_label(timelines_sexy_tree_view, timeline);
	timelines_sexy_tree_view_stop_toggle_setup(timelines_sexy_tree_view);
	
	gtk_widget_show_all(GTK_WIDGET(GET_PRIVATE(timelines_sexy_tree_view)->vbox));
	
	timelines_sexy_tree_view_set_adjustment(timelines_sexy_tree_view);
	
	if(gconfig_if_bool( UPDATE_VEWER_TOOLBAR_VISIBILITY, FALSE) )
		timelines_sexy_tree_view_toggle_toolbar(timelines_sexy_tree_view);
	
	if(gconfig_if_bool( UPDATE_VEWER_FROM_COLUMN_VISIBILITY, FALSE) )
		timelines_sexy_tree_view_toggle_from_column(timelines_sexy_tree_view);
	
	if(gconfig_if_bool( UPDATE_VEWER_RCPT_COLUMN_VISIBILITY, FALSE) )
		timelines_sexy_tree_view_toggle_rcpt_column(timelines_sexy_tree_view);
	
	timelines_sexy_tree_view_start(timelines_sexy_tree_view);
	
	return timelines_sexy_tree_view;
}/*timelines_sexy_tree_view_new(timeline, NULL);*/

static void timelines_sexy_tree_view_finalize(TimelinesSexyTreeView *timelines_sexy_tree_view){
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(this->timeout_id && G_STR_N_EMPTY(this->timeline))
		program_timeout_remove(&this->timeout_id, g_strrstr(this->timeline, "/"));
	
	if(this->user) uber_free(this->user);
	if(this->timeline) uber_free(this->timeline);
	if(this->tab_label_string) uber_free(this->tab_label_string);
	if(this->menu_label_string) uber_free(this->menu_label_string);
	if(this->monitoring_string) uber_free(this->monitoring_string);
	uber_free(this->max_updates_str);
	
	if(this->tab_label) gtk_widget_destroy(GTK_WIDGET(this->tab_label));
	if(this->menu_label) gtk_widget_destroy(GTK_WIDGET(this->menu_label));
	if(this->vbox) gtk_widget_destroy(GTK_WIDGET(this->vbox));
	if(this->tree_model_sort) g_object_unref(this->tree_model_sort);
	
	G_OBJECT_CLASS(timelines_sexy_tree_view_parent_class)->finalize(G_OBJECT(timelines_sexy_tree_view));
}/* timelines_sexy_tree_view_finalized */

const gchar *monitoring_to_string(UpdateMonitor monitoring){
	switch(monitoring){
		case Updates:		return _("friends' updates");
		case Replies:		return _("@ replies");
		case DMs:		return _("direct messages");
		case BestFriends:	return _("best friend's updates");
		case Users:		return _("friend's updates");
		case Faves:		return _("star'd updates");
		case Searches:		return _("search results");
		case Groups:		return _("group discussions");
		case Timelines:		return _("global updates");
		case Archive:		return _("own updates");
		case None:	default:
			return _("a lil cracker :-P");
	}
}/*monitoring_to_string(monitoring);*/

/*BEGIN: Custom TimelinesSexyTreeView methods.*/
/**
 *@returns: -1, 0, 1 if the timeline hasn't loaded, is loading, or is re-loading.
 */
gint8 timelines_sexy_tree_view_has_loaded(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return 0;
	return GET_PRIVATE(timelines_sexy_tree_view)->has_loaded;
}/*timelines_sexy_tree_view_has_loaded(timelines_sexy_tree_view);*/

const gchar *timelines_sexy_tree_view_get_timeline(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) )) return NULL;
	return GET_PRIVATE(timelines_sexy_tree_view)->timeline;
}/*timelines_sexy_tree_view_get_timeline(timelines_sexy_tree_view);*/

GtkVBox *timelines_sexy_tree_view_get_child(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) )) return NULL;
	return GET_PRIVATE(timelines_sexy_tree_view)->vbox;
}/*timelines_sexy_tree_view_get_child(timelines_sexy_tree_view);*/

GtkLabel *timelines_sexy_tree_view_get_tab(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) )) return NULL;
	return GET_PRIVATE(timelines_sexy_tree_view)->tab_label;
}/*timelines_sexy_tree_view_get_label(TimelinesSexyTreeView *timelines_sexy_tree_view);*/

GtkLabel *timelines_sexy_tree_view_get_menu(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) )) return NULL;
	return GET_PRIVATE(timelines_sexy_tree_view)->menu_label;
}/*timelines_sexy_tree_view_get_label(TimelinesSexyTreeView *timelines_sexy_tree_view);*/

OnlineService *timelines_sexy_tree_view_get_service(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) && GET_PRIVATE(timelines_sexy_tree_view)->service )) return NULL;
	return GET_PRIVATE(timelines_sexy_tree_view)->service;
}/*timelines_sexy_tree_view_get_timeline(timelines_sexy_tree_view);*/

GtkListStore *timelines_sexy_tree_view_get_list_store(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) )) return NULL;
	return GET_PRIVATE(timelines_sexy_tree_view)->list_store;
}/*timelines_sexy_tree_view_get_list_store(timelines_sexy_tree_view);*/

GtkTreeModel *timelines_sexy_tree_view_get_tree_model(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) )) return NULL;
	return GET_PRIVATE(timelines_sexy_tree_view)->tree_model;
}/*timelines_sexy_tree_view_get_tree_model(timelines_sexy_tree_view);*/

gint timelines_sexy_tree_view_get_page(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return -1;
	return GET_PRIVATE(timelines_sexy_tree_view)->page;
}/*timelines_sexy_tree_view_get_page(timelines_sexy_tree_view);*/

UpdateMonitor timelines_sexy_tree_view_get_monitoring(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return None;
	return GET_PRIVATE(timelines_sexy_tree_view)->monitoring;
}/*timelines_sexy_tree_view_get_monitoring(timelines_sexy_tree_view);*/

const gchar *timelines_sexy_tree_view_get_monitoring_string(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) )) return NULL;
	return GET_PRIVATE(timelines_sexy_tree_view)->monitoring_string;
}/*timelines_sexy_tree_view_get_monitoring_string(TimelinesSexyTreeView *timelines_sexy_tree_view);*/

guint timelines_sexy_tree_view_get_notify_delay(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) )) return 1*10;
	return (GET_PRIVATE(timelines_sexy_tree_view)->page+1)*10;
}/*timelines_sexy_tree_view_get_notify_delay(timelines_sexy_tree_view);*/

gint timelines_sexy_tree_view_get_total(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return 0;
	return GET_PRIVATE(timelines_sexy_tree_view)->total;
}/*timelines_sexy_tree_view_get_total(timelines_sexy_tree_view);*/

void timelines_sexy_tree_view_set_page(TimelinesSexyTreeView *timelines_sexy_tree_view, gint page){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	GET_PRIVATE(timelines_sexy_tree_view)->page=page;
}/*timelines_sexy_tree_view_set_page(timelines_sexy_tree_view, 0);*/

void timelines_sexy_tree_view_start(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	gtk_progress_bar_set_fraction(this->progress_bar, timelines_sexy_tree_view_prepare_reload(timelines_sexy_tree_view) );
	
	if(!gtk_toggle_tool_button_get_active(this->stop_toggle_tool_button)){
		debug("Creating timeout to reload %s.", this->menu_label_string);
		this->timeout_id=g_timeout_add_seconds(this->reload, (GSourceFunc)timelines_sexy_tree_view_refresh, timelines_sexy_tree_view);
	}
	
	if(!this->has_loaded)
		network_set_state_loading_timeline(this->timeline, Load);
	else if(this->has_loaded){
		timelines_sexy_tree_view_check_updates(timelines_sexy_tree_view);
		network_set_state_loading_timeline(this->timeline, Reload);
	}
	
	if(this->minutes){
		if(!this->service){
			online_services_request(QUEUE, this->timeline, NULL, network_display_timeline, timelines_sexy_tree_view, (gpointer)this->monitoring);
		}else{
			online_service_request(this->service, QUEUE, this->timeline, NULL, network_display_timeline, timelines_sexy_tree_view, (gpointer)this->monitoring);
		}
	}
}/*timelines_sexy_tree_view_start(TimelinesSexyTreeView *timelines_sexy_tree_view);*/

static float timelines_sexy_tree_view_prepare_reload(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return 0.0;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	if(this->has_loaded > 1 && this->minutes)
		return 1.0;
	
	this->has_loaded++;
	
	guint seconds=((this->page+1)*5)+(this->monitoring*3)+10;
	if(!this->minutes){
		gconfig_get_int(PREFS_TIMELINE_RELOAD_MINUTES, &this->minutes);
		/* With multiple timeline support timeline re-loading interval shouldn't be less than 5 minutes */
		if(this->minutes < 5)
			gconfig_set_int(PREFS_TIMELINE_RELOAD_MINUTES, (this->minutes=5) );
		
		this->minutes+=this->page+this->monitoring+1;
		debug("Setting %s's, timeline: %s, TimelinesSexyTreeView's minutes and initial timeout.  They'll reload evey: %lu seconds(%d minutes and %d seconds).", this->monitoring_string, this->timeline, this->reload, this->minutes, seconds);
		this->reload=(this->minutes*60)+seconds;
	}
	
	statusbar_printf("Your %s will automatically update every %d minutes and %d seconds.", this->monitoring_string, this->minutes, seconds);
	debug("Setting %s's, timeline: %s,  TimelinesSexyTreeView's timeout.  They'll reload evey: %lu seconds(%d minutes and %d seconds).", this->monitoring_string, this->timeline, this->reload, this->minutes, seconds);
	return 0.0;
}/*timelines_sexy_tree_view_prepare_reload(timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_set_adjustment(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	guint connected_online_services=online_services_has_connected(0);
	if(this->connected_online_services==connected_online_services)	return;
	
	this->connected_online_services=connected_online_services;
	
	this->minimum=connected_online_services*MINIMUM_UPDATES;
	gtk_adjustment_set_lower(this->max_updates_adjustment, this->minimum);
	gtk_adjustment_set_lower(this->progress_bar_adjustment, this->minimum);
	
	this->maximum=connected_online_services*MAXIMUM_UPDATES;
	gtk_adjustment_set_upper(this->max_updates_adjustment, this->maximum);
	gtk_adjustment_set_upper(this->progress_bar_adjustment, this->maximum);
	
	gtk_adjustment_set_value(this->max_updates_adjustment, this->max_updates);
	gtk_spin_button_set_value(this->max_updates_spin_button, this->max_updates);
	
	debug("Connected TimelinesSexyTreeView's for %s's, timeline: %s, for %d OnlineServices and adjustments set-up.  Minimum: %f; Maximum: %f; Max Updates: %f.", this->monitoring_string, this->timeline, this->connected_online_services, this->minimum, this->maximum, this->max_updates);
}/*timelines_sexy_tree_view_set_adjustment(timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_check_updates(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(!this->total) return;
	switch(this->monitoring){
		case	DMs:	case Replies:
		case	Faves:	case BestFriends:
		case	Users:
			timelines_sexy_tree_view_check_inbox(timelines_sexy_tree_view);
			return;
		
		case	Updates:	case Timelines:
		case	Searches:	case Groups:
		case	Archive:
			timelines_sexy_tree_view_check_maximum_updates(timelines_sexy_tree_view);
			return;
		
		case	None: default:
			return;
	}
}/*timelines_sexy_tree_view_check_updates(timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_set_maximum_updates(GtkSpinButton *max_updates_spin_button, TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	gdouble max_updates=gtk_spin_button_get_value(this->max_updates_spin_button);
	if(max_updates==this->max_updates)
		return;
	
	if(max_updates > this->maximum)
		max_updates=this->maximum;
	else if(max_updates < this->minimum)
		max_updates=this->minimum;
	
	if(GTK_WIDGET_IS_SENSITIVE(this->max_updates_spin_button) && (this->monitoring==DMs || this->monitoring==Replies || this->monitoring==Faves) ){
		gtk_widget_set_sensitive(GTK_WIDGET(this->max_updates_spin_button), FALSE);
		gtk_widget_hide(GTK_WIDGET(this->max_updates_spin_button));
		max_updates=this->maximum;
	}
	uber_free(this->max_updates_str);
	this->max_updates_str=gdouble_to_str((this->max_updates=max_updates) );
	
	gtk_adjustment_set_value(this->max_updates_adjustment, this->max_updates);
	gtk_adjustment_set_value(this->progress_bar_adjustment, this->max_updates);
}/*timelines_sexy_tree_view_set_maximum_updates(this->max_updates_spin_button, timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_check_maximum_updates(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(!(this->total && this->total > this->max_updates))	return;
	
	debug("Cleaning up TimelinesSexyTreeView for %s.  TimelinesSexyTreeView's total updates: %d.  Maximum allowed updates: %f", this->timeline, this->total, this->max_updates);
	if( gconfig_if_bool( SCROLL_TO_TOP_WITH_NEW_UPDATES, TRUE ) ){
		debug("Moving focus to TimelinesSexyTreeView's top since no iter is currently selected selected.");
		timelines_sexy_tree_view_scroll_to_top(timelines_sexy_tree_view);
	}
	
	gboolean index_updated=FALSE;
	for(gint i=this->total-1; i>this->max_updates; i--){
		OnlineService *service=NULL;
		gboolean unread=TRUE;
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter(this->tree_model, iter, path))){
			debug("Retrieving iter from path to index %d failed.  Unable to remove row.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gint list_store_index=-1, selected_index=-1;
		gtk_tree_model_get(
				this->tree_model, iter,
					ONLINE_SERVICE, &service,
					GBOOLEAN_UNREAD, &unread,
					ONLINE_SERVICE, &service,
					GINT_LIST_STORE_INDEX, &list_store_index,
					GINT_SELECTED_INDEX, &selected_index,
				-1
		);
		debug("Removing iter for <%s>'s %s at index: %d; list_store_index: %d; selected_index: %d.", service->guid, this->monitoring_string, i, list_store_index, selected_index);
		
		debug( "Removing <%s>'s %s at index %i which exceeds maximum updates: %s.", service->guid, this->monitoring_string, i, this->max_updates_str );
		
		if( unread && this->unread_updates )
			this->unread_updates--;
		
		if( !index_updated && this->index>-1 && selected_index>-1 && this->list_store_index>-1 && this->index==selected_index ){
			index_updated=TRUE;
			this->index=-1;
			
			if(timelines_sexy_tree_view_scroll_to_top(timelines_sexy_tree_view) )
				debug("TimelinesSexyTreeView for %s(timeline %s) index reset to 0 and perspective scrolled to the top.", this->monitoring_string, this->timeline );
		}
		
		gtk_list_store_remove(this->list_store, iter);
		this->total--;
		
		gtk_tree_path_free(path);
		uber_free(iter);
	}
	timelines_sexy_tree_view_update_age(timelines_sexy_tree_view, 0);
}/*timelines_sexy_tree_view_check_maximum_updates(timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_check_inbox(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(!this->total) return;
	
	gint update_expiration=0;
	switch(this->monitoring){
		case	DMs:
			/*By default Direct Messages from the last 4 weeks are loaded.*/
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_DMS, &update_expiration, 2419200);
			break;
		
		case	Replies:
			/*By default Replies, & @ Mentions, from the last 7 days are loaded.*/
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_REPLIES, &update_expiration, 604800);
			break;
		
		case	Faves:
			/*By default "My Favorite/Started Updates" from the last 4 weeks are loaded.*/
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_FAVES, &update_expiration, 2419200);
			break;
		
		case	BestFriends:
			/*By default "Best Friends' Updates" from the last 24 hrs are loaded.*/
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_BEST_FRIENDS, &update_expiration, 86400);
			break;
		
		case	Updates:	case Users:
		case	Searches:	case Groups:
		case	Timelines:	case	Archive:
			break;
		
		case None:
			return;
	}
	
	timelines_sexy_tree_view_update_age(timelines_sexy_tree_view, update_expiration);
}/*timelines_sexy_tree_view_check_inbox(timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_update_age(TimelinesSexyTreeView *timelines_sexy_tree_view, gint expiration){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(!this->total) return;
	if(this->list_store_index>-1) this->list_store_index++;
	
	gboolean index_updated=FALSE;
	for(gint i=0; i<this->total; i++){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter(this->tree_model, iter, path))){
			debug("Retrieving iter from path to index %d failed.  Unable to update row's created_ago time.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		OnlineService	*service=NULL;
		gint list_store_index=-1;
		gint selected_index=-1;
		gint created_ago=0;
		gboolean unread=TRUE;
		gchar *created_at_str=NULL, *created_how_long_ago=NULL;
		gtk_tree_model_get(
				this->tree_model, iter,
					ONLINE_SERVICE, &service,
					GINT_SELECTED_INDEX, &selected_index,
					GINT_LIST_STORE_INDEX, &list_store_index,
					STRING_CREATED_AT, &created_at_str,
				-1
		);
		
		if(this->list_store_index>-1 && i>=this->list_store_index)
			list_store_index+=this->list_store_index;
		
		created_how_long_ago=parser_convert_time(created_at_str, &created_ago);
		if(expiration > 0 && created_ago > 0 && created_ago > expiration){
			if( !index_updated && this->index>-1 && selected_index>-1 && this->list_store_index>-1 && this->index==selected_index ){
				index_updated=TRUE;
				this->index=-1;
				
				if(timelines_sexy_tree_view_scroll_to_top(timelines_sexy_tree_view) )
					debug("TimelinesSexyTreeView for %s(timeline %s) index reset to 0 and perspective scrolled to the top.", this->monitoring_string, this->timeline );
			}
			
			debug("Removing TimelinesSexyTreeView iter for <%s>'s %s at index: %d; list_store_index: %d; selected_index: %d.", service->guid, this->monitoring_string, i, list_store_index, selected_index);
			debug( "Removing <%s>'s expired %s.  Oldest %s allowed: [%d] it was posted %d.", service->guid, this->monitoring_string, this->monitoring_string, expiration, created_ago );
			gtk_list_store_remove(this->list_store, iter);
			this->total--;
			if( unread && this->unread_updates )
				this->unread_updates--;
		}else{
			if( !index_updated && this->index>-1 && selected_index>-1 && this->list_store_index>-1 && this->index==selected_index ){
				debug("Updating index for <%s>'s %s(timeline %s), previous index: %d; new index: %d.", service->guid, this->monitoring_string, this->timeline, this->index, ( this->index ?(this->index+this->list_store_index+1) :(this->index+this->list_store_index) ) );
				index_updated=TRUE;
				this->index+=this->list_store_index;
			}
			gtk_list_store_set(
					this->list_store, iter,
						STRING_CREATED_AGO, created_how_long_ago, /* (seconds|minutes|hours|day) ago.*/ 
						GINT_CREATED_AGO, created_ago, /* How old the post is, in seconds, for sorting.*/
						GINT_LIST_STORE_INDEX, i, /* the row's list store index..*/
						GINT_SELECTED_INDEX, -1, /* the row's tree_model_sortable index.*/ 
				-1
			);
		}
		uber_free(created_how_long_ago);
		uber_free(created_at_str);
		service=NULL;
		gtk_tree_path_free(path);
		uber_free(iter);
	}
	if(this->list_store_index>-1) this->list_store_index=-1;
}/*timelines_sexy_tree_view_update_age(timelines_sexy_tree_view, 0);*/

const gchar *timelines_sexy_tree_view_list_store_column_to_string(TimelinesSexyTreeViewListStoreColumn timelines_sexy_tree_view_list_store_column){
	switch(timelines_sexy_tree_view_list_store_column){
		case	ONLINE_SERVICE: return _("ONLINE_SERVICE");
		case	STRING_USER: return _("STRING_USER");
		case	STRING_NICK: return _("STRING_NICK");
		case	STRING_TEXT: return _("STRING_TEXT");
		case	STRING_UPDATE: return _("STRING_UPDATE");
		case	STRING_SEXY_UPDATE: return _("STRING_SEXY_UPDATE");
		case	STRING_CREATED_AGO: return _("STRING_CREATED_AGO");	
		case	STRING_CREATED_AT: return _("STRING_CREATED_AT");
		case	STRING_FROM: return _("STRING_FROM");
		case	STRING_RCPT: return _("STRING_RCPT");
		case	PIXBUF_AVATAR: return _("PIXBUF_AVATAR");
		case	GUINT_TIMELINES_SEXY_TREE_VIEW_INDEX: return _("GUINT_TIMELINES_SEXY_TREE_VIEW_INDEX");
		case	GDOUBLE_UPDATE_ID: return _("GDOUBLE_UPDATE_ID");
		case	GDOUBLE_USER_ID: return _("GDOUBLE_USER_ID");
		case	GINT_CREATED_AGO: return _("GINT_CREATED_AGO");
		case	ULONG_CREATED_AT: return _("ULONG_CREATED_AT");
		case	GINT_LIST_STORE_INDEX: return _("GINT_LIST_STORE_INDEX");
		case	GINT_SELECTED_INDEX: return _("GINT_SELECTED_INDEX");
		case	GBOOLEAN_UNREAD: return _("GBOOLEAN_UNREAD");
		default: return _("UNKNOWN COLUMN");
	}
}/*timelines_sexy_tree_view_list_store_column_to_string(timelines_sexy_tree_view_list_store_column);*/

static void timelines_sexy_tree_view_modifiy_updates_list_store( TimelinesSexyTreeView *timelines_sexy_tree_view, TimelinesSexyTreeViewListStoreColumn timelines_sexy_tree_view_list_store_column, gpointer value, gboolean list_store_remove ){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(!this->total) return;
	
	for(gint i=this->total-1; i>=0; i--){
		gboolean unread=FALSE;
		OnlineService *service=NULL;
		gpointer value_at_index=NULL;
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter(this->tree_model, iter, path))){
			debug("Retrieving iter from path to index %d failed.  Unable to %s row.", i, (list_store_remove ?_("remove") :_("update") ));
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gint list_store_index=-1, selected_index=-1;
		gtk_tree_model_get(
					this->tree_model, iter,
						timelines_sexy_tree_view_list_store_column, &value_at_index,
						ONLINE_SERVICE, &service,
						GBOOLEAN_UNREAD, &unread,
						GINT_LIST_STORE_INDEX, &list_store_index,
						GINT_SELECTED_INDEX, &selected_index,
					-1
		);
		
		switch(timelines_sexy_tree_view_list_store_column){
			case	ONLINE_SERVICE:
				if(value!=value_at_index) continue;
				break;
			
			case	STRING_USER:
			case	STRING_NICK:
			case	STRING_TEXT:
			case	STRING_UPDATE:
			case	STRING_SEXY_UPDATE:
			case	STRING_CREATED_AGO:	
			case	STRING_CREATED_AT:
			case	STRING_FROM:
			case	STRING_RCPT:
				if(!g_str_equal((gchar *)value, (gchar *)value_at_index)){
					uber_free(value_at_index);
					continue;
				}
				break;
			case	PIXBUF_AVATAR:
				if(value_at_index==value) break;
				
				g_object_unref(value_at_index);
			case	GUINT_TIMELINES_SEXY_TREE_VIEW_INDEX:
			case	GDOUBLE_UPDATE_ID:
			case	GDOUBLE_USER_ID:
			case	GINT_CREATED_AGO:
			case	ULONG_CREATED_AT:
			case	GINT_LIST_STORE_INDEX:
			case	GINT_SELECTED_INDEX:
			case	GBOOLEAN_UNREAD:
				return;
				break;
		}
		
		if(!list_store_remove){
			debug("Updating iter for <%s>'s %s at index: %d; list_store_index: %d; selected_index: %d.", service->guid, this->monitoring_string, i, list_store_index, selected_index);
			gtk_list_store_set( this->list_store, iter, timelines_sexy_tree_view_list_store_column_to_string(timelines_sexy_tree_view_list_store_column), value, -1 );
		}else{
			debug("Removing iter for <%s>'s %s at index: %d; list_store_index: %d; selected_index: %d.", service->guid, this->monitoring_string, i, list_store_index, selected_index);
			if(i==this->index){
				this->index=-1;
				debug("Moving focus to TimelinesSexyTreeView's top since the currently selected iter is being removed.");
				timelines_sexy_tree_view_scroll_to_top(timelines_sexy_tree_view);
			}
			gtk_list_store_remove(this->list_store, iter);
			this->total--;
			if(unread && this->unread_updates)
				this->unread_updates--;
		}
		gtk_tree_path_free(path);
		uber_free(iter);
	}
}/*timelines_sexy_tree_view_modifiy_updates_list_store( timelines_sexy_tree_view, timelines_sexy_tree_view_list_store_column, value, TRUE|FALSE );*/

void timelines_sexy_tree_view_update_list_store( TimelinesSexyTreeView *timelines_sexy_tree_view, TimelinesSexyTreeViewListStoreColumn timelines_sexy_tree_view_list_store_column, gpointer value ){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	timelines_sexy_tree_view_modifiy_updates_list_store( timelines_sexy_tree_view, timelines_sexy_tree_view_list_store_column, value, FALSE );	
}/*timelines_sexy_tree_view_update_list_store( TimelinesSexyTreeView *timelines_sexy_tree_view, TimelinesSexyTreeViewListStoreColumn timelines_sexy_tree_view_list_store_column, gpointer value );*/

void timelines_sexy_tree_view_remove_from_list_store( TimelinesSexyTreeView *timelines_sexy_tree_view, TimelinesSexyTreeViewListStoreColumn timelines_sexy_tree_view_list_store_column, gpointer value ){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	timelines_sexy_tree_view_modifiy_updates_list_store( timelines_sexy_tree_view, timelines_sexy_tree_view_list_store_column, value, TRUE );
}/*timelines_sexy_tree_view_remove_from_list_store( timelines_sexy_tree_view, ONLINE_SERVICE, service );*/

void timelines_sexy_tree_view_remove_service(TimelinesSexyTreeView *timelines_sexy_tree_view, OnlineService *service){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(!this->total) return;
	
	debug( "Removing <%s>'s %s updates.", service->guid, this->monitoring_string );
	timelines_sexy_tree_view_remove_from_list_store( timelines_sexy_tree_view, ONLINE_SERVICE, service );
}/*timelines_sexy_tree_view_remove_service(timelines_sexy_tree_view, service);*/

static void timelines_sexy_tree_view_refresh_clicked(GtkButton *timelines_sexy_tree_view_refresh_tool_button, TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	timelines_sexy_tree_view_clear(timelines_sexy_tree_view);
	timelines_sexy_tree_view_refresh(timelines_sexy_tree_view);
}/*timelines_sexy_tree_view_refresh_clicked(timelines_sexy_tree_view);*/

gboolean timelines_sexy_tree_view_refresh(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) )) return FALSE;
	timelines_sexy_tree_view_stop(timelines_sexy_tree_view);
	timelines_sexy_tree_view_set_adjustment(timelines_sexy_tree_view);
	timelines_sexy_tree_view_start(timelines_sexy_tree_view);
	return FALSE;
}/*timelines_sexy_tree_view_refresh(timelines_sexy_tree_view);*/

void timelines_sexy_tree_view_complete(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(!this->connected_online_services)	return;
	
	if(gconfig_if_bool( SCROLL_TO_TOP_WITH_NEW_UPDATES, TRUE ))
		timelines_sexy_tree_view_scroll_to_top(timelines_sexy_tree_view);
	
	debug("TimelinesSexyTreeView for %s, timeline: %s, completed processing new %d new updates out of %d total updates.", this->monitoring_string, this->timeline, this->unread_updates, this->total);
	gtk_progress_bar_set_fraction(this->progress_bar, 1.0);
	timelines_sexy_tree_view_update_age(timelines_sexy_tree_view, 0);
	if(this->unread && this->unread_updates)
		timelines_sexy_tree_view_mark_as_unread(timelines_sexy_tree_view);	
}/*timelines_sexy_tree_view_complete(timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_stop_toggle_tool_button_toggled(GtkToggleToolButton *stop_toggle_tool_button, TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	timelines_sexy_tree_view_stop_toggle_setup(timelines_sexy_tree_view);
	if(GET_PRIVATE(timelines_sexy_tree_view)->has_loaded)
		timelines_sexy_tree_view_refresh(timelines_sexy_tree_view);
}/*timelines_sexy_tree_view_stop_toggle_tool_button_toggled(stop_toggle_tool_button, timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_stop_toggle_setup(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	gchar *tooltip_markup=NULL;
	if(!gtk_toggle_tool_button_get_active(this->stop_toggle_tool_button)){
		gtk_tool_button_set_label(GTK_TOOL_BUTTON(this->stop_toggle_tool_button), "<span weight=\"bold\">Disable auto_-update.</span>");
		gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(this->stop_toggle_tool_button), "gtk-media-pause");
		tooltip_markup=g_strdup_printf("<span weight=\"bold\">Stop auto re-loading %s.</span>", this->monitoring_string);
		gtk_widget_set_tooltip_markup(GTK_WIDGET(this->stop_toggle_tool_button), tooltip_markup);
		uber_free(tooltip_markup);
	}else{
		gtk_tool_button_set_label(GTK_TOOL_BUTTON(this->stop_toggle_tool_button), "<span weight=\"light\">Enable auto_-update.</span>");
		gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(this->stop_toggle_tool_button), "gtk-media-play");
		tooltip_markup=g_strdup_printf("<span weight=\"bold\">Start auto re-loading %s.</span>", this->monitoring_string);
		gtk_widget_set_tooltip_markup(GTK_WIDGET(this->stop_toggle_tool_button), tooltip_markup);
		uber_free(tooltip_markup);
	}
}/*timelines_sexy_tree_view_stop_toggle_setup(timelines_sexy_tree_view);*/

void timelines_sexy_tree_view_stop(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(this->timeout_id && G_STR_N_EMPTY(this->timeline))
		program_timeout_remove(&this->timeout_id, g_strrstr(this->timeline, "/"));
}/*timelines_sexy_tree_view_stop(timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_close(GtkToolButton *timelines_sexy_tree_view_close_tool_button, TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	tabs_close_page(this->page);
}/*timelines_sexy_tree_view_close(timelines_sexy_tree_view_close_tool_button, timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_set_timeline_label(TimelinesSexyTreeView *timelines_sexy_tree_view, const gchar *timeline){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) && G_STR_N_EMPTY(timeline) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
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
		if(user_is_best_friend && viewing_newest_updates)
			this->monitoring=BestFriends;
		else
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
	if((this->monitoring==Archive) || (this->monitoring==Users) || (this->monitoring==Faves) )
		gtk_toggle_tool_button_set_active(this->stop_toggle_tool_button, TRUE);
	else
		gtk_toggle_tool_button_set_active(this->stop_toggle_tool_button, FALSE);
	
	this->monitoring_string=g_strdup(monitoring_to_string(this->monitoring));
	timelines_sexy_tree_view_mark_as_read(timelines_sexy_tree_view);
}/*timelines_sexy_tree_view_set_timeline_label(timelines_sexy_tree_view, timeline);*/

static void timelines_sexy_tree_view_create_gui(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	GtkBuilder *gtk_builder_ui=gtkbuilder_get_file(
							GtkBuilderUI,
								"timelines_sexy_tree_view_tab_label", &this->tab_label,
								
								"timelines_sexy_tree_view_menu_label", &this->menu_label,
								
								"timelines_sexy_tree_view_vbox", &this->vbox,
								"timelines_sexy_tree_view_hbox", &this->hbox,
								
								"timelines_sexy_tree_view_handlebox", &this->handlebox,
								"timelines_sexy_tree_view_toolbar", &this->toolbar,
								"timelines_sexy_tree_view_refresh_tool_button", &this->refresh_tool_button,
								"timelines_sexy_tree_view_close_tool_button", &this->close_tool_button,

								"timelines_sexy_tree_view_progress_bar_custom_tool_button", &this->progress_bar_custom_tool_button,
								"timelines_sexy_tree_view_progress_bar", &this->progress_bar,
								"timelines_sexy_tree_view_progress_bar_adjustment", &this->progress_bar_adjustment,
								
								"timelines_sexy_tree_view_max_updates_spin_button", &this->max_updates_spin_button,
								"timelines_sexy_tree_view_max_updates_adjustment", &this->max_updates_adjustment,
								
								"timelines_sexy_tree_view_stop_toggle_tool_button", &this->stop_toggle_tool_button,
								
								"timelines_sexy_tree_view_scrolled_window", &this->scrolled_window,
								
								"timelines_sexy_tree_view_hseparator", &this->hseparator,
								
								/* These make up the core of TweetView */
								"timelines_sexy_tree_view_sexy_tree_view", &this->sexy_tree_view,
								
								"timelines_sexy_tree_view_list_store", &this->list_store,
								
								"timelines_sexy_tree_view_pixbuf_avatar_tree_view_column", &this->pixbuf_avatar_tree_view_column,
								"timelines_sexy_tree_view_pixbuf_avatar_cell_renderer_pixbuf", &this->pixbuf_avatar_cell_renderer_pixbuf,
								
								"timelines_sexy_tree_view_string_update_tree_view_column", &this->string_update_tree_view_column,
								"timelines_sexy_tree_view_string_update_cell_renderer_text", &this->string_update_cell_renderer_text,
								
								"timelines_sexy_tree_view_string_from_tree_view_column", &this->string_from_tree_view_column,
								"timelines_sexy_tree_view_string_from_cell_renderer_text", &this->string_from_cell_renderer_text,
								
								"timelines_sexy_tree_view_string_rcpt_tree_view_column", &this->string_rcpt_tree_view_column,
								"timelines_sexy_tree_view_string_rcpt_cell_renderer_text", &this->string_rcpt_cell_renderer_text,
								
								"timelines_sexy_tree_view_created_ago_str_tree_view_column", &this->created_ago_str_tree_view_column,
								"timelines_sexy_tree_view_created_ago_str_cell_renderer_text", &this->created_ago_str_cell_renderer_text,
							NULL
	);
	
	gtkbuilder_connect(gtk_builder_ui, timelines_sexy_tree_view,
								"timelines_sexy_tree_view_refresh_tool_button", "clicked", timelines_sexy_tree_view_refresh_clicked,
								"timelines_sexy_tree_view_max_updates_spin_button", "value-changed", timelines_sexy_tree_view_set_maximum_updates,
								"timelines_sexy_tree_view_stop_toggle_tool_button", "toggled", timelines_sexy_tree_view_stop_toggle_tool_button_toggled,
								"timelines_sexy_tree_view_close_tool_button", "clicked", timelines_sexy_tree_view_close,
								
								"timelines_sexy_tree_view_scrolled_window", "grab-focus", timelines_sexy_tree_view_grab_focus_cb,
								"timelines_sexy_tree_view_scrolled_window", "size-allocate", timelines_sexy_tree_view_resize_cb,
								
								"timelines_sexy_tree_view_sexy_tree_view", "grab-focus", timelines_sexy_tree_view_grab_focus_cb,
								
								"timelines_sexy_tree_view_sexy_tree_view", "get-tooltip", timelines_sexy_tree_view_set_sexy_tooltip,
								
								"timelines_sexy_tree_view_sexy_tree_view", "cursor-changed", timelines_sexy_tree_view_update_selected,
								"timelines_sexy_tree_view_sexy_tree_view", "size-allocate", timelines_sexy_tree_view_resize_cb,
								"timelines_sexy_tree_view_sexy_tree_view", "key-press-event", hotkey_pressed,
							NULL
	);
	
	this->tree_model=GTK_TREE_MODEL(this->list_store);
	this->tree_model_sort=gtk_tree_model_sort_new_with_model(this->tree_model);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(this->tree_model_sort), GINT_CREATED_AGO, GTK_SORT_ASCENDING);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(this->sexy_tree_view), this->tree_model_sort);
	sexy_tree_view_set_tooltip_label_column(this->sexy_tree_view, STRING_SEXY_UPDATE);
}/*timelines_sexy_tree_view_create_gui(timelines_sexy_tree_view);*/

void timelines_sexy_tree_view_key_pressed(TimelinesSexyTreeView *timelines_sexy_tree_view, GdkEventKey *event){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	if(event->keyval!=GDK_Return && event->keyval!=GDK_KP_Enter) return timelines_sexy_tree_view_move(timelines_sexy_tree_view, event);
	
	switch(event->state){
		case GDK_CONTROL_MASK:
			update_viewer_sexy_insert_char('\n');
			return;
		case GDK_MOD1_MASK:
			online_service_request_selected_update_forward();
			return;
		case GDK_SHIFT_MASK:
			update_viewer_new_dm();
			return;
		default:
			online_service_request_selected_update_reply();
			return;
	}//switch
}/*timelines_sexy_tree_view_key_pressed(widget, event);*/

static void timelines_sexy_tree_view_move(TimelinesSexyTreeView *timelines_sexy_tree_view, GdkEventKey *event){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	gboolean supported_keyval=TRUE;
	switch(event->keyval){
		case GDK_Tab: case GDK_KP_Tab:
			this->index=0;
			timelines_sexy_tree_view_index_select(timelines_sexy_tree_view);
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
			supported_keyval=FALSE;
			break;
	}//switch
	
	if(supported_keyval)
		switch(event->state){
			case GDK_SHIFT_MASK: case GDK_CONTROL_MASK|GDK_MOD1_MASK:
				timelines_sexy_tree_view_index_select(timelines_sexy_tree_view);
				break;
			default:
				timelines_sexy_tree_view_index_scroll_to(timelines_sexy_tree_view);
				break;
		}
		
	update_viewer_sexy_select();
}/* timelines_sexy_tree_view_move(timelines_sexy_tree_view, event); */

static gboolean timelines_sexy_tree_view_index_validate(TimelinesSexyTreeView *timelines_sexy_tree_view, gint *index){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return FALSE;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if( *index > -1 && *index < this->total ) return TRUE;
	*index=( (*index)>=this->total ?this->total :0 );
	update_viewer_beep();
	return FALSE;
}/*timelines_sexy_tree_view_index_validate(timelines_sexy_tree_view, &index);*/

static gboolean timelines_sexy_tree_view_goto(TimelinesSexyTreeView *timelines_sexy_tree_view, gint goto_index, gboolean select){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return FALSE;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(!( GTK_IS_TREE_VIEW(GTK_TREE_VIEW(this->sexy_tree_view)) && this->total )){
		update_viewer_sexy_select();
		return FALSE;
	}
	
	timelines_sexy_tree_view_index_validate(timelines_sexy_tree_view, &goto_index);
	
	GtkTreePath *path=gtk_tree_path_new_from_indices(goto_index, -1);
	if(!select) gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(this->sexy_tree_view), path, NULL, FALSE, 0.0, 0.0);
	else gtk_tree_view_set_cursor(GTK_TREE_VIEW(this->sexy_tree_view), path, NULL, FALSE);
	gtk_tree_path_free(path);
	
	update_viewer_sexy_select();
	return TRUE;
}/*timelines_sexy_tree_view_goto(timelines_sexy_tree_view, 0<=this->total, TRUE|FALSE);*/

static gboolean timelines_sexy_tree_view_goto_index(TimelinesSexyTreeView *timelines_sexy_tree_view, gboolean select){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return FALSE;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	return timelines_sexy_tree_view_goto(timelines_sexy_tree_view, this->index, select);
}/*timelines_sexy_tree_view_goto_index(timelines_sexy_tree_view);*/

static gboolean timelines_sexy_tree_view_index_select(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return FALSE;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(!( GTK_IS_TREE_VIEW(GTK_TREE_VIEW(this->sexy_tree_view)) && this->total )) return FALSE;
	if( timelines_sexy_tree_view_goto_index(timelines_sexy_tree_view, TRUE) ){
		debug("TimelinesSexyTreeView for %s(timeline %s) index selected.  Update %d, out of %d total updates.", this->monitoring_string, this->timeline, this->index, this->total );
		return TRUE;
	}
	return FALSE;
}/*timelines_sexy_tree_view_index_select(timelines_sexy_tree_view);*/

static gboolean timelines_sexy_tree_view_index_scroll_to(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return FALSE;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(!( GTK_TREE_VIEW(this->sexy_tree_view) && this->total )) return FALSE;
	
	if( timelines_sexy_tree_view_goto_index(timelines_sexy_tree_view, FALSE) ){
		debug("TimelinesSexyTreeView for %s(timeline %s) perspective scrolled to index.  Update %d, out of %d total updates.", this->monitoring_string, this->timeline, this->index, this->total );
		return TRUE;
	}
	return FALSE;
}/*timelines_sexy_tree_view_index_scroll_to(timelines_sexy_tree_view);*/

static gboolean timelines_sexy_tree_view_scroll_to_top(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return FALSE;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	if(!( GTK_TREE_VIEW(this->sexy_tree_view) && this->total )) return FALSE;
	if( timelines_sexy_tree_view_goto(timelines_sexy_tree_view, 0, FALSE) ){
		debug("TimelinesSexyTreeView for %s(timeline %s) index reset to 0 and perspective scrolled to the top.", this->monitoring_string, this->timeline );
		return TRUE;
	}
	return FALSE;
}/*timelines_sexy_tree_view_scroll_to_top(timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_clear(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	debug("Clearing TimelinesSexyTreeView, for %s (timeline: %s).  TimelinesSexyTreeView has_loaded status:%s(#%d).", this->monitoring_string, this->timeline, (this->has_loaded>0 ?"TRUE" :"FALSE" ), this->has_loaded );
	gtk_list_store_clear(this->list_store);
	gtk_progress_bar_set_fraction(this->progress_bar, 1.0);
	if(this->has_loaded < 1) this->has_loaded=1;
	if(this->list_store_index>-1) this->list_store_index=-1;
	if(this->index) this->index=-1;
	
	this->total=0;
}/*timelines_sexy_tree_view_clear(timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_grab_focus_cb(GtkWidget *widget, TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	timelines_sexy_tree_view_mark_as_read(timelines_sexy_tree_view);
}/*timelines_sexy_tree_view_grab_focus_cb(widget, event, timelines_sexy_tree_view);*/

gboolean timelines_sexy_tree_view_toggle_toolbar(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return FALSE;
	gtk_widget_toggle_visibility( GTK_WIDGET( GET_PRIVATE(timelines_sexy_tree_view)->handlebox ) );
	return gtk_widget_is_visible( GTK_WIDGET( GET_PRIVATE(timelines_sexy_tree_view)->handlebox ) );
}/*timelines_sexy_tree_view_toggle_toolbar(timelines_sexy_tree_view);*/

gboolean timelines_sexy_tree_view_toggle_from_column(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return FALSE;
	gtk_tree_view_column_toggle_visibility( GET_PRIVATE(timelines_sexy_tree_view)->string_from_tree_view_column );
	return gtk_tree_view_column_get_visible( GET_PRIVATE(timelines_sexy_tree_view)->string_from_tree_view_column );
}/*timelines_sexy_tree_view_toggle_from_column(timelines_sexy_tree_view);*/

gboolean timelines_sexy_tree_view_toggle_rcpt_column(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return FALSE;
	gtk_tree_view_column_toggle_visibility( GET_PRIVATE(timelines_sexy_tree_view)->string_rcpt_tree_view_column );
	return gtk_tree_view_column_get_visible( GET_PRIVATE(timelines_sexy_tree_view)->string_rcpt_tree_view_column );
}/*timelines_sexy_tree_view_toggle_rcpt_column(timelines_sexy_tree_view);*/

void timelines_sexy_tree_view_store( TimelinesSexyTreeView *timelines_sexy_tree_view, UserStatus *status){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	gtk_progress_bar_pulse(this->progress_bar);
	
	gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	online_service_update_ids_get(status->service, this->timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	gboolean unread=(status->id && status->id > unread_update_id);
	
	this->list_store_index++;
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	gtk_list_store_insert(this->list_store, iter, this->list_store_index);
	gtk_list_store_set(
				this->list_store, iter,
					GUINT_TIMELINES_SEXY_TREE_VIEW_INDEX, this->total,
					GDOUBLE_UPDATE_ID, status->id,				/*Tweet's ID.*/
					GDOUBLE_USER_ID, status->user->id,			/*User's ID.*/
					STRING_USER, status->user->user_name,			/*Useruser_name string.*/
					STRING_NICK, status->user->nick_name,			/*Author user_name string.*/
					STRING_TEXT, status->text,				/*Tweet string.*/
					STRING_UPDATE, status->update,				/*Display string.*/
					STRING_SEXY_UPDATE, status->sexy_update,			/*SexyTreeView's tooltip.*/
					STRING_CREATED_AGO, status->created_how_long_ago,	/*(seconds|minutes|hours|day) ago.*/
					STRING_CREATED_AT, status->created_at_str,		/*Date string.*/
					GINT_CREATED_AGO, status->created_seconds_ago,		/*How old the post is, in seconds, for sorting.*/
					ULONG_CREATED_AT, status->created_at,			/*Seconds since the post was posted.*/
					ONLINE_SERVICE, status->service,			/*OnlineService pointer.*/
					STRING_FROM, status->from,				/*Who the update/update is from.*/
					STRING_RCPT, status->rcpt,				/*The key for OnlineService displayed as who the update is to.*/
					GINT_SELECTED_INDEX, -1,				/*The row's 'selected index'.*/
					GINT_LIST_STORE_INDEX, this->list_store_index,		/*The row's unsorted index.*/
					GBOOLEAN_UNREAD, unread,
				-1
	);
	this->total++;
	
	debug("Inserting update in to TimelinesSexyTreeView at index %d.  Inserting one of <%s>'s %s; timeline [%s]; update ID: %s; read status: %s.  Total updates: %u", this->list_store_index, status->service->guid, this->monitoring_string, this->timeline, status->id_str, (unread ?"TRUE" :"FALSE"), this->total);
	debug("Inserting iter for <%s>'s %s at index: %d; list_store_index: %d; selected_index: %d.", status->service->guid, this->monitoring_string, this->total, this->total, -1);
	if(unread)
		timelines_sexy_tree_view_increment_unread(timelines_sexy_tree_view);
	
	/* network_get_image, or its callback network_cb_on_image, free's iter once its no longer needed.*/
	if(!g_file_test(status->user->image_file, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)){
		debug("User Avatar file [%s] not found. Attempting to download: %s", status->user->image_file, status->user->image_uri );
		return network_get_image(status->service, timelines_sexy_tree_view, status->user->image_file, status->user->image_uri, iter);
	}
	
	timelines_sexy_tree_view_set_image(timelines_sexy_tree_view, status->user->image_file, iter);
	uber_free(iter);
}/*timelines_sexy_tree_view_store_update( timelines_sexy_tree_view, status );*/

static gboolean timelines_sexy_tree_view_notification(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return FALSE;
	
	if(gconfig_if_bool(PREFS_NOTIFY_ALL, TRUE))
		return TRUE;
	
	switch(GET_PRIVATE(timelines_sexy_tree_view)->monitoring){
		case Updates: case Users:
			if(gconfig_if_bool(PREFS_NOTIFY_FOLLOWING, TRUE))
				return TRUE;
			break;
		
		case DMs:
			if(gconfig_if_bool(PREFS_NOTIFY_DMS, TRUE))
				return TRUE;
			break;
		
		case Replies:
			if(gconfig_if_bool(PREFS_NOTIFY_REPLIES, TRUE))
				return TRUE;
			break;
		
		case BestFriends:
			if(gconfig_if_bool(PREFS_NOTIFY_BEST_FRIENDS, TRUE))
				return TRUE;
			break;
		
		case Searches: case Groups:	case Faves:
		case Archive: case Timelines:	case None:
		default:
			return FALSE;
			break;
	}
	return FALSE;
}/*timelines_sexy_tree_view_notification(timelines_sexy_tree_view)*/

void timelines_sexy_tree_view_mark_as_read(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	debug("Marking TimelinesSexyTreeView, for %s (timeline: %s), as having %d unread updates.  TimelinesSexyTreeView has_loaded status:%s(#%d).", this->monitoring_string, this->timeline, this->unread_updates, (this->has_loaded>0 ?"TRUE" :"FALSE" ), this->has_loaded );
	
	if(this->unread) this->unread=FALSE;
	gchar *tab_label_markup=NULL, *menu_label_markup=NULL;
	if(!this->unread_updates){
		tab_label_markup=g_markup_printf_escaped("%s", this->tab_label_string );
		menu_label_markup=g_markup_printf_escaped("%s", this->menu_label_string );
	}else{
		tab_label_markup=g_markup_printf_escaped("%s (%d unread)", this->tab_label_string, this->unread_updates );
		menu_label_markup=g_markup_printf_escaped("%s (%d unread)", this->menu_label_string, this->unread_updates );
	}
	gtk_label_set_markup_with_mnemonic(this->tab_label, tab_label_markup);
	gtk_label_set_markup_with_mnemonic(this->menu_label, menu_label_markup);
	uber_free(tab_label_markup);
	uber_free(menu_label_markup);
}/*timelines_sexy_tree_view_mark_as_read(timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_mark_as_unread(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	gtk_window_set_urgency_hint(main_window_get_window(), timelines_sexy_tree_view_notification(timelines_sexy_tree_view) );
	debug("Marking TimelinesSexyTreeView, for %s (timeline: %s), as having %d unread updates(maximum allowed updates: %s).  TimelinesSexyTreeView's total updates:%d; has_loaded status:%s(#%d).", this->monitoring_string, this->timeline, this->unread_updates, this->max_updates_str, this->total, (this->has_loaded>0 ?"TRUE" :"FALSE" ), this->has_loaded );
	
	update_viewer_beep();
	
	gchar *label_markup=g_markup_printf_escaped("<span weight=\"ultrabold\">*%s (%d unread)*</span>", this->tab_label_string, this->unread_updates );
	gtk_label_set_markup_with_mnemonic(this->tab_label, label_markup);
	uber_free(label_markup);
	
	label_markup=g_markup_printf_escaped("<span weight=\"ultrabold\">*%s (%d unread)*</span>", this->menu_label_string, this->unread_updates );
	gtk_label_set_markup_with_mnemonic(this->menu_label, label_markup);
	uber_free(label_markup);
}/*timelines_sexy_tree_view_mark_as_unread(timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_increment_unread(TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	switch(this->monitoring){
		case	DMs:		case Replies:
		case	Faves:		case BestFriends:
		case	Users:
			break;
		
		case	Updates:	case Timelines:
		case	Searches:	case Groups:
		case	Archive:
			if( this->has_loaded>1 && this->total ) break;
			debug("Not-Marking TimelinesSexyTreeView, for %s (timeline: %s), as having %d unread updates(maximum allowed updates exceded: %s).  TimelinesSexyTreeView's total updates:%d; has_loaded status:%s(#%d).", this->monitoring_string, this->timeline, this->unread_updates, this->max_updates_str, this->total, (this->has_loaded>0 ?"TRUE" :"FALSE" ), this->has_loaded );
		
		case	None:		default:
			return;
	}
	
	if(!this->unread) this->unread=TRUE;
	if(this->unread_updates<this->max_updates)
		this->unread_updates++;
}/*timelines_sexy_tree_view_increment_unread(timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_set_sexy_tooltip(SexyTreeView *sexy_tree_view, GtkTreePath *path, GtkTreeViewColumn *column, TimelinesSexyTreeView *timelines_sexy_tree_view){
}/*static void timelines_sexy_tree_view_set_sexy_tooltip(sexy_tree_view, path, column, timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_update_selected(SexyTreeView *timelines_sexy_tree_view_sexy_tree_view, TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	GtkTreeSelection	*sel;
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	if(!((sel=gtk_tree_view_get_selection(GTK_TREE_VIEW(this->sexy_tree_view))) && gtk_tree_selection_get_selected(sel, &this->tree_model_sort, iter) )){
		uber_free(iter);
		update_viewer_sexy_select();
		return;
	}
	
	
	gdouble		update_id, user_id;
	OnlineService	*service=NULL;
	GdkPixbuf	*pixbuf;
	gchar		*user_name, *nick_name, *date, *sexy_update, *text_update;
	gboolean	unread=FALSE;
	
	gint list_store_index=-1, selected_index=-1;
	gtk_tree_model_get(
				this->tree_model_sort, iter,
					GDOUBLE_UPDATE_ID, &update_id,
					GDOUBLE_USER_ID, &user_id,
					STRING_NICK, &nick_name,
					STRING_TEXT, &text_update,
					STRING_SEXY_UPDATE, &sexy_update,
					STRING_CREATED_AGO, &date,
					STRING_USER, &user_name,
					PIXBUF_AVATAR, &pixbuf,
					ONLINE_SERVICE, &service,
					GINT_SELECTED_INDEX, &selected_index,
					GINT_LIST_STORE_INDEX, &list_store_index,
					GBOOLEAN_UNREAD, &unread,
				-1
	);
	
	gchar *update_id_str=gdouble_to_str(update_id);
	if(selected_index < 0 ){
		timelines_sexy_tree_view_find_selected_update_index(timelines_sexy_tree_view, service, user_name, update_id);
		selected_index=this->index;
		debug("Updating TimelinesSexyTreeView, for %s (timeline: %s), marking update ID: %s; from: <%s@%s>; to: <%s>; as read.  TimelinesSexyTreeView details: total updates: %d; list_store_index: %d; selected: %d.", this->monitoring_string, this->timeline, update_id_str, user_name, service->uri, service->guid, this->total, list_store_index, selected_index);
	}
	this->index=list_store_index;
	this->index=selected_index;
	
	debug("Displaying update ID: %s.  From <%s@%s>; To: <%s>.  Indices: list_store %d; selected: %d.", update_id_str, user_name, service->uri, service->guid, list_store_index, selected_index);
	statusbar_printf("Displaying update ID: %s.  From <%s@%s>; To: <%s>.  Indices: list_store %d; selected: %d.", update_id_str, user_name, service->uri, service->guid, list_store_index, selected_index);
	
	update_viewer_view_selected_update(service, update_id, user_id, user_name, nick_name, date, sexy_update, text_update, pixbuf);
	
	uber_free(user_name);
	uber_free(sexy_update);
	uber_free(text_update);
	uber_free(date);
	uber_free(nick_name);
	uber_free(update_id_str);
	if(pixbuf) uber_object_unref(pixbuf);
	uber_free(iter);
	
	update_viewer_sexy_select();
}/*timelines_sexy_tree_view_update_selected(timelines_sexy_tree_view_sexy_tree_view, timelines_sexy_tree_view);*/

static void timelines_sexy_tree_view_find_selected_update_index(TimelinesSexyTreeView *timelines_sexy_tree_view, OnlineService *service0, const gchar *user_name, gdouble update_id){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	gdouble index_update_id=0;
	for(gint i=0; i<this->total; i++){
		OnlineService *service1=NULL;
		GtkTreeIter *iter1=g_new0(GtkTreeIter, 1);
		GtkTreePath *path1=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter(this->tree_model_sort, iter1, path1))){
			gtk_tree_path_free(path1);
			uber_free(iter1);
			continue;
		}
		
		gint list_store_index=-1;
		gtk_tree_model_get(
				this->tree_model_sort, iter1,
					ONLINE_SERVICE, &service1,
					GDOUBLE_UPDATE_ID, &index_update_id,
					GINT_LIST_STORE_INDEX, &list_store_index,
				-1
		);
		
		if(!( service0==service1 && index_update_id==update_id )){
			gtk_tree_path_free(path1);
			uber_free(iter1);
			continue;
		}
		
		GtkTreeIter *iter2=g_new0(GtkTreeIter, 1);
		GtkTreePath *path2=gtk_tree_path_new_from_indices(list_store_index, -1);
		if(!(gtk_tree_model_get_iter(this->tree_model, iter2, path2))){
			gtk_tree_path_free(path1);
			uber_free(iter1);
			gtk_tree_path_free(path2);
			uber_free(iter2);
			continue;
		}
		
		gboolean unread=FALSE;
		OnlineService *service2=NULL;
		gtk_tree_model_get(
				this->tree_model, iter2,
					ONLINE_SERVICE, &service2,
					GDOUBLE_UPDATE_ID, &index_update_id,
					GBOOLEAN_UNREAD, &unread,
				-1
		);
		
		if(!( service1==service2 && index_update_id==update_id )){
			gtk_tree_path_free(path1);
			uber_free(iter1);
			gtk_tree_path_free(path2);
			uber_free(iter2);
			continue;
		}
		
		this->index=i;
		
		debug("Updating TimelinesSexyTreeView, for %s (timeline: %s), total updates %d selected index for update at list_store's index: %d.  Setting selecting_index to: %d marking as read.  Previous unread status: %s.", this->monitoring_string, this->timeline, this->total, i, i, (unread ?"TRUE" :"FALSE") );
		
		if(unread && this->unread_updates){
			this->unread_updates--;
			timelines_sexy_tree_view_mark_as_read(timelines_sexy_tree_view);
		}
		
		
		online_service_update_ids_check( service2, this->timeline, update_id, FALSE );
		best_friends_check_update_ids( service2, user_name, update_id );
		if(this->monitoring!=DMs){
			if(online_service_is_user_best_friend(service2, user_name))
				online_services_best_friends_list_store_mark_as_read(service2, user_name, update_id, best_friends_get_list_store() );
			else if(this->monitoring!=BestFriends && this->monitoring!=Users){
				gchar *user_timeline=g_strdup_printf("/%s.xml", user_name);
				online_service_update_ids_check( service2, user_timeline, update_id, FALSE );
				uber_free(user_timeline);
			}
		}
		
		gtk_list_store_set(
				this->list_store, iter2,
					GINT_SELECTED_INDEX, i,
					GBOOLEAN_UNREAD, FALSE,
				-1
		);
		
		gtk_tree_path_free(path1);
		uber_free(iter1);
		gtk_tree_path_free(path2);
		uber_free(iter2);
		break;
	}
}/*timelines_sexy_tree_view_find_selected_update_index(timelines_sexy_tree_view, service, user_name, update_id);*/

static void timelines_sexy_tree_view_resize_cb(GtkWidget *widget, GtkAllocation *allocation, TimelinesSexyTreeView *timelines_sexy_tree_view){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	const guint8 wrap_width=10;
	gtk_word_wrap_tree_view_column(this->string_update_tree_view_column, this->string_update_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(this->string_rcpt_tree_view_column, this->string_rcpt_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(this->string_from_tree_view_column, this->string_from_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(this->created_ago_str_tree_view_column, this->created_ago_str_cell_renderer_text, wrap_width);
}/*timelines_sexy_tree_view_resize_cb(widget, allocation, timelines_sexy_tree_view);*/

void timelines_sexy_tree_view_set_image(TimelinesSexyTreeView *timelines_sexy_tree_view, const gchar *image_filename, GtkTreeIter *iter){
	if(!( timelines_sexy_tree_view && IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view) ))	return;
	TimelinesSexyTreeViewPrivate *this=GET_PRIVATE(timelines_sexy_tree_view);
	
	GdkPixbuf *pixbuf;
	if(!(pixbuf=images_get_pixbuf_from_filename((gchar *)image_filename)))
		return;
	
	debug("Adding image: '%s' to TimelinesSexyTreeView.", image_filename);
	gtk_list_store_set(this->list_store, iter, PIXBUF_AVATAR, pixbuf, -1);
	g_object_unref(pixbuf);
}/* timelines_sexy_tree_view_set_image */


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

