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
#include "datetime.h"

#include "images.h"

#include "update-ids.h"
#include "online-services.rest-uris.defines.h"

#include "online-services.typedefs.h"
#include "online-services.types.h"

#include "online-services.h"

#include "online-service-request.h"
#include "online-service.types.h"
#include "online-service.h"

#include "best-friends.h"

#include "network.h"

#include "gtkbuilder.h"
#include "gconfig.h"
#include "preferences.defines.h"

#include "main-window.h"
#include "tabs.h"

#include "hotkeys.h"
#include "update-viewer.h"

#include "uberchick-tree-view.types.h"
#include "uberchick-tree-view.macros.h"
#include "uberchick-tree-view.h"

#include "update.types.h"
#include "users.types.h"
#include "groups.types.h"
#include "update.types.h"
#include "update.h"

/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
#define	GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), UBERCHICK_TYPE_TREE_VIEW, UberChickTreeViewPrivate))

typedef struct _TimelineLabels TimelineLabels;

struct _TimelineLabels{
	UpdateType	update_type;
	const gchar	*timeline;
	const gchar	*tab_label_string;
	const gchar	*menu_label_string;
}

TimelineLabelsList[]={
	{Homepage,	API_TIMELINE_HOMEPAGE,		N_("My _Hompage"),		N_("My Homepage")},
	{Replies,	API_REPLIES,			N_("@ _Replies"),		N_("@ Replies")},
	{DMs,		API_DIRECT_MESSAGES,		N_("My DMs _Inbox"),		N_("My DMs Inbox")},
	{BestFriends,	API_TIMELINE_USER,		N_("%s_%s's %s%s Updates"),	N_("%s%s's %s%s Updates")},
	{Users,		API_TIMELINE_USER,		N_("%s_%s's %s%s Updates"),	N_("%s%s's %s%s Updates")},
	{Faves,		API_FAVORITES,			N_("My Star_'d Updates"),	N_("My Star'd Updates")},
	{ReTweets,	API_RETWEETED_TO_ME,		N_("My Friends Re_Tweets"),	N_("My Friends' ReTweets")},
	{ReTweets,	API_RETWEETS_OF_ME,		N_("ReTweets of My _Updates"),	N_("ReTweets of My Updates")},
	{Searches,	API_TIMELINE_SEARCH,		N_("_Search Results for %s"),	N_("Search Results for %s")},
	{Groups,	API_STATUSNET_GROUPS_SEARCH,	N_("_%s Group Discussions"),	N_("%s Group Discussions")},
	{Timelines,	API_TIMELINE_PUBLIC,		N_("_Global Updates"),		N_("Public Updates")},
	{Archive,	API_TIMELINE_MINE,		N_("_My Updates"),		N_("My Updates")},
	{None,		NULL,				N_("Unknow_n timeline"),	N_("Unknown timeline")}
};

struct _UberChickTreeViewPrivate {
	guint			timeout_id;
	gint			page;
	OnlineService		*service;
	gchar			*user;
	
	UpdateType		update_type;
	gchar			*update_type_string;
	gint8			has_loaded;
	
	gint			minutes;
	gulong			reload;
	
	gchar			*timeline;
	gchar			*tab_label_string;
	gchar			*menu_label_string;
	
	gint			index;
	gint			selected_index;
	
	gint			total;
	gint			tree_store_index;
	
	gboolean		unread;
	guint			unread_updates;
	
	guint			connected_online_services;
	gdouble			max_updates;
	gchar			*max_updates_str;
	gdouble			maximum;
	gdouble			minimum;
	
	GtkHBox			*tab_hbox;
	GtkLabel		*tab_label;
	GtkToolbar		*tab_toolbar;
	GtkToolButton		*tab_mark_as_read_tool_button;
	GtkToolButton		*tab_close_tool_button;
	
	GtkLabel		*menu_label;
	
	GtkVBox			*main_container_vbox;
	GtkHBox			*toolbar_handlebox_hbox;
	
	GtkHandleBox		*handlebox;
	GtkToolbar		*toolbar;
	GtkToolButton		*refresh_tool_button;
	GtkToolButton		*mark_as_read_tool_button;
	
	GtkToolItem		*progress_bar_custom_tool_button;
	GtkProgressBar		*progress_bar;
	GtkAdjustment		*progress_bar_adjustment;
	
	GtkToolItem		*max_updates_custom_tool_button;
	GtkFrame		*max_updates_frame;
	GtkSpinButton		*max_updates_spin_button;
	GtkAdjustment		*max_updates_adjustment;
	GtkToolItem		*max_updates_separator_tool_item;
	
	GtkToolButton		*close_tool_button;
	GtkToggleToolButton	*auto_refresh_toggle_tool_button;
	
	GtkHSeparator		*hseparator;
	
	/* These make up the core of UberChickTreeView */
	GtkScrolledWindow	*scrolled_window;
	
	SexyTreeView		*sexy_tree_view;
	
	GtkTreeViewColumn	*pixbuf_avatar_tree_view_column;
	GtkCellRendererPixbuf	*pixbuf_avatar_cell_renderer_pixbuf;
	
	GtkTreeViewColumn	*string_detailed_update_tree_view_column;
	GtkCellRendererText	*string_detailed_update_cell_renderer_text;
	
	GtkTreeViewColumn	*string_update_tree_view_column;
	GtkCellRendererText	*string_update_cell_renderer_text;
	
	GtkTreeViewColumn	*string_from_tree_view_column;
	GtkCellRendererText	*string_from_cell_renderer_text;
	
	GtkTreeViewColumn	*string_rcpt_tree_view_column;
	GtkCellRendererText	*string_rcpt_cell_renderer_text;
	
	GtkTreeViewColumn	*created_ago_str_tree_view_column;
	GtkCellRendererText	*created_ago_str_cell_renderer_text;
	
	GtkTreeStore		*tree_store;
};



/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define DEBUG_DOMAINS "OnlineServices:UI:GtkBuilder:GtkBuildable:Networking:Updates:Requests:Notification:WWW:Settings:Setup:Start-Up:uberchick-tree-view.c"
#include "debug.h"

#define GTK_BUILDER_UI_FILENAME "uberchick-tree-view"

#define	MINIMUM_UPDATES	20.00
#define	MAXIMUM_UPDATES	100.00


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static void uberchick_tree_view_class_init(UberChickTreeViewClass *klass);
static void uberchick_tree_view_init(UberChickTreeView *uberchick_tree_view);
static void uberchick_tree_view_finalize(UberChickTreeView *uberchick_tree_view);

static void uberchick_tree_view_create_gui(UberChickTreeView *uberchick_tree_view);
static void uberchick_tree_view_set_visibility(UberChickTreeView *uberchick_tree_view);

static float uberchick_tree_view_prepare_reload(UberChickTreeView *uberchick_tree_view);
static gint uberchick_tree_view_get_reload_details(UberChickTreeView *uberchick_tree_view);

static void uberchick_tree_view_set_timeline_label(UberChickTreeView *uberchick_tree_view, const gchar *timeline);

static void uberchick_tree_view_close(GtkToolButton *close_tool_button, UberChickTreeView *uberchick_tree_view);
static void uberchick_tree_view_refresh_clicked(GtkButton *refresh_tool_button, UberChickTreeView *uberchick_tree_view);
static void uberchick_tree_view_clear(UberChickTreeView *uberchick_tree_view);

static void uberchick_tree_view_setup_icon_and_tooltip_for_auto_refresh_toggle_tool_button(GtkToggleToolButton *auto_refresh_toggle_tool_button, UberChickTreeView *uberchick_tree_view);
static void uberchick_tree_view_auto_refresh_toggle_tool_button_toggled(GtkToggleToolButton *auto_refresh_toggle_tool_button, UberChickTreeView *uberchick_tree_view);

static void uberchick_tree_view_set_maximum_updates(GtkSpinButton *max_updates_spin_button, UberChickTreeView *uberchick_tree_view);
static void uberchick_tree_view_set_adjustment(UberChickTreeView *uberchick_tree_view);

static const gchar *uberchick_tree_view_tree_store_column_to_string(UberChickTreeViewListStoreColumn uberchick_tree_view_tree_store_column);


static void uberchick_tree_view_update_tree_store_mark_all_as_read(GtkToolButton *mark_as_read_tool_button, UberChickTreeView *uberchick_tree_view);


static void uberchick_tree_view_grab_focus_cb(GtkWidget *widget, UberChickTreeView *uberchick_tree_view);
static void uberchick_tree_view_increment_unread(UberChickTreeView *uberchick_tree_view);
static void uberchick_tree_view_mark_as_unread(UberChickTreeView *uberchick_tree_view);

static gboolean uberchick_tree_view_toggle_update_detailed_column(UberChickTreeView *uberchick_tree_view);
static gboolean uberchick_tree_view_toggle_update_column(UberChickTreeView *uberchick_tree_view);
static gboolean uberchick_tree_view_toggle_created_ago_str_column(UberChickTreeView *uberchick_tree_view);

static void uberchick_tree_view_check_updates(UberChickTreeView *uberchick_tree_view);
static void uberchick_tree_view_update_age(UberChickTreeView *uberchick_tree_view, gint expiration);
static void uberchick_tree_view_rerender_detailed_update_for_row(UberChickTreeView *uberchick_tree_view, GtkTreeIter **iter, gint row_index, gint created_ago, const gchar *created_how_long_ago);
static void uberchick_tree_view_check_maximum_updates(UberChickTreeView *uberchick_tree_view);
static void uberchick_tree_view_check_inbox(UberChickTreeView *uberchick_tree_view);

static void uberchick_tree_view_set_sexy_tooltip(SexyTreeView *sexy_tree_view, GtkTreePath *path, GtkTreeViewColumn *column, UberChickTreeView *uberchick_tree_view);
static void uberchick_tree_view_update_selected(SexyTreeView *uberchick_tree_view_sexy_tree_view, UberChickTreeView *uberchick_tree_view);
static void uberchick_tree_view_resized(GtkWidget *widget, GtkAllocation *allocation, UberChickTreeView *uberchick_tree_view);

static void uberchick_tree_view_move(UberChickTreeView *uberchick_tree_view, GdkEventKey *event);

static gboolean uberchick_tree_view_index_validate(UberChickTreeView *uberchick_tree_view, gint *index);
static gboolean uberchick_tree_view_goto(UberChickTreeView *uberchick_tree_view, gint goto_index, gboolean select);
static gboolean uberchick_tree_view_goto_index(UberChickTreeView *uberchick_tree_view, gboolean select);
static gboolean uberchick_tree_view_index_select(UberChickTreeView *uberchick_tree_view);
static gboolean uberchick_tree_view_index_scroll_to(UberChickTreeView *uberchick_tree_view);
static gboolean uberchick_tree_view_scroll_to_top(UberChickTreeView *uberchick_tree_view);

static void uberchick_tree_view_iter_mark_as_read(UberChickTreeView *uberchick_tree_view, GtkTreeIter *iter, OnlineService *service, const gchar *user_name, gdouble update_id);

G_DEFINE_TYPE(UberChickTreeView, uberchick_tree_view, SEXY_TYPE_TREE_VIEW);


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
/* BEGIN: GObject core methods. */
static void uberchick_tree_view_finalize(UberChickTreeView *uberchick_tree_view){
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(this->timeline) uber_free(this->timeline);
	
	if(this->timeout_id && G_STR_N_EMPTY(this->timeline))
		program_timeout_remove(&this->timeout_id, g_strrstr(this->timeline, "/"));
	
	if(this->total) gtk_tree_store_clear(this->tree_store);
	if(this->user) uber_free(this->user);
	if(this->tab_label_string) uber_free(this->tab_label_string);
	if(this->menu_label_string) uber_free(this->menu_label_string);
	if(this->update_type_string) uber_free(this->update_type_string);
	uber_free(this->max_updates_str);
	
	if(this->tab_hbox) gtk_widget_destroy(GTK_WIDGET(this->tab_hbox));
	if(this->menu_label) gtk_widget_destroy(GTK_WIDGET(this->menu_label));
	if(this->main_container_vbox) gtk_widget_destroy(GTK_WIDGET(this->main_container_vbox));
	
	G_OBJECT_CLASS(uberchick_tree_view_parent_class)->finalize(G_OBJECT(uberchick_tree_view));
}/* uberchick_tree_view_finalized */

static void uberchick_tree_view_class_init(UberChickTreeViewClass *klass){
	GObjectClass	*object_class=G_OBJECT_CLASS(klass);
	
	object_class->finalize=(GObjectFinalizeFunc)uberchick_tree_view_finalize;
	
	g_type_class_add_private(object_class, sizeof(UberChickTreeViewPrivate));
}/* uberchick_tree_view_class_init */

static void uberchick_tree_view_init(UberChickTreeView *uberchick_tree_view){
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	this->service=NULL;
	this->user=NULL;
	
	this->has_loaded=-1;
	this->unread_updates=0;
	this->unread=FALSE;
	
	this->minutes=0;
	this->reload=0;
	
	this->connected_online_services=this->timeout_id=this->total=0;
	this->index=this->selected_index=-1;
	this->tree_store_index=-1;
	this->max_updates_str=gdouble_to_str((this->max_updates=0.0));
	this->minimum=MINIMUM_UPDATES;
	this->maximum=MAXIMUM_UPDATES;
	
	this->timeline=this->tab_label_string=this->menu_label_string=NULL;
	this->tab_hbox=NULL;
	this->menu_label=NULL;
	this->main_container_vbox=NULL;
	uberchick_tree_view_create_gui(uberchick_tree_view);
	
	g_object_set(uberchick_tree_view, "rules-hint", TRUE, "reorderable", TRUE, "headers-visible", TRUE, NULL);
	g_signal_connect(uberchick_tree_view, "grab-focus", G_CALLBACK(uberchick_tree_view_grab_focus_cb), uberchick_tree_view);
	g_signal_connect(uberchick_tree_view, "key-press-event", G_CALLBACK(hotkey_pressed), uberchick_tree_view);
}/* uberchick_tree_view_init */

UberChickTreeView *uberchick_tree_view_new(gint page, const gchar *timeline, OnlineService *service){
	if(G_STR_EMPTY(timeline)) {
		debug("**ERROR:** Cannot create update view for an empty timeline");
		return NULL;
	}
	UberChickTreeView *uberchick_tree_view=g_object_new(UBERCHICK_TYPE_TREE_VIEW, NULL);
	UberChickTreeViewPrivate *this=GET_PRIVATE((uberchick_tree_view=g_object_ref_sink(uberchick_tree_view)));
	
	debug("Creating new UberChickTreeView for timeline: %s", timeline);
	if(service && service->enabled && service->connected)
		this->service=service;
	
	uberchick_tree_view_set_page(uberchick_tree_view, page);
	uberchick_tree_view_set_timeline_label(uberchick_tree_view, timeline);
	uberchick_tree_view_setup_icon_and_tooltip_for_auto_refresh_toggle_tool_button(this->auto_refresh_toggle_tool_button, uberchick_tree_view);
	
	uberchick_tree_view_set_adjustment(uberchick_tree_view);
	uberchick_tree_view_toggle_update_detailed_column(uberchick_tree_view);
	
	gtk_widget_show_all(GTK_WIDGET(this->main_container_vbox));
	
	uberchick_tree_view_set_visibility(uberchick_tree_view);
	uberchick_tree_view_start(uberchick_tree_view);
	
	return uberchick_tree_view;
}/*uberchick_tree_view_new(timeline, NULL);*/


static void uberchick_tree_view_set_visibility(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view))) return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(this->update_type==Groups){
		uberchick_tree_view_toggle_from_column(uberchick_tree_view);
		uberchick_tree_view_toggle_rcpt_column(uberchick_tree_view);
		uberchick_tree_view_toggle_created_ago_str_column(uberchick_tree_view);
	}else if(gconfig_if_bool(CONCATENATED_UPDATES, FALSE) || gconfig_if_bool(COMPACT_VIEW, FALSE)){
		uberchick_tree_view_toggle_view(uberchick_tree_view);
		uberchick_tree_view_toggle_toolbar(uberchick_tree_view);
		uberchick_tree_view_toggle_from_column(uberchick_tree_view);
		uberchick_tree_view_toggle_rcpt_column(uberchick_tree_view);
	}else if(!gconfig_if_bool(TIMELINE_SEXY_TREE_VIEW_TOOLBAR_VISIBILITY, FALSE))
		uberchick_tree_view_toggle_toolbar(uberchick_tree_view);
	else if(!gconfig_if_bool(TIMELINE_SEXY_TREE_VIEW_FROM_COLUMN_VISIBILITY, FALSE))
		uberchick_tree_view_toggle_from_column(uberchick_tree_view);
	else if(!gconfig_if_bool(TIMELINE_SEXY_TREE_VIEW_RCPT_COLUMN_VISIBILITY, FALSE))
		uberchick_tree_view_toggle_rcpt_column(uberchick_tree_view);
	
	if(this->update_type!=DMs && this->update_type!=Replies && this->update_type!=Faves)
		return;
	
	gtk_widget_set_sensitive(GTK_WIDGET(this->max_updates_spin_button), FALSE);
	gtk_tool_item_set_visible_vertical(this->max_updates_separator_tool_item, FALSE);
	gtk_tool_item_set_visible_horizontal(this->max_updates_separator_tool_item, FALSE);
	gtk_tool_item_set_visible_vertical(this->max_updates_custom_tool_button, FALSE);
	gtk_tool_item_set_visible_horizontal(this->max_updates_custom_tool_button, FALSE);
}/*uberchick_tree_view_set_visibility(uberchick_tree_view);*/

/*BEGIN: Custom UberChickTreeView methods.*/
/**
 *@returns: -1, 0, 1 if the timeline hasn't loaded, is loading, or is re-loading.
 */
gint8 uberchick_tree_view_has_loaded(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return 0;
	return GET_PRIVATE(uberchick_tree_view)->has_loaded;
}/*uberchick_tree_view_has_loaded(uberchick_tree_view);*/

const gchar *uberchick_tree_view_get_timeline(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view))) return NULL;
	return GET_PRIVATE(uberchick_tree_view)->timeline;
}/*uberchick_tree_view_get_timeline(uberchick_tree_view);*/

GtkWidget *uberchick_tree_view_get_child_widget(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view))) return NULL;
	return GTK_WIDGET(GET_PRIVATE(uberchick_tree_view)->main_container_vbox);
}/*uberchick_tree_view_get_child(uberchick_tree_view);*/

GtkWidget *uberchick_tree_view_get_tab_widget(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view))) return NULL;
	return GTK_WIDGET(GET_PRIVATE(uberchick_tree_view)->tab_hbox);
}/*uberchick_tree_view_get_label(UberChickTreeView *uberchick_tree_view);*/

GtkWidget *uberchick_tree_view_get_menu_widget(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view))) return NULL;
	return GTK_WIDGET(GET_PRIVATE(uberchick_tree_view)->menu_label);
}/*uberchick_tree_view_get_label(UberChickTreeView *uberchick_tree_view);*/

OnlineService *uberchick_tree_view_get_service(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view) && GET_PRIVATE(uberchick_tree_view)->service)) return NULL;
	return GET_PRIVATE(uberchick_tree_view)->service;
}/*uberchick_tree_view_get_timeline(uberchick_tree_view);*/

GtkTreeStore *uberchick_tree_view_get_tree_store(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view))) return NULL;
	return GET_PRIVATE(uberchick_tree_view)->tree_store;
}/*uberchick_tree_view_get_tree_store(uberchick_tree_view);*/

gint uberchick_tree_view_get_page(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return -1;
	return GET_PRIVATE(uberchick_tree_view)->page;
}/*uberchick_tree_view_get_page(uberchick_tree_view);*/

UpdateType uberchick_tree_view_get_update_type(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return None;
	return GET_PRIVATE(uberchick_tree_view)->update_type;
}/*uberchick_tree_view_get_update_type(uberchick_tree_view);*/

const gchar *uberchick_tree_view_get_update_type_string(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view))) return NULL;
	return GET_PRIVATE(uberchick_tree_view)->update_type_string;
}/*uberchick_tree_view_get_update_type_string(UberChickTreeView *uberchick_tree_view);*/

guint uberchick_tree_view_get_notify_delay(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view))) return 100;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	return (this->page*10)+(this->update_type+10);
}/*uberchick_tree_view_get_notify_delay(uberchick_tree_view);*/

gint uberchick_tree_view_get_total(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return 0;
	return GET_PRIVATE(uberchick_tree_view)->total;
}/*uberchick_tree_view_get_total(uberchick_tree_view);*/

void uberchick_tree_view_set_page(UberChickTreeView *uberchick_tree_view, gint page){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	this->page=page;
	this->minutes=0;
}/*uberchick_tree_view_set_page(uberchick_tree_view, 0);*/

void uberchick_tree_view_start(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	gtk_progress_bar_set_fraction(this->progress_bar, uberchick_tree_view_prepare_reload(uberchick_tree_view));
	
	if(!gtk_toggle_tool_button_get_active(this->auto_refresh_toggle_tool_button)){
		debug("Creating timeout to reload %s", this->menu_label_string);
		this->timeout_id=g_timeout_add_seconds(this->reload, (GSourceFunc)uberchick_tree_view_refresh, uberchick_tree_view);
	}
	
	if(!this->has_loaded)
		network_set_state_loading_timeline(this->timeline, Load);
	else{
		uberchick_tree_view_check_updates(uberchick_tree_view);
		network_set_state_loading_timeline(this->timeline, Reload);
	}
	
	if(!this->minutes) return;
	
	if(this->service){
		online_service_request(this->service, QUEUE, this->timeline, NULL, network_display_timeline, uberchick_tree_view, NULL);
		return;
	}
	
	if(this->update_type==Groups){
		online_services_request_statusnet(QUEUE, this->timeline, NULL, network_display_timeline, uberchick_tree_view, NULL);
		return;
	}
	
	if(this->update_type==ReTweets){
		online_services_request(QUEUE, this->timeline, NULL, network_display_timeline, uberchick_tree_view, NULL);
		return;
	}
	
	online_services_request(QUEUE, this->timeline, NULL, network_display_timeline, uberchick_tree_view, NULL);
}/*uberchick_tree_view_start(UberChickTreeView *uberchick_tree_view);*/

static float uberchick_tree_view_prepare_reload(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return 0.0;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(this->has_loaded > 1 && this->minutes){
		return 1.0;
	}
	
	this->has_loaded++;
	
	if(!this->minutes){
		gconfig_get_int_or_default(PREFS_TIMELINE_RELOAD_MINUTES, &this->minutes, 3);
		/* With multiple timeline support timeline re-loading interval shouldn't be less than 5 minutes */
		if(this->minutes < 3)
			gconfig_set_int(PREFS_TIMELINE_RELOAD_MINUTES, (this->minutes=3));
		else if(this->minutes > 100)
			gconfig_set_int(PREFS_TIMELINE_RELOAD_MINUTES, (this->minutes=100));
		
#ifndef	GNOME_ENABLE_DEBUG
		this->minutes+=this->page+1;
#else
		/*this->minutes=this->page+this->update_type+1;*/
		this->minutes=this->page+1;
#endif
		gint seconds=uberchick_tree_view_get_reload_details(uberchick_tree_view);
		this->reload=this->minutes*60+seconds;
		debug("Setting %s's, timeline: %s, UberChickTreeView's minutes and initial timeout.  They'll reload evey: %lu seconds(%d minutes and %d seconds)", this->update_type_string, this->timeline, this->reload, this->minutes, seconds);
	}
	uberchick_tree_view_get_reload_details(uberchick_tree_view);
	return 0.0;
}/*uberchick_tree_view_prepare_reload(uberchick_tree_view);*/

static gint uberchick_tree_view_get_reload_details(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return 0;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	guint seconds=((this->page+1)*5)+(this->update_type*3)+10;
	if(!(this->minutes && this->reload))
		return seconds;
	
	statusbar_printf("Your %s will automatically update every %d minutes and %d seconds.", this->update_type_string, this->minutes, seconds);
	debug("Setting %s's, timeline: %s,  UberChickTreeView's timeout.  They'll reload evey: %lu seconds(%d minutes and %d seconds)", this->update_type_string, this->timeline, this->reload, this->minutes, seconds);
	return seconds;
}/*uberchick_tree_view_get_reload_details(uberchick_tree_view);*/

static void uberchick_tree_view_set_adjustment(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	guint connected_online_services=0;
	if(this->service)
		connected_online_services=1;
	else if(this->update_type==Groups)
		connected_online_services=online_services_has_connected_statusnet(0);
	else
		connected_online_services=online_services_has_connected(0);
	
	if(this->connected_online_services==connected_online_services)	return;
	
	this->connected_online_services=connected_online_services;
	
	this->minimum=connected_online_services*MINIMUM_UPDATES;
	gtk_adjustment_set_lower(this->max_updates_adjustment, this->minimum);
	gtk_adjustment_set_lower(this->progress_bar_adjustment, this->minimum);
	
	this->maximum=connected_online_services*MAXIMUM_UPDATES;
	
	if(this->update_type!=DMs && this->update_type!=Replies && this->update_type!=Faves){
		gfloat max_updates=0.0;
		gchar *timelines_gconfig_prefs_path=update_ids_format_timeline_for_gconfig(this->timeline);
		gchar *prefs_str=g_strdup_printf(PREFS_MAX_UPDATES_TO_SHOW, timelines_gconfig_prefs_path);
		gconfig_get_float(prefs_str, &max_updates);
		uber_free(prefs_str);
		uber_free(timelines_gconfig_prefs_path);
		if(max_updates > 0.0 && max_updates!=this->max_updates)
			this->max_updates=max_updates;
		if(this->max_updates>this->maximum)
			this->max_updates=this->maximum;
	}
	
	gtk_adjustment_set_upper(this->max_updates_adjustment, this->maximum);
	gtk_adjustment_set_upper(this->progress_bar_adjustment, this->maximum);
	
	gtk_adjustment_set_value(this->max_updates_adjustment, this->max_updates);
	gtk_spin_button_set_value(this->max_updates_spin_button, this->max_updates);
	
	debug("Connected UberChickTreeView's for %s's, timeline: %s, for %d OnlineServices and adjustments set-up.  Minimum: %f; Maximum: %f; Max Updates: %f", this->update_type_string, this->timeline, this->connected_online_services, this->minimum, this->maximum, this->max_updates);
}/*uberchick_tree_view_set_adjustment(uberchick_tree_view);*/

static void uberchick_tree_view_check_updates(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(!this->total) return;
	switch(this->update_type){
		case	DMs:	case	Replies:
		case	Faves:	case	BestFriends:
		case	Users:
			uberchick_tree_view_check_inbox(uberchick_tree_view);
			return;
		
		case	Homepage:	case	ReTweets:
		case	Archive:	case	Timelines:
		case	Searches:	case	Groups:
			uberchick_tree_view_check_maximum_updates(uberchick_tree_view);
			return;
		
		case	None: default:
			return;
	}
}/*uberchick_tree_view_check_updates(uberchick_tree_view);*/

static void uberchick_tree_view_set_maximum_updates(GtkSpinButton *max_updates_spin_button, UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	gdouble max_updates=gtk_spin_button_get_value(this->max_updates_spin_button);
	if(max_updates==this->max_updates)
		return;
	
	if(max_updates > this->maximum)
		max_updates=this->maximum;
	else if(max_updates < this->minimum)
		max_updates=this->minimum;
	
	if(this->update_type!=DMs && this->update_type!=Replies && this->update_type!=Faves){
		gchar *timelines_gconfig_prefs_path=update_ids_format_timeline_for_gconfig(this->timeline);
		gchar *prefs_str=g_strdup_printf(PREFS_MAX_UPDATES_TO_SHOW, timelines_gconfig_prefs_path);
		gconfig_set_float(prefs_str, max_updates);
		uber_free(prefs_str);
		uber_free(timelines_gconfig_prefs_path);
	}
	
	uber_free(this->max_updates_str);
	this->max_updates_str=gdouble_to_str((this->max_updates=max_updates));
	
	gtk_adjustment_set_value(this->max_updates_adjustment, this->max_updates);
	gtk_adjustment_set_value(this->progress_bar_adjustment, this->max_updates);
	gtk_progress_bar_set_fraction(this->progress_bar, 1.0);
}/*uberchick_tree_view_set_maximum_updates(this->max_updates_spin_button, uberchick_tree_view);*/

static void uberchick_tree_view_check_maximum_updates(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(!(this->total && this->total > this->max_updates))	return;
	
	debug("Cleaning up UberChickTreeView for %s.  UberChickTreeView's total updates: %d.  Maximum allowed updates: %f", this->timeline, this->total, this->max_updates);
	if(gconfig_if_bool(SCROLL_TO_TOP_WITH_NEW_UPDATES, TRUE)){
		debug("Moving focus to UberChickTreeView's top since no iter is currently selected selected");
		uberchick_tree_view_scroll_to_top(uberchick_tree_view);
	}
	
	gboolean selected_index_updated=FALSE;
	for(gint i=this->total-1; i>this->max_updates; i--){
		OnlineService *service=NULL;
		gboolean unread=TRUE;
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(this->tree_store), iter, path)){
			debug("Retrieving iter from path to index %d failed.  Unable to remove row", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gint tree_store_index=-1;
		gtk_tree_model_get(
				GTK_TREE_MODEL(this->tree_store), iter,
					ONLINE_SERVICE, &service,
					GBOOLEAN_UNREAD, &unread,
					ONLINE_SERVICE, &service,
					GINT_LIST_STORE_INDEX, &tree_store_index,
				-1
		);
		debug("Removing iter for <%s>'s %s at index: %d; tree_store_index: %d", service->guid, this->update_type_string, i, tree_store_index);
		
		debug("Removing <%s>'s %s at index %i which exceeds maximum updates: %s.", service->guid, this->update_type_string, i, this->max_updates_str);
		
		if(!selected_index_updated && this->selected_index>-1 && tree_store_index>-1 && this->tree_store_index>-1 && this->selected_index==tree_store_index){
			selected_index_updated=TRUE;
			this->selected_index=-1;
			
			if(uberchick_tree_view_scroll_to_top(uberchick_tree_view))
				debug("UberChickTreeView for %s(timeline %s) focus moved to row 0", this->update_type_string, this->timeline);
		}
		
		gtk_tree_store_remove(this->tree_store, iter);
		this->total--;
		
		gtk_tree_path_free(path);
		uber_free(iter);
	}
	uberchick_tree_view_update_age(uberchick_tree_view, 0);
}/*uberchick_tree_view_check_maximum_updates(uberchick_tree_view);*/

static void uberchick_tree_view_check_inbox(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(!this->total) return;
	
	gint update_expiration=0;
	switch(this->update_type){
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
			/*By default "Best Friends' Updates" from the last 7 days are loaded.*/
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_BEST_FRIENDS, &update_expiration, 604800);
			break;
		
		case	Homepage:	case	ReTweets:
		case	Timelines:	case	Users:
		case	Searches:	case	Groups:
		case	Archive:
			break;
		
		case None:
			return;
	}
	
	uberchick_tree_view_update_age(uberchick_tree_view, update_expiration);
}/*uberchick_tree_view_check_inbox(uberchick_tree_view);*/

static void uberchick_tree_view_update_age(UberChickTreeView *uberchick_tree_view, gint expiration){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(!this->total) return;
	
	gboolean selected_index_updated=FALSE;
	for(gint i=this->total-1; i>-1; i--){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(this->tree_store), iter, path)){
			debug("Retrieving iter from path to index %d failed.  Unable to update row's created_ago time", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gdouble update_id=0.0;
		
		OnlineService	*service=NULL;
		gint tree_store_index=-1;
		gint created_ago=0;
		gboolean unread=TRUE;
		gchar *created_at_str=NULL, *created_how_long_ago=NULL;
		gtk_tree_model_get(
				GTK_TREE_MODEL(this->tree_store), iter,
					GDOUBLE_UPDATE_ID, &update_id,
					ONLINE_SERVICE, &service,
					GINT_LIST_STORE_INDEX, &tree_store_index,
					STRING_CREATED_AT, &created_at_str,
					GBOOLEAN_UNREAD, &unread,
				-1
		);
		
		if(this->tree_store_index>-1 && i>=this->tree_store_index)
			tree_store_index+=this->tree_store_index+1;
		
		datetime_strp_age(created_at_str, &created_how_long_ago, &created_ago, (this->update_type==Searches ?FALSE :TRUE));
		if(expiration > 0 && created_ago > 0 && created_ago > expiration){
			if(!selected_index_updated && this->selected_index>-1 && tree_store_index>-1 && this->tree_store_index>-1 && this->selected_index==tree_store_index){
				selected_index_updated=TRUE;
				debug("Updating selected_index for <%s>'s %s(timeline %s), previous selected_index: %d; new selected_index: %d", service->guid, this->update_type_string, this->timeline, this->selected_index, -1);
				this->selected_index=-1;
				
				if(uberchick_tree_view_scroll_to_top(uberchick_tree_view))
					debug("UberChickTreeView for %s(timeline %s) focus moved to 0", this->update_type_string, this->timeline);
			}
			
			debug("Removing UberChickTreeView iter for <%s>'s %s at index: %d; tree_store_index: %d", service->guid, this->update_type_string, i, tree_store_index);
			debug("Removing <%s>'s expired %s.  Oldest %s allowed: [%d] it was posted %d.", service->guid, this->update_type_string, this->update_type_string, expiration, created_ago);
			
			gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
			update_ids_get(service, this->timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
			if(update_id > oldest_update_id)
				update_ids_set(service, this->timeline, newest_update_id, unread_update_id, update_id);
			
			gtk_tree_store_remove(this->tree_store, iter);
			if(unread && this->total > -1 && this->unread_updates>=this->total && (this->has_loaded > 0 || (this->update_type==DMs || this->update_type==Replies || this->update_type==BestFriends)))
				this->unread_updates--;
			this->total--;
		}else{
			if(!selected_index_updated && this->selected_index>-1 && tree_store_index>-1 && this->tree_store_index>-1 && this->selected_index==tree_store_index){
				debug("Updating selected_index for <%s>'s %s(timeline %s), previous selected_index: %d; new selected_index: %d", service->guid, this->update_type_string, this->timeline, this->selected_index, ((this->selected_index+this->tree_store_index>=this->total+1) ?-1 :(this->selected_index+this->tree_store_index+1)));
				selected_index_updated=TRUE;
				if((this->selected_index+=this->tree_store_index+1) >= this->total) this->selected_index=-1;
			}
			
			uberchick_tree_view_rerender_detailed_update_for_row(uberchick_tree_view, &iter, i, created_ago, created_how_long_ago);
		}
		uber_free(created_how_long_ago);
		uber_free(created_at_str);
		service=NULL;
		gtk_tree_path_free(path);
		uber_free(iter);
	}
	if(this->tree_store_index>-1) this->tree_store_index=-1;
}/*uberchick_tree_view_update_age(uberchick_tree_view, 0);*/

static void uberchick_tree_view_rerender_detailed_update_for_row(UberChickTreeView *uberchick_tree_view, GtkTreeIter **iter, gint row_index, gint created_ago, const gchar *created_how_long_ago){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(!this->total) return;
	
	OnlineService	*service=NULL;
	
	gchar		*sexy_status_update=NULL;
	gchar		*user_nick_name=NULL;
	gchar		*user_user_name=NULL;
	
	gboolean	retweet=FALSE;
	gchar		*retweeted_user_name=NULL;
	gchar		*retweeted_user_nick=NULL;
	gchar		*retweeted_markup=NULL;
	
	gtk_tree_model_get(
			GTK_TREE_MODEL(this->tree_store), *iter,
				STRING_USER, &user_user_name,			/* Useruser_name string.			*/
				STRING_NICK, &user_nick_name,			/* Author user_name string.			*/
				ONLINE_SERVICE, &service,			/* OnlineService pointer.			*/
				STRING_SEXY_STATUS_UPDATE, &sexy_status_update,		/* Update's string as markup for display.	*/
				GBOOLEAN_RETWEET, &retweet,			/* If the update's a retweet or not		*/
				GCHARARRY_RETWEETED_USER_NAME, &retweeted_user_name,	/* Who retweeted this update. "" for non-retweets.	*/
				GCHARARRY_RETWEETED_USER_NICK, &retweeted_user_nick,	/* Who retweeted this update. "" for non-retweets.	*/
				GCHARARRY_RETWEETED_MARKUP, &retweeted_markup,		/* Who retweeted this update. "" for non-retweets.	*/
			-1
	);
	
	gchar *sexy_complete=g_strdup_printf("%s%s%s%s%s<span weight=\"bold\" size=\"small\">From:</span> <span weight=\"bold\" size=\"small\" underline=\"single\">%s &lt;%s@%s&gt;</span>\n<span style=\"italic\" size=\"x-small\">To:</span> <span style=\"italic\" size=\"x-small\" underline=\"single\">%s &lt;%s&gt;</span>\n\t%s%s%s\n<span size=\"small\" variant=\"smallcaps\">Received: %s</span>\n",
					(retweet ?"<span size=\"small\" weight=\"ultrabold\" variant=\"smallcaps\">":""),
					(retweet ?_("retweeted by") :""),
					(retweet ?": " :""),
					(retweet ?retweeted_markup :""),
					(retweet ?"</span>\n" :""),
					user_nick_name, user_user_name, service->uri,
					service->nick_name, service->key,
					((this->update_type==DMs)
					  	?"<span weight=\"ultrabold\" style=\"italic\" variant=\"smallcaps\">[Direct Message]</span>\n<span weight=\"ultrabold\" style=\"italic\">["
						:(this->update_type==Replies ?"<span style=\"italic\" variant=\"smallcaps\">[@ reply]</span>\n<span style=\"italic\">["
							:""
						)
					),
					sexy_status_update,
					((this->update_type==DMs||this->update_type==Replies) ?"]</span>" :""),
					created_how_long_ago
	);
	
	gtk_tree_store_set(
			this->tree_store, *iter,
				STRING_DETAILED_UPDATE, sexy_complete,		/* Upate's string as markup w/full details.		*/
				STRING_CREATED_AGO, created_how_long_ago,	/* (seconds|minutes|hours|day) ago.			*/
				GINT_CREATED_AGO, created_ago,			/* How old the post is, in seconds, for sorting.	*/
				GINT_LIST_STORE_INDEX, row_index,		/* the row's list store index.				*/
			-1
	);
	
	service=NULL;
	uber_free(user_nick_name);
	uber_free(user_user_name);
	uber_free(sexy_status_update);
	uber_free(sexy_complete);
	uber_free(retweeted_user_name);
	uber_free(retweeted_user_nick);
	uber_free(retweeted_markup);
}/*uberchick_tree_view_rerender_detailed_update_for_row(uberchick_tree_view, &iter, row_index, created_ago, created_ago_string);*/

const gchar *uberchick_tree_view_tree_store_column_to_string(UberChickTreeViewListStoreColumn uberchick_tree_view_tree_store_column){
	switch(uberchick_tree_view_tree_store_column){
		case	ONLINE_SERVICE: return _("ONLINE_SERVICE");
		case	STRING_USER: return _("STRING_USER");
		case	STRING_NICK: return _("STRING_NICK");
		case	STRING_TEXT: return _("STRING_TEXT");
		case	STRING_UPDATE: return _("STRING_UPDATE");
		case	STRING_SEXY_UPDATE: return _("STRING_SEXY_UPDATE");
		case	STRING_DETAILED_UPDATE: return _("STRING_DETAILED_UPDATE");
		case	STRING_CREATED_AGO: return _("STRING_CREATED_AGO");	
		case	STRING_CREATED_AT: return _("STRING_CREATED_AT");
		case	STRING_FROM: return _("STRING_FROM");
		case	STRING_RCPT: return _("STRING_RCPT");
		case	PIXBUF_AVATAR: return _("PIXBUF_AVATAR");
		case	GUINT_UBERCHICK_TREE_VIEW_INDEX: return _("GUINT_UBERCHICK_TREE_VIEW_INDEX");
		case	GDOUBLE_UPDATE_ID: return _("GDOUBLE_UPDATE_ID");
		case	GSTRING_UPDATE_ID_STR: return _("GSTRING_UPDATE_ID_STR");				/*	Update's ID string.	*/
		case	GDOUBLE_USER_ID: return _("GDOUBLE_USER_ID");
		case	GINT_CREATED_AGO: return _("GINT_CREATED_AGO");
		case	ULONG_CREATED_AT: return _("ULONG_CREATED_AT");
		case	GINT_LIST_STORE_INDEX: return _("GINT_LIST_STORE_INDEX");
		case	GBOOLEAN_UNREAD: return _("GBOOLEAN_UNREAD");
		case	GBOOLEAN_RETWEET: return _("GBOOLEAN_RETWEET");
		case	GDOUBLE_RETWEET_UPDATE_ID: return _("GDOUBLE_RETWEET_UPDATE_ID");
		case	GCHARARRY_RETWEETED_USER_NAME: return _("GCHARARRY_RETWEETED_USER_NAME");
		case	GCHARARRY_RETWEETED_USER_NICK: return _("GCHARARRY_RETWEETED_USER_NICK");
		case	GCHARARRY_RETWEETED_MARKUP: return _("GCHARARRY_RETWEETED_MARKUP");
		case	STRING_SEXY_STATUS_UPDATE: return _("STRING_SEXY_STATUS_UPDATE");
		default: return _("UNKNOWN COLUMN");
	}
}/*uberchick_tree_view_tree_store_column_to_string(uberchick_tree_view_tree_store_column);*/

void uberchick_tree_view_update_tree_store(UberChickTreeView *uberchick_tree_view, UberChickTreeViewListStoreColumn uberchick_tree_view_tree_store_column, gpointer value, gboolean tree_store_remove){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(!this->total) return;
	
	OnlineService *service_previous=NULL;
	gboolean unread_found=FALSE;
	for(gint i=this->total-1; i>-1; i--){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter(GTK_TREE_MODEL(this->tree_store), iter, path))){
			debug("Retrieving iter from path to index %d failed.  Unable to %s row", i, (tree_store_remove ?_("remove") :_("update")));
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		OnlineService *service=NULL;
		gchar *user_name=NULL;
		gboolean unread=FALSE;
		gdouble update_id=0.0;
		gint tree_store_index=-1;
		gpointer value_at_index=NULL;
		gtk_tree_model_get(
					GTK_TREE_MODEL(this->tree_store), iter,
						ONLINE_SERVICE, &service,
						STRING_USER, &user_name,
						GBOOLEAN_UNREAD, &unread,
						GDOUBLE_UPDATE_ID, &update_id,
						GINT_LIST_STORE_INDEX, &tree_store_index,
						uberchick_tree_view_tree_store_column, &value_at_index,
					-1
		);
		
		if(!(service_previous && service_previous==service)){
			debug("Updating <%s>'s %s UberChicTreeView; %s tree_store_index: %s(#%i)", service->guid, this->update_type_string, (tree_store_remove ?_("Removing") :_("Updating")), uberchick_tree_view_tree_store_column_to_string(uberchick_tree_view_tree_store_column), tree_store_index);
			service_previous=service;
		}
		switch(uberchick_tree_view_tree_store_column){
			case	ONLINE_SERVICE:
				if(value!=value_at_index){
					uber_free(user_name);
					gtk_tree_path_free(path);
					uber_free(iter);
					continue;
				}
				break;
			
			case	GSTRING_UPDATE_ID_STR:
			case	STRING_USER:
			case	STRING_NICK:
			case	STRING_TEXT:
			case	STRING_UPDATE:
			case	STRING_SEXY_UPDATE:
			case	STRING_DETAILED_UPDATE:
			case	STRING_CREATED_AGO:
			case	STRING_CREATED_AT:
			case	STRING_FROM:
			case	STRING_RCPT:
			case	GCHARARRY_RETWEETED_USER_NAME:
			case	GCHARARRY_RETWEETED_USER_NICK:
			case	GCHARARRY_RETWEETED_MARKUP:
			case	STRING_SEXY_STATUS_UPDATE:
				if(!g_str_equal((gchar *)value, (gchar *)value_at_index)){
					uber_free(value_at_index);
					uber_free(user_name);
					gtk_tree_path_free(path);
					uber_free(iter);
					continue;
				}
				break;
			
			case	PIXBUF_AVATAR:
				if(value_at_index==value){
					uber_object_unref(value_at_index);
					uber_free(user_name);
					gtk_tree_path_free(path);
					uber_free(iter);
					continue;
				}
				break;
			
			case	GBOOLEAN_UNREAD:
				if((gboolean)GPOINTER_TO_INT(value)==FALSE || tree_store_remove)
					uberchick_tree_view_iter_mark_as_read(uberchick_tree_view, iter, service, user_name, update_id);
				break;
			
			case	GBOOLEAN_RETWEET:
				if((gboolean)GPOINTER_TO_INT(value_at_index)==(gboolean)GPOINTER_TO_INT(value)){
					uber_free(user_name);
					gtk_tree_path_free(path);
					uber_free(iter);
					continue;
				}
				break;
			
			case	GINT_CREATED_AGO:
				if(GPOINTER_TO_INT(value_at_index)<=GPOINTER_TO_INT(value)){
					uber_free(user_name);
					gtk_tree_path_free(path);
					uber_free(iter);
					continue;
				}
				break;
			
			case	GUINT_UBERCHICK_TREE_VIEW_INDEX:
			case	GDOUBLE_UPDATE_ID:
			case	GDOUBLE_USER_ID:
			case	GDOUBLE_RETWEET_UPDATE_ID:
			case	ULONG_CREATED_AT:
			case	GINT_LIST_STORE_INDEX:
			default:
				uber_free(user_name);
				gtk_tree_path_free(path);
				uber_free(iter);
				return;
				break;
		}
		
		if(!tree_store_remove){
			if(unread && !unread_found)
				unread_found=TRUE;
			gtk_tree_store_set(
					this->tree_store, iter,
						uberchick_tree_view_tree_store_column, value,
					-1
			);
		}else{
			if(unread){
				if(!unread_found) unread_found=TRUE;
				uberchick_tree_view_iter_mark_as_read(uberchick_tree_view, iter, service, user_name, update_id);
			}
			gtk_tree_store_remove(this->tree_store, iter);
			this->total--;
			if(i==this->selected_index){
				this->selected_index=-1;
				debug("Moving focus of UberChickTreeView to top as the current selected iter's been removed");
				uberchick_tree_view_scroll_to_top(uberchick_tree_view);
			}
		}
		uber_free(user_name);
		gtk_tree_path_free(path);
		uber_free(iter);
	}
}/*uberchick_tree_view_update_tree_store(uberchick_tree_view, uberchick_tree_view_tree_store_column, value, TRUE|FALSE);*/

static void uberchick_tree_view_update_tree_store_mark_all_as_read(GtkToolButton *mark_as_read_tool_button, UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	debug("Marking all %s updates as read", this->update_type_string);
	uberchick_tree_view_update_tree_store(uberchick_tree_view, GBOOLEAN_UNREAD, GINT_TO_POINTER(FALSE), FALSE);
	uberchick_tree_view_mark_as_read(uberchick_tree_view);
}/*uberchick_tree_view_update_tree_store_mark_all_as_read(this->tab_mark_as_read_tool_button|this->mark_as_read_tool_button, uberchick_tree_view);*/

static void uberchick_tree_view_refresh_clicked(GtkButton *refresh_tool_button, UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	uberchick_tree_view_clear(uberchick_tree_view);
	uberchick_tree_view_refresh(uberchick_tree_view);
}/*uberchick_tree_view_refresh_clicked(uberchick_tree_view);*/

gboolean uberchick_tree_view_refresh(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view))) return FALSE;
	uberchick_tree_view_stop(uberchick_tree_view);
	uberchick_tree_view_set_adjustment(uberchick_tree_view);
	uberchick_tree_view_start(uberchick_tree_view);
	return FALSE;
}/*uberchick_tree_view_refresh(uberchick_tree_view);*/

void uberchick_tree_view_complete(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(!this->connected_online_services)	return;
	
	debug("UberChickTreeView for %s, timeline: %s, completed processing new %d new updates out of %d total updates", this->update_type_string, this->timeline, this->unread_updates, this->total);
	gtk_progress_bar_set_fraction(this->progress_bar, 1.0);
	uberchick_tree_view_update_age(uberchick_tree_view, 0);
	if(this->unread)
		uberchick_tree_view_mark_as_unread(uberchick_tree_view);	
	
	if(gconfig_if_bool(SCROLL_TO_TOP_WITH_NEW_UPDATES, TRUE))
		uberchick_tree_view_scroll_to_top(uberchick_tree_view);
}/*uberchick_tree_view_complete(uberchick_tree_view);*/

static void uberchick_tree_view_auto_refresh_toggle_tool_button_toggled(GtkToggleToolButton *auto_refresh_toggle_tool_button, UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	
	uberchick_tree_view_setup_icon_and_tooltip_for_auto_refresh_toggle_tool_button(auto_refresh_toggle_tool_button, uberchick_tree_view);
	if(!gtk_toggle_tool_button_get_active(auto_refresh_toggle_tool_button))
		uberchick_tree_view_refresh(uberchick_tree_view);
	else
		uberchick_tree_view_stop(uberchick_tree_view);
}/*uberchick_tree_view_auto_refresh_toggle_tool_button_toggled(auto_refresh_toggle_tool_button, uberchick_tree_view);*/

static void uberchick_tree_view_setup_icon_and_tooltip_for_auto_refresh_toggle_tool_button(GtkToggleToolButton *auto_refresh_toggle_tool_button, UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	gboolean auto_refresh_disabled=gtk_toggle_tool_button_get_active(this->auto_refresh_toggle_tool_button);
	
	gchar *stock_icon_name_for_toggle_tool_button=g_strdup_printf("gtk-media-%s", (auto_refresh_disabled ?"play" :"pause"));
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(this->auto_refresh_toggle_tool_button), stock_icon_name_for_toggle_tool_button);
	uber_free(stock_icon_name_for_toggle_tool_button);
	
	gchar *label_for_toggle_tool_button=g_strdup_printf("<span weight=\"bold\">%sable auto_-reloading of %s.</span>", (auto_refresh_disabled ?"Dis" :"En"), this->update_type_string);
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(this->auto_refresh_toggle_tool_button), label_for_toggle_tool_button);
	uber_free(label_for_toggle_tool_button);
	
	gchar *tooltip_markup_for_toggle_tool_button=g_strdup_printf("<span weight=\"bold\">St%s auto re-loading %s.</span>", (auto_refresh_disabled ?"art" :"op"), this->update_type_string);
	gtk_widget_set_tooltip_markup(GTK_WIDGET(this->auto_refresh_toggle_tool_button), tooltip_markup_for_toggle_tool_button);
	uber_free(tooltip_markup_for_toggle_tool_button);
}/*uberchick_tree_view_setup_icon_and_tooltip_for_auto_refresh_toggle_tool_button(this->auto_refresh_toggle_tool_button, uberchick_tree_view);*/

void uberchick_tree_view_stop(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(this->timeout_id && G_STR_N_EMPTY(this->timeline))
		program_timeout_remove(&this->timeout_id, g_strrstr(this->timeline, "/"));
}/*uberchick_tree_view_stop(uberchick_tree_view);*/

static void uberchick_tree_view_close(GtkToolButton *close_tool_button, UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	tabs_close_page(GET_PRIVATE(uberchick_tree_view)->page);
}/*uberchick_tree_view_close(this->tab_close_tool_button, uberchick_tree_view);*/

static void uberchick_tree_view_set_timeline_label(UberChickTreeView *uberchick_tree_view, const gchar *timeline){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view) && G_STR_N_EMPTY(timeline)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	TimelineLabels *timeline_labels=NULL;
	this->timeline=g_strdup(timeline);
	for(timeline_labels=TimelineLabelsList; timeline_labels->timeline; timeline_labels++){
		if(g_str_has_prefix(this->timeline, timeline_labels->timeline) || g_str_equal(this->timeline, timeline_labels->timeline)){
			this->update_type=timeline_labels->update_type;
			if(this->update_type!=Searches&&this->update_type!=Groups){
				this->tab_label_string=g_strdup(timeline_labels->tab_label_string);
				this->menu_label_string=g_strdup(timeline_labels->menu_label_string);
			}else{
				this->tab_label_string=g_strdup_printf(timeline_labels->tab_label_string, g_uri_unescape_string((g_strrstr(this->timeline, "?q=")+sizeof("?q=")-1), NULL));
				this->menu_label_string=g_strdup_printf(timeline_labels->menu_label_string, g_uri_unescape_string((g_strrstr(this->timeline, "?q=")+sizeof("?q=")-1), NULL));
			}
			break;
		}
		
		if(timeline_labels->update_type!=BestFriends && timeline_labels->update_type!=Users)
			continue;
		
		
		if(!g_str_has_prefix(this->timeline, "/statuses/user_timeline/"))
			continue;
		
		/* This checks for 'update_type of 'BestFriends' updates 1st & than 'Users' updates but no others. */
		gchar **feed_info=g_strsplit_set(timeline, "/.", -1);
		gchar **user_info=g_strsplit_set(feed_info[3], ".", -1);
		const gchar *user_name=user_info[0];
		if(G_STR_EMPTY(user_name)){
			g_strfreev(feed_info);
			g_strfreev(user_info);
			continue;
		}
		
		this->user=g_strdup(user_name);
		gboolean user_is_best_friend=best_friends_is_user_best_friend(this->service, this->user);
		gboolean viewing_newest_updates=TRUE;
		if(!g_strrstr(this->timeline, "?since_id="))
			viewing_newest_updates=FALSE;
		if(!(this->service && viewing_newest_updates && user_is_best_friend))
			timeline_labels++;
		
		this->timeline=g_strdup_printf(timeline_labels->timeline, this->user);
		this->tab_label_string=g_strdup_printf(timeline_labels->tab_label_string, (user_is_best_friend?"My Best Friend: " :""), this->user, (viewing_newest_updates?"Newest ":""), this->service->uri);
		this->menu_label_string=g_strdup_printf(timeline_labels->menu_label_string, (user_is_best_friend?"My Best Friend: " :""), this->user, (viewing_newest_updates?"Newest ":""), this->service->uri);
		if(user_is_best_friend && viewing_newest_updates)
			this->update_type=BestFriends;
		else
			this->update_type=timeline_labels->update_type;
		g_strfreev(feed_info);
		g_strfreev(user_info);
		break;
	}
	if(!this->menu_label_string){
		debug("**ERROR:** Unable to determine timeline label to use for the timeline: %s", this->timeline);
		this->tab_label_string=g_strdup(timeline_labels->tab_label_string);
		this->menu_label_string=g_strdup(timeline_labels->menu_label_string);
	}
	
	this->update_type_string=g_strdup(update_type_to_string(this->update_type));
	uberchick_tree_view_mark_as_read(uberchick_tree_view);
}/*uberchick_tree_view_set_timeline_label(uberchick_tree_view, timeline);*/

static void uberchick_tree_view_create_gui(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	GtkBuilder *gtk_builder_ui=gtkbuilder_get_file(
							GTK_BUILDER_UI_FILENAME,
								"uberchick_tree_view_tab_hbox", &this->tab_hbox,
								"uberchick_tree_view_tab_label", &this->tab_label,
								"uberchick_tree_view_tab_toolbar", &this->tab_toolbar,
								"uberchick_tree_view_tab_mark_as_read_tool_button", &this->tab_mark_as_read_tool_button,
								"uberchick_tree_view_tab_close_tool_button", &this->tab_close_tool_button,
								
								"uberchick_tree_view_menu_label", &this->menu_label,
								
								"uberchick_tree_view_main_container_vbox", &this->main_container_vbox,
								"uberchick_tree_view_toolbar_handlebox_hbox", &this->toolbar_handlebox_hbox,
								
								"uberchick_tree_view_handlebox", &this->handlebox,
								"uberchick_tree_view_toolbar", &this->toolbar,
								"uberchick_tree_view_refresh_tool_button", &this->refresh_tool_button,
								"uberchick_tree_view_mark_as_read_tool_button", &this->mark_as_read_tool_button,
								"uberchick_tree_view_close_tool_button", &this->close_tool_button,

								"uberchick_tree_view_progress_bar_custom_tool_button", &this->progress_bar_custom_tool_button,
								"uberchick_tree_view_progress_bar", &this->progress_bar,
								"uberchick_tree_view_progress_bar_adjustment", &this->progress_bar_adjustment,
								
								"uberchick_tree_view_max_updates_custom_tool_button", &this->max_updates_custom_tool_button,
								"uberchick_tree_view_max_updates_separator_tool_item", &this->max_updates_separator_tool_item,
								"uberchick_tree_view_max_updates_frame", &this->max_updates_frame,
								"uberchick_tree_view_max_updates_spin_button", &this->max_updates_spin_button,
								"uberchick_tree_view_max_updates_adjustment", &this->max_updates_adjustment,
								
								"uberchick_tree_view_auto_refresh_toggle_tool_button", &this->auto_refresh_toggle_tool_button,
								
								"uberchick_tree_view_scrolled_window", &this->scrolled_window,
								
								"uberchick_tree_view_hseparator", &this->hseparator,
								
								/* These make up the core of UberChickTreeView */
								"uberchick_tree_view_sexy_tree_view", &this->sexy_tree_view,
								
								"uberchick_tree_view_tree_store", &this->tree_store,
								
								"uberchick_tree_view_pixbuf_avatar_tree_view_column", &this->pixbuf_avatar_tree_view_column,
								"uberchick_tree_view_pixbuf_avatar_cell_renderer_pixbuf", &this->pixbuf_avatar_cell_renderer_pixbuf,
								
								"uberchick_tree_view_string_detailed_update_tree_view_column", &this->string_detailed_update_tree_view_column,
								"uberchick_tree_view_string_detailed_update_cell_renderer_text", &this->string_detailed_update_cell_renderer_text,
								"uberchick_tree_view_string_update_tree_view_column", &this->string_update_tree_view_column,
								"uberchick_tree_view_string_update_cell_renderer_text", &this->string_update_cell_renderer_text,
								
								"uberchick_tree_view_string_from_tree_view_column", &this->string_from_tree_view_column,
								"uberchick_tree_view_string_from_cell_renderer_text", &this->string_from_cell_renderer_text,
								
								"uberchick_tree_view_string_rcpt_tree_view_column", &this->string_rcpt_tree_view_column,
								"uberchick_tree_view_string_rcpt_cell_renderer_text", &this->string_rcpt_cell_renderer_text,
								
								"uberchick_tree_view_created_ago_str_tree_view_column", &this->created_ago_str_tree_view_column,
								"uberchick_tree_view_created_ago_str_cell_renderer_text", &this->created_ago_str_cell_renderer_text,
							NULL
	);
	
	gtkbuilder_connect(
			gtk_builder_ui, uberchick_tree_view,
				"uberchick_tree_view_tab_refresh_tool_button",  "clicked", uberchick_tree_view_refresh_clicked,
				"uberchick_tree_view_tab_mark_as_read_tool_button",  "clicked", uberchick_tree_view_update_tree_store_mark_all_as_read,
				"uberchick_tree_view_tab_close_tool_button",  "clicked", uberchick_tree_view_close,
				
				"uberchick_tree_view_mark_as_read_tool_button",  "clicked", uberchick_tree_view_update_tree_store_mark_all_as_read,
				"uberchick_tree_view_refresh_tool_button", "clicked", uberchick_tree_view_refresh_clicked,
				
				"uberchick_tree_view_max_updates_spin_button", "value-changed", uberchick_tree_view_set_maximum_updates,
				"uberchick_tree_view_close_tool_button", "clicked", uberchick_tree_view_close,
				
				"uberchick_tree_view_scrolled_window", "grab-focus", uberchick_tree_view_grab_focus_cb,
				"uberchick_tree_view_scrolled_window", "size-allocate", uberchick_tree_view_resized,
				
				"uberchick_tree_view_sexy_tree_view", "grab-focus", uberchick_tree_view_grab_focus_cb,
				
				"uberchick_tree_view_sexy_tree_view", "get-tooltip", uberchick_tree_view_set_sexy_tooltip,
				
				"uberchick_tree_view_sexy_tree_view", "cursor-changed", uberchick_tree_view_update_selected,
				"uberchick_tree_view_sexy_tree_view", "size-allocate", uberchick_tree_view_resized,
				"uberchick_tree_view_sexy_tree_view", "key-press-event", hotkey_pressed,
			NULL
		);
	
	gtkbuilder_connect_after(
			gtk_builder_ui, uberchick_tree_view,
				"uberchick_tree_view_auto_refresh_toggle_tool_button", "toggled", uberchick_tree_view_auto_refresh_toggle_tool_button_toggled,
			NULL
	);
	
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(this->tree_store), GINT_CREATED_AGO, GTK_SORT_ASCENDING);
	
	sexy_tree_view_set_tooltip_label_column(this->sexy_tree_view, STRING_SEXY_UPDATE);
}/*uberchick_tree_view_create_gui(uberchick_tree_view);*/

void uberchick_tree_view_key_pressed(UberChickTreeView *uberchick_tree_view, GdkEventKey *event){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	if(event->keyval!=GDK_Return && event->keyval!=GDK_KP_Enter) return uberchick_tree_view_move(uberchick_tree_view, event);
	
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
}/*uberchick_tree_view_key_pressed(widget, event);*/

static void uberchick_tree_view_move(UberChickTreeView *uberchick_tree_view, GdkEventKey *event){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	gint row_index;
	switch(event->state){
		case GDK_MOD1_MASK|GDK_CONTROL_MASK:
		switch(event->keyval){
			case GDK_A: case GDK_a:
				if(GTK_WIDGET_IS_SENSITIVE(GTK_WIDGET(this->tab_mark_as_read_tool_button))){
					uberchick_tree_view_update_tree_store_mark_all_as_read(this->tab_mark_as_read_tool_button, uberchick_tree_view);
					return;
				}
				break;
		}
		case GDK_SHIFT_MASK:
			row_index=this->selected_index;
			break;
		
		default:
			row_index=this->index;
			break;
	}
	gboolean supported_keyval=TRUE;
	switch(event->keyval){
		case GDK_Tab: case GDK_KP_Tab:
			this->selected_index=0;
			uberchick_tree_view_index_select(uberchick_tree_view);
			return;
		
		case GDK_Home: case GDK_KP_Home:
		case GDK_Begin: case GDK_Escape:
			row_index=0;
			break;
		
		case GDK_End: case GDK_KP_End:
			row_index=this->total-1;
			break;
		
		case GDK_Up: case GDK_KP_Up: case GDK_KP_Prior:
			row_index--;
			break;
		
		case GDK_Down: case GDK_KP_Down: case GDK_KP_Next:
			row_index++;
			break;
		
		case GDK_Page_Up:
			row_index-=3;
			break;
		
		case GDK_Page_Down:
			if(row_index<1)
				row_index+=5;
			else
				row_index+=3;
			break;
		
		default:
			supported_keyval=FALSE;
			break;
	}//switch
	
	if(!supported_keyval) return;
	switch(event->state){
		case GDK_MOD1_MASK|GDK_CONTROL_MASK:
			this->selected_index=row_index;
			uberchick_tree_view_index_select(uberchick_tree_view);
			break;
		
		case GDK_MOD1_MASK: case GDK_CONTROL_MASK: case GDK_SHIFT_MASK:
			this->selected_index=this->index=row_index;
			uberchick_tree_view_index_select(uberchick_tree_view);
			break;
		
		default:
			this->index=row_index;
			uberchick_tree_view_index_scroll_to(uberchick_tree_view);
			break;
	}
}/* uberchick_tree_view_move(uberchick_tree_view, event); */

static gboolean uberchick_tree_view_index_validate(UberChickTreeView *uberchick_tree_view, gint *index){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(*index > -1 && *index < this->total) return TRUE;
	*index=((*index)>=this->total ?this->total :0);
	return FALSE;
}/*uberchick_tree_view_index_validate(uberchick_tree_view, &index);*/

static gboolean uberchick_tree_view_goto(UberChickTreeView *uberchick_tree_view, gint goto_index, gboolean select){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(!(GTK_IS_TREE_VIEW(GTK_TREE_VIEW(this->sexy_tree_view)) && this->total))
		return FALSE;
	
	uberchick_tree_view_index_validate(uberchick_tree_view, &goto_index);
	
	GtkTreePath *path=gtk_tree_path_new_from_indices(goto_index, -1);
	if(!select) gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(this->sexy_tree_view), path, NULL, FALSE, 0.0, 0.0);
	else gtk_tree_view_set_cursor(GTK_TREE_VIEW(this->sexy_tree_view), path, NULL, FALSE);
	gtk_tree_path_free(path);
	return TRUE;
}/*uberchick_tree_view_goto(uberchick_tree_view, 0<=this->total, TRUE|FALSE);*/

static gboolean uberchick_tree_view_goto_index(UberChickTreeView *uberchick_tree_view, gboolean select){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	return uberchick_tree_view_goto(uberchick_tree_view, this->index, select);
}/*uberchick_tree_view_goto_index(uberchick_tree_view);*/

static gboolean uberchick_tree_view_index_select(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(!(GTK_IS_TREE_VIEW(GTK_TREE_VIEW(this->sexy_tree_view)) && this->total)) return FALSE;
	if(uberchick_tree_view_goto(uberchick_tree_view, this->selected_index, TRUE)){
		debug("UberChickTreeView for %s(timeline %s) index selected.  Update %d, out of %d total updates", this->update_type_string, this->timeline, this->selected_index, this->total);
		return TRUE;
	}
	return FALSE;
}/*uberchick_tree_view_index_select(uberchick_tree_view);*/

static gboolean uberchick_tree_view_index_scroll_to(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(!(GTK_TREE_VIEW(this->sexy_tree_view) && this->total)) return FALSE;
	
	if(uberchick_tree_view_goto_index(uberchick_tree_view, FALSE)){
		debug("UberChickTreeView for %s(timeline %s) perspective scrolled to index.  Update %d, out of %d total updates", this->update_type_string, this->timeline, this->index, this->total);
		return TRUE;
	}
	return FALSE;
}/*uberchick_tree_view_index_scroll_to(uberchick_tree_view);*/

static gboolean uberchick_tree_view_scroll_to_top(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(!(GTK_TREE_VIEW(this->sexy_tree_view) && this->total)) return FALSE;
	this->index=0;
	if(uberchick_tree_view_goto_index(uberchick_tree_view, FALSE)){
		debug("UberChickTreeView for %s(timeline %s) focus moved to index 0", this->update_type_string, this->timeline);
		return TRUE;
	}
	return FALSE;
}/*uberchick_tree_view_scroll_to_top(uberchick_tree_view);*/

static void uberchick_tree_view_clear(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	debug("Clearing UberChickTreeView, for %s (timeline: %s).  UberChickTreeView has_loaded status:%s(#%d)", this->update_type_string, this->timeline, (this->has_loaded>0 ?"TRUE" :"FALSE"), this->has_loaded);
	
	gtk_tree_store_clear(this->tree_store);
	gtk_progress_bar_set_fraction(this->progress_bar, 1.0);
	
	if(this->has_loaded > -1) this->has_loaded=-1;
	if(this->minutes) this->minutes=0.0;
	
	if(this->tree_store_index>-1) this->tree_store_index=-1;
	
	if(this->selected_index) this->selected_index=-1;
	if(this->index) this->index=-1;
	
	if(this->total) this->total=0;
	
	gboolean unread_found=FALSE;
	if(this->unread) unread_found=TRUE;
	if(this->unread_updates){
		if(!unread_found) unread_found=TRUE ;
		this->unread_updates=0;
	}
	if(unread_found){
		if(!this->unread)
			this->unread=TRUE;
		uberchick_tree_view_mark_as_read(uberchick_tree_view);
	}
}/*uberchick_tree_view_clear(uberchick_tree_view);*/

static void uberchick_tree_view_grab_focus_cb(GtkWidget *widget, UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	uberchick_tree_view_mark_as_read(uberchick_tree_view);
}/*uberchick_tree_view_grab_focus_cb(widget, event, uberchick_tree_view);*/

gboolean uberchick_tree_view_toggle_view(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	
	if(GET_PRIVATE(uberchick_tree_view)->update_type==Groups) return FALSE;
	
	uberchick_tree_view_toggle_update_detailed_column(uberchick_tree_view);
	uberchick_tree_view_toggle_update_column(uberchick_tree_view);
	uberchick_tree_view_toggle_created_ago_str_column(uberchick_tree_view);
	
	return TRUE;
}/*uberchick_tree_view_toggle_view(uberchick_tree_view);*/

gboolean uberchick_tree_view_toggle_toolbar(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	gtk_widget_toggle_visibility(GTK_WIDGET(GET_PRIVATE(uberchick_tree_view)->handlebox));
	return gtk_widget_is_visible(GTK_WIDGET(GET_PRIVATE(uberchick_tree_view)->handlebox));
}/*uberchick_tree_view_toggle_toolbar(uberchick_tree_view);*/

static gboolean uberchick_tree_view_toggle_update_detailed_column(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	gtk_tree_view_column_toggle_visibility(GET_PRIVATE(uberchick_tree_view)->string_detailed_update_tree_view_column);
	return gtk_tree_view_column_get_visible(GET_PRIVATE(uberchick_tree_view)->string_detailed_update_tree_view_column);
}/*uberchick_tree_view_toggle_update_detailed_column(uberchick_tree_view);*/

static gboolean uberchick_tree_view_toggle_update_column(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	gtk_tree_view_column_toggle_visibility(GET_PRIVATE(uberchick_tree_view)->string_update_tree_view_column);
	return gtk_tree_view_column_get_visible(GET_PRIVATE(uberchick_tree_view)->string_update_tree_view_column);
}/*uberchick_tree_view_toggle_update_column(uberchick_tree_view);*/

gboolean uberchick_tree_view_toggle_from_column(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	gtk_tree_view_column_toggle_visibility(GET_PRIVATE(uberchick_tree_view)->string_from_tree_view_column);
	return gtk_tree_view_column_get_visible(GET_PRIVATE(uberchick_tree_view)->string_from_tree_view_column);
}/*uberchick_tree_view_toggle_from_column(uberchick_tree_view);*/

gboolean uberchick_tree_view_toggle_rcpt_column(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	gtk_tree_view_column_toggle_visibility(GET_PRIVATE(uberchick_tree_view)->string_rcpt_tree_view_column);
	return gtk_tree_view_column_get_visible(GET_PRIVATE(uberchick_tree_view)->string_rcpt_tree_view_column);
}/*uberchick_tree_view_toggle_rcpt_column(uberchick_tree_view);*/

static gboolean uberchick_tree_view_toggle_created_ago_str_column(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	gtk_tree_view_column_toggle_visibility(GET_PRIVATE(uberchick_tree_view)->created_ago_str_tree_view_column);
	return gtk_tree_view_column_get_visible(GET_PRIVATE(uberchick_tree_view)->created_ago_str_tree_view_column);
}/*uberchick_tree_view_toggle_created_ago_str_column(uberchick_tree_view);*/

void uberchick_tree_view_store_update(UberChickTreeView *uberchick_tree_view, UserStatus *status){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	gtk_progress_bar_pulse(this->progress_bar);
	
	gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	update_ids_get(status->service, this->timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	gboolean unread=(status->id && status->id > unread_update_id);
	
	this->tree_store_index++;
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	gtk_tree_store_insert(this->tree_store, iter, NULL, this->tree_store_index);
	gtk_tree_store_set(
			this->tree_store, iter,
				GUINT_UBERCHICK_TREE_VIEW_INDEX, this->total,		/* The total of updates in the TreeStore when the rows entered.	*/
				GDOUBLE_UPDATE_ID, status->id,				/* Update's ID.							*/
				GSTRING_UPDATE_ID_STR, status->id_str,			/* Update s ID as a string.					*/
				GDOUBLE_USER_ID, status->user->id,			/* User's ID.							*/
				STRING_USER, status->user->user_name,			/* Useruser_name string.					*/
				STRING_NICK, status->user->nick_name,			/* Author user_name string.					*/
				STRING_TEXT, status->text,				/* Update's string as plain text.				*/
				STRING_UPDATE, status->update,				/* Update's string as markup for display.			*/
				STRING_SEXY_STATUS_UPDATE, status->sexy_status_text,	/* Update's string as markup for display.			*/
				STRING_SEXY_UPDATE, status->sexy_update,		/* SexyTreeView's tooltip.					*/
				STRING_DETAILED_UPDATE, NULL,				/* Upate's string as markup w/full details.			*/
				STRING_CREATED_AGO, status->created_how_long_ago,	/* (seconds|minutes|hours|day) ago.				*/
				STRING_CREATED_AT, status->created_at_str,		/* Date string.							*/
				GINT_CREATED_AGO, status->created_seconds_ago,		/* How old the post is, in seconds, for sorting.		*/
				ULONG_CREATED_AT, status->created_at,			/* Seconds since the post was posted.				*/
				ONLINE_SERVICE, status->service,			/* OnlineService pointer.					*/
				STRING_FROM, status->from,				/* Who the update/update is from.				*/
				STRING_RCPT, status->rcpt,				/* The OnlineService's key this update's from.			*/
				GINT_LIST_STORE_INDEX, this->tree_store_index,		/* The row's unsorted index.					*/
				GBOOLEAN_UNREAD, unread,				/* If the update's been read or not.				*/
				GBOOLEAN_RETWEET, status->retweet,			/* If the update's a retweet or not.				*/
				GDOUBLE_RETWEET_UPDATE_ID, ((status->retweet && status->retweeted_status && status->retweeted_status->id) ?status->retweeted_status->id :0.0),
				GCHARARRY_RETWEETED_USER_NAME, status->retweeted_user_name,	/* Who retweeted this update. "" for non-retweets.	*/
				GCHARARRY_RETWEETED_USER_NICK, status->retweeted_user_nick,	/* Who retweeted this update. "" for non-retweets.	*/
				GCHARARRY_RETWEETED_MARKUP, status->retweeted_markup,		/* Who retweeted this update. "" for non-retweets.	*/
			-1
	);
	this->total++;
	
	debug("Inserting update in to UberChickTreeView at index %d.  Inserting one of <%s>'s %s; timeline [%s]; update ID: %s; read status: %s.  Total updates: %u", this->tree_store_index, status->service->guid, this->update_type_string, this->timeline, status->id_str, (unread ?"TRUE" :"FALSE"), this->total);
	debug("Inserting iter for <%s>'s %s at index: %d; tree_view_index: %d", status->service->guid, this->update_type_string, this->total, this->total);
	if(unread)
		uberchick_tree_view_increment_unread(uberchick_tree_view);
	
	/* network_get_image, or its callback network_cb_on_image, free's iter once its no longer needed.*/
	if(!(!status->retweet && g_file_test(status->user->image_file, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR))){
		debug("User Avatar file [%s] not found. Attempting to download: %s", status->user->image_file, status->user->image_uri);
		return network_get_image(status->service, uberchick_tree_view, status->user->image_file, status->user->image_uri, status->retweet, iter);
	}
	
	uberchick_tree_view_set_image(uberchick_tree_view, status->user->image_file, iter);
	uber_free(iter);
}/*uberchick_tree_view_store_update(uberchick_tree_view, status);*/

void uberchick_tree_view_store_group(UberChickTreeView *uberchick_tree_view, StatusNetGroup *group){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	gtk_progress_bar_pulse(this->progress_bar);
	
	gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	update_ids_get(group->service, this->timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	gboolean unread=(group->id && group->id > unread_update_id);
	
	this->tree_store_index++;
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	gtk_tree_store_insert(this->tree_store, iter, NULL, this->tree_store_index);
	gtk_tree_store_set(
			this->tree_store, iter,
				GUINT_UBERCHICK_TREE_VIEW_INDEX, this->total,		/* The total of updates in the TreeStore when the rows entered.	*/
				GDOUBLE_UPDATE_ID, group->id,				/* Update's ID.							*/
				GSTRING_UPDATE_ID_STR, group->id_str,			/* Update s ID as a string.					*/
				GDOUBLE_USER_ID, group->id,				/* User's ID.							*/
				STRING_USER, group->name,				/* Useruser_name string.					*/
				STRING_NICK, group->title,				/* Author user_name string.					*/
				STRING_TEXT, group->note,				/* Update's string as plain text.				*/
				STRING_UPDATE, group->note,				/* Update's string as markup for display.			*/
				STRING_SEXY_STATUS_UPDATE, group->note_markup,		/* Update's string as markup for display.			*/
				STRING_SEXY_UPDATE, group->note_tooltip,		/* SexyTreeView's tooltip.					*/
				STRING_DETAILED_UPDATE, NULL,				/* Upate's string as markup w/full details.			*/
				STRING_CREATED_AGO, g_strdup(""),			/* (seconds|minutes|hours|day) ago.				*/
				STRING_CREATED_AT, g_strdup(""),			/* Date string.							*/
				GINT_CREATED_AGO, 0,					/* How old the post is, in seconds, for sorting.		*/
				ULONG_CREATED_AT, 0,					/* Seconds since the post was posted.				*/
				ONLINE_SERVICE, group->service,				/* OnlineService pointer.					*/
				STRING_FROM, g_strdup(""),				/* Who the update/update is from.				*/
				STRING_RCPT, g_strdup(""),				/* The OnlineService's key this update's from.			*/
				GINT_LIST_STORE_INDEX, this->tree_store_index,		/* The row's unsorted index.					*/
				GBOOLEAN_UNREAD, unread,				/* If the update's been read or not.				*/
				GBOOLEAN_RETWEET, FALSE,				/* If the update's a retweet or not.				*/
				GDOUBLE_RETWEET_UPDATE_ID, 0.0,
				GCHARARRY_RETWEETED_USER_NAME, g_strdup(""),		/* Who retweeted this update. "" for non-retweets.	*/
				GCHARARRY_RETWEETED_USER_NICK, g_strdup(""),		/* Who retweeted this update. "" for non-retweets.	*/
				GCHARARRY_RETWEETED_MARKUP, g_strdup(""),		/* Who retweeted this update. "" for non-retweets.	*/
			-1
	);
	this->total++;
	
	debug("Inserting update in to UberChickTreeView at index %d.  Inserting one of <%s>'s %s; timeline [%s]; update ID: %s; read group: %s.  Total updates: %u", this->tree_store_index, group->service->guid, this->update_type_string, this->timeline, group->id_str, (unread ?"TRUE" :"FALSE"), this->total);
	debug("Inserting iter for <%s>'s %s at index: %d; tree_view_index: %d", group->service->guid, this->update_type_string, this->total, this->total);
	if(unread)
		uberchick_tree_view_increment_unread(uberchick_tree_view);
	
	/* network_get_image, or its callback network_cb_on_image, free's iter once its no longer needed.*/
	if(!(g_file_test(group->image_file, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR))){
		debug("User Avatar file [%s] not found. Attempting to download: %s", group->image_file, group->image_uri);
		return network_get_image(group->service, uberchick_tree_view, group->image_file, group->image_uri, TRUE, iter);
	}
	
	uberchick_tree_view_set_image(uberchick_tree_view, group->image_file, iter);
	uber_free(iter);
}/*uberchick_tree_view_store_group(uberchick_tree_view, group);*/

static gboolean uberchick_tree_view_notification(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return FALSE;
	
	if(gconfig_if_bool(PREFS_NOTIFY_ALL, TRUE))
		return TRUE;
	
	switch(GET_PRIVATE(uberchick_tree_view)->update_type){
		case	Homepage:	case	ReTweets:
		case	Users:		case	Timelines:
			if(!gconfig_if_bool(PREFS_NOTIFY_FOLLOWING, TRUE))
				return FALSE;
			break;
		
		case	DMs:
			if(!gconfig_if_bool(PREFS_NOTIFY_DMS, TRUE))
				return FALSE;
			break;
		
		case	Replies:
			if(!gconfig_if_bool(PREFS_NOTIFY_REPLIES, TRUE))
				return FALSE;
			break;
		
		case	BestFriends:
			if(!gconfig_if_bool(PREFS_NOTIFY_BEST_FRIENDS, TRUE))
				return FALSE;
			break;
		
		case	Searches:	case	Groups:
			if(!gconfig_if_bool(PREFS_NOTIFY_ALL, TRUE))
				return FALSE;
		
		case	Faves:		case	Archive:
		case	None:		default:
			return FALSE;
			break;
	}
	return TRUE;
}/*uberchick_tree_view_notification(uberchick_tree_view);*/

void uberchick_tree_view_mark_as_read(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	/*if(GET_PRIVATE(uberchick_tree_view)->unread){
		uberchick_tree_view_mark_as_unread(uberchick_tree_view);
		return;
	}*/
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(this->unread)
		this->unread=FALSE;
	
	debug("Marking UberChickTreeView, for %s (timeline: %s), as having %d unread updates.  UberChickTreeView has_loaded status:%s(#%d)", this->update_type_string, this->timeline, this->unread_updates, (this->has_loaded>0 ?"TRUE" :"FALSE"), this->has_loaded);
	
	gchar *tab_label_markup=NULL, *menu_label_markup=NULL;
	if(!this->unread_updates){
		tab_label_markup=g_markup_printf_escaped("%s\n", this->tab_label_string);
		menu_label_markup=g_markup_printf_escaped("%s\n", this->menu_label_string);
		if(GTK_WIDGET_IS_SENSITIVE(GTK_WIDGET(this->mark_as_read_tool_button)))
			gtk_widget_set_sensitive(GTK_WIDGET(this->mark_as_read_tool_button), FALSE);
		if(GTK_WIDGET_IS_SENSITIVE(GTK_WIDGET(this->tab_mark_as_read_tool_button)))
			gtk_widget_set_sensitive(GTK_WIDGET(this->tab_mark_as_read_tool_button), FALSE);
	}else{
		tab_label_markup=g_markup_printf_escaped("%s\n  <span size=\"x-small\" variant=\"smallcaps\" color=\"#990000\">*%d unread updates*</span>", this->tab_label_string, this->unread_updates);
		menu_label_markup=g_markup_printf_escaped("%s  <span size=\"x-small\" variant=\"smallcaps\" color=\"#990000\">*%d unread updates*</span>", this->menu_label_string, this->unread_updates);
		if(!GTK_WIDGET_IS_SENSITIVE(GTK_WIDGET(this->mark_as_read_tool_button)))
			gtk_widget_set_sensitive(GTK_WIDGET(this->mark_as_read_tool_button), TRUE);
		if(!GTK_WIDGET_IS_SENSITIVE(GTK_WIDGET(this->tab_mark_as_read_tool_button)))
			gtk_widget_set_sensitive(GTK_WIDGET(this->tab_mark_as_read_tool_button), TRUE);
	}
	gtk_label_set_markup_with_mnemonic(this->tab_label, tab_label_markup);
	gtk_label_set_markup_with_mnemonic(this->menu_label, menu_label_markup);
	uber_free(tab_label_markup);
	uber_free(menu_label_markup);
}/*uberchick_tree_view_mark_as_read(uberchick_tree_view);*/

static void uberchick_tree_view_mark_as_unread(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	/*if(!(GET_PRIVATE(uberchick_tree_view)->unread)){
		uberchick_tree_view_mark_as_unread(uberchick_tree_view);
		return;
	}*/
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(!this->unread)
		this->unread=TRUE;
	
	debug("Marking UberChickTreeView, for %s (timeline: %s), as having %d unread updates(maximum allowed updates: %s).  UberChickTreeView's total updates:%d; has_loaded status:%s(#%d)", this->update_type_string, this->timeline, this->unread_updates, this->max_updates_str, this->total, (this->has_loaded>0 ?"TRUE" :"FALSE"), this->has_loaded);
	
	if(!GTK_WIDGET_IS_SENSITIVE(GTK_WIDGET(this->mark_as_read_tool_button)))
		gtk_widget_set_sensitive(GTK_WIDGET(this->mark_as_read_tool_button), TRUE);
	if(!GTK_WIDGET_IS_SENSITIVE(GTK_WIDGET(this->tab_mark_as_read_tool_button)))
		gtk_widget_set_sensitive(GTK_WIDGET(this->tab_mark_as_read_tool_button), TRUE);
	
	gchar *tab_label_markup=g_markup_printf_escaped("%s\n  <span size=\"x-small\" variant=\"smallcaps\" color=\"#FF0000\">*%d unread updates*</span>", this->tab_label_string, this->unread_updates);
	gchar *menu_label_markup=g_markup_printf_escaped("%s  <span size=\"x-small\" variant=\"smallcaps\" color=\"#FF0000\">*%d unread updates*</span>", this->menu_label_string, this->unread_updates);
	
	gtk_label_set_markup_with_mnemonic(this->tab_label, tab_label_markup);
	gtk_label_set_markup_with_mnemonic(this->menu_label, menu_label_markup);
	
	uber_free(tab_label_markup);
	uber_free(menu_label_markup);
	
	if(!uberchick_tree_view_notification(uberchick_tree_view))
		return;
	
	GtkWindow *window=NULL;
	if(!gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE))
		window=main_window_get_window();
	else
		window=update_viewer_get_window();
	
	gtk_window_set_urgency_hint(window, uberchick_tree_view_notification(uberchick_tree_view));
}/*uberchick_tree_view_mark_as_unread(uberchick_tree_view);*/

static void uberchick_tree_view_increment_unread(UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	switch(this->update_type){
		case	DMs:		case	Replies:
		case	Faves:		case	BestFriends:
		case	Users:
			break;
		
		case	Homepage:	case	ReTweets:
		case	Archive:	case	Timelines:
		case	Searches:	case	Groups:
			if(this->has_loaded > 0 && this->total) break;
			debug("Not-Marking UberChickTreeView, for %s (timeline: %s), as having %d unread updates(maximum allowed updates exceded: %s).  UberChickTreeView's total updates:%d; has_loaded status:%s(#%d)", this->update_type_string, this->timeline, this->unread_updates, this->max_updates_str, this->total, (this->has_loaded>0 ?"TRUE" :"FALSE"), this->has_loaded);
		
		case	None:		default:
			return;
	}
	
	if(this->unread_updates<this->max_updates && this->unread_updates <= this->total)
		this->unread_updates++;
	if(!this->unread)
		this->unread=TRUE;
}/*uberchick_tree_view_increment_unread(uberchick_tree_view);*/

static void uberchick_tree_view_set_sexy_tooltip(SexyTreeView *sexy_tree_view, GtkTreePath *path, GtkTreeViewColumn *column, UberChickTreeView *uberchick_tree_view){
}/*static void uberchick_tree_view_set_sexy_tooltip(sexy_tree_view, path, column, uberchick_tree_view);*/

static void uberchick_tree_view_update_selected(SexyTreeView *uberchick_tree_view_sexy_tree_view, UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	GtkTreeSelection	*sel;
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	if(!((sel=gtk_tree_view_get_selection(GTK_TREE_VIEW(this->sexy_tree_view))) && gtk_tree_selection_get_selected(sel, NULL, iter))){
		uber_free(iter);
		update_viewer_sexy_select(update_viewer_sexy_entry_get_widget());
		return;
	}
	
	if(this->unread)
		uberchick_tree_view_mark_as_read(uberchick_tree_view);
	
	gdouble		update_id=0.0, user_id=0.0, retweet_update_id=0.0;
	OnlineService	*service=NULL;
	GdkPixbuf	*pixbuf=NULL;
	gchar		*user_name=NULL, *nick_name=NULL, *date=NULL, *sexy_update=NULL, *text_update=NULL;
	gchar		*retweeted_user_name=NULL, *retweeted_user_nick=NULL, *retweeted_markup=NULL;
	gboolean	unread=FALSE, retweet=FALSE;
	
	gint tree_store_index=-1;
	gtk_tree_model_get(
				GTK_TREE_MODEL(this->tree_store), iter,
					GDOUBLE_UPDATE_ID, &update_id,
					GDOUBLE_USER_ID, &user_id,
					STRING_NICK, &nick_name,
					STRING_TEXT, &text_update,
					STRING_SEXY_UPDATE, &sexy_update,
					STRING_CREATED_AGO, &date,
					STRING_USER, &user_name,
					PIXBUF_AVATAR, &pixbuf,
					ONLINE_SERVICE, &service,
					GINT_LIST_STORE_INDEX, &tree_store_index,
					GBOOLEAN_UNREAD, &unread,
					GBOOLEAN_RETWEET, &retweet,
					GDOUBLE_RETWEET_UPDATE_ID, &retweet_update_id,
					GCHARARRY_RETWEETED_USER_NAME, &retweeted_user_name,	/* Who retweeted this update. "" for non-retweets.	*/
					GCHARARRY_RETWEETED_USER_NICK, &retweeted_user_nick,	/* Who retweeted this update. "" for non-retweets.	*/
					GCHARARRY_RETWEETED_MARKUP, &retweeted_markup,		/* Who retweeted this update. "" for non-retweets.	*/
				-1
	);
	
	gchar *update_id_str=gdouble_to_str(update_id);
	/* new code using tree_store's inheritance from GtkTreeSortModel */
	if(unread)
		uberchick_tree_view_iter_mark_as_read(uberchick_tree_view, iter, service, user_name, update_id);
	this->selected_index=tree_store_index;
	
	if(!G_STR_N_EMPTY(retweeted_user_name))
		uber_free(retweeted_user_name);
	if(!G_STR_N_EMPTY(retweeted_user_nick))
		uber_free(retweeted_user_nick);
	if(!G_STR_N_EMPTY(retweeted_markup))
		uber_free(retweeted_markup);
	
	debug("Displaying update ID: %s.  From <%s@%s>; To: <%s>.  Indices: tree_store %d", update_id_str, user_name, service->uri, service->guid, tree_store_index);
	statusbar_printf("Displaying update ID: %s.  From <%s@%s>; To: <%s>.  Indices: tree_store %d.", update_id_str, user_name, service->uri, service->guid, tree_store_index);
	
	update_viewer_view_update(service, update_id, user_id, user_name, nick_name, date, sexy_update, text_update, pixbuf, this->update_type, retweet_update_id, retweeted_user_name, retweeted_user_nick);
	
	if(retweeted_user_name)
		uber_free(retweeted_user_name);
	if(retweeted_user_nick)
		uber_free(retweeted_user_nick);
	if(retweeted_markup)
		uber_free(retweeted_markup);
	uber_free(user_name);
	uber_free(sexy_update);
	uber_free(text_update);
	uber_free(date);
	uber_free(nick_name);
	uber_free(update_id_str);
	if(pixbuf) uber_object_unref(pixbuf);
	uber_free(iter);
	
	update_viewer_sexy_select(update_viewer_sexy_entry_get_widget());
}/*uberchick_tree_view_update_selected(uberchick_tree_view_sexy_tree_view, uberchick_tree_view);*/

static void uberchick_tree_view_iter_mark_as_read(UberChickTreeView *uberchick_tree_view, GtkTreeIter *iter, OnlineService *service, const gchar *user_name, gdouble update_id){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	if(this->unread_updates)
		this->unread_updates--;
	
	if(!this->unread)
		uberchick_tree_view_mark_as_read(uberchick_tree_view);
	else
		uberchick_tree_view_mark_as_unread(uberchick_tree_view);
	
	update_ids_check(service, this->timeline, update_id, FALSE);
	best_friends_check_update_ids(service, user_name, update_id);
	if(this->update_type!=DMs){
		if(best_friends_is_user_best_friend(service, user_name))
			best_friends_tree_store_mark_as_read(service, user_name, update_id);
		if(this->update_type!=BestFriends && this->update_type!=Users){
			gchar *user_timeline=g_strdup_printf(API_TIMELINE_USER, user_name);
			update_ids_check(service, user_timeline, update_id, FALSE);
			uber_free(user_timeline);
		}
	}
	
	gtk_tree_store_set(
			this->tree_store, iter,
				GBOOLEAN_UNREAD, FALSE,
			-1
	);
}/*uberchick_tree_view_iter_mark_as_read(uberchick_tree_view, iter);*/

static void uberchick_tree_view_resized(GtkWidget *widget, GtkAllocation *allocation, UberChickTreeView *uberchick_tree_view){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	const guint8 wrap_width=10;
	gtk_word_wrap_tree_view_column(this->string_detailed_update_tree_view_column, this->string_detailed_update_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(this->string_update_tree_view_column, this->string_update_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(this->string_rcpt_tree_view_column, this->string_rcpt_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(this->string_from_tree_view_column, this->string_from_cell_renderer_text, wrap_width);
}/*uberchick_tree_view_resized(widget, allocation, uberchick_tree_view);*/

void uberchick_tree_view_set_image(UberChickTreeView *uberchick_tree_view, const gchar *image_filename, GtkTreeIter *iter){
	if(!(uberchick_tree_view && IS_UBERCHICK_TREE_VIEW(uberchick_tree_view)))	return;
	UberChickTreeViewPrivate *this=GET_PRIVATE(uberchick_tree_view);
	
	GdkPixbuf *pixbuf;
	if(!(pixbuf=images_get_pixbuf_from_filename((gchar *)image_filename)))
		return;
	
	debug("Adding image: '%s' to UberChickTreeView", image_filename);
	gtk_tree_store_set(
			this->tree_store, iter,
				PIXBUF_AVATAR, pixbuf,
			-1
	);
	uber_object_unref(pixbuf);
}/* uberchick_tree_view_set_image */
/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
