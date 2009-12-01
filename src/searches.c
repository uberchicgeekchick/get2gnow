/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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
/********************************************************************************
 *                      My art, code, & programming.                            *
 ********************************************************************************/
#define _GNU_SOURCE
#define _THREAD_SAFE



/********************************************************************************
 *      Project, system & library headers, eg #include <gdk/gdkkeysyms.h>       *
 ********************************************************************************/
#include <glib/gi18n.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <libsoup/soup-message.h>

#include "config.h"
#include "program.h"

#include "online-services.h"
#include "online-service.types.h"
#include "online-service.h"
#include "network.h"

#include "users.types.h"
#include "users.h"

#include "main-window.h"
#include "uberchick-tree-view.h"
#include "preferences.h"
#include "images.h"
#include "gconfig.h"

#include "parser.h"
#include "searches.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"Search:Parser:Requests:OnlineServices:Updates:UI:Refreshing:Dates:Times:search.c"
#include "debug.h"



/* Parse a timeline XML file */
guint searches_parse_results(OnlineService *service, SoupMessage *xml, const gchar *uri, UberChickTreeView *uberchick_tree_view, UpdateMonitor monitoring){
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	xmlNode		*current_node=NULL;
	UserStatus 	*status=NULL;
	
	/* Count new tweets */
	guint		new_updates=0;
	
	gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	
	gchar *timeline=searches_format_timeline_from_uri(uri);
	online_service_update_ids_get(service, timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	
	gdouble	last_notified_update=newest_update_id;
	newest_update_id=0.0;
	
	gboolean	has_loaded=uberchick_tree_view_has_loaded(uberchick_tree_view);
	gboolean	notify=( (oldest_update_id && has_loaded) ?gconfig_if_bool(PREFS_NOTIFY_ALL, TRUE) :FALSE );
	gboolean	save_oldest_id=(has_loaded?FALSE:TRUE);
	
	guint		uberchick_tree_view_notify_delay=uberchick_tree_view_get_notify_delay(uberchick_tree_view);
	const gint	tweet_display_interval=10;
	const gint	notify_priority=(uberchick_tree_view_get_page(uberchick_tree_view)-1)*100;
	
	if(!(doc=parse_xml_doc(xml, &root_element))){
		debug("Failed to parse xml document, timeline: %s; uri: %s.", timeline, uri);
		xmlCleanupParser();
		uber_free(timeline);
		return 0;
	}
	
	/* get tweets or direct messages */
	debug("Parsing searches node %s.", root_element->name);
	gboolean free_status;
	for(current_node=root_element->children; current_node; current_node = current_node->next) {
		if(!( current_node->type == XML_ELEMENT_NODE && g_str_equal(current_node->name, "entry") ))
			continue;
		
		if(!current_node->children){
			debug("*WARNING:* Cannot parse %s. Its missing children nodes.", current_node->name);
			continue;
		}
		
		debug("Parsing searches result's update * entry.");
		status=NULL;
		if(!( (( status=user_status_parse_from_atom_entry(service, current_node->children, Searches ))) && status->id )){
			if(status) user_status_free(status);
			continue;
		}
		
		new_updates++;
		free_status=TRUE;
		debug("Adding UserStatus ID: %f, on <%s> to UberChickTreeView.", status->id, service->key);
		uberchick_tree_view_store_update(uberchick_tree_view, status);
		
		if(!save_oldest_id && status->id > last_notified_update ){
			if(notify){
				free_status=FALSE;
				g_timeout_add_seconds_full(notify_priority, uberchick_tree_view_notify_delay, (GSourceFunc)user_status_notify_on_timeout, status, (GDestroyNotify)user_status_free);
				uberchick_tree_view_notify_delay+=tweet_display_interval;
			}
		}
		
		if(!newest_update_id && status->id) newest_update_id=status->id;
		
		if(save_oldest_id && status->id)
			oldest_update_id=status->id;
		
		if(free_status) user_status_free(status);
	}
	
	if(new_updates && newest_update_id){
		/*TODO implement this only once it won't ending up causing bloating.
		 *cache_save_page(service, uri, xml->response_body);
		 */
		debug("Processing <%s>'s requested URI's: [%s] new update IDs", service->guid, uri);
		debug("Saving <%s>'s; update IDs for [%s];  newest ID: %f; unread ID: %f; oldest ID: %f.", service->guid, timeline, newest_update_id, unread_update_id, oldest_update_id );
		online_service_update_ids_set(service, timeline, newest_update_id, unread_update_id, oldest_update_id);
	}
	
	uber_free(timeline);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return new_updates;
}/*search_parse_results(service, xml, uri, uberchick_tree_view);*/

gchar *searches_format_timeline_from_uri(const gchar *uri){
	debug("Parsing searches timeline: from uri: <%s>.", uri);
	gchar **uri_split=g_strsplit_set( g_strrstr(uri, "/"), "?=", 3);
	gchar *search_phrase_encoded;
	if(!strstr(uri_split[2], "%"))
		search_phrase_encoded=g_strdup_printf("_%s", uri_split[2]);
	else{
		gchar **search_phrase_parts=g_strsplit( uri_split[2], "%", -1 );
		search_phrase_encoded=g_strjoinv("_", search_phrase_parts);
		g_strfreev(search_phrase_parts);
	}
	gchar *timeline=g_strdup_printf("%s%s", uri_split[0], search_phrase_encoded);
	g_strfreev(uri_split);
	uber_free(search_phrase_encoded);
	debug("Parsed searches timeline: <%s>; from uri: <%s>.", timeline, uri);
	return timeline;
}/*gchar *timeline=searches_format_timeline_from_uri(uri);*/

/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

