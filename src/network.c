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
#include "gconf.h"
#ifdef HAVE_GNOME_KEYRING
#include "keyring.h"
#endif

#include "main.h"
#include "network.h"
#include "parser.h"
#include "users.h"
#include "images.h"
#include "app.h"
#include "following-viewer.h"
#include "tweets.h"
#include "timer.h"
#include "oauth.h"

#define DEBUG_DOMAIN	  "Network"

typedef struct {
	gchar        *src;
	GtkTreeIter   iter;
} Image;

/* libsoup callbacks */
static void network_cb_on_login( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_tweet_cb( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_cb_on_timeline( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_cb_on_image( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_cb_on_auth( SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer user_data );

/* Copyright(C) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com> */
gboolean getting_followers=FALSE;

static gboolean network_get_users_page(SoupMessage *msg);
/* My, Kaity G. B., new stuff ends here. */

/* Autoreload timeout functions */
static gboolean 	network_timeout			(gpointer user_data);
static void			network_timeout_new		(void);

static SoupSession			*soup_connection=NULL;
static GList *all_users=NULL;
static gchar *current_timeline=NULL;
static gboolean				 processing=FALSE;
static guint				 timeout_id;


SoupSession *network_get_connection(void){
	return soup_connection;
}//network_get_connection

gchar *url_encode_message(gchar *text){
	const char        *good;
	static const char  hex[16] = "0123456789ABCDEF";
	GString           *result;
	
	g_return_val_if_fail(text != NULL, NULL);
	g_return_val_if_fail(*text != '\0', NULL);
	
	/* RFC 3986 */ 
	good = "~-._";
	
	result = g_string_new(NULL);
	while(*text) {
		unsigned char c = *text++;
		
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


/* This function must be called at startup */
void network_new(void) {
	Conf	*conf;
	gboolean	check_proxy=FALSE;
	
	debug(DEBUG_DOMAIN, "Libsoup %sstarted",(soup_connection?"re-":"") );
	
	/* Close previous networking */
	if(soup_connection)
		network_close();

	/* Async connections */
	soup_connection=soup_session_async_new_with_options( SOUP_SESSION_MAX_CONNS, 8, NULL );
	
	/* Set the proxy, if configuration is set */
	conf=conf_get();
	conf_get_bool(conf, PROXY_USE, &check_proxy);
	
	if(!check_proxy) return;
	
	gchar *server=NULL;
	gint port;
	
	/* Get proxy */
	conf_get_string(conf, PROXY_HOST, &server);
	conf_get_int(conf, PROXY_PORT, &port);
	
	if(G_STR_EMPTY(server))
		return;
	
	SoupURI *suri;
	gchar *proxy_uri=NULL;
	
	check_proxy=FALSE;
	conf_get_bool( conf, PROXY_USE_AUTH, &check_proxy);
	
	/* Get proxy auth data */
	if(!check_proxy)
		proxy_uri=g_strdup_printf("http://%s:%d", server, port);
	else {
		char *user, *password;
		conf_get_string( conf, PROXY_USER, &user);
		conf_get_string( conf, PROXY_PASS, &password);
		
		proxy_uri=g_strdup_printf( "http://%s:%s@%s:%d", user, password, server, port);
		
		g_free(user);
		g_free(password);
	}
	
	debug( DEBUG_DOMAIN, "Proxy uri: %s", proxy_uri );
		
	/* Setup proxy info */
	suri=soup_uri_new(proxy_uri);
	g_object_set( G_OBJECT( soup_connection ), SOUP_SESSION_PROXY_URI, suri, NULL );
	
	soup_uri_free(suri);
	g_free(server);
	g_free(proxy_uri);
}


/* Cancels requests, and unref libsoup. */
void network_close(void){
	/* Close all connections */
	network_stop();
	timer_deinit();
	user_free_lists();
	g_object_unref(soup_connection);
	debug(DEBUG_DOMAIN, "Libsoup closed");
}


/* Cancels all pending requests in session. */
void network_stop(void){
	debug(DEBUG_DOMAIN,"Cancelled all connections");
	soup_session_abort(soup_connection);
}

/* Get data from net */
SoupMessage *network_get_uri( const gchar *uri ){
	SoupMessage *msg;
	debug( DEBUG_DOMAIN, "Get: %s", uri );
	
	msg=soup_message_new( "GET", uri );
	soup_session_send_message( soup_connection, msg );
	if(network_check_http( msg ))
		return msg;
	return NULL;
}



/* Get data from net */
SoupMessage *network_get( const gchar *uri ){
	gchar *new_uri=g_strdup_printf("https://%s%s", API_SERVICE, uri );
	SoupMessage *msg=network_get_uri( (const gchar *)new_uri );
	g_free(new_uri);
	return msg;
}

/* Get data from net */
void network_queue_uri( const gchar *uri, SoupSessionCallback callback, gpointer data){
	debug(DEBUG_DOMAIN, "Get: %s",uri);
	SoupMessage *msg=soup_message_new( "GET", uri );
	soup_session_queue_message(soup_connection, msg, callback, data);
}



/* Get data from net */
void network_queue( const gchar *uri, SoupSessionCallback callback, gpointer data){
	gchar *new_uri=g_strdup_printf("https://%s%s", API_SERVICE, uri );
	network_queue_uri((const gchar *)new_uri, callback, data);
	g_free(new_uri);
}


/* Private: Post data to net */
void network_post_uri(const gchar *uri, gchar *formdata, SoupSessionCallback callback, gpointer data){
	SoupMessage *msg;
	
	debug( DEBUG_DOMAIN, "Post: %s", uri );
	
	msg=soup_message_new( "POST", uri );
	
	soup_message_headers_append( msg->request_headers, "X-Twitter-Client", PACKAGE_NAME);
	soup_message_headers_append( msg->request_headers, "X-Twitter-Client-Version", PACKAGE_VERSION);
	
	if(formdata)
		soup_message_set_request(
						msg,
						"application/x-www-form-urlencoded",
						SOUP_MEMORY_TAKE,
						formdata,
						strlen( formdata )
		);
	
	soup_session_queue_message(soup_connection, msg, callback, data);
}

void network_post(const gchar *uri, gchar *formdata, SoupSessionCallback callback, gpointer data){
	gchar *new_uri=g_strdup_printf("https://%s%s", API_SERVICE, uri );
	network_post_uri((const gchar *)new_uri, formdata, callback, data);
	g_free(new_uri);
}//vnetwork_post_data


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
		timer_main(msg);
		return TRUE;
	}
	
	app_statusbar_printf("%s: %s.", error, msg->reason_phrase);
	
	return FALSE;
}


/* Login in Twitter */
void network_login(OAuthService **services){
	for(int i=0; services[i]; i++){
		debug(DEBUG_DOMAIN, "Logging on to %s...", services[i]->account->auth_uri);
		
		app_set_statusbar_msg(_("Connecting..."));
		
		debug(DEBUG_DOMAIN, "Authenticating https://%s:%s@ %s ", services[i]->account->username, services[i]->account->password, services[i]->account->auth_uri);
		/* HTTP Basic Authentication */
		g_signal_connect(soup_connection, "authenticate", G_CALLBACK(network_cb_on_auth), services[i]);
		
		/* Verify cedentials */
		network_queue(API_LOGIN, network_cb_on_login, NULL);
		break;
	}
}


/* Logout current user */
void network_logout(void){
	network_new();
	if(current_timeline) {
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
	network_post( API_POST_STATUS, formdata, network_tweet_cb, g_strdup("Tweet") );
}//network_post_status


/* Send a direct message to a follower - text must be Url encoded  */
void network_send_message( const gchar *friend, const gchar *text ){
	gchar *formdata=g_strdup_printf( "source=%s&user=%s&text=%s", API_CLIENT_AUTH, friend, text );
	network_post( API_SEND_MESSAGE, formdata, network_tweet_cb, g_strdup("DM") );
}

void network_refresh(void){
	if(!current_timeline || processing)
		return;

	/* UI */
	app_set_statusbar_msg(_("Loading timeline..."));

	processing=TRUE;
	network_queue(current_timeline, network_cb_on_timeline, NULL );
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
	
	network_queue(uri_timeline, network_cb_on_timeline, g_strdup(uri_timeline) );
	/* network_queue's 3rd argument is used to trigger a new timeline & enables 'Refresh' */
}//network_get_timeline

/* Get a user timeline */
void network_get_user_timeline(const gchar *username){
	gchar *user_id;

	if(!username){
		gchar *prefs_auth_path=g_strdup_printf( PREFS_AUTH_USERNAME, API_SERVICE );
		conf_get_string(conf_get(), prefs_auth_path, &user_id);
		g_free(prefs_auth_path);
	} else
		user_id=g_strdup(username);

	if(!G_STR_EMPTY(user_id)) {
		gchar *user_timeline=g_strdup_printf( API_TIMELINE_USER, user_id );
		network_get_timeline( user_timeline );
		if(user_timeline) g_free( user_timeline );
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
	
	app_statusbar_printf("Please wait while %s loads the users %s...", PACKAGE_NAME, (get_friends?"who are following you":"you're following") );
	
	while(fetching){
		page++;
		uri=g_strdup_printf("%s?page=%d",(get_friends?API_FOLLOWING:API_FOLLOWERS), page);
		debug(DEBUG_DOMAIN, "Getting page %d of who%s.", page, (get_friends?"m the user is following":" is following the user") );
		msg=network_get( uri );
		fetching=network_get_users_page(msg);
		if(uri) g_free(uri); uri=NULL;
	}
	
	if(!all_users)
		app_set_statusbar_msg( _("Users parser error.") );
	else
		all_users=g_list_sort(all_users,(GCompareFunc) usrcasecmp);

	return all_users;
}//network_get_users_glist


void network_get_combined_timeline(void){
	/* SoupMessage *msg; */
	gchar *timelines[]={
		API_TIMELINE_FRIENDS,
		API_REPLIES,
		API_DIRECT_MESSAGES,
		NULL
	};
	
	for(int i=0; timelines[i]; i++){
		/* msg=network_get( timeline[i] ); */
	 }
	if(current_timeline) g_free(current_timeline);
	current_timeline=g_strdup("combined");
}//network_get_combined_timeline


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

	SoupMessage *msg=network_get_uri( image_uri );
		
	debug(DEBUG_DOMAIN, "Image response: %i", msg->status_code);

	/* check response */
	if(!((network_check_http( msg )) &&( g_file_set_contents( image_filename, msg->response_body->data, msg->response_body->length, NULL )) ))
		return FALSE;
	
	return TRUE;
}


void network_get_image(const gchar *image_uri, GtkTreeIter iter ){
	gchar *image_filename=NULL;
	/* TODO: fix - check if image already exists */
	if(g_file_test((image_filename=images_get_filename(image_uri)), G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)) {		
		/* Set image from file here */
		app_set_image(image_filename, iter);
		return;
	}
	
	Image *image=g_new0(Image, 1);
	image->src=image_filename;
	image->iter=iter;
	
	network_queue_uri( image_uri, network_cb_on_image, image );
}//network_get_image


/* HTTP Basic Authentication */
static void network_cb_on_auth(SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer user_data){
	OAuthService *service=(OAuthService *)user_data;
	/* Don't bother to continue if there is no user_id */
	if(G_STR_EMPTY(service->account->username))
		return debug(DEBUG_DOMAIN, "Authentication error: unknown username.");

	if(service->oauth_enabled){
		soup_auth_update(auth, msg, g_strdup_printf("OAuth realm=\"https://%s/\"", service->account->auth_uri));
		soup_auth_authenticate(auth, service->account->username, service->account->password);
	}
	/* verify that the password has been set */
	if(G_STR_EMPTY(service->account->password))
		return debug(DEBUG_DOMAIN, "Authentication error: unknown password.");
	
	if(service->oauth_enabled){
		gchar *request_uri=g_strdup_printf("https://%s%s", service->account->auth_uri, API_LOGIN);
		soup_auth_update(auth, msg, oauth_get_auth_headers(service, GET, request_uri));
		g_free(request_uri);
	}
	soup_auth_authenticate(auth, service->account->username, service->account->password);
}


/* On verify credentials */
static void network_cb_on_login(SoupSession *session, SoupMessage *msg, gpointer user_data){
	debug(DEBUG_DOMAIN, "Login response: %i",msg->status_code);
	
	timer_init();
	if(network_check_http( msg )) {
		debug(DEBUG_DOMAIN, "Loading default timeline");
		app_state_on_connection(TRUE);
		return;
	}
	
	timer_deinit();
	app_state_on_connection(FALSE);
}


/* On send a direct message */
static void network_tweet_cb(SoupSession *session, SoupMessage *msg, gpointer user_data){
	if(network_check_http( msg ))
		app_statusbar_printf("%s Sent", (const gchar *)user_data);
	
	if(in_reply_to_status_id) in_reply_to_status_id=0;
	
	debug(DEBUG_DOMAIN, "%s response: %i", ((const gchar *)user_data), msg->status_code);
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
		if(new_timeline) g_free(new_timeline); new_timeline=NULL;
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
			app_set_image(image->src,image->iter);
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

	conf_get_int(conf_get(), PREFS_TWEETS_RELOAD_TIMELINES, &minutes);

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
	network_queue(current_timeline, network_cb_on_timeline, NULL);

	timeout_id=0;

	return FALSE;
}
