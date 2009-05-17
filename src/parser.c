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

#include "config.h"

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

#include <libsoup/soup-message.h>

#include "main.h"
#include "network.h"
#include "online-services.h"
#include "users.h"

#include "app.h"
#include "tweet-list.h"
#include "preferences.h"
#include "images.h"
#include "label.h"
#include "gconfig.h"

#include "parser.h"

#define	DEBUG_DOMAINS	"Parser:Requests:OnlineServices:Tweets:UI"
#include "debug.h"

typedef struct {
	User	*user;
	gchar	*text;
	gchar	*created_at_str;
	guint	created_at;
	guint	id;
} Status;

static Status *parser_node_status(OnlineService *service, xmlNode *a_node);
static void parser_node_status_free(Status *status);
static gchar *parser_convert_time(const char *datetime);
static xmlDoc *parser_parse_dom_content(SoupMessage *xml);


/* id of the newest tweet showed */
static unsigned long int last_id=0;




gchar *parser_get_cache_file_from_uri(const gchar *uri){
	return g_strdelimit(g_strdup(uri), ":/&?", '_');
}/* parser_get_cache_file_from_uri */


static xmlDoc *parser_parse_dom_content(SoupMessage *xml){
	xmlDoc *doc=NULL;
	
	SoupURI	*suri=NULL;
	gchar *uri=NULL;
	if(!( (suri=soup_message_get_uri(xml)) && (uri=soup_uri_to_string(suri, FALSE)) )){
		debug("*WARNING*: Unknown XML document URI.");
		uri=g_strdup("[*unknown*]");
	}
	if(suri) soup_uri_free(suri);
	
	
	if(!( xml->response_body && xml->response_body->data && xml->response_body->length )){
		debug("**ERROR**: Cannot parse empty or xml resonse from: %s.", uri);
		g_free(uri);
		return NULL;
	}
	
	
	gchar **content_v=NULL;
	debug("Parsing xml document's content-type & DOM information from:\n\t\t%s", uri);
	gchar *content_info=NULL;
	if(!(content_info=(gchar *)soup_message_headers_get_one(xml->response_headers, "Content-Type"))){
		debug("**ERROR**: Failed to determine content-type for:\n\t\t %s.", uri);
		g_free(uri);
		return NULL;
	}
	
	debug("Parsing content info: [%s] from:\n\t\t%s", content_info, uri);
	content_v=g_strsplit(content_info, "; ", -1);
	g_free(content_info);
	gchar *content_type=NULL;
	if(!( ((content_v[0])) && (content_type=g_strdup(content_v[0])) )){
		debug("**ERROR**: Failed to determine content-type for:\n\t\t %s.", uri);
		g_strfreev(content_v);
		g_free(uri);
		return NULL;
	}
	
	
	gchar *charset=NULL;
	if(!( ((content_v[1])) && (charset=g_strdup(content_v[1])) )){
		debug("**ERROR**: Failed to determine charset for:\n\t\t %s.", uri);
		g_free(content_type);
		g_strfreev(content_v);
		g_free(uri);
		return NULL;
	}
	g_strfreev(content_v);
	
	
	debug("Parsing charset: [%s] for:\n\t\t%s", charset, uri);
	content_v=g_strsplit(charset, "=", -1);
	g_free(charset);
	if(!(content_v && content_v[1])){
		debug("Defaulting to charset: [utf-8] for:\n\t\t%s", uri);
		charset=g_strdup("utf-8");
	}else{
		charset=g_strdup(content_v[1]);
		debug("Setting parsed charset: [%s] for:\n\t\t%s", charset, uri);
	}
	g_strfreev(content_v);
	
	
	gchar *encoding=NULL;
	if(!g_str_equal(charset, "utf-8"))
		encoding=g_ascii_strup(charset, -1);
	else
		encoding=g_utf8_strup(charset, -1);
	
	content_v=g_strsplit(content_type, "/", -1);
	gchar *dom_base_entity=NULL;
	if(!( ((content_v[1])) && (dom_base_entity=g_strdup(content_v[1])) )){
		debug("**ERROR**: Failed to determine DOM base entity URL for:\n\t\t%s.", uri);
		g_free(uri);
		g_free(content_type);
		g_strfreev(content_v);
		return NULL;
	}
	g_strfreev(content_v);
	
	
	debug("Parsed xml document's information.\n\t\tURI: [%s]\n\t\tcontent-type: [%s]; charset: [%s]; encoding: [%s]; dom element: [%s]", uri, content_type, charset, encoding, dom_base_entity);
	
	
	debug("Parsing %s document.", dom_base_entity);
	if(!( (doc=xmlReadMemory(xml->response_body->data, xml->response_body->length, dom_base_entity, encoding, (XML_PARSE_NOENT | XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING) )) )){
		debug("**ERROR**: Failed to parse %s document.", dom_base_entity);
		g_free(uri);
		g_free(content_type);
		g_free(dom_base_entity);
		g_free(charset);
		return NULL;
	}
	
	g_free(dom_base_entity);
	g_free(encoding);
	g_free(content_type);
	g_free(charset);
	
	debug("XML document parsed.  Returning xmlDoc.");
	return doc;
}/* parser_parse_dom_content */

xmlDoc *parser_parse(SoupMessage *xml, xmlNode **first_element){
	xmlDoc *doc=NULL;
	
	if(!(doc=parser_parse_dom_content(xml))) {
		debug("failed to read xml data");
		return NULL;
	}
	
	debug("Setting XML nodes.");
	xmlNode *root_element=NULL;
	*first_element=NULL;
	
	
	/* Get first element */
	root_element=xmlDocGetRootElement(doc);
	if(root_element==NULL) {
		debug("failed getting first element of xml data");
		xmlFreeDoc(doc);
		return NULL;
	} else
		*first_element=root_element;
	
	return doc;
}


gchar *parser_parse_xpath_content(SoupMessage *xml, const gchar *xpath){
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	if(!(doc=parser_parse(xml, &root_element))){
		xmlCleanupParser();
		return NULL;
	}
	
	xmlNode	*current_node=NULL;
	gchar	*xpath_content=NULL;
	gchar	**xpathv=g_strsplit(xpath, "->", -1);
	guint	xpath_index=0, xpath_target_index=g_strv_length(xpathv)-1;
	debug("Searching for xpath: '%s' content.", xpath);
	for(current_node=root_element; current_node; current_node=current_node->next) {
		debug("Checking current xpath: '%s' against current node: '%s (#%d)'.", xpathv[xpath_index], current_node->name, current_node->type);
		
		if(!g_str_equal(current_node->name, xpathv[xpath_index])) continue;
		
		if(xpath_index<xpath_target_index){
			if(!current_node->children) break;
			
			current_node=current_node->children;
			xpath_index++;
			continue;
		}
		
		xpath_content=(gchar *)xmlNodeGetContent(current_node);
		break;
	}
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	g_strfreev(xpathv);
	
	if(!G_STR_EMPTY(xpath_content))
		return g_strstrip(xpath_content);
	
	if(xpath_content)
		g_free(xpath_content);
	
	return NULL;
}/* parser_get_xpath */



/* Parse a timeline XML file */
gboolean parser_timeline(OnlineService *service, SoupMessage *xml){
	GtkListStore	*store=NULL;
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	xmlNode		*cur_node=NULL;
	
	Status 	*status=NULL;
	
	/* Count new tweets */
	gboolean		show_notification =(last_id > 0);
	unsigned long int	last_tweet = 0;
	/*
	 * On multiple tweet updates we only want to 
	 * play the sound notification once.
	 */
	gboolean	multiple_new_tweets = FALSE;
	gboolean	first_tweet=TRUE;
	
	gint		tweet_display_delay = 0;
	const int	tweet_display_interval = 5;
	
	/* parse the xml */
	if(!(doc=parser_parse(xml, &root_element))){
		xmlCleanupParser();
		return FALSE;
	}
	guint service_store_index=(online_services_get_which_connected(online_services, service)*20);
	
	/* Get the ListStore and clear previous */
	store=tweet_list_get_store();
	
	/* get tweets or direct messages */
	debug("Parsing %s timeline.", root_element->name);
	for(cur_node = root_element; cur_node; cur_node = cur_node->next) {
		if(cur_node->type != XML_ELEMENT_NODE ) continue;
		if( g_str_equal(cur_node->name, "statuses") ||	g_str_equal(cur_node->name, "direct-messages") ){
			cur_node = cur_node->children;
			continue;
		}
		
		if((!g_str_equal(cur_node->name, "status")) && (!g_str_equal(cur_node->name, "direct_message")) )
			continue;
		
		debug("Parsing tweet.  Its a %s.", (g_str_equal(cur_node->name, "status") ?"status update" :"direct message" ) );
		
		/* Timelines and direct messages */
		gchar	*tweet, *sexy_tweet, *datetime;
		guint	sid;
		
		/* Parse node */
		debug("Creating tweet's Status *.");
		status=parser_node_status(service, cur_node->children);
		if(first_tweet){
			struct tm	post;
			strptime(status->created_at_str, "%s", &post);
			post.tm_isdst=-1;
			status->created_at=mktime(&post);
			first_tweet=FALSE;
		}
		sid=status->id;
		
		/* the first tweet parsed is the 'newest' */
		if(last_tweet == 0)
			last_tweet = sid;
		
		/* Create string for text column */
		debug("Parsing tweet's 'created_at' time: [%s] to a display ready format.", status->created_at_str);
		datetime=parser_convert_time( status->created_at_str );
		debug("Display time set to: %s.", datetime);
		
		debug("Formating status text for display.");
		tweet=g_strdup_printf(
					"<small><u><b>From:</b></u><b> %s &lt;@%s on %s&gt;</b></small> | <span size=\"small\" weight=\"light\" variant=\"smallcaps\"><u>To:</u> &lt;%s&gt;</span>\n%s\n%s",
						status->user->nick_name, status->user->user_name, service->uri,
						service->decoded_key,
						datetime,
						status->text
		);
		
		if(!(gconfig_if_bool(PREFS_URLS_EXPAND_SELECTED_ONLY, TRUE)))
			sexy_tweet=label_msg_get_string(service, status->text);
		else
			sexy_tweet=g_strdup(status->text);
		
		if(sid > last_id && show_notification) {
			if(!multiple_new_tweets) {
				app_notify_sound(FALSE);
				multiple_new_tweets=TRUE;
			}
			g_timeout_add_seconds(tweet_display_delay, app_notify_on_timeout, g_strdup(tweet) );
			
			tweet_display_delay+=tweet_display_interval;
		}
		
		/* Append to ListStore */
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		
		/* find where to place this tweet's data. */
		gtk_list_store_insert(store, iter, service_store_index++);
		
		gtk_list_store_set(
					store, iter,
						STRING_TEXT, tweet,
						STRING_NICK, status->user->nick_name,
						STRING_DATE, datetime,
						STRING_TWEET, status->text,
						STRING_USER, status->user->user_name,
						SEXY_TWEET, sexy_tweet,
						CREATED_DATE, status->created_at_str,
						CREATED_AT, status->created_at,
						ULONG_TWEET_ID, sid,
						ULONG_USER_ID, status->user->id,
						SERVICE_POINTER, service,
					-1
		);
		
		/* Free the text column string */
		g_free(tweet);
		g_free(sexy_tweet);
		
		/* network_get_image, or its callback, free's iter once its no longer needed. */
		network_get_image(service, g_strdup(status->user->image_url), iter);
		
		/* Free struct */
		parser_node_status_free(status);
		
		if(datetime){
			g_free(datetime);
			datetime=NULL;
		}
	} /* end of loop */
	
	/* Remember last id showed */
	if(last_tweet > 0)
		last_id=last_tweet;
	
	/* Free memory */
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return TRUE;
}

static void parser_node_status_free(Status *status){
	if(!status) return;
	
	if(status->user){
		user_free(status->user);
		status->user=NULL;
	}
	if(status->text) g_free(status->text);
	if(status->created_at_str) g_free(status->created_at_str);
	
	g_free(status);
	status=NULL;
}/* parser_status_free */

static Status *parser_node_status(OnlineService *service, xmlNode *a_node){
	xmlNode		*cur_node = NULL;
	gchar		*content=NULL;
	Status		*status;
	
	status = g_new0(Status, 1);
	
	/* Begin 'status' or 'direct-messages' loop */
	debug("Parsing status & tweet at node: %s", a_node->name);
	for(cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if(cur_node->type != XML_ELEMENT_NODE) continue;
		
		if( G_STR_EMPTY( (content=(gchar *)xmlNodeGetContent(cur_node)) ) ) continue;
		
		
		if(g_str_equal(cur_node->name, "created_at")){
			struct tm	post;
			status->created_at_str=g_strdup(content);
			strptime(content, "%s", &post);
			post.tm_isdst=-1;
			status->created_at=mktime(&post);
			debug("Parsing tweet's 'created_at' date: [%s] to Unix seconds since: %u", status->created_at_str, status->created_at);
		}else if(g_str_equal(cur_node->name, "id")){
			debug("Parsing tweet's 'id': %s", content);
			status->id=strtoul( content, NULL, 10 );
		}else if( g_str_equal(cur_node->name, "sender") || g_str_equal(cur_node->name, "user")){
			debug("Parsing user node: %s.", cur_node->name);
			status->user=user_parse_profile(service, cur_node->children);
			debug("User parsed and created for user: %s.", status->user->user_name);
		}else if(g_str_equal(cur_node->name, "text")) {
			debug("Parsing tweet.");
			gchar *cur=status->text=g_markup_escape_text(content, -1);
			while((cur = strstr(cur, "&amp;"))) {
				if(strncmp(cur + 5, "lt;", 3) == 0 || strncmp(cur + 5, "gt;", 3) == 0)
					g_memmove(cur + 1, cur + 5, strlen(cur + 5) + 1);
				else
					cur += 5;
			}
		}
		xmlFree(content);
		
	} /* End of loop */
	
	return status;
}

static gchar *parser_convert_time(const char *datetime){
	struct tm	*ta;
	struct tm	post;
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
