/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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

/********************************************************
 *          My art, code, & programming.                *
 ********************************************************/


/********************************************************
 *        Project headers, eg #include "config.h"       *
 ********************************************************/
#include "config.h"
#include "main.h"

#include "online-services.h"
#include "online-service.h"
#include "online-service-wrapper.h"

#include "network.h"
#include "online-service-request.h"
#include "users-glists.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define	DEBUG_DOMAINS	"OnlineServices:UI:Network:Tweets:Requests:Users:Authentication"
#include "debug.h"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
typedef enum{
	DataSet,
	DataFree,
} OnlineServiceWrapperDataHandler;

struct _OnlineServiceWrapper {
	OnlineService				*service;
	gchar					*requested_uri;
	OnlineServiceCallbackAfterSoup		online_service_callback_after_soup;
	OnlineServiceSoupSessionCallback	callback;
	gpointer				user_data;
	gpointer				form_data;
};


static void online_service_wrapper_data_handler(gpointer *data, OnlineServiceWrapperDataHandler data_handler);
static void online_service_wrapper_form_data_handler(OnlineServiceWrapper *service_wrapper, OnlineServiceWrapperDataHandler data_handler);
static void online_service_wrapper_user_data_handler(OnlineServiceWrapper *service_wrapper, OnlineServiceWrapperDataHandler data_handler);



/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
OnlineServiceWrapper *online_service_wrapper_new(OnlineService *service, const gchar *request_uri, OnlineServiceCallbackAfterSoup online_service_callback_after_soup, OnlineServiceSoupSessionCallback callback, gpointer user_data, gpointer form_data){
	OnlineServiceWrapper *service_wrapper=g_new0(OnlineServiceWrapper, 1);
	
	service_wrapper->service=service;
	
	service_wrapper->requested_uri=g_strdup(request_uri);
	
	if(online_service_callback_after_soup!=NULL)
		service_wrapper->online_service_callback_after_soup=online_service_callback_after_soup;
	else
		service_wrapper->online_service_callback_after_soup=online_service_callback_after_soup_default;
	
	service_wrapper->callback=callback;
	
	service_wrapper->user_data=user_data;
	service_wrapper->form_data=form_data;
	
	if(callback==NULL) return service_wrapper;
	
	online_service_wrapper_user_data_handler(service_wrapper, DataSet);
	
	online_service_wrapper_form_data_handler(service_wrapper, DataSet);
	
	return service_wrapper;
}/*online_service_wrapper_new(service, request_uri, callback, user_data, form_data);*/

static void online_service_wrapper_user_data_handler(OnlineServiceWrapper *service_wrapper, OnlineServiceWrapperDataHandler data_handler){
	if(!(
		service_wrapper->user_data!=NULL
		&&
		service_wrapper->callback!=NULL
		&&
		service_wrapper->callback!=online_service_callback
		&&
		service_wrapper->callback!=online_service_request_main_quit
		&&
		service_wrapper->callback!=network_cb_on_image
		&&
		service_wrapper->callback!=users_glist_process
		&&
		service_wrapper->callback!=network_display_timeline
	))
		return;
	
	online_service_wrapper_data_handler(&service_wrapper->user_data, data_handler);
	
}/*online_service_wrapper_handler_user_data_handler(service_wrapper, DataSet|DataFree);*/

static void online_service_wrapper_form_data_handler(OnlineServiceWrapper *service_wrapper, OnlineServiceWrapperDataHandler data_handler){
	if(!(
		service_wrapper->callback!=NULL
		&&
		service_wrapper->form_data!=NULL
		&&
		service_wrapper->callback!=network_display_timeline
	))
		return;
	
	online_service_wrapper_data_handler(&service_wrapper->form_data, data_handler);
}/*online_service_wrapper_form_data_handler(service_wrapper, DataSet|DataFree);*/


static void online_service_wrapper_data_handler(gpointer *data, OnlineServiceWrapperDataHandler data_handler){
	if(!(*data)) return;
	switch(data_handler){
		case DataFree:
			uber_free( (*data) );
			break;
		case DataSet:
			*data=g_strdup(*data);
			break;
		default:
			debug("**ERROR:** OnlineServiceWrapperDataHandler reached unsupported user_data_method.");
			break;
	}
}/*online_service_wrapper_handler_user_data(&data, DataSet|DataFree);*/


void online_service_wrapper_run(OnlineServiceWrapper *service_wrapper, SoupSession *session, SoupMessage *xml){
	if(service_wrapper->callback==NULL) return;
	
	service_wrapper->online_service_callback_after_soup( service_wrapper->callback(session, xml, service_wrapper) );
}/*online_service_wrapper_run*/

const gchar *online_service_wrapper_get_requested_uri(OnlineServiceWrapper *service_wrapper){
	if(!service_wrapper) return NULL;
	return service_wrapper->requested_uri;
}/*online_service_wrapper_get_requested_uri(service_wrapper);*/

OnlineService *online_service_wrapper_get_online_service(OnlineServiceWrapper *service_wrapper){
	if(!service_wrapper) return NULL;
	return service_wrapper->service;
}/*online_service_wrapper_get_online_service(service_wrapper);*/

gpointer online_service_wrapper_get_user_data(OnlineServiceWrapper *service_wrapper){
	if(!service_wrapper) return NULL;
	return service_wrapper->user_data;
}/*online_service_wrapper_get_user_data(service_wrapper)*/

gpointer online_service_wrapper_get_form_data(OnlineServiceWrapper *service_wrapper){
	if(!service_wrapper) return NULL;
	return service_wrapper->form_data;
}/*online_service_wrapper_get_form_data(service_wrapper);*/

void online_service_wrapper_free(OnlineServiceWrapper *service_wrapper){
	if(!service_wrapper) return;
	
	online_service_wrapper_user_data_handler(service_wrapper, DataFree);
	
	online_service_wrapper_form_data_handler(service_wrapper, DataFree);
	
	service_wrapper->online_service_callback_after_soup=NULL;
	service_wrapper->callback=NULL;
	service_wrapper->service=NULL;
	
	uber_object_free(&service_wrapper->requested_uri, &service_wrapper, NULL);
}/*online_service_free_wrapper*/


/********************************************************
 *                       eof                            *
 ********************************************************/
