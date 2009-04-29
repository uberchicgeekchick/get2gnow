/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2007-2008 Brian Pepple <bpepple@fedoraproject.org>
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

#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>


#include "gconfig.h"
#include "gtkbuilder.h"
#ifdef HAVE_GNOME_KEYRING
#include "keyring.h"
#endif

#include "main.h"
#include "services-dialog.h"
#include "debug.h"
#include "network.h"
#include "online-service.h"

#define GtkBuilderUI "services-dialog.ui"
#define DEBUG_DOMAIN "OnlineServices"

enum {
	SERVICES_URI
};

typedef struct {
	GtkDialog		*dialog;
	GtkCheckButton		*enabled;
	GtkComboBoxEntry	*services;
	GtkListStore		*services_list;
	GtkEntry		*username;
	GtkEntry		*password;
	GtkCheckButton		*show_password;
	GtkCheckButton		*auto_connect;
	GtkButton		*apply_button;
} AccountDialog;

static void accounts_response_cb(GtkWidget *widget, gint response, AccountDialog *act);
static void accounts_destroy_cb(GtkWidget *widget, AccountDialog *act);
static void accounts_show_password_cb(GtkWidget *widget, AccountDialog *act);
static void accounts_load_service(GtkComboBox *services, AccountDialog *act);
static void accounts_apply(GtkButton *button, AccountDialog *act);


static void accounts_response_cb( GtkWidget *widget, gint response, AccountDialog *act){
	if(response == GTK_RESPONSE_CLOSE)
		return gtk_widget_destroy(widget);
	
	accounts_apply(act->apply_button, act);
	
	if(response == GTK_RESPONSE_OK)
		gtk_widget_destroy (widget);
}

static void accounts_apply(GtkButton *button, AccountDialog *act){
	gchar *auth_uri=gtk_combo_box_get_active_text(GTK_COMBO_BOX(act->services));

	OnlineService *service=online_service_new(auth_uri);
	online_service_save(
				service,
				gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(act->enabled)),
				gtk_entry_get_text(GTK_ENTRY(act->username)),
				gtk_entry_get_text(GTK_ENTRY(act->password)),
				gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(act->auto_connect))
	);
	online_service_deinit(service);
	g_free(auth_uri);
}

static void accounts_destroy_cb (GtkWidget *widget, AccountDialog *act){
	g_free (act);
}

static void accounts_show_password_cb(GtkWidget *widget, AccountDialog *act){
	gboolean visible;

	visible = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (act->show_password));
	gtk_entry_set_visibility (GTK_ENTRY (act->password), visible);
}

void accounts_dialog_show (GtkWindow *parent){
	static AccountDialog	*act;
	GtkTreeIter		iter;
	GtkBuilder		*ui;
	
	if(act){
		gtk_window_present(GTK_WINDOW (act->dialog));
		return;
	}
	
	act=g_new0(AccountDialog, 1);
	
	/* Get widgets */
	ui=gtkbuilder_get_file(GtkBuilderUI,
					"service_enabled", &act->enabled,
					"accounts_dialog", &act->dialog,
					"services", &act->services,
					"services_list", &act->services_list,
					"username_entry", &act->username,
					"password_entry", &act->password,
					"show_password_checkbutton", &act->show_password,
					"autoconnect_checkbutton", &act->auto_connect,
					"apply_button", &act->apply_button,
					NULL
	);
	
	/* Connect the signals */
	gtkbuilder_connect(ui, act,
				"accounts_dialog", "destroy", accounts_destroy_cb,
				"accounts_dialog", "response", accounts_response_cb,
				"show_password_checkbutton", "toggled", accounts_show_password_cb,
				"services", "changed", accounts_load_service,
				NULL
	);
	
	g_object_unref(ui);
	GtkCellRenderer *renderer=gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(act->services), renderer, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(act->services), renderer, "text", 0, NULL);
	gtk_combo_box_entry_set_text_column(act->services, 0);
	gtk_list_store_append(act->services_list, &iter);
	gtk_list_store_set(act->services_list, &iter, 0, "twitter.com", -1);
	gtk_list_store_append(act->services_list, &iter);
	gtk_list_store_set(act->services_list, &iter, 0, "identi.ca", -1);
	gtk_combo_box_set_active(GTK_COMBO_BOX(act->services), 0);
	
	g_object_add_weak_pointer(G_OBJECT(act->dialog), (gpointer) &act);
	
	gtk_window_set_transient_for(GTK_WINDOW(act->dialog), parent);
	
	accounts_load_service(GTK_COMBO_BOX(act->services), act);
	/* Ok, let's go ahead and show it */
	gtk_widget_show(GTK_WIDGET(act->dialog));
}

static void accounts_load_service(GtkComboBox *services, AccountDialog *act){
	OnlineService *service=online_service_new( gtk_combo_box_get_active_text(GTK_COMBO_BOX(act->services)) );
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(act->enabled), service->enabled);
	gtk_entry_set_text(GTK_ENTRY (act->username), service->username ? service->username : "");
	gtk_entry_set_text(GTK_ENTRY(act->password), service->password ? service->password : "");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(act->auto_connect), service->auto_connect);
	online_service_deinit(service);
}//accounts_update_fields
