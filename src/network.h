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
#ifndef __NETWORK_H__
#define __NETWORK_H__

#define _GNU_SOURCE
#define _THREAD_SAFE

#include <gtk/gtk.h>
#include <glib.h>
#include <libsoup/soup.h>

/**********************************************************************
 *        System & library headers, eg #include <gdk/gdkkeysyms.h>    *
 **********************************************************************/
#include "config.h"
#include "tweet-list.h"

#include "online-service.h"

#include "online-services-typedefs.h"


/**********************************************************************
 *          Global method  & function prototypes                      *
 **********************************************************************/
G_BEGIN_DECLS

gboolean network_check_http(OnlineService *service, SoupMessage *xml);


void network_get_image(OnlineService *service, TweetList *tweet_list, const gchar *image_filename, const gchar *image_url, GtkTreeIter *iter);
void *network_cb_on_image(SoupSession *session, SoupMessage *msg, OnlineServiceWrapper *service_wrapper);


void network_post_status(gchar *text);
void network_send_message(OnlineService *service, const gchar *friend, gchar *text);
void *network_tweet_cb(SoupSession *session, SoupMessage *msg, OnlineServiceWrapper *service_wrapper);


void network_set_state_loading_timeline(const gchar *uri, ReloadState state);
void *network_display_timeline(SoupSession *session, SoupMessage *msg, OnlineServiceWrapper *service_wrapper);


G_END_DECLS
#endif /*  __NETWORK_H__ */
/**********************************************************************
 *                               eof                                  *
 **********************************************************************/

