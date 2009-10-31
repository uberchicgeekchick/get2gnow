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

/********************************************************
 *          My art, code, & programming.                *
 ********************************************************/
#define _GNU_SOURCE
#define _THREAD_SAFE



/********************************************************
 *        Project headers, eg #include "config.h"       *
 ********************************************************/
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <libsoup/soup.h>

#include "config.h"
#include "program.h"

#include "gconfig.h"

#include "online-services-typedefs.h"
#include "online-services.defines.h"
#include "online-services.h"
#include "online-service.types.h"
#include "online-service.h"
#include "online-service-wrapper.h"
#include "online-service-request.h"

#include "parser.h"
#include "groups.h"
#include "searches.h"
#include "cache.h"

#include "main-window.h"
#include "timelines-sexy-tree-view.h"

#include "preferences.h"

#include "network.h"

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define	DEBUG_DOMAINS	"Networking:OnlineServices:Updates:Requests:Users:Images:Authentication:Refreshing:Setup:Start-Up:network.c"
#include "debug.h"

typedef struct _NetworkTimelinesSexyTreeViewImageDL NetworkTimelinesSexyTreeViewImageDL;

struct _NetworkTimelinesSexyTreeViewImageDL{
	TimelinesSexyTreeView	*timelines_sexy_tree_view;
	gchar			*filename;
	GtkTreeIter		*iter;
};

/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static NetworkTimelinesSexyTreeViewImageDL *network_timelines_sexy_tree_view_image_dl_new(TimelinesSexyTreeView *timelines_sexy_tree_view, const gchar *filename, GtkTreeIter *iter);
static void network_timelines_sexy_tree_view_image_dl_free(NetworkTimelinesSexyTreeViewImageDL *image);

static void *network_retry(OnlineServiceWrapper *service_wrapper);

/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
static NetworkTimelinesSexyTreeViewImageDL *network_timelines_sexy_tree_view_image_dl_new(TimelinesSexyTreeView *timelines_sexy_tree_view, const gchar *filename, GtkTreeIter *iter){
	NetworkTimelinesSexyTreeViewImageDL *image=g_new0(NetworkTimelinesSexyTreeViewImageDL, 1);
	image->timelines_sexy_tree_view=timelines_sexy_tree_view;
	image->filename=g_strdup(filename);
	image->iter=iter;
	return image;
}/*network_timelines_sexy_tree_view_image_dl_new*/


void network_get_image(OnlineService *service, TimelinesSexyTreeView *timelines_sexy_tree_view, const gchar *image_filename, const gchar *image_url, GtkTreeIter *iter){
	debug("Downloading Image: %s.  GET: %s", image_filename, image_url);
	NetworkTimelinesSexyTreeViewImageDL *image=network_timelines_sexy_tree_view_image_dl_new(timelines_sexy_tree_view, image_filename, iter);
	
	online_service_request_uri(service, QUEUE, image_url, 0, NULL, network_cb_on_image, image, NULL);
}/*network_get_image*/


void *network_cb_on_image(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	const gchar *requested_uri=online_service_wrapper_get_requested_uri(service_wrapper);
	NetworkTimelinesSexyTreeViewImageDL *image=(NetworkTimelinesSexyTreeViewImageDL *)online_service_wrapper_get_user_data(service_wrapper);
	if(!( image && image->timelines_sexy_tree_view && image->filename && image->iter )){
		debug("**ERROR**: Missing image information.  Image filename: %s; Image iter: %s.", image->filename, (image->iter ?"valid" :"unknown") );
		return NULL;
	}
	
	gchar *image_filename=NULL, *error_message=NULL;
	if(!(parser_xml_error_check(service, requested_uri, xml, &error_message))){
		debug("Failed to download and save <%s> as <%s>.", requested_uri, image->filename);
		debug("Detailed error message: %s.", error_message);
		image_filename=cache_images_get_unknown_image_filename();
		main_window_statusbar_printf("Error adding avatar to TimelinesSexyTreeView.  GNOME's unknown-image will be used instead.");
	}else{
		debug("Saving avatar to file: %s", image->filename);
		if(!(g_file_set_contents(
					image->filename,
						xml->response_body->data,
						xml->response_body->length,
					NULL
		)))
			image_filename=cache_images_get_unknown_image_filename();
		else
			image_filename=g_strdup(image->filename);
		main_window_statusbar_printf("New avatar added to TimelinesSexyTreeView.");
	}
	
	timelines_sexy_tree_view_set_image(image->timelines_sexy_tree_view, image_filename, image->iter);
	
	uber_free(error_message);
	uber_free(image_filename);
	network_timelines_sexy_tree_view_image_dl_free(image);
	return NULL;
}/*network_cb_on_image(session, xml, user_data);*/

static void network_timelines_sexy_tree_view_image_dl_free(NetworkTimelinesSexyTreeViewImageDL *image){
	image->timelines_sexy_tree_view=NULL;
	uber_object_free(&image->filename, &image->iter, &image, NULL);
}/*network_image_free*/




void network_post_status(gchar *update){
	if(G_STR_EMPTY(update)) return;
	
	if(!( in_reply_to_service && gconfig_if_bool(PREFS_UPDATES_DIRECT_REPLY_ONLY, TRUE)))
		online_services_request(POST, API_POST_STATUS, NULL, network_update_posted, "post->update", update);
	else
		online_service_request(in_reply_to_service, POST, API_POST_STATUS, NULL, network_update_posted, "post->update", update);
}/*network_post_status(tweet);*/

void network_send_message(OnlineService *service, const gchar *friend, gchar *dm){
	online_service_request(service, POST, API_SEND_MESSAGE, NULL, network_update_posted, (gchar *)friend, dm);
}/*network_send_message(service, friend, dm);*/


void *network_update_posted(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	gchar *user_data=(gchar *)online_service_wrapper_get_user_data(service_wrapper);
	
	gboolean direct_message=FALSE;
	if(!g_str_equal("post->update", user_data)) direct_message=TRUE;
	
	gchar *message=NULL;
	if(!direct_message) message=g_strdup_printf("<%s>'s status", service->guid);
	else message=g_strdup_printf("Direct Message. To: <%s@%s> From: <%s>", user_data, service->uri, service->guid);
	
	gchar *error_message=NULL;
	if(!(parser_xml_error_check(service, online_service_wrapper_get_requested_uri(service_wrapper), xml, &error_message))){
		debug("%s couldn't be %s :'(", message, (direct_message?"sent":"updated"));
		debug("http error: #%i: %s", xml->status_code, xml->reason_phrase);
		
		statusbar_printf("%s couldn't be %s :'(", message, (direct_message?"sent":"updated"));
		statusbar_printf("http error: #%i: %s", xml->status_code, xml->reason_phrase);
		
		if(xml->status_code==100 && !online_service_wrapper_get_attempt(service_wrapper)){
			debug("Resubmitting Tweet/Status update to: [%s] per http response.", service->key);
			online_service_wrapper_reattempt(service_wrapper);
		}
	}else{
		debug("%s %s :-)", message, (direct_message?"succeeded":"updated"));
		statusbar_printf("%s %s :-)", message, (direct_message?"sent":"updated"));
	}
	uber_free(message);
	uber_free(error_message);
	
	debug("HTTP response: %s(#%d) while %s.", xml->reason_phrase, xml->status_code, (direct_message ?"sending direct messaging" :"updating your status") );
	
	if( !direct_message && xml->status_code==404 && (service->micro_blogging_service==Identica || service->micro_blogging_service==StatusNet)){
		debug("Resubmitting status update to: <%s> due to StatusNet bug.", service->key);
		online_service_request(service, POST, (direct_message?API_SEND_MESSAGE:API_POST_STATUS), NULL, network_update_posted, user_data, online_service_wrapper_get_form_data(service_wrapper));
	}
	return NULL;
}/*network_update_posted*/




void network_set_state_loading_timeline(const gchar *uri, ReloadState state){
	const gchar *notice_prefix=NULL;
	switch(state){
		case Retry:
			notice_prefix=_("Re-trying");
			break;
			
		case Reload:
			notice_prefix=_("Re-loading");
			break;
		
		case Reattempt:
			notice_prefix=_("Re-attempting");
			break;
		
		case Load:
		default:
			notice_prefix=_("Loading");
			break;
	}
	const gchar *notice_suffix=NULL;
	if( !gconfig_if_bool(PREFS_URLS_EXPANSION_DISABLED, FALSE) && !gconfig_if_bool(PREFS_URLS_EXPANSION_SELECTED_ONLY, TRUE) )
		notice_suffix=_("  URL auto-expansion is enabled loading timelines may take a while.");
	debug("%s request for %s.%s", notice_prefix, uri, notice_suffix);
	statusbar_printf("%s request for %s.%s", notice_prefix, uri, (notice_suffix ?_("  This may take several moments.") :"" ) );
}/*network_set_state_loading_timeline(uri, Load);*/


void *network_display_timeline(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	TimelinesSexyTreeView *timelines_sexy_tree_view=(TimelinesSexyTreeView *)online_service_wrapper_get_user_data(service_wrapper);
	UpdateMonitor monitoring=(UpdateMonitor)online_service_wrapper_get_form_data(service_wrapper);
	const gchar *requested_uri=online_service_wrapper_get_requested_uri(service_wrapper);
	if(!IS_TIMELINES_SEXY_TREE_VIEW(timelines_sexy_tree_view))
		return NULL;
	
	gchar *error_message=NULL;
	if(!(parser_xml_error_check(service, requested_uri, xml, &error_message)))
		if(!online_service_wrapper_get_attempt(service_wrapper) && xml->status_code==100 ){
			uber_free(error_message);
			return network_retry(service_wrapper);
		}
	uber_free(error_message);
	
	gchar		**uri_split=g_strsplit( g_strrstr(requested_uri, "/"), "?", 2);
	gchar		*timeline=g_strdup(uri_split[0]);
			g_strfreev(uri_split);
	
	debug("Started processing <%s>'s timeline: %s.  Requested URI: %s.", service->key, timeline, requested_uri);
	
	guint new_updates=0;
	switch(monitoring){
		case None:
			debug("Attempting to parse an unsupport network request.");
			break;
		case Searches:
			new_updates=searches_parse_results(service, xml, timeline, timelines_sexy_tree_view);
			break;
		case Groups:
			new_updates=groups_parse_conversation(service, xml, timeline, timelines_sexy_tree_view);
			break;
		case DMs:
		case Replies:
		case Faves:
		case Updates:
		case Timelines:
		case Users:
		case Archive:
		case BestFriends:
		default:
			new_updates=parse_timeline(service, xml, timeline, timelines_sexy_tree_view, monitoring);
			break;
	}
	
	if(!online_service_wrapper_get_attempt(service_wrapper) && !new_updates && !g_strrstr(requested_uri, "?since_id=") && timelines_sexy_tree_view_has_loaded(timelines_sexy_tree_view) > 0 && xml->status_code==200){
		uber_free(timeline);
		return network_retry(service_wrapper);
	}
	
	if(new_updates)
		debug("Total tweets in this timeline: %d.", new_updates);
	
	timelines_sexy_tree_view_complete(timelines_sexy_tree_view);
	uber_free(timeline);
	
	return NULL;
}/*network_display_timeline(session, xml, service_wrapper);*/

static void *network_retry(OnlineServiceWrapper *service_wrapper){
	const gchar *requested_uri=online_service_wrapper_get_requested_uri(service_wrapper);
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	debug("Resubmitting: %s to <%s>.", requested_uri, service->uri);
	online_service_wrapper_reattempt(service_wrapper);
	return NULL;
}/*network_retry(new_timeline, service_wrapper, monitoring);*/



/********************************************************
 *                       eof                            *
 ********************************************************/
