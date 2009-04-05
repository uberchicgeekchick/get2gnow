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
#include "popup-dialog.h"
#include "network.h"

#define GtkBuilderUI "popup-dialog.ui"

typedef struct {
	GtkWidget	*dialog;
	GtkWidget	*entry;
	PopupUsage	usage;
} Popup;

static gchar *popup_set_title( PopupUsage usage );
static void popup_dialog_show( GtkWindow *parent, PopupUsage usage );

static void popup_response_cb( GtkWidget *widget, gint response, Popup *popup );
static void popup_destroy_cb( GtkWidget *widget, Popup *popup);

static void popup_destroy_and_free( Popup *popup );



static gchar *popup_set_title( PopupUsage usage ){
	switch( usage ){
		case PopupUnfollow:
			return g_strdup( "Unfollow a friend:" );
		case PopupBlock:
			return g_strdup( "Block a user from reading your tweets and sending you messages:" );
		case PopupFollow: default:
			return g_strdup( "Follow a new friend:" );
	}
}//popup_response_cb



static void popup_response_cb( GtkWidget *widget, gint response, Popup *popup){
	if( response != GTK_RESPONSE_OK )
		return gtk_widget_destroy(widget);
	
	switch( popup->usage ){
		case PopupFollow:
			return network_follow_user( gtk_entry_get_text(GTK_ENTRY(popup->entry)) );
		case PopupUnfollow:
			return network_unfollow_user( gtk_entry_get_text(GTK_ENTRY(popup->entry)) );
		case PopupBlock:
			return network_block_user( gtk_entry_get_text(GTK_ENTRY(popup->entry)) );
	}//switch
	
	gtk_widget_destroy(widget);
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
	popup_dialog_show( GTK_WINDOW(parent), PopupFollow );
}

void popup_friend_unfollow( GtkWindow *parent ){
	popup_dialog_show( GTK_WINDOW(parent), PopupUnfollow );
}

void popup_friend_block( GtkWindow *parent ){
	popup_dialog_show( GTK_WINDOW(parent), PopupBlock );
}


static void popup_dialog_show(GtkWindow *parent, PopupUsage usage ){
	static Popup *popup;
	GtkBuilder *ui;

	if( popup ){
		if( popup->usage==usage )
			return gtk_window_present( GTK_WINDOW(popup->dialog) );
		popup_destroy_and_free(popup);
	}

	popup = g_new0 (Popup, 1);

	/* Get widgets */
	ui = gtkbuilder_get_file (GtkBuilderUI,
						"popup_friend_dialog", &popup->dialog,
						"frienduser_entry", &popup->entry,
						NULL);

	/* Connect the signals */
	gtkbuilder_connect (ui, popup,
						"popup_friend_dialog", "destroy", popup_destroy_cb,
						"popup_friend_dialog", "response", popup_response_cb,
						NULL);

	g_object_unref (ui);
	
	gchar *title=popup_set_title( usage );
	gtk_window_set_title( GTK_WINDOW(popup->dialog), title );
	g_free( title );
	
	/* Set the parent */
	g_object_add_weak_pointer (G_OBJECT (popup->dialog), (gpointer) &popup);
	gtk_window_set_transient_for (GTK_WINDOW (popup->dialog), parent);

	/* Now that we're done setting up, let's show the widget */
	gtk_widget_show (popup->dialog);
}
