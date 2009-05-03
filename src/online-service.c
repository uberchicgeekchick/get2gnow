/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright(c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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
 * License("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
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
 * user-visible display as defined in Section 6.4(d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright(C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright(C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright(C) of the User the data is connected to.
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
#include "gconfig.h"
#include "network.h"
#include "keyring.h"
#include "debug.h"
#include "gtkbuilder.h"
#include "preferences.h"
#include "services-dialog.h"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static void online_service_connect(OnlineService *service);
static gboolean online_service_login(OnlineService *service);
static void online_service_reconnect(OnlineService *service);
static void online_service_logout(OnlineService *service);

static void online_service_login_success(SoupSession *session, SoupMessage *msg, gpointer user_data);
static void online_service_http_authenticate(SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer user_data);

static void online_services_proxy_release(void);
static int online_services_proxy_load(void);
static gboolean online_services_proxy_init(SoupSession *connection);

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
static int proxy_init=0;
static SoupURI *proxy_suri=NULL;
OnlineServices *online_services=NULL;
OnlineService *current_service=NULL;
#define DEBUG_DOMAIN "OnlineServices"


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
OnlineServices *online_services_init(void){
	GSList		*k=NULL;
	OnlineService	*service=NULL;
	gboolean	found_active_account=FALSE;
	
	OnlineServices *services=g_new0(OnlineServices,1);
	
	gconfig_get_list_string(PREFS_AUTH_ACCOUNTS, &services->keys);
	
	for( k=services->keys; k; k=k->next ){
		services->accounts=g_list_append(services->accounts, online_service_load( (const gchar *)k->data ) );
		service=(OnlineService *)services->accounts->data;

		if(!service->enabled){
			debug(DEBUG_DOMAIN, "%s account not enabled.", service->key);
			continue;
		}
		
		if(G_STR_EMPTY(service->username) || G_STR_EMPTY(service->password)){
			debug(DEBUG_DOMAIN, "%s account is missing its username (=%s) and/or password (=%s).", service->key, service->username, service->password);
			continue;
		}
		
		if(!service->auto_connect){
			debug(DEBUG_DOMAIN, "%s account in not set to auto-connect.", service->key);
			continue;
		}
		
		if(!found_active_account)
			found_active_account=TRUE;
		if(!current_service){
			debug(DEBUG_DOMAIN, "Selecting %s as the default & current accout to start with.", service->key);
			current_service=service;
		}
		
		online_service_connect(service);
	}
	
	if(!found_active_account){
		debug(DEBUG_DOMAIN, "No accounts are enabled or setup.  Loading accounts dialog." );
		services_dialog_show(NULL);
	}
	
	debug(DEBUG_DOMAIN, "Accounts loaded.");
	return services;
}//online_services_init

/* Login to services. */
gboolean online_services_login(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	gboolean	login_okay=FALSE;
	
	for(a=online_services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		if(service->enabled){
			online_service_login(service);
			if(!login_okay) login_okay=TRUE;
		}
	}
	return login_okay;
}//online_services_login

void online_services_request(OnlineServices *services, RequestMethod request, const gchar *resource, SoupSessionCallback callback, gpointer data, gpointer formdata){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		if(service->enabled)
			online_service_request(service, request, resource, callback, data, formdata);
	}
}//online_services_request



/*
 * 	Methods for use with an OnlineService instance.
 */
OnlineService *online_service_load(const gchar *account_key){
	debug(DEBUG_DOMAIN, "Loading account: '%s'.", account_key);
	gchar **account_data=g_strsplit(account_key, "@", 3);
	OnlineService *service=online_service_open((const gchar *)account_data[0],(const gchar *)account_data[1]);
	g_strfreev(account_data);
	debug(DEBUG_DOMAIN, "Account loaded: %s.", account_key);
	return service;
}//online_service_load

OnlineService *online_service_open(const gchar *url, const gchar *username){
	debug(DEBUG_DOMAIN, "Loading OnlineService instance for: '%s@%s' account.", username, url );
	OnlineService *service=g_new0(OnlineService, 1);
	
	service->timer=timer_new();
	
	service->key=g_strdup_printf("%s@%s", username, url );
	service->url=g_strdup(url);
	service->username=g_strdup(username);
	
	gchar *prefs_auth_path=NULL;
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_ENABLED, service->username, service->url);
	gconfig_get_bool(prefs_auth_path, &service->enabled);
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_URL, service->username, service->url);
	gconfig_get_string(prefs_auth_path, &service->url);
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_USERNAME, service->username, service->url);
	gconfig_get_string(prefs_auth_path, &service->username);
	g_free(prefs_auth_path);
	
	service->password=NULL;
	
#ifdef HAVE_GNOME_KEYRING
	if(G_STR_EMPTY(service->username))
		service->password=NULL;
	else if(!(keyring_get_password(service->key, &service->password)))
		service->password=NULL;
#else
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_PASSWORD, service->username, service->url);
	gconfig_get_string(prefs_auth_path, &service->password);
	g_free(prefs_auth_path);
#endif
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_AUTO_CONNECT, service->username, service->url);
	gconfig_get_bool(prefs_auth_path, &service->auto_connect);
	g_free(prefs_auth_path);
	
	debug(DEBUG_DOMAIN, "OnlineService instance '%s' created.\n\t account\t\t\t[%sabled]\n\tservice url: %s; username: %s; password: %s; auto_connect: %s", service->key, (service->enabled?"en":"dis"), service->url, service->username, service->password, (service->auto_connect?"TRUE":"FALSE"));
	
	return service;
}//online_service_new

gboolean online_service_save(OnlineService *service, gboolean enabled, const gchar *url, const gchar *username, const gchar *password, gboolean auto_connect){
	if( G_STR_EMPTY(url) || G_STR_EMPTY(username) )
		return FALSE;
	
	if(!service)
		service=g_new0(OnlineService, 1);
	else if(!( g_str_equal(service->url, url) && g_str_equal(service->username, username) )){
		gconfig_rm_rf(service->key);
		online_service_free(service);
		service=g_new0(OnlineService, 1);
	}
	
	service->key=g_strdup_printf("%s@%s", username, url);
	service->enabled=enabled;
	service->url=g_strdup(url);
	service->password=g_strdup(password);
	service->username=g_strdup(username);
	service->auto_connect=auto_connect;
	
	gchar *prefs_auth_path=NULL;
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_ENABLED, service->username, service->url);
	gconfig_set_bool(prefs_auth_path, service->enabled);
	g_free(prefs_auth_path);
		
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_URL, service->username, service->url);
	gconfig_set_string(prefs_auth_path, service->url);
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_USERNAME, service->username, service->url);
	gconfig_set_string(prefs_auth_path, service->username);
	g_free(prefs_auth_path);
	
#ifdef HAVE_GNOME_KEYRING
	keyring_set_password( service->key, service->password);
#else
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_PASSWORD, service->username, service->url);
	gconfig_set_string(prefs_auth_path, service->password);
	g_free(prefs_auth_path);
#endif
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_AUTO_CONNECT, service->username, service->url);
	gconfig_set_bool(prefs_auth_path, service->auto_connect);
	g_free(prefs_auth_path);
	
	online_service_reconnect(service);
	
	debug(DEBUG_DOMAIN, "%s@%s account saved\t\t[%sabled]\n\tusername: %s; password: %s; auto_connect: %s", service->username, service->url,(service->enabled?"en":"dis"), service->username, service->password,(service->auto_connect?"TRUE":"FALSE"));
	
	return TRUE;
}//online_service_save

static void online_service_connect(OnlineService *service){
	debug(DEBUG_DOMAIN, "Creating %s's connection to %s", service->key, service->url);
	service->connection=soup_session_async_new_with_options(SOUP_SESSION_MAX_CONNS, 8, NULL);
	online_services_proxy_init(service->connection);
}//online_service_connect

static void online_service_reconnect(OnlineService *service){
	online_service_logout(service);
	online_service_connect(service);
	online_service_login(service);
}//online_service_reconnect

static void online_service_logout(OnlineService *service){
	debug(DEBUG_DOMAIN, "Closing %s's connection to: %s", service->key, service->url);
	soup_session_abort(service->connection);
	g_object_unref(service->connection);
	service->connection=NULL;
}//online_service_logout

/* Login to service. */
gboolean online_service_login(OnlineService *service){
	debug(DEBUG_DOMAIN, "Attempting to log in to %s...", service->key);
	if(!service->enabled) return FALSE;
	//current_service=service;
	debug(DEBUG_DOMAIN, "Logging on to %s...", service->key);
	
	app_statusbar_printf("Connecting to %s...", service->key);
	
	debug(DEBUG_DOMAIN, "Authenticating https://%s:%s@%s/", service->username, service->password, service->url);
	/* HTTP Basic Authentication */
	g_signal_connect(service->connection, "authenticate", G_CALLBACK(online_service_http_authenticate), service);
	
	/* Verify cedentials */
	online_service_request(service, QUEUE, API_LOGIN, online_service_login_success, service, NULL);
	
	return TRUE;
}//online_service_login

/* On verify credentials */
static void online_service_login_success(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineService *service=(OnlineService *)user_data;
	debug(DEBUG_DOMAIN, "Login response: %i",msg->status_code);
	
	timer_main(service->timer, msg);
	if(network_check_http( msg )) {
		debug(DEBUG_DOMAIN, "Loading default timeline");
		app_state_on_connection(TRUE);
		return;
	}
	
	timer_free(service->timer);
	app_state_on_connection(FALSE);
}

/* HTTP Basic Authentication */
static void online_service_http_authenticate(SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer user_data){
	OnlineService *service=(OnlineService *)user_data;
	if(!service->enabled) return;
	/* Don't bother to continue if there is no user_id */
	if(G_STR_EMPTY(service->username))
		return debug(DEBUG_DOMAIN, "Could not authenticate: %s, unknown username.", service->key);
	
	/* verify that the password has been set */
	if(G_STR_EMPTY(service->password))
		return debug(DEBUG_DOMAIN, "Authentication error: unknown password.");
	
	soup_auth_authenticate(auth, service->username, service->password);
}//online_service_http_authenticate


SoupMessage *online_service_request(OnlineService *service, RequestMethod request, const gchar *resource, SoupSessionCallback callback, gpointer data, gpointer formdata){
	gchar *new_uri=g_strdup_printf("https://%s%s", service->url, resource );
	SoupMessage *msg=online_service_request_url(service, request, (const gchar *)new_uri, callback, data, formdata);
	g_free(new_uri);
	return msg;
}//online_service_request

SoupMessage *online_service_request_url(OnlineService *service, RequestMethod request, const gchar *resource, SoupSessionCallback callback, gpointer data, gpointer formdata){
	SoupMessage *msg=NULL;
	
	switch(request){
		case GET:
		case QUEUE:
			debug(DEBUG_DOMAIN, "GET: %s", resource);
			msg=soup_message_new("GET", resource);
			break;
		case POST:
			debug(DEBUG_DOMAIN, "POST: %s", resource);
			msg=soup_message_new("POST", resource);
	
			soup_message_headers_append( msg->request_headers, "X-Twitter-Client", PACKAGE_NAME);
			soup_message_headers_append( msg->request_headers, "X-Twitter-Client-Version", PACKAGE_VERSION);
	
			if((gchar *)formdata)
				soup_message_set_request(
					msg,
					"application/x-www-form-urlencoded",
					SOUP_MEMORY_TAKE,
					(gchar *)formdata,
					strlen( formdata )
				);
			break;
	}
	
	switch(request){
		case QUEUE:
		case POST:
			soup_session_queue_message(service->connection, msg, callback, data);
			return NULL;
		case GET:
			soup_session_send_message(service->connection, msg);
			if(network_check_http(msg))
				return msg;
			return NULL;
	}
	return msg;
}//online_service_request_url

void online_service_free(OnlineService *service){
	if(!service) return;
	
	debug(DEBUG_DOMAIN, "Unloading instance of: %s service", service->url);
	online_service_logout(service);
	
	debug(DEBUG_DOMAIN, "Closing %s account", service->key );
	g_free(service->key);
	g_free(service->url);
	g_free(service->username);
	g_free(service->password);
	
	g_free(service);
	service=NULL;
}//online_service_free

void online_services_deinit(OnlineServices *services){
	online_services_proxy_release();
	
	user_free_lists();
	
	current_service=NULL;
	
	GList *a=NULL;
	for(a=services->accounts; a; a=a->next)
		online_service_free( (OnlineService *)a->data );
	
	g_slist_free(services->keys);
	services->keys=NULL;
	
	g_free(services);
	services=NULL;
}//online_services_deinit

static gboolean online_services_proxy_init(SoupSession *connection){
	if(!proxy_init)
		proxy_init=online_services_proxy_load();
	
	if(proxy_init==-1)
		return FALSE;
	
	g_object_set( G_OBJECT(connection), SOUP_SESSION_PROXY_URI, proxy_suri, NULL);
	return TRUE;
}//online_services_proxy_init

/**
 *  * Sets SoupUri *proxy_suri for use with a SoupConnection.
 *   *
 *    * @returns 1 if a proxy is used or -1 if not.
 *     */
static int online_services_proxy_load(void){
	if(proxy_init) return proxy_init;
	
	/* Set the proxy, if configuration is set */
	if(!gconfig_if_bool(PROXY_USE))
		return -1;
	
	proxy_init=1;
	if(proxy_suri)
		online_services_proxy_release();
	
	gchar *server=NULL;
	gint port;
	
	/* Get proxy */
	gconfig_get_string(PROXY_HOST, &server);
	gconfig_get_int(PROXY_PORT, &port);
	
	if(G_STR_EMPTY(server))
		return -1;
	
	gchar *proxy_uri=NULL;
	
	/* Check proxy auth data */
	if(!gconfig_if_bool(PROXY_USE_AUTH))
		proxy_uri=g_strdup_printf("http://%s:%d", server, port);
	else {
		char *user, *password;
		gconfig_get_string(PROXY_USER, &user);
		gconfig_get_string(PROXY_PASS, &password);
		
		proxy_uri=g_strdup_printf( "http://%s:%s@%s:%d", user, password, server, port);
		
		g_free(user);
		g_free(password);
	}
	
	debug( DEBUG_DOMAIN, "Proxy uri: %s", proxy_uri );
	
	/* Setup proxy info */
	proxy_suri=soup_uri_new(proxy_uri);
	
	g_free(server);
	g_free(proxy_uri);
	
	return 1;
}//online_services_proxy_load

static void online_services_proxy_release(void){
	soup_uri_free(proxy_suri);
	proxy_suri=NULL;
	proxy_init=0;
}//online_services_proxy_release

/********************************************************
 *                       eof                            *
 ********************************************************/

