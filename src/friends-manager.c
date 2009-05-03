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

#include "config.h"

#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>


#include "gtkbuilder.h"

#include "main.h"
#include "app.h"
#include "debug.h"
#include "friends-manager.h"
#include "popup-dialog.h"
#include "network.h"
#include "images.h"
#include "users.h"
#include "profile-viewer.h"


enum {
	USER_NAME,
	USER_SERVICE,
	USER_NICK,
	FOLLOWING,
	FOLLOWER,
	FRIEND_POINTER,
};

typedef struct {
	GtkWindow	*dialog;
	GtkTreeView	*friends_and_followers;
	GtkTreeModel	*friends_and_follows_model;
	GtkButton	*refresh;
	GtkButton	*user_follow;
	GtkButton	*user_unfollow;
	GtkButton	*user_block;
	GtkButton	*view_profile;
	GtkButton	*view_timeline;
} FriendsManager;

#define DEBUG_DOMAIN "Friends-Manager"

#define GtkBuilderUI "friends-manager.ui"

/* GtkBuilder methods.
 * 'friends_manager_show' is this object's public method & prototyped in 'friends-manager.h' */
static void friends_manager_setup( GtkWindow *parent );
static void friends_manager_display_following_and_followers(gboolean refresh);

/* UI event handlers */
static void friends_manager_refresh(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_follow(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_unfollow(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_block(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_free(void);
static void friends_manager_set_buttons_sensitivity(FriendsManager *friends_manager, gboolean is_sensitive);

/* signal handlers */
static void friends_manager_destroy(GtkWidget *widget, FriendsManager *friends_manager);
static void friends_manager_response(GtkWidget *widget, gint response, FriendsManager *friends_manager);
static void friends_manager_list_clicked(GtkTreeView *tree_view, FriendsManager *friends_manager);
static void friends_manager_view_profile(void);
static void friends_manager_view_timeline(GtkButton *button, FriendsManager *friends_manager);


static FriendsManager *friends_manager=NULL;


static void friends_manager_destroy(GtkWidget *widget, FriendsManager *friends_manager){
	debug(DEBUG_DOMAIN, "Destroying friends manager");
	friends_manager_free();
}//friends_manager_destroy


static void friends_manager_response(GtkWidget *widget, gint response, FriendsManager *friends_manager){
	gtk_widget_destroy(widget);
}//friends_message_response


static void friends_manager_free(void){
	if(!(friends_manager)) return;
	
	if(friends_manager->user_follow) {
		gtk_widget_destroy( GTK_WIDGET(friends_manager->user_follow) );
		friends_manager->user_follow=NULL;
	}
	
	if(friends_manager->user_unfollow) {
		gtk_widget_destroy( GTK_WIDGET(friends_manager->user_unfollow) );
		friends_manager->user_unfollow=NULL;
	}
	
	if(friends_manager->user_block) {
		gtk_widget_destroy( GTK_WIDGET(friends_manager->user_block) );
		friends_manager->user_block=NULL;
	}
	
	if(friends_manager->view_profile) {
		gtk_widget_destroy( GTK_WIDGET(friends_manager->view_profile) );
		friends_manager->view_profile=NULL;
	}
	
	if(friends_manager->view_timeline) {
		gtk_widget_destroy( GTK_WIDGET(friends_manager->view_timeline) );
		friends_manager->view_timeline=NULL;
	}
	
	if(friends_manager->dialog) {
		gtk_widget_destroy( GTK_WIDGET(friends_manager->dialog) );
		friends_manager->dialog=NULL;
	}
	
	g_free(friends_manager);
	friends_manager=NULL;
}//friends_manager_free

static void friends_manager_refresh(GtkButton *button, FriendsManager *friends_manager){
	friends_manager_display_following_and_followers(TRUE);
}//friends_manager_refresh

static void friends_manager_follow(GtkButton   *button, FriendsManager *friends_manager){
	GtkTreeSelection	*sel;
	GtkTreeIter		*iter=NULL;
	gchar			*following, *user_name;
	
	/* Get selected Iter */
	if(!( (sel=gtk_tree_view_get_selection(friends_manager->friends_and_followers)) && gtk_tree_selection_get_selected(sel, NULL, iter) ))
		return;
	
	gtk_tree_model_get(
				friends_manager->friends_and_follows_model,
				iter,
				FOLLOWING, &following,
				USER_NAME, &user_name,
				-1
	);
	
	if(G_STR_EMPTY(user_name)) {
		if(user_name) g_free(user_name);
		if(following) g_free(following);
		return;
	}

	if(g_str_equal(following, "No")){
		gtk_list_store_set(
					GTK_LIST_STORE(friends_manager->friends_and_follows_model),
					iter,
					FOLLOWING, "Yes",
					-1
		);
		user_request_main(Follow, friends_manager->dialog, (const gchar *)user_name);
	}
	
	gtk_tree_selection_select_iter(sel, iter);
	
	g_free(following);
	g_free(user_name);
}

static void friends_manager_unfollow(GtkButton   *button, FriendsManager *friends_manager){
	GtkTreeSelection	*sel=NULL;
	GtkTreeIter		*iter=NULL;
	gchar			*user_name=NULL, *following=NULL;
	
	/* Get selected Iter */
	if(!( (sel=gtk_tree_view_get_selection(friends_manager->friends_and_followers)) && gtk_tree_selection_get_selected(sel, NULL, iter) ))
		return;

	gtk_tree_model_get(
				friends_manager->friends_and_follows_model,
				iter,
				FOLLOWING, &following,
				USER_NAME, &user_name,
				-1
	);
	
	
	if(G_STR_EMPTY(user_name)) {
		if(user_name) g_free(user_name);
		if(following) g_free(following);
		return;
	}
	
	if(g_str_equal(following, "Yes")){
		user_request_main(UnFollow, friends_manager->dialog, (const gchar *)user_name);
		gtk_list_store_set(
				GTK_LIST_STORE(friends_manager->friends_and_follows_model),
				iter,
				FOLLOWING, "No",
				-1
		);
	} else {
		gtk_list_store_remove(GTK_LIST_STORE (friends_manager->friends_and_follows_model), iter);
		friends_manager_set_buttons_sensitivity(friends_manager, FALSE);
	}
	
	g_free(following);
	g_free(user_name);
}

static void friends_manager_block(GtkButton   *button, FriendsManager *friends_manager){
	GtkTreeSelection	*sel=NULL;
	GtkTreeIter		*iter=NULL;
	gchar			*user_name=NULL, *following=NULL;
	
	/* Get selected Iter */
	if(!( (sel=gtk_tree_view_get_selection(friends_manager->friends_and_followers)) && gtk_tree_selection_get_selected(sel, NULL, iter) ))
		return;

	gtk_tree_model_get(
				friends_manager->friends_and_follows_model,
				iter,
				FOLLOWING, &following,
				USER_NAME, &user_name,
				-1
	);

	if(G_STR_EMPTY(user_name)) {
		if(user_name) g_free(user_name);
		if(following) g_free(following);
		return;
	}
	
	user_request_main(Block, friends_manager->dialog, (const gchar *)user_name);
	gtk_list_store_remove(GTK_LIST_STORE (friends_manager->friends_and_follows_model), iter);
	friends_manager_set_buttons_sensitivity(friends_manager, FALSE);

	g_free(user_name);
	g_free(following);
}

/* This handles viewing profiles via button clicks & 'activating the tree_view.
 * 	optional parameters for button clicks are:
 *	 	GtkButtion *button
 *
 *	 for when the tree_view is activated the optional parameters are:
 *	 	GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column
 *
 *	 both methods are also sent gpointer user_data which is a pointer to FriendsManager *friends_manager
 */
void friends_manager_view_profile(void){
	GtkTreeSelection	*selection=NULL;
	gchar			*user_name=NULL;
	GtkTreeIter		*iter=NULL;

	if(!( (selection=gtk_tree_view_get_selection(friends_manager->friends_and_followers)) && gtk_tree_selection_get_selected(selection, NULL, iter) ))
		return;

	gtk_tree_model_get(
				GTK_TREE_MODEL(friends_manager->friends_and_follows_model),
				iter,
				USER_NAME, &user_name,
				-1
	);
	
	if(G_STR_EMPTY(user_name)) {
		if(user_name) g_free(user_name);
		return;
	}
	
	user_request_main(ViewProfile, friends_manager->dialog, (const gchar *)user_name);
	g_free( user_name );
}//friends_manager_view_profile


static void friends_manager_view_timeline(GtkButton *button, FriendsManager *friends_manager){
	GtkTreeSelection	*selection=NULL;
	GtkTreeIter		*iter=NULL;
	gchar			*user_name=NULL;

	if(!( (selection=gtk_tree_view_get_selection(friends_manager->friends_and_followers)) && gtk_tree_selection_get_selected(selection, NULL, iter) ))
		return;
	
	gtk_tree_model_get(
				GTK_TREE_MODEL(friends_manager->friends_and_follows_model),
				iter,
				USER_NAME, &user_name,
				-1
	);
	
	network_get_user_timeline((const gchar *)user_name);
	g_free(user_name);
}//friends_manager_view_timeline


static void friends_manager_display_following_and_followers(gboolean refresh){
	User		*following, *follower;
	GtkTreeIter	*iter=NULL;
	GList		*list;

	GList *friends_and_followers=user_get_friends_and_followers(refresh);
	
	gboolean following_too=FALSE, follower_too=FALSE;
	for( list=friends_and_followers; list; list=list->next ){
		following=(User *)list->data;
		if(!list->next)
			following_too=!(follower_too=following->follower);
		else{
			list=list->next;
			follower=(User *)list->data;
			if(g_str_equal( following->user_name, follower->user_name ))
				follower_too=following_too=TRUE;
			else{
				list=list->prev;
				following_too=!(follower_too=following->follower);
			}
		}
		gtk_list_store_append( GTK_LIST_STORE(friends_manager->friends_and_follows_model), iter );
		gtk_list_store_set(
					GTK_LIST_STORE(friends_manager->friends_and_follows_model),
					iter,
					USER_NAME, following->user_name,
					USER_NICK, following->nick_name,
					FOLLOWING, (following_too?"Yes":"No"),
					FOLLOWER, (follower_too?"Yes":"No"),
					FRIEND_POINTER, following,
					-1
		);
	}
}//friends_manager_display_following_and_followers


static void friends_manager_list_clicked(GtkTreeView *tree_view, FriendsManager *friends_manager){
	friends_manager_set_buttons_sensitivity(friends_manager, TRUE);
}//friends_manager_list_clicked


static void friends_manager_set_buttons_sensitivity(FriendsManager *friends_manager, gboolean is_sensitive){
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_follow), is_sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_unfollow), is_sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_block), is_sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->view_profile), is_sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->view_timeline), is_sensitive);
}//friends_manager_set_buttons_sensitivity


void friends_manager_show(GtkWindow *parent){
	if(!(friends_manager && friends_manager->dialog ))
		return friends_manager_setup(parent);
	
	gtk_window_present((GTK_WINDOW( friends_manager->dialog )) );
}//friends_manager_show


static void friends_manager_setup(GtkWindow *parent){
	GtkBuilder	*ui;
	GdkCursor	*cursor;

	debug(DEBUG_DOMAIN, "Initialising friends manager");

	if(friends_manager) friends_manager_free();
	
	friends_manager=g_new0(FriendsManager, 1);

	/* Get widgets */
	ui=gtkbuilder_get_file(GtkBuilderUI,
						"friends_manager", &friends_manager->dialog,
						"friends_and_followers", &friends_manager->friends_and_followers,
						"refresh", &friends_manager->refresh,
						"user_follow", &friends_manager->user_follow,
						"user_unfollow", &friends_manager->user_unfollow,
						"user_block", &friends_manager->user_block,
						"view_profile", &friends_manager->view_profile,
						"view_timeline", &friends_manager->view_timeline,
						NULL);
	
	friends_manager->friends_and_follows_model=gtk_tree_view_get_model (friends_manager->friends_and_followers);

	/* Connect the signals */
	gtkbuilder_connect(ui, friends_manager,
						"friends_manager", "destroy", friends_manager_destroy,
						"friends_manager", "destroy-event", friends_manager_destroy,
						"friends_manager", "close", friends_manager_response,
						"friends_manager", "response", friends_manager_response,
						"refresh", "clicked", friends_manager_refresh,
						"user_follow", "clicked", friends_manager_follow,
						"user_unfollow", "clicked", friends_manager_unfollow,
						"user_block", "clicked", friends_manager_block,
						"view_profile", "clicked", friends_manager_view_profile,
						"view_timeline", "clicked", friends_manager_view_timeline,
						"friends_and_followers", "row-activated", friends_manager_view_profile,
						"friends_and_followers", "cursor-changed", friends_manager_list_clicked,
						NULL
	);

	g_object_unref(ui);

	/* Set the parent */
	g_object_add_weak_pointer(G_OBJECT(friends_manager->dialog), (gpointer) &friends_manager);
	gtk_window_set_transient_for(GTK_WINDOW(friends_manager->dialog), parent);

	/* Now that we're done setting up, let's show the widget */
	gtk_window_present(friends_manager->dialog);
	
	app_set_statusbar_msg(_("Please wait while friends and followers are loaded.  This may take several minutes..."));
	cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(friends_manager->dialog)->window, cursor);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->dialog), FALSE);

	/* Load friends_manager */
	friends_manager_display_following_and_followers(FALSE);
	
	gdk_window_set_cursor(GTK_WIDGET(friends_manager->dialog)->window, NULL);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->dialog), TRUE);
	friends_manager_set_buttons_sensitivity(friends_manager, FALSE);
	app_set_statusbar_msg("");
}
