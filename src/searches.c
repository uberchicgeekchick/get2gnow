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
#include "update-viewer.h"
#include "preferences.h"
#include "images.h"
#include "label.h"
#include "gconfig.h"

#include "parser.h"
#include "searches.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"Search:Groups:Parser:Requests:OnlineServices:Tweets:UI:Refreshing:Dates:Times:Search.c"
#include "debug.h"


/* Parse a timeline XML file */
guint searches_parse_results(OnlineService *service, SoupMessage *xml, const gchar *uri, UpdateViewer *update_viewer){
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	xmlNode		*current_node=NULL;
	UserStatus 	*status=NULL;
	
	/* Count new tweets */
	guint		new_updates=0;
	
	gchar		**uri_split=g_strsplit( g_strrstr(uri, "/"), "?", 2);
	gchar		*timeline=g_strdup(uri_split[0]);
			g_strfreev(uri_split);
	
	gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	online_service_update_ids_get(service, timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	gdouble	last_notified_update=newest_update_id;
	newest_update_id=0.0;
	
	gboolean	has_loaded=update_viewer_has_loaded(update_viewer);
	gboolean	notify=( (oldest_update_id && has_loaded) ?gconfig_if_bool(PREFS_NOTIFY_FOLLOWING, TRUE) :FALSE );
	gboolean	save_oldest_id=(has_loaded?FALSE:TRUE);
	
	guint		update_viewer_notify_delay=update_viewer_get_notify_delay(update_viewer);
	const gint	tweet_display_interval=10;
	const gint	notify_priority=(update_viewer_get_page(update_viewer)-1)*100;
	
	if(!(doc=parse_xml_doc(xml, &root_element))){
		debug("Failed to parse xml document, timeline: %s; uri: %s.", timeline, uri);
		xmlCleanupParser();
		uber_free(timeline);
		return 0;
	}
	
	/* get tweets or direct messages */
	debug("Parsing %s timeline.", root_element->name);
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
		if(!( (( status=user_status_parse(service, current_node->children, Searches ))) && status->id )){
			if(status) user_status_free(status);
			continue;
		}
		
		new_updates++;
		free_status=TRUE;
		debug("Adding UserStatus from: %s, ID: %f, on <%s> to UpdateViewer.", status->user->user_name, status->id, service->key);
		update_viewer_store(update_viewer, status);
		
		if(!save_oldest_id && status->id > last_notified_update && strcasecmp(status->user->user_name, service->user_name) ){
			if(notify){
				free_status=FALSE;
				g_timeout_add_seconds_full(notify_priority, update_viewer_notify_delay, (GSourceFunc)user_status_notify_on_timeout, status, (GDestroyNotify)user_status_free);
				update_viewer_notify_delay+=tweet_display_interval;
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
}/*search_parse_results(service, xml, uri, update_viewer);*/

/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

