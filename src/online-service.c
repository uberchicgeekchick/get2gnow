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
#include <libxml/parser.h>

#include "config.h"
#include "program.h"

#include "parser.h"

#include "online-services.defines.h"
#include "online-services-typedefs.h"
#include "online-services.h"
#include "online-service-wrapper.h"
#include "online-service.types.h"
#include "online-service.h"

#include "online-services-dialog.h"
#include "online-service-request.h"

#include "tweet-lists.h"

#include "users.types.h"
#include "users-glists.h"
#include "users.h"
#include "network.h"

#include "preferences.h"
#include "proxy.h"

#ifdef HAVE_GNOME_KEYRING
#include "keyring.h"
#endif

#include "main-window.h"
#include "gconfig.h"
#include "cache.h"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static OnlineService *online_service_constructor(const gchar *uri, const gchar *user_name);

static const gchar *micro_blogging_service_to_string(MicroBloggingService micro_blogging_service);
static void online_service_set_micro_blogging_service(OnlineService **service);

static void online_service_http_authenticate(SoupSession *session, SoupMessage *xml, SoupAuth *auth, gboolean retrying, gpointer user_data);
/*static void *online_service_login_check( SoupMessage *xml, OnlineService *service);*/
static void *online_service_login_check(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper);

static void online_service_set_profile(OnlineServiceWrapper *service_wrapper, SoupMessage *xml, User *user);

static gboolean online_service_best_friends_load( OnlineService *service );
static gboolean online_service_best_friends_save( OnlineService *service );
static void online_service_best_friends_list_store_append( OnlineService *service, const gchar *user_name );
static gboolean online_service_best_friends_confirm_clean_up( OnlineService *service, const gchar *user_name );
static gboolean online_service_best_friends_list_store_remove( OnlineService *service, const gchar *user_name );

static void online_service_message_restarted(SoupMessage *xml, gpointer user_data);

static void online_service_cookie_jar_open(OnlineService *service);

static void online_service_request_validate_uri(OnlineService *service, gchar **request_uri, guint attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer *user_data, gpointer *form_data);
static void online_service_request_validate_form_data(OnlineService *service, gchar **request_uri, guint attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer *user_data, gpointer *form_data);

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
	{Identica,	"identi.ca",		"get2gnow"},
	{Twitter,	"twitter.com",		"greettweetknow"},
	{StatusNet,	"*",			"get2gnow"},
	{Unsupported,	NULL,			NULL},
};

#define ONLINE_SERVICE_PREFIX				GCONF_PATH "/online-services/%s"
#define ONLINE_SERVICE_IDS_TWEETS			GCONF_PATH "/online-services/xml-cache/archive/since-ids/%s%s/%s"

#define	ONLINE_SERVICE_PASSWORD				ONLINE_SERVICE_PREFIX "/password"
#define	ONLINE_SERVICE_AUTO_CONNECT			ONLINE_SERVICE_PREFIX "/auto_connect"
#define	ONLINE_SERVICE_HTTPS				ONLINE_SERVICE_PREFIX "/https"
#define	ONLINE_SERVICE_ENABLED				ONLINE_SERVICE_PREFIX "/enabled"
#define	ONLINE_SERVICE_LAST_REQUEST			ONLINE_SERVICE_PREFIX "/timestamps/last_request"

#define	ONLINE_SERVICE_BEST_FRIENDS			ONLINE_SERVICE_PREFIX "/best_friends"
#define	ONLINE_SERVICE_CONFIRM_BEST_FRIENDS_CLEAN_UP	GCONF_PATH "/popup_confirmation_dialog/disabled_when/cleaning_up/best_friends"
#define	ONLINE_SERVICE_CONFIRM_BEST_FRIENDS_DELETION	GCONF_PATH "/popup_confirmation_dialog/disabled_when/deleting/best_friends"


#ifdef GNOME_ENABLE_DEBUG
#define	DEBUG_DISPLAY_PASSWORDS	TRUE
#else
#define	DEBUG_DISPLAY_PASSWORDS NULL
#endif


#define	DEBUG_DOMAINS	"OnlineServices:Network:Tweets:Requests:Users:Settings:Authentication:Settings:Setup:Start-Up:online-service.c"
#include "debug.h"


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
gboolean online_service_validate_guid(OnlineService *service, const gchar *user_name, const gchar *uri){
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
		if( G_STR_N_EMPTY(micro_blogging_services->server) && (g_str_has_prefix((*service)->server, micro_blogging_services->server) || g_str_equal((*service)->server, micro_blogging_services->server) || g_str_equal(micro_blogging_services->server, "*")) ){
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
		case Identica: return _("Identica");
		case Twitter: return _("Twitter");
		case StatusNet: return _("StatusNet");
		case Unsupported: return _("Unsupported");
	}
}/*micro_blogging_service_to_string(MicroBloggingServices micro_blogging_service)*/

GList *online_service_users_glist_get(OnlineService *service, UsersGListGetWhich users_glist_get_which){
	if(!service) return NULL;
	GList *users=NULL;
	switch(users_glist_get_which){
		case GetFriends:
			if(service->friends)
				users=service->friends;
			break;
		case GetFollowers:
			if(service->followers)
				users=service->followers;
			break;
		case GetBoth: default:
			if(!(service->friends && service->followers))
				return NULL;
			if(!service->friends_and_followers)
				 service->friends_and_followers=g_list_concat(service->friends, service->followers);
			users=service->friends_and_followers;
			break;
	}
	return g_list_first(users);
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
}/*online_service_users_glist_set(service, GetFriends|GetFollowers|GetBoth, new_users);*/

static OnlineService *online_service_constructor(const gchar *uri, const gchar *user_name){
	OnlineService *service=g_new0(OnlineService, 1);
	
	service->uri=g_strdup(uri);
	
	service->user_name=g_strdup(user_name);
	service->nick_name=g_strdup(service->nick_name);
	
	service->connected=service->has_loaded=FALSE;
	service->authenticated=TRUE;
	
	service->session=NULL;
	service->status=NULL;
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
	
	if(!g_strrstr(account_key, "@")){
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
	if(!(keyring_get_password(&service))) service->password=NULL;
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
	
	online_service_display_debug_details(service, FALSE, "opened");
	
	online_service_best_friends_load( service );
	
	return service;
}/*online_service_open*/

OnlineService *online_service_new(const gchar *uri, const gchar *user_name, const gchar *password, gboolean enabled, gboolean https, gboolean auto_connect){
	debug("Creating new OnlineService for '%s@%s'.", user_name, uri);
	OnlineService *service=online_service_constructor(uri, user_name);
	
	service->enabled=enabled;
	service->auto_connect=auto_connect;
	service->password=g_strdup(password);
	service->https=https;
	
	online_service_display_debug_details(service, TRUE, "created");
	
	return service;
}/*online_service_new*/

gboolean online_service_save(OnlineService *service, const gchar *password, gboolean enabled, gboolean https, gboolean auto_connect){
	service->enabled=enabled;
	service->https=https;
	service->password=g_strdup(password);
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
	
	online_service_display_debug_details(service, FALSE, "saved");
	
	debug("Attempting to connect to OnlineService for: '%s'.", service->guid);
	
	return TRUE;
}/*online_service_save*/

gboolean online_service_delete(OnlineService *service, gboolean service_cache_rm_rf){
	debug("Preparing to delete OnlineService.  GUID: <%s>; key: (%s).", service->guid, service->key);
	
	gchar *prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_PREFIX, service->key);
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
	
	tweet_lists_remove_service(service);
	
	online_service_display_debug_details(service, FALSE, "deleted");
	online_service_free(service);
	return TRUE;
}/*online_service_delete*/

void online_service_display_debug_details(OnlineService *service, gboolean new_service, const char *action){
	gchar *prefs_auth_path=g_strdup_printf(ONLINE_SERVICE_PREFIX, service->key);
	debug("OnlineService: %s %s service.  GCONF path:\t [%s]. OnlineService details: account guid:%s(key==%s)'\t\t\t[%sabled] %sservice uri: %s over https: [%s]; user_name: %s; password: %s; auto_connect: [%s]", action, (new_service ?"created" :"existing"), prefs_auth_path, service->guid, service->key, (service->enabled?"en":"dis"), micro_blogging_service_to_string(service->micro_blogging_service), service->uri, (service->https ?_("TRUE") :_("FALSE")), service->user_name, (DEBUG_DISPLAY_PASSWORDS ?service->password :"[*passwords are hidden out side of debug mode*]"), (service->auto_connect ?_("TRUE") :_("FALSE") ) );
	g_free(prefs_auth_path);
}/*online_service_display_debug_details(service, "action");*/

static gboolean online_service_best_friends_load( OnlineService *service ){
	gchar *gconf_prefs_path=g_strdup_printf( ONLINE_SERVICE_BEST_FRIENDS, service->key );
	gboolean loaded=gconfig_get_list_string( gconf_prefs_path, &service->best_friends );
	service->best_friends=g_slist_sort( service->best_friends, (GCompareFunc)strcasecmp );
	uber_free( gconf_prefs_path );
	return loaded;
}/*online_service_best_friends_load( service );*/

static gboolean online_service_best_friends_save( OnlineService *service ){
	gchar *gconf_prefs_path=g_strdup_printf( ONLINE_SERVICE_BEST_FRIENDS, service->key );
	service->best_friends=g_slist_sort( service->best_friends, (GCompareFunc)strcasecmp );
	gboolean saved=gconfig_set_list_string( gconf_prefs_path, service->best_friends );
	uber_free( gconf_prefs_path );
	return saved;
}/*online_service_best_friends_save( service );*/

gint online_service_best_friends_list_store_fill( OnlineService *service, GtkListStore *list_store ){
	GSList *best_friends=NULL;
	debug("Loading <%s>'s best_friends.", service->key );
	service->best_friends_total=0;
	for( best_friends=g_slist_nth( service->best_friends, 0 ); best_friends; best_friends=best_friends->next, service->best_friends_total++ )
		online_service_best_friends_list_store_append( service, (const gchar *)best_friends->data );
	
	return service->best_friends_total;
}/*online_service_best_friends_list_store_fill( service );*/

gint online_service_best_friends_list_store_validate( OnlineService *service, GtkListStore *list_store ){
	GSList *best_friends=NULL;
	service->best_friends_total=0;
	for( best_friends=g_slist_nth( service->best_friends, 0 ); best_friends; best_friends=best_friends->next)
		online_service_fetch_profile( service, (const gchar *)best_friends->data, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)online_service_best_friends_list_store_update_check );
	
	return service->best_friends_total;
}/*online_service_best_friends_list_store_validate( service, list_store );*/

gboolean online_service_is_user_best_friend( OnlineService *service, const gchar *user_name ){
	if(!(service->best_friends && G_STR_N_EMPTY(user_name) )) return FALSE;
	GSList *best_friends=NULL;
	for( best_friends=g_slist_nth(service->best_friends, 0); best_friends; best_friends=best_friends->next )
		if(!strcasecmp( user_name, (gchar *)best_friends->data ))
			return TRUE;
	
	return FALSE;
}/*online_service_is_user_best_friend( service, user_name );*/

gboolean online_service_best_friends_add( OnlineService *service, const gchar *user_name ){
	if(!(service->best_friends && G_STR_N_EMPTY(user_name) )) return FALSE;
	gboolean found=online_service_is_user_best_friend( service, user_name );
	
	if(found){
		debug( "Cannot add: %s to <%s>'s best_friends.  %s is already listed in <%s>'s best friends list.", user_name, service->guid, user_name, service->guid );
		statusbar_printf( "%s is already one of your, <%s>'s, best friends.", user_name, service->guid );
	}else{
		debug( "Attempting to load: %s's profile to add them to <%s>'s best_friends.", user_name, service->guid );
		statusbar_printf( "Adding %s's to your, <%s>, best friends.", user_name, service->guid );
		online_service_fetch_profile( service, user_name, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)online_service_best_friends_list_store_update_check );
	}
	
	return !found;
}/*online_service_best_friends_add( OnlineService *service, const gchar *user_name );*/

gboolean online_service_best_friends_drop( OnlineService *service, GtkWindow *parent, const gchar *user_name ){
	if(!(service->best_friends && G_STR_N_EMPTY(user_name) )) return FALSE;
	gboolean found=online_service_is_user_best_friend( service, user_name );
	
	if(!found){
		debug( "Cannot remove: %s from <%s>'s best_friends.  %s was not found in <%s>'s best friends list.", user_name, service->guid, user_name, service->guid );
		statusbar_printf( "%s is not one of your, <%s>'s, best friends.", user_name, service->guid );
		online_service_request_best_friend_drop(service, parent, user_name);
	}else{
		debug( "Attempting to load: %s's profile to remove them from <%s>'s best_friends.", user_name, service->guid );
		statusbar_printf( "Removing %s's from your, <%s>, best friends.", user_name, service->guid );
		online_service_fetch_profile( service, user_name, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)online_service_best_friends_list_store_update_check );
	}
	
	return found;
}/*online_service_best_friends_drop( service, user_name );*/

void online_service_best_friends_list_store_update_check(OnlineServiceWrapper *online_service_wrapper, SoupMessage *xml, User *user){
	OnlineService *service=online_service_wrapper_get_online_service(online_service_wrapper);
	const gchar *user_name=user->user_name;
	if(!user){
		debug( "User %s's profile could not be found, on %s.  Their user name has most likely changed.  Though unlikely its possible the netwok connection may have been lost.  Unlikely because prior 'status' checks would have kept this method from being called.", user_name, service->guid );
		if( online_service_best_friends_confirm_clean_up( service, user_name ) )
			service->best_friends=g_slist_remove(service->best_friends, user_name);
	}else if(! online_service_is_user_best_friend(service, user_name ) ){
		debug( "Adding best friend %s, on %s to the best friends list_store & GSList.", user_name, service->guid );
		service->best_friends=g_slist_append( service->best_friends, g_strdup(user_name) );
		gchar *user_timeline=g_strdup_printf(API_TIMELINE_USER, user->user_name );
		online_service_update_ids_set( service, user_timeline, user->status->id, user->status->id );
		uber_free( user_timeline );
		online_service_best_friends_list_store_append( service, user_name );
	}else{
		debug( "Removing best friend %s, on %s from the best friends list_store & GSList.", user_name, service->guid );
		online_service_best_friends_list_store_remove( service, user_name );
	}
	
	service->best_friends=g_slist_sort( service->best_friends, (GCompareFunc)strcasecmp );
	online_service_best_friends_save( service );
	service->best_friends=g_slist_nth( service->best_friends, 0 );
}/*online_service_best_friends_list_store_update_check( online_service_wrapper, xml, user_name );*/

static void online_service_best_friends_list_store_append( OnlineService *service, const gchar *user_name ){
	static GtkListStore *list_store=NULL;
	if(!list_store) list_store=main_window_get_best_friends_list_store();
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	gdouble id_best_friend_newest_update=0.0, id_best_friend_oldest_update=0.0;
	gchar *user_timeline=g_strdup_printf(API_TIMELINE_USER, user_name );
	online_service_update_ids_get( service, user_timeline, &id_best_friend_newest_update, &id_best_friend_oldest_update );
	uber_free( user_timeline );
	gtk_list_store_append( list_store, iter );
	gtk_list_store_set(
				list_store, iter,
					ONLINE_SERVICE_BEST_FRIEND_ONLINE_SERVICE, service,
					STRING_BEST_FRIEND_USER, user_name,
					STRING_BEST_FRIEND_ONlINE_SERVICE_GUID, service->guid,
					STRING_BEST_FRIEND_USER_NAME, user_name,
					GUINT_BEST_FRIENDS_UNREAD_UPDATES, 0,
					GDOUBLE_BEST_FRIENDS_NEWEST_UPDATE_ID, id_best_friend_newest_update,
				-1
	);
	uber_free( iter );
	online_services_best_friends_total_update(1);
}/*online_service_best_friends_list_store_append( service, user_name );*/

static gboolean online_service_best_friends_confirm_clean_up( OnlineService *service, const gchar *user_name ){
	gchar *message=NULL;
	debug( "Failed to fetch <%s>'s best_friend: %s.", service->guid, user_name );
	statusbar_printf( "<%s> loading best friend from: %s [failed]", service->guid, user_name );
	if(online_service_request_popup_confirmation_dialog(
			ONLINE_SERVICE_CONFIRM_BEST_FRIENDS_CLEAN_UP,
			_("Unable to find a best friend:"),
			(message=g_strdup_printf( "%s was unable to load one of your best friends.\n%s could not be found on <%s>.\nThis usually means you're not connected to <%s>\nOr your best friend may have changed their user name.\n\nWould you like to remove %s from <%s>'s best friends?", _(GETTEXT_PACKAGE), user_name, service->guid, service->guid, user_name, service->guid )),
			NULL, NULL
	)){
		uber_free(message);
		return online_service_best_friends_list_store_remove( service, user_name );
	}
	
	uber_free(message);
	return FALSE;
}/*online_service_best_friends_confirm_clean_up( service, user_name );*/

static gboolean online_service_best_friends_list_store_remove( OnlineService *service, const gchar *user_name ){
	static GtkListStore *list_store=NULL;
	if(!list_store) list_store=main_window_get_best_friends_list_store();
	
	OnlineService *service_at_index=NULL;
	gchar *user_name_at_index=NULL;
	gint best_friends_total=online_services_best_friends_total_update(0 );
	for(gint i=0; i<=best_friends_total; i++){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter((GtkTreeModel *)list_store, iter, path))){
			debug("Removing iter at index: %d failed.  Unable to retrieve iter from path.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gtk_tree_model_get(
				(GtkTreeModel *)list_store, iter,
					ONLINE_SERVICE_BEST_FRIEND_ONLINE_SERVICE, &service_at_index,
					STRING_BEST_FRIEND_USER, &user_name_at_index,
				-1
		);
		
		if(service==service_at_index && strcasecmp(user_name, user_name_at_index)){
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		debug("Removing best friend: %s from iter at index: %d", user_name_at_index, i);
		online_services_best_friends_total_update(-1);
		service->best_friends_total--;
		gtk_list_store_remove(list_store, iter);
		
		gtk_tree_path_free(path);
		uber_free(iter);
		return TRUE;
	}
	debug("Could not remove %s, on %s.  The user could not be found.", user_name, service->guid);
	return FALSE;
}/*online_service_best_friends_list_store_remove( service, user_name );*/

void online_service_best_friends_list_store_free( OnlineService *service, GtkListStore *list_store ){
	gchar *user_at_index=NULL;
	gchar *user_name_at_index=NULL;
	gint best_friends_total=online_services_best_friends_total_update(0 );
	for(gint i=best_friends_total-1; i>=0; i--){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter( (GtkTreeModel *)list_store, iter, path))){
			debug("Removing iter at index: %d failed.  Unable to retrieve iter from path.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gtk_tree_model_get(
				(GtkTreeModel *)list_store, iter,
					STRING_BEST_FRIEND_USER, &user_at_index,
					STRING_BEST_FRIEND_USER_NAME, &user_name_at_index,
				-1
		);
		
		debug("Removing best friend: %s from iter at index: %d", user_name_at_index, i);
		gtk_list_store_remove(list_store, iter);
		uber_free(user_name_at_index);
		
		gtk_tree_path_free(path);
		uber_free(iter);
	}
	g_slist_foreach(service->best_friends, (GFunc)g_free, NULL);
}/*online_service_best_friends_list_store_free( service, list_store );*/

void online_service_fetch_profile( OnlineService *service, const gchar *user_name, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_user_parser_func ){
	gchar *user_profile_uri=g_strdup_printf(API_USER_PROFILE, user_name);
	online_service_request( service, QUEUE, user_profile_uri, online_service_user_parser_func, (OnlineServiceSoupSessionCallbackFunc)user_parse_profile, NULL, NULL );
	uber_free(user_profile_uri);
}/*online_service_best_friend_fetch_profile( OnlineService *service, const gchar *user_name );*/

void online_service_update_ids_get(OnlineService *service, const gchar *timeline, gdouble *id_newest_update, gdouble *id_oldest_update){
	online_service_update_id_get( service, timeline, "newest", id_newest_update );
	online_service_update_id_get( service, timeline, "oldest", id_oldest_update );
}/*online_service_update_ids_get(service, "/friends.xml", id_newest_update, id_oldest_update);*/

void online_service_update_id_get( OnlineService *service, const gchar *timeline, const gchar *key, gdouble *update_id ){
	/* INFO:
	 * GCONF_PATH:		ONLINE_SERVICE_PREFIX: ONLINE_SERVICE_IDS_TWEETS:
	 * "(/apps/get2gnow)	(/online-services/%s)		/xml-cache%s/%s"
	 * 				service->key			/timeline.xml	(newest|oldest)
	 */
	if(G_STR_EMPTY(timeline)) return;
	gchar *prefs_path=NULL, *swap_id_str=NULL;
	gdouble swap_id;
	gboolean success;
	
	prefs_path=g_strdup_printf(ONLINE_SERVICE_IDS_TWEETS, service->key, timeline, key);
	success=gconfig_get_string(prefs_path, &swap_id_str);
	uber_free(prefs_path);
	
	if(!(success && swap_id_str)) swap_id=0.0;
	else{
		swap_id=strtod(swap_id_str, NULL);
		uber_free(swap_id_str);
	}
	if(swap_id>0) *update_id=swap_id;
	debug("Loaded <%s>'s; [%s] %s ID: %f.", service->uri, timeline, key, *update_id);
}/*online_service_update_id_get( service, "/friends.xml", "newest", &newest_update_id );*/

void online_service_update_ids_set( OnlineService *service, const gchar *timeline, gdouble id_newest_update, gdouble id_oldest_update ){
	online_service_update_id_set( service, timeline, "newest", id_newest_update );
	online_service_update_id_set( service, timeline, "oldest", id_oldest_update );
}/*online_service_update_ids_set(service, "/friends.xml", id_newest_update, id_oldest_update);*/

void online_service_update_id_set( OnlineService *service, const gchar *timeline, const gchar *key, gdouble update_id ){
	/* INFO:
	 * GCONF_PATH:		ONLINE_SERVICE_PREFIX: ONLINE_SERVICE_IDS_TWEETS:
	 * "(/apps/get2gnow)	(/online-services/%s)		/xml-cache%s/%s"
	 * 				service->key			/timeline.xml (newest|oldest)
	 */
	if(G_STR_EMPTY(timeline)) return;
	gchar *prefs_path=NULL, *swap_id_str=NULL;
	gboolean success;
	
	prefs_path=g_strdup_printf(ONLINE_SERVICE_IDS_TWEETS, service->key, timeline, key);
	swap_id_str=gdouble_to_str(update_id);
	success=gconfig_set_string(prefs_path, swap_id_str);
	uber_free(prefs_path);
	debug("Saved <%s>'s; [%s] %s ID: %f (using string: %s).", service->uri, timeline, key, update_id, swap_id_str);
	uber_free(swap_id_str);
}/*online_service_id_set( service, "/friends.xml", "newest", &newest_update_id );*/

gboolean online_service_connect(OnlineService *service){
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
		return (service->connected=(service->authenticated=FALSE));
	}
	
	debug("Adding authenticating callback for: '%s'. user_name: %s, password: %s", service->guid, service->user_name, service->password);
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
	
	return (service->connected=(service->authenticated=TRUE));
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
	
	uber_free(cookie_jar_filename);
	uber_object_unref(cookie_jar);
}/*online_servce_open_cookie_jar*/

/* Login to service. */
gboolean online_service_login(OnlineService *service, gboolean temporary_connection){
	debug("Attempting to log in to %s...", service->guid);
	if(!(service->enabled && service->auto_connect && service->connected)) return FALSE;
	
	if(!SOUP_IS_SESSION(service->session)){
		debug("**ERROR**: Unable to authenticate OnlineService: <%s>. Invalid libsoup session.", (service->guid ?service->guid :"invalid service") );
		return FALSE;
	}
	
	if(!service->authenticated )	return FALSE;
	if(!online_service_refresh(service)) return FALSE;
	
	main_window_statusbar_printf("Connecting to %s...", service->key);
	
	/* Verify cedentials */
	debug("Logging into: '%s'. user_name: %s, password: %s.", service->guid, service->user_name, service->password);
	
	online_service_request(service, QUEUE, API_LOGIN, NULL, online_service_login_check, API_LOGIN, NULL);
	
	if(!temporary_connection) online_services_increment_connected(service->guid);
	
	return TRUE;
}/*online_service_login(service, TRUE|FALSE);*/

/*static void *online_service_login_check(SoupMessage *xml, OnlineService *service){*/
static void *online_service_login_check(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);

	debug("OnlineService: <%s>'s http status: %d.  Status: authenticated: [%s].", service->guid, xml->status_code, (service->authenticated ?"TRUE" :"FALSE" ) );

	gchar *error_message=NULL;
	if(!parser_xml_error_check(service, xml, &error_message)){
		debug("Logging on to <%s> failed.  Please check your user name and/or password. %s said: %s(#%d).", service->guid, service->uri, xml->reason_phrase, xml->status_code );
		statusbar_printf("Logging on to <%s> failed.  Please check your user name and/or password. %s said: %s(#%d).", service->guid, service->uri, xml->reason_phrase, xml->status_code );
		service->authenticated=FALSE;
		//service->enabled=FALSE;
	}else{
		debug("Logged on to <%s> succesfully.", service->guid);
		statusbar_printf("Logged on to <%s> succesfully.", service->guid);
		service->authenticated=TRUE;
		service->enabled=TRUE;
		online_service_fetch_profile( service, service->user_name, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)online_service_set_profile );
	}
	
	uber_free(error_message);
	
	return NULL;
}/*online_service_login_check*/

static void online_service_set_profile(OnlineServiceWrapper *service_wrapper, SoupMessage *xml, User *user){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	if(service->nick_name) uber_free(service->nick_name);
	if(!user){
		debug("Failed to validate user profile for <%s>.  Please check your user name and/or password.  %s said: %s(#%d).", service->guid, service->uri, xml->reason_phrase, xml->status_code );
		service->nick_name=g_strdup(service->user_name);
		service->has_loaded=FALSE;
		service->authenticated=FALSE;
		service->enabled=FALSE;
		return;
	}
	service->connected=service->authenticated=TRUE;
	service->has_loaded=TRUE;
	service->nick_name=g_strdup(user->nick_name);
	debug("Setting nick_name for: %s to %s.", service->key, service->nick_name);
	user_free(user);
}/*online_service_set_profile(user);*/

static void online_service_http_authenticate(SoupSession *session, SoupMessage *xml, SoupAuth *auth, gboolean retrying, gpointer user_data){
	OnlineService *service=(OnlineService *)user_data;
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
		if( (++service->logins) > ONLINE_SERVICE_MAX_REQUESTS){
			debug("**ERROR**: Authentication attempts %d exceeded maximum attempts: %d.", service->logins, ONLINE_SERVICE_MAX_REQUESTS);
			service->authenticated=FALSE;
			return;
		}
	}
	
	debug("Authenticating OnlineService: [%s] Attempt #%d of %d maximum allowed attempts. Username: [%s]; Password: [%s]; Server: [%s].", service->key, service->logins, ONLINE_SERVICE_MAX_REQUESTS, service->user_name, service->password, service->uri);
	soup_auth_update(auth, xml, "WWW-Authenticate");
	soup_auth_authenticate(auth, service->user_name, service->password);
	service->authenticated=TRUE;
}/*online_service_http_authenticate(service);*/

gboolean online_service_refresh(OnlineService *service){
	if(service->enabled && service->connected) return TRUE;
	
	if(!(service->enabled && service->auto_connect)) return FALSE;
	
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
	main_window_statusbar_printf("Unable to load one of <%s>'s REST resources.  You're not connected to: %s.", service->uri, service->guid);
	
	return FALSE;
}/*online_service_refresh(service);*/

void online_service_disconnect(OnlineService *service, gboolean no_state_change){
	if(!(service->session && service->authenticated)) return;

	debug("Closing %s's connection to: %s", service->guid, service->uri);
	if(service->session){
		if(SOUP_IS_SESSION(service->session))
			soup_session_abort(service->session);
		
		uber_object_unref(service->session);
	}
	if(service->nick_name) uber_free(service->nick_name);
	service->has_loaded=service->connected=service->authenticated=FALSE;
	service->logins=0;
	debug("Disconnected from OnlineService [%s].", service->guid);
	online_services_decrement_connected(service->guid, no_state_change);
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
	if(!( G_STR_N_EMPTY(uri) && service->enabled )) return NULL;
	
	gchar *new_uri=online_service_request_uri_create(service, uri);
	debug("Creating new service request for: '%s', requesting: %s.", service->guid, new_uri);
	SoupMessage *xml=online_service_request_uri(service, request, (const gchar *)new_uri, 0, online_service_soup_session_callback_return_processor_func, callback, user_data, form_data);
	g_free(new_uri);
	return xml;
}/*online_service_request*/

SoupMessage *online_service_request_uri(OnlineService *service, RequestMethod request, const gchar *uri, guint attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data){
	if(!( G_STR_N_EMPTY(uri) && service->enabled )) return NULL;
	
	if(!(service->connected && service->authenticated)){
		if(!online_service_refresh(service)){
			debug("Unable to load: %s.  You're not connected to %s.", uri, service->key);
			main_window_statusbar_printf("Unable to load: %s.  You're not connected to: %s.", uri, service->key);
			return NULL;
		}
	}
	
	time_t current_time=time(NULL);
	gchar *datetime=ctime(&current_time);
	gchar *prefs_path=g_strdup_printf(ONLINE_SERVICE_LAST_REQUEST, service->key);
	gconfig_set_string(prefs_path, datetime);
	uber_free(prefs_path);

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
	
	SoupMessage *xml=NULL;
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
	OnlineServiceWrapper *online_service_wrapper=NULL;
	switch(request){
		case QUEUE:
			online_service_request_validate_uri(service, &request_uri, attempt, online_service_soup_session_callback_return_processor_func, callback, &user_data, &form_data);
		case POST:
			online_service_wrapper=online_service_wrapper_new(service, request, request_uri, attempt, online_service_soup_session_callback_return_processor_func, callback, user_data, form_data);
			if(request!=POST) break;
			online_service_request_validate_form_data(service, &request_uri, attempt, online_service_soup_session_callback_return_processor_func, callback, &user_data, &form_data);
		case GET: default: break;
	}
	
	switch(request){
		case QUEUE:
		case GET:
			debug("%s: %s", request_string, request_uri);
			xml=soup_message_new("GET", request_uri);
			break;
		
		case POST:
			debug("form_data: [%s]", (gchar *)form_data);
			xml=soup_message_new(request_string, request_uri);
			
			soup_message_headers_append(xml->request_headers, "X-Twitter-Client", PACKAGE_NAME);
			soup_message_headers_append(xml->request_headers, "X-Twitter-Client-Version", PACKAGE_VERSION);
			
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
		debug("Unable to process libsoup request for service: '%s'. Attempting to %s: %s", service->guid, request_string, request_uri);
		uber_free(request_uri);
		return NULL;
	}
	
	/*g_signal_connect(xml, "restarted", G_CALLBACK(online_service_message_restarted), (gpointer)service);*/
	
	debug("Processing libsoup request for service: '%s'.", service->guid);
	switch(request){
		case QUEUE:
		case POST:
			debug("Adding libsoup request to service: '%s' libsoup's message queue.", service->guid);
			soup_session_queue_message(service->session, xml, (SoupSessionCallback)online_service_callback, online_service_wrapper);
			break;
			
		case GET:
			debug("Sending libsoup request to service: '%s' & returning libsoup's message.", service->guid);
			soup_session_send_message(service->session, xml);
			break;
	}
	
	uber_free(request_uri);
	return xml;
	g_signal_connect(xml, "restarted", G_CALLBACK(online_service_message_restarted), (gpointer)service);
}/*online_service_request_uri*/

static void online_service_request_validate_uri(OnlineService *service, gchar **request_uri, guint attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer *user_data, gpointer *form_data){
	if(!(
		!attempt
		&&
		callback!=NULL
		&&
		callback==network_display_timeline
	))
		return;
	
	if(g_strrstr(*request_uri, "?since_id=")) return;
	
	UpdateMonitor monitoring=(UpdateMonitor)*form_data;
	TweetList *tweet_list=(TweetList *)*user_data;
	gint8 has_loaded=tweet_list_has_loaded(tweet_list);
	if(!( (*form_data) && (*user_data) )) return;
	
	gdouble id_newest_update=0, id_oldest_update=0;
	const gchar *timeline=g_strrstr(*request_uri, "/");
	online_service_update_ids_get(service, timeline, &id_newest_update, &id_oldest_update);
	
	const gchar *requesting;
	gdouble since_id=0;
	debug("Updating request uri for <%s> to new updates posted to %s which has loaded %i.", service->key, *request_uri, has_loaded);
	if( has_loaded && tweet_list_get_total(tweet_list) ){
		requesting=_("new");
		since_id=id_newest_update;
	}else if(monitoring==DMs || monitoring==Replies){
		requesting=_("total");
		since_id=id_oldest_update;
	}else if(monitoring==BestFriends){
		requesting=_("best friend's newest");
		since_id=id_newest_update;
	}else return;
	
	if(!since_id) return;
	
	gchar *update_str=gdouble_to_str(since_id);
	gchar *request_uri_swap=g_strdup_printf("%s?since_id=%s", *request_uri, update_str);
	g_free(*request_uri);
	*request_uri=request_uri_swap;
	
	debug("Requesting <%s>'s timeline: %s; %s updates since: %f (using string: %s).", service->key, timeline, requesting, since_id, update_str);
	uber_free(update_str);
	request_uri_swap=NULL;
}/*online_service_request_validate_uri(service, &request_uri, attempt, callback, &user_data, &form_data);*/

static void online_service_request_validate_form_data(OnlineService *service, gchar **request_uri, guint attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer *user_data, gpointer *form_data){
	if(!(
		!attempt
		&&
		callback!=NULL
		&&
		callback==network_tweet_cb
	))
		return;
	
	if(!( (*form_data) && (*user_data) )) return;
	
	gchar *reply_form_data=NULL;
	if(service->user_name && g_strrstr( (gchar *)(*form_data), "/me" ) ){
		gint replace=0;
		gconfig_get_int_or_default(PREFS_TWEET_REPLACE_ME_W_NICK, &replace, 2);
		if(replace!=-1){
			debug("Auto-replacement trigger match '/me' to be replaced with user nick.");
			debug("/me auto-replacement triggered.  Replacing '/me' with: '%s'", service->user_name);
			gchar **reply_form_data_parts=g_strsplit( (gchar *)(*form_data), "/me", -1);
			gchar *replace_with=g_strdup_printf("*%s", ( replace==1 ? service->nick_name : service->user_name ) );
			reply_form_data=g_strdup("");
			gchar *reply_form_data_swap=NULL;
			for(gint i=0; reply_form_data_parts[i]; i++){
				reply_form_data_swap=g_strdup_printf("%s%s%s", reply_form_data, reply_form_data_parts[i], (reply_form_data_parts[i+1]?replace_with:""));
				g_free(reply_form_data);
				reply_form_data=reply_form_data_swap;
				reply_form_data_swap=NULL;
			}
			g_strfreev(reply_form_data_parts);
			(*form_data)=(gpointer)reply_form_data;
			reply_form_data=NULL;
			g_free( replace_with );
		}
	}
	
	(*form_data)=(gpointer)g_uri_escape_string((*form_data), NULL, TRUE);
	
	debug("Posting update to: [%s].", service->key);
	if(!g_str_equal((gchar *)(*user_data), "post->update")){
		debug("Sending direct message to: <%s@%s> from: <%s>", (gchar *)(*user_data), service->uri, service->key);
		reply_form_data=g_strdup_printf("source=%s&user=%s&text=%s", service->micro_blogging_client, (gchar *)(*user_data), (gchar *)(*form_data));
	}else if(!(in_reply_to_status_id && in_reply_to_service==service)){
		debug("Posting update to: <%s>", service->key);
		reply_form_data=g_strdup_printf("source=%s&status=%s", service->micro_blogging_client, (gchar *)(*form_data));
	}else{
		gchar *in_reply_to_status_id_str=gdouble_to_str(in_reply_to_status_id);
		debug("Replying to Update: #%f (using string: %s).", in_reply_to_status_id, in_reply_to_status_id_str);
		reply_form_data=g_strdup_printf("source=%s&in_reply_to_status_id=%s&status=%s", service->micro_blogging_client, in_reply_to_status_id_str, (gchar *)(*form_data));
		uber_free(in_reply_to_status_id_str);
	}
	
	g_free(*form_data);
	(*form_data)=(gpointer)reply_form_data;
	
	debug("Update: [%s].", (gchar *)(*form_data));
}/*online_service_request_validate_form_data(service, &request_uri, attempt, callback, &user_data, &form_data);*/


static void online_service_message_restarted(SoupMessage *xml, gpointer user_data){
	if(!( (SOUP_IS_MESSAGE(xml)) && xml->status_code!=401 )) return;
	OnlineService *service=(OnlineService *)user_data;
	if(!(service->authenticated && service->has_loaded)) return;
	
	service->logins++;
	
	if(service->logins < ONLINE_SERVICE_MAX_REQUESTS) return;
	
	debug("**ERROR:** Cancelling restarted message, authentication failed.");
	soup_session_cancel_message(service->session, xml, 401);
	online_service_disconnect(service, FALSE);
}/*onlin_service_message_restarted*/


void online_service_soup_session_callback_return_processor_func_default(OnlineServiceWrapper *service_wrapper, SoupMessage *xml, gpointer soup_session_callback_return_gpointer){
	debug(" \t\t\t|-----------------------------------------------------------------------------|\n"
		"\t\t\t\t\t|     online_service_soup_session_callback_return_processor_func_default      |\n"
		"\t\t\t\t\t|_____________________________________________________________________________|"
	);
}/*online_service_soup_session_callback_return_processor_func_default(soup_session_callback_return_gpointer);*/

void *online_service_callback(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	const gchar *requested_uri=online_service_wrapper_get_requested_uri(service_wrapper);
	if(!(requested_uri && G_STR_N_EMPTY(requested_uri) )) requested_uri=_("Unknown URI");
	if(!(service && service->key && G_STR_N_EMPTY(service->key) )) return NULL;
	
	if(service->status) uber_free(service->status);
	const gchar *status=NULL;
	gchar *error_message=NULL;
	if(!parser_xml_error_check(service, xml, &error_message))
		status=_("[Failed]");
	else
		status=_("[Success]");
	
	service->status=g_strdup_printf("OnlineService: <%s> requested: %s.  URI: %s returned: %s(%d).", service->key, status, requested_uri, xml->reason_phrase, xml->status_code);
	debug("%s", service->status);
	statusbar_printf("<%s> loading %s: %s.", service->key, g_strrstr(requested_uri, "/"), status);
	
	online_service_wrapper_run(service_wrapper, session, xml);
	
	timer_main(service->timer, xml);
	
	online_service_wrapper_free(service_wrapper);
	
	uber_free(error_message);
	
	return NULL;
}/*online_service_callback(session, xml, online_service_wrapper);*/

gchar *online_service_get_uri_content_type(OnlineService *service, const gchar *uri, SoupMessage **xml){
	debug("Downloading headers for: %s to determine its content-type.", uri);
	*xml=online_service_request_uri(service, GET, uri, 0, NULL, NULL, NULL, NULL);
	gchar *error_message=NULL;
	if(!parser_xml_error_check(service, *xml, &error_message)){
		uber_free(error_message);
		return NULL;
	}
	
	uber_free(error_message);
	
	debug("Getting content-type from uri: '%s'.", uri);
	gchar **content_v=NULL;
	debug("Parsing xml document's content-type & DOM information from: %s", uri);
	gchar *content_info=NULL;
	if(!(content_info=g_strdup((gchar *)soup_message_headers_get_one((*xml)->response_headers, "Content-Type")))){
		debug("**ERROR**: Failed to determine content-type for:  %s.", uri);
		return NULL;
	}
	
	debug("Parsing content info: [%s] from: %s", content_info, uri);
	content_v=g_strsplit(content_info, "; ", -1);
	g_free(content_info);
	gchar *content_type=NULL;
	if(!( ((content_v[0])) && (content_type=g_strdup(content_v[0])) )){
		debug("**ERROR**: Failed to determine content-type for:  %s.", uri);
		g_strfreev(content_v);
		return NULL;
	}
	debug("Parsed Content-Type: [%s] for: %s", content_type, uri);
	
	g_strfreev(content_v);
	
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
	uber_object_free(&service->key, &service->uri, &service->user_name, &service->nick_name, &service->password, &service->status, &service->guid, &service->micro_blogging_client, &service, NULL);
}/*online_service_free*/

/********************************************************
 *                       eof                            *
 ********************************************************/

