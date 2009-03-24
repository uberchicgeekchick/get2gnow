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
	USER_POINTER
};

typedef struct {
	GtkWidget	*dialog;
	GtkTreeView	*friends_and_followers;
	GtkTreeModel	*friends_and_follows_model;
} Lists;

static void friends_manager_load_friends_and_followers(GList *friends, GList *followers);
static void friends_manager_add_response_cb(GtkButton *button, Lists *friends_manager);
static void friends_manager_rem_response_cb(GtkButton *button, Lists *friends_manager);
static void friends_manager_response_cb(GtkWidget *widget, gint response, Lists *friends_manager);
static void friends_manager_destroy_cb      (GtkWidget   *widget,
								   Lists *friends_manager);

static Lists *friends_manager;

static void
friends_manager_response_cb (GtkWidget     *widget,
				   gint           response,
				   Lists   *friends_manager)
{
	gtk_widget_destroy (widget);
}

static void
friends_manager_destroy_cb (GtkWidget    *widget,
				  Lists  *friends_manager)
{
	g_free (friends_manager);
}

static void friends_manager_add_response_cb(GtkButton   *button, Lists *friends_manager){
	GtkTreeSelection *sel;
	GtkTreeIter       iter;
	User       *user;
	
	/* Get selected Iter */
	sel = gtk_tree_view_get_selection (friends_manager->friends_and_followers);
	
	if (!gtk_tree_selection_get_selected (sel, NULL, &iter))
		return;
	
	gtk_tree_model_get(
				friends_manager->friends_and_follows_model,
				&iter,
				USER_POINTER, &user,
				-1
	);
	
	//gtk_list_store_remove(GTK_LIST_STORE (friends_manager->friends_and_follows_model), &iter);
	
	network_add_user (user);
}

static void friends_manager_rem_response_cb(GtkButton   *button, Lists *friends_manager){
	GtkTreeSelection *sel;
	GtkTreeIter       iter;
	User       *user;

	/* Get selected Iter */
	sel = gtk_tree_view_get_selection (friends_manager->friends_and_followers);
	
	if (!gtk_tree_selection_get_selected (sel, NULL, &iter))
		return;

	gtk_tree_model_get (friends_manager->friends_and_follows_model,
						&iter,
						USER_POINTER, &user,
						-1);

	//gtk_list_store_remove (GTK_LIST_STORE (friends_manager->friends_and_follows_model), &iter);

	network_del_user (user);
}

static void
list_follower_activated_cb (GtkTreeView       *tree_view,
							GtkTreePath       *path,
							GtkTreeViewColumn *column,
							Lists       *friends_manager)
{
	GtkTreeIter  iter;
	gchar       *username;

	gtk_tree_model_get_iter (GTK_TREE_MODEL (friends_manager->friends_and_follows_model),
							 &iter,
							 path);

	gtk_tree_model_get (GTK_TREE_MODEL (friends_manager->friends_and_follows_model),
						&iter,
						USER_NAME, &username,
						-1);

	/* Retrive timeline */
	network_get_user (username);

	g_free (username);
}

static void friends_manager_load_friends_and_followers(GList *friends, GList *followers){
	User		*friend, *follower;
	GtkTreeIter	iter;
	GList		*list, *friends_and_followers=g_list_concat(friends, followers);
	friends_and_followers=g_list_sort(friends_and_followers, (GCompareFunc) parser_sort_users);
	int following_too=0;

	for( list=friends_and_followers; friends_and_followers; friends_and_followers=friends_and_followers->next ){
		friend = (User *)friends_and_followers->data;
		friends_and_followers=friends_and_followers->next;
		follower=(User *)friends_and_followers->data;
		following_too=!g_strcmp0(friend->screen_name, follower->screen_name);
		gtk_list_store_append( GTK_LIST_STORE(friends_manager->friends_and_follows_model), &iter );
		gtk_list_store_set(
					GTK_LIST_STORE (friends_manager->friends_and_follows_model),
					&iter,
					USER_NAME, friend->screen_name,
					USER_NICK, friend->name,
					FOLLOWING_TOO, (following_too?"Yes":"No"),
					USER_POINTER, friend,
					-1
		);
		if(following_too) continue;
		gtk_list_store_append( GTK_LIST_STORE(friends_manager->friends_and_follows_model), &iter );
		gtk_list_store_set(
					GTK_LIST_STORE (friends_manager->friends_and_follows_model),
					&iter,
					USER_NAME, follower->screen_name,
					USER_NICK, follower->name,
					FOLLOWING_TOO, "No",
					USER_POINTER, follower,
					-1
		);
	}
}

void friends_manager_show(GtkWindow *parent){
	if (friends_manager) 
		return gtk_window_present (GTK_WINDOW (friends_manager->dialog));
	
	GtkBuilder	*ui;
	GList		*friends, *followers;
	GdkCursor	*cursor;

	friends_manager = g_new0 (Lists, 1);

	/* Get widgets */
	ui = glade_get_file(GLADE_FILE,
						"friends_manager", &friends_manager->dialog,
						"friends_and_followers", &friends_manager->friends_and_followers,
						NULL);
	/*GtkTreeViewColumn *following_too_column=gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(following_too_column, "Are we following each other?");
	gtk_tree_view_append_column(following_too_column);*/
	
	friends_manager->friends_and_follows_model = gtk_tree_view_get_model (friends_manager->friends_and_followers);

	/* Connect the signals */
	glade_connect (ui, friends_manager,
						"friends_manager", "destroy", friends_manager_destroy_cb,
						"friends_manager", "response", friends_manager_response_cb,
						"follow_add", "clicked", friends_manager_add_response_cb,
						"follow_rem", "clicked", friends_manager_rem_response_cb,
						"friends_and_followers", "row-activated", list_follower_activated_cb,
						NULL);

	g_object_unref(ui);

	/* Set the parent */
	g_object_add_weak_pointer (G_OBJECT (friends_manager->dialog), (gpointer) &friends_manager);
	gtk_window_set_transient_for (GTK_WINDOW (friends_manager->dialog), parent);

	/* Now that we're done setting up, let's show the widget */
	gtk_widget_show (friends_manager->dialog);
	
	app_set_statusbar_msg(_("Please wait while the list of every one you\'re friends_and_followers is loaded."));
	cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(friends_manager->dialog)->window, cursor);
	gtk_widget_set_sensitive(friends_manager->dialog, FALSE);

	/* Load friends_manager */
	if( (friends=network_get_users_glist( (gboolean)TRUE )) && (followers=network_get_users_glist( (gboolean)FALSE )) )
		friends_manager_load_friends_and_followers(friends, followers);
	
	gdk_window_set_cursor(GTK_WIDGET(friends_manager->dialog)->window, NULL);
	gtk_widget_set_sensitive(friends_manager->dialog, TRUE);
	app_set_statusbar_msg("");
}
