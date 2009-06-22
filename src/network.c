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

#include "online-services-typedefs.h"
#include "online-services.h"
#include "online-service.h"
#include "online-service-wrapper.h"
#include "online-service-request.h"
#include "network.h"

#include "parser.h"
#include "cache.h"

#include "main-window.h"
#include "preferences.h"

#include "users.h"
#include "friends-manager.h"
#include "following-viewer.h"
#include "tweet-view.h"

#include "tweet-list.h"
#include "tweets.h"
#include "timer.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define	DEBUG_DOMAINS	"Networking:OnlineServices:Tweets:Requests:Users:Images:Authentication:Refreshing:Setup:Start-Up"
#include "debug.h"

typedef struct _NetworkTweetListImageDL NetworkTweetListImageDL;

struct _NetworkTweetListImageDL{
	TweetList	*tweet_list;
	gchar		*filename;
	GtkTreeIter	*iter;
};

static gboolean retrying=FALSE;

/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static NetworkTweetListImageDL *network_tweet_list_image_dl_new(TweetList *tweet_list, const gchar *filename, GtkTreeIter *iter);
static void network_tweet_list_image_dl_free(NetworkTweetListImageDL *image);

static void *network_tweet_cb(SoupSession *session, SoupMessage *msg, OnlineServiceWrapper *service_wrapper);

static void *network_retry(OnlineServiceWrapper *service_wrapper);

/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
static NetworkTweetListImageDL *network_tweet_list_image_dl_new(TweetList *tweet_list, const gchar *filename, GtkTreeIter *iter){
	NetworkTweetListImageDL *image=g_new0(NetworkTweetListImageDL, 1);
	image->tweet_list=tweet_list;
	image->filename=g_strdup(filename);
	image->iter=iter;
	return image;
}/*network_tweet_list_image_dl_new*/
			
static void network_tweet_list_image_dl_free(NetworkTweetListImageDL *image){
	image->tweet_list=NULL;
	uber_object_free(&image->filename, &image->iter, &image, NULL);
}/*network_image_free*/

/* Check HTTP response code */
gboolean network_check_http(OnlineService *service, SoupMessage *xml){
	if(!( SOUP_IS_MESSAGE(xml) )){
		debug("**ERROR**: Attempting validate invalid SoupMessage.");
		return FALSE;
	}
	
	if(!SOUP_STATUS_IS_SUCCESSFUL(xml->status_code))
		return FALSE;
	
	return TRUE;
}/*network_check_http(service, xml);*/

/* Post a new tweet - text must be Url encoded */
void network_post_status(const gchar *update){
	if(G_STR_EMPTY(update)) return;
	
	if(!( in_reply_to_service && gconfig_if_bool(PREFS_TWEETS_DIRECT_REPLY_ONLY, TRUE)))
		online_services_request(online_services, POST, API_POST_STATUS, NULL, network_tweet_cb, "Tweet", (gchar *)update);
	else
		online_service_request(in_reply_to_service, POST, API_POST_STATUS, NULL, network_tweet_cb, "Tweet", (gchar *)update);
}/*network_post_status(tweet);*/

/* Send a direct message to a follower - text must be Url encoded  */
void network_send_message(OnlineService *service, const gchar *friend, const gchar *dm){
	gchar *form_data=g_strdup_printf("source=%s&user=%s&text=%s", online_service_get_micro_blogging_client(service), friend, dm);
	online_service_request(service, POST, API_SEND_MESSAGE, NULL, network_tweet_cb, "DM", form_data);
	g_free(form_data);
}/*network_send_message(service, friend, dm);*/

void network_set_state_loading_timeline(const gchar *uri, ReloadState state){
	const gchar *notice_prefix=NULL;
	switch(state){
		case Retry:
			retrying=TRUE;
		case Reload:
			notice_prefix=_("Reloading");
			break;
		
		case Load:
		default:
			notice_prefix=_("Loading");
			break;
	}
	const gchar *timeline=g_strrstr(uri, "/");
	debug("%s current timeline: %s", notice_prefix, timeline);
	statusbar_printf("%s: %s %s.%s", notice_prefix, _("timeline"),  timeline, ( ( gconfig_if_bool(PREFS_URLS_EXPAND_DISABLED, FALSE) || gconfig_if_bool(PREFS_URLS_EXPAND_SELECTED_ONLY, TRUE) ) ?"" :_("  This may take several moments.") ));
}/*network_timeline_loading_notification*/


void network_get_image(OnlineService *service, TweetList *tweet_list, const gchar *image_filename, const gchar *image_url, GtkTreeIter *iter){
	debug("Downloading Image: %s.  GET: %s", image_filename, image_url);
	NetworkTweetListImageDL *image=network_tweet_list_image_dl_new(tweet_list, image_filename, iter);
	
	online_service_request_uri(service, QUEUE, image_url, NULL, network_cb_on_image, image, NULL);
}/*network_get_image*/


/* On send a direct message */
static void *network_tweet_cb(SoupSession *session, SoupMessage *msg, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	gchar *status=(gchar *)online_service_wrapper_get_user_data(service_wrapper);
	
	if(!network_check_http(service, msg)){
		gchar *status_error=g_utf8_strup(status, -1);
		debug("**%s-ERROR:** send failed for: '%s'.", status_error, online_service_get_key(service));
		statusbar_printf("%s's %s was not sent.", online_service_get_key(service), status);
		uber_free(status_error);
	}else{
		debug("%s sent successfully to: '%s'.", status, online_service_get_key(service));
		statusbar_printf("%s's %s sent successfully.", online_service_get_key(service), status);
	}
	
	debug("HTTP response: %s(#%i)", msg->reason_phrase, msg->status_code);
	
	if(in_reply_to_service){
		if(service==in_reply_to_service){
			in_reply_to_service=NULL;
			in_reply_to_status_id=0;
			if(msg->status_code==404){
				debug("Resubmitting Tweet/Status update to: [%s] due to Laconica bug.", online_service_get_key(service));
				online_service_request(service, POST, API_POST_STATUS, NULL, network_tweet_cb, "Tweet", (gchar *)online_service_wrapper_get_form_data(service_wrapper));
			}
		}
	}
	return NULL;
}/*network_tweet_cb*/

/* On get a timeline */
void *network_display_timeline(SoupSession *session, SoupMessage *msg, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	TweetList *tweet_list=(TweetList *)online_service_wrapper_get_user_data(service_wrapper);
	TweetLists monitoring=(TweetLists)online_service_wrapper_get_form_data(service_wrapper);

	if(!network_check_http(service, msg)){
		if(msg->status_code==401){
			debug("*WARNING:* Authentication failed for online service: %s.", online_service_get_key(service));
			return NULL;
		}
		
		if(!retrying && (msg->status_code==100||msg->status_code==404)){
			network_retry(service_wrapper);
			return NULL;
		}
		
	}
	
	gchar *request_uri=online_service_request_uri_create(service, NULL);
	const gchar *timeline=g_strrstr(online_service_wrapper_get_requested_uri(service_wrapper), request_uri);
	
	debug("Started processing timeline: %s.", timeline);
	
	guint tweets_parsed=parse_timeline(service, msg, timeline, tweet_list, monitoring);
		
	debug("Total tweets in this timeline: %d.", tweets_parsed);
	if(!retrying && !tweets_parsed && !g_strrstr(request_uri, "?since_id=") && msg->status_code==200){
		uber_free(request_uri);
		network_retry(service_wrapper);
		return NULL;
	}
	
	if(retrying) retrying=FALSE;
	if(service==online_services_connected_get_last(online_services))
		tweet_list_complete(tweet_list);
	
	uber_free(request_uri);
	return NULL;
}/*network_display_timeline(session, msg, service_wrapper);*/

static void *network_retry(OnlineServiceWrapper *service_wrapper){
	const gchar *requested_uri=online_service_wrapper_get_requested_uri(service_wrapper);
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	TweetList *tweet_list=(TweetList *)online_service_wrapper_get_user_data(service_wrapper);
	TweetLists monitoring=(TweetLists)online_service_wrapper_get_form_data(service_wrapper);
	debug("Resubmitting: %s to <%s>.", requested_uri, online_service_get_uri(service));
	network_set_state_loading_timeline(requested_uri, Retry);
	online_service_request_uri(service, QUEUE, requested_uri, NULL, network_display_timeline, tweet_list, (gpointer)monitoring);
	return NULL;
}/*network_retry(new_timeline, service_wrapper, monitoring);*/

/* On get a image */
void *network_cb_on_image(SoupSession *session, SoupMessage *msg, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	NetworkTweetListImageDL *image=(NetworkTweetListImageDL *)online_service_wrapper_get_user_data(service_wrapper);
	if(!( image && image->tweet_list && image->filename && image->iter )){
		debug("**ERROR**: Missing image information.  Image filename: %s; Image iter: %s.", image->filename, (image->iter ?"valid" :"unknown") );
		return NULL;
	}
	
	gchar *image_filename=NULL;
	if(!(network_check_http(service, msg)))
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
	
	main_window_statusbar_printf("New avatar added to TweetList.");
	tweet_list_set_image(image->tweet_list, image_filename, image->iter);
	
	uber_free(image_filename);
	
	network_tweet_list_image_dl_free(image);
	return NULL;
}/*network_cb_on_image(session, msg, user_data);*/


/********************************************************
 *                       eof                            *
 ********************************************************/
