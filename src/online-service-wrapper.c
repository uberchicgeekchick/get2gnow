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
#define _GNU_SOURCE
#define _THREAD_SAFE



/********************************************************
 *        Project headers, eg #include "config.h"       *
 ********************************************************/
#include "config.h"
#include "program.h"

#include "online-services.rest-uris.defines.h"
#include "online-services.h"
#include "online-service.h"
#include "online-service-wrapper.h"

#include "www.h"
#include "main-window.h"

#include "online-service-request.h"
#include "users-glists.h"

#include "network.h"
#include "profile-manager.h"

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define	DEBUG_DOMAINS	"OnlineServices:UI:Network:Updates:Requests:Users:Authentication:online-service-wrapper.c"
#include "debug.h"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
typedef enum{
	DataSet,
	DataFree,
} OnlineServiceWrapperDataProcessor;

struct _OnlineServiceWrapper {
	OnlineService						*service;
	SoupMessage						*xml;
	RequestMethod						request_method;
	gchar							*requested_uri;
	OnlineServiceSoupSessionCallbackReturnProcessorFunc	online_service_soup_session_callback_return_processor_func;
	OnlineServiceSoupSessionCallbackFunc			callback;
	gpointer						user_data;
	gpointer						form_data;
	gboolean						can_run;
	gboolean						can_free;
	guint8							attempt;
	guint							process_timeout;
	guint							process_timeout_id;
};

static OnlineServiceWrapper *online_service_wrapper_new(OnlineService *service, SoupMessage *xml, RequestMethod request_method, const gchar *requested_uri, guint8 attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data);
static gboolean online_service_wrapper_queue(OnlineServiceWrapper *online_service_wrapper);
static gboolean online_service_wrapper_process(OnlineServiceWrapper *service_wrapper);
static void online_service_wrapper_run(OnlineServiceWrapper *online_service_wrapper, SoupSession *session, SoupMessage *xml);

static void online_service_wrapper_data_processor(gpointer *data, OnlineServiceWrapperDataProcessor data_processor);
static void online_service_wrapper_form_data_processor(OnlineServiceWrapper *online_service_wrapper, OnlineServiceWrapperDataProcessor data_processor);
static void online_service_wrapper_user_data_processor(OnlineServiceWrapper *online_service_wrapper, OnlineServiceWrapperDataProcessor data_processor);

static void online_service_wrapper_soup_session_callback_return_processor_func_default(OnlineServiceWrapper *online_service_wrapper, SoupMessage *xml, gpointer soup_session_callback_return_gpointer);
static void *online_service_wrapper_callback(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *online_service_wrapper);



/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
static OnlineServiceWrapper *online_service_wrapper_new(OnlineService *service, SoupMessage *xml, RequestMethod request_method, const gchar *requested_uri, guint8 attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data);
OnlineServiceWrapper *online_service_wrapper_new(OnlineService *service, SoupMessage *xml, RequestMethod request_method, const gchar *requested_uri, guint8 attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data){
	if(!(service && callback!=NULL )) return NULL;
	
	OnlineServiceWrapper *online_service_wrapper=g_new0(OnlineServiceWrapper, 1);
	
	online_service_wrapper->service=service;
	online_service_wrapper->process_timeout=0;
	online_service_wrapper->process_timeout_id=0;
	online_service_wrapper->xml=xml;
	online_service_wrapper->request_method=request_method;
	online_service_wrapper->requested_uri=g_strdup(requested_uri);
	online_service_wrapper->attempt=attempt;
	
	if(online_service_soup_session_callback_return_processor_func!=NULL)
		online_service_wrapper->online_service_soup_session_callback_return_processor_func=online_service_soup_session_callback_return_processor_func;
	else
		online_service_wrapper->online_service_soup_session_callback_return_processor_func=online_service_wrapper_soup_session_callback_return_processor_func_default;
	
	online_service_wrapper->callback=callback;
	
	online_service_wrapper->user_data=user_data;
	online_service_wrapper->form_data=form_data;
	
	online_service_wrapper->can_run=online_service_wrapper->can_free=TRUE;
	
	online_service_wrapper_user_data_processor(online_service_wrapper, DataSet);
	
	online_service_wrapper_form_data_processor(online_service_wrapper, DataSet);
	
	return online_service_wrapper;
}/*online_service_wrapper_new(service, xml, requested_uri, 0, post_process_callback, callback, user_data, form_data);*/

gboolean online_service_wrapper_init(OnlineService *service, SoupMessage *xml, RequestMethod request_method, const gchar *requested_uri, guint8 attempt, OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_soup_session_callback_return_processor_func, OnlineServiceSoupSessionCallbackFunc callback, gpointer user_data, gpointer form_data){
	if(!service) return FALSE;
	debug("Processing <%s>'s libsoup %s request for: [%s].  Creating and Queueing OnlineServiceWrapper.", service->guid, online_service_request_method_to_string(request_method), requested_uri );
	online_service_wrapper_process( online_service_wrapper_new(service, xml, request_method, requested_uri, attempt, online_service_soup_session_callback_return_processor_func, callback, user_data, form_data) );
	return service->processing;
}/*online_service_wrapper_init(service, xml, requested_uri, 0, post_process_callback, callback, user_data, form_data);*/

static gboolean online_service_wrapper_process(OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->service)) return FALSE;
	OnlineService *service=online_service_wrapper->service;
	if(!online_service_validate_session(service, online_service_wrapper->requested_uri)){
		online_service_wrapper_free(online_service_wrapper, FALSE);
		return FALSE;
	}
	
	if(service->processing)
		return online_service_wrapper_queue(online_service_wrapper);
	if( !service->authenticated && !g_strrstr(online_service_wrapper->requested_uri, API_LOGIN) )
		return online_service_wrapper_queue(online_service_wrapper);
	else if(service->authenticated && !g_strrstr(online_service_wrapper->requested_uri, API_LOGIN) && !(service->has_loaded && service->nick_name) ){
		const gchar *profile_auth_test;
		if(!( (profile_auth_test=g_strrstr(online_service_wrapper->requested_uri, service->user_name)) && g_strrstr(profile_auth_test, ".xml") ))
			if(online_service_wrapper_queue(online_service_wrapper))
				return TRUE;
	}
	
	service->processing=TRUE;
	service->processing_queue=g_list_append(service->processing_queue, online_service_wrapper);
	if(online_service_wrapper->process_timeout_id) online_service_wrapper->process_timeout_id=0;
	if(service->processing_timer > 0) service->processing_timer--;
	debug("OnlineService: <%s>'s began processing: <%s>.", online_service_wrapper->service->guid, online_service_wrapper->requested_uri);
	debug("Adding libsoup request to service: <%s> libsoup's message queue.", service->guid);
	soup_session_queue_message(online_service_wrapper->service->session, online_service_wrapper->xml, (SoupSessionCallback)online_service_wrapper_callback, online_service_wrapper);
	
	return FALSE;
}/*online_service_wrapper_process(online_service_wrapper);*/

static gboolean online_service_wrapper_queue(OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->service && online_service_validate_session(online_service_wrapper->service, online_service_wrapper->requested_uri) )){
		online_service_wrapper_free(online_service_wrapper, FALSE);
		return FALSE;
	}
	OnlineService *service=online_service_wrapper->service;
	if(!(service->enabled && service->connected && service->authenticated && online_service_validate_session(online_service_wrapper->service, online_service_wrapper->requested_uri) )){
		if(!online_service_refresh(service)){
			debug("Unable to load: %s.  You are no longer connected to %s.", online_service_wrapper->requested_uri, service->key);
			statusbar_printf("Unable to load: %s.  You are no longer connected to: %s.", online_service_wrapper->requested_uri, service->key);
			if(++online_service_wrapper->attempt > ONLINE_SERVICE_MAX_REQUESTS){
			online_service_wrapper_free(online_service_wrapper, (service?TRUE :FALSE));
				return FALSE;
			}
		}
	}
	
	if(online_service_wrapper->process_timeout_id) return TRUE;
	if(!online_service_wrapper->process_timeout) online_service_wrapper->process_timeout=++service->processing_timer;
	online_service_wrapper->process_timeout_id=g_timeout_add((online_service_wrapper->process_timeout)*100, (GSourceFunc)online_service_wrapper_process, online_service_wrapper);
	debug("OnlineService: <%s>'s re-queuing the requested uri: <%s> to be re-processed in %d00 milliseconds.", service->key, online_service_wrapper->requested_uri, online_service_wrapper->process_timeout);
	return TRUE;
}/*online_service_wrapper_queue(online_service_wrapper);*/

static void online_service_wrapper_run(OnlineServiceWrapper *online_service_wrapper, SoupSession *session, SoupMessage *xml){
	if(!( online_service_wrapper->service && online_service_wrapper->callback && online_service_wrapper->can_run)) return;
	
	online_service_wrapper->can_run=FALSE;
	online_service_wrapper->online_service_soup_session_callback_return_processor_func( online_service_wrapper, xml, (online_service_wrapper->callback(session, xml, online_service_wrapper)) );
}/*online_service_wrapper_run*/

void *online_service_wrapper_callback(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->service && online_service_validate_session(online_service_wrapper->service, online_service_wrapper->requested_uri) )){
		if(online_service_wrapper) online_service_wrapper_free(online_service_wrapper, FALSE);
		return NULL;
	}
	
	OnlineService *service=online_service_wrapper->service;
	if(!(service && service->key && G_STR_N_EMPTY(service->key) )){
		debug("Invalid OnlineService.  Cannot process request for: %s.", online_service_wrapper->requested_uri);
		return NULL;
	}
	if(!(online_service_wrapper->requested_uri && G_STR_N_EMPTY(online_service_wrapper->requested_uri) )) online_service_wrapper->requested_uri=_("Unknown URI");
	
	uber_free(service->status);
	const gchar *status=NULL;
	gchar *error_message=NULL;
	if(!www_xml_error_check(service, online_service_wrapper->requested_uri, xml, &error_message))
		status=_("[Failed]");
	else
		status=_("[Success]");
	
	if(!( online_service_wrapper && online_service_wrapper->service && online_service_validate_session(online_service_wrapper->service, online_service_wrapper->requested_uri) )){
		online_service_wrapper_free(online_service_wrapper, FALSE);
		return NULL;
	}
	
	debug("%s  www_xml_error_check returned: %s.", (service->status=g_strdup_printf("OnlineService: <%s> requested: %s.  URI: %s returned: %s(%d).", service->key, status, online_service_wrapper->requested_uri, xml->reason_phrase, xml->status_code)), error_message );
	statusbar_printf("<%s> loading %s: %s.", service->key, g_strrstr(online_service_wrapper->requested_uri, "/"), status);
	
	uber_free(error_message);
	
	if(!( online_service_wrapper && online_service_wrapper->service && online_service_validate_session(online_service_wrapper->service, online_service_wrapper->requested_uri) )){
		online_service_wrapper_free(online_service_wrapper, FALSE);
		return NULL;
	}
	online_service_wrapper_run(online_service_wrapper, session, xml);
	if(!( online_service_wrapper && online_service_wrapper->service && online_service_validate_session(online_service_wrapper->service, online_service_wrapper->requested_uri) )){
		online_service_wrapper_free(online_service_wrapper, FALSE);
		return NULL;
	}
	
	timer_main(service->timer, xml, online_service_wrapper->request_method);
	
	online_service_wrapper_free(online_service_wrapper, TRUE);
	
	return NULL;
}/*online_service_wrapper_callback(session, xml, online_service_wrapper);*/

static void online_service_wrapper_soup_session_callback_return_processor_func_default(OnlineServiceWrapper *online_service_wrapper, SoupMessage *xml, gpointer soup_session_callback_return_gpointer){
	if(!(online_service_wrapper && online_service_wrapper->service)) return;
	
	debug("|-------------------------------------------------------------------------------------|\n"
		"\t\t|     online_service_wrapper_soup_session_callback_return_processor_func_default      |\n"
		"\t\t|_____________________________________________________________________________________|"
	);
}/*online_service_wrapper_soup_session_callback_return_processor_func_default(soup_session_callback_return_gpointer);*/

static void online_service_wrapper_user_data_processor(OnlineServiceWrapper *online_service_wrapper, OnlineServiceWrapperDataProcessor data_processor){
	if(!online_service_wrapper->service) return;
	
	if(!(
		online_service_wrapper->user_data!=NULL
		&&
		online_service_wrapper->callback!=online_service_wrapper_callback
		&&
		online_service_wrapper->callback!=online_service_request_main_quit
		&&
		online_service_wrapper->callback!=network_cb_on_image
		&&
		online_service_wrapper->callback!=users_glist_process
		&&
		online_service_wrapper->callback!=network_display_timeline
		&&
		online_service_wrapper->online_service_soup_session_callback_return_processor_func!=(OnlineServiceSoupSessionCallbackReturnProcessorFunc)profile_manager_edit_profile
	))
		return;
	
	online_service_wrapper_data_processor(&online_service_wrapper->user_data, data_processor);
	
}/*online_service_wrapper_user_data_processor(online_service_wrapper, DataSet|DataFree);*/

static void online_service_wrapper_form_data_processor(OnlineServiceWrapper *online_service_wrapper, OnlineServiceWrapperDataProcessor data_processor){
	if(!online_service_wrapper->service) return;
	
	if(!(
		online_service_wrapper->form_data!=NULL
		&&
		online_service_wrapper->callback!=network_display_timeline
		&&
		online_service_wrapper->callback!=profile_manager_save_image
		&&
		online_service_wrapper->callback!=users_glist_process
	))
		return;
	
	online_service_wrapper_data_processor(&online_service_wrapper->form_data, data_processor);
}/*online_service_wrapper_form_data_processor(online_service_wrapper, DataSet|DataFree);*/

static void online_service_wrapper_data_processor(gpointer *data, OnlineServiceWrapperDataProcessor data_processor){
	if(!(*data)) return;
	switch(data_processor){
		case DataFree:
			uber_free(*data);
			break;
		case DataSet:
			*data=g_strdup(*data);
			break;
		default:
			debug("**ERROR:** OnlineServiceWrapperDataProcessor reached unsupported data_processor.");
			break;
	}
}/*online_service_wrapper_data_processor(&data, DataSet|DataFree);*/

OnlineService *online_service_wrapper_get_online_service(OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->service )) return NULL;
	return online_service_wrapper->service;
}/*online_service_wrapper_get_online_service(online_service_wrapper);*/

RequestMethod online_service_wrapper_get_request_method(OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->service )) return QUEUE;
	return online_service_wrapper->request_method;
}/*RequestMethod online_service_wrapper_get_requested_method(online_service_wrapper);*/

const gchar *online_service_wrapper_get_requested_uri(OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->service )) return NULL;
	return online_service_wrapper->requested_uri;
}/*online_service_wrapper_get_requested_uri(online_service_wrapper);*/

guint8 online_service_wrapper_increment_attempt(OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->service )) return -1;
	
	if(!(++online_service_wrapper->attempt < ONLINE_SERVICE_MAX_REQUESTS)) return 0;
	
	return online_service_wrapper->attempt;
}/*online_service_wrapper_increment_reattempt(online_service_wrapper);*/

guint8 online_service_wrapper_reattempt(OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->service )) return -1;
	
	if(online_service_wrapper->can_free) online_service_wrapper->can_free=FALSE;
	if(!online_service_wrapper->can_run) online_service_wrapper->can_run=TRUE;
	
	if(online_service_wrapper->attempt < ONLINE_SERVICE_MAX_REQUESTS){
		network_set_state_loading_timeline(online_service_wrapper->requested_uri, Reattempt);
		online_service_request_uri(online_service_wrapper->service, online_service_wrapper->request_method, online_service_wrapper->requested_uri, ++online_service_wrapper->attempt, online_service_wrapper->online_service_soup_session_callback_return_processor_func, online_service_wrapper->callback, online_service_wrapper->user_data, online_service_wrapper->form_data );
	}
	
	return online_service_wrapper->attempt;
}/*online_service_wrapper_reattempt(online_service_wrapper);*/

guint8 online_service_wrapper_get_attempt(OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->service )) return -1;
	return online_service_wrapper->attempt;
}/*online_service_wrapper_get_attempt(online_service_wrapper);*/

OnlineServiceSoupSessionCallbackReturnProcessorFunc online_service_wrapper_get_online_service_soup_session_callback_return_processor_func(OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->can_run && online_service_wrapper->service)) return NULL;
	return online_service_wrapper->online_service_soup_session_callback_return_processor_func;
}/*online_service_wrapper_get_online_service_soup_session_callback_return_processor_func(online_service_wrapper);*/


OnlineServiceSoupSessionCallbackFunc online_service_wrapper_get_callback(OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->can_run && online_service_wrapper->service)) return NULL;
	return online_service_wrapper->callback;
}/*online_service_wrapper_get_callback(online_service_wrapper);*/

gpointer online_service_wrapper_get_user_data(OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->service)) return NULL;
	return online_service_wrapper->user_data;
}/*online_service_wrapper_get_user_data(online_service_wrapper)*/

gpointer online_service_wrapper_get_form_data(OnlineServiceWrapper *online_service_wrapper){
	if(!(online_service_wrapper && online_service_wrapper->service)) return NULL;
	return online_service_wrapper->form_data;
}/*online_service_wrapper_get_form_data(online_service_wrapper);*/

void online_service_wrapper_free(OnlineServiceWrapper *online_service_wrapper, gboolean free_queue){
	if(!(online_service_wrapper && online_service_wrapper->service)) return;
	OnlineService *service=online_service_wrapper->service;
	debug("OnlineService: <%s> has finished processing: <%s>.", service->guid, online_service_wrapper->requested_uri);
	if(service->processing) service->processing=FALSE;
	
	if(online_service_wrapper->process_timeout_id)
		program_timeout_remove(&online_service_wrapper->process_timeout_id, "OnlineServiceWrapper timeout");
	online_service_wrapper_user_data_processor(online_service_wrapper, DataFree);
	online_service_wrapper_form_data_processor(online_service_wrapper, DataFree);
	
	online_service_wrapper->online_service_soup_session_callback_return_processor_func=NULL;
	online_service_wrapper->callback=NULL;
	if(free_queue)
		service->processing_queue=g_list_remove(service->processing_queue, online_service_wrapper);
	online_service_wrapper->service=NULL;
	
	uber_object_free(&online_service_wrapper->requested_uri, &online_service_wrapper, NULL);
}/*online_service_wrapper_free(service_wrapper);*/


/********************************************************
 *                       eof                            *
 ********************************************************/
