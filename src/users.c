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
 * project, object, system & library headers, eg #include <gdk/gdkkeysyms.h>    *
 ********************************************************************************/
#include <time.h>
#include <strings.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <libsoup/soup.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libnotify/notify.h>

#include "config.h"
#include "program.h"
#include "main-window.h"

#include "online-services-typedefs.h"
#include "online-service.types.h"
#include "online-service.h"
#include "online-services.h"
#include "online-service-wrapper.h"
#include "network.h"

#include "users.types.h"
#include "users-glists.h"
#include "users.h"
#include "cache.h"

#include "gconfig.h"
#include "preferences.h"
#include "gtkbuilder.h"
#include "cache.h"
#include "parser.h"
#include "label.h"

#include "ui-utils.h"
#include "update-viewer.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
typedef struct  _UserProfileViewer UserProfileViewer;

struct _UserProfileViewer{
	OnlineService		*service;
	gchar			*user_name;
	gboolean		loading;
	
	GtkMessageDialog	*dialog;
	
	GtkImage		*user_image;
	GtkImage		*image;
	
	GtkVBox			*title_vbox;
	
	Label			*service_label;
	Label			*user_label;
	
	GtkVBox			*profile_vbox;
	Label			*url_hyperlink;
	Label			*bio_html;
	
	GtkVBox			*latest_update_vbox;
	GtkLabel		*latest_update_label;
	GtkLabel		*updated_when_label;
	Label			*most_recent_update;
};


#define API_USER_FOLLOW		"/friendships/create/%s.xml"
#define API_USER_UNFOLLOW	"/friendships/destroy/%s.xml"
#define API_USER_BLOCK		"/blocks/create/%s.xml"
#define API_USER_UNBLOCK	"/blocks/destroy/%s.xml"

#define API_USER_PROFILE	"/users/show/%s.xml"

#define	DEBUG_DOMAINS	"UI:GtkBuilder:GtkBuildable:OnlineServices:Updates:Requests:Dates:Times:Users:Settings:Profiles:UserProfileViewer:ViewProfile:users.c"
#include "debug.h"

#define GtkBuilderUI "user-profile-viewer"
static UserProfileViewer *user_profile_viewer=NULL;


/********************************************************************************
 *                    prototypes for private method & function                  *
 ********************************************************************************/
static User *user_new(OnlineService *service, gboolean a_follower);
static void user_validate( User **user );


static UserStatus *user_status_new(OnlineService *service, UpdateMonitor monitoring);
static void user_status_validate( UserStatus **status );
static void user_status_format_updates(OnlineService *service, User *user, UserStatus *status);
static void user_status_format_dates(UserStatus *status);


static void user_profile_viewer_response(GtkDialog *dialog, gint response);
static void user_profile_viewer_destroy(GtkDialog *dialog, UserProfileViewer *user_profile_viewer);

static void user_profile_viewer_setup(void);

static void user_profile_viewer_display_profile(OnlineServiceWrapper *online_service_wrapper, SoupMessage *xml, User *user);

static void user_profile_viewer_set_avatar(const gchar *image_file, const gchar *image_uri);
static void user_profile_viewer_download_avatar(const gchar *image_file, const gchar *image_uri);
static void *user_profile_viewer_save_avatar(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper);

static void user_profile_viewer_hide_all(void);
static void user_profile_viewer_show_all(void);



/********************************************************************************
 *                  'Here be Dragons'...art, beauty, fun, & magic.              *
 ********************************************************************************/


/********************************************************************************
 *                                User methods                                  *
 ********************************************************************************/
static User *user_new(OnlineService *service, gboolean a_follower){
	User *user=g_new0(User, 1);
	
	user->follower=a_follower;
	user->service=service;
	user->status=NULL;
	user->id_str=user->user_name=user->nick_name=user->location=user->bio=user->url=user->image_uri=user->image_file=NULL;
	
	return user;
}/*user_new*/

static void user_validate( User **user ){
	if(! (*user)->user_name ) (*user)->user_name=g_strdup("");
	if(! (*user)->nick_name ) (*user)->nick_name=g_strdup("");
	if(! (*user)->location ) (*user)->location=g_strdup("");
	if(! (*user)->bio ) (*user)->bio=g_strdup("");
	if(! (*user)->url ) (*user)->url=g_strdup("");
	if(! (*user)->image_uri ) (*user)->image_uri=g_strdup("");
	if(! (*user)->image_file ) (*user)->image_file=g_strdup("");
}/*user_validate(&user);*/ 

User *user_parse_profile(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	xmlDoc *doc=NULL;
	xmlNode *root_element=NULL;
	User *user=NULL;
	
	if(!( (doc=parse_xml_doc(xml, &root_element )) )){
		xmlCleanupParser();
		return NULL;
	}
	
	if(g_str_equal(root_element->name, "user"))
		user=user_parse_node(service, root_element->children);
		/*TODO finish caching
		if((user=user_parse_node(service, root_element->children)))
			cache_save_page(service, online_service_wrapper_get_requested_uri(service_wrapper), xml->response_body);
		*/
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return user;
}/*user_parse_profile(service, xml, service_wrapper);*/

User *user_parse_node(OnlineService *service, xmlNode *root_element){
	xmlNode		*current_node=NULL;
	gchar		*content=NULL;
	
	User		*user=user_new(service, getting_followers);
	
	debug("Parsing user profile data.");
	/* Begin 'users' node loop */
	for(current_node=root_element; current_node; current_node=current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE)
			continue;
		
		if( G_STR_EMPTY( (content=(gchar *)xmlNodeGetContent(current_node)) ) ) continue;
		
		if(g_str_equal(current_node->name, "id" )){
			user->id=strtod(content, NULL);
			user->id_str=gdouble_to_str(user->id);
			debug("User ID: %s(=%f).", user->id_str, user->id);
			
		}else if(g_str_equal(current_node->name, "name" ))
			user->nick_name=g_markup_printf_escaped("%s", content);
		
		else if(g_str_equal(current_node->name, "screen_name" ))
			user->user_name=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "location" ))
			user->location=g_markup_printf_escaped("%s", content);
		
		else if(g_str_equal(current_node->name, "description" ))
			user->bio=g_markup_printf_escaped( "%s", content );
		
		else if(g_str_equal(current_node->name, "url" ))
			user->url=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "followers_count" ))
			user->followers=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_node->name, "friends_count" ))
			user->following=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_node->name, "statuses_count" ))
			user->updates=strtoul( content, NULL, 10 );
		
		else if(g_str_equal(current_node->name, "profile_image_url"))
			user->image_uri=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "status") && current_node->children)
			user->status=user_status_parse(service, current_node->children, Updates);
		
		xmlFree(content);
		
	}
	
	user_validate(&user);
	
	if(user->status)
		user_status_format_updates(service, user, user->status);
	
	user->image_file=cache_images_get_user_avatar_filename(service, user->user_name, user->image_uri);
	
	return user;
}/*user_parse_node(service, root_node); || user_parse_node(service, current_node->children);*/

void user_free(User *user){
	if(!( user && user->id && user->service )) return;
	if(user->status) user_status_free(user->status);
	user_validate(&user);
	
	user->service=NULL;
	
	if(user->user_name) uber_free(user->user_name);
	if(user->nick_name) uber_free(user->nick_name);
	if(user->location) uber_free(user->location);
	if(user->bio) uber_free(user->bio);
	if(user->url) uber_free(user->url);
	if(user->image_uri) uber_free(user->image_uri);
	if(user->image_file) uber_free(user->image_file);

	uber_free(user);
	
	/*uber_object_free(&user->user_name, &user->nick_name, &user->location, &user->bio, &user->url, &user->image_uri, &user->image_file, &user, NULL);*/
}/*user_free*/


/********************************************************************************
 *                           UserStatus methods                                 *
 ********************************************************************************/
static UserStatus *user_status_new(OnlineService *service, UpdateMonitor monitoring){
	UserStatus	*status=g_new0(UserStatus, 1);
	
	status->service=service;
	status->user=NULL;
	status->type=monitoring;
	status->id_str=status->text=status->update=status->notification=status->sexy_update=status->created_at_str=status->created_how_long_ago=NULL;
	status->id=status->in_reply_to_status_id=0.0;
	status->created_at=0;
	status->created_seconds_ago=0;
	
	return status;
}/*user_status_new(service, Updates|Replies|Dms);*/

static void user_status_validate( UserStatus **status ){
	if(! (*status)->text ) (*status)->text=g_strdup("");
	if(! (*status)->id_str ) (*status)->id_str=g_strdup("");
	if(! (*status)->from ) (*status)->from=g_strdup("");
	if(! (*status)->rcpt ) (*status)->rcpt=g_strdup("");
	if(! (*status)->update ) (*status)->update=g_strdup("");
	if(! (*status)->source ) (*status)->source=g_strdup("");
	if(! (*status)->sexy_update ) (*status)->sexy_update=g_strdup("");
	if(! (*status)->notification ) (*status)->notification=g_strdup("");
	if(! (*status)->created_at_str ) (*status)->created_at_str=g_strdup("");
	if(! (*status)->created_how_long_ago ) (*status)->created_how_long_ago=g_strdup("");
}/*user_status_validate(&status);*/ 

UserStatus *user_status_parse(OnlineService *service, xmlNode *root_element, UpdateMonitor monitoring){
	xmlNode		*current_node = NULL;
	gchar		*content=NULL;
	UserStatus	*status=user_status_new(service, monitoring);
	
	/* Begin 'status' or 'direct-messages' loop */
	debug("Parsing status & update at node: %s", root_element->name);
	for(current_node=root_element; current_node; current_node=current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE) continue;
		
		if( G_STR_EMPTY( (content=(gchar *)xmlNodeGetContent(current_node)) ) ){
			if(content) g_free(content);
			continue;
		}
		
		if(g_str_equal(current_node->name, "id")){
			status->id=strtod(content, NULL);
			status->id_str=gdouble_to_str(status->id);
			debug("Status ID: %s(=%f).", content, status->id);
			
		}else if(g_str_equal(current_node->name, "in_reply_to_status_id"))
			status->in_reply_to_status_id=strtod(content, NULL);
		
		else if(g_str_equal(current_node->name, "source"))
			status->source=g_strdup(content);
		
		else if((g_str_equal(current_node->name, "sender") || g_str_equal(current_node->name, "user")) && current_node->children)
			status->user=user_parse_node(service, current_node->children);
		
		else if(g_str_equal(current_node->name, "text"))
			status->text=g_strdup(content);
		
		else if(g_str_equal(current_node->name, "created_at")){
			status->created_at_str=g_strdup(content);
			user_status_format_dates(status);
		}
		
		xmlFree(content);
		
	}

	user_status_validate(&status);
	
	if(status->user)
		user_status_format_updates(service, status->user, status);
	
	return status;
}/*user_status_parse(service, current_node->children, monitoring);*/

static void user_status_format_dates(UserStatus *status){
	tzset();
	time_t		t=time(NULL);
	struct tm	*ta=gmtime(&t);
	ta->tm_isdst=-1;
	
	struct tm	post;
	strptime(status->created_at_str, "%s", &post);
	post.tm_isdst=-1;
	status->created_at=mktime(&post);
	
	debug("Parsing update's 'created_at' date: [%s] to Unix seconds since: %lu", status->created_at_str, status->created_at);
	status->created_how_long_ago=parser_convert_time(status->created_at_str, &status->created_seconds_ago);
	debug("Display time set to: %s, %d.", status->created_how_long_ago, status->created_seconds_ago);
}/*user_status_format_dates*/


static void user_status_format_updates(OnlineService *service, User *user, UserStatus *status){
	if(!(service->connected && G_STR_N_EMPTY(status->text) && G_STR_N_EMPTY(user->user_name) && G_STR_N_EMPTY(user->nick_name))) return;
	debug("Formatting status text for display.");
	
	gchar *sexy_status_text=NULL, *sexy_status_swap=parser_escape_text(status->text);
	/*if(!gconfig_if_bool(PREFS_URLS_EXPANSION_SELECTED_ONLY, TRUE)){
		status->sexy_update=label_msg_format_urls(service, sexy_status_swap, TRUE, TRUE);
		sexy_status_text=label_msg_format_urls(service, sexy_status_swap, TRUE, FALSE);
	}else{*/
		status->sexy_update=g_strdup(sexy_status_swap);
		sexy_status_text=label_msg_format_urls(service, sexy_status_swap, FALSE, FALSE);
	//}
	uber_free(sexy_status_swap);
	
	status->from=g_strdup_printf("<span size=\"small\" weight=\"ultrabold\">%s\n&lt;%s@%s&gt;</span>", user->nick_name, user->user_name, service->uri);
	
	status->rcpt=g_strdup_printf("<span size=\"small\" weight=\"light\">%s\n&lt;%s&gt;</span>", service->nick_name, service->key);
	
	status->update=g_strdup_printf("%s%s%s%s\n",
					( (status->type==DMs)
					  	?"<span weight=\"ultrabold\" style=\"italic\" variant=\"smallcaps\">[Direct Message]</span>\n"
						:(status->type==Replies ?"<span style=\"italic\" variant=\"smallcaps\">[@ reply]</span>\n"
							:""
						)
					),
					((status->type==DMs) ?"<span weight=\"ultrabold\" style=\"italic\">[" :""), sexy_status_text, ((status->type==DMs) ?"]</span>" :"")
	);
	
	status->notification=g_strdup_printf(
						"%s<i>[%s]</i>\n\t<u><b>From:</b></u> <b>%s &lt;@%s on %s&gt;</b>\n<i><u>To:</u></i> <i>%s &lt;%s&gt;</i>\n<b>\t%s%s%s</b>",
						((status->type==DMs) ?"<b><i><u>[Direct Message]</u></i></b>\n" :((status->type==Replies) ?"<i><u>[@ Reply]</u></i>\n" :"")),
						status->created_how_long_ago,
						user->nick_name, user->user_name, service->uri,
						service->nick_name, service->key,
						((status->type==DMs) ?"<b><i>[" :((status->type==Replies) ?"<i>[" :"")), sexy_status_text, ((status->type==DMs) ?"]</i></b>" :((status->type==Replies) ?"]</i>" :""))
	);
	
	g_free(sexy_status_text);
}/*user_status_format_updates(service, status, user);*/

gboolean user_status_notify_on_timeout(UserStatus *status){
	if(!(status && G_STR_N_EMPTY(status->notification) )) return FALSE;
	
	NotifyNotification *notify_notification;
	GError             *error=NULL;
	
	if(!main_window_status_icon_is_embedded())
		notify_notification=notify_notification_new(PACKAGE_TARNAME, status->notification, PACKAGE_TARNAME, NULL);
	else
		notify_notification=notify_notification_new_with_status_icon(PACKAGE_TARNAME, status->notification, PACKAGE_TARNAME, main_window_status_icon_get());
	
	notify_notification_set_timeout(notify_notification, 10000);
	
	if(gconfig_if_bool(PREFS_NOTIFY_BEEP, TRUE))
		update_viewer_beep();
	
	notify_notification_show(notify_notification, &error);
	
	g_object_unref(G_OBJECT(notify_notification));
	
	if(error){
		debug("Error displaying status->notification: %s.", error->message);
		g_error_free(error);
		return FALSE;
	}
	
	gtk_window_set_urgency_hint(main_window_get_window(), FALSE );
	
	return FALSE;
}/*user_status_notify_on_timeout*/

void user_status_free(UserStatus *status){
	if(!( status && status->id && status->service )) return;
	if(status->user) user_free(status->user);
	user_status_validate(&status);
	status->service=NULL;
	
	if(status->text) uber_free(status->text);
	if(status->id_str) uber_free(status->id_str);
	if(status->from) uber_free(status->from);
	if(status->rcpt) uber_free(status->rcpt);
	if(status->update) uber_free(status->update);
	if(status->source) uber_free(status->source);
	if(status->sexy_update) uber_free(status->sexy_update);
	if(status->notification) uber_free(status->notification);
	if(status->created_at_str) uber_free(status->created_at_str);
	if(status->created_how_long_ago) uber_free(status->created_how_long_ago);

	uber_free(status);
	
	/*uber_object_free(&status->text, &status->id_str, &status->from, &status->rcpt, &status->update, &status->source, &status->sexy_update, &status->notification, &status->created_at_str, &status->created_how_long_ago, &status, NULL);*/
}/*user_status_free*/



/********************************************************************************
 *                       UserProfileViewer methods                              *
 ********************************************************************************/
void user_profile_viewer_cleanup(void){
	if(!user_profile_viewer) return;
	if(user_profile_viewer->service) user_profile_viewer->service=NULL;
	if(user_profile_viewer->user_name) uber_free(user_profile_viewer->user_name);
	if(user_profile_viewer->dialog)
		gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->dialog));
	if(user_profile_viewer)
		uber_free(user_profile_viewer);
}/*user_profile_viewer_cleanup();*/


void user_profile_viewer_show(OnlineService *service, const gchar *user_name, GtkWindow *parent){
	if(!user_profile_viewer) user_profile_viewer_setup();
	else if(user_profile_viewer->service && user_profile_viewer->user_name && user_profile_viewer->service==service && !strcasecmp(user_profile_viewer->user_name, user_name) ){
		user_profile_viewer_show_all();
		return;
	}
	
	user_profile_viewer->service=service;;
	if(user_profile_viewer->user_name) uber_free(user_profile_viewer->user_name);
	user_profile_viewer->user_name=g_strdup(user_name);
	
	user_profile_viewer_hide_all();
	
	gchar *window_title=g_strdup_printf("%s - <%s@%s>'s %s", _(GETTEXT_PACKAGE), user_name, service->uri, _("profile"));
	gtk_window_set_title(GTK_WINDOW(user_profile_viewer->dialog), window_title);
	uber_free(window_title);
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->dialog));
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->user_image));
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->bio_html));
	
	g_object_add_weak_pointer(G_OBJECT(user_profile_viewer->dialog), (gpointer)&user_profile_viewer);
	gtk_window_set_transient_for(GTK_WINDOW(user_profile_viewer->dialog), parent);
	window_present(GTK_WINDOW(user_profile_viewer->dialog), TRUE);
	
	online_service_fetch_profile( service, user_name, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)user_profile_viewer_display_profile );
}/*user_profile_viewer_show( service, "uberChick", update_viewer->update_viewer );*/

static void user_profile_viewer_display_profile(OnlineServiceWrapper *online_service_wrapper, SoupMessage *xml, User *user){ 
	OnlineService *service=online_service_wrapper_get_online_service(online_service_wrapper);
	
	debug("Downloading user's avatar from: <%s> to [ file://%s ]", user->image_uri, user->image_file);
	user_profile_viewer_set_avatar(user->image_file, user->image_uri);
	
	gchar *profile_details=g_strdup_printf(
					"\t[<a href=\"http%s://%s/\">%s</a>]\n",
					(service->https?"s":""), service->uri, service->guid
	);
	
	label_set_text(service, LABEL(user_profile_viewer->service_label), user->status->id, profile_details, TRUE, TRUE);
	uber_free(profile_details);
	
	
	
	profile_details=g_strdup_printf(
					"\t\t<u><b>%s:</b></u> @%s",
					user->nick_name, user->user_name
	);
	label_set_text(service, LABEL(user_profile_viewer->user_label), user->status->id, profile_details, TRUE, TRUE);
	uber_free(profile_details);
	
	
	
	profile_details=g_strdup_printf(
					"\t<b>Updates:</b> %lu\n\t<b>Following:</b> %lu\n\t<b>Followers:</b> %lu\n\t<b>Location:</b> %s\n",
					user->updates,
					user->following,
					user->followers,
					user->location
	);
	gtk_message_dialog_set_markup( user_profile_viewer->dialog, profile_details );
	uber_free(profile_details);
	
	profile_details=g_strdup_printf( "\t<b>URL:</b>\t<a href=\"%s\">%s</a>\n", user->url, user->url );
	label_set_text(service, user_profile_viewer->url_hyperlink, user->status->id, profile_details, TRUE, TRUE);
	uber_free(profile_details);
	
	g_object_set(GTK_LABEL(user_profile_viewer->bio_html), "single-line-mode", FALSE, NULL );
	profile_details=g_strdup_printf( "\t<b>Bio:</b>\n\t\t%s\n", user->bio );
	label_set_text(service, user_profile_viewer->bio_html, user->status->id, profile_details, TRUE, TRUE);
	uber_free(profile_details);
	
	profile_details=g_markup_printf_escaped("<b>Last updated:</b> <i>[%s]</i>", user->status->created_how_long_ago);
	gtk_label_set_markup(user_profile_viewer->updated_when_label, profile_details);
	uber_free(profile_details);
	
	label_set_text(service, user_profile_viewer->most_recent_update, user->status->id, user->status->sexy_update, TRUE, TRUE);
	
	if(!user_profile_viewer->loading)
		user_profile_viewer_show_all();
	
	user_free(user);
}/*static void user_profile_viewer_display_profile(online_service_wrapper, xml, user);*/

static void user_profile_viewer_response(GtkDialog *dialog, gint response){
	gtk_widget_hide(GTK_WIDGET(dialog));
}/*user_profile_viewer_response(dialog, response, user_profile_viewer);*/

static void user_profile_viewer_destroy(GtkDialog *dialog, UserProfileViewer *user_profile_viewer){
	debug("Destroying user profile viewer");
	user_profile_viewer->service=NULL;
	if(user_profile_viewer->user_name) uber_free(user_profile_viewer->user_name);
	gtk_widget_destroy(GTK_WIDGET(dialog));
	uber_free(user_profile_viewer);
}/*user_profile_viewer_destroy(dialog, user_profile_viewer);*/


static void user_profile_viewer_setup(void){
	user_profile_viewer=g_new( UserProfileViewer, 1 );
	
	user_profile_viewer->service=NULL;
	user_profile_viewer->user_name=NULL;
	user_profile_viewer->loading=TRUE;
	
	GtkBuilder *ui=gtkbuilder_get_file(
						GtkBuilderUI,
							"profile_dialog", &user_profile_viewer->dialog,
							
							"title_vbox", &user_profile_viewer->title_vbox,
							
							"profile_vbox", &user_profile_viewer->profile_vbox,
							"user_image", &user_profile_viewer->user_image,
							
							"latest_update_vbox", &user_profile_viewer->latest_update_vbox,
						NULL
	);
	
	
	gtkbuilder_connect(
				ui, user_profile_viewer,
					"profile_dialog", "destroy", user_profile_viewer_destroy,
					"profile_dialog", "response", user_profile_viewer_response,
				NULL
	);
	
	user_profile_viewer->service_label=label_new();
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->title_vbox),
				GTK_WIDGET(user_profile_viewer->service_label),
				TRUE, TRUE, 0
	);
	
	user_profile_viewer->user_label=label_new();
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->title_vbox),
				GTK_WIDGET(user_profile_viewer->user_label),
				TRUE, TRUE, 0
	);
	
	user_profile_viewer->url_hyperlink=label_new();
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->profile_vbox),
				GTK_WIDGET(user_profile_viewer->url_hyperlink),
				TRUE, TRUE, 0
	);
	
	user_profile_viewer->bio_html=label_new();
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->profile_vbox),
				GTK_WIDGET(user_profile_viewer->bio_html),
				TRUE, TRUE, 0
	);
	
	user_profile_viewer->latest_update_label=(GtkLabel *)gtk_label_new(NULL);
	gtk_label_set_markup(user_profile_viewer->latest_update_label, "<b><u>Most recent update:</u></b>");
	g_object_set(user_profile_viewer->latest_update_label, "single-line-mode", TRUE, NULL);
	gtk_box_pack_start(
			GTK_BOX(user_profile_viewer->latest_update_vbox),
			GTK_WIDGET(user_profile_viewer->latest_update_label),
			TRUE, TRUE, 0
	);
	
	user_profile_viewer->most_recent_update=label_new();
	gtk_box_pack_start(
			GTK_BOX(user_profile_viewer->latest_update_vbox),
			GTK_WIDGET(user_profile_viewer->most_recent_update),
			TRUE, TRUE, 0
	);
	
	user_profile_viewer->updated_when_label=(GtkLabel *)gtk_label_new(NULL);
	g_object_set(user_profile_viewer->updated_when_label, "xalign", 1.0, "single-line-mode", TRUE, NULL);
	gtk_box_pack_start(
			GTK_BOX(user_profile_viewer->latest_update_vbox),
			GTK_WIDGET(user_profile_viewer->updated_when_label),
			TRUE, TRUE, 0
	);
	
	gtk_widget_show_all(GTK_WIDGET(user_profile_viewer->dialog));
}/*user_profile_viewer_setup();*/

static void user_profile_viewer_set_avatar(const gchar *image_file, const gchar *image_uri){
	if( !g_file_test(image_file, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR) && user_profile_viewer->loading )
		return user_profile_viewer_download_avatar(image_file, image_uri);
	
	if(user_profile_viewer->loading)
		user_profile_viewer_show_all();
	
	debug("Avatar: GtkImage created GtkImage for display in UserProfileViewer");
	debug("Avatar: Downloaded from <%s>", image_uri);
	debug("Avatar: saved to: [%s]", image_file);
	gtk_message_dialog_set_image( user_profile_viewer->dialog, GTK_WIDGET(user_profile_viewer->image=images_get_maximized_image_from_filename((gchar *)image_file)) );
}/*user_profile_viewer_set_avatar(const gchar *image_file, const gchar *image_uri);*/

static void user_profile_viewer_download_avatar(const gchar *image_file, const gchar *image_uri){
	debug("Downloading Image: %s.  GET: %s", image_file, image_uri);
	
	online_service_request_uri(user_profile_viewer->service, QUEUE, image_uri, 0, NULL, user_profile_viewer_save_avatar, g_strdup(image_uri), g_strdup(image_file) );
}/*user_profile_viewer_download_avatar(user->service, user);*/

static void *user_profile_viewer_save_avatar(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	const gchar *image_uri=online_service_wrapper_get_user_data(service_wrapper);
	const gchar *image_file=online_service_wrapper_get_form_data(service_wrapper);
	debug("Image download returned: %s(%d).", xml->reason_phrase, xml->status_code);
	
	gchar *error_message=NULL;
	if( parser_xml_error_check(service, image_uri, xml, &error_message) && g_file_set_contents(image_file, xml->response_body->data, xml->response_body->length, NULL) )
		user_profile_viewer_set_avatar(image_file, image_uri);
	
	uber_free(error_message);
	
	return NULL;
}/*user_profile_viewer_save_avatar(user);*/

static void user_profile_viewer_show_all(void){
	user_profile_viewer->loading=FALSE;
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->dialog));
	gtk_widget_show_all(GTK_WIDGET(user_profile_viewer->dialog));
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->service_label));
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->user_label));
	
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->user_image));
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->url_hyperlink));
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->bio_html));
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->latest_update_label));
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->most_recent_update));
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->updated_when_label));
	
	window_present(GTK_WINDOW(user_profile_viewer->dialog), TRUE);
}/*user_profile_viewer_show_all();*/

static void user_profile_viewer_hide_all(void){
	user_profile_viewer->loading=TRUE;
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->user_image));
	gtk_message_dialog_set_image( user_profile_viewer->dialog, GTK_WIDGET(images_get_dialog_image_from_stock("gtk-dialog-info") ) );
	gtk_image_set_from_stock(user_profile_viewer->user_image, "gtk-dialog-info", ImagesDialog );
	gtk_message_dialog_set_markup( user_profile_viewer->dialog, "" );
	
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->service_label));
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->user_label));
	
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->url_hyperlink));
	
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->latest_update_label));
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->most_recent_update));
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->updated_when_label));
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->bio_html));
	
	g_object_set(GTK_LABEL(user_profile_viewer->bio_html), "single-line-mode", TRUE, NULL );
	gchar *profile_details=g_strdup_printf( "<span weight=\"bold\">Please wait for @%s's <a href=\"http%s://%s/\">%s</a> profile to load,</span>", user_profile_viewer->user_name, (user_profile_viewer->service->https?"s":""), user_profile_viewer->service->uri, user_profile_viewer->service->uri );
	label_set_text(user_profile_viewer->service, user_profile_viewer->bio_html, 0.0, profile_details, FALSE, TRUE);
	uber_free(profile_details);
	
	user_profile_viewer->loading=TRUE;
}/*user_profile_viewer_hide_all();*/
/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
