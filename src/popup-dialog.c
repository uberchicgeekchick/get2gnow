/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2007-2008 Daniel Morales <daniminas@gmail.com>
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
 */

#include "config.h"

#include "gtkbuilder.h"

#include "main.h"
#include "app.h"
#include "popup-dialog.h"
#include "network.h"
#include "users.h"
#include "profile-viewer.h"

#define	DEBUG_DOMAINS	"Popup::UI:GtkBuilder:GtkBuildable:OnlineServices:Tweets:Requests:Users:Settings"
#include "debug.h"

#define GtkBuilderUI "popup-dialog.ui"

typedef struct {
	GtkWindow	*dialog;
	GtkLabel	*username_label;
	GtkEntry	*entry;
	GtkComboBox	*online_service_combobox;
	GtkListStore	*online_service_liststore;
	GtkTreeModel	*online_service_model;
	UserAction	action;
} Popup;

static void popup_set_title(UserAction action, Popup *popup);
static Popup *popup_dialog_show(GtkWindow *parent, UserAction action);
static void popup_response_cb(GtkWidget *widget, gint response, Popup *popup);
static void popup_destroy_cb(GtkWidget *widget, Popup *popup);
static gboolean popup_validate_usage(UserAction action);
static void popup_destroy_and_free(Popup *popup);


static void popup_set_title(UserAction action, Popup *popup){
	switch( action ){
		case SelectService:
			gtk_window_set_title( GTK_WINDOW(popup->dialog), "Please select which account you want to use:");
			break;
		case ViewTweets:
			gtk_window_set_title( GTK_WINDOW(popup->dialog), "Please enter the user name, or user id, who's resent updates you would like to view:" );
			break;
		case ViewProfile:
			gtk_window_set_title( GTK_WINDOW(popup->dialog), "Please enter the user name, or id, for whom you want to view?" );
			break;
		case Block:
			gtk_window_set_title( GTK_WINDOW(popup->dialog), "Please enter the user name, or id, for whom you want to block?  They'll no longer be able to read your tweets, send you messages, and you'll no longer be notified when they 'mention' you, using the @ symbol:" );
			break;
		case UnBlock:
			gtk_window_set_title( GTK_WINDOW(popup->dialog), "Please enter the user name, or id, for whom you want to un-block?  They'll be able to once again read your tweets and you'll see when they mention you, using the @ symbol, again:" );
			break;
		case UnFollow:
			gtk_window_set_title( GTK_WINDOW(popup->dialog), "Please enter the user name, or id, for whom you want to un-follow?" );
			break;
		case Follow:
			gtk_window_set_title( GTK_WINDOW(popup->dialog), "Please enter the user name, or id, for whom you want to follow:" );
			break;
		case Fave:
		case UnFave:
		default:
			break;
	}
}/*popup_set_title*/



static void popup_response_cb( GtkWidget *widget, gint response, Popup *popup){
	if( response != GTK_RESPONSE_OK )
		return gtk_widget_destroy(widget);
	
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	OnlineService		*service=NULL;
	const gchar		*username=gtk_entry_get_text(popup->entry);
	
	if(G_STR_EMPTY(username)){
		gtk_widget_error_bell(GTK_WIDGET(popup->entry));
		return;
	}
	
	if(!(gtk_combo_box_get_active_iter(popup->online_service_combobox, iter))) {
		if(iter) g_free(iter);
		return;
	}
	
	gtk_tree_model_get(
				popup->online_service_model,
				iter,
				OnlineServicePointer, &service,
				-1
	);
	
	switch( popup->action ){
		case SelectService:
			current_service=service;
			break;
		case ViewProfile:
		case Follow:
		case UnFollow:
		case Block:
		case UnBlock:
		case ViewTweets:
			user_request_main(service, popup->action, popup->dialog, username);
			break;
		case Fave:
		case UnFave:
			break;
	}//switch
	gtk_entry_set_text(GTK_ENTRY(popup->entry), "");
	g_free(iter);
}/*popup_response_cb*/

static void popup_destroy_cb(GtkWidget *widget, Popup *popup){
	popup_destroy_and_free( popup );
}/*popup_destroy_cb*/

static void popup_destroy_and_free( Popup *popup ){
	gtk_widget_destroy( GTK_WIDGET(popup->dialog) );
	g_free( popup );
}/*popup_destroy_and_free*/

void popup_select_service( GtkWindow *parent ){
	Popup *popup=popup_dialog_show( GTK_WINDOW(parent), SelectService );
	gtk_widget_hide( GTK_WIDGET(popup->username_label) );
	gtk_widget_hide( GTK_WIDGET(popup->entry) );
}

void popup_friend_follow( GtkWindow *parent ){
	popup_dialog_show( GTK_WINDOW(parent), Follow );
}

void popup_friend_unfollow( GtkWindow *parent ){
	popup_dialog_show( GTK_WINDOW(parent), UnFollow );
}

void popup_friend_block( GtkWindow *parent ){
	popup_dialog_show( GTK_WINDOW(parent), Block );
}

void popup_friend_unblock( GtkWindow *parent ){
	popup_dialog_show( GTK_WINDOW(parent), UnBlock );
}/*popup_friend_unblock*/

void popup_friend_tweets( GtkWindow *parent ){
	popup_dialog_show( GTK_WINDOW(parent), ViewTweets );
}

void popup_friend_profile( GtkWindow *parent ){
	popup_dialog_show( GTK_WINDOW(parent), ViewProfile );
}


static gboolean popup_validate_usage(UserAction action){
	switch( action ){
		case SelectService:
		case ViewProfile:
		case ViewTweets:
		case Follow:
		case UnFollow:
		case Block:
		case UnBlock:
			return TRUE;
		case Fave:
		case UnFave:
		default:
			break;
	}//switch
	app_statusbar_printf("%s is not supported by %s's popup prompt.", user_action_to_string(action), PACKAGE_NAME);
	return FALSE;
}/*popup_validate_usage*/


static Popup *popup_dialog_show(GtkWindow *parent, UserAction action ){
	static Popup *popup;
	GtkBuilder *ui;

	if( popup ){
		if( popup->action==action ){
			gtk_window_present( GTK_WINDOW(popup->dialog) );
			return popup;
		}
		popup_destroy_and_free(popup);
	}
	
	if(!(popup_validate_usage(action)))
		return NULL;
	
	popup=g_new0(Popup, 1);
	popup->action=action;
	
	/* Get widgets */
	ui=gtkbuilder_get_file(
					GtkBuilderUI,
						"entry_popup", &popup->dialog,
						"entry", &popup->entry,
						"username_label", &popup->username_label,
						"online_service_combobox", &popup->online_service_combobox,
						"online_service_liststore", &popup->online_service_liststore,
					NULL
	);
	popup->online_service_model=gtk_combo_box_get_model(GTK_COMBO_BOX(popup->online_service_combobox));

	/* Connect the signals */
	gtkbuilder_connect(
					ui, popup,
						"entry_popup", "destroy", popup_destroy_cb,
						"entry_popup", "response", popup_response_cb,
					NULL
	);

	g_object_unref(ui);
	
	debug("Signal handlers set... loading accounts.");
	if(!( online_services_fill_liststore(online_services, popup->online_service_liststore) ))
		debug("No services found to load, new accounts need to be setup.");
	else{
		debug("OnlineServices found & loaded.  Selecting active service.");
		gtk_combo_box_set_active(popup->online_service_combobox, 0);
	}

	popup_set_title(action, popup);
	
	/* Set the parent */
	g_object_add_weak_pointer(G_OBJECT(popup->dialog), (gpointer) &popup);
	gtk_window_set_transient_for(GTK_WINDOW(popup->dialog), parent);

	/* Now that we're done setting up, let's show the widget */
	gtk_window_present(popup->dialog);
	return popup;
}

