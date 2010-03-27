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

#include "online-services.typedefs.h"
#include "online-services.types.h"
#include "online-service.types.h"

#include "online-service-request.h"
#include "online-service.h"
#include "online-services.h"
#include "online-service-wrapper.h"
#include "network.h"

#include "users.types.h"
#include "users-glists.h"
#include "users.h"
#include "cache.h"

#include "gconfig.h"
#include "preferences.defines.h"
#include "gtkbuilder.h"
#include "cache.h"

#include "www.h"

#include "ui-utils.h"
#include "main-window.h"

#include "uberchick-label.h"

#include "update-viewer.h"

#include "user-profile-viewer.h"


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
	
	UberChickLabel		*service_label;
	UberChickLabel		*user_label;
	
	GtkVBox			*profile_vbox;
	UberChickLabel		*url_hyperlink;
	UberChickLabel		*bio_html;
	
	GtkVBox			*latest_update_vbox;
	GtkLabel		*latest_update_label;
	GtkLabel		*updated_when_label;
	UberChickLabel		*most_recent_update;
	
	GtkToolbar		*users_tool_bar;
	GtkButton		*okay_button;
};


#define	DEBUG_DOMAINS	"UI:GtkBuilder:GtkBuildable:OnlineServices:Updates:About:Users:Profiles:UserProfileViewer:ViewProfile:user-profile-viewer.c"
#include "debug.h"

#define GTK_BUILDER_UI_FILENAME "user-profile-viewer"
static UserProfileViewer *user_profile_viewer=NULL;


/********************************************************************************
 *                    prototypes for private method & function                  *
 ********************************************************************************/
static void user_profile_viewer_response(GtkMessageDialog *message_dialog, gint response);
static void user_profile_viewer_destroy(GtkMessageDialog *message_dialog);

static void user_profile_viewer_follow(GtkToolButton *button, UserProfileViewer *user_profile_viewer);
static void user_profile_viewer_unfollow(GtkToolButton *button, UserProfileViewer *user_profile_viewer);
static void user_profile_viewer_block(GtkToolButton *button, UserProfileViewer *user_profile_viewer);

static void user_profile_viewer_view_unread_updates(GtkToolButton *button, UserProfileViewer *user_profile_viewer);
static void user_profile_viewer_view_recent_updates(GtkToolButton *button, UserProfileViewer *user_profile_viewer);

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
 *                       UserProfileViewer methods                              *
 ********************************************************************************/
static void user_profile_viewer_response(GtkMessageDialog *message_dialog, gint response){
	gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->dialog));
}/*user_profile_viewer_response(message_dialog, response, user_profile_viewer);*/

static void user_profile_viewer_destroy(GtkMessageDialog *message_dialog){
	debug("Destroying user profile viewer");
	user_profile_viewer->service=NULL;
	if(user_profile_viewer->user_name) uber_free(user_profile_viewer->user_name);
	if(user_profile_viewer->most_recent_update) gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->most_recent_update));
	if(user_profile_viewer->url_hyperlink) gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->url_hyperlink));
	if(user_profile_viewer->bio_html) gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->bio_html));
	if(user_profile_viewer->service_label) gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->service_label));
	if(user_profile_viewer->user_label) gtk_widget_destroy(GTK_WIDGET(user_profile_viewer->user_label));
	uber_free(user_profile_viewer);
}/*user_profile_viewer_destroy(dialog);*/

static void user_profile_viewer_follow(GtkToolButton *button, UserProfileViewer *user_profile_viewer){
	if(!(user_profile_viewer && user_profile_viewer->service && user_profile_viewer->user_name)) return;
	online_service_request_follow(user_profile_viewer->service, GTK_WINDOW(user_profile_viewer->dialog), user_profile_viewer->user_name);
}/*user_profile_viewer_follow(button, user_profile_viewer);*/

static void user_profile_viewer_unfollow(GtkToolButton *button, UserProfileViewer *user_profile_viewer){
	if(!(user_profile_viewer && user_profile_viewer->service && user_profile_viewer->user_name)) return;
	online_service_request_unfollow(user_profile_viewer->service, GTK_WINDOW(user_profile_viewer->dialog), user_profile_viewer->user_name);
}/*user_profile_viewer_unfolow( button, user_profile_viewer );*/

static void user_profile_viewer_block(GtkToolButton *button, UserProfileViewer *user_profile_viewer){
	if(!(user_profile_viewer && user_profile_viewer->service && user_profile_viewer->user_name)) return;
	online_service_request_block(user_profile_viewer->service, GTK_WINDOW(user_profile_viewer->dialog), user_profile_viewer->user_name);
}/*user_profile_viewer_block( button, user_profile_viewer );*/

static void user_profile_viewer_view_unread_updates(GtkToolButton *button, UserProfileViewer *user_profile_viewer){
	if(!(user_profile_viewer && user_profile_viewer->service && user_profile_viewer->user_name)) return;
	online_service_request_view_updates_new(user_profile_viewer->service, GTK_WINDOW(user_profile_viewer->dialog), user_profile_viewer->user_name);
}/*user_profile_viewer_view_unread_updates*/

static void user_profile_viewer_view_recent_updates(GtkToolButton *button, UserProfileViewer *user_profile_viewer){
	if(!(user_profile_viewer && user_profile_viewer->service && user_profile_viewer->user_name)) return;
	online_service_request_view_updates(user_profile_viewer->service, GTK_WINDOW(user_profile_viewer->dialog), user_profile_viewer->user_name);
}/*user_profile_viewer_view_recent_updates*/


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
	
	online_service_fetch_profile(service, user_name, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)user_profile_viewer_display_profile);
}/*user_profile_viewer_show( service, "uberChick", update_viewer->update_viewer );*/

static void user_profile_viewer_display_profile(OnlineServiceWrapper *online_service_wrapper, SoupMessage *xml, User *user){ 
	OnlineService *service=online_service_wrapper_get_online_service(online_service_wrapper);
	
	debug("Downloading user's avatar from: <%s> to [ file://%s ]", user->image_uri, user->image_file);
	user_profile_viewer_set_avatar(user->image_file, user->image_uri);
	
	gchar *profile_details=g_strdup_printf(
					"\t[<a href=\"http%s://%s/\">%s</a>]\n",
					(service->https?"s":""), service->uri, service->guid
	);
	
	uberchick_label_set_markup(user_profile_viewer->service_label, service, Users, user->user_name, user->id, user->status->id, profile_details, TRUE, TRUE);
	uber_free(profile_details);
	
	
	
	profile_details=g_strdup_printf(
					"\t\t<u><b>%s:</b></u> @%s",
					user->nick_name, user->user_name
	);
	uberchick_label_set_markup(user_profile_viewer->user_label, service, Users, user->user_name, user->id, user->status->id, profile_details, TRUE, TRUE);
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
	
	profile_details=g_strdup_printf( "\t<b>URL:</b>\t<a href=\"%s\">%s</a>\n", user->uri, user->uri );
	uberchick_label_set_markup(user_profile_viewer->url_hyperlink, service, Users, user->user_name, user->id, user->status->id, profile_details, TRUE, TRUE);
	uber_free(profile_details);
	
	g_object_set(GTK_LABEL(user_profile_viewer->bio_html), "single-line-mode", FALSE, NULL );
	profile_details=g_strdup_printf( "\t<b>Bio:</b>\n\t\t%s\n", user->bio );
	uberchick_label_set_markup(user_profile_viewer->bio_html, service, Users, user->user_name, user->id, user->status->id, profile_details, TRUE, TRUE);
	uber_free(profile_details);
	
	profile_details=g_markup_printf_escaped("<b>Last updated:</b> <i>[%s]</i>", user->status->created_how_long_ago);
	gtk_label_set_markup(user_profile_viewer->updated_when_label, profile_details);
	uber_free(profile_details);
	
	uberchick_label_set_markup(user_profile_viewer->most_recent_update, service, Users, user->user_name, user->id, user->status->id, user->status->sexy_update, TRUE, TRUE);
	
	if(!user_profile_viewer->loading)
		user_profile_viewer_show_all();
	
	user_free(user);
}/*user_profile_viewer_display_profile(online_service_wrapper, xml, user);*/

static void user_profile_viewer_setup(void){
	user_profile_viewer=g_new( UserProfileViewer, 1 );
	
	user_profile_viewer->loading=TRUE;
	user_profile_viewer->service=NULL;
	user_profile_viewer->user_name=NULL;
	
	GtkBuilder *ui=gtkbuilder_get_file(
						GTK_BUILDER_UI_FILENAME,
							"user-profile-viewer-dialog", &user_profile_viewer->dialog,
							
							"title_vbox", &user_profile_viewer->title_vbox,
							
							"profile_vbox", &user_profile_viewer->profile_vbox,
							"user_image", &user_profile_viewer->user_image,
							
							"latest_update_vbox", &user_profile_viewer->latest_update_vbox,
							"users_tool_bar", &user_profile_viewer->users_tool_bar,
							
							"okay_button", &user_profile_viewer->okay_button,
						NULL
	);
	
	
	gtkbuilder_connect(
				ui, user_profile_viewer,
					"user-profile-viewer-dialog", "destroy", user_profile_viewer_destroy,
					"user-profile-viewer-dialog", "response", user_profile_viewer_response,
					
					"user_follow_tool_button", "clicked", user_profile_viewer_follow,
					"user_unfollow_tool_button", "clicked", user_profile_viewer_unfollow,
					"user_block_tool_button", "clicked", user_profile_viewer_block,
					"user_view_unread_updates_tool_button", "clicked", user_profile_viewer_view_unread_updates,
					"user_view_recent_updates_tool_button", "clicked", user_profile_viewer_view_recent_updates,
				NULL
	);
	
	if(!gtk_widget_has_focus(GTK_WIDGET(user_profile_viewer->okay_button)))
		gtk_widget_grab_focus(GTK_WIDGET(user_profile_viewer->okay_button));
	
	user_profile_viewer->service_label=uberchick_label_new();
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->title_vbox),
				GTK_WIDGET(user_profile_viewer->service_label),
				TRUE, TRUE, 0
	);
	
	user_profile_viewer->user_label=uberchick_label_new();
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->title_vbox),
				GTK_WIDGET(user_profile_viewer->user_label),
				TRUE, TRUE, 0
	);
	
	user_profile_viewer->url_hyperlink=uberchick_label_new();
	gtk_box_pack_start(
				GTK_BOX(user_profile_viewer->profile_vbox),
				GTK_WIDGET(user_profile_viewer->url_hyperlink),
				TRUE, TRUE, 0
	);
	
	user_profile_viewer->bio_html=uberchick_label_new();
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
	
	user_profile_viewer->most_recent_update=uberchick_label_new();
	g_object_set(user_profile_viewer->most_recent_update, "xalign", 0.15, NULL);
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
	
	debug("Avatar: GtkImage created GtkImage for display in UserProfileViewer");
	debug("Avatar: Downloaded from <%s>", image_uri);
	debug("Avatar: saved to: [%s]", image_file);
	gtk_message_dialog_set_image( user_profile_viewer->dialog, GTK_WIDGET(user_profile_viewer->image=images_get_maximized_image_from_filename((gchar *)image_file)) );
	
	if(user_profile_viewer->loading)
		user_profile_viewer_show_all();
}/*user_profile_viewer_set_avatar(image_file, image_uri);*/

static void user_profile_viewer_download_avatar(const gchar *image_file, const gchar *image_uri){
	debug("Downloading Image: %s.  GET: %s", image_file, image_uri);
	
	online_service_request_uri(user_profile_viewer->service, QUEUE, image_uri, 0, NULL, user_profile_viewer_save_avatar, g_strdup(image_file), NULL );
}/*user_profile_viewer_download_avatar(image_file, image_uri);*/

static void *user_profile_viewer_save_avatar(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	const gchar *image_uri=online_service_wrapper_get_requested_uri(service_wrapper);
	const gchar *image_file=online_service_wrapper_get_user_data(service_wrapper);
	debug("Image download returned: %s(%d)", xml->reason_phrase, xml->status_code);
	
	gchar *image_filename=NULL;
	if(images_save_image(service, xml, image_uri, image_file, &image_filename))
		main_window_statusbar_printf("Avatar added to UserProfileViewer.");
	
	user_profile_viewer_set_avatar(image_filename, image_uri);
	
	uber_free(image_filename);
	
	return NULL;
}/*user_profile_viewer_save_avatar(session, xml, online_service_wrappep);*/

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

	gtk_widget_show(GTK_WIDGET(user_profile_viewer->users_tool_bar));
	
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
	
	gtk_widget_hide(GTK_WIDGET(user_profile_viewer->users_tool_bar));
	
	gtk_widget_show(GTK_WIDGET(user_profile_viewer->bio_html));
	
	g_object_set(GTK_LABEL(user_profile_viewer->bio_html), "single-line-mode", TRUE, NULL );
	gchar *profile_details=g_strdup_printf( "<span weight=\"bold\">Please wait for @%s's <a href=\"http%s://%s/\">%s</a> profile to load,</span>", user_profile_viewer->user_name, (user_profile_viewer->service->https?"s":""), user_profile_viewer->service->uri, user_profile_viewer->service->uri );
	uberchick_label_set_markup(user_profile_viewer->bio_html, user_profile_viewer->service, None, NULL, 0.0, 0.0, profile_details, FALSE, TRUE);
	uber_free(profile_details);
	
	user_profile_viewer->loading=TRUE;
}/*user_profile_viewer_hide_all();*/
/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
