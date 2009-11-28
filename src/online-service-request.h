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

/**********************************************************************
 *          My art, code, & programming.                              *
 **********************************************************************/
#ifndef __ONLINE_SERVICE_REQUEST_H__
#define __ONLINE_SERVICE_REQUEST_H__

#define _GNU_SOURCE
#define _THREAD_SAFE


/**********************************************************************
 *        System & library headers, eg #include <gdk/gdkkeysyms.h>    *
 **********************************************************************/
#include <gtk/gtk.h>
#include <libxml/parser.h>
#include <libsoup/soup.h>

#include "online-services-typedefs.h"


/**********************************************************************
 *        Objects, structures, and etc typedefs                       *
 **********************************************************************/
typedef struct _OnlineServiceRequest OnlineServiceRequest;
typedef enum _RequestAction RequestAction;


extern OnlineService *selected_service;

extern gdouble in_reply_to_status_id;
extern OnlineService *in_reply_to_service;

/**********************************************************************
 *          Global method & function prototypes                      *
 **********************************************************************/
G_BEGIN_DECLS

const gchar *online_service_request_method_to_string(RequestMethod request_method);
const gchar *online_service_request_action_to_string(RequestAction action);

void online_service_request_view_profile(OnlineService *service, GtkWindow *parent, const gchar *user_name);
void online_service_request_view_updates_new(OnlineService *service, GtkWindow *parent, const gchar *user_name);
void online_service_request_view_updates(OnlineService *service, GtkWindow *parent, const gchar *user_name);
void online_service_request_view_forwards(OnlineService *service, GtkWindow *parent, const gchar *user_name);
void online_service_request_follow(OnlineService *service, GtkWindow *parent, const gchar *user_name);
void online_service_request_unfollow(OnlineService *service, GtkWindow *parent, const gchar *user_name);
void online_service_request_best_friend_add(OnlineService *service, GtkWindow *parent, const gchar *user_name);
void online_service_request_best_friend_drop(OnlineService *service, GtkWindow *parent, const gchar *user_name);
void online_service_request_block(OnlineService *service, GtkWindow *parent, const gchar *user_name);
void online_service_request_unblock(OnlineService *service, GtkWindow *parent, const gchar *user_name);
void online_service_request_fave(OnlineService *service, GtkWindow *parent, const gchar *user_name);
void online_service_request_unfave(OnlineService *service, GtkWindow *parent, const gchar *user_name);

void *online_service_request_main_quit(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper);

void online_service_request_selected_update_view_updates_new(void);
void online_service_request_selected_update_view_updates(void);
void online_service_request_selected_update_view_forwards(void);
void online_service_request_selected_update_view_profile(void);
void online_service_request_selected_update_best_friend_add(void);
void online_service_request_selected_update_best_friend_drop(void);
void online_service_request_selected_update_follow(void);
void online_service_request_selected_update_unfollow(void);
void online_service_request_selected_update_block(void);
void online_service_request_selected_update_unblock(void);

void online_service_request_selected_update_save_fave(void);
void online_service_request_selected_update_destroy_fave(void);

gboolean online_service_request_is_update_selected(void);
void online_service_request_set_selected_update(OnlineService *service, const gdouble id, const gdouble user_id, const gchar *user_name, const gchar *update);
OnlineService *online_service_request_selected_update_get_service(void);
gdouble online_service_request_selected_update_get_id(void);
const gchar *online_service_request_selected_update_get_user_name(void);
gdouble online_service_request_selected_update_get_user_id(void);
gchar *online_service_request_selected_update_get_update_text(void);
gboolean online_service_request_selected_update_reply(void);
gboolean online_service_request_selected_update_forward(void);
void online_service_request_unset_selected_update(void);

void online_service_request_popup_select_service(void);
void online_service_request_popup_profile(void);
void online_service_request_popup_updates_new(void);
void online_service_request_popup_updates(void);
void online_service_request_popup_forwards(void);
void online_service_request_popup_follow(void);
void online_service_request_popup_best_friend_add(void);
void online_service_request_popup_best_friend_drop(void);
void online_service_request_popup_unfollow(void);
void online_service_request_popup_block(void);
void online_service_request_popup_unblock(void);
gboolean online_service_request_popup_confirmation_dialog(const gchar *gconfig_path, const gchar *message1, const gchar *message2, GFunc func, gpointer user_data);


G_END_DECLS


#endif /*__ONLINE_SERVICE_REQUEST_H__*/
/**********************************************************************
 *                               eof                                  *
 **********************************************************************/

