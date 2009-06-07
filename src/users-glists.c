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


/********************************************************
 *        Project headers, eg #include "config.h"       *
 ********************************************************/
#include "config.h"
#include "main.h"
#include "users-glists.h"

#include "app.h"
#include "users.h"
#include "online-service-wrapper.h"
#include "network.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
static OnlineService *service=NULL;
static UsersGListLoadFunc on_load_func=NULL;

static gboolean fetching_users=FALSE;
gboolean getting_followers=FALSE;

static guint page=0;
static guint which_pass=0;


#define API_FOLLOWING		"/statuses/friends.xml"
#define API_FOLLOWERS		"/statuses/followers.xml"


#define usrglistscasecmp	users_glists_sort_by_user_name
#define	usrglistscmp		users_glists_sort_by_user_name


#define	DEBUG_DOMAINS	"Networking:OnlineServices:Friends:Followers:XML:Parser:UsersGlists."
#include "debug.h"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static GList *users_glist_parse(OnlineService *service, SoupMessage *xml);

static int users_glists_sort_by_user_name(User *a, User *b);

static void users_glists_free(UsersGListGetWhich users_glist_get_which, OnlineService *service);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
GList *users_glist_get(UsersGListGetWhich users_glist_get_which, gboolean refresh, UsersGListLoadFunc func){
	if(!selected_service) return NULL;
	
	if(!fetching_users){
		page=0;
		if(!which_pass){
			on_load_func=func;
			if(refresh && service && service==selected_service)
				users_glists_free_lists(service);
			service=selected_service;
		}
		
		if( !refresh && service && service==selected_service ){
			GList *users=NULL;
			switch(users_glist_get_which){
				case GetFriends:
					if(!service->friends) break;
					debug("Displaying & loading, %d pages, friends list for: [%s].", page, service->decoded_key);
					service->friends=g_list_sort(service->friends, (GCompareFunc)usrglistscasecmp);
					users=service->friends;
					break;
				case GetFollowers:
					if(!service->followers) break;
					debug("Displaying & loading, %d pages, followers list for: [%s].", page, service->decoded_key);
					service->followers=g_list_sort(service->followers, (GCompareFunc)usrglistscasecmp);
					users=service->followers;
					break;
				case GetBoth:
					if(!(service->friends && service->followers)) break;
					debug("Displaying & loading, %d pages, friends and followers list for: [%s].", page, service->decoded_key);
					if(which_pass<2) break;
					selected_service->friends_and_followers=g_list_alloc();
					selected_service->friends_and_followers=g_list_concat(selected_service->followers, selected_service->friends);
					selected_service->friends_and_followers=g_list_sort(selected_service->friends_and_followers, (GCompareFunc)usrglistscasecmp);
					users=selected_service->friends_and_followers;
					break;
			}
			if(users){
				which_pass=0;
				if(on_load_func!=NULL)
					on_load_func(users);
				return users;
			}
			
		}
	}
	
	fetching_users=TRUE;
	page++;
	debug("Please wait while %s downloads %s page #%d.", GETTEXT_PACKAGE, ( (users_glist_get_which==GetFollowers||which_pass) ?"followers" :"friends"), page );
	app_statusbar_printf("Please wait while %s downloads page #%d of users %s.", GETTEXT_PACKAGE, page, ( (users_glist_get_which==GetFollowers||which_pass) ?"who're following you" :"you're following") );
	
	gchar *uri=NULL;
	if( (users_glist_get_which==GetFriends || (users_glist_get_which==GetBoth && !which_pass && !service->friends) ) ){
		getting_followers=FALSE;
		uri=g_strdup_printf("%s?page=%d", API_FOLLOWING, page);
	}else{
		which_pass=1;
		if(users_glist_get_which==GetBoth && service->friends && service->followers){
			which_pass=2;
			fetching_users=FALSE;
			return users_glist_get(users_glist_get_which, FALSE, NULL);
		}
		getting_followers=TRUE;
		uri=g_strdup_printf("%s?page=%d", API_FOLLOWERS, page);
	}
	
	debug("Getting users page, uri:  %s.", uri );
	online_service_request(service, QUEUE, uri, users_glist_save, (gpointer)users_glist_get_which, NULL );
	g_free(uri);
	return NULL;
}/*users_glist_get*/

void users_glist_save(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceWrapper *service_wrapper=(OnlineServiceWrapper *)user_data;
	
	UsersGListGetWhich users_glist_get_which=(UsersGListGetWhich)service_wrapper->user_data;
	OnlineService *service=service_wrapper->service;
	online_service_wrapper_free(service_wrapper);
	
	GList *new_users;
	debug("Users response: %i", msg->status_code);
	
	/* Check response */
	if(!network_check_http(service, msg)){
		debug("No more %s where downloaded so we're assuming we're finished.\n\t\t\tOnlineService: [%s] recieved %i from its server: [%s].", (which_pass ?"followers" :"friends"), service->decoded_key, msg->status_code, service->server);
		fetching_users=FALSE;
		which_pass++;
		users_glist_get(users_glist_get_which, FALSE, NULL);
		return;
	}
	
	/* parse user list */
	debug("Parsing user list");
	if(!(new_users=users_glist_parse(service, msg)) ){
		debug("No more %s where found, yippies we've got'em all.\n\t\t\tOnlineService: [%s] recieved %i from its server: [%s].", (which_pass ?"followers" :"friends"), service->decoded_key, msg->status_code, service->server);
		fetching_users=FALSE;
		which_pass++;
		users_glist_get(users_glist_get_which, FALSE, NULL);
		return;
	}
	
	if(!which_pass){
		debug("Appending users to friends list.");
		if(!service->friends)
			service->friends=new_users;
		else
			service->friends=g_list_concat(service->friends, new_users);
	}else{
		debug("Addending users to followers list.");
		if(!service->followers)
			service->followers=new_users;
		else
			service->followers=g_list_concat(service->followers, new_users);
	}
	/*now we get the next page - or send the results where they belong.*/
	service=NULL;
	debug("Retrieving the next list of friends.");
	users_glist_get(users_glist_get_which, FALSE, NULL);
}/*users_glist_save*/

/**
 *returns: 1 if a is different from b, -1 if b is different from a, 0 if they're the same
 */
static int users_glists_sort_by_user_name(User *a, User *b){
	return strcasecmp( (const char *)a->user_name, (const char *)b->user_name );
}/*user_sort_by_user_name(l1->data, l2->data);*/


/* Parse a user-list XML( friends, followers,... ) */
GList *users_glist_parse(OnlineService *service, SoupMessage *xml){
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	xmlNode		*current_node=NULL;
	
	GList		*list=NULL;
	User		*user=NULL;
	
	/* parse the xml */
	debug("Parsing users xml.");
	if(!( (doc=parser_parse(xml, &root_element)) && root_element )){
		xmlCleanupParser();
		return NULL;
	}
	
	debug("\t\t\tParsing new users. Starting with: '%s' node.", root_element->name);
	for(current_node=root_element; current_node; current_node=current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE)
			continue;
		
		if(g_str_equal(current_node->name, "user")){
			debug("\t\t\tCreating User * from current node.");
			user=user_parse_node(service, current_node->children);
			debug("\t\t\tAdding user: [%s] to user list.", user->user_name);
			list=g_list_append(list, user);
		}else if(g_str_equal(current_node->name, "users") && current_node->children){
			current_node=current_node->children;
		}
	}
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return list;
}


void users_glists_free_lists(OnlineService *service){
	if(service->friends_and_followers){
		users_glists_free(GetBoth, service);
		return;
	}
	
	users_glists_free(GetFriends, service);
	users_glists_free(GetFollowers, service);
}/*users_glists_free_lists(service);*/

static void users_glists_free(UsersGListGetWhich users_glist_get_which, OnlineService *service){
	GList *glist_free=NULL;
	switch(users_glist_get_which){
		case GetBoth:
			if(!service->friends_and_followers) return;
			glist_free=service->friends_and_followers;
			break;
		case GetFriends:
			if(!service->friends) return;
			glist_free=service->friends;
			break;
		case GetFollowers:
			if(!service->followers) return;
			glist_free=service->followers;
			break;
	}
	if(!glist_free) return;
	
	if(IF_DEBUG){
		gchar *type=NULL;
		switch(users_glist_get_which){
			case GetBoth:
				type=g_strdup_printf("%s, %s %s", _("friends, ie you're following"), _("and"), _("who's following you"));
				break;
				
			case GetFriends:
				type=g_strdup(_("friends, ie you're following"));
				break;
				
			case GetFollowers:
				type=g_strdup(_("who's following you"));
				break;
		}
		debug("Freeing the authenticated user's %s.", type );
		uber_free(type);
	}
	
	g_list_foreach(glist_free, (GFunc)user_free, NULL);
	
	uber_list_free(glist_free);
	if(users_glist_get_which!=GetBoth) return;
	
	uber_list_free(service->friends);
	uber_list_free(service->followers);
}/*users_glists_free(GetBoth, service);*/



/********************************************************
 *                       eof                            *
 ********************************************************/
