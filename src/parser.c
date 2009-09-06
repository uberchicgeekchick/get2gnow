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

#define _GNU_SOURCE
#define _THREAD_SAFE


#define _GNU_SOURCE
#define _THREAD_SAFE


#include <string.h>
#include <strings.h>

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <libsoup/soup-message.h>

#include "config.h"
#include "program.h"

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


static xmlDoc *parse_dom_content(SoupMessage *xml);


static xmlDoc *parse_dom_content(SoupMessage *xml){
	xmlDoc *doc=NULL;
	
	SoupURI	*suri=NULL;
	gchar *uri=NULL;
	if(!( (((suri=soup_message_get_uri(xml)))) && ((uri=soup_uri_to_string(suri, FALSE))) && (G_STR_N_EMPTY(uri)) )){
		debug("*WARNING*: Unknown XML document URI.");
		if(uri) g_free(uri);
		uri=g_strdup("[*unknown*]");
	}
	if(suri) soup_uri_free(suri);
	
	
	if(!( xml->response_headers && xml->response_body && xml->response_body->data && xml->response_body->length )){
		debug("**ERROR**: Cannot parse empty or xml resonse from: %s.", uri);
		g_free(uri);
		return NULL;
	}
	
	
	gchar **content_v=NULL;
	debug("Parsing xml document's content-type & DOM information from: %s", uri);
	gchar *content_info=NULL;
	if(!(content_info=g_strdup((gchar *)soup_message_headers_get_one(xml->response_headers, "Content-Type")))){
		debug("**ERROR**: Failed to determine content-type for:  %s.", uri);
		g_free(uri);
		return NULL;
	}
	
	debug("Parsing content info: [%s] from: %s", content_info, uri);
	content_v=g_strsplit(content_info, "; ", -1);
	g_free(content_info);
	gchar *content_type=NULL;
	if(!( ((content_v[0])) && (content_type=g_strdup(content_v[0])) )){
		debug("**ERROR**: Failed to determine content-type for:  %s.", uri);
		g_strfreev(content_v);
		g_free(uri);
		return NULL;
	}
	debug("Parsed Content-Type: [%s] for: %s", content_type, uri);
	
	gchar *charset=NULL;
	if(!( ((content_v[1])) && (charset=g_strdup(content_v[1])) )){
		debug("**ERROR**: Failed to determine charset for:  %s.", uri);
		g_free(content_type);
		g_strfreev(content_v);
		g_free(uri);
		return NULL;
	}
	g_strfreev(content_v);
	
	
	debug("Parsing charset: [%s] for: %s", charset, uri);
	content_v=g_strsplit(charset, "=", -1);
	g_free(charset);
	if(!(content_v && content_v[1])){
		debug("Defaulting to charset: [utf-8] for: %s", uri);
		charset=g_strdup("utf-8");
	}else{
		charset=g_strdup(content_v[1]);
		debug("Setting parsed charset: [%s] for: %s", charset, uri);
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
		debug("**ERROR**: Failed to determine DOM base entity URL for: %s.", uri);
		g_free(uri);
		g_free(content_type);
		g_strfreev(content_v);
		return NULL;
	}
	g_strfreev(content_v);
	
	
	debug("Parsed xml document's information. URI: [%s] content-type: [%s]; charset: [%s]; encoding: [%s]; dom element: [%s]", uri, content_type, charset, encoding, dom_base_entity);
	
	
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

const gchar *parser_xml_node_type_to_string(xmlElementType type){
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
	debug("Parsing xml document before searching for xpath: '%s' content.", xpath);
	if(!(doc=parse_xml_doc(xml, &root_element))){
		debug("Unable to parse xml doc.");
		xmlCleanupParser();
		return NULL;
	}
	
	xmlNode	*current_node=NULL;
	gchar	*xpath_content=NULL;
	
	gchar	**xpathv=g_strsplit(xpath, "->", -1);
	guint	xpath_index=0, xpath_target_index=g_strv_length(xpathv)-1;
	
	debug("Searching for xpath: '%s' content.", xpath);
	for(current_node=root_element; current_node; current_node=current_node->next) {
		
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
	
	if(!( ((xpath_content)) && (xpath_content=g_strstrip(xpath_content)) && G_STR_N_EMPTY(xpath_content) )){
		if(xpath_content) g_free(xpath_content);
		return NULL;
	}
	
	return xpath_content;
}/*parser_get_xpath*/



/* Parse a timeline XML file */
guint parse_timeline(OnlineService *service, SoupMessage *xml, const gchar *uri, TweetList *tweet_list, UpdateMonitor monitoring){
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	xmlNode		*current_node=NULL;
	UserStatus 	*status=NULL;
	
	/* Count new tweets */
	gboolean	notify=gconfig_if_bool(PREFS_NOTIFY_ALL, TRUE);;
	
	gboolean	oldest_update_id_saved=FALSE;
	gboolean	save_oldest_id=(tweet_list_has_loaded(tweet_list)?FALSE:TRUE);
	
	guint		new_updates=0;
	gdouble		status_id=0;
	gdouble		id_newest_update=0, id_oldest_update=0;
	gint		update_expiration=0;
	
	gchar		**uri_split=g_strsplit( g_strrstr(uri, "/"), "?", 2);
	gchar		*timeline=g_strdup(uri_split[0]);
			g_strfreev(uri_split);
	
	online_service_update_ids_get(service, timeline, &id_newest_update, &id_oldest_update);
	gdouble	last_notified_update=id_newest_update;
	id_newest_update=0.0;
	
	switch(monitoring){
		case Searches: case Groups:
			debug("**ERROR:** parse_timeline requested to parse %s.  This method sould not have been called.", (monitoring==Groups?"Groups":"Searches"));
			uber_free(timeline);
			return 0;
			
		case DMs:
			/*Direct Messages are kept for 4 weeks, by default.*/
			debug("Parsing DMs.");
			gconfig_get_int_or_default(PREFS_TWEETS_ARCHIVE_DMS, &update_expiration, 2419200);
			if(!notify) notify=gconfig_if_bool(PREFS_NOTIFY_DMS, TRUE);
			if(!id_oldest_update) save_oldest_id=TRUE;
			else save_oldest_id=FALSE;
			break;
		
		case Replies:
			/*By default Replies, & @ Mentions, from the last 7 days are loaded.*/
			debug("Parsing Replies and/or @ Mentions.");
			gconfig_get_int_or_default(PREFS_TWEETS_ARCHIVE_REPLIES, &update_expiration, 604800);
			if(!notify) notify=gconfig_if_bool(PREFS_NOTIFY_REPLIES, TRUE);
			if(!id_oldest_update) save_oldest_id=TRUE;
			else save_oldest_id=FALSE;
			break;
		
		case Faves:
			/*Favorite/Star'd updates are kept for 4 weeks, by default.*/
			debug("Parsing Faves.");
			gconfig_get_int_or_default(PREFS_TWEETS_ARCHIVE_FAVES, &update_expiration, 2419200);
			if(!id_oldest_update) save_oldest_id=TRUE;
			else save_oldest_id=FALSE;
			break;
		
		case BestFriends:
			gconfig_get_int_or_default(PREFS_TWEETS_ARCHIVE_BEST_FRIENDS, &update_expiration, 86400);
			if(!notify) notify=gconfig_if_bool(PREFS_NOTIFY_BEST_FRIENDS, TRUE);
			break;
		
		case Tweets:
			debug("Parsing updates from someone I'm following.");
			if(!notify) notify=gconfig_if_bool(PREFS_NOTIFY_FOLLOWING, TRUE);
			break;
		
		case Timelines: case Users:
			debug("Parsing timeline.");
			break;
			
		case Archive:
			debug("Parsing my own updates or favorites.");
			break;
		
		case None: default:
			uber_free(timeline);
			return 0;
	}
	if(notify && monitoring!=DMs && monitoring!=Replies && !save_oldest_id ) notify=FALSE;
	
	guint		tweet_list_notify_delay=tweet_list_get_notify_delay(tweet_list);
	const gint	tweet_display_interval=10;
	const gint	notify_priority=(tweet_list_get_page(tweet_list)-1)*100;
	
	if(!(doc=parse_xml_doc(xml, &root_element))){
		debug("Failed to parse xml document, timeline: %s; uri: %s.", timeline, uri);
		xmlCleanupParser();
		uber_free(timeline);
		return 0;
	}
	
	/* get tweets or direct messages */
	debug("Parsing %s timeline.", root_element->name);
	const gchar *service_user_name=online_service_get_user_name(service);
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
		
		debug("Parsing %s.", (g_str_equal(current_node->name, "status") ?"status update" :"direct message" ) );
		
		status=NULL;
		debug("Creating tweet's Status *.");
		if(!( (( status=user_status_parse(service, current_node->children, monitoring ))) && (status_id=user_status_get_id(status)) )){
			if(status) user_status_free(status);
			continue;
		}
		
		new_updates++;
		free_status=TRUE;
		/* id_oldest_tweet is only set when monitoring DMs or Replies */
		debug("Adding UserStatus from: %s, ID: %f, on <%s> to TweetList.", user_status_get_user_name(status), status_id, online_service_get_key(service));
		user_status_store(status, tweet_list);
		
		if(!save_oldest_id && status_id > last_notified_update && strcasecmp(user_status_get_user_name(status), service_user_name) ){
			tweet_list_mark_as_unread(tweet_list);
			if(notify){
				free_status=FALSE;
				g_timeout_add_seconds_full(notify_priority, tweet_list_notify_delay, main_window_notify_on_timeout, status, (GDestroyNotify)user_status_free);
				tweet_list_notify_delay+=tweet_display_interval;
			}
		}
		
		if(!id_newest_update) id_newest_update=status_id;
		/*if(!oldest_update_id_saved && update_expiration && user_status_get_created_seconds_ago(status) > update_expiration) save_oldest_id=TRUE;*/
		if(save_oldest_id){
			oldest_update_id_saved=TRUE;
			id_oldest_update=status_id;
		}
		
		if(free_status) user_status_free(status);
	}
	
	if(new_updates && id_newest_update){
		/*TODO implement this only once it won't ending up causing bloating.
		 *cache_save_page(service, uri, xml->response_body);
		 */
		const gchar *online_service_guid=online_service_get_guid(service);
		debug("Processing <%s>'s requested URI's: [%s] new update IDs", online_service_guid, uri);
		debug("Saving <%s>'s; update IDs for [%s].  %f - newest ID.  %f - oldest ID.", online_service_guid, timeline, id_newest_update, id_oldest_update);
		online_service_update_ids_set(service, timeline, id_newest_update, id_oldest_update);
	}
	
	uber_free(timeline);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return new_updates;
}/*parse_timeline(service, xml, uri, tweet_list);*/

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

gchar *parser_convert_time(const gchar *datetime, gint *my_diff){
	struct tm	*ta;
	struct tm	post;
	int		seconds_local;
	int		seconds_post;
	int		diff;
	time_t		t=time(NULL);
	
	tzset();
	ta=gmtime(&t);
	ta->tm_isdst=-1;
	seconds_local=mktime(ta);
	
	strptime(datetime, "%a %b %d %T +0000 %Y", &post);
	post.tm_isdst=-1;
	seconds_post=mktime(&post);
	
	diff=difftime(seconds_local, seconds_post);
	if(diff < 0) *my_diff=0;
	else *my_diff=diff;
	
	/* Up to one minute ago. */
	
	if(diff < 2) return g_strdup(_("1 second ago"));
	if(diff < 60 ) return g_strdup_printf(_("%i seconds ago"), diff);
	if(diff < 120) return g_strdup(_("1 minute ago"));
	
	/* Minutes */
	diff=diff/60;
	if(diff < 60) return g_strdup_printf(_("%i minutes ago"), diff);
	if(diff < 120) return g_strdup(_("1 hour ago"));
	
	/* Hours */
	diff=diff/60;
	if(diff < 24) return g_strdup_printf(_("%i hours ago"), diff);
	if(diff < 48) return g_strdup(_("1 day ago"));
	
	/* Days */
	diff=diff/24;
	if(diff < 30) return g_strdup_printf(_("%i days ago"), diff);
	if(diff < 365) return g_strdup_printf(_("%i months ago"), (diff/30));
	
	return g_strdup_printf(_("%i years ago"), (diff/365));
	/* NOTE:
	 * 	About time, month, & year precision, "years aren't...blah blah".
	 * 	yeah well I agree!
	 * 	but I'm dealing w/integers not floating point arthmatic.
	 * 	so we'll all just have to get over is.
	 */
}/*parser_convert_time(date_created_string, &created_seconds_ago);*/

