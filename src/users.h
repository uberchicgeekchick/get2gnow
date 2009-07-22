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
#ifndef __USERS_H__
#define __USERS_H__

#define _GNU_SOURCE
#define _THREAD_SAFE


/**********************************************************************
 *        System & library headers, eg #include <gdk/gdkkeysyms.h>    *
 **********************************************************************/
#include <gtk/gtk.h>
#include <libxml/parser.h>
#include <libsoup/soup.h>
#include "online-services-typedefs.h"
#include "tweet-list.h"


/**********************************************************************
 *        Objects, structures, and etc typedefs                       *
 **********************************************************************/
#ifndef view_profile
#define view_profile(service, user_name, parent)	user_profile_viewer_show(service, user_name, parent);
#endif

typedef struct _User User;
typedef struct _UserStatus UserStatus;

/**********************************************************************
 *          Global method & function prototypes                      *
 **********************************************************************/
gdouble user_get_id(User *user);
const gchar *user_status_get_id_str(UserStatus *status);
const gchar *user_get_user_name(User *user);
const gchar *user_get_user_nick(User *user);
OnlineService *user_get_online_service(User *user);
gboolean user_is_follower(User *user);

User *user_fetch_profile(OnlineService *service, const gchar *user_name);
User *user_parse_profile(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper);

gboolean user_download_avatar(User *user);
void user_profile_viewer_show(OnlineService *service, const gchar *user_name, GtkWindow *parent);

User *user_parse_node(OnlineService *service, xmlNode *root_element);

void user_free(User *user);

UserStatus *user_status_parse(OnlineService *service, xmlNode *root_element, UpdateMonitor tweet_list);
void user_status_store(UserStatus *status, TweetList *tweet_list);

OnlineService *user_status_get_online_service(UserStatus *status);

gdouble user_status_get_id(UserStatus *status);
const gchar *user_status_get_id_str(UserStatus *status);
const gchar *user_status_get_user_name(UserStatus *status);
const gchar *user_status_get_notification(UserStatus *status);
gint user_status_get_created_seconds_ago(UserStatus *status);

void user_status_free(UserStatus *status);


#endif /*__USERS_H__*/
/**********************************************************************
 *                               eof                                  *
 **********************************************************************/

