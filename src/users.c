/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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


#include <strings.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <libsoup/soup.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "main.h"
#include "debug.h"
#include "app.h"
#include "users.h"

#include "gtkbuilder.h"
#include "network.h"
#include "images.h"
#include "parser.h"
#include "label.h"
#include "tweet-view.h"

#include "friends-manager.h"
#include "following-viewer.h"
#include "profile-viewer.h"


static FriendRequest *user_request_new(OnlineService *service, FriendAction action, GtkWindow *parent, const gchar *user_data);
static void user_request_process_get(FriendRequest *request);
static void user_request_process_post(SoupSession *session, SoupMessage *msg, gpointer user_data);
static void user_request_free(FriendRequest *request);

static User *user_constructor( gboolean a_follower );


#define	DEBUG_DOMAINS	"ServiceRequests"
#define GtkBuilderUI "user-profile.ui"

static GList *user_friends=NULL, *user_followers=NULL, *following_and_followers=NULL;

static FriendRequest *user_request_new(OnlineService *service, FriendAction action, GtkWindow *parent, const gchar *user_data){
	if(G_STR_EMPTY(user_data))
		return NULL;
	
	FriendRequest *request=g_new(FriendRequest, 1);
	request->parent=parent;
	request->user_data=g_strdup(user_data);
	request->action=action;
	request->method=POST;
	switch(request->action){
		case ViewProfile:
			request->message=g_strdup("viewing profile");
			request->uri=g_strdup_printf(API_ABOUT_USER, request->user_data);
			request->method=GET;
			break;
		case ViewTweets:
			request->message=g_strdup("tweets displaying");
			request->uri=g_strdup_printf(API_TIMELINE_USER, request->user_data);
			request->method=GET;
			break;
		case Follow:
			request->uri=g_strdup_printf(API_USER_FOLLOW, request->user_data);
			request->message=g_strdup("following");
			break;
		case UnFollow:
			request->uri=g_strdup_printf(API_USER_UNFOLLOW, request->user_data);
			request->message=g_strdup("unfollowed");
			break;
		case Block:
			request->uri=g_strdup_printf(API_USER_BLOCK, request->user_data);
			request->message=g_strdup("blocked");
			break;
		case UnBlock:
			request->uri=g_strdup_printf(API_USER_UNBLOCK, request->user_data);
			request->message=g_strdup("unblocked");
			break;
		case Fave:
			request->uri=g_strdup_printf(API_FAVE, request->user_data);
			request->message=g_strdup("star'd");
			break;
		case UnFave:
			request->uri=g_strdup_printf(API_UNFAVE, request->user_data);
			request->message=g_strdup("deleted");
			break;
		case SelectService:
		default:
			g_free(request);
			return NULL;
	}//switch
	request->service=service;
	debug(DEBUG_DOMAINS, "%sing %s on %s", request->message, request->user_data, request->service->key);
	return request;
}//user_request_new

void user_request_main(OnlineService *service, FriendAction action, GtkWindow *parent, const gchar *user_data){
	FriendRequest *request=NULL;
	if(!(request=user_request_new(service, action, parent, user_data)))
		return;
	
	if(request->method == POST){
		online_service_request(request->service, POST, request->uri, user_request_main_quit, request, NULL);
		return;
	}
	
	user_request_main_quit(NULL, NULL, (gpointer *)request);
	tweet_view_sexy_select();
}//user_request_main

void user_request_main_quit(SoupSession *session, SoupMessage *msg, gpointer user_data){
	FriendRequest *request=(FriendRequest *)user_data;
	switch(request->method){
		case POST:
			user_request_process_post(session, msg, request);
			break;
		case GET:
			user_request_process_get(request);
			break;
		case QUEUE:
			break;
	}//switch
}//user_request_main_quit

static void user_request_process_get(FriendRequest *request){
	switch(request->action){
		case ViewTweets:
			return network_get_user_timeline(request->service, request->user_data);
		case ViewProfile:
			return view_profile(request->service, request->user_data, request->parent);
		case SelectService:
		case UnFollow:
		case Block:
		case Follow:
		case Fave:
		case UnFave:
		case UnBlock:
			break;
	}//switch
	user_request_free(request);
}//user_request_process_get

static void user_request_process_post(SoupSession *session, SoupMessage *msg, gpointer user_data){
	FriendRequest *request=(FriendRequest *)user_data;
	debug(DEBUG_DOMAINS, "%s user response: %i", request->message, msg->status_code);
	
	/* Check response */
	if(!network_check_http(request->service, msg)){
		app_statusbar_printf("Failed %s.", request->message, NULL);
		user_request_free(request);
		return;
	}
	
	/* parse new user */
	debug(DEBUG_DOMAINS, "Parsing user response");
	User *user=user_parse_new(request->service, msg);
	app_statusbar_printf("Successfully %s.", request->message, NULL);
	
	switch(request->action){
		case UnFollow:
		case Block:
			if(user) user_remove_friend(user);
			break;
		case Follow:
			if(user) user_append_friend(user);
			break;
		case Fave:
		case UnFave:
		case UnBlock:
		case ViewTweets:
		case ViewProfile:
		case SelectService:
			break;
	}//switch
	if(user) user_free(user);
	user_request_free(request);
}//user_request_process_post


static void user_request_free(FriendRequest *request){
	return;
	request->parent=NULL;
	if(request->uri) g_free(request->uri);
	if(request->user_data) g_free(request->user_data);
	if(request->message) g_free(request->message);
	request->service=NULL;
	g_free(request);
	request=NULL;
}//user_request_free


static User *user_constructor( gboolean a_follower ){
	User *user=g_new(User, 1);
	
	user->follower=a_follower;
		
	user->user_name=user->nick_name=user->location=user->bio=user->url=user->image_url=user->image_filename=NULL;
	
	return user;
}//user_constructor


/* Parse a xml user node. Ex: user's profile & add/del/block users responses */
User *user_parse_new(OnlineService *service, SoupMessage *xml){
	xmlDoc *doc=NULL;
	xmlNode *root_element=NULL;
	User *user=NULL;
	
	/* parse the xml */
	if(!( (doc=parser_parse(xml, &root_element )) )){
		xmlCleanupParser();
		return NULL;
	}
	
	if(g_str_equal(root_element->name, "user"))
		user=user_parse_profile(service, root_element->children);
	
	/* Free memory */
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return user;
}


User *user_parse_profile(OnlineService *service, xmlNode *a_node){
	xmlNode		*cur_node=NULL;
	gchar		*content=NULL;
	
	User		*user;
	
	user=user_constructor( getting_followers );
	
	debug(DEBUG_DOMAINS, "Parsing user profile data.");
	/* Begin 'users' node loop */
	for(cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if(cur_node->type != XML_ELEMENT_NODE)
			continue;
		
		if( G_STR_EMPTY( (content=(gchar *)xmlNodeGetContent(cur_node)) ) ) continue;
		
		debug(DEBUG_DOMAINS, "name: %s; content: %s.", cur_node->name, content);
		
		if(g_str_equal(cur_node->name, "id" ))
			user->id=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(cur_node->name, "name" ))
			user->nick_name=g_strdup(content);
		
		else if(g_str_equal(cur_node->name, "screen_name" ))
			user->user_name=g_strdup(content);
		
		else if(g_str_equal(cur_node->name, "location" ))
			user->location=g_strdup(content);
		
		else if(g_str_equal(cur_node->name, "description" ))
			user->bio=g_markup_printf_escaped( "%s", content );
		
		else if(g_str_equal(cur_node->name, "url" ))
			user->url=g_strdup(content);
		
		else if(g_str_equal(cur_node->name, "followers_count" ))
			user->followers=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(cur_node->name, "friends_count" ))
			user->following=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(cur_node->name, "statuses_count" ))
			user->tweets=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(cur_node->name, "profile_image_url"))
			user->image_filename=images_get_filename( (user->image_url=g_strdup(content)) );
		
		xmlFree(content);
		
	} /* End of loop */
	
	return user;
}


int user_sort_by_user_name(User *a, User *b){
	return strcasecmp( (const char *)a->user_name, (const char *)b->user_name );
}


/* Parse a user-list XML( friends, followers,... ) */
GList *users_new(OnlineService *service, SoupMessage *xml){
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	xmlNode		*cur_node=NULL;
	
	GList		*list=NULL;
	User		*user=NULL;
	
	/* parse the xml */
	if(!( (doc=parser_parse(xml, &root_element)) )){
		xmlCleanupParser();
		return NULL;
	}
	
	/* get users */
	for(cur_node = root_element; cur_node; cur_node = cur_node->next) {
		if(cur_node->type != XML_ELEMENT_NODE)
			continue;
		if(g_str_equal(cur_node->name, "user")){
			/* parse user */
			user=user_parse_profile(service, cur_node->children);
			/* add to list */
			list=g_list_append(list, user);
		} else if(g_str_equal(cur_node->name, "users")){
			cur_node = cur_node->children;
		}
	} /* End of loop */
	
	/* Free memory */
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return list;
}

/* Get a user timeline */
User *user_fetch_profile(OnlineService *service, const gchar *user_name){
	if( !user_name || G_STR_EMPTY(user_name))
		return NULL;
			
	User *user=NULL;
	SoupMessage *msg=NULL;
	
	gchar *user_profile=g_strdup_printf( API_ABOUT_USER, user_name );
	msg=online_service_request( service, GET, user_profile, NULL, NULL, NULL );
	g_free( user_profile );
	
	if((user=user_parse_new(service, msg)) )
		return user;
	
	return NULL;
}//user_fetch_profile



void users_free(const char *type, GList *users ){
	debug( DEBUG_DOMAINS, "Freeing the authenticated user's %s.", type );
	
	g_list_foreach(users, (GFunc)user_free, NULL);
	
	g_list_free(users);
	users=NULL;
}//users_free

/* Free a user struct */
void user_free(User *user){
	if(!user) return;
	if(!(G_STR_EMPTY(user->user_name))) g_free(user->user_name);
	if(!(G_STR_EMPTY(user->nick_name))) g_free(user->nick_name);
	if(!(G_STR_EMPTY(user->location))) g_free(user->location);
	if(!(G_STR_EMPTY(user->bio))) g_free(user->bio);
	if(!(G_STR_EMPTY(user->url))) g_free(user->url);
	if(!(G_STR_EMPTY(user->image_url))) g_free(user->image_url);
	if(!(G_STR_EMPTY(user->image_filename))) g_free(user->image_filename);
	user->service=NULL;
	g_free(user);
	user=NULL;
}//user_free

/* Free a list of Users */
void user_free_lists(void){
	if(following_and_followers)
		users_free("friends, ie who the user is following, and the authenticated user's followers", following_and_followers);
	
	if(user_friends)
		users_free("friends, ie who they're following", user_friends);
	
	if(user_followers)
		users_free("followers", user_followers);
}//user_free_lists


void user_append_friend(User *user){
	if(user_friends)
		user_friends=g_list_append(user_friends, user );
	app_set_statusbar_msg (_("Friend Added."));
}//user_append_friend

void user_remove_friend(User *user){
	if(user_friends)
		user_friends=g_list_remove(user_friends, user);
	app_set_statusbar_msg (_("Friend Removed."));
	user_free(user);
}//user_remove_friend


void user_append_follower(User *user){
	if(user_followers)
		user_followers=g_list_append(user_followers, user );
	app_set_statusbar_msg (_("Follower Added."));
}//user_append_friend

void user_remove_follower(User *user){
	if(user_followers)
		user_followers=g_list_remove(user_followers, user);
	app_set_statusbar_msg (_("Follower Removed."));
	user_free(user);
}//user_remove_friend


/* Get authenticating user's friends(following)
 * Returns:
 * 		NULL: Friends will be fetched
 * 		GList: The list of friends (fetched previously)
 */
GList *user_get_friends(gboolean refresh){
	if(user_friends && !refresh)
		return user_friends;
	return user_friends=network_get_users_glist(TRUE);
}


/* Get the authenticating user's followers
 * Returns:
 * 		NULL: Followers will be fetched
 * 		GList: The list of friends (fetched previously)
 */
GList *user_get_followers(gboolean refresh){
	if(user_followers && !refresh)
		return user_followers;
	return user_followers=network_get_users_glist(FALSE);
}

GList *user_get_friends_and_followers(gboolean refresh){
	if(following_and_followers && !refresh)
		return following_and_followers;
	
	if( refresh || !user_friends)
		user_friends=network_get_users_glist( TRUE );
	if( refresh || !user_followers)
		user_followers=network_get_users_glist( FALSE );
	
	following_and_followers=g_list_alloc();
	following_and_followers=g_list_concat( user_followers, user_friends );
	following_and_followers=g_list_sort(following_and_followers, (GCompareFunc) usrcasecmp);
	return following_and_followers;
}//user_get_friends_and_followers

