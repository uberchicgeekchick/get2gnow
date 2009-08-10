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
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "gconfig.h"
#include "program.h"

#include "online-services-typedefs.h"
#include "online-service-request.h"
#include "online-service.h"
#include "online-service-wrapper.h"
#include "network.h"

#include "gconfig.h"
#include "preferences.h"

#include "users-glists.h"

#include "gtkbuilder.h"

#include "main-window.h"
#include "tweet-view.h"

/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"OnlineServices:Requests:Settings:UI:GtkBuilder:GtkBuildable:Settings:Popup:OnlineServiceReoqest.c"
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
typedef struct _SelectedTweet SelectedTweet;
typedef struct _OnlineServiceRequestPopup OnlineServiceRequestPopup;

struct _SelectedTweet{
	OnlineService	*service;
	gdouble		id;
	gdouble		user_id;
	gchar		*user_name;
	gchar		*tweet;
};

enum _RequestAction{
	SelectService,
	ViewProfile,
	ViewTweets,
	Follow,
	UnFollow,
	Block,
	UnBlock,
	Fave,
	UnFave,
	Confirmation,
};


struct _OnlineServiceRequest{
	RequestAction action;
	RequestMethod method;
	GtkWindow *parent;
	gchar *user_data;
	gchar *message;
	gchar *uri;
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

static SelectedTweet *selected_update=NULL;

static OnlineServiceRequestPopup *online_service_request_popup=NULL;
static gint online_service_request_popup_dialog_response=0;


/********************************************************************************
 *         online_service_request's methods, handlers, callbacks, & etc.        *
 ********************************************************************************/
static OnlineServiceRequest *online_service_request_new(RequestAction action, GtkWindow *parent, const gchar *user_data);
static void online_service_request_main(OnlineService *service, RequestAction action, GtkWindow *parent, const gchar *user_data);
static void online_service_request_free(OnlineServiceRequest *request);


static void selected_update_include_and_begin_to_send(gchar *tweet, gboolean in_response, gboolean release);


static void online_service_request_popup_set_title_and_label(RequestAction action, OnlineServiceRequestPopup *online_service_request_popup);
static void online_service_request_popup_dialog_show(RequestAction action);
static void online_service_request_popup_response_cb(GtkWidget *widget, gint response, OnlineServiceRequestPopup *online_service_request_popup);
static void online_service_request_popup_dialog_process_confirmation(GtkWidget *widget, gint response, OnlineServiceRequestPopup *online_service_request_popup);
static void online_service_request_popup_set_selected_service(GtkWidget *widget, gint response, OnlineServiceRequestPopup *online_service_request_popup);
static gboolean online_service_request_popup_dialog_process_requests(GtkWidget *widget, gint response, OnlineServiceRequestPopup *online_service_request_popup);
static void online_service_request_popup_destroy_cb(GtkWidget *widget, OnlineServiceRequestPopup *online_service_request_popup);
static gboolean online_service_request_popup_validate_usage(RequestAction action);
static void online_service_request_popup_destroy_and_free(void);



/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
void online_service_request_view_profile(OnlineService *service, GtkWindow *parent, const gchar *user_data){
	online_service_request_main(service, ViewProfile, parent, user_data);
}/*online_service_request_view_profile(service, parent, user_data);*/

void online_service_request_view_tweets(OnlineService *service, GtkWindow *parent, const gchar *user_data){
	online_service_request_main(service, ViewTweets, parent, user_data);
}/*online_service_request_view_profile(service, parent, user_data);*/

void online_service_request_follow(OnlineService *service, GtkWindow *parent, const gchar *user_data){
	online_service_request_main(service, Follow, parent, user_data);
}/*online_service_request_view_profile(service, parent, user_data);*/

void online_service_request_unfollow(OnlineService *service, GtkWindow *parent, const gchar *user_data){
	online_service_request_main(service, UnFollow, parent, user_data);
}/*online_service_request_view_profile(service, parent, user_data);*/

void online_service_request_block(OnlineService *service, GtkWindow *parent, const gchar *user_data){
	online_service_request_main(service, Block, parent, user_data);
}/*online_service_request_view_profile(service, parent, user_data);*/

void online_service_request_unblock(OnlineService *service, GtkWindow *parent, const gchar *user_data){
	online_service_request_main(service, UnBlock, parent, user_data);
}/*online_service_request_view_profile(service, parent, user_data);*/

void online_service_request_fave(OnlineService *service, GtkWindow *parent, const gchar *user_data){
	online_service_request_main(service, Fave, parent, user_data);
}/*online_service_request_view_profile(service, parent, user_data);*/

void online_service_request_unfave(OnlineService *service, GtkWindow *parent, const gchar *user_data){
	online_service_request_main(service, UnFave, parent, user_data);
}/*online_service_request_view_profile(service, parent, user_data);*/

gchar *online_service_request_action_to_string(RequestAction action){
	switch(action){
		case Confirmation: _("confirming action");
		case ViewTweets:
			return _("displaying tweets");
		case Follow:
			return _("following");
		case UnFollow:
			return _("unfollowed");
		case Block:
			return _("blocked");
		case UnBlock:
			return _("unblocked user");
		case Fave:
			return _("star'ing tweet");
		case UnFave:
			return _("un-star'ing tweet");
		case ViewProfile:
			return _("viewing user profile");
		case SelectService:
			return _("selecting default account");
		default:
			/*We never get here, but it makes gcc happy.*/
			return _("unsupported user action");
	}//switch
}/*online_service_request_action_to_string*/

static OnlineServiceRequest *online_service_request_new(RequestAction action, GtkWindow *parent, const gchar *user_data){
	if(action==SelectService || action==ViewProfile || action == Confirmation || G_STR_EMPTY(user_data)) return NULL;
	
	OnlineServiceRequest *request=g_new(OnlineServiceRequest, 1);
	
	request->parent=parent;
	request->user_data=g_strdup(user_data);
	request->action=action;
	request->method=QUEUE;
	request->message=g_strdup(online_service_request_action_to_string(action));
	
	switch(request->action){
		case ViewTweets:
			request->method=QUEUE;
			request->uri=g_strdup_printf(API_TIMELINE_USER, request->user_data);
			network_set_state_loading_timeline(request->uri, Load);
			break;
		case Follow:
			request->method=POST;
			request->uri=g_strdup_printf(API_USER_FOLLOW, request->user_data);
			break;
		case UnFollow:
			request->method=POST;
			request->uri=g_strdup_printf(API_USER_UNFOLLOW, request->user_data);
			break;
		case Block:
			request->method=POST;
			request->uri=g_strdup_printf(API_USER_BLOCK, request->user_data);
			break;
		case UnBlock:
			request->method=POST;
			request->uri=g_strdup_printf(API_USER_UNBLOCK, request->user_data);
			break;
		case Fave:
			request->method=POST;
			request->uri=g_strdup_printf(API_FAVE, request->user_data);
			break;
		case UnFave:
			request->method=POST;
			request->uri=g_strdup_printf(API_UNFAVE, request->user_data);
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

static void online_service_request_main(OnlineService *service, RequestAction action, GtkWindow *parent, const gchar *user_data){
	if(action==SelectService || action==Confirmation) return;

	if(G_STR_EMPTY(user_data)){
		debug("Cannot %s required information is missing.", online_service_request_action_to_string(action));
		main_window_statusbar_printf("Cannot %s required information is missing.", online_service_request_action_to_string(action));
		return;
	}

	if(action==ViewProfile){
		user_profile_viewer_show(service, user_data, parent);
		return;
	}
	
	OnlineServiceRequest *request=NULL;
	if(!(request=online_service_request_new(action, parent, user_data)))
		return;
	
	debug("Processing OnlineServiceRequest to %s %s on %s", request->message, request->user_data, online_service_get_key(service));
	
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
	
	tweet_view_sexy_select();
}/*online_service_request_main(service, parent, Follow|UnFollow|ViewProfile|ViewTweets|..., user_data);*/

void *online_service_request_main_quit(SoupSession *session, SoupMessage *msg, OnlineServiceWrapper *service_wrapper){
	OnlineServiceRequest *request=(OnlineServiceRequest *)online_service_wrapper_get_user_data(service_wrapper);
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	
	const gchar *service_key=online_service_get_key(service);
	if( msg->status_code!=403 && !network_check_http(service, msg) ){
		debug("**ERORR:** OnlineServiceRequest to %s %s.  OnlineService: '%s':\n\t\tServer response: %i", request->message, request->user_data, service_key, msg->status_code);
		
		main_window_statusbar_printf("Failed to %s on %s.  Error %s (%d).", request->message, service_key, msg->reason_phrase, msg->status_code);
		online_service_request_free(request);
		return NULL;
	}
	
	User *user=NULL;
	OnlineServiceWrapper *request_wrapper=NULL;
	debug("OnlineServiceRequest to %s %s.  OnlineService: '%s':", request->message, request->user_data, service_key);
	switch(request->action){
		case ViewTweets:
			request_wrapper=online_service_wrapper_new(service, QUEUE, online_service_wrapper_get_requested_uri(service_wrapper), (online_service_wrapper_get_attempt(service_wrapper)+1), NULL, network_display_timeline, tweet_lists_get_timeline(request->uri), (gpointer)Users);
			network_display_timeline(session, msg, request_wrapper);
			online_service_wrapper_free(request_wrapper);
			break;
		case UnFollow:
		case Block:
		case Follow:
			if(!(user=user_parse_profile(online_service_get_session(service), msg, service_wrapper))){
				if(msg->status_code!=403){
					debug("\t\t[failed]");
					main_window_statusbar_printf("Failed to %s %s on %s.", request->message, request->user_data, service_key);
				}else{
					debug("\t\t[duplicate request]");
					debug("%s %s %s on %s.", ((request->action==UnFollow) ?_("You're not following") :_("You've already") ), (request->action==UnFollow ?"" : request->message), request->user_data, service_key);
					statusbar_printf("%s %s %s on %s.", ((request->action==UnFollow) ?_("You're not following") :_("You've already") ), (request->action==UnFollow ?"" : request->message), request->user_data, service_key);
				}
			}else{
				debug("\t\t[succeeded]");
				if(request->action==Follow)
					users_glists_append_friend(service, user);
				else if(request->action==UnFollow)
					users_glists_remove_friend(service, user);
				else if(request->action==Block){
					users_glists_remove_friend(service, user);
					users_glists_remove_follower(service, user);
				}
				main_window_statusbar_printf("Successfully %s %s on %s.", request->message, request->user_data, service_key);
				user_free(user);
			}
			
			break;
		case Fave:
		case UnFave:
		case UnBlock:
			if(msg->status_code!=403){
				debug("\t\t[succceed]");
				main_window_statusbar_printf("Successfully %s on %s.", request->message, service_key);
			}else{
				debug("\t\t[duplicate request]");
				main_window_statusbar_printf("You've already %s on %s.", request->message, service_key);
			}
			break;
		case SelectService:
		case ViewProfile:
		case Confirmation:
		default:
			/*more cases of to make gcc happy.*/
			break;
	}//switch
	
	online_service_request_free(request);
	return NULL;
}/*online_service_request_main_quit*/

static void online_service_request_free(OnlineServiceRequest *request){
	request->parent=NULL;
	uber_object_free(&request->uri, &request->user_data, &request->message, &request, NULL);
}/*online_service_request_free*/


/********************************************************************************
 *               selected_update methods, handlers, callbacks, & etc.            *
 ********************************************************************************/
void set_selected_update(OnlineService *service, const gdouble id, const gdouble user_id, const gchar *user_name, const gchar *tweet){
	/*	gint id=atoi(string);	*/
	if(selected_update) unset_selected_update();
	
	debug("SelectedTweet created from '%s', update ID: #%f from: '%s' on <%s>.", online_service_get_key(service), id, user_name, online_service_get_uri(service));
	debug("SelectedTweet's update: %s.", tweet);
	selected_update=g_new0(SelectedTweet, 1);
	selected_update->service=service;
	selected_update->id=id;
	selected_update->user_id=user_id;
	selected_update->user_name=g_strdup(user_name);
	selected_update->tweet=g_uri_unescape_string(tweet, NULL);
}/*set_selected_update*/

OnlineService *selected_update_get_service(void){
	return ( (selected_update && selected_update->service) ?selected_update->service :NULL );
}/*selected_update_get_service();*/

gdouble selected_update_get_id(void){
	return ( (selected_update && selected_update->id) ?selected_update->id :0.0 );
}/*selected_update_get_id();*/

gchar *selected_update_get_user_name(void){
	return ( (selected_update && selected_update->user_name) ?selected_update->user_name :NULL );
}/*selected_update_get_user_name();*/

gdouble selected_update_get_user_id(void){
	return ( (selected_update && selected_update->user_id) ?selected_update->user_id :0.0 );
}/*selected_update_get_user_id();*/

gchar *selected_update_reply_to_strdup(gboolean retweet){
	if(!(selected_update && selected_update->user_name && G_STR_N_EMPTY(selected_update->user_name)))
		return NULL;
	
	if(!( (gconfig_if_bool(PREFS_TWEETS_NO_PROFILE_LINK, TRUE)) && online_services_has_connected(online_services, 1) ))
		return g_strdup_printf("%s@%s ( http://%s/%s ) %s", (retweet ?"RT " :""), selected_update->user_name, online_service_get_uri(selected_update->service), selected_update->user_name, (retweet ?selected_update->tweet :"" ));
	
	return g_strdup_printf("%s@%s %s", (retweet ?"RT " :""), selected_update->user_name, (retweet ?selected_update->tweet :"" ));
}/*selected_update_reply_to_strdup();*/

void selected_update_reply(void){
	selected_update_include_and_begin_to_send(selected_update_reply_to_strdup(FALSE), TRUE, TRUE);
}/*selected_update_reply();*/

void selected_update_retweet(void){
	selected_update_include_and_begin_to_send(selected_update_reply_to_strdup(TRUE), TRUE, TRUE);
}/*selected_update_reply();*/

static void selected_update_include_and_begin_to_send(gchar *tweet, gboolean in_response, gboolean release){
	if(!( ( tweet && G_STR_N_EMPTY(tweet) ) )){
		tweet_view_beep();
		if(tweet && release) uber_free(tweet);
		return;
	}
	
	if(!selected_update) return;
	
	if(in_response){
		in_reply_to_status_id=selected_update->id;
		in_reply_to_service=selected_update->service;
	}
	
	tweet_view_sexy_prefix_string(tweet);
	
	if(!release) return;
	
	uber_free(tweet);
}/*selected_update_include_and_begin_to_send*/

void unset_selected_update(void){
	if(!selected_update) return;
	debug("Destroying current selected_update object.");
	selected_update->service=NULL;
	
	uber_object_free(&selected_update->user_name, &selected_update->tweet, &selected_update, NULL);
}/*unset_selected_update*/

void online_service_request_selected_update_view_tweets(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, ViewTweets, ( gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE) ?tweet_view_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_view_tweets();*/

void online_service_request_selected_update_view_profile(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, ViewProfile, ( gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE) ?tweet_view_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_view_profile*/

void online_service_request_selected_update_follow(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, Follow, ( gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE) ?tweet_view_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_follow*/

void online_service_request_selected_update_unfollow(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, UnFollow, ( gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE) ?tweet_view_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_unfollow*/

void online_service_request_selected_update_block(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, Block, ( gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE) ?tweet_view_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_block*/

void online_service_request_selected_update_unblock(void){
	if(!(selected_update && selected_update->user_name)) return;
	online_service_request_main(selected_update->service, UnBlock, ( gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE) ?tweet_view_get_window() :main_window_get_window() ), selected_update->user_name);
}/*online_service_request_selected_update_unblock*/

void online_service_request_selected_update_save_fave(void){
	if(!(selected_update && selected_update->id)) return;
	gchar *fave_tweet_id=gdouble_to_str(selected_update->id);
	online_service_request_main(selected_update->service, Fave, ( gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE) ?tweet_view_get_window() :main_window_get_window() ), fave_tweet_id);
	g_free(fave_tweet_id);
}/*online_service_request_selected_update_save_fave*/

void online_service_request_selected_update_destroy_fave(void){
	if(!(selected_update && selected_update->id)) return;
	gchar *fave_tweet_id=gdouble_to_str(selected_update->id);
	online_service_request_main(selected_update->service, UnFave, ( gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE) ?tweet_view_get_window() :main_window_get_window() ), fave_tweet_id);
	g_free(fave_tweet_id);
}/*online_service_request_selected_update_destroy_fave*/


/********************************************************************************
 *         online_service_request's methods, handlers, callbacks, & etc.        *
 ********************************************************************************/
static void online_service_request_popup_set_title_and_label(RequestAction action, OnlineServiceRequestPopup *online_service_request_popup){
	switch( action ){
		case Confirmation:
			gtk_window_set_title(GTK_WINDOW( online_service_request_popup->dialog), "Are you sure you want to:");
			break;
		case SelectService:
			gtk_window_set_title(GTK_WINDOW( online_service_request_popup->dialog), "Please select account to use:");
			gchar *label_markup=g_markup_printf_escaped("Please select the 'default' account you want to use for sending direct messages, managing friends, and etc.\n\n<span weight=\"bold\">NOTE: You're being asked this before %s loads your friends, followers, or both.  This may take a while, so after selecting your account, please be patient while you're friends, followers, or both are download.  They will be displeyed, it may just take awhile.</span>\n\nYou can select a different account at any time by selecting 'Select Default Account' from the 'Accounts' file menu:", GETTEXT_PACKAGE);
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, label_markup);
			g_free(label_markup);
			break;
		case ViewTweets:
			gtk_window_set_title(GTK_WINDOW( online_service_request_popup->dialog), "Who's tweets do you want to see?" );
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or user id, who's resent updates you would like to view:");
			break;
		case ViewProfile:
			gtk_window_set_title(GTK_WINDOW( online_service_request_popup->dialog), "Who's profile do you want to see?" );
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to view:");
			break;
		case Block:
			gtk_window_set_title(GTK_WINDOW( online_service_request_popup->dialog), "Whom do you want to block?" );
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to block?  They'll no longer be able to read your tweets, send you messages, and you'll no longer be notified when they 'mention' you, using the @ symbol:");
			break;
		case UnBlock:
			gtk_window_set_title(GTK_WINDOW( online_service_request_popup->dialog), "Whom do you want to un-block?" );
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to un-block?  They'll be able to once again read your tweets and you'll see when they mention you, using the @ symbol, again:");
			break;
		case UnFollow:
			gtk_window_set_title(GTK_WINDOW( online_service_request_popup->dialog), "Whom you want to un-follow?" );
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to un-follow?");
			break;
		case Follow:
			gtk_window_set_title(GTK_WINDOW( online_service_request_popup->dialog), "Whom do you want to follow:" );
			gtk_message_dialog_set_markup( online_service_request_popup->dialog, "Please enter the user user_name, or id, of whom you want to follow:");
			break;
		case Fave:
		case UnFave:
		default:
			break;
	}
}/*online_service_request_popup_set_title*/


static void online_service_request_popup_response_cb(GtkWidget *widget, gint response, OnlineServiceRequestPopup *online_service_request_popup){
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
			gtk_widget_destroy(widget);
			return;
	}
	
	if( online_service_request_popup->action==SelectService){
		online_service_request_popup_set_selected_service(widget, response, online_service_request_popup);
		gtk_widget_destroy(widget);
		return;
	}
	
	if( online_service_request_popup->action==Confirmation){
		online_service_request_popup_dialog_process_confirmation(widget, response, online_service_request_popup);
		gtk_widget_destroy(widget);
		return;
	}
	
	switch(  online_service_request_popup->action ){
		case ViewProfile:
		case Follow:
		case UnFollow:
		case Block:
		case UnBlock:
		case ViewTweets:
			if(online_service_request_popup_dialog_process_requests(widget, response, online_service_request_popup))
				gtk_widget_destroy(widget);
			break;
		case SelectService:
		case Fave:
		case UnFave:
		case Confirmation:
		default:
			/*All to make gcc nice & happy.*/
			gtk_widget_destroy(widget);
			break;
	}//switch
}/*online_service_request_popup_response_cb*/

static gboolean online_service_request_popup_dialog_process_requests(GtkWidget *widget, gint response, OnlineServiceRequestPopup *online_service_request_popup){
	OnlineService		*service=NULL;
	const gchar		*user_name=gtk_entry_get_text(online_service_request_popup->user_name_entry);
	
	if(G_STR_EMPTY(user_name)){
		tweet_view_beep();
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

static void online_service_request_popup_dialog_process_confirmation(GtkWidget *widget, gint response, OnlineServiceRequestPopup *online_service_request_popup){
	GFunc        func;
	gpointer     user_data;
	const gchar *gconfig_path;
	
	gconfig_path=g_object_get_data(G_OBJECT(widget), "gconfig_path");
	func=g_object_get_data(G_OBJECT(widget), "func");
	user_data=g_object_get_data(G_OBJECT(widget), "user_data");
	
	gconfig_set_bool(gconfig_path, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON( online_service_request_popup->check_button)));
	
	if(func)
		func( (gpointer)gconfig_path, user_data);
}/*online_service_request_popup_dialog_process_confirmation(widget, response, popup);*/


static void online_service_request_popup_set_selected_service(GtkWidget *widget, gint response, OnlineServiceRequestPopup *online_service_request_popup){
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	
	if(gtk_combo_box_get_active_iter( online_service_request_popup->online_services_combo_box, iter)){
		gtk_tree_model_get(
				 online_service_request_popup->online_service_model,
				iter,
				OnlineServicePointer, &selected_service,
				-1
		);
	}
	uber_free(iter);
}/*online_service_request_popup_set_selected_service*/

static void online_service_request_popup_destroy_cb(GtkWidget *widget, OnlineServiceRequestPopup *online_service_request_popup){
	online_service_request_popup_destroy_and_free();
}/*online_service_request_popup_destroy_cb*/

static void online_service_request_popup_destroy_and_free(void){
	gtk_widget_destroy( GTK_WIDGET( online_service_request_popup->dialog) );
	uber_free(online_service_request_popup);
}/*online_service_request_popup_destroy_and_free*/

gboolean online_service_request_popup_confirmation_dialog(const gchar *gconfig_path, const gchar *message1, const gchar *message2, GFunc func, gpointer user_data){
	if(gconfig_path && gconfig_if_bool(gconfig_path, FALSE))
		return TRUE;
	
	online_service_request_popup_dialog_show(Confirmation);
	
	gtk_message_dialog_set_markup( online_service_request_popup->dialog, message1);
	gtk_message_dialog_format_secondary_text( online_service_request_popup->dialog, "%s", message2);
	if(gconfig_path){
		GtkVBox			*vbox;
		
		 online_service_request_popup->check_button=(GtkCheckButton *)gtk_check_button_new_with_label(_("Do not show this again"));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON( online_service_request_popup->check_button), FALSE);
		
		vbox=(GtkVBox *)gtk_vbox_new(FALSE, 6);
		gtk_container_set_border_width(GTK_CONTAINER(vbox), 6);
		
		gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET( online_service_request_popup->check_button), FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(GTK_DIALOG( online_service_request_popup->dialog)->vbox), GTK_WIDGET(vbox), FALSE, FALSE, 0);
		
		g_object_set_data_full(G_OBJECT( online_service_request_popup->dialog), "gconfig_path", g_strdup(gconfig_path), g_free);
		g_object_set_data(G_OBJECT( online_service_request_popup->dialog), "user_data", user_data);
		g_object_set_data(G_OBJECT( online_service_request_popup->dialog), "func", func);
	}
	
	gtk_widget_show_all(GTK_WIDGET( online_service_request_popup->dialog));
	
	gtk_widget_hide(GTK_WIDGET( online_service_request_popup->user_name_frame));
	gtk_widget_hide(GTK_WIDGET( online_service_request_popup->online_services_frame));
	
	gtk_dialog_run(GTK_DIALOG( online_service_request_popup->dialog));
	switch(online_service_request_popup_dialog_response){
		case GTK_RESPONSE_YES: case GTK_RESPONSE_OK: case GTK_RESPONSE_NONE: case GTK_RESPONSE_APPLY:
			return TRUE;
		default:
			return FALSE;
	}
}/*online_service_request_popup_confirm_dialog();*/

void online_service_request_popup_select_service(void){
	if(!online_services_has_connected(online_services, 1)){
		if(selected_service) return;
		selected_service=online_services_connected_get_first(online_services);
		debug("There is only one connected OnlineService, auto-selecting: %s.", online_service_get_key(selected_service));
		return;
	}
	
	debug("Prompting to select OnlineService to use as 'selected_service'.");
	online_service_request_popup_dialog_show(SelectService);
	
	gtk_widget_hide(GTK_WIDGET( online_service_request_popup->user_name_frame));
	gtk_dialog_run(GTK_DIALOG( online_service_request_popup->dialog));
}/*online_service_request_popup_select_service*/

void online_service_request_popup_friend_follow(void){
	online_service_request_popup_dialog_show(Follow);
}/*online_service_request_popup_friend_follow*/

void online_service_request_popup_friend_unfollow(void){
	online_service_request_popup_dialog_show(UnFollow);
}/*online_service_request_popup_friend_unfollow*/

void online_service_request_popup_friend_block(void){
	online_service_request_popup_dialog_show(Block);
}/*online_service_request_popup_friend_block*/

void online_service_request_popup_friend_unblock(void){
	online_service_request_popup_dialog_show(UnBlock);
}/*online_service_request_popup_friend_unblock*/

void online_service_request_popup_friend_tweets(void){
	online_service_request_popup_dialog_show(ViewTweets);
}/*online_service_request_popup_friend_tweets*/

void online_service_request_popup_friend_profile(void){
	online_service_request_popup_dialog_show(ViewProfile);
}/*online_service_request_popup_friend_profile*/


static gboolean online_service_request_popup_validate_usage(RequestAction action){
	switch( action ){
		case Confirmation:
		case SelectService:
		case ViewProfile:
		case ViewTweets:
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
	
	GtkWindow *parent=( gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE) ?tweet_view_get_window() :main_window_get_window() );
	
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
	gtkbuilder_connect(
			ui, online_service_request_popup,
				"entry_popup", "destroy", online_service_request_popup_destroy_cb,
				"entry_popup", "response", online_service_request_popup_response_cb,
			NULL
	);
	
	g_object_unref(ui);
	
	debug("Signal handlers set... loading accounts.");
	if(!( online_services_combo_box_fill(online_services,  online_service_request_popup->online_services_combo_box,  online_service_request_popup->online_services_list_store, TRUE) ))
		debug("No services found to load, new accounts need to be setup.");
	else
		debug("OnlineServices found & loaded.  Selecting active service.");
	
	if(!(online_services_has_connected(online_services, 1) && online_services_has_total(online_services, 1))){
		debug("There is only one service to select from so we don't really need to ask.\n\t\tSo we'll just hide 'online_services_frame'.");
		gtk_widget_hide(GTK_WIDGET( online_service_request_popup->online_services_frame));
	}
	
	online_service_request_popup_set_title_and_label(action, online_service_request_popup);
	
	g_object_add_weak_pointer(G_OBJECT( online_service_request_popup->dialog), (gpointer)&online_service_request_popup);
	gtk_window_set_transient_for(GTK_WINDOW( online_service_request_popup->dialog), parent );
	gtk_widget_show_all(GTK_WIDGET( online_service_request_popup->dialog));
	gtk_window_present(GTK_WINDOW( online_service_request_popup->dialog));
}/*online_service_request_popup_dialog_show*/


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

