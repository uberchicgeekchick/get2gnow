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

/********************************************************
 *        System & library headers.                     *
 ********************************************************/
#define _GNU_SOURCE
#define _THREAD_SAFE

#include <glib/gi18n.h>

#include "config.h"
#include "program.h"

/********************************************************
 *        Project headers.                              *
 ********************************************************/
#include "gtkbuilder.h"
#include "ui-utils.h"

#include "online-service.h"
#include "online-service-request.h"
#include "users-glists.h"

#include "main-window.h"

#include "following-viewer.h"

#include "network.h"
#include "users.types.h"
#include "users.h"


/********************************************************
 *        Objects, structures, and etc typedefs         *
 ********************************************************/
enum {
	FOLLOWER_USER,
	FOLLOWER_NAME,
	USER_POINTER
};

typedef struct {
	GtkWindow	*viewer;
	
	SexyTreeView	*following_sexy_tree_view;
	GtkTreeStore	*following_tree_store;
	
	GtkToolbar	*users_tool_bar;
	GtkToolButton	*user_view_profile_tool_button;
	GtkToolButton	*user_unfollow_tool_button;
	GtkToolButton	*user_block_tool_button;
	GtkToolButton	*user_view_recent_updates_tool_button;
	GtkToolButton	*user_view_unread_updates_tool_button;
	GtkToolButton	*refresh_tool_button;
	
	GtkButton	*close_button;
} FollowingViewer;

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define	DEBUG_DOMAINS	"FollowingViewer:UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Updates:Requests:Users:Setup:following-viewer.c"
#include "debug.h"

#define GTK_BUILDER_UI_FILENAME "following-viewer"

static FollowingViewer *following_viewer=NULL;

/********************************************************
 *          Method  & function prototypes               *
 ********************************************************/
static User *following_viewer_get_selected_user(GtkTreeIter **iter, FollowingViewer *following_viewer);
static void following_viewer_setup(GtkWindow *parent);

static void following_viewer_load_lists(GList *followers);

static void following_viewer_response_cb(GtkDialog *dialog, gint response, FollowingViewer *following_viewer);
static void following_viewer_destroy_cb(GtkDialog *dialog, FollowingViewer *following_viewer);

static void following_viewer_refresh(GtkToolButton *button, FollowingViewer *following_viewer);
static void following_viewer_list_clicked(GtkTreeView *tree_view, FollowingViewer *following_viewer);
static void following_viewer_set_buttons_sensitivity(FollowingViewer *following_viewer, gboolean is_sensitive);
static void following_viewer_view_profile(GtkToolButton *button, FollowingViewer *following_viewer);
static void following_viewer_unfollow(GtkToolButton *button, FollowingViewer *following_viewer);
static void following_viewer_block(GtkToolButton *button, FollowingViewer *following_viewer);
static void following_viewer_view_recent_updates(GtkToolButton *button, FollowingViewer *following_viewer);
static void following_viewer_view_unread_updates(GtkToolButton *button, FollowingViewer *following_viewer);

/********************************************************
 *          My art, code, & programming.                *
 ********************************************************/
static void following_viewer_response_cb(GtkDialog *dialog, gint response, FollowingViewer *following_viewer){
	gtk_widget_destroy(GTK_WIDGET(dialog));
}/*following_viewer_response_cb();*/

static void following_viewer_destroy_cb(GtkDialog *dialog, FollowingViewer *following_viewer){
	debug("Destroying following viewer.");
	gtk_tree_store_clear(following_viewer->following_tree_store);
	gtk_widget_destroy(GTK_WIDGET(dialog));
	uber_free(following_viewer);
}/*following_viewer_destroy_cb(dialog);*/


static void following_viewer_list_clicked(GtkTreeView *tree_view, FollowingViewer *following_viewer){
	following_viewer_set_buttons_sensitivity(following_viewer, TRUE);
}/*following_viewer_list_clicked*/


static void following_viewer_set_buttons_sensitivity(FollowingViewer *following_viewer, gboolean is_sensitive){
	if(GTK_WIDGET_IS_SENSITIVE(following_viewer->user_view_profile_tool_button)!=is_sensitive)
		gtk_widget_set_sensitive(GTK_WIDGET(following_viewer->user_view_profile_tool_button), is_sensitive);
	
	if(GTK_WIDGET_IS_SENSITIVE(following_viewer->user_unfollow_tool_button)!=is_sensitive)
		gtk_widget_set_sensitive(GTK_WIDGET(following_viewer->user_unfollow_tool_button), is_sensitive);
	
	if(GTK_WIDGET_IS_SENSITIVE(following_viewer->user_block_tool_button)!=is_sensitive)
		gtk_widget_set_sensitive(GTK_WIDGET(following_viewer->user_block_tool_button), is_sensitive);
	
	if(GTK_WIDGET_IS_SENSITIVE(following_viewer->user_view_unread_updates_tool_button)!=is_sensitive)
		gtk_widget_set_sensitive(GTK_WIDGET(following_viewer->user_view_unread_updates_tool_button), is_sensitive);
	
	if(GTK_WIDGET_IS_SENSITIVE(following_viewer->user_view_recent_updates_tool_button)!=is_sensitive)
		gtk_widget_set_sensitive(GTK_WIDGET(following_viewer->user_view_recent_updates_tool_button), is_sensitive);
	
	if(GTK_WIDGET_IS_SENSITIVE(following_viewer->refresh_tool_button)!=is_sensitive)
		gtk_widget_set_sensitive(GTK_WIDGET(following_viewer->refresh_tool_button), is_sensitive);
}/*following_viewer_set_buttons_sensitivity*/

static User *following_viewer_get_selected_user(GtkTreeIter **iter, FollowingViewer *following_viewer){
	User *user=NULL;
	*iter=g_new0(GtkTreeIter, 1);
	GtkTreeSelection *sel=gtk_tree_view_get_selection(GTK_TREE_VIEW(following_viewer->following_sexy_tree_view));
	if(!gtk_tree_selection_get_selected(sel, NULL, *iter)){
		uber_free(*iter);
		return NULL;
	}
	
	gtk_tree_model_get(
				GTK_TREE_MODEL(following_viewer->following_tree_store), *iter,
					USER_POINTER, &user,
				-1
	);
	
	if(user)
		return user;
	
	uber_free( *iter );
	return NULL;
}/*following_viewer_get_selected_user(following_viewer);*/

static void following_viewer_unfollow(GtkToolButton *button, FollowingViewer *following_viewer ){
	GtkTreeIter *iter=NULL;
	User *user=NULL;
	if(!(user=following_viewer_get_selected_user(&iter, following_viewer)))
		return;
	
	gtk_tree_store_remove(following_viewer->following_tree_store, iter);
	following_viewer_set_buttons_sensitivity(following_viewer, FALSE);
	online_service_request_unfollow(user->service, following_viewer->viewer, user->user_name);
	
	uber_free(iter);
}

static void following_viewer_block( GtkToolButton *button, FollowingViewer *following_viewer ){
	GtkTreeIter *iter=NULL;
	User *user=NULL;
	if(!(user=following_viewer_get_selected_user(&iter, following_viewer)))
		return;
	
	gtk_tree_store_remove(following_viewer->following_tree_store, iter);
	following_viewer_set_buttons_sensitivity(following_viewer, FALSE);
	online_service_request_block(user->service, following_viewer->viewer, user->user_name);
	
	uber_free(iter);
}

static void following_viewer_view_profile(GtkToolButton *button, FollowingViewer *following_viewer){
	GtkTreeIter *iter=NULL;
	User *user=NULL;
	if(!(user=following_viewer_get_selected_user(&iter, following_viewer)))
		return;
	
	online_service_request_view_profile(user->service, following_viewer->viewer, user->user_name);
	uber_free(iter);
}/*following_view_profile*/

static void following_viewer_view_unread_updates(GtkToolButton *button, FollowingViewer *following_viewer){
	GtkTreeIter *iter=NULL;
	User *user=NULL;
	if(!(user=following_viewer_get_selected_user(&iter, following_viewer)))
		return;
	
	online_service_request_view_updates(user->service, following_viewer->viewer, user->user_name);
	uber_free(iter);
}/*following_view_unread_updates*/


static void following_viewer_view_recent_updates(GtkToolButton *button, FollowingViewer *following_viewer){
	GtkTreeIter *iter=NULL;
	User *user=NULL;
	if(!(user=following_viewer_get_selected_user(&iter, following_viewer)))
		return;
	
	online_service_request_view_updates_new(user->service, following_viewer->viewer, user->user_name);
	uber_free(iter);
}/*following_view__updates*/


static void following_viewer_list_activated_cb(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, FollowingViewer *following_viewer){
	GtkTreeIter *iter=NULL;
	User *user=NULL;
	if(!(user=following_viewer_get_selected_user(&iter, following_viewer)))
		return;
	
	online_service_request_view_updates(user->service, following_viewer->viewer, user->user_name);
	uber_free(iter);
}/*following_viewer_list_activated_cb();*/

void following_viewer_refresh(GtkToolButton *button, FollowingViewer *following_viewer){
	GdkCursor *cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(following_viewer->viewer)->window, cursor);
	gtk_widget_set_sensitive(GTK_WIDGET(following_viewer->viewer), FALSE);
	
	online_service_request_popup_select_service();
	users_glist_get(GetFriends, TRUE, following_viewer_load_lists);
}/*following_viewer_refresh*/

static void following_viewer_load_lists(GList *followers){
	if(!followers) return;
	
	gdk_window_set_cursor(GTK_WIDGET(following_viewer->viewer)->window, NULL);
	following_viewer_set_buttons_sensitivity(following_viewer, FALSE);
	gtk_tree_store_clear(following_viewer->following_tree_store);
	
	User *user=NULL;
	for(followers=g_list_first(followers); followers; followers=followers->next){
		user = (User *)followers->data;
		GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
		gtk_tree_store_append(following_viewer->following_tree_store, iter, NULL);

		gtk_tree_store_set(
				following_viewer->following_tree_store, iter,
					FOLLOWER_USER, user->user_name,
					FOLLOWER_NAME, user->nick_name,
					USER_POINTER, user,
				-1
		);
		g_free(iter);
	}
	gtk_widget_set_sensitive(GTK_WIDGET(following_viewer->viewer), TRUE);
}

void following_viewer_show(GtkWindow *parent){
	online_service_request_popup_select_service();
	if(!(selected_service)) return;
	
	if(!(following_viewer && following_viewer->viewer ))
		return following_viewer_setup(parent);
	
	window_present((GTK_WINDOW(following_viewer->viewer)), TRUE);
}/*following_viewer_show*/


static void following_viewer_setup(GtkWindow *parent){
	GtkBuilder *ui;

	following_viewer=g_new0(FollowingViewer, 1);

	/* Get widgets */
	ui=gtkbuilder_get_file(
				GTK_BUILDER_UI_FILENAME,
					"following_viewer", &following_viewer->viewer,
					
					"following_sexy_tree_view", &following_viewer->following_sexy_tree_view,
					"following_tree_store", &following_viewer->following_tree_store,
					
					"users_tool_bar", &following_viewer->users_tool_bar,
					"user_unfollow_tool_button", &following_viewer->user_unfollow_tool_button,
					"user_block_tool_button", &following_viewer->user_block_tool_button,
					"user_view_profile_tool_button", &following_viewer->user_view_profile_tool_button,
					"user_view_unread_updates_tool_button", &following_viewer->user_view_unread_updates_tool_button,
					"user_view_recent_updates_tool_button", &following_viewer->user_view_recent_updates_tool_button,
					"refresh_tool_button", &following_viewer->refresh_tool_button,
					
					"viewer_close_button", &following_viewer->close_button,
				NULL
	);
	
	gchar *window_title=g_strdup_printf("%s - %s %s <%s>'s %s:", _(GETTEXT_PACKAGE), _("Manage"), _("who"), selected_service->key, _("follows"));
	gtk_window_set_title(GTK_WINDOW(following_viewer->viewer), window_title);
	uber_free(window_title);
	
	/* Connect the signals */
	gtkbuilder_connect(
				ui, following_viewer,
					"following_viewer", "destroy", following_viewer_destroy_cb,
					"following_viewer", "response", following_viewer_response_cb,
					
					"following_sexy_tree_view", "row-activated", following_viewer_list_activated_cb,
					"following_sexy_tree_view", "cursor-changed", following_viewer_list_clicked,
					
					"user_unfollow_tool_button", "clicked", following_viewer_unfollow,
					"user_block_tool_button", "clicked", following_viewer_block,
					"user_view_profile_tool_button", "clicked", following_viewer_view_profile,
					"user_view_unread_updates_tool_button", "clicked", following_viewer_view_unread_updates,
					"user_view_recent_updates_tool_button", "clicked", following_viewer_view_recent_updates,
					
					"refresh_tool_button", "clicked", following_viewer_refresh,
				NULL
	);
	
	g_object_unref(ui);
	
	/*TODO: FIXME:*/
	gtk_widget_hide(GTK_WIDGET(following_viewer->refresh_tool_button));
	/* Set the parent */
	g_object_add_weak_pointer(G_OBJECT(following_viewer->viewer), (gpointer) &following_viewer);
	gtk_window_set_transient_for(GTK_WINDOW(following_viewer->viewer), parent);
	
	/* Now that we're done setting up, let's show the widget */
	window_present((GTK_WINDOW(following_viewer->viewer)), TRUE);
	
	main_window_set_statusbar_msg(_("Please wait while the list of every one you\'re following is loaded."));
	GdkCursor *cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(following_viewer->viewer)->window, cursor);
	gtk_widget_set_sensitive(GTK_WIDGET(following_viewer->viewer), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(following_viewer->close_button), TRUE);
	
	/* Load following */
	users_glist_get(GetFriends, FALSE, following_viewer_load_lists);
}/*following_viewer_setup*/
