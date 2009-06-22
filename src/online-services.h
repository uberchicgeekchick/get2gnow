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
#ifndef __ONLINE_SERVICES_H__
#define __ONLINE_SERVICES_H__


/**********************************************************************
 *        System & library headers, eg #include <gdk/gdkkeysyms.h>    *
 **********************************************************************/
#include <fcntl.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <libsoup/soup.h>

#include "online-service.h"
#include "online-services-typedefs.h"


/*********************************************************************
 *        Objects, structures, and etc typedefs                      *
 *********************************************************************/
extern OnlineServices *online_services;


/********************************************************
 *          Global method  & function prototypes        *
 ********************************************************/
OnlineServices *online_services_init(void);

gboolean online_services_login(OnlineServices *services);
gboolean online_services_relogin(OnlineServices *services);
void online_services_disconnect(OnlineServices *services);

OnlineService *online_services_save_service(OnlineServices *services, OnlineService *service, gboolean enabled, const gchar *url, gboolean https, const gchar *username, const gchar *password, gboolean auto_connect);
void online_services_delete_service(OnlineServices *services, OnlineService *service);

/**
 * @returns:	-2 if services->total equals 0
 *		services->total if count equals 0
 * 		-1, 0, 1, if services->total is greater than, equal to, or less than count.
 */
gint online_services_has_total(OnlineServices *services, guint count);


/**
 * @returns:	-2 if services->connected equals 0
 *		services->connected if count equals 0
 * 		-1, 0, 1, if services->connected is greater than, equal to, or less than count.
 */
gint online_services_has_connected(OnlineServices *services, guint count);

OnlineService *online_services_connected_get_first(OnlineServices *services);
OnlineService *online_services_connected_get_last(OnlineServices *services);

void online_services_request(OnlineServices *services, RequestMethod request, const gchar *uri, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data);

void online_services_increment_connected(OnlineServices *services);

void online_services_decrement_connected(OnlineServices *services, gboolean no_state_change);

gboolean online_services_combo_box_fill(OnlineServices *services, GtkComboBox *combo_box, GtkListStore *list_store, gboolean connected_only);

void online_services_deinit(OnlineServices *services);

#endif /* __ONLINE_SERVICES_H__ */

