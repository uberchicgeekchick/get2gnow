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

#include "timer.h"

/*********************************************************************
 *        Objects, structures, and etc typedefs                      *
 *********************************************************************/
/* Twitter API */
#define API_CLIENT_AUTH		"greettweetknow"

/* Laconica API */
/*#define	OPEN_CLIENT		"<a href=\"http://uberchicgeekchick.com/?projects=" PACKAGE_TARNAME "\">" PACKAGE_TARNAME "</a>"*/
#define		OPEN_CLIENT		"get2gnow"


#define	PREFS_AUTH_SERVICES		PREFS_PATH "/auth/services"
#define PREFS_AUTH_PREFIX		PREFS_PATH "/auth/%s"
#define	PREFS_AUTH_PASSWORD		PREFS_AUTH_PREFIX "/password"
#define	PREFS_AUTH_AUTO_CONNECT		PREFS_AUTH_PREFIX "/auto_connect"
#define	PREFS_AUTH_ENABLED		PREFS_AUTH_PREFIX "/enabled"

typedef enum{
	POST,
	GET,
	QUEUE,
} RequestMethod;

/**
 *@accounts contains an 'OnlineServics' object for each account that's available.
 */
typedef struct {
	guint		total;
	guint		connected;
	GSList		*keys;
	GList		*accounts;
} OnlineServices;

typedef enum {
	Laconica,
	Twitter,
} SupportedREST;

typedef struct {
	SoupSession	*session;
	RateLimitTimer	*timer;
	
	gboolean	connected;
	guint		logins;
	
	gboolean	enabled;
	gboolean	auto_connect;
	
	gchar		*key;
	gchar		*decoded_key;
	SupportedREST	which_rest;
	gchar		*uri;
	gchar		*username;
	gchar		*password;
} OnlineService;

typedef struct{
	OnlineService	*service;
	gpointer	user_data;
} OnlineServiceCBWrapper;

typedef enum{
	UrlString,
	OnlineServicePointer,
} OnlineServicesListStoreColumns;

extern OnlineServices *online_services;
extern OnlineService *current_service;


/********************************************************
 *          Global method  & function prototypes        *
 ********************************************************/
OnlineServices *online_services_init(void);

gboolean online_services_login(OnlineServices *services);
gboolean online_services_reconnect(OnlineServices *services);
void online_services_disconnect(OnlineServices *services);

gboolean online_services_save(OnlineServices *services, OnlineService *service, gboolean enabled, const gchar *url, const gchar *username, const gchar *password, gboolean auto_connect);

guint online_services_count_connections(OnlineServices *services);
OnlineService *online_services_get_first_connected(OnlineServices *services);
guint online_services_get_which_connected(OnlineServices *services, OnlineService *which_service);
OnlineService *online_services_get_last_connected(OnlineServices *services);

void online_services_request(OnlineServices *services, RequestMethod request, const gchar *uri, SoupSessionCallback callback, gpointer user_data, gpointer formdata);

void online_services_deinit(OnlineServices *online_services);

gboolean online_services_fill_liststore(OnlineServices *services, GtkListStore *liststore);


SoupMessage *online_service_request(OnlineService *service, RequestMethod request, const gchar *uri, SoupSessionCallback callback, gpointer user_data, gpointer formdata);
SoupMessage *online_service_request_url(OnlineService *service, RequestMethod request, const gchar *uri, SoupSessionCallback callback, gpointer user_data, gpointer formdata);
gchar *online_service_get_url_content_type(OnlineService *service, const gchar *uri, SoupMessage **msg);

void online_service_wrapper_free(OnlineServiceCBWrapper *service_wrapper);

void online_service_free(OnlineService *service);

#endif /* __ONLINE_SERVICES_H__ */

