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

#include "cache.h"

#include "users.types.h"
#include "users-glists.h"
#include "users.h"

#include "main-window.h"

#include "online-services.typedefs.h"
#include "online-service-wrapper.h"
#include "online-service-request.h"
#include "online-service.types.h"
#include "online-service.h"

#include "network.h"
#include "www.h"
#include "parser.h"

#include "friends-manager.h"
#include "following-viewer.h"
#include "update-viewer.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define	DEBUG_DOMAINS	"Networking:OnlineServices:Friends:Followers:XML:Parser:UsersGlists:users-glists.c"
#include "debug.h"


#define API_FOLLOWING		"/statuses/friends.xml"
#define API_FOLLOWERS		"/statuses/followers.xml"

static OnlineService *service=NULL;
static UsersGListOnLoadFunc on_load_func=NULL;
static guint page=0;

static gboolean fetching_users=FALSE;
gboolean getting_followers=FALSE;

static guint which_pass=0;

/*static UsersGListGetWhich users_glist_get_which_previous;*/


/*******************************************************
 *          Static method & function prototypes         *
 ********************************************************/
const gchar *users_glist_get_which_to_string(UsersGListGetWhich users_glist_get_which);

static int users_glists_sort_by_user_name(User *user1, User *user2);

static GList *users_glist_check(UsersGListGetWhich users_glist_get_which, gboolean refresh, UsersGListOnLoadFunc func);
static gboolean users_glist_get_recall_check(UsersGListGetWhich users_glist_get_which, OnlineService *service);

static GList *users_glist_parse(OnlineService *service, SoupMessage *xml);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
const gchar *users_glist_get_which_to_string(UsersGListGetWhich users_glist_get_which){
	switch(users_glist_get_which){
		case GetFriends: return "friends";
		case GetFollowers: return "followers";
		case GetBoth: return "friends and followers";
		default:
			      debug("**ERROR:** Attempting to download unsupport list of user relationships.");
			      return "**ERROR:** Unsupported [UsersGlistGetWhich type]";
	}
}/*users_glist_get_which_to_string(users_glist_get_which);*/

GList *users_glist_get(UsersGListGetWhich users_glist_get_which, gboolean refresh, UsersGListOnLoadFunc func){
	if(!selected_service){
		debug("**ERROR:** Default OnlineService is not set.  I'm unable to retreive page %d your %s.", page, (which_pass ?_("who are following you") :_("you're following")));
		return NULL;
	}
	
	debug("Loading users_glist; users_glist_get_which type: %s. page #%d; on pass: #%d.", users_glist_get_which_to_string(users_glist_get_which), page, which_pass );
	
	GList *users=NULL;
	if( (!fetching_users) && (users=users_glist_check( users_glist_get_which, refresh, func)) )
		return users;
	
	fetching_users=TRUE;
	page++;
	
	gchar *uri=NULL;
	if(users_glist_get_recall_check(users_glist_get_which, service)){
		if(fetching_users) fetching_users=FALSE;
		return users_glist_get(users_glist_get_which, FALSE, NULL);
	}
	
	if(!which_pass)
		uri=g_strdup_printf("%s?page=%d", API_FOLLOWING, page);
	else
		uri=g_strdup_printf("%s?page=%d", API_FOLLOWERS, page);
	
	const gchar *users_glist_get_which_str=users_glist_get_which_to_string(users_glist_get_which);
	debug("Downloading %s page #%d(pass #%d).", users_glist_get_which_str, page, which_pass );
	main_window_statusbar_printf("Please wait while %s downloads your %s(page #%d on pass #%d).", _(GETTEXT_PACKAGE), users_glist_get_which_str, page, which_pass );
	
	debug("Getting users_glist; uri: [%s]; page: %d; pass: %d; users_glist_get_which type: %s", uri, page, which_pass, users_glist_get_which_str );
	online_service_request(service, QUEUE, uri, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)users_glist_save, users_glist_process, GINT_TO_POINTER(users_glist_get_which), GINT_TO_POINTER(which_pass));
	
	g_free(uri);
	return NULL;
}/*users_glist_get(users_glist_get_which, (refresh ?TRUE: FALSE), NULL);*/

static GList *users_glist_check(UsersGListGetWhich users_glist_get_which, gboolean refresh, UsersGListOnLoadFunc func){
	if(!which_pass){
		on_load_func=func;
		service=selected_service;
		if(refresh && service && service==selected_service)
			users_glists_free_lists(service, users_glist_get_which);
	}
	
	GList *users=NULL;
	if( service && service==selected_service ){
		const gchar *users_glist_get_which_str=users_glist_get_which_to_string(users_glist_get_which);
		switch(users_glist_get_which){
			case GetFriends:
				if(!service->friends) break;
				debug("Displaying & loading, <%s>'s %s from %d pages.", service->key, users_glist_get_which_str, page-1);
				users=g_list_sort(g_list_first(service->friends), (GCompareFunc)users_glists_sort_by_user_name);
				break;
			case GetFollowers:
				if(!service->followers) break;
				debug("Displaying & loading, <%s>'s %s from %d pages.", service->key, users_glist_get_which_str, page-1);
				users=g_list_sort(g_list_first(service->followers), (GCompareFunc)users_glists_sort_by_user_name);
				break;
			case GetBoth:
				if(!service->friends){
					if(which_pass!=0) which_pass=0;
					break;
				}
				if(!service->followers){
					if(which_pass!=1) which_pass=1;
					break;
				}
				debug("Displaying & loading, <%s>'s %s from %d pages.", service->key, users_glist_get_which_str, page-1);
				if(!service->friends_and_followers){
					service->friends_and_followers=g_list_alloc();
					service->friends_and_followers=g_list_concat(g_list_first(service->friends), g_list_first(service->followers));
				}
				
				users=g_list_sort(service->friends_and_followers, (GCompareFunc)users_glists_sort_by_user_name);
				break;
		}
		page=0;
		if(users){
			which_pass=0;
			if(on_load_func!=NULL)
				on_load_func(users);
			on_load_func=NULL;
		}
	}
	return users;
}/*users_glist_check();*/

static gboolean users_glist_get_recall_check(UsersGListGetWhich users_glist_get_which, OnlineService *service){
	switch(users_glist_get_which){
		case GetBoth:
		case GetFriends:
			if(which_pass==0) return FALSE;
			if(!service->friends){
				which_pass=0;
				return TRUE;
			}
			if(users_glist_get_which!=GetBoth) return FALSE;
		case GetFollowers:
			if(which_pass==1) return FALSE;
			if(!service->followers){
				which_pass=1;
				return TRUE;
			}
			if(users_glist_get_which!=GetBoth) return FALSE;
		default: /*handle remaining 'GetBoth' checks:*/
			if(which_pass==2) return FALSE;
			if(!(service->friends && service->followers)){
				if(!service->friends) which_pass=0;
				else if(!service->followers) which_pass=1;
				else which_pass=2;
				return TRUE;
			}else{
				which_pass=2;
				return TRUE;
			}
			break;
	}
	return FALSE;
}/*users_glist_get_recall_check(service);*/

void *users_glist_process(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	const gchar *uri=online_service_wrapper_get_requested_uri(service_wrapper);
	UsersGListGetWhich users_glist_get_which=(UsersGListGetWhich)GPOINTER_TO_INT(online_service_wrapper_get_user_data(service_wrapper));
	const gchar *users_glist_get_which_str=users_glist_get_which_to_string(users_glist_get_which);
	const gchar *page_num_str=g_strrstr(g_strrstr(uri, "?"), "=");
	debug("Processing users_glist; users_glist_get_which type: %s", users_glist_get_which_to_string(users_glist_get_which) );
	debug("Processing <%s>'s %s, page #%s.  Server response: %s [%i].", service->key, users_glist_get_which_str, page_num_str, xml->reason_phrase, xml->status_code);
	
	gchar *error_message=NULL;
	if(!(www_xml_error_check(service, uri, xml, &error_message))){
		debug("**ERROR:** No more %s could be downloaded the request was not successful.", users_glist_get_which_str);
		debug("**ERROR:** <%s>'s %s should be refreshed.", service->key, users_glist_get_which_str);
		uber_free(error_message);
		return NULL;
	}
	uber_free(error_message);
	
	getting_followers=( g_strrstr(uri, API_FOLLOWERS) ? TRUE : FALSE );
	GList *new_users=NULL;
	debug("Parsing user list");
	if(!(new_users=users_glist_parse(service, xml)) ){
		debug("No more %s where found, yippies we've got'em all.", users_glist_get_which_str);
		return NULL;
	}
	
	return new_users;
}/*users_glist_process(session, xml, service_wrapper);*/

void users_glist_save(OnlineServiceWrapper *service_wrapper, SoupMessage *xml, GList *new_users){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	const gchar *uri=online_service_wrapper_get_requested_uri(service_wrapper);
	UsersGListGetWhich users_glist_get_which=(UsersGListGetWhich)GPOINTER_TO_INT(online_service_wrapper_get_user_data(service_wrapper));
	const gchar *users_glist_get_which_str=users_glist_get_which_to_string(users_glist_get_which);
	
	if(!new_users){
		GList *users=NULL;
		if(!which_pass) users=service->friends;
		else users=service->followers;
		users=g_list_last(users);
		users=g_list_remove(users, users->data);
		which_pass++;
		fetching_users=FALSE;
		users_glist_get(users_glist_get_which, FALSE, NULL);
		return;
	}
	
	debug("Appending users to <%s>'s %s. parsed from uri: [%s].", service->key, users_glist_get_which_str, uri);
	if(!which_pass){
		if(!service->friends)
			service->friends=new_users;
		else
			service->friends=g_list_append(service->friends, new_users);
	}else{
		if(!service->followers)
			service->followers=new_users;
		else
			service->followers=g_list_append(service->followers, new_users);
	}
	
	/*cache_save_page(service, uri, xml->response_body);*/
	
	/*now we get the next page - or send the results where they belong.*/
	debug("Retrieving the next page of %s.", (which_pass ?"followers" :"friends") );
	debug("Saving users_glist; page %d - pass %d users_glist_get_which type: %s", page, which_pass, users_glist_get_which_to_string(users_glist_get_which) );
	users_glist_get(users_glist_get_which, FALSE, NULL);
}/*users_glist_save*/

/**
 *returns: -1, 0 or 1, if user1 is <, == or > than user2.
 */
int users_glists_sort_by_user_name(User *user1, User *user2){
	if(!( user2 && user2->user_name )) return 1;
	return strcasecmp( user1->user_name, user2->user_name );
}/*user_sort_by_user_name(l1->data, { l1=l1->next; l1->data; } );*/


GList *users_glist_parse(OnlineService *service, SoupMessage *xml){
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	
	debug("Parsing users xml listing.");
	if(!( (doc=parse_xml_doc(xml, &root_element)) && root_element )){
		xmlCleanupParser();
		return NULL;
	}
	
	debug("Parsed new users XML.  beginning search for user nodes starting at: <%s>", root_element->name);
	
	User *user=NULL;
	GList *users=NULL;
	xmlNode *current_node=NULL;
	gboolean users_found=FALSE;
	for(current_node=root_element; current_node; current_node=current_node->next){
		if( current_node->type!=XML_ELEMENT_NODE ) continue;
		
		if(g_str_equal(current_node->name, "users")){
			if(!current_node->children) break;
			current_node=current_node->children;
			users_found=TRUE;
		}
		if(!users_found) continue;
		
		if(!( g_str_equal(current_node->name, "user") && current_node->children ))
			continue;
		
		if(!( (((current_node->children))) && (user=user_parse_node(service, current_node->children)) ))
			continue;
		if(!( user && user->id && user->user_name )) continue;
		
		debug("Added user: [%s] to user list.", user->user_name);
		users=g_list_append(users, user);
		user=NULL;
	}
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	if(!users_found){
		debug("No new users where found.");
		return NULL;
	}
	
	return users;
}/*users_glist_parse(service, xml)*/


gboolean users_glists_free_lists(OnlineService *service, UsersGListGetWhich users_glist_get_which){
	switch(users_glist_get_which){
		case GetBoth:
			if(!service->friends_and_followers) return FALSE;
			users_glists_free(service, GetBoth);
			return TRUE;
		case GetFriends:
			if(!service->friends) return FALSE;
			users_glists_free(service, GetFriends);
			return TRUE;
		case GetFollowers:
			if(!service->followers)return FALSE;
			users_glists_free(service, GetFollowers);
			return TRUE;
	}
	return FALSE;
}/*users_glists_free_lists(service, GetBoth|GetFriends|GetFollowers);*/

void users_glists_free(OnlineService *service, UsersGListGetWhich users_glist_get_which){
	GList *glist_free=NULL;
	switch(users_glist_get_which){
		case GetBoth:
			debug("%s %s %s %s %s, %s %s %s %s %s %s %s %s.", _("friends"), _("of"), service->key, _("i.e."), _("who"), _("is"), _("following"), _("you"), _("and"), _("who"), _("is"), _("following"), service->key);
			if(service->friends_and_followers) uber_list_free(service->friends_and_followers);
			if(service->friends) users_glists_free(service, GetFriends);
			if(service->followers) users_glists_free(service, GetFollowers);
			return;
			
		case GetFriends:
			if(!service->friends) return;
			glist_free=service->friends;
			debug("%s %s %s %s, %s %s %s %s.", _("friends"), _("of"), service->key, _("i.e."), _("who"), _("is"), _("following"), service->key);
			break;
			
		case GetFollowers:
			if(!service->followers) return;
			glist_free=service->followers;
			debug("%s %s %s %s.", _("who"), _("is"), _("following"), service->key);
			break;
	}
	
	g_list_foreach(glist_free, (GFunc)user_free, NULL);
	uber_list_free(glist_free);
}/*users_glists_free(service, GetBoth);*/


void users_glists_append_friend(OnlineService *service, User *user){
	main_window_statusbar_printf("%s %s %s %s %s.", service->key, _("'s"), _("now"), _("following"), user->user_name);
	service->friends=g_list_append(service->friends, user);
}/*users_glists_append_friend(service, user);*/

void users_glists_append_follower(OnlineService *service, User *user){
	main_window_statusbar_printf("%s %s %s %s %s %s %s.", service->key, _("'s"), _("now"), _("being"), _("followed"), _("by"), user->user_name);
	service->followers=g_list_append(service->followers, user);
}/*users_glists_append_follower(service, user);*/

/********************************************************
 *                       eof                            *
 ********************************************************/
