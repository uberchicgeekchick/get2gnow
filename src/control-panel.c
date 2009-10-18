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
#include "control-panel.h"

struct _ControlPanel{ 
	/* Used to trigger resizing. */
	guint			size_timeout_id;
	
	/* ControlPanel Dialog & embeding Widgets */
	GtkWindow		*control_panel;
	GtkHBox			*control_panel_embed;
	
	/* Linked lists used for changing the appearance of the ControlPanel. */
	/* Widgets which are only seen when the user's enabled the compact ControlPanel by default.
	 * Or whenever a update is selected. */
	GList			*compact_control_panel_hidden_containers;
	/* Widgets that are enabled when we a update is selected */
	GList			*selected_update_buttons;
	
	/* GtkWidgets for viewing user details & for controlling one's relationship to them. */
	GtkHBox			*user_vbox;
	
	GtkVBox			*user_updates_control_hbox;
	GtkImage		*user_image;
	
	/* Buttons for viewing details about the user of the current selected/extended Tweet. */
	GtkButton		*view_user_profile_button;
	GtkButton		*view_user_updates_button;
	GtkToggleButton		*best_friend_toggle_button;
	
	/* Buttons for viewing details about the user of the current selected/extended Tweet. */
	GtkVBox			*user_relationship_controls_hbox;
	GtkButton		*user_follow_button;
	GtkButton		*user_unfollow_button;
	GtkButton		*user_block_button;
	
	/* Just for padding & appearance. */
	GtkVSeparator		*left_vseparator;
	
	/* View & Sending Tweets. */
	GtkVBox			*status_vbox;
	
	/* ControlPanel */
	GtkVBox			*status_view_vbox;
	Label			*sexy_to;
	Label			*sexy_from;
	GtkLabel		*update_datetime_label;
	Label			*sexy_update;
	
	/* Tweet, status, & DM writing area & widgets. */
	GtkAspectFrame		*view_controls_aspect_frame;
	GtkHBox			*view_controls_hbox;
	GtkToggleButton		*compact_view_toggle_button;
	GtkImage		*compact_view_image;
	
	/* Togglebutton & image used to indicate & switch between embed control-panel & floating. */
	GtkToggleButton		*embed_toggle_button;
	GtkImage		*embed_image;
	
	/* Stuff for actually writing your updates & DMs. */
	GtkVBox			*update_composition_vbox;
	GtkHBox			*notification_labels_hbox;
	GtkLabel		*best_friend_dm_notification_label;
	GtkLabel		*char_count;
	
	GtkHBox			*update_composition_hbox;
	
	gint			updates;
	GtkComboBoxEntry	*sexy_entry_combo_box_entry;
	GtkListStore		*previous_updates_list_store;
	GtkTreeModel		*previous_updates_tree_model;
	SexySpellEntry		*sexy_entry;
	GtkEntryCompletion	*sexy_completion;
	gint			sexy_position;
	
	/* Info on the update being viewed to avoid issues with the 'best friends' toggle button. */
	OnlineService		*viewing_service;
	gchar			*viewing_user;
	
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
	
	/* Container for ControlPanel's right side widgets. */
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
	GSTRING_UPDATE,
	IN_REPLY_TO_SERVICE,
	IN_REPLY_TO_STATUS_ID,
	GSTRING_NULL,
} PeviousUpdateInfo;


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define	DEBUG_DOMAINS	"UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Tweets:Requests:Users:Start-Up:control-panel.c"
#include "debug.h"

#define GtkBuilderUI "control-panel"
#define	TWEET_MAX_CHARS	140

static ControlPanel *control_panel=NULL;


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static void control_panel_destroy_cb(GtkWidget *window, ControlPanel *control_panel); 
static gboolean control_panel_delete_event_cb(GtkWidget *window, GdkEvent *event, ControlPanel *control_panel);
static gboolean control_panel_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, ControlPanel *control_panel);
static gboolean control_panel_configure_event_timeout_cb(GtkWidget *widget);

static void control_panel_sexy_init(void);
static void control_panel_reorder(void);

static void control_panel_selected_update_author_best_friend_toggled(GtkToggleButton *best_friend_toggle_button);

static void control_panel_selected_update_buttons_setup(GtkBuilder *ui);
static void control_panel_bind_hotkeys(GtkBuilder *ui);
static void control_panel_selected_update_buttons_show(gboolean selected_update, const gchar *user_name);

static void control_panel_grab_widgets_compact_control_panel_hidden(GtkBuilder *ui);
static void control_panel_compact_view_display(gboolean compact);
static void control_panel_scale(gboolean compact);

static gshort control_panel_sexy_entry_update_length(gchar *update);
static void control_panel_sexy_entry_character_count(GtkEntry *entry, GdkEventKey *event, GtkLabel *update_character_counter);

static void control_panel_sexy_send(OnlineService *service, const gchar *user_name);
static void control_panel_sexy_append(const gchar *update, ControlPanel *control_panel);
static void control_panel_select_previous_update(GtkComboBoxEntry *sexy_entry_combo_box_entry, ControlPanel *control_panel);
static void control_panel_free_updates(ControlPanel *control_panel);

static void control_panel_dm_show(GtkToggleButton *toggle_button);
static void control_panel_dm_form_activate(gboolean dm_activate);
static void control_panel_dm_refresh(void);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
GtkHBox *control_panel_get_embed(void){
	return control_panel->control_panel_embed;
}/*control_panel_get_embed();*/

static void control_panel_destroy_cb(GtkWidget *window, ControlPanel *control_panel){
	debug("Destroying ControlPanel & freeing resources");
	control_panel_free_updates(control_panel);
	
	if(control_panel->viewing_user) uber_free(control_panel->viewing_user);
	if(control_panel->best_friends_user_name) uber_free(control_panel->best_friends_user_name);
	
	gtk_widget_destroy(GTK_WIDGET(control_panel->control_panel) );
	uber_free(control_panel);
}/*control_panel_destroy_cb*/

static gboolean control_panel_delete_event_cb(GtkWidget *window, GdkEvent *event, ControlPanel *control_panel){
	gtk_toggle_button_set_active(control_panel->embed_toggle_button, FALSE);
	
	return TRUE;
}/*control_panel_delete_event_cb*/

static gboolean control_panel_configure_event_timeout_cb(GtkWidget *widget){
	if(!gconfig_if_bool(PREFS_CONTROL_PANEL_DIALOG, FALSE)){
		control_panel->size_timeout_id=0;
		return FALSE;
	}
	
	geometry_save();
	
	control_panel->size_timeout_id=0;
	
	return FALSE;
}

static gboolean control_panel_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, ControlPanel *control_panel){
	program_timeout_remove(&control_panel->size_timeout_id, _("main window configuration"));
	
	control_panel->size_timeout_id=g_timeout_add(500,(GSourceFunc) control_panel_configure_event_timeout_cb, widget );
	
	return FALSE;
}

ControlPanel *control_panel_new(GtkWindow *parent){
	GtkBuilder	*ui;
	
	control_panel=g_new0(ControlPanel, 1);
	debug("Building ControlPanel");
	control_panel->sexy_position=-1;
	control_panel->updates=0;
	ui=gtkbuilder_get_file(
				GtkBuilderUI,
					"control_panel", &control_panel->control_panel,
					"control_panel_embed", &control_panel->control_panel_embed,
					
					"user_vbox", &control_panel->user_vbox,
					"user_image", &control_panel->user_image,
					
					"view_user_profile_button", &control_panel->view_user_profile_button,
					"view_user_updates_button", &control_panel->view_user_updates_button,
					"best_friend_toggle_button", &control_panel->best_friend_toggle_button,
					
					"user_follow_button", &control_panel->user_follow_button,
					"user_unfollow_button", &control_panel->user_unfollow_button,
					"user_block_button", &control_panel->user_block_button,
					
					"control_panel_left_vseparator", &control_panel->left_vseparator,
					"status_vbox", &control_panel->status_vbox,
					"status_view_vbox", &control_panel->status_view_vbox,
					"update_datetime_label", &control_panel->update_datetime_label,
					
					"control_panel_update_composition_vbox", &control_panel->update_composition_vbox,
					"notification_labels_hbox", &control_panel->notification_labels_hbox,
					"best_friend_dm_notification_label", &control_panel->best_friend_dm_notification_label,
					"char_count", &control_panel->char_count,
					
					"control_panel_update_composition_hbox", &control_panel->update_composition_hbox,
					"control_panel_sexy_entry_combo_box_entry", &control_panel->sexy_entry_combo_box_entry,
					"previous_updates_list_store", &control_panel->previous_updates_list_store,
					"sexy_send", &control_panel->sexy_send,
					"sexy_dm_button", &control_panel->sexy_dm_button,
					"sexy_vseparator", &control_panel->sexy_vseparator,
					"new_update_button", &control_panel->new_update_button,
					
					"dm_frame", &control_panel->dm_frame,
					"dm_form_hbox", &control_panel->dm_form_hbox,
					"dm_frame_label", &control_panel->dm_frame_label,
					"dm_refresh", &control_panel->dm_refresh,
					"followers_combo_box", &control_panel->followers_combo_box,
					"followers_list_store", &control_panel->followers_list_store,
					"followers_send_dm", &control_panel->followers_send_dm,
					
					"control_panel_compact_view_toggle_button", &control_panel->compact_view_toggle_button,
					"control_panel_compact_view_image", &control_panel->compact_view_image,
					"control_panel_embed_toggle_button", &control_panel->embed_toggle_button,
					"control_panel_embed_image", &control_panel->embed_image,
					
					"dm_form_active_togglebutton", &control_panel->dm_form_active_togglebutton,
					"dm_form_active_image", &control_panel->dm_form_active_image,
					
					"control_panel_right_vseparator", &control_panel->right_vseparator,
					"controls_vbox", &control_panel->controls_vbox,
					
					"status_control_hbox", &control_panel->status_control_hbox,
					"reply_button", &control_panel->reply_button, 
					"forward_update_button", &control_panel->forward_update_button,
					"make_fave_button", &control_panel->make_fave_button,
				NULL
	);
	control_panel->followers_model=gtk_combo_box_get_model(control_panel->followers_combo_box);
	control_panel->previous_updates_tree_model=gtk_combo_box_get_model(GTK_COMBO_BOX(control_panel->sexy_entry_combo_box_entry));
	
	debug("Building & setting up new Tweet entry area.");
	control_panel_sexy_init();
	control_panel_reorder();
	debug("ControlPanel view & entry area setup.  Grabbing selected widgets.");
	control_panel_selected_update_buttons_setup(ui);
	control_panel_grab_widgets_compact_control_panel_hidden(ui);
	
	debug("ControlPanel's hotkey connected.  Connecting signal handlers.");
	control_panel_bind_hotkeys(ui);
	
	gtkbuilder_connect( ui, control_panel,
				"control_panel", "destroy", control_panel_destroy_cb,
				"control_panel", "delete_event", control_panel_delete_event_cb,
				"control_panel", "configure_event", control_panel_configure_event_cb,

				"view_user_profile_button", "clicked", online_service_request_selected_update_view_profile,
				"view_user_updates_button", "clicked", online_service_request_selected_update_view_updates,
				
				"user_follow_button", "clicked", online_service_request_selected_update_follow,
				"user_unfollow_button", "clicked", online_service_request_selected_update_unfollow,
				"user_block_button", "clicked", online_service_request_selected_update_block,
				
				"control_panel_sexy_entry_combo_box_entry", "changed", control_panel_select_previous_update,
				"sexy_send", "clicked", control_panel_send,
				"sexy_dm_button", "clicked", control_panel_send,
				"new_update_button", "clicked", control_panel_new_update,
				"dm_form_active_togglebutton", "toggled", control_panel_dm_show,
				
				"dm_refresh", "clicked", control_panel_dm_refresh,
				"followers_send_dm", "clicked", control_panel_send,
				
				"reply_button", "clicked", online_service_request_selected_update_reply,
				"forward_update_button", "clicked", online_service_request_selected_update_forward_update,
				"make_fave_button", "clicked", online_service_request_selected_update_save_fave,
			NULL
	);
	
	gchar *window_title=g_strdup_printf("%s - %s", _(GETTEXT_PACKAGE), _("Control Panel"));
	gtk_window_set_title(control_panel->control_panel, window_title);
	uber_free(window_title);
	
	if(!( parent && gconfig_if_bool(PREFS_CONTROL_PANEL_DIALOG, FALSE) )){
		debug("ControlPanel's set to be embed, no further setup needed.");
	}else{
		debug("Displaying ControlPanel as a stand alone dialog & setting ControlPanel's parent window..");
		gtk_widget_show_all(GTK_WIDGET(control_panel->control_panel));
		g_object_add_weak_pointer(G_OBJECT(control_panel->control_panel),(gpointer)&control_panel);
		gtk_window_set_transient_for(GTK_WINDOW(control_panel->control_panel), parent);
	}
	
	debug("Setting ControlPanel's embed state indicators.");
	control_panel_set_embed_toggle_and_image();
	
	debug("Disabling 'selected widgets' since no update could be selected when we 1st start.");
	control_panel_selected_update_buttons_show(FALSE, NULL);
	
	debug("Disabling & hiding ControlPanel's dm form since friends have not yet been loaded.");
	control_panel_dm_form_activate(FALSE);
	gtk_widget_hide(GTK_WIDGET(control_panel->dm_refresh));
	gtk_widget_hide( (GtkWidget *)control_panel->best_friend_dm_notification_label );
	
	online_service_request_unset_selected_update();
	
	gboolean compact;
	if((compact=gconfig_if_bool( PREFS_CONTROL_PANEL_COMPACT, TRUE)) != gtk_toggle_button_get_active(control_panel->compact_view_toggle_button) )
		gtk_toggle_button_set_active(control_panel->compact_view_toggle_button, compact);
	else
		control_panel_compact_view_display(compact);
	
	return control_panel;
}/*control_panel_new*/

static void control_panel_selected_update_author_best_friend_toggled(GtkToggleButton *best_friend_toggle_button){
	OnlineService *service=online_service_request_selected_update_get_service();
	const gchar *user_name=online_service_request_selected_update_get_user_name();
	if(!( G_STR_N_EMPTY(user_name) && service && control_panel->viewing_service==service && g_str_equal(control_panel->viewing_user, user_name) )) return;
	
	if(! online_service_is_user_best_friend( service, user_name) )
		online_service_request_selected_update_best_friend_add();
	else
		online_service_request_selected_update_best_friend_drop();
}/*control_panel_selected_update_author_best_friend_toggled(best_friend_toggle_widget);*/

GtkWindow *control_panel_get_window(void){
	return control_panel->control_panel;
}/*control_panel_get_window*/

void contol_panel_emulate_embed_toggle(void){
	gtk_toggle_button_set_active(control_panel->embed_toggle_button, !gtk_toggle_button_get_active(control_panel->embed_toggle_button) );
}/*contol_panel_emulate_embed_toggle();*/

void contol_panel_emulate_compact_view_toggle(void){
	gtk_toggle_button_set_active(control_panel->compact_view_toggle_button, !gtk_toggle_button_get_active(control_panel->compact_view_toggle_button) );
}/*contol_panel_emulate_compact_view_toggle();*/

void control_panel_set_embed_toggle_and_image(void){
	if(!gconfig_if_bool(PREFS_CONTROL_PANEL_DIALOG, FALSE)){
		debug("Setting ControlPanel's embed state indicators to split ControlPanel off into a floating window.");
		gtk_toggle_button_set_active(control_panel->embed_toggle_button, FALSE);
		gtk_widget_set_tooltip_markup(GTK_WIDGET(control_panel->embed_toggle_button), "<span weight=\"bold\">Split ControlPanel into its own window.</span>");
		gtk_image_set_from_icon_name(control_panel->embed_image, "gtk-go-up", ImagesMinimum);
	}else{
		debug("Setting ControlPanel's embed state indicators to embed ControlPanel into %s's main window.", PACKAGE_NAME);
		gtk_toggle_button_set_active(control_panel->embed_toggle_button, TRUE);
		gchar *tooltip_markup=g_strdup_printf("<span weight=\"light\">Move ControlPanel back into %s's main window.</span>", PACKAGE_NAME);
		gtk_widget_set_tooltip_markup(GTK_WIDGET(control_panel->embed_toggle_button), tooltip_markup );
		uber_free(tooltip_markup);
		gtk_image_set_from_icon_name(control_panel->embed_image, "gtk-go-down", ImagesMinimum);
	}
}/*control_panel_set_embed_toggle_and_image*/

static void control_panel_grab_widgets_compact_control_panel_hidden(GtkBuilder *ui){
	const gchar *update_view_containers[]={
		"user_vbox",
		"control_panel_left_vseparator",
		"status_view_vbox",
		"control_panel_view_controls_aspect_frame",
		"control_panel_right_vseparator",
		"controls_vbox"
	};
	
	GList *list=NULL;
	for(int i=0; i < G_N_ELEMENTS(update_view_containers); i++)
		list=g_list_append(list, (gtk_builder_get_object(ui, update_view_containers[i])) );
	control_panel->compact_control_panel_hidden_containers=list;
}/*control_panel_grab_widgets_for_compact_control_panel_hidden(ui);*/

static void control_panel_selected_update_buttons_setup(GtkBuilder *ui){
	const gchar *selected_update_buttons[]={
		"view_user_profile_button",
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
	control_panel->selected_update_buttons=list;
}/*control_panel_selected_widgets_setup*/

static void control_panel_bind_hotkeys(GtkBuilder *ui){
	g_signal_connect_after( control_panel->embed_toggle_button, "toggled", (GCallback)main_window_control_panel_set_embed, NULL );
	g_signal_connect_after( control_panel->compact_view_toggle_button, "toggled", (GCallback)control_panel_compact_view_toggled, NULL );
	g_signal_connect_after( control_panel->best_friend_toggle_button, "toggled", (GCallback)control_panel_selected_update_author_best_friend_toggled, NULL );
	g_signal_connect_after( control_panel->control_panel, "event-after", (GCallback)control_panel_sexy_select, NULL );
	g_signal_connect_after( control_panel->control_panel_embed, "event-after", (GCallback)control_panel_sexy_select, NULL );
	
	const gchar *hotkey_widgets[]={
		/* Connect the signals */
		"control_panel",
		"control_panel_embed",
		"user_vbox",
		
		"status_vbox",
		"status_view_vbox",
		"control_panel_update_composition_vbox",
		"notification_labels_hbox",
		"control_panel_update_composition_hbox",
		"control_panel_sexy_entry_combo_box_entry",
		"dm_frame",
		"followers_combo_box",
		
		"controls_vbox",
		"status_control_hbox",
	};
	GObject *widget=NULL;
	debug("ControlPanel interface loaded.  Setting up hotkeys.");
	for(int i=0; i<G_N_ELEMENTS(hotkey_widgets); i++){
		widget=NULL;
		if(!(widget=gtk_builder_get_object(ui, hotkey_widgets[i]))){
			debug("**ERROR:** Cannot bind %s's hotkeys to %s, the widget could not be found in %s", _(GETTEXT_PACKAGE), hotkey_widgets[i], GtkBuilderUI);
			continue;
		}
		
		debug("Binding %s's hotkeys to %s.", _(GETTEXT_PACKAGE), hotkey_widgets[i]);
		g_signal_connect_after(widget, "key-press-event",(GCallback)hotkey_pressed, widget);
	}
}/*control_panel_bind_hotkeys(ui);*/

void control_panel_compact_view_toggled(GtkToggleButton *compact_view_toggle_button){
	gboolean compact;
	if((compact=gtk_toggle_button_get_active(compact_view_toggle_button)) != gconfig_if_bool(PREFS_CONTROL_PANEL_COMPACT, TRUE) )
		gconfig_set_bool(PREFS_CONTROL_PANEL_COMPACT, compact);
	
	control_panel_compact_view_display(compact);
	
	if(!compact){
		gtk_widget_set_tooltip_markup(GTK_WIDGET(control_panel->compact_view_toggle_button), "<span weight=\"bold\" variant=\"small-caps\">Enlarge control panel.</span>\n<span style=\"italic\" size=\"small\">Shows all control panel buttons &amp; items.</span>");
		gtk_image_set_from_icon_name(control_panel->compact_view_image, "gtk-fullscreen", ImagesMinimum);
	}else{
		gtk_widget_set_tooltip_markup(GTK_WIDGET(control_panel->compact_view_toggle_button), "<span weight=\"light\">Compact control panel.</span>\n<span style=\"italic\" size=\"small\">Shows only enabled control panel buttons &amp; items.</span>");
		gtk_image_set_from_icon_name(control_panel->compact_view_image, "gtk-leave-fullscreen", ImagesMinimum);
	}
}/*control_panel_compact_view_toggled(control_panel->compact_view_toggle_button); */

static void control_panel_compact_view_display(gboolean compact){
	GList *l=NULL;
	debug("%sabling Control Panel compact view.  Control Panel's Compact View Toggle Button is %sactive", (compact ?"En" :"Dis" ), (gtk_toggle_button_get_active(control_panel->compact_view_toggle_button) ?"" :"in" ) );
	for(l=control_panel->compact_control_panel_hidden_containers; l; l=l->next)
		if(!compact)
			gtk_widget_show( (GtkWidget *)l->data );
		else
			gtk_widget_hide( (GtkWidget *)l->data );
	g_list_free(l);
	
	control_panel_scale(compact);
	gtk_widget_set_sensitive(GTK_WIDGET(control_panel->update_composition_vbox), TRUE );
	gtk_widget_show(GTK_WIDGET(control_panel->update_composition_vbox) );
	control_panel_sexy_select();
}/*control_panel_compact_view_display(TRUE); */

static void control_panel_scale(gboolean compact){
	gint h=0, w=0;
	gtk_window_get_size(main_window_get_window(), &w, &h );
	gint position=gtk_paned_get_position(main_window_get_main_paned() );
	position++;
}/* control_panel_scale(TRUE); */ 

void control_panel_best_friends_start_dm(OnlineService *service, const gchar *user_name){
	control_panel->best_friends_service=service;
	control_panel->best_friends_user_name=g_strdup(user_name);
	gchar *best_friend_dm_markup_message=NULL;
	gtk_label_set_markup( control_panel->best_friend_dm_notification_label, (best_friend_dm_markup_message=g_strdup_printf("<b>Direct Message %s:</b>", control_panel->best_friends_user_name)) );
	uber_free(best_friend_dm_markup_message);
	gtk_widget_show( (GtkWidget *)control_panel->best_friend_dm_notification_label );
}/*void control_panel_best_friends_start_dm(service, user_name);*/

static void control_panel_selected_update_buttons_show(gboolean selected_update, const gchar *user_name){
	main_window_selected_update_image_menu_items_show(selected_update);
	GList *l=NULL;
	for(l=control_panel->selected_update_buttons; l; l=l->next)
		gtk_widget_set_sensitive(GTK_WIDGET(l->data), selected_update);
	g_list_free(l);
	if(selected_update && G_STR_N_EMPTY(user_name) )
		gtk_toggle_button_set_active( control_panel->best_friend_toggle_button, (online_service_is_user_best_friend(online_service_request_selected_update_get_service(), user_name)) );
	control_panel_sexy_select();
}/*control_panel_selected_widgets_show*/

static void control_panel_dm_form_set_toggle_and_image(void){
	if(!gtk_toggle_button_get_active(control_panel->dm_form_active_togglebutton)){
		debug("Setting ControlPanel's dm form toggle button to enable the DM form.");
		gtk_widget_set_tooltip_markup(GTK_WIDGET(control_panel->dm_form_active_togglebutton), "<span weight=\"bold\">Compose DM's to any of your friends.</span>");
		gtk_image_set_from_icon_name(control_panel->dm_form_active_image, "gtk-edit", ImagesMinimum);
	}else{
		debug("Setting ControlPanel's dm form toggle button to disable the DM form.");
		gtk_widget_set_tooltip_markup(GTK_WIDGET(control_panel->dm_form_active_togglebutton), "<span weight=\"light\">Hide the DM form &amp; area.  You'll still be able to send DMs in the future by clicking this button again.</span>");
		gtk_image_set_from_icon_name(control_panel->dm_form_active_image, "gtk-close", ImagesMinimum);
	}
}/*control_panel_dm_form_set_toggle_and_image*/

static void control_panel_dm_form_activate(gboolean dm_activate){
	control_panel_dm_form_set_toggle_and_image();
	
	if(!dm_activate){
		gtk_widget_hide(GTK_WIDGET(control_panel->dm_frame));
		gtk_widget_grab_focus(GTK_WIDGET(control_panel->sexy_entry));
	}else{
		gtk_widget_show(GTK_WIDGET(control_panel->dm_frame));
		gtk_widget_grab_focus(GTK_WIDGET(control_panel->followers_combo_box));
	}
}/*control_panel_dm_form_activate*/

static void control_panel_sexy_init(void){
	/* Set-up expand ControlPanel.  Used to view updates in detailed & send updates and DMs. */
	debug("Creating Tweet's service link area, 'control_panel->sexy_to', using sexy label interface.");
	control_panel->sexy_to=label_new();
	gtk_box_pack_start(
			GTK_BOX(control_panel->status_view_vbox),
			GTK_WIDGET(control_panel->sexy_to),
			FALSE, FALSE, 5
	);
	g_object_set(control_panel->sexy_to, "yalign", 0.00, "xalign", 1.00, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL);
	gtk_widget_show(GTK_WIDGET(control_panel->sexy_to));
	
	debug("Creating Tweet's title area, 'control_panel->sexy_from', using sexy label interface.");
	control_panel->sexy_from=label_new();
	gtk_box_pack_start(
			GTK_BOX(control_panel->status_view_vbox),
			GTK_WIDGET(control_panel->sexy_from),
			FALSE, FALSE, 5
	);
	
	debug("Aligning Tweet's view area, 'control_panel->update_datetime_label',");
	g_object_set(control_panel->update_datetime_label, "yalign", 0.00, "xalign", 1.00, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL);
	
	g_object_set(control_panel->sexy_from, "yalign", 0.00, "xalign", 0.00, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL);
	gtk_widget_show(GTK_WIDGET(control_panel->sexy_from));
	
	debug("Creating Tweet's view area, 'control_panel->sexy_update', using sexy label interface.");
	control_panel->sexy_update=label_new();
	gtk_box_pack_start(
			GTK_BOX(control_panel->status_view_vbox),
			GTK_WIDGET(control_panel->sexy_update),
			TRUE, TRUE, 5
	);
	g_object_set(control_panel->sexy_update, "yalign", 0.00, "xalign", 0.00, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL);
	gtk_widget_show(GTK_WIDGET(control_panel->sexy_update));
	
	debug("Creating Tweet's entry, 'control_panel->sexy_entry', using SexyEntry, and adding it to ControlPanel's 'sexy_entry_combo_box_entry'.");
	control_panel->sexy_entry=(SexySpellEntry *)sexy_spell_entry_new();
	control_panel->sexy_entry=g_object_ref_sink(control_panel->sexy_entry);
	gtk_container_remove(GTK_CONTAINER(control_panel->sexy_entry_combo_box_entry), gtk_bin_get_child(GTK_BIN(control_panel->sexy_entry_combo_box_entry)));
	gtk_container_add(GTK_CONTAINER(control_panel->sexy_entry_combo_box_entry), GTK_WIDGET(control_panel->sexy_entry));
	control_panel->sexy_completion=gtk_entry_completion_new();
	gtk_entry_completion_set_model(control_panel->sexy_completion, control_panel->previous_updates_tree_model);
	gtk_entry_set_completion(GTK_ENTRY(control_panel->sexy_entry), control_panel->sexy_completion);
	gtk_widget_show(GTK_WIDGET(control_panel->sexy_entry));
		
	g_signal_connect_after(control_panel->sexy_entry, "key-press-event", G_CALLBACK(hotkey_pressed), NULL);
	g_signal_connect_after(control_panel->sexy_entry, "key-release-event", G_CALLBACK(control_panel_sexy_entry_character_count), control_panel->char_count);
	g_signal_connect(control_panel->sexy_entry, "activate", G_CALLBACK(control_panel_send), NULL);
	g_signal_connect_after(control_panel->followers_combo_box, "changed", G_CALLBACK(control_panel_sexy_select), control_panel->followers_combo_box);
}/*control_panel_sexy_init();*/

static void control_panel_reorder(void){
	debug("Ordering ControlPanel.");
	debug("Setting 'sexy_to' as 'status_view_vbox' 1st widget.");
	gtk_box_reorder_child(
			GTK_BOX(control_panel->status_view_vbox),
			GTK_WIDGET(control_panel->sexy_to),
			0
	);
	
	debug("Setting 'sexy_from' as 'status_view_vbox' 2nd widget.");
	gtk_box_reorder_child(
			GTK_BOX(control_panel->status_view_vbox),
			GTK_WIDGET(control_panel->sexy_from),
			1
	);
	
	debug("Setting 'update_datetime_label' as 'status_view_vbox' 3rd widget.");
	gtk_box_reorder_child(
			GTK_BOX(control_panel->status_view_vbox),
			GTK_WIDGET(control_panel->update_datetime_label),
			2
	);
		
	debug("Setting 'sexy_update' as 'status_view_vbox' 4th widget.");
	gtk_box_reorder_child(
			GTK_BOX(control_panel->status_view_vbox),
			GTK_WIDGET(control_panel->sexy_update),
			3
	);
}/*control_panel_reorder*/


void control_panel_view_selected_update(OnlineService *service, const gdouble id, const gdouble user_id, const gchar *user_name, const gchar *nick_name, const gchar *date, const gchar *sexy_update, const gchar *text_update, GdkPixbuf *pixbuf){
	if(!id)
		online_service_request_unset_selected_update();
	else
		online_service_request_set_selected_update(service, id, user_id, user_name, text_update);
	
	debug("%s the Control Panel.", (id ?_("Enlarging") :"") );
	control_panel_compact_view_display( (id ?FALSE :gconfig_if_bool( PREFS_CONTROL_PANEL_COMPACT, TRUE) ) );
	
	debug("%sabling 'selected_update_buttons'.", (id ?"En" :"Dis") );
	control_panel_selected_update_buttons_show((id ?TRUE :FALSE), (G_STR_EMPTY(user_name) ?NULL :user_name ) );
	
	const gchar *uri_scheme_suffix=(service->https?"s":"");
	
	control_panel->viewing_service=service;
	if(control_panel->viewing_user) uber_free(control_panel->viewing_user);
	control_panel->viewing_user=g_strdup((G_STR_EMPTY(user_name) ?"" :user_name ) );
	
	if(!id) main_window_set_statusbar_msg(NULL);
	
	gchar *sexy_text=NULL;
	if(!id)
		sexy_text=g_strdup("");
	else
		sexy_text=g_strdup_printf("<span size=\"small\" weight=\"light\" variant=\"smallcaps\">To: %s<a href=\"http%s://%s/%s\">&lt;@%s on %s&gt;</a></span>", service->nick_name, uri_scheme_suffix, service->uri, service->user_name, service->user_name, service->uri);
	debug("Setting 'sexy_to' for 'selected_update':\n\t\t%s.", sexy_text);
	label_set_text(service, control_panel->sexy_to, sexy_text, FALSE, TRUE);
	uber_free(sexy_text);
	
	if(!(G_STR_EMPTY(nick_name) && G_STR_EMPTY(user_name) )){
		if(! G_STR_EMPTY(user_name) )
		sexy_text=g_strdup_printf("<span weight=\"ultrabold\">From: %s <a href=\"http%s://%s/%s\">&lt;@%s on %s&gt;</a></span>", nick_name, uri_scheme_suffix, service->uri, user_name, user_name, service->uri);
	}else
		sexy_text=g_strdup("");
	debug("Setting 'sexy_from' for 'selected_update':\n\t\t%s.", sexy_text);
	label_set_text(service, control_panel->sexy_from, sexy_text, FALSE, TRUE);
	uber_free(sexy_text);
	
	if(!( G_STR_EMPTY(date)))
		sexy_text=g_markup_printf_escaped("<span style=\"italic\">[%s]</span>", date);
	else
		sexy_text=g_strdup("");
	gtk_label_set_markup(control_panel->update_datetime_label, sexy_text);
	uber_free(sexy_text);
	
	
	/*
	 * gchar sexy_update has already been formatted to include urls, hyperlinks, titles, & etc.
	 * So we just set it as a SexyLable & bypass Label
	 */
	debug("Setting 'sexy_update' for 'selected_update':\n\t\t%s.", sexy_update);
	if(!(gconfig_if_bool(PREFS_URLS_EXPANSION_SELECTED_ONLY, TRUE)))
		sexy_url_label_set_markup(SEXY_URL_LABEL(control_panel->sexy_update), sexy_update);
	else
		label_set_text(service, control_panel->sexy_update, sexy_update, TRUE, TRUE);
	
	if(!pixbuf)
		gtk_image_set_from_icon_name(control_panel->user_image, GETTEXT_PACKAGE, ImagesExpanded);
	else{
		debug("Setting avatar for the user who wrote the 'selected_update'.");
		GdkPixbuf *resized=NULL;
		if(!( resized=images_expand_pixbuf(pixbuf)))
			gtk_image_set_from_pixbuf(control_panel->user_image, pixbuf);
		else{
			gtk_image_set_from_pixbuf(control_panel->user_image, resized);
			g_object_unref(resized);
		}
	}
	
	debug("Selecting 'sexy_entry' for entering a new update.");
	control_panel_sexy_select();
}/*control_panel_view_selected_update*/

static gshort control_panel_sexy_entry_update_length(gchar *update){
	gushort character_count=0;
	gushort me_match=0;
	gint		replace_me=0;
	gconfig_get_int_or_default(PREFS_UPDATES_REPLACE_ME_W_NICK, &replace_me, 2);
	while(*update){
		unsigned char l=*update++;
		if(l=='<' || l=='>')
			character_count+=3;
		else if(l=='/' && replace_me>0)
			me_match=1;
		else if(l=='m' && me_match==1)
			me_match=2;
		else if(l=='e' && me_match==2)
			character_count+=online_services_get_length_of_longest_replacement();
		else if(me_match)
			me_match=0;
		
		character_count++;
	}
	
	return TWEET_MAX_CHARS-character_count;
}/*control_panel_sexy_entry_update_length(update);*/

static void control_panel_sexy_entry_character_count(GtkEntry *entry, GdkEventKey *event, GtkLabel *update_character_counter){
	control_panel->sexy_position=gtk_editable_get_position(GTK_EDITABLE(entry));
	gshort remaining_character_count=control_panel_sexy_entry_update_length(entry->text);
	gchar *remaining_characters_markup_label=NULL;
	if(remaining_character_count < 0){
		if(!gconfig_if_bool(PREFS_DISABLE_UPDATE_LENGTH_ALERT, FALSE))
			control_panel_beep();
		remaining_characters_markup_label=g_strdup_printf("<span size=\"small\" foreground=\"red\">%d</span>", remaining_character_count);
	}else{
		if(TWEET_MAX_CHARS==remaining_character_count && (control_panel->best_friends_service ||  in_reply_to_status_id) ){
			in_reply_to_status_id=0.0;
			in_reply_to_service=NULL;
			if(control_panel->best_friends_service || G_STR_N_EMPTY(control_panel->best_friends_user_name) ){
				control_panel->best_friends_service=NULL;
				if(control_panel->best_friends_user_name) uber_free(control_panel->best_friends_user_name);
				gtk_widget_hide( (GtkWidget *)control_panel->best_friend_dm_notification_label );
			}
		}
		remaining_characters_markup_label=g_strdup_printf("<span size=\"small\" foreground=\"green\">%d</span>", remaining_character_count);
	}
	
	gtk_label_set_markup(update_character_counter, remaining_characters_markup_label);
	uber_free(remaining_characters_markup_label);
}/*control_panel_sexy_entry_character_count*/

void control_panel_beep(void){
	if(!gconfig_if_bool(PREFS_DISABLE_SYSTEM_BELL, FALSE))
		gtk_widget_error_bell(GTK_WIDGET(control_panel->sexy_entry));
}/*control_panel_beep*/

void control_panel_sexy_select(void){
	if(gtk_widget_has_focus(GTK_WIDGET(control_panel->sexy_entry))) return;
	gtk_widget_grab_focus(GTK_WIDGET(control_panel->sexy_entry));
	gint sexy_position=-1;
	if( control_panel->sexy_position > 0 && control_panel->sexy_position <= gtk_entry_get_text_length((GtkEntry *)control_panel->sexy_entry) )
		sexy_position=control_panel->sexy_position;
	gtk_entry_set_position(GTK_ENTRY(control_panel->sexy_entry), sexy_position );
}/*control_panel_sexy_select*/

void control_panel_sexy_prefix_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	control_panel_sexy_prefix_string((const gchar *)str);
	uber_free(str);
}/*control_panel_sexy_prefix_char*/

void control_panel_sexy_prefix_string(const gchar *str){
	control_panel_sexy_puts(str, 0);
}/*control_panel_sexy_prefix_string*/

void control_panel_sexy_set(gchar *text){
	gtk_entry_set_text(GTK_ENTRY(control_panel->sexy_entry), (text==NULL ?(gchar *)"" :text));
	control_panel_sexy_select();
}/*control_panel_sexy_set*/

void control_panel_sexy_insert_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	control_panel_sexy_insert_string((const gchar *)str);
	uber_free(str);
}/*control_panel_sexy_insert_char*/

void control_panel_sexy_insert_string(const gchar *str){
	control_panel_sexy_puts(str, gtk_editable_get_position(GTK_EDITABLE(control_panel->sexy_entry)) );
}/*control_panel_sexy_insert_string*/

void control_panel_sexy_append_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	control_panel_sexy_append_string((const gchar *)str);
	uber_free(str);
}/*control_panel_sexy_append_char*/

void control_panel_sexy_append_string(const gchar *str){
	control_panel_sexy_puts(str,(gint)gtk_entry_get_text_length(GTK_ENTRY(control_panel->sexy_entry)) );
}/*control_panel_sexy_append_string*/

gint control_panel_sexy_puts(const gchar *str, gint position_after){
	gint position_prior=gtk_editable_get_position(GTK_EDITABLE(control_panel->sexy_entry));
	gtk_editable_insert_text(GTK_EDITABLE(control_panel->sexy_entry), str, -1, &position_after );
	if(position_after>position_prior)
		gtk_entry_set_position(GTK_ENTRY(control_panel->sexy_entry), position_after);
	control_panel_sexy_select();
	return position_after;
}/*control_panel_sexy_puts*/

void control_panel_hide_previous_updates(void){
	g_signal_emit_by_name(control_panel->sexy_entry_combo_box_entry, "popup");
}/*control_panel_hide_previous_updates();*/

void control_panel_show_previous_updates(void){
	g_signal_emit_by_name(control_panel->sexy_entry_combo_box_entry, "popup");
}/*control_panel_show_previous_updates();*/

void control_panel_send(GtkWidget *activated_widget){
	gchar *user_name=NULL;
	
	const gchar *text=GTK_ENTRY(control_panel->sexy_entry)->text;
	if(G_STR_EMPTY(text)){
		gchar *reply_to_string=online_service_request_selected_update_reply_to_strdup(FALSE);
		if(!online_service_request_selected_update_get_user_name())
			control_panel_beep();
		else if(g_str_has_prefix(text, reply_to_string))
			control_panel_beep();
		else
			online_service_request_selected_update_reply();
		uber_free(reply_to_string);
		return;
	}
	
	if(activated_widget==GTK_WIDGET(control_panel->sexy_dm_button) ){
		const gchar *user_name;
		if(!(( user_name=online_service_request_selected_update_get_user_name()) && G_STR_N_EMPTY(user_name) )) return control_panel_beep();
		control_panel_sexy_send(online_service_request_selected_update_get_service(), user_name);
		return;
	}
	
	if((activated_widget==GTK_WIDGET(control_panel->followers_send_dm)) &&(GTK_WIDGET_IS_SENSITIVE(control_panel->followers_combo_box)) &&G_STR_N_EMPTY((user_name=gtk_combo_box_get_active_text(control_panel->followers_combo_box)) ) ){
		uber_free(user_name);
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		User *user=NULL;
		gtk_combo_box_get_active_iter(control_panel->followers_combo_box, iter);
		gtk_tree_model_get(
					control_panel->followers_model,
					iter,
						USER_POINTER, &user,
					-1
		);
		control_panel_sexy_send(user->service, user->user_name);
		uber_free(iter);
		return;
	}
	
	if(control_panel->best_friends_service && control_panel->best_friends_user_name)
		return control_panel_sexy_send(control_panel->best_friends_service, control_panel->best_friends_user_name);
	
	control_panel_sexy_send(NULL, NULL);
}/*control_panel_send*/

void control_panel_new_update(void){
	control_panel_view_selected_update((selected_service ?selected_service :online_services_connected_get_first()), 0, 0, NULL, NULL, NULL, NULL, NULL, NULL);
	
	if(control_panel->best_friends_service) control_panel->best_friends_service=NULL;
	if(control_panel->best_friends_user_name) uber_free(control_panel->best_friends_user_name);
	
	if(in_reply_to_service) in_reply_to_service=NULL;
	if(in_reply_to_status_id) in_reply_to_status_id=0;
	
	gtk_widget_hide( (GtkWidget *)control_panel->best_friend_dm_notification_label );
	online_service_request_unset_selected_update();
	control_panel_sexy_set((gchar *)"");
	
	if(gtk_toggle_button_get_active(control_panel->best_friend_toggle_button) )
		gtk_toggle_button_set_active(control_panel->best_friend_toggle_button, FALSE);
	
	geometry_load();
	control_panel_sexy_select();
}/*control_panel_new_update();*/

static void control_panel_sexy_send(OnlineService *service, const gchar *user_name){
	if(!((GTK_ENTRY(control_panel->sexy_entry)->text) &&(control_panel_sexy_entry_update_length(GTK_ENTRY(control_panel->sexy_entry)->text) > -1) )){
		if(!gconfig_if_bool(PREFS_DISABLE_UPDATE_LENGTH_ALERT, FALSE))
			control_panel_beep();
		return;
	}
	
	if(!(service && user_name && G_STR_N_EMPTY(user_name) ))
		network_post_status(GTK_ENTRY(control_panel->sexy_entry)->text);
	else
		network_send_message(service, user_name, GTK_ENTRY(control_panel->sexy_entry)->text);
	
	control_panel_sexy_append(GTK_ENTRY(control_panel->sexy_entry)->text, control_panel);
	control_panel_sexy_set((gchar *)"");
}/*control_panel_sexy_send(online_service_request_selected_update_get_service(), online_service_request_selected_update_get_user_name() );*/

void control_panel_sexy_send_dm(void){
	OnlineService *service=NULL;
	const gchar *user_name=NULL;
	if((service=control_panel->best_friends_service) && G_STR_N_EMPTY((user_name=control_panel->best_friends_user_name) ) )
		return control_panel_sexy_send(service, user_name );
	
	if((service=online_service_request_selected_update_get_service()) && G_STR_N_EMPTY((user_name=online_service_request_selected_update_get_user_name()) ) )
		return control_panel_sexy_send(service, user_name);
	
	if((GTK_WIDGET_IS_SENSITIVE(control_panel->followers_combo_box)) && G_STR_N_EMPTY((user_name=gtk_combo_box_get_active_text(control_panel->followers_combo_box)) )  )
		return control_panel_send(GTK_WIDGET(control_panel->followers_send_dm) );
	
	control_panel_beep();
}/*control_panel_sexy_send_dm();*/


static void control_panel_sexy_append(const gchar *update, ControlPanel *control_panel){
	if(G_STR_EMPTY(update)) return;
	const gint max_updates=50;
	static gboolean first_update=TRUE;
	
	GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
	gtk_list_store_prepend(control_panel->previous_updates_list_store, iter);
	gtk_list_store_set(
				control_panel->previous_updates_list_store, iter,
					GSTRING_UPDATE, g_strdup(update),
					IN_REPLY_TO_SERVICE,(in_reply_to_service?in_reply_to_service:NULL),
					IN_REPLY_TO_STATUS_ID,(in_reply_to_status_id?in_reply_to_status_id:0.0),
					GSTRING_NULL, g_strdup(""),
				-1
	);
	uber_free(iter);
	
	if(first_update){
		gtk_combo_box_entry_set_text_column(control_panel->sexy_entry_combo_box_entry, GSTRING_NULL);
		gtk_entry_completion_set_text_column(control_panel->sexy_completion, GSTRING_UPDATE);
		first_update=FALSE;
	}
	
	if(control_panel->updates<=max_updates){
		control_panel->updates++;
		return;
	}
	iter=g_new0(GtkTreeIter, 1);
	GtkTreePath *path=gtk_tree_path_new_from_indices(max_updates, -1);
	debug("Removing saved update #%d.  Total update saved: %d", max_updates, control_panel->updates);
	if(!(gtk_tree_model_get_iter(control_panel->previous_updates_tree_model, iter, path)))
		debug("Removing saved update #%d failed: it appears to be an invalid iter for the list store.", max_updates);
	else{
		debug("Removing iter at index: %d", max_updates);
		gtk_list_store_remove(control_panel->previous_updates_list_store, iter);
	}
	uber_free(iter);
}/*control_panel_sexy_append(GTK_ENTRY(control_panel->sexy_entry), control_panel);*/

static void control_panel_select_previous_update(GtkComboBoxEntry *sexy_entry_combo_box_entry, ControlPanel *control_panel){
	GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
	if(!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(control_panel->sexy_entry_combo_box_entry), iter)){
		uber_free(iter);
		return;
	}
	
	gchar	*update=NULL;
	gtk_tree_model_get(
				control_panel->previous_updates_tree_model, iter,
					GSTRING_UPDATE, &update,
					IN_REPLY_TO_SERVICE, &in_reply_to_service,
					IN_REPLY_TO_STATUS_ID, &in_reply_to_status_id,
				-1
	);
	if(!G_STR_N_EMPTY(update)){
		if(update) uber_free(update);
		uber_free(iter);
		return;
	}
	control_panel_sexy_set(g_strdup(update));
	uber_free(update);
	uber_free(iter);
}/*control_panel_select_previous_update(control_panel->sexy_entry_combo_box_entry, control_panel);*/

static void control_panel_free_updates(ControlPanel *control_panel){
	debug("Removing all %d saved updates from this session.", control_panel->updates);
	
	for(guint i=0; i <= control_panel->updates; i++){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!(gtk_tree_model_get_iter(control_panel->previous_updates_tree_model, iter, path)))
			debug("Removing iter at index: %d failed.  Unable to retrieve iter from path.", i);
		else{
			debug("Destroying saved update #%d.", i);
			gtk_list_store_remove(control_panel->previous_updates_list_store, iter);
		}
		gtk_tree_path_free(path);
		uber_free(iter);
	}
}/*control_panel_free_updates(control_panel);*/

void control_panel_new_dm(void){
	gtk_toggle_button_set_active(control_panel->dm_form_active_togglebutton, !gtk_toggle_button_get_active(control_panel->dm_form_active_togglebutton));
}/*control_panel_new_dm*/

static void control_panel_dm_show(GtkToggleButton *toggle_button){
	if(!gtk_toggle_button_get_active(control_panel->dm_form_active_togglebutton)){
		debug("Disabled ControlPanel's dm form.");
		control_panel_dm_form_activate(FALSE);
		return;
	}
	online_service_request_popup_select_service();
	
	if(!( selected_service)) {
		if(gtk_toggle_button_get_active(control_panel->dm_form_active_togglebutton))
			gtk_toggle_button_set_active(control_panel->dm_form_active_togglebutton, FALSE);
		return;
	}
	
	debug("Enabling ControlPanel's dm form.");
	control_panel_dm_form_activate(TRUE);
	users_glist_get(GetFollowers, FALSE, control_panel_dm_data_fill);
}/*control_panel_dm_show*/

static void control_panel_dm_refresh(void){
	online_service_request_popup_select_service();
	users_glist_get(GetFollowers, TRUE, control_panel_dm_data_fill);
}/*control_panel_dm_refresh*/

void control_panel_dm_data_fill(GList *followers){
	if(!followers) return;
	static gboolean first_fill=TRUE;
	
	User		*user;
	gchar		*null_friend=g_strdup("");
	GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
	
	gtk_list_store_clear(control_panel->followers_list_store);
	
	gtk_list_store_append(control_panel->followers_list_store, iter);
	gtk_list_store_set(
				control_panel->followers_list_store, iter,
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
		gtk_list_store_append(control_panel->followers_list_store, iter);
		gtk_list_store_set(
				control_panel->followers_list_store,
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
	gtk_label_set_markup(control_panel->dm_frame_label, new_label);
	gtk_label_set_use_underline(control_panel->dm_frame_label, TRUE);
	gtk_label_set_single_line_mode(control_panel->dm_frame_label, TRUE);
	uber_free(new_label);
	
	if(first_fill) first_fill=FALSE;
}/*control_panel_dm_data_fill*/

/********************************************************
 *                       eof                            *
 ********************************************************/
