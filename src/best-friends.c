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

#include "config.h"
#include "program.h"

#include "best-friends.h"

#include "update-ids.h"
#include "online-services.defines.h"
#include "online-services-typedefs.h"
#include "online-services.h"
#include "online-service.h"

#include "main-window.h"

/********************************************************************************
 *      Project, system, & library headers.  eg #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/


/********************************************************************************
 *                prototypes for private methods & functions                    *
 ********************************************************************************/


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/


/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"OnlineServices:BestFriends:Updates:Requests:Settings:Setup:best-friends.c"
#include "debug.h"

static GtkTreeStore *best_friends_tree_store=NULL;

/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
void best_friends_set_tree_store(GtkTreeStore *tree_store){
	best_friends_tree_store=tree_store;
}/*best_friends_set_tree_store(main_window->private->best_friends_tree_store);*/

GtkTreeStore *best_friends_get_tree_store(void){
	return best_friends_tree_store;
}/*best_friends_get_tree_store();*/

gboolean best_friends_check_update_ids( OnlineService *service, const gchar *best_friends_user_name, gdouble update_id){
	gdouble unread_update_id=0.0;
	gchar *user_timeline=g_strdup_printf(API_TIMELINE_USER, best_friends_user_name );
	gdouble best_friends_newest_update_id=0.0, best_friends_unread_update_id=0.0, best_friends_oldest_update_id=0.0;
	update_ids_get( service, user_timeline, &best_friends_newest_update_id, &best_friends_unread_update_id, &best_friends_oldest_update_id );
	gboolean notify_of_new_update=FALSE, save_update_ids=FALSE;;
	unread_update_id=online_services_best_friends_tree_store_mark_as_unread(service, best_friends_user_name, update_id, best_friends_tree_store );
	if( unread_update_id > best_friends_unread_update_id ){
		debug("OnlineService: <%s>'s Best Friend: <%s@%s> has a new unread update.  Update->ID: %f.  Previous unread_update_id: %f.", service->guid, best_friends_user_name, service->uri, unread_update_id, best_friends_unread_update_id);
		notify_of_new_update=TRUE;
		save_update_ids=TRUE;
		best_friends_unread_update_id=unread_update_id;
	}
	
	if( update_id > best_friends_newest_update_id ){
		debug("OnlineService: <%s>'s Best Friend: <%s@%s> has a new newest update.  Update->ID: %f.  Previous newest_update_id: %f.", service->guid, best_friends_user_name, service->uri, update_id, best_friends_newest_update_id);
		if(!notify_of_new_update) notify_of_new_update=TRUE;
		if(!save_update_ids) save_update_ids=TRUE;
		best_friends_newest_update_id=update_id;
	}
	
	if( update_id < best_friends_oldest_update_id ){
		debug("OnlineService: <%s>'s Best Friend: <%s@%s> has a new oldest update.  Update->ID: %f.  Previous oldest_update_id: %f.", service->guid, best_friends_user_name, service->uri, update_id, best_friends_oldest_update_id);
		if(!save_update_ids) save_update_ids=TRUE;
		best_friends_oldest_update_id=update_id;
	}
	
	uber_free(user_timeline);
	return notify_of_new_update;
}/*best_friends_check_update_ids( service, update->user->user_name, update->id);*/

gboolean best_friends_new_update_notify(void){
	return FALSE;
}


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

