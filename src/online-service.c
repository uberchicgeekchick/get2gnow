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
#include "main.h"

#include "online-services.h"
#include "online-service-wrapper.h"
#include "users-glists.h"

#include "tweets.h"
#include "users.h"

#include "network.h"
#include "proxy.h"
#include "parser.h"

#ifdef HAVE_GNOME_KEYRING
#include "keyring.h"
#endif

#include "app.h"
#include "gconfig.h"
#include "cache.h"
#include "preferences.h"
#include "services-dialog.h"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static void online_service_http_authenticate(SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer user_data);
static void online_service_login_check(SoupSession *session, SoupMessage *msg, gpointer user_data);
static void online_service_message_restarted(SoupMessage *msg, gpointer user_data);

static void online_service_cookie_jar_hands_caught_in_the_cookie_jar(SoupCookieJar *cookie_jar, SoupCookie *old_cookie, SoupCookie *new_cookie, OnlineService *service);
static void online_service_cookie_jar_open(OnlineService *service);

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define ONLINE_SERVICE_PREFIX			PREFS_PATH "/auth/%s"
#define	ONLINE_SERVICE_PASSWORD			ONLINE_SERVICE_PREFIX "/password"
#define	ONLINE_SERVICE_AUTO_CONNECT		ONLINE_SERVICE_PREFIX "/auto_connect"
#define	ONLINE_SERVICE_HTTPS			ONLINE_SERVICE_PREFIX "/https"
#define	ONLINE_SERVICE_ENABLED			ONLINE_SERVICE_PREFIX "/enabled"
#define ONLINE_SERVICE_IDS_TWEETS		ONLINE_SERVICE_PREFIX "/tweet-ids%s/last_id"


OnlineService *selected_service=NULL;

unsigned long int in_reply_to_status_id=0;
OnlineService *in_reply_to_service=NULL;

#define	DEBUG_DOMAINS	"OnlineServices:UI:Network:Tweets:Requests:Users:Authentication:OnlineService.c"
#include "debug.h"

#define MAX_LOGINS 5


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
OnlineService *online_service_open(const gchar *account_key){
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
	service->authenticated=TRUE;
	service->session=NULL;
	service->logins=0;
	service->id_last_tweet=service->id_last_dm=service->id_last_reply=0;
	
	service->timer=timer_new();
	
	debug("Loading OnlineService settings for: '%s'" , service->decoded_key);
	
	gchar *encoded_username=g_uri_escape_string(service->username, NULL, TRUE);
	gchar *encoded_uri=g_uri_escape_string(service->uri, NULL, TRUE);
	service->key=g_strdup_printf("%s@%s", encoded_username, encoded_uri );
	g_free(encoded_username);
	g_free(encoded_uri);
	
	if(g_str_equal(service->uri, "twitter.com"))
		service->which_rest=Twitter;
	else
		service->which_rest=Laconica;
	
	gchar *prefs_auth_path=NULL;
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_PREFIX, service->key);
	debug("Account's gconf path:\t\t [%s]", prefs_auth_path );
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_ENABLED, service->key);
	service->enabled=gconfig_if_bool(prefs_auth_path, TRUE);
	g_free(prefs_auth_path);
	debug("%sabling [%s] OnlineService.", (service->enabled ?"En" :"Dis" ), service->decoded_key);
	
	service->password=NULL;
	
#ifdef HAVE_GNOME_KEYRING
	if(!(keyring_get_password(&service)))
		service->password=NULL;
#else
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_PASSWORD, service->key);
	gconfig_get_string(prefs_auth_path, &service->password);
	g_free(prefs_auth_path);
#endif
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_HTTPS, service->key);
	service->https=gconfig_if_bool(prefs_auth_path, TRUE);
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_AUTO_CONNECT, service->key);
	service->auto_connect=gconfig_if_bool(prefs_auth_path, TRUE);
	g_free(prefs_auth_path);
	
	debug("OnlineService created.\n\t\taccount '%s(=%s)'\t\t\t[%sabled]\n\t\tservice uri: %s over https: [%s]; username: %s; password: %s; auto_connect: [%s]", service->decoded_key, service->key, (service->enabled?"en":"dis"), service->uri, (service->https ?_("TRUE") :_("FALSE")), service->username, service->password, (service->auto_connect?"TRUE":"FALSE"));
	
	return service;
}/*online_service_open*/

OnlineService *online_service_new(gboolean enabled, const gchar *uri, gboolean https, const gchar *username, const gchar *password, gboolean auto_connect){
	debug("Creating new OnlineService for '%s@%s'.", username, uri);
	OnlineService *service=g_new0(OnlineService, 1);
	service->session=NULL;
	
	service->connected=FALSE;
	service->authenticated=TRUE;
	service->enabled=enabled;
	service->logins=0;
	
	service->id_last_tweet=service->id_last_dm=service->id_last_reply=0;
	
	service->timer=timer_new();
	service->auto_connect=auto_connect;
	
	service->decoded_key=g_strdup_printf("%s@%s", username, uri );
	service->username=g_strdup(username);
	service->password=g_strdup(password);
	service->https=https;
	
	service->friends=service->followers=service->friends_and_followers=NULL;
	
	service->uri=g_strdup(uri);
	gchar **parsed_uri=g_strsplit(service->uri, "/", 1);
	service->server=g_strdup(parsed_uri[0]);
	if(G_STR_EMPTY(parsed_uri[1]))
		service->path=g_strdup("");
	else
		service->path=g_strdup(parsed_uri[1]);
	g_strfreev(parsed_uri);
	
	gchar *encoded_username=g_uri_escape_string(username, NULL, TRUE);
	gchar *encoded_uri=g_uri_escape_string(uri, NULL, TRUE);
	service->key=g_strdup_printf("%s@%s", encoded_username, encoded_uri);
	g_free(encoded_username);
	g_free(encoded_uri);
	
	if(!(strcasecmp(service->server, "twitter.com")))
		service->which_rest=Twitter;
	else
		service->which_rest=Laconica;
	
	return service;
}/*online_service_new*/

gboolean online_service_save(OnlineService *service){
	debug("Preparing to save OnlineService for '%s'.", service->decoded_key);
	
	gchar *prefs_auth_path=NULL;
	
	debug("Saving '%s' 'enabled' status: %s.", service->decoded_key, (service->enabled ?"TRUE" :"FALSE" ) );
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_ENABLED, service->key);
	gconfig_set_bool(prefs_auth_path, service->enabled);
	g_free(prefs_auth_path);
	
	debug("Saving '%s' password: '%s'.", service->decoded_key, service->password );
#ifdef HAVE_GNOME_KEYRING
	keyring_set_password(service);
#else
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_PASSWORD, service->key);
	gconfig_set_string(prefs_auth_path, service->password);
	g_free(prefs_auth_path);
#endif
	
	debug("Saving whether to use https when connecting to '%s': %s.", service->decoded_key, (service->https ?"TRUE" :"FALSE" ) );
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_HTTPS, service->key);
	gconfig_set_bool(prefs_auth_path, service->https);
	g_free(prefs_auth_path);
	
	debug("Saving '%s' 'auto_connect' status: %s.", service->decoded_key, (service->auto_connect ?"TRUE" :"FALSE" ) );
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_AUTO_CONNECT, service->key);
	gconfig_set_bool(prefs_auth_path, service->auto_connect);
	g_free(prefs_auth_path);
	
	debug("OnlineService saved.\n\t\taccount '%s(=%s)'\t\t\t[%sabled]\n\t\tservice uri: %s over https: [%s]; username: %s; password: %s; auto_connect: [%s]", service->decoded_key, service->key, (service->enabled?"en":"dis"), service->uri, (service->https ?_("TRUE") :_("FALSE")), service->username, service->password, (service->auto_connect?"TRUE":"FALSE"));

	debug("Attempting to connect to OnlineService for: '%s'.", service->decoded_key);
	online_service_reconnect(service);
	
	return TRUE;
}/*online_service_save*/

gboolean online_service_delete(OnlineService *service, gboolean service_cache_rm_rf){
	gchar *prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_PREFIX, service->key);
	debug("Account's gconf path:\t\t [%s]", prefs_auth_path );
	if(!(gconfig_rm_rf(prefs_auth_path))){
		debug("**ERROR:** Failed to delete '%s' gconf auth prefernces.  GConf key used: [%s].", service->decoded_key, prefs_auth_path);
		uber_free(prefs_auth_path);
		return FALSE;
	}
	uber_free(prefs_auth_path);
	
	gchar *cache_dir=NULL;
	if(!service_cache_rm_rf){
		debug("Deleting OnlineService's user cache directory for '%s'.", service->decoded_key);
		cache_dir=g_build_filename("services", service->uri, service->username, NULL);
	}else{
		debug("Deleting OnlineService's cache directory [%s].", service->uri);
		cache_dir=g_build_filename("services", service->uri, NULL);
	}
	
	cache_dir_clean_up(cache_dir, TRUE);
	uber_free(cache_dir);
	
	debug("Destroying OnlineService: [%s].", service->decoded_key);
	online_service_free(service);
	return TRUE;
}/*online_service_delete*/

void online_service_load_tweet_ids(OnlineService *service, const gchar *uri){
	gchar *timeline=g_strrstr(uri, "/");
	
	gchar *prefs_path_ids=NULL;
	gint id_swap=0;
	
	prefs_path_ids=g_strdup_printf(ONLINE_SERVICE_IDS_TWEETS, service->key, timeline);
	gconfig_get_int(prefs_path_ids, &id_swap);
	uber_free(prefs_path_ids);
	if(id_swap>0) service->id_last_tweet=id_swap;
	
	if(service->id_last_dm==0){
		id_swap=0;
		prefs_path_ids=g_strdup_printf(ONLINE_SERVICE_IDS_TWEETS, service->key, "/dm");
		gconfig_get_int(prefs_path_ids, &id_swap);
		uber_free(prefs_path_ids);
		if(id_swap>0) service->id_last_dm=id_swap;
	}
	
	if(service->id_last_reply==0){
		id_swap=0;
		prefs_path_ids=g_strdup_printf(ONLINE_SERVICE_IDS_TWEETS, service->key, "/replies");
		gconfig_get_int(prefs_path_ids, &id_swap);
		uber_free(prefs_path_ids);
		if(id_swap>0) service->id_last_reply=id_swap;
	}
	
	debug("Loaded last tweet ids for '%s'.  Last DM ID: %u; Last Reply ID: %u; Timeline: %s; Last Tweet ID: %u.", service->decoded_key, service->id_last_dm, service->id_last_reply, timeline, service->id_last_tweet);
}/*online_service_load_tweet_ids(service, uri);*/

void online_service_save_tweet_ids(OnlineService *service, const gchar *uri){
	gchar *timeline=g_strrstr(uri, "/");
	debug("Saving last tweet ids for '%s'.  Last DM ID: %u; Last Reply ID: %u; Timeline: %s; Last Tweet ID: %u.", service->decoded_key, service->id_last_dm, service->id_last_reply, timeline, service->id_last_tweet);
	
	gchar *prefs_path_ids=NULL;
	
	if(service->id_last_tweet>0){
		prefs_path_ids=g_strdup_printf(ONLINE_SERVICE_IDS_TWEETS, service->key, timeline);
		gconfig_set_int(prefs_path_ids, (gint)service->id_last_tweet);
		uber_free(prefs_path_ids);
	}
	
	if(service->id_last_dm>0){
		prefs_path_ids=g_strdup_printf(ONLINE_SERVICE_IDS_TWEETS, service->key, "/dm");
		gconfig_set_int(prefs_path_ids, (gint)service->id_last_dm);
		uber_free(prefs_path_ids);
	}
	
	if(service->id_last_reply>0){
		prefs_path_ids=g_strdup_printf(ONLINE_SERVICE_IDS_TWEETS, service->key, "/replies");
		gconfig_set_int(prefs_path_ids, (gint)service->id_last_reply);
		uber_free(prefs_path_ids);
	}
}/*online_service_save_tweet_ids(service, uri);*/

gboolean online_service_connect(OnlineService *service){
	debug("Creating '%s' connection to '%s'", service->decoded_key, service->uri);
	if(!( (service->session=soup_session_sync_new_with_options(SOUP_SESSION_MAX_CONNS_PER_HOST, 8, SOUP_SESSION_TIMEOUT, 20, SOUP_SESSION_IDLE_TIMEOUT, 20, NULL)) )){
		debug("**ERROR:** Failed to creating a new soup session for '%s'.", service->decoded_key);
		service->session=NULL;
		return (service->connected=service->authenticated=FALSE);
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
	
	proxy_attach_online_service(service);
	
	online_service_cookie_jar_open(service);
	
	return (service->connected=service->authenticated=TRUE);
}/*online_service_connect*/

static void online_service_cookie_jar_open(OnlineService *service){
	SoupCookieJar	*cookie_jar=NULL;
	gchar		*cookie_jar_filename=NULL;
	
	debug("Connecting cookie jar to online service: [%s].", service->key);
		
	if(!( (cookie_jar_filename=cache_create_cookie_jar(service)) ))
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

gboolean online_service_reconnect(OnlineService *service){
	if(service->connected)
		online_service_disconnect(service, TRUE);
	
	return online_service_connect(service);
}/*online_service_reconnect*/

void online_service_disconnect(OnlineService *service, gboolean no_state_change){
	debug("Closing %s's connection to: %s", service->decoded_key, service->uri);
	if(service->session){
		if(SOUP_IS_SESSION(service->session))
			soup_session_abort(service->session);
		
		uber_unref(service->session);
	}
	service->connected=service->authenticated=FALSE;
	service->logins=0;
	debug("Disconnected from OnlineService [%s].", service->decoded_key);
	online_services_decrement_connected(online_services, no_state_change);
}/*online_service_disconnect*/

/* Login to service. */
gboolean online_service_login(OnlineService *service, gboolean temporary_connection){
	debug("Attempting to log in to %s...", service->decoded_key);
	if(!(service->enabled && service->connected)) return FALSE;
	
	if(!(service->auto_connect || temporary_connection)) return FALSE;
	
	if(!SOUP_IS_SESSION(service->session)){
		debug("**ERROR**: Unable to authenticating OnlineService: %s. Invalide libsoup session.", (service->decoded_key ?service->decoded_key :"invalid service") );
		return FALSE;
	}
	
	app_statusbar_printf("Connecting to %s...", service->key);
	
	/* Verify cedentials */
	debug("Logging into: '%s'. username: %s, password: %s.", service->decoded_key, service->username, service->password);
	
	online_service_request(service, QUEUE, API_LOGIN, online_service_login_check, API_LOGIN, NULL);
	debug("OnlineService: %s.  Status: authenticated: [%s].", service->decoded_key, (service->authenticated ?"TRUE" :"FALSE" ) );
	if(!temporary_connection) online_services_increment_connected(online_services);
	
	return TRUE;
}/*online_service_login*/

/* On verify credentials */
static void online_service_login_check(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceWrapper *service_wrapper=(OnlineServiceWrapper *)user_data;
	OnlineService *service=service_wrapper->service;
	
	debug("Login response: %i",msg->status_code);
	
	/* if the check succeeds then it start the request limit timer:
	 * 	timer_main(service->timer, msg);
	 */
	if(!network_check_http(service, msg)) {
		debug("Logging into '%s'.", service->decoded_key);
	}else{
		debug("Logged into '%s'.", service->decoded_key);
	}
	online_service_wrapper_free(service_wrapper);
}/*online_service_login_check*/

/* HTTP Basic Authentication */
static void online_service_http_authenticate(SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer user_data){
	OnlineService *service=(OnlineService *)user_data;
	debug("**Authenticating:** %s.", service->key );
	if(!(service && service->key)){
		debug("**FATAL-ERROR**: Unable to authenticating OnlineService: %s. Invalid online service.", service->decoded_key);
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
	uber_free(uri);
	
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
		service->authenticated=FALSE;
	}
}/*online_service_http_authenticate*/

SoupMessage *online_service_request(OnlineService *service, RequestMethod request, const gchar *uri, SoupSessionCallback callback, gpointer user_data, gpointer formdata){
	if(!(service->enabled && service->connected && service->authenticated)){
		debug("Unable to load: %s.  You're not connected to %s.", uri, service->key);
		app_statusbar_printf("Unable to load: %s.  You're not connected to: %s.", uri, service->key);
		return NULL;
	}
	
	gchar *new_uri=g_strdup_printf("http%s://%s%s%s", (service->https ?"s" :"" ), service->uri, ( (service->which_rest==Twitter) ?"" :"/api" ), uri );
	debug("Creating new service request for: '%s', requesting: %s.", service->decoded_key, new_uri);
	SoupMessage *msg=online_service_request_uri(service, request, (const gchar *)new_uri, callback, user_data, formdata);
	g_free(new_uri);
	return msg;
}/*online_service_request*/

SoupMessage *online_service_request_uri(OnlineService *service, RequestMethod request, const gchar *uri, SoupSessionCallback callback, gpointer user_data, gpointer formdata){
	if(!(service->enabled && service->connected && service->authenticated)){
		debug("Unable to load: %s.  You're not connected to %s.", uri, service->key);
		app_statusbar_printf("Unable to load: %s.  You're not connected to: %s.", uri, service->key);
		return NULL;
	}
	
	if(!(service && service->session && SOUP_IS_SESSION(service->session) )){
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
							service_formdata=g_strdup_printf( "source=%s&in_reply_to_status_id=%lu&status=%s", ((service->which_rest==Twitter) ?SOURCE_TWITTER :SOURCE_LACONICA ), in_reply_to_status_id, form_data);
						}else{
							debug("Submitting Tweet/Status update to: [%s].\n\t\tTweet: [%s].", service->decoded_key, form_data);
							service_formdata=g_strdup_printf( "source=%s&status=%s", ((service->which_rest==Twitter) ?SOURCE_TWITTER :SOURCE_LACONICA ), form_data);
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
	OnlineServiceWrapper *service_wrapper=NULL;
	switch(request){
		case QUEUE:
		case POST:
			debug("Adding libsoup request to service: '%s' libsoup's message queue.", service->decoded_key);
			
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
	OnlineService *service=(OnlineService *)user_data;
	if(!service->authenticated) return;
	
	service->logins++;
	
	if(service->logins < MAX_LOGINS ) return;
	
	debug("**ERROR:** Cancelling restarted message, authentication failed.");
	soup_session_cancel_message(service->session, msg, 401);
	online_service_disconnect(service, FALSE);
}/*online_service_message_restarted*/

gchar *online_service_get_uri_content_type(OnlineService *service, const gchar *uri, SoupMessage **msg){
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
}/*online_service_get_uri_content_type*/

void online_service_free(OnlineService *service){
	if(!service) return;
	
	debug("Unloading instance of: %s service", service->decoded_key);
	online_service_disconnect(service, TRUE);
	
	debug("Shutting down network rate-limit timer for OnlineService\t[%s].", service->decoded_key );
	timer_free(service->timer);
	service->timer=NULL;
	
	debug("Closing %s account", service->decoded_key );
	uber_free(service->key);
	uber_free(service->uri);
	uber_free(service->username);
	uber_free(service->password);
	users_glists_free_lists(service);
	
	debug("Releasing OnlineService\t[%s].", service->decoded_key );
	uber_free(service->decoded_key);
	uber_free(service);
}/*online_service_free*/

/********************************************************
 *                       eof                            *
 ********************************************************/

