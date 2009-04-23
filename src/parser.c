/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
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
 *
 */

#include <config.h>

/*
 * Just make sure we include the prototype for strptime as well
 */
#define _XOPEN_SOURCE
#include <time.h>
#include <string.h> /* for g_memmove - memmove */
#include <strings.h>

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "debug.h"
#include "gconf.h"

#include "main.h"
#include "app.h"
#include "network.h"
#include "parser.h"
#include "tweet-list.h"
#include "images.h"
#include "users.h"

#define DEBUG_DOMAIN_SETUP       "Parser" 

typedef struct {
	User		*user;
	gchar		*text;
	gchar		*created_at;
	gchar		*id;
} Status;

static Status *parser_node_status(xmlNode *a_node);
static gchar *parser_convert_time(const char *datetime);
static gboolean display_notification( gpointer tweet );

/* id of the newest tweet showed */
static unsigned long int last_id=0;

xmlDoc *parser_parse(const char *data, gssize length, xmlNode **first_element){
	xmlDoc* doc = NULL;
	xmlNode* root_element = NULL;

	/* Read the XML */
	doc = xmlReadMemory(data, length, "xml", "UTF-8", 0);
	if(doc == NULL) {
		debug(DEBUG_DOMAIN_SETUP,
					  "failed to read xml data");
		return NULL;
	}

	/* Get first element */
	root_element = xmlDocGetRootElement(doc);
	if(root_element == NULL) {
		debug(DEBUG_DOMAIN_SETUP,
					  "failed getting first element of xml data");
		xmlFreeDoc(doc);
		return NULL;
	} else {
		*first_element = root_element;
	}

	return doc;
}


static gboolean display_notification(gpointer tweet){
	app_notify(tweet);
	if(tweet) g_free(tweet);
	return FALSE;
}


/* Parse a timeline XML file */
gboolean parser_timeline(const gchar *data, gssize length){
	xmlDoc		    *doc          = NULL;
	xmlNode		    *root_element = NULL;
	xmlNode		    *cur_node     = NULL;

	GtkListStore 	*store        = NULL;
	GtkTreeIter	     iter;

	Status 	*status;

	/* Count new tweets */
	gboolean		show_notification =(last_id > 0);
	unsigned long int	last_tweet = 0;
	/*
	 * On multiple tweet updates we only want to 
	 * play the sound notification once.
	 */
	gboolean         multiple_new_tweets = FALSE;

	gint             tweet_display_delay = 0;
	const int        tweet_display_interval = 5;

	/* parse the xml */
	doc = parser_parse(data, length, &root_element);

	if(!doc) {
		xmlCleanupParser();
		return FALSE;
	}

	/* Get the ListStore and clear previous */
	store = tweet_list_get_store();
	gtk_list_store_clear(store);

	/* get tweets or direct messages */
	for(cur_node = root_element; cur_node; cur_node = cur_node->next) {
		if(cur_node->type != XML_ELEMENT_NODE ) continue;
		if( g_str_equal(cur_node->name, "statuses") ||	g_str_equal(cur_node->name, "direct-messages") ){
			cur_node = cur_node->children;
			continue;
		}


		if((!g_str_equal(cur_node->name, "status")) && (!g_str_equal(cur_node->name, "direct_message")) )
			continue;
		
		/* Timelines and direct messages */
		gchar *tweet;
		gchar *datetime;
		unsigned long int   sid;
		
		/* Parse node */
		status=parser_node_status(cur_node->children);
		sid=strtoul( status->id, NULL, 10 );
		
		/* the first tweet parsed is the 'newest' */
		if(last_tweet == 0)
			last_tweet = sid;

		/* Create string for text column */
		datetime=parser_convert_time( status->created_at );
		tweet=g_strconcat(
					"<b>",	status->user->nick_name, " ( @",  status->user->user_name, " )", "</b> - ", datetime, "\n",
					"<small>", status->text, "</small>",
					NULL
		);
		
		if(sid > last_id && show_notification) {
			if(multiple_new_tweets != TRUE) {
				app_notify_sound();
				multiple_new_tweets = TRUE;
			}
			g_timeout_add_seconds(tweet_display_delay,
								   display_notification,
								   g_strdup(tweet));

			tweet_display_delay += tweet_display_interval;
		}

		/* Append to ListStore */
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter,
							STRING_TEXT, tweet,
							STRING_NICK,status->user->nick_name,
							STRING_DATE, datetime,
							STRING_TWEET, status->text,
							STRING_USER, status->user->user_name,
							ULONG_TWEET_ID, (guint)sid,
							-1);
		
		/* Free the text column string */
		g_free(tweet);

		/* Get Image */
		network_get_image(status->user->image_url, iter);

		/* Free struct */
		user_free(status->user);
		if(status->text) g_free(status->text);
		if(status->created_at) g_free(status->created_at);
		if(status->id) g_free(status->id);
		
		g_free(status);
		g_free(datetime);
	} /* end of loop */

	/* Remember last id showed */
	if(last_tweet > 0)
		last_id=last_tweet;

	/* Free memory */
	xmlFreeDoc(doc);
	xmlCleanupParser();

	return TRUE;
}

static Status *parser_node_status(xmlNode *a_node){
	xmlNode		   *cur_node = NULL;
	xmlBufferPtr	buffer;
	Status   *status;

	buffer = xmlBufferCreate();
	status = g_new0(Status, 1);

	/* Begin 'status' or 'direct-messages' loop */
	for(cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if(cur_node->type != XML_ELEMENT_NODE)
			continue;
		if(xmlNodeBufGetContent(buffer, cur_node) != 0)
			continue;
		if(g_str_equal(cur_node->name, "created_at")) {
			const xmlChar *tmp;
			tmp = xmlBufferContent(buffer);
			status->created_at = g_strdup((const gchar *)tmp);
		} else if(g_str_equal(cur_node->name, "id")) {
			const xmlChar *tmp;

			tmp = xmlBufferContent(buffer);
			status->id = g_strdup((const gchar *)tmp);
		} else if(g_str_equal(cur_node->name, "text")) {
			const xmlChar *msg;
			gchar *cur;

			msg = xmlBufferContent(buffer);

			status->text = g_markup_escape_text((const char *)msg, -1);

			/* &amp;lt; becomes &lt; */
			cur = status->text;
			while((cur = strstr(cur, "&amp;"))) {
				if(strncmp(cur + 5, "lt;", 3) == 0 || strncmp(cur + 5, "gt;", 3) == 0)
					g_memmove(cur + 1, cur + 5, strlen(cur + 5) + 1);
				else
					cur += 5;
			}

		} else if(g_str_equal(cur_node->name, "sender") ||
			g_str_equal(cur_node->name, "user")) {
			status->user=user_parse_profile(cur_node->children);
		}

		/* Free buffer content */
		xmlBufferEmpty(buffer);

	} /* End of loop */

	/* Free buffer pointer */
	xmlBufferFree(buffer);
	
	return status;
}

static gchar *parser_convert_time(const char *datetime){
	struct tm	*ta;
	struct tm	 post;
	int			 seconds_local;
	int			 seconds_post;
	int 		 diff;
	char        *oldenv;
	time_t		 t = time(NULL);

	tzset();

	ta = gmtime(&t);
	ta->tm_isdst = -1;
	seconds_local = mktime(ta);

	oldenv = setlocale(LC_TIME, "C");
	strptime(datetime, "%a %b %d %T +0000 %Y", &post);
	post.tm_isdst = -1;
	seconds_post =  mktime(&post);

	setlocale(LC_TIME, oldenv);

	diff = difftime(seconds_local, seconds_post);
	
	if(diff < 2) {
		return g_strdup(_("1 second ago"));
	}
	/* Seconds */
	if(diff < 60 ) {
		return g_strdup_printf(_("%i seconds ago"), diff);
	} else if(diff < 120) {
		return g_strdup(_("1 minute ago"));
	} else {
		/* Minutes */
		diff = diff/60;
		if(diff < 60) {
			return g_strdup_printf(_("%i minutes ago"), diff);
		} else if(diff < 120) {
			return g_strdup(_("1 hour ago"));
		} else {
			/* Hours */
			diff = diff/60;
			if(diff < 24) {
				return g_strdup_printf(_("%i hours ago"), diff);
			} else if(diff < 48) {
				return g_strdup(_("1 day ago"));
			} else {
				/* Days */
				diff = diff/24;
				if(diff < 30) {
					return g_strdup_printf(_("%i days ago"), diff);
				} else if(diff < 60) {
					return g_strdup(_("1 month ago"));
				} else {
					return g_strdup_printf(_("%i months ago"),(diff/30));
				}
			}
		}
	}
	return NULL;
}


void parser_reset_lastid(){
	last_id=0;
}
