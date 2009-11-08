/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright(c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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
 * License("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
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
 * user-visible display as defined in Section 6.4(d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright(C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright(C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright(C) of the User the data is connected to.
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
#include <glib.h>
#include <gtk/gtk.h>
#include <libsexy/sexy.h>

#include "config.h"
#include "program.h"

#include "images.h"

#include "gconfig.h"

#include "online-services-typedefs.h"
#include "online-services.types.h"
#include "online-service.types.h"
#include "online-services.h"

#include "online-service.h"
#include "online-service-request.h"

#include "users.types.h"
#include "users.h"

#include "network.h"
#include "users-glists.h"
#include "gtkbuilder.h"
#include "hotkeys.h"

#include "main-window.h"
#include "geometry.h"
#include "preferences.h"

#include "www.h"
#include "uberchick-label.h"

#include "update-viewer.h"

struct _UpdateViewer{ 
	/* Used to trigger resizing. */
	guint			size_timeout_id;
	
	/* UpdateViewer Dialog & embeding Widgets */
	GtkWindow		*update_viewer;
	GtkHBox			*update_viewer_embed;
	
	/* Linked lists used for changing the appearance of the UpdateViewer. */
	/* Widgets which are only seen when the user's enabled the compact UpdateViewer by default.
	 * Or whenever a update is selected. */
	GList			*compact_update_viewer_hidden_containers;
	/* Widgets that are enabled when we a update is selected */
	GList			*selected_update_buttons;
	
	/* GtkWidgets for viewing user details & for controlling one's relationship to them. */
	GtkHBox			*user_vbox;
	
	GtkVBox			*user_updates_control_hbox;
	GtkImage		*user_image;
	
	/* Buttons for viewing details about the auther of the current selected/extended Tweet. */
	GtkToggleButton		*best_friend_toggle_button;
	GtkButton		*view_user_profile_button;
	
	/* Buttons for viewing updates, new & all, update from the selected updates author or the selected update's retweets. */
	GtkButton		*view_user_unread_updates_button;
	GtkButton		*view_user_updates_button;
	GtkButton		*view_forwards_button;
	
	/* Buttons for viewing details about the user of the current selected/extended Tweet. */
	GtkVBox			*user_relationship_controls_hbox;
	GtkButton		*user_follow_button;
	GtkButton		*user_unfollow_button;
	GtkButton		*user_block_button;
	
	/* Just for padding & appearance. */
	GtkVSeparator		*left_vseparator;
	
	/* View & Sending Tweets. */
	GtkVBox			*status_vbox;
	
	/* UpdateViewer */
	GtkVBox			*status_view_vbox;
	UberChickLabel		*sexy_to;
	UberChickLabel		*sexy_from;
	GtkLabel		*update_datetime_label;
	UberChickLabel		*sexy_update;
	
	/* Tweet, status, & DM writing area & widgets. */
	GtkAspectFrame		*view_controls_aspect_frame;
	GtkHBox			*view_controls_hbox;
	GtkToggleButton		*compact_view_toggle_button;
	GtkImage		*compact_view_image;
	
	/* Togglebutton & image used to indicate & switch between embed update-viewer & floating. */
	GtkToggleButton		*embed_toggle_button;
	GtkImage		*embed_image;
	
	/* Stuff for actually writing your updates & DMs. */
	GtkVBox			*update_composition_vbox;
	GtkHBox			*notification_labels_hbox;
	GtkLabel		*best_friend_dm_notification_label;
	GtkLabel		*char_count;
	
	GtkHBox			*update_composition_hbox;
	
	gint			max_updates;
	gint			total_updates;
	GtkComboBoxEntry	*sexy_entry_combo_box_entry;
	GtkListStore		*previous_updates_list_store;
	GtkTreeModel		*previous_updates_tree_model;
	SexySpellEntry		*sexy_entry;
	GtkEntryCompletion	*sexy_completion;
	gint			sexy_position;
	
	/* Info on the update being viewed to avoid issues with the 'best friends' toggle button. */
	OnlineService		*viewing_service;
	gchar			*viewing_user;
	gdouble			viewing_update_id;
	gchar			*viewing_update_id_str;
	
	/* For sending one of your best friends a dm. */
	OnlineService		*best_friends_service;
	gchar			*best_friends_user_name;
	
	GtkButton		*sexy_send;
	GtkButton		*sexy_dm_button;
	GtkVSeparator		*sexy_vseparator;
	GtkButton		*new_update_button;
	
	/* The list & actual GtkWidgets for sending DMs. */
	GtkFrame		*dm_frame;
	GtkLabel		*dm_frame_label;
	GtkHBox			*dm_form_hbox;
	GtkButton		*dm_refresh;
	GtkComboBox		*followers_combo_box;
	GtkListStore		*followers_list_store;
	GtkTreeModel		*followers_model;
	GtkButton		*followers_send_dm;
	
	GtkToggleButton		*dm_form_active_togglebutton;
	GtkImage		*dm_form_active_image;
	
	/* Just for padding & appearance. */
	GtkVSeparator		*right_vseparator;
	
	/* Container for UpdateViewer's right side widgets. */
	GtkVBox			*controls_vbox;
	
	GtkHBox			*status_control_hbox;
	/* Buttons for stuff to do with the current selected & extended update. */
	GtkButton		*reply_button;
	GtkButton		*forward_update_button;
	GtkButton		*make_fave_button;
};


typedef enum{
	USER_LABEL,
	USER_POINTER,
} FriendsDMColumns;

typedef enum{
	GSTRING_UPDATE = 0,
	IN_REPLY_TO_SERVICE = 1,
	IN_REPLY_TO_STATUS_ID = 2,
} PeviousUpdateInfo;


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define UPDATE_VIEWER_PREVIOUS_UPDATES_STRING			GCONF_PATH "/update_viewer/previous_updates/%d"

#define	DEBUG_DOMAINS	"UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Tweets:Requests:Users:Start-Up:update-viewer.c"
#include "debug.h"

#define GtkBuilderUI "update-viewer"
#define	TWEET_MAX_CHARS	140

static UpdateViewer *update_viewer=NULL;


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static void update_viewer_destroy_cb(GtkWidget *window, UpdateViewer *update_viewer); 
static gboolean update_viewer_delete_event_cb(GtkWidget *window, GdkEvent *event, UpdateViewer *update_viewer);
static gboolean update_viewer_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, UpdateViewer *update_viewer);
static gboolean update_viewer_configure_event_timeout_cb(GtkWidget *widget);

static void update_viewer_sexy_init(void);
static void update_viewer_reorder(void);

static void update_viewer_selected_update_author_best_friend_toggled(GtkToggleButton *best_friend_toggle_button);

static void update_viewer_selected_update_buttons_setup(GtkBuilder *ui);
static void update_viewer_bind_hotkeys(GtkBuilder *ui);
static void update_viewer_selected_update_buttons_show(gboolean selected_update, const gchar *user_name);

static void update_viewer_grab_widgets_compact_update_viewer_hidden(GtkBuilder *ui);
static void update_viewer_compact_view_display(gboolean compact);
static void update_viewer_scale(gboolean compact);

static gint update_viewer_sexy_entry_update_length(gchar *update);
static void update_viewer_sexy_entry_character_count(GtkEntry *entry, GdkEventKey *event, GtkLabel *update_character_counter);

static void update_viewer_sexy_send(OnlineService *service, const gchar *user_name);

static void update_viewer_previous_updates_load(UpdateViewer *update_viewer);
static void update_viewer_previous_updates_prepend(const gchar *update, UpdateViewer *update_viewer);
static gboolean update_viewer_previous_updates_check_unique(const gchar *update, UpdateViewer *update_viewer);
static void update_viewer_previous_updates_remove(gint list_store_index, UpdateViewer *update_viewer);
static void update_viewer_previous_updates_rotate(UpdateViewer *update_viewer);
static void update_viewer_previous_updates_select(GtkComboBoxEntry *sexy_entry_combo_box_entry, UpdateViewer *update_viewer);
static void update_viewer_previous_updates_free(UpdateViewer *update_viewer);

static void update_viewer_reply_or_forward(GtkButton *update_button);

static void update_viewer_dm_show(GtkToggleButton *toggle_button);
static void update_viewer_dm_form_activate(gboolean dm_activate);
static void update_viewer_dm_refresh(void);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
GtkHBox *update_viewer_get_embed(void){
	return update_viewer->update_viewer_embed;
}/*update_viewer_get_embed();*/

static void update_viewer_destroy_cb(GtkWidget *window, UpdateViewer *update_viewer){
	debug("Destroying UpdateViewer & freeing resources");
	update_viewer_previous_updates_free(update_viewer);
	
	if(update_viewer->viewing_user) uber_free(update_viewer->viewing_user);
	if(update_viewer->best_friends_user_name) uber_free(update_viewer->best_friends_user_name);
	if(update_viewer->viewing_update_id_str) uber_free(update_viewer->viewing_update_id_str);
	if(update_viewer->viewing_update_id) update_viewer->viewing_update_id=0.0;
	
	if(update_viewer->sexy_to) g_object_unref(update_viewer->sexy_to);
	if(update_viewer->sexy_from) g_object_unref(update_viewer->sexy_from);
	if(update_viewer->sexy_update) g_object_unref(update_viewer->sexy_update);
	
	gtk_widget_destroy( GTK_WIDGET(update_viewer->update_viewer) );
	uber_free(update_viewer);
}/*update_viewer_destroy_cb*/

static gboolean update_viewer_delete_event_cb(GtkWidget *window, GdkEvent *event, UpdateViewer *update_viewer){
	gtk_toggle_button_set_active(update_viewer->embed_toggle_button, FALSE);
	
	return TRUE;
}/*update_viewer_delete_event_cb*/

static gboolean update_viewer_configure_event_timeout_cb(GtkWidget *widget){
	if(!gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE)){
		update_viewer->size_timeout_id=0;
		return FALSE;
	}
	geometry_save();
	return FALSE;
}

static gboolean update_viewer_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, UpdateViewer *update_viewer){
	if(!gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE)) return FALSE;
	program_timeout_remove(&update_viewer->size_timeout_id, _("main window configuration"));
	update_viewer->size_timeout_id=g_timeout_add_seconds(1, (GSourceFunc)update_viewer_configure_event_timeout_cb, widget );
	return FALSE;
}

UpdateViewer *update_viewer_new(GtkWindow *parent){
	GtkBuilder	*ui;
	
	update_viewer=g_new0(UpdateViewer, 1);
	debug("Building UpdateViewer");
	update_viewer->sexy_position=-1;
	ui=gtkbuilder_get_file(
				GtkBuilderUI,
					"update_viewer", &update_viewer->update_viewer,
					"update_viewer_embed", &update_viewer->update_viewer_embed,
					
					"user_vbox", &update_viewer->user_vbox,
					"user_image", &update_viewer->user_image,
					
					"best_friend_toggle_button", &update_viewer->best_friend_toggle_button,
					"view_user_profile_button", &update_viewer->view_user_profile_button,
					
					"view_user_unread_updates_button", &update_viewer->view_user_unread_updates_button,
					"view_user_updates_button", &update_viewer->view_user_updates_button,
					"selected_update_view_forwards_button", &update_viewer->view_forwards_button,
					
					"user_follow_button", &update_viewer->user_follow_button,
					"user_unfollow_button", &update_viewer->user_unfollow_button,
					"user_block_button", &update_viewer->user_block_button,
					
					"update_viewer_left_vseparator", &update_viewer->left_vseparator,
					"status_vbox", &update_viewer->status_vbox,
					"status_view_vbox", &update_viewer->status_view_vbox,
					"update_datetime_label", &update_viewer->update_datetime_label,
					
					"update_viewer_update_composition_vbox", &update_viewer->update_composition_vbox,
					"notification_labels_hbox", &update_viewer->notification_labels_hbox,
					"best_friend_dm_notification_label", &update_viewer->best_friend_dm_notification_label,
					"char_count", &update_viewer->char_count,
					
					"update_viewer_update_composition_hbox", &update_viewer->update_composition_hbox,
					"update_viewer_sexy_entry_combo_box_entry", &update_viewer->sexy_entry_combo_box_entry,
					"previous_updates_list_store", &update_viewer->previous_updates_list_store,
					"sexy_send", &update_viewer->sexy_send,
					"sexy_dm_button", &update_viewer->sexy_dm_button,
					"sexy_vseparator", &update_viewer->sexy_vseparator,
					"new_update_button", &update_viewer->new_update_button,
					
					"dm_frame", &update_viewer->dm_frame,
					"dm_form_hbox", &update_viewer->dm_form_hbox,
					"dm_frame_label", &update_viewer->dm_frame_label,
					"dm_refresh", &update_viewer->dm_refresh,
					"followers_combo_box", &update_viewer->followers_combo_box,
					"followers_list_store", &update_viewer->followers_list_store,
					"followers_send_dm", &update_viewer->followers_send_dm,
					
					"update_viewer_compact_view_toggle_button", &update_viewer->compact_view_toggle_button,
					"update_viewer_compact_view_image", &update_viewer->compact_view_image,
					"update_viewer_embed_toggle_button", &update_viewer->embed_toggle_button,
					"update_viewer_embed_image", &update_viewer->embed_image,
					
					"dm_form_active_togglebutton", &update_viewer->dm_form_active_togglebutton,
					"dm_form_active_image", &update_viewer->dm_form_active_image,
					
					"update_viewer_right_vseparator", &update_viewer->right_vseparator,
					"controls_vbox", &update_viewer->controls_vbox,
					
					"status_control_hbox", &update_viewer->status_control_hbox,
					"reply_button", &update_viewer->reply_button, 
					"forward_update_button", &update_viewer->forward_update_button,
					"make_fave_button", &update_viewer->make_fave_button,
				NULL
	);
	update_viewer->followers_model=gtk_combo_box_get_model(update_viewer->followers_combo_box);
	update_viewer->previous_updates_tree_model=gtk_combo_box_get_model(GTK_COMBO_BOX(update_viewer->sexy_entry_combo_box_entry));
	
	debug("Building & setting up new Tweet entry area.");
	update_viewer_sexy_init();
	update_viewer_reorder();
	debug("UpdateViewer view & entry area setup.  Grabbing selected widgets.");
	update_viewer_selected_update_buttons_setup(ui);
	update_viewer_grab_widgets_compact_update_viewer_hidden(ui);
	
	debug("UpdateViewer's hotkey connected.  Connecting signal handlers.");
	update_viewer_bind_hotkeys(ui);
	
	gtkbuilder_connect( ui, update_viewer,
				"update_viewer", "destroy", update_viewer_destroy_cb,
				"update_viewer", "delete_event", update_viewer_delete_event_cb,
				"update_viewer", "configure_event", update_viewer_configure_event_cb,

				"view_user_profile_button", "clicked", online_service_request_selected_update_view_profile,
				"view_user_unread_updates_button", "clicked", online_service_request_selected_update_view_updates_new,
				"view_user_updates_button", "clicked", online_service_request_selected_update_view_updates,
				"selected_update_view_forwards_button", "clicked", online_service_request_selected_update_view_forwards,
				
				"user_follow_button", "clicked", online_service_request_selected_update_follow,
				"user_unfollow_button", "clicked", online_service_request_selected_update_unfollow,
				"user_block_button", "clicked", online_service_request_selected_update_block,
				
				"update_viewer_sexy_entry_combo_box_entry", "changed", update_viewer_previous_updates_select,
				"sexy_send", "clicked", update_viewer_send,
				"sexy_dm_button", "clicked", update_viewer_send,
				"new_update_button", "clicked", update_viewer_new_update,
				"dm_form_active_togglebutton", "toggled", update_viewer_dm_show,
				
				"dm_refresh", "clicked", update_viewer_dm_refresh,
				"followers_send_dm", "clicked", update_viewer_send,
				
				"reply_button", "clicked", update_viewer_reply,
				"forward_update_button", "clicked", update_viewer_forward,
				"make_fave_button", "clicked", online_service_request_selected_update_save_fave,
			NULL
	);
	
	gchar *window_title=g_strdup_printf("%s - %s", _(GETTEXT_PACKAGE), _("Control Panel"));
	gtk_window_set_title(update_viewer->update_viewer, window_title);
	uber_free(window_title);
	
	if(!( parent && gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE) )){
		debug("UpdateViewer's set to be embed, no further setup needed.");
	}else{
		debug("Displaying UpdateViewer as a stand alone dialog & setting UpdateViewer's parent window..");
		gtk_widget_show_all(GTK_WIDGET(update_viewer->update_viewer));
		g_object_add_weak_pointer(G_OBJECT(update_viewer->update_viewer),(gpointer)&update_viewer);
		gtk_window_set_transient_for(GTK_WINDOW(update_viewer->update_viewer), parent);
	}
	
	debug("Setting UpdateViewer's embed state indicators.");
	update_viewer_set_embed_toggle_and_image();
	
	debug("Disabling 'selected widgets' since no update could be selected when we 1st start.");
	update_viewer_selected_update_buttons_show(FALSE, NULL);
	
	debug("Disabling & hiding UpdateViewer's dm form since friends have not yet been loaded.");
	update_viewer_dm_form_activate(FALSE);
	gtk_widget_hide(GTK_WIDGET(update_viewer->dm_refresh));
	gtk_widget_hide( (GtkWidget *)update_viewer->best_friend_dm_notification_label );
	
	online_service_request_unset_selected_update();
	
	gboolean compact;
	if((compact=gconfig_if_bool( PREFS_UPDATE_VIEWER_COMPACT, FALSE)) != gtk_toggle_button_get_active(update_viewer->compact_view_toggle_button) )
		gtk_toggle_button_set_active(update_viewer->compact_view_toggle_button, compact);
	else
		update_viewer_compact_view_display(compact);
	
	return update_viewer;
}/*update_viewer_new*/

static void update_viewer_selected_update_author_best_friend_toggled(GtkToggleButton *best_friend_toggle_button){
	OnlineService *service=online_service_request_selected_update_get_service();
	const gchar *user_name=online_service_request_selected_update_get_user_name();
	if(!( G_STR_N_EMPTY(user_name) && service && update_viewer->viewing_service==service && g_str_equal(update_viewer->viewing_user, user_name) )) return;
	
	if(! online_service_is_user_best_friend( service, user_name) )
		online_service_request_selected_update_best_friend_add();
	else
		online_service_request_selected_update_best_friend_drop();
}/*update_viewer_selected_update_author_best_friend_toggled(best_friend_toggle_widget);*/

GtkWindow *update_viewer_get_window(void){
	return update_viewer->update_viewer;
}/*update_viewer_get_window*/

void update_viewer_emulate_embed_toggle(void){
	gtk_toggle_button_set_active(update_viewer->embed_toggle_button, !gtk_toggle_button_get_active(update_viewer->embed_toggle_button) );
}/*update_viewer_emulate_embed_toggle();*/

void update_viewer_emulate_compact_view_toggle(void){
	gtk_toggle_button_set_active(update_viewer->compact_view_toggle_button, !gtk_toggle_button_get_active(update_viewer->compact_view_toggle_button) );
}/*update_viewer_emulate_compact_view_toggle();*/

void update_viewer_set_embed_toggle_and_image(void){
	if(!gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE)){
		debug("Setting UpdateViewer's embed state indicators to split UpdateViewer off into a floating window.");
		gtk_toggle_button_set_active(update_viewer->embed_toggle_button, FALSE);
		gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->embed_toggle_button), "<span weight=\"bold\">Split UpdateViewer into its own window.</span>");
		gtk_image_set_from_icon_name(update_viewer->embed_image, "gtk-go-up", ImagesMinimum);
	}else{
		debug("Setting UpdateViewer's embed state indicators to embed UpdateViewer into %s's main window.", PACKAGE_NAME);
		gtk_toggle_button_set_active(update_viewer->embed_toggle_button, TRUE);
		gchar *tooltip_markup=g_strdup_printf("<span weight=\"light\">Move UpdateViewer back into %s's main window.</span>", PACKAGE_NAME);
		gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->embed_toggle_button), tooltip_markup );
		uber_free(tooltip_markup);
		gtk_image_set_from_icon_name(update_viewer->embed_image, "gtk-go-down", ImagesMinimum);
	}
}/*update_viewer_set_embed_toggle_and_image*/

static void update_viewer_grab_widgets_compact_update_viewer_hidden(GtkBuilder *ui){
	const gchar *update_view_containers[]={
		"user_vbox",
		"update_viewer_left_vseparator",
		"status_view_vbox",
		"update_viewer_view_controls_aspect_frame",
		"update_viewer_right_vseparator",
		"controls_vbox"
	};
	
	GList *list=NULL;
	for(int i=0; i < G_N_ELEMENTS(update_view_containers); i++)
		list=g_list_append(list, (gtk_builder_get_object(ui, update_view_containers[i])) );
	update_viewer->compact_update_viewer_hidden_containers=list;
}/*update_viewer_grab_widgets_for_compact_update_viewer_hidden(ui);*/

static void update_viewer_selected_update_buttons_setup(GtkBuilder *ui){
	const gchar *selected_update_buttons[]={
		"view_user_profile_button",
		"view_user_unread_updates_button",
		"view_user_updates_button",
		"best_friend_toggle_button",
		
		"user_follow_button",
		"user_unfollow_button",
		"user_block_button",
		
		"sexy_dm_button",
		"new_update_button",
		
		"reply_button",
		"forward_update_button",
		"make_fave_button",
	};
	
	GList *list=NULL;
	for(int i=0; i < G_N_ELEMENTS(selected_update_buttons); i++)
		list=g_list_append(list, (gtk_builder_get_object(ui, selected_update_buttons[i])) );
	update_viewer->selected_update_buttons=list;
}/*update_viewer_selected_widgets_setup*/

static void update_viewer_bind_hotkeys(GtkBuilder *ui){
	g_signal_connect_after( update_viewer->embed_toggle_button, "toggled", (GCallback)main_window_update_viewer_set_embed, NULL );
	g_signal_connect_after( update_viewer->compact_view_toggle_button, "toggled", (GCallback)update_viewer_compact_view_toggled, NULL );
	g_signal_connect_after( update_viewer->best_friend_toggle_button, "toggled", (GCallback)update_viewer_selected_update_author_best_friend_toggled, NULL );
	g_signal_connect_after( update_viewer->update_viewer, "event-after", (GCallback)update_viewer_sexy_select, NULL );
	g_signal_connect_after( update_viewer->update_viewer_embed, "event-after", (GCallback)update_viewer_sexy_select, NULL );
	
	const gchar *hotkey_widgets[]={
		/* Connect the signals */
		"update_viewer",
		"update_viewer_embed",
		"user_vbox",
		
		"status_vbox",
		"status_view_vbox",
		"update_viewer_update_composition_vbox",
		"notification_labels_hbox",
		"update_viewer_update_composition_hbox",
		"update_viewer_sexy_entry_combo_box_entry",
		"dm_frame",
		"followers_combo_box",
		
		"controls_vbox",
		"status_control_hbox",
	};
	GObject *widget=NULL;
	debug("UpdateViewer interface loaded.  Setting up hotkeys.");
	for(int i=0; i<G_N_ELEMENTS(hotkey_widgets); i++){
		widget=NULL;
		if(!(widget=gtk_builder_get_object(ui, hotkey_widgets[i]))){
			debug("**ERROR:** Cannot bind %s's hotkeys to %s, the widget could not be found in %s", _(GETTEXT_PACKAGE), hotkey_widgets[i], GtkBuilderUI);
			continue;
		}
		
		debug("Binding %s's hotkeys to %s.", _(GETTEXT_PACKAGE), hotkey_widgets[i]);
		g_signal_connect_after(widget, "key-press-event",(GCallback)hotkey_pressed, widget);
	}
}/*update_viewer_bind_hotkeys(ui);*/

void update_viewer_compact_view_toggled(GtkToggleButton *compact_view_toggle_button){
	gboolean compact;
	if((compact=gtk_toggle_button_get_active(compact_view_toggle_button))!=gconfig_if_bool(PREFS_UPDATE_VIEWER_COMPACT, FALSE) )
		gconfig_set_bool(PREFS_UPDATE_VIEWER_COMPACT, compact);
	
	update_viewer_compact_view_display(compact);
	
	if(!compact){
		gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->compact_view_toggle_button), "<span weight=\"bold\" variant=\"small-caps\">Enlarge control panel.</span>\n<span style=\"italic\" size=\"small\">Shows all control panel buttons &amp; items.</span>");
		gtk_image_set_from_icon_name(update_viewer->compact_view_image, "gtk-fullscreen", ImagesMinimum);
	}else{
		gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->compact_view_toggle_button), "<span weight=\"light\">Compact control panel.</span>\n<span style=\"italic\" size=\"small\">Shows only enabled control panel buttons &amp; items.</span>");
		gtk_image_set_from_icon_name(update_viewer->compact_view_image, "gtk-leave-fullscreen", ImagesMinimum);
	}
}/*update_viewer_compact_view_toggled(update_viewer->compact_view_toggle_button); */

static void update_viewer_compact_view_display(gboolean compact){
	GList *l=NULL;
	debug("%sabling Control Panel compact view.  Control Panel's Compact View Toggle Button is %sactive", (compact ?"En" :"Dis" ), (gtk_toggle_button_get_active(update_viewer->compact_view_toggle_button) ?"" :"in" ) );
	for(l=update_viewer->compact_update_viewer_hidden_containers; l; l=l->next)
		if(!compact)
			gtk_widget_show( (GtkWidget *)l->data );
		else
			gtk_widget_hide( (GtkWidget *)l->data );
	g_list_free(l);
	
	update_viewer_scale(compact);
	gtk_widget_set_sensitive(GTK_WIDGET(update_viewer->update_composition_vbox), TRUE );
	gtk_widget_show(GTK_WIDGET(update_viewer->update_composition_vbox) );
	update_viewer_sexy_select();
}/*update_viewer_compact_view_display(TRUE); */

static void update_viewer_scale(gboolean compact){
	gint h=0, w=0;
	gtk_window_get_size(main_window_get_window(), &w, &h );
	gint position=gtk_paned_get_position(main_window_get_main_paned() );
	position++;
}/* update_viewer_scale(TRUE); */ 

void update_viewer_best_friends_start_dm(OnlineService *service, const gchar *user_name){
	update_viewer->best_friends_service=service;
	update_viewer->best_friends_user_name=g_strdup(user_name);
	gchar *best_friend_dm_markup_message=NULL;
	gtk_label_set_markup( update_viewer->best_friend_dm_notification_label, (best_friend_dm_markup_message=g_strdup_printf("<b>Direct Message %s:</b>", update_viewer->best_friends_user_name)) );
	uber_free(best_friend_dm_markup_message);
	gtk_widget_show( (GtkWidget *)update_viewer->best_friend_dm_notification_label );
}/*void update_viewer_best_friends_start_dm(service, user_name);*/

static void update_viewer_selected_update_buttons_show(gboolean selected_update, const gchar *user_name){
	main_window_selected_update_image_menu_items_show(selected_update);
	GList *l=NULL;
	for(l=update_viewer->selected_update_buttons; l; l=l->next)
		gtk_widget_set_sensitive(GTK_WIDGET(l->data), selected_update);
	g_list_free(l);
	if(selected_update && G_STR_N_EMPTY(user_name) )
		gtk_toggle_button_set_active( update_viewer->best_friend_toggle_button, (online_service_is_user_best_friend(online_service_request_selected_update_get_service(), user_name)) );
	update_viewer_sexy_select();
}/*update_viewer_selected_widgets_show*/

static void update_viewer_dm_form_set_toggle_and_image(void){
	if(!gtk_toggle_button_get_active(update_viewer->dm_form_active_togglebutton)){
		debug("Setting UpdateViewer's dm form toggle button to enable the DM form.");
		gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->dm_form_active_togglebutton), "<span weight=\"bold\">Compose DM's to any of your friends.</span>");
		gtk_image_set_from_icon_name(update_viewer->dm_form_active_image, "gtk-edit", ImagesMinimum);
	}else{
		debug("Setting UpdateViewer's dm form toggle button to disable the DM form.");
		gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->dm_form_active_togglebutton), "<span weight=\"light\">Hide the DM form &amp; area.  You'll still be able to send DMs in the future by clicking this button again.</span>");
		gtk_image_set_from_icon_name(update_viewer->dm_form_active_image, "gtk-close", ImagesMinimum);
	}
}/*update_viewer_dm_form_set_toggle_and_image*/

static void update_viewer_dm_form_activate(gboolean dm_activate){
	update_viewer_dm_form_set_toggle_and_image();
	
	if(!dm_activate){
		gtk_widget_hide(GTK_WIDGET(update_viewer->dm_frame));
		gtk_widget_grab_focus(GTK_WIDGET(update_viewer->sexy_entry));
	}else{
		gtk_widget_show(GTK_WIDGET(update_viewer->dm_frame));
		gtk_widget_grab_focus(GTK_WIDGET(update_viewer->followers_combo_box));
	}
}/*update_viewer_dm_form_activate*/

static void update_viewer_sexy_init(void){
	/* Set-up expand UpdateViewer.  Used to view updates in detailed & send updates and DMs. */
	debug("Creating Tweet's service link area, 'update_viewer->sexy_to', using sexy label interface.");
	update_viewer->sexy_to=uberchick_label_new();
	gtk_box_pack_start(
			GTK_BOX(update_viewer->status_view_vbox),
			GTK_WIDGET(update_viewer->sexy_to),
			FALSE, FALSE, 5
	);
	g_object_set(update_viewer->sexy_to, "yalign", 0.50, "xalign", 1.00, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL);
	gtk_widget_show(GTK_WIDGET(update_viewer->sexy_to));
	
	debug("Creating Tweet's title area, 'update_viewer->sexy_from', using sexy label interface.");
	update_viewer->sexy_from=uberchick_label_new();
	gtk_box_pack_start(
			GTK_BOX(update_viewer->status_view_vbox),
			GTK_WIDGET(update_viewer->sexy_from),
			FALSE, FALSE, 5
	);
	
	debug("Aligning Tweet's view area, 'update_viewer->update_datetime_label',");
	g_object_set(update_viewer->update_datetime_label, "yalign", 0.50, "xalign", 1.00, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL);
	
	g_object_set(update_viewer->sexy_from, "yalign", 0.50, "xalign", 0.00, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL);
	gtk_widget_show(GTK_WIDGET(update_viewer->sexy_from));
	
	debug("Creating Tweet's view area, 'update_viewer->sexy_update', using sexy label interface.");
	update_viewer->sexy_update=uberchick_label_new();
	g_object_set(update_viewer->sexy_update, "yalign", 0.50, "xalign", 0.00, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL);
	gtk_box_pack_start(
			GTK_BOX(update_viewer->status_view_vbox),
			GTK_WIDGET(update_viewer->sexy_update),
			TRUE, TRUE, 5
	);
	gtk_widget_show(GTK_WIDGET(update_viewer->sexy_update));
	
	debug("Creating Tweet's entry, \"update_viewer->sexy_entry\", using SexyEntry, and adding it to UpdateViewer's \"sexy_entry_combo_box_entry\".");
	update_viewer->sexy_entry=(SexySpellEntry *)sexy_spell_entry_new();
	update_viewer->sexy_entry=g_object_ref_sink(update_viewer->sexy_entry);
	gtk_container_remove(GTK_CONTAINER(update_viewer->sexy_entry_combo_box_entry), gtk_bin_get_child(GTK_BIN(update_viewer->sexy_entry_combo_box_entry)));
	gtk_container_add(GTK_CONTAINER(update_viewer->sexy_entry_combo_box_entry), GTK_WIDGET(update_viewer->sexy_entry));
	update_viewer->sexy_completion=gtk_entry_completion_new();
	gtk_entry_completion_set_model(update_viewer->sexy_completion, update_viewer->previous_updates_tree_model);
	gtk_entry_set_completion(GTK_ENTRY(update_viewer->sexy_entry), update_viewer->sexy_completion);
	gtk_combo_box_entry_set_text_column(update_viewer->sexy_entry_combo_box_entry, GSTRING_UPDATE);
	gtk_entry_completion_set_text_column(update_viewer->sexy_completion, GSTRING_UPDATE);
	update_viewer_previous_updates_load(update_viewer);
	gtk_widget_show(GTK_WIDGET(update_viewer->sexy_entry));
	
	g_signal_connect_after(update_viewer->sexy_entry, "key-press-event", G_CALLBACK(hotkey_pressed), NULL);
	g_signal_connect_after(update_viewer->sexy_entry, "key-release-event", G_CALLBACK(update_viewer_sexy_entry_character_count), update_viewer->char_count);
	g_signal_connect(update_viewer->sexy_entry, "activate", G_CALLBACK(update_viewer_send), NULL);
	g_signal_connect_after(update_viewer->followers_combo_box, "changed", G_CALLBACK(update_viewer_sexy_select), update_viewer->followers_combo_box);
}/*update_viewer_sexy_init();*/

static void update_viewer_reorder(void){
	debug("Ordering UpdateViewer.");
	debug("Setting 'sexy_to' as 'status_view_vbox' 1st widget.");
	gtk_box_reorder_child(
			GTK_BOX(update_viewer->status_view_vbox),
			GTK_WIDGET(update_viewer->sexy_to),
			0
	);
	
	debug("Setting 'sexy_from' as 'status_view_vbox' 2nd widget.");
	gtk_box_reorder_child(
			GTK_BOX(update_viewer->status_view_vbox),
			GTK_WIDGET(update_viewer->sexy_from),
			1
	);
	
	debug("Setting 'update_datetime_label' as 'status_view_vbox' 3rd widget.");
	gtk_box_reorder_child(
			GTK_BOX(update_viewer->status_view_vbox),
			GTK_WIDGET(update_viewer->update_datetime_label),
			2
	);
		
	debug("Setting 'sexy_update' as 'status_view_vbox' 4th widget.");
	gtk_box_reorder_child(
			GTK_BOX(update_viewer->status_view_vbox),
			GTK_WIDGET(update_viewer->sexy_update),
			3
	);
}/*update_viewer_reorder*/


void update_viewer_view_selected_update(OnlineService *service, const gdouble id, const gdouble user_id, const gchar *user_name, const gchar *nick_name, const gchar *date, const gchar *sexy_update, const gchar *text_update, GdkPixbuf *pixbuf){
	if(!(service && service->session && online_service_validate_session(service, user_name) )) return;
	if(!id)
		online_service_request_unset_selected_update();
	else{
		online_service_request_set_selected_update(service, id, user_id, user_name, text_update);
		main_window_set_statusbar_default_message( _("Hotkeys: <CTRL+N> start a new tweet; <CTRL+D> or <SHIFT+Return> to dm; <CTRL+R>, <Return>, or '@' to reply, <CTRL+F> or '>' to forward/retweet.") );
	}
	
	debug("%s the Control Panel.", (id ?_("Enlarging") :"") );
	update_viewer_compact_view_display( (id ?FALSE :gconfig_if_bool( PREFS_UPDATE_VIEWER_COMPACT, FALSE) ) );
	
	debug("%sabling 'selected_update_buttons'.", (id ?"En" :"Dis") );
	update_viewer_selected_update_buttons_show((id ?TRUE :FALSE), (G_STR_EMPTY(user_name) ?NULL :user_name ) );
	
	const gchar *uri_scheme_suffix=(service->https?"s":"");
	
	update_viewer->viewing_service=service;
	if(update_viewer->viewing_user) uber_free(update_viewer->viewing_user);
	update_viewer->viewing_user=g_strdup((G_STR_EMPTY(user_name) ?"" :user_name ) );
	
	if(id){
		update_viewer->viewing_update_id=id;
		update_viewer->viewing_update_id_str=gdouble_to_str(id);
	}else{
		main_window_set_statusbar_msg(NULL);
		update_viewer->viewing_update_id=0.0;
		uber_free(update_viewer->viewing_update_id_str);
	}
	
	gchar *sexy_text=NULL;
	if(!id)
		sexy_text=g_strdup("");
	else
		sexy_text=g_strdup_printf("<span size=\"small\" weight=\"light\" variant=\"smallcaps\">To: <a href=\"http%s://%s/%s\">%s &lt;%s@%s&gt;</a></span>", uri_scheme_suffix, service->uri, service->user_name, service->nick_name, service->user_name, service->uri);
	debug("Setting 'sexy_to' for 'selected_update':\n\t\t%s.", sexy_text);
	uberchick_label_set_text(update_viewer->sexy_to, service, id, sexy_text, FALSE, TRUE);
	uber_free(sexy_text);
	
	if(!(G_STR_EMPTY(nick_name) && G_STR_EMPTY(user_name) )){
		sexy_text=g_strdup_printf("<span weight=\"ultrabold\">From: <a href=\"http%s://%s/%s\">%s &lt;%s@%s&gt;</a></span>", uri_scheme_suffix, service->uri, user_name, nick_name, user_name, service->uri);
	}else
		sexy_text=g_strdup("");
	debug("Setting 'sexy_from' for 'selected_update':\n\t\t%s.", sexy_text);
	uberchick_label_set_text(update_viewer->sexy_from, service, id, sexy_text, FALSE, TRUE);
	uber_free(sexy_text);
	
	if(!( G_STR_EMPTY(date)))
		sexy_text=g_markup_printf_escaped("<span style=\"italic\">[%s]</span>", date);
	else
		sexy_text=g_strdup("");
	gtk_label_set_markup(update_viewer->update_datetime_label, sexy_text);
	uber_free(sexy_text);
	
	
	/*
	 * gchar sexy_update has already been formatted to include urls, hyperlinks, titles, & etc.
	 * So we just set it as a SexyLable & bypass UberChickLabel
	 */
	debug("Setting 'sexy_update' for 'selected_update':\n\t\t%s.", sexy_update);
	if(!(gconfig_if_bool(PREFS_URLS_EXPANSION_SELECTED_ONLY, TRUE)))
		sexy_url_label_set_markup(SEXY_URL_LABEL(update_viewer->sexy_update), sexy_update);
	else
		uberchick_label_set_text(update_viewer->sexy_update, service, id, sexy_update, TRUE, TRUE);
	
	if(!pixbuf)
		gtk_image_set_from_icon_name(update_viewer->user_image, GETTEXT_PACKAGE, ImagesExpanded);
	else{
		debug("Setting avatar for the user who wrote the 'selected_update'.");
		GdkPixbuf *resized=NULL;
		if(!( resized=images_expand_pixbuf(pixbuf)))
			gtk_image_set_from_pixbuf(update_viewer->user_image, pixbuf);
		else{
			gtk_image_set_from_pixbuf(update_viewer->user_image, resized);
			g_object_unref(resized);
		}
	}
	
	debug("Selecting 'sexy_entry' for entering a new update.");
	update_viewer_sexy_select();
}/*update_viewer_view_selected_update*/

static gint update_viewer_sexy_entry_update_length(gchar *update){
	guint16 character_count=0;
	gint replace_me=0;
	gconfig_get_int_or_default(PREFS_UPDATES_REPLACE_ME_W_NICK, &replace_me, 2);
	for(guint16 i=0; update[i]; i++){
		if(update[i]=='<' || update[i]=='>')
			character_count+=3;
		if(replace_me!=-1 && update[i]=='/' && update[i+1] && update[i+1]=='m' && update[i+2] && update[i+2]=='e' ){
			character_count+=online_services_get_length_of_longest_replacement();
			i+=2;
		}
		character_count++;
	}
	
	return TWEET_MAX_CHARS-character_count;
}/*update_viewer_sexy_entry_update_length(update);*/

static void update_viewer_sexy_entry_character_count(GtkEntry *entry, GdkEventKey *event, GtkLabel *update_character_counter){
	update_viewer->sexy_position=gtk_editable_get_position(GTK_EDITABLE(entry));
	gshort remaining_character_count=update_viewer_sexy_entry_update_length(entry->text);
	gchar *remaining_characters_markup_label=NULL;
	if(remaining_character_count < 0){
		if(!gconfig_if_bool(PREFS_DISABLE_UPDATE_LENGTH_ALERT, FALSE))
			update_viewer_beep();
		remaining_characters_markup_label=g_strdup_printf("<span size=\"small\" foreground=\"red\">%d</span>", remaining_character_count);
	}else{
		if(TWEET_MAX_CHARS==remaining_character_count && (update_viewer->best_friends_service ||  in_reply_to_status_id) ){
			in_reply_to_status_id=0.0;
			in_reply_to_service=NULL;
			if(update_viewer->best_friends_service || G_STR_N_EMPTY(update_viewer->best_friends_user_name) ){
				update_viewer->best_friends_service=NULL;
				if(update_viewer->best_friends_user_name) uber_free(update_viewer->best_friends_user_name);
				gtk_widget_hide( (GtkWidget *)update_viewer->best_friend_dm_notification_label );
			}
		}
		remaining_characters_markup_label=g_strdup_printf("<span size=\"small\" foreground=\"green\">%d</span>", remaining_character_count);
	}
	
	gtk_label_set_markup(update_character_counter, remaining_characters_markup_label);
	uber_free(remaining_characters_markup_label);
}/*update_viewer_sexy_entry_character_count*/

void update_viewer_beep(void){
	if(!gconfig_if_bool(PREFS_DISABLE_SYSTEM_BELL, FALSE))
		gtk_widget_error_bell(GTK_WIDGET(update_viewer->sexy_entry));
}/*update_viewer_beep*/

void update_viewer_sexy_select(void){
	if(gtk_widget_has_focus(GTK_WIDGET(update_viewer->sexy_entry))) return;
	gtk_widget_grab_focus(GTK_WIDGET(update_viewer->sexy_entry));
	gint sexy_position=-1;
	if( update_viewer->sexy_position > 0 && update_viewer->sexy_position <= gtk_entry_get_text_length((GtkEntry *)update_viewer->sexy_entry) )
		sexy_position=update_viewer->sexy_position;
	gtk_entry_set_position(GTK_ENTRY(update_viewer->sexy_entry), sexy_position );
}/*update_viewer_sexy_select*/

void update_viewer_sexy_prefix_char(const char c){
	gchar *prefix=g_strdup_printf("%c", c);
	update_viewer_sexy_prefix_string((const gchar *)prefix, FALSE);
	uber_free(prefix);
}/*update_viewer_sexy_prefix_char*/

gboolean update_viewer_sexy_prefix_string(const gchar *prefix, gboolean uniq){
	if(g_str_has_prefix(GTK_ENTRY(update_viewer->sexy_entry)->text, prefix) && !g_str_equal(GTK_ENTRY(update_viewer->sexy_entry)->text, prefix) )
		return TRUE;
	if(!(uniq && g_str_has_prefix(GTK_ENTRY(update_viewer->sexy_entry)->text, prefix) ))
		update_viewer_sexy_puts(prefix, 0);
	
	return FALSE;
}/*update_viewer_sexy_prefix_string*/

void update_viewer_sexy_set(gchar *text){
	gtk_entry_set_text(GTK_ENTRY(update_viewer->sexy_entry), (text==NULL ?(gchar *)"" :text));
	update_viewer_sexy_select();
}/*update_viewer_sexy_set*/

void update_viewer_sexy_insert_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	update_viewer_sexy_insert_string((const gchar *)str);
	uber_free(str);
}/*update_viewer_sexy_insert_char*/

void update_viewer_sexy_insert_string(const gchar *str){
	update_viewer_sexy_puts(str, gtk_editable_get_position(GTK_EDITABLE(update_viewer->sexy_entry)) );
}/*update_viewer_sexy_insert_string*/

void update_viewer_sexy_append_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	update_viewer_sexy_append_string((const gchar *)str);
	uber_free(str);
}/*update_viewer_sexy_append_char*/

void update_viewer_sexy_append_string(const gchar *str){
	update_viewer_sexy_puts(str,(gint)gtk_entry_get_text_length(GTK_ENTRY(update_viewer->sexy_entry)) );
}/*update_viewer_sexy_append_string*/

gint update_viewer_sexy_puts(const gchar *str, gint position_after){
	gint position_prior=gtk_editable_get_position(GTK_EDITABLE(update_viewer->sexy_entry));
	gtk_editable_insert_text(GTK_EDITABLE(update_viewer->sexy_entry), str, -1, &position_after );
	if(position_after>position_prior)
		gtk_entry_set_position(GTK_ENTRY(update_viewer->sexy_entry), position_after);
	update_viewer_sexy_select();
	return position_after;
}/*update_viewer_sexy_puts*/

void update_viewer_hide_previous_updates(void){
	g_signal_emit_by_name(update_viewer->sexy_entry_combo_box_entry, "popup");
}/*update_viewer_hide_previous_updates();*/

void update_viewer_show_previous_updates(void){
	g_signal_emit_by_name(update_viewer->sexy_entry_combo_box_entry, "popup");
}/*update_viewer_show_previous_updates();*/

void update_viewer_reply(void){
	update_viewer_reply_or_forward(update_viewer->reply_button);
}/*update_viewer_reply();*/

void update_viewer_forward(void){
	update_viewer_reply_or_forward(update_viewer->forward_update_button);
}/*update_viewer_forward();*/

static void update_viewer_reply_or_forward(GtkButton *update_button){
	gboolean okay_to_send=TRUE;
	if(G_STR_EMPTY(GTK_ENTRY(update_viewer->sexy_entry)->text) && !online_service_request_selected_update_get_user_name())
		update_viewer_beep();
	
	if(update_button!=update_viewer->forward_update_button) okay_to_send=online_service_request_selected_update_reply();
	else okay_to_send=online_service_request_selected_update_forward();
	
	if(!(G_STR_N_EMPTY(GTK_ENTRY(update_viewer->sexy_entry)->text) && okay_to_send))
		return;
	
	update_viewer_sexy_send(NULL, NULL);
}/*update_viewer_reply_or_forward(button);*/

void update_viewer_send(GtkWidget *activated_widget){
	gchar *user_name=NULL;
	
	if(activated_widget==GTK_WIDGET(update_viewer->sexy_entry))
		if(G_STR_EMPTY(GTK_ENTRY(update_viewer->sexy_entry)->text))
			update_viewer_reply();
	
	if(activated_widget==GTK_WIDGET(update_viewer->sexy_dm_button) ){
		const gchar *user_name;
		if(!(( user_name=online_service_request_selected_update_get_user_name()) && G_STR_N_EMPTY(user_name) )) return update_viewer_beep();
		update_viewer_sexy_send(online_service_request_selected_update_get_service(), user_name);
		return;
	}
	
	if((activated_widget==GTK_WIDGET(update_viewer->followers_send_dm)) &&(GTK_WIDGET_IS_SENSITIVE(update_viewer->followers_combo_box)) &&G_STR_N_EMPTY((user_name=gtk_combo_box_get_active_text(update_viewer->followers_combo_box)) ) ){
		uber_free(user_name);
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		User *user=NULL;
		gtk_combo_box_get_active_iter(update_viewer->followers_combo_box, iter);
		gtk_tree_model_get(
					update_viewer->followers_model,
					iter,
						USER_POINTER, &user,
					-1
		);
		update_viewer_sexy_send(user->service, user->user_name);
		uber_free(iter);
		return;
	}
	
	if(update_viewer->best_friends_service && update_viewer->best_friends_user_name)
		return update_viewer_sexy_send(update_viewer->best_friends_service, update_viewer->best_friends_user_name);
	
	update_viewer_sexy_send(NULL, NULL);
}/*update_viewer_send*/

void update_viewer_new_update(void){
	main_window_set_statusbar_default_message( _("Hotkeys: Press Up, Down, Page Up, or Page Down to browse updates.  Press & Hold <CTRL+SHIFT> while browsing to select the update.") );
	update_viewer_view_selected_update((selected_service ?selected_service :online_services_connected_get_first()), 0, 0, NULL, NULL, NULL, NULL, NULL, NULL);
	
	if(update_viewer->best_friends_service) update_viewer->best_friends_service=NULL;
	if(update_viewer->best_friends_user_name) uber_free(update_viewer->best_friends_user_name);
	
	if(in_reply_to_service) in_reply_to_service=NULL;
	if(in_reply_to_status_id) in_reply_to_status_id=0;
	
	gtk_widget_hide( (GtkWidget *)update_viewer->best_friend_dm_notification_label );
	online_service_request_unset_selected_update();
	update_viewer_sexy_set((gchar *)"");
	
	if(gtk_toggle_button_get_active(update_viewer->best_friend_toggle_button) )
		gtk_toggle_button_set_active(update_viewer->best_friend_toggle_button, FALSE);
	
	geometry_load();
	update_viewer_sexy_select();
}/*update_viewer_new_update();*/

static void update_viewer_sexy_send(OnlineService *service, const gchar *user_name){
	if(!((GTK_ENTRY(update_viewer->sexy_entry)->text) &&(update_viewer_sexy_entry_update_length(GTK_ENTRY(update_viewer->sexy_entry)->text) > -1) )){
		if(!gconfig_if_bool(PREFS_DISABLE_UPDATE_LENGTH_ALERT, FALSE))
			update_viewer_beep();
		return;
	}
	
	if(!(service && user_name && G_STR_N_EMPTY(user_name) ))
		network_post_status(GTK_ENTRY(update_viewer->sexy_entry)->text);
	else
		network_send_message(service, user_name, GTK_ENTRY(update_viewer->sexy_entry)->text);
	
	update_viewer_previous_updates_prepend(GTK_ENTRY(update_viewer->sexy_entry)->text, update_viewer);
	update_viewer_sexy_set((gchar *)"");
}/*update_viewer_sexy_send(online_service_request_selected_update_get_service(), online_service_request_selected_update_get_user_name() );*/

void update_viewer_sexy_send_dm(void){
	OnlineService *service=NULL;
	const gchar *user_name=NULL;
	if((service=update_viewer->best_friends_service) && G_STR_N_EMPTY((user_name=update_viewer->best_friends_user_name) ) )
		return update_viewer_sexy_send(service, user_name );
	
	if((service=online_service_request_selected_update_get_service()) && G_STR_N_EMPTY((user_name=online_service_request_selected_update_get_user_name()) ) )
		return update_viewer_sexy_send(service, user_name);
	
	if((GTK_WIDGET_IS_SENSITIVE(update_viewer->followers_combo_box)) && G_STR_N_EMPTY((user_name=gtk_combo_box_get_active_text(update_viewer->followers_combo_box)) )  )
		return update_viewer_send(GTK_WIDGET(update_viewer->followers_send_dm) );
	
	update_viewer_beep();
}/*update_viewer_sexy_send_dm();*/

static void update_viewer_previous_updates_load(UpdateViewer *update_viewer){
	if(in_reply_to_service) in_reply_to_service=NULL;
	if(in_reply_to_status_id) in_reply_to_status_id=0.0;
	gchar *previous_update=NULL, *previous_update_gconf_path=NULL;
	gconfig_get_int_or_default(PREFS_PREVIOUS_UPDATES_MAXIMUM_UPDATES, &update_viewer->max_updates, 50);
	if(update_viewer->max_updates < 5)
		gconfig_set_int(PREFS_PREVIOUS_UPDATES_MAXIMUM_UPDATES, (update_viewer->max_updates=5));
	else if(update_viewer->max_updates > 100)
		gconfig_set_int(PREFS_PREVIOUS_UPDATES_MAXIMUM_UPDATES, (update_viewer->max_updates=100));
	for(update_viewer->total_updates=0; update_viewer->total_updates<=update_viewer->max_updates; update_viewer->total_updates++){
		if(!( (gconfig_get_string( (previous_update_gconf_path=g_strdup_printf(UPDATE_VIEWER_PREVIOUS_UPDATES_STRING, update_viewer->total_updates)), &previous_update )) && G_STR_N_EMPTY(previous_update) )) break;
		
		gchar **previous_update_details=g_strsplit(previous_update, "->", 3);
		if(G_STR_EMPTY(previous_update_details[2])){
			uber_free(previous_update);
			uber_free(previous_update_gconf_path);
			g_strfreev(previous_update_details);
			continue;
		}
		if(G_STR_N_EMPTY(previous_update_details[0]))
			if(!( update_viewer->viewing_service=online_services_get_service_by_key(previous_update_details[0]) ))
				update_viewer->viewing_service=NULL;
		
		if(G_STR_N_EMPTY(previous_update_details[1]))
			if(!( update_viewer->viewing_update_id=strtod(previous_update_details[1], NULL) > 0.0 ))
				update_viewer->viewing_update_id=0.0;
		
		GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
		gtk_list_store_append(update_viewer->previous_updates_list_store, iter);
		gtk_list_store_set(
				update_viewer->previous_updates_list_store, iter,
					GSTRING_UPDATE, g_strdup(previous_update_details[2]),
					IN_REPLY_TO_SERVICE,update_viewer->viewing_service,
					IN_REPLY_TO_STATUS_ID, update_viewer->viewing_update_id,
				-1
		);
		uber_free(iter);
		uber_free(previous_update);
		uber_free(previous_update_gconf_path);
		g_strfreev(previous_update_details);
		update_viewer->viewing_service=NULL;
		update_viewer->viewing_update_id=0.0;
	}

	update_viewer_previous_updates_prepend(_("[new update]"), update_viewer);
	if(previous_update) uber_free(previous_update);
	if(previous_update_gconf_path) uber_free(previous_update_gconf_path);
}/*update_view_previous_updates_load(update_viewer);*/

static void update_viewer_previous_updates_prepend(const gchar *update, UpdateViewer *update_viewer){
	if(G_STR_EMPTY(update)) return;
	
	if(
		gconfig_if_bool(PREFS_PREVIOUS_UPDATES_UNIQUE_ONLY, TRUE)
		&&
		!update_viewer_previous_updates_check_unique(update, update_viewer)
	){
		debug("Update being sent: %s is already in the previous update's list and will not be stored again.", update);
		return;
	}
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	gtk_list_store_prepend(update_viewer->previous_updates_list_store, iter);
	gtk_list_store_set(
			update_viewer->previous_updates_list_store, iter,
				GSTRING_UPDATE, g_strdup(update),
				IN_REPLY_TO_SERVICE, (update_viewer->viewing_service?update_viewer->viewing_service:NULL),
				IN_REPLY_TO_STATUS_ID, (update_viewer->viewing_update_id?update_viewer->viewing_update_id:0.0),
			-1
	);
	uber_free(iter);
	
	if(g_str_equal(update, "[new update]"))
		return;
	
	update_viewer_previous_updates_remove(1, update_viewer);
	update_viewer_previous_updates_prepend("[new update]", update_viewer);
	
	if(update_viewer->total_updates<update_viewer->max_updates)
		update_viewer->total_updates++;
	else
		update_viewer_previous_updates_remove(update_viewer->total_updates, update_viewer);
	
	update_viewer_previous_updates_rotate(update_viewer);
	gchar *previous_update=NULL, *previous_update_gconf_path=NULL;
	gconfig_set_string( previous_update_gconf_path=g_strdup_printf(UPDATE_VIEWER_PREVIOUS_UPDATES_STRING, 0), previous_update=g_strdup_printf("%s->%s->%s", (update_viewer->viewing_service ?update_viewer->viewing_service->key :""), (update_viewer->viewing_update_id_str?update_viewer->viewing_update_id_str:""), update) );
	uber_free(previous_update);
	uber_free(previous_update_gconf_path);
}/*update_viewer_previous_updates_prepend(GTK_ENTRY(update_viewer->sexy_entry), update_viewer);*/

static gboolean update_viewer_previous_updates_check_unique(const gchar *update, UpdateViewer *update_viewer){
	gboolean uniq=TRUE;
	if(g_str_equal(update, "[new update]"))
		return TRUE;
	
	debug("Checking uniqueness of update.  New update:");
	debug("\t[%s]", update);
	gchar *update_at_index=NULL;
	for(gint i=1; i<=update_viewer->total_updates && uniq; i++) {
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter(update_viewer->previous_updates_tree_model, iter, path))){
			debug("Checking update, at index: %d, failed to get valid iter for the list store.", i);
			continue;
		}
		
		gtk_tree_model_get(
				update_viewer->previous_updates_tree_model, iter,
					GSTRING_UPDATE, &update_at_index,
				-1
		);
		
		if(!strcmp(update, update_at_index)){
			debug("Update is not unique.  Duplicate update found at index: %d.", i);
			debug("\tComparing new update: [%s]", update);
			debug("\tAgainst old update: [%s]", update_at_index);
			uniq=FALSE;
		}
		
		uber_free(update_at_index);
		uber_free(iter);
	}
	return uniq;
}/*update_viewer_previous_updates_check_unique(update, update_viewer);*/

static void update_viewer_previous_updates_remove(gint list_store_index, UpdateViewer *update_viewer){
	if(!(list_store_index > 0 && list_store_index <= update_viewer->total_updates)) return;
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	GtkTreePath *path=gtk_tree_path_new_from_indices(list_store_index, -1);
	debug("Removing saved update %d.  Total update saved: %d", list_store_index, update_viewer->total_updates);
	if(!(gtk_tree_model_get_iter(update_viewer->previous_updates_tree_model, iter, path)))
		debug("Removing saved update, at index: %d, failed to get valid iter for the list store.", list_store_index);
	else{
		debug("Removing iter at index: %d", list_store_index);
		gtk_list_store_remove(update_viewer->previous_updates_list_store, iter);
	}
	uber_free(iter);
}/*update_viewer_previous_updates_remove(GTK_ENTRY(update_viewer->sexy_entry), update_viewer);*/

static void update_viewer_previous_updates_rotate(UpdateViewer *update_viewer){
	gchar *previous_update=NULL, *previous_update_gconf_path=NULL;
	for(gint i=update_viewer->max_updates; i>=0; i--){
		if( (gconfig_get_string( previous_update_gconf_path=g_strdup_printf(UPDATE_VIEWER_PREVIOUS_UPDATES_STRING, i-1), &previous_update)) && G_STR_N_EMPTY(previous_update) ){
			uber_free(previous_update_gconf_path);
			previous_update_gconf_path=g_strdup_printf(UPDATE_VIEWER_PREVIOUS_UPDATES_STRING, i);
			gconfig_set_string(previous_update_gconf_path, previous_update);
		}
		if(previous_update) uber_free(previous_update);
		uber_free(previous_update_gconf_path);
	}
}/*update_viewer_previous_updates_rotate(update_viewer);*/

static void update_viewer_previous_updates_select(GtkComboBoxEntry *sexy_entry_combo_box_entry, UpdateViewer *update_viewer){
	GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
	if(!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(update_viewer->sexy_entry_combo_box_entry), iter)){
		uber_free(iter);
		return;
	}
	
	gchar	*update=NULL;
	OnlineService *selected_service=NULL;
	gdouble selected_update_id=0.0;
	gtk_tree_model_get(
				update_viewer->previous_updates_tree_model, iter,
					GSTRING_UPDATE, &update,
					IN_REPLY_TO_SERVICE, &selected_service,
					IN_REPLY_TO_STATUS_ID, &selected_update_id,
				-1
	);
	
	if(g_str_equal(update, "[new update]")){
		update_viewer_new_update();
		uber_free(update);
		return;
	}
	
	if(!G_STR_N_EMPTY(update)){
		if(update) uber_free(update);
		uber_free(iter);
		return;
	}
	if(selected_service) update_viewer->viewing_service=selected_service;
	if(selected_update_id){
		update_viewer->viewing_update_id=selected_update_id;
		if(update_viewer->viewing_update_id_str) uber_free(update_viewer->viewing_update_id_str);
		update_viewer->viewing_update_id_str=gdouble_to_str(update_viewer->viewing_update_id);
	}
	update_viewer_sexy_set(g_strdup(update));
	uber_free(update);
	uber_free(iter);
}/*update_viewer_previous_updates_select(update_viewer->sexy_entry_combo_box_entry, update_viewer);*/

static void update_viewer_previous_updates_free(UpdateViewer *update_viewer){
	debug("Removing all %d saved updates from this session.", update_viewer->total_updates);
	
	for(guint i=0; i<=update_viewer->total_updates; i++){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter(update_viewer->previous_updates_tree_model, iter, path)))
			debug("Removing iter at index: %d failed.  Unable to retrieve iter from path.", i);
		else{
			debug("Destroying saved update #%d.", i);
			gtk_list_store_remove(update_viewer->previous_updates_list_store, iter);
		}
		gtk_tree_path_free(path);
		uber_free(iter);
	}
}/*update_viewer_previous_updates_free(update_viewer);*/

void update_viewer_new_dm(void){
	gtk_toggle_button_set_active(update_viewer->dm_form_active_togglebutton, !gtk_toggle_button_get_active(update_viewer->dm_form_active_togglebutton));
}/*update_viewer_new_dm*/

static void update_viewer_dm_show(GtkToggleButton *toggle_button){
	if(!gtk_toggle_button_get_active(update_viewer->dm_form_active_togglebutton)){
		debug("Disabled UpdateViewer's dm form.");
		update_viewer_dm_form_activate(FALSE);
		return;
	}
	online_service_request_popup_select_service();
	
	if(!( selected_service)) {
		if(gtk_toggle_button_get_active(update_viewer->dm_form_active_togglebutton))
			gtk_toggle_button_set_active(update_viewer->dm_form_active_togglebutton, FALSE);
		return;
	}
	
	debug("Enabling UpdateViewer's dm form.");
	update_viewer_dm_form_activate(TRUE);
	users_glist_get(GetFollowers, FALSE, update_viewer_dm_data_fill);
}/*update_viewer_dm_show*/

static void update_viewer_dm_refresh(void){
	online_service_request_popup_select_service();
	users_glist_get(GetFollowers, TRUE, update_viewer_dm_data_fill);
}/*update_viewer_dm_refresh*/

void update_viewer_dm_data_fill(GList *followers){
	if(!followers) return;
	static gboolean first_fill=TRUE;
	
	User		*user;
	gchar		*null_friend=g_strdup("");
	GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
	
	gtk_list_store_clear(update_viewer->followers_list_store);
	
	gtk_list_store_append(update_viewer->followers_list_store, iter);
	gtk_list_store_set(
				update_viewer->followers_list_store, iter,
					USER_LABEL, null_friend,
					USER_POINTER, NULL,
				-1
	);
	uber_free(iter);
	
	OnlineService *service=NULL;
	for(followers=g_list_first(followers); followers; followers=followers->next) {
		user=(User *)followers->data;
		if(!service) service=user->service;
		gchar *user_label=g_strdup_printf("%s &lt;%s&gt;", user->user_name, user->nick_name);
		debug("Adding user: %s <%s> from <%s> to DM form", user->user_name, user->nick_name, service->guid );
		iter=g_new0(GtkTreeIter, 1);
		gtk_list_store_append(update_viewer->followers_list_store, iter);
		gtk_list_store_set(
				update_viewer->followers_list_store,
				iter,
					USER_LABEL, user_label,
					USER_POINTER, user,
				-1
		);
		uber_free(user_label);
		uber_free(iter);
		iter=NULL;
	}
	followers=g_list_first(followers);
	
	gchar *new_label=g_markup_printf_escaped("<b>_DM one of your, &lt;%s&gt;, followers:</b>", service->key);
	gtk_label_set_markup(update_viewer->dm_frame_label, new_label);
	gtk_label_set_use_underline(update_viewer->dm_frame_label, TRUE);
	gtk_label_set_single_line_mode(update_viewer->dm_frame_label, TRUE);
	uber_free(new_label);
	
	if(first_fill) first_fill=FALSE;
}/*update_viewer_dm_data_fill*/

/********************************************************
 *                       eof                            *
 ********************************************************/
