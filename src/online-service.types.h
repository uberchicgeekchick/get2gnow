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
#ifndef	__ONLINE_SERVICE_TYPEDEF_H__
#define	__ONLINE_SERVICE_TYPEDEF_H__

#define _GNU_SOURCE
#define _THREAD_SAFE


/********************************************************************************
 *      Project, system, & library headers.  eg #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/
#include <glib.h>
#include <gtk/gtk.h>

#include "timer.h"

G_BEGIN_DECLS
/********************************************************************************
 *                        defines, macros, methods, & etc                       *
 ********************************************************************************/


/********************************************************************************
 *                        objects, structs, and enum typedefs                   *
 ********************************************************************************/
typedef enum _MicroBloggingService MicroBloggingService;


enum _MicroBloggingService{
	Unknown = 0,
	Identica = 1,
	Twitter = 2,
	StatusNet = 3,
	Unsupported = 4,
};

struct _OnlineService{
	SoupSession			*session;
	RateLimitTimer			*timer;
	
	gboolean			authenticated;
	gboolean			connected;
	gboolean			has_loaded;
	guint				logins;
	
	gboolean			enabled;
	gboolean			auto_connect;
	
	gchar				*key;
	gchar				*guid;
	
	MicroBloggingService		micro_blogging_service;
	gchar				*micro_blogging_client;
	
	gboolean			https;
	gchar				*uri;
	gchar				*server;
	gchar				*path;
	gchar				*user_name;
	gchar				*user_nick;
	gchar				*password;
	
	GSList				*best_friends;
	gint				best_friends_total;
	
	GList				*friends;
	GList				*followers;
	GList				*friends_and_followers;
	
	gchar				*status;
};


/********************************************************************************
 *       prototypes for methods, handlers, callbacks, function, & etc           *
 ********************************************************************************/


G_END_DECLS
/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
#endif /* __ONLINE_SERVICE_TYPEDEF_H__*/