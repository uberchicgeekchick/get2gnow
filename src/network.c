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
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <libsoup/soup.h>

#include "config.h"
#include "program.h"

#include "gconfig.h"

#include "online-services-typedefs.h"
#include "online-services.defines.h"
#include "online-services.h"
#include "online-service.types.h"
#include "online-service.h"
#include "online-service-wrapper.h"
#include "online-service-request.h"
#include "network.h"

#include "parser.h"
#include "groups.h"
#include "searches.h"
#include "cache.h"

#include "main-window.h"
#include "preferences.h"

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

/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static NetworkTweetListImageDL *network_tweet_list_image_dl_new(TweetList *tweet_list, const gchar *filename, GtkTreeIter *iter);
static void network_tweet_list_image_dl_free(NetworkTweetListImageDL *image);

static void *network_retry(OnlineServiceWrapper *service_wrapper);

/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
/* Check HTTP response code */
gboolean network_check_http(OnlineService *service, SoupMessage *xml){
	if(!( SOUP_IS_MESSAGE(xml) )){
		debug("**ERROR**: Attempting validate invalid SoupMessage.");
		return FALSE;
	}
	
	if(!(SOUP_IS_MESSAGE(xml) && SOUP_STATUS_IS_SUCCESSFUL(xml->status_code) ))
		return FALSE;
	
	return TRUE;
}/*network_check_http(service, xml);*/




static NetworkTweetListImageDL *network_tweet_list_image_dl_new(TweetList *tweet_list, const gchar *filename, GtkTreeIter *iter){
	NetworkTweetListImageDL *image=g_new0(NetworkTweetListImageDL, 1);
	image->tweet_list=tweet_list;
	image->filename=g_strdup(filename);
	image->iter=iter;
	return image;
}/*network_tweet_list_image_dl_new*/


void network_get_image(OnlineService *service, TweetList *tweet_list, const gchar *image_filename, const gchar *image_url, GtkTreeIter *iter){
	debug("Downloading Image: %s.  GET: %s", image_filename, image_url);
	NetworkTweetListImageDL *image=network_tweet_list_image_dl_new(tweet_list, image_filename, iter);
	
	online_service_request_uri(service, QUEUE, image_url, 0, NULL, network_cb_on_image, image, NULL);
}/*network_get_image*/


void *network_cb_on_image(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	NetworkTweetListImageDL *image=(NetworkTweetListImageDL *)online_service_wrapper_get_user_data(service_wrapper);
	if(!( image && image->tweet_list && image->filename && image->iter )){
		debug("**ERROR**: Missing image information.  Image filename: %s; Image iter: %s.", image->filename, (image->iter ?"valid" :"unknown") );
		return NULL;
	}
	
	gchar *image_filename=NULL;
	if(!(network_check_http(service, xml)))
		image_filename=cache_images_get_unknown_image_filename();
	else{
		debug("Saving avatar to file: %s", image->filename);
		if(!(g_file_set_contents(
					image->filename,
						xml->response_body->data,
						xml->response_body->length,
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
}/*network_cb_on_image(session, xml, user_data);*/

static void network_tweet_list_image_dl_free(NetworkTweetListImageDL *image){
	image->tweet_list=NULL;
	uber_object_free(&image->filename, &image->iter, &image, NULL);
}/*network_image_free*/




void network_post_status(gchar *update){
	if(G_STR_EMPTY(update)) return;
	
	if(!( in_reply_to_service && gconfig_if_bool(PREFS_TWEETS_DIRECT_REPLY_ONLY, TRUE)))
		online_services_request(POST, API_POST_STATUS, NULL, network_tweet_cb, "post->update", update);
	else
		online_service_request(in_reply_to_service, POST, API_POST_STATUS, NULL, network_tweet_cb, "post->update", update);
}/*network_post_status(tweet);*/

void network_send_message(OnlineService *service, const gchar *friend, gchar *dm){
	online_service_request(service, POST, API_SEND_MESSAGE, NULL, network_tweet_cb, (gchar *)friend, dm);
}/*network_send_message(service, friend, dm);*/


void *network_tweet_cb(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	gchar *user_data=(gchar *)online_service_wrapper_get_user_data(service_wrapper);
	
	gboolean direct_message=FALSE;
	if(!g_str_equal("post->update", user_data)) direct_message=TRUE;
	
	gchar *message=NULL;
	if(!direct_message) message=g_strdup_printf("<%s>'s status", service->guid);
	else message=g_strdup_printf("Direct Message. To: <%s@%s> From: <%s>", user_data, service->uri, service->guid);
	
	if(!network_check_http(service, xml)){
		debug("%s couldn't be %s :'(", message, (direct_message?"sent":"updated"));
		debug("http error: #%i: %s", xml->status_code, xml->reason_phrase);
		
		statusbar_printf("%s couldn't be %s :'(", message, (direct_message?"sent":"updated"));
		statusbar_printf("http error: #%i: %s", xml->status_code, xml->reason_phrase);
		
		if(xml->status_code==100 && !online_service_wrapper_get_attempt(service_wrapper)){
			debug("Resubmitting Tweet/Status update to: [%s] per http response.", service->key);
			online_service_wrapper_reattempt(service_wrapper);
		}
	}else{
		debug("%s %s :-)", message, (direct_message?"succeeded":"updated"));
		statusbar_printf("%s %s :-)", message, (direct_message?"sent":"updated"));
	}
	uber_free(message);
	
	debug("HTTP response: %s(#%i)", xml->reason_phrase, xml->status_code);
	
	if(in_reply_to_service && service==in_reply_to_service){
		in_reply_to_service=NULL;
		in_reply_to_status_id=0;
		if(xml->status_code==404){
			debug("Resubmitting Tweet/Status update to: [%s] due to Laconica bug.", service->key);
			online_service_request(service, POST, (direct_message?API_SEND_MESSAGE:API_POST_STATUS), NULL, network_tweet_cb, user_data, online_service_wrapper_get_form_data(service_wrapper));
		}
	}
	return NULL;
}/*network_tweet_cb*/




void network_set_state_loading_timeline(const gchar *uri, ReloadState state){
	const gchar *notice_prefix=NULL;
	switch(state){
		case Retry:
			notice_prefix=_("Retrying");
			break;
			
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
	statusbar_printf("%s: %s %s.%s", notice_prefix, _("timeline"),  timeline, ( ( gconfig_if_bool(PREFS_URLS_EXPANSION_DISABLED, FALSE) || gconfig_if_bool(PREFS_URLS_EXPANSION_SELECTED_ONLY, TRUE) ) ?"" :_("  This may take several moments.") ));
}/*network_timeline_loading_notification*/


void *network_display_timeline(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	TweetList *tweet_list=(TweetList *)online_service_wrapper_get_user_data(service_wrapper);
	UpdateMonitor monitoring=(UpdateMonitor)online_service_wrapper_get_form_data(service_wrapper);
	
	if(!network_check_http(service, xml)){
		if(xml->status_code==401){
			debug("*WARNING:* Authentication failed for online service: %s.", service->key);
			return NULL;
		}
		
		if(!online_service_wrapper_get_attempt(service_wrapper) && (xml->status_code==100||xml->status_code==404)){
			network_retry(service_wrapper);
			return NULL;
		}
		
	}
	
	gchar *request_uri=online_service_request_uri_create(service, NULL);
	const gchar *timeline=g_strrstr(online_service_wrapper_get_requested_uri(service_wrapper), request_uri);
	
	debug("Started processing timeline: %s.", timeline);
	
	guint new_updates=0;
	switch(monitoring){
		case None:
			debug("Attempting to parse an unsupport network request.");
			break;
		case Searches:
			new_updates=searches_parse_results(service, xml, timeline, tweet_list);
			break;
		case Groups:
			new_updates=groups_parse_conversation(service, xml, timeline, tweet_list);
			break;
		case DMs:
		case Replies:
		case Faves:
		case Tweets:
		case Timelines:
		case Users:
		case Archive:
		case BestFriends:
		default:
			new_updates=parse_timeline(service, xml, timeline, tweet_list, monitoring);
			break;
	}
	
	debug("Total tweets in this timeline: %d.", new_updates);
	if(!online_service_wrapper_get_attempt(service_wrapper) && !new_updates && !g_strrstr(request_uri, "?since_id=") && xml->status_code==200){
		uber_free(request_uri);
		network_retry(service_wrapper);
		return NULL;
	}
	tweet_list_complete(tweet_list);
	
	uber_free(request_uri);
	return NULL;
}/*network_display_timeline(session, xml, service_wrapper);*/

static void *network_retry(OnlineServiceWrapper *service_wrapper){
	const gchar *requested_uri=online_service_wrapper_get_requested_uri(service_wrapper);
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	/*TweetList *tweet_list=(TweetList *)online_service_wrapper_get_user_data(service_wrapper);
	UpdateMonitor monitoring=(UpdateMonitor)online_service_wrapper_get_form_data(service_wrapper);*/
	debug("Resubmitting: %s to <%s>.", requested_uri, service->uri);
	network_set_state_loading_timeline(requested_uri, Retry);
	online_service_wrapper_reattempt(service_wrapper);
	/*online_service_request_uri(service, QUEUE, requested_uri, 0, NULL, network_display_timeline, tweet_list, (gpointer)monitoring);*/
	return NULL;
}/*network_retry(new_timeline, service_wrapper, monitoring);*/



/********************************************************
 *                       eof                            *
 ********************************************************/
