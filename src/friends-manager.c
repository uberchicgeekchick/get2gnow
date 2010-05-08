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
#define _GNU_SOURCE
#define _THREAD_SAFE


#include "config.h"

#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>


#include "program.h"

#include "ui-utils.h"

#include "online-services.typedefs.h"
#include "online-services.types.h"
#include "online-service.types.h"
#include "online-service.h"
#include "online-service-request.h"

#include "update.types.h"
#include "users.types.h"
#include "users-glists.h"
#include "users.h"

#include "gtkbuilder.h"
#include "main-window.h"
#include "friends-manager.h"
#include "network.h"
#include "images.h"

#include "uberchick-tree-view.macros.h"

enum {
	GSTRING_USER_NAME,
	GSTRING_USER_NICK,
	GSTRING_FOLLOWING,
	GSTRING_FOLLOWER,
	GSTRING_CREATED_AGO,
	USER_POINTER,
	GUINT_COLUMNS_COUNT
} FriendsManagerColums;

typedef struct {
	GtkDialog			*dialog;
	
	SexyTreeView			*friends_and_followers_sexy_tree_view;
	GtkTreeStore			*tree_store;
	
	GtkTreeViewColumn		*user_name_tree_view_column;
	GtkCellRendererText		*user_name_tree_cell_renderer_text;
	GtkTreeViewColumn		*user_nick_name_tree_view_column;
	GtkCellRendererText		*user_nick_name_cell_renderer_text;
	GtkTreeViewColumn		*friends_tree_view_column;
	GtkCellRendererText		*friends_cell_renderer_text;
	GtkTreeViewColumn		*follower_tree_view_column;
	GtkCellRendererText		*follower_cell_renderer_text;
	GtkTreeViewColumn		*created_age_tree_view_column;
	GtkCellRendererText		*created_age_cell_renderer_text;
	
	GtkToolbar			*users_tool_bar;
	GtkToolButton			*user_view_profile_tool_button;
	GtkToolButton			*user_follow_tool_button;
	GtkToolButton			*user_unfollow_tool_button;
	GtkToolButton			*user_block_tool_button;
	GtkToolButton			*user_view_recent_updates_tool_button;
	GtkToolButton			*user_view_unread_updates_tool_button;
	GtkToolButton			*refresh_tool_button;
	
	GtkButton			*close_button;
} FriendsManager;

static FriendsManager *friends_manager=NULL;

#define	DEBUG_DOMAINS	"FriendsManager:UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Requests:Updates:Users:Setup:friends-manager.c"
#include "debug.h"

#define GTK_BUILDER_UI_FILENAME "friends-manager"

/* GtkBuilder methods.
 * 'friends_manager_show' is this object's public method & prototyped in 'friends-manager.h' */
static void friends_manager_setup(GtkWindow *parent);

/* signal handlers */
static void friends_manager_destroy(GtkDialog *dialog, FriendsManager *friends_manager);
static void friends_manager_response(GtkDialog *dialog, gint response, FriendsManager *friends_manager);

static User *friends_manager_get_selected_user(GtkTreeIter **iter, FriendsManager *friends_manager);

static void friends_manager_tree_view_resized(GtkWidget *widget, GtkAllocation *allocation, FriendsManager *friends_manager);
static void friends_manager_list_clicked(GtkTreeView *tree_view, FriendsManager *friends_manager);

static void friends_manager_refresh(GtkToolButton *button, FriendsManager *friends_manager);
static void friends_manager_follow(GtkToolButton *button, FriendsManager *friends_manager);
static void friends_manager_unfollow(GtkToolButton *button, FriendsManager *friends_manager);
static void friends_manager_block(GtkToolButton *button, FriendsManager *friends_manager);

static void friends_manager_view_profile(void);
static void friends_manager_view_unread_updates(GtkToolButton *button, FriendsManager *friends_manager);
static void friends_manager_view_recent_updates(GtkToolButton *button, FriendsManager *friends_manager);

static void friends_manager_set_buttons_sensitivity(FriendsManager *friends_manager, gboolean is_sensitive);

static void friends_manager_display_following_and_followers(GList *friends_and_followers);


static void friends_manager_response(GtkDialog *dialog, gint response, FriendsManager *friends_manager){
	gtk_widget_destroy(GTK_WIDGET(dialog));
}/*friends_message_response*/

static void friends_manager_destroy(GtkDialog *dialog, FriendsManager *friends_manager){
	debug("Destroying friends manager");
	gtk_tree_store_clear(friends_manager->tree_store);
	gtk_widget_destroy(GTK_WIDGET(dialog));
	uber_free(friends_manager);
}/*friends_manager_destroy*/

static User *friends_manager_get_selected_user(GtkTreeIter **iter, FriendsManager *friends_manager){
	User *user=NULL;
	*iter=g_new0(GtkTreeIter, 1);
	GtkTreeSelection *sel=gtk_tree_view_get_selection(GTK_TREE_VIEW(friends_manager->friends_and_followers_sexy_tree_view));
	if(!gtk_tree_selection_get_selected(sel, NULL, *iter)){
		uber_free(*iter);
		return NULL;
	}
	
	gtk_tree_model_get(
			GTK_TREE_MODEL(friends_manager->tree_store), *iter,
				USER_POINTER, &user,
			-1
	);
	
	if(!user){
		uber_free(*iter);
		return NULL;
	}
	
	return user;
}/*following_viewer_get_selected_user(following_viewer);*/

static void friends_manager_follow(GtkToolButton   *button, FriendsManager *friends_manager){
	GtkTreeIter *iter=NULL;
	User *user=friends_manager_get_selected_user(&iter, friends_manager);
	
	gchar *following=NULL;
	gtk_tree_model_get(
			GTK_TREE_MODEL(friends_manager->tree_store), iter,
				GSTRING_FOLLOWING, &following,
			-1
	);
	
	if(g_str_equal(following, "No")){
		gtk_tree_store_set(
				friends_manager->tree_store, iter,
					GSTRING_FOLLOWING, "Yes",
				-1
		);
		online_service_request_follow(user->service, GTK_WINDOW(friends_manager->dialog), user->user_name);
	}
	
	uber_free(following);
	uber_free(iter);
}

static void friends_manager_unfollow(GtkToolButton   *button, FriendsManager *friends_manager){
	GtkTreeIter *iter=NULL;
	User *user=friends_manager_get_selected_user(&iter, friends_manager);
	
	gchar *follower=NULL, *following=NULL;
	gtk_tree_model_get(
			GTK_TREE_MODEL(friends_manager->tree_store), iter,
				GSTRING_FOLLOWING, &following,
				GSTRING_FOLLOWER, &follower,
			-1
	);
	
	if(g_str_equal(following, "Yes"))
		online_service_request_unfollow(user->service, GTK_WINDOW(friends_manager->dialog), user->user_name);
	
	if(g_str_equal(follower, "Yes")){
		gtk_tree_store_set(
				friends_manager->tree_store, iter,
					GSTRING_FOLLOWING, "No",
				-1
		);
	} else {
		gtk_tree_store_remove(friends_manager->tree_store, iter);
		friends_manager_set_buttons_sensitivity(friends_manager, FALSE);
	}
	
	uber_free(following);
	uber_free(follower);
	uber_free(iter);
}/*friends_manager_unfolow(button, friends_manager);*/

static void friends_manager_block(GtkToolButton   *button, FriendsManager *friends_manager){
	GtkTreeIter *iter=NULL;
	User *user=friends_manager_get_selected_user(&iter, friends_manager);
	
	online_service_request_block(user->service, GTK_WINDOW(friends_manager->dialog), user->user_name);
	gtk_tree_store_remove(friends_manager->tree_store, iter);
	friends_manager_set_buttons_sensitivity(friends_manager, FALSE);
	
	uber_free(iter);
}/*friends_manager_block(button, friends_manager);*/

/* This handles viewing profiles via button clicks & 'activating the tree_view.
 * 	optional parameters for button clicks are:
 *	 	GtkButtion *button
 *
 *	 for when the tree_view is activated the optional parameters are:
 *	 	GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column
 *
 *	 both methods are also sent gpointer user_data which is a pointer to FriendsManager *friends_manager
 */
static void friends_manager_view_profile(void){
	GtkTreeIter *iter=NULL;
	User *user=friends_manager_get_selected_user(&iter, friends_manager);
	if(!user) return;
	
	online_service_request_view_profile(user->service, GTK_WINDOW(friends_manager->dialog), user->user_name);
	uber_free(iter);
}/*friends_manager_view_profile*/

static void friends_manager_view_unread_updates(GtkToolButton *button, FriendsManager *friends_manager){
	GtkTreeIter *iter=NULL;
	User *user=friends_manager_get_selected_user(&iter, friends_manager);
	if(!user) return;
	
	online_service_request_view_updates_new(user->service, GTK_WINDOW(friends_manager->dialog), user->user_name);
	uber_free(iter);
}/*friends_manager_view_unread_updates*/

static void friends_manager_view_recent_updates(GtkToolButton *button, FriendsManager *friends_manager){
	GtkTreeIter *iter=NULL;
	User *user=friends_manager_get_selected_user(&iter, friends_manager);
	if(!user) return;
	
	online_service_request_view_updates(user->service, GTK_WINDOW(friends_manager->dialog), user->user_name);
	uber_free(iter);
}/*friends_manager_view_recent_updates*/

static void friends_manager_refresh(GtkToolButton *button, FriendsManager *friends_manager){
	GdkCursor *cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(friends_manager->dialog)->window, cursor);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->dialog), FALSE);
	
	online_service_request_popup_select_service();
	users_glist_get(GetBoth, TRUE, friends_manager_display_following_and_followers);
}/*friends_manager_refresh*/

static void friends_manager_display_following_and_followers(GList *friends_and_followers){
	User		*user1=NULL, *user2=NULL;
	GList		*list=NULL;
	
	gdk_window_set_cursor(GTK_WIDGET(friends_manager->dialog)->window, NULL);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->dialog), TRUE);
	friends_manager_set_buttons_sensitivity(friends_manager, FALSE);
	
	gboolean following=FALSE, follower=FALSE;
	gtk_tree_store_clear(friends_manager->tree_store);
	for(list=friends_and_followers; list; list=list->next){
		user1=(User *)list->data;
		if(!(user1 && G_STR_N_EMPTY(user1->user_name))) continue;
		if(!(list && list->next && list->next->data))
			following=!(follower=user1->follower);
		else{
			list=list->next;
			user2=(User *)list->data;
			if(user1 && user2 && G_STR_N_EMPTY(user1->user_name) && G_STR_N_EMPTY(user2->user_name) && g_str_equal(user1->user_name, user2->user_name))
				follower=following=TRUE;
			else{
				list=list->prev;
				following=!(follower=user1->follower);
			}
			user2=NULL;
		}
		
		GtkTreeIter *iter=g_new(GtkTreeIter, 1);
		gtk_tree_store_append(friends_manager->tree_store, iter, NULL);
		const gchar *created_ago=NULL;
		if(!(user1 && user1->status && user1->status->created_how_long_ago && G_STR_N_EMPTY(user1->status->created_how_long_ago)))
			created_ago="Never";
		else
			created_ago=user1->status->created_how_long_ago;
		gtk_tree_store_set(
				friends_manager->tree_store, iter,
					GSTRING_USER_NAME, user1->user_name,
					GSTRING_USER_NICK, user1->nick_name,
					GSTRING_FOLLOWING, (following?"Yes":"No"),
					GSTRING_FOLLOWER, (follower?"Yes":"No"),
					GSTRING_CREATED_AGO, created_ago,
					USER_POINTER, user1,
				-1
		);
		uber_free(iter);
		created_ago=NULL;
	}
}/*friends_manager_display_following_and_followers*/


static void friends_manager_list_clicked(GtkTreeView *tree_view, FriendsManager *friends_manager){
	friends_manager_set_buttons_sensitivity(friends_manager, TRUE);
}/*friends_manager_list_clicked*/


static void friends_manager_set_buttons_sensitivity(FriendsManager *friends_manager, gboolean is_sensitive){
	if(gtk_widget_is_sensitive(GTK_WIDGET(friends_manager->user_block_tool_button))!=is_sensitive)
		gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_block_tool_button), is_sensitive);
	
	if(gtk_widget_is_sensitive(GTK_WIDGET(friends_manager->user_view_profile_tool_button))!=is_sensitive)
		gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_view_profile_tool_button), is_sensitive);
	
	if(gtk_widget_is_sensitive(GTK_WIDGET(friends_manager->user_view_unread_updates_tool_button))!=is_sensitive)
		gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_view_unread_updates_tool_button), is_sensitive);
	
	if(gtk_widget_is_sensitive(GTK_WIDGET(friends_manager->user_view_recent_updates_tool_button))!=is_sensitive)
		gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_view_recent_updates_tool_button), is_sensitive);
	
	User *user=NULL;
	GtkTreeIter *iter=NULL;
	if(!(is_sensitive && (user=friends_manager_get_selected_user(&iter, friends_manager)))){
		if(gtk_widget_is_sensitive(GTK_WIDGET(friends_manager->user_follow_tool_button)))
			gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_follow_tool_button), FALSE);
		if(gtk_widget_is_sensitive(GTK_WIDGET(friends_manager->user_unfollow_tool_button)))
			gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_unfollow_tool_button), FALSE);
		return;
	}
	
	gchar *following=NULL;
	gtk_tree_model_get(
			GTK_TREE_MODEL(friends_manager->tree_store), iter,
				GSTRING_FOLLOWING, &following,
			-1
	);
	
	is_sensitive=(!g_str_equal(following, "Yes") && user->follower);
	if(gtk_widget_is_sensitive(GTK_WIDGET(friends_manager->user_follow_tool_button))!=is_sensitive)
		gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_follow_tool_button), is_sensitive);
	
	is_sensitive=g_str_equal(following, "Yes");
	if(gtk_widget_is_sensitive(GTK_WIDGET(friends_manager->user_unfollow_tool_button))!=is_sensitive)
		gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_unfollow_tool_button), is_sensitive);
	uber_free(following);
	uber_free(iter);
}/*friends_manager_set_buttons_sensitivity*/

static void friends_manager_tree_view_resized(GtkWidget *widget, GtkAllocation *allocation, FriendsManager *friends_manager){
	const guint8 wrap_width=10;
	gtk_word_wrap_tree_view_column(friends_manager->user_name_tree_view_column, friends_manager->user_name_tree_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(friends_manager->user_nick_name_tree_view_column, friends_manager->user_nick_name_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(friends_manager->friends_tree_view_column, friends_manager->friends_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(friends_manager->follower_tree_view_column, friends_manager->follower_cell_renderer_text, wrap_width);
	gtk_word_wrap_tree_view_column(friends_manager->created_age_tree_view_column, friends_manager->created_age_cell_renderer_text, wrap_width);
}/*friends_manager_tree_view_resized(widget, friends_manager);*/

void friends_manager_show(GtkWindow *parent){
	online_service_request_popup_select_service();
	if(!selected_service) return;
	
	if(!(friends_manager && friends_manager->dialog))
		return friends_manager_setup(parent);
	
	window_present(GTK_WINDOW(friends_manager->dialog), TRUE);
}/*friends_manager_show*/


static void friends_manager_setup(GtkWindow *parent){
	friends_manager=g_new0(FriendsManager, 1);

	/* Get widgets */
	debug("Initialising, building ui, loading, & displaying friends manager from: %s", GTK_BUILDER_UI_FILENAME);
	GtkBuilder *ui=gtkbuilder_get_file(
				GTK_BUILDER_UI_FILENAME,
					"friends_manager", &friends_manager->dialog,
					
					"friends_and_followers_sexy_tree_view", &friends_manager->friends_and_followers_sexy_tree_view,
					"friends_manager_tree_store", &friends_manager->tree_store,
					
					"friends_manager_user_name_tree_view_column", &friends_manager->user_name_tree_view_column,
					"friends_manager_user_name_tree_cell_renderer_text", &friends_manager->user_name_tree_cell_renderer_text,
					"friends_manager_user_nick_name_tree_view_column", &friends_manager->user_nick_name_tree_view_column,
					"friends_manager_user_nick_name_cell_renderer_text", &friends_manager->user_nick_name_cell_renderer_text,
					"friends_manager_friends_tree_view_column", &friends_manager->friends_tree_view_column,
					"friends_manager_friends_cell_renderer_text", &friends_manager->friends_cell_renderer_text,
					"friends_manager_follower_tree_view_column", &friends_manager->follower_tree_view_column,
					"friends_manager_follower_cell_renderer_text", &friends_manager->follower_cell_renderer_text,
					"friends_manager_created_age_tree_view_column", &friends_manager->created_age_tree_view_column,
					"friends_manager_created_age_cell_renderer_text", &friends_manager->created_age_cell_renderer_text,
					
					"users_tool_bar", &friends_manager->users_tool_bar,
					"refresh_tool_button", &friends_manager->refresh_tool_button,
					"user_follow_tool_button", &friends_manager->user_follow_tool_button,
					"user_unfollow_tool_button", &friends_manager->user_unfollow_tool_button,
					"user_block_tool_button", &friends_manager->user_block_tool_button,
					"user_view_profile_tool_button", &friends_manager->user_view_profile_tool_button,
					"user_view_unread_updates_tool_button", &friends_manager->user_view_unread_updates_tool_button,
					"user_view_recent_updates_tool_button", &friends_manager->user_view_recent_updates_tool_button,
					
					"viewer_close_button", &friends_manager->close_button,
				NULL
	);
	
	debug("FriendsManager created now connecting its signal handlers");
	gtkbuilder_connect(
				ui, friends_manager,
					"friends_manager", "destroy", friends_manager_destroy,
					"friends_manager", "response", friends_manager_response,
					
					"friends_and_followers_sexy_tree_view", "size-allocate", friends_manager_tree_view_resized,
					
					"refresh_tool_button", "clicked", friends_manager_refresh,
					"user_follow_tool_button", "clicked", friends_manager_follow,
					"user_unfollow_tool_button", "clicked", friends_manager_unfollow,
					"user_block_tool_button", "clicked", friends_manager_block,
					"user_view_profile_tool_button", "clicked", friends_manager_view_profile,
					"user_view_unread_updates_tool_button", "clicked", friends_manager_view_unread_updates,
					"user_view_recent_updates_tool_button", "clicked", friends_manager_view_recent_updates,
					
					"friends_and_followers_sexy_tree_view", "row-activated", friends_manager_view_profile,
					"friends_and_followers_sexy_tree_view", "cursor-changed", friends_manager_list_clicked,
				NULL
	);
	
	uber_object_unref(ui);
	
	gchar *window_title=g_strdup_printf("%s - <%s>'s %s", _(GETTEXT_PACKAGE), selected_service->key, _("Friends Manager"));
	gtk_window_set_title(GTK_WINDOW(friends_manager->dialog), window_title);
	uber_free(window_title);
	
	gtk_widget_hide(GTK_WIDGET(friends_manager->refresh_tool_button));
	g_object_add_weak_pointer(G_OBJECT(friends_manager->dialog), (gpointer) &friends_manager);
	gtk_window_set_transient_for(GTK_WINDOW(friends_manager->dialog), parent);
	
	/* Now that we're done setting up, let's show the widget */
	window_present(GTK_WINDOW(friends_manager->dialog), TRUE);
	
	statusbar_printf(_("Please wait while friends and followers are loaded.  This will take a few moments..."));
	debug("FriendsManager setup & loaded.  Please wait while friends and followers are loaded");
	GdkCursor *cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(friends_manager->dialog)->window, cursor);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->dialog), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->close_button), TRUE);
	
	/* Load friends_manager */
	users_glist_get(GetBoth, FALSE, friends_manager_display_following_and_followers);
}/*friends_manager_setup*/
