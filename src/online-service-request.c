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

#include "online-services.defines.h"
#include "online-services-typedefs.h"
#include "online-service-request.h"
#include "online-service.types.h"
#include "online-service.h"
#include "online-service-wrapper.h"

#include "online-services.h"

#include "www.h"

#include "gconfig.h"
#include "preferences.h"

#include "users-glists.h"

#include "gtkbuilder.h"

#include "tabs.h"
#include "main-window.h"
#include "update-viewer.h"

/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"OnlineServices:Requests:Settings:UI:GtkBuilder:GtkBuildable:Settings:Popup:OnlineServiceRequest:online-service-request.c"
#include "debug.h"

#define GtkBuilderUI "online-services-request-popup"

gdouble in_reply_to_status_id=0.0;
OnlineService *in_reply_to_service=NULL;

OnlineService *selected_service=NULL;

#define API_USER_FOLLOW		"/friendships/create/%s.xml"
#define API_USER_UNFOLLOW	"/friendships/destroy/%s.xml"

#define API_USER_BLOCK		"/blocks/create/%s.xml"
#define API_USER_UNBLOCK	"/blocks/destroy/%s.xml"

#define API_FAVE		"/favorites/create/%s.xml"
#define API_UNFAVE		"/favorites/destroy/%s.xml"



/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
typedef struct _SelectedUpdate SelectedUpdate;
typedef struct _OnlineServiceRequestPopup OnlineServiceRequestPopup;

struct _SelectedUpdate{
	OnlineService	*service;
	gdouble		id;
	gchar		*id_str;
	gdouble		user_id;
	gchar		*user_id_str;
	gchar		*user_name;
	gchar		*update;
};

enum _RequestAction{
	SelectService,
	ViewProfile,
	ViewUpdates,
	ViewUpdatesNew,
	ViewForwards,
	BestFriendAdd,
	BestFriendDrop,
	Follow,
	UnFollow,
	Block,
	UnBlock,
	Fave,
	UnFave,
	Confirmation,
};


struct _OnlineServiceRequest{
	OnlineService	*service;
	
	RequestAction	action;
	RequestMethod	method;
	
	GtkWindow	*parent;
	gpointer	extra;
	
	gchar		*user_name;
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
	GtkComboBox		*online_services_combo_box;
	GtkListStore		*online_services_list_store;
	GtkTreeModel		*online_service_model;
	
	GtkCheckButton		*check_button;
	
	GtkButton		*cancel_button;
};

static SelectedUpdate *selected_update=NULL;

static OnlineServiceRequestPopup *online_service_request_popup=NULL;
static gint online_service_request_popup_dialog_response=0;


/********************************************************************************
 *         online_service_request's methods, handlers, callbacks, & etc.        *
 ********************************************************************************/
static OnlineServiceRequest *online_service_request_new(OnlineService *service, RequestAction action, GtkWindow *parent, const gchar *user_name);
static void online_service_request_main(OnlineService *service, RequestAction action, GtkWindow *parent, const gchar *user_name);
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
void online_service_request_view_profile(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	online_service_request_main(service, ViewProfile, parent, user_name);
}/*online_service_request_view_profile(service, parent, user_name);*/

void online_service_request_view_updates_new(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	online_service_request_main(service, ViewUpdatesNew, parent, user_name);
}/*online_service_request_view_updates_new(service, parent, user_name);*/

void online_service_request_view_updates(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	online_service_request_main(service, ViewUpdates, parent, user_name);
}/*online_service_request_view_updates(service, parent, user_name);*/

void online_service_request_view_forwards(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	online_service_request_main(service, ViewForwards, parent, user_name);
}/*online_service_request_view_updates(service, parent, user_name);*/

void online_service_request_best_friend_add(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	online_service_request_main(service, BestFriendAdd, parent, user_name);
}/*online_service_request_best_friend_add(service, parent, user_name);*/

void online_service_request_best_friend_drop(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	online_service_request_main(service, BestFriendDrop, parent, user_name);
}/*online_service_request_best_friend_drop(service, parent, user_name);*/

void online_service_request_follow(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	online_service_request_main(service, Follow, parent, user_name);
}/*online_service_request_view_profile(service, parent, user_name);*/

void online_service_request_unfollow(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	online_service_request_main(service, UnFollow, parent, user_name);
}/*online_service_request_view_unfollow(service, parent, user_name);*/

void online_service_request_block(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	online_service_request_main(service, Block, parent, user_name);
}/*online_service_request_view_block(service, parent, user_name);*/

void online_service_request_unblock(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	online_service_request_main(service, UnBlock, parent, user_name);
}/*online_service_request_view_unblock(service, parent, user_name);*/

void online_service_request_fave(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	online_service_request_main(service, Fave, parent, user_name);
}/*online_service_request_view_fave(service, parent, user_name);*/

void online_service_request_unfave(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	online_service_request_main(service, UnFave, parent, user_name);
}/*online_service_request_view_unfave(service, parent, user_name);*/

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
			return _("removed a best friend");
		case Follow:
			return _("following");
		case UnFollow:
			return _("unfollowed");
		case Block:
			return _("blocked");
		case UnBlock:
			return _("unblocked user");
		case Fave:
			return _("star'd an update");
		case UnFave:
			return _("un-star'd an update");
		case SelectService:
			return _("selecting default account");
		default:
			/*We never get here, but it makes gcc happy.*/
			return _("unsupported user action");
	}//switch
}/*online_service_request_action_to_string*/

static OnlineServiceRequest *online_service_request_new( OnlineService *service, RequestAction action, GtkWindow *parent, const gchar *user_name ){
	if(action==SelectService || action==ViewProfile || action == Confirmation || G_STR_EMPTY(user_name)) return NULL;
	
	OnlineServiceRequest *request=g_new(OnlineServiceRequest, 1);
	
	request->parent=parent;
	request->service=service;
	request->extra=NULL;
	request->user_name=g_strdup(user_name);
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
			request->method=QUEUE;
			request->uri=g_strdup_printf(API_USER_PROFILE, request->user_name);
			break;
		case Follow:
			request->method=POST;
			request->uri=g_strdup_printf(API_USER_FOLLOW, request->user_name);
			break;
		case UnFollow:
			request->method=POST;
			request->uri=g_strdup_printf(API_USER_UNFOLLOW, request->user_name);
			break;
		case Block:
			request->method=POST;
			request->uri=g_strdup_printf(API_USER_BLOCK, request->user_name);
			break;
		case UnBlock:
			request->method=POST;
			request->uri=g_strdup_printf(API_USER_UNBLOCK, request->user_name);
			break;
		case Fave:
			request->method=POST;
			request->uri=g_strdup_printf(API_FAVE, request->user_name);
			break;
		case UnFave:
			request->method=POST;
			request->uri=g_strdup_printf(API_UNFAVE, request->user_name);
			break;
		case ViewProfile:
		case SelectService:
		case Confirmation:
		default:
			/*We never get here, but it makes gcc happy.*/
			g_free(request);
			return NULL;
	}//switch
	return request;
}/*online_service_request_new*/

static void online_service_request_main(OnlineService *service, RequestAction action, GtkWindow *parent, const gchar *user_name){
	if(action==SelectService || action==Confirmation) return;

	if(G_STR_EMPTY(user_name)){
		debug("Cannot %s required information is missing.", online_service_request_action_to_string(action));
		main_window_statusbar_printf("Cannot %s required information is missing.", online_service_request_action_to_string(action));
		return;
	}

	if(action==ViewProfile){
		user_profile_viewer_show(service, user_name, parent);
		return;
	}
	
	if(action==ViewUpdatesNew||action==ViewUpdates||action==ViewForwards){
		gchar *timeline=NULL;
		gchar *user_timeline=g_strdup_printf( API_TIMELINE_USER, user_name );
		if(action==ViewUpdatesNew){
			gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
			online_service_update_ids_get(service, user_timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
			if(!unread_update_id){
				debug( "Loading %s's updates, on <%s>, all updates will be loaded because this best friends timeline has never been loaded before.", user_name, service->guid );
				timeline=user_timeline;
			}else{
				debug( "Loading %s's updates, on <%s>, new updates since their last read update: %f(ID).", user_name, service->guid, newest_update_id );
				uber_free( user_timeline );
				timeline=g_strdup_printf( API_TIMELINE_USER_UNREAD, user_name, unread_update_id );
			}
		}else if(action==ViewForwards){
			uber_free(user_timeline);
			timeline=g_strdup_printf(API_FORWARDS_BY_ID, user_name);
		}else{
			debug( "Loading %s's updates, on <%s>.  Displaying all updates.", user_name, service->guid );
			timeline=user_timeline;
		}
		
		tabs_open_timeline(timeline, service);
		uber_free(timeline);
		return;
	}
	
	OnlineServiceRequest *request=NULL;
	if(!(request=online_service_request_new(service, action, parent, user_name)))
		return;
	
	debug("Processing OnlineServiceRequest to %s %s on %s", request->message, request->user_name, service->guid);
	
	switch(request->method){
		case POST:
			online_service_request(service, POST, request->uri, NULL, online_service_request_main_quit, request, NULL);
			break;
		case GET:
		case QUEUE:
		default:
			online_service_request(service, QUEUE, request->uri, NULL, online_service_request_main_quit, request, NULL);
			break;
	}
	
	update_viewer_sexy_select();
}/*online_service_request_main(service, parent, Follow|UnFollow|ViewProfile|ViewUpdates|..., user_name);*/

void *online_service_request_main_quit(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineServiceRequest *request=(OnlineServiceRequest *)online_service_wrapper_get_user_data(service_wrapper);
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	
	const gchar *service_guid=service->guid;
	gchar *error_message=NULL;
	if(!(www_xml_error_check(service, request->uri, xml, &error_message))){
		debug("**ERORR:** OnlineServiceRequest to %s %s.  OnlineService: '%s':\n\t\tServer response: %i", request->message, request->user_name, service_guid, xml->status_code);
		
		main_window_statusbar_printf("Failed to %s on %s.  Error %s (%d).", request->message, service_guid, xml->reason_phrase, xml->status_code);
		online_service_request_free(request);
		uber_free(error_message);
		return NULL;
	}
	uber_free(error_message);
	
	User *user=NULL;
	debug("OnlineServiceRequest to %s %s.  OnlineService: <%s> Loading: <%s>:", request->message, request->user_name, service_guid, request->uri);
	switch(request->action){
		case UnFollow:
		case Block:
			tabs_remove_from_timelines_sexy_tree_views_list_stores(STRING_USER, request->user_name);
		case Follow:
		case BestFriendAdd:
		case BestFriendDrop:
			if(!(user=user_parse_profile(service->session, xml, service_wrapper))){
				if(xml->status_code!=403){
					debug("\t\t[failed]");
					main_window_statusbar_printf("Failed to %s %s on %s.", request->message, request->user_name, service_guid);
				}else{
					debug("\t\t[duplicate request]");
					debug("%s %s %s on %s.", ((request->action==UnFollow) ?_("You're not following") :_("You've already") ), (request->action==UnFollow ?"" : request->message), request->user_name, service_guid);
					statusbar_printf("%s %s %s on %s.", ((request->action==UnFollow) ?_("You're not following") :_("You've already") ), (request->action==UnFollow ?"" : request->message), request->user_name, service_guid);
				}
			}else{
				debug("\t\t[succeeded]");
				if(request->action==BestFriendAdd||request->action==BestFriendDrop)
					online_service_best_friends_list_store_update_check( service_wrapper, xml, user );
				else if(request->action==Follow)
					users_glists_append_friend(service, user);
				else if(request->action==UnFollow)
					users_glists_remove_friend(service, user);
				else if(request->action==Block){
					users_glists_remove_friend(service, user);
					users_glists_remove_follower(service, user);
				}
				main_window_statusbar_printf("Successfully %s %s on %s.", request->message, request->user_name, service_guid);
				user_free(user);
			}
			
			break;
		case Fave:
		case UnFave:
		case UnBlock:
			if(xml->status_code!=403){
				debug("\t\t[succceed]");
				main_window_statusbar_printf("Successfully %s on %s.", request->message, service_guid);
			}else{
				debug("\t\t[duplicate request]");
				main_window_statusbar_printf("You've already %s on %s.", request->message, service_guid);
			}
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
	request->parent=NULL;
	request->extra=NULL;
	request->service=NULL;
	uber_object_free(&request->uri, &request->user_name, &request->message, &request, NULL);
}/*online_service_request_free*/


/********************************************************************************
 *               selected_update methods, handlers, callbacks, & etc.            *
 ********************************************************************************/
gboolean online_service_request_is_update_selected(void){
	return ( !selected_update ?FALSE :TRUE );
}/*online_service_request_is_update_selected();*/

void online_service_request_set_selected_update(OnlineService *service, const gdouble id, const gdouble user_id, const gchar *user_name, const gchar *update){
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
	selected_update->update=g_uri_unescape_string(update, NULL);
}/*online_service_request_set_selected_update*/

OnlineService *online_service_request_selected_update_get_service(void){
	return ( (selected_update && selected_update->service) ?selected_update->service :NULL );
}/*online_service_request_selected_update_get_service();*/

gdouble online_service_request_selected_update_get_id(void){
	return ( (selected_update && selected_update->id) ?selected_update->id :0.0 );
}/*online_service_request_selected_update_get_id();*/

const gchar *online_service_request_selected_update_get_user_name(void){
	return ( (selected_update && selected_update->user_name) ?selected_update->user_name :NULL );
}/*online_service_request_selected_update_get_user_name();*/

gdouble online_service_request_selected_update_get_user_id(void){
	return ( (selected_update && selected_update->user_id) ?selected_update->user_id :0.0 );
}/*online_service_request_selected_update_get_user_id();*/

gchar *online_service_request_selected_update_reply_to_strdup(gboolean forwarding){
	if(!( selected_update && selected_update->id )) return NULL;
	
	if( online_services_has_connected(1) > 0 && !gconfig_if_bool(PREFS_UPDATES_NO_PROFILE_LINK, TRUE) )
		return g_strdup_printf("%s@%s ( http://%s/%s ) %s", (forwarding ?"RT " :""), selected_update->user_name, selected_update->service->uri, selected_update->user_name, (forwarding ?selected_update->update :"" ));
	
	return g_strdup_printf("%s@%s %s", (forwarding ?"RT " :""), selected_update->user_name, (forwarding ?selected_update->update :"" ));
}/*online_service_request_selected_update_reply_to_strdup();*/

gboolean online_service_request_selected_update_reply(void){
	return online_service_request_selected_update_include_and_begin_to_send(FALSE);
}/*online_service_request_selected_update_reply();*/

gboolean online_service_request_selected_update_forward(void){
	return online_service_request_selected_update_include_and_begin_to_send(TRUE);
}/*online_service_request_selected_update_forward();*/

static gboolean online_service_request_selected_update_include_and_begin_to_send(gboolean forwarding){
	gchar *prefix=NULL;
	if(G_STR_EMPTY( (prefix=online_service_request_selected_update_reply_to_strdup(forwarding)) )){
		update_viewer_beep();
		if(prefix) uber_free(prefix);
		return FALSE;
	}
	
	if( selected_update && selected_update->id )
		update_viewer_set_in_reply_to_data(selected_update->service, selected_update->id, FALSE);
	
	gboolean prefix_added=update_viewer_sexy_prefix_string(prefix, forwarding);
	uber_free(prefix);
	return prefix_added;
}/*online_service_request_selected_update_include_and_begin_to_send*/

void online_service_request_unset_selected_update(void){
	if(!selected_update) return;
	debug("Destroying current selected_update object.");
	selected_update->service=NULL;
	
	uber_object_free(&selected_update->user_name, &selected_update->id_str, &selected_update->user_id_str, &selected_update->update, &selected_update, NULL);
}/*online_service_request_unset_selected_update*/

void online_service_request_selected_update_view_updates_new(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, ViewUpdatesNew, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_view_updates_new();*/

void online_service_request_selected_update_view_forwards(void){
	if(!(selected_update && selected_update->id)) return;
	online_service_request_main(selected_update->service, ViewForwards, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->id_str);
}/*online_service_request_selected_update_view_updates();*/

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
	if(!online_service_is_user_best_friend( selected_update->service, selected_update->user_name ))
		online_service_request_main(selected_update->service, BestFriendAdd, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_best_friend_add();*/

void online_service_request_selected_update_best_friend_drop(void){
	if(!(selected_update && selected_update->user_name)) return;
	if(online_service_is_user_best_friend( selected_update->service, selected_update->user_name ))
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

void online_service_request_selected_update_destroy_fave(void){
	if(!(selected_update && selected_update->id)) return;
	online_service_request_main(selected_update->service, UnFave, ( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() ), selected_update->id_str);
}/*online_service_request_selected_update_destroy_fave*/


/********************************************************************************
 *         online_service_request's methods, handlers, callbacks, & etc.        *
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
		case SelectService:
			return _("Which Online Service/Account do you want to use?");
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
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, label_markup);
			g_free(label_markup);
			break;
		case ViewUpdates:
		case ViewUpdatesNew:
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or user id, who's resent updates you'd like to view:");
			break;
		case ViewForwards:
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the update's id of which forwards/retwees you'd like to view:");
			break;
		case ViewProfile:
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to view:");
			break;
		case BestFriendAdd:
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you'd like to add to your best friends list:");
			break;
		case BestFriendDrop:
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you'd like to remove from your best friends list:");
			break;
		case Block:
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to block?  They'll no longer be able to read your updates, send you messages, and you'll no longer be notified when they 'mention' you, using the @ symbol:");
			break;
		case UnBlock:
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to un-block?  They'll be able to once again read your updates and you'll see when they mention you, using the @ symbol, again:");
			break;
		case UnFollow:
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to un-follow?");
			break;
		case Follow:
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to follow:");
			break;
		case Fave:
		case UnFave:
		case Confirmation:
		default:
			break;
	}
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
	
	if( online_service_request_popup->action==SelectService){
		online_service_request_popup_set_selected_service(dialog, response, online_service_request_popup);
		gtk_widget_destroy(GTK_WIDGET(dialog));
		return;
	}
	
	if( online_service_request_popup->action==Confirmation){
		online_service_request_popup_dialog_process_confirmation(dialog, response, online_service_request_popup);
		gtk_widget_destroy(GTK_WIDGET(dialog));
		return;
	}
	
	switch(  online_service_request_popup->action ){
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
			if(online_service_request_popup_dialog_process_requests(dialog, response, online_service_request_popup))
				gtk_widget_destroy(GTK_WIDGET(dialog));
			break;
		case SelectService:
		case Fave:
		case UnFave:
		case Confirmation:
		default:
			/*All to make gcc nice & happy.*/
			gtk_widget_destroy(GTK_WIDGET(dialog));
			break;
	}//switch
}/*online_service_request_popup_response_cb(online_service_request_popup->dialog, GTK_RESPONSE_OK|GTK_RESPONSE_CANCEL, online_service_request_popup);*/

static gboolean online_service_request_popup_dialog_process_requests(GtkMessageDialog *dialog, gint response, OnlineServiceRequestPopup *online_service_request_popup){
	OnlineService		*service=NULL;
	const gchar		*user_name=gtk_entry_get_text(online_service_request_popup->user_name_entry);
	
	if(G_STR_EMPTY(user_name)){
		update_viewer_beep();
		return FALSE;
	}
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	if(!(gtk_combo_box_get_active_iter(online_service_request_popup->online_services_combo_box, iter))) {
		if(iter) g_free(iter);
		return FALSE;
	}
	
	gtk_tree_model_get(
			online_service_request_popup->online_service_model, iter,
				OnlineServicePointer, &service,
			-1
	);
	
	online_service_request_main(service,  online_service_request_popup->action, GTK_WINDOW( online_service_request_popup->dialog), user_name);
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
				 online_service_request_popup->online_service_model, iter,
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
	gtk_widget_destroy( GTK_WIDGET( online_service_request_popup->dialog) );
	uber_free(online_service_request_popup);
}/*online_service_request_popup_destroy_and_free*/

gboolean online_service_request_popup_confirmation_dialog(const gchar *gconfig_key, const gchar *message1, const gchar *message2, GFunc func, gpointer user_data){
	if(G_STR_N_EMPTY(gconfig_key) && gconfig_if_bool(gconfig_key, FALSE))
		return TRUE;
	
	online_service_request_popup_dialog_show(Confirmation);
	
	gtk_message_dialog_set_markup( online_service_request_popup->dialog, message1 );
	if( G_STR_N_EMPTY( message2 ) )
		gtk_message_dialog_format_secondary_text( online_service_request_popup->dialog, "%s", message2 );
	
	if( G_STR_N_EMPTY( gconfig_key ) )
		online_service_request_popup_confirmation_dialog_add_gconfig_key( gconfig_key, func, user_data );
	
	gtk_widget_show_all( GTK_WIDGET( online_service_request_popup->dialog ) );
	
	gtk_widget_hide( GTK_WIDGET( online_service_request_popup->user_name_frame ) );
	gtk_widget_hide( GTK_WIDGET( online_service_request_popup->online_services_frame ) );
	
	gtk_dialog_run(GTK_DIALOG( online_service_request_popup->dialog));
	switch(online_service_request_popup_dialog_response){
		case GTK_RESPONSE_YES: case GTK_RESPONSE_OK: case GTK_RESPONSE_NONE: case GTK_RESPONSE_APPLY:
			return TRUE;
		default:
			return FALSE;
	}
}/*online_service_request_popup_confirm_dialog();*/

static void online_service_request_popup_confirmation_dialog_add_gconfig_key( const gchar *gconfig_key, GFunc func, gpointer user_data ){
	GtkVBox			*vbox;
	
	online_service_request_popup->check_button=(GtkCheckButton *)gtk_check_button_new_with_label(_("Do not show this again"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON( online_service_request_popup->check_button), FALSE);
	
	vbox=(GtkVBox *)gtk_vbox_new(FALSE, 6);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 6);
	
	gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET( online_service_request_popup->check_button), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG( online_service_request_popup->dialog)->vbox), GTK_WIDGET(vbox), FALSE, FALSE, 0);
	
	g_object_set_data_full(G_OBJECT(online_service_request_popup->dialog), "gconfig_key", g_strdup(gconfig_key), g_free);
	g_object_set_data(G_OBJECT(online_service_request_popup->dialog), "user_data", user_data);
	g_object_set_data(G_OBJECT(online_service_request_popup->dialog), "func", func);
}/*online_service_request_popup_confirmation_dialog_add_gconfig_key( gconfig_key, func, user_data );*/

void online_service_request_popup_select_service(void){
	if(!( online_services_has_connected(1) > 0 )){
		selected_service=online_services_connected_get_first();
		debug("There is only one connected OnlineService, auto-selecting: %s.", selected_service->guid);
		return;
	}
	
	debug("Prompting to select OnlineService to use as 'selected_service'.");
	online_service_request_popup_dialog_show(SelectService);
	
	gtk_widget_hide(GTK_WIDGET( online_service_request_popup->user_name_frame));
	gtk_dialog_run(GTK_DIALOG( online_service_request_popup->dialog));
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
			return TRUE;
		case Fave:
		case UnFave:
		default:
			break;
	}//switch
	main_window_statusbar_printf("%s is not supported by %s's popup prompt.", online_service_request_action_to_string(action), PACKAGE_NAME);
	return FALSE;
}/*online_service_request_popup_validate_usage*/


static void online_service_request_popup_dialog_show(RequestAction action){
	if(!(online_service_request_popup_validate_usage(action))) return;
	
	if(online_service_request_popup){
		if(online_service_request_popup->action==action){
			debug("Displaying existing popup instance.");
			return gtk_window_present(GTK_WINDOW( online_service_request_popup->dialog));
		}
		
		online_service_request_popup_destroy_and_free();
	}
	
	GtkWindow *parent=( gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) ?update_viewer_get_window() :main_window_get_window() );
	
	if(online_service_request_popup_dialog_response) online_service_request_popup_dialog_response=0;
	
	GtkBuilder *ui;
	
	online_service_request_popup=g_new0(OnlineServiceRequestPopup, 1);
	online_service_request_popup->action=action;
	
	/* Get widgets */
	ui=gtkbuilder_get_file(
					GtkBuilderUI,
						"entry_popup", &online_service_request_popup->dialog,
						
						"user_name_frame", &online_service_request_popup->user_name_frame,
						"user_name_entry", &online_service_request_popup->user_name_entry,
						"user_name_label", &online_service_request_popup->user_name_label,
						
						"online_services_frame", &online_service_request_popup->online_services_frame,
						"online_services_combo_box", &online_service_request_popup->online_services_combo_box,
						"online_services_list_store", &online_service_request_popup->online_services_list_store,
						
						"cancel_button", &online_service_request_popup->cancel_button,
					NULL
	);
	
	online_service_request_popup->online_service_model=gtk_combo_box_get_model(GTK_COMBO_BOX( online_service_request_popup->online_services_combo_box));

	/* Connect the signals */
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
	gtk_window_set_transient_for(GTK_WINDOW( online_service_request_popup->dialog), parent );
	gtk_widget_show_all(GTK_WIDGET( online_service_request_popup->dialog));
	if(!(online_services_has_connected(1) > 0 && online_services_has_total(1) > 0)){
		debug("There is only one service to select from so we don't really need to ask.\n\t\tSo we'll just hide 'online_services_frame'.");
		gtk_widget_hide(GTK_WIDGET(online_service_request_popup->online_services_frame));
	}
	
	gtk_window_present(GTK_WINDOW( online_service_request_popup->dialog));
}/*online_service_request_popup_dialog_show*/

static void online_service_request_user_name_active_cb(GtkEntry *user_name_entry, OnlineServiceRequestPopup *online_service_request_popup){
	online_service_request_popup_response_cb(online_service_request_popup->dialog, GTK_RESPONSE_OK, online_service_request_popup);
}/**/


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

