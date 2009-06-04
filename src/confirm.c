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
#include "confirm.h"
#include "main.h"

static void confirm_dialog_response(GtkWidget *widget, gint response, GtkWidget *checkbutton);

static void confirm_dialog_response(GtkWidget *widget, gint response, GtkWidget *checkbutton){
	GFunc        func;
	gpointer     user_data;
	const gchar *gconfig_path;

	gconfig_path = g_object_get_data (G_OBJECT (widget), "gconfig_path");
	func = g_object_get_data (G_OBJECT (widget), "func");
	user_data = g_object_get_data (G_OBJECT (widget), "user_data");

	gconfig_set_bool(gconfig_path, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton)));

	gtk_widget_destroy (widget);

	if(func)
		func( (gpointer)gconfig_path, user_data);
}

gboolean confirm_dialog_show(const gchar *gconfig_path, const gchar *message1, const gchar *message2, GtkWindow *parent, GFunc func, gpointer user_data){
	GtkMessageDialog	*message_dialog;
	GtkCheckButton		*check_button;
	GtkVBox			*vbox;
	
	g_return_val_if_fail(message1 != NULL, FALSE);
	
	if(gconfig_if_bool(gconfig_path, FALSE))
		return TRUE;
	
	message_dialog=(GtkMessageDialog *)gtk_message_dialog_new_with_markup(
								parent,
								GTK_DIALOG_DESTROY_WITH_PARENT,
								GTK_MESSAGE_INFO,
								GTK_BUTTONS_YES_NO,
								"<b>%s</b>",
								message1
	);
	
	gtk_message_dialog_format_secondary_text(message_dialog, "%s", message2);
	
	if(gconfig_path){
		check_button=(GtkCheckButton *)gtk_check_button_new_with_label(_("Do not show this again"));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_button), FALSE);
		
		vbox=(GtkVBox *)gtk_vbox_new(FALSE, 6);
		gtk_container_set_border_width (GTK_CONTAINER (vbox), 6);
		
		gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(check_button), FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(GTK_DIALOG(message_dialog)->vbox), GTK_WIDGET(vbox), FALSE, FALSE, 0);
		
		g_object_set_data_full(G_OBJECT(message_dialog), "gconfig_path", g_strdup(gconfig_path), g_free);
		g_object_set_data(G_OBJECT(message_dialog), "user_data", user_data);
		g_object_set_data(G_OBJECT(message_dialog), "func", func);
		
		g_signal_connect(message_dialog, "response", G_CALLBACK(confirm_dialog_response), check_button);
	}
	gtk_widget_show_all(GTK_WIDGET(message_dialog));
	
	switch(gtk_dialog_run(GTK_DIALOG(message_dialog))){
		case GTK_RESPONSE_YES: case GTK_RESPONSE_OK: return TRUE;
		default: return FALSE;
	}
}/*hint_dialog_show*/

