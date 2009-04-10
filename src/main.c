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
#include <glib/gprintf.h>

#include <libnotify/notify.h>

#include "app.h"
#include "friends-manager.h"
#include "network.h"
#include "ipc.h"

int main(int argc, char **argv){
	if( (ipc_init_check( argc-1, argv-1)) ){
		g_printf( "%s is already running.  Be sure to check system try for %s's icon.\n", PACKAGE_NAME, PACKAGE_NAME );
		ipc_deinit();
		exit(0);
	}
	gboolean notifing=FALSE;

	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);

	g_set_application_name(_("Greet-Tweet-Know"));

	if(!g_thread_supported()) g_thread_init(NULL);

	gtk_init(&argc, &argv);

	gtk_window_set_default_icon_name("greet-tweet-know");

	/* Start the network */
	network_new();

	/* Start libnotify */
	notifing=notify_init("greet-tweet-know");

	/* Create the ui */
	app_create();

	gtk_main();
	
	/* Close libnotify */
	if(notifing) notify_uninit();

	/* Close the network */
	network_close();

	ipc_deinit();
	
	/* Clean up the ui */
	g_object_unref(app_get());

	return 0;
}
