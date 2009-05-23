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
#include "tweet-view.h"

#define	DEBUG_DOMAINS	"Popup::UI:GtkBuilder:GtkBuildable:OnlineServices:Tweets:Requests:Users:Settings"
#include "debug.h"

#define GtkBuilderUI "popup-dialog.ui"

typedef struct {
	UserAction	action;
	GtkDialog	*dialog;
	GtkLabel	*used_for_label;
	GtkLabel	*username_label;
	GtkEntry	*entry;
	GtkComboBox	*online_services_combo_box;
	GtkListStore	*online_services_list_store;
	GtkTreeModel	*online_service_model;
	GtkButton	*cancel_button;
} Popup;

static void popup_set_title_and_label(UserAction action, Popup *popup);

static void popup_dialog_show(GtkWindow *parent, UserAction action);

static void popup_response_cb(GtkWidget *widget, gint response, Popup *popup);
static void popup_destroy_cb(GtkWidget *widget, Popup *popup);

static gboolean popup_validate_usage(UserAction action);

static void popup_set_selected_service(GtkComboBox *combo_box, Popup *popup);

static void popup_destroy_and_free(Popup *popup);


static void popup_set_title_and_label(UserAction action, Popup *popup){
	switch( action ){
		case SelectService:
			gtk_window_set_title(GTK_WINDOW(popup->dialog), "Which account do you want to use?");
			gtk_label_set_text(popup->used_for_label, "Please select the 'default' account you want to use for sending direct messages, managing friends, and etc.  You can select a different account at any time by selecting 'Select Default Account' from the 'Accounts' file menu:");
			break;
		case ViewTweets:
			gtk_window_set_title(GTK_WINDOW(popup->dialog), "Who's tweets do you want to see?" );
			gtk_label_set_text(popup->used_for_label, "Please enter the user name, or user id, who's resent updates you would like to view:");
			break;
		case ViewProfile:
			gtk_window_set_title(GTK_WINDOW(popup->dialog), "Who's profile do you want to see?" );
			gtk_label_set_text(popup->used_for_label, "Please enter the user name, or id, of whom you want to view:");
			break;
		case Block:
			gtk_window_set_title(GTK_WINDOW(popup->dialog), "Whom do you want to block?" );
			gtk_label_set_text(popup->used_for_label, "Please enter the user name, or id, of whom you want to block?  They'll no longer be able to read your tweets, send you messages, and you'll no longer be notified when they 'mention' you, using the @ symbol:");
			break;
		case UnBlock:
			gtk_window_set_title(GTK_WINDOW(popup->dialog), "Whom do you want to un-block?" );
			gtk_label_set_text(popup->used_for_label, "Please enter the user name, or id, of whom you want to un-block?  They'll be able to once again read your tweets and you'll see when they mention you, using the @ symbol, again:");
			break;
		case UnFollow:
			gtk_window_set_title(GTK_WINDOW(popup->dialog), "Whom you want to un-follow?" );
			gtk_label_set_text(popup->used_for_label, "Please enter the user name, or id, of whom you want to un-follow?");
			break;
		case Follow:
			gtk_window_set_title(GTK_WINDOW(popup->dialog), "Whom do you want to follow:" );
			gtk_label_set_text(popup->used_for_label, "Please enter the user name, or id, of whom you want to follow:");
			break;
		case Fave:
		case UnFave:
		default:
			break;
	}
}/*popup_set_title*/


static void popup_response_cb(GtkWidget *widget, gint response, Popup *popup){
	debug("Popup-Dialog for %s recieved response: %u", user_action_to_string(popup->action), response);
	switch(response){
		case GTK_RESPONSE_OK:
			break;
		case GTK_RESPONSE_NONE:
		case GTK_RESPONSE_REJECT:
		case GTK_RESPONSE_DELETE_EVENT:
		case GTK_RESPONSE_CANCEL:
		case GTK_RESPONSE_CLOSE:
		case GTK_RESPONSE_NO:
		default:
			gtk_widget_destroy(widget);
			return;
	}
	
	if(popup->action==SelectService){
		popup_set_selected_service(popup->online_services_combo_box, popup);
		gtk_widget_destroy(widget);
		return;
	}
	
	if( response != GTK_RESPONSE_OK )
		return gtk_widget_destroy(widget);
	
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	OnlineService		*service=NULL;
	const gchar		*username=gtk_entry_get_text(popup->entry);
	
	if(G_STR_EMPTY(username)){
		gtk_widget_error_bell(GTK_WIDGET(popup->entry));
		return;
	}
	
	if(!(gtk_combo_box_get_active_iter(popup->online_services_combo_box, iter))) {
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
		case ViewProfile:
		case Follow:
		case UnFollow:
		case Block:
		case UnBlock:
		case ViewTweets:
			user_request_main(service, popup->action, GTK_WINDOW(popup->dialog), username);
			break;
		case SelectService:
		case Fave:
		case UnFave:
			break;
	}//switch
	gtk_entry_set_text(GTK_ENTRY(popup->entry), "");
	g_free(iter);
}/*popup_response_cb*/

static void popup_set_selected_service(GtkComboBox *combo_box, Popup *popup){
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	
	if(gtk_combo_box_get_active_iter(popup->online_services_combo_box, iter)){
		gtk_tree_model_get(
				popup->online_service_model,
				iter,
				OnlineServicePointer, &selected_service,
				-1
		);
	}
	g_free(iter);
}/*popup_set_selected_service*/

static void popup_destroy_cb(GtkWidget *widget, Popup *popup){
	popup_destroy_and_free(popup);
}/*popup_destroy_cb*/

static void popup_destroy_and_free( Popup *popup ){
	gtk_widget_destroy( GTK_WIDGET(popup->dialog) );
	g_free( popup );
}/*popup_destroy_and_free*/

void popup_select_service( GtkWindow *parent ){
	popup_dialog_show( GTK_WINDOW(parent), SelectService );
}/*popup_select_service*/

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


static void popup_dialog_show(GtkWindow *parent, UserAction action ){
	GtkBuilder *ui;

	if(!(popup_validate_usage(action)))
		return;
	
	static Popup *popup=NULL;
	if(popup){
		if(popup->action==action)
			return gtk_window_present(GTK_WINDOW(popup->dialog));
		
		popup_destroy_and_free(popup);
	}
	
	popup=g_new0(Popup, 1);
	popup->action=action;
	
	/* Get widgets */
	ui=gtkbuilder_get_file(
					GtkBuilderUI,
						"entry_popup", &popup->dialog,
						"used_for_label", &popup->used_for_label,
						"entry", &popup->entry,
						"username_label", &popup->username_label,
						"online_services_combo_box", &popup->online_services_combo_box,
						"online_services_list_store", &popup->online_services_list_store,
						"cancel_button", &popup->cancel_button,
					NULL
	);
	popup->online_service_model=gtk_combo_box_get_model(GTK_COMBO_BOX(popup->online_services_combo_box));
	
	/* Connect the signals */
	gtkbuilder_connect(
			ui, popup,
				"entry_popup", "destroy", popup_destroy_cb,
				"entry_popup", "response", popup_response_cb,
			NULL
	);

	g_object_unref(ui);
	
	debug("Signal handlers set... loading accounts.");
	if(!( online_services_fill_liststore(online_services, popup->online_services_list_store) ))
		debug("No services found to load, new accounts need to be setup.");
	else{
		debug("OnlineServices found & loaded.  Selecting active service.");
		gtk_combo_box_set_active(popup->online_services_combo_box, 0);
	}

	if(popup->action==SelectService){
		debug("Connecting online_services_combo_box's \"GtkComboBox::changed\" to 'popup_set_selected_service' method.");
		g_signal_connect(popup->online_services_combo_box, "changed", (GCallback)popup_set_selected_service, popup);
	}
	
	popup_set_title_and_label(action, popup);
	
	/* Set the parent */
	g_object_add_weak_pointer(G_OBJECT(popup->dialog), (gpointer) &popup);
	gtk_window_set_transient_for(GTK_WINDOW(popup->dialog), parent);
	
	gtk_widget_show_all(GTK_WIDGET(popup->dialog));
	
	if(popup->action==SelectService){
		gtk_widget_hide( GTK_WIDGET(popup->username_label) );
		gtk_widget_hide( GTK_WIDGET(popup->entry) );
		gtk_window_set_modal(GTK_WINDOW(popup->dialog), TRUE);
		gtk_dialog_run(popup->dialog);
		return;
	}
	
	gtk_window_present(GTK_WINDOW(popup->dialog));
}/*popup_dialog_show*/

