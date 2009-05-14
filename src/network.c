/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright(C) 2007-2009 Daniel Morales <daniminas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or(at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors: Daniel Morales <daniminas@gmail.com>
 * 		Kaity G. B. <uberChick@uberChicGeekChick.Com>
 *
 */

#include <config.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <libsoup/soup.h>

#include "debug.h"
#include "gconfig.h"
#ifdef HAVE_GNOME_KEYRING
#include "keyring.h"
#endif

#include "main.h"
#include "network.h"
#include "parser.h"
#include "users.h"
#include "images.h"
#include "app.h"
#include "preferences.h"
#include "following-viewer.h"
#include "tweet-view.h"
#include "tweet-list.h"
#include "tweets.h"
#include "timer.h"
#include "online-services.h"

#define	DEBUG_DOMAINS	"OnlineServices:Networking:Tweets:Requests:Users:Authentication:Setup:Start-Up"

typedef struct {
	gchar        *src;
	GtkTreeIter   *iter;
} Image;

typedef enum {
	Load,
	Reload,
	Timeout,
} ReloadState;


/* Autoreload timeout functions */
static gboolean network_timeout(gpointer user_data);
static void network_timeout_new(void);

/* libsoup callbacks */
static void network_tweet_cb( SoupSession *session, SoupMessage *msg, gpointer user_data );

static void network_set_state_loading_timeline(const gchar *timeline, ReloadState state);
static void network_cb_on_timeline( SoupSession *session, SoupMessage *msg, gpointer user_data );

/* Copyright(C) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com> */
static gboolean network_get_users_page(OnlineService *service, SoupMessage *msg);

gboolean getting_followers=FALSE;
static GList *all_users=NULL;
/* My, Kaity G. B., new stuff ends here. */

static gchar *current_timeline=NULL;
static gboolean processing=FALSE;
static guint timeout_id;

gchar *url_encode(const gchar *text){
	const char        *good;
	static const char  hex[16]="0123456789ABCDEF";
	GString           *result;
	
	g_return_val_if_fail(text != NULL, NULL);
	g_return_val_if_fail(*text != '\0', NULL);
	
	/* RFC 3986 */ 
	good="~-._";
	
	result=g_string_new(NULL);
	while(*text) {
		unsigned char c=*text++;
		
		if(g_ascii_isalnum(c) || strchr(good, c))
			g_string_append_c(result, c);
		else {
			g_string_append_c(result, '%');
			g_string_append_c(result, hex[c >> 4]);
			g_string_append_c(result, hex[c & 0xf]);
		}
	}
	
	return g_string_free(result, FALSE);
}

/* Check HTTP response code */
gboolean network_check_http(OnlineService *service, SoupMessage *msg){
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
		timer_main(service->timer, msg);
		return TRUE;
	}
	
	app_statusbar_printf("%s: %s (%d).", error, msg->reason_phrase, msg->status_code );
	
	return FALSE;
}

/* Logout current user */
void network_logout(void){
	if(current_timeline){
		g_free(current_timeline);
		current_timeline=NULL;
	}
	
	debug(DEBUG_DOMAINS, "Logout");
}


/* Post a new tweet - text must be Url encoded */
void network_post_status(const gchar *text){
	gchar *formdata=NULL;
	formdata=g_strdup_printf("source=%s&status=%s", API_CLIENT_AUTH, text);
	gchar *action=g_strdup("Tweet");
	online_services_request(online_services, POST, API_POST_STATUS, network_tweet_cb, action, (gchar *)text);
	g_free(action);
	g_free(formdata);
}//network_post_status


/* Send a direct message to a follower - text must be Url encoded  */
void network_send_message(OnlineService *service, const gchar *friend, const gchar *text){
	gchar *formdata=g_strdup_printf( "%s&user=%s", text, friend);
	gchar *action=g_strdup("DM");
	online_service_request(service, POST, API_SEND_MESSAGE, network_tweet_cb, action, formdata);
	g_free(action);
	g_free(formdata);
}

static void network_set_state_loading_timeline(const gchar *timeline, ReloadState state){
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
	debug(DEBUG_DOMAINS, "%s current timeline: %s", notice_prefix, timeline);
	app_statusbar_printf("%s: %s.%s", notice_prefix, _("timeline"), timeline, ( ( gconfig_if_bool(PREFS_URLS_EXPAND_DISABLED) || gconfig_if_bool(PREFS_URLS_EXPAND_SELECTED_ONLY) ) ?"" :_("  This may take several moments.") ));
	
	processing=TRUE;
}//network_timeline_loading_notification

void network_refresh(void){
	if(!current_timeline || processing)
		return;
		
	/* UI */
	network_set_state_loading_timeline(current_timeline, Reload);
	online_services_request( online_services, QUEUE, current_timeline, network_cb_on_timeline, NULL, NULL );
}

/* Get and parse a timeline */
void network_get_timeline(const gchar *uri_timeline){
	if(processing)
		return;
		
	parser_reset_lastid();
	
	/* UI */
	network_set_state_loading_timeline(uri_timeline, Load);
	gchar *new_timeline=g_strdup(uri_timeline);
	
	online_services_request( online_services, QUEUE, uri_timeline, network_cb_on_timeline, new_timeline, NULL );
	g_free(new_timeline);
	/* network_queue's 3rd argument is used to trigger a new timeline & enables 'Refresh' */
}//network_get_timeline

/* Get a user timeline */
void network_get_user_timeline(OnlineService *service, const gchar *username){
	gchar *user_id=NULL;
	
	if(!username)
		user_id=g_strdup(service->username);
	else
		user_id=g_strdup(username);
		
	if(G_STR_EMPTY(user_id)){
		if(user_id) g_free(user_id);
		debug(DEBUG_DOMAINS, "*WARNING:* Attempting to loading timeline for an unknow user.");
		return;
	}
	
	gchar *user_timeline=g_strdup_printf(API_TIMELINE_USER, user_id);
	
	network_set_state_loading_timeline(user_timeline, Load);

	online_service_request(service, QUEUE, user_timeline, network_cb_on_timeline, user_timeline, NULL);
	g_free(user_timeline);
	g_free(user_id);
}//network_get_user_timeline


GList *network_get_users_glist(gboolean get_friends){
	SoupMessage *msg;
	gint page=0; 
	
	gchar *uri;
	all_users=NULL;
	gboolean fetching=TRUE;
	getting_followers=!get_friends;
	
	app_statusbar_printf("Please wait while %s loads the users %s...", PACKAGE_NAME,(get_friends?"who are following you":"you're following") );
	
	while(fetching){
		page++;
		uri=g_strdup_printf("%s?page=%d",(get_friends?API_FOLLOWING:API_FOLLOWERS), page);
		debug(DEBUG_DOMAINS, "Getting page %d of who%s.", page,(get_friends?"m the user is following":" is following the user") );
		/* TODO: implement using
		 * msg=network_get( uri );
		 */
		msg=online_service_request(current_service, GET, uri, NULL, NULL, NULL );
		fetching=network_get_users_page(current_service, msg);
		if(uri){
			g_free(uri);
			uri=NULL;
		}
	}
	
	if(!all_users)
		app_set_statusbar_msg( _("Users parser error.") );
	else
		all_users=g_list_sort(all_users,(GCompareFunc) usrcasecmp);
		
	return all_users;
}//network_get_users_glist


static gboolean network_get_users_page(OnlineService *service, SoupMessage *msg){
	debug(DEBUG_DOMAINS, "Users response: %i",msg->status_code);
	
	/* Check response */
	if(!network_check_http(service, msg))
		return FALSE;
		
	/* parse user list */
	debug(DEBUG_DOMAINS, "Parsing user list");
	GList *new_users;
	if(!(new_users=users_new(service, msg)) )
		return FALSE;
	
	if(!all_users)
		all_users=new_users;
	else
		all_users=g_list_concat(all_users, new_users);
	
	return TRUE;
}//network_get_users_page


/* Get an image from servers */
gboolean network_download_avatar(OnlineService *service, const gchar *image_uri){
	gchar *image_filename=NULL;
	if(g_file_test((image_filename=images_get_filename(image_uri)), G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR))
		return TRUE;
	debug(DEBUG_DOMAINS, "Downloading Image: %s\nGet: %s", image_filename, image_uri);
	
	SoupMessage *msg=online_service_request_url(service, GET, image_uri, NULL, NULL, NULL);
		
	debug(DEBUG_DOMAINS, "Image response: %i", msg->status_code);
	
	/* check response */
	if(!((network_check_http(service, msg)) &&( g_file_set_contents( image_filename, msg->response_body->data, msg->response_body->length, NULL )) ))
		return FALSE;
	
	return TRUE;
}


void network_get_image(OnlineService *service, const gchar *image_uri, GtkTreeIter *iter){
	gchar *image_filename=NULL;
	if(g_file_test((image_filename=images_get_filename(image_uri)), G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)) {		
		/* Set image from file here */
		tweet_list_set_image(image_filename, iter);
		g_free(image_filename);
		g_free(iter);
		return;
	}
	
	Image *image=g_new0(Image, 1);
	image->src=image_filename;
	image->iter=iter;
	
	online_service_request_url( service, QUEUE, image_uri, network_cb_on_image, image, NULL );
}//network_get_image


/* On send a direct message */
static void network_tweet_cb(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceCBWrapper *service_wrapper=(OnlineServiceCBWrapper *)user_data;
	gchar *status=(gchar *)service_wrapper->user_data;
	
	if(!network_check_http(service_wrapper->service, msg)){
		debug(DEBUG_DOMAINS, "**ERROR**: %s failed.\n\t\tTweet request for: '%s'; http status: %s(#%i)", status, service_wrapper->service->key, msg->reason_phrase, msg->status_code);
		app_statusbar_printf("%s's %s was not sent.", service_wrapper->service->key, status);
	}else{
		debug(DEBUG_DOMAINS, "%s's: 'network_tweet_cb' successfully processed '%s'.\n\t\thttp status: %s(#%i)", service_wrapper->service->key, status, msg->reason_phrase, msg->status_code);
		app_statusbar_printf("%s's %s successfully sent.", service_wrapper->service->key, status);
	}
	
	if(service_wrapper->service==online_services_get_last_connected(online_services)){
		if(in_reply_to_status_id) in_reply_to_status_id=0;
		if(in_reply_to_service) in_reply_to_service=NULL;
	}
	
	online_services_free_wrapper(online_services, service_wrapper);
}//network_tweet_cb


/* On get a timeline */
static void network_cb_on_timeline(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceCBWrapper *service_wrapper=(OnlineServiceCBWrapper *)user_data;
	gchar        *new_timeline=NULL;
	
	new_timeline=(gchar *)service_wrapper->user_data;
	
	debug(DEBUG_DOMAINS, "Timeline response: %i",msg->status_code);
	
	if(processing)
		tweet_list_refresh();
	
	processing=FALSE;
	
	/* Timeout */
	network_timeout_new();
	
	/* Check response */
	if(!network_check_http(service_wrapper->service, msg)) {
		online_services_free_wrapper(online_services, service_wrapper);
		return;
	}
	
	debug(DEBUG_DOMAINS, "Parsing timeline");
	
	/* Parse and set ListStore */
	if(!(parser_timeline(service_wrapper->service, msg)))
		app_statusbar_printf("Error Parsing %s's Timeline Parser.", service_wrapper->service->decoded_key);
	else {
		app_statusbar_printf("Timeline Loaded for: %s", service_wrapper->service->decoded_key);
		
		if(new_timeline){
			if(!( current_timeline && g_str_equal(new_timeline, current_timeline) )){
				g_free(current_timeline);
				current_timeline=g_strdup(new_timeline);
			}
		}
	}
	
	/* move the pseudo focus of the tweet list to the start of the list */
	tweet_list_goto_top();
	
	online_services_free_wrapper(online_services, service_wrapper);
}

/* On get a image */
void network_cb_on_image(SoupSession *session, SoupMessage *msg, gpointer user_data){
	OnlineServiceCBWrapper *service_wrapper=(OnlineServiceCBWrapper *)user_data;
	
	debug(DEBUG_DOMAINS, "Image response: %i", msg->status_code);
	
	Image *image=(Image *)service_wrapper->user_data;
	if(!( image && image->src && image->iter )){
		debug(DEBUG_DOMAINS, "**ERROR**: Missing image information.  Image filename: %s; Image iter: %s", image->src, (image->iter ?"valid" :"unknown") );
		return;
	}
	
	/* check response */
	if(network_check_http(service_wrapper->service, msg)) {
		/* Save image data */
		debug(DEBUG_DOMAINS, "Saving image to file: %s", image->src);
		if(g_file_set_contents(
					image->src,
						msg->response_body->data,
						msg->response_body->length,
					NULL
		)) {
			/* Set image from file here(image_file) */
			tweet_list_set_image(image->src, image->iter);
			tweet_list_goto_top();
		}
	}
	
	if(image->src) g_free(image->src);
	if(image->iter) g_free(image->iter);
	g_free(image);
	
	g_free(service_wrapper);
	service_wrapper=NULL;
}


static void network_timeout_new(void){
	gint minutes;
	guint reload_time;
	
	if(timeout_id) {
		debug(DEBUG_DOMAINS, "Stopping timeout id: %i", timeout_id);
		g_source_remove(timeout_id);
	}
	
	gconfig_get_int(PREFS_TWEETS_RELOAD_TIMELINES, &minutes);
	
	/* The timeline reload interval shouldn't be less than 3 minutes */
	if(minutes < 3)
		minutes=3;
	
	/* This should be the number of milliseconds */
	reload_time=minutes*60*1000;
	
	timeout_id=g_timeout_add(reload_time, network_timeout, NULL);
	
	debug(DEBUG_DOMAINS, "Starting timeout id: %i", timeout_id);
}

static gboolean network_timeout(gpointer user_data){
	if(!current_timeline || processing)
		return FALSE;
	
	/* UI */
	network_set_state_loading_timeline(current_timeline, Timeout);
	
	online_services_request( online_services, QUEUE, current_timeline, network_cb_on_timeline, NULL, NULL );
	
	timeout_id=0;
	
	return FALSE;
}
