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

#define GtkBuilderUI "popup-dialog.ui"

typedef struct {
	GtkWidget	*dialog;
	GtkWidget	*entry;
	FriendAction	action;
} Popup;

static gchar *popup_set_title( FriendAction action );
static void popup_dialog_show( GtkWindow *parent, FriendAction action );

static void popup_response_cb( GtkWidget *widget, gint response, Popup *popup );
static void popup_destroy_cb( GtkWidget *widget, Popup *popup);

static gboolean popup_validate_and_set_usage(Popup *popup, FriendAction action);

static void popup_destroy_and_free( Popup *popup );



static gchar *popup_set_title( FriendAction action ){
	switch( action ){
		case Fave:
		case UnFave:
			break;
		case ViewTweets:
			return g_strdup( "View someone's resent tweets:" );
		case ViewProfile:
			return g_strdup( "View someone's profile:" );
		case UnFollow:
			return g_strdup( "Unfollow:" );
		case Block:
			return g_strdup( "Unblock this user, they'll be ablt to read your tweets and send you messages, again:" );
		case UnBlock:
			return g_strdup( "Who do you want to block:" );
		case Follow:
			return g_strdup( "Who do you want to follow:" );
	}
	return NULL;
}//popup_set_title



static void popup_response_cb( GtkWidget *widget, gint response, Popup *popup){
	if( response != GTK_RESPONSE_OK )
		return gtk_widget_hide(widget);
	const gchar *username=gtk_entry_get_text(GTK_ENTRY(popup->entry));
	
	switch( popup->action ){
		case Follow:
		case UnFollow:
		case Block:
		case UnBlock:
		case ViewTweets:
		case ViewProfile:
			user_request_main(popup->action, username);
			break;
		case Fave:
		case UnFave:
			break;
	}//switch
	gtk_widget_hide(widget);
}//popup_response_cb

static void popup_destroy_cb(GtkWidget *widget, Popup *popup){
	popup_destroy_and_free( popup );
}//popup_destroy_cb

static void popup_destroy_and_free( Popup *popup ){
	gtk_widget_destroy( popup->dialog );
	gtk_widget_destroy( popup->entry );
	g_free( popup );
}//popup_destroy_and_free

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
}//popup_friend_unblock

void popup_friend_tweets( GtkWindow *parent ){
	popup_dialog_show( GTK_WINDOW(parent), ViewTweets );
}

void popup_friend_profile( GtkWindow *parent ){
	popup_dialog_show( GTK_WINDOW(parent), ViewProfile );
}


static gboolean popup_validate_and_set_usage(Popup *popup, FriendAction action){
	switch( action ){
		case ViewProfile:
		case ViewTweets:
		case Follow:
		case UnFollow:
		case Block:
		case UnBlock:
			popup->action=action;
			return TRUE;
		case Fave:
		case UnFave:
			break;
	}//switch
	app_statusbar_printf("This cannot be done using %s's popup prompt.", PACKAGE_NAME);
	return FALSE;
}//popup_validate_and_set_usage


static void popup_dialog_show(GtkWindow *parent, FriendAction action ){
	static Popup *popup;
	GtkBuilder *ui;

	if( popup ){
		if( popup->action==action )
			return gtk_window_present( GTK_WINDOW(popup->dialog) );
		popup_destroy_and_free(popup);
	}

	popup = g_new0(Popup, 1);
	
	if(!(popup_validate_and_set_usage(popup, action)))
		return;

	/* Get widgets */
	ui = gtkbuilder_get_file (GtkBuilderUI,
						"entry_popup", &popup->dialog,
						"entry", &popup->entry,
						NULL);

	/* Connect the signals */
	gtkbuilder_connect (ui, popup,
						"entry_popup", "destroy", popup_destroy_cb,
						"entry_popup", "response", popup_response_cb,
						NULL);

	g_object_unref (ui);
	
	gchar *title=popup_set_title( action );
	gtk_window_set_title( GTK_WINDOW(popup->dialog), title );
	g_free( title );
	
	/* Set the parent */
	g_object_add_weak_pointer (G_OBJECT (popup->dialog), (gpointer) &popup);
	gtk_window_set_transient_for (GTK_WINDOW (popup->dialog), parent);

	/* Now that we're done setting up, let's show the widget */
	gtk_widget_show (popup->dialog);
}

