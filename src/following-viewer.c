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
#include "config.h"

#include <glib/gi18n.h>

/********************************************************
 *        Project headers.                              *
 ********************************************************/
#include "gtkbuilder.h"
#include "main.h"
#include "app.h"
#include "following-viewer.h"
#include "popup-dialog.h"
#include "network.h"
#include "profile-viewer.h"
#include "users.h"


/********************************************************
 *        Objects, structures, and etc typedefs         *
 ********************************************************/
enum {
	FOLLOWER_USER,
	FOLLOWER_NAME,
	FOLLOWER_POINTER
};

typedef struct {
	GtkWidget	*viewer;
	GtkTreeView	*following_list;
	GtkTreeModel	*following_store;
	GtkWidget	*user_unfollow;
	GtkWidget	*user_block;
	GtkWidget	*view_profile;
	GtkWidget	*view_timeline;
} Followers;

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define GtkBuilderUI "following-viewer.ui"

static Followers *following;

/********************************************************
 *          Method  & function prototypes               *
 ********************************************************/

void following_viewer_setup(GtkWindow *parent);
static void following_rem_response_cb( GtkButton   *button, Followers *following);
static void following_block_response_cb( GtkButton   *button, Followers *following);
static void following_response_cb( GtkWidget *widget, gint response, Followers *following);
static void following_destroy_cb( GtkWidget *widget, Followers *following );
static void following_view_profile(GtkButton *button, Followers *following);

/********************************************************
 *          My art, code, & programming.                *
 ********************************************************/

static void following_response_cb(GtkWidget *widget, gint response, Followers *following){
	gtk_widget_hide(widget);
}

static void following_destroy_cb( GtkWidget *widget, Followers *following){
	g_free(following);
}

static void following_rem_response_cb( GtkButton *button, Followers *following ){
	GtkTreeSelection *sel;
	GtkTreeIter       iter;
	gchar       *user_name=NULL;

	/* Get selected Iter */
	sel = gtk_tree_view_get_selection (following->following_list);
	
	if (!gtk_tree_selection_get_selected (sel, NULL, &iter))
		return;
	
	gtk_tree_model_get(
			following->following_store,
			&iter,
			FOLLOWER_USER, &user_name,
			-1
	);

	gtk_list_store_remove( GTK_LIST_STORE( following->following_store ), &iter );

	user_request_main(UnFollow, (const gchar *)user_name);
}



static void following_block_response_cb( GtkButton *button, Followers *following ){
	GtkTreeSelection *sel;
	GtkTreeIter       iter;
	gchar       *user_name=NULL;
	
	/* Get selected Iter */
	sel = gtk_tree_view_get_selection (following->following_list);
	
	if (!gtk_tree_selection_get_selected (sel, NULL, &iter))
		return;
	
	gtk_tree_model_get(
				following->following_store,
				&iter,
				FOLLOWER_USER, &user_name,
				-1
	);
	
	gtk_list_store_remove( GTK_LIST_STORE( following->following_store ), &iter );
	
	user_request_main(Block, (const gchar *)user_name);
}

static void following_view_profile(GtkButton *button, Followers *following){
	GtkTreeIter iter;
	gchar *user_name;
	
	
	GtkTreeSelection *selection=gtk_tree_view_get_selection(following->following_list);
	if(!(gtk_tree_selection_get_selected(selection, NULL, &iter))) return;
	
	
	gtk_tree_model_get(
			GTK_TREE_MODEL( following->following_list ),
			&iter,
			FOLLOWER_USER, &user_name,
			-1
	);
	
	if(!user_name) return;
	
	view_profile( user_name, GTK_WINDOW(following->viewer) );
	g_free(user_name);
}//following_view_profile


static void following_view_timeline(GtkButton *button, Followers *following){
	GtkTreeIter iter;
	gchar *user_name;
	
	
	GtkTreeSelection *selection=gtk_tree_view_get_selection(following->following_list);
	if(!(gtk_tree_selection_get_selected(selection, NULL, &iter))) return;
	
	
	gtk_tree_model_get(
			GTK_TREE_MODEL( following->following_list ),
			&iter,
			FOLLOWER_USER, &user_name,
			-1
	);
	
	if(!user_name)
		return;
	
	network_get_user_timeline((const gchar *)user_name);
	g_free(user_name);
}//following_view_timeline


static void
list_following_activated_cb (GtkTreeView       *tree_view,
							GtkTreePath       *path,
							GtkTreeViewColumn *column,
							Followers       *following)
{
	GtkTreeIter  iter;
	gchar       *user_name;

	gtk_tree_model_get_iter (GTK_TREE_MODEL (following->following_store),
							 &iter,
							 path);

	gtk_tree_model_get (GTK_TREE_MODEL (following->following_store),
						&iter,
						FOLLOWER_USER, &user_name,
						-1);

	/* Retrive timeline */
	network_get_user_timeline((const gchar *)user_name);

	g_free(user_name);
	gtk_widget_hide(following->viewer);
	g_free(following);
}

void following_viewer_load_lists (GList *users){
	User  *user;
	GtkTreeIter  iter;
	GList       *list;

	/* Following */
	for (list = users; list; list = list->next)
	{
		user = (User *)list->data;
		gtk_list_store_append( GTK_LIST_STORE(following->following_store), &iter);

		gtk_list_store_set(
					GTK_LIST_STORE(following->following_store),
					&iter,
					FOLLOWER_USER, user->user_name,
					FOLLOWER_NAME, user->nick_name,
					FOLLOWER_POINTER, user,
					-1);
	}
}

void following_viewer_show(GtkWindow *parent){
	if(!(following->viewer && following->viewer ))
		return following_viewer_setup(parent);
	
	gtk_window_present((GTK_WINDOW( following->viewer )) );
}//following_viewer_show


void following_viewer_setup(GtkWindow *parent){
	GtkBuilder *ui;
	GList      *friends;
	GdkCursor *cursor;

	following = g_new0 (Followers, 1);

	/* Get widgets */
	ui = gtkbuilder_get_file (GtkBuilderUI,
						"following_viewer", &following->viewer,
						"following_list", &following->following_list,
						"user_unfollow", &following->user_unfollow,
						"user_block", &following->user_block,
						"view_timeline", &following->view_timeline,
						"view_profile", &following->view_profile,
						NULL);
	
	following->following_store=gtk_tree_view_get_model(following->following_list);
	
	/* Connect the signals */
	gtkbuilder_connect (ui, following,
						"following_viewer", "destroy", following_destroy_cb,
						"following_viewer", "response", following_response_cb,
						"user_unfollow", "clicked", following_rem_response_cb,
						"user_block", "clicked", following_block_response_cb,
						"view_timeline", "clicked", following_view_timeline,
						"view_profile", "clicked", following_view_profile,
						"following_list", "row-activated", list_following_activated_cb,
						NULL);

	g_object_unref(ui);
	
	/* Set the parent */
	g_object_add_weak_pointer (G_OBJECT (following->viewer), (gpointer) &following);
	gtk_window_set_transient_for (GTK_WINDOW (following->viewer), parent);
	
	/* Now that we're done setting up, let's show the widget */
	gtk_widget_show(following->viewer);
	
	app_set_statusbar_msg(_("Please wait while the list of every one you\'re following is loaded."));
	cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(following->viewer)->window, cursor);
	gtk_widget_set_sensitive(following->viewer, FALSE);
	
	/* Load following */
	if( (friends=user_get_friends()) )
		following_viewer_load_lists(friends);
	
	gdk_window_set_cursor(GTK_WIDGET(following->viewer)->window, NULL);
	gtk_widget_set_sensitive(following->viewer, TRUE);
	app_set_statusbar_msg("");
}
