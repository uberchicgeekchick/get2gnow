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
 *
 * Authors: Brian Pepple <bpepple@fedoraproject.org>
 *
 */

#include <config.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include <libnotify/notify.h>

#include <libtwitux/twitux-paths.h>

#include "twitux-app.h"
#include "twitux-network.h"

int
main (int argc, char *argv[])
{
	gchar *localedir;

	localedir = twitux_paths_get_locale_path ();
	bindtextdomain (GETTEXT_PACKAGE, localedir);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
	g_free (localedir);

	g_set_application_name (_("Twitux"));

	if (!g_thread_supported ()) g_thread_init (NULL);

	gtk_init (&argc, &argv);

	gtk_window_set_default_icon_name ("twitux");

	/* Start the network */
	twitux_network_new ();

	/* Start libnotify */
	notify_init ("twitux");

	/* Create the ui */
	twitux_app_create ();

	gtk_main ();

	/* Close libnotify */
	notify_uninit ();

	/* Close the network */
	twitux_network_close ();

	/* Clean up the ui */
	g_object_unref (twitux_app_get ());

	return 0;
}
