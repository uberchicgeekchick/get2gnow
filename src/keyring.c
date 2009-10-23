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

#define _GNU_SOURCE
#define _THREAD_SAFE

#include "config.h"

#include <gnome-keyring.h>

#include "online-services-typedefs.h"
#include "online-service.types.h"
#include "keyring.h"

#define DEBUG_DOMAINS   "OnlineServices:Authentication:Settings:Setup:GNOME-Keyring:Security:keyring.c"
#include "debug.h"

#ifndef GNOME_ENABLE_DEBUG
#define	DOMAIN	NULL
#else
#define	DOMAIN "debug"
#endif

static const gchar *account_gnome_keyring_result_to_string (GnomeKeyringResult result){
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

gboolean keyring_get_password(OnlineService **service){
	GnomeKeyringNetworkPasswordData *data;
	GnomeKeyringResult               result;
	GList                           *passwords;
	
	if(! (result=gnome_keyring_find_network_password_sync(
							(*service)->user_name,       /* User */
								DOMAIN,           /* Domain */
								(*service)->uri, /* Server */
								NULL,           /* Object */
								NULL,           /* Protocol */
								NULL,           /* Authentication Type */
								0,              /* Port */
							&passwords    /* Result */
	))==GNOME_KEYRING_RESULT_OK){
		debug("**ERROR:** Could not retrieve password from keyring, result:%d->'%s'", result, account_gnome_keyring_result_to_string(result) );
		return FALSE;
	}

	debug("Found %d matching passwords in keyring, using first available",  g_list_length(passwords));
	
	data=passwords->data;
	GList *p=NULL;
	(*service)->password=g_strdup(data->password);
	if(DOMAIN){
		debug("Password(s) found for OnlineService: '%s'.  Server: %s; Username: '%s'; Password: %s(using).", (*service)->key, (*service)->uri, (*service)->user_name, (DOMAIN ? (*service)->password :"[*passwords are hidden outside of debug mode*]")  );
		debug("Passwords found: (=");
		for(p=passwords; p; p=p->next){
			data=(GnomeKeyringNetworkPasswordData *)p->data;
			debug("\t'%s'", (gchar *)data->password);
		}
		debug(")");
	}
	
	g_list_foreach(passwords, (GFunc)g_free, NULL);
	g_list_free(passwords);
	
	return TRUE;
}/*keyring_get_password(&service);*/

gboolean keyring_set_password(OnlineService *service){
	GnomeKeyringResult result;
	guint              id;
	
	if(! (result=gnome_keyring_set_network_password_sync(
								NULL,            /* Keyring */
								service->user_name,        /* User */
								DOMAIN,            /* Domain */
								service->uri,  /* Server */
								NULL,            /* Object */
								NULL,            /* Protocol */
								NULL,            /* Authentication Type */
								0,               /* Port */
								service->password,        /* Password */
							&id            /* Unique ID */
	))==GNOME_KEYRING_RESULT_OK){
		debug("Could not set password to keyring, result: %d->'%s'", result, account_gnome_keyring_result_to_string(result));
		return FALSE;
	}

	debug("Password found for OnlineService: '%s'.\n\t\tServer: %s; Username: '%s'; Password: %s.", service->key, service->uri, service->user_name, (DOMAIN ?service->password :"[*passwords are hidden outside of debug mode*]") );
	return TRUE;
}/*keyring_set_password(service);*/
