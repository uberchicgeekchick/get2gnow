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
#include "program.h"

#include "online-services.h"
#include "online-service.h"
#include "online-service-wrapper.h"
#include "network.h"

#include "parser.h"
#include "cache.h"
#include "app.h"
#include "preferences.h"

#include "users.h"
#include "friends-manager.h"
#include "following-viewer.h"
#include "tweet-view.h"

#include "tweet-list.h"
#include "tweets.h"
#include "timer.h"
#include "popup-dialog.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define	DEBUG_DOMAINS	"Networking:OnlineServices:Tweets:Requests:Users:Images:Authentication:Refreshing:Setup:Start-Up"
#include "debug.h"

typedef struct {
	gchar		*filename;
	GtkTreeIter	*iter;
} NetworkTweetListImageDL;

static gchar *current_timeline=NULL;

static gboolean processing=FALSE;
static gboolean retrying=FALSE;

static guint timeout_id_timelines=0, timeout_id_replies=0, timeout_id_dms=0;

/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static gboolean network_timeout(gpointer user_data);

static NetworkTweetListImageDL *network_tweet_list_image_dl_new(gchar *filename, GtkTreeIter *iter);
static void network_tweet_list_image_dl_free(NetworkTweetListImageDL *image);

static void network_tweet_cb(SoupSession *session, SoupMessage *msg, gpointer user_data);

static void network_retry(OnlineServiceWrapper *service_wrapper, StatusMonitor monitoring);

static void network_update_timeline_display(OnlineService *service, guint tweets_parsed, const gchar *new_timeline);

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
		debug("**ERROR**: Attempting validate invalid SoupMessage.");
		return FALSE;
	}
	
	const gchar *error=NULL;
	if(msg->status_code == 1)
		error=_("Log-in failed");
	else if(msg->status_code == 401)
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

void network_init(void){
	gint minutes;
	
	gconfig_get_int(PREFS_TWEETS_RELOAD_TIMELINES, &minutes);
	
	/* The timeline reload interval shouldn't be less than 3 minutes */
	if(minutes < 3) minutes=3;
	
	if( (!timeout_id_dms && gconfig_if_bool(PREFS_NOTIFY_DMS, TRUE)) && (current_timeline && !g_str_equal(API_DIRECT_MESSAGES, current_timeline)) ){
		debug("Creating timeout to monitor for new Direct Messages.");
		guint reload_dms=minutes*75*1000;
		timeout_id_dms=g_timeout_add(reload_dms, network_timeout, (gpointer)DMs);
	}
	
	if( (!timeout_id_replies && gconfig_if_bool(PREFS_NOTIFY_REPLIES, TRUE)) && (current_timeline && !g_str_equal(API_REPLIES, current_timeline)) ){
		debug("Creating timeout to monitor for new Replies.");
		guint reload_replies=minutes*40*1000;
		timeout_id_replies=g_timeout_add(reload_replies, network_timeout, (gpointer)Replies);
	}
	
	if(current_timeline && !timeout_id_timelines){
		if(g_str_equal(API_REPLIES, current_timeline))
			network_deinit(FALSE, Replies);
		else if(g_str_equal(API_DIRECT_MESSAGES, current_timeline))
			network_deinit(FALSE, DMs);
		
		debug("Creating timeout to monitor current timeline: %s.", current_timeline);
		guint reload_timelines=minutes*60*1000;
		timeout_id_timelines=g_timeout_add(reload_timelines, network_timeout, (gpointer)Tweets);
	}
}/*network_init();*/

static gboolean network_timeout(gpointer user_data){
	StatusMonitor monitoring=(StatusMonitor)user_data;
	
	switch(monitoring){
		case DMs:
			debug("Reloading DMs.");
			online_services_request( online_services, QUEUE, API_DIRECT_MESSAGES, network_display_timeline, current_timeline, (gpointer)DMs );
			timeout_id_dms=0;
			break;
		case Replies:
			debug("Reloading Replies.");
			online_services_request( online_services, QUEUE, API_REPLIES, network_display_timeline, current_timeline, (gpointer)Replies );
			timeout_id_replies=0;
			break;
		case Tweets:
			debug("Reloading Tweets.");
			network_set_state_loading_timeline(current_timeline, Reload);
			online_services_request( online_services, QUEUE, current_timeline, network_display_timeline, current_timeline, (gpointer)Tweets );
			timeout_id_timelines=0;
			break;
		case All: break;
	}
	return FALSE;
}/*network_timeout(gpointer user_data);*/

/* Logout current user */
void network_deinit(gboolean free_timeline, StatusMonitor monitor){
	if(current_timeline && free_timeline) uber_free(current_timeline);
	
	switch(monitor){
		case All:
		case DMs:
		default:
			program_timeout_remove(&timeout_id_dms, _("DM"));
			if(monitor!=All) break;
		case Replies:
			program_timeout_remove(&timeout_id_replies, _("reply"));
			if(monitor!=All) break;
		case Tweets:
			program_timeout_remove(&timeout_id_timelines, _("timeline"));
			break;
	}
}/*network_deinit(TRUE||FALSE, All||DMs||Replies||Tweets);*/

/* Post a new tweet - text must be Url encoded */
void network_post_status(const gchar *update){
	if(G_STR_EMPTY(update)) return;
	
	if(!( in_reply_to_service && gconfig_if_bool(PREFS_TWEETS_DIRECT_REPLY_ONLY, TRUE)))
		online_services_request(online_services, POST, API_POST_STATUS, network_tweet_cb, "Tweet", (gchar *)update);
	else
		online_service_request(in_reply_to_service, POST, API_POST_STATUS, network_tweet_cb, "Tweet", (gchar *)update);
}/*network_post_status(tweet);*/

/* Send a direct message to a follower - text must be Url encoded  */
void network_send_message(OnlineService *service, const gchar *friend, const gchar *dm){
	gchar *form_data=g_strdup_printf("source=%s&user=%s&text=%s", (g_str_equal("twitter.com", service->uri) ?SOURCE_TWITTER :SOURCE_LACONICA ), friend, dm);
	online_service_request(service, POST, API_SEND_MESSAGE, network_tweet_cb, "DM", form_data);
	g_free(form_data);
}/*network_send_message(service, friend, dm);*/

void network_set_state_loading_timeline(const gchar *timeline, ReloadState state){
	const gchar *notice_prefix=NULL;
	switch(state){
		case Retry:
			retrying=TRUE;
		case Reload:
			notice_prefix=_("Reloading");
			break;
		
		case Load:
		default:
			if(!( current_timeline && g_str_equal(timeline, current_timeline) )){
				debug("Switching current timeline.  Previous timeline: %s; New timeline: %s.", (current_timeline ?current_timeline :"N/A"), timeline);
				
				network_deinit(FALSE, Tweets);
				
				if(current_timeline) uber_free(current_timeline);
				current_timeline=g_strdup(timeline);
			}
			
			notice_prefix=_("Loading");
			break;
	}
	debug("%s current timeline: %s", notice_prefix, timeline);
	app_statusbar_printf("%s: %s %s.%s", notice_prefix, _("timeline"),  timeline, ( ( gconfig_if_bool(PREFS_URLS_EXPAND_DISABLED, FALSE) || gconfig_if_bool(PREFS_URLS_EXPAND_SELECTED_ONLY, TRUE) ) ?"" :_("  This may take several moments.") ));
	
	if(state==Retry) return;
	debug("Setting processing to true to trigger timeline refresh.");
	processing=TRUE;
}/*network_timeline_loading_notification*/


void network_refresh(void){
	if(!current_timeline || processing)
		return;
	
	network_set_state_loading_timeline(current_timeline, Reload);
	online_services_request( online_services, QUEUE, current_timeline, network_display_timeline, current_timeline, (gpointer)Tweets );
}

/* Get and parse a timeline */
void network_get_timeline(const gchar *uri_timeline){
	if(processing)
		return;
		
	/* UI */
	network_set_state_loading_timeline(uri_timeline, Load);
	
	online_services_request( online_services, QUEUE, uri_timeline, network_display_timeline, (gchar *)uri_timeline, (gpointer)Tweets );
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

	online_service_request(service, QUEUE, user_timeline, network_display_timeline, user_timeline, (gpointer)Tweets);
	g_free(user_timeline);
	g_free(user_id);
}/*network_get_user_timeline*/

/* Get an image from servers */
gboolean network_download_avatar(User *user){
	gchar *image_filename=NULL;
	if(g_file_test(user->image_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR))
		return TRUE;
	
	debug("Downloading Image: %s.  GET: %s", image_filename, user->image_url);
	
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
	
	debug("Downloading Image: %s.  GET: %s", user->image_filename, user->image_url);
	NetworkTweetListImageDL *image=network_tweet_list_image_dl_new(user->image_filename, iter);
	
	online_service_request_uri(user->service, QUEUE, user->image_url, network_cb_on_image, image, NULL);
}/*network_get_image*/


/* On send a direct message */
static void network_tweet_cb(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceWrapper *service_wrapper=(OnlineServiceWrapper *)user_data;
	gchar *status=(gchar *)service_wrapper->user_data;
	
	if(!network_check_http(service_wrapper->service, msg)){
		gchar *status_error=g_utf8_strup(status, -1);
		debug("**%s-ERROR:** send failed for: '%s'.", status_error, service_wrapper->service->decoded_key);
		app_statusbar_printf("%s's %s was not sent.", service_wrapper->service->decoded_key, status);
		uber_free(status_error);
	}else{
		debug("%s sent successfully to: '%s'.", status, service_wrapper->service->decoded_key);
		app_statusbar_printf("%s's %s sent successfully.", service_wrapper->service->decoded_key, status);
	}
	
	debug("HTTP response: %s(#%i)", msg->reason_phrase, msg->status_code);
	
	if(in_reply_to_service){
		if(service_wrapper->service==in_reply_to_service){
			in_reply_to_service=NULL;
			in_reply_to_status_id=0;
			if(msg->status_code==404){
				debug("Resubmitting Tweet/Status update to: [%s] due to Laconica bug.", service_wrapper->service->decoded_key);
				online_service_request(service_wrapper->service, POST, API_POST_STATUS, network_tweet_cb, "Tweet", (gchar *)service_wrapper->form_data);
			}
		}
	}
	
	online_service_wrapper_free(service_wrapper);
}/*network_tweet_cb*/

/* On get a timeline */
void network_display_timeline(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceWrapper *service_wrapper=(OnlineServiceWrapper *)user_data;
	gchar        *new_timeline=(gchar *)service_wrapper->user_data;
	StatusMonitor monitoring=(StatusMonitor)service_wrapper->form_data;
	
	if(!network_check_http(service_wrapper->service, msg)) {
		if(msg->status_code==401){
			debug("*WARNING:* Authentication failed for online service: %s.", service_wrapper->service->decoded_key);
			online_service_wrapper_free(service_wrapper);
			return;
		}
		
		if(!retrying && (msg->status_code==100||msg->status_code==404))
			return network_retry(service_wrapper, monitoring);
		
	}
	
	const gchar *this_timeline=g_strrstr(service_wrapper->requested_uri, service_wrapper->service->uri);
	
	debug("Started processing timeline: %s.", this_timeline);
	if(processing){
		tweet_list_clear();
		processing=FALSE;
	}
	
	online_service_load_tweet_ids(service_wrapper->service, new_timeline);
	
	guint tweets_parsed=parser_timeline(service_wrapper->service, msg, monitoring);
		
	if(monitoring==Tweets){
		debug("Total tweets in this timeline: %d.", tweets_parsed);
		if(!tweets_parsed && msg->status_code==200)
			return network_retry(service_wrapper, monitoring);
		network_update_timeline_display(service_wrapper->service, tweets_parsed, new_timeline);
	}
	
	if(retrying) retrying=FALSE;
	
	online_service_save_tweet_ids(service_wrapper->service, new_timeline);
	
	network_init();
	
	online_service_wrapper_free(service_wrapper);
}

static void network_retry(OnlineServiceWrapper *service_wrapper, StatusMonitor monitoring){
	debug("Attempting to reload: %s.", service_wrapper->requested_uri);
	network_set_state_loading_timeline(service_wrapper->requested_uri, Retry);
	online_service_request_uri(service_wrapper->service, QUEUE, service_wrapper->requested_uri, network_display_timeline, service_wrapper->requested_uri, (gpointer)monitoring);
	online_service_wrapper_free(service_wrapper);
}/*network_retry(new_timeline, service_wrapper, monitoring);*/

static void network_update_timeline_display(OnlineService *service, guint tweets_parsed, const gchar *new_timeline){
	if(!tweets_parsed){
		app_statusbar_printf("Error Parsing %s's Timeline Parser.", service->decoded_key);
		return;
	}
	app_statusbar_printf("Timeline Loaded for: %s", service->decoded_key);
	
	/* move the pseudo focus of the tweet list to the start of the list */
	tweet_list_goto_top();
}/*network_update_timeline_display(timeline_parsed, new_timeline);*/

/* On get a image */
void network_cb_on_image(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceWrapper *service_wrapper=(OnlineServiceWrapper *)user_data;
	
	NetworkTweetListImageDL *image=(NetworkTweetListImageDL *)service_wrapper->user_data;
	if(!( image && image->filename && image->iter )){
		debug("**ERROR**: Missing image information.  Image filename: %s; Image iter: %s", image->filename, (image->iter ?"valid" :"unknown") );
		return;
	}
	
	gchar *image_filename=NULL;
	if(!(network_check_http(service_wrapper->service, msg)))
		image_filename=cache_images_get_unknown_image_filename();
	else{
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
	
	app_statusbar_printf("New avatar added to TweetList.");
	tweet_list_set_image(image_filename, image->iter);
	
	uber_free(image_filename);
	
	network_tweet_list_image_dl_free(image);
	
	online_service_wrapper_free(service_wrapper);
}/*network_cb_on_image(session, msg, user_data);*/


/********************************************************
 *                       eof                            *
 ********************************************************/
