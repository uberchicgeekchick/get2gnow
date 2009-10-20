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

#include "update-viewer.h"

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
#include "control-panel.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
#define	GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_UPDATE_VIEWER, UpdateViewerPrivate))

typedef struct _TimelineLabels TimelineLabels;

struct _TimelineLabels{
	UpdateMonitor	monitoring;
	const gchar	*timeline;
	const gchar	*tab_label_string;
	const gchar	*menu_label_string;
};

TimelineLabels TimelineLabelsList[]={
	{Tweets,	API_TIMELINE_FRIENDS,		N_("My _Friends' Updates"),	N_("My Friends' Updates")},
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

struct _UpdateViewerPrivate {
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
	guint			list_store_index;
	
	guint			total;
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
#define DEBUG_DOMAINS "OnlineServices:UI:GtkBuilder:GtkBuildable:Networking:Tweets:Requests:Notification:Settings:Setup:Start-Up:update-viewer.c"
#include "debug.h"

#define GtkBuilderUI "update-viewer"

#define	MINIMUM_UPDATES	20.00
#define	MAXIMUM_UPDATES	100.00


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static void update_viewer_class_init(UpdateViewerClass *klass);
static void update_viewer_init(UpdateViewer *update_viewer);
static void update_viewer_finalize(UpdateViewer *update_viewer);

static void update_viewer_create_gui(UpdateViewer *update_viewer);
static float update_viewer_prepare_reload(UpdateViewer *update_viewer);

static void update_viewer_set_timeline_label(UpdateViewer *update_viewer, const gchar *timeline);

static void update_viewer_refresh_clicked(GtkButton *update_viewer_refresh_tool_button, UpdateViewer *update_viewer);

static void update_viewer_stop_toggle_setup(UpdateViewer *update_viewer);
static void update_viewer_stop_toggle_tool_button_toggled(GtkToggleToolButton *stop_toggle_tool_button, UpdateViewer *update_viewer);

static void update_viewer_set_maximum_updates(GtkSpinButton *max_updates_spin_button, UpdateViewer *update_viewer);
static void update_viewer_set_adjustment(UpdateViewer *update_viewer);

const gchar *update_viewer_list_store_column_to_string(UpdateViewerListStoreColumn update_viewer_list_store_column);
static void update_viewer_modifiy_updates_list_store( UpdateViewer *update_viewer, UpdateViewerListStoreColumn update_viewer_list_store_column, gpointer value, gboolean list_store_remove );

static gboolean update_viewer_notification(UpdateViewer *update_viewer);
static void update_viewer_increment_unread(UpdateViewer *update_viewer);
static void update_viewer_mark_as_unread(UpdateViewer *update_viewer);

static void update_viewer_check_updates(UpdateViewer *update_viewer);
static void update_viewer_update_age(UpdateViewer *update_viewer, gint expiration);
static void update_viewer_check_maximum_updates(UpdateViewer *update_viewer);
static void update_viewer_check_inbox(UpdateViewer *update_viewer);

static void update_viewer_grab_focus_cb(GtkWidget *widget, UpdateViewer *update_viewer);
static void update_viewer_set_sexy_tooltip(SexyTreeView *sexy_tree_view, GtkTreePath *path, GtkTreeViewColumn *column, UpdateViewer *update_viewer);
static void update_viewer_changed_cb(SexyTreeView *update_viewer_sexy_tree_view, UpdateViewer *update_viewer);
static void update_viewer_size_cb(GtkWidget *widget, GtkAllocation *allocation, UpdateViewer *update_viewer);

static void update_viewer_clear(UpdateViewer *update_viewer);

static void update_viewer_move(UpdateViewer *update_viewer, GdkEventKey *event);

static gboolean update_viewer_index_validate(UpdateViewer *update_viewer, gint *index);
static gboolean update_viewer_goto(UpdateViewer *update_viewer, gint goto_index, gboolean select);
static gboolean update_viewer_goto_index(UpdateViewer *update_viewer, gboolean select);
static gboolean update_viewer_index_select(UpdateViewer *update_viewer);
static gboolean update_viewer_index_scroll_to(UpdateViewer *update_viewer);
static gboolean update_viewer_scroll_to_top(UpdateViewer *update_viewer);

static void update_viewer_find_selected_update_index(UpdateViewer *update_viewer, OnlineService *service0, const gchar *user_name, gdouble update_id);

G_DEFINE_TYPE(UpdateViewer, update_viewer, SEXY_TYPE_TREE_VIEW);


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
/* BEGIN: GObject core methods. */
static void update_viewer_class_init(UpdateViewerClass *klass){
	GObjectClass	*object_class=G_OBJECT_CLASS(klass);
	
	object_class->finalize=(GObjectFinalizeFunc)update_viewer_finalize;
	
	g_type_class_add_private(object_class, sizeof(UpdateViewerPrivate));
}/* update_viewer_class_init */

static void update_viewer_init(UpdateViewer *update_viewer){
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	this->service=NULL;
	this->user=NULL;
	
	this->has_loaded=-1;
	this->unread_updates=0;
	this->unread=FALSE;
	
	this->minutes=0;
	this->reload=0;
	this->list_store_index=0;
	
	this->connected_online_services=this->timeout_id=this->index=this->total=0;
	this->max_updates_str=gdouble_to_str((this->max_updates=0.0) );
	this->minimum=MINIMUM_UPDATES;
	this->maximum=MAXIMUM_UPDATES;
	
	this->timeline=this->tab_label_string=this->menu_label_string=NULL;
	this->tab_label=this->menu_label=NULL;
	this->vbox=NULL;
	this->tree_model_sort=NULL;
	update_viewer_create_gui(update_viewer);
	
	g_object_set(update_viewer, "rules-hint", TRUE, "reorderable", TRUE, "headers-visible", TRUE, NULL);
	/*
	g_signal_connect(update_viewer, "size-allocate", G_CALLBACK(update_viewer_size_cb), update_viewer);
	g_signal_connect(update_viewer, "cursor-changed", G_CALLBACK(update_viewer_changed_cb), update_viewer);
	g_signal_connect(update_viewer, "row-activated", G_CALLBACK(online_service_request_selected_update_reply), update_viewer);
	*/
	g_signal_connect(update_viewer, "grab-focus", G_CALLBACK(update_viewer_grab_focus_cb), update_viewer);
	g_signal_connect(update_viewer, "key-press-event", G_CALLBACK(hotkey_pressed), update_viewer);
}/* update_viewer_init */

UpdateViewer *update_viewer_new(const gchar *timeline, OnlineService *service){
	if(G_STR_EMPTY(timeline)) {
		debug("**ERROR:** Cannot create tweet view for an empty timeline.");
		return NULL;
	}
	UpdateViewer *update_viewer=g_object_new(TYPE_UPDATE_VIEWER, NULL);
	
	debug("Creating new TweetView for timeline: %s.", timeline);
	if(service && service->connected )
		GET_PRIVATE(update_viewer)->service=service;
	
	update_viewer_set_timeline_label(update_viewer, timeline);
	update_viewer_stop_toggle_setup(update_viewer);
	
	update_viewer_start(update_viewer);
	
	gtk_widget_show_all(GTK_WIDGET(GET_PRIVATE(update_viewer)->vbox));
	
	update_viewer=g_object_ref_sink(update_viewer);
	
	if(gconfig_if_bool( UPDATE_VEWER_TOOLBAR_VISIBILITY, FALSE) )
		update_viewer_toggle_toolbar(update_viewer);
	
	if(gconfig_if_bool( UPDATE_VEWER_FROM_COLUMN_VISIBILITY, FALSE) )
		update_viewer_toggle_from_column(update_viewer);
	
	if(gconfig_if_bool( UPDATE_VEWER_RCPT_COLUMN_VISIBILITY, FALSE) )
		update_viewer_toggle_rcpt_column(update_viewer);
	
	return update_viewer;
}/*update_viewer_new(timeline);*/

static void update_viewer_finalize(UpdateViewer *update_viewer){
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
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
	
	G_OBJECT_CLASS(update_viewer_parent_class)->finalize(G_OBJECT(update_viewer));
}/* update_viewer_finalized */

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

/*BEGIN: Custom UpdateViewer methods.*/
/**
 *@returns: -1, 0, 1 if the timeline hasn't loaded, is loading, or is re-loading.
 */
gint8 update_viewer_has_loaded(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return 0;
	return GET_PRIVATE(update_viewer)->has_loaded;
}/*update_viewer_has_loaded(update_viewer);*/

const gchar *update_viewer_get_timeline(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) )) return NULL;
	return GET_PRIVATE(update_viewer)->timeline;
}/*update_viewer_get_timeline(update_viewer);*/

GtkVBox *update_viewer_get_child(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) )) return NULL;
	return GET_PRIVATE(update_viewer)->vbox;
}/*update_viewer_get_child(update_viewer);*/

GtkLabel *update_viewer_get_tab(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) )) return NULL;
	return GET_PRIVATE(update_viewer)->tab_label;
}/*update_viewer_get_label(UpdateViewer *update_viewer);*/

GtkLabel *update_viewer_get_menu(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) )) return NULL;
	return GET_PRIVATE(update_viewer)->menu_label;
}/*update_viewer_get_label(UpdateViewer *update_viewer);*/

OnlineService *update_viewer_get_service(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) && GET_PRIVATE(update_viewer)->service )) return NULL;
	return GET_PRIVATE(update_viewer)->service;
}/*update_viewer_get_timeline(update_viewer);*/

GtkListStore *update_viewer_get_list_store(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) )) return NULL;
	return GET_PRIVATE(update_viewer)->list_store;
}/*update_viewer_get_list_store(update_viewer);*/

GtkTreeModel *update_viewer_get_tree_model(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) )) return NULL;
	return GET_PRIVATE(update_viewer)->tree_model;
}/*update_viewer_get_tree_model(update_viewer);*/

gint update_viewer_get_page(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return -1;
	return GET_PRIVATE(update_viewer)->page;
}/*update_viewer_get_page(update_viewer);*/

UpdateMonitor update_viewer_get_monitoring(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return None;
	return GET_PRIVATE(update_viewer)->monitoring;
}/*update_viewer_get_monitoring(update_viewer);*/

const gchar *update_viewer_get_monitoring_string(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) )) return NULL;
	return GET_PRIVATE(update_viewer)->monitoring_string;
}/*update_viewer_get_monitoring_string(UpdateViewer *update_viewer);*/

guint update_viewer_get_notify_delay(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) )) return 1*10;
	return (GET_PRIVATE(update_viewer)->page+1)*10;
}/*update_viewer_get_notify_delay(update_viewer);*/

gint update_viewer_get_total(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return 0;
	return GET_PRIVATE(update_viewer)->total;
}/*update_viewer_get_total(update_viewer);*/

void update_viewer_set_page(UpdateViewer *update_viewer, gint page){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	GET_PRIVATE(update_viewer)->page=page;
}/*update_viewer_set_page(update_viewer, 0);*/

void update_viewer_start(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	gtk_progress_bar_set_fraction(this->progress_bar, update_viewer_prepare_reload(update_viewer) );
	
	if(!gtk_toggle_tool_button_get_active(this->stop_toggle_tool_button)){
		debug("Creating timeout to reload %s.", this->menu_label_string);
		this->timeout_id=g_timeout_add(this->reload, (GSourceFunc)update_viewer_refresh, update_viewer);
	}
	
	if(this->has_loaded < 1) return;
	
	/*if(this->has_loaded>1)
		update_viewer_check_updates(update_viewer);*/
	
	if(this->minutes){
		if(this->list_store_index) this->list_store_index=0;
		if(!this->service){
			online_services_request(QUEUE, this->timeline, NULL, network_display_timeline, update_viewer, (gpointer)this->monitoring);
		}else{
			online_service_request(this->service, QUEUE, this->timeline, NULL, network_display_timeline, update_viewer, (gpointer)this->monitoring);
		}
	}
}/*update_viewer_start(UpdateViewer *update_viewer);*/

static float update_viewer_prepare_reload(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return 0.0;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	gint minutes=0;
	gconfig_get_int(PREFS_TIMELINE_RELOAD_MINUTES, &minutes);
	/* With multiple timeline support timeline re-loading interval shouldn't be less than 5 minutes */
	if(this->minutes < 5)
		gconfig_set_int(PREFS_TIMELINE_RELOAD_MINUTES, (this->minutes=minutes=5) );
	
	guint seconds=((this->page+1)*5)+(this->monitoring*3)+10;
	if(!(seconds%2)) seconds+=1;
	minutes+=(this->page+1)+this->monitoring;
	if(!(minutes%2)) minutes+=1;
	if(this->minutes!=minutes)
		this->minutes=minutes;
	else if(this->has_loaded > 1)
		return 0.0;
	
	this->has_loaded++;
	if(this->has_loaded >= 1){
		statusbar_printf("%s will reload every %d minutes.", this->monitoring_string, minutes);
		this->reload=(seconds*1000)+(this->minutes*60000);
	}else{
		this->minutes=0;
		statusbar_printf("Please wait %d seconds for %s to load.", seconds, this->monitoring_string);
		this->reload=seconds*1000;
	}
	
	debug("Setting %s(timeline: %s) to reload evey %lu;", this->monitoring_string, this->timeline, this->reload);
	return 0.0;
}/*update_viewer_prepare_reload(update_viewer);*/

static void update_viewer_set_adjustment(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	guint connected_online_services=online_services_has_connected(0);
	if(connected_online_services==this->connected_online_services)	return;
	
	this->connected_online_services=connected_online_services;
	
	this->minimum=connected_online_services*MINIMUM_UPDATES;
	gtk_adjustment_set_lower(this->max_updates_adjustment, this->minimum);
	gtk_adjustment_set_lower(this->progress_bar_adjustment, this->maximum);
	
	this->maximum=connected_online_services*MAXIMUM_UPDATES;
	gtk_adjustment_set_upper(this->max_updates_adjustment, this->maximum);
	gtk_adjustment_set_upper(this->progress_bar_adjustment, this->maximum);
	
	gtk_adjustment_set_value(this->max_updates_adjustment, this->max_updates);
	
	gtk_spin_button_set_value(this->max_updates_spin_button, this->max_updates);
}/*update_viewer_set_adjustment(update_viewer);*/

static void update_viewer_check_updates(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if(!this->total) return;
	switch(this->monitoring){
		case	DMs:	case Replies:
		case	Faves:	case BestFriends:
		case	Users:
			update_viewer_check_inbox(update_viewer);
			return;
		
		case	Tweets:	case Timelines:
		case	Searches:	case Groups:
		case	Archive:
			update_viewer_check_maximum_updates(update_viewer);
			return;
		
		case	None: default:
			return;
	}
}/*update_viewer_check_updates(update_viewer);*/

static void update_viewer_set_maximum_updates(GtkSpinButton *max_updates_spin_button, UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
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
}/*update_viewer_set_maximum_updates(this->max_updates_spin_button, update_viewer);*/

static void update_viewer_check_maximum_updates(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if(!(this->total && this->total > this->max_updates))	return;
	
	gboolean unread_found=FALSE;
	debug("Cleaning up UpdateViewer for %s.  UpdateViewer's total updates: %d.  Maximum allowed updates: %f", this->timeline, this->total, this->max_updates);
	if( gconfig_if_bool( SCROLL_TO_TOP_WITH_NEW_UPDATES, TRUE ) ){
		debug("Moving focus to UpdateViewer's top since no iter is currently selected selected.");
		update_viewer_scroll_to_top(update_viewer);
	}
	
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
		debug("Removing iter at index: %d; list_store_index: %d", i, list_store_index);
		
		debug( "Removing <%s>'s %s at index %i which exceeds maximum updates: %s.", service->guid, this->monitoring_string, i, this->max_updates_str );
		
		if( unread && this->unread_updates ){
			if(unread_found) unread_found=TRUE;
			this->unread_updates--;
		}
		if(this->index==selected_index){
			this->index=0;
			debug("Moving focus to UpdateViewer's top since the currently selected iter is being removed.");
			update_viewer_scroll_to_top(update_viewer);
		}
		
		gtk_list_store_remove(this->list_store, iter);
		this->total--;
		
		gtk_tree_path_free(path);
		uber_free(iter);
	}
	if(unread_found)
		if(this->unread_updates)
			update_viewer_mark_as_unread(update_viewer);
		else
			update_viewer_mark_as_read(update_viewer);
	update_viewer_update_age(update_viewer, 0);
}/*update_viewer_check_maximum_updates(update_viewer);*/

static void update_viewer_check_inbox(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if(!this->total) return;
	
	gint update_expiration=0;
	switch(this->monitoring){
		case DMs:
			/*By default Direct Messages from the last 4 weeks are loaded.*/
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_DMS, &update_expiration, 2419200);
			break;
		
		case Replies:
			/*By default Replies, & @ Mentions, from the last 7 days are loaded.*/
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_REPLIES, &update_expiration, 604800);
			break;
		
		case Faves:
			/*By default "My Favorite/Started Updates" from the last 4 weeks are loaded.*/
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_FAVES, &update_expiration, 2419200);
			break;
		
		case BestFriends:
			/*By default "Best Friends' Updates" from the last 24 hrs are loaded.*/
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_BEST_FRIENDS, &update_expiration, 86400);
			break;
		
		case Users:
			update_expiration=86400;
			break;
		
		case	None:	case	Tweets:
		case	Searches:	case Groups:
		case	Timelines:	case	Archive:
			return;
	}
	
	update_viewer_update_age(update_viewer, update_expiration);
}/*update_viewer_check_inbox(update_viewer);*/

const gchar *update_viewer_list_store_column_to_string(UpdateViewerListStoreColumn update_viewer_list_store_column){
	switch(update_viewer_list_store_column){
		case	ONLINE_SERVICE: return _("ONLINE_SERVICE");
		case	STRING_USER: return _("STRING_USER");
		case	STRING_NICK: return _("STRING_NICK");
		case	STRING_TEXT: return _("STRING_TEXT");
		case	STRING_TWEET: return _("STRING_TWEET");
		case	STRING_SEXY_TWEET: return _("STRING_SEXY_TWEET");
		case	STRING_CREATED_AGO: return _("STRING_CREATED_AGO");	
		case	STRING_CREATED_AT: return _("STRING_CREATED_AT");
		case	STRING_FROM: return _("STRING_FROM");
		case	STRING_RCPT: return _("STRING_RCPT");
		case	PIXBUF_AVATAR: return _("PIXBUF_AVATAR");
		case	GUINT_UPDATE_VIEWER_INDEX: return _("GUINT_UPDATE_VIEWER_INDEX");
		case	GDOUBLE_UPDATE_ID: return _("GDOUBLE_UPDATE_ID");
		case	GDOUBLE_USER_ID: return _("GDOUBLE_USER_ID");
		case	GINT_CREATED_AGO: return _("GINT_CREATED_AGO");
		case	ULONG_CREATED_AT: return _("ULONG_CREATED_AT");
		case	GINT_LIST_STORE_INDEX: return _("GINT_LIST_STORE_INDEX");
		case	GINT_SELECTED_INDEX: return _("GINT_SELECTED_INDEX");
		case	GBOOLEAN_UNREAD: return _("GBOOLEAN_UNREAD");
		default: return _("UNKNOWN COLUMN");
	}
}/*update_viewer_list_store_column_to_string(update_viewer_list_store_column);*/

static void update_viewer_modifiy_updates_list_store( UpdateViewer *update_viewer, UpdateViewerListStoreColumn update_viewer_list_store_column, gpointer value, gboolean list_store_remove ){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if(!this->total) return;
	
	gboolean unread_found=FALSE;
	
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
		
		gint list_store_index=-1;
		gtk_tree_model_get(
					this->tree_model, iter,
						update_viewer_list_store_column, &value_at_index,
						ONLINE_SERVICE, &service,
						GBOOLEAN_UNREAD, &unread,
						GINT_LIST_STORE_INDEX, &list_store_index,
					-1
		);
		
		switch(update_viewer_list_store_column){
			case	ONLINE_SERVICE:
				if(value!=value_at_index) continue;
				break;
			
			case	STRING_USER:
			case	STRING_NICK:
			case	STRING_TEXT:
			case	STRING_TWEET:
			case	STRING_SEXY_TWEET:
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
			case	GUINT_UPDATE_VIEWER_INDEX:
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
			debug( "Updating <%s>'s %s at index: %d.", service->guid, this->monitoring_string, i );
			gtk_list_store_set( this->list_store, iter, update_viewer_list_store_column_to_string(update_viewer_list_store_column), value, -1 );
		}else{
			debug( "Removing <%s>'s %s at index: %d.", service->guid, this->monitoring_string, i );
			if(i==this->index){
				this->index=0;
				debug("Moving focus to UpdateViewer's top since the currently selected iter is being removed.");
				update_viewer_scroll_to_top(update_viewer);
			}
			gtk_list_store_remove(this->list_store, iter);
			this->total--;
			if(unread && this->unread_updates){
				if(!unread_found) unread_found=TRUE;
				this->unread_updates--;
			}
		}
		
		uber_free(iter);
	}
	if(unread_found)
		if(this->unread_updates)
			update_viewer_mark_as_unread(update_viewer);
		else
			update_viewer_mark_as_read(update_viewer);
}/*update_viewer_modifiy_updates_list_store( update_viewer, update_viewer_list_store_column, value, TRUE|FALSE );*/

void update_viewer_update_list_store( UpdateViewer *update_viewer, UpdateViewerListStoreColumn update_viewer_list_store_column, gpointer value ){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	update_viewer_modifiy_updates_list_store( update_viewer, update_viewer_list_store_column, value, FALSE );	
}/*update_viewer_update_list_store( UpdateViewer *update_viewer, UpdateViewerListStoreColumn update_viewer_list_store_column, gpointer value );*/

void update_viewer_remove_from_list_store( UpdateViewer *update_viewer, UpdateViewerListStoreColumn update_viewer_list_store_column, gpointer value ){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	update_viewer_modifiy_updates_list_store( update_viewer, update_viewer_list_store_column, value, TRUE );
}/*update_viewer_remove_from_list_store( update_viewer, ONLINE_SERVICE, service );*/

void update_viewer_remove_service(UpdateViewer *update_viewer, OnlineService *service){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if(!this->total) return;
	
	debug( "Removing <%s>'s %s updates.", service->guid, this->monitoring_string );
	update_viewer_remove_from_list_store( update_viewer, ONLINE_SERVICE, service );
}/*update_viewer_remove_service(update_viewer, service);*/

static void update_viewer_update_age(UpdateViewer *update_viewer, gint expiration){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if(!this->total) return;
	
	gboolean	unread_found=FALSE;
	
	for(gint i=0; i<this->total; i++){
		gint created_ago=0;
		gboolean unread=TRUE;
		OnlineService	*service=NULL;
		gchar *created_at_str=NULL, *created_how_long_ago=NULL;
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter(this->tree_model, iter, path))){
			debug("Retrieving iter from path to index %d failed.  Unable to update row's created_ago time.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gint list_store_index=-1, selected_index=-1;
		gtk_tree_model_get(
					this->tree_model, iter,
						STRING_CREATED_AT, &created_at_str,
						ONLINE_SERVICE, &service,
						GINT_SELECTED_INDEX, &selected_index,
						GBOOLEAN_UNREAD, &unread,
						GINT_LIST_STORE_INDEX, &list_store_index,
					-1
		);
		
		created_how_long_ago=parser_convert_time(created_at_str, &created_ago);
		if(expiration > 0 && created_ago > 0 && created_ago > expiration){
			if(this->index==selected_index) this->index=0;
			if(update_viewer_scroll_to_top(update_viewer) )
				debug("UpdateViewer for %s(timeline %s) index reset to 0 and perspective scrolled to the top.", this->monitoring_string, this->timeline );
			
			debug( "Removing <%s>'s expired %s.  Oldest %s allowed: [%i] it was posted %i.", service->guid, this->monitoring_string, this->monitoring_string, expiration, created_ago );
			gtk_list_store_remove(this->list_store, iter);
			this->total--;
			if( unread && this->unread_updates ){
				if(!unread_found) unread_found=TRUE;
				this->unread_updates--;
			}
		}else{
			if(this->index==selected_index) this->index=selected_index+1;;
			gtk_list_store_set(
					this->list_store, iter,
						STRING_CREATED_AGO, created_how_long_ago,
							/*(seconds|minutes|hours|day) ago.*/
						GINT_CREATED_AGO, created_ago,
							/*How old the post is, in seconds, for sorting.*/
						GINT_SELECTED_INDEX, ( (selected_index>-1) ? (selected_index+1) :-1 ),
							/* the rows location in the list store.*/
						GINT_LIST_STORE_INDEX, ( this->list_store_index ?(list_store_index+this->list_store_index) :list_store_index ),
				-1
			);
		}
		if(unread && !unread_found) unread_found=TRUE;
		
		uber_free(created_how_long_ago);
		uber_free(created_at_str);
		service=NULL;
		gtk_tree_path_free(path);
		uber_free(iter);
	}
	if(unread_found)
		if(this->unread_updates)
			update_viewer_mark_as_unread(update_viewer);
		else
			update_viewer_mark_as_read(update_viewer);
}/*update_viewer_update_age(update_viewer, 0);*/

static void update_viewer_refresh_clicked(GtkButton *update_viewer_refresh_tool_button, UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	update_viewer_clear(update_viewer);
	update_viewer_refresh(update_viewer);
}/*update_viewer_refresh_clicked(update_viewer);*/

gboolean update_viewer_refresh(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) )) return FALSE;
	update_viewer_stop(update_viewer);
	update_viewer_start(update_viewer);
	return FALSE;
}/*update_viewer_refresh(update_viewer);*/

void update_viewer_complete(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if(!this->connected_online_services)	return;
	
	if(gconfig_if_bool( SCROLL_TO_TOP_WITH_NEW_UPDATES, TRUE ))
		update_viewer_scroll_to_top(update_viewer);
	
	update_viewer_check_updates(update_viewer);
	
	gtk_progress_bar_set_fraction(this->progress_bar, 1.0);
}/*update_viewer_complete(update_viewer);*/

static void update_viewer_stop_toggle_tool_button_toggled(GtkToggleToolButton *stop_toggle_tool_button, UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	update_viewer_stop_toggle_setup(update_viewer);
	if(GET_PRIVATE(update_viewer)->has_loaded)
		update_viewer_refresh(update_viewer);
}/*update_viewer_stop_toggle_tool_button_toggled(stop_toggle_tool_button, update_viewer);*/

static void update_viewer_stop_toggle_setup(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
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
}/*update_viewer_stop_toggle_setup(update_viewer);*/

void update_viewer_stop(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if(this->timeout_id && G_STR_N_EMPTY(this->timeline))
		program_timeout_remove(&this->timeout_id, g_strrstr(this->timeline, "/"));
}/*update_viewer_stop(update_viewer);*/

static void update_viewer_close(GtkToolButton *update_viewer_close_tool_button, UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	tabs_close_page(this->page);
}/*update_viewer_close(update_viewer_close_tool_button, update_viewer);*/

static void update_viewer_set_timeline_label(UpdateViewer *update_viewer, const gchar *timeline){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) && G_STR_N_EMPTY(timeline) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
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
	update_viewer_mark_as_read(update_viewer);
}/*update_viewer_set_timeline_label(update_viewer, timeline);*/

static void update_viewer_create_gui(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	GtkBuilder *gtk_builder_ui=gtkbuilder_get_file(
							GtkBuilderUI,
								"update_viewer_tab_label", &this->tab_label,
								
								"update_viewer_menu_label", &this->menu_label,
								
								"update_viewer_vbox", &this->vbox,
								"update_viewer_hbox", &this->hbox,
								
								"update_viewer_handlebox", &this->handlebox,
								"update_viewer_toolbar", &this->toolbar,
								"update_viewer_refresh_tool_button", &this->refresh_tool_button,
								"update_viewer_close_tool_button", &this->close_tool_button,

								"update_viewer_progress_bar_custom_tool_button", &this->progress_bar_custom_tool_button,
								"update_viewer_progress_bar", &this->progress_bar,
								"update_viewer_progress_bar_adjustment", &this->progress_bar_adjustment,
								
								"update_viewer_max_updates_spin_button", &this->max_updates_spin_button,
								"update_viewer_max_updates_adjustment", &this->max_updates_adjustment,
								
								"update_viewer_stop_toggle_tool_button", &this->stop_toggle_tool_button,
								
								"update_viewer_scrolled_window", &this->scrolled_window,
								
								"update_viewer_hseparator", &this->hseparator,
								
								/* These make up the core of TweetView */
								"update_viewer_sexy_tree_view", &this->sexy_tree_view,
								
								"update_viewer_list_store", &this->list_store,
								
								"update_viewer_pixbuf_avatar_tree_view_column", &this->pixbuf_avatar_tree_view_column,
								"update_viewer_pixbuf_avatar_cell_renderer_pixbuf", &this->pixbuf_avatar_cell_renderer_pixbuf,
								
								"update_viewer_string_update_tree_view_column", &this->string_update_tree_view_column,
								"update_viewer_string_update_cell_renderer_text", &this->string_update_cell_renderer_text,
								
								"update_viewer_string_from_tree_view_column", &this->string_from_tree_view_column,
								"update_viewer_string_from_cell_renderer_text", &this->string_from_cell_renderer_text,
								
								"update_viewer_string_rcpt_tree_view_column", &this->string_rcpt_tree_view_column,
								"update_viewer_string_rcpt_cell_renderer_text", &this->string_rcpt_cell_renderer_text,
								
								"update_viewer_created_ago_str_tree_view_column", &this->created_ago_str_tree_view_column,
								"update_viewer_created_ago_str_cell_renderer_text", &this->created_ago_str_cell_renderer_text,
							NULL
	);
	
	gtkbuilder_connect(gtk_builder_ui, update_viewer,
								"update_viewer_refresh_tool_button", "clicked", update_viewer_refresh_clicked,
								"update_viewer_max_updates_spin_button", "value-changed", update_viewer_set_maximum_updates,
								"update_viewer_stop_toggle_tool_button", "toggled", update_viewer_stop_toggle_tool_button_toggled,
								"update_viewer_close_tool_button", "clicked", update_viewer_close,
								
								"update_viewer_scrolled_window", "grab-focus", update_viewer_grab_focus_cb,
								"update_viewer_scrolled_window", "size-allocate", update_viewer_size_cb,
								
								"update_viewer_sexy_tree_view", "grab-focus", update_viewer_grab_focus_cb,
								
								"update_viewer_sexy_tree_view", "get-tooltip", update_viewer_set_sexy_tooltip,
								
								"update_viewer_sexy_tree_view", "cursor-changed", update_viewer_changed_cb,
								"update_viewer_sexy_tree_view", "size-allocate", update_viewer_size_cb,
								"update_viewer_sexy_tree_view", "key-press-event", hotkey_pressed,
							NULL
	);
	
	this->tree_model=GTK_TREE_MODEL(this->list_store);
	this->tree_model_sort=gtk_tree_model_sort_new_with_model(this->tree_model);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(this->tree_model_sort), GINT_CREATED_AGO, GTK_SORT_ASCENDING);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(this->sexy_tree_view), this->tree_model_sort);
	sexy_tree_view_set_tooltip_label_column(this->sexy_tree_view, STRING_SEXY_TWEET);
	
	update_viewer_set_adjustment(update_viewer);
}/*update_viewer_create_gui(update_viewer);*/

void update_viewer_key_pressed(UpdateViewer *update_viewer, GdkEventKey *event){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	if(event->keyval!=GDK_Return && event->keyval!=GDK_KP_Enter) return update_viewer_move(update_viewer, event);
	
	switch(event->state){
		case GDK_CONTROL_MASK:
			control_panel_sexy_insert_char('\n');
			return;
		case GDK_MOD1_MASK:
			online_service_request_selected_update_forward_update();
			return;
		case GDK_SHIFT_MASK:
			control_panel_new_dm();
			return;
		default:
			online_service_request_selected_update_reply();
			return;
	}//switch
}/*update_viewer_key_pressed(widget, event);*/

static void update_viewer_move(UpdateViewer *update_viewer, GdkEventKey *event){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	gint index=this->index;
	switch(event->keyval){
		case GDK_Tab: case GDK_KP_Tab:
			this->index=0;
			update_viewer_index_select(update_viewer);
			return;
		case GDK_Home: case GDK_KP_Home:
		case GDK_Begin: case GDK_Escape:
			index=0;
			break;
		case GDK_End: case GDK_KP_End:
			index=this->total-1;
			break;
		case GDK_Up: case GDK_KP_Up: case GDK_KP_Prior:
			index--;
			break;
		case GDK_Down: case GDK_KP_Down: case GDK_KP_Next:
			index++;
			break;
		case GDK_Page_Up:
			index-=3; break;
		case GDK_Page_Down:
			index+=3; break;
		default: break;
	}//switch
	
	if( (update_viewer_index_validate(update_viewer, &index)) && index!=this->index ){
		this->index=index;
		switch(event->state){
			case GDK_CONTROL_MASK|GDK_MOD1_MASK:
				update_viewer_index_select(update_viewer);
				break;
			default:
				update_viewer_index_scroll_to(update_viewer);
				break;
		}
	}
	
	if(this->unread) update_viewer_mark_as_read(update_viewer);
	control_panel_sexy_select();
}/* update_viewer_move(update_viewer, event); */

static gboolean update_viewer_index_validate(UpdateViewer *update_viewer, gint *index){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return FALSE;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if((*index) > -1 && (*index) < this->total ) return TRUE;
	
	*index=( (*index)>=this->total ?this->total :0 );
	control_panel_beep();
	return FALSE;
}/*update_viewer_index_validate(update_viewer, &index);*/

static gboolean update_viewer_goto(UpdateViewer *update_viewer, gint goto_index, gboolean select){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return FALSE;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if(!( GTK_IS_TREE_VIEW(GTK_TREE_VIEW(this->sexy_tree_view)) && this->total )){
		control_panel_sexy_select();
		return FALSE;
	}
	
	update_viewer_index_validate(update_viewer, &goto_index);
	
	GtkTreePath *path=gtk_tree_path_new_from_indices(goto_index, -1);
	if(!select)
		gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(this->sexy_tree_view), path, NULL, FALSE, 0.0, 0.0);
	else
		gtk_tree_view_set_cursor(GTK_TREE_VIEW(this->sexy_tree_view), path, NULL, FALSE);
	gtk_tree_path_free(path);
	
	control_panel_sexy_select();
	return TRUE;
}/*update_viewer_goto(update_viewer, 0<=this->total, TRUE|FALSE);*/

static gboolean update_viewer_goto_index(UpdateViewer *update_viewer, gboolean select){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return FALSE;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	return update_viewer_goto(update_viewer, this->index, select);
}/*update_viewer_goto_index(update_viewer);*/

static gboolean update_viewer_index_select(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return FALSE;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if(!( GTK_IS_TREE_VIEW(GTK_TREE_VIEW(this->sexy_tree_view)) && this->total )) return FALSE;
	if( update_viewer_goto_index(update_viewer, TRUE) ){
		debug("UpdateViewer for %s(timeline %s) index selected.  Update %d, out of %d total updates.", this->monitoring_string, this->timeline, this->index, this->total );
		return TRUE;
	}
	return FALSE;
}/*update_viewer_index_select(update_viewer);*/

static gboolean update_viewer_index_scroll_to(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return FALSE;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if(!( GTK_TREE_VIEW(this->sexy_tree_view) && this->total )) return FALSE;
	
	if( update_viewer_goto_index(update_viewer, FALSE) ){
		debug("UpdateViewer for %s(timeline %s) perspective scrolled to index.  Update %d, out of %d total updates.", this->monitoring_string, this->timeline, this->index, this->total );
		return TRUE;
	}
	return FALSE;
}/*update_viewer_index_scroll_to(update_viewer);*/

static gboolean update_viewer_scroll_to_top(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return FALSE;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	if(!( GTK_TREE_VIEW(this->sexy_tree_view) && this->total )) return FALSE;
	gint index_zero=0;
	if( update_viewer_goto(update_viewer, index_zero, FALSE) ){
		debug("UpdateViewer for %s(timeline %s) index reset to 0 and perspective scrolled to the top.", this->monitoring_string, this->timeline );
		return TRUE;
	}
	return FALSE;
}/*update_viewer_scroll_to_top(update_viewer);*/

static void update_viewer_clear(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	debug("Clearing UpdatViewer, for %s (timeline: %s).  UpdateViewer has_loaded status:%s(#%d).", this->monitoring_string, this->timeline, (this->has_loaded>0 ?"TRUE" :"FALSE" ), this->has_loaded );
	gtk_list_store_clear(this->list_store);
	gtk_progress_bar_set_fraction(this->progress_bar, 1.0);
	this->has_loaded=1;
	this->minutes=0;
	if(this->index) {
		this->index=0;
		update_viewer_scroll_to_top(update_viewer);
	}
	
	this->total=0;
}/*update_viewer_clear(update_viewer);*/

static void update_viewer_grab_focus_cb(GtkWidget *widget, UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	update_viewer_mark_as_read(update_viewer);
}/*update_viewer_grab_focus_cb(widget, event, update_viewer);*/

gboolean update_viewer_toggle_toolbar(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return FALSE;
	return gtk_widget_toggle_visibility(GTK_WIDGET(GET_PRIVATE(update_viewer)->handlebox ) );
}/*update_viewer_toggle_toolbar(update_viewer);*/

gboolean update_viewer_toggle_from_column(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return FALSE;
	gtk_tree_view_column_toggle_visibility(GET_PRIVATE(update_viewer)->string_from_tree_view_column);
}/*update_viewer_toggle_from_column(update_viewer);*/

gboolean update_viewer_toggle_rcpt_column(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return FALSE;
	gtk_tree_view_column_toggle_visibility(GET_PRIVATE(update_viewer)->string_rcpt_tree_view_column);
}/*update_viewer_toggle_rcpt_column(update_viewer);*/

void update_viewer_store( UpdateViewer *update_viewer, UserStatus *status){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	
	gtk_progress_bar_pulse(this->progress_bar);
	debug("Appending update to UpdateViewer <%s>'s; timeline [%s]; update ID: %s.  Total updates: %u", status->service->guid, this->timeline, status->id_str, this->total);
	
	gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	online_service_update_ids_get(status->service, this->timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	gboolean unread=(status->id && status->id > unread_update_id);
	
	gtk_list_store_append(this->list_store, iter);
	gtk_list_store_set(
				this->list_store, iter,
					GUINT_UPDATE_VIEWER_INDEX, this->total,
					GDOUBLE_UPDATE_ID, status->id,				/*Tweet's ID.*/
					GDOUBLE_USER_ID, status->user->id,			/*User's ID.*/
					STRING_USER, status->user->user_name,			/*Useruser_name string.*/
					STRING_NICK, status->user->nick_name,			/*Author user_name string.*/
					STRING_TEXT, status->text,				/*Tweet string.*/
					STRING_TWEET, status->tweet,				/*Display string.*/
					STRING_SEXY_TWEET, status->sexy_tweet,			/*SexyTreeView's tooltip.*/
					STRING_CREATED_AGO, status->created_how_long_ago,	/*(seconds|minutes|hours|day) ago.*/
					STRING_CREATED_AT, status->created_at_str,		/*Date string.*/
					GINT_CREATED_AGO, status->created_seconds_ago,		/*How old the post is, in seconds, for sorting.*/
					ULONG_CREATED_AT, status->created_at,			/*Seconds since the post was posted.*/
					ONLINE_SERVICE, status->service,			/*OnlineService pointer.*/
					STRING_FROM, status->from,				/*Who the tweet/update is from.*/
					STRING_RCPT, status->rcpt,				/*The key for OnlineService displayed as who the tweet is to.*/
					GINT_SELECTED_INDEX, -1,				/*The row's 'selected index'.*/
					GINT_LIST_STORE_INDEX, this->total,			/*The row's unsorted index.*/
					/*GINT_LIST_STORE_INDEX, this->list_store_index,*/		/*The row's unsorted index.*/
					GBOOLEAN_UNREAD, unread,
				-1
	);
	this->total++;
	this->list_store_index++;
	
	if(unread)
		update_viewer_increment_unread(update_viewer);
	
	/* network_get_image, or its callback network_cb_on_image, free's iter once its no longer needed.*/
	if(!g_file_test(status->user->image_file, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)){
		debug("User Avatar file [%s] not found. Attempting to download: %s", status->user->image_file, status->user->image_uri );
		return network_get_image(status->service, update_viewer, status->user->image_file, status->user->image_uri, iter);
	}
	
	update_viewer_set_image(update_viewer, status->user->image_file, iter);
	uber_free(iter);
}/*update_viewer_store_update( update_viewer, status );*/

static gboolean update_viewer_notification(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return FALSE;
	
	if(gconfig_if_bool(PREFS_NOTIFY_ALL, TRUE))
		return TRUE;
	
	switch(GET_PRIVATE(update_viewer)->monitoring){
		case Tweets: case Users:
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
}/*update_viewer_notification(update_viewer)*/

void update_viewer_mark_as_read(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	debug("Marking UpdatViewer, for %s (timeline: %s), as having %d unread updates.  UpdateViewer has_loaded status:%s(#%d).", this->monitoring_string, this->timeline, this->unread_updates, (this->has_loaded>0 ?"TRUE" :"FALSE" ), this->has_loaded );
	
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
}/*update_viewer_mark_as_read(update_viewer);*/

static void update_viewer_mark_as_unread(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	gtk_window_set_urgency_hint(main_window_get_window(), update_viewer_notification(update_viewer) );
	debug("Marking UpdatViewer, for %s (timeline: %s), as having %d unread updates(maximum allowed updates: %s).  UpdateViewer's total updates:%d; has_loaded status:%s(#%d).", this->monitoring_string, this->timeline, this->unread_updates, this->max_updates_str, this->total, (this->has_loaded>0 ?"TRUE" :"FALSE" ), this->has_loaded );
	
	control_panel_beep();
	
	gchar *label_markup=g_markup_printf_escaped("<span weight=\"ultrabold\">*%s (%d unread)*</span>", this->tab_label_string, this->unread_updates );
	gtk_label_set_markup_with_mnemonic(this->tab_label, label_markup);
	uber_free(label_markup);
	
	label_markup=g_markup_printf_escaped("<span weight=\"ultrabold\">*%s (%d unread)*</span>", this->menu_label_string, this->unread_updates );
	gtk_label_set_markup_with_mnemonic(this->menu_label, label_markup);
	uber_free(label_markup);
}/*update_viewer_mark_as_unread(update_viewer);*/

static void update_viewer_increment_unread(UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	switch(this->monitoring){
		case	DMs:		case Replies:
		case	Faves:		case BestFriends:
		case	Users:
			break;
		
		case	Tweets:		case Timelines:
		case	Searches:	case Groups:
		case	Archive:
			if( this->has_loaded>1 && this->total ) break;
			debug("Not-Marking UpdatViewer, for %s (timeline: %s), as having %d unread updates(maximum allowed updates exceded: %s).  UpdateViewer's total updates:%d; has_loaded status:%s(#%d).", this->monitoring_string, this->timeline, this->unread_updates, this->max_updates_str, this->total, (this->has_loaded>0 ?"TRUE" :"FALSE" ), this->has_loaded );
		
		case	None:		default:
			return;
	}
	
	if(!this->unread) this->unread=TRUE;
	if(this->unread_updates<this->max_updates)
		this->unread_updates++;
	
	update_viewer_mark_as_unread(update_viewer);
}/*update_viewer_increment_unread(update_viewer);*/

static void update_viewer_set_sexy_tooltip(SexyTreeView *sexy_tree_view, GtkTreePath *path, GtkTreeViewColumn *column, UpdateViewer *update_viewer){
}/*static void update_viewer_set_sexy_tooltip(sexy_tree_view, path, column, update_viewer);*/

static void update_viewer_changed_cb(SexyTreeView *update_viewer_sexy_tree_view, UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	GtkTreeSelection	*sel;
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	if(!((sel=gtk_tree_view_get_selection(GTK_TREE_VIEW(this->sexy_tree_view))) && gtk_tree_selection_get_selected(sel, &this->tree_model_sort, iter) )){
		uber_free(iter);
		control_panel_sexy_select();
		return;
	}
	
	
	gdouble		update_id, user_id;
	OnlineService	*service=NULL;
	GdkPixbuf	*pixbuf;
	gchar		*user_name, *nick_name, *date, *sexy_tweet, *text_tweet;
	gboolean	unread=FALSE;
	
	gtk_tree_model_get(
				this->tree_model_sort, iter,
					GDOUBLE_UPDATE_ID, &update_id,
					GDOUBLE_USER_ID, &user_id,
					STRING_NICK, &nick_name,
					STRING_TEXT, &text_tweet,
					STRING_SEXY_TWEET, &sexy_tweet,
					STRING_CREATED_AGO, &date,
					STRING_USER, &user_name,
					PIXBUF_AVATAR, &pixbuf,
					ONLINE_SERVICE, &service,
					GINT_SELECTED_INDEX, &this->index,
					GBOOLEAN_UNREAD, &unread,
				-1
	);
	
	gchar *update_id_str=gdouble_to_str(update_id);
	/*if(this->index < 0){*/
		debug("Searching for update ID's: %s selected_index.  For update from <%s@%s> & to: <%s>.", update_id_str, user_name, service->uri, service->guid);
		update_viewer_find_selected_update_index(update_viewer, service, user_name, update_id);
	/*}*/
	
	debug("Displaying update: #%d, ID: %s.  From <%s@%s>; To: <%s>.", this->index, update_id_str, user_name, service->uri, service->guid);
	statusbar_printf("Displaying update: #%d, ID: %s.  From <%s@%s>; To: <%s>.", this->index, update_id_str, user_name, service->uri, service->guid);
	
	control_panel_view_selected_update(service, update_id, user_id, user_name, nick_name, date, sexy_tweet, text_tweet, pixbuf);
	
	uber_free(user_name);
	uber_free(sexy_tweet);
	uber_free(text_tweet);
	uber_free(date);
	uber_free(nick_name);
	uber_free(update_id_str);
	if(pixbuf) uber_object_unref(pixbuf);
	uber_free(iter);
	
	control_panel_sexy_select();
}/*update_viewer_changed_cb(update_viewer_sexy_tree_view, update_viewer);*/

static void update_viewer_find_selected_update_index(UpdateViewer *update_viewer, OnlineService *service0, const gchar *user_name, gdouble update_id){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
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
		
		gboolean unread=FALSE;
		OnlineService *service2=NULL;
		GtkTreeIter *iter2=g_new0(GtkTreeIter, 1);
		GtkTreePath *path2=gtk_tree_path_new_from_indices(list_store_index, -1);
		if(!(gtk_tree_model_get_iter(this->tree_model, iter2, path2))){
			gtk_tree_path_free(path1);
			uber_free(iter1);
			gtk_tree_path_free(path2);
			uber_free(iter2);
			continue;
		}
		
		gtk_tree_model_get(
					this->tree_model, iter2,
						ONLINE_SERVICE, &service2,
						GBOOLEAN_UNREAD, &unread,
						GDOUBLE_UPDATE_ID, &index_update_id,
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
		
		debug("Updating UpdateViewer, for %s (timeline: %s), total updates %d selected index for update at list_store's index: %d.  Setting selecting_index to: %d marking as read.  Previous unread status: %s.", this->monitoring_string, this->timeline, this->total, i, i, (unread ?"TRUE" :"FALSE") );
		gtk_list_store_set(this->list_store, iter2, GINT_SELECTED_INDEX, i, GBOOLEAN_UNREAD, FALSE, -1);
		if( unread && this->unread_updates ){
			this->unread_updates--;
			update_viewer_mark_as_read(update_viewer);
		}
		
		online_service_update_ids_check( service2, this->timeline, index_update_id, FALSE );
		best_friends_check_update_ids( service2, user_name, index_update_id );
		gchar *user_timeline=g_strdup_printf("/%s.xml", user_name);
		online_service_update_ids_check( service2, user_timeline, index_update_id, FALSE );
		uber_free(user_timeline);
		online_services_best_friends_list_store_mark_as_read(service2, user_name, index_update_id, main_window_get_best_friends_list_store() );
		
		gtk_tree_path_free(path1);
		uber_free(iter1);
		gtk_tree_path_free(path2);
		uber_free(iter2);
		i=this->total+1;
	}
}/*update_viewer_find_selected_update_index(update_viewer, service, user_name, update_id);*/

static void update_viewer_size_cb(GtkWidget *widget, GtkAllocation *allocation, UpdateViewer *update_viewer){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	const guint8 wrap_width=10;
	gtk_word_wrap_tree_view_column(this->string_update_tree_view_column, this->string_update_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(this->string_rcpt_tree_view_column, this->string_rcpt_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(this->string_from_tree_view_column, this->string_from_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(this->created_ago_str_tree_view_column, this->created_ago_str_cell_renderer_text, wrap_width);
}/*update_viewer_size_cb(widget, allocation, update_viewer);*/

void update_viewer_set_image(UpdateViewer *update_viewer, const gchar *image_filename, GtkTreeIter *iter){
	if(!( update_viewer && IS_UPDATE_VIEWER(update_viewer) ))	return;
	UpdateViewerPrivate *this=GET_PRIVATE(update_viewer);
	
	GdkPixbuf *pixbuf;
	if(!(pixbuf=images_get_pixbuf_from_filename((gchar *)image_filename)))
		return;
	
	debug("Adding image: '%s' to UpdateViewer.", image_filename);
	gtk_list_store_set(this->list_store, iter, PIXBUF_AVATAR, pixbuf, -1);
	g_object_unref(pixbuf);
}/* update_viewer_set_image */


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

