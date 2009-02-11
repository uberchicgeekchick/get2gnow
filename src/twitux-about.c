/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2007-2009 Brian Pepple <bpepple@fedoraproject.org>
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

#include <gio/gio.h>
#include <glib/gi18n.h>

#include "twitux-about.h"

#define WEB_SITE "https://sourceforge.net/projects/twitux/"

const gchar *authors[] = {
	"Daniel Morales <daniminas@gmail.com>",
	"Brian Pepple <bpepple@fedoraproject.org>",
	NULL
};

const gchar *artists[] = {
	"Architetto Francesco Rollandin",
	NULL
};

const gchar *license[] = {
	N_("Twitux is free software; you can redistribute it and/or modify "
   	   "it under the terms of the GNU General Public License as published by "
       "the Free Software Foundation; either version 2 of the License, or "
	   "(at your option) any later version."),
	N_("Twitux is distributed in the hope that it will be useful, "
       "but WITHOUT ANY WARRANTY; without even the implied warranty of "
       "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
       "GNU General Public License for more details."),
	N_("You should have received a copy of the GNU Lesser General Public "
       "License along with this library; if not, write to the "
       "Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, "
   	   "Boston, MA 02110-1301, USA.")
};

static void
about_dialog_activate_link_cb (GtkAboutDialog *about,
							   const gchar    *link,
							   gpointer        data)
{
	if (g_app_info_launch_default_for_uri (link, NULL, NULL) == FALSE) {
		g_warning ("Couldn't show URL: '%s'", link);
	}
}

void
twitux_about_dialog_new (GtkWindow *parent)
{
	gchar *license_trans;

	gtk_about_dialog_set_url_hook (about_dialog_activate_link_cb, NULL, NULL);

	license_trans = g_strconcat (_(license[0]), "\n\n", _(license[1]), "\n\n",
								 _(license[2]), "\n\n", NULL);

	gtk_show_about_dialog (parent,
						   "authors", authors,
						   "artists", artists,
						   "comments", _("A GNOME client for Twitter."),
						   "copyright", _("Copyright \xc2\xa9 2007-2009 Daniel Morales"),
						   "license", license_trans,
						   "translator-credits", _("translator-credits"),
						   "version", PACKAGE_VERSION,
						   "website", WEB_SITE,
						   "wrap-license", TRUE,
						   "logo-icon-name", "twitux",
						   NULL);

	g_free (license_trans);
}
