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
#include "followers-dialog.h"
#include "add-dialog.h"
#include "network.h"

#define GLADE_FILE "follows_dlg.xml"

enum {
	FOLLOWER_USER,
	FOLLOWER_NAME,
	FOLLOWER_POINTER
};

typedef struct {
	GtkWidget    *dialog;
	GtkTreeView  *following_list;
	GtkTreeModel *following_store;
} Lists;

static void followers_rem_response_cb (GtkButton   *button,
								   Lists *followers);
static void followers_response_cb     (GtkWidget   *widget,
								   gint         response,
								   Lists *followers);
static void followers_destroy_cb      (GtkWidget   *widget,
								   Lists *followers);

static Lists *followers;

static void
followers_response_cb (GtkWidget     *widget,
				   gint           response,
				   Lists   *followers)
{
	gtk_widget_destroy (widget);
}

static void
followers_destroy_cb (GtkWidget    *widget,
				  Lists  *followers)
{
	g_free (followers);
}

static void
followers_rem_response_cb (GtkButton   *button,
					   Lists *followers)
{
	GtkTreeSelection *sel;
	GtkTreeIter       iter;
	User       *user;

	/* Get selected Iter */
	sel = gtk_tree_view_get_selection (followers->following_list);
	
	if (!gtk_tree_selection_get_selected (sel, NULL, &iter))
		return;

	gtk_tree_model_get (followers->following_store,
						&iter,
						FOLLOWER_POINTER, &user,
						-1);

	gtk_list_store_remove (GTK_LIST_STORE (followers->following_store), &iter);

	network_del_user(user->user_name);
}

static void
list_follower_activated_cb (GtkTreeView       *tree_view,
							GtkTreePath       *path,
							GtkTreeViewColumn *column,
							Lists       *followers)
{
	GtkTreeIter  iter;
	gchar       *user_name;

	gtk_tree_model_get_iter (GTK_TREE_MODEL (followers->following_store),
							 &iter,
							 path);

	gtk_tree_model_get (GTK_TREE_MODEL (followers->following_store),
						&iter,
						FOLLOWER_USER, &user_name,
						-1);

	/* Retrive timeline */
	network_get_user(user_name);

	g_free(user_name);
}

void
followers_dialog_load_lists (GList *users)
{
	User  *user;
	GtkTreeIter  iter;
	GList       *list;

	/* Following */
	for (list = users; list; list = list->next)
	{
		user = (User *)list->data;
		gtk_list_store_append (GTK_LIST_STORE (followers->following_store), &iter);

		gtk_list_store_set (GTK_LIST_STORE (followers->following_store),
							&iter,
							FOLLOWER_USER, user->user_name,
							FOLLOWER_NAME, user->nick_name,
							FOLLOWER_POINTER, user,
							-1);
	}
}

void
followers_dialog_show (GtkWindow *parent)
{
	if (followers) 
		return gtk_window_present (GTK_WINDOW (followers->dialog));
	
	GtkBuilder *ui;
	GList      *friends;
	GdkCursor *cursor;

	followers = g_new0 (Lists, 1);

	/* Get widgets */
	ui = glade_get_file (GLADE_FILE,
						"followers_dialog", &followers->dialog,
						"following_list", &followers->following_list,
						NULL);

	followers->following_store = gtk_tree_view_get_model (followers->following_list);

	/* Connect the signals */
	glade_connect (ui, followers,
						"followers_dialog", "destroy", followers_destroy_cb,
						"followers_dialog", "response", followers_response_cb,
						"follow_rem", "clicked", followers_rem_response_cb,
						"following_list", "row-activated", list_follower_activated_cb,
						NULL);

	g_object_unref(ui);

	/* Set the parent */
	g_object_add_weak_pointer (G_OBJECT (followers->dialog), (gpointer) &followers);
	gtk_window_set_transient_for (GTK_WINDOW (followers->dialog), parent);

	/* Now that we're done setting up, let's show the widget */
	gtk_widget_show (followers->dialog);
	
	app_set_statusbar_msg(_("Please wait while the list of every one you\'re following is loaded."));
	cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(followers->dialog)->window, cursor);
	gtk_widget_set_sensitive(followers->dialog, FALSE);

	/* Load followers */
	if( (friends=network_get_friends()) )
		followers_dialog_load_lists(friends);
	
	gdk_window_set_cursor(GTK_WIDGET(followers->dialog)->window, NULL);
	gtk_widget_set_sensitive(followers->dialog, TRUE);
	app_set_statusbar_msg("");
}
