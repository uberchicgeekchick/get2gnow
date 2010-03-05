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
/********************************************************************************
 *                      My art, code, & programming.                            *
 ********************************************************************************/
#define _GNU_SOURCE
#define _THREAD_SAFE


/********************************************************************************
 *      Project, system & library headers, eg #include <gdk/gdkkeysyms.h>       *
 ********************************************************************************/
#include <time.h>
#include <strings.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <libsoup/soup.h>

#include "gconfig.h"
#include "program.h"

#include "update-ids.h"
#include "online-services.rest-uris.defines.h"
#include "online-services.typedefs.h"
#include "online-service-request.h"
#include "online-services.types.h"
#include "online-service.types.h"
#include "online-service.h"
#include "online-service-wrapper.h"

#include "online-services.h"

#include "www.h"

#include "gconfig.h"
#include "preferences.defines.h"

#include "users-glists.h"

#include "gtkbuilder.h"

#include "tabs.h"
#include "main-window.h"
#include "update-viewer.h"
#include "user-profile-viewer.h"
#include "best-friends.h"

/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"OnlineServices:Requests:Settings:UI:GtkBuilder:GtkBuildable:Settings:Popup:OnlineServiceRequest:online-service-request.c"
#include "debug.h"

#define GTK_BUILDER_UI_FILENAME "online-services-request-popup"

gdouble in_reply_to_status_id=0.0;
OnlineService *in_reply_to_service=NULL;

OnlineService *selected_service=NULL;



/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
typedef struct _SelectedUpdate SelectedUpdate;
typedef struct _OnlineServiceRequestPopup OnlineServiceRequestPopup;

struct _SelectedUpdate{
	OnlineService	*service;
	UpdateType	type;
	gdouble		id;
	gchar		*id_str;
	gdouble		user_id;
	gchar		*user_id_str;
	gchar		*user_name;
	gchar		*update;
};

struct _OnlineServiceRequest{
	OnlineService	*service;
	
	RequestAction	action;
	RequestMethod	method;
	
	GtkWindow	*parent_window;
	gpointer	extra;
	
	gchar		*get_rest_xml;
	gchar		*message;
	gchar		*uri;
};

struct  _OnlineServiceRequestPopup{
	RequestAction		action;
	
	GtkMessageDialog	*dialog;

	GtkFrame		*user_name_frame;
	GtkLabel		*user_name_label;
	GtkEntry		*user_name_entry;
	
	GtkFrame		*online_services_frame;
	GtkLabel		*online_services_label;
	GtkComboBox		*online_services_combo_box;
	GtkListStore		*online_services_list_store;
	
	GtkCheckButton		*check_button;
};

static SelectedUpdate *selected_update=NULL;

static OnlineServiceRequestPopup *online_service_request_popup=NULL;
static gint online_service_request_popup_dialog_response=0;


/********************************************************************************
 *         online_service_request's methods, handlers, callbacks, & etc.        *
 ********************************************************************************/
static OnlineServiceRequest *online_service_request_new(OnlineService *service, RequestAction action, GtkWindow *parent_window, const gchar *get_rest_xml);
static void online_service_request_main(OnlineService *service, RequestAction action, GtkWindow *parent_window, const gchar *get_rest_xml);
static void online_service_request_open_timeline_tab(OnlineService *service, RequestAction action, const gchar *get_rest_xml);
static gboolean online_service_request_set_post_method_data(OnlineServiceRequest **request);
static void online_service_request_free(OnlineServiceRequest *request);


static gboolean online_service_request_selected_update_include_and_begin_to_send(gboolean forwarding);


static gchar *online_service_request_action_to_title(RequestAction action);
static void online_service_request_popup_set_title_and_label(RequestAction action, OnlineServiceRequestPopup *online_service_request_popup);
static void online_service_request_popup_dialog_show(RequestAction action);
static void online_service_request_popup_dialog_process_confirmation(GtkMessageDialog *dialog, gint response, OnlineServiceRequestPopup *online_service_request_popup);
static void online_service_request_popup_set_selected_service(GtkMessageDialog *dialog, gint response, OnlineServiceRequestPopup *online_service_request_popup);
static gboolean online_service_request_popup_dialog_process_requests(GtkMessageDialog *dialog, gint response, OnlineServiceRequestPopup *online_service_request_popup);
static void online_service_request_popup_response_cb(GtkMessageDialog *dialog, gint response, OnlineServiceRequestPopup *online_service_request_popup);
static void online_service_request_popup_destroy_cb(GtkMessageDialog *dialog, OnlineServiceRequestPopup *online_service_request_popup);
static void online_service_request_popup_confirmation_dialog_add_gconfig_key( const gchar *gconfig_key, GFunc func, gpointer user_data );
static gboolean online_service_request_popup_validate_usage(RequestAction action);
static void online_service_request_popup_destroy_and_free(void);
static void online_service_request_user_name_active_cb(GtkEntry *user_name_entry, OnlineServiceRequestPopup *online_service_request_popup);



/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
const gchar *online_service_request_method_to_string(RequestMethod request_method){
	switch(request_method){
		case QUEUE: return _("QUEUE");
		case POST: return _("POST");
		case GET: return _("GET");
	}
	return _("**ERROR:** Unknown RequestMethod ");
}/*online_service_request_method_to_string(request_method);*/

const gchar *online_service_request_action_to_string(RequestAction action){
	switch(action){
		case Confirmation:
			return _("confirming action");
		case ViewUpdates:
			return _("displaying recent updates");
		case ViewUpdatesNew:
			return _("displaying new updates");
		case ViewForwards:
			return _("displaying forwards");
		case ViewProfile:
			return _("viewing user profile");
		case BestFriendAdd:
			return _("added a best friend");
		case BestFriendDrop:
			return _("removing a best friend");
		case Follow:
			return _("following");
		case UnFollow:
			return _("unfollowing");
		case Block:
			return _("blocking");
		case UnBlock:
			return _("unblocking user");
		case Fave:
			return _("saving an update to favorites");
		case UnFave:
			return _("removing an update from favorites");
		case DeleteStep1:
		case DeleteStep2:
			return _("delete one of your updates");
		case SelectService:
			return _("selecting default account");
		case ShortenURI:
			return _("shortening a URI");
		default:
			/*We never get here, but it makes gcc happy.*/
			return _("unsupported user action");
	}//switch
}/*online_service_request_action_to_string*/

static OnlineServiceRequest *online_service_request_new(OnlineService *service, RequestAction action, GtkWindow *parent_window, const gchar *get_rest_xml){
	OnlineServiceRequest *request=g_new(OnlineServiceRequest, 1);
	
	request->parent_window=parent_window;
	request->service=service;
	request->extra=NULL;
	request->get_rest_xml=g_strdup(get_rest_xml);
	request->action=action;
	request->method=QUEUE;
	request->message=g_strdup(online_service_request_action_to_string(action));
	
	switch(request->action){
		case ViewForwards:
		case ViewUpdatesNew:
		case ViewUpdates:
			break;
		case BestFriendAdd:
		case BestFriendDrop:
			request->uri=g_strdup_printf(API_USER_PROFILE, request->get_rest_xml);
			break;
		case Follow:
		case UnFollow:
		case Block:
		case UnBlock:
		case Fave:
		case UnFave:
		case DeleteStep1:
		case DeleteStep2:
		case ShortenURI:
			if(online_service_request_set_post_method_data(&request))
				break;
		case ViewProfile:
		case SelectService:
		case Confirmation:
		default:
			/*We never get here, but it makes gcc happy.*/
			request->uri=g_strdup("");
			online_service_request_free(request);
			return NULL;
	}//switch
	return request;
}/*online_service_request_new(service, action, parent_window, get_rest_xml);*/

static gboolean online_service_request_set_post_method_data(OnlineServiceRequest **request){
	(*request)->method=POST;
	switch((*request)->action){
		case Follow:
			(*request)->uri=g_strdup_printf(API_USER_FOLLOW, (*request)->get_rest_xml);
			break;
		case UnFollow:
			(*request)->uri=g_strdup_printf(API_USER_UNFOLLOW, (*request)->get_rest_xml);
			break;
		case Block:
			(*request)->uri=g_strdup_printf(API_USER_BLOCK, (*request)->get_rest_xml);
			break;
		case UnBlock:
			(*request)->uri=g_strdup_printf(API_USER_UNBLOCK, (*request)->get_rest_xml);
			break;
		case Fave:
			(*request)->uri=g_strdup_printf(API_FAVE, (*request)->get_rest_xml);
			break;
		case UnFave:
			(*request)->uri=g_strdup_printf(API_UNFAVE, (*request)->get_rest_xml);
			break;
		case DeleteStep1:
			(*request)->uri=g_strdup_printf(API_DESTROY_STEP1, (*request)->get_rest_xml);
			break;
		case DeleteStep2:
			(*request)->uri=g_strdup_printf(API_DESTROY_STEP2, (*request)->get_rest_xml);
			break;
		case ShortenURI:
			if(!g_strrstr( (*request)->get_rest_xml, "://" )){
				main_window_statusbar_printf("%s is an invalid URI.", (*request)->get_rest_xml);
				return FALSE;
			}
			(*request)->uri=g_strdup(URI_SHORTENER_URI);
			gchar *get_rest_xml_escaped=g_uri_escape_string((*request)->get_rest_xml, NULL, TRUE);
			gchar *get_rest_xml_swap=g_strdup_printf("longurl=%s", get_rest_xml_escaped);
			uber_free( (*request)->get_rest_xml );
			uber_free(get_rest_xml_escaped);
			(*request)->get_rest_xml=get_rest_xml_swap;
			break;
		case ViewProfile:
		case SelectService:
		case Confirmation:
		case ViewForwards:
		case ViewUpdatesNew:
		case ViewUpdates:
		case BestFriendAdd:
		case BestFriendDrop:
		default:
			debug("**ERROR:** RequestMethod does not support a HTTP 1.1 POST request");
			return FALSE;
	}
	return TRUE;
}/*online_service_request_set_post_method_data(&request);*/

static void online_service_request_main(OnlineService *service, RequestAction action, GtkWindow *parent_window, const gchar *get_rest_xml){
	if(action==SelectService || action==Confirmation) return;

	if(G_STR_EMPTY(get_rest_xml)){
		debug("Cannot %s required information is missing.", online_service_request_action_to_string(action));
		main_window_statusbar_printf("Cannot %s required information is missing.", online_service_request_action_to_string(action));
		return;
	}

	if(action==ViewProfile){
		user_profile_viewer_show(service, get_rest_xml, parent_window);
		return;
	}
	
	if(action==ViewUpdatesNew||action==ViewUpdates||action==ViewForwards)
		return online_service_request_open_timeline_tab(service, action, get_rest_xml);
	
	if(action==SelectService || action==ViewProfile || action == Confirmation || G_STR_EMPTY(get_rest_xml))
		return;
	
	OnlineServiceRequest *request=NULL;
	if(!(request=online_service_request_new(service, action, parent_window, get_rest_xml)))
		return;
	
	debug("Processing OnlineServiceRequest to %s %s on %s", request->message, request->get_rest_xml, service->guid);
	
	if(request->action!=ShortenURI)
		online_service_request(service, request->method, request->uri, NULL, online_service_request_main_quit, request, NULL);
	else{
		gtk_widget_set_sensitive(GTK_WIDGET(update_viewer_sexy_entry_get_widget()), FALSE);
		online_service_request_uri(service, request->method, request->uri, 0, NULL, online_service_request_main_quit, request, request->get_rest_xml);
	}
	
	update_viewer_sexy_select();
}/*online_service_request_main(service, parent_window, Follow|UnFollow|ViewProfile|ViewUpdates|..., user_name|update_id);*/

static void online_service_request_open_timeline_tab(OnlineService *service, RequestAction action, const gchar *get_rest_xml){
	gchar *timeline=NULL;
	gchar *user_timeline=g_strdup_printf( API_TIMELINE_USER, get_rest_xml );
	if(action==ViewUpdatesNew){
		gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
		update_ids_get(service, user_timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
		if(!unread_update_id){
			debug( "Loading %s's updates, on <%s>, all updates will be loaded because this best friends timeline has never been loaded before.", get_rest_xml, service->guid );
			timeline=user_timeline;
		}else{
			debug( "Loading %s's updates, on <%s>, new updates since their last read update: %f(ID).", get_rest_xml, service->guid, newest_update_id );
			uber_free(user_timeline);
			timeline=g_strdup_printf(API_TIMELINE_USER_UNREAD, get_rest_xml, unread_update_id);
		}
	}else if(action==ViewForwards){
		uber_free(user_timeline);
		timeline=g_strdup_printf(API_FORWARDS_BY_ID, get_rest_xml);
	}else{
		debug( "Loading %s's updates, on <%s>.  Displaying all updates.", get_rest_xml, service->guid );
		timeline=user_timeline;
	}
	
	tabs_open_timeline(timeline, service);
	uber_free(timeline);
}/*online_service_request_open_timeline_tab(service, action, get_rest_xml);*/

void *online_service_request_main_quit(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	const glong uri_shortener_uri_strlen=g_utf8_strlen(URI_SHORTENER_URI, -1);
	OnlineServiceRequest *request=(OnlineServiceRequest *)online_service_wrapper_get_user_data(service_wrapper);
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	
	gchar *error_message=NULL;
	if(!(www_xml_error_check(service, request->uri, xml, &error_message))){
		debug("OnlineServiceRequest failed to %s %s.  OnlineService: '%s':\n\t\tServer response: %i", request->message, request->get_rest_xml, service->guid, xml->status_code);
		
		main_window_statusbar_printf("Failed to %s on %s.  Error %s (%d).", request->message, service->guid, xml->reason_phrase, xml->status_code);
		online_service_request_free(request);
		uber_free(error_message);
		return NULL;
	}
	uber_free(error_message);
	
	switch(request->action){
		case UnFollow:
		case Block:
		case Follow:
		case UnBlock:
		case BestFriendAdd:
		case BestFriendDrop:
			debug("Validating OnlineServiceRequest of OnlineService: <%s>", service->guid);
			debug("Loading: <%s> while %s <%s>", request->uri, request->message, request->get_rest_xml);
			User *user=NULL;
			if(!(user=user_parse_profile(service->session, xml, service_wrapper))){
				if(xml->status_code!=403){
					debug("\t\t\t\t[failed]");
					main_window_statusbar_printf("Failed to %s %s on %s.", request->message, request->get_rest_xml, service->guid);
					break;
				}
				debug("\t\t\t\t[duplicate request]");
				debug("%s %s %s on %s.", ((request->action==UnFollow) ?_("You're not following") :_("You've already") ), (request->action==UnFollow ?"" : request->message), request->get_rest_xml, service->guid);
				main_window_statusbar_printf("%s %s %s on %s.", ((request->action==UnFollow) ?_("You're not following") :_("You've already") ), (request->action==UnFollow ?"" : request->message), request->get_rest_xml, service->guid);
				break;
			}
			debug("\t\t\t\t[succeeded]");
			if(request->action==BestFriendAdd||request->action==BestFriendDrop)
				best_friends_tree_store_update_check(service_wrapper, xml, user);
			else if(request->action==Follow)
				users_glists_append_friend(service, user);
			else if(request->action==UnFollow||request->action==Block){
				if(best_friends_is_user_best_friend(user->service, user->user_name))
					best_friends_drop(user->service, request->parent_window, user->user_name);
				tabs_remove_from_uberchick_tree_views_tree_stores(STRING_USER, request->get_rest_xml);
				gchar *user_timeline=g_strdup_printf(API_TIMELINE_USER, request->get_rest_xml);
				tabs_close_timeline(user_timeline);
				uber_free(user_timeline);
			}
			
			main_window_statusbar_printf("<%s>'s successeded in %s <%s@%s>.", service->guid, request->message, request->get_rest_xml, service->uri);
			user_free(user);
			break;
		case DeleteStep1:
		case Fave:
			debug("Validating OnlineServiceRequest of OnlineService: <%s>", service->guid);
			debug("Loading: <%s> while %s <%s>", request->uri, request->message, request->get_rest_xml);
			UserStatus *status=NULL;
			if(!(status=user_status_parse_new(service, xml, request->uri))){
				if(xml->status_code!=403){
					debug("\t\t\t\t[failed]");
					main_window_statusbar_printf("Failed to %s %s on %s.", request->message, request->get_rest_xml, service->guid);
					break;
				}
				debug("\t\t\t\t[duplicate request]");
				debug("%s %s %s on %s.", ((request->action==UnFollow) ?_("You're not following") :_("You've already") ), (request->action==UnFollow ?"" : request->message), request->get_rest_xml, service->guid);
				main_window_statusbar_printf("%s %s %s on %s.", ((request->action==UnFollow) ?_("You're not following") :_("You've already") ), (request->action==UnFollow ?"" : request->message), request->get_rest_xml, service->guid);
				break;
			}
			if(request->action==DeleteStep1){
				if(!online_services_is_user_name_mine(request->service, status->user->user_name))
					debug("Unable to delete update %s; it does not belong to you.", request->get_rest_xml);
				else
					online_service_request_main(request->service, DeleteStep2, request->parent_window, status->id_str);
			}else
				debug("\t\t\t\t[succeeded]");
			user_status_free(status);
			break;
		case DeleteStep2:
		case UnFave:
			debug("Validating OnlineServiceRequest of OnlineService: <%s>", service->guid);
			debug("Loading: <%s> while %s <%s>", request->uri, request->message, request->get_rest_xml);
			if(xml->status_code!=403){
				if(request->action!=Fave){
					tabs_remove_from_uberchick_tree_views_tree_stores(GSTRING_UPDATE_ID_STR, request->get_rest_xml);
					update_viewer_new_update();
				}
				debug("\t\t\t\t[succeeded]");
				main_window_statusbar_printf("Successfully %s on %s.", request->message, service->guid);
			}else{
				debug("\t\t\t\t[duplicate request]");
				main_window_statusbar_printf("You've already %s on %s.", request->message, service->guid);
			}
			break;
		case ShortenURI:
			debug("Looking for shortened URI.");
			gchar *shortened_uri=NULL;
			if(!( (shortened_uri=www_get_uri_dom_xpath_element_content(xml, "html->body->p->a")) && g_utf8_strlen(shortened_uri, -1) > uri_shortener_uri_strlen && g_str_has_prefix(shortened_uri, URI_SHORTENER_URI) && G_STR_N_EMPTY(g_strrstr(shortened_uri, URI_SHORTENER_URI)) )){
				debug("**ERROR:** %s failed to create a shortened URI.  %s returned %s(%d).", URI_SHORTENER_URI, URI_SHORTENER_URI, xml->reason_phrase, xml->status_code);
				main_window_statusbar_printf("%s failed to create a shortened URI.  %s returned %s(%d).", URI_SHORTENER_URI, URI_SHORTENER_URI, xml->reason_phrase, xml->status_code);
			}else{
				update_viewer_sexy_insert_string(shortened_uri, FALSE, FALSE);
				uber_free(shortened_uri);
			}
			gtk_widget_set_sensitive(GTK_WIDGET(update_viewer_sexy_entry_get_widget()), TRUE);
			break;
		case SelectService:
		case ViewProfile:
		case Confirmation:
		case ViewUpdates:
		case ViewUpdatesNew:
		case ViewForwards:
		default:
			/*more cases of to make gcc happy.*/
			debug("Unsupported OnlineServiceRequest action! %s", online_service_request_action_to_string(request->action) );
			break;
	}//switch
	
	online_service_request_free(request);
	return NULL;
}/*online_service_request_main_quit*/

static void online_service_request_free(OnlineServiceRequest *request){
	request->parent_window=NULL;
	request->extra=NULL;
	request->service=NULL;
	uber_object_free(&request->uri, &request->get_rest_xml, &request->message, &request, NULL);
}/*online_service_request_free*/


/********************************************************************************
 *               SelectedUpdate methods, handlers, callbacks, & etc.            *
 ********************************************************************************/
gboolean online_service_request_isset_selected_update(void){
	return ( (!(selected_update && selected_update->id)) ?FALSE :TRUE );
}/*online_service_request_isset_selected_update();*/

void online_service_request_set_selected_update(OnlineService *service, UpdateType type, const gdouble id, const gdouble user_id, const gchar *user_name, const gchar *update){
	/*	gint id=atoi(string);	*/
	if(selected_update) online_service_request_unset_selected_update();
	
	debug("SelectedUpdate created from '%s', update ID: #%f from: '%s' on <%s>.", service->guid, id, user_name, service->uri);
	debug("SelectedUpdate's update: %s.", update);
	selected_update=g_new0(SelectedUpdate, 1);
	selected_update->service=service;
	selected_update->id=id;
	selected_update->id_str=gdouble_to_str(id);
	selected_update->user_id=user_id;
	selected_update->user_id_str=gdouble_to_str(user_id);
	selected_update->user_name=g_strdup(user_name);
	selected_update->type=type;
	selected_update->update=g_strdup(update);
}/*online_service_request_set_selected_update*/

OnlineService *online_service_request_selected_update_get_service(void){
	return ( (selected_update && selected_update->service) ?selected_update->service :NULL );
}/*online_service_request_selected_update_get_service();*/

UpdateType online_service_request_selected_update_get_update_type(void){
	return ( (selected_update && selected_update->type) ?selected_update->type :None);
}/*online_service_request_selected_update_get_update_type();*/

gdouble online_service_request_selected_update_get_id(void){
	return ( (selected_update && selected_update->id) ?selected_update->id :0.0 );
}/*online_service_request_selected_update_get_id();*/

const gchar *online_service_request_selected_update_get_id_str(void){
	return ( (selected_update && selected_update->id_str) ?selected_update->id_str :NULL );
}/*online_service_request_selected_update_get_id_str();*/

const gchar *online_service_request_selected_update_get_user_name(void){
	return ( (selected_update && selected_update->user_name) ?selected_update->user_name :NULL );
}/*online_service_request_selected_update_get_user_name();*/

gdouble online_service_request_selected_update_get_user_id(void){
	return ( (selected_update && selected_update->user_id) ?selected_update->user_id :0.0 );
}/*online_service_request_selected_update_get_user_id();*/

gboolean online_service_request_selected_update_reply(void){
	return online_service_request_selected_update_include_and_begin_to_send(FALSE);
}/*online_service_request_selected_update_reply();*/

gboolean online_service_request_selected_update_forward(void){
	return online_service_request_selected_update_include_and_begin_to_send(TRUE);
}/*online_service_request_selected_update_forward();*/

static gboolean online_service_request_selected_update_include_and_begin_to_send(gboolean forwarding){
	if(!( selected_update && selected_update->id )){
		update_viewer_beep();
		return FALSE;
	}
	if(forwarding && !update_viewer_sexy_entry_clear()) return FALSE;
	
	return update_viewer_set_in_reply_to_data(selected_update->service, selected_update->type, selected_update->user_name, selected_update->update, selected_update->user_id, selected_update->id, !forwarding, forwarding, TRUE);
}/*online_service_request_selected_update_include_and_begin_to_send*/

void online_service_request_unset_selected_update(void){
	if(!selected_update) return;
	debug("Destroying current selected_update object.");
	selected_update->service=NULL;
	
	uber_object_free(&selected_update->user_name, &selected_update->id_str, &selected_update->user_id_str, &selected_update->update, &selected_update, NULL);
}/*online_service_request_unset_selected_update*/


/********************************************************************************
 *         OnlineServiceRequestPopup's methods, handlers, callbacks, & etc.        *
 ********************************************************************************/
static gchar *online_service_request_action_to_title(RequestAction action){
	switch(action){
		case Confirmation:
			return _("Confirmation Dialog");
		case ViewUpdatesNew:
			return _("Who's newest updates do you want see?");
		case ViewUpdates:
			return _("Who's updates do you want see?");
		case ViewForwards:
			return _("Which update's forwards would you like to view?");
		case ViewProfile:
			return _("Who's user profile would you like to view?");
		case BestFriendAdd:
			return _("Who would you like to add to your best friends list?");
		case BestFriendDrop:
			return _("Who do you want to remove from your best friends list?");
		case Follow:
			return _("Who do you want to follow?");
		case UnFollow:
			return _("Who do you want to unfollow?");
		case Block:
			return _("Who do you want to block?");
		case UnBlock:
			return _("Who do you want to unblocked?");
		case Fave:
			return _("What update ID # do you want to star?");
		case UnFave:
			return _("What update ID # do you want to un-star?");
		case DeleteStep1:
			return _("What update's ID do you to delete?");
		case ShortenURI:
			return _("What URL would you like to shorten?");
		case SelectService:
			return _("Which Online Service/Account do you want to use?");
		case DeleteStep2:
			return NULL;
		default:
			/*We never get here, but it makes gcc happy.*/
			return _("**ERROR:** Unsupported?  How on any planet did you get here?");
	}//switch
}/*online_service_request_action_to_title(action);*/

static void online_service_request_popup_set_title_and_label(RequestAction action, OnlineServiceRequestPopup *online_service_request_popup){
	gtk_window_set_title(GTK_WINDOW(online_service_request_popup->dialog), online_service_request_action_to_title(action) );
	
	gchar *label_markup=NULL;
	switch( action ){
		case SelectService:
			label_markup=g_markup_printf_escaped("Please select the 'default' account you want to use for sending direct messages, managing friends, and etc.\n\n<span weight=\"bold\">NOTE: You're being asked this before %s loads your friends, followers, or both.  This may take a while, so after selecting your account, please be patient while you're friends, followers, or both are download.  They will be displeyed, it may just take awhile.</span>\n\nYou can select a different account at any time by selecting 'Select Default Account' from the 'Accounts' file menu:", GETTEXT_PACKAGE);
			gtk_message_dialog_set_markup(online_service_request_popup->dialog, label_markup);
			g_free(label_markup);
			break;
		case ViewUpdates:
		case ViewUpdatesNew:
			gtk_message_dialog_set_markup(online_service_request_popup->dialog, "Please enter the user user_name, or user id, who's resent updates you'd like to view:");
			break;
		case ViewForwards:
			gtk_message_dialog_set_markup(online_service_request_popup->dialog, "Please enter the update's id of which forwards/retwees you'd like to view:");
			break;
		case ViewProfile:
			gtk_message_dialog_set_markup(online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to view:");
			break;
		case BestFriendAdd:
			gtk_message_dialog_set_markup(online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you'd like to add to your best friends list:");
			break;
		case BestFriendDrop:
			gtk_message_dialog_set_markup(online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you'd like to remove from your best friends list:");
			break;
		case Block:
			gtk_message_dialog_set_markup(online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to block?  They'll no longer be able to read your updates, send you messages, and you'll no longer be notified when they 'mention' you, using the @ symbol:");
			break;
		case UnBlock:
			gtk_message_dialog_set_markup(online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to un-block?  They'll be able to once again read your updates and you'll see when they mention you, using the @ symbol, again:");
			break;
		case UnFollow:
			gtk_message_dialog_set_markup(online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to un-follow?");
			break;
		case Follow:
			gtk_message_dialog_set_markup(online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to follow:");
			break;
		case ShortenURI:
			label_markup=g_strdup_printf("Please enter the URI, e.g.: web address, that you want to shorten into a smaller URI, e.g. %sex-yEs:", URI_SHORTENER_URI);
			gtk_message_dialog_set_markup(online_service_request_popup->dialog, label_markup);
			uber_free(label_markup);
			gtk_label_set_markup_with_mnemonic(online_service_request_popup->user_name_label, "<b>_URI, e.g. web address:</b>");
			return;
		case Fave:
		case UnFave:
		case DeleteStep1:
		case DeleteStep2:
			label_markup=g_strdup_printf("Please enter the update ID of the update you want to %s:", online_service_request_action_to_title(action));
			gtk_message_dialog_set_markup(online_service_request_popup->dialog, label_markup);
			uber_free(label_markup);
			gtk_label_set_markup_with_mnemonic(online_service_request_popup->user_name_label, "<b>_Update ID:</b>");
			return;
		case Confirmation:
		default:
			return;
	}
	gtk_label_set_markup_with_mnemonic(online_service_request_popup->user_name_label, "<b>_Username:</b>");
}/*online_service_request_popup_set_title*/


static void online_service_request_popup_response_cb(GtkMessageDialog *dialog, gint response, OnlineServiceRequestPopup *online_service_request_popup){
	online_service_request_popup_dialog_response=response;
	debug("Request-Popup-Dialog for %s recieved response: %s(=%i)", online_service_request_action_to_string( online_service_request_popup->action), program_gtk_response_to_string(response), response);
	switch(response){
		case GTK_RESPONSE_YES:
		case GTK_RESPONSE_OK:
		case GTK_RESPONSE_NONE:
		case GTK_RESPONSE_APPLY:
			break;
		case GTK_RESPONSE_REJECT:
		case GTK_RESPONSE_DELETE_EVENT:
		case GTK_RESPONSE_CANCEL:
		case GTK_RESPONSE_CLOSE:
		case GTK_RESPONSE_NO:
		default:
			gtk_widget_destroy(GTK_WIDGET(dialog));
			return;
	}
	
	if(online_service_request_popup->action==SelectService){
		online_service_request_popup_set_selected_service(dialog, response, online_service_request_popup);
		gtk_widget_destroy(GTK_WIDGET(dialog));
		return;
	}
	
	if(online_service_request_popup->action==Confirmation){
		online_service_request_popup_dialog_process_confirmation(dialog, response, online_service_request_popup);
		gtk_widget_destroy(GTK_WIDGET(dialog));
		return;
	}
	
	switch(online_service_request_popup->action){
		case ViewProfile:
		case Follow:
		case UnFollow:
		case Block:
		case UnBlock:
		case ViewUpdates:
		case ViewUpdatesNew:
		case ViewForwards:
		case BestFriendAdd:
		case BestFriendDrop:
		case Fave:
		case UnFave:
		case DeleteStep1:
		case DeleteStep2:
		case ShortenURI:
			if(online_service_request_popup_dialog_process_requests(dialog, response, online_service_request_popup))
				gtk_widget_destroy(GTK_WIDGET(dialog));
			break;
		case SelectService:
		case Confirmation:
		default:
			/*All to make gcc nice & happy.*/
			gtk_widget_destroy(GTK_WIDGET(dialog));
			break;
	}//switch
}/*online_service_request_popup_response_cb(online_service_request_popup->dialog, GTK_RESPONSE_OK|GTK_RESPONSE_CANCEL, online_service_request_popup);*/

static gboolean online_service_request_popup_dialog_process_requests(GtkMessageDialog *dialog, gint response, OnlineServiceRequestPopup *online_service_request_popup){
	OnlineService		*service=NULL;
	const gchar		*get_rest_xml=gtk_entry_get_text(online_service_request_popup->user_name_entry);
	
	if(!( GTK_WIDGET_IS_SENSITIVE(online_service_request_popup->user_name_entry) && GTK_WIDGET_IS_SENSITIVE(online_service_request_popup->online_services_combo_box) ))
		return TRUE;
	
	if(G_STR_EMPTY(get_rest_xml)){
		update_viewer_beep();
		return FALSE;
	}
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	if(!(gtk_combo_box_get_active_iter(online_service_request_popup->online_services_combo_box, iter))) {
		if(iter) g_free(iter);
		return FALSE;
	}
	
	gtk_tree_model_get(
			GTK_TREE_MODEL(online_service_request_popup->online_services_list_store), iter,
				OnlineServicePointer, &service,
			-1
	);
	
	online_service_request_main(service, online_service_request_popup->action, GTK_WINDOW(online_service_request_popup->dialog), get_rest_xml);
	return TRUE;
}/*online_service_request_popup_dialog_process_requests(widget, response, popup);*/

static void online_service_request_popup_dialog_process_confirmation(GtkMessageDialog *dialog, gint response, OnlineServiceRequestPopup *online_service_request_popup){
	GFunc		func=g_object_get_data(G_OBJECT(dialog), "func");
	gpointer	user_data=g_object_get_data(G_OBJECT(dialog), "user_data");
	const gchar	*gconfig_key=g_object_get_data(G_OBJECT(dialog), "gconfig_key");
	
	gconfig_set_bool(gconfig_key, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON( online_service_request_popup->check_button)));
	
	if(func)
		func( (gpointer)gconfig_key, user_data);
}/*online_service_request_popup_dialog_process_confirmation(widget, response, popup);*/


static void online_service_request_popup_set_selected_service(GtkMessageDialog *dialog, gint response, OnlineServiceRequestPopup *online_service_request_popup){
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	
	if(gtk_combo_box_get_active_iter( online_service_request_popup->online_services_combo_box, iter)){
		gtk_tree_model_get(
				 GTK_TREE_MODEL(online_service_request_popup->online_services_list_store), iter,
					OnlineServicePointer, &selected_service,
				-1
		);
	}
	uber_free(iter);
}/*online_service_request_popup_set_selected_service*/

static void online_service_request_popup_destroy_cb(GtkMessageDialog *dialog, OnlineServiceRequestPopup *online_service_request_popup){
	online_service_request_popup_destroy_and_free();
}/*online_service_request_popup_destroy_cb*/

static void online_service_request_popup_destroy_and_free(void){
	gtk_widget_destroy(GTK_WIDGET(online_service_request_popup->dialog));
	uber_free(online_service_request_popup);
}/*online_service_request_popup_destroy_and_free*/

gboolean online_service_request_popup_confirmation_dialog(const gchar *gconfig_key, const gchar *message1, const gchar *message2, GFunc func, gpointer user_data){
	if(G_STR_N_EMPTY(gconfig_key) && gconfig_if_bool(gconfig_key, FALSE))
		return TRUE;
	
	online_service_request_popup_dialog_show(Confirmation);
	
	gtk_label_set_markup( online_service_request_popup->user_name_label, message1 );
	if(!G_STR_EMPTY(message2))
		gtk_label_set_markup( online_service_request_popup->online_services_label, message2 );
	
	if(!G_STR_EMPTY(gconfig_key))
		online_service_request_popup_confirmation_dialog_add_gconfig_key( gconfig_key, func, user_data );
	
	gtk_widget_show_all( GTK_WIDGET( online_service_request_popup->dialog ) );
	
	gtk_widget_set_sensitive(GTK_WIDGET(online_service_request_popup->user_name_entry), FALSE);
	gtk_widget_hide( GTK_WIDGET(online_service_request_popup->user_name_entry) );
	
	gtk_widget_set_sensitive(GTK_WIDGET(online_service_request_popup->online_services_combo_box), FALSE);
	gtk_widget_hide( GTK_WIDGET(online_service_request_popup->online_services_combo_box));
	
	gtk_dialog_run(GTK_DIALOG( online_service_request_popup->dialog));
	switch(online_service_request_popup_dialog_response){
		case GTK_RESPONSE_YES: case GTK_RESPONSE_OK: case GTK_RESPONSE_NONE: case GTK_RESPONSE_APPLY:
			return TRUE;
		default:
			return FALSE;
	}
}/*online_service_request_popup_confirm_dialog();*/

static void online_service_request_popup_confirmation_dialog_add_gconfig_key( const gchar *gconfig_key, GFunc func, gpointer user_data ){
	g_object_set_data_full(G_OBJECT(online_service_request_popup->dialog), "gconfig_key", g_strdup(gconfig_key), g_free);
	g_object_set_data(G_OBJECT(online_service_request_popup->dialog), "user_data", user_data);
	g_object_set_data(G_OBJECT(online_service_request_popup->dialog), "func", func);
}/*online_service_request_popup_confirmation_dialog_add_gconfig_key( gconfig_key, func, user_data );*/

void online_service_request_popup_select_service(void){
	if(!( online_services_has_connected(0) > 1 )){
		selected_service=online_services_connected_get_first();
		debug("There is only one connected OnlineService, auto-selecting: %s.", selected_service->guid);
		return;
	}
	
	debug("Prompting to select OnlineService to use as 'selected_service'.");
	online_service_request_popup_dialog_show(SelectService);
	
	gtk_widget_hide(GTK_WIDGET(online_service_request_popup->user_name_frame));
	gtk_dialog_run(GTK_DIALOG(online_service_request_popup->dialog));
}/*online_service_request_popup_select_service();*/

void online_service_request_popup_follow(void){
	online_service_request_popup_dialog_show(Follow);
}/*online_service_request_popup_follow();*/

void online_service_request_popup_unfollow(void){
	online_service_request_popup_dialog_show(UnFollow);
}/*online_service_request_popup_unfollow();*/

void online_service_request_popup_block(void){
	online_service_request_popup_dialog_show(Block);
}/*online_service_request_popup_block();*/

void online_service_request_popup_fave(void){
	online_service_request_popup_dialog_show(Fave);
}/*online_service_request_popup_fave();*/

void online_service_request_popup_unfave(void){
	online_service_request_popup_dialog_show(UnFave);
}/*online_service_request_popup_unfave();*/

void online_service_request_popup_delete(void){
	online_service_request_popup_dialog_show(DeleteStep1);
}/*online_service_request_popup_delete();*/

void online_service_request_popup_unblock(void){
	online_service_request_popup_dialog_show(UnBlock);
}/*online_service_request_popup_unblock();*/

void online_service_request_popup_updates_new(void){
	online_service_request_popup_dialog_show(ViewUpdatesNew);
}/*online_service_request_popup_updates();*/

void online_service_request_popup_forwards(void){
	online_service_request_popup_dialog_show(ViewForwards);
}/*online_service_request_popup_updates();*/

void online_service_request_popup_updates(void){
	online_service_request_popup_dialog_show(ViewUpdates);
}/*online_service_request_popup_updates();*/

void online_service_request_popup_profile(void){
	online_service_request_popup_dialog_show(ViewProfile);
}/*online_service_request_popup_profile();*/

void online_service_request_popup_best_friend_add(void){
	online_service_request_popup_dialog_show(BestFriendAdd);
}/*online_service_request_popup_best_friend_add();*/

void online_service_request_popup_best_friend_drop(void){
	online_service_request_popup_dialog_show(BestFriendDrop);
}/*online_service_request_popup_best_friend_drop();*/

void online_service_request_popup_shorten_uri(void){
	online_service_request_popup_dialog_show(ShortenURI);
}/*online_service_request_popup_shorten_uri();*/


static gboolean online_service_request_popup_validate_usage(RequestAction action){
	switch( action ){
		case Confirmation:
		case SelectService:
		case ViewProfile:
		case ViewUpdates:
		case ViewUpdatesNew:
		case ViewForwards:
		case BestFriendAdd:
		case BestFriendDrop:
		case Follow:
		case UnFollow:
		case Block:
		case UnBlock:
		case Fave:
		case UnFave:
		case DeleteStep1:
		case DeleteStep2:
		case ShortenURI:
			return TRUE;
		default:
			break;
	}//switch
	debug("**ERROR:** %s is not supported by %s's popup prompt.", online_service_request_action_to_string(action), PACKAGE_NAME);
	main_window_statusbar_printf("%s is not supported by %s's popup prompt.", online_service_request_action_to_string(action), PACKAGE_NAME);
	return FALSE;
}/*online_service_request_popup_validate_usage*/


static void online_service_request_popup_dialog_show(RequestAction action){
	if(!(online_service_request_popup_validate_usage(action))) return;
	
	if(online_service_request_popup){
		if(online_service_request_popup->action==action){
			debug("Displaying existing popup instance.");
			return gtk_window_present(GTK_WINDOW(online_service_request_popup->dialog));
		}
		
		online_service_request_popup_destroy_and_free();
	}
	
	GtkWindow *parent_window=( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() );
	
	if(online_service_request_popup_dialog_response) online_service_request_popup_dialog_response=0;
	
	GtkBuilder *ui;
	
	online_service_request_popup=g_new0(OnlineServiceRequestPopup, 1);
	online_service_request_popup->action=action;
	
	/* Get widgets */
	ui=gtkbuilder_get_file(
					GTK_BUILDER_UI_FILENAME,
						"entry_popup", &online_service_request_popup->dialog,
						
						"user_name_frame", &online_service_request_popup->user_name_frame,
						"user_name_label", &online_service_request_popup->user_name_label,
						"user_name_entry", &online_service_request_popup->user_name_entry,
						
						"online_services_frame", &online_service_request_popup->online_services_frame,
						"online_services_label", &online_service_request_popup->online_services_label,
						"online_services_combo_box", &online_service_request_popup->online_services_combo_box,
						"online_services_list_store", &online_service_request_popup->online_services_list_store,
						
						"online_service_request_popup_confirmation_disable_check_button", &online_service_request_popup->check_button,
					NULL
	);
	
	/* Connect the signals */
	if(action!=Confirmation)
		gtkbuilder_connect_after(
				ui, online_service_request_popup,
					"user_name_entry", "activate", online_service_request_user_name_active_cb,
				NULL
		);
	
	gtkbuilder_connect(
			ui, online_service_request_popup,
				"entry_popup", "destroy", online_service_request_popup_destroy_cb,
				"entry_popup", "response", online_service_request_popup_response_cb,
			NULL
	);
	
	g_object_unref(ui);
	
	debug("Signal handlers set... loading accounts.");
	if(!( online_services_combo_box_fill( online_service_request_popup->online_services_combo_box,  online_service_request_popup->online_services_list_store, TRUE) ))
		debug("No services found to load, new accounts need to be setup.");
	else
		debug("OnlineServices found & loaded.  Selecting active service.");
	
	online_service_request_popup_set_title_and_label(action, online_service_request_popup);
	
	g_object_add_weak_pointer(G_OBJECT( online_service_request_popup->dialog), (gpointer)&online_service_request_popup);
	gtk_window_set_transient_for(GTK_WINDOW( online_service_request_popup->dialog), parent_window );
	gtk_widget_show_all(GTK_WIDGET( online_service_request_popup->dialog));
	
	if(action!=Confirmation){
		gtk_widget_hide( GTK_WIDGET(online_service_request_popup->check_button) );
		gtk_widget_set_sensitive(GTK_WIDGET(online_service_request_popup->check_button), FALSE);
	}else{
		gtk_widget_show( GTK_WIDGET(online_service_request_popup->check_button) );
		gtk_widget_set_sensitive(GTK_WIDGET(online_service_request_popup->check_button), TRUE);
	}
	
	if(!(online_services_has_connected(0) > 1 && online_services_has_total(0) > 1 )){
		debug("There is only one service to select from so we don't really need to ask.\n\t\tSo we'll just hide 'online_services_frame'.");
		gtk_widget_hide(GTK_WIDGET(online_service_request_popup->online_services_frame));
	}
	
	gtk_window_present(GTK_WINDOW( online_service_request_popup->dialog));
}/*online_service_request_popup_dialog_show*/

static void online_service_request_user_name_active_cb(GtkEntry *user_name_entry, OnlineServiceRequestPopup *online_service_request_popup){
	if(!( G_STR_N_EMPTY(online_service_request_popup->user_name_entry->text) ))
		update_viewer_beep();
	else
		online_service_request_popup_response_cb(online_service_request_popup->dialog, GTK_RESPONSE_OK, online_service_request_popup);
}/*online_service_request_user_name_active_cb(online_service_request_popup->user_name_entry, online_service_request_popup);*/


/*************************************************************************************************************
 *  specialized and automated methods, handlers, callbacks, & etc for OnlineServiceRequest various actions.  *
 *************************************************************************************************************/
void online_service_request_view_profile(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, ViewProfile, parent_window, user_name);
}/*online_service_request_view_profile(service, parent_window, user_name);*/

void online_service_request_view_updates_new(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, ViewUpdatesNew, parent_window, user_name);
}/*online_service_request_view_updates_new(service, parent_window, user_name);*/

void online_service_request_view_updates(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, ViewUpdates, parent_window, user_name);
}/*online_service_request_view_updates(service, parent_window, user_name);*/

void online_service_request_view_forwards(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, ViewForwards, parent_window, user_name);
}/*online_service_request_view_updates(service, parent_window, user_name);*/

void online_service_request_best_friend_add(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, BestFriendAdd, parent_window, user_name);
}/*online_service_request_best_friend_add(service, parent_window, user_name);*/

void online_service_request_best_friend_drop(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, BestFriendDrop, parent_window, user_name);
}/*online_service_request_best_friend_drop(service, parent_window, user_name);*/

void online_service_request_follow(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, Follow, parent_window, user_name);
}/*online_service_request_view_profile(service, parent_window, user_name);*/

void online_service_request_unfollow(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, UnFollow, parent_window, user_name);
}/*online_service_request_view_unfollow(service, parent_window, user_name);*/

void online_service_request_block(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, Block, parent_window, user_name);
}/*online_service_request_view_block(service, parent_window, user_name);*/

void online_service_request_unblock(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, UnBlock, parent_window, user_name);
}/*online_service_request_view_unblock(service, parent_window, user_name);*/

void online_service_request_fave(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, Fave, parent_window, user_name);
}/*online_service_request_view_fave(service, parent_window, user_name);*/

void online_service_request_unfave(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, UnFave, parent_window, user_name);
}/*online_service_request_view_unfave(service, parent_window, user_name);*/

void online_service_request_delete(OnlineService *service, GtkWindow *parent_window, const gchar *user_name){
	online_service_request_main(service, DeleteStep1, parent_window, user_name);
}/*online_service_request_view_delete(service, parent_window, user_name);*/

void online_service_request_selected_update_view_updates_new(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, ViewUpdatesNew, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_view_updates_new();*/

void online_service_request_selected_update_view_forwards(void){
	if(!(selected_update && selected_update->id)) return;
	online_service_request_main(selected_update->service, ViewForwards, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->id_str);
}/*online_service_request_selected_update_view_forwards();*/

void online_service_request_selected_update_view_updates(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, ViewUpdates, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_view_updates();*/

void online_service_request_selected_update_view_profile(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, ViewProfile, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_view_profile*/

void online_service_request_selected_update_best_friend_add(void){
	if(!(selected_update && selected_update->user_name)) return;
	if(!best_friends_is_user_best_friend( selected_update->service, selected_update->user_name ))
		online_service_request_main(selected_update->service, BestFriendAdd, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_best_friend_add();*/

void online_service_request_selected_update_best_friend_drop(void){
	if(!(selected_update && selected_update->user_name)) return;
	if(best_friends_is_user_best_friend( selected_update->service, selected_update->user_name ))
		online_service_request_main(selected_update->service, BestFriendDrop, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_best_friend_drop();*/

void online_service_request_selected_update_follow(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, Follow, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_follow*/

void online_service_request_selected_update_unfollow(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, UnFollow, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_unfollow*/

void online_service_request_selected_update_block(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, Block, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_block*/

void online_service_request_selected_update_unblock(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, UnBlock, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_unblock*/

void online_service_request_selected_update_save_fave(void){
	if(!(selected_update && selected_update->id)) return;
	online_service_request_main(selected_update->service, Fave, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->id_str);
}/*online_service_request_selected_update_save_fave*/

void online_service_request_selected_update_unfave(void){
	if(!(selected_update && selected_update->id)) return;
	online_service_request_main(selected_update->service, UnFave, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->id_str);
}/*online_service_request_selected_update_unfave();*/

void online_service_request_selected_update_delete(void){
	if(!(selected_update && selected_update->id && selected_update->user_name)) return;
	if(!online_services_is_user_name_mine(selected_update->service, selected_update->user_name)) return;
	online_service_request_main(selected_update->service, DeleteStep2, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->id_str);
}/*online_service_request_selected_update_delete();*/


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

