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

#include "config.h"

#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>


#include "gtkbuilder.h"

#include "main.h"
#include "app.h"
#include "friends-manager.h"
#include "popup-dialog.h"
#include "network.h"
#include "images.h"
#include "users.h"

#define GtkBuilderUI "friends-manager.ui"

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
	GtkWidget	*user_follow;
	GtkWidget	*user_unfollow;
	GtkWidget	*user_block;
	GtkWidget	*view_profile;
	GtkWidget	*view_timeline;
} FriendsManager;

static void friends_manager_display_following_and_followers(GList *im_following, GList *my_followers);
static void friends_manager_follow_response_cb(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_unfollow_response_cb(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_block_response_cb(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_response_cb(GtkWidget *widget, gint response, FriendsManager *friends_manager);
static void friends_manager_destroy_cb(GtkWidget *widget, FriendsManager *friends_manager);
static void friends_manager_list_clicked(GtkTreeView *tree_view, FriendsManager *friends_manager);
static void friends_manager_set_buttons_sensitivity(FriendsManager *friends_manager, gboolean is_sensitive);
static void friends_manager_view_profile(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_view_timeline(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_friend_selected(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, FriendsManager *friends_manager);
static void friends_manager_popup_profile( FriendsManager *friends_manager, GtkTreeIter iter );


static FriendsManager *friends_manager;



static void
friends_manager_response_cb( GtkWidget *widget, gint response, FriendsManager *friends_manager){
	gtk_widget_destroy(widget);
}

static void friends_manager_destroy_cb( GtkWidget *widget, FriendsManager *friends_manager ){
	g_free(friends_manager);
}

static void friends_manager_follow_response_cb(GtkButton   *button, FriendsManager *friends_manager){
	GtkTreeSelection *sel;
	GtkTreeIter       iter;
	gchar *following_too;
	gchar       *user_name;
	
	/* Get selected Iter */
	sel = gtk_tree_view_get_selection (friends_manager->friends_and_followers);
	
	if (!gtk_tree_selection_get_selected (sel, NULL, &iter))
		return;
	
	gtk_tree_model_get(
				friends_manager->friends_and_follows_model,
				&iter,
				FOLLOWING_TOO, &following_too,
				USER_NAME, &user_name,
				-1
	);

	if(!(g_strcmp0(following_too, "No")))
		gtk_list_store_set(
					GTK_LIST_STORE(friends_manager->friends_and_follows_model),
					&iter,
					FOLLOWING_TOO, "Yes",
					-1
		);
	
	
	network_follow_user(user_name);
}

static void friends_manager_unfollow_response_cb(GtkButton   *button, FriendsManager *friends_manager){
	GtkTreeIter		iter;
	gchar			*user_name=NULL, *following_too=NULL;
	
	/* Get selected Iter */
	GtkTreeSelection *sel=gtk_tree_view_get_selection( friends_manager->friends_and_followers );
	
	if(!(gtk_tree_selection_get_selected( sel, NULL, &iter )))
		return;

	gtk_tree_model_get(
				friends_manager->friends_and_follows_model,
				&iter,
				FOLLOWING_TOO, &following_too,
				USER_NAME, &user_name,
				-1
	);

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

	network_unfollow_user(user_name);
}

static void friends_manager_block_response_cb(GtkButton   *button, FriendsManager *friends_manager){
	GtkTreeIter		iter;
	gchar			*user_name=NULL, *following_too=NULL;
	
	/* Get selected Iter */
	GtkTreeSelection *sel=gtk_tree_view_get_selection( friends_manager->friends_and_followers );
	
	if(!(gtk_tree_selection_get_selected( sel, NULL, &iter )))
		return;

	gtk_tree_model_get(
				friends_manager->friends_and_follows_model,
				&iter,
				FOLLOWING_TOO, &following_too,
				USER_NAME, &user_name,
				-1
	);

	gtk_list_store_remove (GTK_LIST_STORE (friends_manager->friends_and_follows_model), &iter);
	friends_manager_set_buttons_sensitivity(friends_manager, FALSE);

	network_block_user(user_name);
}

static void friends_manager_list_clicked(GtkTreeView *tree_view, FriendsManager *friends_manager){
	friends_manager_set_buttons_sensitivity(friends_manager, TRUE);
}//friends_manager_list_clicked

static void friends_manager_set_buttons_sensitivity(FriendsManager *friends_manager, gboolean is_sensitive){
	gtk_widget_set_sensitive(friends_manager->user_follow, is_sensitive);
	gtk_widget_set_sensitive(friends_manager->user_unfollow, is_sensitive);
	gtk_widget_set_sensitive(friends_manager->user_block, is_sensitive);
	gtk_widget_set_sensitive(friends_manager->view_profile, is_sensitive);
	gtk_widget_set_sensitive(friends_manager->view_timeline, is_sensitive);
}//friends_manager_set_buttons_sensitivity


static void friends_manager_view_profile(GtkButton *button, FriendsManager *friends_manager){
	User *profile;
	GtkTreeIter iter;

	GtkTreeSelection *selection=gtk_tree_view_get_selection(friends_manager->friends_and_followers);
	gtk_tree_selection_get_selected(selection, NULL, &iter);

	friends_manager_popup_profile(friends_manager, iter);
}//friends_manager_view_profile


static void friends_manager_view_timeline(GtkButton *button, FriendsManager *friends_manager){
	GtkTreeIter iter;
	gchar *user_name;
	
	
	GtkTreeSelection *selection=gtk_tree_view_get_selection(friends_manager->friends_and_followers);
	gtk_tree_selection_get_selected(selection, NULL, &iter);
	
	
	gtk_tree_model_get(
			GTK_TREE_MODEL( friends_manager->friends_and_follows_model ),
			&iter,
			USER_NAME, &user_name,
			-1
	);
	
	network_get_user((const gchar *)user_name);
	
	g_free(user_name);
}//friends_manager_view_timeline


static void friends_manager_friend_selected( GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, FriendsManager *friends_manager ){
	GtkTreeIter iter;
	gtk_tree_model_get_iter(
				GTK_TREE_MODEL( friends_manager->friends_and_follows_model ),
				&iter,
				path
	);
	friends_manager_popup_profile(friends_manager, iter);
}//friends_manager_friend_selected

static void friends_manager_popup_profile( FriendsManager *friends_manager, GtkTreeIter iter ){
	gchar *user_name=NULL;
	gtk_tree_model_get(
			GTK_TREE_MODEL( friends_manager->friends_and_follows_model ),
			&iter,
			USER_NAME, &user_name,
			-1
	);
	
	
	User *profile=network_fetch_profile(user_name);;
	
	
	GtkMessageDialog *dialog=(GtkMessageDialog *)gtk_message_dialog_new_with_markup(
			GTK_WINDOW(app_priv->window),
			( GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT ),
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_CLOSE,
			NULL
	);
	g_signal_connect( dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog );
	
	
	GtkImage *image;
	if(g_str_equal("unknown_image", profile->image_filename)){
		network_download_avatar( profile->image_url );
		profile->image_filename=images_get_filename( profile->image_url );
	}
	
	gtk_message_dialog_set_image( GTK_MESSAGE_DIALOG( dialog ), GTK_WIDGET(image=images_get_image_from_filename( profile->image_filename )) );

	gchar *message=g_strdup_printf(
			"<u><b>%s:</b> %s</u>\n\t<b>Tweets:</b> %lu\n\t<b>Friends:</b> %lu\n\t<b>Followers:</b> %lu\n\t<b>Location:</b> %s\n\t<b>URL:</b> <a href=\"%s\">%s</a>\n\t<b>Bio:</b>\n\t\t%s\n",
			profile->user_name, profile->nick_name,
			profile->tweets,
			profile->friends,
			profile->followers,
			profile->location,
			profile->url,
			profile->url,
			profile->bio
	);
	gtk_message_dialog_set_markup( GTK_MESSAGE_DIALOG( dialog ), message );
	
	
	gtk_widget_show( GTK_WIDGET( dialog ) );
	
	g_free(message);

	//gtk_widget_destroy( GTK_WIDGET( image ) );
	
	g_free(profile);
}//friends_manager_popup_profile

static void friends_manager_display_following_and_followers(GList *im_following, GList *my_followers){
	User		*following, *follower;
	GtkTreeIter	iter;
	GList		*list, *following_and_followers=g_list_alloc();
	following_and_followers=g_list_concat(im_following, my_followers);
	following_and_followers=g_list_sort(following_and_followers, (GCompareFunc) usrcasecmp);
	gboolean following_too=FALSE, list_break=FALSE;
	
	for( list=following_and_followers; list; list=list->next ){
		following = (User *)list->data;
		if(!( (list=list->next) && list->data ))
			list_break=TRUE;
		else{
			follower=(User *)list->data;
			following_too=(gboolean)!g_strcmp0( following->user_name, follower->user_name );
			if(!following_too)
				list=list->prev;
		}
		gtk_list_store_append( GTK_LIST_STORE(friends_manager->friends_and_follows_model), &iter );
		gtk_list_store_set(
					GTK_LIST_STORE(friends_manager->friends_and_follows_model),
					&iter,
					USER_NAME, following->user_name,
					USER_NICK, following->nick_name,
					FOLLOWING_TOO, (following_too?"Yes":"No"),
					FOLLOWER_TOO, (following->follower?"Yes":"No"),
					FRIEND_POINTER, following,
					-1
		);
		if(list_break) break;
	}
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
	ui = gtkbuilder_get_file(GtkBuilderUI,
						"friends_manager", &friends_manager->dialog,
						"friends_and_followers", &friends_manager->friends_and_followers,
						"user_follow", &friends_manager->user_follow,
						"user_unfollow", &friends_manager->user_unfollow,
						"user_unfollow", &friends_manager->user_block,
						"view_profile", &friends_manager->view_profile,
						"view_timeline", &friends_manager->view_timeline,
						NULL);
	
	friends_manager->friends_and_follows_model = gtk_tree_view_get_model (friends_manager->friends_and_followers);

	/* Connect the signals */
	gtkbuilder_connect (ui, friends_manager,
						"friends_manager", "destroy", friends_manager_destroy_cb,
						"friends_manager", "response", friends_manager_response_cb,
						"user_follow", "clicked", friends_manager_follow_response_cb,
						"user_unfollow", "clicked", friends_manager_unfollow_response_cb,
						"user_block", "clicked", friends_manager_block_response_cb,
						"view_profile", "clicked", friends_manager_view_profile,
						"view_timeline", "clicked", friends_manager_view_timeline,
						"friends_and_followers", "row-activated", friends_manager_friend_selected,
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
