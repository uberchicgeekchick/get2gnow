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
#ifndef __ONLINE_SERVICE_WRAPPER_H__
#define __ONLINE_SERVICE_WRAPPER_H__

#define _GNU_SOURCE
#define _THREAD_SAFE


/**********************************************************************
 *        System & library headers, eg #include <gdk/gdkkeysyms.h>    *
 **********************************************************************/
#include <glib.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <libsoup/soup.h>

#include "online-services-typedefs.h"

/**********************************************************************
 *  Macros, constants, objects, structures, and enum typedefs         *
 **********************************************************************/


/**********************************************************************
 *          Global method & function prototypes                       *
 **********************************************************************/
G_BEGIN_DECLS

OnlineServiceWrapper *online_service_wrapper_new(OnlineService *service, RequestMethod request, const gchar *request_uri, guint attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data);

void online_service_wrapper_retry(OnlineServiceWrapper *online_service_wrapper);
void online_service_wrapper_run(OnlineServiceWrapper *online_service_wrapper, SoupSession *session, SoupMessage *xml);

OnlineService *online_service_wrapper_get_online_service(OnlineServiceWrapper *online_service_wrapper);
RequestMethod online_service_wrapper_get_request_method(OnlineServiceWrapper *online_service_wrapper);
const gchar *online_service_wrapper_get_requested_uri(OnlineServiceWrapper *online_service_wrapper);

guint online_service_wrapper_reattempt(OnlineServiceWrapper *online_service_wrapper);
guint online_service_wrapper_get_attempt(OnlineServiceWrapper *online_service_wrapper);

OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_wrapper_get_online_service_soup_session_callback_return_processor_func(OnlineServiceWrapper *online_service_wrapper);
OnlineServiceSoupSessionCallbackFunc online_service_wrapper_get_callback(OnlineServiceWrapper *online_service_wrapper);
gpointer online_service_wrapper_get_user_data(OnlineServiceWrapper *online_service_wrapper);
gpointer online_service_wrapper_get_form_data(OnlineServiceWrapper *online_service_wrapper);

void online_service_wrapper_free(OnlineServiceWrapper *online_service_wrapper);


G_END_DECLS
#endif /* __ONLINE_SERVICE_WRAPPER_H__ */
/**********************************************************************
 *                               eof                                  *
 **********************************************************************/
