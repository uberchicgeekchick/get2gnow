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

#include <libtwitux/twitux-conf.h>
#include <libtwitux/twitux-xml.h>
#ifdef HAVE_GNOME_KEYRING
#include <libtwitux/twitux-keyring.h>
#endif

#include "twitux.h"
#include "twitux-account-dialog.h"

#define XML_FILE "account_dlg.xml"

typedef struct {
	GtkWidget *dialog;
	GtkWidget *username;
	GtkWidget *password;
	GtkWidget *show_password;
} TwituxAccount;

static void      account_response_cb          (GtkWidget         *widget,
											   gint               response,
											   TwituxAccount     *act);
static void      account_destroy_cb           (GtkWidget         *widget,
											   TwituxAccount     *act);
static void      account_show_password_cb     (GtkWidget         *widget,
											   TwituxAccount     *act);

static void
account_response_cb (GtkWidget     *widget,
					 gint           response,
					 TwituxAccount *act)
{
	if (response == GTK_RESPONSE_OK) {
		TwituxConf *conf;

		conf = twitux_conf_get ();

		twitux_conf_set_string (conf,
								TWITUX_PREFS_AUTH_USER_ID,
								gtk_entry_get_text (GTK_ENTRY (act->username)));

#ifdef HAVE_GNOME_KEYRING
		twitux_keyring_set_password (gtk_entry_get_text (GTK_ENTRY (act->username)),
									 gtk_entry_get_text (GTK_ENTRY (act->password)));
#else
		twitux_conf_set_string (conf,
								TWITUX_PREFS_AUTH_PASSWORD,
								gtk_entry_get_text (GTK_ENTRY (act->password)));
#endif
	}
	gtk_widget_destroy (widget);
}

static void
account_destroy_cb (GtkWidget     *widget,
					TwituxAccount *act)
{
	g_free (act);
}

static void
account_show_password_cb (GtkWidget     *widget,
						  TwituxAccount *act)
{
	gboolean visible;

	visible = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (act->show_password));
	gtk_entry_set_visibility (GTK_ENTRY (act->password), visible);
}

void
twitux_account_dialog_show (GtkWindow *parent)
{
	static TwituxAccount *act;
	GtkBuilder           *ui;
	TwituxConf           *conf;
	gchar                *username;
	gchar                *password;

	if (act) {
		gtk_window_present (GTK_WINDOW (act->dialog));
		return;
	}

	act = g_new0 (TwituxAccount, 1);

	/* Get widgets */
	ui = twitux_xml_get_file (XML_FILE,
						"account_dialog", &act->dialog,
						"username_entry", &act->username,
						"password_entry", &act->password,
						"show_password_checkbutton", &act->show_password,
						NULL);

	/* Connect the signals */
	twitux_xml_connect (ui, act,
						"account_dialog", "destroy", account_destroy_cb,
						"account_dialog", "response", account_response_cb,
						"show_password_checkbutton", "toggled", account_show_password_cb,
						NULL);

	g_object_unref (ui);

	g_object_add_weak_pointer (G_OBJECT (act->dialog), (gpointer) &act);

	gtk_window_set_transient_for (GTK_WINDOW (act->dialog), parent);

	/*
	 * Check to see if the username & pasword are already in gconf,
	 * and if so fill in the appropriate entry widget.
	 */
	conf = twitux_conf_get ();
	twitux_conf_get_string (conf,
							TWITUX_PREFS_AUTH_USER_ID,
							&username);
	gtk_entry_set_text (GTK_ENTRY (act->username), username ? username : "");

#ifdef HAVE_GNOME_KEYRING
	/* If there is no username, don't bother searching for the password */
	if (G_STR_EMPTY (username)) {
		username = NULL;
		password = NULL;
	} else {
		if (!(twitux_keyring_get_password (username, &password))) {
			password = NULL;
		}
	}
#else
	twitux_conf_get_string (conf,
							TWITUX_PREFS_AUTH_PASSWORD,
							&password);
#endif

	gtk_entry_set_text (GTK_ENTRY (act->password), password ? password : "");
	g_free (username);
	g_free (password);

	/* Ok, let's go ahead and show it */
	gtk_widget_show (act->dialog);
}
