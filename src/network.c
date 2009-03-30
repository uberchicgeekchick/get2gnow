/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2007-2009 Daniel Morales <daniminas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
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
#include "app.h"
#include "send-message-dialog.h"
#include "followers-dialog.h"

#define DEBUG_DOMAIN	  "Network"

typedef struct {
	gchar        *src;
	GtkTreeIter   iter;
} Image;

static void network_get_data( const gchar *url, SoupSessionCallback callback, gpointer data);
static void network_post_data( const gchar *url, gchar *formdata, SoupSessionCallback callback, gpointer data );
static gboolean	network_check_http( SoupMessage *msg );
static void network_parser_free_lists (void);

/* libsoup callbacks */
static void network_cb_on_login( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_cb_on_post( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_cb_on_message( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_cb_on_timeline( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_cb_on_image( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_cb_on_add( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_cb_on_del( SoupSession *session, SoupMessage *msg, gpointer user_data );
static void network_cb_on_auth( SoupSession *session, SoupMessage *msg, SoupAuth *auth, gboolean retrying, gpointer data );

/* Copyright (C) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com> */
static gboolean network_get_users_page(SoupMessage *msg);
/* My, Kaity G. B., new stuff ends here. */

/* Autoreload timeout functions */
static gboolean 	network_timeout			(gpointer user_data);
static void			network_timeout_new		(void);

static SoupSession			*soup_connection = NULL;
static GList				*user_friends = NULL;
static GList				*user_followers = NULL;
static GList *all_users=NULL;
static gboolean				 processing = FALSE;
static gchar				*current_timeline = NULL;
static guint				 timeout_id;
static gchar                *global_username = NULL;
static gchar                *global_password = NULL;

/* This function must be called at startup */
void
network_new (void)
{
	Conf	*conf;
	gboolean	check_proxy = FALSE;
 
	/* Close previous networking */
	if (soup_connection) {
		network_close ();
	}

	/* Async connections */
	soup_connection = soup_session_async_new_with_options (SOUP_SESSION_MAX_CONNS,
														   8,
														   NULL);

	debug (DEBUG_DOMAIN, "Libsoup (re)started");

	/* Set the proxy, if configuration is set */
	conf = conf_get ();
	conf_get_bool (conf,
						  PROXY_USE,
						  &check_proxy);

	if (check_proxy) {
		gchar *server, *proxy_uri;
		gint port;

		/* Get proxy */
		conf_get_string (conf,
								PROXY_HOST,
								&server);
		conf_get_int (conf,
							 PROXY_PORT,
							 &port);

		if (server && server[0]) {
			SoupURI *suri;

			check_proxy = FALSE;
			conf_get_bool (conf,
								  PROXY_USE_AUTH,
								  &check_proxy);

			/* Get proxy auth data */
			if (check_proxy) {
				char *user, *password;

				conf_get_string (conf,
										PROXY_USER,
										&user);
				conf_get_string (conf,
										PROXY_PASS,
										&password);

				proxy_uri = g_strdup_printf ("http://%s:%s@%s:%d",
											 user,
											 password,
											 server,
											 port);

				g_free (user);
				g_free (password);
			} else {
				proxy_uri = g_strdup_printf ("http://%s:%d", 
											 server, port);
			}

			debug (DEBUG_DOMAIN, "Proxy uri: %s",
						  proxy_uri);

			/* Setup proxy info */
			suri = soup_uri_new (proxy_uri);
			g_object_set (G_OBJECT (soup_connection),
						  SOUP_SESSION_PROXY_URI,
						  suri,
						  NULL);

			soup_uri_free (suri);
			g_free (server);
			g_free (proxy_uri);
		}
	}
}


/* Cancels requests, and unref libsoup. */
void
network_close (void)
{
	/* Close all connections */
	network_stop ();

	network_parser_free_lists ();

	g_object_unref (soup_connection);

	if (current_timeline) {
		g_free (current_timeline);
		current_timeline = NULL;
	}

	debug (DEBUG_DOMAIN, "Libsoup closed");
}


/* Cancels all pending requests in session. */
void
network_stop	(void)
{
	debug (DEBUG_DOMAIN,"Cancelled all connections");

	soup_session_abort (soup_connection);
}


/* Login in Twitter */
void
network_login (const char *username, const char *password)
{
	debug (DEBUG_DOMAIN, "Begin login.. ");

	g_free (global_username);
	global_username = g_strdup (username);
	g_free (global_password);
	global_password = g_strdup (password);

	app_set_statusbar_msg (_("Connecting..."));

	/* HTTP Basic Authentication */
	g_signal_connect (soup_connection,
					  "authenticate",
					  G_CALLBACK (network_cb_on_auth),
					  NULL);

	/* Verify cedentials */
	network_get_data (API_TWITTER_LOGIN, network_cb_on_login, NULL);
}


/* Logout current user */
void network_logout (void)
{
	network_new ();
	
	debug (DEBUG_DOMAIN, "Logout");
}


/* Post a new tweet - text must be Url encoded */
void network_post_status (const gchar *text){
	network_post_data( API_TWITTER_POST_STATUS, (g_strdup_printf( "source=%s&status=%s", API_TWITTER_CLIENT_AUTH, text )), network_cb_on_post, NULL );
}


/* Send a direct message to a follower - text must be Url encoded  */
void
network_send_message (const gchar *friend,
							 const gchar *text)
{
	gchar *formdata;

	formdata = g_strdup_printf ( "user=%s&text=%s", friend, text);
	
	network_post_data (API_TWITTER_SEND_MESSAGE,
					   formdata,
					   network_cb_on_message,
					   NULL);
}

void
network_refresh (void)
{
	if (!current_timeline || processing)
		return;

	/* UI */
	app_set_statusbar_msg (_("Loading timeline..."));

	processing = TRUE;
	network_get_data (current_timeline, network_cb_on_timeline, NULL);
}

/* Get and parse a timeline */
void
network_get_timeline (const gchar *url_timeline)
{
	if (processing)
		return;

	parser_reset_lastid();

	/* UI */
	app_set_statusbar_msg(_("Loading timeline..."));

	processing=TRUE;
	network_get_data(url_timeline, network_cb_on_timeline, g_strdup(url_timeline));
}

/* Get a user timeline */
void
network_get_user (const gchar *username)
{
	gchar *user_timeline;
	gchar *user_id;

	if (!username){
		conf_get_string (conf_get (),
								PREFS_AUTH_USER_ID,
								&user_id);
	} else {
		user_id = g_strdup (username);
	}

	if(!G_STR_EMPTY (user_id)) {
		user_timeline = g_strdup_printf (API_TWITTER_TIMELINE_USER,
										 user_id);
	
		network_get_timeline (user_timeline);
		g_free (user_timeline);
	}

	if(user_id) g_free(user_id);
}//network_get_timeline

/* Begin: (C) 2009 Kaity G. B. <uberChick@uberChitGeekChick.Com> */
OnlineProfile *network_get_user_profile(const gchar *username){
	OnlineProfile *profile=g_new0(OnlineProfile, 1);

	return profile;
}//OnlineProfile *network_get_user_profile(const gchar *username);

/* Get authenticating user's friends(following)
 * Returns:
 * 		NULL: Friends will be fetched
 * 		GList: The list of friends (fetched previously)
 */
GList *network_get_friends (void){
	if(user_friends) return user_friends;
	user_friends=network_get_users_glist((gboolean)TRUE);
	followers_dialog_load_lists(user_friends);
	return NULL;
}


/* Get the authenticating user's followers
 * Returns:
 * 		NULL: Followers will be fetched
 * 		GList: The list of friends (fetched previously)
 */
GList *network_get_followers (void){
	if(user_followers) return user_followers;
	user_followers=network_get_users_glist((gboolean)FALSE);	
	message_set_followers(user_followers);
	return NULL;
}


GList *network_get_users_glist(gboolean get_friends){
	GTimer *request_timer=g_timer_new();// request_timer is used to avoid
	guint request_microseconds=1;
	SoupMessage *msg;
	gint page=0, remaining_requests;
	
	all_users=NULL;
	gboolean fetching=TRUE;
	while(fetching){
		page++;
		msg=soup_message_new( "GET", (g_strdup_printf("%s?page=%d", (get_friends?API_TWITTER_FOLLOWING:API_TWITTER_FOLLOWERS), page)) );
		soup_session_send_message(soup_connection, msg);
		fetching=network_get_users_page(msg);
		remaining_requests=atoi( (soup_message_headers_get(msg->response_headers, "X-RateLimit-Remaining")) );
		g_timer_start(request_timer);
		if( remaining_requests && remaining_requests < 2 )
			while( 30 > (g_timer_elapsed(request_timer, &request_microseconds)) ){}
		else
			while( 5 > (g_timer_elapsed(request_timer, &request_microseconds)) ){}
		g_timer_stop(request_timer);
	}
	g_timer_destroy(request_timer);
	if(!all_users)
		app_set_statusbar_msg( _("Users parser error.") );
	else
		all_users=g_list_sort(all_users, (GCompareFunc) parser_sort_users);

	return all_users;
}//network_get_users_glist


void network_get_combined_timeline(void){
	SoupMessage *msg;
	const gchar *timelines[]={
		API_TWITTER_TIMELINE_FRIENDS,
		API_TWITTER_REPLIES,
		API_TWITTER_DIRECT_MESSAGES,
		NULL
	};
	
	for(int i=0; timelines[i]; i++)
		network_get_data(timelines[i], network_cb_on_timeline, g_strdup(timelines[i]));
	/*{	msg=soup_message_new("GET", timeline[i]);
	 * 	soup_session_send_message(soup_connection, msg);
	 }*/
	current_timeline=g_strdup("combined");
}//network_get_combined_timeline


static gboolean network_get_users_page(SoupMessage *msg){
	debug(DEBUG_DOMAIN, "Users response: %i",msg->status_code);
	
	/* Check response */
	if (!network_check_http( msg ))
		return (gboolean)FALSE;

	/* parse user list */
	debug(DEBUG_DOMAIN, "Parsing user list");
	GList *new_users;
	if(! (new_users=parser_users_list(msg->response_body->data, msg->response_body->length)) )
		return (gboolean)FALSE;
	
	if(!all_users)
		all_users=new_users;
	else
		all_users=g_list_concat(all_users, new_users);
	
	return TRUE;
}//network_get_users_page


/* Get an image from servers */
void network_get_image (const gchar  *url_image, GtkTreeIter   iter){
	gchar *image_file, **image_name_info;
	
	Image *image;
	
	/* save using the filename */
	image_name_info=g_strsplit(url_image, (const gchar *)"/", 7);
	if( image_name_info[5] && image_name_info[6] )
		image_file=g_strconcat(image_name_info[5], "_", image_name_info[6], NULL);
	else
		image_file="unknown_image";
	
	if(image_name_info)
		g_strfreev(image_name_info);
	
	image_file=g_build_filename( g_get_home_dir(), ".gnome2", CACHE_IMAGES, image_file, NULL );
	
	/* TODO: fix - check if image already exists */
	if (g_file_test(image_file, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)) {		
		/* Set image from file here */
		app_set_image(image_file, iter);
		g_free(image_file);
		return;
	}

	image=g_new0(Image, 1);
	image->src=g_strdup(image_file);
	image->iter=iter;
	
	g_free(image_file);
	
	network_get_data(url_image, network_cb_on_image, image);
}



/* Add a user to follow */
void
network_add_user (const gchar *username)
{
	gchar *url;
	
	if((G_STR_EMPTY(username)))
		return;
	
	url = g_strdup_printf (API_TWITTER_FOLLOWING_ADD, username);

	network_post_data (url, NULL, network_cb_on_add, NULL);

	g_free (url);
}


/* Add a user to follow */
void
network_del_user(const gchar *username)
{
	gchar *url;
	
	if((G_STR_EMPTY(username)))
		return;
	
	url = g_strdup_printf (API_TWITTER_FOLLOWING_DEL, username);

	network_post_data (url, NULL, network_cb_on_del, NULL);

	g_free (url);
}


/* Get data from net */
static void
network_get_data (const gchar           *url,
				  SoupSessionCallback    callback,
				  gpointer               data)
{
	SoupMessage *msg;

	debug (DEBUG_DOMAIN, "Get: %s",url);

	msg = soup_message_new ( "GET", url );

	soup_session_queue_message (soup_connection, msg, callback, data);
}


/* Private: Post data to net */
static void
network_post_data (const gchar           *url,
				   gchar                 *formdata,
				   SoupSessionCallback    callback,
				   gpointer               data)
{
	SoupMessage *msg;

	debug( DEBUG_DOMAIN, "Post: %s",url );

	msg=soup_message_new( "POST", url );
	
	soup_message_headers_append( msg->request_headers, "X-Twitter-Client", PACKAGE_NAME);
	soup_message_headers_append( msg->request_headers, "X-Twitter-Client-Version", PACKAGE_VERSION);

	if (formdata)
		soup_message_set_request(
						msg, 
						"application/x-www-form-urlencoded",
						SOUP_MEMORY_TAKE,
						formdata,
						strlen( formdata )
		);

	soup_session_queue_message (soup_connection, msg, callback, data);
}


/* Check HTTP response code */
static gboolean network_check_http( SoupMessage *msg ){
	const gchar *error=NULL;
	if(msg->status_code == 401)
		error=_("Access denied");
	else if(SOUP_STATUS_IS_CLIENT_ERROR(msg->status_code))
		error=_("HTTP communication error");
	else if(SOUP_STATUS_IS_SERVER_ERROR(msg->status_code))
		error=_("Internal server error");
	else if(!SOUP_STATUS_IS_SUCCESSFUL(msg->status_code))
		error=_("Stopped");

	else
		return TRUE;
	
	gchar *http_message=g_strdup_printf("%s: %s.", error, msg->reason_phrase);
	app_set_statusbar_msg( http_message );
	g_free( http_message );
	
	return FALSE;
}

/* Callback to free every element on a User list */
static void network_free_user_list_each( gpointer user, gpointer data ){
	if (!user)
		return;
	parser_free_user( (User *)user );
}


/* Free a list of Users */
static void
network_parser_free_lists ()
{
	if (user_friends) {
		g_list_foreach (user_friends, network_free_user_list_each, NULL);
		g_list_free (user_friends);
		user_friends = NULL;
		debug (DEBUG_DOMAIN, "Friends freed");
	}

	if (user_followers) {
		g_list_foreach (user_followers, network_free_user_list_each, NULL);
		g_list_free (user_followers);
		user_followers = NULL;
		debug (DEBUG_DOMAIN, "Followers freed");
	}
}



/* HTTP Basic Authentication */
static void
network_cb_on_auth (SoupSession  *session,
					SoupMessage  *msg,
					SoupAuth     *auth,
					gboolean      retrying,
					gpointer      data)
{
	/* Don't bother to continue if there is no user_id */
	if (G_STR_EMPTY (global_username)) {
		return;
	}

	/* verify that the password has been set */
	if (!G_STR_EMPTY (global_password))
		soup_auth_authenticate (auth, global_username, 
								global_password);
}


/* On verify credentials */
static void
network_cb_on_login (SoupSession *session,
					 SoupMessage *msg,
					 gpointer     user_data)
{
	debug (DEBUG_DOMAIN,
				  "Login response: %i",msg->status_code);

	if (network_check_http( msg )) {
		app_state_on_connection (TRUE);
		return;
	}

	app_state_on_connection (FALSE);
}


/* On post a tweet */
static void
network_cb_on_post (SoupSession *session,
					SoupMessage *msg,
					gpointer     user_data)
{
	if (network_check_http( msg )) {
		app_set_statusbar_msg (_("Status Sent"));
	}
	
	debug (DEBUG_DOMAIN,
				  "Tweet response: %i",msg->status_code);
}


/* On send a direct message */
static void
network_cb_on_message (SoupSession *session,
					   SoupMessage *msg,
					   gpointer     user_data)
{
	if (network_check_http( msg )) {
		app_set_statusbar_msg (_("Message Sent"));
	}

	debug (DEBUG_DOMAIN,
				  "Message response: %i",msg->status_code);
}


/* On get a timeline */
static void
network_cb_on_timeline (SoupSession *session,
						SoupMessage *msg,
						gpointer     user_data)
{
	gchar        *new_timeline = NULL;
	
	if (user_data){
		new_timeline = (gchar *)user_data;
	}

	debug (DEBUG_DOMAIN,
				  "Timeline response: %i",msg->status_code);

	processing = FALSE;

	/* Timeout */
	network_timeout_new ();

	/* Check response */
	if (!network_check_http( msg )) {
		if (new_timeline)
			g_free (new_timeline);
		return;
	}

	debug (DEBUG_DOMAIN, "Parsing timeline");

	/* Parse and set ListStore */
	if (parser_timeline (msg->response_body->data, msg->response_body->length)) {
		app_set_statusbar_msg (_("Timeline Loaded"));

		if (new_timeline){
			if (current_timeline)
				g_free (current_timeline);
			current_timeline = g_strdup (new_timeline);
		}
	} else {
		app_set_statusbar_msg (_("Timeline Parser Error."));
	}

	if (new_timeline)
		g_free (new_timeline);
}


/* On get a image */
static void
network_cb_on_image (SoupSession *session,
					 SoupMessage *msg,
					 gpointer     user_data)
{
	Image *image = (Image *)user_data;

	debug (DEBUG_DOMAIN,
				  "Image response: %i", msg->status_code);

	/* check response */
	if (network_check_http( msg )) {
		/* Save image data */
		if (g_file_set_contents (image->src,
								 msg->response_body->data,
								 msg->response_body->length,
								 NULL)) {
			/* Set image from file here (image_file) */
			app_set_image (image->src,image->iter);
		}
	}

	g_free (image->src);
	g_free (image);
}


/* On add a user */
static void
network_cb_on_add (SoupSession *session,
				   SoupMessage *msg,
				   gpointer     user_data)
{
	User *user;

	debug (DEBUG_DOMAIN,
				  "Add user response: %i", msg->status_code);
	
	/* Check response */
	if (!network_check_http( msg ))
		return;

	/* parse new user */
	debug (DEBUG_DOMAIN, "Parsing new user");

	user = parser_single_user (msg->response_body->data,
									  msg->response_body->length);

	if (user) {
		user_friends = g_list_append ( user_friends, user );
		app_set_statusbar_msg (_("Friend Added"));
	} else {
		app_set_statusbar_msg (_("Failed to add user"));
	}
}


/* On remove a user */
static void
network_cb_on_del (SoupSession *session,
				   SoupMessage *msg,
				   gpointer     user_data)
{
	debug (DEBUG_DOMAIN,
				  "Delete user response: %i", msg->status_code);

	if (network_check_http( msg )) {		
		app_set_statusbar_msg (_("Friend Removed"));
	} else {
		app_set_statusbar_msg (_("Failed to remove user"));
	}
	
	if (user_data) {
		User *user = (User *)user_data;
		user_friends = g_list_remove (user_friends, user);
		parser_free_user (user);
	}
}

static void
network_timeout_new (void)
{
	gint minutes;
	guint reload_time;

	if (timeout_id) {
		debug (DEBUG_DOMAIN,
					  "Stopping timeout id: %i", timeout_id);

		g_source_remove (timeout_id);
	}

	conf_get_int (conf_get (),
						 PREFS_TWEETS_RELOAD_TIMELINES,
						 &minutes);

	/* The timeline reload interval shouldn't be less than 3 minutes */
	if (minutes < 3) {
		minutes = 3;
	}

	/* This should be the number of milliseconds */
	reload_time = minutes * 60 * 1000;

	timeout_id = g_timeout_add (reload_time,
								network_timeout,
								NULL);

	debug (DEBUG_DOMAIN,
				  "Starting timeout id: %i", timeout_id);
}

static gboolean
network_timeout (gpointer user_data)
{
	if (!current_timeline || processing)
		return FALSE;

	/* UI */
	app_set_statusbar_msg (_("Reloading timeline..."));

	debug (DEBUG_DOMAIN,
				  "Auto reloading. Timeout: %i", timeout_id);

	processing = TRUE;
	network_get_data (current_timeline, network_cb_on_timeline, NULL);

	timeout_id = 0;

	return FALSE;
}
