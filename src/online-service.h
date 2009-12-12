/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright(c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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
 * License("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
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
 * user-visible display as defined in Section 6.4(d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright(C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright(C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright(C) of the User the data is connected to.
 * Users may lisences their data under the terms of an OSI approved or Creative Commons
 * license.  Users must be allowed to select their choice of license for each piece of data
 * on an individual bases and cannot be blanketly applied to all of the Users.  The User may
 * select a default license for their data.  All of the Software's data pertaining to each
 * User must be fully accessible, exportable, and deletable to that User.
 */

/********************************************************
 *          My art, code, & programming.                *
 ********************************************************/
#ifndef __ONLINE_SERVICE_H__
#define __ONLINE_SERVICE_H__

#ifndef	_GNU_SOURCE 
#	define _GNU_SOURCE
#endif

#ifndef _THREAD_SAFE
#	define _THREAD_SAFE
#endif


/**********************************************************************
 *        System & library headers, eg #include <gdk/gdkkeysyms.h>    *
 **********************************************************************/
#include <fcntl.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <libsoup/soup.h>

#include "timer.h"
#include "online-services-typedefs.h"
#include "users.types.h"


/*********************************************************************
 *        Objects, structures, and etc typedefs                      *
 *********************************************************************/

#ifndef ONLINE_SERVICE_MAX_REQUESTS
#	define ONLINE_SERVICE_MAX_REQUESTS 5
#endif

G_BEGIN_DECLS
/********************************************************
 *          Global method  & function prototypes        *
 ********************************************************/
OnlineService *online_service_open(const gchar *guid);
OnlineService *online_service_new(const gchar *uri, const gchar *user_name, const gchar *password, gboolean enabled, gboolean https, gboolean auto_connect);
void online_service_display_debug_details(OnlineService *service, gboolean new_service, const char *action);
gboolean online_service_save(OnlineService *service, const gchar *password, gboolean enabled, gboolean https, gboolean auto_connect);
gboolean online_service_delete(OnlineService *service, gboolean service_cache_rm_rf);

gboolean online_service_validate_guid(OnlineService *service, const gchar *user_name, const gchar *uri);

gint online_service_best_friends_tree_store_fill( OnlineService *service, GtkTreeStore *tree_store );
gint online_service_best_friends_tree_store_validate( OnlineService *service, GtkTreeStore *tree_store );
gboolean online_service_is_user_best_friend( OnlineService *service, const gchar *user_name );
gboolean online_service_best_friends_add( OnlineService *service, const gchar *user_name );
gboolean online_service_best_friends_drop( OnlineService *service, GtkWindow *parent, const gchar *user_name );
void online_service_best_friends_tree_store_update_check(OnlineServiceWrapper *online_service_wrapper, SoupMessage *xml, User *user);
void online_service_best_friends_tree_store_free( OnlineService *service, GtkTreeStore *tree_store );

gboolean online_service_connect(OnlineService *service);
gboolean online_service_login(OnlineService *service, gboolean temporary_connection);
gboolean online_service_refresh(OnlineService *service);
gboolean online_service_reconnect(OnlineService *service);
void online_service_disconnect(OnlineService *service, gboolean no_state_change);

gchar *online_service_request_uri_create(OnlineService *service, const gchar *uri);

void online_service_fetch_profile( OnlineService *service, const gchar *user_name, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_user_parser_func );

gboolean online_service_validate_session(OnlineService *service, const gchar *requested_uri);

SoupMessage *online_service_request(OnlineService *service, RequestMethod request_method, const gchar *uri, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data);

SoupMessage *online_service_request_uri(OnlineService *service, RequestMethod request_method, const gchar *uri, guint8 attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data);

void online_service_free(OnlineService *service, gboolean no_state_change);

G_END_DECLS
#endif /* __ONLINE_SERVICE_H__ */
