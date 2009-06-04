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


#define _XOPEN_SOURCE
#include <time.h>
#include <strings.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <libsoup/soup.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "main.h"
#include "program.h"
#include "app.h"

#include "online-service.h"
#include "online-services.h"
#include "online-service-wrapper.h"
#include "network.h"

#include "users-glists.h"
#include "users.h"

#include "gconfig.h"
#include "preferences.h"
#include "gtkbuilder.h"
#include "cache.h"
#include "parser.h"
#include "label.h"

#include "tweet-view.h"
#include "friends-manager.h"
#include "following-viewer.h"
#include "profile-viewer.h"


static UserRequest *user_request_new(UserAction action, GtkWindow *parent, const gchar *user_data);
static void user_request_free(UserRequest *request);

static User *user_constructor(OnlineService *service, gboolean a_follower);
static User *user_parse_new(OnlineService *service, SoupMessage *xml);

static void user_status_format_dates(UserStatus *status);

static void user_append_friend(User *user);
static void user_remove_friend(User *user);

static void user_remove_follower(User *user);


#define	DEBUG_DOMAINS	"OnlineServices:Tweets:Requests:Users:Settings"
#include "debug.h"

#define GtkBuilderUI "user-profile.ui"

gchar *user_action_to_string(UserAction action){
	switch(action){
		case Confirmation: _("confirming action");
		case ViewTweets:
			return _("displaying tweets");
		case Follow:
			return _("started following:");
		case UnFollow:
			return _("unfollowed:");
		case Block:
			return _("blocked:");
		case UnBlock:
			return _("unblocked user");
		case Fave:
			return _("star'ing tweet");
		case UnFave:
			return _("un-star'ing tweet");
		case ViewProfile:
			return _("viewing user profile");
		case SelectService:
			return _("selecting default account");
		default:
			/*We never get here, but it makes gcc happy.*/
			return _("unsupported user action");
	}//switch
}/*user_action_to_string*/

static UserRequest *user_request_new(UserAction action, GtkWindow *parent, const gchar *user_data){
	if(action==SelectService || action==ViewProfile || action == Confirmation || G_STR_EMPTY(user_data)) return NULL;
	
	UserRequest *request=g_new(UserRequest, 1);
	
	request->parent=parent;
	request->user_data=g_strdup(user_data);
	request->action=action;
	request->method=QUEUE;
	request->message=g_strdup(user_action_to_string(action));
	
	switch(request->action){
		case ViewTweets:
			request->method=QUEUE;
			request->uri=g_strdup_printf(API_TIMELINE_USER, request->user_data);
			network_set_state_loading_timeline(request->uri, Load);
			break;
		case Follow:
			request->method=POST;
			request->uri=g_strdup_printf(API_USER_FOLLOW, request->user_data);
			break;
		case UnFollow:
			request->method=POST;
			request->uri=g_strdup_printf(API_USER_UNFOLLOW, request->user_data);
			break;
		case Block:
			request->method=POST;
			request->uri=g_strdup_printf(API_USER_BLOCK, request->user_data);
			break;
		case UnBlock:
			request->method=POST;
			request->uri=g_strdup_printf(API_USER_UNBLOCK, request->user_data);
			break;
		case Fave:
			request->method=POST;
			request->uri=g_strdup_printf(API_FAVE, request->user_data);
			break;
		case UnFave:
			request->method=POST;
			request->uri=g_strdup_printf(API_UNFAVE, request->user_data);
			break;
		case ViewProfile:
		case SelectService:
		case Confirmation:
		default:
			/*We never get here, but it makes gcc happy.*/
			g_free(request);
			return NULL;
	}//switch
	return request;
}/*user_request_new*/

void user_request_main(OnlineService *service, UserAction action, GtkWindow *parent, const gchar *user_data){
	if(action==SelectService || action==Confirmation || G_STR_EMPTY(user_data)) return;

	if(action==ViewProfile){
		view_profile(service, user_data, parent);
		return;
	}
	
	UserRequest *request=NULL;
	if(!(request=user_request_new(action, parent, user_data)))
		return;
	
	debug("Processing UserRequest to %s %s on %s", request->message, request->user_data, service->key);
	
	switch(request->method){
		case POST:
			online_service_request(service, POST, request->uri, user_request_main_quit, request, NULL);
			break;
		case GET:
		case QUEUE:
		default:
			online_service_request(service, QUEUE, request->uri, user_request_main_quit, request, NULL);
			break;
	}
	
	/*user_request_free(request);*/
	tweet_view_sexy_select();
}/*user_request_main*/

void user_request_main_quit(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceWrapper *service_wrapper=(OnlineServiceWrapper *)user_data;
	UserRequest *request=(UserRequest *)service_wrapper->user_data;
	
	if(!network_check_http(service_wrapper->service, msg)){
		debug("**ERORR:** UserRequest to %s %s.  OnlineService: '%s':\n\t\tServer response: %i", request->message, request->user_data, service_wrapper->service->decoded_key, msg->status_code);
		
		user_request_free(request);
		online_service_wrapper_free(service_wrapper);
		return;
	}
	
	User *user=NULL;
	OnlineServiceWrapper *request_wrapper=NULL;
	switch(request->action){
		case ViewTweets:
			request_wrapper=online_service_wrapper_new(service_wrapper->service, service_wrapper->requested_uri, network_display_timeline, request->uri, (gpointer)Tweets);
			network_display_timeline(session, msg, request_wrapper);
			break;
		case UnFollow:
		case Block:
		case Follow:
			/* parse new user */
			debug("UserRequest to %s %s.  OnlineService: '%s':", request->message, request->user_data, service_wrapper->service->decoded_key);
			if(!(user=user_parse_new(service_wrapper->service, msg))){
				debug("\t\t[failed]");
				app_statusbar_printf("Failed to %s %s on %s.", request->message, request->user_data, service_wrapper->service->decoded_key);
			}else{
				debug("\t\t[succeeded]");
				if(request->action==Follow)
					user_append_friend(user);
				else if(request->action==UnFollow)
					user_remove_friend(user);
				else if(request->action==Block){
					user_remove_friend(user);
					user_remove_follower(user);
				}
				app_statusbar_printf("Successfully %s %s on %s.", request->message, request->user_data, service_wrapper->service->decoded_key);
				user_free(user);
			}
			
			break;
		case Fave:
		case UnFave:
		case UnBlock:
			debug("UserRequest to %s %s.  OnlineService: '%s':\n\t\t[successed]", request->message, request->user_data, service_wrapper->service->decoded_key);
			app_statusbar_printf("Successfully %s on %s.", request->message, service_wrapper->service->decoded_key);
			break;
		case SelectService:
		case ViewProfile:
		case Confirmation:
		default:
			/*more cases of to make gcc happy.*/
			break;
	}//switch
	
	user_request_free(request);
	online_service_wrapper_free(service_wrapper);
}/*user_request_main_quit*/

static void user_request_free(UserRequest *request){
	return;
	request->parent=NULL;
	if(request->uri) g_free(request->uri);
	if(request->user_data) g_free(request->user_data);
	if(request->message) g_free(request->message);
	g_free(request);
	request=NULL;
}/*user_request_free*/


static User *user_constructor(OnlineService *service, gboolean a_follower){
	User *user=g_new(User, 1);
	user->follower=a_follower;
	user->service=service;
	user->status=NULL;
	user->user_name=user->nick_name=user->location=user->bio=user->url=user->image_url=user->image_filename=NULL;
	
	return user;
}/*user_constructor*/


/* Parse a xml user node. Ex: user's profile & add/del/block users responses */
static User *user_parse_new(OnlineService *service, SoupMessage *xml){
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
}/*user_parse_new(service, xml);*/


User *user_parse_profile(OnlineService *service, xmlNode *a_node){
	xmlNode		*current_node=NULL;
	gchar		*content=NULL;
	
	User		*user;
	
	user=user_constructor(service, getting_followers);
	
	debug("Parsing user profile data.");
	/* Begin 'users' node loop */
	for(current_node = a_node; current_node; current_node = current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE)
			continue;
		
		if( G_STR_EMPTY( (content=(gchar *)xmlNodeGetContent(current_node)) ) ) continue;
		
		debug("name: %s; content: %s.", current_node->name, content);
		
		if(g_str_equal(current_node->name, "id" ))
			user->id=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_node->name, "name" ))
			user->nick_name=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "screen_name" ))
			user->user_name=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "location" ))
			user->location=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "description" ))
			user->bio=g_markup_printf_escaped( "%s", content );
		
		else if(g_str_equal(current_node->name, "url" ))
			user->url=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "followers_count" ))
			user->followers=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_node->name, "friends_count" ))
			user->following=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_node->name, "statuses_count" ))
			user->tweets=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_node->name, "profile_image_url"))
			user->image_url=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "status") && current_node->children)
			user->status=user_status_new(service, current_node->children, Tweets);
		
		xmlFree(content);
		
	} /* End of loop */
	if(user->status)
		parser_format_user_status(service, user, user->status);
	
	user->image_filename=cache_images_get_filename(user);
	
	return user;
}/*user_parse_new(service, root_node); || user_parse_new(service, current_node->children);*/

UserStatus *user_status_new(OnlineService *service, xmlNode *status_node, StatusMonitor type){
	xmlNode		*current_node = NULL;
	gchar		*content=NULL;
	UserStatus	*status=g_new0(UserStatus, 1);
	
	status->service=service;
	status->user=NULL;
	status->type=type;
	status->text=status->tweet=status->notification=status->sexy_tweet=NULL;
	status->created_at_str=status->created_how_long_ago=NULL;
	status->id=status->in_reply_to_status_id=0;
	status->created_at=status->created_seconds_ago=0;
	
	/* Begin 'status' or 'direct-messages' loop */
	debug("Parsing status & tweet at node: %s", status_node->name);
	for(current_node=status_node; current_node; current_node=current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE) continue;
		
		if( G_STR_EMPTY( (content=(gchar *)xmlNodeGetContent(current_node)) ) ){
			if(content) g_free(content);
			continue;
		}
		
		if(g_str_equal(current_node->name, "id"))
			status->id=strtoul(content, NULL, 10);
		
		else if(g_str_equal(current_node->name, "in_reply_to_status_id"))
			status->in_reply_to_status_id=strtoul(content, NULL, 10);
		
		else if(g_str_equal(current_node->name, "source"))
			status->source=g_strdup(content);
		
		else if((g_str_equal(current_node->name, "sender") || g_str_equal(current_node->name, "user")) && current_node->children)
			status->user=user_parse_profile(service, current_node->children);
		
		else if(g_str_equal(current_node->name, "text"))
			status->text=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "created_at")){
			status->created_at_str=g_strdup(content);
			user_status_format_dates(status);
		}
		
		xmlFree(content);
		
	}
	
	if(status->user) parser_format_user_status(service, status->user, status);
	
	return status;
}/*user_status_new(service, current_node->children);*/

static void user_status_format_dates(UserStatus *status){
	struct tm	post;
	strptime(status->created_at_str, "%s", &post);
	post.tm_isdst=-1;
	status->created_at=mktime(&post);
	
	strptime(status->created_at_str, "%s", &post);
	post.tm_isdst=-1;
	status->created_at=mktime(&post);									
	
	debug("Parsing tweet's 'created_at' date: [%s] to Unix seconds since: %u", status->created_at_str, status->created_at);
	status->created_how_long_ago=parser_convert_time(status->created_at_str, &status->created_seconds_ago);
	debug("Display time set to: %s, %u.", status->created_how_long_ago, status->created_seconds_ago);
}/*user_status_format_dates*/

void user_status_free(UserStatus *status){
	if(!status) return;
	
	if(status->user) user_free(status->user);
	
	if(status->text) uber_free(status->text);
	if(status->tweet) uber_free(status->tweet);
	if(status->source) uber_free(status->source);
	
	if(status->sexy_tweet) uber_free(status->sexy_tweet);
	if(status->notification) uber_free(status->notification);
	
	if(status->created_at_str) uber_free(status->created_at_str);
	if(status->created_how_long_ago) uber_free(status->created_how_long_ago);
	
	uber_free(status);
	status=NULL;
}/*user_status_free*/

/* Get a user timeline */
User *user_fetch_profile(OnlineService *service, const gchar *user_name){
	if( !user_name || G_STR_EMPTY(user_name))
		return NULL;
			
	User *user=NULL;
	
	gchar *user_profile_uri=g_strdup_printf(API_ABOUT_USER, user_name);
	SoupMessage *msg=online_service_request(service, GET, user_profile_uri, NULL, NULL, NULL);
	g_free( user_profile_uri );
	
	if(!(user=user_parse_new(service, msg)))
		return NULL;
	
	return user;
}/*user_fetch_profile*/


/* Free a user struct */
void user_free(User *user){
	if(!user) return;
	
	if(user->status) user_status_free(user->status);
	
	if(user->user_name) uber_free(user->user_name);
	if(user->nick_name) uber_free(user->nick_name);
	
	if(user->location) uber_free(user->location);
	if(user->bio) uber_free(user->bio);
	if(user->url) uber_free(user->url);
	
	if(user->image_url) uber_free(user->image_url);
	if(user->image_filename) uber_free(user->image_filename);
	
	uber_free(user);
}/*user_free*/

static void user_append_friend(User *user){
	if(user->service->friends)
		user->service->friends=g_list_append(user->service->friends, user );
	app_set_statusbar_msg (_("Friend Added."));
}/*user_append_friend*/

static void user_remove_friend(User *user){
	if(user->service->friends)
		user->service->friends=g_list_remove(user->service->friends, user);
	app_set_statusbar_msg (_("Friend Removed."));
}/*user_remove_friend*/


static void user_remove_follower(User *user){
	if(user->service->followers)
		user->service->followers=g_list_remove(user->service->followers, user);
	app_set_statusbar_msg (_("Follower Removed."));
}/*user_remove_friend*/

