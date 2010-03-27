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
#ifndef __ONLINE_SERVICES_TYPES_H__
#define __ONLINE_SERVICES_TYPES_H__

#ifndef	_GNU_SOURCE 
#	define _GNU_SOURCE
#endif

#ifndef _THREAD_SAFE
#	define _THREAD_SAFE
#endif


/********************************************************************************
 *      Project, system, & library headers.  eg #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/
#include <glib.h>
#include <gtk/gtk.h>
#include <libsoup/soup-message.h>

G_BEGIN_DECLS
/********************************************************************************
 *                        objects, structs, and enum typedefs                   *
 ********************************************************************************/
typedef struct _OnlineServices OnlineServices;
typedef struct _OnlineService OnlineService;
typedef struct _OnlineServiceWrapper OnlineServiceWrapper;
typedef struct _OnlineServiceXmlDoc OnlineServiceXmlDoc;

typedef struct _OnlineServiceRequest OnlineServiceRequest;
typedef struct _SelectedUpdate SelectedUpdate;
typedef struct _OnlineServiceRequestPopup OnlineServiceRequestPopup;
typedef struct _OnlineServiceRequestedResource OnlineServiceRequestedResource;

typedef enum _RequestAction RequestAction;

typedef struct _User User;
typedef struct _UserStatus UserStatus;

typedef struct _StatusNetGroup StatusNetGroup;

typedef struct _RateLimitTimer RateLimitTimer;
typedef enum _UpdateType UpdateType;

typedef enum _MicroBloggingService MicroBloggingService;

typedef enum _ReloadState ReloadState;
typedef enum _RequestMethod RequestMethod;

typedef enum _UsersGListGetWhich UsersGListGetWhich;

typedef enum _OnlineServicesListStoreColumns OnlineServicesListStoreColumns;


/**********************************************************************
 *                         Macros & methods.                          *
 **********************************************************************/
#define	ONLINE_SERVICE_REQUEST_FUNC	((OnlineServiceRequestFunc)(func))
typedef void (*OnlineServiceRequestFunc) (OnlineService *service, GtkWindow *parent_window, const gchar *user_data);

#define	ONLINE_SERVICE_SOUPSESSION_CALLBACK_RETURN_PROCESSOR_FUNC	((OnlineServiceSoupSessionCallbackReturnProcessorFunc)(func))
typedef void (*OnlineServiceSoupSessionCallbackReturnProcessorFunc) (OnlineServiceWrapper *service_wrapper, SoupMessage *xml, gpointer soup_session_callback_return_gpointer);

#define	ONLINE_SERVICE_SOUP_SESSION_CALLBACK_FUNC	((OnlineServiceSoupSessionCallbackFunc)(func))
typedef void* (*OnlineServiceSoupSessionCallbackFunc) (SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper);

#define	ONLINE_SERVICE_POST_PROCESS_FUNC	((OnlineServicePostProcessFunc)(func))
typedef void (*OnlineServicePostProcessFunc) (OnlineServiceWrapper *service_wrapper, OnlineServiceXmlDoc *xml_doc, gpointer soup_session_callback_return_gpointer);

#define	ONLINE_SERVICE_CALLBACK_FUNC	((OnlineServiceCallbackFunc)(func))
typedef void* (*OnlineServiceCallbackFunc) (OnlineService *service, OnlineServiceXmlDoc *xml_doc, OnlineServiceWrapper *service_wrapper);

#define	USERS_GLIST_ONLOAD_FUNC(func)	((UsersGListOnLoadFunc)(func))
typedef void (*UsersGListOnLoadFunc) (GList *users);

#define	G_POINTER_FUNC(func)	((GPointerFunc)(func))
typedef void (*GPointerFunc) (gpointer user_data);


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
G_END_DECLS
#endif /* __ONLINE_SERVICES_TYPES_H__ */
