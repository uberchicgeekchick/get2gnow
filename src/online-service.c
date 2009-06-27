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
#include "network.h"

#include "tweets.h"
#include "users.h"

#include "proxy.h"
#include "parser.h"

#ifdef HAVE_GNOME_KEYRING
#include "keyring.h"
#endif

#include "main-window.h"
#include "gconfig.h"
#include "cache.h"
#include "preferences.h"
#include "online-services-dialog.h"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
typedef struct _MicroBloggingServices MicroBloggingServices;
typedef enum _MicroBloggingService MicroBloggingService;

enum _MicroBloggingService{
	Unknown = 0,
	Identica = 1,
	Twitter = 2,
	Laconica = 3,
	Unsupported = 4,
};

struct _MicroBloggingServices{
	MicroBloggingService		micro_blogging_service;
	const gchar			*server;
	const gchar			*client;
};

MicroBloggingServices MicroBloggingServicesList[]={
	{Unknown,	NULL,			"get2gnow"},
	{Identica,	"identi.ca",		"get2gnow"},
	{Twitter,	"twitter.com",		"greettweetknow"},
	{Laconica,	"*",			"get2gnow"},
	{Unsupported,	NULL,			NULL},
};

struct _OnlineService{
	SoupSession			*session;
	RateLimitTimer			*timer;
	
	gboolean			authenticated;
	gboolean			connected;
	gboolean			has_loaded;
	guint				logins;
	
	gboolean			enabled;
	gboolean			auto_connect;
	
	gchar				*key;
	gchar				*guid;
	MicroBloggingService		micro_blogging_service;
	
	gboolean			https;
	gchar				*uri;
	gchar				*server;
	gchar				*path;
	gchar				*username;
	gchar				*nickname;
	gchar				*password;
	
	GList				*friends;
	GList				*followers;
	GList				*friends_and_followers;
	
	gchar				*status;
};

static OnlineService *online_service_constructor(void);

static const gchar *micro_blogging_service_to_string(MicroBloggingService micro_blogging_service);
static void online_service_set_micro_blogging_service(OnlineService *service);

static void online_service_http_authenticate(SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer user_data);
static void *online_service_login_check(SoupSession *session, SoupMessage *msg, OnlineServiceWrapper *service_wrapper);

static void online_service_get_profile(OnlineService *service);
static void online_service_set_profile(OnlineServiceWrapper *service_wrapper, User *user);

static void online_service_message_restarted(SoupMessage *msg, gpointer user_data);

static void online_service_cookie_jar_open(OnlineService *service);

static void online_service_request_validate_uri(OnlineService *service, gchar **request_uri, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer *user_data, gpointer *form_data);
static void online_service_request_validate_form_data(OnlineService *service, gchar **request_uri, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer *user_data, gpointer *form_data);

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define ONLINE_SERVICE_PREFIX			GCONF_PATH "/online-services/%s"

#define	ONLINE_SERVICE_PASSWORD			ONLINE_SERVICE_PREFIX "/password"
#define	ONLINE_SERVICE_AUTO_CONNECT		ONLINE_SERVICE_PREFIX "/auto_connect"
#define	ONLINE_SERVICE_HTTPS			ONLINE_SERVICE_PREFIX "/https"
#define	ONLINE_SERVICE_ENABLED			ONLINE_SERVICE_PREFIX "/enabled"

#define ONLINE_SERVICE_IDS_TWEETS		ONLINE_SERVICE_PREFIX	"/xml-cache%s/%s"


OnlineService *selected_service=NULL;

gdouble in_reply_to_status_id=0.0;
OnlineService *in_reply_to_service=NULL;

#define	DEBUG_DOMAINS	"OnlineServices:Network:Tweets:Requests:Users:Settings:Authentication:Settings:Setup:Start-Up:OnlineService.c"
#include "debug.h"

#define MAX_LOGINS 5


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
gboolean online_service_validate_key(OnlineService *service, const gchar *guid){
	if(!( service && g_str_equal(service->guid, guid) )) return FALSE;
	return TRUE;
}/*online_service_validate_key(service, guid);*/

const gchar *online_service_get_guid(OnlineService *service){
	if(!service) return NULL;
	return service->guid;
}/*online_service_get_guid(service);*/

const gchar *online_service_get_key(OnlineService *service){
	if(!service) return NULL;
	return service->key;
}/*online_service_get_key(service);*/

const gchar *online_service_get_uri(OnlineService *service){
	if(!service) return NULL;
	return service->uri;
}/*online_service_get_uri(service);*/

const gchar *online_service_get_username(OnlineService *service){
	if(!service) return NULL;
	return service->username;
}/*online_service_get_username(service);*/

const gchar *online_service_get_nickname(OnlineService *service){
	if(!service) return NULL;
	return service->nickname;
}/*online_service_get_username(service);*/

const gchar *online_service_get_password(OnlineService *service){
	if(!service) return NULL;
	return service->password;
}/*online_service_get_password(service);*/


gboolean online_service_is_enabled(OnlineService *service){
	if(!service) return FALSE;
	return service->enabled;
}/*online_service_is_enabled(service);*/

gboolean online_service_is_secure(OnlineService *service){
	if(!service) return FALSE;
	return service->https;
}/*online_service_is_secure(service);*/

gboolean online_service_is_auto_connected(OnlineService *service){
	if(!service) return FALSE;
	return service->auto_connect;
}/*online_service_is_auto_connected(service);*/


gboolean online_service_is_connected(OnlineService *service){
	if(!service) return FALSE;
	return service->connected;
}/*online_service_is_connected(service);*/


gboolean online_service_has_loaded(OnlineService *service){
	if(!service) return FALSE;
	return service->has_loaded;
}/*online_service_has_loaded(service);*/


SoupSession *online_service_get_session(OnlineService *service){
	if(!service) return NULL;
	return service->session;
}/**online_service_get_session(service);*/


const gchar *online_service_get_micro_blogging_client(OnlineService *service){
	if(!service) return NULL;
	MicroBloggingServices *micro_blogging_services=MicroBloggingServicesList;
	while(micro_blogging_services->client){
		if(service->micro_blogging_service==micro_blogging_services->micro_blogging_service)
			return micro_blogging_services->client;
		
		micro_blogging_services++;
	}
	return NULL;
}/*online_service_get_micro_blogging_client(service);*/

static void online_service_set_micro_blogging_service(OnlineService *service){
	if(!service) return;
	if(service->server) service->micro_blogging_service=Unknown;
	
	MicroBloggingServices *micro_blogging_services=MicroBloggingServicesList;
	while(micro_blogging_services->client){
		if( G_STR_N_EMPTY(micro_blogging_services->server) && (g_str_equal(service->server, micro_blogging_services->server) || g_str_equal(micro_blogging_services->server, "*")) ){
			service->micro_blogging_service=micro_blogging_services->micro_blogging_service;
			return;
		}
		
		micro_blogging_services++;
	}
	
	if(!service->micro_blogging_service)
		service->micro_blogging_service=Laconica;
}/*online_service_set_micro_blogging_service(service);*/

static const gchar *micro_blogging_service_to_string(MicroBloggingService micro_blogging_service){
	switch(micro_blogging_service){
		case Unknown: default: return _("Unkown");
		case Identica: return _("Identica");
	case Twitter: return _("Twitter");
		case Laconica: return _("Laconica");
		case Unsupported: return _("Unsupported");
	}
}/*micro_blogging_service_to_string(MicroBloggingServices micro_blogging_service)*/

gboolean online_service_uses_twitter(OnlineService *service){
	if(service->micro_blogging_service==Twitter) return TRUE;
	return FALSE;
}/*online_service_uses_twitter(service);*/

gboolean online_service_uses_laconica(OnlineService *service){
	if(service->micro_blogging_service==Identica||service->micro_blogging_service==Laconica) return TRUE;
	return FALSE;
}/*online_service_uses_laconica(service);*/

GList *online_service_users_glist_get(OnlineService *service, UsersGListGetWhich users_glist_get_which){
	if(!service) return NULL;
	switch(users_glist_get_which){
		case GetFriends: return service->friends;
		case GetFollowers: return service->followers;
		case GetBoth: default: return service->friends_and_followers;
	}
}/*online_service_users_glist_get(service, GetFriends|GetFollowers|GetBoth);*/

void online_service_users_glist_set(OnlineService *service, UsersGListGetWhich users_glist_get_which, GList *new_users){
	if(!service) return;
	GList *users=online_service_users_glist_get(service, users_glist_get_which);
	if(!users)
		users=new_users;
	else
		users=g_list_concat(users, new_users);
	users=g_list_sort(users, (GCompareFunc)usrglistscasecmp);
	switch(users_glist_get_which){
		case GetFriends:
			service->friends=users;
			break;
		case GetFollowers:
			service->followers=users;
			break;
		case GetBoth: default:
			service->friends_and_followers=users;
			break;
	}
}/*online_service_users_glist_get(service, GetFriends|GetFollowers|GetBoth, new_users);*/

static OnlineService *online_service_constructor(void){
	OnlineService *service=g_new0(OnlineService, 1);
	
	service->connected=FALSE;
	service->authenticated=TRUE;
	service->has_loaded=FALSE;
	
	service->session=NULL;
	service->status=NULL;
	service->nickname=NULL;
	service->logins=0;
	
	service->password=NULL;
	
	service->friends=service->followers=service->friends_and_followers=NULL;
	service->timer=timer_new();
	
	service->micro_blogging_service=Unknown;
	
	return service;
}/*online_service_constructor();*/

OnlineService *online_service_open(const gchar *account_key){
	debug("Loading OnlineService instance for: '%s' account.", account_key);
	
	OnlineService *service=online_service_constructor();
	
	gchar **account_data=g_strsplit(account_key, "@", 3);
	service->guid=g_strdup(account_key);
	service->uri=g_strdup(account_data[1]);
	service->username=g_strdup(account_data[0]);
	g_strfreev(account_data);
	
	gchar **parsed_uri=g_strsplit(service->uri, "/", 2);
	service->server=g_strdup(parsed_uri[0]);
	if(G_STR_EMPTY(parsed_uri[1]))
		service->path=g_strdup("");
	else
		service->path=g_strdup(parsed_uri[1]);
	g_strfreev(parsed_uri);
	
	online_service_set_micro_blogging_service(service);
	
	debug("Loading OnlineService settings for: '%s'" , service->guid);
	
	gchar *encoded_username=g_uri_escape_string(service->username, NULL, TRUE);
	gchar *encoded_uri=g_uri_escape_string(service->uri, NULL, TRUE);
	service->key=g_strdup_printf("%s@%s", encoded_username, encoded_uri );
	g_free(encoded_username);
	g_free(encoded_uri);
	
	gchar *prefs_auth_path=NULL;
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_PREFIX, service->key);
	debug("Account's gconf path:\t\t [%s]", prefs_auth_path );
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_ENABLED, service->key);
	service->enabled=gconfig_if_bool(prefs_auth_path, TRUE);
	g_free(prefs_auth_path);
	debug("%sabling [%s] OnlineService.", (service->enabled ?"En" :"Dis" ), service->guid);
	
#ifdef HAVE_GNOME_KEYRING
	if(!(keyring_get_password(service, &service->password)))
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
	
	debug("Existing OnlineService opened.\n\t\taccount '%s(=%s)'\t\t\t[%sabled]\n\t\t%sservice uri: %s over https: [%s]; username: %s; password: %s; auto_connect: [%s]", service->guid, service->key, (service->enabled?"en":"dis"), micro_blogging_service_to_string(service->micro_blogging_service), service->uri, (service->https ?_("TRUE") :_("FALSE")), service->username, service->password, (service->auto_connect?"TRUE":"FALSE"));
	
	return service;
}/*online_service_open*/

OnlineService *online_service_new(gboolean enabled, const gchar *uri, gboolean https, const gchar *username, const gchar *password, gboolean auto_connect){
	debug("Creating new OnlineService for '%s@%s'.", username, uri);
	OnlineService *service=online_service_constructor();
	
	service->enabled=enabled;
	
	service->auto_connect=auto_connect;
	
	service->guid=g_strdup_printf("%s@%s", username, uri );
	service->username=g_strdup(username);
	service->password=g_strdup(password);
	service->https=https;
	
	service->uri=g_strdup(uri);
	gchar **parsed_uri=g_strsplit(service->uri, "/", 1);
	service->server=g_strdup(parsed_uri[0]);
	if(G_STR_EMPTY(parsed_uri[1]))
		service->path=g_strdup("");
	else
		service->path=g_strdup(parsed_uri[1]);
	g_strfreev(parsed_uri);
	
	online_service_set_micro_blogging_service(service);
	
	gchar *encoded_username=g_uri_escape_string(username, NULL, TRUE);
	gchar *encoded_uri=g_uri_escape_string(uri, NULL, TRUE);
	service->key=g_strdup_printf("%s@%s", encoded_username, encoded_uri);
	g_free(encoded_username);
	g_free(encoded_uri);
	
	debug("New OnlineService created.\n\t\taccount '%s(=%s)'\t\t\t[%sabled]\n\t\t%sservice uri: %s over https: [%s]; username: %s; password: %s; auto_connect: [%s]", service->guid, service->key, (service->enabled?"en":"dis"), micro_blogging_service_to_string(service->micro_blogging_service), service->uri, (service->https ?_("TRUE") :_("FALSE")), service->username, service->password, (service->auto_connect?"TRUE":"FALSE"));
	
	return service;
}/*online_service_new*/

gboolean online_service_save(OnlineService *service, gboolean enabled, gboolean https, const gchar *password, gboolean auto_connect){
	service->enabled=enabled;
	service->https=https;
	service->password=g_strdup(password);
	service->auto_connect=auto_connect;
	
	debug("Preparing to save OnlineService for '%s'.", service->guid);
	
	gchar *prefs_auth_path=NULL;
	
	debug("Saving '%s' 'enabled' status: %s.", service->guid, (service->enabled ?"TRUE" :"FALSE" ) );
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_ENABLED, service->key);
	gconfig_set_bool(prefs_auth_path, service->enabled);
	g_free(prefs_auth_path);
	
	debug("Saving '%s' password: '%s'.", service->guid, service->password );
#ifdef HAVE_GNOME_KEYRING
	keyring_set_password(service, service->password);
#else
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_PASSWORD, service->key);
	gconfig_set_string(prefs_auth_path, service->password);
	g_free(prefs_auth_path);
#endif
	
	debug("Saving whether to use https when connecting to '%s': %s.", service->guid, (service->https ?"TRUE" :"FALSE" ) );
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_HTTPS, service->key);
	gconfig_set_bool(prefs_auth_path, service->https);
	g_free(prefs_auth_path);
	
	debug("Saving '%s' 'auto_connect' status: %s.", service->guid, (service->auto_connect ?"TRUE" :"FALSE" ) );
	prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_AUTO_CONNECT, service->key);
	gconfig_set_bool(prefs_auth_path, service->auto_connect);
	g_free(prefs_auth_path);
	
	debug("OnlineService saved.\n\t\taccount '%s(=%s)'\t\t\t[%sabled]\n\t\t%sservice uri: %s over https: [%s]; username: %s; password: %s; auto_connect: [%s]", service->guid, service->key, (service->enabled?"en":"dis"), micro_blogging_service_to_string(service->micro_blogging_service), service->uri, (service->https ?_("TRUE") :_("FALSE")), service->username, service->password, (service->auto_connect?"TRUE":"FALSE"));
	
	debug("Attempting to connect to OnlineService for: '%s'.", service->guid);
	online_service_reconnect(service);
	
	return TRUE;
}/*online_service_save*/

gboolean online_service_delete(OnlineService *service, gboolean service_cache_rm_rf){
	gchar *prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_PREFIX, service->key);
	debug("Account's gconf path:\t\t [%s]", prefs_auth_path );
	if(!(gconfig_rm_rf(prefs_auth_path))){
		debug("**ERROR:** Failed to delete '%s' gconf auth prefernces.  GConf key used: [%s].", service->guid, prefs_auth_path);
		uber_free(prefs_auth_path);
		return FALSE;
	}
	uber_free(prefs_auth_path);
	
	gchar *cache_dir=NULL;
	if(!service_cache_rm_rf){
		debug("Deleting OnlineService's user cache directory for '%s'.", service->guid);
		cache_dir=g_build_filename("services", service->uri, service->username, NULL);
	}else{
		debug("Deleting OnlineService's cache directory [%s].", service->uri);
		cache_dir=g_build_filename("services", service->uri, NULL);
	}
	
	cache_dir_clean_up(cache_dir, TRUE);
	uber_free(cache_dir);
	
	debug("Destroying OnlineService: [%s].", service->guid);
	online_service_free(service);
	return TRUE;
}/*online_service_delete*/

void online_service_update_ids_get(OnlineService *service, const gchar *timeline, gdouble *id_newest_update, gdouble *id_oldest_update){
	/* INFO:
	 * GCONF_PATH:		ONLINE_SERVICE_PREFIX: ONLINE_SERVICE_IDS_TWEETS:
	 * "(/apps/get2gnow)	(/online-services/%s)		/xml-cache%s/%s"
	 * 				service->key			/timeline.xml	(newest|oldest)
	 */
	if(G_STR_EMPTY(timeline)) return;
	gchar *prefs_path=NULL, *swap_id_str=NULL;
	gdouble swap_id;
	gboolean success;
	
	prefs_path=g_strdup_printf(ONLINE_SERVICE_IDS_TWEETS, service->key, timeline, "newest");
	success=gconfig_get_string(prefs_path, &swap_id_str);
	
	if(!(success && swap_id_str)) swap_id=0.0;
	else{
		swap_id=strtod(swap_id_str, NULL);
		uber_free(swap_id_str);
	}
	if(swap_id>0) *id_newest_update=swap_id;
	debug("Loaded <%s>'s; [%s] newest ID: %f.", service->uri, timeline, *id_newest_update);
	
	uber_free(prefs_path);
	
	prefs_path=g_strdup_printf(ONLINE_SERVICE_IDS_TWEETS, service->key, timeline, "oldest");
	success=gconfig_get_string(prefs_path, &swap_id_str);
	
	if(!(success && swap_id_str)) swap_id=0.0;
	else{
		swap_id=strtod(swap_id_str, NULL);
		uber_free(swap_id_str);
	}
	uber_free(swap_id_str);
	if(swap_id>0) *id_oldest_update=swap_id;
	
	uber_free(prefs_path);
	
	debug("Loaded <%s>'s; [%s] oldest ID: %f.", service->uri, timeline, *id_oldest_update);
}/*online_service_update_ids_get(service, "/friends.xml", id_newest_update, id_oldest_update);*/

void online_service_update_ids_set(OnlineService *service, const gchar *timeline, gdouble id_newest_update, gdouble id_oldest_update){
	/* INFO:
	 * GCONF_PATH:		ONLINE_SERVICE_PREFIX: ONLINE_SERVICE_IDS_TWEETS:
	 * "(/apps/get2gnow)	(/online-services/%s)		/xml-cache%s/%s"
	 * 				service->key			/timeline.xml (newest|oldest)
	 */
	if(G_STR_EMPTY(timeline)) return;
	gchar *prefs_path=NULL, *swap_id_str=NULL;
	gboolean success;
	
	prefs_path=g_strdup_printf(ONLINE_SERVICE_IDS_TWEETS, service->key, timeline, "newest");
	swap_id_str=g_strdup_printf("%f", id_newest_update);
	success=gconfig_set_string(prefs_path, swap_id_str);
	uber_free(prefs_path);
	debug("Saved <%s>'s; [%s] newest ID: %f (using string: %s).", service->uri, timeline, id_newest_update, swap_id_str);
	uber_free(swap_id_str);
	
	prefs_path=g_strdup_printf(ONLINE_SERVICE_IDS_TWEETS, service->key, timeline, "oldest");
	swap_id_str=g_strdup_printf("%f", id_oldest_update);
	success=gconfig_set_string(prefs_path, swap_id_str);
	uber_free(prefs_path);
	debug("Saved: <%s>'s; [%s] oldest ID: %f (using string: %s).", service->uri, timeline, id_oldest_update, swap_id_str);
	uber_free(swap_id_str);
}/*online_service_update_ids_set(service, "/direct_messages.xml", id_newest_update, id_oldest_update);*/

gboolean online_service_connect(OnlineService *service){
	debug("Loaded account: '%s'.  Validating & connecting.", service->guid);
	if(!service->enabled){
		debug("%s account not enabled.", service->guid);
		return FALSE;
	}
	
	if(G_STR_EMPTY(service->username) || G_STR_EMPTY(service->password)){
		debug("%s account is missing its username (=%s) and/or password (=%s).", service->guid, service->username, service->password);
		return FALSE;
	}
	
	if(!service->auto_connect){
		debug("%s account in not set to auto-connect.", service->guid);
		return FALSE;
	}
	
	debug("Creating '%s' connection to '%s'", service->guid, service->uri);
	if(!( (service->session=soup_session_sync_new_with_options(SOUP_SESSION_MAX_CONNS_PER_HOST, 8, SOUP_SESSION_TIMEOUT, 20, SOUP_SESSION_IDLE_TIMEOUT, 20, NULL)) )){
		debug("**ERROR:** Failed to creating a new soup session for '%s'.", service->guid);
		service->session=NULL;
		return (service->connected=service->authenticated=FALSE);
	}
	
	debug("Adding authenticating callback for: '%s'. username: %s, password: %s", service->guid, service->username, service->password);
	g_signal_connect(service->session, "authenticate", (GCallback)online_service_http_authenticate, service);
	
#ifdef GNOME_ENABLE_DEBUG
	
	SoupLogger *logger=soup_logger_new(SOUP_LOGGER_LOG_HEADERS, -1);
	soup_session_add_feature(service->session, SOUP_SESSION_FEATURE(logger));
	g_object_unref(logger);
	
#else
	
	IF_DEBUG{
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
		
	if(!( (cookie_jar_filename=cache_file_create_file_for_online_service(service, "cookies.txt", NULL)) ))
		return;

	cookie_jar=soup_cookie_jar_text_new(cookie_jar_filename, FALSE);
	
	soup_session_add_feature_by_type(service->session, soup_cookie_jar_get_type());
	soup_session_add_feature(service->session, SOUP_SESSION_FEATURE(cookie_jar));
	
	g_free(cookie_jar_filename);
	g_object_unref(cookie_jar);
}/*online_servce_open_cookie_jar*/

/* Login to service. */
gboolean online_service_login(OnlineService *service, gboolean temporary_connection){
	debug("Attempting to log in to %s...", service->guid);
	if(!(service->enabled && service->auto_connect && service->connected)) return FALSE;
	
	if(!(service->auto_connect || temporary_connection)) return FALSE;
	
	if(!SOUP_IS_SESSION(service->session)){
		debug("**ERROR**: Unable to authenticating OnlineService: %s. Invalide libsoup session.", (service->guid ?service->guid :"invalid service") );
		return FALSE;
	}
	
	if(!service->authenticated) return FALSE;
	
	main_window_statusbar_printf("Connecting to %s...", service->key);
	
	/* Verify cedentials */
	debug("Logging into: '%s'. username: %s, password: %s.", service->guid, service->username, service->password);
	
	online_service_request(service, QUEUE, API_LOGIN, NULL, online_service_login_check, API_LOGIN, NULL);
	
	debug("OnlineService: %s.  Status: authenticated: [%s].", service->guid, (service->authenticated ?"TRUE" :"FALSE" ) );
	if(!temporary_connection) online_services_increment_connected(online_services);
	
	online_service_get_profile(service);
	
	return TRUE;
}/*online_service_login*/

static void online_service_get_profile(OnlineService *service){
	if(service->has_loaded) return;
	
	gchar *user_profile_uri=g_strdup_printf(API_USER_PROFILE, service->username);
	online_service_request(service, QUEUE, user_profile_uri, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)online_service_set_profile, (OnlineServiceSoupSessionCallbackFunc)user_parse_profile, NULL, NULL);
	uber_free(user_profile_uri);
}/*online_service_get_profile(service);*/

static void online_service_set_profile(OnlineServiceWrapper *service_wrapper, User *user){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	if(service->nickname) uber_free(service->nickname);
	if(!user){
		debug("Failed to validate user profile for <%s>.  Using username instead.", service->key);
		service->nickname=g_strdup(service->username);
		service->has_loaded=FALSE;
		return;
	}
	service->has_loaded=TRUE;
	service->nickname=g_strdup(user_get_nick_name(user));
	debug("Setting nickname for: %s to %s.", service->key, service->nickname);
	user_free(user);
}/*online_service_set_profile(user);*/

static void *online_service_login_check(SoupSession *session, SoupMessage *msg, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);

	debug("Login response: %i",msg->status_code);
	
	if(!network_check_http(service, msg)) {
		debug("Logging into '%s'.", service->guid);
	}else{
		debug("Logged into '%s'.", service->guid);
	}
	return NULL;
}/*online_service_login_check*/

static void online_service_http_authenticate(SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer user_data){
	OnlineService *service=(OnlineService *)user_data;
	debug("**Authenticating:** %s.", service->key );
	if(!(service && service->key)){
		debug("**FATAL-ERROR**: Unable to authenticating OnlineService: %s. Invalid online service.", service->guid);
		return;
	}
	
	if(!service->enabled){
		debug("\t\t**WARNING:** Skipping authentication for disabled OnlineService '%s'.", service->guid);
		return;
	}
	
	SoupURI *suri=soup_message_get_uri(msg);
	gchar *uri=soup_uri_to_string(suri, FALSE);
	debug("\t\tSoupURI: Authenticating: %s", uri);
	soup_uri_free(suri);
	uber_free(uri);
	
	if(G_STR_EMPTY(service->username))
		return debug("\t\t**WARNING:** Could not authenticate: %s, unknown username.", service->guid);
	
	/* verify that the password has been set */
	if(G_STR_EMPTY(service->password))
		return debug("\t\t**WARNING:** Could not authenticate: %s, unknown password.", service->guid);
	
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
	
	online_service_get_profile(service);
}/*online_service_http_authenticate(service);*/

gboolean online_service_refresh(OnlineService *service, const gchar *uri){
	if(service->enabled && service->connected) return TRUE;
	
	if(!(service->enabled && service->auto_connect)) return FALSE;
	
	debug("Unable to load: %s.  You're not connected to %s.  Attempting to reconnect.", uri, service->key);
	if(online_service_reconnect(service) && online_service_login(service, FALSE)){
		debug("Reconnected to: %s.", service->key);
		return TRUE;
	}
	
	if(!service->connected)
		debug("Unable to reconnect to: %s%s.", service->key, service->status);
	else
		debug("Unable to log in to: %s%s.", service->key, service->status);
	
	main_window_statusbar_printf("Unable to load: %s.  You're not connected to: %s.", uri, service->key);
	
	return FALSE;
}/*online_service_refresh(service);*/

void online_service_disconnect(OnlineService *service, gboolean no_state_change){
	if(!service->connected) return;

	debug("Closing %s's connection to: %s", service->guid, service->uri);
	if(service->session){
		if(SOUP_IS_SESSION(service->session))
			soup_session_abort(service->session);
		
		uber_unref(service->session);
	}
	if(service->nickname) uber_free(service->nickname);
	service->has_loaded=service->connected=service->authenticated=FALSE;
	service->logins=0;
	debug("Disconnected from OnlineService [%s].", service->guid);
	online_services_decrement_connected(online_services, no_state_change);
}/*online_service_disconnect*/

gboolean online_service_reconnect(OnlineService *service){
	if(!(service->enabled && service->auto_connect))
		return FALSE;
	
	if(service->connected)
		online_service_disconnect(service, TRUE);
	
	return online_service_connect(service);
}/*online_service_reconnect(service);*/

gchar *online_service_request_uri_create(OnlineService *service, const gchar *uri){
	return g_strdup_printf("http%s://%s%s%s", (service->https ?"s" :"" ), service->uri, ( (service->micro_blogging_service!=Twitter) ?"/api" :"" ), (G_STR_N_EMPTY(uri) ?uri :"") );
}/*online_service_request_uri_create(service, uri);*/

SoupMessage *online_service_request(OnlineService *service, RequestMethod request, const gchar *uri, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data){
	if(!(service->enabled && service->connected)){
		if(!online_service_refresh(service, uri)){
			debug("Unable to load: %s.  You're not connected to %s.", uri, service->key);
			main_window_statusbar_printf("Unable to load: %s.  You're not connected to: %s.", uri, service->key);
			return NULL;
		}
	}
	
	gchar *new_uri=online_service_request_uri_create(service, uri);
	debug("Creating new service request for: '%s', requesting: %s.", service->guid, new_uri);
	SoupMessage *msg=online_service_request_uri(service, request, (const gchar *)new_uri, online_service_soup_session_callback_return_processor_func, callback, user_data, form_data);
	g_free(new_uri);
	return msg;
}/*online_service_request*/

SoupMessage *online_service_request_uri(OnlineService *service, RequestMethod request, const gchar *uri, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data){
	if(!(service->enabled && service->connected)){
		if(!online_service_refresh(service, uri)){
			debug("Unable to load: %s.  You're not connected to %s.", uri, service->key);
			main_window_statusbar_printf("Unable to load: %s.  You're not connected to: %s.", uri, service->key);
			return NULL;
		}
	}
	
	if(!(service && service->session && SOUP_IS_SESSION(service->session) )){
		online_service_reconnect(service);
		if(!SOUP_IS_SESSION(service->session))
			return NULL;
	}
	
	gchar *request_uri=NULL;
	if(!( g_str_has_prefix(uri, "https") || g_str_has_prefix(uri, "http") || g_str_has_prefix(uri, "ftp") ))
		request_uri=g_strdup_printf("http://%s", uri);
	else
		request_uri=g_strdup(uri);
	
	SoupMessage *msg=NULL;
	gchar *request_string=NULL;
	switch(request){
		case GET:
			request_string=_("GET");
			break;
		case QUEUE:
			request_string=_("QUEUE");
			break;
		case POST:
			request_string=_("POST");
			break;
	}
	
	debug("Creating libsoup request for service: '%s'.", service->guid);
	switch(request){
		case GET:
		case QUEUE:
			online_service_request_validate_uri(service, &request_uri, online_service_soup_session_callback_return_processor_func, callback, &user_data, &form_data);
			debug("%s: %s", request_string, request_uri);
			msg=soup_message_new("GET", request_uri);
			break;
		
		case POST:
			online_service_request_validate_form_data(service, &request_uri, online_service_soup_session_callback_return_processor_func, callback, &user_data, &form_data);
			debug("form_data: [%s]", (gchar *)form_data);
			msg=soup_message_new(request_string, request_uri);
			
			soup_message_headers_append(msg->request_headers, "X-Twitter-Client", PACKAGE_NAME);
			soup_message_headers_append(msg->request_headers, "X-Twitter-Client-Version", PACKAGE_VERSION);
			
			if(form_data)
				soup_message_set_request(
					msg,
					"application/x-www-form-urlencoded",
					SOUP_MEMORY_COPY,
					(gchar *)form_data,
					(strlen( (gchar *)form_data ) )
				);
			break;
	}
	
	if(!( SOUP_IS_SESSION(service->session) && SOUP_IS_MESSAGE(msg) )){
		debug("Unable to process libsoup request for service: '%s'.\n\t\tAttempting to %s: %s", service->guid, request_string, request_uri);
		uber_free(request_uri);
		return NULL;
	}
	
	g_signal_connect(msg, "restarted", G_CALLBACK(online_service_message_restarted), (gpointer)service);
	
	debug("Processing libsoup request for service: '%s'.", service->guid);
	OnlineServiceWrapper *online_service_wrapper=NULL;
	switch(request){
		case QUEUE:
		case POST:
			online_service_wrapper=online_service_wrapper_new(service, request_uri, online_service_soup_session_callback_return_processor_func, callback, user_data, form_data);
			debug("Adding libsoup request to service: '%s' libsoup's message queue.", service->guid);
			soup_session_queue_message(service->session, msg, (SoupSessionCallback)online_service_callback, online_service_wrapper);
			break;
			
		case GET:
			debug("Sending libsoup request to service: '%s' & returning libsoup's message.", service->guid);
			soup_session_send_message(service->session, msg);
			break;
	}
	
	uber_free(request_uri);
	return msg;
}/*online_service_request_uri*/

static void online_service_request_validate_uri(OnlineService *service, gchar **request_uri, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer *user_data, gpointer *form_data){
	if(!(
		callback!=NULL
		&&
		callback==network_display_timeline
	))
		return;
	
	if(g_strrstr(*request_uri, "?since_id=")) return;
	
	TweetLists monitoring=(TweetLists)*form_data;
	TweetList *tweet_list=(TweetList *)*user_data;
	gint8 has_loaded=tweet_list_has_loaded(tweet_list);
	if(!( (*form_data) && (*user_data) )) return;
	
	gdouble id_newest_update=0, id_oldest_update=0;
	const gchar *timeline=g_strrstr(*request_uri, "/");
	online_service_update_ids_get(service, timeline, &id_newest_update, &id_oldest_update);
	
	gchar *request_uri_swap=NULL;
	debug("Updating request uri for <%s> to new updates posted to %s which has loaded %i.", service->key, *request_uri, has_loaded);
	if(has_loaded){
		if(!id_newest_update) return;
		if(!tweet_list_get_total(tweet_list)) return;
		else{
			gchar *id_newest_update_str=double_no_zeros(id_newest_update);
			request_uri_swap=g_strdup_printf("%s?since_id=%s", *request_uri, id_newest_update_str);
			debug("Requesting <%s>'s timeline: %s; new updates since: %f (using string: %s).", service->key, timeline, id_newest_update, id_newest_update_str);
			uber_free(id_newest_update_str);
		}
	}else if(monitoring==DMs || monitoring==Replies){
		if(!id_oldest_update) return;
		else{
			gchar *id_oldest_update_str=double_no_zeros(id_oldest_update);
			request_uri_swap=g_strdup_printf("%s?since_id=%s", *request_uri, id_oldest_update_str);
			debug("Requesting <%s>'s timeline: %s; total updates since: %f (using string: %s).", service->key, timeline, id_oldest_update, id_oldest_update_str);
			uber_free(id_oldest_update_str);
		}
	}else return;
	
	g_free(*request_uri);
	*request_uri=request_uri_swap;
	request_uri_swap=NULL;
}/*online_service_request_validate_uri(service, &request_uri, callback, &user_data, &form_data);*/

static void online_service_request_validate_form_data(OnlineService *service, gchar **request_uri, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer *user_data, gpointer *form_data){
	if(!( (*form_data) &&  (*user_data) )) return;
	
	if(!g_str_equal( ((gchar *)(*user_data)), "Tweet") ) return;
	
	gchar *reply_form_data=NULL;
	debug("Posting update to: [%s].", service->key);
	if(!(in_reply_to_status_id && in_reply_to_service==service)){
		debug("Posting update to: <%s>", service->key);
		reply_form_data=g_strdup_printf( "source=%s&status=%s", online_service_get_micro_blogging_client(service), (gchar *)(*form_data));
	}else{
		gchar *in_reply_to_status_id_str=double_no_zeros(in_reply_to_status_id);
		debug("Replying to Update: #%f (using string: %s).", in_reply_to_status_id, in_reply_to_status_id_str);
		reply_form_data=g_strdup_printf( "source=%s&in_reply_to_status_id=%s&status=%s", online_service_get_micro_blogging_client(service), in_reply_to_status_id_str, (gchar *)(*form_data));
		uber_free(in_reply_to_status_id_str);
	}
	
	(*form_data)=(gpointer)reply_form_data;
	
	debug("Update: [%s].", (gchar *)(*form_data));
}/*online_service_request_validate_form_data(service, &request_uri, callback, &user_data, &form_data);*/

static void online_service_message_restarted(SoupMessage *msg, gpointer user_data){
	if(msg->status_code!=401) return;
	OnlineService *service=(OnlineService *)user_data;
	if(!service->authenticated) return;
	
	service->logins++;
	
	if(service->logins < MAX_LOGINS ) return;
	
	debug("**ERROR:** Cancelling restarted message, authentication failed.");
	soup_session_cancel_message(service->session, msg, 401);
	online_service_disconnect(service, FALSE);
}/*onlin_service_message_restarted*/

void online_service_soup_session_callback_return_processor_func_default(OnlineServiceWrapper *service_wrapper, gpointer soup_session_callback_return_gpointer){
	debug("\n\t\t\t\t\t\t|-----------------------------------------------------------------------------|\n"
		"\t\t\t\t\t\t|     online_service_soup_session_callback_return_processor_func_default      |\n"
		"\t\t\t\t\t\t|_____________________________________________________________________________|"
	);
}/*online_service_soup_session_callback_return_processor_func_default(soup_session_callback_return_gpointer);*/

void *online_service_callback(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	
	gchar *status=NULL;
	gboolean run_timer=TRUE;
	if(!SOUP_STATUS_IS_SUCCESSFUL(xml->status_code)){
		run_timer=FALSE;
		status=_("[Failed]");
	}else
		status=_("[Success]");
	
	const gchar *requested_uri=online_service_wrapper_get_requested_uri(service_wrapper);
	if(service->status) uber_free(service->status);
	service->status=g_strdup_printf("OnlineService: %s request: %s.  <%s> responsed %s(%d).", service->key, status, requested_uri, xml->reason_phrase, xml->status_code);
	debug("%s", service->status);
	statusbar_printf("<%s> loading %s: %s.", service->key, g_strrstr(requested_uri, "/"), status);
	
	online_service_wrapper_run(service_wrapper, session, xml);
	
	
	if(run_timer) timer_main(service->timer, xml);
	
	online_service_wrapper_free(service_wrapper);
	return NULL;
}/*online_service_callback(session, msg, online_service_wrapper);*/

gchar *online_service_get_uri_content_type(OnlineService *service, const gchar *uri, SoupMessage **msg){
	*msg=online_service_request_uri(service, GET, uri, NULL, NULL, NULL, NULL);
	if(!network_check_http(service, *msg))
		return NULL;

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
	
	debug("Unloading instance of: %s service", service->guid);
	online_service_disconnect(service, TRUE);
	
	debug("Shutting down network rate-limit timer for OnlineService\t[%s].", service->guid );
	timer_free(service->timer);
	service->timer=NULL;
	
	debug("Closing %s account", service->guid );
	/*
	 * service->status maybe NULL.
	 * so it needs to be last to make sure every thing else is released.
	 * see: program.c::program_uber_free();
	 */
	users_glists_free_lists(service);
	
	debug("Destroying OnlineService <%s> object.", service->guid );
	uber_object_free(&service->key, &service->uri, &service->username, &service->nickname, &service->password, &service->status, &service->guid, &service, NULL);
}/*online_service_free*/

/********************************************************
 *                       eof                            *
 ********************************************************/

