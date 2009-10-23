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


#include "gtkbuilder.h"

#include "program.h"

#include "main-window.h"

#include "online-service.types.h"
#include "online-service.h"
#include "online-service-request.h"
#include "users.types.h"
#include "users-glists.h"
#include "users.h"

#include "friends-manager.h"
#include "network.h"
#include "images.h"


enum {
	USER_NAME,
	USER_NICK,
	FOLLOWING,
	FOLLOWER,
	USER_POINTER,
};

typedef struct {
	GtkDialog	*dialog;
	GtkTreeView	*friends_and_followers;
	GtkListStore	*friends_manager_liststore;
	GtkTreeModel	*friends_and_followers_model;
	GtkButton	*refresh;
	GtkButton	*user_follow;
	GtkButton	*user_unfollow;
	GtkButton	*user_block;
	GtkButton	*view_profile;
	GtkButton	*view_timeline;
	
	GtkButton	*close_button;
} FriendsManager;

static FriendsManager *friends_manager=NULL;

#define	DEBUG_DOMAINS	"FriendsManager:UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Requests:Updates:Users:Setup:friends-manager.c"
#include "debug.h"

#define GtkBuilderUI "friends-manager"

/* GtkBuilder methods.
 * 'friends_manager_show' is this object's public method & prototyped in 'friends-manager.h' */
static void friends_manager_setup( GtkWindow *parent );

/* UI event handlers */
static User *friends_manager_get_selected_user(GtkTreeIter **iter, FriendsManager *friends_manager);
static void friends_manager_refresh(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_follow(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_unfollow(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_block(GtkButton *button, FriendsManager *friends_manager);
static void friends_manager_set_buttons_sensitivity(FriendsManager *friends_manager, gboolean is_sensitive);

/* signal handlers */
static void friends_manager_destroy(GtkDialog *dialog, FriendsManager *friends_manager);
static void friends_manager_response(GtkDialog *dialog, gint response, FriendsManager *friends_manager);
static void friends_manager_list_clicked(GtkTreeView *tree_view, FriendsManager *friends_manager);
static void friends_manager_view_profile(void);
static void friends_manager_view_timeline(GtkButton *button, FriendsManager *friends_manager);



static void friends_manager_response(GtkDialog *dialog, gint response, FriendsManager *friends_manager){
	gtk_widget_destroy(GTK_WIDGET(dialog));
}/*friends_message_response*/

static void friends_manager_destroy(GtkDialog *dialog, FriendsManager *friends_manager){
	debug("Destroying friends manager");
	gtk_widget_destroy(GTK_WIDGET(dialog));
	uber_free(friends_manager);
}/*friends_manager_destroy*/

static User *friends_manager_get_selected_user(GtkTreeIter **iter, FriendsManager *friends_manager){
	User *user=NULL;
	*iter=g_new0(GtkTreeIter, 1);
	GtkTreeSelection *sel=gtk_tree_view_get_selection(friends_manager->friends_and_followers);
	if(!gtk_tree_selection_get_selected(sel, NULL, *iter)){
		uber_free(iter);
		return NULL;
	}
	
	gtk_tree_model_get(
				friends_manager->friends_and_followers_model, *iter,
					USER_POINTER, &user,
				-1
	);
	
	return (user ?user :NULL);
}/*following_viewer_get_selected_user(following_viewer);*/

static void friends_manager_follow(GtkButton   *button, FriendsManager *friends_manager){
	GtkTreeIter *iter=NULL;
	User *user=friends_manager_get_selected_user(&iter, friends_manager);
	
	gchar *following=NULL;
	gtk_tree_model_get(
				friends_manager->friends_and_followers_model,
				iter,
					FOLLOWING, &following,
				-1
	);
	
	if(g_str_equal(following, "No")){
		gtk_list_store_set(
					friends_manager->friends_manager_liststore,
					iter,
					FOLLOWING, "Yes",
					-1
		);
		online_service_request_follow(user->service, GTK_WINDOW(friends_manager->dialog), user->user_name);
	}
	
	uber_free(following);
	uber_free(iter);
}

static void friends_manager_unfollow(GtkButton   *button, FriendsManager *friends_manager){
	GtkTreeIter *iter=NULL;
	User *user=friends_manager_get_selected_user(&iter, friends_manager);
	
	gchar *follower=NULL;
	gtk_tree_model_get(
				friends_manager->friends_and_followers_model,
				iter,
					FOLLOWER, &follower,
				-1
	);
	
	
	if(g_str_equal(follower, "Yes")){
		online_service_request_unfollow(user->service, GTK_WINDOW(friends_manager->dialog), user->user_name);
		gtk_list_store_set(
				friends_manager->friends_manager_liststore,
				iter,
					FOLLOWING, "No",
				-1
		);
	} else {
		gtk_list_store_remove(friends_manager->friends_manager_liststore, iter);
		friends_manager_set_buttons_sensitivity(friends_manager, FALSE);
	}
	
	uber_free(follower);
	uber_free(iter);
}/*friends_manager_unfolow( button, friends_manager );*/

static void friends_manager_block(GtkButton   *button, FriendsManager *friends_manager){
	GtkTreeIter *iter=NULL;
	User *user=friends_manager_get_selected_user(&iter, friends_manager);
	
	online_service_request_block(user->service, GTK_WINDOW(friends_manager->dialog), user->user_name);
	gtk_list_store_remove(friends_manager->friends_manager_liststore, iter);
	friends_manager_set_buttons_sensitivity(friends_manager, FALSE);

	uber_free(iter);
}/*friends_manager_block( button, friends_manager );*/

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
	GtkTreeIter *iter=NULL;
	User *user=friends_manager_get_selected_user(&iter, friends_manager);
	if(!user) return;
	
	online_service_request_view_profile(user->service, GTK_WINDOW(friends_manager->dialog), user->user_name);
	uber_free(iter);
}/*friends_manager_view_profile*/

static void friends_manager_view_timeline(GtkButton *button, FriendsManager *friends_manager){
	GtkTreeIter *iter=NULL;
	User *user=friends_manager_get_selected_user(&iter, friends_manager);
	if(!user) return;
	
	online_service_request_view_updates(user->service, GTK_WINDOW(friends_manager->dialog), user->user_name);
	uber_free(iter);
}/*friends_manager_view_timeline*/

static void friends_manager_refresh(GtkButton *button, FriendsManager *friends_manager){
	GdkCursor *cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(friends_manager->dialog)->window, cursor);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->dialog), FALSE);
	
	online_service_request_popup_select_service();
	users_glist_get(GetBoth, TRUE, friends_manager_display_following_and_followers);
}/*friends_manager_refresh*/

void friends_manager_display_following_and_followers(GList *friends_and_followers){
	User		*user1=NULL, *user2=NULL;
	GList		*list=NULL;
	
	gdk_window_set_cursor(GTK_WIDGET(friends_manager->dialog)->window, NULL);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->dialog), TRUE);
	friends_manager_set_buttons_sensitivity(friends_manager, FALSE);
	
	gboolean following=FALSE, follower=FALSE;
	gtk_list_store_clear(friends_manager->friends_manager_liststore);
	for( list=friends_and_followers; list; list=list->next ){
		user1=(User *)list->data;
		if(!list->next)
			following=!(follower=user1->follower);
		else{
			list=list->next;
			user2=(User *)list->data;
			if(g_str_equal( user1->user_name, user2->user_name ))
				follower=following=TRUE;
			else{
				list=list->prev;
				following=!(follower=user1->follower);
			}
		}
		
		GtkTreeIter *iter=g_new(GtkTreeIter, 1);
		gtk_list_store_append(friends_manager->friends_manager_liststore, iter );
		gtk_list_store_set(
					friends_manager->friends_manager_liststore,
					iter,
						USER_NAME, user1->user_name,
						USER_NICK, user1->nick_name,
						FOLLOWING, (following?"Yes":"No"),
						FOLLOWER, (follower?"Yes":"No"),
						USER_POINTER, user1,
					-1
		);
		g_free(iter);
	}
}/*friends_manager_display_following_and_followers*/


static void friends_manager_list_clicked(GtkTreeView *tree_view, FriendsManager *friends_manager){
	friends_manager_set_buttons_sensitivity(friends_manager, TRUE);
}/*friends_manager_list_clicked*/


static void friends_manager_set_buttons_sensitivity(FriendsManager *friends_manager, gboolean is_sensitive){
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_follow), is_sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_unfollow), is_sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->user_block), is_sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->view_profile), is_sensitive);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->view_timeline), is_sensitive);
}/*friends_manager_set_buttons_sensitivity*/


void friends_manager_show(GtkWindow *parent){
	online_service_request_popup_select_service();
	if(!(selected_service)) return;
	
	if(!(friends_manager && friends_manager->dialog ))
		return friends_manager_setup(parent);
	
	gtk_window_present(GTK_WINDOW(friends_manager->dialog));
}/*friends_manager_show*/


static void friends_manager_setup(GtkWindow *parent){
	GtkBuilder	*ui;

	debug("Initialising friends manager");

	friends_manager=g_new0(FriendsManager, 1);

	/* Get widgets */
	debug("Building & loading friends manager from: %s", GtkBuilderUI);
	ui=gtkbuilder_get_file(
				GtkBuilderUI,
					"friends_manager", &friends_manager->dialog,
					"friends_and_followers", &friends_manager->friends_and_followers,
					"friends_manager_liststore", &friends_manager->friends_manager_liststore,
					"refresh", &friends_manager->refresh,
					"user_follow", &friends_manager->user_follow,
					"user_unfollow", &friends_manager->user_unfollow,
					"user_block", &friends_manager->user_block,
					"view_profile", &friends_manager->view_profile,
					"view_timeline", &friends_manager->view_timeline,
					
					"lists_close", &friends_manager->close_button,
				NULL
	);
	friends_manager->friends_and_followers_model=gtk_tree_view_get_model(friends_manager->friends_and_followers);
	
	gchar *window_title=g_strdup_printf("%s - <%s>'s %s", _(GETTEXT_PACKAGE), selected_service->key, _("Friends Manager"));
	gtk_window_set_title(GTK_WINDOW(friends_manager->dialog), window_title);
	uber_free(window_title);
	
	debug("FriendsManager created now connecting its signal handlers.");
	gtkbuilder_connect(
				ui, friends_manager,
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
	
	/*TODO: FIXME:*/
	gtk_widget_hide(GTK_WIDGET(friends_manager->refresh));
	g_object_add_weak_pointer(G_OBJECT(friends_manager->dialog), (gpointer) &friends_manager);
	gtk_window_set_transient_for(GTK_WINDOW(friends_manager->dialog), parent);

	/* Now that we're done setting up, let's show the widget */
	gtk_window_present(GTK_WINDOW(friends_manager->dialog));
	
	main_window_set_statusbar_msg(_("Please wait while friends and followers are loaded.  This may take several minutes..."));
	debug("FriendsManager setup & loaded.  Please wait while friends and followers are loaded.");
	GdkCursor *cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(friends_manager->dialog)->window, cursor);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->dialog), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(friends_manager->close_button), TRUE);
	
	/* Load friends_manager */
	if(selected_service && selected_service->friends_and_followers)
		users_glists_free(selected_service, GetBoth);
	users_glist_get(GetBoth, FALSE, friends_manager_display_following_and_followers);
}/*friends_manager_setup*/
