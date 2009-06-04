/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright(C) 2007-2008 Brian Pepple <bpepple@fedoraproject.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or(at your option) any later version.
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

#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>

#include "gconfig.h"
#include "gtkbuilder.h"

#include "main.h"
#include "services-dialog.h"
#include "network.h"
#include "online-services.h"
#include "preferences.h"
#include "confirm.h"


#define GtkBuilderUI "services-dialog.ui"

#define DEBUG_DOMAINS "OnlineServices:UI:GtkBuilder:GtkBuildable:Requests:Authentication:Preferences:Settings:Setup:ServicesDialog"
#include "debug.h"

#define PREFS_ACCOUNT_DELETE	PREFS_PATH "/auth/dont_confirm_delete"

typedef struct {	
	GtkDialog		*dialog;
	GtkCheckButton		*enabled;
	
	GtkComboBoxEntry	*urls;
	GtkListStore		*urls_liststore;
	GtkTreeModel		*urls_model;
	
	GtkButton		*service_new_button;
	GtkButton		*service_delete_button;
	
	GtkToggleButton		*secure_service_toggle_button;
	
	/*currently unused
	GtkComboBox		*service_type_combobox;
	GtkListStore		*service_type_liststore;
	GtkTreeModel		*service_type_model;
	*/
	
	GtkEntry		*username;
	GtkEntry		*password;
	GtkCheckButton		*show_password;
	GtkCheckButton		*auto_connect;
	
	GtkButton		*online_service_connect_button;
	
	/*Buttons in services-dialog 'action-area.*/
	GtkButton		*okay_button;
	GtkButton		*save_button;
} ServicesDialog;

static ServicesDialog	*services_dialog=NULL;
static gboolean		okay_to_exit=TRUE;

static void services_dialog_response_cb(GtkDialog *dialog, gint response, ServicesDialog *services);
static void services_dialog_destroy_cb(GtkDialog *dialog);
static void services_dialog_show_password_cb(GtkCheckButton *show_password, ServicesDialog *services_dialog);
static void services_dialog_load_service(GtkComboBoxEntry *urls, ServicesDialog *services_dialog);

static OnlineService *services_dialog_get_active_service(ServicesDialog *services_dialog);
static void services_dialog_new_service(GtkButton *new_button, ServicesDialog *services_dialog);
static void services_dialog_save_service(GtkButton *save_button, ServicesDialog *services_dialog);
static void services_dialog_delete_service(GtkButton *service_delete_button, ServicesDialog *services_dialog);

static void services_dialog_check_cb(GtkEntry *entry, GdkEventKey *event, ServicesDialog *services_dialog);
static void services_dialog_check_service(ServicesDialog *services_dialog);

static void services_dialog_setup(GtkWindow *parent);

static void services_dialog_response_cb(GtkDialog *dialog, gint response, ServicesDialog *services){
	switch(response){
		case GTK_RESPONSE_CANCEL:
		case GTK_RESPONSE_REJECT:
		case GTK_RESPONSE_DELETE_EVENT:
		case GTK_RESPONSE_CLOSE:
		case GTK_RESPONSE_NO:
			gtk_widget_destroy(GTK_WIDGET(dialog));
			services_dialog=NULL;
			return;
	}
	
	services_dialog_save_service(services_dialog->save_button, services_dialog);
	
	if(response==GTK_RESPONSE_OK && okay_to_exit ){
		gtk_widget_destroy(GTK_WIDGET(dialog));
		services_dialog=NULL;
	}
}/*services_dialog_response_cb*/

static void services_dialog_new_service(GtkButton *new_button, ServicesDialog *services_dialog){
	if(!(services_dialog_get_active_service(services_dialog))) return;
	
	debug("Preparing ServicesDialog for a new account setup.");
	gtk_entry_set_text( GTK_ENTRY( GTK_BIN( services_dialog->urls)->child), "");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(services_dialog->enabled), TRUE);
	gtk_toggle_button_set_active(services_dialog->secure_service_toggle_button, TRUE);
	gtk_entry_set_text(GTK_ENTRY(services_dialog->username), "");
	gtk_entry_set_text(GTK_ENTRY(services_dialog->password), "");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(services_dialog->auto_connect), TRUE);
}/*services_dialog_new_service(services_dialog->service_new_button, services_dialog);*/

static void services_dialog_save_service(GtkButton *save_button, ServicesDialog *services_dialog){
	debug("Saving services.");
	okay_to_exit=FALSE;
	
	gchar *uri=gtk_combo_box_get_active_text(GTK_COMBO_BOX(services_dialog->urls));
	const gchar *username=gtk_entry_get_text(GTK_ENTRY(services_dialog->username));
	const gchar *password=gtk_entry_get_text(GTK_ENTRY(services_dialog->password));
	if(!( G_STR_N_EMPTY(uri) && G_STR_N_EMPTY(username) && G_STR_N_EMPTY(password) )){
		debug("Failed to save current account.  Server, username, and/or password missing.");
		okay_to_exit=TRUE;
		g_free(uri);
		return;
	}
	
	OnlineService		*service=NULL;
	gboolean		new_service=FALSE;
	
	debug("Retriving current service.");
	if(!( (service=services_dialog_get_active_service(services_dialog)) )){
		debug("Current service does not have a valid GtkTreeIter, service will be appended later.");
		new_service=TRUE;
	}
	
	
	if(!( (service=online_services_save_service(
				online_services,
				service,
				gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(services_dialog->enabled)),
				(const gchar *)uri,
				gtk_toggle_button_get_active(services_dialog->secure_service_toggle_button),
				username,
				password,
				gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(services_dialog->auto_connect))
	)) )){
		debug("**ERROR:** Failed to save online service for '%s@%s' please see above for further details.", username, uri);
		g_free(uri);
		return;
	}
	
	if(!new_service){
		okay_to_exit=TRUE;
		g_free(uri);
		return;
	}
	
	if(!( service && service->key && service->server && service->username )){
		debug("**ERROR:** OnlineServices saved resulting OnlineService is invalid.");
		g_free(uri);
		return;
	}
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	debug("Storing new service: '%s'.", service->key);
	gtk_list_store_append(services_dialog->urls_liststore, iter);
	gtk_list_store_set(
				services_dialog->urls_liststore, iter,
					UrlString, uri,
					OnlineServicePointer, service,
				-1
	);
	
	g_free(uri);
	
	debug("New service stored, selecting it new loction in the dialog.");
	okay_to_exit=TRUE;
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(services_dialog->urls), iter);
	uber_free(iter);
}/*services_dialog_save_service(services_dialog->service_new_button, services_dialog);*/

static void services_dialog_connect_service(GtkButton *online_service_connect_button, ServicesDialog *services_dialog){
	OnlineService *service=NULL;
	
	gboolean enabled=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(services_dialog->enabled));
	gboolean https=gtk_toggle_button_get_active(services_dialog->secure_service_toggle_button);
	
	gchar *uri=gtk_combo_box_get_active_text(GTK_COMBO_BOX(services_dialog->urls));
	const gchar *username=gtk_entry_get_text(GTK_ENTRY(services_dialog->username));
	const gchar *password=gtk_entry_get_text(GTK_ENTRY(services_dialog->password));
	
	gboolean auto_connect=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(services_dialog->auto_connect));
	
	if(!(service=services_dialog_get_active_service(services_dialog))){
		debug("OnlineService is not saved.  Creating temporary OnlineService to connect with.");
		service=online_service_new(enabled, uri, https, username, password, auto_connect);
		online_service_connect(service);
		online_service_login(service, TRUE);
	}else if( service && G_STR_N_EMPTY(uri) && g_str_equal(service->uri, uri) && G_STR_N_EMPTY(username) && g_str_equal(service->username, username) && G_STR_N_EMPTY(password) && g_str_equal(service->password, password) ){
		if(service->connected){
			debug("OnlineService: %s is already connected.", service->decoded_key);
			return;
		}
		online_service_connect(service);
		online_service_login(service, TRUE);
	}
}/*services_dialog_connect_service(services_dialog->online_service_connect_button, services_dialog);*/

static void services_dialog_delete_service(GtkButton *delete_button, ServicesDialog *services_dialog){
	OnlineService		*service=NULL;
	
	debug("Retriving current service.");
	if(!( (service=services_dialog_get_active_service(services_dialog)) )){
		debug("There is no valid account selected.");
		return;
	}
	
	gchar *confirm_message=g_strdup_printf("%s: %s?", _("Are you sure you want to delete"), service->decoded_key);
	if(!(confirm_dialog_show(
					PREFS_ACCOUNT_DELETE,
					confirm_message,
					_("You will no longer be able to send or recieve messages using this account.\n\nIf you want to use this service again you will have to set it up again."),
					GTK_WINDOW(services_dialog->dialog),
					NULL, NULL
	))){
		uber_free(confirm_message);
		debug("Account deletion was canceled by the user.");
		return;
	}

	uber_free(confirm_message);
	
	debug("Deleting service: '%s'.", service->key);
	online_services_delete_service(online_services, service);
	
	debug("Reloading OnlineServices into ServicesDialog 'urls_list_store.");
	online_services_combo_box_fill(online_services, GTK_COMBO_BOX(services_dialog->urls), services_dialog->urls_liststore, FALSE);
	services_dialog_new_service(services_dialog->service_new_button, services_dialog);
}/*services_dialog_delete_service(services_dialog->service_delete_button, services_dialog);*/

static void services_dialog_destroy_cb(GtkDialog *dialog){
	uber_free(services_dialog);
}/*services_dialog_destroy_cb*/

static void services_dialog_show_password_cb(GtkCheckButton *show_password, ServicesDialog *services_dialog){
	gboolean visible;

	visible=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(services_dialog->show_password));
	gtk_entry_set_visibility(GTK_ENTRY(services_dialog->password), visible);
}/*services_dialog_show_password_cb*/

static OnlineService *services_dialog_get_active_service(ServicesDialog *services_dialog){
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	OnlineService		*service=NULL;
	
	if(!( (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(services_dialog->urls), iter)) )){
		return NULL;
	}
	
	gtk_tree_model_get(
				services_dialog->urls_model, iter,
					OnlineServicePointer, &service,
				-1
	);
	if(!(service && service->key && service->uri && service->username))
		return NULL;
	
	return service;
}/*services_dialog_get_active_service*/

static void services_dialog_check_cb(GtkEntry *entry, GdkEventKey *event, ServicesDialog *services_dialog){
	services_dialog_check_service(services_dialog);
}/*services_dialog_check_cb*/

static void services_dialog_check_service(ServicesDialog *services_dialog){
	gboolean		service_okay_to_save=FALSE, service_exists=FALSE, service_is_saved=FALSE, service_is_connectable=FALSE;
	OnlineService		*service=services_dialog_get_active_service(services_dialog);
	
	gchar *uri=gtk_combo_box_get_active_text(GTK_COMBO_BOX(services_dialog->urls));
	const gchar *username=gtk_entry_get_text(GTK_ENTRY(services_dialog->username));
	const gchar *password=gtk_entry_get_text(GTK_ENTRY(services_dialog->password));
	
	if(service){
		service_exists=TRUE;
	}
	
	if( service && G_STR_N_EMPTY(uri) && g_str_equal(service->uri, uri) && G_STR_N_EMPTY(username) && g_str_equal(service->username, username) && G_STR_N_EMPTY(password) && g_str_equal(service->password, password) ){
		service_is_saved=TRUE;
		service_okay_to_save=FALSE;
	}else if( service && G_STR_N_EMPTY(uri) && !g_str_equal(service->uri, uri) && G_STR_N_EMPTY(username) && !g_str_equal(service->username, username) && !G_STR_N_EMPTY(password) && !g_str_equal(service->password, password) ){
		service_okay_to_save=TRUE;
	}else if(!( G_STR_N_EMPTY(uri) && G_STR_N_EMPTY(username) && G_STR_N_EMPTY(password) )){
		service_okay_to_save=TRUE;
	}
	
	debug("%s 'service_delete_button'.", (service_exists ?_("Enabling") :_("Disabling")) );
	gtk_widget_set_sensitive(GTK_WIDGET(services_dialog->service_delete_button), service_exists);
	
	debug("%s 'service_new_button'.", ((service_okay_to_save) ?_("Disabling") :_("Enabling")) );
	gtk_widget_set_sensitive(GTK_WIDGET(services_dialog->service_new_button), !service_okay_to_save);
	
	service_is_connectable=((service_is_saved && !service->connected) || service_okay_to_save);
	debug("%s 'online_service_connect_button'.", (service_is_connectable ?_("Enabling") :_("Disabling")) );
	gtk_widget_set_sensitive(GTK_WIDGET(services_dialog->online_service_connect_button), service_is_connectable);
	
	debug("%s 'save_button' & 'okay_button'.", (service_okay_to_save ?_("Enabling") :_("Disabling")) );
	gtk_widget_set_sensitive(GTK_WIDGET(services_dialog->save_button), service_okay_to_save);
	gtk_widget_set_sensitive(GTK_WIDGET(services_dialog->okay_button), service_okay_to_save);
}/*services_dialog_check*/

void services_dialog_show(GtkWindow *parent){
	if(!(services_dialog && services_dialog->dialog ))
		return services_dialog_setup(parent);
	
	debug("Service Dialog exists, presenting.");
	gtk_window_present(GTK_WINDOW(services_dialog->dialog));
}/*services_dialog_show*/


static void services_dialog_setup(GtkWindow *parent){
	GtkBuilder		*ui;
	
	debug("Creating Services Dialog from: '%s'.", GtkBuilderUI);
	services_dialog=g_new0(ServicesDialog, 1);
	
	/* Get widgets */
	ui=gtkbuilder_get_file(
				GtkBuilderUI,
					"services_dialog", &services_dialog->dialog,
					"service_enabled", &services_dialog->enabled,
					
					"urls", &services_dialog->urls,
					"urls_liststore", &services_dialog->urls_liststore,
					
					"service_new_button", &services_dialog->service_new_button,
					"service_delete_button", &services_dialog->service_delete_button,
					
					"secure_service_toggle_button", &services_dialog->secure_service_toggle_button,
					
					"username_entry", &services_dialog->username,
					"password_entry", &services_dialog->password,
					
					"show_password_checkbutton", &services_dialog->show_password,
					"autoconnect_checkbutton", &services_dialog->auto_connect,
					
					"online_service_connect_button", &services_dialog->online_service_connect_button,
					
					"service_save_button", &services_dialog->save_button,
					"services_okay_button", &services_dialog->okay_button,
				NULL
	);
	
	debug("UI loaded... setting services tree view model.");
	services_dialog->urls_model=gtk_combo_box_get_model(GTK_COMBO_BOX(services_dialog->urls));
	
	/* Connect the signals */
	debug("Services tree view model retrieved... setting signal handlers.");
	gtkbuilder_connect(ui, services_dialog,
				"services_dialog", "destroy", services_dialog_destroy_cb,
				"services_dialog", "response", services_dialog_response_cb,
				
				"urls", "changed", services_dialog_load_service,
				
				"service_new_button", "clicked", services_dialog_new_service,
				"service_delete_button", "clicked", services_dialog_delete_service,
				
				"username_entry", "key-release-event", services_dialog_check_cb,
				"password_entry", "key-release-event", services_dialog_check_cb,
				
				"online_service_connect_button", "clicked", services_dialog_connect_service,
				
				"show_password_checkbutton", "toggled", services_dialog_show_password_cb,
			NULL
	);
	
	g_object_unref(ui);
	
	debug("Signal handlers set... loading accounts.");
	gtk_combo_box_entry_set_text_column(services_dialog->urls, UrlString);
	if(!( online_services_combo_box_fill(online_services, GTK_COMBO_BOX(services_dialog->urls), services_dialog->urls_liststore, FALSE) )){
		debug("No services found to load, new accounts need to be setup.");
		services_dialog_new_service(services_dialog->service_new_button, services_dialog);
	}else
		debug("OnlineServices found & loaded.  Selecting active service.");
	
	if(parent){
		g_object_add_weak_pointer(G_OBJECT(services_dialog->dialog), (gpointer)&services_dialog);
		gtk_window_set_transient_for(GTK_WINDOW(services_dialog->dialog), parent);
	}
	gtk_window_present(GTK_WINDOW(services_dialog->dialog));
	
	debug("Loading default service.");
}

static void services_dialog_load_service(GtkComboBoxEntry *urls, ServicesDialog *services_dialog){
	if(!services_dialog) return;
	OnlineService		*service=NULL;
	
	if(!( (service=services_dialog_get_active_service(services_dialog)) )){
		gchar *uri=gtk_combo_box_get_active_text(GTK_COMBO_BOX(services_dialog->urls));
		if(G_STR_EMPTY(uri) || g_str_equal(uri, "[new account]") )
			services_dialog_new_service(services_dialog->service_new_button, services_dialog);
		g_free(uri);
		services_dialog_check_service(services_dialog);
		return;
	}
	
	if(!( service->key && service->username )){
		debug("Unable to load valid account information from 'urls_liststore'.");
		services_dialog_check_service(services_dialog);
		return;
	}
	
	debug("Accounts dialog loaded OnlineService.\n\t\taccount '%s(=%s)'\t\t\t[%sabled]\n\t\tservice url: %s; username: %s; password: %s; auto_connect: [%s]", service->decoded_key, service->key, (service->enabled?"en":"dis"), service->uri, service->username, service->password, (service->auto_connect?"TRUE":"FALSE"));
	
	services_dialog_check_service(services_dialog);
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(services_dialog->enabled), service->enabled);
	gtk_toggle_button_set_active(services_dialog->secure_service_toggle_button, service->https);
	gtk_entry_set_text(GTK_ENTRY(services_dialog->username), service->username ? service->username : "");
	gtk_entry_set_text(GTK_ENTRY(services_dialog->password), service->password ? service->password : "");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(services_dialog->auto_connect), service->auto_connect);
	services_dialog_check_service(services_dialog);
}/*services_dialog_load_service*/
