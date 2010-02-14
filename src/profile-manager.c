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
#define _GNU_SOURCE
#define _THREAD_SAFE

#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include "program.h"

#include "ui-utils.h"
#include "gtkbuilder.h"

#include "online-services.rest-uris.defines.h"
#include "online-service.typedefs.h"
#include "online-service.h"
#include "online-service-request.h"
#include "online-service-wrapper.h"

#include "cache.h"
#include "images.h"

#include "users.types.h"
#include "users.h"

#include "main-window.h"
#include "profile-manager.h"


/********************************************************************************
 *      Project, system, & library headers.  eg #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/


/********************************************************************************
 *        typedefs: methods, macros, constants, objects, structs, and enums     *
 ********************************************************************************/
typedef struct _ProfileManager ProfileManager;
typedef enum _ProfileManagerImageWidget ProfileManagerImageWidget;

enum _ProfileManagerImageWidget{
	Avatar,
	ProfileBackground,
};

struct _ProfileManager{
	gboolean		loading;
	
	OnlineService		*service;
	User			*user;
	gboolean		profile_loaded;
	
	GtkDialog		*dialog;
	
	GtkAspectFrame		*aspect_frame;
	
	GtkLabel		*label;
	GtkToolButton		*reset_tool_button;
	
	gboolean		avatar_image_loaded;
	GtkImage		*user_avatar_image;
	GtkFileChooserButton	*user_avatar_file_chooser_button;
	
	GtkEntry		*nick_name_entry;
	GtkEntry		*uri_entry;
	GtkEntry		*location_entry;
	GtkEntry		*bio_entry;
	
	GtkColorButton		*profile_colors_background_color_button;
	GtkColorButton		*profile_colors_text_color_button;
	GtkColorButton		*profile_colors_link_color_button;
	GtkColorButton		*profile_colors_sidebar_fill_color_button;
	GtkColorButton		*profile_colors_sidebar_border_color_button;
	
	gboolean		profile_background_image_loaded;
	GtkImage		*profile_background_image;
	GtkFileChooserButton	*profile_background_image_file_chooser_button;
	GtkCheckButton		*background_title_check_button;
	
	GtkFileFilter		*image_file_filter;
	
	GtkButton		*save_button;
	GtkButton		*cancel_button;
};

/********************************************************************************
 *                prototypes for private methods & functions                    *
 ********************************************************************************/
static void profile_manager_setup(GtkWindow *parent);
static void profile_manager_setup_image_file_filters(ProfileManager *profile_manager);
static void profile_manager_check_loading_status(ProfileManager *profile_manager);


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static void profile_manager_set_loading_status(ProfileManager *profile_manager, gboolean loading_status);
static void profile_manager_reset(GtkToolButton *reset_tool_button, ProfileManager *profile_manager);

static void profile_manager_edit_profile(OnlineServiceWrapper *online_service_wrapper, SoupMessage *xml, User *user);
static void profile_manager_download_image(ProfileManagerImageWidget which_image, const gchar *image_file, const gchar *image_uri);
static void profile_manager_set_image(ProfileManagerImageWidget which_image, const gchar *image_file, const gchar *image_uri);

static void profile_manager_save(GtkButton *save_button, ProfileManager *profile_manager);
static gboolean profile_manager_update_profile(ProfileManager *profile_manager);

static void profile_manager_destroy(GtkDialog *dialog, ProfileManager *profile_manager);
static void profile_manager_response(GtkDialog *dialog, gint response, ProfileManager *profile_manager);

/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
static ProfileManager *profile_manager=NULL;

#define	DEBUG_DOMAINS	"ProfileManager:UI:GtkBuilder:GtkBuildable:OnlineServices:Profiles:Networking:Requests:Updates:Users:Setup:profile-manager.c"
#include "debug.h"

#define GTK_BUILDER_UI_FILENAME "profile-manager"


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
void profile_manager_show(GtkWindow *parent){
	online_service_request_popup_select_service();
	
	if(!selected_service) return;
	
	if(profile_manager && profile_manager->dialog && profile_manager->service!=selected_service)
		gtk_widget_destroy(GTK_WIDGET(profile_manager->dialog));
	
	if(!(selected_service && profile_manager && profile_manager->dialog && profile_manager->service && profile_manager->service==selected_service))
		profile_manager_setup(parent);
	
	window_present(GTK_WINDOW(profile_manager->dialog), TRUE);
	online_service_fetch_profile(selected_service, selected_service->user_name, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)profile_manager_edit_profile);
	debug("Downloading user's avatar from: <%s> to [ file://%s ]", selected_service->user_profile->image_uri, selected_service->user_profile->image_file);
	profile_manager_set_image(Avatar, selected_service->user_profile->image_file, selected_service->user_profile->image_uri);
	
	debug("Downloading user's profile background image from: <%s> to [ file://%s ]", selected_service->user_profile->profile_background_uri, selected_service->user_profile->profile_background_file);
	profile_manager_set_image(ProfileBackground, selected_service->user_profile->profile_background_file, selected_service->user_profile->profile_background_uri);
	
}/*profile_manager_show(main_window->private->window);*/

static void profile_manager_setup(GtkWindow *parent){
	profile_manager=g_new0(ProfileManager, 1);
	
	profile_manager->service=selected_service;
	profile_manager->user=NULL;
	profile_manager->image_file_filter=NULL;
	
	/* Get widgets */
	debug("Initialising, building ui, loading, & displaying ProfileManager from: %s", GTK_BUILDER_UI_FILENAME);
	GtkBuilder *ui=gtkbuilder_get_file(
				GTK_BUILDER_UI_FILENAME,
					"profile_manager_dialog", &profile_manager->dialog,
					
					"profile_manager_aspect_frame", &profile_manager->aspect_frame,
					
					"profile_manager_label", &profile_manager->label,
					"profile_manager_toolbar_reset_tool_button", &profile_manager->reset_tool_button,
					
					"profile_manager_user_avatar_image", &profile_manager->user_avatar_image,
					"profile_manager_user_avatar_file_chooser_button", &profile_manager->user_avatar_file_chooser_button,
					
					"profile_manager_user_profile_nickname_entry", &profile_manager->nick_name_entry,
					"profile_manager_user_profile_uri_entry", &profile_manager->uri_entry,
					"profile_manager_user_profile_location_entry", &profile_manager->location_entry,
					"profile_manager_user_profile_bio_entry", &profile_manager->bio_entry,
					
					"profile_manager_profile_colors_background_color_button", &profile_manager->profile_colors_background_color_button,
					"profile_manager_profile_colors_text_color_button", &profile_manager->profile_colors_text_color_button,
					"profile_manager_profile_colors_link_color_button", &profile_manager->profile_colors_link_color_button,
					"profile_manager_profile_colors_sidebar_fill_color_button", &profile_manager->profile_colors_sidebar_fill_color_button,
					"profile_manager_profile_colors_sidebar_border_color_button", &profile_manager->profile_colors_sidebar_border_color_button,
					
					"profile_manager_profile_background_image", &profile_manager->profile_background_image,
					"profile_manager_profile_background_image_file_chooser_button", &profile_manager->profile_background_image_file_chooser_button,
					"profile_manager_background_title_check_button", &profile_manager->background_title_check_button,
					
					"profile_manager_save_button", &profile_manager->save_button,
					"profile_manager_cancel_button", &profile_manager->cancel_button,
				NULL
	);
	
	profile_manager_setup_image_file_filters(profile_manager);
	
	debug("ProfileManager created now connecting its signal handlers.");
	gtkbuilder_connect(
				ui, profile_manager,
					"profile_manager_dialog", "destroy", profile_manager_destroy,
					"profile_manager_dialog", "response", profile_manager_response,
					
					"profile_manager_toolbar_reset_tool_button", "clicked", profile_manager_reset,
					
					"profile_manager_save_button", "clicked", profile_manager_save,
				NULL
	);
	
	g_object_unref(ui);
	
	gchar *window_title=g_strdup_printf("%s - <%s>'s %s", _(GETTEXT_PACKAGE), selected_service->key, _("Profile Manager"));
	gtk_window_set_title(GTK_WINDOW(profile_manager->dialog), window_title);
	uber_free(window_title);
	
	g_object_add_weak_pointer(G_OBJECT(profile_manager->dialog), (gpointer) &profile_manager);
	gtk_window_set_transient_for(GTK_WINDOW(profile_manager->dialog), parent);
	
	/* Now that we're done setting up, let's show the widget */
	window_present(GTK_WINDOW(profile_manager->dialog), TRUE);
	gtk_widget_show_all(GTK_WIDGET(profile_manager->dialog));
	
	profile_manager_set_loading_status(profile_manager, TRUE);
	
	gchar *loading_markup_string=g_strdup_printf("<b>Please wait while your: &lt;%s&gt; profile loads</b>", selected_service->key);
	gtk_label_set_markup(profile_manager->label, loading_markup_string);
	uber_free(loading_markup_string);
	
	statusbar_printf(_("Please wait while your profile is loaded.  This will take a few moments..."));
	debug("Loading user profile");
}/*profile_manager_setup(parent);*/

static void profile_manager_setup_image_file_filters(ProfileManager *profile_manager){
	profile_manager->image_file_filter=gtk_file_filter_new();
	gtk_file_filter_add_pixbuf_formats(profile_manager->image_file_filter);
	gtk_file_filter_set_name(profile_manager->image_file_filter, "Images, Graphic Designs, and Photo files");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(profile_manager->user_avatar_file_chooser_button), profile_manager->image_file_filter);
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(profile_manager->profile_background_image_file_chooser_button), profile_manager->image_file_filter);
}/*profile_manager_setup_image_file_filters(profile_manager);*/


/* signal handlers & callback methods. */
static void profile_manager_edit_profile(OnlineServiceWrapper *online_service_wrapper, SoupMessage *xml, User *user){
	OnlineService *service=online_service_wrapper_get_online_service(online_service_wrapper);
	
	gchar *loading_markup_string=g_strdup_printf("<b>&lt;%s&gt;'s profile:</b>", service->key);
	gtk_label_set_markup(profile_manager->label, loading_markup_string);
	uber_free(loading_markup_string);
	
	profile_manager->user=user;
	
	profile_manager_reset(profile_manager->reset_tool_button, profile_manager);
	
	profile_manager->profile_loaded=TRUE;
	
	profile_manager_check_loading_status(profile_manager);
}/*profile_manager_edit_profile(online_service_wrapper, xml, user);*/

static void profile_manager_reset(GtkToolButton *reset_tool_button, ProfileManager *profile_manager){
	gtk_entry_set_text(profile_manager->nick_name_entry, profile_manager->user->nick_name);
	gtk_entry_set_text(profile_manager->uri_entry, profile_manager->user->uri);
	gtk_entry_set_text(profile_manager->location_entry, profile_manager->user->location);
	gtk_entry_set_text(profile_manager->bio_entry, profile_manager->user->bio);
	
	GdkColormap *colormap=gdk_colormap_get_system();
	GdkColor *color=g_new0(GdkColor, 1);
	gdk_colormap_alloc_color(colormap, color, FALSE, TRUE);
	
	if(G_STR_N_EMPTY(profile_manager->user->profile_background_color) && gdk_color_parse(profile_manager->user->profile_background_color, color))
		gtk_color_button_set_color(profile_manager->profile_colors_background_color_button, color);
	
	if(G_STR_N_EMPTY(profile_manager->user->profile_text_color) && gdk_color_parse(profile_manager->user->profile_text_color, color))
		gtk_color_button_set_color(profile_manager->profile_colors_text_color_button, color);
	
	if(G_STR_N_EMPTY(profile_manager->user->profile_link_color) && gdk_color_parse(profile_manager->user->profile_link_color, color))
		gtk_color_button_set_color(profile_manager->profile_colors_link_color_button, color);
	
	if(G_STR_N_EMPTY(profile_manager->user->profile_sidebar_fill_color) && gdk_color_parse(profile_manager->user->profile_sidebar_fill_color, color))
		gtk_color_button_set_color(profile_manager->profile_colors_sidebar_fill_color_button, color);
	
	if(G_STR_N_EMPTY(profile_manager->user->profile_sidebar_border_color) && gdk_color_parse(profile_manager->user->profile_sidebar_border_color, color))
		gtk_color_button_set_color(profile_manager->profile_colors_sidebar_border_color_button, color);
	
	uber_free(color);
	g_object_unref(colormap);
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(profile_manager->background_title_check_button), profile_manager->user->profile_background_tile);
}/*profile_manager_reset(profile_manager->reset_tool_button, profile_manager);*/

static void profile_manager_set_image(ProfileManagerImageWidget which_image, const gchar *image_file, const gchar *image_uri){
	if(!g_file_test(image_file, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR))
		return profile_manager_download_image(which_image, image_file, image_uri);
	
	debug("Image: GtkImage created GtkImage for display in ProfileManager");
	debug("Image: Downloaded from <%s>", image_uri);
	debug("Image: saved to: [%s]", image_file);
	GdkPixbuf *pixbuf=NULL;
	
	if((pixbuf=images_get_maximized_pixbuf_from_filename((gchar *)image_file))){
		GtkImage *profile_manager_image=NULL;
		switch(which_image){
			case ProfileBackground:
				profile_manager->profile_background_image_loaded=TRUE;
				profile_manager_image=profile_manager->profile_background_image;
				break;
			case Avatar:
			default:
				profile_manager->avatar_image_loaded=TRUE;
				profile_manager_image=profile_manager->user_avatar_image;
				break;
		}
		
		gtk_image_set_from_pixbuf(profile_manager_image, pixbuf);
		g_object_unref(pixbuf);
	}
	
	profile_manager_check_loading_status(profile_manager);
}/*profile_manager_set_image(Avatar|ProfileBackground, image_file, image_uri);*/

static void profile_manager_check_loading_status(ProfileManager *profile_manager){
	if(!(profile_manager->profile_loaded && profile_manager->avatar_image_loaded && profile_manager->profile_background_image_loaded))
		return;
	
	profile_manager_set_loading_status(profile_manager, FALSE);
	window_present(GTK_WINDOW(profile_manager->dialog), TRUE);
}/*profile_manager_check_loading_status(profile_manager);*/

static void profile_manager_download_image(ProfileManagerImageWidget which_image, const gchar *image_file, const gchar *image_uri){
	debug("Downloading Image: %s.  GET: %s", image_file, image_uri);
	
	online_service_request_uri(profile_manager->service, QUEUE, image_uri, 0, NULL, profile_manager_save_image, g_strdup(image_file), GINT_TO_POINTER(which_image) );
}/*profile_manager_download_image(Avatar|ProfileBackground, image_file, image_uri);*/

void *profile_manager_save_image(SoupSession *session, SoupMessage *xml, OnlineServiceWrapper *service_wrapper){
	OnlineService *service=online_service_wrapper_get_online_service(service_wrapper);
	const gchar *image_uri=online_service_wrapper_get_requested_uri(service_wrapper);
	const gchar *image_file=online_service_wrapper_get_user_data(service_wrapper);
	ProfileManagerImageWidget which_image=GPOINTER_TO_INT(online_service_wrapper_get_form_data(service_wrapper));
	debug("Image download returned: %s(%d).", xml->reason_phrase, xml->status_code);
	
	gchar *image_filename=NULL;
	if(!images_save_image(service, xml, image_uri, image_file, &image_filename))
		main_window_statusbar_printf("There was an error downloading an image for your ProfileManager.");
	else
		main_window_statusbar_printf("Image added to ProfileManager.");
	
	profile_manager_set_image(which_image, image_filename, image_uri);
	
	uber_free(image_filename);
	
	return NULL;
}/*profile_manager_save_image(service->session, xml, online_service_wrapper);*/

static void profile_manager_save(GtkButton *save_button, ProfileManager *profile_manager){
	if(
		profile_manager_update_profile(profile_manager)
	  )
		profile_manager_set_loading_status(profile_manager, TRUE);
	
}/*profile_manager_save(profile_manager->save_button, profile_manager);*/

static gboolean profile_manager_update_profile(ProfileManager *profile_manager){
	if( g_str_equal(profile_manager->user->nick_name, profile_manager->nick_name_entry->text) && g_str_equal(profile_manager->user->uri, profile_manager->uri_entry->text) && g_str_equal(profile_manager->user->location, profile_manager->location_entry->text) && g_str_equal(profile_manager->user->bio, profile_manager->bio_entry->text) )
		return FALSE;
	
	gchar *form_data=g_strdup_printf("name=%s&url=%s&location=%s&description=%s", g_uri_escape_string(profile_manager->nick_name_entry->text, NULL, TRUE), g_uri_escape_string(profile_manager->uri_entry->text, NULL, TRUE), g_uri_escape_string(profile_manager->location_entry->text, NULL, TRUE), g_uri_escape_string(profile_manager->bio_entry->text, NULL, TRUE));
	online_service_request(profile_manager->service, POST, API_ACCOUNT_UPDATE_PROFILE, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)profile_manager_edit_profile, (OnlineServiceSoupSessionCallbackFunc)user_parse_profile, NULL, form_data);
	uber_free(form_data);
	return TRUE;
}/*profile_manager_update_profile(profile_manager);*/

static void profile_manager_set_loading_status(ProfileManager *profile_manager, gboolean loading_status){
	profile_manager->loading=loading_status;
	
	loading_status=!loading_status;
	
	profile_manager->profile_loaded=loading_status;
	profile_manager->profile_background_image_loaded=loading_status;
	profile_manager->avatar_image_loaded=loading_status;
	
	gtk_widget_set_sensitive(GTK_WIDGET(profile_manager->aspect_frame), loading_status);
}/*profile_manager_set_loading_status(profile_manager, TRUE|FALSE);*/

static void profile_manager_response(GtkDialog *dialog, gint response, ProfileManager *profile_manager){
	switch(response){
		case GTK_RESPONSE_APPLY:
			profile_manager_save(profile_manager->save_button, profile_manager);
			break;
		default:
			gtk_widget_destroy(GTK_WIDGET(dialog));
			break;
	}
}/*profile_message_response(profile_manager->dialog, GTK_RESPONSE_APPLY|GTK_RESPONSE_CANEL, profile_manager);*/

static void profile_manager_destroy(GtkDialog *dialog, ProfileManager *profile_manager){
	debug("Destroying ProfileManager.");
	gtk_widget_destroy(GTK_WIDGET(dialog));
	
	if(profile_manager->user){
		debug("Releasing ProfileManager's <%s> User profile.", profile_manager->user->service->key);
		user_free(profile_manager->user);
	}
	
	uber_free(profile_manager);
}/*profile_manager_destroy*/

/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
