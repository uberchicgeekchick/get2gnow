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
#include "tweets.h"
#include "timer.h"
#include "online-service.h"

#define DEBUG_DOMAIN	  "OnlineServices"

typedef struct {
	gchar        *src;
	GtkTreeIter   iter;
} Image;


/* Autoreload timeout functions */
static gboolean network_timeout(gpointer user_data);
static void network_timeout_new(void);

/* libsoup callbacks */
static void network_tweet_cb( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_cb_on_timeline( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_cb_on_image( SoupSession *session, SoupMessage *msg, gpointer user_data );

/* Copyright(C) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com> */
static gboolean network_get_users_page(SoupMessage *msg);

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
gboolean network_check_http( SoupMessage *msg ){
	const gchar *error=NULL;
	if(msg->status_code == 401)
		error=_("Access denied");
	else if(SOUP_STATUS_IS_CLIENT_ERROR(msg->status_code))
		error=_("HTTP communication error");
	else if(SOUP_STATUS_IS_SERVER_ERROR(msg->status_code))
		error=_("Internal server error");
	else if(!SOUP_STATUS_IS_SUCCESSFUL(msg->status_code))
		error=_("Stopped");
	else {
		timer_main(current_service->timer, msg);
		return TRUE;
	}
	
	app_statusbar_printf("%s: %s.", error, msg->reason_phrase);
	
	return FALSE;
}

/* Logout current user */
void network_logout(void){
	if(current_timeline){
		g_free(current_timeline);
		current_timeline=NULL;
	}
	
	debug(DEBUG_DOMAIN, "Logout");
}


/* Post a new tweet - text must be Url encoded */
void network_post_status(const gchar *text){
	gchar *formdata=NULL;
	if(!in_reply_to_status_id)
		formdata=g_strdup_printf( "source=%s&status=%s", API_CLIENT_AUTH, text );
	else
		formdata=g_strdup_printf( "source=%s&in_reply_to_status_id=%lu&status=%s", API_CLIENT_AUTH, in_reply_to_status_id, text );
	online_services_request( online_services, POST, API_POST_STATUS, network_tweet_cb, g_strdup("Tweet"), formdata );
}//network_post_status


/* Send a direct message to a follower - text must be Url encoded  */
void network_send_message( const gchar *friend, const gchar *text ){
	gchar *formdata=g_strdup_printf( "source=%s&user=%s&text=%s", API_CLIENT_AUTH, friend, text );
	online_services_request( online_services, POST, API_SEND_MESSAGE, network_tweet_cb, g_strdup("DM"), formdata );
}

void network_refresh(void){
	if(!current_timeline || processing)
		return;

	/* UI */
	app_set_statusbar_msg(_("Loading timeline..."));

	processing=TRUE;
	online_services_request( online_services, QUEUE, current_timeline, network_cb_on_timeline, NULL, NULL );
}

/* Get and parse a timeline */
void network_get_timeline(const gchar *uri_timeline){
	if(processing)
		return;

	parser_reset_lastid();

	/* UI */
	processing=TRUE;
	debug(DEBUG_DOMAIN, "Loading timeline: %s", uri_timeline);
	app_set_statusbar_msg(_("Loading timeline..."));
	
	online_services_request( online_services, QUEUE, uri_timeline, network_cb_on_timeline, g_strdup(uri_timeline), NULL );
	/* network_queue's 3rd argument is used to trigger a new timeline & enables 'Refresh' */
}//network_get_timeline

/* Get a user timeline */
void network_get_user_timeline(const gchar *username){
	gchar *user_id;

	if(!username)
		user_id=g_strdup(current_service->username);
	else
		user_id=g_strdup(username);

	if(!G_STR_EMPTY(user_id)) {
		gchar *user_timeline=g_strdup_printf( API_TIMELINE_USER, user_id );
		online_service_request( current_service, GET, user_timeline, NULL, NULL, NULL );
		g_free( user_timeline );
	}

	if(user_id) g_free(user_id);
}//network_get_user


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
		debug(DEBUG_DOMAIN, "Getting page %d of who%s.", page,(get_friends?"m the user is following":" is following the user") );
		/* TODO: implement using
		 * msg=network_get( uri );
		 */
		msg=online_service_request_url( current_service, GET, uri, NULL, NULL, NULL );
		fetching=network_get_users_page(msg);
		if(uri) g_free(uri); uri=NULL;
	}
	
	if(!all_users)
		app_set_statusbar_msg( _("Users parser error.") );
	else
		all_users=g_list_sort(all_users,(GCompareFunc) usrcasecmp);

	return all_users;
}//network_get_users_glist


static gboolean network_get_users_page(SoupMessage *msg){
	debug(DEBUG_DOMAIN, "Users response: %i",msg->status_code);
	
	/* Check response */
	if(!network_check_http( msg ))
		return FALSE;

	/* parse user list */
	debug(DEBUG_DOMAIN, "Parsing user list");
	GList *new_users;
	if(!(new_users=users_new(msg->response_body->data, msg->response_body->length)) )
		return FALSE;
	
	if(!all_users)
		all_users=new_users;
	else
		all_users=g_list_concat(all_users, new_users);
	
	return TRUE;
}//network_get_users_page


/* Get an image from servers */
gboolean network_download_avatar( const gchar *image_uri ){
	gchar *image_filename=NULL;
	if(g_file_test((image_filename=images_get_filename(image_uri)), G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR))
		return TRUE;
	debug(DEBUG_DOMAIN, "Downloading Image: %s\nGet: %s", image_filename, image_uri);

	SoupMessage *msg=online_service_request(current_service, GET, image_uri, NULL, NULL, NULL);
		
	debug(DEBUG_DOMAIN, "Image response: %i", msg->status_code);

	/* check response */
	if(!((network_check_http( msg )) &&( g_file_set_contents( image_filename, msg->response_body->data, msg->response_body->length, NULL )) ))
		return FALSE;
	
	return TRUE;
}


void network_get_image(const gchar *image_uri, GtkTreeIter iter ){
	gchar *image_filename=NULL;
	if(g_file_test((image_filename=images_get_filename(image_uri)), G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)) {		
		/* Set image from file here */
		tweet_list_set_image(image_filename, iter);
		g_free(image_filename);
		return;
	}
	
	Image *image=g_new0(Image, 1);
	image->src=image_filename;
	image->iter=iter;
	
	online_service_request( current_service, QUEUE, image_uri, network_cb_on_image, image, NULL );
}//network_get_image


/* On send a direct message */
static void network_tweet_cb(SoupSession *session, SoupMessage *msg, gpointer user_data){
	if(network_check_http( msg ))
		app_statusbar_printf("%s Sent",(const gchar *)user_data);
	
	if(in_reply_to_status_id) in_reply_to_status_id=0;
	
	debug(DEBUG_DOMAIN, "%s response: %i",((const gchar *)user_data), msg->status_code);
	g_free(user_data);
}//network_tweet_cb


/* On get a timeline */
static void network_cb_on_timeline( SoupSession *session, SoupMessage *msg, gpointer user_data ){
	gchar        *new_timeline=NULL;
	
	if(user_data){
		new_timeline =(gchar *)user_data;
	}

	debug( DEBUG_DOMAIN, "Timeline response: %i",msg->status_code);

	processing=FALSE;

	/* Timeout */
	network_timeout_new();

	/* Check response */
	if(!network_check_http( msg )) {
		if(new_timeline){
			g_free(new_timeline);
			new_timeline=NULL;
		}
		return;
	}

	debug(DEBUG_DOMAIN, "Parsing timeline");

	/* Parse and set ListStore */
	if(parser_timeline(msg->response_body->data, msg->response_body->length)) {
		app_set_statusbar_msg(_("Timeline Loaded"));

		if(new_timeline){
			if(current_timeline) g_free(current_timeline); current_timeline=NULL;
			current_timeline=g_strdup(new_timeline);
		}
	} else {
		app_set_statusbar_msg(_("Timeline Parser Error."));
	}

	if(new_timeline) g_free(new_timeline); new_timeline=NULL;
}


/* On get a image */
static void network_cb_on_image( SoupSession *session, SoupMessage *msg, gpointer user_data ){

	Image *image=(Image *)user_data;
	debug(DEBUG_DOMAIN, "Image response: %i", msg->status_code);

	/* check response */
	if(network_check_http( msg )) {
		/* Save image data */
		if(g_file_set_contents(image->src,
								 msg->response_body->data,
								 msg->response_body->length,
								 NULL)) {
			/* Set image from file here(image_file) */
			tweet_list_set_image(image->src,image->iter);
		}
	}
	
	if(image->src) g_free(image->src); image->src=NULL;
	if(image) g_free(image); image=NULL;
}


static void network_timeout_new(void){
	gint minutes;
	guint reload_time;

	if(timeout_id) {
		debug(DEBUG_DOMAIN,
					  "Stopping timeout id: %i", timeout_id);

		g_source_remove(timeout_id);
	}

	gconfig_get_int(PREFS_TWEETS_RELOAD_TIMELINES, &minutes);

	/* The timeline reload interval shouldn't be less than 3 minutes */
	if(minutes < 3)
		minutes=3;

	/* This should be the number of milliseconds */
	reload_time=minutes*60*1000;

	timeout_id=g_timeout_add(reload_time, network_timeout, NULL);

	debug(DEBUG_DOMAIN, "Starting timeout id: %i", timeout_id);
}

static gboolean network_timeout(gpointer user_data){
	if(!current_timeline || processing)
		return FALSE;

	/* UI */
	app_set_statusbar_msg(_("Reloading timeline..."));

	debug(DEBUG_DOMAIN,
				  "Auto reloading. Timeout: %i", timeout_id);

	processing=TRUE;
	online_service_request( current_service, QUEUE, current_timeline, network_cb_on_timeline, NULL, NULL );

	timeout_id=0;

	return FALSE;
}
