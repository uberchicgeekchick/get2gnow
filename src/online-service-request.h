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


/**********************************************************************
 *          Global method & function prototypes                      *
 **********************************************************************/
G_BEGIN_DECLS

gchar *online_service_request_action_to_string(RequestAction action);

void online_service_request_view_profile(OnlineService *service, GtkWindow *parent, const gchar *user_data);
void online_service_request_view_tweets(OnlineService *service, GtkWindow *parent, const gchar *user_data);
void online_service_request_follow(OnlineService *service, GtkWindow *parent, const gchar *user_data);
void online_service_request_unfollow(OnlineService *service, GtkWindow *parent, const gchar *user_data);
void online_service_request_block(OnlineService *service, GtkWindow *parent, const gchar *user_data);
void online_service_request_unblock(OnlineService *service, GtkWindow *parent, const gchar *user_data);
void online_service_request_fave(OnlineService *service, GtkWindow *parent, const gchar *user_data);
void online_service_request_unfave(OnlineService *service, GtkWindow *parent, const gchar *user_data);

void *online_service_request_main_quit(SoupSession *session, SoupMessage *msg, OnlineServiceWrapper *service_wrapper);

void online_service_request_selected_tweet_view_tweets(void);
void online_service_request_selected_tweet_view_profile(void);

void online_service_request_selected_tweet_follow(void);
void online_service_request_selected_tweet_unfollow(void);
void online_service_request_selected_tweet_block(void);
void online_service_request_selected_tweet_unblock(void);

void online_service_request_selected_tweet_save_fave(void);
void online_service_request_selected_tweet_destroy_fave(void);

void set_selected_tweet(OnlineService *service, const gdouble id, const gdouble user_id, const gchar *user_name, const gchar *tweet);
OnlineService *selected_tweet_get_service(void);
gdouble selected_tweet_get_id(void);
gchar *selected_tweet_get_user_name(void);
gdouble selected_tweet_get_user_id(void);
gchar *selected_tweet_reply_to_strdup(gboolean retweet);
void selected_tweet_reply(void);
void selected_tweet_retweet(void);
void unset_selected_tweet(void);

void online_service_request_popup_select_service(void);
void online_service_request_popup_friend_profile(void);
void online_service_request_popup_friend_tweets(void);
void online_service_request_popup_friend_follow(void);
void online_service_request_popup_friend_unfollow(void);
void online_service_request_popup_friend_block(void);
void online_service_request_popup_friend_unblock(void);
gboolean online_service_request_popup_confirmation_dialog(const gchar *gconfig_path, const gchar *message1, const gchar *message2, GFunc func, gpointer user_data);


G_END_DECLS


#endif /*__ONLINE_SERVICE_REQUEST_H__*/
/**********************************************************************
 *                               eof                                  *
 **********************************************************************/

