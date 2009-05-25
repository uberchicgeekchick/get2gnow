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
#include "network.h"

#ifdef HAVE_GNOME_KEYRING
#include "keyring.h"
#endif

#include "app.h"
#include "main.h"
#include "parser.h"
#include "gconfig.h"
#include "cache.h"
#include "tweets.h"
#include "users.h"
#include "gtkbuilder.h"
#include "preferences.h"
#include "services-dialog.h"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static gboolean online_service_connect(OnlineService *service);
static gboolean online_service_login(OnlineService *service);
static gboolean online_service_reconnect(OnlineService *service);
static void online_service_disconnect(OnlineService *service);


/* key is username@url */
static OnlineService *online_service_open(const gchar *account_key);
static OnlineService *online_service_new(gboolean enabled, const gchar *url, const gchar *username, const gchar *password, gboolean auto_connect);
static gboolean online_service_save(OnlineService *service);

static void online_service_http_authenticate(SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer user_data);
static void online_service_login_success(SoupSession *session, SoupMessage *msg, gpointer user_data);
static void online_service_message_restarted(SoupMessage *msg, gpointer user_data);

static void online_service_cookie_jar_hands_caught_in_the_cookie_jar(SoupCookieJar *cookie_jar, SoupCookie *old_cookie, SoupCookie *new_cookie, OnlineService *service);
static void online_service_cookie_jar_open(OnlineService *service);
static gchar *online_service_cookie_jar_find_filename(OnlineService *service);

static void online_services_proxy_release(void);
static int online_services_proxy_load(void);
static gboolean online_service_proxy_init(OnlineService *service);


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define PROXY				"/system/http_proxy"
#define PROXY_USE			PROXY "/use_http_proxy"
#define PROXY_HOST			PROXY "/host"
#define PROXY_PORT			PROXY "/port"
#define PROXY_USE_AUTH			PROXY "/use_authentication"
#define PROXY_USER			PROXY "/authentication_user"
#define PROXY_PASS			PROXY "/authentication_password"

static int proxy_init=0;
static SoupURI *proxy_suri=NULL;
static OnlineServices *services=NULL;
OnlineService *selected_service=NULL;

OnlineServices *online_services=NULL;

#define	DEBUG_DOMAINS	"OnlineServices:UI:Network:Tweets:Requests:Users:Authentication"
#include "debug.h"

#define MAX_LOGINS 20


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
		debug("Loading  '%s' account.", account_key);
		service=online_service_open( (const gchar *)account_key );
		services->accounts=g_list_append(services->accounts, service );
		services->accounts=g_list_last(services->accounts);
		service=(OnlineService *)services->accounts->data;
		
		services->total++;
		
		online_service_connect(service);
		
		debug("Loaded account: '%s'.  Validating & connecting.", service->decoded_key);
		if(!service->enabled){
			debug("%s account not enabled.", service->decoded_key);
			continue;
		}
		
		if(G_STR_EMPTY(service->username) || G_STR_EMPTY(service->password)){
			debug("%s account is missing its username (=%s) and/or password (=%s).", service->decoded_key, service->username, service->password);
			continue;
		}
		
		if(!service->auto_connect){
			debug("%s account in not set to auto-connect.", service->decoded_key);
			continue;
		}
		
		if(service->connected){
			services->connected++;
			online_service_login(service);
		}
	}
	
	services->accounts=g_list_first(services->accounts);
	
	online_services=services;
	
	if(!services->connected){
		debug("No accounts are enabled or setup.  New accounts will be need to be setup." );
		services_dialog_show(NULL);
	}else
		debug("%d services found and loaded.", services->total);
	
	return services;
}/*online_services_init*/

/* Login to services. */
gboolean online_services_login(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	gboolean	login_okay=FALSE;
	
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		if(service->connected){
			online_service_login(service);
			if(!login_okay) login_okay=TRUE;
		}
	}
	app_state_on_connection(login_okay);
	return login_okay;
}/*online_services_login*/

/* Login to services. */
gboolean online_services_relogin(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	gboolean	relogin_okay=FALSE;
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		if(!service->connected && service->auto_connect)
			online_service_reconnect(service);
		
		if(service->connected){
			online_service_login(service);
			if(!relogin_okay) relogin_okay=TRUE;
		}
	}
	app_state_on_connection(relogin_okay);
	return relogin_okay;
}/*online_services_relogin*/

/* Reconnect to services. */
gboolean online_services_reconnect(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	gboolean	reconnect_success=FALSE;
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		if(service->enabled)
			if(online_service_reconnect(service))
				if(!(reconnect_success))
					reconnect_success=TRUE;
	}
	app_state_on_connection(reconnect_success);
	return reconnect_success;
}/*online_services_reconnect*/

void online_services_disconnect(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	selected_service=NULL;
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		if(service->connected)
			online_service_disconnect(service);
	}
	app_state_on_connection(FALSE);
}/*online_services_disconnect*/


OnlineService *online_services_save(OnlineServices *services, OnlineService *service, gboolean enabled, const gchar *url, const gchar *username, const gchar *password, gboolean auto_connect){
	if( G_STR_EMPTY(url) || G_STR_EMPTY(username) )
		return FALSE;
	
	if(service){
		debug("Saving existing service: '%s'.", service->decoded_key);
		if(!( g_str_equal(service->uri, url) && g_str_equal(service->username, username) )){
			debug("Removing old OnlineService: '%s'.", service->decoded_key);
			gconfig_rm_rf(service->key);
			services->keys=g_slist_remove(services->keys, service->key);
			online_service_free(service);
		}else{
			service->enabled=enabled;
			service->password=g_strdup(password);
			service->auto_connect=auto_connect;
			if( (online_service_save(service)) )
				if(online_service_reconnect(service))
					return service;
			debug("Unable to save existing OnlineService for: [%s].", service->decoded_key);
			return NULL;
		}
	}
	
	debug("Creating & saving new service: '%s@%s'.", username, url);
	service=online_service_new(enabled, url, username, password, auto_connect);
	debug("New OnlineService '%s' created.  Saving OnlineServices", service->decoded_key);
	
	services->total++;
	debug("New service: '%s' created.  OnlineServices total: %d.", service->decoded_key, services->total);
	
	debug("Adding '%s' to OnlineServices keys.", service->decoded_key);
	if(!( (services->keys=g_slist_append(services->keys, service->key)) )){
		debug("**ERROR**: Failed to append new service's key: '%s', to OnlineServices' keys.", service->decoded_key);
		return NULL;
	}
	
	debug("Saving accounts & services list: '%s'.", PREFS_AUTH_SERVICES);
	if(!( (gconfig_set_list_string(PREFS_AUTH_SERVICES, services->keys)) )){
		debug("**ERROR**: Failed to save new service: '%s', couldn't save gconf's services list.", service->decoded_key);
		return NULL;
	}
	
	debug("Adding new service: '%s' to OnlineServices.", service->decoded_key);
	if(!( (services->accounts=g_list_append(services->accounts, service)) )){
		debug("**ERROR**: Failed to add: '%s', to OnlineServices' accounts.", service->decoded_key);
		return NULL;
	}
	
	debug("Retrieving new service: '%s' from OnlineServices accounts.", service->decoded_key);
	services->accounts=g_list_last(services->accounts);
	service=(OnlineService *)services->accounts->data;
	services->accounts=g_list_first(services->accounts);
	
	debug("Saving OnlineService: '%s' reloaded from OnlineServices accounts.", service->decoded_key);
	if(!( online_service_save(service) )){
		debug("**ERROR**: Failed saving new service: '%s@%s'.", username, url);
		return NULL;
	}
	
	debug("Saving accounts & services successful.");
	if(service->connected){
		services->connected++;
		debug("\t\tConnecting to: '%s'\t[succeeded]", service->decoded_key);
	}else
		debug("\t\tConnecting to: '%s'\t[failed]", service->decoded_key);
	
	debug("Saving '%s' service complete.  Total services: %d; Total connected: %d", service->decoded_key, services->total, services->connected);
	
	return service;
}/*online_services_save*/

void online_services_request(OnlineServices *services, RequestMethod request, const gchar *uri, SoupSessionCallback callback, gpointer user_data, gpointer formdata){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		if(service->enabled)
			online_service_request(service, request, uri, callback, user_data, formdata);
	}
}/*online_services_request*/

gboolean online_services_fill_liststore(OnlineServices *services, GtkListStore *liststore, gboolean connected_only){
	if(!online_services->total){
		debug("No services found to load, new accounts need to be setup.");
		return FALSE;
	}
	
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	GList			*a=NULL;
	guint			services_loaded=0;
	debug("Loading services into tree view. total services: '%d'.", online_services->total);
	for(a=services->accounts; a; a=a->next){
		OnlineService *service=(OnlineService *)a->data;
		if( connected_only && !service->connected ) continue;
		
		debug("Appending account: '%s'; server: %s.", service->decoded_key, service->uri);
		gtk_list_store_append(liststore, iter);
		gtk_list_store_set(
					liststore, iter,
						UrlString, service->uri,
						OnlineServicePointer, service,
					-1
		);
		services_loaded++;
	}
	return (services_loaded ?TRUE :FALSE );
}/*online_services_fill_liststore*/


/*
 * 	Methods for use with an OnlineService instance.
 */
static OnlineService *online_service_open(const gchar *account_key){
	debug("Loading OnlineService instance for: '%s' account.", account_key);
	
	OnlineService *service=g_new0(OnlineService, 1);
	
	gchar **account_data=g_strsplit(account_key, "@", 3);
	
	service->decoded_key=g_strdup(account_key);
	service->uri=g_strdup(account_data[1]);
	service->username=g_strdup(account_data[0]);
	g_strfreev(account_data);
	
	service->friends=service->followers=service->friends_and_followers=NULL;
	
	gchar **parsed_uri=g_strsplit(service->uri, "/", 1);
	service->server=g_strdup(parsed_uri[0]);
	if(G_STR_EMPTY(parsed_uri[1]))
		service->path=g_strdup("");
	else
		service->path=g_strdup(parsed_uri[1]);
	g_strfreev(parsed_uri);

	service->connected=FALSE;
	service->session=NULL;
	service->logins=0;
	
	service->timer=timer_new();
	
	gchar *encoded_username=g_uri_escape_string(service->username, NULL, TRUE);
	gchar *encoded_url=g_uri_escape_string(service->uri, NULL, TRUE);
	service->key=g_strdup_printf("%s@%s", encoded_username, encoded_url );
	g_free(encoded_username);
	g_free(encoded_url);
	
	if(g_str_equal(service->uri, "twitter.com"))
		service->which_rest=Twitter;
	else
		service->which_rest=Laconica;
	
	gchar *prefs_auth_path=NULL;
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_PREFIX, service->key);
	debug("Loading OnlineService settings for: '%s'\n\t\tAccount's gconf path: %s", service->decoded_key, prefs_auth_path );
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_ENABLED, service->key);
	gconfig_get_bool(prefs_auth_path, &service->enabled);
	g_free(prefs_auth_path);
	debug("OnlineService for: '%s' is %sabled.", service->decoded_key, (service->enabled ?"en" :"dis") );
	
	service->password=NULL;
	
#ifdef HAVE_GNOME_KEYRING
	if(G_STR_EMPTY(service->username))
		service->password=NULL;
	else if(!(keyring_get_password(&service)))
		service->password=NULL;
#else
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_PASSWORD, service->key);
	gconfig_get_string(prefs_auth_path, &service->password);
	g_free(prefs_auth_path);
#endif
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_AUTO_CONNECT, service->key);
	gconfig_get_bool(prefs_auth_path, &service->auto_connect);
	g_free(prefs_auth_path);
	
	debug("OnlineService instance created.\n\t\taccount '%s(=%s)'\t\t\t[%sabled]\n\t\tservice url: %s; username: %s; password: %s; auto_connect: %s", service->decoded_key, service->key, (service->enabled?"en":"dis"), service->uri, service->username, service->password, (service->auto_connect?"TRUE":"FALSE"));
	
	return service;
}/*online_service_open*/

static OnlineService *online_service_new(gboolean enabled, const gchar *url, const gchar *username, const gchar *password, gboolean auto_connect){
	debug("Creating new OnlineService for '%s@%s'.", username, url);
	OnlineService *service=g_new0(OnlineService, 1);
	service->session=NULL;
	
	service->connected=FALSE;
	service->enabled=enabled;
	service->logins=0;
	service->timer=timer_new();
	service->auto_connect=auto_connect;
	
	service->decoded_key=g_strdup_printf("%s@%s", username, url );
	service->username=g_strdup(username);
	service->password=g_strdup(password);
	
	service->friends=service->followers=service->friends_and_followers=NULL;
	
	service->uri=g_strdup(url);
	gchar **parsed_uri=g_strsplit(service->uri, "/", 1);
	service->server=g_strdup(parsed_uri[0]);
	if(G_STR_EMPTY(parsed_uri[1]))
		service->path=g_strdup("");
	else
		service->path=g_strdup(parsed_uri[1]);
	g_strfreev(parsed_uri);
	
	gchar *encoded_username=g_uri_escape_string(username, NULL, TRUE);
	gchar *encoded_url=g_uri_escape_string(url, NULL, TRUE);
	service->key=g_strdup_printf("%s@%s", encoded_username, encoded_url);
	g_free(encoded_username);
	g_free(encoded_url);
	
	if(g_str_equal(service->uri, "twitter.com"))
		service->which_rest=Twitter;
	else
		service->which_rest=Laconica;
	
	return service;
}/*online_service_new*/

static gboolean online_service_save(OnlineService *service){
	debug("Preparing to save OnlineService for '%s'.", service->decoded_key);
	
	gchar *prefs_auth_path=NULL;
	
	debug("Saving '%s' 'enabled' status: %s.", service->decoded_key, (service->enabled ?"TRUE" :"FALSE" ) );
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_ENABLED, service->key);
	gconfig_set_bool(prefs_auth_path, service->enabled);
	g_free(prefs_auth_path);
	
	debug("Saving '%s' password: '%s'.", service->decoded_key, service->password );
#ifdef HAVE_GNOME_KEYRING
	keyring_set_password(service);
#else
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_PASSWORD, service->key);
	gconfig_set_string(prefs_auth_path, service->password);
	g_free(prefs_auth_path);
#endif
	
	debug("Saving '%s' 'auto_connect' status: %s.", service->decoded_key, (service->auto_connect ?"TRUE" :"FALSE" ) );
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_AUTO_CONNECT, service->key);
	gconfig_set_bool(prefs_auth_path, service->auto_connect);
	g_free(prefs_auth_path);
	
	debug("OnlineService saved.\n\t\taccount '%s(=%s)'\t\t\t[%sabled]\n\t\tservice url: %s; username: %s; password: %s; auto_connect: [%s]", service->decoded_key, service->key, (service->enabled?"en":"dis"), service->uri, service->username, service->password, (service->auto_connect?"TRUE":"FALSE"));

	debug("Attempting to connect to OnlineService for: '%s'.", service->decoded_key);
	online_service_reconnect(service);
	
	return TRUE;
}/*online_service_save*/

OnlineService *online_services_connected_get_first(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	for(a=g_list_first(services->accounts); a; a=a->next){
		service=(OnlineService *)a->data;
		if(service->connected)
			break;
	}
	return service;
}/*online_services_connected_get_first*/

static gboolean online_service_connect(OnlineService *service){
	debug("Creating '%s' connection to '%s'", service->decoded_key, service->uri);
	if(!( (service->session=soup_session_sync_new_with_options(SOUP_SESSION_MAX_CONNS_PER_HOST, 8, SOUP_SESSION_TIMEOUT, 20, SOUP_SESSION_IDLE_TIMEOUT, 20, NULL)) )){
		debug("**ERROR:** Failed to creating a new soup session for '%s'.", service->decoded_key);
		service->session=NULL;
		return (service->connected=FALSE);
	}
	
	/* HTTP Basic Authentication */
	debug("Adding authenticating callback for: '%s'. username: %s, password: %s", service->decoded_key, service->username, service->password);
	g_signal_connect(service->session, "authenticate", G_CALLBACK(online_service_http_authenticate), service);
	
#ifdef GNOME_ENABLE_DEBUG
	
	/*SoupLogger *logger=soup_logger_new(SOUP_LOGGER_LOG_MINIMAL, -1);*/
	SoupLogger *logger=soup_logger_new(SOUP_LOGGER_LOG_HEADERS, -1);
	soup_session_add_feature(service->session, SOUP_SESSION_FEATURE(logger));
	g_object_unref(logger);
	
#else
	
	if(IF_DEBUG){
		SoupLogger *logger=soup_logger_new(SOUP_LOGGER_LOG_HEADERS, -1);
		soup_session_add_feature(service->session, SOUP_SESSION_FEATURE(logger));
		g_object_unref(logger);
	}
	
#endif
	
	online_service_proxy_init(service);
	
	online_service_cookie_jar_open(service);
	
	return (service->connected=TRUE);
}/*online_service_connect*/

static gchar *online_service_cookie_jar_find_filename(OnlineService *service){
	gchar	*cookie_jar_dir=NULL;
	gchar	*cookie_jar_directory=NULL;
	
	gchar	*cookie_jar_file=NULL;
	gchar	*cookie_jar_filename=NULL;
	
	cookie_jar_dir=g_build_filename("services", service->uri, "cookies", service->username, NULL);
	
	if(!( (cookie_jar_directory=cache_dir_test(cookie_jar_dir, TRUE)) )){
			debug("\t\t**ERROR:** Failed to open cookie jar.\n\t\tUnable to create cookie jar's directory: [%s].", cookie_jar_directory);
			g_free(cookie_jar_dir);
			return NULL;
		}
	
	cookie_jar_file=g_build_filename(cookie_jar_dir, "cookies.txt", NULL);
	if(!( (cookie_jar_filename=cache_file_touch(cookie_jar_file)) )){
		debug("\t\t**ERROR:** Failed to open cookie jar.\n\t\tUnable to create cookie jar: [%s].", cookie_jar_filename);
		g_free(cookie_jar_dir);
		g_free(cookie_jar_directory);
		g_free(cookie_jar_filename);
		return NULL;
	}
				
	debug("\t\tCreated cookie jar for [%s].\n\t\tDirectory: [%s]\n\t\tFilename: [%s].", service->key, cookie_jar_directory, cookie_jar_filename );
	
	g_free(cookie_jar_dir);
	g_free(cookie_jar_directory);
	return cookie_jar_filename;
}/*online_service_cookie_jar_find_filename*/

static void online_service_cookie_jar_open(OnlineService *service){
	SoupCookieJar	*cookie_jar=NULL;
	gchar		*cookie_jar_filename=NULL;
	
	debug("Connecting cookie jar to online service: [%s].", service->key);
		
	if(!( (cookie_jar_filename=online_service_cookie_jar_find_filename(service)) ))
		return;

	cookie_jar=soup_cookie_jar_text_new(cookie_jar_filename, FALSE);
	
	soup_session_add_feature_by_type(service->session, soup_cookie_jar_get_type());
	soup_session_add_feature(service->session, SOUP_SESSION_FEATURE(cookie_jar));
	
	g_free(cookie_jar_filename);
	g_object_unref(cookie_jar);
	return;
	
	g_signal_connect_after(cookie_jar, "changed", (GCallback)online_service_cookie_jar_hands_caught_in_the_cookie_jar, service);
}/*online_servce_open_cookie_jar*/

static void online_service_cookie_jar_hands_caught_in_the_cookie_jar(SoupCookieJar *cookie_jar, SoupCookie *old_cookie, SoupCookie *new_cookie, OnlineService *service){
}/*online_service_hands_caught_in_the_cookie_jar*/

static gboolean online_service_reconnect(OnlineService *service){
	if(service->connected)
		online_service_disconnect(service);
	
	return online_service_connect(service);
}/*online_service_reconnect*/

static void online_service_disconnect(OnlineService *service){
	debug("Closing %s's connection to: %s", service->decoded_key, service->uri);
	if(service->session){
		if(SOUP_IS_SESSION(service->session))
			soup_session_abort(service->session);
		
		g_object_unref(service->session);
		service->session=NULL;
	}
	service->connected=FALSE;
}/*online_service_disconnect*/

/* Login to service. */
gboolean online_service_login(OnlineService *service){
	debug("Attempting to log in to %s...", service->decoded_key);
	if(!service->enabled) return FALSE;
	
	if(!SOUP_IS_SESSION(service->session)){
		debug("**ERROR**: Unable to authenticating OnlineService: %s. Invalide libsoup session.", (service->decoded_key ?service->decoded_key :"invalid service") );
		return FALSE;
	}
	
	app_statusbar_printf("Connecting to %s...", service->key);
	
	/* Verify cedentials */
	debug("Logging into: '%s'. username: %s, password: %s.", service->decoded_key, service->username, service->password);
	
	online_service_request(service, QUEUE, API_LOGIN, online_service_login_success, (gpointer)"Login", NULL);
	return TRUE;
}/*online_service_login*/

/* On verify credentials */
static void online_service_login_success(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceCBWrapper *service_wrapper=(OnlineServiceCBWrapper *)user_data;
	debug("Login response: %i",msg->status_code);
	
	/* if the check succeeds then it start the request limit timer:
	 * 	timer_main(service->timer, msg);
	 */
	if(!network_check_http(service_wrapper->service, msg)) {
		debug("Login to '%s' failed.", service_wrapper->service->decoded_key);
	}else{
		debug("Login to '%s' succeeded.", service_wrapper->service->decoded_key);
	}
	online_service_wrapper_free(service_wrapper);
}/*online_service_login_success*/

/* HTTP Basic Authentication */
static void online_service_http_authenticate(SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer user_data){
	OnlineService *service=(OnlineService *)user_data;
	debug("**Authenticating:** %s.", service->key );
	if(!(service && service->key)){
		debug("**FATAL-ERROR**: Unable to authenticating OnlineService: %s. Invalid online service." );
		return;
	}
	
	if(!service->enabled){
		debug("\t\t**WARNING:** Skipping authentication for disabled OnlineService '%s'.", service->decoded_key);
		return;
	}
	
	SoupURI *suri=soup_message_get_uri(msg);
	gchar *uri=soup_uri_to_string(suri, FALSE);
	debug("\t\tSoupURI: Authenticating: %s", uri);
	soup_uri_free(suri);
	g_free(uri);
	
	if(G_STR_EMPTY(service->username))
		return debug("\t\t**WARNING:** Could not authenticate: %s, unknown username.", service->decoded_key);
	
	/* verify that the password has been set */
	if(G_STR_EMPTY(service->password))
		return debug("\t\t**WARNING:** Could not authenticate: %s, unknown password.", service->decoded_key);
	
	if(retrying)
		service->logins++;
	
	if(service->logins < MAX_LOGINS ){
		debug("Authenticating OnlineService: [%s]\n\t\t\tAttempt #%d of %d maximum allowed attempts.\n\t\t\tUsername: [%s]; Password: [%s]; Server: [%s].", service->key, service->logins, MAX_LOGINS, service->username, service->password, service->uri);
		soup_auth_update(auth, msg, "WWW-Authenticate");
		soup_auth_authenticate(auth, service->username, service->password);
	}else{
		debug("**ERROR**: Authentication attempts %d exceeded maximum attempts: %d.", service->logins, MAX_LOGINS);
	}
}/*online_service_http_authenticate*/


SoupMessage *online_service_request(OnlineService *service, RequestMethod request, const gchar *uri, SoupSessionCallback callback, gpointer user_data, gpointer formdata){
	if(!(service->connected)) return NULL;
	gchar *new_uri=g_strdup_printf("https://%s%s%s", service->uri, ( (service->which_rest==Twitter) ?"" :"/api" ), uri );
	debug("Creating new service request for: '%s', requesting: %s.", service->decoded_key, new_uri);
	SoupMessage *msg=online_service_request_uri(service, request, (const gchar *)new_uri, callback, user_data, formdata);
	g_free(new_uri);
	return msg;
}/*online_service_request*/

SoupMessage *online_service_request_uri(OnlineService *service, RequestMethod request, const gchar *uri, SoupSessionCallback callback, gpointer user_data, gpointer formdata){
	if(!(service->enabled && service->connected)) return NULL;
	
	if(!SOUP_IS_SESSION(service->session)){
		online_service_reconnect(service);
		if(!SOUP_IS_SESSION(service->session))
			return NULL;
	}
	
	gchar *request_uri=NULL;
	if(!( g_str_has_prefix(uri, "http") || g_str_has_prefix(uri, "ftp") ))
		request_uri=g_strdup_printf("http://%s", uri);
	else
		request_uri=g_strdup(uri);
	
	SoupMessage *msg=NULL;
	gchar *service_formdata=NULL;
	gchar *request_string=NULL;
	switch(request){
		case GET:
			request_string=g_strdup("GET");
			break;
		case QUEUE:
			request_string=g_strdup("QUEUE");
			break;
		case POST:
			request_string=g_strdup("POST");
			break;
	}
	
	debug("Creating libsoup request for service: '%s'.", service->decoded_key);
	switch(request){
		case GET:
		case QUEUE:
			debug("GET: %s", request_uri);
			msg=soup_message_new("GET", request_uri);
			break;
		
		case POST:
			if((gchar *)formdata){
				if((gchar *)user_data){
					gchar *test_data=(gchar *)user_data;
					if( g_str_equal(test_data, "Tweet") ){
						gchar *form_data=(gchar *)formdata;
						if(G_STR_EMPTY(form_data)){
							g_free(request_string);
							g_free(request_uri);
							return NULL;
						}
						if( in_reply_to_status_id && in_reply_to_service==service ){
							debug("Replying to Tweet/Status [#%lu] on: [%s].\n\t\tTweet: [%s].", in_reply_to_status_id, service->decoded_key, form_data);
							service_formdata=g_strdup_printf( "source=%s&in_reply_to_status_id=%lu&status=%s", ((service->which_rest==Twitter) ?API_CLIENT_AUTH :OPEN_CLIENT ), in_reply_to_status_id, form_data);
						}else{
							debug("Submitting Tweet/Status update to: [%s].\n\t\tTweet: [%s].", service->decoded_key, form_data);
							service_formdata=g_strdup_printf( "source=%s&status=%s", ((service->which_rest==Twitter) ?API_CLIENT_AUTH :OPEN_CLIENT ), form_data);
						}
					}
				}
			}
			
			debug("POST: %s\n\t\tformdata: [%s]", request_uri, (service_formdata ?service_formdata :(gchar *)formdata));
			msg=soup_message_new("POST", request_uri);
	
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
	
	if(!( SOUP_IS_SESSION(service->session) && SOUP_IS_MESSAGE(msg) )){
		debug("Unable to process libsoup request for service: '%s'.\n\t\tAttempting to %s: %s", service->decoded_key, request_string, request_uri);
		if(service_formdata) g_free(service_formdata);
		g_free(request_string);
		g_free(request_uri);
		return NULL;
	}
	
	g_signal_connect(msg, "restarted", G_CALLBACK(online_service_message_restarted), (gpointer)service);
	
	debug("Processing libsoup request for service: '%s'.", service->decoded_key);
	switch(request){
		case QUEUE:
		case POST:
			debug("Adding libsoup request to service: '%s' libsoup's message queue.", service->decoded_key);
			
			OnlineServiceCBWrapper *service_wrapper=NULL;
			if(callback!=NULL)
				service_wrapper=online_service_wrapper_new(service, request_uri, callback, user_data, formdata);
			
			soup_session_queue_message(service->session, msg, callback, service_wrapper);
			
			break;
			
		case GET:
			debug("Sending libsoup request to service: '%s' & returning libsoup's message.", service->decoded_key);
			soup_session_send_message(service->session, msg);
			break;
	}
	
	if(service_formdata) g_free(service_formdata);
	g_free(request_string);
	g_free(request_uri);
	return msg;
}/*online_service_request_uri*/

static void online_service_message_restarted(SoupMessage *msg, gpointer user_data){
//	OnlineService *service=(OnlineService *)user_data;
}/*online_service_message_restarted*/

gchar *online_service_get_url_content_type(OnlineService *service, const gchar *uri, SoupMessage **msg){
	*msg=online_service_request_uri(service, GET, uri, NULL, NULL, NULL);
	if(!network_check_http(service, *msg))
		return g_strdup(uri);

	debug("Getting content-type from uri: '%s'.", uri);
	gchar *content_type=NULL;
	if(!(content_type=(gchar *)soup_message_headers_get_content_type( (*msg)->response_headers, NULL))){
		debug("Unable to determine the content type from uri: '%s'.", uri);
		return NULL;
	}
	
	return content_type;
}/*online_service_get_url_content_type*/

OnlineServiceCBWrapper *online_service_wrapper_new(OnlineService *service, gchar *request_uri, SoupSessionCallback callback, gpointer user_data, gpointer formdata){
	OnlineServiceCBWrapper *service_wrapper=g_new0(OnlineServiceCBWrapper, 1);
	
	service_wrapper->service=service;

	service_wrapper->requested_uri=g_strdup(request_uri);
	
	if(callback==network_cb_on_image||callback==user_request_main_quit||callback==network_users_glist_save)
		service_wrapper->user_data=user_data;
	else if(user_data!=NULL)
		service_wrapper->user_data=g_strdup(user_data);
	else
		service_wrapper->user_data=NULL;
	
	if(formdata!=NULL)
		service_wrapper->formdata=g_strdup(formdata);
	else
		service_wrapper->formdata=NULL;
	
	return service_wrapper;
}

void online_service_wrapper_free(OnlineServiceCBWrapper *service_wrapper){
	if(service_wrapper==NULL)
		return;
	
	g_free(service_wrapper->requested_uri);
	service_wrapper->requested_uri=NULL;
	
	if(service_wrapper->user_data!=NULL){
		g_free(service_wrapper->user_data);
		service_wrapper->user_data=NULL;
	}
	
	if(service_wrapper->formdata!=NULL){
		g_free(service_wrapper->formdata);
		service_wrapper->formdata=NULL;
	}
	
	service_wrapper->service=NULL;
	
	g_free(service_wrapper);
	service_wrapper=NULL;
}/*online_service_free_wrapper*/

void online_service_free(OnlineService *service){
	if(!service) return;
	
	debug("Unloading instance of: %s service", service->decoded_key);
	online_service_disconnect(service);
	
	debug("Closing %s account", service->decoded_key );
	g_free(service->decoded_key);
	g_free(service->key);
	g_free(service->uri);
	g_free(service->username);
	g_free(service->password);
	user_free_lists(service);
	
	timer_free(service->timer);
	service->timer=NULL;
	
	g_free(service);
	service=NULL;
}/*online_service_free*/

void online_services_deinit(OnlineServices *o_services){
	online_services_proxy_release();
	
	debug("**SHUTDOWN:** Closing & releasing %d accounts.", o_services->total);
	g_list_foreach(o_services->accounts, (GFunc)online_service_free, NULL);

	debug("**SHUTDOWN:** freeing OnlineServices keys.");
	g_slist_free(o_services->keys);
	o_services->keys=NULL;
	
	g_free(o_services);
	o_services=NULL;
	
	selected_service=NULL;
	
	services=NULL;
}/*online_services_deinit*/

static gboolean online_service_proxy_init(OnlineService *service){
	if(!proxy_init)
		proxy_init=online_services_proxy_load();
	
	if(proxy_init==-1)
		return FALSE;
	
	g_object_set( G_OBJECT(service->session), SOUP_SESSION_PROXY_URI, proxy_suri, NULL);
	return TRUE;
}/*online_service_proxy_init*/

/**
 *  * Sets SoupUri *proxy_suri for use with a SoupConnection.
 *   *
 *    * @returns 1 if a proxy is used or -1 if not.
 *     */
static int online_services_proxy_load(void){
	if(proxy_init) return proxy_init;
	
	/* Set the proxy, if configuration is set */
	gboolean proxy_use=FALSE;
	if(!( (gconfig_get_bool(PROXY_USE, &proxy_use)) && proxy_use ))
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
	gboolean proxy_auth=FALSE;
	if(!( (gconfig_get_bool(PROXY_USE_AUTH, &proxy_auth)) && proxy_auth ))
		proxy_uri=g_strdup_printf("http://%s:%d", server, port);
	else {
		char *user, *password;
		gconfig_get_string(PROXY_USER, &user);
		gconfig_get_string(PROXY_PASS, &password);
		
		proxy_uri=g_strdup_printf( "http://%s:%s@%s:%d", user, password, server, port);
		
		g_free(user);
		g_free(password);
	}
	
	debug("Proxy uri: %s", proxy_uri );
	
	/* Setup proxy info */
	proxy_suri=soup_uri_new(proxy_uri);
	
	g_free(server);
	g_free(proxy_uri);
	
	return 1;
}/*online_services_proxy_load*/

static void online_services_proxy_release(void){
	if(proxy_init==1 && proxy_suri){
		debug("**SHUTDOWN:** closing proxy connection");
		soup_uri_free(proxy_suri);
		proxy_suri=NULL;
	}
	proxy_init=0;
}/*online_services_proxy_release*/

/********************************************************
 *                       eof                            *
 ********************************************************/

