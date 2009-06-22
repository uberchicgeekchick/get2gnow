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
#include <string.h> /* for g_memmove - memmove */
#include <strings.h>

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <libsoup/soup-message.h>

#include "main.h"
#include "online-services.h"
#include "network.h"
#include "users.h"

#include "main-window.h"
#include "tweet-list.h"
#include "preferences.h"
#include "images.h"
#include "label.h"
#include "gconfig.h"

#include "parser.h"

#define	DEBUG_DOMAINS	"Parser:Requests:OnlineServices:Tweets:UI:Refreshing:Dates:Times:Parser.c"
#include "debug.h"


static gchar *parser_xml_node_type_to_string(xmlElementType type);
static xmlDoc *parse_dom_content(SoupMessage *xml);


static xmlDoc *parse_dom_content(SoupMessage *xml){
	xmlDoc *doc=NULL;
	
	SoupURI	*suri=NULL;
	gchar *uri=NULL;
	if(!( (suri=soup_message_get_uri(xml)) && (uri=soup_uri_to_string(suri, FALSE)) )){
		debug("*WARNING*: Unknown XML document URI.");
		uri=g_strdup("[*unknown*]");
	}
	if(suri) soup_uri_free(suri);
	
	
	if(!( xml->response_headers && xml->response_body && xml->response_body->data && xml->response_body->length )){
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
}/*parse_dom_content*/

xmlDoc *parse_xml_doc(SoupMessage *xml, xmlNode **first_element){
	xmlDoc *doc=NULL;
	
	if(!(doc=parse_dom_content(xml))) {
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

static gchar *parser_xml_node_type_to_string(xmlElementType type){
	switch(type){
		case XML_ELEMENT_NODE: return _("XML_ELEMENT_NODE");
		case XML_ATTRIBUTE_NODE: return _("XML_ATTRIBUTE_NODE");
		case XML_TEXT_NODE: return _("XML_TEXT_NODE");
		case XML_CDATA_SECTION_NODE: return _("XML_CDATA_SECTION_NODE");
		case XML_ENTITY_REF_NODE: return _("XML_ENTITY_REF_NODE");
		case XML_ENTITY_NODE: return _("XML_ENTITY_NODE");
		case XML_PI_NODE: return _("XML_PI_NODE");
		case XML_COMMENT_NODE: return _("XML_COMMENT_NODE");
		case XML_DOCUMENT_NODE: return _("XML_DOCUMENT_NODE");
		case XML_DOCUMENT_TYPE_NODE: return _("XML_DOCUMENT_TYPE_NODE");
		case XML_DOCUMENT_FRAG_NODE: return _("XML_DOCUMENT_FRAG_NODE");
		case XML_NOTATION_NODE: return _("XML_NOTATION_NODE");
		case XML_HTML_DOCUMENT_NODE: return _("XML_HTML_DOCUMENT_NODE");
		case XML_DTD_NODE: return _("XML_DTD_NODE");
		case XML_ELEMENT_DECL: return _("XML_ELEMENT_DECL");
		case XML_ATTRIBUTE_DECL: return _("XML_ATTRIBUTE_DECL");
		case XML_ENTITY_DECL: return _("XML_ENTITY_DECL");
		case XML_NAMESPACE_DECL: return _("XML_NAMESPACE_DECL");
		case XML_XINCLUDE_START: return _("XML_XINCLUDE_START");
		case XML_XINCLUDE_END: return _("XML_XINCLUDE_END");
		case XML_DOCB_DOCUMENT_NODE: return _("XML_DOCB_DOCUMENT_NODE");
		default: return _("XML_TYPE_UNKNOWN");
	}
}/*parser_xml_node_type_to_string(node->type);*/

gchar *parse_xpath_content(SoupMessage *xml, const gchar *xpath){
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	if(!(doc=parse_xml_doc(xml, &root_element))){
		xmlCleanupParser();
		return NULL;
	}
	
	xmlNode	*current_node=NULL;
	gchar	*xpath_content=NULL;
	
	gchar	**xpathv=g_strsplit(xpath, "->", -1);
	guint	xpath_index=0, xpath_target_index=g_strv_length(xpathv)-1;
	
	debug("Searching for xpath: '%s' content.", xpath);
	for(current_node=root_element; current_node; current_node=current_node->next) {
		debug("Checking current xpath: '%s' against current node: '%s'\n\t\t\txmlElementType: enum typdef %s(#%d)'.", xpathv[xpath_index], current_node->name, parser_xml_node_type_to_string(current_node->type), current_node->type);
		
		if(xpath_index>xpath_target_index) break;
		
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
}/*parser_get_xpath*/



/* Parse a timeline XML file */
guint parse_timeline(OnlineService *service, SoupMessage *xml, const gchar *timeline, TweetList *tweet_list, TweetLists monitoring){
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	xmlNode		*current_node=NULL;
	UserStatus 	*status=NULL;
	
	/* Count new tweets */
	gboolean	notify;
	guint		tweets_parsed=0;
	gulong		status_id=0;
	gulong		id_newest_update=0, id_oldest_update=0;
	
	gint has_loaded=tweet_list_has_loaded(tweet_list);
	if( has_loaded || monitoring==DMs || monitoring==Replies )
		online_service_update_ids_get(service, timeline, &id_newest_update, &id_oldest_update);
	gulong last_notified_update=id_newest_update;
	
	switch(monitoring){
		case DMs:
			debug("Parsing DMs.");
			notify=gconfig_if_bool(PREFS_NOTIFY_DMS, TRUE);
			break;
			
		case Replies:
			debug("Parsing Replies.");
			notify=gconfig_if_bool(PREFS_NOTIFY_REPLIES, TRUE);
			break;
		
		case Tweets:
			debug("Parsing my friends' tweets");
			notify=gconfig_if_bool(PREFS_NOTIFY_MY_FRIENDS_TWEETS, TRUE);
			break;
		
		case Timelines: case Users:
			debug("Parsing timeline.");
			notify=gconfig_if_bool(PREFS_NOTIFY_ALL, TRUE);
			break;
			
		case Archive:
			debug("Parsing my own tweets or favorites.");
			notify=FALSE;
			break;
		
		case None: default: return 0;
	}
	if(!id_oldest_update && notify && ( monitoring!=DMs || monitoring!=Replies ) ) notify=FALSE;
	
	guint tweet_list_notify_delay=tweet_list_get_notify_delay(tweet_list);
	const gint	tweet_display_interval=10;
	const gint notify_priority=(monitoring-2)*100;
	
	if(!(doc=parse_xml_doc(xml, &root_element))){
		debug("**ERROR:** Failed to parse xml document, timeline: %s.", timeline);
		xmlCleanupParser();
		return tweets_parsed;
	}
	
	/* get tweets or direct messages */
	debug("Parsing %s timeline.", root_element->name);
	const gchar *service_username=online_service_get_username(service);
	gboolean free_status;
	for(current_node = root_element; current_node; current_node = current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE ) continue;
		
		if( g_str_equal(current_node->name, "statuses") || g_str_equal(current_node->name, "direct-messages") ){
			if(!current_node->children) continue;
			current_node = current_node->children;
			continue;
		}
		
		if(!( g_str_equal(current_node->name, "status") || g_str_equal(current_node->name, "direct_message") ))
			continue;
		
		if(!current_node->children){
			debug("*WARNING:* Cannot parse %s. Its missing children nodes.", current_node->name);
			continue;
		}
		
		debug("Parsing tweet.  Its a %s.", (g_str_equal(current_node->name, "status") ?"status update" :"direct message" ) );
		
		status=NULL;
		debug("Creating tweet's Status *.");
		if(!( (( status=user_status_parse(service, current_node->children, monitoring ))) && (status_id=user_status_get_id(status)) )){
			if(status) user_status_free(status);
			continue;
		}
		
		tweets_parsed++;
		free_status=TRUE;
		/* id_oldest_tweet is only set when monitoring DMs or Replies */
		debug("Adding UserStatus from: %s, ID: %lu, on <%s> to TweetList.", user_status_get_user_name(status), status_id, online_service_get_key(service));
		user_status_store(status, tweet_list);
		
		if( notify && status_id > last_notified_update && strcasecmp(user_status_get_user_name(status), service_username) ){
			free_status=FALSE;
			g_timeout_add_seconds_full(notify_priority, tweet_list_notify_delay, main_window_notify_on_timeout, status, (GDestroyNotify)user_status_free);
			tweet_list_notify_delay+=tweet_display_interval;
		}
		
		if(status_id > id_newest_update) id_newest_update=status_id;
		if( (monitoring!=DMs && monitoring!=Replies) || !(has_loaded == 2 && id_oldest_update) )
			id_oldest_update=status_id;
		
		if(free_status) user_status_free(status);
	}
	
	if(id_newest_update)
		online_service_update_ids_set(service, timeline, id_newest_update, id_oldest_update);
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return tweets_parsed;
}

gchar *parser_escape_text(gchar *text){
	gchar *escaped_text=NULL;
	gchar *current_character=escaped_text=g_markup_escape_text(text, -1);
	while((current_character=strstr(current_character, "&amp;"))) {
		if(!( strncmp(current_character+5, "lt;", 3) && strncmp(current_character+5, "gt;", 3) ))
			g_memmove(current_character+1, current_character+5, strlen(current_character+5)+1);
		else
			current_character+=5;
	}
	return escaped_text;
}/*parser_escape_text(text);*/

gchar *parser_convert_time(const gchar *datetime, gulong *my_diff){
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
	
	(*my_diff)=diff=difftime(seconds_local, seconds_post);
	
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
					return g_strdup_printf(_("%i months ago"), (diff/30));
				}
			}
		}
	}
	return NULL;
}

