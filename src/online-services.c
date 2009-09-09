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

#include "online-services-typedefs.h"
#include "online-service-request.h"
#include "online-services.h"
#include "online-service.h"

#include "main-window.h"
#include "gconfig.h"
#include "online-services-dialog.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
struct _OnlineServices{
	guint		total;
	guint		connected;
	GSList		*keys;
	GList		*accounts;
	GList		*best_friends;
};

#define	ONLINE_SERVICES_ACCOUNTS	GCONF_PATH "/online-services/accounts"
#define	MY_BEST_FRIENDS			GCONF_PATH "/online-services/best_friends"


/********************************************************************************
 *                    prototypes for private method & function                  *
 ********************************************************************************/
static void online_services_combo_box_add_new(GtkComboBox *combo_box, GtkListStore *list_store);
static gint online_services_cmp_count(guint compare, guint count);


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
static gint longest_replacement_length=0;
static OnlineServices *services=NULL;
OnlineServices *online_services=NULL;

#define	DEBUG_DOMAINS	"OnlineServices:UI:Network:Tweets:Requests:Users:Authentication:online-services.c"
#include "debug.h"

#define MAX_LOGINS 5


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
OnlineServices *online_services_init(void){
	GSList		*k=NULL;
	OnlineService	*service=NULL;
	gchar		*account_key=NULL;
	
	services=g_new0(OnlineServices, 1);
	services->total=services->connected=0;
	
	gconfig_get_list_string(ONLINE_SERVICES_ACCOUNTS, &services->keys);
	
	for( k=services->keys; k; k=k->next ){
		account_key=(gchar *)k->data;
		if(!g_strrstr(account_key, "@")){
			debug("**ERROR:** Invalid OnlineService: <%s> - skipping.", account_key);
			continue;
		}
		debug("Loading '%s' account.", account_key);
		if(!(service=online_service_open( (const gchar *)account_key ))) continue;
		
		services->accounts=g_list_append(services->accounts, service);
		services->accounts=g_list_last(services->accounts);
		service=(OnlineService *)services->accounts->data;
		
		if(online_service_connect(service)) services->total++;
	}
	
	services->accounts=g_list_first(services->accounts);
	
	online_services=services;
	
	if(!services->total)
		debug("**ERROR:** No accounts are enabled or setup.  New accounts will be need to be setup.");
	else
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
		if(online_service_login(service, FALSE)){
			if(!login_okay) login_okay=TRUE;
		}
	}
	if(!services->connected)
		debug("**ERROR:** Zero accounts are connected.  Network requests cannot be processed.");
	else
		debug("Connected to %d OnlineServices.", services->connected);
	
	main_window_state_on_connection(login_okay);
	return login_okay;
}/*online_services_login*/

/* Login to services. */
gboolean online_services_relogin(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	gboolean	relogin_okay=FALSE;
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		online_service_reconnect(service);
		
		if(online_service_login(service, FALSE))
			if(!relogin_okay) relogin_okay=TRUE;
	}
	main_window_state_on_connection(relogin_okay);
	return relogin_okay;
}/*online_services_relogin*/

void online_services_disconnect(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	selected_service=NULL;
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		online_service_disconnect(service, TRUE);
	}
	main_window_state_on_connection(FALSE);
}/*online_services_disconnect*/

gint online_services_has_total(OnlineServices *services, guint count){
	return online_services_cmp_count(services->total, count);
}/*online_services_has_total(services, >1);*/

gint online_services_has_connected(OnlineServices *services, guint count){
	return online_services_cmp_count(services->connected, count);
}/*online_services_has_connected(services, >1);*/

static gint online_services_cmp_count(guint compare, guint count){
	if(!compare) return -2;
	if(!count) return compare;
	if(compare > count ) return -1;
	if(compare == count ) return 0;
	return 1;
}/*online_services_has_connected(services, >1);*/

OnlineService *online_services_save_service(OnlineServices *services, OnlineService *service, const gchar *uri, const gchar *user_name, const gchar *password, gboolean enabled, gboolean https, gboolean auto_connect){
	if(!( G_STR_N_EMPTY(uri) && G_STR_N_EMPTY(user_name) )){
		return FALSE;
	}
	
	gchar *decoded_key=g_strdup_printf("%s@%s", user_name, uri);
	if(service){
		if(!(online_service_validate_key(service, decoded_key)))
			online_services_delete_service(services, service);
		else{
			debug("Saving existing service: '%s'.", decoded_key);
			if( (online_service_save(service, password, enabled, https, auto_connect)) )
				if(online_service_reconnect(service))
					return service;
			debug("Unable to save existing OnlineService for: [%s].", decoded_key);
			uber_free(decoded_key);
			return NULL;
		}
	}
	
	longest_replacement_length=0;
	
	debug("Creating & saving new service: '%s'.", decoded_key);
	service=online_service_new(uri, user_name, password, enabled, https, auto_connect);
	debug("New OnlineService '%s' created.  Saving OnlineServices", decoded_key);
	
	services->total++;
	debug("New service: '%s' created.  OnlineServices total: %d.", decoded_key, services->total);
	
	debug("Adding '%s' to OnlineServices' keys.", decoded_key);
	if(!( (services->keys=g_slist_append(services->keys, decoded_key)) )){
		debug("**ERROR**: Failed to append new service's key: '%s', to OnlineServices' keys.", decoded_key);
		uber_free(decoded_key);
		return NULL;
	}
	
	debug("Saving accounts & services list: '%s'.", ONLINE_SERVICES_ACCOUNTS);
	if(!( (gconfig_set_list_string(ONLINE_SERVICES_ACCOUNTS, services->keys)) )){
		debug("**ERROR**: Failed to save new service: '%s', couldn't save gconf's services list.", decoded_key);
		uber_free(decoded_key);
		return NULL;
	}
	
	debug("Adding new service: '%s' to OnlineServices.", decoded_key);
	if(!( (services->accounts=g_list_append(services->accounts, service)) )){
		debug("**ERROR**: Failed to add: '%s', to OnlineServices' accounts.", decoded_key);
		uber_free(decoded_key);
		return NULL;
	}
	
	debug("Retrieving new service: '%s' from OnlineServices accounts.", decoded_key);
	services->accounts=g_list_last(services->accounts);
	service=(OnlineService *)services->accounts->data;
	services->accounts=g_list_first(services->accounts);
	
	debug("Saving OnlineService: '%s' reloaded from OnlineServices accounts.", decoded_key);
	if(!( online_service_save(service, password, enabled, https, auto_connect) )){
		debug("**ERROR**: Failed saving new service: '%s'.", decoded_key);
		uber_free(decoded_key);
		return NULL;
	}

	debug("Saving accounts & services successful.");
	if(online_service_connect(service)){
		debug("\t\tConnecting to: '%s'\t[succeeded]", decoded_key);

		online_service_login(service, FALSE);
		if( services->total!=1 )
			tweet_lists_refresh();
		else{
			tweet_lists_init();
			main_window_state_on_connection(TRUE);
		}
	}else
		debug("\t\tConnecting to: '%s'\t[failed]", decoded_key);
	
	debug("Saving '%s' service complete.  Total services: %d; Total connected: %d", decoded_key, services->total, services->connected);
	
	uber_free(decoded_key);
	return service;
}/*online_services_save*/

void online_services_delete_service(OnlineServices *services, OnlineService *service){
	const gchar *service_key=online_service_get_key(service);
	debug("Removing old OnlineService: '%s'.", service_key);
	online_service_disconnect(service, TRUE);
	if(!services->connected)
		main_window_state_on_connection(FALSE);
	
	debug("Rebuilding OnlineServices' key while removing old key: %s.", service_key);
	g_slist_free(services->keys);
	services->keys=g_slist_alloc();
	GList *accounts=NULL;
	GSList *keys=NULL;
	debug("Rebuilding OnlineServices' keys, removing: '%s'.", service_key);
	OnlineService *key=NULL;
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		key=(OnlineService *)accounts->data;
		const gchar *account_key=online_service_get_key(key);
		if(!g_str_equal(account_key, service_key))
			keys=g_slist_append(keys, (gchar *)account_key);
	}
	services->keys=keys;
	
	debug("Saving re-built OnlineServices' keys.");
	if(!( (gconfig_set_list_string(ONLINE_SERVICES_ACCOUNTS, services->keys)) )){
		debug("**ERROR**: Failed to delete service: '%s', couldn't save gconf's services list.", service_key);
		return;
	}
	
	debug("Updating OnlineServices' accounts.  Removing OnlineService: [%s].", service_key);
	services->accounts=g_list_remove(services->accounts, service);
	
	debug("Determining what level of the OnlineService's cache directory to delete.");
	gboolean service_cache_rm_rf=TRUE;
	const gchar *service_uri=online_service_get_uri(service);
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		key=(OnlineService *)accounts->data;
		if(g_str_equal(online_service_get_uri(key), service_uri)){
			debug("OnlineService: [%s] share's avatars cache, only cookies will be deleted.", service_key);
			service_cache_rm_rf=FALSE;
		}
	}
	
	online_service_delete(service, service_cache_rm_rf);
	
	if(services->total) services->total--;
	debug("OnlineService deleted.  OnlineServices now has %d accounts.", services->total);
	if(!services->total){
		main_window_state_on_connection(FALSE);
		online_services_dialog_show(main_window_get_window());
	}
	longest_replacement_length=0;
}/*online_services_delete(services, service);*/

static void online_services_combo_box_add_new(GtkComboBox *combo_box, GtkListStore *list_store){
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	gtk_list_store_append(list_store, iter);
	gtk_list_store_set(
			list_store, iter,
				UrlString, "[new account]",
				OnlineServicePointer, NULL,
			-1
	);
	uber_free(iter);
	gtk_combo_box_set_active(combo_box, 0);
}/*online_services_combo_box_add_new(combo_box, list_store);*/

gboolean online_services_combo_box_fill(OnlineServices *services, GtkComboBox *combo_box, GtkListStore *list_store, gboolean connected_only){
	if(!list_store) return FALSE;
	gtk_list_store_clear(list_store);
	if(!online_services->total){
		debug("No services found to load, new accounts need to be setup.");
		if(!connected_only) online_services_combo_box_add_new(combo_box, list_store);
		return FALSE;
	}
	
	GtkTreeIter		*iter=NULL;
	GList			*a=NULL;
	guint			services_loaded=0;
	debug("Loading services into tree view. total services: '%d'.", online_services->total);
	for(a=services->accounts; a; a=a->next){
		OnlineService *service=(OnlineService *)a->data;
		if( connected_only && !online_service_is_connected(service) ) continue;
		
		const gchar *service_uri=online_service_get_uri(service);
		debug("Appending account: '%s'; server: %s.", online_service_get_key(service), service_uri);
		iter=g_new0(GtkTreeIter, 1);
		gtk_list_store_append(list_store, iter);
		gtk_list_store_set(
					list_store, iter,
						UrlString, service_uri,
						OnlineServicePointer, service,
					-1
		);
		services_loaded++;
		uber_free(iter);
	}
	if(!connected_only)
		online_services_combo_box_add_new(combo_box, list_store);
	else if(services_loaded){
		debug("Accounts & services loaded.  Selecting combo_box's default account.");
		gtk_combo_box_set_active(combo_box, 0);
	}
	
	return (services_loaded ?TRUE :FALSE );
}/*online_services_combo_box_fill*/

OnlineService *online_services_connected_get_first(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	for(a=services->accounts; a; a=a->next)
		if(online_service_is_connected( (service=(OnlineService *)a->data) ))
			return service;
	
	return NULL;
}/*online_services_connected_get_first(online_services);*/

OnlineService *online_services_connected_get_last(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	for(a=services->accounts; a; a=a->next)
		if(online_service_is_connected( (OnlineService *)a->data ))
			service=(OnlineService *)a->data;
	
	return service;
}/*online_services_connected_get_last(online_services);*/

void online_services_reset_length_of_longest_replacement(OnlineServices *services){
	if(longest_replacement_length) longest_replacement_length=0;
}/*online_services_reset_length_of_longest_replacement(online_services);*/

gssize online_services_get_length_of_longest_replacement(OnlineServices *services){
	GList		*a=NULL;
	OnlineService	*service=NULL;
	gssize		replacement_length=0;
	
	if(!longest_replacement_length){
		gint		replace_with=0;
		gconfig_get_int_or_default(PREFS_TWEET_REPLACE_ME_W_NICK, &replace_with, 2);
		
		for(a=services->accounts; a; a=a->next){
			service=(OnlineService *)a->data;
			if(online_service_is_connected(service))
				if( (replacement_length=strlen( (replace_with==1?online_service_get_user_nick(service):online_service_get_user_name(service)) )) > longest_replacement_length)
					longest_replacement_length=replacement_length;
		}
	}
	
	return longest_replacement_length;
}/*online_services_get_length_of_longest_replacement(online_services);*/


void online_services_request(OnlineServices *services, RequestMethod request, const gchar *uri, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data){
	/*if(!services->connected){
		debug("**ERROR:** Zero accounts are connected.  Network requests cannot be processed.");
		return;
	}*/
	GList		*a=NULL;
	OnlineService	*service=NULL;
	
	for(a=services->accounts; a; a=a->next){
		service=(OnlineService *)a->data;
		const gchar *service_key=online_service_get_key(service);
		if(!online_service_refresh(service, uri)){
			debug("**ERROR:** Unable do load: %s refreshing: <%s> failed.", uri, service_key);
			continue;
		}
		
		debug("Requesting: %s from <%s>.", uri, service_key);
		online_service_request(service, request, uri, online_service_soup_session_callback_return_processor_func, callback, user_data, form_data);
	}
}/*online_services_request*/

void online_services_increment_total(OnlineServices *services, const gchar *service_guid){
	services->total++;
	debug("OnlineServices has enabled the OnlineService: <%s>.  Total services: #%d.", service_guid, services->total);
}/*online_services_increment_total(online_services);*/

void online_services_decrement_total(OnlineServices *services, const gchar *service_guid){
	if(services->total) services->total--;
	
	debug("OnlineServices has had OnlineService: <%s> removed.  Remaining services: #%d.", service_guid, services->total);
	if(services->total) return;
	
	main_window_state_on_connection(FALSE);
	online_services_dialog_show(main_window_get_window());
}/*online_services_decrement_total(online_services);*/

void online_services_increment_connected(OnlineServices *services, const gchar *service_guid){
	services->connected++;
	debug("OnlineServices has connected to OnlineService: <%s>.  Total connected: #%d.", service_guid, services->connected);
}/*online_services_increment_connected(online_services);*/

void online_services_decrement_connected(OnlineServices *services, const gchar *service_guid, gboolean no_state_change){
	if(services->connected) services->connected--;
	
	debug("OnlineServices has disconnected from OnlineService: <%s>.  Remaining connections: #%d.", service_guid, services->connected);
	if(services->connected) return;
	
	main_window_state_on_connection(FALSE);
	
	if(no_state_change) return;
	online_services_dialog_show(main_window_get_window());
}/*online_services_decrement_connected(online_services);*/

void online_services_deinit(OnlineServices *services){
	debug("**SHUTDOWN:** Closing & releasing %d accounts.", services->total);
	g_list_foreach(services->accounts, (GFunc)online_service_free, NULL);

	debug("**SHUTDOWN:** freeing OnlineServices keys.");
	g_slist_free(services->keys);
	services->keys=NULL;
	
	g_free(services);
	services=NULL;
	
	selected_service=NULL;
	
	services=NULL;
	online_services=NULL;
}/*online_services_deinit*/

/********************************************************
 *                       eof                            *
 ********************************************************/

