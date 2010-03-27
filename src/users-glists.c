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

#include "online-services.typedefs.h"
#include "online-services.types.h"

#include "online-service.types.h"
#include "online-service.h"

#include "online-service-wrapper.h"
#include "online-service-request.h"

#include "network.h"
#include "www.h"
#include "xml.h"

#include "users.types.h"
#include "users-glists.h"
#include "users.h"

#include "main-window.h"
#include "friends-manager.h"
#include "following-viewer.h"
#include "update-viewer.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define	DEBUG_DOMAINS	"Networking:OnlineServices:Friends:Followers:XML:Parser:UsersGlists:users-glists.c"
#include "debug.h"


#define API_FOLLOWING		"/statuses/friends/%s.xml?%s=%li"
#define API_FOLLOWERS		"/statuses/followers/%s.xml?%s=%li"

static OnlineService *service=NULL;
static UsersGListOnLoadFunc on_load_func=NULL;
static UsersGListGetWhich users_glist_get_which;

static long signed int next_cursor=-1;
static long signed int previous_cursor=-1;
static guint page=0;

static gboolean fetching_users=FALSE;
gboolean getting_followers=FALSE;

static gint which_pass=-1;
const gchar *get_parameter=NULL;

/*static UsersGListGetWhich users_glist_get_which_previous;*/


/*******************************************************
 *          Static method & function prototypes         *
 ********************************************************/
const gchar *users_glist_get_which_to_string(UsersGListGetWhich users_glist_get_which);

static int users_glists_sort_by_user_name(User *user1, User *user2);

static GList *users_glist_check(UsersGListGetWhich users_glist_get_which_list, gboolean refresh, UsersGListOnLoadFunc func);
static gboolean users_glist_get_recall_check(UsersGListGetWhich users_glist_get_which, OnlineService *service);

GList *users_glist_parse(OnlineServiceWrapper *service_wrapper, SoupMessage *xml);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
const gchar *users_glist_get_which_to_string(UsersGListGetWhich users_glist_get_which){
	switch(users_glist_get_which){
		case GetFriends: return _("friends");
		case GetFollowers: return _("followers");
		case GetBoth: return _("friends and followers");
		default:
			      debug("**ERROR:** Attempting to download unsupport list of user relationships");
			      return "**ERROR:** Unsupported [UsersGlistGetWhich type]";
	}
}/*users_glist_get_which_to_string(users_glist_get_which);*/

GList *users_glist_get(UsersGListGetWhich users_glist_get_which, gboolean refresh, UsersGListOnLoadFunc func){
	if(!selected_service){
		debug("**ERROR:** Default OnlineService is not set.  I'm unable to retreive the users who %s", (which_pass ?_("are following you") :_("you're following")));
		return NULL;
	}
	
	GList *users=NULL;
	if( (!fetching_users) && (users=users_glist_check(users_glist_get_which, refresh, func)) )
		return users;
	
	if(!fetching_users) fetching_users=TRUE;
	
	if(users_glist_get_recall_check(users_glist_get_which, service)){
		if(fetching_users) fetching_users=FALSE;
		return users_glist_get(users_glist_get_which, FALSE, NULL);
	}
	
	if(service->micro_blogging_service!=Twitter){
		get_parameter="page";
		if(next_cursor<0)
			next_cursor++;
		next_cursor++;
	}else{
		get_parameter="cursor";
		if(next_cursor==0){
			if(which_pass<1) which_pass++;
			else which_pass=2;
		}
	}
	debug("Initial service parameters setup");
	debug("get_parameter: [%s]; page/cursor: %li; which_pass: %i", get_parameter, next_cursor, which_pass);
	
	debug("Loading users_glist; users_glist_get_which type: %s. %s #%li; on which_pass: #%d", users_glist_get_which_to_string(users_glist_get_which), get_parameter, next_cursor, which_pass);
	
	gchar *uri=NULL;
	if(!which_pass)
		uri=g_strdup_printf(API_FOLLOWING, service->user_name, get_parameter, next_cursor);
	else
		uri=g_strdup_printf(API_FOLLOWERS, service->user_name, get_parameter, next_cursor);
	
	const gchar *users_glist_get_which_str=users_glist_get_which_to_string(users_glist_get_which);
	
	statusbar_printf("Downloading %s page: %i; uri: <%s>", (which_pass ?_("who are following you") :_("you're following")), page, uri);
	debug("Downloading users_glist page: %i; uri: <%s>", page, uri);
	debug("requesting %s: %li; which_pass: %d; users_glist_get_which type: %s", get_parameter, next_cursor, which_pass, users_glist_get_which_str );
	online_service_request(service, QUEUE, uri, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)users_glist_save, users_glist_process, GINT_TO_POINTER(users_glist_get_which), GINT_TO_POINTER(which_pass));
	
	uber_free(uri);
	return NULL;
}/*users_glist_get(users_glist_get_which, (refresh ?TRUE: FALSE), NULL);*/

static GList *users_glist_check(UsersGListGetWhich users_glist_get_which_list, gboolean refresh, UsersGListOnLoadFunc func){
	if(which_pass==-1){
		if(fetching_users) fetching_users=FALSE;
		on_load_func=func;
		service=selected_service;
		users_glist_get_which=users_glist_get_which_list;
		page=0;
		next_cursor=-1;
		previous_cursor=-1;
		
		if(refresh && service && service==selected_service)
			users_glists_free_lists(service, users_glist_get_which);
	}
	
	if(!(service && service==selected_service))
		return NULL;
	
	GList *users=NULL;
	const gchar *users_glist_get_which_str=users_glist_get_which_to_string(users_glist_get_which);
	switch(users_glist_get_which){
		case GetFriends:
			if(!service->friends){
				if(which_pass!=0){
					which_pass=0;
					next_cursor=-1;
					previous_cursor=-1;
					page=0;
				}
				return NULL;
			}
			debug("Displaying & loading, <%s>'s %s from %i pages", service->key, users_glist_get_which_str, page);
			users=g_list_sort(g_list_first(service->friends), (GCompareFunc)users_glists_sort_by_user_name);
			break;
		case GetFollowers:
			if(!service->followers){
				if(which_pass!=1){
					which_pass=1;
					next_cursor=-1;
					previous_cursor=-1;
					page=0;
				}
				return NULL;
			}
			debug("Displaying & loading, <%s>'s %s from %i pages", service->key, users_glist_get_which_str, page);
			users=g_list_sort(g_list_first(service->followers), (GCompareFunc)users_glists_sort_by_user_name);
			break;
		case GetBoth:
			if(!service->friends){
				if(which_pass!=0){
					which_pass=0;
					next_cursor=-1;
					previous_cursor=-1;
					page=0;
				}
				return NULL;
			}
			if(!service->followers){
				if(which_pass!=1){
					which_pass=1;
					next_cursor=-1;
					previous_cursor=-1;
					page=0;
				}
				return NULL;
			}
			/*if(which_pass!=2){
				which_pass=2;
				next_cursor=-1;
				previous_cursor=-1;
				page=0;
			}*/
			debug("Displaying & loading, <%s>'s %s from %i pages", service->key, users_glist_get_which_str, page);
			if(!service->friends_and_followers){
				GList *friends=g_list_last(service->friends);
				friends=g_list_remove(friends, friends->data);
				
				GList *followers=g_list_last(service->followers);
				followers=g_list_remove(followers, followers->data);
				
				service->friends_and_followers=g_list_alloc();
				service->friends_and_followers=g_list_concat(g_list_first(friends), g_list_first(followers));
				users=service->friends_and_followers;
			}else
				users=g_list_sort(g_list_first(service->friends_and_followers), (GCompareFunc)users_glists_sort_by_user_name);
			break;
	}
	
	if(fetching_users) fetching_users=FALSE;
	page=0;
	which_pass=-1;
	next_cursor=-1;
	previous_cursor=-1;
	if(on_load_func!=NULL)
		on_load_func(users);
	on_load_func=NULL;
	return users;
}/*users_glist_check(users_glist_get_which, refresh, func);*/

static gboolean users_glist_get_recall_check(UsersGListGetWhich users_glist_get_which, OnlineService *service){
	if(which_pass==-1)
		which_pass=0;
	
	switch(users_glist_get_which){
		case GetBoth:
		case GetFriends:
			if(which_pass<1) return FALSE;
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
			if(which_pass>1) return FALSE;
			if(!(service->friends && service->followers)){
				if(!service->friends) which_pass=0;
				else if(!service->followers) which_pass=1;
				else which_pass=2;
				return TRUE;
			}else{
				which_pass=2;
				return FALSE;
			}
			break;
	}
	return FALSE;
}/*users_glist_get_recall_check(GetFriends|GetFollowers|GetBoth, service);*/

/*void *users_glist_process(OnlineService *service, OnlineServiceXmlDoc *xml_doc, OnlineServiceWrapper *service_wrapper){*/
void *users_glist_process(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	const gchar *uri=online_service_wrapper_get_requested_uri(service_wrapper);
	/*UsersGListGetWhich users_glist_get_which=(UsersGListGetWhich)GPOINTER_TO_INT(online_service_wrapper_get_user_data(service_wrapper));*/
	const gchar *users_glist_get_which_str=users_glist_get_which_to_string(users_glist_get_which);
	const gchar *next_cursor_str=g_strrstr(g_strrstr(uri, "?"), "=");
	debug("Processing users_glist; users_glist_get_which type: %s", users_glist_get_which_to_string(users_glist_get_which) );
	debug("Processing <%s>'s %s, %s #%s.  Server response: %s [%i]", service->key, users_glist_get_which_str, get_parameter, next_cursor_str, xml->reason_phrase, xml->status_code);
	
	gchar *error_message=NULL;
	if(!(xml_error_check(service, uri, xml, &error_message))){
		debug("**ERROR:** No more %s could be downloaded the request was not successful", users_glist_get_which_str);
		debug("**ERROR:** <%s>'s %s should be refreshed", service->key, users_glist_get_which_str);
		uber_free(error_message);
		return NULL;
	}
	uber_free(error_message);
	
	getting_followers=(which_pass ?TRUE :FALSE);
	GList *new_users=NULL;
	debug("Parsing user list");
	if(!(new_users=users_glist_parse(service_wrapper, xml)) ){
		debug("No more %s where found, yippies we've got'em all", users_glist_get_which_str);
		return NULL;
	}
	
	page++;
	return new_users;
}/*users_glist_process(session, xml, service_wrapper);*/

void users_glist_save(OnlineServiceWrapper *service_wrapper, SoupMessage *xml, GList *new_users){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	const gchar *uri=online_service_wrapper_get_requested_uri(service_wrapper);
	/*UsersGListGetWhich users_glist_get_which=(UsersGListGetWhich)GPOINTER_TO_INT(online_service_wrapper_get_user_data(service_wrapper));*/
	const gchar *users_glist_get_which_str=users_glist_get_which_to_string(users_glist_get_which);
	
	if(!new_users){
		if(which_pass<1) which_pass++;
		else which_pass=2;
		fetching_users=FALSE;
		users_glist_get(users_glist_get_which, FALSE, NULL);
		return;
	}
	
	debug("Appending users to <%s>'s %s. parsed from uri: [%s]", service->key, users_glist_get_which_str, uri);
	if(which_pass<1){
		if(!service->friends)
			service->friends=new_users;
		else
			service->friends=g_list_append(service->friends, new_users);
	}else if(which_pass==1){
		if(!service->followers)
			service->followers=new_users;
		else
			service->followers=g_list_append(service->followers, new_users);
	}else{
		debug("**ERROR:** Unknown list condition reached for <%s>", uri);
		debug("**ERROR:** Failed to save users. page: %i; which_pass: %i; %s: %li", page, which_pass, get_parameter, next_cursor);
		if(which_pass<1) which_pass++;
		else which_pass=2;
		fetching_users=FALSE;
		users_glist_get(users_glist_get_which, FALSE, NULL);
		return;
	}
	
	if(service->micro_blogging_service==Twitter && next_cursor==0){
		if(which_pass<1) which_pass++;
		else which_pass=2;
		fetching_users=FALSE;
		users_glist_get(users_glist_get_which, FALSE, NULL);
		return;
	}
	
	/*cache_save_page(service, uri, xml->response_body);*/
	
	/*now we get the next page/next_cursor - or send the results where they belong.*/
	debug("Appended new_users to <%s>->%s from; <%s>", service->key, (which_pass ?"followers" :"friends"), uri);
	debug("While processing the current request to %s: which_pass: %i; page: %i", users_glist_get_which_to_string(users_glist_get_which), which_pass, page);
	debug("Retrieving next: <%s?%li>; ", get_parameter, next_cursor);
	users_glist_get(users_glist_get_which, FALSE, NULL);
}/*users_glist_save(service_wrapper, xml, users);*/

GList *users_glist_parse(OnlineServiceWrapper *service_wrapper, SoupMessage *xml){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	const gchar *uri=online_service_wrapper_get_requested_uri(service_wrapper);
	
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	
	debug("Parsing users from: <%s>", uri);
	if(!( (doc=xml_create_xml_doc_and_get_root_element_from_soup_message(xml, &root_element)) && root_element )){
		debug("[failed]");
		xmlCleanupParser();
		return NULL;
	}
	debug("[succeed]");
	
	gchar		*content=NULL;
	xmlNode		*current_element=NULL;
	debug("Parsed new users XML. Searching for users; starting element: <%s>", root_element->name);
	
	debug("Searching through <%s>'s elements", uri);
	debug("Searching for <users>, <next_cursor>, and <previous_cursor> elements");
	for(current_element=root_element; current_element; current_element=current_element->next){
		if(current_element->type!=XML_ELEMENT_NODE) continue;
		debug("Current element: <%s>; type: [%s]", current_element->name, xml_node_type_to_string(current_element->type));
		if((g_str_equal(current_element->name, "users_list")) && current_element->children){
			root_element=current_element->children;
			debug("Found <%s>'s <users_list> element", uri);
			debug("Moving root_element from: <%s>; to child element: <%s>", current_element->name, root_element->name);
			break;
		}
	}
	if((g_str_equal(root_element->name, "users_list")) && root_element->children){
		root_element=root_element->children;
		debug("Found <%s>'s users_list: moving to its children element: <%s>", uri, root_element->name);
		
		debug("2nd pass: Searching through <%s>'s elements", uri);
		debug("2nd pass: Searching for <users>, <next_cursor>, and <previous_cursor> elements");
		for(current_element=root_element; current_element; current_element=current_element->next){
			if(current_element->type!=XML_ELEMENT_NODE) continue;
			debug("2nd pass: Current element: <%s>; type: [%s]", current_element->name, xml_node_type_to_string(current_element->type));
			if(g_str_equal(current_element->name, "users")){
				root_element=current_element;
				break;
			}
		}
	}
	
	GList		*users=NULL;
	for(current_element=root_element; current_element; current_element=current_element->next){
		debug("Parsing users, next_cursor, & previous_cursor.  Current element: <%s>; type: [%s]", current_element->name, xml_node_type_to_string(current_element->type));
		if(current_element->type!=XML_ELEMENT_NODE) continue;
		
		if(g_str_equal(current_element->name, "next_cursor")){
			content=(gchar *)xmlNodeGetContent(current_element);
			next_cursor=g_ascii_strtoll(content, NULL, 0);
			debug("**DEBUG:** Found <%s>'s \"next_cursor\": %li; string: [%s]", uri, next_cursor, content);
			uber_free(content);
			continue;
		}
		
		if(g_str_equal(current_element->name, "previous_cursor")){
			content=(gchar *)xmlNodeGetContent(current_element);
			previous_cursor=g_ascii_strtoll(content, NULL, 0);
			debug("**DEBUG:** Found <%s>'s \"previous_cursor\": %li; string: [%s]", uri, previous_cursor, content);
			uber_free(content);
			continue;
		}
		
		if(!g_str_equal(current_element->name, "users")) continue;
		if(!current_element->children) continue;
		
		xmlNode *user_element=NULL;
		debug("Parsing users from <%s>; current element: <%s>", uri, current_element->name);
		for(user_element=current_element->children; user_element; user_element=user_element->next){
			if( user_element->type!=XML_ELEMENT_NODE ) continue;
			
			if(!( g_str_equal(user_element->name, "user") && user_element->children )) continue;
			
			User *user=NULL;
			if(!(user=user_parse_node(service, user_element->children))) continue;
			if(!( user && user->id && user->user_name )) continue;
			
			debug("Added user: <%s@%s> to <%s>'s %s", user->user_name, service->uri, service->key, (which_pass ?_("who are following you") :_("you're following")));
			users=g_list_append(users, user);
			user=NULL;
		}
	}
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	if(next_cursor==-1) next_cursor++;
	if(previous_cursor==-1) previous_cursor++;
	
	if(!users){
		debug("No new users where found");
		return NULL;
	}
	
	return users;
}/*users_glist_parse(service_wrapper, xml);*/


/**
 *returns: -1, 0 or 1, if user1 is <, == or > than user2.
 */
int users_glists_sort_by_user_name(User *user1, User *user2){
	if(!( user2 && user2->user_name )) return 1;
	return strcasecmp( user1->user_name, user2->user_name );
}/*user_sort_by_user_name(l1->data, { l1=l1->next; l1->data; } );*/


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
			debug("%s %s %s %s %s, %s %s %s %s %s %s %s %s", _("friends"), _("of"), service->key, _("i.e"), _("who"), _("is"), _("following"), _("you"), _("and"), _("who"), _("is"), _("following"), service->key);
			if(service->friends_and_followers) uber_list_free(service->friends_and_followers);
			if(service->friends) users_glists_free(service, GetFriends);
			if(service->followers) users_glists_free(service, GetFollowers);
			return;
			
		case GetFriends:
			if(!service->friends) return;
			glist_free=service->friends;
			debug("%s %s %s %s, %s %s %s %s", _("friends"), _("of"), service->key, _("i.e"), _("who"), _("is"), _("following"), service->key);
			break;
			
		case GetFollowers:
			if(!service->followers) return;
			glist_free=service->followers;
			debug("%s %s %s %s", _("who"), _("is"), _("following"), service->key);
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
