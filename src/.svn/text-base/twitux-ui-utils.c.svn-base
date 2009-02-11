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
 */

#include <config.h>

#include <glib/gi18n.h>

#include "twitux-ui-utils.h"

static gboolean
window_get_is_on_current_workspace (GtkWindow *window)
{
	GdkWindow *gdk_window;

	gdk_window = GTK_WIDGET (window)->window;
	if (gdk_window) {
		return !(gdk_window_get_state (gdk_window) &
			 GDK_WINDOW_STATE_ICONIFIED);
	} else {
		return FALSE;
	}
}


/* Checks if the window is visible as in visible on the current workspace. */
gboolean
twitux_window_get_is_visible (GtkWindow *window)
{
	gboolean visible;

	g_return_val_if_fail (window != NULL, FALSE);

	g_object_get (window,
				  "visible", &visible,
				  NULL);

	return visible && window_get_is_on_current_workspace (window);
}

/* Takes care of moving the window to the current workspace. */
void
twitux_window_present (GtkWindow *window,
					   gboolean   steal_focus)
{
	gboolean visible;
	gboolean on_current;
	guint32  timestamp;

	g_return_if_fail (window != NULL);

	/*
	 * There are three cases: hidden, visible, visible on another
	 * workspace.
	 */

	g_object_get (window,
				  "visible", &visible,
				  NULL);

	on_current = window_get_is_on_current_workspace (window);

	if (visible && !on_current) {
		/* Hide it so present brings it to the current workspace. */
		gtk_widget_hide (GTK_WIDGET (window));
	}

	timestamp = gtk_get_current_event_time ();
	if (steal_focus && timestamp != GDK_CURRENT_TIME) {
		gtk_window_present_with_time (window, timestamp);
	} else {
		gtk_window_present (window);
	}
}

void
twitux_help_show (GtkWindow *parent)
{
	GdkScreen *screen;
	GError    *err = NULL;

	screen = gtk_widget_get_screen (GTK_WIDGET (parent));

	gtk_show_uri (screen,
				  "ghelp:twitux",
				  gtk_get_current_event_time (),
				  &err);

	if (err != NULL) {
		GtkWidget *w;

		w =	gtk_message_dialog_new (GTK_WINDOW (parent),
									GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
									GTK_MESSAGE_ERROR,
									GTK_BUTTONS_CLOSE,
									_("Unable to open help file"));

		gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (w),
												  "%s", err->message);

		g_signal_connect (w, "response",
						  G_CALLBACK (gtk_widget_destroy),
						  NULL);
		gtk_window_present (GTK_WINDOW (w));
		g_error_free (err);
	}
}
