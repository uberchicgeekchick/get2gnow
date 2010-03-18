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
#define _GNU_SOURCE
#define _THREAD_SAFE


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

#include "program.h"

#include "www.h"
#include "xml.h"
#include "update-ids.h"

#include "online-services.typedefs.h"
#include "online-services.types.h"
#include "online-services.rest-uris.defines.h"
#include "online-services.h"
#include "online-service.types.h"
#include "online-service.h"

#include "online-service-wrapper.h"
#include "online-service-request.h"

#include "online-services-dialog.h"

#include "best-friends.h"

#include "tabs.h"

#include "users.types.h"
#include "users-glists.h"
#include "users.h"

#include "network.h"

#include "preferences.defines.h"
#include "proxy.h"

#ifdef HAVE_GNOME_KEYRING
#include "keyring.h"
#endif

#include "uberchick-label.h"
#include "update-viewer.h"
#include "main-window.h"
#include "gconfig.h"
#include "cache.h"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static OnlineService *online_service_constructor(const gchar *uri, const gchar *user_name);

static const gchar *micro_blogging_service_to_string(MicroBloggingService micro_blogging_service);
static void online_service_set_micro_blogging_service(OnlineService **service);

static void online_service_http_authenticate(SoupSession *session, SoupMessage *xml, SoupAuth *auth, gboolean retrying, OnlineService *service);
static void *online_service_login_check(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper);

static void online_service_set_profile(OnlineServiceWrapper *service_wrapper, SoupMessage *xml, User *user);

static void online_service_message_restarted(SoupMessage *xml, OnlineService *service);

static void online_service_cookie_jar_open(OnlineService *service);

static void online_service_request_validate_uri(OnlineService *service, gchar **requested_uri, guint attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer *user_data, gpointer *form_data);
static void online_service_request_validate_form_data(OnlineService *service, gchar **requested_uri, guint attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer *user_data, gpointer *form_data);
static gboolean online_service_form_data_replace( OnlineService *service, gpointer  **form_data, gchar *form_data_swap );



/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
typedef struct _MicroBloggingServices MicroBloggingServices;
struct _MicroBloggingServices{
	MicroBloggingService		micro_blogging_service;
	const gchar			*server;
	const gchar			*client;
};

MicroBloggingServices MicroBloggingServicesList[]={
	{Unknown,	NULL,			"get2gnow"},
	{Twitter,	"twitter.com",		"greettweetknow"},
	{StatusNet,	"*",			"get2gnow"},
	{Unsupported,	NULL,			NULL},
};

#define	ONLINE_SERVICE_PASSWORD				PREFS_ONLINE_SERVICE "/password"
#define	ONLINE_SERVICE_AUTO_CONNECT			PREFS_ONLINE_SERVICE "/auto_connect"
#define	ONLINE_SERVICE_HTTPS				PREFS_ONLINE_SERVICE "/https"
#define	ONLINE_SERVICE_ENABLED				PREFS_ONLINE_SERVICE "/enabled"
#define	ONLINE_SERVICE_POST_TO_BY_DEFAULT		PREFS_ONLINE_SERVICE "/post_to_by_default"
#define	ONLINE_SERVICE_LAST_REQUEST			PREFS_ONLINE_SERVICE "/timestamps/last_request"


#ifdef GNOME_ENABLE_DEBUG
#define	DEBUG_DISPLAY_PASSWORDS	TRUE
#else
#define	DEBUG_DISPLAY_PASSWORDS NULL
#endif


#define	DEBUG_DOMAINS	"OnlineServices:Network:Updates:Requests:Users:Authentication:Settings:Setup:Start-Up:online-service.c"
#include "debug.h"


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
gboolean online_service_open_uri(GtkWidget *widget, const gchar *uri, OnlineService *service){
	UberChickLabel *uberchick_label=NULL;
	if(!(widget && (IS_UBERCHICK_LABEL( (uberchick_label=UBERCHICK_LABEL(widget)) )) ))
		uberchick_label=NULL;
	
	if(!service){
		if(!uberchick_label) return FALSE;
		service=uberchick_label_get_service(uberchick_label);
	}
	debug("Loading: <%s>; via: <%s>", uri, service->key);
	if(!(service && G_STR_N_EMPTY(service->uri) && G_STR_N_EMPTY(uri) && g_strstr_len(uri, -1, service->uri))){
		if(G_STR_N_EMPTY(uri))
			www_open_uri(widget, uri);
		return FALSE;
	}
	
	gchar *requested_resource=NULL;
	if(!(requested_resource=online_service_get_uri_requested_resource(service, uri))){
		www_open_uri(widget, uri);
		return FALSE;
	}
	
	switch(requested_resource[0]){
		case '!':
		case '#':
			debug("OnlineServices: Searching for <%s> on <%s>", requested_resource, service->key);
			main_window_sexy_search_entry_set(requested_resource, TRUE);
			break;
		
		case '@':
		default:
			if(!uberchick_label){
				debug("OnlineServices: Inserting: <%s@%s> in to current update.", &requested_resource[1], service->uri);
				update_viewer_set_in_reply_to_data(service, Users, &requested_resource[1], NULL, 0.0, 0.0, TRUE, FALSE, FALSE);
				break;
			}
			debug("OnlineServices via UberChick_Label: Inserting: <%s@%s> in to current update.", &requested_resource[1], service->uri);
			update_viewer_set_in_reply_to_data(service, uberchick_label_get_update_type(uberchick_label), &requested_resource[1], uberchick_label_get_text(uberchick_label), uberchick_label_get_user_id(uberchick_label), uberchick_label_get_update_id(uberchick_label), TRUE, FALSE, FALSE);
			break;
	}
	uber_free(requested_resource);
	return TRUE;
}/*online_service_open_uri(service, uri);*/

gchar *online_service_get_uri_requested_resource(OnlineService *service, const gchar *uri){
	debug("Parsing requested_resource from: <%s>; for: <%s>", uri, service->key);
	if(!(service && G_STR_N_EMPTY(service->uri) && G_STR_N_EMPTY(uri) && g_strstr_len(uri, -1, service->uri)))
		return FALSE;
	
	gchar *requested_resource=NULL;
	if(!g_strstr_len(uri, -1, "search")){
		if(G_STR_EMPTY( (requested_resource=g_strrstr(uri, "/")) ))
			return NULL;
		
		if(!g_regex_match_simple("^[A-Za-z_]+$", &requested_resource[1], G_REGEX_DOLLAR_ENDONLY|G_REGEX_OPTIMIZE, 0))
			return NULL;
		
		return g_strdup_printf("@%s", &requested_resource[1]);
	}
	
	requested_resource=g_uri_unescape_string( g_strstr_len(uri, -1, "?q=")+g_utf8_strlen("?q=", -1), NULL );
	if(G_STR_EMPTY(requested_resource)){
		uber_free(requested_resource);
		return NULL;
	}
	
	return requested_resource;
}/*online_service_get_uri_service_resource(uri);*/

gboolean online_service_validate_guid(OnlineService *service, const gchar *user_name, const gchar *uri){
	if(!service) return FALSE;
	gchar *guid=g_strdup_printf("%s@%s", user_name, uri);
	if(!( service && g_str_equal(service->guid, guid) && g_str_equal(service->user_name, user_name) && g_str_equal(service->uri, uri) )){
		uber_free(guid);
		return FALSE;
	}
	uber_free(guid);
	return TRUE;
}/*online_service_validate_guid(service, "user_name", "uri");*/

static void online_service_set_micro_blogging_service(OnlineService **service){
	if(!(*service)) return;
	if((*service)->server) (*service)->micro_blogging_service=Unknown;
	
	MicroBloggingServices *micro_blogging_services=MicroBloggingServicesList;
	while(micro_blogging_services->client){
		if(G_STR_N_EMPTY(micro_blogging_services->server) && (g_str_has_prefix((*service)->server, micro_blogging_services->server) || g_str_equal((*service)->server, micro_blogging_services->server) || g_str_equal(micro_blogging_services->server, "*")) ){
			(*service)->micro_blogging_service=micro_blogging_services->micro_blogging_service;
			(*service)->micro_blogging_client=g_strdup(micro_blogging_services->client);
			return;
		}
		
		micro_blogging_services++;
	}
	
	if(!( (*service)->micro_blogging_service ))
		(*service)->micro_blogging_service=StatusNet;
}/*online_service_set_micro_blogging_service(service);*/

static const gchar *micro_blogging_service_to_string(MicroBloggingService micro_blogging_service){
	switch(micro_blogging_service){
		case Unknown: default: return _("Unkown");
		case StatusNet: return _("StatusNet");
		case Twitter: return _("Twitter");
		case Unsupported: return _("Unsupported");
	}
}/*micro_blogging_service_to_string(MicroBloggingServices micro_blogging_service)*/

static OnlineService *online_service_constructor(const gchar *uri, const gchar *user_name){
	OnlineService *service=g_new0(OnlineService, 1);
	
	service->processing_queue=NULL;
	service->processing=FALSE;
	service->processing_timer=0;
	
	service->uri=g_strdup(uri);
	
	service->user_name=g_strdup(user_name);
	service->user_profile=NULL;
	service->nick_name=NULL;
	
	service->enabled=TRUE;
	service->post_to_enabled=service->post_to_by_default=TRUE;
	service->connected=service->has_loaded=service->authenticated=FALSE;
	
	service->session=NULL;
	/* service->status is set in 'online-service-wrapper.c' after processing a request. */
	service->status=g_strdup("newly constructed");
	service->logins=0;
	
	service->password=NULL;
	
	service->best_friends=NULL;
	service->best_friends_total=0;
	
	service->friends=service->followers=service->friends_and_followers=NULL;
	service->timer=timer_new();
	
	service->guid=g_strdup_printf("%s@%s", service->user_name, service->uri);
	
	gchar *encoded_user_name=g_uri_escape_string(service->user_name, NULL, TRUE);
	gchar *encoded_uri=g_uri_escape_string(service->uri, NULL, TRUE);
	service->key=g_strdup_printf("%s@%s", encoded_user_name, encoded_uri );
	uber_free(encoded_user_name);
	uber_free(encoded_uri);
	
	gchar **parsed_uri=g_strsplit(service->uri, "/", 2);
	service->server=g_strdup(parsed_uri[0]);
	if(G_STR_EMPTY(parsed_uri[1]))
		service->path=g_strdup("");
	else
		service->path=g_strdup(parsed_uri[1]);
	g_strfreev(parsed_uri);
	
	online_service_set_micro_blogging_service(&service);
	
	return service;
}/*online_service_constructor();*/

OnlineService *online_service_open(const gchar *account_key){
	debug("Loading OnlineService instance for: '%s' account.", account_key);
	
	if(!g_strstr_len(account_key, -1, "@")){
		debug("**ERROR:** Invalid OnlineService: <%s> - skipping.", account_key);
		return NULL;
	}
	
	gchar **account_data=g_strsplit(account_key, "@", 2);
	OnlineService *service=online_service_constructor(account_data[1], account_data[0]);
	g_strfreev(account_data);
	
	gchar *prefs_auth_path=NULL;
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_ENABLED, service->key);
	service->enabled=gconfig_if_bool(prefs_auth_path, TRUE);
	uber_free(prefs_auth_path);
	
#ifdef HAVE_GNOME_KEYRING
	if(!(keyring_get_password(&service)))
		service->password=g_strdup("");
#else
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_PASSWORD, service->key);
	gconfig_get_string(prefs_auth_path, &service->password);
	uber_free(prefs_auth_path);
#endif
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_HTTPS, service->key);
	service->https=gconfig_if_bool(prefs_auth_path, TRUE);
	uber_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_AUTO_CONNECT, service->key);
	service->auto_connect=gconfig_if_bool(prefs_auth_path, TRUE);
	uber_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_POST_TO_BY_DEFAULT, service->key);
	service->post_to_enabled=service->post_to_by_default=gconfig_if_bool(prefs_auth_path, TRUE);
	uber_free(prefs_auth_path);
	
	online_service_display_debug_details(service, FALSE, "opened");
	
	best_friends_load(service);
	
	return service;
}/*online_service_open("user@service.uri");*/

OnlineService *online_service_new(const gchar *uri, const gchar *user_name, const gchar *password, gboolean enabled, gboolean https, gboolean auto_connect, gboolean post_to_by_default){
	debug("Creating new OnlineService for '%s@%s'.", user_name, uri);
	OnlineService *service=online_service_constructor(uri, user_name);
	
	service->enabled=enabled;
	service->password=g_strdup(password);
	service->https=https;
	service->post_to_enabled=service->post_to_by_default=post_to_by_default;
	service->auto_connect=auto_connect;
	
	online_service_display_debug_details(service, TRUE, "created");
	
	return service;
}/*online_service_new*/

gboolean online_service_save(OnlineService *service, const gchar *password, gboolean enabled, gboolean https, gboolean auto_connect, gboolean post_to_by_default){
	if(!service) return FALSE;
	
	service->enabled=enabled;
	service->password=g_strdup(password);
	service->https=https;
	service->post_to_enabled=service->post_to_by_default=post_to_by_default;
	service->auto_connect=auto_connect;
	
	debug("Preparing to save OnlineService.  GUID: <%s>; key: (%s).", service->guid, service->key);
	
	gchar *prefs_auth_path=NULL;
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_ENABLED, service->key);
	gconfig_set_bool(prefs_auth_path, service->enabled);
	uber_free(prefs_auth_path);
	
#ifdef HAVE_GNOME_KEYRING
	keyring_set_password(service);
#else
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_PASSWORD, service->key);
	gconfig_set_string(prefs_auth_path, service->password);
	uber_free(prefs_auth_path);
#endif
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_HTTPS, service->key);
	gconfig_set_bool(prefs_auth_path, service->https);
	uber_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_AUTO_CONNECT, service->key);
	gconfig_set_bool(prefs_auth_path, service->auto_connect);
	uber_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_POST_TO_BY_DEFAULT, service->key);
	gconfig_set_bool(prefs_auth_path, service->post_to_by_default);
	uber_free(prefs_auth_path);
	
	online_service_display_debug_details(service, FALSE, "saved");
	
	debug("Attempting to connect to OnlineService for: '%s'.", service->guid);
	
	return TRUE;
}/*online_service_save*/

gboolean online_service_delete(OnlineService *service, gboolean service_cache_rm_rf){
	if(!service) return FALSE;
	debug("Preparing to delete OnlineService.  GUID: <%s>; key: (%s).", service->guid, service->key);
	
	update_viewer_postable_online_service_delete(service);
	
	gchar *prefs_auth_path=g_strdup_printf(PREFS_ONLINE_SERVICE, service->key);
	if(!(gconfig_rm_rf(prefs_auth_path))){
		debug("**ERROR:** Failed to delete '%s' gconf auth prefernces.  GConf key used: [%s].", service->guid, prefs_auth_path);
		uber_free(prefs_auth_path);
		return FALSE;
	}
	uber_free(prefs_auth_path);
	
	gchar *cache_dir=NULL;
	if(!service_cache_rm_rf){
		debug("Deleting OnlineService's user cache directory for '%s'.", service->guid);
		cache_dir=g_build_filename("services", service->uri, service->user_name, NULL);
	}else{
		debug("Deleting OnlineService's cache directory [%s].", service->uri);
		cache_dir=g_build_filename("services", service->uri, NULL);
	}
	
	cache_dir_clean_up(cache_dir, TRUE);
	uber_free(cache_dir);
	
	tabs_remove_service(service);
	
	online_service_display_debug_details(service, FALSE, "deleted");
	online_service_free(service, TRUE);
	return TRUE;
}/*online_service_delete*/

void online_service_display_debug_details(OnlineService *service, gboolean new_service, const char *action){
	if(!service) return;
	gchar *prefs_auth_path=g_strdup_printf(PREFS_ONLINE_SERVICE, service->key);
	debug("OnlineService: <%s>, or a child request, has been <%s>.  GCONF path: [%s]", action, (new_service ?"created" :"existing"), prefs_auth_path );
	g_free(prefs_auth_path);
	debug("OnlineService account details guid: %s; key: %s.'\n\t\t\t[%sabled]", service->guid, service->key, (service->enabled?"en":"dis") );
	debug("Micro-Blogging Service: %s @ service uri: %s", micro_blogging_service_to_string(service->micro_blogging_service), service->uri );
	debug("Connectng using: https: [%s]; user_name: %s; password: %s; auto_connect: [%s]", (service->https ?_("TRUE") :_("FALSE")), service->user_name, (DEBUG_DISPLAY_PASSWORDS ?service->password :"[*passwords are hidden out side of debug mode*]"), (service->auto_connect ?_("TRUE") :_("FALSE") ) );
	debug("By default all updates will%s be posted to this OnlineService.", (service->post_to_by_default?"" :" not"));
	debug("Posting updates to <%s> is %sabled.", service->key, (service->post_to_enabled?"en" :" dis"));
}/*online_service_display_debug_details(service, "action");*/

void online_service_fetch_profile(OnlineService *service, const gchar *user_name, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_user_parser_func){
	if(!service) return;
	gchar *user_profile_uri=g_strdup_printf(API_USER_PROFILE, user_name);
	online_service_request(service, QUEUE, user_profile_uri, online_service_user_parser_func, (OnlineServiceSoupSessionCallbackFunc)user_parse_profile, NULL, NULL);
	uber_free(user_profile_uri);
}/*online_service_fetch_profile(OnlineService *service, const gchar *user_name, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)online_service_set_profile);*/

gboolean online_service_connect(OnlineService *service){
	if(!service) return FALSE;
	debug("Loaded account: '%s'.  Validating & connecting.", service->guid);
	
	if(!service->auto_connect){
		debug("%s account in not set to auto-connect.", service->guid);
		return FALSE;
	}
	
	if(!(G_STR_N_EMPTY(service->user_name) && G_STR_N_EMPTY(service->password) )){
		debug("%s account is missing its user_name (=%s) and/or password (=%s).", service->guid, service->user_name, service->password);
		return FALSE;
	}
	
	debug("Creating '%s' connection to '%s'", service->guid, service->uri);
	if(!( (service->session=soup_session_sync_new_with_options(SOUP_SESSION_MAX_CONNS_PER_HOST, 8, SOUP_SESSION_TIMEOUT, 20, SOUP_SESSION_IDLE_TIMEOUT, 20, NULL)) )){
		debug("**ERROR:** Failed to creating a new soup session for '%s'.", service->guid);
		service->session=NULL;
		return (service->connected=FALSE);
	}
	
	debug("Adding authenticating callback for: '%s'. user_name: %s, password: %s", service->guid, service->user_name, service->password);
	g_signal_connect(service->session, "authenticate", (GCallback)online_service_http_authenticate, service);
	
	online_service_cookie_jar_open(service);
	
	proxy_attach_online_service(service);
	
	return (service->connected=TRUE);
}/*online_service_connect(service);*/

static void online_service_cookie_jar_open(OnlineService *service){
	if(!service) return;
	SoupCookieJar	*cookie_jar=NULL;
	gchar		*cookie_jar_filename=NULL;
	
	debug("Connecting cookie jar to online service: [%s].", service->key);
	
	if(!( (cookie_jar_filename=cache_file_create_file_for_online_service(service, "cookies.txt", NULL)) ))
		return;

	cookie_jar=soup_cookie_jar_text_new(cookie_jar_filename, FALSE);
	
	soup_session_add_feature_by_type(service->session, soup_cookie_jar_get_type());
	soup_session_add_feature(service->session, SOUP_SESSION_FEATURE(cookie_jar));
	
	uber_free(cookie_jar_filename);
	uber_object_unref(cookie_jar);
}/*online_servce_open_cookie_jar(service);*/

/* Login to service. */
gboolean online_service_login(OnlineService *service, gboolean temporary_connection){
	if(!service) return FALSE;
	debug("Attempting to log in to %s...", service->guid);
	if(!(service->enabled && service->auto_connect && service->connected)) return FALSE;
	
	if(!SOUP_IS_SESSION(service->session)){
		debug("**ERROR**: Unable to authenticate OnlineService: <%s>. Invalid libsoup session.", (service->guid ?service->guid :"invalid service") );
		return FALSE;
	}
	
	if(!online_service_refresh(service)) return FALSE;
	
	statusbar_printf("Connecting to %s...", service->key);
	
	/* Verify cedentials */
	debug("Logging into: '%s'. user_name: %s, password: %s.", service->guid, service->user_name, service->password);
	
	online_service_request(service, QUEUE, API_LOGIN, NULL, online_service_login_check, API_LOGIN, NULL);
	
	if(!temporary_connection)
		online_services_increment_connected(service);
	
	online_service_fetch_profile(service, service->user_name, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)online_service_set_profile);
	
	return TRUE;
}/*online_service_login(service, TRUE|FALSE);*/

static void *online_service_login_check(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	if(!service) return NULL;

	debug("OnlineService: <%s>'s http status: %d.  Status: authenticated: [%s].", service->guid, xml->status_code, (service->authenticated ?"TRUE" :"FALSE" ) );
	if( xml->status_code==504 && !online_service_wrapper_get_attempt(service_wrapper) ){
		debug("Log-in to <%s> timed out. Reattemping authentication.", service->guid);
		online_service_wrapper_reattempt(service_wrapper);
		return NULL;
	}

	gchar *error_message=NULL;
	if(!xml_error_check(service, online_service_wrapper_get_requested_uri(service_wrapper), xml, &error_message)){
		debug("Logging on to <%s> failed.  Please check your user name and/or password. %s said: %s(#%d).", service->guid, service->uri, xml->reason_phrase, xml->status_code );
		statusbar_printf("Logging on to <%s> failed.  Please check your user name and/or password. %s said: %s(#%d).", service->guid, service->uri, xml->reason_phrase, xml->status_code );
		service->authenticated=FALSE;
	}else{
		debug("Logged on to <%s> succesfully.", service->guid);
		statusbar_printf("Logged on to <%s> succesfully.", service->guid);
		service->authenticated=TRUE;
	}
	
	uber_free(error_message);
	
	return NULL;
}/*online_service_login_check(service->session, xml, online_service_wrapper);*/

static void online_service_set_profile(OnlineServiceWrapper *service_wrapper, SoupMessage *xml, User *user){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	if(!service) return;
	if(service->user_profile && service->nick_name){
		if(user) user_free(user);
		return;
	}
	if(!user){
		debug("Failed to validate user profile for <%s>.  Please check your user name and/or password.  %s said: %s(#%d).", service->guid, service->uri, xml->reason_phrase, xml->status_code );
		service->nick_name=g_strdup(service->user_name);
		service->has_loaded=service->authenticated=FALSE;
		service->user_profile=NULL;
		return;
	}
	service->connected=service->authenticated=TRUE;
	service->has_loaded=TRUE;
	service->nick_name=g_strdup(user->nick_name);
	update_viewer_postable_online_services_append(service);
	debug("Setting nick_name for: %s to %s.", service->key, service->nick_name);
	service->user_profile=user;
}/*online_service_set_profile(online_service_wrapper, xml, user);*/

static void online_service_http_authenticate(SoupSession *session, SoupMessage *xml, SoupAuth *auth, gboolean retrying, OnlineService *service){
	if(!service) return;
	debug("**Authenticating:** %s.", service->key );
	if(!(service && service->key)){
		debug("**FATAL-ERROR**: Unable to authenticating OnlineService: %s. Invalid online service.", service->guid);
		return;
	}
	
	if(!service->enabled){
		debug("**WARNING:** Skipping authentication for disabled OnlineService '%s'.", service->guid);
		return;
	}
	
	if(G_STR_EMPTY(service->user_name))
		return debug("**WARNING:** Could not authenticate: %s, unknown user_name.", service->guid);
	
	/* verify that the password has been set */
	if(G_STR_EMPTY(service->password))
		return debug("**WARNING:** Could not authenticate: %s, unknown password.", service->guid);
	
	debug("OnlineService: <%s>'s http status: %i.  Status: authenticated: [%s].  Attempt #%d out of #%d maximum login attempts.", service->guid, xml->status_code, (service->authenticated ?"TRUE" :"FALSE" ), service->logins, ONLINE_SERVICE_MAX_REQUESTS);
	if(soup_auth_is_authenticated(auth) && retrying){
		if((++service->logins) > ONLINE_SERVICE_MAX_REQUESTS){
			debug("**ERROR**: Authentication attempts %d exceeded maximum attempts: %d.", service->logins, ONLINE_SERVICE_MAX_REQUESTS);
			service->authenticated=FALSE;
			return;
		}
	}
	
	debug("Authenticating OnlineService: [%s] Attempt #%d of %d maximum allowed attempts. Username: [%s]; Password: [%s]; Server: [%s].", service->key, service->logins, ONLINE_SERVICE_MAX_REQUESTS, service->user_name, service->password, service->uri);
	soup_auth_update(auth, xml, "WWW-Authenticate");
	soup_auth_authenticate(auth, service->user_name, service->password);
}/*online_service_http_authenticate(service);*/

gboolean online_service_refresh(OnlineService *service){
	if(!service) return FALSE;
	if(!(service->enabled && service->auto_connect)) return FALSE;
	
	if(service->connected) return TRUE;
	
	if(online_service_reconnect(service) && online_service_login(service, FALSE)){
		debug("Reconnected to: %s.", service->key);
		return TRUE;
	}
	
	if(!(service && service->key && service->status)) return FALSE;
	
	if(!service->connected)
		debug("Unable to reconnect to: %s%s.", service->key, service->status);
	else
		debug("Unable to log in to: %s%s.", service->key, service->status);
	
	debug("Unable to load one of <%s>'s REST resources.  You're not connected to %s.  Attempting to reconnect.", service->uri, service->guid);
	statusbar_printf("Unable to load one of <%s>'s REST resources.  You're not connected to: %s.", service->uri, service->guid);
	
	return FALSE;
}/*online_service_refresh(service);*/

gboolean online_service_validate_session(OnlineService *service, const gchar *requested_uri){
	if(!( ((service && service->enabled && service->auto_connect && service->connected)) && (service->session && SOUP_IS_SESSION(SOUP_SESSION(service->session))) )){
		debug("OnlineService <%s> has become invalid/disconnected while requesting: <%s> and will be cancelled.", (service? (G_STR_N_EMPTY(service->key) ?service->key :"") :""), requested_uri);
		if(service && service->key)
			online_service_display_debug_details(service, FALSE, "canceled");
		return FALSE;
	}
	return TRUE;
}/*online_service_validate_session(service, requested_uri);*/

void online_service_disconnect(OnlineService *service, gboolean temporary, gboolean no_state_change){
	if(!service) return;
	if(!(service->session && service->enabled && service->connected)) return;

	debug("Closing %s's connection to: %s", service->guid, service->uri);
	if(service->session){
		if(SOUP_IS_SESSION(service->session))
			soup_session_abort(service->session);
		
		uber_object_unref(service->session);
	}
	if(service->nick_name) uber_free(service->nick_name);
	if(service->user_profile){
		user_free(service->user_profile);
		service->user_profile=NULL;
	}
	service->has_loaded=service->connected=service->authenticated=FALSE;
	service->logins=0;
	debug("Disconnected from OnlineService [%s].", service->guid);
	if(!temporary)
		online_services_decrement_connected(service, no_state_change);
}/*online_service_disconnect(service, TRUE|FALSE, TRUE|FALSE);*/

gboolean online_service_reconnect(OnlineService *service){
	if(!service) return FALSE;
	if(!(service->enabled && service->auto_connect))
		return FALSE;
	
	if(service->connected || service->session)
		online_service_disconnect(service, FALSE, TRUE);
	
	return online_service_connect(service);
}/*online_service_reconnect(service);*/

gchar *online_service_request_uri_create(OnlineService *service, const gchar *uri){
	if(G_STR_EMPTY(uri)){
		debug("**ERROR:** Cannot create an OnlineServiceRequest from an empty URI");
		return NULL;
	}
	return g_strdup_printf("http%s://%s%s%s%s", (service->https ?"s" :"" ), ( g_strstr_len(uri, -1, "search.") && service->micro_blogging_service==Twitter ?"search." :"" ), service->uri, ( ( !g_str_has_prefix(uri, "/search/") && service->micro_blogging_service!=Twitter) ?"/api" :"" ), (G_STR_N_EMPTY(uri) ?uri :"") );
}/*online_service_request_uri_create(service, uri);*/

SoupMessage *online_service_request(OnlineService *service, RequestMethod request_method, const gchar *uri, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data){
	if(!service) return NULL;
	if(!( G_STR_N_EMPTY(uri) && service->enabled )) return NULL;
	if(request_method==POST && !service->post_to_enabled) return NULL;
	
	gchar *new_uri=online_service_request_uri_create(service, uri);
	debug("Creating new service request for: '%s', requesting: %s.", service->guid, new_uri);
	SoupMessage *xml=online_service_request_uri(service, request_method, (const gchar *)new_uri, 0, online_service_soup_session_callback_return_processor_func, callback, user_data, form_data);
	g_free(new_uri);
	return xml;
}/*online_service_request(service, QUEUE, API_LOGIN, NULL, online_service_login_check, API_LOGIN, NULL);*/

SoupMessage *online_service_request_uri(OnlineService *service, RequestMethod request_method, const gchar *uri, guint8 attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data){
	if(!service) return NULL;
	if(!( G_STR_N_EMPTY(uri) && service->enabled )) return NULL;
	if(request_method==POST && !service->post_to_enabled) return NULL;
	
	if(!(service->connected)){
		if(!online_service_refresh(service)){
			debug("Unable to load: %s.  You're not connected to %s.", uri, service->key);
			statusbar_printf("Unable to load: %s.  You're not connected to: %s.", uri, service->key);
			return NULL;
		}
	}
	
	time_t current_time=time(NULL);
	gchar *datetime=ctime(&current_time);
	gchar *prefs_path=g_strdup_printf(ONLINE_SERVICE_LAST_REQUEST, service->key);
	gconfig_set_string(prefs_path, datetime);
	uber_free(prefs_path);
	
	gchar *requested_uri=NULL;
	if(!g_strstr_len(uri, 10, "://"))
		requested_uri=g_strdup_printf("http://%s", uri);
	else
		requested_uri=g_strdup(uri);
	
	if(!online_service_validate_session(service, requested_uri)){
		uber_free(requested_uri);
		return NULL;
	}
	
	SoupMessage *xml=NULL;
	const gchar *request_string=online_service_request_method_to_string(request_method);
	
	debug("Creating libsoup request for service: '%s'.", service->guid);
	switch(request_method){
		case GET:
		case QUEUE:
			online_service_request_validate_uri(service, &requested_uri, attempt, online_service_soup_session_callback_return_processor_func, callback, &user_data, &form_data);
			debug("%s: %s", request_string, requested_uri);
			xml=soup_message_new("GET", requested_uri);
			break;
		
		case POST:
			online_service_request_validate_form_data(service, &requested_uri, attempt, online_service_soup_session_callback_return_processor_func, callback, &user_data, &form_data);
			debug("form_data: [%s]", (gchar *)form_data);
			xml=soup_message_new("POST", requested_uri);
			
			if(g_strstr_len(requested_uri, -1, service->uri)){
				soup_message_headers_append(xml->request_headers, "X-Twitter-Client", PACKAGE_NAME);
				soup_message_headers_append(xml->request_headers, "X-Twitter-Client-Version", PACKAGE_VERSION);
			}
			
			if(form_data)
				soup_message_set_request(
					xml,
					"application/x-www-form-urlencoded",
					SOUP_MEMORY_COPY,
					(gchar *)form_data,
					(strlen( (gchar *)form_data ) )
				);
			break;
	}
	
	if(!( SOUP_IS_SESSION(service->session) && SOUP_IS_MESSAGE(xml) )){
		debug("Unable to process libsoup request for service: '%s'. Attempting to %s: %s", service->guid, request_string, requested_uri);
		uber_free(requested_uri);
		return NULL;
	}
	
	switch(request_method){
		case QUEUE:
		case POST:
			debug("Creating and Queueing OnlineServiceWrapper request.");
			online_service_wrapper_init(service, xml, request_method, requested_uri, attempt, online_service_soup_session_callback_return_processor_func, callback, user_data, form_data);
			break;
			
		case GET:
			debug("Sending <%s>'s libsoup %s request for: [%s].", service->guid, request_string, requested_uri );
			OnlineService *new_service=online_service_open(service->key);
			online_service_connect(new_service);
			soup_session_send_message(new_service->session, xml);
			online_service_disconnect(new_service, TRUE, TRUE);
			online_service_free(new_service, TRUE);
			break;
	}
	
	uber_free(requested_uri);
	return xml;
	g_signal_connect(xml, "restarted", (GCallback)online_service_message_restarted, service);
}/*online_service_request_uri(service, QUEUE, API_LOGIN, 0, NULL, online_service_login_check, API_LOGIN, NULL);*/

static void online_service_request_validate_uri(OnlineService *service, gchar **requested_uri, guint attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer *user_data, gpointer *form_data){
	if(!service) return;
	if(!(
		!attempt
		&&
		callback!=NULL
		&&
		callback==network_display_timeline
	))
		return;
	
	if(g_strstr_len(*requested_uri, -1, "?since_id=")) return;
	
	UberChickTreeView *uberchick_tree_view=(UberChickTreeView *)*user_data;
	UpdateType update_type=uberchick_tree_view_get_update_type(uberchick_tree_view);
	gint8 has_loaded=uberchick_tree_view_has_loaded(uberchick_tree_view);
	if(!( (*user_data) )) return;
	
	const gchar *timeline=g_strstr_len(*requested_uri, -1, "/");
	
	gdouble newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	update_ids_get(service, timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	
	const gchar *requesting;
	gdouble since_id=0;
	debug("Updating request uri for <%s> to new updates posted to %s which has loaded %i.", service->key, *requested_uri, has_loaded);
	if( has_loaded && uberchick_tree_view_get_total(uberchick_tree_view) ){
		requesting=_("new");
		since_id=newest_update_id;
	}else if(update_type==DMs || update_type==Replies){
		requesting=_("total");
		since_id=oldest_update_id;
	}else if(update_type==BestFriends){
		requesting=_("best friend's newest");
		since_id=unread_update_id;
	}else
		return;
	
	if(!since_id) return;
	
	gchar *update_str=gdouble_to_str(since_id);
	gchar *request_uri_swap=g_strdup_printf("%s?since_id=%s", *requested_uri, update_str);
	g_free(*requested_uri);
	*requested_uri=request_uri_swap;
	
	debug("Requesting <%s>'s timeline: %s; %s updates since: %f (using string: %s).", service->key, timeline, requesting, since_id, update_str);
	uber_free(update_str);
	request_uri_swap=NULL;
}/*online_service_request_validate_uri(service, &requested_uri, attempt, callback, &user_data, &form_data);*/

static void online_service_request_validate_form_data(OnlineService *service, gchar **requested_uri, guint attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer *user_data, gpointer *form_data){
	if(!service) return;
	if(!(
		!attempt
		&&
		callback!=NULL
		&&
		callback==network_update_posted
	))
		return;
	
	if(!( (*form_data) && (*user_data) )) return;
	
	gboolean free_form_data=FALSE;
	if(!(free_form_data=online_service_form_data_replace( service, &form_data, NULL ) ))
		debug("Resetting form data free gboolean to ensure that only the 3rd & further call will free the orginal form data pointer");
	if(service->user_name && service->nick_name && g_strstr_len( (gchar *)(*form_data), -1, "/me" ) ){
		gint replace=0;
		gconfig_get_int_or_default(PREFS_UPDATES_REPLACE_ME_W_NICK, &replace, 2);
		if(replace!=-1){
			debug("Auto-replacement trigger match '/me' to be replaced with user nick.");
			debug("/me auto-replacement triggered.  Replacing '/me' with: '%s'", service->user_name);
			gchar **form_data_parts=g_strsplit( (gchar *)(*form_data), "/me", -1);
			gchar *replace_with=g_strdup_printf("*%s", ( replace==1 ? service->nick_name : service->user_name ) );
			
			free_form_data=online_service_form_data_replace( service, (&form_data), g_strjoinv(replace_with, form_data_parts) );
			
			g_strfreev(form_data_parts);
			uber_free(replace_with);
		}
	}
	
	if( g_str_has_prefix(*form_data, "*") && service->micro_blogging_service==StatusNet )
		free_form_data=online_service_form_data_replace( service, (&form_data), g_strdup_printf(" %s", (gchar *)(*form_data) ) );
	
	free_form_data=online_service_form_data_replace( service, (&form_data), g_uri_escape_string((*form_data), NULL, TRUE) );
	
	debug("Posting update to: [%s].", service->key);
	if(!g_str_equal((gchar *)(*user_data), "post->update")){
		debug("Sending direct message to: <%s@%s> from: <%s>", (gchar *)(*user_data), service->uri, service->key);
		free_form_data=online_service_form_data_replace( service, (&form_data), g_strdup_printf("source=%s&user=%s&text=%s", service->micro_blogging_client, (gchar *)(*user_data), (gchar *)(*form_data) ) );
	}else if(!(in_reply_to_status_id && in_reply_to_service==service)){
		debug("Posting update to: <%s>", service->key);
		free_form_data=online_service_form_data_replace( service, (&form_data), g_strdup_printf("source=%s&status=%s", service->micro_blogging_client, (gchar *)(*form_data) ) );
	}else{
		gchar *in_reply_to_status_id_str=gdouble_to_str(in_reply_to_status_id);
		debug("Replying to Update: #%f (using string: %s).", in_reply_to_status_id, in_reply_to_status_id_str);
		free_form_data=online_service_form_data_replace( service, (&form_data), g_strdup_printf("source=%s&in_reply_to_status_id=%s&status=%s", service->micro_blogging_client, in_reply_to_status_id_str, (gchar *)(*form_data) ) );
		uber_free(in_reply_to_status_id_str);
		in_reply_to_service=NULL;
		in_reply_to_status_id=0.0;
	}
	
	
	debug("Posting <%s>'s Status Update: [%s].", service->key, (gchar *)(*form_data));
}/*online_service_request_validate_form_data(service, &requested_uri, attempt, callback, &user_data, &form_data);*/

static gboolean online_service_form_data_replace( OnlineService *service, gpointer  **form_data, gchar *form_data_swap ){
	static gboolean free_form_data=FALSE;
	if(!( service && service->key && (*form_data) && form_data_swap )) return (free_form_data=FALSE);
	
	if(free_form_data) uber_free( **form_data );
	(**form_data)=form_data_swap;
	
	return (free_form_data=TRUE);
}/*online_service_form_data_replace( service, *form_data, &form_data_swap, &form_data_free );*/

static void online_service_message_restarted(SoupMessage *xml, OnlineService *service){
	if(!service) return;
	if(!( ( (SOUP_IS_MESSAGE(xml)) && xml->status_code!=401 ) && (service->authenticated && service->has_loaded) )) return;
	
	service->logins++;
	
	if(service->logins < ONLINE_SERVICE_MAX_REQUESTS) return;
	
	debug("**ERROR:** Cancelling restarted message, authentication failed.");
	soup_session_cancel_message(service->session, xml, 401);
	online_service_disconnect(service, FALSE, FALSE);
}/*onlin_service_message_restarted*/

void online_service_free(OnlineService *service, gboolean no_state_change){
	if(!service) return;
	
	debug("Unloading instance of: <%s> service", service->guid);
	debug("Cancelling any queued requests using <%s>'s connection.", service->key);
	g_list_foreach(service->processing_queue, (GFunc)online_service_wrapper_free, GUINT_TO_POINTER(FALSE));
	
	debug("Shutting <%s>'s rate-limit timer", service->guid );
	timer_free(service->timer);
	service->timer=NULL;
	
	debug("Disconnecting <%s>", service->key);
	if(service->session && service->connected)
		online_service_disconnect(service, FALSE, no_state_change);
	
	if(service->user_profile){
		user_free(service->user_profile);
		service->user_profile=NULL;
	}
	
	debug("Closing %s account", service->guid );
	if(!users_glists_free_lists(service, GetBoth)){
		users_glists_free_lists(service, GetFriends);
		users_glists_free_lists(service, GetFollowers);
	}
	
	debug("Destroying OnlineService <%s> object.", service->guid );
	uber_object_free(&service->key, &service->uri, &service->user_name, &service->nick_name, &service->password, &service->guid, &service->micro_blogging_client, &service->status, &service, NULL);
}/*online_service_free(online_service, (TRUE|FALSE));*/

/********************************************************
 *                       eof                            *
 ********************************************************/

