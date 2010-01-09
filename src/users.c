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
#include "main-window.h"

#include "online-services-typedefs.h"
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
#include "parser.h"

#include "www.h"
#include "uberchick-label.h"

#include "ui-utils.h"
#include "update-viewer.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
typedef struct  _UserProfileViewer UserProfileViewer;

struct _UserProfileViewer{
	OnlineService		*service;
	gchar			*user_name;
	gboolean		loading;
	
	GtkMessageDialog	*dialog;
	
	GtkImage		*user_image;
	GtkImage		*image;
	
	GtkVBox			*title_vbox;
	
	UberChickLabel		*service_label;
	UberChickLabel		*user_label;
	
	GtkVBox			*profile_vbox;
	UberChickLabel		*url_hyperlink;
	UberChickLabel		*bio_html;
	
	GtkVBox			*latest_update_vbox;
	GtkLabel		*latest_update_label;
	GtkLabel		*updated_when_label;
	UberChickLabel		*most_recent_update;
};


#define API_USER_FOLLOW		"/friendships/create/%s.xml"
#define API_USER_UNFOLLOW	"/friendships/destroy/%s.xml"
#define API_USER_BLOCK		"/blocks/create/%s.xml"
#define API_USER_UNBLOCK	"/blocks/destroy/%s.xml"

#define API_USER_PROFILE	"/users/show/%s.xml"

#define	DEBUG_DOMAINS	"UI:GtkBuilder:GtkBuildable:OnlineServices:Updates:Requests:Dates:Times:Users:Settings:Profiles:UserProfileViewer:ViewProfile:users.c"
#include "debug.h"

#define GtkBuilderUI "user-profile-viewer"
static UserProfileViewer *user_profile_viewer=NULL;


/********************************************************************************
 *                    prototypes for private method & function                  *
 ********************************************************************************/
static User *user_new(OnlineService *service, gboolean a_follower);
static void user_validate( User **user );


static UserStatus *user_status_new(OnlineService *service, UpdateMonitor monitoring);
static void user_status_validate( UserStatus **status );
static UserStatus *user_status_parse_retweeted_status(OnlineService *service, xmlNode *root_element, UpdateMonitor monitoring);
static void user_status_format_retweeted_status(OnlineService *service, UserStatus *retweeted_status, UserStatus **status, User *user);
static void user_status_format_updates(OnlineService *service, UserStatus *status, User *user);
static void user_status_format_dates(UserStatus *status);


static void user_profile_viewer_response(GtkMessageDialog *message_dialog, gint response);
static void user_profile_viewer_destroy(GtkMessageDialog *message_dialog);

static void user_profile_viewer_setup(void);

static void user_profile_viewer_display_profile(OnlineServiceWrapper *online_service_wrapper, SoupMessage *xml, User *user);

static void user_profile_viewer_set_avatar(const gchar *image_file, const gchar *image_uri);
static void user_profile_viewer_download_avatar(const gchar *image_file, const gchar *image_uri);
static void *user_profile_viewer_save_avatar(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper);

static void user_profile_viewer_hide_all(void);
static void user_profile_viewer_show_all(void);



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
	user->id_str=user->user_name=user->nick_name=user->location=user->bio=user->url=user->image_uri=user->image_file=NULL;
	
	return user;
}/*user_new*/

static void user_validate( User **user ){
	if(! G_STR_N_EMPTY( (*user)->user_name ) ) (*user)->user_name=g_strdup("");
	if(! G_STR_N_EMPTY( (*user)->nick_name ) )
		if(!( G_STR_EMPTY( (*user)->user_name ) )) (*user)->nick_name=g_strdup( (*user)->user_name );
		else (*user)->nick_name=g_strdup("");
	if(! G_STR_N_EMPTY( (*user)->location ) ) (*user)->location=g_strdup("");
	if(! G_STR_N_EMPTY( (*user)->bio ) ) (*user)->bio=g_strdup("unavailable");
	if(! G_STR_N_EMPTY( (*user)->url ) )
		if(G_STR_N_EMPTY( (*user)->user_name ) && (*user)->service && (*user)->service->key )
			(*user)->url=g_strdup_printf("http://%s/%s", (*user)->service->uri, (*user)->user_name );
		else
			(*user)->url=g_strdup("");
	if(! G_STR_N_EMPTY( (*user)->image_uri ) ) (*user)->image_uri=g_strdup("");
	if(! G_STR_N_EMPTY( (*user)->image_file ) ) (*user)->image_file=g_strdup("");
}/*user_validate(&user);*/ 

User *user_parse_profile(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	xmlDoc *doc=NULL;
	xmlNode *root_element=NULL;
	User *user=NULL;
	
	if(!( (doc=parse_xml_doc(xml, &root_element )) )){
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
	xmlNode		*current_node=NULL;
	gchar		*content=NULL;
	
	User		*user=user_new(service, getting_followers);
	
	debug("Parsing user profile data.");
	/* Begin 'users' node loop */
	debug("Parsing user beginning at node: %s", root_element->name);
	for(current_node=root_element; current_node; current_node=current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE && current_node->type != XML_ATTRIBUTE_NODE )
			continue;
		
		if(!(G_STR_N_EMPTY( (content=(gchar *)xmlNodeGetContent(current_node)) ) )) continue;
		
		if(g_str_equal(current_node->name, "id" )){
			user->id=strtod(content, NULL);
			user->id_str=gdouble_to_str(user->id);
			debug("User ID: %s(=%f).", user->id_str, user->id);
			
		}else if(g_str_equal(current_node->name, "name" ))
			user->nick_name=g_markup_printf_escaped("%s", content);
		
		else if(g_str_equal(current_node->name, "screen_name" ))
			user->user_name=g_markup_printf_escaped("%s", content);
		
		else if(g_str_equal(current_node->name, "location" ))
			user->location=g_markup_printf_escaped("%s", content);
		
		else if(g_str_equal(current_node->name, "description" )){
			if(G_STR_N_EMPTY(content))
				user->bio=g_markup_printf_escaped("%s", content);
			else
				user->bio=g_strdup("unavailable");
		}else if(g_str_equal(current_node->name, "url" ))
			user->url=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "followers_count" ))
			user->followers=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_node->name, "friends_count" ))
			user->following=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_node->name, "statuses_count" ))
			user->updates=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_node->name, "profile_image_url"))
			user->image_uri=g_strdup(content);
		
		else if( g_str_equal(current_node->name, "status") && current_node->children)
			user->status=user_status_parse(service, current_node->children, Updates);
		
		else if( g_str_equal(current_node->name, "retweeted_status") && user->status && current_node->children){
			user->status->retweeted_status=user_status_parse_retweeted_status(service, current_node->children, Updates);
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
	
	return user;
}/*user_parse_node(service, root_node); || user_parse_node(service, current_node->children);*/

void user_free(User *user){
	if(!( user && user->id && user->service )) return;
	user_validate(&user);
	if(user->status) user_status_free(user->status);
	
	user->service=NULL;
	
	uber_object_free(&user->user_name, &user->nick_name, &user->location, &user->bio, &user->url, &user->image_uri, &user->image_file, &user, NULL);
}/*user_free*/


/********************************************************************************
 *                           UserStatus methods                                 *
 ********************************************************************************/
static UserStatus *user_status_new(OnlineService *service, UpdateMonitor monitoring){
	UserStatus	*status=g_new0(UserStatus, 1);
	
	status->service=service;
	status->retweet=FALSE;
	status->user=NULL;
	status->retweeted_status=NULL;
	status->type=monitoring;
	status->id_str=status->text=status->sexy_status_text=status->update=status->notification=status->sexy_update=status->created_at_str=status->created_how_long_ago=status->retweeted_by=status->retweeted_user_name=NULL;
	status->id=status->in_reply_to_status_id=0.0;
	status->created_at=0;
	status->created_seconds_ago=0;
	
	return status;
}/*user_status_new(service, Updates|Replies|Dms);*/

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

UserStatus *user_status_parse_from_atom_entry(OnlineService *service, xmlNode *root_element, UpdateMonitor monitoring){
	xmlNode		*current_node=NULL, *temp_node=NULL;
	gchar		*content=NULL;
	UserStatus	*status=user_status_new(service, Searches);
	status->user=user_new(service, getting_followers);
	
	/* Begin 'status' or 'direct-messages' loop */
	debug("Parsing searches results beginning at node: %s", root_element->name);
	for(current_node=root_element; current_node; current_node=current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE && current_node->type != XML_ATTRIBUTE_NODE ) continue;
		
		if( !g_str_equal(current_node->name, "link") && G_STR_EMPTY( (content=(gchar *)xmlNodeGetContent(current_node)) ) ){
			if(content) uber_free(content);
			continue;
		}
		
		debug("Parsing searches beginning at node: %s.", current_node->name );
		if(g_str_equal(current_node->name, "id")){
			status->id=strtod((g_strrstr(content, ":")+1), NULL);
			status->id_str=gdouble_to_str(status->id);
			debug("Parsing searches Update ID: %s(=%f).", status->id_str, status->id);
		}else if(g_str_equal(current_node->name, "published") || g_str_equal(current_node->name, "updated")){
			if( status->created_at_str && !g_str_equal(status->created_at_str, content) ){
				uber_free(status->created_at_str);
				uber_free(status->created_how_long_ago);
				status->created_at=0;
			}
			status->created_at_str=g_strdup(content);
			user_status_format_dates(status);
		}else if(g_str_equal(current_node->name, "title")){
			status->text=g_strdup(content);
			debug("Parsing searches update: %s; from: %s.", status->text, current_node->name );
		}else if(g_str_equal(current_node->name, "link")){
			if(content) uber_free(content);
			xmlAttr *attr_node;
			debug("Parsing searches link node searching for type=\"image/png\".");
			for( attr_node=current_node->properties; attr_node; attr_node=attr_node->next ){
				debug("Parsing searches at node: %s looking for type.", attr_node->name );
				if(g_str_equal(attr_node->name, "type")) break;
			}
			if(!attr_node) continue;
			
			for(temp_node=attr_node->children; temp_node; temp_node=temp_node->next){
				content=(gchar *)xmlNodeGetContent(temp_node);
				debug("Parsing searches at node: %s looking for image/png.", content );
				if(g_str_equal(content, "image/png")) break;
				uber_free(content);
			}
			if(content) uber_free(content);
			if(!temp_node) continue;
			
			debug("Parsed searches type and image/png nodes.  Now searching href node.");
			for( attr_node=attr_node->next; attr_node; attr_node=attr_node->next ){
				debug("Parsing searches node %s looking for href.", attr_node->name );
				if(g_str_equal(attr_node->name, "href") ) break;
			}
			if(!attr_node) continue;
			
			debug("Parsed searches href nodes.  Setting avatar's uri.");
			status->user->image_uri=(gchar *)xmlNodeGetContent(attr_node->children);
			debug("Parsing searches nodes finished.  Setting user->image: %s.", status->user->image_uri );
			
			attr_node=NULL;
			temp_node=NULL;
			continue;
		}else if(g_str_equal(current_node->name, "twitter:source")){
			status->source=g_strdup(content);
		}else if(g_str_equal(current_node->name, "author")){
			if(content) uber_free(content);
			debug("Parsing searches user_name & user_nick from: %s.", current_node->name );
			for( temp_node=current_node->children; temp_node; temp_node=temp_node->next ){
				debug("Parsing searches for user_name & user_nick: %s.", temp_node->name );
				if(g_str_equal(temp_node->name, "name") ) break;
			}
			if(content) uber_free(content);
			if(!temp_node) continue;
			if(!(G_STR_N_EMPTY( (content=(gchar *)xmlNodeGetContent(temp_node)) ) )){
				if(content) uber_free(content);
				temp_node=NULL;
				continue;
			}
			
			debug("Parsing searches user_name & user_nick from content: %s.", content );
			if(!g_strrstr(content, "(")){
				status->user->user_name=g_markup_printf_escaped("%s", content);
				/*status->user->nick_name=g_markup_printf_escaped("%s", content);*/
			}else{
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
			temp_node=NULL;
		}
		if(content) uber_free(content);
	}
	
	status->user->image_file=cache_images_get_user_avatar_filename(service, status->user->user_name, status->user->image_uri);
	
	user_validate( &status->user );
	user_status_validate(&status);
	
	user_status_format_updates(service, status, status->user);
	
	return status;
}/*user_status_parse_from_atom_entry(service, current_node->children, monitoring);*/

static UserStatus *user_status_parse_retweeted_status(OnlineService *service, xmlNode *root_element, UpdateMonitor monitoring){
	debug("Parsing retweeted update beginning at node: %s", root_element->name);
	UserStatus *status=user_status_parse(service, root_element, monitoring);
	status->retweet=TRUE;
	return status;
}/*user_status_parse_retweeted_status(service, current_node->children, Updates);*/

UserStatus *user_status_parse(OnlineService *service, xmlNode *root_element, UpdateMonitor monitoring){
	xmlNode		*current_node=NULL;
	gchar		*content=NULL;
	UserStatus	*status=user_status_new(service, monitoring);
	
	/* Begin 'status' or 'direct-messages' loop */
	debug("Parsing update at beginning node: %s", root_element->name);
	for(current_node=root_element; current_node; current_node=current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE && current_node->type != XML_ATTRIBUTE_NODE ) continue;
		
		if(!(G_STR_N_EMPTY( (content=(gchar *)xmlNodeGetContent(current_node)) ) )){
			if(content) uber_free(content);
			continue;
		}
		
		if( g_str_equal(current_node->name, "retweeted_status") && current_node->children)
			status->retweeted_status=user_status_parse_retweeted_status(service, current_node->children, monitoring);
		
		else if(g_str_equal(current_node->name, "id")){
			status->id=strtod(content, NULL);
			status->id_str=gdouble_to_str(status->id);
			debug("Update ID: %s(=%f).", content, status->id);
			
		}else if(g_str_equal(current_node->name, "in_reply_to_status_id"))
			status->in_reply_to_status_id=strtod(content, NULL);
		
		else if(g_str_equal(current_node->name, "source"))
			status->source=g_strdup(content);
		
		else if((g_str_equal(current_node->name, "sender") || g_str_equal(current_node->name, "user")) && current_node->children )
			status->user=user_parse_node(service, current_node->children);
		
		else if(g_str_equal(current_node->name, "text"))
			status->text=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "created_at")){
			status->created_at_str=g_strdup(content);
			user_status_format_dates(status);
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
}/*user_status_parse(service, current_node->children, monitoring);*/

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

static void user_status_format_dates(UserStatus *status){
	tzset();
	time_t		t=time(NULL);
	struct tm	*ta;
	if(status->type!=Searches)
		ta=gmtime(&t);
	else
		ta=localtime(&t);
	ta->tm_isdst=-1;
	
	struct tm	post;
	strptime(status->created_at_str, "%s", &post);
	post.tm_isdst=-1;
	status->created_at=mktime(&post);
	
	debug("Parsing update's 'created_at' date: [%s] to Unix seconds since: %lu", status->created_at_str, status->created_at);
	status->created_how_long_ago=user_status_convert_time(status->created_at_str, &status->created_seconds_ago, (status->type==Searches ?FALSE :TRUE) );
	debug("Display time set to: %s, %d.", status->created_how_long_ago, status->created_seconds_ago);
}/*user_status_format_dates*/

gchar *user_status_convert_time(const gchar *datetime, gint *my_diff, gboolean use_gmt){
	gint diff=update_convert_datetime_to_seconds_old(datetime, use_gmt);
	if(diff < 0) *my_diff=0;
	else *my_diff=diff;
	
	/* Up to one minute ago. */
	
	if(diff < 2) return g_strdup(_("1 second ago"));
	if(diff < 60 ) return g_strdup_printf(_("%d seconds ago"), diff);
	if(diff < 120) return g_strdup(_("1 minute ago"));
	
	/* Minutes */
	diff=diff/60;
	if(diff < 60) return g_strdup_printf(_("%d minutes ago"), diff);
	if(diff < 120) return g_strdup(_("1 hour ago"));
	
	/* Hours */
	diff=diff/60;
	if(diff < 24) return g_strdup_printf(_("%d hours ago"), diff);
	if(diff < 48) return g_strdup(_("1 day ago"));
	
	/* Days */
	diff=diff/24;
	if(diff < 30) return g_strdup_printf(_("%d days ago"), diff);
	if(diff < 365) return g_strdup_printf(_("%d months ago"), (diff/30));
	
	diff=diff/365;
	return g_strdup_printf(_("%d year%s ago"), diff, (diff==1 ?"" :"s") );
	/* NOTE:
	 * 	About time, month, & year precision, "years aren't...blah blah".
	 * 	yeah well I agree!
	 * 	but I'm dealing w/integers not floating point arthmatic.
	 * 	so we'll all just have to get over is.
	 */
}/*user_status_convert_time(date_created_string, &created_seconds_ago);*/

gint update_convert_datetime_to_seconds_old(const gchar *datetime, gboolean use_gmt){
	struct tm	*ta;
	struct tm	post;
	int		seconds_local;
	int		seconds_post;
	time_t		t=time(NULL);
	
	tzset();
	if(use_gmt)
		ta=gmtime(&t);
	else
		ta=localtime(&t);
	ta->tm_isdst=-1;
	
	seconds_local=mktime(ta);
	
	strptime(datetime, "%a %b %d %T +0000 %Y", &post);
	post.tm_isdst=-1;
	seconds_post=mktime(&post);
	
	return difftime(seconds_local, seconds_post);
}/*
	update_convert_datetime_to_seconds_old("Fri Nov  6 16:30:31 -0000 2009");
	update_convert_datetime_to_seconds_old(datetime);
*/


static void user_status_format_updates(OnlineService *service, UserStatus *status, User *user){
	if(!(service->connected && G_STR_N_EMPTY(status->text) && G_STR_N_EMPTY(user->user_name) && G_STR_N_EMPTY(user->nick_name))) return;
	
	gchar *sexy_status_swap=www_html_entity_escape_text(status->text);
	/*if(!gconfig_if_bool(PREFS_URLS_EXPANSION_SELECTED_ONLY, TRUE)){
		status->sexy_update=www_format_urls(service, sexy_status_swap, TRUE, TRUE);
		status->sexy_status_text=www_format_urls(service, sexy_status_swap, TRUE, FALSE);
	}else{*/
		status->sexy_update=g_strdup(sexy_status_swap);
		status->sexy_status_text=www_format_urls(service, sexy_status_swap, FALSE, FALSE);
	//}
	if(status->type==Searches){
		debug("Formatting update for display.");
		debug("\tstatus->text: [%s],", status->text);
		debug("\tstatus->update: [%s],", status->update);
		debug("\tsexy_status_text: [%s],", status->sexy_status_text);
		debug("\tsexy_status_swap: [%s],", sexy_status_swap);
	}
	uber_free(sexy_status_swap);
	
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
						"%s%s%s%s%s%s<i>[%s]</i>\n\t<b>From:</b> <b><u>%s &lt;%s@%s&gt;</u></b>\n<i>To:</i> <i><u>%s &lt;%s&gt;</u></i>\n<b>\t%s%s%s</b>",
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
	
	NotifyNotification *notify_notification;
	GError             *error=NULL;
	
	if(!main_window_status_icon_is_embedded())
		notify_notification=notify_notification_new(PACKAGE_TARNAME, status->notification, PACKAGE_TARNAME, NULL);
	else
		notify_notification=notify_notification_new_with_status_icon(PACKAGE_TARNAME, status->notification, PACKAGE_TARNAME, main_window_status_icon_get());
	
	notify_notification_set_timeout(notify_notification, 10000);
	
	if(gconfig_if_bool(PREFS_NOTIFY_BEEP, TRUE))
		update_viewer_beep();
	
	notify_notification_show(notify_notification, &error);
	
	if(error){
		debug("Error displaying status->notification: %s.", error->message);
		g_error_free(error);
	}
	
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
 *                       UserProfileViewer methods                              *
 ********************************************************************************/
static void user_profile_viewer_response(GtkMessageDialog *message_dialog, gint response){
	gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->dialog));
}/*user_profile_viewer_response(message_dialog, response, user_profile_viewer);*/

static void user_profile_viewer_destroy(GtkMessageDialog *message_dialog){
	debug("Destroying user profile viewer");
	user_profile_viewer->service=NULL;
	if(user_profile_viewer->user_name) uber_free(user_profile_viewer->user_name);
	if(user_profile_viewer->most_recent_update) gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->most_recent_update));
	if(user_profile_viewer->url_hyperlink) gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->url_hyperlink));
	if(user_profile_viewer->bio_html) gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->bio_html));
	if(user_profile_viewer->service_label) gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->service_label));
	if(user_profile_viewer->user_label) gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->user_label));
	uber_free(user_profile_viewer);
}/*user_profile_viewer_destroy(dialog);*/


void user_profile_viewer_show(OnlineService *service, const gchar *user_name, GtkWindow *parent){
	if(!user_profile_viewer) user_profile_viewer_setup();
	else if(user_profile_viewer->service && user_profile_viewer->user_name && user_profile_viewer->service==service && !strcasecmp(user_profile_viewer->user_name, user_name) ){
		user_profile_viewer_show_all();
		return;
	}
	
	user_profile_viewer->service=service;;
	if(user_profile_viewer->user_name) uber_free(user_profile_viewer->user_name);
	user_profile_viewer->user_name=g_strdup(user_name);
	
	user_profile_viewer_hide_all();
	
	gchar *window_title=g_strdup_printf("%s - <%s@%s>'s %s", _(GETTEXT_PACKAGE), user_name, service->uri, _("profile"));
	gtk_window_set_title(GTK_WINDOW(user_profile_viewer->dialog), window_title);
	uber_free(window_title);
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->dialog));
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->user_image));
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->bio_html));
	
	g_object_add_weak_pointer(G_OBJECT(user_profile_viewer->dialog), (gpointer)&user_profile_viewer);
	gtk_window_set_transient_for(GTK_WINDOW(user_profile_viewer->dialog), parent);
	window_present(GTK_WINDOW(user_profile_viewer->dialog), TRUE);
	
	online_service_fetch_profile( service, user_name, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)user_profile_viewer_display_profile );
}/*user_profile_viewer_show( service, "uberChick", update_viewer->update_viewer );*/

static void user_profile_viewer_display_profile(OnlineServiceWrapper *online_service_wrapper, SoupMessage *xml, User *user){ 
	OnlineService *service=online_service_wrapper_get_online_service(online_service_wrapper);
	
	debug("Downloading user's avatar from: <%s> to [ file://%s ]", user->image_uri, user->image_file);
	user_profile_viewer_set_avatar(user->image_file, user->image_uri);
	
	gchar *profile_details=g_strdup_printf(
					"\t[<a href=\"http%s://%s/\">%s</a>]\n",
					(service->https?"s":""), service->uri, service->guid
	);
	
	uberchick_label_set_text(user_profile_viewer->service_label, service, user->user_name, user->id, user->status->id, profile_details, TRUE, TRUE);
	uber_free(profile_details);
	
	
	
	profile_details=g_strdup_printf(
					"\t\t<u><b>%s:</b></u> @%s",
					user->nick_name, user->user_name
	);
	uberchick_label_set_text(user_profile_viewer->user_label, service, user->user_name, user->id, user->status->id, profile_details, TRUE, TRUE);
	uber_free(profile_details);
	
	
	
	profile_details=g_strdup_printf(
					"\t<b>Updates:</b> %lu\n\t<b>Following:</b> %lu\n\t<b>Followers:</b> %lu\n\t<b>Location:</b> %s\n",
					user->updates,
					user->following,
					user->followers,
					user->location
	);
	gtk_message_dialog_set_markup( user_profile_viewer->dialog, profile_details );
	uber_free(profile_details);
	
	profile_details=g_strdup_printf( "\t<b>URL:</b>\t<a href=\"%s\">%s</a>\n", user->url, user->url );
	uberchick_label_set_text(user_profile_viewer->url_hyperlink, service, user->user_name, user->id, user->status->id, profile_details, TRUE, TRUE);
	uber_free(profile_details);
	
	g_object_set(GTK_LABEL(user_profile_viewer->bio_html), "single-line-mode", FALSE, NULL );
	profile_details=g_strdup_printf( "\t<b>Bio:</b>\n\t\t%s\n", user->bio );
	uberchick_label_set_text(user_profile_viewer->bio_html, service, user->user_name, user->id, user->status->id, profile_details, TRUE, TRUE);
	uber_free(profile_details);
	
	profile_details=g_markup_printf_escaped("<b>Last updated:</b> <i>[%s]</i>", user->status->created_how_long_ago);
	gtk_label_set_markup(user_profile_viewer->updated_when_label, profile_details);
	uber_free(profile_details);
	
	uberchick_label_set_text(user_profile_viewer->most_recent_update, service, user->user_name, user->id, user->status->id, user->status->sexy_update, TRUE, TRUE);
	
	if(!user_profile_viewer->loading)
		user_profile_viewer_show_all();
	
	user_free(user);
}/*static void user_profile_viewer_display_profile(online_service_wrapper, xml, user);*/

static void user_profile_viewer_setup(void){
	user_profile_viewer=g_new( UserProfileViewer, 1 );
	
	user_profile_viewer->service=NULL;
	user_profile_viewer->user_name=NULL;
	user_profile_viewer->loading=TRUE;
	
	GtkBuilder *ui=gtkbuilder_get_file(
						GtkBuilderUI,
							"profile_dialog", &user_profile_viewer->dialog,
							
							"title_vbox", &user_profile_viewer->title_vbox,
							
							"profile_vbox", &user_profile_viewer->profile_vbox,
							"user_image", &user_profile_viewer->user_image,
							
							"latest_update_vbox", &user_profile_viewer->latest_update_vbox,
						NULL
	);
	
	
	gtkbuilder_connect(
				ui, user_profile_viewer,
					"profile_dialog", "destroy", user_profile_viewer_destroy,
					"profile_dialog", "response", user_profile_viewer_response,
				NULL
	);
	
	user_profile_viewer->service_label=uberchick_label_new();
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->title_vbox),
				GTK_WIDGET(user_profile_viewer->service_label),
				TRUE, TRUE, 0
	);
	
	user_profile_viewer->user_label=uberchick_label_new();
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->title_vbox),
				GTK_WIDGET(user_profile_viewer->user_label),
				TRUE, TRUE, 0
	);
	
	user_profile_viewer->url_hyperlink=uberchick_label_new();
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->profile_vbox),
				GTK_WIDGET(user_profile_viewer->url_hyperlink),
				TRUE, TRUE, 0
	);
	
	user_profile_viewer->bio_html=uberchick_label_new();
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->profile_vbox),
				GTK_WIDGET(user_profile_viewer->bio_html),
				TRUE, TRUE, 0
	);
	
	user_profile_viewer->latest_update_label=(GtkLabel *)gtk_label_new(NULL);
	gtk_label_set_markup(user_profile_viewer->latest_update_label, "<b><u>Most recent update:</u></b>");
	g_object_set(user_profile_viewer->latest_update_label, "single-line-mode", TRUE, NULL);
	gtk_box_pack_start(
			GTK_BOX(user_profile_viewer->latest_update_vbox),
			GTK_WIDGET(user_profile_viewer->latest_update_label),
			TRUE, TRUE, 0
	);
	
	user_profile_viewer->most_recent_update=uberchick_label_new();
	gtk_box_pack_start(
			GTK_BOX(user_profile_viewer->latest_update_vbox),
			GTK_WIDGET(user_profile_viewer->most_recent_update),
			TRUE, TRUE, 0
	);
	
	user_profile_viewer->updated_when_label=(GtkLabel *)gtk_label_new(NULL);
	g_object_set(user_profile_viewer->updated_when_label, "xalign", 1.0, "single-line-mode", TRUE, NULL);
	gtk_box_pack_start(
			GTK_BOX(user_profile_viewer->latest_update_vbox),
			GTK_WIDGET(user_profile_viewer->updated_when_label),
			TRUE, TRUE, 0
	);
	
	gtk_widget_show_all(GTK_WIDGET(user_profile_viewer->dialog));
}/*user_profile_viewer_setup();*/

static void user_profile_viewer_set_avatar(const gchar *image_file, const gchar *image_uri){
	if( !g_file_test(image_file, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR) && user_profile_viewer->loading )
		return user_profile_viewer_download_avatar(image_file, image_uri);
	
	debug("Avatar: GtkImage created GtkImage for display in UserProfileViewer");
	debug("Avatar: Downloaded from <%s>", image_uri);
	debug("Avatar: saved to: [%s]", image_file);
	gtk_message_dialog_set_image( user_profile_viewer->dialog, GTK_WIDGET(user_profile_viewer->image=images_get_maximized_image_from_filename((gchar *)image_file)) );
	
	if(user_profile_viewer->loading)
		user_profile_viewer_show_all();
}/*user_profile_viewer_set_avatar(const gchar *image_file, const gchar *image_uri);*/

static void user_profile_viewer_download_avatar(const gchar *image_file, const gchar *image_uri){
	debug("Downloading Image: %s.  GET: %s", image_file, image_uri);
	
	online_service_request_uri(user_profile_viewer->service, QUEUE, image_uri, 0, NULL, user_profile_viewer_save_avatar, g_strdup(image_file), g_strdup(image_uri) );
}/*user_profile_viewer_download_avatar(user->service, user);*/

static void *user_profile_viewer_save_avatar(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	const gchar *image_file=online_service_wrapper_get_user_data(service_wrapper);
	const gchar *image_uri=online_service_wrapper_get_form_data(service_wrapper);
	debug("Image download returned: %s(%d).", xml->reason_phrase, xml->status_code);
	
	gchar *image_filename=NULL, *error_message=NULL;
	if(!(www_xml_error_check(service, image_uri, xml, &error_message))){
		debug("Failed to download and save avatar: <%s> to file: [%s].", image_uri, image_file);
		debug("Detailed error message: %s.", error_message);
		image_filename=cache_images_get_unknown_image_filename();
		main_window_statusbar_printf("Error adding avatar to UserProfileViewer.  GNOME's unknown-image will be used instead.");
	}else if(!( g_strrstr( soup_message_headers_get_one( xml->response_headers, "Content-Type"), "image" ) )){
		debug("Avatar does not appear to be an image: <%s>.  GNOME's unknown-image will be used instead.", image_uri);
		image_filename=cache_images_get_unknown_image_filename();
	}else if(!(g_file_set_contents(
				image_file,
					xml->response_body->data,
					xml->response_body->length,
				NULL
	))){
		debug("Failed to download and save avatar: <%s> to file: [%s].", image_uri, image_file);
		image_filename=cache_images_get_unknown_image_filename();
	}else{
		debug("Saved avatar: <%s> to file: [%s]", image_uri, image_file);
		image_filename=g_strdup(image_file);
	}
	main_window_statusbar_printf("Avatar added to UserProfileViewer.");
	
	user_profile_viewer_set_avatar(image_filename, image_uri);
	
	uber_free(image_filename);
	uber_free(error_message);
	
	return NULL;
}/*user_profile_viewer_save_avatar(user);*/

static void user_profile_viewer_show_all(void){
	user_profile_viewer->loading=FALSE;
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->dialog));
	gtk_widget_show_all(GTK_WIDGET(user_profile_viewer->dialog));
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->service_label));
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->user_label));
	
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->user_image));
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->url_hyperlink));
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->bio_html));
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->latest_update_label));
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->most_recent_update));
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->updated_when_label));
	
	window_present(GTK_WINDOW(user_profile_viewer->dialog), TRUE);
}/*user_profile_viewer_show_all();*/

static void user_profile_viewer_hide_all(void){
	user_profile_viewer->loading=TRUE;
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->user_image));
	gtk_message_dialog_set_image( user_profile_viewer->dialog, GTK_WIDGET(images_get_dialog_image_from_stock("gtk-dialog-info") ) );
	gtk_image_set_from_stock(user_profile_viewer->user_image, "gtk-dialog-info", ImagesDialog );
	gtk_message_dialog_set_markup( user_profile_viewer->dialog, "" );
	
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->service_label));
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->user_label));
	
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->url_hyperlink));
	
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->latest_update_label));
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->most_recent_update));
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->updated_when_label));
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->bio_html));
	
	g_object_set(GTK_LABEL(user_profile_viewer->bio_html), "single-line-mode", TRUE, NULL );
	gchar *profile_details=g_strdup_printf( "<span weight=\"bold\">Please wait for @%s's <a href=\"http%s://%s/\">%s</a> profile to load,</span>", user_profile_viewer->user_name, (user_profile_viewer->service->https?"s":""), user_profile_viewer->service->uri, user_profile_viewer->service->uri );
	uberchick_label_set_text(user_profile_viewer->bio_html, user_profile_viewer->service, NULL, 0.0, 0.0, profile_details, FALSE, TRUE);
	uber_free(profile_details);
	
	user_profile_viewer->loading=TRUE;
}/*user_profile_viewer_hide_all();*/
/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
