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

#include "gconfig.h"

#include "hint.h"

static void
hint_dialog_response_cb (GtkWidget *widget,
						 gint       response,
						 GtkWidget *checkbutton)
{
	GFunc        func;
	gpointer     user_data;
	const gchar *gconfig_path;
	gboolean     hide_hint;

	gconfig_path = g_object_get_data (G_OBJECT (widget), "gconfig_path");
	func = g_object_get_data (G_OBJECT (widget), "func");
	user_data = g_object_get_data (G_OBJECT (widget), "user_data");

	hide_hint = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbutton));
	gconfig_set_bool(gconfig_path, !hide_hint);

	gtk_widget_destroy (widget);

	if (func) {
		(func) ((gpointer) gconfig_path, user_data);
	}
}

gboolean
hint_dialog_show (const gchar         *gconfig_path,
						 const gchar         *message1,
						 const gchar         *message2,
						 GtkWindow           *parent,
						 GFunc                func,
						 gpointer             user_data)
{
	GtkWidget *dialog;
	GtkWidget *checkbutton;
	GtkWidget *vbox;

	g_return_val_if_fail (gconfig_path != NULL, FALSE);
	g_return_val_if_fail (message1 != NULL, FALSE);

	if(gconfig_if_bool(gconfig_path))
		return FALSE;

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

	g_object_set_data_full (G_OBJECT (dialog), "gconfig_path", g_strdup (gconfig_path), g_free);
	g_object_set_data (G_OBJECT (dialog), "user_data", user_data);
	g_object_set_data (G_OBJECT (dialog), "func", func);

	g_signal_connect (dialog, "response",
					  G_CALLBACK (hint_dialog_response_cb),
					  checkbutton);

	gtk_widget_show_all (dialog);

	return TRUE;
}

gboolean 
hint_show (const gchar         *gconfig_path,
				  const gchar         *message1,
				  const gchar         *message2,
				  GtkWindow           *parent,
				  GFunc                func,
				  gpointer             user_data)
{
	return hint_dialog_show (gconfig_path,
									message1, message2,
									parent,
									func, user_data);
}
