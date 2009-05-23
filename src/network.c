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
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <libsoup/soup.h>

#include "gconfig.h"

#include "main.h"
#include "network.h"
#include "parser.h"
#include "users.h"
#include "cache.h"
#include "app.h"
#include "preferences.h"
#include "following-viewer.h"
#include "tweet-view.h"
#include "tweet-list.h"
#include "tweets.h"
#include "timer.h"
#include "online-services.h"
#include "popup-dialog.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define	DEBUG_DOMAINS	"Networking:OnlineServices:Tweets:Requests:Users:Images:Authentication:Setup:Start-Up"
#include "debug.h"

typedef struct {
	gchar		*filename;
	GtkTreeIter	*iter;
} NetworkTweetListImageDL;

gboolean getting_followers=FALSE;
static gchar *current_timeline=NULL;
static gboolean processing=FALSE;
static gboolean fetching_users=FALSE;
static guint timeout_id;

/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static NetworkTweetListImageDL *network_tweet_list_image_dl_new(gchar *filename, GtkTreeIter *iter);
static void network_tweet_list_image_dl_free(NetworkTweetListImageDL *image);

static gboolean network_timeout(gpointer user_data);
static void network_timeout_new(void);

static void network_users_glist_save(SoupSession *session, SoupMessage *msg, gpointer user_data);

static void network_tweet_cb(SoupSession *session, SoupMessage *msg, gpointer user_data);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
static NetworkTweetListImageDL *network_tweet_list_image_dl_new(gchar *filename, GtkTreeIter *iter){
	NetworkTweetListImageDL *image=g_new0(NetworkTweetListImageDL, 1);
	image->filename=g_strdup(filename);
	image->iter=iter;
	return image;
}/*network_tweet_list_image_dl_new*/
			
static void network_tweet_list_image_dl_free(NetworkTweetListImageDL *image){
	if(image->filename) g_free(image->filename);
	if(image->iter) g_free(image->iter);
	g_free(image);
}/*network_image_free*/

/* Check HTTP response code */
gboolean network_check_http(OnlineService *service, SoupMessage *msg){
	if(!( SOUP_IS_MESSAGE(msg) )){
		debug("**ERROR**: Attempting validate invalid SoupMessage.\n\t\tService '%s' responsed: %s (%d).", service->decoded_key, msg->reason_phrase, msg->status_code);
		return FALSE;
	}
	
	const gchar *error=NULL;
	if(msg->status_code == 401)
		error=_("Access denied");
	else if(msg->status_code == 404)
		error=_("Resource cannot be found");
	else if(SOUP_STATUS_IS_CLIENT_ERROR(msg->status_code))
		error=_("HTTP communication error");
	else if(SOUP_STATUS_IS_SERVER_ERROR(msg->status_code))
		error=_("Internal server error");
	else if(!SOUP_STATUS_IS_SUCCESSFUL(msg->status_code))
		error=_("Stopped");
	else if(msg->status_code >= 400)
		error=_("Unkown error");
	else{
		debug("Loading succeed.  Service '%s' responsed: %s (%d).", service->decoded_key, msg->reason_phrase, msg->status_code);
		app_statusbar_printf("Loading succeed.  Service '%s' responsed: %s (%d).", service->decoded_key, msg->reason_phrase, msg->status_code);
		timer_main(service->timer, msg);
		return TRUE;
	}
	
	debug("Loading failed.  Service '%s' responsed: %s: %s (%d).", service->decoded_key, error, msg->reason_phrase, msg->status_code);
	app_statusbar_printf("Loading failed.  Service '%s' responsed: %s: %s (%d).", service->decoded_key, error, msg->reason_phrase, msg->status_code);
	
	return FALSE;
}

/* Logout current user */
void network_logout(void){
	if(current_timeline){
		g_free(current_timeline);
		current_timeline=NULL;
	}
	
	debug("Logout");
}


/* Post a new tweet - text must be Url encoded */
void network_post_status(const gchar *text){
	if(!( in_reply_to_service && gconfig_if_bool(PREFS_TWEETS_DIRECT_REPLY_ONLY, FALSE)))
		online_services_request(online_services, POST, API_POST_STATUS, network_tweet_cb, "Tweet", (gchar *)text);
	else
		online_service_request(in_reply_to_service, POST, API_POST_STATUS, network_tweet_cb, "Tweet", (gchar *)text);
}/*network_post_status*/


/* Send a direct message to a follower - text must be Url encoded  */
void network_send_message(OnlineService *service, const gchar *friend, const gchar *text){
	gchar *formdata=g_strdup_printf("source=%s&user=%s&text=%s", (g_str_equal("twitter.com", service->uri) ?API_CLIENT_AUTH :OPEN_CLIENT ), friend, text);
	online_service_request(service, POST, API_SEND_MESSAGE, network_tweet_cb, "DM", formdata);
	g_free(formdata);
}

void network_set_state_loading_timeline(const gchar *timeline, ReloadState state){
	const gchar *notice_prefix=NULL;
	switch(state){
		case Timeout:
			notice_prefix=_("Auto-Reloading");
			break;
		
		case Reload:
			notice_prefix=_("Reloading");
			break;
		
		case Load:
		default:
			notice_prefix=_("Loading");
			break;
	}
	debug("%s current timeline: %s", notice_prefix, timeline);
	app_statusbar_printf("%s: %s %s.%s", notice_prefix, _("timeline"),  timeline, ( ( gconfig_if_bool(PREFS_URLS_EXPAND_DISABLED, FALSE) || gconfig_if_bool(PREFS_URLS_EXPAND_SELECTED_ONLY, TRUE) ) ?"" :_("  This may take several moments.") ));
	
	processing=TRUE;
}/*network_timeline_loading_notification*/


static void network_timeout_new(void){
	gint minutes;
	guint reload_time;
	
	if(timeout_id) {
		debug("Stopping timeout id: %i", timeout_id);
		g_source_remove(timeout_id);
	}
	
	gconfig_get_int(PREFS_TWEETS_RELOAD_TIMELINES, &minutes);
	
	/* The timeline reload interval shouldn't be less than 3 minutes */
	if(minutes < 3)
		minutes=3;
	
	/* This should be the number of milliseconds */
	reload_time=minutes*60*1000;
	
	timeout_id=g_timeout_add(reload_time, network_timeout, NULL);
	
	debug("Starting timeout id: %i", timeout_id);
}

static gboolean network_timeout(gpointer user_data){
	if(!current_timeline || processing)
		return FALSE;
	
	network_set_state_loading_timeline(current_timeline, Timeout);
	online_services_request( online_services, QUEUE, current_timeline, network_display_timeline, current_timeline, NULL );
	timeout_id=0;
	return FALSE;
}

void network_refresh(void){
	if(!current_timeline || processing)
		return;
	
	network_set_state_loading_timeline(current_timeline, Reload);
	online_services_request( online_services, QUEUE, current_timeline, network_display_timeline, current_timeline, NULL );
}

/* Get and parse a timeline */
void network_get_timeline(const gchar *uri_timeline){
	if(processing)
		return;
		
	parser_reset_lastid();
	
	/* UI */
	network_set_state_loading_timeline(uri_timeline, Load);
	
	online_services_request( online_services, QUEUE, uri_timeline, network_display_timeline, (gchar *)uri_timeline, NULL );
	/* network_queue's 3rd argument is used to trigger a new timeline & enables 'Refresh' */
}/*network_get_timeline*/

/* Get a user timeline */
void network_get_user_timeline(OnlineService *service, const gchar *username){
	if(!service){
		popup_select_service(app_get_window());
		service=selected_service;
	}
	
	gchar *user_id=NULL;
	
	if(!username)
		user_id=g_strdup(service->username);
	else
		user_id=g_strdup(username);
		
	if(G_STR_EMPTY(user_id)){
		if(user_id) g_free(user_id);
		debug("*WARNING:* Attempting to loading timeline for an unknow user.");
		return;
	}
	
	gchar *user_timeline=g_strdup_printf(API_TIMELINE_USER, user_id);
	
	network_set_state_loading_timeline(user_timeline, Load);

	online_service_request(service, QUEUE, user_timeline, network_display_timeline, user_timeline, NULL);
	g_free(user_timeline);
	g_free(user_id);
}/*network_get_user_timeline*/


GList *network_users_glist_get(gboolean get_friends, gboolean refresh){
	static OnlineService *service=NULL;
	
	if(!selected_service)
		return NULL;
	
	if( !refresh && service && service==selected_service ){
		if(get_friends && service->friends)
			return service->friends;
		else if( !get_friends && service->followers )
			return service->followers;
	}
	
	if(fetching_users){
		return NULL;
	}
	
	fetching_users=TRUE;
	service=selected_service;
	gint page=0; 
	
	gchar *uri;
	getting_followers=!get_friends;
	
	app_statusbar_printf("Please wait while %s downloads the users %s...", GETTEXT_PACKAGE, (get_friends ?"who are following you" :"you're following") );
	
	gchar *getting_message=NULL;
	if(get_friends)
		getting_message=_("friends");
	else
		getting_message=_("followers");
	
	while(fetching_users){
		page++;
		uri=g_strdup_printf("%s?page=%d", (get_friends ?API_FOLLOWING :API_FOLLOWERS), page);
		debug("Getting page %d of who%s.", page, (get_friends ?"m the user is following" :" is following the user") );
		/*
		online_service_request(service, QUEUE, uri, network_users_glist_save, (gpointer)getting_message, NULL );
		*/
		SoupMessage *msg=online_service_request(service, GET, uri, network_users_glist_save, getting_message, NULL );
		OnlineServiceCBWrapper *request_wrapper=request_wrapper=online_service_wrapper_new(service, network_users_glist_save, getting_message, NULL);
		network_users_glist_save(service->session, msg, request_wrapper);
		g_free(uri);
	}
	
	if(get_friends){
		service->friends=g_list_sort(service->friends, (GCompareFunc)usrcasecmp);
		return service->friends;
	}else{
		service->followers=g_list_sort(service->followers, (GCompareFunc)usrcasecmp);
		return service->followers;
	}
}/*network_users_glist_get*/


static void network_users_glist_save(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceCBWrapper *service_wrapper=(OnlineServiceCBWrapper *)user_data;
	gchar *getting=(gchar *)service_wrapper->user_data;
	debug("Users response: %i",msg->status_code);
	
	/* Check response */
	if(!network_check_http(service_wrapper->service, msg)){
		fetching_users=FALSE;
		return;
	}
	
	/* parse user list */
	debug("Parsing user list");
	GList *new_users;
	if(!(new_users=users_new(service_wrapper->service, msg)) ){
		fetching_users=FALSE;
		return;
	}
	
	if(g_str_equal(getting, "friends")){
		if(!service_wrapper->service->friends)
			service_wrapper->service->friends=new_users;
		else
			service_wrapper->service->friends=g_list_concat(service_wrapper->service->friends, new_users);
	}else{
		if(!service_wrapper->service->followers)
			service_wrapper->service->followers=new_users;
		else
			service_wrapper->service->followers=g_list_concat(service_wrapper->service->followers, new_users);
	}
}/*network_users_glist_save*/


/* Get an image from servers */
gboolean network_download_avatar(User *user){
	gchar *image_filename=NULL;
	if(g_file_test(user->image_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR))
		return TRUE;
	
	debug("Downloading Image: %s\n\t\tGET: %s", image_filename, user->image_url);
	
	SoupMessage *msg=online_service_request_uri(user->service, GET, user->image_url, NULL, NULL, NULL);
	
	debug("Image response: %i", msg->status_code);
	
	/* check response */
	if(!( (network_check_http(user->service, msg)) && (g_file_set_contents(user->image_filename, msg->response_body->data, msg->response_body->length, NULL)) ))
		return FALSE;
	
	return TRUE;
}


void network_get_image(User *user, GtkTreeIter *iter){
	if(g_file_test(user->image_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)){
		/* Set image from file here */
		tweet_list_set_image(user->image_filename, iter);
		g_free(iter);
		return;
	}
	
	debug("Downloading Image: %s\n\t\tGET: %s", user->image_filename, user->image_url);
	NetworkTweetListImageDL *image=network_tweet_list_image_dl_new(user->image_filename, iter);
	
	online_service_request_uri(user->service, QUEUE, user->image_url, network_cb_on_image, image, NULL);
}/*network_get_image*/


/* On send a direct message */
static void network_tweet_cb(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceCBWrapper *service_wrapper=(OnlineServiceCBWrapper *)user_data;
	gchar *status=(gchar *)service_wrapper->user_data;
	
	if(!network_check_http(service_wrapper->service, msg)){
		gchar *status_error=g_utf8_strup(status, -1);
		debug("**%s-ERROR:** send failed for: '%s'.", status_error, service_wrapper->service->key);
		app_statusbar_printf("%s's %s was not sent.", service_wrapper->service->key, status);
	}else{
		debug("%s sent successfully to: '%s'.", status, service_wrapper->service->key);
		app_statusbar_printf("%s's %s sent successfully.", service_wrapper->service->key, status);
	}
	
	debug("\t\tHTTP response: %s(#%i)", msg->reason_phrase, msg->status_code);
	
	if(in_reply_to_service){
		if(service_wrapper->service==in_reply_to_service){
			in_reply_to_service=NULL;
			in_reply_to_status_id=0;
			if(msg->status_code==404){
				debug("Resubmitting Tweet/Status update to: [%s] due to Laconica bug.", service_wrapper->service->decoded_key);
				online_service_request(service_wrapper->service, POST, API_POST_STATUS, network_tweet_cb, "Tweet", (gchar *)service_wrapper->formdata);
			}
		}
	}
	
	online_service_wrapper_free(service_wrapper);
}/*network_tweet_cb*/


/* On get a timeline */
void network_display_timeline(SoupSession *session, SoupMessage *msg, gpointer user_data){
	static gboolean retrying=FALSE;
	OnlineServiceCBWrapper *service_wrapper=(OnlineServiceCBWrapper *)user_data;
	gchar        *new_timeline=NULL;
	
	new_timeline=(gchar *)service_wrapper->user_data;
	
	debug("Timeline response: %i",msg->status_code);
	
	if(processing)
		tweet_list_refresh();
	
	processing=FALSE;
	
	network_timeout_new();
	
	if(!network_check_http(service_wrapper->service, msg)) {
		if(!retrying && (msg->status_code==100||msg->status_code==404)){
			retrying=TRUE;
			online_service_request(service_wrapper->service, QUEUE, new_timeline, network_display_timeline, new_timeline, NULL);
		}
		online_service_wrapper_free(service_wrapper);
		return;
	}
	if(retrying)
		retrying=FALSE;
	
	debug("Parsing timeline");
	if(!(parser_timeline(service_wrapper->service, msg)))
		app_statusbar_printf("Error Parsing %s's Timeline Parser.", service_wrapper->service->decoded_key);
	else {
		app_statusbar_printf("Timeline Loaded for: %s", service_wrapper->service->decoded_key);
		
		if(new_timeline){
			if(!( current_timeline && g_str_equal(new_timeline, current_timeline) )){
				if(current_timeline) g_free(current_timeline);
				current_timeline=g_strdup(new_timeline);
			}
		}
	}
	
	/* move the pseudo focus of the tweet list to the start of the list */
	tweet_list_goto_top();
	
	online_service_wrapper_free(service_wrapper);
}

/* On get a image */
void network_cb_on_image(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceCBWrapper *service_wrapper=(OnlineServiceCBWrapper *)user_data;
	
	debug("Image response: %i", msg->status_code);
	
	NetworkTweetListImageDL *image=(NetworkTweetListImageDL *)service_wrapper->user_data;
	if(!( image && image->filename && image->iter )){
		debug("**ERROR**: Missing image information.  Image filename: %s; Image iter: %s", image->filename, (image->iter ?"valid" :"unknown") );
		return;
	}
	
	/* check response */
	gchar *image_filename=NULL;
	if(!(network_check_http(service_wrapper->service, msg)))
		image_filename=cache_images_get_unknown_image_filename();
	else{
		/* Save image data */
		debug("Saving avatar to file: %s", image->filename);
		if(!(g_file_set_contents(
					image->filename,
						msg->response_body->data,
						msg->response_body->length,
					NULL
		)))
			image_filename=cache_images_get_unknown_image_filename();
		else
			image_filename=g_strdup(image->filename);
	}
	
	/* Set image from file here(image_file) */
	tweet_list_set_image(image_filename, image->iter);
	
	g_free(image_filename);
	
	network_tweet_list_image_dl_free(image);
	
	g_free(service_wrapper);
	service_wrapper=NULL;
}


/********************************************************
 *                       eof                            *
 ********************************************************/
