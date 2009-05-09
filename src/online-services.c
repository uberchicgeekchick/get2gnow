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
#include <libsoup/soup.h>
#include <openssl/hmac.h>
#include <libxml/parser.h>

#include "config.h"
#include "online-services.h"

#include "app.h"
#include "main.h"
#include "gconfig.h"
#include "network.h"
#include "keyring.h"
#include "debug.h"
#include "tweets.h"
#include "gtkbuilder.h"
#include "preferences.h"
#include "services-dialog.h"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static void online_services_reconnect(OnlineServices *services);
static gboolean online_service_connect(OnlineService *service);
static gboolean online_service_login(OnlineService *service);
static gboolean online_service_reconnect(OnlineService *service);
static void online_service_disconnect(OnlineService *service);

static OnlineService *online_service_open(const gchar *username, const gchar *url);
static gboolean online_service_save(OnlineService *service, gboolean enabled, const gchar *url, const gchar *username, const gchar *password, gboolean auto_connect);

static void online_service_http_authenticate(SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer user_data);
static void online_service_login_success(SoupSession *session, SoupMessage *msg, gpointer user_data);

static void online_services_proxy_release(void);
static int online_services_proxy_load(void);
static gboolean online_service_proxy_init(OnlineService *service);


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
static int proxy_init=0;
static SoupURI *proxy_suri=NULL;
static OnlineServices *services=NULL;
OnlineServices *online_services=NULL;
OnlineService *current_service=NULL;
#define DEBUG_DOMAIN "OnlineServices"


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
OnlineServices *online_services_init(void){
	GSList		*k=NULL;
	OnlineService	*service=NULL;
	gchar	*account_key=NULL;
	
	services=g_new0(OnlineServices,1);
	services->total=0;
	
	gconfig_get_list_string(PREFS_AUTH_SERVICES, &services->keys);

	for( k=services->keys; k; k=k->next ){
		account_key=(gchar *)k->data;
		debug(DEBUG_DOMAIN, "Loading  '%s' account.", account_key);
		services->accounts=g_list_append(services->accounts, (online_service_load( (const gchar *)account_key )) );
		services->accounts=g_list_last(services->accounts);
		service=(OnlineService *)services->accounts->data;

		debug(DEBUG_DOMAIN, "Loaded account: '%s'.  Validating & connecting.", service->decoded_key);
		if(!service->enabled){
			debug(DEBUG_DOMAIN, "%s account not enabled.", service->decoded_key);
			continue;
		}
		
		if(G_STR_EMPTY(service->username) || G_STR_EMPTY(service->password)){
			debug(DEBUG_DOMAIN, "%s account is missing its username (=%s) and/or password (=%s).", service->decoded_key, service->username, service->password);
			continue;
		}
		
		if(!service->auto_connect){
			debug(DEBUG_DOMAIN, "%s account in not set to auto-connect.", service->decoded_key);
			continue;
		}
		
		if(!current_service){
			debug(DEBUG_DOMAIN, "Selecting %s as the default & current accout to start with.", service->decoded_key);
			current_service=service;
		}
		
		services->total++;
		
		if(online_service_connect(service))
			//if(online_service_login(service))
				services->connected++;
	}
	
	services->accounts=g_list_first(services->accounts);
	
	online_services=services;
	
	if(!services->total){
		debug(DEBUG_DOMAIN, "No accounts are enabled or setup.  New accounts will be setup when 'online_services_login' is called." );
		services_dialog_show(NULL);
	}else
		debug(DEBUG_DOMAIN, "%d services found and loaded.", services->total);
	
	return services;
}//online_services_init

/* Login to services. */
gboolean online_services_login(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	gboolean	login_okay=FALSE;
	
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		if(service->enabled){
			online_service_login(service);
			if(!login_okay) login_okay=TRUE;
		}
	}
	app_state_on_connection(login_okay);
	return login_okay;
}//online_services_login

/* Login to services. */
static void online_services_reconnect(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		if(service->enabled)
			online_service_reconnect(service);
	}
}//online_services_reconnect

gboolean online_services_save(OnlineServices *services, OnlineService *service, gboolean enabled, const gchar *url, const gchar *username, const gchar *password, gboolean auto_connect){
	if(service){
		debug(DEBUG_DOMAIN, "Saving existing service: '%s'.", service->decoded_key);
		if( online_service_save(service, enabled, url, username, password, auto_connect) )
			return online_service_reconnect(service);
	}
	
	debug(DEBUG_DOMAIN, "Creating & saving new service: '%s@%s'.", username, url);
	if(!( online_service_save(service, enabled, url, username, password, auto_connect) )){
		debug(DEBUG_DOMAIN, "**ERROR**: Failed saving new service: '%s@%s'.", username, url);
		return FALSE;
	}
	
	debug(DEBUG_DOMAIN, "Adding new service: '%s' to online service.", service->decoded_key);
	if(!( (services->accounts=g_list_append(services->accounts, service)) )){
		debug(DEBUG_DOMAIN, "**ERROR**: Failed to add: '%s', to OnlineServices' accounts.", service->decoded_key);
		return FALSE;
	}
	
	services->total++;
	debug(DEBUG_DOMAIN, "New service: '%s' saved.  OnlineServices total: %d.", service->decoded_key, services->total);
	
	debug(DEBUG_DOMAIN, "Retrieving new service: '%s' from OnlineServices accounts.", service->decoded_key);
	service=g_list_nth_data(services->accounts, services->total);
	debug(DEBUG_DOMAIN, "OnlineService: '%s' reloaded from OnlineServices accounts.", service->decoded_key);
	
	debug(DEBUG_DOMAIN, "Adding '%s' to OnlineServices keys.", service->decoded_key);
	if(!( (services->keys=g_slist_append(services->keys, service->key)) )){
		debug(DEBUG_DOMAIN, "**ERROR**: Failed to append new service's key: '%s', to OnlineServices' keys.", service->decoded_key);
		return FALSE;
	}
	
	debug(DEBUG_DOMAIN, "Saving accounts & services list: '%s'.", PREFS_AUTH_SERVICES);
	if(!( (gconfig_set_list_string(PREFS_AUTH_SERVICES, services->keys)) )){
		debug(DEBUG_DOMAIN, "**ERROR**: Failed to save new service: '%s', couldn't save gconf's services list.", service->decoded_key);
		return FALSE;
	}
	
	debug(DEBUG_DOMAIN, "Saving accounts & services successful.  Connecting to: '%s'\t\t[", service->decoded_key);
	if(online_service_connect(service)){
		services->connected++;
		debug(DEBUG_DOMAIN, "success");
	}else
		debug(DEBUG_DOMAIN, "failed");
	
	debug(DEBUG_DOMAIN, "Saving '%s' service complete.  Total services: %d; Total connected: %d", service->decoded_key, services->total, services->connected);
	
	return TRUE;
	online_services_reconnect(services);
}//online_services_save

void online_services_request(OnlineServices *services, RequestMethod request, const gchar *resource, SoupSessionCallback callback, gpointer user_data, gpointer formdata){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		if(service->connected)
			online_service_request(service, request, resource, callback, user_data, formdata);
	}
}//online_services_request

gboolean online_services_fill_liststore(OnlineServices *services, GtkListStore *liststore){
	if(!online_services->total){
		debug(DEBUG_DOMAIN, "No services found to load, new accounts need to be setup.");
		return FALSE;
	}
	
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	GList			*a=NULL;
	guint			services_loaded=0;
	debug(DEBUG_DOMAIN, "Loading services into tree view. total services: '%d'.", online_services->total);
	for(a=services->accounts; a; a=a->next){
		OnlineService *service=(OnlineService *)a->data;
		gchar **account_data=g_strsplit(service->key, "@", 3);
		debug(DEBUG_DOMAIN, "Appending account: '%s'; server: %s.", service->decoded_key, account_data[1]);
		gtk_list_store_append(liststore, iter);
		gtk_list_store_set(
					liststore, iter,
						UrlString, account_data[1],
						OnlineServicePointer, service,
					-1
		);
		g_strfreev(account_data);
		services_loaded++;
	}
	return (services_loaded ?TRUE :FALSE );
}//online_services_fill_liststore


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

static OnlineService *online_service_open(const gchar *username, const gchar *url){
	debug(DEBUG_DOMAIN, "Loading OnlineService instance for: '%s@%s' account.", username, url );
	OnlineService *service=g_new0(OnlineService, 1);
	
	service->connected=FALSE;
	service->session=NULL;
	service->logins=0;
	service->timer=timer_new();
	
	gchar *encoded_username=g_uri_escape_string(username, NULL, TRUE);
	gchar *encoded_url=g_uri_escape_string(url, NULL, TRUE);
	service->key=g_strdup_printf("%s@%s", encoded_username, encoded_url );
	g_free(encoded_username);
	g_free(encoded_url);
	
	service->decoded_key=g_strdup_printf("%s@%s", username, url );
	service->url=g_strdup(url);
	service->username=g_strdup(username);
	
	gchar *prefs_auth_path=NULL;
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_PREFIX, service->key);
	debug(DEBUG_DOMAIN, "Loading OnlineService settings for: '%s@%s'\n\t\tAccount's gconf path: %s", service->username, service->url, prefs_auth_path );
	g_free(prefs_auth_path);

	prefs_auth_path=g_strdup_printf(PREFS_AUTH_ENABLED, service->key);
	gconfig_get_bool(prefs_auth_path, &service->enabled);
	g_free(prefs_auth_path);
	debug(DEBUG_DOMAIN, "OnlineService for: '%s@%s' is %sabled.", service->username, service->url, (service->enabled ?"en" :"dis") );
	
	service->password=NULL;
	
#ifdef HAVE_GNOME_KEYRING
	if(G_STR_EMPTY(service->username))
		service->password=NULL;
	else if(!(keyring_get_password(service->key, &service->password)))
		service->password=NULL;
#else
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_PASSWORD, service->key);
	gconfig_get_string(prefs_auth_path, &service->password);
	g_free(prefs_auth_path);
#endif
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_AUTO_CONNECT, service->key);
	gconfig_get_bool(prefs_auth_path, &service->auto_connect);
	g_free(prefs_auth_path);
	
	debug(DEBUG_DOMAIN, "OnlineService instance created.\n\t\taccount '%s(=%s)'\t\t\t[%sabled]\n\t\tservice url: %s; username: %s; password: %s; auto_connect: %s", service->decoded_key, service->key, (service->enabled?"en":"dis"), service->url, service->username, service->password, (service->auto_connect?"TRUE":"FALSE"));
	
	return service;
}//online_service_open

gboolean online_service_save(OnlineService *service, gboolean enabled, const gchar *url, const gchar *username, const gchar *password, gboolean auto_connect){
	if( G_STR_EMPTY(url) || G_STR_EMPTY(username) )
		return FALSE;
	
	if(!service){
		debug(DEBUG_DOMAIN, "Creating new OnlineService for '%s@%s'.", username, url);
		service=g_new0(OnlineService, 1);
		service->session=NULL;
	}else if(!( g_str_equal(service->url, url) && g_str_equal(service->username, username) )){
		debug(DEBUG_DOMAIN, "Removing old OnlineService: '%s'.");
		gconfig_rm_rf(service->key);
		online_service_free(service);
		debug(DEBUG_DOMAIN, "Preparing to save OnlineService for '%s@%s'.", username, url);
		service=g_new0(OnlineService, 1);
		service->session=NULL;
	}
	
	service->connected=FALSE;
	service->session=NULL;
	service->enabled=enabled;
	service->logins=0;
	service->timer=timer_new();
	service->auto_connect=auto_connect;
	
	gchar *encoded_username=g_uri_escape_string(username, NULL, TRUE);
	gchar *encoded_url=g_uri_escape_string(url, NULL, TRUE);
	service->key=g_strdup_printf("%s@%s", encoded_username, encoded_url );
	g_free(encoded_username);
	g_free(encoded_url);
	
	service->decoded_key=g_strdup_printf("%s@%s", username, url );
	service->url=g_strdup(url);
	service->username=g_strdup(username);
	service->password=g_strdup(password);
	
	gchar *prefs_auth_path=NULL;
	
	debug(DEBUG_DOMAIN, "Saving '%s' 'enabled' status: %s.", service->decoded_key, (service->enabled ?"TRUE" :"FALSE" ) );
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_ENABLED, service->key);
	gconfig_set_bool(prefs_auth_path, service->enabled);
	g_free(prefs_auth_path);
	
	debug(DEBUG_DOMAIN, "Saving '%s' password: %s.", service->decoded_key, service->password );
#ifdef HAVE_GNOME_KEYRING
	keyring_set_password( service->key, service->password);
#else
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_PASSWORD, service->key);
	gconfig_set_string(prefs_auth_path, service->password);
	g_free(prefs_auth_path);
#endif
	
	debug(DEBUG_DOMAIN, "Saving '%s' 'auto_connect' status: %s.", service->decoded_key, (service->auto_connect ?"TRUE" :"FALSE" ) );
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_AUTO_CONNECT, service->key);
	gconfig_set_bool(prefs_auth_path, service->auto_connect);
	g_free(prefs_auth_path);
	
	debug(DEBUG_DOMAIN, "OnlineService saved.\n\t\taccount '%s(=%s)'\t\t\t[%sabled]\n\t\tservice url: %s; username: %s; password: %s; auto_connect: [%s]", service->decoded_key, service->key, (service->enabled?"en":"dis"), service->url, service->username, service->password, (service->auto_connect?"TRUE":"FALSE"));
	
	return TRUE;
}//online_service_save

guint online_services_count_connections(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	services->connected=0;
	
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		if(service->connected)
			services->connected++;
	}
	return services->connected;
}//online_services_count_connections

OnlineService *online_services_get_first_connected(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	for(a=g_list_first(services->accounts); a; a=a->next){
		service=(OnlineService *)a->data;
		if(service->connected)
			break;
	}
	return service;
}//online_services_get_first_connected

OnlineService *online_services_get_last_connected(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	for(a=g_list_last(services->accounts); a; a=a->prev){
		service=(OnlineService *)a->data;
		if(service->connected)
			break;
	}
	return service;
}//online_services_get_last_connected


static gboolean online_service_connect(OnlineService *service){
	debug(DEBUG_DOMAIN, "Creating '%s' connection to '%s'", service->decoded_key, service->url);
	if(!( (service->session=soup_session_sync_new_with_options(SOUP_SESSION_MAX_CONNS_PER_HOST, 8, SOUP_SESSION_TIMEOUT, 20, SOUP_SESSION_IDLE_TIMEOUT, 20, NULL)) )){
		debug(DEBUG_DOMAIN, "**ERROR:** Failed to creating a new soup session for '%s'.", service->decoded_key);
		service->session=NULL;
		return (service->connected=FALSE);
	}
	
	/* HTTP Basic Authentication */
	debug(DEBUG_DOMAIN, "Adding authenticating callback for: '%s'. username: %s, password: %s", service->decoded_key, service->username, service->password);
	g_signal_connect(service->session, "authenticate", G_CALLBACK(online_service_http_authenticate), service);
	
#ifdef GNOME_ENABLE_DEBUG
	
	SoupLogger *logger=soup_logger_new(SOUP_LOGGER_LOG_MINIMAL, -1);
	soup_session_add_feature(service->session, SOUP_SESSION_FEATURE(logger));
	g_object_unref(logger);
	
#else
	
	gchar *enable_logger=NULL;
	if( ((g_getenv("DEBUG"))) && (enable_logger=g_strrstr(g_getenv("DEBUG"), DEBUG_DOMAIN)) ){
		SoupLogger *logger=soup_logger_new(SOUP_LOGGER_LOG_MINIMAL, -1);
		soup_session_add_feature(service->session, SOUP_SESSION_FEATURE(logger));
		g_object_unref(logger);
	}
	
#endif	
	online_service_proxy_init(service);
	return (service->connected=TRUE);
}//online_service_connect

static gboolean online_service_reconnect(OnlineService *service){
	if(service->session)
		online_service_disconnect(service);
	
	if(online_service_connect(service))
		if(online_service_login(service))
			return TRUE;
	
	return FALSE;
}//online_service_reconnect

static void online_service_disconnect(OnlineService *service){
	debug(DEBUG_DOMAIN, "Closing %s's connection to: %s", service->decoded_key, service->url);
	if(service->session){
		if(SOUP_IS_SESSION(service->session))
			soup_session_abort(service->session);
		
		g_object_unref(service->session);
		service->session=NULL;
	}
}//online_service_disconnect

/* Login to service. */
gboolean online_service_login(OnlineService *service){
	debug(DEBUG_DOMAIN, "Attempting to log in to %s...", service->decoded_key);
	if(!service->enabled) return FALSE;
	
	debug(DEBUG_DOMAIN, "Logging on to %s...", service->decoded_key);
	app_statusbar_printf("Connecting to %s...", service->key);
	
	/* Verify cedentials */
	debug(DEBUG_DOMAIN, "Attempting log in for: '%s'. username: %s, password: %s", service->decoded_key, service->username, service->password);
	gchar *login_uri=g_strdup_printf("https://%s%s%s", service->url, ( (g_str_equal(service->url, "twitter.com")) ?"" :"/api" ), API_LOGIN);
	debug(DEBUG_DOMAIN, "Login request: GET: %s", login_uri);
	SoupMessage *msg=soup_message_new("GET", login_uri);
	soup_session_send_message(service->session, msg);
	g_free(login_uri);
	if(!network_check_http(service, msg))
		return FALSE;
	
	online_service_login_success(service->session, msg, service);
	return TRUE;
}//online_service_login

/* On verify credentials */
static void online_service_login_success(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineService *service=(OnlineService *)user_data;
	debug(DEBUG_DOMAIN, "Login response: %i",msg->status_code);
	
	/* if the check succeeds then it start the request limit timer:
	 * 	timer_main(service->timer, msg);
	 */
	if(network_check_http(service, msg)) {
		debug(DEBUG_DOMAIN, "Login to '%s' succeeded.", service->decoded_key);
		return;
	}
	
	debug(DEBUG_DOMAIN, "Login to '%s' failed, 'authenticate' signal triggered.", service->decoded_key);
}

/* HTTP Basic Authentication */
static void online_service_http_authenticate(SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer user_data){
	OnlineService *service=(OnlineService *)user_data;
	if(!service->enabled) return;
	/* Don't bother to continue if there is no user_id */
	SoupURI *suri=soup_message_get_uri(msg);
	char *uri=soup_uri_to_string(suri, FALSE);
	debug(DEBUG_DOMAIN, "Authenticating SoupUri: %s", uri);
	soup_uri_free(suri);
	free(uri);
	if(G_STR_EMPTY(service->username))
		return debug(DEBUG_DOMAIN, "Could not authenticate: %s, unknown username.", service->decoded_key);
	
	/* verify that the password has been set */
	if(G_STR_EMPTY(service->password))
		return debug(DEBUG_DOMAIN, "Could not authenticate: %s, unknown password.", service->decoded_key);
	
	if(retrying)
		service->logins++;
	
	if(service->logins < MAX_LOGINS ){
		debug(DEBUG_DOMAIN, "Authenticating https://%s:%s@%s/", service->username, service->password, service->url);
		soup_auth_authenticate(auth, service->username, service->password);
		service->connected=TRUE;
	}else{
		debug(DEBUG_DOMAIN, "Authentication for https://%s:%s@%s/ failed.  Disabling service's connection.", service->username, service->password, service->url);
		service->connected=FALSE;
	}
}//online_service_http_authenticate


SoupMessage *online_service_request(OnlineService *service, RequestMethod request, const gchar *resource, SoupSessionCallback callback, gpointer user_data, gpointer formdata){
	if(!(service->connected)) return NULL;
	gchar *new_uri=g_strdup_printf("https://%s%s%s", service->url, ( (g_str_equal(service->url, "twitter.com")) ?"" :"/api" ), resource );
	debug(DEBUG_DOMAIN, "Creating new service request for: '%s', requesting: %s.", service->decoded_key, new_uri);
	SoupMessage *msg=online_service_request_url(service, request, (const gchar *)new_uri, callback, user_data, formdata);
	g_free(new_uri);
	return msg;
}//online_service_request

SoupMessage *online_service_request_url(OnlineService *service, RequestMethod request, const gchar *resource, SoupSessionCallback callback, gpointer user_data, gpointer formdata){
	if(!(service->connected)) return NULL;
	if(!SOUP_IS_SESSION(service->session)){
		online_service_reconnect(service);
		if(!SOUP_IS_SESSION(service->session))
			return NULL;
	}
	SoupMessage *msg=NULL;
	gchar *service_formdata=NULL;
	
	debug(DEBUG_DOMAIN, "Creating libsoup request for service: '%s'.", service->decoded_key);
	switch(request){
		case GET:
		case QUEUE:
			debug(DEBUG_DOMAIN, "GET: %s", resource);
			msg=soup_message_new("GET", resource);
			break;
		case POST:
			if((gchar *)formdata){
				if((gchar *)user_data){
					gchar *test_data=(gchar *)user_data;
					if( g_str_equal(test_data, "Tweet") ){
						gchar *form_data=(gchar *)formdata;
						if(G_STR_EMPTY(form_data))
							return NULL;
						if( in_reply_to_status_id && in_reply_to_service==service )
							service_formdata=g_strdup_printf( "source=%s&in_reply_to_status_id=%lu&status=%s", (g_str_equal("twitter.com", service->url) ?API_CLIENT_AUTH :OPEN_CLIENT ), in_reply_to_status_id, form_data);
						else
							service_formdata=g_strdup_printf( "source=%s&status=%s", (g_str_equal("twitter.com", service->url) ?API_CLIENT_AUTH :OPEN_CLIENT ), form_data);
					}
				}
			}

			debug(DEBUG_DOMAIN, "POST: %s\n\t\tformdata:%s", resource, (service_formdata ?service_formdata :(gchar *)formdata));
			msg=soup_message_new("POST", resource);
	
			soup_message_headers_append(msg->request_headers, "X-Twitter-Client", PACKAGE_NAME);
			soup_message_headers_append(msg->request_headers, "X-Twitter-Client-Version", PACKAGE_VERSION);
			
			if((gchar *)formdata)
				soup_message_set_request(
					msg,
					"application/x-www-form-urlencoded",
					SOUP_MEMORY_COPY,
					(service_formdata ?service_formdata :(gchar *)formdata),
					(strlen( ( service_formdata ?service_formdata :(gchar *)formdata ) ))
				);
			break;
	}
	
	if(!SOUP_IS_SESSION(service->session))
		return NULL;
	
	debug(DEBUG_DOMAIN, "Processing libsoup request for service: '%s'.", service->decoded_key);
	switch(request){
		case QUEUE:
		case POST:
			debug(DEBUG_DOMAIN, "Adding libsoup request to service: '%s' libsoup's message queue.", service->decoded_key);
			
			OnlineServiceCBWrapper *service_wrapper=NULL;
			if(callback){
				service_wrapper=g_new0(OnlineServiceCBWrapper, 1);
				service_wrapper->service=service;
				service_wrapper->user_data=user_data;
			}
			
			soup_session_queue_message(service->session, msg, callback, service_wrapper);
			
			if(service_formdata){
				g_free(service_formdata);
				service_formdata=NULL;
			}
			
			return msg;
		case GET:
			debug(DEBUG_DOMAIN, "Sending libsoup request to service: '%s' & returning libsoup's message.", service->decoded_key);
			soup_session_send_message(service->session, msg);
			if(!network_check_http(service, msg))
				return msg;
			return msg;
	}
	return msg;
}//online_service_request_url

void online_services_free_wrapper(OnlineServices *services, OnlineServiceCBWrapper *service_wrapper){
	if(!service_wrapper)
		return;
	
	service_wrapper->service=NULL;
	
	if( (service_wrapper->service==online_services_get_last_connected(services)) && service_wrapper->user_data ){
		g_free(service_wrapper->user_data);
		service_wrapper->user_data=NULL;
	}
	g_free(service_wrapper);
	service_wrapper=NULL;
}//online_service_free_wrapper

void online_service_free(OnlineService *service){
	if(!service) return;
	
	debug(DEBUG_DOMAIN, "Unloading instance of: %s service", service->decoded_key);
	online_service_disconnect(service);
	
	debug(DEBUG_DOMAIN, "Closing %s account", service->decoded_key );
	g_free(service->decoded_key);
	g_free(service->key);
	g_free(service->url);
	g_free(service->username);
	g_free(service->password);
	timer_free(service->timer);
	service->timer=NULL;
	
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

static gboolean online_service_proxy_init(OnlineService *service){
	if(!proxy_init)
		proxy_init=online_services_proxy_load();
	
	if(proxy_init==-1)
		return FALSE;
	
	g_object_set( G_OBJECT(service->session), SOUP_SESSION_PROXY_URI, proxy_suri, NULL);
	return TRUE;
}//online_service_proxy_init

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
	if(proxy_init==1 && proxy_suri){
		soup_uri_free(proxy_suri);
		proxy_suri=NULL;
	}
	proxy_init=0;
}//online_services_proxy_release

/********************************************************
 *                       eof                            *
 ********************************************************/

