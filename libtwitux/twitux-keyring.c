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

#include "config.h"

#include <gnome-keyring.h>

#include "twitux-debug.h"
#include "twitux-keyring.h"

#define DEBUG_DOMAIN   "Keyring"

#define TWITTER_SERVER "twitter.com"

static const gchar *
account_gnome_keyring_result_to_string (GnomeKeyringResult result)
{
	switch (result) {
	case GNOME_KEYRING_RESULT_OK:
		return "GNOME_KEYRING_RESULT_OK";
	case GNOME_KEYRING_RESULT_DENIED:
		return "GNOME_KEYRING_RESULT_DENIED";
	case GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON:
		return "GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON";
	case GNOME_KEYRING_RESULT_ALREADY_UNLOCKED:
		return "GNOME_KEYRING_RESULT_ALREADY_UNLOCKED";
	case GNOME_KEYRING_RESULT_NO_SUCH_KEYRING:
		return "GNOME_KEYRING_RESULT_NO_SUCH_KEYRING";
	case GNOME_KEYRING_RESULT_BAD_ARGUMENTS:
		return "GNOME_KEYRING_RESULT_BAD_ARGUMENTS";
	case GNOME_KEYRING_RESULT_IO_ERROR:
		return "GNOME_KEYRING_RESULT_IO_ERROR";
	case GNOME_KEYRING_RESULT_CANCELLED:
		return "GNOME_KEYRING_RESULT_CANCELLED";
	case GNOME_KEYRING_RESULT_ALREADY_EXISTS:
		return "GNOME_KEYRING_RESULT_ALREADY_EXISTS";
	case GNOME_KEYRING_RESULT_NO_MATCH:
		return "GNOME_KEYRING_RESULT_NO_MATCH";
	}

	return "";
}

gboolean
twitux_keyring_get_password (gchar  *username,
							 gchar **password)
{
	GnomeKeyringNetworkPasswordData *data;
	GnomeKeyringResult               result;
	GList                           *passwords;
 
	result = gnome_keyring_find_network_password_sync (username,       /* User */
													   NULL,           /* Domain */
													   TWITTER_SERVER, /* Server */
													   NULL,           /* Object */
													   NULL,           /* Protocol */
													   NULL,           /* Authentication Type */
													   0,              /* Port */
													   &passwords);    /* Result */

	if (result != GNOME_KEYRING_RESULT_OK) {
		twitux_debug (DEBUG_DOMAIN,
					  "Could not retrieve password from keyring, result:%d->'%s'",
					  result, account_gnome_keyring_result_to_string (result));

		return FALSE;
	}

	if (g_list_length (passwords) > 1) {
		twitux_debug (DEBUG_DOMAIN,
					  "Found %d matching passwords in keyring, using first available",
					  g_list_length (passwords));
	}

	data = passwords->data;
	*password = g_strdup (data->password);

	g_list_foreach (passwords, (GFunc) g_free, NULL);
	g_list_free (passwords);

	return TRUE;
}

gboolean
twitux_keyring_set_password (const gchar *username,
							 const gchar *password)
{
	GnomeKeyringResult result;
	guint              id;

	result = gnome_keyring_set_network_password_sync (NULL,            /* Keyring */
													  username,        /* User */
													  NULL,            /* Domain */
													  TWITTER_SERVER,  /* Server */
													  NULL,            /* Object */
													  NULL,            /* Protocol */
													  NULL,            /* Authentication Type */
													  0,               /* Port */
													  password,        /* Password */
													  &id);            /* Unique ID */

	if (result != GNOME_KEYRING_RESULT_OK) {
		twitux_debug (DEBUG_DOMAIN,
					  "Could not set password to keyring, result:%d->'%s'",
					  result, account_gnome_keyring_result_to_string (result));

		return FALSE;
	}

	return TRUE;
}
