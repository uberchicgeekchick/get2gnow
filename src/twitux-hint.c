/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2002-2007 Imendio AB
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
 *
 */
 
#include <glib/gi18n.h>

#include <libtwitux/twitux-conf.h>

#include "twitux-hint.h"

static void
hint_dialog_response_cb (GtkWidget *widget,
						 gint       response,
						 GtkWidget *checkbutton)
{
	GFunc        func;
	gpointer     user_data;
	const gchar *conf_path;
	gboolean     hide_hint;

	conf_path = g_object_get_data (G_OBJECT (widget), "conf_path");
	func = g_object_get_data (G_OBJECT (widget), "func");
	user_data = g_object_get_data (G_OBJECT (widget), "user_data");

	hide_hint = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbutton));
	twitux_conf_set_bool (twitux_conf_get (), conf_path, !hide_hint);

	gtk_widget_destroy (widget);

	if (func) {
		(func) ((gpointer) conf_path, user_data);
	}
}

gboolean
twitux_hint_dialog_show (const gchar         *conf_path,
						 const gchar         *message1,
						 const gchar         *message2,
						 GtkWindow           *parent,
						 GFunc                func,
						 gpointer             user_data)
{
	GtkWidget *dialog;
	GtkWidget *checkbutton;
	GtkWidget *vbox;
	gboolean   ok;
	gboolean   show_hint = TRUE;

	g_return_val_if_fail (conf_path != NULL, FALSE);
	g_return_val_if_fail (message1 != NULL, FALSE);

	ok = twitux_conf_get_bool (twitux_conf_get (),
							   conf_path,
							   &show_hint);

	if (ok && !show_hint) {
		return FALSE;
	}

	dialog = gtk_message_dialog_new_with_markup (parent,
												 GTK_DIALOG_DESTROY_WITH_PARENT,
												 GTK_MESSAGE_INFO,
												 GTK_BUTTONS_CLOSE,
												 "<b>%s</b>",
												 message1);

	gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),
											  "%s", message2);
	checkbutton = gtk_check_button_new_with_label (_("Do not show this again"));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);

	vbox = gtk_vbox_new (FALSE, 6);
	gtk_container_set_border_width  (GTK_CONTAINER (vbox), 6);
	gtk_box_pack_start (GTK_BOX (vbox), checkbutton, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox), vbox, FALSE, FALSE, 0);

	g_object_set_data_full (G_OBJECT (dialog), "conf_path", g_strdup (conf_path), g_free);
	g_object_set_data (G_OBJECT (dialog), "user_data", user_data);
	g_object_set_data (G_OBJECT (dialog), "func", func);

	g_signal_connect (dialog, "response",
					  G_CALLBACK (hint_dialog_response_cb),
					  checkbutton);

	gtk_widget_show_all (dialog);

	return TRUE;
}

gboolean 
twitux_hint_show (const gchar         *conf_path,
				  const gchar         *message1,
				  const gchar         *message2,
				  GtkWindow           *parent,
				  GFunc                func,
				  gpointer             user_data)
{
	return twitux_hint_dialog_show (conf_path,
									message1, message2,
									parent,
									func, user_data);
}
