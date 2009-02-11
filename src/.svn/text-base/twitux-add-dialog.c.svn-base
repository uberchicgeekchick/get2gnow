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

#include <libtwitux/twitux-xml.h>

#include "twitux.h"
#include "twitux-add-dialog.h"
#include "twitux-network.h"

#define XML_FILE "add_friend_dlg.xml"

typedef struct {
	GtkWidget *entry;
	GtkWidget *dialog;
} TwituxAdd;

static void add_response_cb (GtkWidget *widget,
							 gint       response,
							 TwituxAdd *add);
static void add_destroy_cb  (GtkWidget *widget,
							 TwituxAdd *add);

static void
add_response_cb (GtkWidget     *widget,
				 gint           response,
				 TwituxAdd     *add)
{
	if (response == GTK_RESPONSE_OK) {
		twitux_network_add_user (gtk_entry_get_text (GTK_ENTRY (add->entry)));
	}
	gtk_widget_destroy (widget);
}

static void
add_destroy_cb (GtkWidget     *widget,
				TwituxAdd     *add)
{
	g_free (add);
}

void
twitux_add_dialog_show (GtkWindow *parent)
{
	static TwituxAdd *add;
	GtkBuilder       *ui;

	if (add) {
		gtk_window_present (GTK_WINDOW (add->dialog));
		return;
	}

	add = g_new0 (TwituxAdd, 1);

	/* Get widgets */
	ui = twitux_xml_get_file (XML_FILE,
						"add_friend_dialog", &add->dialog,
						"frienduser_entry", &add->entry,
						NULL);

	/* Connect the signals */
	twitux_xml_connect (ui, add,
						"add_friend_dialog", "destroy", add_destroy_cb,
						"add_friend_dialog", "response", add_response_cb,
						NULL);

	g_object_unref (ui);

	/* Set the parent */
	g_object_add_weak_pointer (G_OBJECT (add->dialog), (gpointer) &add);
	gtk_window_set_transient_for (GTK_WINDOW (add->dialog), parent);

	/* Now that we're done setting up, let's show the widget */
	gtk_widget_show (add->dialog);
}
