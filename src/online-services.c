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

#include "online-services.defines.h"
#include "online-services-typedefs.h"
#include "online-service-request.h"
#include "online-services.h"
#include "online-service.types.h"
#include "online-service.h"

#include "main-window.h"
#include "update-viewer.h"
#include "uberchick-label.h"
#include "preferences.h"
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
	gint		best_friends_total;
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
	services->best_friends_total=0;
	
	gconfig_get_list_string(ONLINE_SERVICES_ACCOUNTS, &services->keys);
	
	for(k=services->keys; k; k=k->next){
		account_key=(gchar *)k->data;
		if(!g_strrstr(account_key, "@")){
			debug("**ERROR:** Invalid OnlineService: <%s> - skipping.", account_key);
			continue;
		}
		debug("Loading '%s' account.", account_key);
		if(!(service=online_service_open( (const gchar *)account_key))) continue;
		
		services->accounts=g_list_append(services->accounts, service);
		services->accounts=g_list_last(services->accounts);
		service=(OnlineService *)services->accounts->data;
		
		if(online_service_connect(service)) services->total++;
	}
	
	services->accounts=g_list_first(services->accounts);
	
	if(!services->total)
		debug("**ERROR:** No accounts are enabled or setup.  New accounts will be need to be setup.");
	else
		debug("%d services found and loaded.", services->total);
	
	return services;
}/*online_services_init*/

/* Login to services. */
gboolean online_services_login(void){
	GList		*accounts=NULL;
	OnlineService	*service=NULL;
	gboolean	login_okay=FALSE;
	
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		service=(OnlineService *)accounts->data;
		if(online_service_login(service, FALSE)){
			if(!login_okay) login_okay=TRUE;
		}
	}
	if(!services->connected)
		debug("**ERROR:** Zero accounts are connected.  Network requests cannot be processed.");
	else
		debug("Connected to %d OnlineServices.", services->connected);
	
	main_window_state_on_connection(login_okay);
	g_list_free(accounts);
	return login_okay;
}/*online_services_login*/

/* Login to services. */
gboolean online_services_reconnect(void){
	GList		*accounts=NULL;
	OnlineService	*service=NULL;
	
	gboolean	relogin_okay=FALSE;
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		service=(OnlineService *)accounts->data;
		online_service_reconnect(service);
		
		if(online_service_login(service, FALSE))
			if(!relogin_okay) relogin_okay=TRUE;
	}
	main_window_state_on_connection(relogin_okay);
	g_list_free(accounts);
	return relogin_okay;
}/*online_services_reconnect*/

gboolean online_services_refresh(void){
	GList		*accounts=NULL;
	OnlineService	*service=NULL;
	
	gboolean	refresh_okay=FALSE;
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		service=(OnlineService *)accounts->data;
		if(online_service_refresh(service))
			if(!refresh_okay) refresh_okay=TRUE;
	}
	g_list_free(accounts);
	return refresh_okay;
}/*online_services_refresh();*/

void online_services_disconnect(void){
	GList		*accounts=NULL;
	OnlineService	*service=NULL;
	
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		service=(OnlineService *)accounts->data;
		if(selected_service==service)
			selected_service=NULL;
		online_service_disconnect(service, TRUE);
	}
	main_window_state_on_connection(FALSE);
	g_list_free(accounts);
}/*online_services_disconnect*/


void online_services_uri_clicked(GtkWidget *widget, const gchar *uri){
	GList		*accounts=NULL;
	OnlineService	*service=NULL;
	
	gboolean service_uri_handled=FALSE;
	UberChickLabel *uberchick_label;
	if(!( IS_UBERCHICK_LABEL( (uberchick_label=UBERCHICK_LABEL(widget)) ) ))
		uberchick_label=NULL;
	
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		service=(OnlineService *)accounts->data;
		if( !g_strrstr(uri, service->uri) ) continue;

		if( g_strrstr(uri, "search") ){
			gchar *search_phrase=g_strrstr(uri, "?q=%");
			if(!(search_phrase[4] && search_phrase[4] =='2'))
				if(G_STR_N_EMPTY( (search_phrase=g_strrstr(uri, "=")+sizeof("=")) ))
					return main_window_sexy_search_entry_set(search_phrase, TRUE);
				else
					return;
			
			static char tag;
			if(!search_phrase[7])
				if(G_STR_N_EMPTY( (search_phrase=g_strrstr(uri, "=")+sizeof("=")) ))
					return main_window_sexy_search_entry_set(search_phrase, TRUE);
				else
					return;
			if( search_phrase[5] && search_phrase[5] =='1') tag='!';
			else if( search_phrase[5] && search_phrase[5] =='3') tag='#';
			search_phrase=g_strdup_printf("%c%s", tag, &search_phrase[7]);
			main_window_sexy_search_entry_set(search_phrase, TRUE);
			uber_free(search_phrase);
			return;
		}
		
		service_uri_handled=TRUE;
		gchar *services_resource=g_strrstr( (g_strrstr(uri, service->uri)), "/");
		if(!uberchick_label)
			debug("OnlineServices: Inserting: <%s@%s> in to current update.", &services_resource[1], service->uri );
		else{
			update_viewer_set_in_reply_to_data(uberchick_label_get_service(uberchick_label), uberchick_label_get_update_id(uberchick_label), FALSE);
			debug("OnlineServices via UberChick_Label: Inserting: <%s@%s> in to current update.", &services_resource[1], service->uri );
		}
		gchar *user_profile_link=NULL;
		if( online_services_has_connected(1) > 0 && !gconfig_if_bool(PREFS_UPDATES_NO_PROFILE_LINK, TRUE) )
			user_profile_link=g_strdup_printf(" ( http://%s%s )", service->uri, services_resource );
		gchar *users_at=g_strdup_printf("@%s%s ", &services_resource[1], (user_profile_link ?user_profile_link :""));
		update_viewer_sexy_insert_string(users_at);
		uber_free(users_at);
		if(user_profile_link) uber_free(user_profile_link);
		break;
	}
	g_list_free(accounts);
	if(!service_uri_handled)
		if(g_app_info_launch_default_for_uri(uri, NULL, NULL))
			debug("**NOTICE:** Opening URI: <%s>.", uri );
		else
			debug("**ERROR:** Can't handle URI: <%s>.", uri );
}/*online_services_url_activated_cb(widget, const gchar *uri);*/


gint online_services_has_total(guint count){
	return online_services_cmp_count(services->total, count);
}/*online_services_has_total(>1);*/

gint online_services_has_connected(guint count){
	return online_services_cmp_count(services->connected, count);
}/*online_services_has_connected(1);*/

static gint online_services_cmp_count(guint compare, guint count){
	if(!compare) return -2;
	if(!count) return compare;
	if(compare == count) return 0;
	if(compare > count) return 1;
	return -1;
}/*online_services_cmp_count(service->logins, 1);*/

OnlineService *online_services_save_service(OnlineService *service, const gchar *uri, const gchar *user_name, const gchar *password, gboolean enabled, gboolean https, gboolean auto_connect){
	if(!( G_STR_N_EMPTY(uri) && G_STR_N_EMPTY(user_name))){
		return NULL;
	}
	
	gboolean recreating=FALSE;
	gchar *decoded_key=g_strdup_printf("%s@%s", user_name, uri);
	if(service){
		if(!(online_service_validate_guid(service, user_name, uri))){
			online_services_delete_service(service);
			recreating=TRUE;
		}else{
			debug("Saving existing service: <%s>.", decoded_key);
			online_service_disconnect(service, FALSE);
			if((online_service_save(service, password, enabled, https, auto_connect)))
				if(online_service_reconnect(service))
					return service;
			debug("Unable to save existing OnlineService for: <%s>.", decoded_key);
			uber_free(decoded_key);
			return NULL;
		}
	}
	
	longest_replacement_length=0;
	
	debug("Creating & saving new service: <%s>.", decoded_key);
	service=online_service_new(uri, user_name, password, enabled, https, auto_connect);
	debug("New OnlineService <%s> created.  Saving OnlineServices", decoded_key);
	
	services->total++;
	debug("New service: <%s> created.  OnlineServices total: %d.", decoded_key, services->total);
	
	debug("Adding <%s> to OnlineServices' keys.", decoded_key);
	if(!( (services->keys=g_slist_append(services->keys, decoded_key)))){
		debug("**ERROR**: Failed to append new service's key: <%s>, to OnlineServices' keys.", decoded_key);
		uber_free(decoded_key);
		return NULL;
	}
	
	debug("Saving accounts & services list: '%s'.", ONLINE_SERVICES_ACCOUNTS);
	if(!( (gconfig_set_list_string(ONLINE_SERVICES_ACCOUNTS, services->keys)))){
		debug("**ERROR**: Failed to save new service: <%s>, couldn't save gconf's services list.", decoded_key);
		uber_free(decoded_key);
		return NULL;
	}
	
	debug("Adding new service: <%s> to OnlineServices.", decoded_key);
	if(!( (services->accounts=g_list_append(services->accounts, service)))){
		debug("**ERROR**: Failed to add: <%s>, to OnlineServices' accounts.", decoded_key);
		uber_free(decoded_key);
		return NULL;
	}
	
	debug("Retrieving new service: <%s> from OnlineServices accounts.", decoded_key);
	services->accounts=g_list_last(services->accounts);
	service=(OnlineService *)services->accounts->data;
	services->accounts=g_list_first(services->accounts);
	
	debug("Saving OnlineService: <%s> reloaded from OnlineServices accounts.", decoded_key);
	if(!( online_service_save(service, password, enabled, https, auto_connect))){
		debug("**ERROR**: Failed saving new service: <%s>.", decoded_key);
		uber_free(decoded_key);
		return NULL;
	}

	debug("Saving accounts & services successful.");
	if(!online_service_connect(service))
		debug("\t\tConnecting to: <%s>\t[failed]", decoded_key);
	else{
		debug("\t\tConnecting to: <%s>\t[succeeded]", decoded_key);
		
		if(!online_service_login(service, FALSE)){
			online_service_display_debug_details( service, TRUE, "re-connect");
			uber_free(decoded_key);
			return NULL;
		}
		
		if(services->total<2 && !recreating)
			main_window_tabs_init();
		main_window_state_on_connection(TRUE);
		
	}
	
	debug("Saving <%s> service complete.  Total services: %d; Total connected: %d", decoded_key, services->total, services->connected);
	
	uber_free(decoded_key);
	return service;
}/*online_services_save*/

void online_services_delete_service(OnlineService *service){
	if(!(services->connected > 1 && services->total > 1))
		main_window_state_on_connection(FALSE);
	debug("Removing old OnlineService: '%s'.", service->key);
	online_service_disconnect(service, TRUE);
	
	debug("Rebuilding OnlineServices' key while removing old key: %s.", service->key);
	g_slist_free(services->keys);
	services->keys=g_slist_alloc();
	GList *accounts=NULL;
	GSList *keys=NULL;
	debug("Rebuilding OnlineServices' keys, removing: '%s'.", service->key);
	OnlineService *key=NULL;
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		key=(OnlineService *)accounts->data;
		if(!g_str_equal(key->key, service->key))
			keys=g_slist_append(keys, (gchar *)key->key);
	}
	services->keys=keys;
	
	debug("Saving re-built OnlineServices' keys.");
	if(!( (gconfig_set_list_string(ONLINE_SERVICES_ACCOUNTS, services->keys)))){
		debug("**ERROR**: Failed to delete service: '%s', couldn't save gconf's services list.", service->key);
		return;
	}
	
	debug("Updating OnlineServices' accounts.  Removing OnlineService: [%s].", service->key);
	services->accounts=g_list_remove(services->accounts, service);
	
	debug("Determining what level of the OnlineService's cache directory to delete.");
	gboolean service_cache_rm_rf=TRUE;
	const gchar *service_uri=service->uri;
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		key=(OnlineService *)accounts->data;
		if(g_str_equal(key->uri, service_uri)){
			debug("OnlineService: [%s] share's avatars cache, only cookies will be deleted.", service->key);
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
}/*online_services_delete(service);*/



static void online_services_combo_box_add_new(GtkComboBox *combo_box, GtkListStore *list_store){
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	gtk_list_store_prepend(list_store, iter);
	gtk_list_store_set(
			list_store, iter,
				OnlineServiceKey, "[new account]",
				UrlString, NULL,
				OnlineServicePointer, NULL,
			-1
	);
	uber_free(iter);
	gtk_combo_box_set_active(combo_box, 0);
}/*online_services_combo_box_add_new(combo_box, list_store);*/

gboolean online_services_combo_box_fill(GtkComboBox *combo_box, GtkListStore *list_store, gboolean connected_only){
	if(!list_store) return FALSE;
	gtk_list_store_clear(list_store);
	if(!connected_only) online_services_combo_box_add_new(combo_box, list_store);
	if(!services->total){
		debug("No services found to load, new accounts need to be setup.");
		return FALSE;
	}
	
	GtkTreeIter		*iter=NULL;
	GList			*accounts=NULL;
	guint			services_loaded=0;
	debug("Loading services into tree view. total services: '%d'.", services->total);
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		OnlineService *service=(OnlineService *)accounts->data;
		if(connected_only && !service->connected) continue;
		
		debug("Appending account: <%s>; server: <%s>.", service->key, service->uri);
		iter=g_new0(GtkTreeIter, 1);
		gtk_list_store_append(list_store, iter);
		gtk_list_store_set(
					list_store, iter,
						OnlineServiceKey, service->key,
						UrlString, service->uri,
						OnlineServicePointer, service,
					-1
		);
		services_loaded++;
		uber_free(iter);
	}
	if(services_loaded){
		debug("Accounts & services loaded.  %s", (connected_only ?_("Selecting default account.") :_("Setting up combo_box for new account setup.") ) );
		gtk_combo_box_set_active(combo_box, 0);
	}
	
	g_list_free(accounts);
	return (services_loaded ?TRUE :FALSE );
}/*online_services_combo_box_fill*/



void online_services_decrement_total(const gchar *service_guid){
	if(services->total) services->total--;
	
	debug("OnlineServices has had OnlineService: <%s> removed.  Remaining services: #%d.", service_guid, services->total);
	if(services->total) return;
	
	main_window_state_on_connection(FALSE);
	online_services_dialog_show(main_window_get_window());
}/*online_services_decrement_total();*/

void online_services_increment_connected(const gchar *service_guid){
	services->connected++;
	debug("OnlineServices has connected to OnlineService: <%s>.  Total connected: #%d.", service_guid, services->connected);
}/*online_services_increment_connected();*/



OnlineService *online_services_connected_get_first(void){
	GList		*accounts=NULL;
	OnlineService	*service=NULL;
	
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		if(!accounts->data) continue;
		service=(OnlineService *)accounts->data;
		if(service && service->connected)
			return service;
	}
		
	g_list_free(accounts);
	return NULL;
}/*online_services_connected_get_first();*/

OnlineService *online_services_get_service_by_key(const gchar *online_service_key){
	GList		*accounts=NULL;
	OnlineService	*service=NULL;
	
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		if(g_str_equal( (service=(OnlineService *)accounts->data)->key, online_service_key))
			return service;
	}
	g_list_free(accounts);
	return NULL;
}/*online_services_get_service_by_key(Online_services, online_service_guid);*/

void online_services_increment_total(const gchar *service_guid){
	services->total++;
	debug("OnlineServices has enabled the OnlineService: <%s>.  Total services: #%d.", service_guid, services->total);
}/*online_services_increment_total();*/

OnlineService *online_services_connected_get_last(void){
	GList		*accounts=NULL;
	OnlineService	*service=NULL;
	
	for(accounts=g_list_last(services->accounts); accounts; accounts=accounts->prev){
		service=(OnlineService *)accounts->data;
		if(service->connected) return service;
	}
	return service;
}/*online_services_connected_get_last();*/

void online_services_decrement_connected(const gchar *service_guid, gboolean no_state_change){
	if(services->connected) services->connected--;
	
	debug("OnlineServices has disconnected from OnlineService: <%s>.  Remaining connections: #%d.", service_guid, services->connected);
	if(services->connected || no_state_change) return;
	
	main_window_state_on_connection(!no_state_change);
	online_services_dialog_show(main_window_get_window());
}/*online_services_decrement_connected();*/



void online_services_request(RequestMethod request, const gchar *uri, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data){
	if(G_STR_EMPTY(uri)) return;
	GList		*accounts=NULL;
	OnlineService	*service=NULL;
	
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		service=(OnlineService *)accounts->data;
		if(!(service->connected && service->authenticated)){
			if(!online_service_refresh(service)){
				debug("**ERROR:** Unable to load: %s refreshing: <%s> failed.", uri, service->key);
				continue;
			}
		}
		
		debug("Requesting: %s from <%s>.", uri, service->key);
		online_service_request(service, request, uri, online_service_soup_session_callback_return_processor_func, callback, user_data, form_data);
	}
	g_list_free(accounts);
}/*online_services_request*/



void online_services_reset_length_of_longest_replacement(void){
	if(longest_replacement_length) longest_replacement_length=0;
}/*online_services_reset_length_of_longest_replacement();*/

gssize online_services_get_length_of_longest_replacement(void){
	GList		*accounts=NULL;
	OnlineService	*service=NULL;
	gssize		replacement_length=0;
	
	if(longest_replacement_length)
		return longest_replacement_length;
	
	gint		replace_with=0;
	gconfig_get_int_or_default(PREFS_UPDATES_REPLACE_ME_W_NICK, &replace_with, 2);
	
	for(accounts=services->accounts; accounts; accounts=accounts->next){
		service=(OnlineService *)accounts->data;
		if(service->connected)
			if((replacement_length=strlen((replace_with==1?service->nick_name:service->user_name))) > longest_replacement_length)
				longest_replacement_length=replacement_length;
	}
	
	g_list_free(accounts);
	return longest_replacement_length;
}/*online_services_get_length_of_longest_replacement();*/



gint online_services_best_friends_tree_store_fill(GtkTreeStore *tree_store){
	GList		*accounts=NULL;
	gtk_tree_store_clear(tree_store);
	services->best_friends_total=0;
	for(accounts=services->accounts; accounts; accounts=accounts->next)
		services->best_friends_total+=online_service_best_friends_tree_store_fill((OnlineService *)accounts->data, tree_store);
	g_list_free(accounts);
	return services->best_friends_total;
}/*online_services_best_friends_tree_store_fill(tree_store);*/


gint online_services_best_friends_tree_store_validate(GtkTreeStore *tree_store){
	GList		*accounts=NULL;
	gtk_tree_store_clear(tree_store);
	services->best_friends_total=0;
	for(accounts=services->accounts; accounts; accounts=accounts->next)
		services->best_friends_total+=online_service_best_friends_tree_store_validate((OnlineService *)accounts->data, tree_store);
	g_list_free(accounts);
	return services->best_friends_total;
}/*online_services_best_friends_tree_store_fill(tree_store);*/


void online_services_best_friends_tree_store_free(GtkTreeStore *tree_store){
	GList		*accounts=NULL;
	for(accounts=services->accounts; accounts; accounts=accounts->next)
		online_service_best_friends_tree_store_free((OnlineService *)accounts->data, tree_store);
	g_list_free(accounts);
}/*online_services_best_friends_tree_store_free();*/

static gboolean online_services_best_friends_tree_store_get_user_iter(OnlineService *service, const gchar *user_name, GtkTreeStore *tree_store, GtkTreeIter **iter){
	if(!(services->best_friends_total && G_STR_N_EMPTY(user_name))) return FALSE;
	
	OnlineService *service_at_index=NULL;
	gchar *user_at_index=NULL;
	for(gint i=0; i<=services->best_friends_total; i++){
		*iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter( (GtkTreeModel *)tree_store, *iter, path)){
			debug("Failed to get best friend: %s, on %s aned index: %d, as read has failed.  Unable to retrieve iter from path.", user_name, service->guid, i);
			gtk_tree_path_free(path);
			uber_free(*iter);
			continue;
		}
		
		gtk_tree_model_get(
				 (GtkTreeModel *)tree_store, *iter,
				 	ONLINE_SERVICE_BEST_FRIEND_ONLINE_SERVICE, &service_at_index,
					STRING_BEST_FRIEND_USER, &user_at_index,
				-1
		);
		if(!( service==service_at_index && !strcasecmp(user_name, user_at_index))){
			gtk_tree_path_free(path);
			uber_free(user_at_index);
			uber_free(*iter);
			continue;
		}
		
		debug("Found best friend iter for best friend: %s, on service: <%s>, at index: %d.", user_at_index, service_at_index->guid, i);
		gtk_tree_path_free(path);
		uber_free(user_at_index);
		return TRUE;
	}
	debug("Unable to find best friend iter for best friend: %s, on service: <%s>.", user_at_index, service_at_index->guid);
	uber_free(user_at_index);
	if(*iter) uber_free(*iter);
	return FALSE;
}/*online_services_best_friends_tree_store_get_user_iter(service, user_name, tree_store, &iter);*/

gdouble online_services_best_friends_tree_store_mark_as_unread(OnlineService *service, const gchar *user_name, gdouble update_id, GtkTreeStore *tree_store){
	if(!(services->best_friends_total && G_STR_N_EMPTY(user_name))) return FALSE;
	
	GtkTreeIter *iter=NULL;
	if(!online_services_best_friends_tree_store_get_user_iter(service, user_name, tree_store, &iter)){
		debug("User: %s  on service: <%s>, could not be found in your best friends list store.", user_name, service->guid);
		return 0.0;
	}
	
	guint unread_updates=0;
	gdouble unread_update_id=0.0;
	gchar *user_at_index=NULL, *user_name_at_index=NULL;
	gtk_tree_model_get(
			 (GtkTreeModel *)tree_store, iter,
				STRING_BEST_FRIEND_USER, &user_at_index,
				STRING_BEST_FRIEND_USER_NAME, &user_name_at_index,
				GDOUBLE_BEST_FRIENDS_UNREAD_UPDATE_ID, &unread_update_id,
				GUINT_BEST_FRIENDS_UNREAD_UPDATES, &unread_updates,
			-1
	);
	
	if(!(unread_update_id && update_id && update_id > unread_update_id )) return unread_update_id;
	
	unread_updates++;
	if(!g_str_has_prefix(user_name_at_index, "<b>")){
		uber_free(user_name_at_index);
		user_name_at_index=g_strdup_printf("<b>%s</b>", user_at_index);
	}
	
	debug("Marking best friend: %s(%s), on service <%s>, as having %d unread updates.  Last read update: %f; current update ID: %f..", user_at_index, user_name_at_index, service->guid, unread_updates, unread_update_id, update_id);
	gtk_tree_store_set(
			tree_store, iter,
				STRING_BEST_FRIEND_USER_NAME, user_name_at_index,
				GDOUBLE_BEST_FRIENDS_UNREAD_UPDATE_ID, unread_update_id,
				GUINT_BEST_FRIENDS_UNREAD_UPDATES, unread_updates,
			-1
	);
	
	uber_free(iter);
	uber_free(user_at_index);
	uber_free(user_name_at_index);
	update_viewer_sexy_select();
	return update_id;
}/*online_services_best_friends_tree_store_mark_as_unread(user_name)*/

gboolean online_services_best_friends_tree_store_mark_as_read(OnlineService *service, const gchar *user_name, gdouble update_id, GtkTreeStore *tree_store){
	if(!(services->best_friends_total && G_STR_N_EMPTY(user_name))) return FALSE;
	
	GtkTreeIter *iter=NULL;
	if(!online_services_best_friends_tree_store_get_user_iter(service, user_name, tree_store, &iter)){
		debug("User: %s  on service: <%s>, could not be found in your best friends list store.", user_name, service->guid);
		return FALSE;
	}
	
	guint unread_updates=0;
	gchar *user_at_index=NULL, *user_name_at_index=NULL;
	gtk_tree_model_get(
			 (GtkTreeModel *)tree_store, iter,
				STRING_BEST_FRIEND_USER, &user_at_index,
				STRING_BEST_FRIEND_USER_NAME, &user_name_at_index,
				GUINT_BEST_FRIENDS_UNREAD_UPDATES, &unread_updates,
			-1
	);
	
	gchar		*user_timeline=g_strdup_printf("/%s.xml", user_at_index);
	gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	online_service_update_ids_get(service, user_timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	online_service_update_ids_set(service, user_timeline, ( (update_id>newest_update_id) ?update_id :newest_update_id) , ( (update_id>unread_update_id) ?(unread_update_id=update_id) :unread_update_id ), ( (update_id>oldest_update_id) ?update_id :oldest_update_id));
	uber_free(user_timeline);
	
	debug("Marking best friend: %s, on service <%s>, as having all of their updates as read.", user_at_index, service->guid);
	gtk_tree_store_set(
			tree_store, iter,
				STRING_BEST_FRIEND_USER_NAME, user_at_index,
				GUINT_BEST_FRIENDS_UNREAD_UPDATES, 0,
				GDOUBLE_BEST_FRIENDS_UNREAD_UPDATE_ID, unread_update_id,
			-1
	);
	
	uber_free(iter);
	uber_free(user_at_index);
	uber_free(user_name_at_index);
	update_viewer_sexy_select();
	return TRUE;
}/*online_services_best_friends_tree_store_mark_as_read(service, user_name, tree_view, tree_store);*/

gboolean online_services_is_user_best_friend(OnlineService *service, const gchar *user_name){
	if(G_STR_EMPTY(user_name)) return FALSE;
	GList		*accounts=NULL;
	for(accounts=services->accounts; accounts; accounts=accounts->next)
		if((service==(OnlineService *)accounts->data) && online_service_is_user_best_friend((OnlineService *)accounts->data, user_name))
			return TRUE;
	
	g_list_free(accounts);
	return FALSE;
}/*online_services_is_user_best_friend(service, user_name);*/

gint online_services_best_friends_total_update(gint best_friends_to_add){
	return (services->best_friends_total+=best_friends_to_add);
}/*online_services_best_friends_total_update( Online_service, 1 || -1);*/


void online_services_deinit(void){
	debug("**SHUTDOWN:** Closing & releasing %d accounts.", services->total);
	g_list_foreach(services->accounts, (GFunc)online_service_free, GINT_TO_POINTER(TRUE) );
	g_list_free(services->accounts);

	debug("**SHUTDOWN:** freeing OnlineServices keys.");
	g_slist_free(services->keys);
	services->keys=NULL;
	
	selected_service=NULL;
	uber_free(services);
}/*online_services_deinit*/

/********************************************************
 *                       eof                            *
 ********************************************************/

