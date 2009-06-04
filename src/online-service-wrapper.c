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
#include "users.h"
#include "users-glists.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define	DEBUG_DOMAINS	"OnlineServices:UI:Network:Tweets:Requests:Users:Authentication"
#include "debug.h"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
OnlineServiceWrapper *online_service_wrapper_new(OnlineService *service, gchar *request_uri, SoupSessionCallback callback, gpointer user_data, gpointer formdata){
	OnlineServiceWrapper *service_wrapper=g_new0(OnlineServiceWrapper, 1);
	
	service_wrapper->service=service;
	
	service_wrapper->requested_uri=g_strdup(request_uri);
	service_wrapper->callback=callback;
	
	if(
		user_data!=NULL
		&&
		callback!=network_cb_on_image
		&&
		callback!=user_request_main_quit
		&&
		callback!=users_glist_save
	)
		service_wrapper->user_data=g_strdup(user_data);
	else
		service_wrapper->user_data=user_data;
	
	if(
		formdata!=NULL
		&&
		callback!=network_display_timeline
	)
		service_wrapper->formdata=g_strdup(formdata);
	else
		service_wrapper->formdata=formdata;
	
	return service_wrapper;
}

void online_service_wrapper_free(OnlineServiceWrapper *service_wrapper){
	if(!service_wrapper) return;
	
	uber_free(service_wrapper->requested_uri);
	
	if(
		service_wrapper->user_data!=NULL
		&&
		service_wrapper->callback!=network_cb_on_image
		&&
		service_wrapper->callback!=user_request_main_quit
		&&
		service_wrapper->callback!=users_glist_save
	)
		uber_free(service_wrapper->user_data);
	
	if(
		service_wrapper->formdata!=NULL
		&&
		service_wrapper->callback!=network_display_timeline
	)
		uber_free(service_wrapper->formdata);
	
	service_wrapper->callback=NULL;
	service_wrapper->service=NULL;
	
	uber_free(service_wrapper);
}/*online_service_free_wrapper*/


/********************************************************
 *                       eof                            *
 ********************************************************/
