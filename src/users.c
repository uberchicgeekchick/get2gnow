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
/********************************************************************************
 *                      My art, code, & programming.                            *
 ********************************************************************************/
#define _GNU_SOURCE
#define _THREAD_SAFE



/********************************************************************************
 * project, object, system & library headers, eg #include <gdk/gdkkeysyms.h>    *
 ********************************************************************************/
#include <time.h>
#include <strings.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <libsoup/soup.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libnotify/notify.h>

#include "config.h"
#include "program.h"
#include "datetime.h"

#include "main-window.h"

#include "online-services.typedefs.h"
#include "online-service.types.h"
#include "online-service.h"
#include "online-services.h"
#include "online-service-wrapper.h"
#include "network.h"

#include "users.types.h"
#include "users-glists.h"
#include "users.h"
#include "cache.h"

#include "gconfig.h"
#include "preferences.defines.h"
#include "gtkbuilder.h"
#include "cache.h"

#include "www.h"
#include "xml.h"

#include "ui-utils.h"
#include "update-viewer.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"Users:Updates:Requests:Dates:Times:OnlineServices:Settings:Profiles:users.c"
#include "debug.h"


/********************************************************************************
 *                    prototypes for private method & function                  *
 ********************************************************************************/
static User *user_new(OnlineService *service, gboolean a_follower);
static void user_validate(User **user);


static UserStatus *user_status_new(OnlineService *service, UpdateType update_type);
static void user_status_validate(UserStatus **status);
static UserStatus *user_status_parse_retweeted_status(OnlineService *service, xmlNode *root_element, UpdateType update_type);
static void user_status_format_retweeted_status(OnlineService *service, UserStatus *retweeted_status, UserStatus **status, User *user);
static void user_status_format_updates(OnlineService *service, UserStatus *status, User *user);


/********************************************************************************
 *                  'Here be Dragons'...art, beauty, fun, & magic.              *
 ********************************************************************************/


/********************************************************************************
 *                                User methods                                  *
 ********************************************************************************/
static User *user_new(OnlineService *service, gboolean a_follower){
	User *user=g_new0(User, 1);
	
	user->follower=a_follower;
	user->service=service;
	user->status=NULL;
	user->profile_background_tile=FALSE;
	user->id_str=user->user_name=user->nick_name=user->location=user->bio=NULL;
	user->uri=NULL;
	user->image_uri=user->image_file=NULL;
	user->profile_background_uri=user->profile_background_file=NULL;
	user->profile_background_color=user->profile_text_color=user->profile_link_color=user->profile_sidebar_fill_color=user->profile_sidebar_border_color=NULL;
	
	return user;
}/*user_new*/

static void user_validate( User **user ){
	if(! G_STR_N_EMPTY( (*user)->user_name ) ) (*user)->user_name=g_strdup("");
	if(! G_STR_N_EMPTY( (*user)->nick_name ) )
		if(!( G_STR_EMPTY( (*user)->user_name ) )) (*user)->nick_name=g_strdup( (*user)->user_name );
		else (*user)->nick_name=g_strdup("");
	if(! G_STR_N_EMPTY( (*user)->location ) ) (*user)->location=g_strdup("");
	if(! G_STR_N_EMPTY( (*user)->bio ) ) (*user)->bio=g_strdup("unavailable");
	if(! G_STR_N_EMPTY( (*user)->uri ) )
		if(G_STR_N_EMPTY( (*user)->user_name ) && (*user)->service && (*user)->service->key )
			(*user)->uri=g_strdup_printf("http://%s/%s", (*user)->service->uri, (*user)->user_name );
		else
			(*user)->uri=g_strdup("");
	
	if(! G_STR_N_EMPTY( (*user)->image_uri ) ) (*user)->image_uri=g_strdup("");
	if(! G_STR_N_EMPTY( (*user)->image_file ) ) (*user)->image_file=g_strdup("");
	
	if(! G_STR_N_EMPTY( (*user)->profile_background_uri ) ) (*user)->profile_background_uri=g_strdup("");
	if(! G_STR_N_EMPTY( (*user)->profile_background_file ) ) (*user)->profile_background_file=g_strdup("");
	
	if(! G_STR_N_EMPTY( (*user)->profile_background_color ) ) (*user)->profile_background_color=g_strdup("#F0F2F5");
	if(! G_STR_N_EMPTY( (*user)->profile_text_color ) ) (*user)->profile_text_color=g_strdup("#000000");
	if(! G_STR_N_EMPTY( (*user)->profile_link_color ) ) (*user)->profile_link_color=g_strdup("#002FA7");
	if(! G_STR_N_EMPTY( (*user)->profile_sidebar_fill_color ) ) (*user)->profile_sidebar_fill_color=g_strdup("#FFFFFF");
	if(! G_STR_N_EMPTY( (*user)->profile_sidebar_border_color ) ) (*user)->profile_sidebar_border_color=g_strdup("#CEE1E9");
	
}/*user_validate(&user);*/ 

User *user_parse_profile(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	xmlDoc *doc=NULL;
	xmlNode *root_element=NULL;
	User *user=NULL;
	
	if(!( (doc=xml_create_xml_doc_and_get_root_element_from_soup_message(xml, &root_element )) )){
		xmlCleanupParser();
		return NULL;
	}
	
	if(g_str_equal(root_element->name, "user")){
		user=user_parse_node(service, root_element->children);
		if(!user->status)
			user->status=user_status_new(service, Timelines);
		user_status_validate(&user->status);
		
	}
		/*TODO finish caching
		if((user=user_parse_node(service, root_element->children)))
			cache_save_page(service, online_service_wrapper_get_requested_uri(service_wrapper), xml->response_body);
		*/
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return user;
}/*user_parse_profile(service, xml, service_wrapper);*/

User *user_parse_node(OnlineService *service, xmlNode *root_element){
	xmlNode		*current_element=NULL;
	gchar		*content=NULL;
	
	User		*user=user_new(service, getting_followers);
	
	debug("Parsing user profile data.");
	/* Begin 'users' node loop */
	debug("Parsing user beginning at node: %s", root_element->name);
	for(current_element=root_element; current_element; current_element=current_element->next) {
		if(current_element->type != XML_ELEMENT_NODE && current_element->type != XML_ATTRIBUTE_NODE )
			continue;
		
		if(!(G_STR_N_EMPTY( (content=(gchar *)xmlNodeGetContent(current_element)) ) )) continue;
		
		if(g_str_equal(current_element->name, "id" )){
			user->id=g_ascii_strtod(content, NULL);
			user->id_str=gdouble_to_str(user->id);
			debug("User ID: %s(=%f).", user->id_str, user->id);
			
		}else if(g_str_equal(current_element->name, "name" ))
			user->nick_name=g_markup_printf_escaped("%s", content);
		
		else if(g_str_equal(current_element->name, "screen_name" ))
			user->user_name=g_markup_printf_escaped("%s", content);
		
		else if(g_str_equal(current_element->name, "location" ))
			user->location=g_markup_printf_escaped("%s", content);
		
		else if(g_str_equal(current_element->name, "description" )){
			if(G_STR_N_EMPTY(content))
				user->bio=g_markup_printf_escaped("%s", content);
			else
				user->bio=g_strdup("unavailable");
		}else if(g_str_equal(current_element->name, "url" ))
			user->uri=g_strdup(content);
		
		else if(g_str_equal(current_element->name, "followers_count" ))
			user->followers=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_element->name, "friends_count" ))
			user->following=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_element->name, "statuses_count" ))
			user->updates=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_element->name, "profile_image_url"))
			user->image_uri=g_strdup(content);
		
		else if(g_str_equal(current_element->name, "profile_background_image_url"))
			user->profile_background_uri=g_strdup(content);
		
		else if(g_str_equal(current_element->name, "profile_background_tile")){
			if(!strcasecmp(content, "true"))
				user->profile_background_tile=TRUE;
		
		}else if(g_str_equal(current_element->name, "profile_background_color"))
			user->profile_background_color=g_strdup_printf("#%s", content);

		else if(g_str_equal(current_element->name, "profile_text_color"))
			user->profile_text_color=g_strdup_printf("#%s", content);

		else if(g_str_equal(current_element->name, "profile_link_color"))
			user->profile_link_color=g_strdup_printf("#%s", content);

		else if(g_str_equal(current_element->name, "profile_sidebar_fill_color"))
			user->profile_sidebar_fill_color=g_strdup_printf("#%s", content);

		else if(g_str_equal(current_element->name, "profile_sidebar_border_color"))
			user->profile_sidebar_border_color=g_strdup_printf("#%s", content);

		else if( g_str_equal(current_element->name, "status") && current_element->children)
			user->status=user_status_parse(service, current_element->children, Users);
		
		else if( g_str_equal(current_element->name, "retweeted_status") && user->status && current_element->children){
			user->status->retweeted_status=user_status_parse_retweeted_status(service, current_element->children, Users);
		}
		
		uber_free(content);
		
	}
	
	user_validate(&user);
	
	if(user && user->status && user->status->retweeted_status)
		user_status_format_retweeted_status(service, user->status->retweeted_status, &user->status, user);
	
	if(user->status){
		user_status_validate( &user->status );
		user_status_format_updates(service, user->status, user);
	}
	
	user->image_file=cache_images_get_user_avatar_filename(service, user->user_name, user->image_uri);
	user->profile_background_file=cache_images_get_user_profile_background_filename(service, user->user_name, user->profile_background_uri);
	
	return user;
}/*user_parse_node(service, root_node); || user_parse_node(service, current_element->children);*/

void user_free(User *user){
	if(!( user && user->id && user->service )) return;
	user_validate(&user);
	if(user->status) user_status_free(user->status);
	
	user->service=NULL;
	
	uber_object_free(&user->user_name, &user->nick_name, &user->location, &user->bio, &user->uri, &user->image_uri, &user->image_file, &user->profile_background_uri, &user->profile_background_file,&user->profile_background_color, &user->profile_text_color, &user->profile_link_color, &user->profile_sidebar_fill_color, &user->profile_sidebar_border_color, &user, NULL);
}/*user_free*/


/********************************************************************************
 *                           UserStatus methods                                 *
 ********************************************************************************/
static UserStatus *user_status_new(OnlineService *service, UpdateType update_type){
	UserStatus	*status=g_new0(UserStatus, 1);
	
	status->service=service;
	status->retweet=FALSE;
	status->user=NULL;
	status->retweeted_status=NULL;
	status->type=update_type;
	status->id_str=status->text=status->sexy_status_text=status->update=status->notification=status->sexy_update=status->created_at_str=status->created_how_long_ago=status->retweeted_by=status->retweeted_user_name=NULL;
	status->id=status->in_reply_to_status_id=0.0;
	status->created_at=0;
	status->created_seconds_ago=0;
	
	return status;
}/*user_status_new(service, Users|Replies|Dms);*/

static void user_status_validate( UserStatus **status ){
	if(! (*status)->text ) (*status)->text=g_strdup("");
	if(! (*status)->id_str ) (*status)->id_str=g_strdup("");
	if(! (*status)->from ) (*status)->from=g_strdup("");
	if(! (*status)->rcpt ) (*status)->rcpt=g_strdup("");
	if(! (*status)->update ) (*status)->update=g_strdup("");
	if(! (*status)->sexy_status_text ) (*status)->sexy_status_text=g_strdup("");
	if(! (*status)->source ) (*status)->source=g_strdup("");
	if(! (*status)->sexy_update ) (*status)->sexy_update=g_strdup("");
	if(! (*status)->notification ) (*status)->notification=g_strdup("");
	if(! (*status)->created_at_str ) (*status)->created_at_str=g_strdup("");
	if(! (*status)->created_how_long_ago ) (*status)->created_how_long_ago=g_strdup("");
	if(! (*status)->retweeted_by ) (*status)->retweeted_by=g_strdup("");
	if(! (*status)->retweeted_user_name ) (*status)->retweeted_user_name=g_strdup("");
}/*user_status_validate(&status);*/ 

UserStatus *user_status_parse_from_search_result_atom_entry(OnlineService *service, xmlNode *root_element, UpdateType update_type){
	xmlNode		*current_element=NULL, *current_elements_attributes=NULL;
	gchar		*content=NULL;
	UserStatus	*status=user_status_new(service, Searches);
	status->user=user_new(service, getting_followers);
	
	/* Begin 'status' or 'direct-messages' loop */
	debug("Parsing searches results beginning at node: %s", root_element->name);
	for(current_element=root_element; current_element; current_element=current_element->next) {
		if(current_element->type != XML_ELEMENT_NODE && current_element->type != XML_ATTRIBUTE_NODE ) continue;
		
		if( !g_str_equal(current_element->name, "link") && G_STR_EMPTY( (content=(gchar *)xmlNodeGetContent(current_element)) ) ){
			if(content) uber_free(content);
			continue;
		}
		
		debug("Parsing searches beginning at node: %s.", current_element->name );
		if(g_str_equal(current_element->name, "id")){
			gchar *status_id=NULL;
			status->id=g_ascii_strtod((status_id=g_strrstr(content, ":")+1), NULL);
			status->id_str=g_strdup(status_id);
			debug("Parsing searches Update ID: %s(=%f).", status->id_str, status->id);
		}else if(g_str_equal(current_element->name, "published") || g_str_equal(current_element->name, "updated")){
			if( status->created_at_str )
				if(!g_str_equal(status->created_at_str, content)){
					uber_free(status->created_at_str);
					uber_free(status->created_how_long_ago);
					status->created_at=0;
				}else{
					uber_free(content);
					continue;
				}
			
			status->created_at_str=g_strdup(content);
			datetime_strp_ages(status->created_at_str, &status->created_at, &status->created_how_long_ago, &status->created_seconds_ago, TRUE);
		}else if(g_str_equal(current_element->name, "title")){
			status->text=g_strdup(content);
			debug("Parsing searches update: %s; from: %s.", status->text, current_element->name );
		}else if(g_str_equal(current_element->name, "link")){
			if(content) uber_free(content);
			xmlAttr *elements_attributes;
			debug("Parsing searches link node searching for type=\"image/png\".");
			for( elements_attributes=current_element->properties; elements_attributes; elements_attributes=elements_attributes->next ){
				debug("Parsing searches at node: %s looking for type.", elements_attributes->name );
				if(g_str_equal(elements_attributes->name, "type")) break;
			}
			if(!elements_attributes) continue;
			
			for(current_elements_attributes=elements_attributes->children; current_elements_attributes; current_elements_attributes=current_elements_attributes->next){
				content=(gchar *)xmlNodeGetContent(current_elements_attributes);
				debug("Parsing searches at node: %s looking for image/png.", content );
				if(g_str_equal(content, "image/png")) break;
				uber_free(content);
			}
			if(content) uber_free(content);
			if(!current_elements_attributes) continue;
			
			debug("Parsed searches type and image/png nodes.  Now searching href node.");
			for( elements_attributes=elements_attributes->next; elements_attributes; elements_attributes=elements_attributes->next ){
				debug("Parsing searches node %s looking for href.", elements_attributes->name );
				if(g_str_equal(elements_attributes->name, "href") ) break;
			}
			if(!elements_attributes) continue;
			
			debug("Parsed searches href nodes.  Setting avatar's uri.");
			status->user->image_uri=(gchar *)xmlNodeGetContent(elements_attributes->children);
			debug("Parsing searches nodes finished.  Setting user->image: %s.", status->user->image_uri );
			
			elements_attributes=NULL;
			current_elements_attributes=NULL;
			continue;
		}else if(g_str_equal(current_element->name, "twitter:source")){
			status->source=g_strdup(content);
		}else if(g_str_equal(current_element->name, "author")){
			if(content) uber_free(content);
			debug("Parsing searches user_name & user_nick from: %s.", current_element->name );
			for( current_elements_attributes=current_element->children; current_elements_attributes; current_elements_attributes=current_elements_attributes->next ){
				debug("Parsing searches for user_name & user_nick: %s.", current_elements_attributes->name );
				if(g_str_equal(current_elements_attributes->name, "name") ) break;
			}
			if(content) uber_free(content);
			if(!current_elements_attributes) continue;
			if(!(G_STR_N_EMPTY( (content=(gchar *)xmlNodeGetContent(current_elements_attributes)) ) )){
				if(content) uber_free(content);
				current_elements_attributes=NULL;
				continue;
			}
			
			debug("Parsing searches user_name & user_nick from content: %s.", content);
			const char *nick_name=NULL;
			if(!(nick_name=g_strstr_len(content, -1, "(")))
				status->user->user_name=g_markup_printf_escaped("%s", content);
			else{
				gchar **user_data=g_strsplit(content, " (", -1);
				status->user->user_name=g_markup_printf_escaped("%s", user_data[0]);
				if(G_STR_N_EMPTY(user_data[1])){
					gchar **user_nick_data=g_strsplit(user_data[1], ")", -1);
					if( user_nick_data[0] && G_STR_N_EMPTY(user_nick_data[0]) )
						status->user->nick_name=g_markup_printf_escaped("%s", user_nick_data[0]);
					/*else
						status->user->nick_name=g_markup_printf_escaped("%s", user_data[1]);*/
					g_strfreev(user_nick_data);
				}
				g_strfreev(user_data);
			}
			debug("Parsed searches user_name: %s; user_nick: %s.", status->user->user_name, status->user->nick_name);
			current_elements_attributes=NULL;
		}
		if(content) uber_free(content);
	}
	
	status->user->image_file=cache_images_get_user_avatar_filename(service, status->user->user_name, status->user->image_uri);
	status->user->profile_background_file=cache_images_get_user_profile_background_filename(service, status->user->user_name, status->user->profile_background_uri);
	
	user_validate(&status->user);
	user_status_validate(&status);
	
	user_status_format_updates(service, status, status->user);
	
	return status;
}/*user_status_parse_from_search_result_atom_entry(service, current_element->children, update_type);*/

static UserStatus *user_status_parse_retweeted_status(OnlineService *service, xmlNode *root_element, UpdateType update_type){
	debug("Parsing retweeted update beginning at node: %s", root_element->name);
	UserStatus *status=user_status_parse(service, root_element, update_type);
	status->retweet=TRUE;
	return status;
}/*user_status_parse_retweeted_status(service, current_element->children, Homepage);*/

UserStatus *user_status_parse_new(OnlineService *service, SoupMessage *xml, const gchar *uri){
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	xmlNode		*current_element=NULL;
	UserStatus 	*status=NULL;
	
	if(!(doc=xml_create_xml_doc_and_get_root_element_from_soup_message(xml, &root_element))){
		debug("Failed to parse xml document, from <%s/%s>.", service->key, uri);
		xmlCleanupParser();
		return NULL;
	}
	
	/* get updates or direct messages */
	debug("Parsing %s.", root_element->name);
	for(current_element=root_element; current_element; current_element=current_element->next) {
		if(current_element->type != XML_ELEMENT_NODE ) continue;
		
		if(!( g_str_equal(current_element->name, "status") ))
			continue;
		
		if(!current_element->children){
			debug("*WARNING:* Cannot parse %s. Its missing children nodes.", current_element->name);
			continue;
		}
		
		debug("Parsing %s.", (g_str_equal(current_element->name, "status") ?"status update" :"direct message" ) );
		
		debug("Creating Status *.");
		if(!( (( status=user_status_parse(service, current_element->children, Archive))) && status->id )){
			if(status) user_status_free(status);
			break;
		}
		break;
	}
	return status;
}/*user_status_parse_new(service, xml);*/

UserStatus *user_status_parse(OnlineService *service, xmlNode *root_element, UpdateType update_type){
	xmlNode		*current_element=NULL;
	gchar		*content=NULL;
	UserStatus	*status=user_status_new(service, update_type);
	
	/* Begin 'status' or 'direct-messages' loop */
	debug("Parsing update at beginning node: %s", root_element->name);
	for(current_element=root_element; current_element; current_element=current_element->next) {
		if(current_element->type != XML_ELEMENT_NODE && current_element->type != XML_ATTRIBUTE_NODE ) continue;
		
		if(!(G_STR_N_EMPTY( (content=(gchar *)xmlNodeGetContent(current_element)) ) )){
			if(content) uber_free(content);
			continue;
		}
		
		if( g_str_equal(current_element->name, "retweeted_status") && current_element->children)
			status->retweeted_status=user_status_parse_retweeted_status(service, current_element->children, update_type);
		
		else if(g_str_equal(current_element->name, "id")){
			status->id=g_ascii_strtod(content, NULL);
			status->id_str=gdouble_to_str(status->id);
			debug("Update ID: %s(=%f).", content, status->id);
			
		}else if(g_str_equal(current_element->name, "in_reply_to_status_id"))
			status->in_reply_to_status_id=g_ascii_strtod(content, NULL);
		
		else if(g_str_equal(current_element->name, "source"))
			status->source=g_strdup(content);
		
		else if((g_str_equal(current_element->name, "sender") || g_str_equal(current_element->name, "user")) && current_element->children )
			status->user=user_parse_node(service, current_element->children);
		
		else if(g_str_equal(current_element->name, "text"))
			status->text=g_strdup(content);
		
		else if(g_str_equal(current_element->name, "created_at")){
			status->created_at_str=g_strdup(content);
			datetime_strp_ages(status->created_at_str, &status->created_at, &status->created_how_long_ago, &status->created_seconds_ago, FALSE);
		}
		
		uber_free(content);
		
	}

	user_status_validate(&status);
	
	if(status && status->user && status->retweeted_status)
		user_status_format_retweeted_status(service, status->retweeted_status, &status, status->user);
	
	if(status->user){
		user_validate( &status->user );
		user_status_format_updates(service, status, status->user);
	}
	
	return status;
}/*user_status_parse(service, current_element->children, update_type);*/

static void user_status_format_retweeted_status(OnlineService *service, UserStatus *retweeted_status, UserStatus **status, User *user){
	if(!( service && (*status) && user && retweeted_status )) return;
	gchar *retweeted_user_name=NULL;
	gchar *retweeted_by=NULL;
	UserStatus *status_swap=(*status);
	
	gdouble status_id=(*status)->id;
	gchar *status_id_str=g_strdup( (*status)->id_str );
	
	retweeted_status->created_at=(*status)->created_at;
	uber_free(retweeted_status->created_at_str);
	retweeted_status->created_at_str=g_strdup( (*status)->created_at_str );
	
	retweeted_status->created_seconds_ago=(*status)->created_seconds_ago;
	uber_free(retweeted_status->created_how_long_ago);
	retweeted_status->created_how_long_ago=g_strdup( (*status)->created_how_long_ago );
	
	if(!( (*status) && G_STR_N_EMPTY(user->user_name) && G_STR_N_EMPTY(user->nick_name) )){
		retweeted_user_name=g_strdup("unknown");
		retweeted_by=g_strdup_printf("Anonymous <u>&lt;unknown@%s&gt;</u>", service->uri);
	}else{
		retweeted_user_name=g_strdup(user->user_name);
		retweeted_by=g_strdup_printf("%s <u>&lt;%s@%s&gt;</u>", user->nick_name, user->user_name, service->uri);
	}
	
	(*status)=retweeted_status;
	(*status)->retweeted_status=status_swap;
	(*status)->id=status_id;
	uber_free( (*status)->id_str );
	(*status)->id_str=status_id_str;
	uber_free((*status)->retweeted_by);
	(*status)->retweeted_by=retweeted_by;
	uber_free((*status)->retweeted_user_name);
	(*status)->retweeted_user_name=retweeted_user_name;
}/*user_status_format_retweeted_status(service, retweeted_status, &status, user);*/


static void user_status_format_updates(OnlineService *service, UserStatus *status, User *user){
	if(!(service->connected && G_STR_N_EMPTY(status->text) && G_STR_N_EMPTY(user->user_name) && G_STR_N_EMPTY(user->nick_name))) return;
	
	status->sexy_update=www_html_entity_escape_text(status->text);
	status->sexy_status_text=www_format_urls(service, status->sexy_update, FALSE, FALSE);
	
	if(status->type==Searches){
		debug("Formatting update for display.");
		debug("\tstatus->text: [%s],", status->text);
		debug("\tstatus->update: [%s],", status->update);
		debug("\tsexy_status_text: [%s],", status->sexy_status_text);
		debug("\tsexy_status_swap: [%s],", status->sexy_update);
	}
	
	status->from=g_strdup_printf("<span size=\"small\" weight=\"ultrabold\">%s\n&lt;%s@%s&gt;</span>",
					user->nick_name, user->user_name, service->uri
	);
	
	status->rcpt=g_strdup_printf("<span size=\"small\" weight=\"light\">%s\n&lt;%s&gt;</span>", service->nick_name, service->key);
	
	status->update=g_strdup_printf("%s%s%s%s%s%s%s%s\n",
					(status->retweet ?"<span size=\"x-small\" weight=\"ultrabold\">":"" ),
					(status->retweet ?_("retweeted by") :"" ),
					(status->retweet ?": " :""),
					(status->retweet ?status->retweeted_by :"" ),
					(status->retweet ?"</span>\n" :"" ),
					( (status->type==DMs)
					  	?"<span weight=\"ultrabold\" style=\"italic\" variant=\"smallcaps\">[Direct Message]</span>\n<span weight=\"ultrabold\" style=\"italic\">["
						:(status->type==Replies ?"<span style=\"italic\" variant=\"smallcaps\">[@ reply]</span>\n"
							:""
						)
					),
					status->sexy_status_text,
					((status->type==DMs) ?"]</span>" :"")
	);
	
	status->notification=g_strdup_printf(
						"%s%s%s%s%s%s<i>[%s]</i>\n\t<b>From:</b> <b><u>%s &lt;%s@%s&gt;</u></b>\n\t<i>To:</i> <i><u>%s &lt;%s&gt;</u></i>\n<b>\t%s%s%s</b>",
						(status->retweet ?"<b><i>[" :"" ),
						(status->retweet ?_("retweeted by") :"" ),
						(status->retweet ?"]\n\t" :""),
						(status->retweet ?status->retweeted_by :"" ),
						(status->retweet ?"</i></b>\n" :"" ),
						((status->type==DMs) ?"<b><i><u>[Direct Message]</u></i></b>\n" :((status->type==Replies) ?"<i><u>[@ Reply]</u></i>\n" :"")),
						status->created_how_long_ago,
						user->nick_name, user->user_name, service->uri,
						service->nick_name, service->key,
						((status->type==DMs) ?"<b><i>[" :((status->type==Replies) ?"<i>[" :"")), status->sexy_status_text, ((status->type==DMs) ?"]</i></b>" :((status->type==Replies) ?"]</i>" :""))
	);
}/*user_status_format_updates(service, user->status, user);*/

gboolean user_status_notify_on_timeout(UserStatus *status){
	if(!(status && G_STR_N_EMPTY(status->notification) )) return FALSE;
	
	NotifyNotification *notify_notification=NULL;
	GError		*error=NULL;
	const gchar	*notification_icon_file_name=NULL;
	if(!(g_file_test(status->user->image_file, G_FILE_TEST_EXISTS|G_FILE_TEST_IS_REGULAR )))
		notification_icon_file_name=GETTEXT_PACKAGE;
	else
		notification_icon_file_name=status->user->image_file;
	
	if(!main_window_status_icon_is_embedded())
		notify_notification=notify_notification_new(GETTEXT_PACKAGE, status->notification, notification_icon_file_name, NULL);
	else
		notify_notification=notify_notification_new_with_status_icon(GETTEXT_PACKAGE, status->notification, notification_icon_file_name, main_window_status_icon_get());
	
	notify_notification_set_hint_string(notify_notification, "suppress-sound", "");
	notify_notification_set_timeout(notify_notification, 10000);
	
	if(gconfig_if_bool(PREFS_NOTIFY_BEEP, TRUE))
		update_viewer_beep();
	
	notify_notification_show(notify_notification, &error);
	
	if(error){
		debug("Error displaying status->notification: %s.", error->message);
		g_error_free(error);
	}
	
	if(G_IS_OBJECT(G_OBJECT(notify_notification)))
		g_object_unref(G_OBJECT(notify_notification));
	
	return FALSE;
}/*user_status_notify_on_timeout(status);*/

void user_status_free(UserStatus *status){
	if(!( status && status->id && status->service )) return;
	if(status->user) user_free(status->user);
	else if(status->retweeted_status) user_status_free(status->retweeted_status);
	user_status_validate(&status);
	status->service=NULL;
	
	uber_object_free(&status->text, &status->id_str, &status->from, &status->rcpt, &status->update, &status->source, &status->sexy_status_text, &status->sexy_update, &status->notification, &status->created_at_str, &status->created_how_long_ago, &status->retweeted_by, &status->retweeted_user_name, &status, NULL);
}/*user_status_free*/
/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
