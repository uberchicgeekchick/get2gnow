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
typedef struct _OnlineService OnlineService;

typedef enum{
	Laconica,
	Twitter,
} SupportedREST;

typedef enum{
	POST,
	GET,
	QUEUE,
} RequestMethod;

struct _OnlineService{
	SoupSession	*session;
	RateLimitTimer	*timer;
	
	gboolean	authenticated;
	gboolean	connected;
	guint		logins;
	
	guint		id_last_tweet;
	guint		id_last_dm;
	guint		id_last_reply;
	
	gboolean	enabled;
	gboolean	auto_connect;
	
	gchar		*key;
	gchar		*decoded_key;
	SupportedREST	which_rest;
	
	gboolean	https;
	gchar		*uri;
	gchar		*server;
	gchar		*path;
	gchar		*username;
	gchar		*password;
	
	GList		*friends;
	GList		*followers;
	GList		*friends_and_followers;
};

/* Twitter*/
#define SOURCE_TWITTER		"greettweetknow"

/* Laconica*/
/*#define	SOURCE_LACONICA		"<a href=\"http://uberchicgeekchick.com/?projects=" PACKAGE_TARNAME "\">" PACKAGE_TARNAME "</a>"*/
#define		SOURCE_LACONICA		"get2gnow"

extern OnlineService *selected_service;

extern unsigned long int in_reply_to_status_id;
extern OnlineService *in_reply_to_service;


/********************************************************
 *          Global method  & function prototypes        *
 ********************************************************/
OnlineService *online_service_open(const gchar *account_key);
OnlineService *online_service_new(gboolean enabled, const gchar *uri, gboolean https, const gchar *username, const gchar *password, gboolean auto_connect);
gboolean online_service_save(OnlineService *service);
gboolean online_service_delete(OnlineService *service, gboolean service_cache_rm_rf);

gboolean online_service_connect(OnlineService *service);
gboolean online_service_login(OnlineService *service, gboolean temporary_connection);
gboolean online_service_reconnect(OnlineService *service);
void online_service_disconnect(OnlineService *service, gboolean no_state_change);

void online_service_load_tweet_ids(OnlineService *service, const gchar *uri);
void online_service_save_tweet_ids(OnlineService *service, const gchar *uri);

SoupMessage *online_service_request(OnlineService *service, RequestMethod request, const gchar *uri, SoupSessionCallback callback, gpointer user_data, gpointer formdata);
SoupMessage *online_service_request_uri(OnlineService *service, RequestMethod request, const gchar *uri, SoupSessionCallback callback, gpointer user_data, gpointer formdata);

gchar *online_service_get_uri_content_type(OnlineService *service, const gchar *uri, SoupMessage **msg);

void online_service_free(OnlineService *service);

#endif /* __ONLINE_SERVICE_H__ */

