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

#define _GNU_SOURCE
#define _THREAD_SAFE


#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>

#include "gconfig.h"
#include "gtkbuilder.h"

#include "config.h"
#include "program.h"

#include "online-services.typedefs.h"
#include "online-services.types.h"
#include "online-services-dialog.h"
#include "online-services.h"
#include "online-service.types.h"
#include "online-service.h"
#include "online-service-request.h"

#include "tabs.h"

#include "network.h"

#include "preferences.defines.h"

#include "ui-utils.h"


#define GTK_BUILDER_UI_FILENAME "online-services-dialog"

#define DEBUG_DOMAINS "OnlineServices:UI:GtkBuilder:GtkBuildable:Requests:Authentication:Preferences:Accounts:Settings:Setup:OnlineServicesDialog:online-services-dialog.c"
#include "debug.h"


typedef struct {	
	GtkDialog		*online_services_dialog;
	GtkCheckButton		*enabled;
	
	GtkComboBoxEntry	*keys;
	GtkEntryCompletion	*keys_completion;
	GtkListStore		*keys_list_store;
	
	GtkButton		*online_service_new_button;
	GtkButton		*online_service_delete_button;
	
	GtkToggleButton		*https;
	
	/*currently unused
	GtkComboBox		*service_type_combo_box;
	GtkListStore		*service_type_list_store;
	*/
	GtkEntry		*uri;
	
	GtkEntry		*user_name;
	GtkEntry		*password;
	GtkCheckButton		*show_password;
	GtkCheckButton		*auto_connect;
	GtkCheckButton		*post_to_by_default;
	
	GtkButton		*online_service_connect_button;
	
	/*Buttons in services-dialog 'action-area.*/
	GtkButton		*online_services_close_button;
	GtkButton		*online_services_okay_button;
	GtkButton		*online_services_save_button;
} OnlineServicesDialog;

static OnlineServicesDialog	*online_services_dialog=NULL;
static gboolean			exit_okay=TRUE;

static void online_services_dialog_response(GtkDialog *dialog, gint response, OnlineServicesDialog *services);
static void online_services_dialog_destroy(GtkDialog *dialog);
static void online_services_dialog_show_password(GtkCheckButton *show_password, OnlineServicesDialog *online_services_dialog);
static void online_services_dialog_load_service(GtkWidget *changed_entry, OnlineServicesDialog *online_services_dialog);

static OnlineService *online_services_dialog_get_active_service(OnlineServicesDialog *online_services_dialog);
static void online_services_dialog_new_service(GtkButton *online_service_new_button, OnlineServicesDialog *online_services_dialog);
static void online_services_dialog_save_service(GtkButton *online_services_save_button, OnlineServicesDialog *online_services_dialog);
static void online_services_dialog_connect(GtkButton *online_service_connect_button, OnlineServicesDialog *online_services_dialog);
static void online_services_dialog_delete_service(GtkButton *service_delete_button, OnlineServicesDialog *online_services_dialog);

static void online_services_dialog_check(GtkEntry *entry, GdkEventKey *event, OnlineServicesDialog *online_services_dialog);
static void online_services_dialog_check_service(OnlineServicesDialog *online_services_dialog);
static void online_services_dialog_set_okay_to_save(gboolean service_okay_to_save, OnlineServicesDialog *online_services_dialog);
static void online_services_dialog_toggle_button_clicked(GtkToggleButton *toggle_button, OnlineServicesDialog *online_services_dialog);

static void online_services_dialog_setup(GtkWindow *parent);

static void online_services_dialog_response(GtkDialog *dialog, gint response, OnlineServicesDialog *online_services_dialog){
	debug("OnlineServicesDialog has received a response... changes will be now be saved");
	switch(response){
		case GTK_RESPONSE_CANCEL:
		case GTK_RESPONSE_REJECT:
		case GTK_RESPONSE_DELETE_EVENT:
		case GTK_RESPONSE_CLOSE:
		case GTK_RESPONSE_NO:
			gtk_widget_destroy(GTK_WIDGET(dialog));
			online_services_dialog=NULL;
			return;
	}
	
	online_services_dialog_save_service(online_services_dialog->online_services_save_button, online_services_dialog);
	
	if(response==GTK_RESPONSE_OK && exit_okay){
		gtk_widget_destroy(GTK_WIDGET(dialog));
		online_services_dialog=NULL;
	}
}/*online_services_dialog_response*/

static void online_services_dialog_new_service(GtkButton *online_service_new_button, OnlineServicesDialog *online_services_dialog){
	debug("Preparing OnlineServicesDialog for a new account setup");
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(online_services_dialog->enabled), TRUE);
	gtk_entry_set_text(GTK_ENTRY(online_services_dialog->uri), "");
	gtk_toggle_button_set_active(online_services_dialog->https, TRUE);
	gtk_entry_set_text(GTK_ENTRY(online_services_dialog->user_name), "");
	gtk_entry_set_text(GTK_ENTRY(online_services_dialog->password), "");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(online_services_dialog->auto_connect), TRUE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(online_services_dialog->post_to_by_default), TRUE);
	
	gtk_widget_grab_focus(GTK_WIDGET(online_services_dialog->keys));
	
	if(!(online_services_dialog_get_active_service(online_services_dialog)))
		return;
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(online_services_dialog->keys), 0);
}/*online_services_dialog_new_service(online_services_dialog->online_service_new_button, online_services_dialog);*/

static void online_services_dialog_save_service(GtkButton *save_button, OnlineServicesDialog *online_services_dialog){
	debug("Saving services");
	exit_okay=FALSE;
	
	const gchar *uri=gtk_entry_get_text(GTK_ENTRY(online_services_dialog->uri));
	const gchar *user_name=gtk_entry_get_text(GTK_ENTRY(online_services_dialog->user_name));
	const gchar *password=gtk_entry_get_text(GTK_ENTRY(online_services_dialog->password));
	if(!(G_STR_N_EMPTY(uri) && G_STR_N_EMPTY(user_name) && G_STR_N_EMPTY(password))){
		debug("Failed to save current account.  Server, user_name, and/or password missing");
		exit_okay=TRUE;
		return;
	}
	
	OnlineService		*service=NULL;
	gboolean		new_service=FALSE;
	
	debug("Retriving current service");
	if(!((service=online_services_dialog_get_active_service(online_services_dialog)) && g_str_equal(service->uri, uri) && g_str_equal(service->user_name, user_name))){
		debug("Current service does not have a valid GtkTreeIter, service will be appended later");
		new_service=TRUE;
	}
	
	
	if(!( (service=online_services_save_service(
				service,
				uri,
				user_name,
				password,
				gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(online_services_dialog->enabled)),
				gtk_toggle_button_get_active(online_services_dialog->https),
				gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(online_services_dialog->auto_connect)),
				gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(online_services_dialog->post_to_by_default))
	)) )){
		debug("**ERROR:** Failed to save online service for <%s@%s> please see above for further details", user_name, uri);
		return;
	}
	
	if(!new_service){
		exit_okay=TRUE;
		return;
	}
	
	if(!(service && service->key && service->uri && service->user_name)){
		debug("**ERROR:** OnlineServices saved resulting OnlineService is invalid");
		return;
	}
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	debug("Storing new service: '%s'", service->key);
	gtk_list_store_append(online_services_dialog->keys_list_store, iter);
	gtk_list_store_set(
				online_services_dialog->keys_list_store, iter,
					OnlineServiceKey, service->key,
					UrlString, uri,
					OnlineServicePointer, service,
				-1
	);
	
	debug("New service stored, selecting it new loction in the dialog");
	exit_okay=TRUE;
	gtk_combo_box_set_active_iter(GTK_COMBO_BOX(online_services_dialog->keys), iter);
	if(iter) uber_free(iter);
	tabs_refresh();
}/*online_services_dialog_save_service(online_services_dialog->online_services_save_button, online_services_dialog);*/

static void online_services_dialog_connect(GtkButton *connect_button, OnlineServicesDialog *online_services_dialog){
	OnlineService *service=NULL;
	
	gboolean enabled=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(online_services_dialog->enabled));
	gboolean https=gtk_toggle_button_get_active(online_services_dialog->https);
	
	const gchar *uri=gtk_entry_get_text(GTK_ENTRY(online_services_dialog->uri));
	const gchar *user_name=gtk_entry_get_text(GTK_ENTRY(online_services_dialog->user_name));
	const gchar *password=gtk_entry_get_text(GTK_ENTRY(online_services_dialog->password));
	
	gboolean auto_connect=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(online_services_dialog->auto_connect));
	gboolean post_to_by_default=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(online_services_dialog->post_to_by_default));
	
	if(!(service=online_services_dialog_get_active_service(online_services_dialog))){
		debug("OnlineService is not saved.  Creating temporary OnlineService to connect with");
		service=online_service_new(uri, user_name, password, enabled, https, auto_connect, post_to_by_default);
		online_service_connect(service);
		online_service_login(service, TRUE);
	}else if(service && G_STR_N_EMPTY(uri) && g_str_equal(service->uri, uri) && G_STR_N_EMPTY(user_name) && g_str_equal(service->user_name, user_name) && G_STR_N_EMPTY(password) && g_str_equal(service->password, password)){
		if(service->connected){
			debug("OnlineService: %s is already connected", service->key);
			return;
		}
		online_service_connect(service);
		online_service_login(service, TRUE);
	}
}/*online_services_dialog_connect(online_services_dialog->online_service_connect_button, online_services_dialog);*/

static void online_services_dialog_delete_service(GtkButton *online_service_delete_button, OnlineServicesDialog *online_services_dialog){
	OnlineService		*service=NULL;
	
	debug("Retriving current service");
	if(!((service=online_services_dialog_get_active_service(online_services_dialog)))){
		debug("There is no valid account selected");
		return;
	}
	
	gchar *confirm_message=g_strdup_printf("%s: %s?", _("Are you sure you want to delete"), service->key);
	if(!(online_service_request_popup_confirmation_dialog(
					PREFS_ACCOUNT_DELETE,
					confirm_message,
					_("You will no longer be able to send or recieve messages using this account.\n\nIf you want to use this service again you will have to set it up again."),
					NULL, NULL
	))){
		uber_free(confirm_message);
		debug("Account deletion was canceled by the user");
		return;
	}

	uber_free(confirm_message);
	
	online_services_dialog_new_service(online_services_dialog->online_service_new_button, online_services_dialog);
	debug("Deleting service: '%s'", service->key);
	online_services_delete_service(service);
	
	debug("Reloading OnlineServices into OnlineServicesDialog 'keys_list_store");
	if(!(online_services_combo_box_fill(GTK_COMBO_BOX(online_services_dialog->keys), online_services_dialog->keys_list_store, FALSE)))
		debug("No services found to load, new accounts need to be setup");
	tabs_refresh();
}/*online_services_dialog_delete_service(online_services_dialog->online_service_delete_button, online_services_dialog);*/

static void online_services_dialog_destroy(GtkDialog *dialog){
	uber_object_unref(online_services_dialog->keys_completion);
	uber_free(online_services_dialog);
}/*online_services_dialog_destroy(dialog);*/

static void online_services_dialog_show_password(GtkCheckButton *show_password, OnlineServicesDialog *online_services_dialog){
	gtk_entry_set_visibility(GTK_ENTRY(online_services_dialog->password), gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(online_services_dialog->show_password)));
}/*online_services_dialog_show_password*/

static OnlineService *online_services_dialog_get_active_service(OnlineServicesDialog *online_services_dialog){
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	OnlineService		*service=NULL;
	
	const gchar *key=GTK_ENTRY(GTK_BIN(online_services_dialog->keys)->child)->text;
	if(!gtk_tree_model_get_iter_first(GTK_TREE_MODEL(online_services_dialog->keys_list_store), iter)){
		uber_free(iter);
		return NULL;
	}
	if(!gtk_tree_model_iter_next(GTK_TREE_MODEL(online_services_dialog->keys_list_store), iter)){
		uber_free(iter);
		return NULL;
	}
	
	do{
		gtk_tree_model_get(
				GTK_TREE_MODEL(online_services_dialog->keys_list_store), iter,
					OnlineServicePointer, &service,
				-1
		);
		if(service && service->key && g_str_equal(service->key, key)) break;
		service=NULL;
	}while(gtk_tree_model_iter_next(GTK_TREE_MODEL(online_services_dialog->keys_list_store), iter));
	uber_free(iter);
	
	if(!(service && service->key && service->uri && service->user_name))
		return NULL;
	
	return service;
}/*online_services_dialog_get_active_service(online_services_dialog);*/

static void online_services_dialog_check(GtkEntry *entry, GdkEventKey *event, OnlineServicesDialog *online_services_dialog){
	online_services_dialog_check_service(online_services_dialog);
}/*online_services_dialog_check*/

static void online_services_dialog_check_service(OnlineServicesDialog *online_services_dialog){
	gboolean		service_okay_to_save=FALSE, service_exists=FALSE, service_is_saved=FALSE, service_is_connectable=FALSE;
	OnlineService		*service=online_services_dialog_get_active_service(online_services_dialog);
	
	gboolean enabled=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(online_services_dialog->enabled));
	const gchar *uri=gtk_entry_get_text(GTK_ENTRY(online_services_dialog->uri));
	gboolean https=gtk_toggle_button_get_active(online_services_dialog->https);
	const gchar *user_name=gtk_entry_get_text(GTK_ENTRY(online_services_dialog->user_name));
	const gchar *password=gtk_entry_get_text(GTK_ENTRY(online_services_dialog->password));
	gboolean auto_connect=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(online_services_dialog->auto_connect));
	gboolean post_to_by_default=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(online_services_dialog->post_to_by_default));
	
	if(service){
		debug("Service dialog is editing an existing service");
		service_exists=TRUE;
	}
	
	if(service && enabled==service->enabled && G_STR_N_EMPTY(uri) && service->uri && g_str_equal(service->uri, uri) && G_STR_N_EMPTY(user_name) && service->user_name && g_str_equal(service->user_name, user_name) && G_STR_N_EMPTY(password) && service->password && g_str_equal(service->password, password) && https==service->https && auto_connect==service->auto_connect && service->post_to_by_default==post_to_by_default){
		debug("Services is up to date, no changes need saved");
		service_is_saved=TRUE;
		service_okay_to_save=FALSE;
	}else if(service && (enabled!=service->enabled || (G_STR_N_EMPTY(uri) && service->uri && !g_str_equal(service->uri, uri)) || (G_STR_N_EMPTY(user_name) && service->user_name && !g_str_equal(service->user_name, user_name)) || (G_STR_N_EMPTY(password) && service->password && !g_str_equal(service->password, password)) || https!=service->https || auto_connect!=service->auto_connect || post_to_by_default!=service->post_to_by_default)){
		debug("Existing service has changes that need to be saved");
		service_okay_to_save=TRUE;
	}else if(!service && (G_STR_N_EMPTY(uri) && G_STR_N_EMPTY(user_name) && G_STR_N_EMPTY(password))){
		debug("Service Dialog is creating a OnlineService & all needed fields have data so the service may be saved");
		service_okay_to_save=TRUE;
	}
	
	debug("%s 'service_delete_button'", (service_exists ?_("Enabling") :_("Disabling")));
	gtk_widget_set_sensitive(GTK_WIDGET(online_services_dialog->online_service_delete_button), service_exists);
	
	service_is_connectable=((service_is_saved && !service->connected) || service_okay_to_save);
	debug("%s 'online_service_connect_button'", (service_is_connectable ?_("Enabling") :_("Disabling")));
	gtk_widget_set_sensitive(GTK_WIDGET(online_services_dialog->online_service_connect_button), service_is_connectable);
	
	online_services_dialog_set_okay_to_save(service_okay_to_save, online_services_dialog);
}/*online_services_dialog_check*/

static void online_services_dialog_set_okay_to_save(gboolean service_okay_to_save, OnlineServicesDialog *online_services_dialog){
	debug("%s 'service_new_button'", ((service_okay_to_save) ?_("Disabling") :_("Enabling")));
	gtk_widget_set_sensitive(GTK_WIDGET(online_services_dialog->online_service_new_button), !service_okay_to_save);
	
	debug("%s 'save_button' & 'okay_button'", (service_okay_to_save ?_("Enabling") :_("Disabling")));
	gtk_widget_set_sensitive(GTK_WIDGET(online_services_dialog->online_services_save_button), service_okay_to_save);
	gtk_widget_set_sensitive(GTK_WIDGET(online_services_dialog->online_services_okay_button), service_okay_to_save);
}/*online_services_dialog_set_okay_to_save(TRUE||FALSE, online_services_dialog);*/

static void online_services_dialog_toggle_button_clicked(GtkToggleButton *toggle_button, OnlineServicesDialog *online_services_dialog){
	online_services_dialog_set_okay_to_save(TRUE, online_services_dialog);
}/*online_services_dialog_toggle_button_clicked(toggle_button, online_services_dialog);*/

void online_services_dialog_show(GtkWindow *parent){
	if(!(online_services_dialog && online_services_dialog->online_services_dialog))
		return online_services_dialog_setup(parent);
	
	debug("Service Dialog exists, presenting");
	window_present(GTK_WINDOW(online_services_dialog->online_services_dialog), TRUE);
}/*online_services_dialog_show*/


static void online_services_dialog_setup(GtkWindow *parent){
	debug("Creating Services Dialog from: '%s'", GTK_BUILDER_UI_FILENAME);
	online_services_dialog=g_new0(OnlineServicesDialog, 1);
	
	/* Get widgets */
	GtkBuilder *ui=gtkbuilder_get_file(
				GTK_BUILDER_UI_FILENAME,
					"online_services_dialog", &online_services_dialog->online_services_dialog,
					"online_service_enabled", &online_services_dialog->enabled,
					
					"online_services_keys_combo_box", &online_services_dialog->keys,
					"online_services_keys_list_store", &online_services_dialog->keys_list_store,
					
					"online_service_new_button", &online_services_dialog->online_service_new_button,
					"online_service_delete_button", &online_services_dialog->online_service_delete_button,
					
					"online_services_service_uri_entry", &online_services_dialog->uri,
					
					"https_toggle_button", &online_services_dialog->https,
					
					"user_name_entry", &online_services_dialog->user_name,
					"password_entry", &online_services_dialog->password,
					
					"show_password_checkbutton", &online_services_dialog->show_password,
					"autoconnect_checkbutton", &online_services_dialog->auto_connect,
					"post_to_by_default_check_button", &online_services_dialog->post_to_by_default,
					
					"online_service_connect_button", &online_services_dialog->online_service_connect_button,
					
					"online_services_close_button", &online_services_dialog->online_services_close_button,
					"online_services_save_button", &online_services_dialog->online_services_save_button,
					"online_services_okay_button", &online_services_dialog->online_services_okay_button,
				NULL
	);
	
	debug("UI loaded... setting services tree view model");
	
	gchar *window_title=g_strdup_printf("%s - %s", _(GETTEXT_PACKAGE), _("Manage Online Services"));
	gtk_window_set_title(GTK_WINDOW(online_services_dialog->online_services_dialog), window_title);
	uber_free(window_title);
	
	/* Connect the signals */
	debug("Services tree view model retrieved... setting signal handlers");
	gtkbuilder_connect(ui, online_services_dialog,
				"online_services_dialog", "destroy", online_services_dialog_destroy,
				"online_services_dialog", "response", online_services_dialog_response,
				
				"online_service_enabled", "toggled", online_services_dialog_toggle_button_clicked,
				
				"online_services_keys_combo_box", "changed", online_services_dialog_load_service,
				
				"online_service_new_button", "clicked", online_services_dialog_new_service,
				"online_service_delete_button", "clicked", online_services_dialog_delete_service,
				
				"online_services_service_uri_entry", "key-release-event", online_services_dialog_check,
				"online_services_service_uri_entry", "changed", online_services_dialog_load_service,
				
				"https_toggle_button", "toggled", online_services_dialog_toggle_button_clicked,
				
				"user_name_entry", "key-release-event", online_services_dialog_check,
				"user_name_entry", "changed", online_services_dialog_load_service,
				
				"password_entry", "key-release-event", online_services_dialog_check,
				
				"autoconnect_checkbutton", "toggled", online_services_dialog_toggle_button_clicked,
				
				"post_to_by_default_check_button", "toggled", online_services_dialog_toggle_button_clicked,
				
				"online_service_connect_button", "clicked", online_services_dialog_connect,
				
				"show_password_checkbutton", "toggled", online_services_dialog_show_password,
			NULL
	);
	
	uber_object_unref(ui);
	
	debug("Signal handlers set... loading accounts");
	online_services_dialog->keys_completion=gtk_entry_completion_new();
	gtk_entry_completion_set_model(online_services_dialog->keys_completion, GTK_TREE_MODEL(online_services_dialog->keys_list_store));
	gtk_entry_set_completion(GTK_ENTRY(GTK_BIN(online_services_dialog->keys)->child), online_services_dialog->keys_completion);
	gtk_entry_completion_set_text_column(online_services_dialog->keys_completion, OnlineServiceKey);
	gtk_combo_box_entry_set_text_column(online_services_dialog->keys, OnlineServiceKey);
	if(!(online_services_combo_box_fill(GTK_COMBO_BOX(online_services_dialog->keys), online_services_dialog->keys_list_store, FALSE))){
		debug("No services found to load, new accounts need to be setup");
		online_services_dialog_new_service(online_services_dialog->online_service_new_button, online_services_dialog);
	}else
		debug("OnlineServices found & loaded.  Selecting active service");
	
	if(parent){
		g_object_add_weak_pointer(G_OBJECT(online_services_dialog->online_services_dialog), (gpointer)&online_services_dialog);
		gtk_window_set_transient_for(GTK_WINDOW(online_services_dialog->online_services_dialog), parent);
	}
	window_present(GTK_WINDOW(online_services_dialog->online_services_dialog), TRUE);
}

static void online_services_dialog_load_service(GtkWidget *changed_entry, OnlineServicesDialog *online_services_dialog){
	if(!online_services_dialog) return;
	OnlineService		*service=NULL;
	if(GTK_IS_EDITABLE(changed_entry) && GTK_ENTRY(changed_entry)!=GTK_ENTRY(GTK_BIN(online_services_dialog->keys)->child) && !gtk_widget_has_focus(GTK_WIDGET(GTK_BIN(online_services_dialog->keys)->child))){
		gchar *services_key=g_strdup_printf("%s@%s", online_services_dialog->user_name->text, online_services_dialog->uri->text);
		if(g_str_n_equal("@", services_key) && g_str_n_equal(GTK_ENTRY(GTK_BIN(online_services_dialog->keys)->child)->text, services_key))
			gtk_entry_set_text(GTK_ENTRY(GTK_BIN(online_services_dialog->keys)->child), services_key);
		uber_free(services_key);
		return;
	}
	
	if(!((service=online_services_dialog_get_active_service(online_services_dialog)))){
		const gchar *key=GTK_ENTRY(GTK_BIN(online_services_dialog->keys)->child)->text;
		if(G_STR_EMPTY(key) || g_str_equal(key, "[new account]"))
			online_services_dialog_new_service(online_services_dialog->online_service_new_button, online_services_dialog);
		else{
			if(!g_strrstr(key, "@"))
				gtk_entry_set_text(GTK_ENTRY(online_services_dialog->user_name), key);
			else{
				gchar **account_details=g_strsplit(key, "@", 2);
				gtk_entry_set_text(GTK_ENTRY(online_services_dialog->uri), account_details[1]);
				gtk_entry_set_text(GTK_ENTRY(online_services_dialog->user_name), account_details[0]);
				g_strfreev(account_details);
			}
		}
		online_services_dialog_check_service(online_services_dialog);
		return;
	}
	
	if(!(service->key && service->user_name)){
		debug("Unable to load valid account information from 'keys_list_store'");
		online_services_dialog_check_service(online_services_dialog);
		return;
	}
	
	debug("Accounts dialog loaded OnlineService.\n\t\taccount '%s(=%s)'\t\t\t[%sabled]\n\t\tservice url: %s; user_name: %s; password: %s; auto_connect: [%s]; post updates to by default: [%s]", service->key, service->key, (service->enabled?"en":"dis"), service->uri, service->user_name, service->password, (service->auto_connect?"TRUE":"FALSE"), (service->post_to_by_default?"TRUE":"FALSE"));
	
	online_services_dialog_check_service(online_services_dialog);
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(online_services_dialog->enabled), service->enabled);
	gtk_entry_set_text(GTK_ENTRY(online_services_dialog->uri), service->uri);
	gtk_toggle_button_set_active(online_services_dialog->https, service->https);
	gtk_entry_set_text(GTK_ENTRY(online_services_dialog->user_name), service->user_name);
	gtk_entry_set_text(GTK_ENTRY(online_services_dialog->password), service->password);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(online_services_dialog->auto_connect), service->auto_connect);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(online_services_dialog->post_to_by_default), service->post_to_by_default);
	online_services_dialog_check_service(online_services_dialog);
}/*online_services_dialog_load_service*/
