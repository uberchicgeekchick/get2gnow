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

#include "online-services.defines.h"
#include "online-services.h"
#include "online-service.types.h"
#include "online-service.h"
#include "network.h"

#include "users.types.h"
#include "users.h"

#include "main-window.h"
#include "update-viewer.h"
#include "preferences.h"
#include "images.h"
#include "label.h"
#include "gconfig.h"

#include "best-friends.h"
#include "parser.h"

#define	DEBUG_DOMAINS	"Parser:Requests:OnlineServices:Updates:UI:Refreshing:Dates:Times:parser.c"
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
		debug("*ERROR:* Cannot parse empty or xml resonse from: %s.", uri);
		g_free(uri);
		return NULL;
	}
	
	
	gchar **content_v=NULL;
	debug("Parsing xml document's content-type & DOM information from: %s", uri);
	gchar *content_info=NULL;
	if(!(content_info=g_strdup((gchar *)soup_message_headers_get_one(xml->response_headers, "Content-Type")))){
		debug("*ERROR:* Failed to determine content-type for:  %s.", uri);
		g_free(uri);
		return NULL;
	}
	
	debug("Parsing content info: [%s] from: %s", content_info, uri);
	content_v=g_strsplit(content_info, "; ", -1);
	g_free(content_info);
	gchar *content_type=NULL;
	if(!( ((content_v[0])) && (content_type=g_strdup(content_v[0])) )){
		debug("*ERROR:*: Failed to determine content-type for:  %s.", uri);
		g_strfreev(content_v);
		g_free(uri);
		return NULL;
	}
	
	if(!( g_strrstr(content_type, "text") || g_strrstr(content_type, "xml") )){
		debug("*ERROR:* <%s>'s Content-Type: [%s] is not contain text or xml content and cannot be parsed any further.", uri, content_type );
		uber_free(content_type);
		return NULL;
	}
	
	debug("Parsed Content-Type: [%s] for: %s", content_type, uri);
	
	gchar *charset=NULL;
	if(!( ((content_v[1])) && (charset=g_strdup(content_v[1])) )){
		debug("*ERROR:* Failed to determine charset for:  %s.", uri);
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
		debug("*ERROR:* Failed to parse %s document.", dom_base_entity);
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

gboolean parser_xml_error_check(OnlineService *service, const gchar *uri, SoupMessage *xml, gchar **error_message){
	if(!( SOUP_IS_MESSAGE(xml) && SOUP_STATUS_IS_SUCCESSFUL(xml->status_code) && xml->status_code>99 )){
		*error_message=g_strdup_printf("OnlineService: <%s> has returned an invalid or failed libsoup request.  The URI [%s] returned:: %s(%d).", service->key, uri, xml->reason_phrase, xml->status_code );
		return FALSE;
	}
	
	*error_message=g_strdup("");;
	
	if(xml->status_code==100||xml->status_code==200){
		debug("OnlineService: <%s>'s request for: %s has succeed.  HTTP status returned: %s(%d).", service->key, uri, xml->reason_phrase, xml->status_code);
		return TRUE;
	}
	
	if(xml->status_code==401 || xml->status_code==503){
		debug("**ERROR:** Authentication failed and/or access denied.  OnlineService: <%s> returned. %s(%d) when requesting [%s].", service->key, xml->reason_phrase, xml->status_code, uri );
		return FALSE;
	}
	
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	
	debug("Getting content-type from uri: [%s].", uri);
	gchar **content_v=NULL;
	debug("Parsing xml document's content-type & DOM information from: [%s].", uri);
	gchar *content_info=NULL;
	if(!(content_info=g_strdup((gchar *)soup_message_headers_get_one(xml->response_headers, "Content-Type")))){
		debug("*ERROR:* Failed to determine content-type for:  [%s].", uri);
		return FALSE;
	}
	
	debug("Parsing content info: [%s] from: [%s].", content_info, uri);
	content_v=g_strsplit(content_info, "; ", -1);
	g_free(content_info);
	gchar *content_type=NULL;
	if(!( ((content_v[0])) && (content_type=g_strdup(content_v[0])) )){
		debug("*ERROR:* Failed to determine content-type for:  [%s].", uri);
		g_strfreev(content_v);
		return FALSE;
	}
	debug("Parsed Content-Type: [%s] for: [%s].", content_type, uri);
	
	g_strfreev(content_v);
	if(!g_str_equal(content_type, "application/xml")){
		uber_free(content_type);
		return TRUE;
	}
	uber_free(content_type);
	
	debug("Parsing xml document to find any authentication errors.");
	if(!( (doc=xmlReadMemory(xml->response_body->data, xml->response_body->length, "xml", "utf-8", (XML_PARSE_NOENT | XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING) )) )){
		debug("Unable to parse xml doc.");
		uber_free( *error_message );
		*error_message=g_strdup("Unable to parse xml doc.");
		xmlCleanupParser();
		return FALSE;
	}
	
	root_element=xmlDocGetRootElement(doc);
	if(root_element==NULL) {
		debug("Failed getting first element of xml data.");
		uber_free( *error_message );
		*error_message=g_strdup("Failed getting first element of xml data.");
		xmlFreeDoc(doc);
		xmlCleanupParser();
		return FALSE;
	}
	
	gboolean error_free=TRUE;
	xmlNode	*current_node=NULL;
	for(current_node = root_element; current_node; current_node = current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE ) continue;
		
		if(!( g_str_equal(current_node->name, "error") )) continue;
		
		error_free=FALSE;
		uber_free( *error_message );
		*error_message=(gchar *)xmlNodeGetContent(current_node);
		break;
	}
	
	if(!error_free){
		gchar *error_message_swap=g_strdup_printf("OnlineService: <%s> returned: %s(%d) with the error: %s.", service->key, xml->reason_phrase, xml->status_code, *error_message);
		uber_free((*error_message) );
		*error_message=error_message_swap;
		error_message_swap=NULL;
		debug("%s", *error_message);
		statusbar_printf( "%s", *error_message );
	}
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return error_free;
}/*parser_xml_error_check(service, xml);*/

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
	for(current_node=root_element; current_node; current_node=current_node->next){
		
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
guint parse_timeline(OnlineService *service, SoupMessage *xml, const gchar *timeline, UpdateViewer *update_viewer, UpdateMonitor monitoring){
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	xmlNode		*current_node=NULL;
	UserStatus 	*status=NULL;
	
	/* Count new tweets */
	gboolean	notify=gconfig_if_bool(PREFS_NOTIFY_ALL, TRUE);;
	
	gboolean	oldest_update_id_saved=FALSE;
	gboolean	save_oldest_id=(update_viewer_has_loaded(update_viewer)?FALSE:TRUE);
	gboolean	notify_best_friends=gconfig_if_bool(PREFS_NOTIFY_BEST_FRIENDS, TRUE);
	
	guint		new_updates=0;
	gint		update_expiration=0, best_friends_expiration=0;
	gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_BEST_FRIENDS, &best_friends_expiration, 86400);
	
	gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	online_service_update_ids_get(service, timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	gdouble	last_notified_update=newest_update_id;
	newest_update_id=0.0;
	
	switch(monitoring){
		case Searches: case Groups:
			debug("*ERROR:* Unsupported timeline.  parse_timeline requested to parse %s.  This method sould not have been called.", (monitoring==Groups?"Groups":"Searches"));
			return 0;
			
		case DMs:
			/*Direct Messages are kept for 4 weeks, by default.*/
			debug("Parsing DMs.");
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_DMS, &update_expiration, 2419200);
			if(!notify) notify=gconfig_if_bool(PREFS_NOTIFY_DMS, TRUE);
			if(!oldest_update_id) save_oldest_id=TRUE;
			else save_oldest_id=FALSE;
			break;
		
		case Replies:
			/*By default Replies, & @ Mentions, from the last 7 days are loaded.*/
			debug("Parsing Replies and/or @ Mentions.");
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_REPLIES, &update_expiration, 604800);
			if(!notify) notify=gconfig_if_bool(PREFS_NOTIFY_REPLIES, TRUE);
			if(!oldest_update_id) save_oldest_id=TRUE;
			else save_oldest_id=FALSE;
			break;
		
		case Faves:
			/*Favorite/Star'd updates are kept for 4 weeks, by default.*/
			debug("Parsing Faves.");
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_FAVES, &update_expiration, 2419200);
			if(!oldest_update_id) save_oldest_id=TRUE;
			else save_oldest_id=FALSE;
			break;
		
		case BestFriends:
			/*Best Friends' updates are kept for 1 day, by default.*/
			debug("Parsing best friends updates.");
			notify=notify_best_friends;
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_BEST_FRIENDS, &update_expiration, 86400);
			if(!save_oldest_id) save_oldest_id=TRUE;
			break;
		
		case Updates:	case Timelines: case Users:
			debug("Parsing updates from someone I'm following.");
			if(!notify) notify=gconfig_if_bool(PREFS_NOTIFY_FOLLOWING, TRUE);
			if(!save_oldest_id) save_oldest_id=TRUE;
			break;
			
		case Archive:
			debug("Parsing my own updates or favorites.");
			break;
		
		case None: default:
			/* These cases are never reached - they're just here to make gcc happy. */
			return 0;
	}
	if(!oldest_update_id && notify && ( monitoring!=DMs || monitoring!=Replies ) ) notify=FALSE;
	
	guint		update_viewer_notify_delay=update_viewer_get_notify_delay(update_viewer);
	const gint	tweet_display_interval=10;
	const gint	notify_priority=(update_viewer_get_page(update_viewer)-1)*100;
	
	if(!(doc=parse_xml_doc(xml, &root_element))){
		debug("Failed to parse xml document, <%s>'s timeline: %s.", service->key, timeline);
		xmlCleanupParser();
		return 0;
	}
	
	/* get tweets or direct messages */
	debug("Parsing %s timeline.", root_element->name);
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
		if(!( (( status=user_status_parse(service, current_node->children, Searches ))) && status->id )){
			if(status) user_status_free(status);
			continue;
		}
		
		new_updates++;
		gboolean free_status=TRUE;
		/* id_oldest_tweet is only set when monitoring DMs or Replies */
		debug("Adding UserStatus from: %s, ID: %s, on <%s> to UpdateViewer.", status->user->user_name, status->id_str, service->key);
		update_viewer_store(update_viewer, status);
		if( monitoring!=BestFriends && monitoring!=DMs && online_service_is_user_best_friend(service, status->user->user_name) ){
			if( (best_friends_check_update_ids( service, status->user->user_name, status->id)) && notify_best_friends){
				free_status=FALSE;
				g_timeout_add_seconds_full(notify_priority, update_viewer_notify_delay, (GSourceFunc)user_status_notify_on_timeout, status, (GDestroyNotify)user_status_free);
				update_viewer_notify_delay+=tweet_display_interval;
			}
		}
		
		if( notify && free_status && !save_oldest_id && status->id > last_notified_update && strcasecmp(status->user->user_name, service->user_name) ){
			free_status=FALSE;
			g_timeout_add_seconds_full(notify_priority, update_viewer_notify_delay, (GSourceFunc)user_status_notify_on_timeout, status, (GDestroyNotify)user_status_free);
			update_viewer_notify_delay+=tweet_display_interval;
		}
		
		if(!newest_update_id && status->id) newest_update_id=status->id;
		if(save_oldest_id && status->id){
			if(!oldest_update_id_saved) oldest_update_id_saved=TRUE;
			oldest_update_id=status->id;
		}
		
		if(free_status) user_status_free(status);
	}
	
	if(new_updates && newest_update_id){
		/*TODO implement this only once it won't ending up causing bloating.
		 *cache_save_page(service, timeline, xml->response_body);
		 */
		debug("Processing <%s>'s requested URI's: [%s] new update IDs", service->guid, timeline);
		debug("Saving <%s>'s; update IDs for [%s];  newest ID: %f; unread ID: %f; oldest ID: %f.", service->guid, timeline, newest_update_id, unread_update_id, oldest_update_id );
		online_service_update_ids_set(service, timeline, newest_update_id, unread_update_id, oldest_update_id);
	}
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return new_updates;
}/*parse_timeline(service, xml, timeline, update_viewer, monitoring);*/

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

