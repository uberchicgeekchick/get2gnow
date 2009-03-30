/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2008 Daniel Morales <daniminas@gmail.com>
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
 * Authors: Daniel Morales <daniminas@gmail.com>
 * 		Kaity G. B. <uberChick@uberChicGeekChick.Com>
 *
 */

#include "config.h"

#include <glib/gi18n.h>
#include <glib/gprintf.h>

#include "glade.h"

#include "main.h"
#include "app.h"
#include "friends-manager.h"
#include "add-dialog.h"
#include "network.h"

#define GLADE_FILE "friends_manager.xml"

enum {
	USER_NAME,
	USER_NICK,
	FOLLOWING_TOO,
	FOLLOWER_TOO,
	FRIEND_POINTER
};

typedef struct {
	GtkWidget	*dialog;
	GtkTreeView	*friends_and_followers;
	GtkTreeModel	*friends_and_follows_model;
	GtkWidget	*follow_add;
	GtkWidget	*follow_rem;
	GtkWidget	*view_profile;
} FriendsManager;

static void friends_manager_display_following_and_followers(GList *im_following, GList *my_followers);
static void friends_manager_add_response_cb(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_rem_response_cb(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_response_cb(GtkWidget *widget, gint response, FriendsManager *friends_manager);
static void friends_manager_destroy_cb(GtkWidget *widget, FriendsManager *friends_manager);
static void friends_manager_list_activated(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, FriendsManager *friends_manager);
static void friends_manager_list_clicked(GtkTreeView *tree_view, FriendsManager *friends_manager);
static void friends_manager_set_buttons_sensitivity(FriendsManager *friends_manager, gboolean is_sensitive);
static void friends_manager_popup_profile(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, FriendsManager *friends_manager);
static FriendsManager *friends_manager;

static void
friends_manager_response_cb( GtkWidget *widget, gint response, FriendsManager *friends_manager){
	gtk_widget_destroy(widget);
}

static void friends_manager_destroy_cb( GtkWidget *widget, FriendsManager *friends_manager ){
	g_free(friends_manager);
}

static void friends_manager_add_response_cb(GtkButton   *button, FriendsManager *friends_manager){
	GtkTreeSelection *sel;
	GtkTreeIter       iter;
	gchar *following_too;
	User       *user;
	
	/* Get selected Iter */
	sel = gtk_tree_view_get_selection (friends_manager->friends_and_followers);
	
	if (!gtk_tree_selection_get_selected (sel, NULL, &iter))
		return;
	
	gtk_tree_model_get(
				friends_manager->friends_and_follows_model,
				&iter,
				FOLLOWING_TOO, &following_too,
				FRIEND_POINTER, &user,
				-1
	);

	if(!(g_strcmp0(following_too, "No")))
		gtk_list_store_set(
					GTK_LIST_STORE(friends_manager->friends_and_follows_model),
					&iter,
					FOLLOWING_TOO, "Yes",
					-1
		);
	
	
	network_add_user(user->user_name);
}

static void friends_manager_rem_response_cb(GtkButton   *button, FriendsManager *friends_manager){
	GtkTreeSelection *sel;
	GtkTreeIter       iter;
	User       *user;
	gchar *following_too;

	/* Get selected Iter */
	sel = gtk_tree_view_get_selection (friends_manager->friends_and_followers);
	
	if (!gtk_tree_selection_get_selected (sel, NULL, &iter))
		return;

	gtk_tree_model_get (friends_manager->friends_and_follows_model,
						&iter,
						FOLLOWING_TOO, &following_too,
						FRIEND_POINTER, &user,
						-1);

	if(!(g_strcmp0(following_too, "Yes")))
		gtk_list_store_set(
				GTK_LIST_STORE(friends_manager->friends_and_follows_model),
				&iter,
				FOLLOWING_TOO, "No",
				-1
		);
	else {
		gtk_list_store_remove (GTK_LIST_STORE (friends_manager->friends_and_follows_model), &iter);
		friends_manager_set_buttons_sensitivity(friends_manager, FALSE);
	}

	network_del_user(user->user_name);
}

static void friends_manager_list_clicked(GtkTreeView *tree_view, FriendsManager *friends_manager){
	friends_manager_set_buttons_sensitivity(friends_manager, TRUE);
}//friends_manager_list_clicked

static void friends_manager_set_buttons_sensitivity(FriendsManager *friends_manager, gboolean is_sensitive){
	gtk_widget_set_sensitive(friends_manager->follow_add, is_sensitive);
	gtk_widget_set_sensitive(friends_manager->follow_rem, is_sensitive);
	/* TODO:
	 * 	Make the about button actually pull the user's profile information.
	 * 	Display it in a gtk_dialog_new_with_buttons
	 * gtk_widget_set_sensitive(friends_manager->view_profile, is_sensitive);
	 */
}//friends_manager_set_buttons_sensitivity


static void friends_manager_list_activated( GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, FriendsManager *friends_manager ){
	return;
	friends_manager_popup_profile( tree_view, path, column, friends_manager );
}//friends_manager_list_activated

static void friends_manager_popup_profile( GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, FriendsManager *friends_manager ){
	GtkTreeIter  iter;
	gchar       *username;
	OnlineProfile *profile;

	gtk_tree_model_get_iter (GTK_TREE_MODEL (friends_manager->friends_and_follows_model),
							 &iter,
							 path);

	gtk_tree_model_get (GTK_TREE_MODEL (friends_manager->friends_and_follows_model),
						&iter,
						USER_NAME, &username,
						-1);

	/* Retrive timeline */
	profile=network_get_user_profile(username);

	g_free(profile);
	g_free(username);
}

static void friends_manager_display_following_and_followers(GList *im_following, GList *my_followers){
	User		*following, *follower;
	GtkTreeIter	iter;
	GList		*list, *following_and_followers=g_list_alloc();
	following_and_followers=g_list_concat(im_following, my_followers);
	following_and_followers=g_list_sort(following_and_followers, (GCompareFunc) parser_sort_users);
	gboolean follower_too=FALSE, list_break=FALSE, following_user=TRUE;
	
	for( list=following_and_followers; list; list=list->next ){
		following = (User *)list->data;
		if(!( (list=list->next) && list->data ))
			list_break=TRUE;
		else{
			follower=(User *)list->data;
			follower_too=(gboolean)!g_strcmp0( following->user_name, follower->user_name );
			if(!follower_too) {
				list=list->prev;
				following_user=FALSE;
			} else following_user=TRUE;
		}
		gtk_list_store_append( GTK_LIST_STORE(friends_manager->friends_and_follows_model), &iter );
		gtk_list_store_set(
					GTK_LIST_STORE(friends_manager->friends_and_follows_model),
					&iter,
					USER_NAME, following->user_name,
					USER_NICK, following->nick_name,
					FOLLOWING_TOO, (following_user?"Yes":"No"),
					FOLLOWER_TOO, (follower_too?"Yes":"No"),
					FRIEND_POINTER, following,
					-1
		);
		if(list_break) break;
	}
	/*g_list_free(list);
	g_list_free(following_and_followers);*/
	g_free(following);
	g_free(follower);
}//friends_manager_display_following_and_followers

void friends_manager_show(GtkWindow *parent){
	if (friends_manager) 
		return gtk_window_present (GTK_WINDOW (friends_manager->dialog));
	
	GtkBuilder	*ui;
	GdkCursor	*cursor;

	friends_manager = g_new0 (FriendsManager, 1);

	/* Get widgets */
	ui = glade_get_file(GLADE_FILE,
						"friends_manager", &friends_manager->dialog,
						"friends_and_followers", &friends_manager->friends_and_followers,
						"follow_add", &friends_manager->follow_add,
						"follow_rem", &friends_manager->follow_rem,
						"view_profile", &friends_manager->view_profile,
						NULL);
	
	friends_manager->friends_and_follows_model = gtk_tree_view_get_model (friends_manager->friends_and_followers);

	/* Connect the signals */
	glade_connect (ui, friends_manager,
						"friends_manager", "destroy", friends_manager_destroy_cb,
						"friends_manager", "response", friends_manager_response_cb,
						"follow_add", "clicked", friends_manager_add_response_cb,
						"follow_rem", "clicked", friends_manager_rem_response_cb,
						"view_profile", "clicked", friends_manager_popup_profile,
						"friends_and_followers", "row-activated", friends_manager_list_activated,
						"friends_and_followers", "cursor-changed", friends_manager_list_clicked,
						NULL);

	g_object_unref(ui);

	/* Set the parent */
	g_object_add_weak_pointer (G_OBJECT (friends_manager->dialog), (gpointer) &friends_manager);
	gtk_window_set_transient_for (GTK_WINDOW (friends_manager->dialog), parent);

	/* Now that we're done setting up, let's show the widget */
	gtk_widget_show (friends_manager->dialog);
	
	app_set_statusbar_msg(_("Please wait while friends and followers are loaded.  This may take several minutes..."));
	cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(friends_manager->dialog)->window, cursor);
	gtk_widget_set_sensitive(friends_manager->dialog, FALSE);

	/* Load friends_manager */
	GList *im_following, *my_followers;
	if( ( ((im_following=network_get_users_glist( TRUE ))) && (my_followers=network_get_users_glist( FALSE )) ) )
		friends_manager_display_following_and_followers(im_following, my_followers);
	
	gdk_window_set_cursor(GTK_WIDGET(friends_manager->dialog)->window, NULL);
	gtk_widget_set_sensitive(friends_manager->dialog, TRUE);
	app_set_statusbar_msg("");
}
