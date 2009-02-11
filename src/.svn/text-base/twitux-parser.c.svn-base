/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
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
 *
 */

#include <config.h>

/*
 * Just make sure we include the prototype for strptime as well
 */
#define _XOPEN_SOURCE
#include <time.h>
#include <string.h> /* for g_memmove - memmove */

#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <libtwitux/twitux-debug.h>
#include <libtwitux/twitux-conf.h>

#include "twitux.h"
#include "twitux-app.h"
#include "twitux-network.h"
#include "twitux-parser.h"
#include "twitux-tweet-list.h"

#define DEBUG_DOMAIN_SETUP       "Parser" 

typedef struct 
{
	TwituxUser	*user;
	gchar		*text;
	gchar		*created_at;
	gchar		*id;
} TwituxStatus;

static TwituxUser	*parser_twitux_node_user   (xmlNode     *a_node);
static TwituxStatus	*parser_twitux_node_status (xmlNode     *a_node);
static xmlDoc       *parser_twitux_parse       (const char  *data,
												gssize       length,
												xmlNode    **first_element);
static gchar		*parser_convert_time       (const char	*datetime);
static gboolean      display_notification      (gpointer     tweet);

/* id of the newest tweet showed */
static gint			last_id = 0;

static xmlDoc*
parser_twitux_parse (const char  *data,
					 gssize       length,
					 xmlNode    **first_element)
{
	xmlDoc	*doc = NULL;
	xmlNode	*root_element = NULL;

	/* Read the XML */
	doc = xmlReadMemory (data, length, "xml", "UTF-8", 0);
	if (doc == NULL) {
		twitux_debug (DEBUG_DOMAIN_SETUP,
					  "failed to read xml data");
		return NULL;
	}

	/* Get first element */
	root_element = xmlDocGetRootElement (doc);
	if (root_element == NULL) {
		twitux_debug (DEBUG_DOMAIN_SETUP,
					  "failed getting first element of xml data");
		xmlFreeDoc (doc);
		return NULL;
	} else {
		*first_element = root_element;
	}

	return doc;
}


/* Parse a user-list XML ( friends, followers,... ) */
GList *
twitux_parser_users_list (const gchar *data,
						  gssize       length)
{
	xmlDoc		*doc = NULL;
	xmlNode		*root_element = NULL;
	xmlNode		*cur_node = NULL;

	GList		*friends = NULL;

	TwituxUser 	*user;

	/* parse the xml */
	doc = parser_twitux_parse (data, length, &root_element);

	if (!doc) {
		xmlCleanupParser ();
		return NULL;
	}

	/* get users */
	for (cur_node = root_element; cur_node; cur_node = cur_node->next) {
		if (cur_node->type != XML_ELEMENT_NODE)
			continue;
		if (g_str_equal (cur_node->name, "user")){
			/* parse user */
			user = parser_twitux_node_user (cur_node->children);
			/* add to list */
			friends = g_list_append (friends, user);
		} else if (g_str_equal (cur_node->name, "users")){
			cur_node = cur_node->children;
		}
	} /* End of loop */

	/* Free memory */
	xmlFreeDoc (doc);
	xmlCleanupParser ();

	return friends;
}


/* Parse a xml user node. Ex: add/del users responses */
TwituxUser *
twitux_parser_single_user (const gchar *data,
						   gssize       length)
{
	xmlDoc		*doc = NULL;
	xmlNode		*root_element = NULL;
	TwituxUser 	*user = NULL;
	
	/* parse the xml */
	doc = parser_twitux_parse (data, length, &root_element);

	if (!doc) {
		xmlCleanupParser ();
		return NULL;
	}

	if (g_str_equal (root_element->name, "user")) {
		user = parser_twitux_node_user (root_element->children);
	}

	/* Free memory */
	xmlFreeDoc (doc);
	xmlCleanupParser ();
	
	return user;
}

static gboolean
display_notification (gpointer tweet)
{
	twitux_app_notify (tweet);
	g_free (tweet);

	return FALSE;
}


/* Parse a timeline XML file */
gboolean
twitux_parser_timeline (const gchar *data, 
						gssize       length)
{
	xmlDoc		    *doc          = NULL;
	xmlNode		    *root_element = NULL;
	xmlNode		    *cur_node     = NULL;

	GtkListStore 	*store        = NULL;
	GtkTreeIter	     iter;

	TwituxStatus 	*status;

	/* Count new tweets */
	gboolean         show_notification = (last_id > 0);
	gint             lastTweet = 0;
    /*
	 * On multiple tweet updates we only want to 
	 * play the sound notification once.
	 */
    gboolean         multiple_new_tweets = FALSE;

	gboolean         show_username;
	gint             tweet_display_delay = 0;
	const int        tweet_display_interval = 5;

	/* parse the xml */
	doc = parser_twitux_parse (data, length, &root_element);

	if (!doc) {
		xmlCleanupParser ();
		return FALSE;
	}

	/* Get the twitux ListStore and clear previous */
	store = twitux_tweet_list_get_store ();
	gtk_list_store_clear (store);

	/* Show user names or real names */
	twitux_conf_get_bool (twitux_conf_get (),
						  TWITUX_PREFS_TWEETS_SHOW_NAMES,
						  &show_username);

	/* get tweets or direct messages */
	for (cur_node = root_element; cur_node; cur_node = cur_node->next) {
		if (cur_node->type != XML_ELEMENT_NODE)
			continue;
		/* Timelines and direct messages */
		if (g_str_equal (cur_node->name, "status") ||
		    g_str_equal (cur_node->name, "direct_message")) {
			gchar *tweet;
			gchar *datetime;
			gint   sid;

			/* Parse node */
			status = parser_twitux_node_status (cur_node->children);

			sid = atoi (status->id);
			
			/* the first tweet parsed is the 'newest' */
			if (lastTweet == 0){
				lastTweet = sid;
			}

			/* Create string for text column */
			datetime = parser_convert_time (status->created_at);
			tweet = g_strconcat ("<b>",
								 (show_username ? status->user->screen_name:status->user->name),
								 "</b> - ", datetime, "\n",
								 "<small>", status->text, "</small>",
								 NULL);
			
			if (sid > last_id && show_notification) {
				if (multiple_new_tweets != TRUE) {
					twitux_app_notify_sound ();
					multiple_new_tweets = TRUE;
				}
				g_timeout_add_seconds (tweet_display_delay,
									   display_notification,
									   g_strdup (tweet));

				tweet_display_delay += tweet_display_interval;
			}

			/* Append to ListStore */
			gtk_list_store_append (store, &iter);
			gtk_list_store_set (store, &iter,
								STRING_TEXT, tweet,
								STRING_AUTHOR, status->user->name,
								STRING_DATE, datetime,
								STRING_TWEET, status->text,
								STRING_USER, status->user->screen_name,
								-1);
			
			/* Free the text column string */
			g_free (tweet);

			/* Get Image */
			twitux_network_get_image (status->user->image_url,
									  iter);

			/* Free struct */
			parser_free_user (status->user);
			if (status->text)
				g_free (status->text);
			if (status->created_at)
				g_free (status->created_at);
			if (status->id)
				g_free (status->id);
			
			g_free (status);
			g_free (datetime);
		} else if (g_str_equal (cur_node->name, "statuses") ||
			g_str_equal (cur_node->name, "direct-messages")) {

			cur_node = cur_node->children;
		}

	} /* end of loop */

	/* Remember last id showed */
	if (lastTweet > 0) {
		last_id = lastTweet;
	}

	/* Free memory */
	xmlFreeDoc (doc);
	xmlCleanupParser ();

	return TRUE;
}

static TwituxUser *
parser_twitux_node_user (xmlNode *a_node)
{
	xmlNode		   *cur_node = NULL;
	xmlBufferPtr	buffer;
	TwituxUser     *user;

	buffer = xmlBufferCreate ();
	user = g_new0 (TwituxUser, 1);

	/* Begin 'users' node loop */
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type != XML_ELEMENT_NODE)
			continue;
		if (xmlNodeBufGetContent (buffer, cur_node) != 0)
			continue;
		if (g_str_equal (cur_node->name, "screen_name" )) {
			const xmlChar *tmp;

			tmp = xmlBufferContent (buffer);
			user->screen_name = g_strdup ((const gchar *)tmp);

		} else if (g_str_equal (cur_node->name, "name" )) {
			const xmlChar *tmp;

			tmp = xmlBufferContent (buffer);
			user->name = g_strdup ((const gchar *)tmp);

		} else if (g_str_equal (cur_node->name, "profile_image_url")) {
			const xmlChar *tmp;

			tmp = xmlBufferContent (buffer);
			user->image_url = g_strdup ((const gchar *)tmp);

		}

		/* Free buffer content */
		xmlBufferEmpty (buffer);

	} /* End of loop */

	/* Free buffer pointer */
	xmlBufferFree (buffer);

	return user;
}


static TwituxStatus *
parser_twitux_node_status (xmlNode *a_node)
{
	xmlNode		   *cur_node = NULL;
	xmlBufferPtr	buffer;
	TwituxStatus   *status;

	buffer = xmlBufferCreate ();
	status = g_new0 (TwituxStatus, 1);

	/* Begin 'status' or 'direct-messages' loop */
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type != XML_ELEMENT_NODE)
			continue;
		if (xmlNodeBufGetContent (buffer, cur_node) != 0)
			continue;
		if (g_str_equal (cur_node->name, "created_at")) {
			const xmlChar *tmp;
			tmp = xmlBufferContent(buffer);
			status->created_at = g_strdup ((const gchar *)tmp);
		} else if (g_str_equal (cur_node->name, "id")) {
			const xmlChar *tmp;

			tmp = xmlBufferContent(buffer);
			status->id = g_strdup ((const gchar *)tmp);
		} else if (g_str_equal (cur_node->name, "text")) {
			const xmlChar *msg;
			gchar *cur;

			msg = xmlBufferContent (buffer);

			status->text = g_markup_escape_text ((const char *)msg, -1);

			/* &amp;lt; becomes &lt; */
			cur = status->text;
			while ((cur = strstr (cur, "&amp;"))) {
				if (strncmp (cur + 5, "lt;", 3) == 0 || strncmp (cur + 5, "gt;", 3) == 0)
					g_memmove (cur + 1, cur + 5, strlen (cur + 5) + 1);
				else
					cur += 5;
			}

		} else if (g_str_equal (cur_node->name, "sender") ||
				   g_str_equal (cur_node->name, "user")) {

			status->user = parser_twitux_node_user (cur_node->children);
		}

		/* Free buffer content */
		xmlBufferEmpty (buffer);

	} /* End of loop */

	/* Free buffer pointer */
	xmlBufferFree (buffer);
	
	return status;
}

static gchar *
parser_convert_time (const char *datetime)
{
	struct tm	*ta;
	struct tm	 post;
	int			 seconds_local;
	int			 seconds_post;
	int 		 diff;
	char        *oldenv;
	time_t		 t = time(NULL);

	tzset ();

	ta = gmtime (&t);
	ta->tm_isdst = -1;
	seconds_local = mktime (ta);

	oldenv = setlocale (LC_TIME, "C");
	strptime (datetime, "%a %b %d %T +0000 %Y", &post);
	post.tm_isdst = -1;
	seconds_post =  mktime (&post);

	setlocale (LC_TIME, oldenv);

	diff = difftime (seconds_local, seconds_post);
	
	if (diff < 2) {
		return g_strdup (_("1 second ago"));
	}
	/* Seconds */
	if (diff < 60 ) {
		return g_strdup_printf (_("%i seconds ago"), diff);
	} else if (diff < 120) {
		return g_strdup (_("1 minute ago"));
	} else {
		/* Minutes */
		diff = diff/60;
		if (diff < 60) {
			return g_strdup_printf (_("%i minutes ago"), diff);
		} else if (diff < 120) {
			return g_strdup (_("1 hour ago"));
		} else {
			/* Hours */
			diff = diff/60;
			if (diff < 24) {
				return g_strdup_printf (_("%i hours ago"), diff);
			} else if (diff < 48) {
				return g_strdup (_("1 day ago"));
			} else {
				/* Days */
				diff = diff/24;
				if (diff < 30) {
					return g_strdup_printf (_("%i days ago"), diff);
				} else if (diff < 60) {
					return g_strdup (_("1 month ago"));
				} else {
					return g_strdup_printf (_("%i months ago"), (diff/30));
				}
			}
		}
	}
	return NULL;
}

/* Free a user struct */
void
parser_free_user (TwituxUser *user)
{
	if (!user)
		return;

	if (user->screen_name)
		g_free (user->screen_name);
	if (user->name)
		g_free (user->name);
	if (user->image_url)
		g_free (user->image_url);

	g_free (user);
}


void
parser_reset_lastid ()
{
	last_id = 0;
}
