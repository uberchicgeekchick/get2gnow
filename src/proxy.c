/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the RPL
 *
 * For more information or to find the latest release, visit our
 * website at: http://uberChicGeekChick.Com/?projects=get2gnow
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
#define _GNU_SOURCE
#define _THREAD_SAFE



/********************************************************
 *        Project headers, eg #include "config.h"       *
 ********************************************************/
#include "config.h"
#include "program.h"

#include "gconfig.h"

#include "online-services-typedefs.h"
#include "online-services.h"
#include "online-service.types.h"
#include "online-service.h"
#include "proxy.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define PROXY			"/system/http_proxy"
#define PROXY_USE		PROXY "/use_http_proxy"
#define PROXY_HOST		PROXY "/host"
#define PROXY_PORT		PROXY "/port"
#define PROXY_USE_AUTH		PROXY "/use_authentication"
#define PROXY_USER		PROXY "/authentication_user"
#define PROXY_PASS		PROXY "/authentication_password"

static PROXY_STATUS proxy_status=PROXY_STATUS_UNKNOWN;
static SoupURI *proxy_suri=NULL;
static gchar *proxy_uri=NULL;

#define	DEBUG_DOMAINS	"OnlineServices:Network:Requests:Authentication"
#include "debug.h"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
/**
 * Sets SoupUri *proxy_suri for use with a SoupConnection.
 *
 * @returns 1 if a proxy is used or -1 if not.
 */
gboolean proxy_init(void){
	if(proxy_status)
		switch(proxy_status){
			case PROXY_STATUS_DISABLED: 
				return FALSE;
			case PROXY_STATUS_ENABLED:
				return TRUE;
			case PROXY_STATUS_UNKNOWN:
			default: break;
		}
	
	if(!gconfig_if_bool(PROXY_USE, FALSE)){
		proxy_status=PROXY_STATUS_DISABLED;
		debug("Connect to the tubes using a proxy\t\t[%s]", _("disable"));
		return FALSE;
	}
	
	if(proxy_suri||proxy_uri) proxy_deinit();
	
	gchar *server=NULL;
	gint port;
	
	/* Get proxy */
	gconfig_get_string(PROXY_HOST, &server);
	gconfig_get_int(PROXY_PORT, &port);
	
	if(G_STR_EMPTY(server)){
		debug("Connect to the tubes using a proxy\t\t[%s]", _("disable"));
		debug("*NOTICE:* Proxy connection is enabled but no server is provided. Please check your puter's configuration.");
		proxy_status=PROXY_STATUS_DISABLED;
		return FALSE;
	}
	
	proxy_uri=NULL;
	
	if(!gconfig_if_bool(PROXY_USE_AUTH, FALSE))
		proxy_uri=g_strdup_printf("http://%s:%d", server, port);
	else {
		gchar *user, *password;
		gconfig_get_string(PROXY_USER, &user);
		gconfig_get_string(PROXY_PASS, &password);
		
		proxy_uri=g_strdup_printf( "http://%s:%s@%s:%d", user, password, server, port);
		
		g_free(user);
		g_free(password);
	}
	
	debug("Connect to the tubes using a proxy\t\t[%s]", _("enable"));
	debug("Proxy uri: <%s>.", proxy_uri );
	
	proxy_suri=soup_uri_new(proxy_uri);
	
	g_free(server);
	
	proxy_status=PROXY_STATUS_ENABLED;
	return TRUE;
}/*proxy_init();*/


gboolean proxy_attach_online_service(OnlineService *service){
	if(proxy_status==PROXY_STATUS_UNKNOWN) proxy_init();
	
	if(proxy_status==PROXY_STATUS_DISABLED) return FALSE;
	
	debug("Piping OnlineService: <%s> to the tubes through this proxy: <%s>", service->key, proxy_uri);
	g_object_set( G_OBJECT(service->session), SOUP_SESSION_PROXY_URI, proxy_suri, NULL);
	return TRUE;
}/*proxy_attach_online_service*/

void proxy_deinit(void){
	if(proxy_status==PROXY_STATUS_ENABLED){
		debug("**SHUTDOWN:** disconnecting from proxy: <%s>", proxy_uri);
		soup_uri_free(proxy_suri);
		proxy_suri=NULL;
		uber_free(proxy_uri);
	}
	proxy_status=PROXY_STATUS_UNKNOWN;
}/*proxy_deinit();*/

/********************************************************
 *                       eof                            *
 ********************************************************/
