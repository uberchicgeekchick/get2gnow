/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Greet-Tweet-Know is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the RPL
 *
 * For more information or to find the latest release, visit our
 * website at: http://uberChicGeekChick.Com/?projects=Greet-Tweet-Know
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
#include "followers-dialog.h"
#include "popup-dialog.h"
#include "network.h"
#include "profile-viewer.h"


/********************************************************
 *        Objects, structures, and etc typedefs         *
 ********************************************************/
enum {
	FOLLOWER_USER,
	FOLLOWER_NAME,
	FOLLOWER_POINTER
};

typedef struct {
	GtkWidget	*dialog;
	GtkTreeView	*following_list;
	GtkTreeModel	*following_store;
	GtkWidget	*user_unfollow;
	GtkWidget	*user_block;
	GtkWidget	*view_profile;
} Followers;

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define GtkBuilderUI "followers-dialog.ui"

static Followers *followers;

/********************************************************
 *          Method  & function prototypes               *
 ********************************************************/

static void followers_rem_response_cb( GtkButton   *button, Followers *followers);
static void followers_block_response_cb( GtkButton   *button, Followers *followers);
static void followers_response_cb( GtkWidget *widget, gint response, Followers *followers);
static void followers_destroy_cb( GtkWidget *widget, Followers *followers );
static void followers_view_profile(GtkButton *button, Followers *followers);

/********************************************************
 *          My art, code, & programming.                *
 ********************************************************/

static void
followers_response_cb (GtkWidget     *widget,
				   gint           response,
				   Followers   *followers)
{
	gtk_widget_destroy (widget);
}

static void
followers_destroy_cb (GtkWidget    *widget,
				  Followers  *followers)
{
	g_free (followers);
}

static void followers_rem_response_cb( GtkButton *button, Followers *followers ){
	GtkTreeSelection *sel;
	GtkTreeIter       iter;
	gchar       *user_name=NULL;

	/* Get selected Iter */
	sel = gtk_tree_view_get_selection (followers->following_list);
	
	if (!gtk_tree_selection_get_selected (sel, NULL, &iter))
		return;
	
	gtk_tree_model_get(
			followers->following_store,
			&iter,
			FOLLOWER_USER, &user_name,
			-1
	);

	gtk_list_store_remove( GTK_LIST_STORE( followers->following_store ), &iter );

	network_unfollow_user((const gchar *)user_name);
}



static void followers_block_response_cb( GtkButton *button, Followers *followers ){
	GtkTreeSelection *sel;
	GtkTreeIter       iter;
	gchar       *user_name=NULL;
	
	/* Get selected Iter */
	sel = gtk_tree_view_get_selection (followers->following_list);
	
	if (!gtk_tree_selection_get_selected (sel, NULL, &iter))
		return;
	
	gtk_tree_model_get(
				followers->following_store,
				&iter,
				FOLLOWER_USER, &user_name,
				-1
	);
	
	gtk_list_store_remove( GTK_LIST_STORE( followers->following_store ), &iter );
	
	network_block_user((const gchar *)user_name);
}

static void followers_view_profile(GtkButton *button, Followers *followers){
	GtkTreeIter iter;
	gchar *user_name;
	
	
	GtkTreeSelection *selection=gtk_tree_view_get_selection(followers->following_list);
	if(!(gtk_tree_selection_get_selected(selection, NULL, &iter))) return;
	
	
	gtk_tree_model_get(
			GTK_TREE_MODEL( followers->following_list ),
			&iter,
			FOLLOWER_USER, &user_name,
			-1
	);

	if(!user_name)
		return;
	
	view_profile( user_name, GTK_WINDOW(followers->dialog) );
	g_free(user_name);
}//followers_view_profile


static void
list_follower_activated_cb (GtkTreeView       *tree_view,
							GtkTreePath       *path,
							GtkTreeViewColumn *column,
							Followers       *followers)
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
	gtk_widget_destroy(followers->dialog);
	g_free(followers);
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
		gtk_list_store_append( GTK_LIST_STORE(followers->following_store), &iter);

		gtk_list_store_set(
					GTK_LIST_STORE(followers->following_store),
					&iter,
					FOLLOWER_USER, user->user_name,
					FOLLOWER_NAME, user->nick_name,
					FOLLOWER_POINTER, user,
					-1);
	}
}

void followers_dialog_show (GtkWindow *parent){
	if (followers) 
		return gtk_window_present (GTK_WINDOW (followers->dialog));
	
	GtkBuilder *ui;
	GList      *friends;
	GdkCursor *cursor;

	followers = g_new0 (Followers, 1);

	/* Get widgets */
	ui = gtkbuilder_get_file (GtkBuilderUI,
						"followers_dialog", &followers->dialog,
						"following_list", &followers->following_list,
						"user_unfollow", &followers->user_unfollow,
						"user_block", &followers->user_block,
						"view_profile", &followers->view_profile,
						NULL);
	
	followers->following_store=gtk_tree_view_get_model(followers->following_list);
	
	/* Connect the signals */
	gtkbuilder_connect (ui, followers,
						"followers_dialog", "destroy", followers_destroy_cb,
						"followers_dialog", "response", followers_response_cb,
						"user_unfollow", "clicked", followers_rem_response_cb,
						"user_block", "clicked", followers_block_response_cb,
						"view_profile", "clicked", followers_view_profile,
						"following_list", "row-activated", list_follower_activated_cb,
						NULL);

	g_object_unref(ui);
	
	/* Set the parent */
	g_object_add_weak_pointer (G_OBJECT (followers->dialog), (gpointer) &followers);
	gtk_window_set_transient_for (GTK_WINDOW (followers->dialog), parent);
	
	/* Now that we're done setting up, let's show the widget */
	gtk_widget_show(followers->dialog);
	
	app_set_statusbar_msg(_("Please wait while the list of every one you\'re following is loaded."));
	cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(followers->dialog)->window, cursor);
	gtk_widget_set_sensitive(followers->dialog, FALSE);
	
	/* Load followers */
	if( (friends=user_get_friends()) )
		followers_dialog_load_lists(friends);
	
	gdk_window_set_cursor(GTK_WIDGET(followers->dialog)->window, NULL);
	gtk_widget_set_sensitive(followers->dialog, TRUE);
	app_set_statusbar_msg("");
}
