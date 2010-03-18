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

#include "online-services.rest-uris.defines.h"
#include "online-services.h"
#include "online-service.types.h"
#include "online-service.h"
#include "update-ids.h"
#include "network.h"

#include "xml.h"

#include "users.types.h"
#include "users.h"

#include "main-window.h"
#include "uberchick-tree-view.h"
#include "preferences.defines.h"
#include "images.h"
#include "gconfig.h"

#include "best-friends.h"
#include "timelines.h"

#define	DEBUG_DOMAINS	"Parser:Requests:OnlineServices:Updates:UI:Refreshing:Dates:Times:timelines.c"
#include "debug.h"


/* Parse a timeline */
guint timelines_parse(OnlineService *service, SoupMessage *xml, const gchar *uri, UberChickTreeView *uberchick_tree_view, UpdateType update_type){
	const gchar	*timeline=g_strrstr(uri, "/");
	
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	xmlNode		*current_element=NULL;
	UserStatus 	*status=NULL;
	
	/* Count new tweets */
	gboolean	notify=gconfig_if_bool(PREFS_NOTIFY_ALL, TRUE);;
	
	gboolean	save_oldest_id=(uberchick_tree_view_has_loaded(uberchick_tree_view)?FALSE:TRUE);
	gboolean	notify_best_friends=gconfig_if_bool(PREFS_NOTIFY_BEST_FRIENDS, TRUE);
	
	guint		new_updates=0, notified_updates=0;
	gint		update_expiration=0, best_friends_expiration=0;
	gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_BEST_FRIENDS, &best_friends_expiration, 86400);
	
	gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	update_ids_get(service, timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	gdouble	last_notified_update=newest_update_id;
	newest_update_id=0.0;
	
	switch(update_type){
		case	Searches: case	Groups:
			debug("*ERROR:* Unsupported timeline.  timelines_parse requested to parse %s.  This method sould not have been called.", (update_type==Groups?"Groups":"Searches"));
			return 0;
			
		case	DMs:
			/*Direct Messages are kept for 4 weeks, by default.*/
			debug("Parsing DMs.");
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_DMS, &update_expiration, 2419200);
			if(!notify) notify=gconfig_if_bool(PREFS_NOTIFY_DMS, TRUE);
			if(!oldest_update_id) save_oldest_id=TRUE;
			else save_oldest_id=FALSE;
			break;
		
		case	Replies:
			/*By default Replies, & @ Mentions, from the last 7 days are loaded.*/
			debug("Parsing Replies and/or @ Mentions.");
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_REPLIES, &update_expiration, 604800);
			if(!notify) notify=gconfig_if_bool(PREFS_NOTIFY_REPLIES, TRUE);
			if(!oldest_update_id) save_oldest_id=TRUE;
			else save_oldest_id=FALSE;
			break;
		
		case	Faves:
			/*Favorite/Star'd updates are kept for 4 weeks, by default.*/
			debug("Parsing Faves.");
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_FAVES, &update_expiration, 2419200);
			if(!oldest_update_id) save_oldest_id=TRUE;
			else save_oldest_id=FALSE;
			break;
		
		case	BestFriends:
			/*Best Friends' updates are kept for 1 day, by default.*/
			debug("Parsing best friends updates.");
			notify=notify_best_friends;
			gconfig_get_int_or_default(PREFS_UPDATES_ARCHIVE_BEST_FRIENDS, &update_expiration, 86400);
			if(!save_oldest_id) save_oldest_id=TRUE;
			break;
		
		case	Homepage:	case	ReTweets:
		case	Timelines:	case	Users:
			debug("Parsing updates from someone I'm following.");
			if(!notify) notify=gconfig_if_bool(PREFS_NOTIFY_FOLLOWING, TRUE);
			if(!save_oldest_id) save_oldest_id=TRUE;
			break;
			
		case	Archive:
			debug("Parsing my own updates or favorites.");
			break;
		
		case	None:	default:
			/* These cases are never reached - they're just here to make gcc happy. */
			return 0;
	}
	if(!oldest_update_id && notify && ( update_type!=DMs || update_type!=Replies ) ) notify=FALSE;
	
	guint		update_notification_delay=uberchick_tree_view_get_notify_delay(uberchick_tree_view);
	const gint	update_notification_interval=10;
	const gint	notify_priority=(uberchick_tree_view_get_page(uberchick_tree_view)+1)*100;
	
	if(!(doc=xml_create_xml_doc_and_get_root_element_from_soup_message(xml, &root_element))){
		debug("Failed to parse xml document, <%s>'s timeline: %s.", service->key, timeline);
		xmlCleanupParser();
		return 0;
	}
	
	/* get updates or direct messages */
	debug("Parsing %s.", root_element->name);
	for(current_element=root_element; current_element; current_element=current_element->next) {
		if(current_element->type != XML_ELEMENT_NODE ) continue;
		
		if( g_str_equal(current_element->name, "statuses") || g_str_equal(current_element->name, "direct-messages") ){
			if(!current_element->children) continue;
			current_element=current_element->children;
			continue;
		}
		
		if(!( g_str_equal(current_element->name, "status") || g_str_equal(current_element->name, "direct_message") ))
			continue;
		
		if(!current_element->children){
			debug("*WARNING:* Cannot parse %s. Its missing children nodes.", current_element->name);
			continue;
		}
		
		debug("Parsing %s.", (g_str_equal(current_element->name, "status") ?"status update" :"direct message" ) );
		
		status=NULL;
		debug("Creating Status *.");
		if(!( (( status=user_status_parse(service, current_element->children, update_type ))) && status->id )){
			if(status) user_status_free(status);
			continue;
		}
		
		new_updates++;
		gboolean notify_of_new_update=FALSE;
		/* id_oldest_tweet is only set when update_type DMs or Replies */
		debug("Adding UserStatus from: %s, ID: %s, on <%s> to UberChickTreeView.", status->user->user_name, status->id_str, service->key);
		uberchick_tree_view_store_update(uberchick_tree_view, status);
		if( update_type!=BestFriends && update_type!=DMs && ( best_friends_is_user_best_friend(service, status->user->user_name) || ( status->retweet && best_friends_is_user_best_friend(service, status->retweeted_user_name) ) ) )
			if( (best_friends_check_update_ids( service, status->user->user_name, status->id)) && notify_best_friends)
				notify_of_new_update=TRUE;
			
		
		if( notify && !notify_of_new_update && !save_oldest_id && status->id > last_notified_update && strcasecmp(status->user->user_name, service->user_name) )
			notify_of_new_update=TRUE;
		
		if(!newest_update_id && status->id) newest_update_id=status->id;
		if(save_oldest_id && status->id) oldest_update_id=status->id;
		
		if(!notify_of_new_update)
			user_status_free(status);
		else{
			g_timeout_add_seconds_full(notify_priority, update_notification_delay, (GSourceFunc)user_status_notify_on_timeout, status, (GDestroyNotify)user_status_free);
			update_notification_delay-=update_notification_interval;
			notified_updates++;
		}
	}
	
	if(new_updates && newest_update_id){
		/*TODO implement
		 *cache_save_page(service, timeline, xml->response_body);
		 * this only once it won't ending up causing bloating.
		 */
		debug("Processing <%s>'s requested URI's: [%s] new update IDs", service->guid, timeline);
		debug("Saving <%s>'s; update IDs for [%s];  newest ID: %f; unread ID: %f; oldest ID: %f.", service->guid, timeline, newest_update_id, unread_update_id, oldest_update_id );
		update_ids_set(service, timeline, newest_update_id, unread_update_id, oldest_update_id);
	}
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return new_updates;
}/*timelines_parse(service, xml, timeline, uberchick_tree_view, update_type);*/

