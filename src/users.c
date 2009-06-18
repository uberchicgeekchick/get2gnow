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


/********************************************************************************
 * project, objece, system & library headers, eg #include <gdk/gdkkeysyms.h>    *
 ********************************************************************************/
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
#include "main-window.h"

#include "online-service.h"
#include "online-services.h"
#include "online-service-wrapper.h"
#include "network.h"

#include "users-glists.h"
#include "users.h"
#include "cache.h"

#include "gconfig.h"
#include "preferences.h"
#include "gtkbuilder.h"
#include "cache.h"
#include "parser.h"
#include "label.h"

#include "ui-utils.h"
#include "tweet-view.h"
#include "tweet-list.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
typedef struct  _UserProfileViewer UserProfileViewer;

struct _User {
	OnlineService		*service;
	
	gulong			id;
	gchar			*id_str;
	
	gchar			*user_name;
	gchar			*nick_name;
	
	UserStatus		*status;
	
	gchar			*location;
	gchar			*bio;
	gchar			*url;
	
	gchar			*image_url;
	gchar			*image_filename;
	
	gulong			tweets;
	gulong			following;
	gulong			followers;
	
	gboolean		follower;
};

struct _UserStatus {
	OnlineService	*service;
	
	User		*user;
	
	TweetLists	type;
	
	gulong		id;
	gchar		*id_str;
	
	gulong		in_reply_to_status_id;
	
	guint		notification_timeout_id;
	
	gchar		*from;
	gchar		*rcpt;
	
	gchar		*text;
	gchar		*tweet;
	gchar		*notification;
	gchar		*sexy_tweet;
	
	gchar		*source;
	
	gchar		*created_at_str;
	gchar		*created_how_long_ago;
	
	gulong		created_at;
	gulong		created_seconds_ago;
};

struct _UserProfileViewer{
	User			*user;
	GtkMessageDialog	*dialog;
	
	GtkLabel		*loading_label;
	
	GtkImage		*image;
	
	GtkVBox			*title_vbox;
	Label			*user_label;
	
	GtkVBox			*profile_vbox;
	Label			*url_hyperlink;
	Label			*bio_html;
	
	GtkHBox			*latest_update_hbox;
	GtkLabel		*updated_when_label;
	
	GtkVBox			*latest_update_vbox;
	Label			*latest_tweet;
};


#define API_USER_FOLLOW		"/friendships/create/%s.xml"
#define API_USER_UNFOLLOW	"/friendships/destroy/%s.xml"
#define API_USER_BLOCK		"/blocks/create/%s.xml"
#define API_USER_UNBLOCK	"/blocks/destroy/%s.xml"

#define API_USER_PROFILE	"/users/show/%s.xml"

#define	DEBUG_DOMAINS	"OnlineServices:Tweets:Requests:Users:Settings:UserProfileViewer:ViewProfile:Users.c"
#include "debug.h"

#define GtkBuilderUI "user-profile-viewer.ui"
static UserProfileViewer *user_profile_viewer=NULL;


/********************************************************************************
 *                    prototypes for private method & function                  *
 ********************************************************************************/
static User *user_new(OnlineService *service, gboolean a_follower);

static UserStatus *user_status_new(OnlineService *service, TweetLists tweet_list);
static void user_status_format_updates(OnlineService *service, User *user, UserStatus *status);
static void user_status_format_dates(UserStatus *status);

static void user_profile_viewer_response(GtkDialog *dialog, gint response, UserProfileViewer *user_profile_viewer);
static void user_profile_viewer_destroy(GtkDialog *dialog);
static void user_profile_viewer_setup(GtkWindow *parent);



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
	user->id_str=user->user_name=user->nick_name=user->location=user->bio=user->url=user->image_url=user->image_filename=NULL;
	
	return user;
}/*user_new*/

gulong user_get_id(User *user){
	if(!user) return 0;
	return user->id;
}/*user_get_id(user);*/

const gchar *user_get_user_name(User *user){
	if(!user) return NULL;
	return user->user_name;
}/*user_get_user_name(user);*/

const gchar *user_get_nick_name(User *user){
	if(!user) return NULL;
	return user->nick_name;
}/*user_get_user_nick(user);*/

OnlineService *user_get_online_service(User *user){
	if(!user) return NULL;
	return user->service;
}/*user_get_online_service(user);*/

gboolean user_is_follower(User *user){
	if(!user) return FALSE;
	return user->follower;
}/*user_is_follower(user);*/

User *user_parse_profile(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	xmlDoc *doc=NULL;
	xmlNode *root_element=NULL;
	User *user=NULL;
	
	if(!( (doc=parse_xml_doc(xml, &root_element )) )){
		xmlCleanupParser();
		return NULL;
	}
	
	if(g_str_equal(root_element->name, "user"))
		user=user_parse_node(service, root_element->children);
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return user;
}/*user_parse_profile(service, xml, service_wrapper);*/

User *user_fetch_profile(OnlineService *service, const gchar *user_name){
	if( !user_name || G_STR_EMPTY(user_name))
		return NULL;
	
	User *user=NULL;
	
	gchar *user_profile_uri=g_strdup_printf(API_USER_PROFILE, user_name);
	SoupMessage *msg=online_service_request(service, GET, user_profile_uri, NULL, NULL, NULL, NULL);
	
	OnlineServiceWrapper *online_service_wrapper=online_service_wrapper=online_service_wrapper_new(service, user_profile_uri, NULL, NULL, NULL, NULL);
	uber_free(user_profile_uri);
	if(!(user=user_parse_profile(online_service_get_session(service), msg, online_service_wrapper))){
		online_service_wrapper_free(online_service_wrapper);
		return NULL;
	}
	
	online_service_wrapper_free(online_service_wrapper);
	return user;
}/*user_fetch_profile*/

User *user_parse_node(OnlineService *service, xmlNode *root_element){
	xmlNode		*current_node=NULL;
	gchar		*content=NULL;
	
	User		*user=user_new(service, getting_followers);
	
	debug("Parsing user profile data.");
	/* Begin 'users' node loop */
	for(current_node=root_element; current_node; current_node=current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE)
			continue;
		
		if( G_STR_EMPTY( (content=(gchar *)xmlNodeGetContent(current_node)) ) ) continue;
		
		if(g_str_equal(current_node->name, "id" )){
			user->id_str=g_strdup(content);
			user->id=strtoul( content, NULL, 10 );
			debug("User ID: %s(=%lu).", user->id_str, user->id);
			
		}else if(g_str_equal(current_node->name, "name" ))
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
			user->status=user_status_parse(service, current_node->children, Tweets);
		
		xmlFree(content);
		
	}
	
	if(user->status){
		/*TODO: implement cache checking to limit saved statuses or its gonna bloat the hard drive really quick.
		 * cache_save_xml(service, root_element, "users", status->id_str);
		 */
		user_status_format_updates(service, user, user->status);
	}
	
	user->image_filename=cache_images_get_user_filename(service, user->user_name, user->image_url);
	
	return user;
}/*user_parse_node(service, root_node); || user_parse_node(service, current_node->children);*/

void user_free(User *user){
	if(!user) return;
	debug("Destroying user object for %s on <%s>.", user->user_name, online_service_get_key(user->service));
	if(user->status) user_status_free(user->status);
	
	user->service=NULL;
			
	uber_object_free(&user->user_name, &user->nick_name, &user->location, &user->bio, &user->url, &user->image_url, &user->image_filename, &user, NULL);
}/*user_free*/


/********************************************************************************
 *                           UserStatus methods                                 *
 ********************************************************************************/
static UserStatus *user_status_new(OnlineService *service, TweetLists tweet_list){
	UserStatus	*status=g_new0(UserStatus, 1);
	
	status->service=service;
	status->user=NULL;
	status->type=tweet_list;
	status->id_str=status->text=status->tweet=status->notification=status->sexy_tweet=status->created_at_str=status->created_how_long_ago=NULL;
	status->id=status->in_reply_to_status_id=0;
	status->created_at=status->created_seconds_ago=0;
	
	return status;
}/*user_status_new(service, Tweets|Replies|Dms);*/

gulong user_status_get_id(UserStatus *status){
	if(!status) return 0;
	return status->id;
}/*user_status_get_id(status);*/

const gchar *user_status_get_id_str(UserStatus *status){
	if(!(status && status->id_str)) return NULL;
	return status->id_str;
}/*user_status_get_id_str(status);*/

const gchar *user_status_get_user_name(UserStatus *status){
	if(!(status && status->user)) return NULL;
	return user_get_user_name(status->user);
}/*user_status_get_user_name(status);*/

const gchar *user_status_get_notification(UserStatus *status){
	if(!(status && status->notification)) return NULL;
	return status->notification;
}/*user_status_get_notification(status);*/

OnlineService *user_status_get_online_service(UserStatus *status){
	if(!status) return NULL;
	return status->service;
}/*user_status_get_online_service(status);*/

UserStatus *user_status_parse(OnlineService *service, xmlNode *root_element, TweetLists tweet_list){
	xmlNode		*current_node = NULL;
	gchar		*content=NULL;
	UserStatus	*status=user_status_new(service, tweet_list);
	
	/* Begin 'status' or 'direct-messages' loop */
	debug("Parsing status & tweet at node: %s", root_element->name);
	for(current_node=root_element; current_node; current_node=current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE) continue;
		
		if( G_STR_EMPTY( (content=(gchar *)xmlNodeGetContent(current_node)) ) ){
			if(content) g_free(content);
			continue;
		}
		
		if(g_str_equal(current_node->name, "id")){
			status->id_str=g_strdup(content);
			status->id=strtoul(content, NULL, 10);
			debug("Status ID: %s(=%lu).", content, status->id);
			
		}else if(g_str_equal(current_node->name, "in_reply_to_status_id"))
			status->in_reply_to_status_id=strtoul(content, NULL, 10);
		
		else if(g_str_equal(current_node->name, "source"))
			status->source=g_strdup(content);
		
		else if((g_str_equal(current_node->name, "sender") || g_str_equal(current_node->name, "user")) && current_node->children)
			status->user=user_parse_node(service, current_node->children);
		
		else if(g_str_equal(current_node->name, "text"))
			status->text=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "created_at")){
			status->created_at_str=g_strdup(content);
			user_status_format_dates(status);
		}
		
		xmlFree(content);
		
	}
	
	if(status->user){
		/*TODO: implement cache checking to limit saved statuses or its gonna bloat the hard drive really quick.
		 * cache_save_xml(service, root_element, "statuses", status->id_str);
		 */
		user_status_format_updates(service, status->user, status);
	}
	
	return status;
}/*user_status_parse(service, current_node->children, tweet_list);*/

static void user_status_format_dates(UserStatus *status){
	struct tm	post;
	strptime(status->created_at_str, "%s", &post);
	post.tm_isdst=-1;
	status->created_at=mktime(&post);
	
	strptime(status->created_at_str, "%s", &post);
	post.tm_isdst=-1;
	status->created_at=mktime(&post);									
	
	debug("Parsing tweet's 'created_at' date: [%s] to Unix seconds since: %lu", status->created_at_str, status->created_at);
	status->created_how_long_ago=parser_convert_time(status->created_at_str, &status->created_seconds_ago);
	debug("Display time set to: %s, %lu.", status->created_how_long_ago, status->created_seconds_ago);
}/*user_status_format_dates*/


static void user_status_format_updates(OnlineService *service, User *user, UserStatus *status){
	if(!(online_service_is_connected(service) && G_STR_N_EMPTY(status->text) && G_STR_N_EMPTY(user->user_name) && G_STR_N_EMPTY(user->nick_name))) return;
	debug("Formating status text for display.");
	
	gchar *sexy_status_text=NULL, *sexy_status_swap=parser_escape_text(status->text);
	if(!gconfig_if_bool(PREFS_URLS_EXPAND_SELECTED_ONLY, TRUE)){
		status->sexy_tweet=label_msg_format_urls(service, sexy_status_swap, TRUE, TRUE);
		sexy_status_text=label_msg_format_urls(service, sexy_status_swap, TRUE, FALSE);
	}else{
		status->sexy_tweet=g_strdup(sexy_status_swap);
		sexy_status_text=label_msg_format_urls(service, sexy_status_swap, FALSE, FALSE);
	}
	uber_free(sexy_status_swap);

	status->from=g_strdup_printf("<small><b>%s\n&lt;%s@%s&gt;</b></small>", user->nick_name, user->user_name, online_service_get_uri(service));
	
	status->rcpt=g_strdup_printf("<span size=\"small\" weight=\"light\">&lt;%s&gt;</span>", online_service_get_key(service));
	
	status->tweet=g_strdup_printf("%s%s%s%s",
					( (status->type==DMs)
					  	?"<span weight=\"ultrabold\" style=\"italic\" variant=\"smallcaps\">[Direct Message]</span>\n"
						:(status->type==Replies ?"<span style=\"italic\" variant=\"smallcaps\">[@ reply]</span>\n"
							:""
						)
					),
					((status->type==DMs) ?"<span weight=\"ultrabold\" style=\"italic\">" :""), sexy_status_text, ((status->type==DMs) ?"]</span>" :"")
	);
	
	status->notification=g_strdup_printf(
						"%s<i>[%s]</i>\n\t<u><b>From:</b></u> <b>%s &lt;@%s on %s&gt;</b>\n<i><u>To:</u></i> <i>&lt;%s&gt;</i>\n<b>\t%s%s%s</b>",
						((status->type==DMs) ?"<b><i><u>[Direct Message]</u></i></b>\n" :((status->type==Replies) ?"<i><u>[@ Reply]</u</i>\n" :"")),
						status->created_how_long_ago,
						user->nick_name, user->user_name, online_service_get_uri(service),
						online_service_get_key(service),
						((status->type==DMs) ?"<b><i>[" :((status->type==Replies) ?"<i>[" :"")), sexy_status_text, ((status->type==DMs) ?"]</b></i>" :((status->type==Replies) ?"]</i>" :""))
	);
	
	g_free(sexy_status_text);
}/*user_status_format_updates(service, status, user);*/

void user_status_store(UserStatus *status, TweetList *tweet_list){
	GtkListStore	*tweet_list_store=tweet_list_get_list_store(tweet_list);
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	
	debug("Appending tweet to TweetList\n\t\t\tTo: <%s> From: <%s@%s>\n\t\t\tTweet ID: %lu; posted on [%s]\n\t\t\tStatus update: %s\n\t\t\tFormatted Tweet: %s", online_service_get_key(status->service), status->user->user_name, online_service_get_uri(status->service), status->id, status->created_at_str, status->text, status->sexy_tweet);
	
	gtk_list_store_append(tweet_list_store, iter);
	gtk_list_store_set(
				tweet_list_store, iter,
					ULONG_TWEET_ID, status->id,				/*Tweet's ID.*/
					ULONG_USER_ID, status->user->id,			/*User's ID.*/
					STRING_USER, status->user->user_name,			/*Username string.*/
					STRING_NICK, status->user->nick_name,			/*Author name string.*/
					STRING_TEXT, status->text,				/*Tweet string.*/
					STRING_TWEET, status->tweet,				/*Display string.*/
					STRING_SEXY_TWEET, status->sexy_tweet,			/*SexyTreeView's tooltip.*/
					STRING_CREATED_AGO, status->created_how_long_ago,	/*(seconds|minutes|hours|day) ago.*/
					STRING_CREATED_AT, status->created_at_str,		/*Date string.*/
					ULONG_CREATED_AGO, status->created_seconds_ago,		/*How old the post is, in seconds, for sorting.*/
					ULONG_CREATED_AT, status->created_at,			/*Seconds since the post was posted.*/
					ONLINE_SERVICE, status->service,			/*OnlineService pointer.*/
					STRING_FROM, status->from,
					STRING_RCPT, status->rcpt,
				-1
	);
	
	tweet_list_increment(tweet_list);
	
	/* network_get_image, or its callback network_cb_on_image, free's iter once its no longer needed.*/
	if(!g_file_test(status->user->image_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR))
		return network_get_image(status->service, tweet_list, status->user->image_filename, status->user->image_url, iter);
	
	tweet_list_set_image(tweet_list, status->user->image_filename, iter);
	uber_free(iter);
}/*void user_status_store(status);*/

void user_status_free(UserStatus *status){
	if(!status) return;
	debug("Destroying status object %lu, from <%s>.", status->id, online_service_get_key(status->service));
	if(status->user) user_free(status->user);
			
	status->service=NULL;
	
	uber_object_free(&status->text, &status->id_str, &status->from, &status->rcpt, &status->tweet, &status->source, &status->sexy_tweet, &status->notification, &status->created_at_str, &status->created_how_long_ago, &status, NULL);
}/*user_status_free*/



/********************************************************************************
 *                       UserProfileViewer methods                              *
 ********************************************************************************/
static void user_profile_viewer_setup(GtkWindow *parent){
	user_profile_viewer=g_new( UserProfileViewer, 1 );
	
	GtkBuilder *ui=gtkbuilder_get_file(
						GtkBuilderUI,
							"profile_dialog", &user_profile_viewer->dialog,
							
							"loading_label", &user_profile_viewer->loading_label,
							
							"title_vbox", &user_profile_viewer->title_vbox,
							
							"profile_vbox", &user_profile_viewer->profile_vbox,
							
							"latest_update_hbox", &user_profile_viewer->latest_update_hbox,
							"updated_when_label", &user_profile_viewer->updated_when_label,
							
							"latest_update_vbox", &user_profile_viewer->latest_update_vbox,
						NULL
	);
	
	gtkbuilder_connect(
				ui, user_profile_viewer,
					"profile_dialog", "destroy", user_profile_viewer_destroy,
					"profile_dialog", "response", user_profile_viewer_response,
				NULL
	);
	
	g_object_add_weak_pointer(G_OBJECT(user_profile_viewer->dialog), (gpointer)&user_profile_viewer);
	gtk_window_set_transient_for(GTK_WINDOW(user_profile_viewer->dialog), parent);
	gtk_widget_show( GTK_WIDGET( user_profile_viewer->loading_label ) );
	gtk_window_present(GTK_WINDOW(user_profile_viewer->dialog));
}/*user_profile_viewer_setup(parent);*/

gboolean user_download_avatar(User *user){
	gchar *image_filename=NULL;
	if(g_file_test(user->image_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR))
		return TRUE;
	
	debug("Downloading Image: %s.  GET: %s", image_filename, user->image_url);
	
	SoupMessage *msg=online_service_request_uri(user->service, GET, user->image_url, NULL, NULL, NULL, NULL);
	
	debug("Image response: %i", msg->status_code);
	
	if(!( (network_check_http(user->service, msg)) && (g_file_set_contents(user->image_filename, msg->response_body->data, msg->response_body->length, NULL)) ))
		return FALSE;
	
	return TRUE;
}/*user_download_avatar(user);*/

void user_profile_viewer_show(OnlineService *service, const gchar *user_name, GtkWindow *parent){
	if(!(user_profile_viewer)) user_profile_viewer_setup(parent);
	gchar	*profile_details;
	
	gtk_widget_show_all( GTK_WIDGET( user_profile_viewer->dialog ) );
	
	user_profile_viewer->user=user_fetch_profile(service, user_name);
	
	user_download_avatar(user_profile_viewer->user);
	
	if(!(g_str_equal("unknown_image", user_profile_viewer->user->image_filename))){
		gtk_message_dialog_set_image( user_profile_viewer->dialog, GTK_WIDGET(user_profile_viewer->image=images_get_maximized_image_from_filename( user_profile_viewer->user->image_filename )) );
	}
	
	profile_details=g_strdup_printf(
					"\t<u><b>%s:</b> @%s</u>\n",
					user_profile_viewer->user->nick_name, user_profile_viewer->user->user_name
	);
	
	
	user_profile_viewer->user_label=label_new();
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->user_label));
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->title_vbox),
				GTK_WIDGET(user_profile_viewer->user_label),
				TRUE, TRUE, 0
	);
	label_set_text(service, LABEL(user_profile_viewer->user_label), profile_details, TRUE, TRUE);
	g_free( profile_details );
	
	profile_details=g_strdup_printf(
					"\t<b>Tweets:</b> %lu\n\t<b>Following:</b> %lu\n\t<b>Followers:</b> %lu\n\t<b>Location:</b> %s\n",
					user_profile_viewer->user->tweets,
					user_profile_viewer->user->following,
					user_profile_viewer->user->followers,
					user_profile_viewer->user->location
	);
	gtk_message_dialog_set_markup( user_profile_viewer->dialog, profile_details );
	g_free(profile_details);
	
	profile_details=g_strdup_printf( "\t<b>URL:</b>\t<a href=\"%s\">%s</a>\n", user_profile_viewer->user->url, user_profile_viewer->user->url );
	user_profile_viewer->url_hyperlink=label_new();
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->url_hyperlink));
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->profile_vbox),
				GTK_WIDGET(user_profile_viewer->url_hyperlink),
				TRUE, TRUE, 0
	);
	label_set_text(service, user_profile_viewer->url_hyperlink, profile_details, TRUE, TRUE);
	g_free( profile_details );
	
	profile_details=g_strdup_printf( "\t<b>Bio:</b>\n\t\t%s\n", user_profile_viewer->user->bio );
	user_profile_viewer->bio_html=label_new();
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->bio_html));
	gtk_box_pack_end(
				GTK_BOX(user_profile_viewer->profile_vbox),
				GTK_WIDGET(user_profile_viewer->bio_html),
				TRUE, TRUE, 0
	);
	label_set_text(service, user_profile_viewer->bio_html, profile_details, TRUE, TRUE);
	g_free( profile_details );
	
	profile_details=g_markup_printf_escaped("<b>Last updated:</b> <i>[%s]</i>", user_profile_viewer->user->status->created_how_long_ago);
	gtk_label_set_markup(user_profile_viewer->updated_when_label, profile_details);
	uber_free(profile_details);
	
	user_profile_viewer->latest_tweet=label_new();
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->latest_tweet));
	gtk_box_pack_end(
			GTK_BOX(user_profile_viewer->latest_update_vbox),
			GTK_WIDGET(user_profile_viewer->latest_tweet),
			TRUE, TRUE, 0
	);
	label_set_text(service, user_profile_viewer->latest_tweet, user_profile_viewer->user->status->sexy_tweet, TRUE, TRUE);
	
	
	gtk_widget_show_all( GTK_WIDGET( user_profile_viewer->dialog ) );
	
	gtk_widget_hide( GTK_WIDGET( user_profile_viewer->loading_label ) );
	
	user_free( user_profile_viewer->user );
	
}/*user_popup_profile*/

static void user_profile_viewer_response(GtkDialog *dialog, gint response, UserProfileViewer *user_profile_viewer){
	gtk_widget_destroy(GTK_WIDGET(dialog));
}/*user_profile_viewer_response(dialog, response, user_profile_viewer);*/

static void user_profile_viewer_destroy(GtkDialog *dialog){
	uber_free(user_profile_viewer);
}/*user_profile_viewer_response(dialog, user_profile_viewer);*/


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

