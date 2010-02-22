/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the Reciprocal Public License (RPL).
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

#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "program.h"
#include "update-ids.h"
#include "gconfig.h"

#include "preferences.defines.h"
#include "online-services.typedefs.h"
#include "online-service.typedefs.h"


/********************************************************************************
 *      Project, system, & library headers.  eg #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/


/********************************************************************************
 *                        defines, macros, methods, & etc                       *
 ********************************************************************************/


/********************************************************************************
 *                        objects, structs, and enum typedefs                   *
 ********************************************************************************/


/********************************************************************************
 *                prototypes for private methods & functions                    *
 ********************************************************************************/
static void update_id_get(OnlineService *service, const gchar *timelines_gconfig_prefs_path, const gchar *key, gdouble *update_id);
static void update_id_set(OnlineService *service, const gchar *timelines_gconfig_prefs_path, const gchar *key, gdouble update_id);
static gchar *update_ids_format_timeline_for_gconfig(const gchar *timeline);


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/


/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"OnlineServices:Network:Updates:Requests:GConf:GConfig:Settings:IDs:Update IDs:update-ids.c"
#include "debug.h"


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
gboolean update_ids_check(OnlineService *service, const gchar *timeline, gdouble update_id, gboolean check_oldest){
	if(!service) return FALSE;
	gboolean	ids_set=FALSE;
	gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	update_ids_get(service, timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	debug("Checking <%s>'s; update IDs for [%s]; against update ID: %f; newest update ID: %f; unread update ID: %f; oldest update ID: %f.", service->guid, timeline, update_id, newest_update_id, unread_update_id, oldest_update_id );
	if( update_id > newest_update_id ){
		ids_set=TRUE;
		newest_update_id=update_id;
	}
	
	if( update_id > unread_update_id ){
		if(!ids_set) ids_set=TRUE;
		unread_update_id=update_id;
	}
	
	if( check_oldest && update_id > oldest_update_id ){
		if(!ids_set) ids_set=TRUE;
		oldest_update_id=update_id;
	}
	
	if(ids_set){
		debug("Saving <%s>'s; update IDs for [%s]; against update ID: %f; newest update ID: %f; unread update ID: %f; oldest update ID: %f.", service->guid, timeline, update_id, newest_update_id, unread_update_id, oldest_update_id );
		update_ids_set(service, timeline, newest_update_id, unread_update_id, oldest_update_id);
	}
	
	return ids_set;
}/*update_ids_check( service, timeline, index_update_id, FALSE );*/

void update_ids_get(OnlineService *service, const gchar *timeline, gdouble *newest_update_id, gdouble *unread_update_id, gdouble *oldest_update_id){
	if(!(service && G_STR_N_EMPTY(timeline) )) return;
	gchar *timelines_gconfig_prefs_path=update_ids_format_timeline_for_gconfig(timeline);
	debug("Retrieving: <%s>'s update IDs [%s]", service->key, timeline);
	debug("Using gconfig key: [%s].", timelines_gconfig_prefs_path );
	update_id_get( service, timelines_gconfig_prefs_path, "newest", newest_update_id );
	update_id_get( service, timelines_gconfig_prefs_path, "unread", unread_update_id );
	update_id_get( service, timelines_gconfig_prefs_path, "oldest", oldest_update_id );
	debug("Retrieving: <%s>'s update IDs [%s]", service->key, timeline);
	debug("Using gconfig key: [%s].", timelines_gconfig_prefs_path );
	uber_free(timelines_gconfig_prefs_path);
}/*update_ids_get(service, "/friends.xml", id_newest_update, id_oldest_update);*/

static void update_id_get( OnlineService *service, const gchar *timelines_gconfig_prefs_path, const gchar *key, gdouble *update_id ){
	if(!( service && G_STR_N_EMPTY(timelines_gconfig_prefs_path) )) return;
	/* INFO:
	 * GCONF_PATH:		ONLINE_SERVICE_PREFIX: ONLINE_SERVICE_UPDATE_IDS_GCONF_KEY:
	 * "(/apps/get2gnow)	(/online-services/%s)		/xml-cache%s/%s"
	 * 				service->key			/timeline.xml	(newest|oldest|unread)
	 */
	gchar *prefs_path=NULL, *swap_id_str=NULL;
	gdouble swap_id;
	gboolean success;
	
	prefs_path=g_strdup_printf(ONLINE_SERVICE_UPDATE_IDS_GCONF_KEY, service->key, timelines_gconfig_prefs_path, key);
	debug("Retrieving <%s>'s %s update ID.  From gconf key: [%s].", service->key, key, prefs_path);
	success=gconfig_get_string(prefs_path, &swap_id_str);
	
	if(!(success && swap_id_str)) swap_id=0.0;
	else swap_id=g_ascii_strtod(swap_id_str, NULL);
	
	if(swap_id>0) *update_id=swap_id;
	debug("Retrieved <%s>'s %s update ID: %f(from string %s) from gconf key: [%s].", service->key, key, *update_id, swap_id_str, prefs_path);
	uber_free(prefs_path);
	uber_free(swap_id_str);
}/*update_id_get( service, "/friends.xml", "newest", &newest_update_id );*/

void update_ids_set( OnlineService *service, const gchar *timeline, gdouble newest_update_id, gdouble unread_update_id, gdouble oldest_update_id ){
	if(!(service && G_STR_N_EMPTY(timeline) )) return;
	gchar *timelines_gconfig_prefs_path=update_ids_format_timeline_for_gconfig(timeline);
	debug("Saving: <%s>'s update IDs [%s]", service->key, timeline);
	debug("Using gconfig key: [%s].", timelines_gconfig_prefs_path);
	update_id_set(service, timelines_gconfig_prefs_path, "newest", newest_update_id);
	update_id_set(service, timelines_gconfig_prefs_path, "unread", unread_update_id);
	update_id_set(service, timelines_gconfig_prefs_path, "oldest", oldest_update_id);
	debug("Saved: <%s>'s update IDs [%s]", service->key, timeline);
	debug("Using gconfig key: [%s].", timelines_gconfig_prefs_path);
	uber_free(timelines_gconfig_prefs_path);
}/*update_ids_set(service, "/friends.xml", id_newest_update, id_oldest_update);*/

static void update_id_set( OnlineService *service, const gchar *timelines_gconfig_prefs_path, const gchar *key, gdouble update_id ){
	if(!(service && G_STR_N_EMPTY(timelines_gconfig_prefs_path) )) return;
	/* INFO:
	 * GCONF_PATH:		ONLINE_SERVICE_PREFIX: ONLINE_SERVICE_UPDATE_IDS_GCONF_KEY:
	 * "(/apps/get2gnow)	(/online-services/%s)		/xml-cache%s/%s"
	 * 				service->key			/timeline.xml (newest|oldest|unread)
	 */
	gchar *prefs_path=NULL, *swap_id_str=NULL;
	gboolean success;
	
	prefs_path=g_strdup_printf(ONLINE_SERVICE_UPDATE_IDS_GCONF_KEY, service->key, timelines_gconfig_prefs_path, key);
	swap_id_str=gdouble_to_str(update_id);
	debug("Saving <%s>'s %s update ID: %f(from string: %s) to gconfig path: [%s].", service->key, key, update_id, swap_id_str, prefs_path);
	success=gconfig_set_string(prefs_path, swap_id_str);
	debug("Saved <%s>'s %s update ID: %f(from string: %s) to gconfig path: [%s].", service->key, key, update_id, swap_id_str, prefs_path);
	uber_free(prefs_path);
	uber_free(swap_id_str);
}/*online_service_id_set( service, "/friends.xml", "newest", &newest_update_id );*/

static gchar *update_ids_format_timeline_for_gconfig(const gchar *uri){
	debug("Formatting timeline for use with gconf: from uri: <%s>.", uri);
	
	if(!strstr(uri, "%"))
		return g_strdup( ( strstr(uri, "=") ?g_strrstr(uri, "=") :g_strrstr(uri, "/") ) );
	
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
}/*gchar *timeline=update_ids_format_timeline_for_gconfig(uri);*/

/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

