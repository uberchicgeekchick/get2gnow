/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2002-2007 Imendio AB
 * Copyright (C) 2008 Daniel Morales <daniminas@gmail.com>
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
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 */

#include <config.h>

#include "twitux-paths.h"
#include "twitux-xml.h"

static GtkBuilder *
xml_get_file (const gchar *filename,
              const gchar *first_widget,
              va_list      args)
{
	GtkBuilder  *ui = NULL;
	GObject    **pointer;
	const char  *name;
	gchar       *path;
	GError      *err = NULL;

	/* Create gtkbuilder & load the xml file */
	ui = gtk_builder_new ();
	gtk_builder_set_translation_domain (ui, GETTEXT_PACKAGE);
	path = twitux_paths_get_xml_path (filename);
	if (gtk_builder_add_from_file (ui, path, &err) == 0) {
		g_warning ("XML file error: %s", err->message);
		g_error_free (err);
		g_free (path);
		return NULL;
	}
	g_free (path);

	/* Grab the widgets */
	for (name = first_widget; name; name = va_arg (args, char *)) {
		pointer = va_arg (args, void *);
		
		*pointer = gtk_builder_get_object (ui, name);
		
		if (!*pointer) {
			g_warning ("Widget '%s' at '%s' is missing.", name, filename);
			continue;
		}
	}

	return ui;
}

GtkBuilder *
twitux_xml_get_file (const gchar *filename,
                     const gchar *first_widget,
                     ...)
{
	GtkBuilder *ui;
	va_list args;

	va_start (args, first_widget);

	ui = xml_get_file (filename, first_widget, args);

	va_end (args);

	if (!ui) {
		return NULL;
	}

	return ui;
}

void
twitux_xml_connect (GtkBuilder *ui,
                    gpointer    user_data,
                    gchar      *first_widget,
                    ...)
{
	GObject     *pointer;
	gpointer    *callback;
	const gchar *signal;
	const gchar *name;
	va_list      args;

	va_start (args, first_widget);
	
	for (name = first_widget; name; name = va_arg (args, char *)) {
		signal = va_arg (args, void *);
		callback = va_arg (args, void *);

		pointer = gtk_builder_get_object (ui, name);
		if (!pointer) {
			g_warning ("Missing widget '%s'", name);
			continue;
		}

		g_signal_connect (pointer, signal, G_CALLBACK (callback), user_data);
	}

	va_end (args);
}

