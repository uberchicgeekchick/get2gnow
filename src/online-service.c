/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * {project} is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the RPL
 *
 * For more information or to find the latest release, visit our
 * website at: http://uberChicGeekChick.Com/?projects={project}
 *
 * Writen by an uberChick, other uberChicks please meet me & others @:
 * 	http://uberChicks.Net/
 *
 * I'm also disabled. I live with a progressive neuro-muscular disease.
 * DYT1+ Early-Onset Generalized Dystonia, a type of Generalized Dystonia.
 * 	http://Dystonia-DREAMS.Org/
 *
 *
 *
 * Unless explicitly acquired and licensed from Licensor under another
 * license, the contents of this file are subject to the Reciprocal Public
 * License ("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
 * and You may not copy or use this file in either source code or executable
 * form, except in compliance with the terms and conditions of the RPL.
 *
 * All software distributed under the RPL is provided strictly on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, AND
 * LICENSOR HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, QUIET ENJOYMENT, OR NON-INFRINGEMENT. See the RPL for specific
 * language governing rights and limitations under the RPL.
 *
 * The User-Visible Attribution Notice below, when provided, must appear in each
 * user-visible display as defined in Section 6.4 (d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright (C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright (C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright (C) of the User the data is connected to.
 * Users may lisences their data under the terms of an OSI approved or Creative Commons
 * license.  Users must be allowed to select their choice of license for each piece of data
 * on an individual bases and cannot be blanketly applied to all of the Users.  The User may
 * select a default license for their data.  All of the Software's data pertaining to each
 * User must be fully accessible, exportable, and deletable to that User.
 */

/********************************************************
 *          My art, code, & programming.                *
 ********************************************************/



/********************************************************
 *        Project headers, eg #include "config.h"       *
 ********************************************************/
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <openssl/hmac.h>
#include <libxml/parser.h>

#include "config.h"
#include "online-service.h"

#include "app.h"
#include "main.h"
#include "gconf.h"
#include "network.h"
#include "keyring.h"
#include "debug.h"
#include "gtkbuilder.h"
#include "accounts-dialog.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define DEBUG_DOMAIN "OnlineService"

/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
OnlineService **online_service_init(gint *accounts){
	OnlineService **services=g_malloc(sizeof(OnlineService)*2);
	services[0]=online_service_new("twitter.com");
	services[1]=online_service_new("itdenti.ca");
	debug(DEBUG_DOMAIN, "Loading accounts.");

	for(int i=0; i<2; i++){
		if(!services[i]->enabled){
			debug(DEBUG_DOMAIN, "%s account not enabled.", services[i]->auth_uri);
			continue;
		}
		
		if(G_STR_EMPTY(services[i]->username) || G_STR_EMPTY(services[i]->password)){
			debug(DEBUG_DOMAIN, "Loading account dialog for inital account setup.");
			continue;
		}
		
		if(!services[i]->auto_connect){
			debug(DEBUG_DOMAIN, "%s account in not set to auto-connect.", services[i]->auth_uri);
			continue;
		}
		*accounts=*accounts+1;
	}
	
	if(!*accounts){
		debug(DEBUG_DOMAIN, "No accounts are enabled or setup.  Loading accounts dialog." );
		accounts_dialog_show(GTK_WINDOW(app_get_window()));
	}
	
	debug(DEBUG_DOMAIN, "Accounts loaded.");
	return services;
}//online_service_init

OnlineService *online_service_new(const gchar *auth_uri){
	debug(DEBUG_DOMAIN, "Loading account: %s", auth_uri );
	Conf *conf=conf_get();
	OnlineService *service=g_new0(OnlineService, 1);
	service->auth_uri=g_strdup(auth_uri);
	
	gchar *prefs_auth_path=NULL;
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_ENABLED, service->auth_uri);
	conf_get_bool(conf, prefs_auth_path, &service->enabled);
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_AUTO_CONNECT, service->auth_uri);
	conf_get_bool(conf, prefs_auth_path, &service->auto_connect);
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_USERNAME, service->auth_uri);
	conf_get_string(conf, prefs_auth_path, &service->username);
	g_free(prefs_auth_path);
	
#ifdef HAVE_GNOME_KEYRING
	service->password=NULL;
	gchar *keyring_key=g_strdup_printf("%s@%s", service->username, service->auth_uri );
	if(G_STR_EMPTY(service->username))
		service->password=NULL;
	else if(!(keyring_get_password(keyring_key, &service->password)))
		service->password=NULL;
	g_free(keyring_key);
#else
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_PASSWORD, service->auth_uri);
	conf_get_string(conf, prefs_auth_path, &service->password);
	g_free(prefs_auth_path);
#endif
	
	debug( DEBUG_DOMAIN, "%s account loaded\t\t[%sabled]\n\tusername: %s; password: %s; auto_connect: %s", service->auth_uri, (service->enabled?"en":"dis"), service->username, service->password, (service->auto_connect?"TRUE":"FALSE") );
	
	return service;
}//online_service_new

void online_service_save(OnlineService *service, gboolean enabled, const gchar *username, const gchar *password, gboolean auto_connect){
	Conf *conf=conf_get();
	gchar *prefs_auth_path=NULL;
	
	service->enabled=enabled;
	service->auto_connect=auto_connect;
	service->username=g_strdup(username);
	service->password=g_strdup(password);
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_ENABLED, service->auth_uri);
	conf_set_bool(conf, prefs_auth_path, service->enabled);
	g_free(prefs_auth_path);
				
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_AUTO_CONNECT, service->auth_uri);
	conf_set_bool(conf, prefs_auth_path, service->auto_connect);
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_USERNAME, service->auth_uri);
	conf_set_string(conf, prefs_auth_path, service->username);
	g_free(prefs_auth_path);

	debug( DEBUG_DOMAIN, "%s account saved\t\t[%sabled]\n\tusername: %s; password: %s; auto_connect: %s", service->auth_uri, (service->enabled?"en":"dis"), service->username, service->password, (service->auto_connect?"TRUE":"FALSE") );

#ifdef HAVE_GNOME_KEYRING
	gchar *keyring_key=g_strdup_printf("%s@%s", service->username, service->auth_uri );
	keyring_set_password( keyring_key, service->password);
	g_free(keyring_key);
#else
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_PASSWORD, service->auth_uri);
	conf_set_string(conf, prefs_auth_path, service->password);
	g_free(prefs_auth_path);
#endif
}//online_service_save


void online_service_deinit(OnlineService *service){
	if(!service) return;
	
	debug(DEBUG_DOMAIN, "Unloading instance of: %s service", service->auth_uri );
	service->connection=NULL;
	
	debug(DEBUG_DOMAIN, "Closing %s account", service->auth_uri );
	g_free(service->auth_uri);
	g_free(service->username);
	g_free(service->password);
	
	g_free(service);
	service=NULL;
}//online_service_deinit
/********************************************************
 *                       eof                            *
 ********************************************************/

