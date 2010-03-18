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

#include "online-services.rest-uris.defines.h"
#include "online-services.typedefs.h"
#include "online-services.types.h"
#include "online-service.types.h"
#include "online-services.h"

#include "online-service.h"
#include "online-service-request.h"

#include "best-friends.h"

#include "online-services-dialog.h"

#include "users.types.h"
#include "users.h"

#include "network.h"
#include "users-glists.h"
#include "gtkbuilder.h"
#include "hotkeys.h"

#include "main-window.h"
#include "geometry.h"
#include "preferences.defines.h"

#include "www.h"

#include "uberchick-label.h"

#include "uberchick-sexy-entry-completion.h"

#include "ui-utils.h"
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
	GList			*selected_update_widgets;
	
	/* GtkWidgets for viewing user details & for controlling one's relationship to them. */
	GtkHBox			*user_vbox;
	
	GtkImage		*user_image;
	GtkVBox			*selected_update_controls_vbox;
	UberChickLabel		*sexy_selected_update_author;
	
	/* Buttons for viewing details about the auther of the current selected/extended Update. */
	GtkToggleButton		*best_friend_toggle_button;
	GtkButton		*view_user_profile_button;
	
	/* Buttons for viewing updates, new & all, update from the selected updates author or the selected update's retweets. */
	GtkButton		*view_user_unread_updates_button;
	GtkButton		*view_user_updates_button;
	GtkButton		*view_forwards_button;
	
	/* Buttons for viewing details about the user of the current selected/extended Update. */
	GtkVBox			*user_relationship_controls_hbox;
	GtkButton		*user_follow_button;
	GtkButton		*user_unfollow_button;
	GtkButton		*user_block_button;
	
	/* Just for padding & appearance. */
	GtkVSeparator		*left_vseparator;
	
	/* View & Sending Updates. */
	GtkVBox			*status_vbox;
	
	/* UpdateViewer */
	GtkVBox			*status_view_vbox;
	UberChickLabel		*sexy_to;
	UberChickLabel		*sexy_from;
	GtkLabel		*update_datetime_label;
	UberChickLabel		*sexy_update;
	
	/* Stuff for actually writing your updates & DMs. */
	GtkVBox			*update_composition_vbox;
	GtkHBox			*notification_labels_hbox;
	GtkLabel		*best_friend_dm_notification_label;
	GtkLabel		*char_count;
	
	GtkHBox			*update_composition_hbox;
	
	/* BEGIN: UberChickSexyEntryCompletion::private values. */
	gint			max_updates;
	gint			total_updates;
	GtkComboBoxEntry	*sexy_entry_combo_box_entry;
	GtkListStore		*previous_updates_list_store;
	GRegex			*upper_case_regex;
	GRegex			*username_regex;
	SexySpellEntry		*sexy_entry;
	GtkEntryCompletion	*sexy_completion;
	gint			sexy_entry_text_position;
	/* END: UberChickSexyEntryCompletion::private values. */
	
	/* Info on the update being viewed to avoid issues with the 'best friends' toggle button. */
	OnlineService		*viewing_service;
	UpdateType		viewing_update_type;
	gchar			*viewing_user_name;
	gdouble			viewing_user_id;
	gchar			*viewing_user_id_str;
	gdouble			viewing_update_id;
	gchar			*viewing_update_id_str;
	gchar			*viewing_update_text;
	
	/* For sending one of your best friends a dm. */
	OnlineService		*best_friends_service;
	gchar			*best_friends_user_name;
	
	GtkButton		*shorten_uri_button;
	
	GtkButton		*sexy_send;
	GtkButton		*sexy_dm_button;
	GtkVSeparator		*sexy_vseparator;
	
	/* The list & actual GtkWidgets for sending DMs. */
	GtkFrame		*dm_frame;
	GtkLabel		*dm_frame_label;
	GtkHBox			*dm_form_hbox;
	GtkButton		*dm_refresh;
	GtkComboBox		*followers_combo_box;
	GtkListStore		*followers_list_store;
	GtkButton		*followers_send_dm;
	
	GtkToggleButton		*dm_form_active_toggle_button;
	GtkImage		*dm_form_active_image;
	
	/* Container for UpdateViewer's right side widgets. */
	GtkVBox			*controls_vbox;
	/* Update, status, & DM writing area & widgets. */
	/* Buttons for stuff to do with the current selected & extended update. */
	GtkHBox			*message_controls_hbox;
	GtkButton		*reply_button;
	GtkButton		*forward_update_button;
	GtkButton		*retweet_button;
	GtkButton		*make_fave_button;
	GtkButton		*unfave_button;
	GtkButton		*destroy_update_button;
	
	
	/* ToggleButtons for controlling the update viewer's appearance. */
	GtkVPaned		*view_controls_vpaned;
	GtkHPaned		*status_view_hpaned;
	GtkHBox			*update_viewer_appearance_controls_hbox;
	
	GtkToggleButton		*compact_view_toggle_button;
	GtkImage		*compact_view_image;
	
	/* Togglebutton & image used to indicate & switch between embed update-viewer & floating. */
	GtkToggleButton		*embed_toggle_button;
	GtkImage		*embed_image;
	
	/* Enables/disables the advanced posting options check buttons. */
	GtkToggleButton		*online_services_view_toggle_button;
	GtkImage		*online_services_view_toggle_image;
	
	GtkVBox			*online_services_vbox;
	GtkHBox			*online_services_controls_hbox;
	GtkLabel		*online_services_controls_label;
	GtkToolbar		*online_services_controls_toolbar;
	GtkToolButton		*online_services_controls_edit_accounts_tool_button;
	GtkToolButton		*online_services_controls_reset_accounts_tool_button;
	
	GtkScrolledWindow	*online_services_accounts_scrolled_window;
	gint			online_services_accounts_total;
	gint			online_services_accounts_with_post_to_enabled;
	gint			online_services_accounts_with_post_to_enabled_toggle_enabled;
	SexyTreeView		*online_services_accounts_sexy_tree_view;
	GtkTreeStore		*online_services_accounts_tree_store;
	GtkTreeViewColumn	*online_services_accounts_sexy_tree_view_key_tree_view_column;
	GtkCellRendererText	*online_services_accounts_sexy_tree_view_key_cell_renderer_text;
	GtkTreeViewColumn	*online_services_accounts_sexy_tree_view_post_to_enabled_tree_view_column;
	GtkCellRendererToggle	*online_services_accounts_sexy_tree_view_post_to_enabled_cell_renderer_toggle;
};

typedef enum{
      POSTABLE_ONLINE_SERVICE,
      POSTABLE_ONLINE_SERVICE_KEY,
      POSTABLE_ONLINE_SERVICE_POST_TOGGLE_ENABLED,
      POSTABLE_ONLINE_SERVICE_POST_TO_ENABLED,
      POSTABLE_ONLINE_SERVICE_URI,
      POSTABLE_ONLINE_SERVICE_INFO_STRING,
}UpdateViewerPostToEnabledTreeStoreColumns;

typedef enum{
	USER_LABEL,
	USER_POINTER,
} FriendsDMColumns;

typedef enum{
	GSTRING_UPDATE = 0,
} PeviousUpdateInfo;


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/

#define	DEBUG_DOMAINS	"UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Updates:Requests:Users:Start-Up:update-viewer.c"
#include "debug.h"

#define GTK_BUILDER_UI_FILENAME "update-viewer"
#define	UPDATE_MAX_LENGTH	140

static UpdateViewer *update_viewer=NULL;


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static void update_viewer_destroy_cb(GtkWidget *window, UpdateViewer *update_viewer); 
static gboolean update_viewer_delete_event_cb(GtkWidget *window, GdkEvent *event, UpdateViewer *update_viewer);
static gboolean update_viewer_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, UpdateViewer *update_viewer);
static gboolean update_viewer_configure_event_timeout_cb(GtkWidget *widget);

static UpdateViewer *update_viewer_init(void);
static void update_viewer_sexy_init(void);
static void update_viewer_update_view_reorder(void);

static void update_viewer_selected_update_author_best_friend_toggled(GtkToggleButton *best_friend_toggle_button, UpdateViewer *update_viewer);

static void update_viewer_selected_update_buttons_setup(GtkBuilder *ui, UpdateViewer *update_viewer);
static void update_viewer_bind_hotkeys(GtkBuilder *ui);
static void update_viewer_selected_update_buttons_show(OnlineService *service, const gchar *user_name, gboolean selected_update);

static void update_viewer_grab_widgets_compact_update_viewer_hidden(GtkBuilder *ui);
static void update_viewer_compact_view_display(gboolean compact);
static void update_viewer_scale(gboolean compact);

static void update_viewer_sexy_entry_setup(UpdateViewer *update_viewer);
static gint update_viewer_sexy_entry_update_length(gchar *update);
static void update_viewer_sexy_entry_update_remaining_character_count(void);

static void update_viewer_sexy_entry_icon_clicked(GtkEntry *sexy_entry, GtkEntryIconPosition icon_clicked, GdkEvent *event);
static gboolean update_viewer_sexy_entry_confirm_clear(void);
static void update_viewer_sexy_send(OnlineService *service, const gchar *user_name);

static gint update_viewer_previous_maxium_updates_validate(UpdateViewer *update_viewer);
static void update_viewer_previous_updates_load(UpdateViewer *update_viewer);

static void update_viewer_previous_update_add(UpdateViewer *update_viewer, const gchar *update, gint list_store_index);
#define	update_viewer_previous_update_restore(update_viewer, update)				\
		update_viewer_previous_update_add(update_viewer, update, -3)
#define	update_viewer_previous_update_append(update_viewer, update)				\
		update_viewer_previous_update_add(update_viewer, update, -2)
#define	update_viewer_previous_update_prepend(update_viewer, update)				\
		update_viewer_previous_update_add(update_viewer, update, -1)
#define	update_viewer_previous_update_insert(update_viewer, update, list_store_index)		\
		update_viewer_previous_update_add(update_viewer, update, list_store_index)

static gboolean update_viewer_previous_updates_is_unique(UpdateViewer *update_viewer, const gchar *update);
static void update_viewer_previous_updates_remove(UpdateViewer *update_viewer, gint list_store_index);
static void update_viewer_previous_updates_rotate(UpdateViewer *update_viewer);
static void update_viewer_previous_update_selected(GtkComboBoxEntry *sexy_entry_combo_box_entry, UpdateViewer *update_viewer);
static void update_viewer_previous_updates_free(UpdateViewer *update_viewer);

static void update_viewer_sexy_set(gchar *text);
static gboolean update_viewer_sexy_puts(const gchar *str, gint position, gboolean to_lower, gboolean uniq);

static void update_viewer_reply_or_forward(GtkButton *update_button);
static void update_viewer_insert_shortened_uri(GtkButton *shorten_uri_button, UpdateViewer *update_viewer);

static void update_viewer_dm_data_fill(GList *followers);
static void update_viewer_dm_form_set_loading_label(UpdateViewer *update_viewer);
static void update_viewer_dm_show(GtkToggleButton *toggle_button);
static void update_viewer_dm_form_activate(gboolean dm_activate);
static void update_viewer_dm_refresh(GtkButton *dm_refresh, UpdateViewer *update_viewer);


static void update_viewer_online_services_controls_edit_accounts_clicked(GtkToolButton *edit_accounts_tool_button, UpdateViewer *update_viewer);
static void update_viewer_postable_online_services_setup(UpdateViewer *update_viewer);
static gboolean update_viewer_postable_online_services_check(OnlineService *service);
static GtkTreeIter *update_viewer_postable_online_service_get_iter(OnlineService *service);
static void update_viewer_online_services_set_postable_check_buttons(GtkToolButton *online_services_controls_reset_accounts_tool_button, OnlineService *default_service);
static void update_viewer_online_services_postable_form_viewable_toggled(GtkToggleButton *online_services_view_toggle_button, UpdateViewer *update_viewer);
static void update_viewer_online_service_postable_toggled(GtkCellRendererToggle *online_services_accounts_sexy_tree_view_post_to_enabled_cell_renderer_toggle, gchar *path, UpdateViewer *update_viewer);
static void update_viewer_online_service_check_button_set_status(GtkTreeIter *iter, OnlineService *service, OnlineService *default_service);

/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
static void update_viewer_destroy_cb(GtkWidget *window, UpdateViewer *update_viewer){
	if(!update_viewer) return;
	debug("Destroying UpdateViewer & freeing resources");
	update_viewer_previous_updates_free(update_viewer);
	
	if(update_viewer->viewing_user_name)
		uber_free(update_viewer->viewing_user_name);
	if(update_viewer->best_friends_user_name)
		uber_free(update_viewer->best_friends_user_name);
	if(update_viewer->viewing_update_id_str)
		uber_free(update_viewer->viewing_update_id_str);
	
	if(update_viewer->sexy_to)
		gtk_widget_destroy(GTK_WIDGET(update_viewer->sexy_to));
	if(update_viewer->sexy_from)
		gtk_widget_destroy(GTK_WIDGET(update_viewer->sexy_from));
	if(update_viewer->sexy_selected_update_author)
		gtk_widget_destroy(GTK_WIDGET(update_viewer->sexy_selected_update_author));
	if(update_viewer->sexy_update)
		gtk_widget_destroy(GTK_WIDGET(update_viewer->sexy_update));
	
	if(update_viewer->sexy_entry)
		gtk_widget_destroy(GTK_WIDGET(update_viewer->sexy_entry));
	if(update_viewer->sexy_completion)
		gtk_widget_destroy(GTK_WIDGET(update_viewer->sexy_completion));
	
	if(update_viewer->update_viewer)
		gtk_widget_destroy( GTK_WIDGET(update_viewer->update_viewer) );
	
	if(update_viewer->online_services_accounts_tree_store)
		gtk_tree_store_clear(update_viewer->online_services_accounts_tree_store);
	
	if(update_viewer->upper_case_regex)
		g_regex_unref(update_viewer->upper_case_regex);
	if(update_viewer->username_regex)
		g_regex_unref(update_viewer->username_regex);
	
	if(update_viewer->compact_update_viewer_hidden_containers)
		uber_list_free(update_viewer->compact_update_viewer_hidden_containers);
	if(update_viewer->selected_update_widgets)
		uber_list_free(update_viewer->selected_update_widgets);
	
	uber_free(update_viewer);
}/*update_viewer_destroy_cb*/

static gboolean update_viewer_delete_event_cb(GtkWidget *window, GdkEvent *event, UpdateViewer *update_viewer){
	gtk_toggle_button_set_active(update_viewer->embed_toggle_button, FALSE);
	
	return TRUE;
}/*update_viewer_delete_event_cb*/

static gboolean update_viewer_configure_event_timeout_cb(GtkWidget *widget){
	geometry_save();
	update_viewer->size_timeout_id=0;
	return FALSE;
}

static gboolean update_viewer_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, UpdateViewer *update_viewer){
	program_timeout_remove(&update_viewer->size_timeout_id, _("window(s), vbar, & hbar, geometry & postions"));
	update_viewer->size_timeout_id=g_timeout_add_seconds(1, (GSourceFunc)update_viewer_configure_event_timeout_cb, widget );
	return FALSE;
}

static UpdateViewer *update_viewer_init(void){
	UpdateViewer *update_viewer=g_new0(UpdateViewer, 1);
	debug("Building UpdateViewer");
	update_viewer->sexy_entry_text_position=-1;
	
	GError *error=NULL;
	const gchar *g_regex_pattern="[A-Z]";
	update_viewer->upper_case_regex=g_regex_new(g_regex_pattern, G_REGEX_DOLLAR_ENDONLY|G_REGEX_OPTIMIZE, 0, &error);
	if(error){
		debug("**ERROR:** creating case-matching GRegex using the pattern %s.  GError message: %s.", g_regex_pattern, error->message );
		g_error_free(error);
	}
	
	error=NULL;
	g_regex_pattern="^@[^ ]+ $";
	update_viewer->username_regex=g_regex_new(g_regex_pattern, G_REGEX_DOLLAR_ENDONLY|G_REGEX_OPTIMIZE, 0, &error);
	if(error){
		debug("**ERROR:** creating username GRegex using the pattern %s.  GError message: %s.", g_regex_pattern, error->message );
		g_error_free(error);
	}
	
	update_viewer->viewing_user_name=NULL;
	update_viewer->best_friends_user_name=NULL;
	update_viewer->viewing_update_id_str=NULL;
	
	update_viewer->sexy_to=NULL;
	update_viewer->sexy_from=NULL;
	update_viewer->sexy_selected_update_author=NULL;
	update_viewer->sexy_update=NULL;
	
	update_viewer->sexy_entry=NULL;
	update_viewer->sexy_completion=NULL;
	
	return update_viewer;
}/*update_viewer=update_viewer_init();*/

UpdateViewer *update_viewer_new(GtkWindow *parent){
	GtkBuilder	*ui;
	
	update_viewer=update_viewer_init();
	
	ui=gtkbuilder_get_file(
				GTK_BUILDER_UI_FILENAME,
					"update_viewer", &update_viewer->update_viewer,
					"update_viewer_embed", &update_viewer->update_viewer_embed,
					
					"user_vbox", &update_viewer->user_vbox,
					"user_image", &update_viewer->user_image,
					
					"selected_update_controls_vbox", &update_viewer->selected_update_controls_vbox,
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
					
					"update_viewer_shorten_uri_button", &update_viewer->shorten_uri_button,
					
					"sexy_send", &update_viewer->sexy_send,
					"sexy_dm_button", &update_viewer->sexy_dm_button,
					"sexy_vseparator", &update_viewer->sexy_vseparator,
					
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
					"update_viewer_online_services_view_toggle_button", &update_viewer->online_services_view_toggle_button,
					"update_viewer_online_services_view_toggle_image", &update_viewer->online_services_view_toggle_image,
					
					"dm_form_active_toggle_button", &update_viewer->dm_form_active_toggle_button,
					"dm_form_active_image", &update_viewer->dm_form_active_image,
					
					"controls_vbox", &update_viewer->controls_vbox,
					"update_viwer_view_controls_vpaned", &update_viewer->view_controls_vpaned,
					"update_viewer_status_view_hpaned", &update_viewer->status_view_hpaned,
					
					"update_viewer_appearance_controls_hbox", &update_viewer->update_viewer_appearance_controls_hbox,
					"reply_button", &update_viewer->reply_button, 
					"forward_update_button", &update_viewer->forward_update_button,
					"retweet_button", &update_viewer->retweet_button,
					"make_fave_button", &update_viewer->make_fave_button,
					"update_viewer_unfave_button", &update_viewer->unfave_button,
					
					"update_viewer_destroy_update_button", &update_viewer->destroy_update_button,
					
					"update_viewer_online_services_vbox", &update_viewer->online_services_vbox,
					"update_viewer_online_services_controls_hbox", &update_viewer->online_services_controls_hbox,
					"update_viewer_online_services_controls_label", &update_viewer->online_services_controls_label,
					"update_viewer_online_services_controls_toolbar", &update_viewer->online_services_controls_toolbar,
					"update_viewer_online_services_controls_edit_accounts_tool_button", &update_viewer->online_services_controls_edit_accounts_tool_button,
					"update_viewer_online_services_controls_reset_accounts_tool_button", &update_viewer->online_services_controls_reset_accounts_tool_button,
					
					"update_viewer_online_services_accounts_scrolled_window", &update_viewer->online_services_accounts_scrolled_window,
					"update_viewer_online_services_accounts_tree_store", &update_viewer->online_services_accounts_tree_store,
					"update_viewer_online_services_accounts_sexy_tree_view", &update_viewer->online_services_accounts_sexy_tree_view,
					"update_viewer_online_services_accounts_sexy_tree_view_key_tree_view_column", &update_viewer->online_services_accounts_sexy_tree_view_key_tree_view_column,
					"update_viewer_online_services_accounts_sexy_tree_view_key_cell_renderer_text", &update_viewer->online_services_accounts_sexy_tree_view_key_cell_renderer_text,
					"update_viewer_online_services_accounts_sexy_tree_view_post_to_enabled_tree_view_column", &update_viewer->online_services_accounts_sexy_tree_view_post_to_enabled_tree_view_column,
					"update_viewer_online_services_accounts_sexy_tree_view_post_to_enabled_cell_renderer_toggle", &update_viewer->online_services_accounts_sexy_tree_view_post_to_enabled_cell_renderer_toggle,
				NULL
	);
	
	debug("Building & setting up new Update entry area.");
	update_viewer_sexy_init();
	
	debug("UpdateViewer view & entry area setup.  Grabbing selected widgets.");
	update_viewer_selected_update_buttons_setup(ui, update_viewer);
	update_viewer_grab_widgets_compact_update_viewer_hidden(ui);
	
	debug("UpdateViewer's hotkey connected.  Connecting signal handlers.");
	g_signal_connect(update_viewer->online_services_controls_reset_accounts_tool_button, "clicked", (GCallback)update_viewer_online_services_set_postable_check_buttons, NULL);
	update_viewer_bind_hotkeys(ui);
	
	gtkbuilder_connect(
			ui, update_viewer,
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
				
				"update_viewer_sexy_entry_combo_box_entry", "changed", update_viewer_previous_update_selected,
				
				"update_viewer_shorten_uri_button", "clicked", online_service_request_popup_shorten_uri,
				
				"sexy_send", "clicked", update_viewer_send,
				"sexy_dm_button", "clicked", update_viewer_send,
				"dm_form_active_toggle_button", "toggled", update_viewer_dm_show,
				
				"dm_refresh", "clicked", update_viewer_dm_refresh,
				"followers_send_dm", "clicked", update_viewer_send,
				
				"reply_button", "clicked", update_viewer_reply,
				"forward_update_button", "clicked", update_viewer_forward,
				"retweet_button", "clicked", update_viewer_retweet,
				"make_fave_button", "clicked", online_service_request_selected_update_save_fave,
				"update_viewer_unfave_button", "clicked", online_service_request_selected_update_unfave,
				"update_viewer_destroy_update_button", "clicked", online_service_request_selected_update_delete,
				
				"update_viewer_online_services_controls_edit_accounts_tool_button", "clicked", update_viewer_online_services_controls_edit_accounts_clicked,
			NULL
	);
	
	gchar *window_title=g_strdup_printf("%s - %s", _(GETTEXT_PACKAGE), _("Update Viewer"));
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
	update_viewer_selected_update_buttons_show(NULL, NULL, FALSE);
	
	debug("Disabling & hiding UpdateViewer's dm form since friends have not yet been loaded.");
	update_viewer_dm_form_activate(FALSE);
	gtk_widget_hide(GTK_WIDGET(update_viewer->dm_refresh));
	gtk_widget_hide( (GtkWidget *)update_viewer->best_friend_dm_notification_label );
	
	update_viewer_postable_online_services_setup(update_viewer);
	online_service_request_unset_selected_update();
	
	gboolean compact;
	if((compact=gconfig_if_bool(PREFS_UPDATE_VIEWER_COMPACT, FALSE)) != gtk_toggle_button_get_active(update_viewer->compact_view_toggle_button) )
		gtk_toggle_button_set_active(update_viewer->compact_view_toggle_button, compact);
	else{
		update_viewer_compact_view_display(compact);
		gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->compact_view_toggle_button), "<span weight=\"light\">Compact update viewer.</span>\n<span style=\"italic\" size=\"small\">Display the minimal update controls, buttons, &amp; items.</span>");
		gtk_image_set_from_icon_name(update_viewer->compact_view_image, "gtk-leave-fullscreen", ImagesMinimum);
	}
	
	return update_viewer;
	
	update_viewer_insert_shortened_uri(update_viewer->shorten_uri_button, update_viewer);
}/*update_viewer_new();*/

GtkWindow *update_viewer_get_window(void){
	return update_viewer->update_viewer;
}/*update_viewer_get_window();*/

GtkHBox *update_viewer_get_embed(void){
	return update_viewer->update_viewer_embed;
}/*update_viewer_get_embed();*/

GtkButton *update_viewer_get_sexy_dm_button(void){
	return update_viewer->sexy_dm_button;
}/*update_viewer_get_sexy_dm_button();*/

GtkPaned *update_viewer_get_vpaned(void){
	return GTK_PANED(update_viewer->view_controls_vpaned);
}/*update_viewer_get_vpaned();*/

GtkPaned *update_viewer_get_hpaned(void){
	return GTK_PANED(update_viewer->status_view_hpaned);
}/*update_viewer_get_hpaned();*/

UpdateType update_viewer_get_update_update_type(void){
	return update_viewer->viewing_update_type;
}/*update_viewer_get_update_update_type();*/

GtkVBox *update_viewer_get_status_view_vbox(void){
	return update_viewer->status_view_vbox;
}/*update_viewer_get_status_view_vbox();*/

static void update_viewer_postable_online_services_setup(UpdateViewer *update_viewer){
	g_signal_connect_after(update_viewer->online_services_view_toggle_button, "toggled", (GCallback)update_viewer_online_services_postable_form_viewable_toggled, update_viewer);
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(update_viewer->online_services_view_toggle_button), gconfig_if_bool(PREFS_UPDATE_VIEWER_SHOW_SERVICES, TRUE));
	
	g_signal_connect_after(update_viewer->online_services_accounts_sexy_tree_view_post_to_enabled_cell_renderer_toggle, "toggled", (GCallback)update_viewer_online_service_postable_toggled, update_viewer);
	
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(update_viewer->online_services_accounts_tree_store), POSTABLE_ONLINE_SERVICE_URI, GTK_SORT_ASCENDING);
	sexy_tree_view_set_tooltip_label_column(update_viewer->online_services_accounts_sexy_tree_view, POSTABLE_ONLINE_SERVICE_INFO_STRING);
	
	update_viewer->online_services_accounts_total=-1;
	update_viewer->online_services_accounts_with_post_to_enabled=-1;
	update_viewer->online_services_accounts_with_post_to_enabled_toggle_enabled=-1;
	
	gtk_widget_hide(GTK_WIDGET(update_viewer->online_services_vbox));
	gtk_widget_hide(GTK_WIDGET(update_viewer->online_services_view_toggle_button));
}/*update_viewer_postable_online_services_setup(update_viewer);*/

static void update_viewer_online_services_postable_form_viewable_toggled(GtkToggleButton *online_services_view_toggle_button, UpdateViewer *update_viewer){
	gboolean postable_form_viewable=gtk_toggle_button_get_active(update_viewer->online_services_view_toggle_button);
	
	if(!postable_form_viewable) gtk_widget_hide(GTK_WIDGET(update_viewer->online_services_vbox));
	else gtk_widget_show(GTK_WIDGET(update_viewer->online_services_vbox));
	
	if(postable_form_viewable!=gconfig_if_bool(PREFS_UPDATE_VIEWER_SHOW_SERVICES, TRUE))
		gconfig_set_bool(PREFS_UPDATE_VIEWER_SHOW_SERVICES, postable_form_viewable);
}/*update_viewer_online_services_postable_form_viewable_toggled(update_viewer->online_services_view_toggle_button, update_viewer);*/

void update_viewer_postable_online_services_append(OnlineService *service){
	if(!(update_viewer && update_viewer->online_services_accounts_tree_store)) return;
	if(!(service && service->user_name)) return;
	if(update_viewer_postable_online_services_check(service))
		return;
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	gtk_tree_store_append(update_viewer->online_services_accounts_tree_store, iter, NULL);
	update_viewer->online_services_accounts_total++;
	update_viewer_online_service_check_button_set_status(iter, service, NULL);
	uber_free(iter);
}/*update_viewer_postable_online_services_append(service);*/

static void update_viewer_online_service_postable_toggled(GtkCellRendererToggle *online_services_accounts_sexy_tree_view_post_to_enabled_cell_renderer_toggle, gchar *path, UpdateViewer *update_viewer){
	GtkTreeIter iter;
	if(!gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(update_viewer->online_services_accounts_tree_store), &iter, path)){
		debug("Looing for postable online service, from path: %s, failed to get valid iter for the tree store.", path);
		return;
	}
	
	OnlineService *service=NULL;
	gboolean post_to_enabled=TRUE;
	gtk_tree_model_get(
			GTK_TREE_MODEL(update_viewer->online_services_accounts_tree_store), &iter,
      				POSTABLE_ONLINE_SERVICE, &service,
				POSTABLE_ONLINE_SERVICE_POST_TO_ENABLED, &post_to_enabled,
			-1
	);
	
	if(update_viewer->online_services_accounts_with_post_to_enabled<1 && post_to_enabled){
		debug("%s cannot disable the only postable OnlineService.", GETTEXT_PACKAGE);
		return;
	}
	
	gchar *post_to_enabled_info_string=g_strdup_printf("<%s>&lt;%s&gt;'s status will%s be updated.</%s>", (!post_to_enabled ?"b" :"i"), service->key, (!post_to_enabled ?"" :" not"), (!post_to_enabled ?"b" :"i"));
	gtk_tree_store_set(
			update_viewer->online_services_accounts_tree_store, &iter,
				POSTABLE_ONLINE_SERVICE_POST_TO_ENABLED, !post_to_enabled,
				POSTABLE_ONLINE_SERVICE_INFO_STRING, post_to_enabled_info_string,
			-1
	);
	uber_free(post_to_enabled_info_string);
	if(!(service->post_to_enabled=!post_to_enabled))
		update_viewer->online_services_accounts_with_post_to_enabled--;
	else
		update_viewer->online_services_accounts_with_post_to_enabled++;
}/*update_viewer_online_service_postable_toggled(online_services_accounts_sexy_tree_view_post_to_enabled_cell_renderer_toggle, "1", update_viewer);*/

static gboolean update_viewer_postable_online_services_check(OnlineService *service){
	GtkTreeIter *iter=update_viewer_postable_online_service_get_iter(service);
	gboolean online_service_found=(iter ?TRUE :FALSE );
	uber_free(iter);
	if(!online_service_found)
		return FALSE;
	return TRUE;
}/*update_viewer_postable_online_services_check(service);*/

static GtkTreeIter *update_viewer_postable_online_service_get_iter(OnlineService *service){
	
	for(gint i=0; i<=update_viewer->online_services_accounts_total; i++){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(update_viewer->online_services_accounts_tree_store), iter, path)){
			debug("Looing for postable online service, at index: %d, failed to get valid iter for the tree store.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		OnlineService *service_at_index=NULL;
		gtk_tree_model_get(
				GTK_TREE_MODEL(update_viewer->online_services_accounts_tree_store), iter,
      					POSTABLE_ONLINE_SERVICE, &service_at_index,
				-1
		);
		
		if(service_at_index==service){
			gtk_tree_path_free(path);
			return iter;
		}
		
		gtk_tree_path_free(path);
		uber_free(iter);
	}
	
	return NULL;
}/*update_viewer_postable_online_service_get_iter(service);*/

static void update_viewer_online_services_set_postable_check_buttons(GtkToolButton *online_services_controls_reset_accounts_tool_button, OnlineService *default_service){
	update_viewer->online_services_accounts_with_post_to_enabled=-1;
	for(gint i=0; i<=update_viewer->online_services_accounts_total; i++){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(update_viewer->online_services_accounts_tree_store), iter, path)){
			debug("Looing for postable online service, at index: %d, failed to get valid iter for the tree store.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		OnlineService *service=NULL;
		gtk_tree_model_get(
				GTK_TREE_MODEL(update_viewer->online_services_accounts_tree_store), iter,
      					POSTABLE_ONLINE_SERVICE, &service,
				-1
		);
		
		update_viewer_online_service_check_button_set_status(iter, service, default_service);
		gtk_tree_path_free(path);
		uber_free(iter);
	}
}/*update_viewer_online_services_set_postable_check_buttons(update_viewer->online_services_controls_reset_accounts_tool_button, service);*/

static void update_viewer_online_service_check_button_set_status(GtkTreeIter *iter, OnlineService *service, OnlineService *default_service){
	gboolean post_to_enabled=TRUE;
	if(default_service && default_service!=service)
		post_to_enabled=FALSE;
	else
		post_to_enabled=service->post_to_by_default;
	
	if(post_to_enabled)
		update_viewer->online_services_accounts_with_post_to_enabled++;
	
	if(service->post_to_enabled!=post_to_enabled)
		service->post_to_enabled=post_to_enabled;
	
	gboolean post_to_enabled_toggle_enabled=TRUE;
	if(!(service->enabled && service->connected && service->authenticated))
		post_to_enabled_toggle_enabled=FALSE;
	
	if(post_to_enabled)
		update_viewer->online_services_accounts_with_post_to_enabled_toggle_enabled++;
	
	if(update_viewer->online_services_accounts_with_post_to_enabled_toggle_enabled>0 && post_to_enabled_toggle_enabled){
		if(!gtk_widget_is_visible(GTK_WIDGET(update_viewer->online_services_view_toggle_button))){
			gtk_widget_show(GTK_WIDGET(update_viewer->online_services_view_toggle_button));
			debug("Displaying %s's postable OnlineService(s) container.", GETTEXT_PACKAGE);
		}
		if(gconfig_if_bool(PREFS_UPDATE_VIEWER_SHOW_SERVICES, TRUE) && !gtk_widget_is_visible(GTK_WIDGET(update_viewer->online_services_vbox))){
			gtk_widget_show(GTK_WIDGET(update_viewer->online_services_vbox));
			debug("Displaying %s's postable OnlineService(s) toggle button.", GETTEXT_PACKAGE);
		}
	}
	
	gchar *post_to_enabled_info_string=g_strdup_printf("<%s>&lt;%s&gt;'s status will%s be updated.</%s>", (!post_to_enabled ?"i" :"b"), service->key, (!post_to_enabled ?" not" :""), (!post_to_enabled ?"i" :"b"));
	debug("Updating <%s>'s post_to_enabled status will%s be updated and changing it will be %sabled", service->key, (!post_to_enabled ?" not" :""), (post_to_enabled_toggle_enabled ?"en" :"dis"));
	if(!post_to_enabled_toggle_enabled)
		debug("**DEBUG:** Disabling toggling of posting to <%s> enabled: %s; connected: %s; authenticated: %s", service->key, (service->enabled ?"TRUE" :"FALSE"), (service->connected ?"TRUE" :"FALSE"), (service->authenticated ?"TRUE" :"FALSE"));
	
	gtk_tree_store_set(
				update_viewer->online_services_accounts_tree_store, iter,
					POSTABLE_ONLINE_SERVICE, service,
					POSTABLE_ONLINE_SERVICE_KEY, service->key,
      					POSTABLE_ONLINE_SERVICE_POST_TOGGLE_ENABLED, post_to_enabled_toggle_enabled,
					POSTABLE_ONLINE_SERVICE_POST_TO_ENABLED, post_to_enabled,
					POSTABLE_ONLINE_SERVICE_URI, service->uri,
					POSTABLE_ONLINE_SERVICE_INFO_STRING, post_to_enabled_info_string,
				-1
	);
	uber_free(post_to_enabled_info_string);
}/*update_viewer_online_service_check_button_set_status(iter, service, NULL);*/

gboolean update_viewer_postable_online_service_delete(OnlineService *service){
	for(gint i=0; i<=update_viewer->online_services_accounts_total; i++){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(update_viewer->online_services_accounts_tree_store), iter, path)){
			debug("Looing for postable online service, at index: %d, failed to get valid iter for the tree store.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		OnlineService *service_at_index=NULL;
		gboolean post_to_enabled=TRUE, post_to_enabled_toggle_enabled=TRUE;
		gtk_tree_model_get(
				GTK_TREE_MODEL(update_viewer->online_services_accounts_tree_store), iter,
      					POSTABLE_ONLINE_SERVICE, &service_at_index,
      					POSTABLE_ONLINE_SERVICE_POST_TOGGLE_ENABLED, post_to_enabled_toggle_enabled,
					POSTABLE_ONLINE_SERVICE_POST_TO_ENABLED, post_to_enabled,
				-1
		);
		
		if(service_at_index!=service){
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gtk_tree_store_remove(update_viewer->online_services_accounts_tree_store, iter);
		update_viewer->online_services_accounts_total--;
		if(post_to_enabled)
			update_viewer->online_services_accounts_with_post_to_enabled--;
		
		if(post_to_enabled_toggle_enabled)
			update_viewer->online_services_accounts_with_post_to_enabled_toggle_enabled--;
		
		if(update_viewer->online_services_accounts_with_post_to_enabled_toggle_enabled<1){
			if(gtk_widget_is_visible(GTK_WIDGET(update_viewer->online_services_view_toggle_button))){
				gtk_widget_hide(GTK_WIDGET(update_viewer->online_services_view_toggle_button));
				debug("Hiding %s's postable OnlineService(s) container.", GETTEXT_PACKAGE);
			}
			if(gtk_widget_is_visible(GTK_WIDGET(update_viewer->online_services_vbox))){
				gtk_widget_hide(GTK_WIDGET(update_viewer->online_services_vbox));
				debug("Hiding %s's postable OnlineService(s) toggle button.", GETTEXT_PACKAGE);
			}
		}
		gtk_tree_path_free(path);
		uber_free(iter);
		return TRUE;
	}
	
	return FALSE;
}/*update_viewer_postable_online_service_delete(service);*/

static void update_viewer_online_services_controls_edit_accounts_clicked(GtkToolButton *edit_accounts_tool_button, UpdateViewer *update_viewer){
	GtkWindow *window=NULL;
	if(!gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE))
		window=main_window_get_window();
	else
		window=update_viewer->update_viewer;
	
	online_services_dialog_show(window);
}/*update_viewer_online_services_controls_edit_accounts_clicked(update_viewer->online_services_controls_edit_accounts_tool_button, update_viewer);*/

static void update_viewer_selected_update_author_best_friend_toggled(GtkToggleButton *best_friend_toggle_button, UpdateViewer *update_viewer){
	OnlineService *service=online_service_request_selected_update_get_service();
	const gchar *user_name=online_service_request_selected_update_get_user_name();
	if(!( G_STR_N_EMPTY(user_name) && G_STR_N_EMPTY(update_viewer->viewing_user_name) && service && update_viewer->viewing_service==service && g_str_equal(update_viewer->viewing_user_name, user_name) )) {
		debug("Not changing best friend's status.  selected_update from:<%s@%s> doesn't match update being viewed from: <%s@%s>.", (G_STR_N_EMPTY(user_name) ?user_name :"unknown"), (service ?service->uri :"unknown"), (update_viewer->viewing_service ?update_viewer->viewing_service->uri :"unknown"), (G_STR_N_EMPTY(update_viewer->viewing_user_name) ?update_viewer->viewing_user_name :"unknown"));
		return;
	}
	
	if(!best_friends_is_user_best_friend(service, user_name) )
		online_service_request_selected_update_best_friend_add();
	else
		online_service_request_selected_update_best_friend_drop();
}/*update_viewer_selected_update_author_best_friend_toggled(best_friend_toggle_widget, update_viewer);*/

void update_viewer_emulate_embed_toggle_via_check_menu_item(GtkCheckMenuItem *view_update_viewer_floating_check_menu_item){
	if(gtk_check_menu_item_get_active(view_update_viewer_floating_check_menu_item)!=gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE))
		gtk_toggle_button_set_active(update_viewer->embed_toggle_button, !gtk_toggle_button_get_active(update_viewer->embed_toggle_button) );
}/*update_viewer_emulate_embed_toggle_via_check_menu_item();*/

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

void update_viewer_compact_view_toggled(GtkToggleButton *compact_view_toggle_button){
	gboolean compact;
	if((compact=gtk_toggle_button_get_active(compact_view_toggle_button))==gconfig_if_bool(PREFS_UPDATE_VIEWER_COMPACT, FALSE) )
		return;
	
	gconfig_set_bool(PREFS_UPDATE_VIEWER_COMPACT, compact);
	
	update_viewer_compact_view_display(compact);
	
	if(!compact){
		gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->compact_view_toggle_button), "<span weight=\"light\">Compact update viewer.</span>\n<span style=\"italic\" size=\"small\">Display the minimal update controls, buttons, &amp; items.</span>");
		gtk_image_set_from_icon_name(update_viewer->compact_view_image, "gtk-leave-fullscreen", ImagesMinimum);
	}else{
		gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->compact_view_toggle_button), "<span weight=\"bold\" variant=\"small-caps\">Enlarge update viewer.</span>\n<span style=\"italic\" size=\"small\">Display all update controls, buttons, &amp; items.</span>");
		gtk_image_set_from_icon_name(update_viewer->compact_view_image, "gtk-fullscreen", ImagesMinimum);
	}
	
	const gchar *user_name=NULL;
	if(!(user_name=online_service_request_selected_update_get_user_name()))
		update_viewer_selected_update_buttons_show(online_service_request_selected_update_get_service(), NULL, FALSE);
	else
		update_viewer_selected_update_buttons_show(online_service_request_selected_update_get_service(), user_name, TRUE);
}/*update_viewer_compact_view_toggled(update_viewer->compact_view_toggle_button);*/

static void update_viewer_grab_widgets_compact_update_viewer_hidden(GtkBuilder *ui){
	const gchar *update_view_containers[]={
		"user_vbox",
		"update_viewer_left_vseparator",
		"status_view_vbox",
	};
	
	GList *list=NULL;
	for(int i=0; i < G_N_ELEMENTS(update_view_containers); i++)
		list=g_list_append(list, (gtk_builder_get_object(ui, update_view_containers[i])) );
	update_viewer->compact_update_viewer_hidden_containers=list;
}/*update_viewer_grab_widgets_compact_update_viewer_hidden(ui);*/

static void update_viewer_compact_view_display(gboolean compact){
	debug("%sabling UpdateViewer compact view.  UpdateViewer's Compact View Toggle Button is %sactive", (compact ?"En" :"Dis" ), (gtk_toggle_button_get_active(update_viewer->compact_view_toggle_button) ?"" :"in" ) );
	GList *l=NULL;
	for(l=update_viewer->compact_update_viewer_hidden_containers; l; l=l->next)
		if(!compact)
			gtk_widget_show( GTK_WIDGET(l->data) );
		else
			gtk_widget_hide( GTK_WIDGET(l->data) );
	g_list_free(l);
	
	update_viewer_scale(compact);
	
	if(!GTK_WIDGET_IS_SENSITIVE(update_viewer->update_composition_vbox))
		gtk_widget_set_sensitive(GTK_WIDGET(update_viewer->update_composition_vbox), TRUE);
	
	if(!gtk_widget_is_visible(GTK_WIDGET(update_viewer->update_composition_vbox)))
		gtk_widget_show(GTK_WIDGET(update_viewer->update_composition_vbox));
	
	update_viewer_sexy_select();
}/*update_viewer_compact_view_display(TRUE); */

static void update_viewer_scale(gboolean compact){
	gint h=0, w=0;
	gtk_window_get_size(main_window_get_window(), &w, &h );
	gint position=gtk_paned_get_position(main_window_get_main_paned() );
	position++;
}/* update_viewer_scale(TRUE); */ 

static void update_viewer_selected_update_buttons_setup(GtkBuilder *ui, UpdateViewer *update_viewer){
	const gchar *selected_update_widgets[]={
		/* Container widgets start here.  Starting with index #0. */
		"user_vbox", /* index[0] */
		
		/* Container widgets end here.  Misc. widgets begin with #1. */
		"update_viewer_update_controls_vseparator", /* index[1] */
		"make_fave_button", /* index[2] */
		
		"update_viewer_left_vseparator", /* index[3] */
		"reply_button", /* index[4] */
		"forward_update_button", /* index[5] */
		
		"user_image", /* index[6] */
		/* Misc. widgets end here.  Buttons begin with index #7. */
		
		"update_viewer_unfave_button", /* index[7] */
		
		"update_viewer_destroy_update_button", /* index[8] */
		
		"view_user_profile_button", /* index[9] */
		"view_user_unread_updates_button", /* index[10] */
		"view_user_updates_button", /* index[11] */
		"best_friend_toggle_button", /* index[12] */
		
		"user_follow_button", /* index[13] */
		"user_unfollow_button", /* index[14] */
		"user_block_button", /* index[15] */
		
		"sexy_dm_button", /* index[16] */
	};
	
	GList *list=NULL;
	for(int i=0; i < G_N_ELEMENTS(selected_update_widgets); i++)
		list=g_list_append(list, (gtk_builder_get_object(ui, selected_update_widgets[i])) );
	
	list=g_list_append(list, update_viewer->retweet_button);
	
	update_viewer->selected_update_widgets=list;
}/*update_viewer_selected_widgets_setup();*/

static void update_viewer_selected_update_buttons_show(OnlineService *service, const gchar *user_name, gboolean selected_update){
	main_window_selected_update_image_menu_items_show(selected_update);
	GList *l=NULL;
	guint8 i=0;
	for(i=0, l=update_viewer->selected_update_widgets; l; l=l->next, i++){
		switch(i){
			/* Container widgets start here.  Starting with index #0. */
			case 0: /* "user_vbox", */
				if(!selected_update && gconfig_if_bool(PREFS_UPDATE_VIEWER_COMPACT, FALSE) )
					gtk_widget_hide_all(GTK_WIDGET(l->data));
				else
					gtk_widget_show_all(GTK_WIDGET(l->data));
				break;
			/* Container widgets end here.  Misc. widgets begin with #1. */
			
			/* Misc. widgets start here.  Starting with index #1. */
			case 1: /* "update_viewer_update_controls_vseparator" */
			case 2: /* "make_fave_button" */
				if(!(selected_update && service))
					gtk_widget_hide(GTK_WIDGET(l->data));
				else
					gtk_widget_show(GTK_WIDGET(l->data));
				break;
			
			case 3: case 4: case 5:
				/*    "update_viewer_left_vseparator"           */
				/*    "reply_button" & "forward_update_button"  */
				if(!selected_update && gconfig_if_bool(PREFS_UPDATE_VIEWER_COMPACT, FALSE) )
					gtk_widget_hide(GTK_WIDGET(l->data));
				else
					gtk_widget_show(GTK_WIDGET(l->data));
				if(i==3) break;
				gtk_widget_set_sensitive(GTK_WIDGET(l->data), selected_update);
				break;
			
			case 6: /* "user_image" */
				if(gconfig_if_bool(PREFS_UPDATE_VIEWER_COMPACT, FALSE))
					gtk_widget_hide(GTK_WIDGET(l->data));
				else
					gtk_widget_show(GTK_WIDGET(l->data));
				break;
			/* Misc. widgets end here.  Buttons begin with index #7. */
			
			case 7: /* "update_viewer_unfave_button" */
				if(!(selected_update && service && update_viewer->viewing_update_type==Faves))
					gtk_widget_hide(GTK_WIDGET(l->data));
				else
					gtk_widget_show(GTK_WIDGET(l->data));
				break;
			
			case 8: /* "update_viewer_destroy_update_button" */
				if(!(selected_update && service && G_STR_N_EMPTY(user_name) && online_services_is_user_name_mine(service, user_name) ))
					gtk_widget_hide(GTK_WIDGET(l->data));
				else
					gtk_widget_show(GTK_WIDGET(l->data));
				break;
			
			default:
				/************************************************
				 * "view_user_profile_button",                  *
				 * "view_user_unread_updates_button",           *
				 * "view_user_updates_button",                  *
				 * "best_friend_toggle_button",                 *
				 *                                              *
				 * "user_follow_button",                        *
				 * "user_unfollow_button",                      *
				 * "user_block_button",                         *
				 *                                              *
				 * "sexy_dm_button",                            *
				 ************************************************/
				if( GTK_BUTTON(l->data) == update_viewer->retweet_button )
					if(!(selected_update && service)){
						gtk_widget_hide(GTK_WIDGET(l->data));
						gtk_widget_set_sensitive(GTK_WIDGET(l->data), FALSE);
					}else{
						gtk_widget_show(GTK_WIDGET(l->data));
						gtk_widget_set_sensitive(GTK_WIDGET(l->data), TRUE);
					}
				else{
					if(selected_update) gtk_widget_show(GTK_WIDGET(l->data));
					gtk_widget_set_sensitive(GTK_WIDGET(l->data), selected_update);
				}
				break;
		}
	}
	g_list_free(l);
	
	if(!(selected_update && service && G_STR_N_EMPTY(user_name) && best_friends_is_user_best_friend(service, user_name) ))
		gtk_toggle_button_set_active(update_viewer->best_friend_toggle_button, FALSE);
	else
		gtk_toggle_button_set_active(update_viewer->best_friend_toggle_button, TRUE);
	
	if(!GTK_WIDGET_IS_SENSITIVE(update_viewer->update_composition_vbox))
		gtk_widget_set_sensitive(GTK_WIDGET(update_viewer->update_composition_vbox), TRUE);
	
	if(!gtk_widget_is_visible(GTK_WIDGET(update_viewer->update_composition_vbox)))
		gtk_widget_show(GTK_WIDGET(update_viewer->update_composition_vbox));
	
	update_viewer_sexy_select();
}/*update_viewer_selected_update_buttons_show(service|NULL, user_name|NULL, TRUE|FALSE);*/

static void update_viewer_bind_hotkeys(GtkBuilder *ui){
	g_signal_connect_after( update_viewer->embed_toggle_button, "toggled", (GCallback)main_window_update_viewer_set_embed, NULL );
	g_signal_connect_after( update_viewer->compact_view_toggle_button, "toggled", (GCallback)update_viewer_compact_view_toggled, NULL);
	g_signal_connect_after( update_viewer->best_friend_toggle_button, "toggled", (GCallback)update_viewer_selected_update_author_best_friend_toggled, update_viewer);
	
	const gchar *hotkey_widgets[]={
		/*"update_viewer",
		"update_viewer_embed",*/
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
		"update_viewer_appearance_controls_hbox",
	};
	GObject *widget=NULL;
	debug("UpdateViewer interface loaded.  Setting up hotkeys.");
	for(int i=0; i<G_N_ELEMENTS(hotkey_widgets); i++){
		widget=NULL;
		if(!(widget=gtk_builder_get_object(ui, hotkey_widgets[i]))){
			debug("**ERROR:** Cannot bind %s's hotkeys to %s, the widget could not be found in %s", _(GETTEXT_PACKAGE), hotkey_widgets[i], GTK_BUILDER_UI_FILENAME);
			continue;
		}
		
		debug("Binding %s's hotkeys to %s.", _(GETTEXT_PACKAGE), hotkey_widgets[i]);
		g_signal_connect_after(widget, "key-press-event",(GCallback)hotkey_pressed, widget);
		g_signal_connect_after(widget, "event-after", (GCallback)update_viewer_sexy_select, NULL );
	}
}/*update_viewer_bind_hotkeys(ui);*/

void update_viewer_best_friends_start_dm(OnlineService *service, const gchar *user_name){
	update_viewer->best_friends_service=service;
	update_viewer->best_friends_user_name=g_strdup(user_name);
	gchar *best_friend_dm_markup_message=NULL;
	gtk_label_set_markup( update_viewer->best_friend_dm_notification_label, (best_friend_dm_markup_message=g_strdup_printf("<b>Direct Message %s:</b>", update_viewer->best_friends_user_name)) );
	uber_free(best_friend_dm_markup_message);
	gtk_widget_show( GTK_WIDGET(update_viewer->best_friend_dm_notification_label) );
}/*void update_viewer_best_friends_start_dm(service, user_name);*/

static void update_viewer_dm_form_set_toggle_and_image(void){
	if(!gtk_toggle_button_get_active(update_viewer->dm_form_active_toggle_button)){
		debug("Setting UpdateViewer's dm form toggle button to enable the DM form.");
		gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->dm_form_active_toggle_button), "<span weight=\"bold\">Compose DM's to any of your friends.</span>");
		gtk_image_set_from_icon_name(update_viewer->dm_form_active_image, "gtk-edit", ImagesMinimum);
	}else{
		debug("Setting UpdateViewer's dm form toggle button to disable the DM form.");
		gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->dm_form_active_toggle_button), "<span weight=\"light\">Hide the DM form &amp; area.  You'll still be able to send DMs in the future by clicking this button again.</span>");
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
	debug("Creating UpdateViewer's title area, 'update_viewer->sexy_selected_update_author', using sexy label interface.");
	update_viewer->sexy_selected_update_author=uberchick_label_new();
	gtk_box_pack_start(
			GTK_BOX(update_viewer->selected_update_controls_vbox),
			GTK_WIDGET(update_viewer->sexy_selected_update_author),
			FALSE, FALSE, 5
	);
	debug("Setting 'sexy_selected_update_author' as 'selected_update_controls_vbox' 2nd widget.");
	g_object_set(update_viewer->sexy_selected_update_author, "xalign", 0.50, "single-line-mode", TRUE, NULL);
	gtk_widget_show(GTK_WIDGET(update_viewer->sexy_selected_update_author));
	
	debug("Creating UpdateViewer's service link area, 'update_viewer->sexy_to', using sexy label interface.");
	update_viewer->sexy_to=uberchick_label_new();
	gtk_box_pack_start(
			GTK_BOX(update_viewer->status_view_vbox),
			GTK_WIDGET(update_viewer->sexy_to),
			FALSE, FALSE, 5
	);
	g_object_set(update_viewer->sexy_to, "xalign", 1.00, "single-line-mode", TRUE, NULL);
	gtk_widget_show(GTK_WIDGET(update_viewer->sexy_to));
	
	debug("Creating UpdateViewer's title area, 'update_viewer->sexy_from', using sexy label interface.");
	update_viewer->sexy_from=uberchick_label_new();
	gtk_box_pack_start(
			GTK_BOX(update_viewer->status_view_vbox),
			GTK_WIDGET(update_viewer->sexy_from),
			FALSE, FALSE, 5
	);
	g_object_set(update_viewer->sexy_from, "single-line-mode", TRUE, NULL);
	gtk_widget_show(GTK_WIDGET(update_viewer->sexy_from));
	
	debug("Aligning UpdateViewer's view area, 'update_viewer->update_datetime_label' label");
	g_object_set(update_viewer->update_datetime_label, "single-line-mode", TRUE, NULL);
	
	debug("Creating UpdateViewer's view area, 'update_viewer->sexy_update', using uberchick-label interface.");
	update_viewer->sexy_update=uberchick_label_new();
	gtk_box_pack_end(
			GTK_BOX(update_viewer->status_view_vbox),
			GTK_WIDGET(update_viewer->sexy_update),
			TRUE, TRUE, 5
	);
	
	debug("Setting 'status_view_vbox' child packing");
	gtk_box_set_child_packing(
			GTK_BOX(update_viewer->status_view_vbox),
			GTK_WIDGET(update_viewer->sexy_update),
			TRUE, TRUE,
			0, GTK_PACK_END
	);
	
	gtk_widget_show(GTK_WIDGET(update_viewer->sexy_update));
	update_viewer_sexy_entry_setup(update_viewer);
	g_signal_connect_after(update_viewer->followers_combo_box, "changed", G_CALLBACK(update_viewer_sexy_select), update_viewer->followers_combo_box);
	
	update_viewer_update_view_reorder();
}/*update_viewer_sexy_init();*/

static void update_viewer_update_view_reorder(void){
	debug("Ordering UpdateViewer.");
	gtk_box_reorder_child(
			GTK_BOX(update_viewer->selected_update_controls_vbox),
			GTK_WIDGET(update_viewer->sexy_selected_update_author),
			1
	);
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
}/*update_viewer_update_view_reorder();*/

gboolean update_viewer_set_in_reply_to_data(OnlineService *service, UpdateType update_type, const gchar *user_name, const gchar *update_text, gdouble user_id, gdouble update_id, gboolean reply, gboolean forwarding, gboolean save_reply_to_data){
	if(!update_viewer) return FALSE;
	if(!save_reply_to_data && !reply) return FALSE;
	
	if(save_reply_to_data){
		if(update_viewer->viewing_user_name) uber_free(update_viewer->viewing_user_name);
		update_viewer->viewing_user_name=g_strdup((G_STR_EMPTY(user_name) ?"" :user_name ) );
		update_viewer->viewing_user_id=user_id;
		if(update_viewer->viewing_user_id_str) uber_free(update_viewer->viewing_user_id_str);
		if(update_id) update_viewer->viewing_user_id_str=gdouble_to_str(user_id);
		update_viewer->viewing_update_id=update_id;
		if(update_viewer->viewing_update_id_str) uber_free(update_viewer->viewing_update_id_str);
		if(update_id)
			update_viewer->viewing_update_id_str=gdouble_to_str(update_id);
		
		if(!(G_STR_N_EMPTY(update_text) && G_STR_N_EMPTY(update_viewer->viewing_update_text) && g_str_equal(update_viewer->viewing_update_text, update_text)))
			uber_free(update_viewer->viewing_update_text);
		
		if(!(G_STR_EMPTY(update_text)))
			update_viewer->viewing_update_text=g_strdup(update_text);
		
		if(update_viewer->viewing_service!=service)
			update_viewer->viewing_service=service;
		if(update_viewer->viewing_update_type!=update_type)
			update_viewer->viewing_update_type=update_type;
	}
	
	if(!((reply || forwarding) && user_name && service)){
		update_viewer_sexy_select();
		return FALSE;
	}
	
	gboolean prefix_added=FALSE;
	if(forwarding && !update_viewer_sexy_entry_clear()){
		update_viewer_sexy_select();
		return FALSE;
	}
	
	gchar *users_at=NULL;
	if(!( online_services_has_connected(1) > 0 && gconfig_if_bool(PREFS_UPDATES_ADD_PROFILE_LINK, TRUE) ))
		users_at=g_strdup_printf("@%s ", user_name);
	else
		users_at=g_strdup_printf("@%s, http://%s/%s, ", user_name, service->uri, user_name);
	
	if(!(prefix_added=update_viewer_sexy_prefix_string(users_at, TRUE, TRUE))){
		update_viewer_sexy_insert_string(" ", FALSE, FALSE);
		update_viewer_sexy_insert_string(users_at, TRUE, FALSE);
	}
	uber_free(users_at);
	
	if(!forwarding){
		update_viewer_sexy_select();
		return prefix_added;
	}
	
	update_viewer_sexy_prefix_string("RT ", FALSE, FALSE);
	update_viewer_sexy_append_string("\"", FALSE, FALSE);
	update_viewer_sexy_append_string(update_viewer->viewing_update_text, FALSE, FALSE);
	update_viewer_sexy_append_string("\"", FALSE, FALSE);
	
	update_viewer_sexy_select();
	return prefix_added;
}/*update_viewer_set_in_reply_to_data(user->user_name|NULL, None|Homepage|Replies|DMs|etc, service, 1212154, TRUE|FALSE, TRUE|FALSE);*/

void update_viewer_view_update(OnlineService *service, const gdouble id, const gdouble user_id, const gchar *user_name, const gchar *nick_name, const gchar *date, const gchar *sexy_update, const gchar *text_update, GdkPixbuf *pixbuf, UpdateType update_type, gdouble retweet_update_id){
	if(!(service && service->session && online_service_validate_session(service, user_name) )) return;
	
	if(!id){
		if(!(service && gconfig_if_bool(PREFS_UPDATES_DIRECT_REPLY_ONLY, FALSE)))
			update_viewer_online_services_set_postable_check_buttons(update_viewer->online_services_controls_reset_accounts_tool_button, NULL);
		else
			update_viewer_online_services_set_postable_check_buttons(update_viewer->online_services_controls_reset_accounts_tool_button, service);
	}
	
	if(id)
		update_viewer_set_in_reply_to_data(service, update_type, (gchar *)user_name, (gchar *)text_update, user_id, (retweet_update_id ?retweet_update_id :id), FALSE, FALSE, TRUE);
	else
		update_viewer_set_in_reply_to_data(NULL, None, NULL, NULL, 0.0, 0.0, FALSE, FALSE, TRUE);
	
	debug("%s the UpdateViewer.", (id ?_("Enlarging") :"") );
	update_viewer_compact_view_display( gconfig_if_bool(PREFS_UPDATE_VIEWER_COMPACT, FALSE) );
	
	debug("%sabling 'selected_update_buttons'.", (id ?"En" :"Dis") );
	update_viewer_selected_update_buttons_show((service? service :NULL), (G_STR_EMPTY(user_name) ?NULL :user_name ), (id ?TRUE :FALSE));
	
	gboolean render_update=(id ?TRUE :FALSE);
	if(!render_update)
		online_service_request_unset_selected_update();
	else{
		online_service_request_set_selected_update(service, update_type, (retweet_update_id ?retweet_update_id :id), user_id, user_name, text_update);
		main_window_set_statusbar_default_message( _("Hotkeys: <Alt+S> to post your update; <Alt+D> to send your update as a DM.  <CTRL+N> start a new tweet; <CTRL+D> or <SHIFT+Return> to DM your friends; <CTRL+R>, <Return>, or '@' to reply, <CTRL+F> or '>' to forward/retweet.") );
	}
	
	main_window_set_statusbar_msg(NULL);
	
	/* Resizes and sets the user's Avatar. */
	if(!pixbuf)
		gtk_image_set_from_icon_name(update_viewer->user_image, GETTEXT_PACKAGE, ImagesExpanded);
	else{
		debug("Setting avatar for the user who wrote the 'selected_update'.");
		GdkPixbuf *resized=NULL;
		if(! (resized=images_expand_pixbuf(pixbuf)) )
			gtk_image_set_from_pixbuf(update_viewer->user_image, pixbuf);
		else{
			gtk_image_set_from_pixbuf(update_viewer->user_image, resized);
			g_object_unref(resized);
		}
	}
	
	const gchar *uri_scheme_suffix=(service->https?"s":"");
	
	gchar *sexy_text=NULL;
	
	if(!render_update){
		uberchick_label_set_markup(update_viewer->sexy_selected_update_author, service, update_type, user_name, user_id, id, "", FALSE, TRUE);
		gtk_widget_hide(GTK_WIDGET(update_viewer->sexy_selected_update_author));
	}else{
		sexy_text=g_strdup_printf("<span weight=\"ultrabold\"><a href=\"http%s://%s/%s\">%s</a></span>", uri_scheme_suffix, service->uri, user_name, user_name);
		debug("Setting 'sexy_selected_update_author' for 'selected_update':\n\t\t%s.", sexy_text);
		uberchick_label_set_markup(update_viewer->sexy_selected_update_author, service, update_type, user_name, user_id, id, sexy_text, FALSE, TRUE);
		uber_free(sexy_text);
	}
	
	if(!render_update)
		sexy_text=g_strdup("");
	else
		sexy_text=g_strdup_printf("<span size=\"small\" weight=\"light\" variant=\"smallcaps\">To: <a href=\"http%s://%s/%s\">%s &lt;%s@%s&gt;</a></span>", uri_scheme_suffix, service->uri, service->user_name, service->nick_name, service->user_name, service->uri);
	
	debug("Setting 'sexy_to' for 'selected_update':\n\t\t%s.", sexy_text);
	uberchick_label_set_markup(update_viewer->sexy_to, service, update_type, user_name, user_id, id, sexy_text, FALSE, TRUE);
	uber_free(sexy_text);
	
	if(!render_update)
		sexy_text=g_strdup("");
	else
		sexy_text=g_strdup_printf("<span weight=\"ultrabold\">From: <a href=\"http%s://%s/%s\">%s &lt;%s@%s&gt;</a></span>", uri_scheme_suffix, service->uri, user_name, nick_name, user_name, service->uri);
	
	debug("Setting 'sexy_from' for 'selected_update':\n\t\t%s.", sexy_text);
	uberchick_label_set_markup(update_viewer->sexy_from, service, update_type, user_name, user_id, id, sexy_text, FALSE, TRUE);
	uber_free(sexy_text);
	
	if(!render_update)
		sexy_text=g_strdup("");
	else
		sexy_text=g_markup_printf_escaped("<span style=\"italic\">[%s]</span>", date);
	
	gtk_label_set_markup(update_viewer->update_datetime_label, sexy_text);
	uber_free(sexy_text);
	
	
	/*
	 * gchar sexy_update has already been formatted to include urls, hyperlinks, titles, & etc.
	 * So we just set it as a SexyLable & bypass UberChickLabel
	 */
	debug("Setting 'sexy_update' for 'selected_update':\n\t\t%s.", sexy_update);
	/*if(!gconfig_if_bool(PREFS_URLS_EXPANSION_SELECTED_ONLY, TRUE))
		gtk_label_set_markup(GTK_LABEL(update_viewer->sexy_update), sexy_update);
	else*/
		uberchick_label_set_markup(update_viewer->sexy_update, service, update_type, user_name, user_id, id, sexy_update, TRUE, TRUE);
	
	debug("Selecting 'sexy_entry' for entering a new update.");
	update_viewer_sexy_select();
}/*update_viewer_view_update(service, update_id, user_id, user_name, nick_name, date, sexy_update, text_update, pixbuf, Timelines|DMs|Replies|None, 0.0|status->retweeted_status->id);*/

static void update_viewer_insert_shortened_uri(GtkButton *shorten_uri_button, UpdateViewer *update_viewer){
	gtk_widget_set_sensitive(GTK_WIDGET(update_viewer->sexy_entry), FALSE);
	online_service_request_popup_shorten_uri();
}/*update_viewer_insert_shortened_uri(update_viewer->shorten_uri_button, update_viewer);*/

static void update_viewer_sexy_entry_setup(UpdateViewer *update_viewer){
	debug("Creating UpdateViewer's entry, \"update_viewer->sexy_entry\", using SexyEntry, and adding it to UpdateViewer's \"sexy_entry_combo_box_entry\".");
	update_viewer->sexy_entry=(SexySpellEntry *)sexy_spell_entry_new();
	update_viewer->sexy_entry=g_object_ref_sink(update_viewer->sexy_entry);
	gtk_entry_set_icon_from_stock(GTK_ENTRY(update_viewer->sexy_entry), GTK_ENTRY_ICON_PRIMARY, GTK_STOCK_NEW);
	gtk_entry_set_icon_tooltip_markup(GTK_ENTRY(update_viewer->sexy_entry), GTK_ENTRY_ICON_PRIMARY, "<b>Reset the update you're viewing &amp; restore the update viewer to a clear canvas &amp; an empty update.</b>");
	gtk_entry_set_icon_from_stock(GTK_ENTRY(update_viewer->sexy_entry), GTK_ENTRY_ICON_SECONDARY, GTK_STOCK_CLEAR);
	gtk_entry_set_icon_tooltip_markup(GTK_ENTRY(update_viewer->sexy_entry), GTK_ENTRY_ICON_SECONDARY, "<i>Clear all of text you've entered &amp; start a new update.</i>");
	gtk_container_remove(GTK_CONTAINER(update_viewer->sexy_entry_combo_box_entry), gtk_bin_get_child(GTK_BIN(update_viewer->sexy_entry_combo_box_entry)));
	gtk_container_add(GTK_CONTAINER(update_viewer->sexy_entry_combo_box_entry), GTK_WIDGET(update_viewer->sexy_entry));
	update_viewer->sexy_completion=gtk_entry_completion_new();
	gtk_entry_completion_set_model(update_viewer->sexy_completion, GTK_TREE_MODEL(update_viewer->previous_updates_list_store));
	gtk_entry_set_completion(GTK_ENTRY(update_viewer->sexy_entry), update_viewer->sexy_completion);
	gtk_combo_box_entry_set_text_column(update_viewer->sexy_entry_combo_box_entry, GSTRING_UPDATE);
	gtk_entry_completion_set_text_column(update_viewer->sexy_completion, GSTRING_UPDATE);
	update_viewer_previous_updates_load(update_viewer);
	gtk_widget_show(GTK_WIDGET(update_viewer->sexy_entry));
	
	g_signal_connect(update_viewer->sexy_entry, "icon-press", G_CALLBACK(update_viewer_sexy_entry_icon_clicked), NULL);
	g_signal_connect_after(update_viewer->sexy_entry, "key-press-event", G_CALLBACK(hotkey_pressed), NULL);
	g_signal_connect_after(update_viewer->sexy_entry, "key-release-event", G_CALLBACK(update_viewer_sexy_entry_update_remaining_character_count), NULL);
	g_signal_connect(update_viewer->sexy_entry, "activate", G_CALLBACK(update_viewer_send), NULL);
}/*update_viewer_sexy_entry_setup(update_viewer);*/

static void update_viewer_sexy_entry_icon_clicked(GtkEntry *sexy_entry, GtkEntryIconPosition icon_clicked, GdkEvent *event){
	switch(icon_clicked){
		case GTK_ENTRY_ICON_PRIMARY:
			update_viewer_new_update();
			return;
		case GTK_ENTRY_ICON_SECONDARY:
			update_viewer_sexy_entry_clear();
			return;
	}
}/*update_viewer_sexy_entry_icon_clicked(update_viewer->sexy_entry, GTK_ENTRY_ICON_PRIMARY|GTK_ENTRY_ICON_SECONDARY, event);*/

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
	if(!character_count)
		if(in_reply_to_service==update_viewer->viewing_service && in_reply_to_status_id==update_viewer->viewing_update_id){
			in_reply_to_service=NULL;
			in_reply_to_status_id=0.0;
		}
	
	return UPDATE_MAX_LENGTH-character_count;
}/*update_viewer_sexy_entry_update_length(update);*/

static void update_viewer_sexy_entry_update_remaining_character_count(void){
	update_viewer->sexy_entry_text_position=gtk_editable_get_position(GTK_EDITABLE(GTK_ENTRY(update_viewer->sexy_entry)));
	gshort remaining_character_count=update_viewer_sexy_entry_update_length(GTK_ENTRY(update_viewer->sexy_entry)->text);
	gchar *remaining_characters_markup_label=NULL;
	if(remaining_character_count < 0){
		if(!gconfig_if_bool(PREFS_DISABLE_UPDATE_LENGTH_ALERT, FALSE))
			update_viewer_beep();
		remaining_characters_markup_label=g_strdup_printf("<span size=\"small\" foreground=\"red\">%d</span>", remaining_character_count);
	}else{
		remaining_characters_markup_label=g_strdup_printf("<span size=\"small\" foreground=\"green\">%d</span>", remaining_character_count);
	}
	
	gtk_label_set_markup(update_viewer->char_count, remaining_characters_markup_label);
	uber_free(remaining_characters_markup_label);
}/*update_viewer_sexy_entry_update_remaining_character_count();*/

static gboolean update_viewer_sexy_entry_confirm_clear(void){
	if(G_STR_EMPTY( GTK_ENTRY(update_viewer->sexy_entry)->text )) return TRUE;
	return online_service_request_popup_confirmation_dialog(
			PREFS_UPDATE_VIEWER_CLEAR,
			_("You are about to empty your current update:"),
			_("Are you sure you want to delete your current update?"),
			NULL, NULL
	);
}/*update_viewer_sexy_entry_confirm_clear();*/

gboolean update_viewer_sexy_entry_clear(void){
	if(!(update_viewer_sexy_entry_confirm_clear()))
		return FALSE;
	
	update_viewer_sexy_set(NULL);
	return TRUE;
}/*update_viewer_sexy_entry_clear();*/

static void update_viewer_sexy_set(gchar *text){
	gtk_entry_set_text(GTK_ENTRY(update_viewer->sexy_entry), (text ?text :(gchar *)"") );
	update_viewer_sexy_entry_update_remaining_character_count();
	update_viewer_sexy_select();
}/*update_viewer_sexy_set(NULL|"an phase or update");*/

void update_viewer_sexy_prefix_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	update_viewer_sexy_prefix_string((const gchar *)str, FALSE, FALSE);
	uber_free(str);
}/*update_viewer_sexy_prefix_char('\t', TRUE|FALSE, TRUE|FALSE);*/

gboolean update_viewer_sexy_prefix_string(const gchar *str, gboolean to_lower, gboolean uniq){
	return update_viewer_sexy_puts(str, 0, to_lower, uniq);
}/*update_viewer_sexy_prefix_string("string", TRUE|FALSE, TRUE|FALSE);*/

void update_viewer_sexy_insert_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	update_viewer_sexy_insert_string((const gchar *)str, FALSE, FALSE);
	uber_free(str);
}/*update_viewer_sexy_insert_char('\t', TRUE|FALSE, TRUE|FALSE);*/

gboolean update_viewer_sexy_insert_string(const gchar *str, gboolean to_lower, gboolean uniq){
	if(gtk_widget_is_sensitive(GTK_WIDGET(update_viewer->sexy_entry)))
		gtk_widget_set_sensitive(GTK_WIDGET(update_viewer->sexy_entry), TRUE);
	return update_viewer_sexy_puts(str, gtk_editable_get_position(GTK_EDITABLE(update_viewer->sexy_entry)), to_lower, uniq);
}/*update_viewer_sexy_insert_string("string", TRUE|FALSE, TRUE|FALSE);*/

void update_viewer_sexy_append_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	update_viewer_sexy_append_string((const gchar *)str, FALSE, FALSE);
	uber_free(str);
}/*update_viewer_sexy_append_char('\t', TRUE|FALSE, TRUE|FALSE);*/

gboolean update_viewer_sexy_append_string(const gchar *str, gboolean to_lower, gboolean uniq){
	return update_viewer_sexy_puts(str, (gint)gtk_entry_get_text_length(GTK_ENTRY(update_viewer->sexy_entry)), to_lower, uniq);
}/*update_viewer_sexy_append_string("string", TRUE|FALSE, TRUE|FALSE);*/

static gboolean update_viewer_sexy_puts(const gchar *str, gint position_after, gboolean to_lower, gboolean uniq){
	if(G_STR_EMPTY(str))
		return FALSE;
	
	gchar *string=NULL;
	gboolean free_string=FALSE;
	GMatchInfo *match_info=NULL;
	if(to_lower && g_regex_match(update_viewer->upper_case_regex, str, 0, &match_info)){
		free_string=TRUE;
		string=g_utf8_strdown(str, -1);
	}else
		string=(gchar *)str;
	g_match_info_free(match_info);
	
	if(uniq && G_STR_N_EMPTY(GTK_ENTRY(update_viewer->sexy_entry)->text) && g_strrstr(GTK_ENTRY(update_viewer->sexy_entry)->text, string)){
		if(free_string) uber_free(string);
		return FALSE;
	}
	
	if(uniq && GTK_ENTRY(update_viewer->sexy_entry)->text[0]=='@' && string[0]=='@'){
		if(free_string) uber_free(string);
		return FALSE;
	}
	
	gint position_prior=gtk_editable_get_position(GTK_EDITABLE(update_viewer->sexy_entry));
	gtk_editable_insert_text(GTK_EDITABLE(update_viewer->sexy_entry), string, -1, &position_after);
	if(position_after<position_prior)
		update_viewer->sexy_entry_text_position=position_prior;
	else
		update_viewer->sexy_entry_text_position=position_after;
	
	gtk_entry_set_position(GTK_ENTRY(update_viewer->sexy_entry), update_viewer->sexy_entry_text_position);
	
	if(free_string) uber_free(string);
	
	update_viewer_sexy_entry_update_remaining_character_count();
	update_viewer_sexy_select();
	return TRUE;
}/*update_viewer_sexy_puts("string", 0...139, TRUE|FALSE, TRUE|FALSE);*/

void update_viewer_reply(void){
	if(!update_viewer->viewing_update_id)
		return update_viewer_beep();
	
	update_viewer_reply_or_forward(update_viewer->reply_button);
}/*update_viewer_reply();*/

void update_viewer_forward(void){
	update_viewer_reply_or_forward(update_viewer->forward_update_button);
}/*update_viewer_forward();*/

static void update_viewer_reply_or_forward(GtkButton *update_button){
	if(!online_service_request_selected_update_get_user_name())
		update_viewer_beep();
	
	if(update_viewer->viewing_update_id && update_viewer->viewing_service){
		if(in_reply_to_status_id!=update_viewer->viewing_update_id)
			in_reply_to_status_id=update_viewer->viewing_update_id;
		if(in_reply_to_service!=update_viewer->viewing_service)
			in_reply_to_service=update_viewer->viewing_service;
	}
	
	if(update_button!=update_viewer->forward_update_button)
		online_service_request_selected_update_reply();
	else
		online_service_request_selected_update_forward();
}/*update_viewer_reply_or_forward(button);*/

void update_viewer_retweet(void){
	OnlineService *service=online_service_request_selected_update_get_service();
	if(!service) return;
	
	gchar *retweet_uri=g_strdup_printf(API_RETWEET_UPDATE, online_service_request_selected_update_get_id_str());
	online_service_request(service, POST, retweet_uri, NULL, network_update_posted, (gchar *)online_service_request_selected_update_get_user_name(), NULL);
	uber_free(retweet_uri);
}/*update_viewer_retweet();*/

void update_viewer_send(GtkWidget *activated_widget){
	if( G_STR_EMPTY(GTK_ENTRY(update_viewer->sexy_entry)->text) && activated_widget==GTK_WIDGET(update_viewer->sexy_entry) && online_service_request_selected_update_get_user_name() )
		return update_viewer_reply();
	
	GMatchInfo *match_info=NULL;
	gboolean contains_username_only=g_regex_match(update_viewer->username_regex, GTK_ENTRY(update_viewer->sexy_entry)->text, 0, &match_info);
	g_match_info_free(match_info);
	
	if(contains_username_only)
		return update_viewer_beep();
	
	if(!activated_widget){
		if(update_viewer->best_friends_service && update_viewer->best_friends_user_name)
			return update_viewer_sexy_send(update_viewer->best_friends_service, update_viewer->best_friends_user_name);
		return update_viewer_sexy_send(NULL, NULL);
	}
	
	const gchar *user_name=NULL;
	if(activated_widget==GTK_WIDGET(update_viewer->sexy_dm_button) ){
		if(!(( user_name=online_service_request_selected_update_get_user_name()) && G_STR_N_EMPTY(user_name) )) return update_viewer_beep();
		update_viewer_sexy_send(online_service_request_selected_update_get_service(), user_name);
		return;
	}
	
	if((activated_widget==GTK_WIDGET(update_viewer->followers_send_dm)) &&(GTK_WIDGET_IS_SENSITIVE(update_viewer->followers_combo_box)) && G_STR_N_EMPTY((user_name=gtk_combo_box_get_active_text(update_viewer->followers_combo_box)) ) ){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		User *user=NULL;
		gtk_combo_box_get_active_iter(update_viewer->followers_combo_box, iter);
		gtk_tree_model_get(
					GTK_TREE_MODEL(update_viewer->followers_list_store),
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
	update_viewer_sexy_entry_clear();
	main_window_set_statusbar_default_message( _("Hotkeys: Press Up, Down, Page Up, or Page Down to browse updates.  Press & Hold <ALT+CTRL> while browsing to select the update.") );
	update_viewer_view_update((selected_service ?selected_service :online_services_connected_get_first()), 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, None, 0.0);
	
	if(update_viewer->best_friends_service) update_viewer->best_friends_service=NULL;
	if(update_viewer->best_friends_user_name) uber_free(update_viewer->best_friends_user_name);
	
	update_viewer_set_in_reply_to_data(NULL, None, NULL, NULL, 0.0, 0.0, FALSE, FALSE, TRUE);
	
	gtk_widget_hide( (GtkWidget *)update_viewer->best_friend_dm_notification_label );
	online_service_request_unset_selected_update();
	
	if(gtk_toggle_button_get_active(update_viewer->best_friend_toggle_button))
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
	
	update_viewer_previous_update_prepend(update_viewer, GTK_ENTRY(update_viewer->sexy_entry)->text);
	
	if(!(service && user_name && G_STR_N_EMPTY(user_name) ))
		network_post_status(GTK_ENTRY(update_viewer->sexy_entry)->text);
	else
		network_send_message(service, user_name, GTK_ENTRY(update_viewer->sexy_entry)->text);
	
	update_viewer_sexy_set(NULL);
}/*update_viewer_sexy_send(online_service_request_selected_update_get_service(), online_service_request_selected_update_get_user_name() );*/

static void update_viewer_previous_updates_load(UpdateViewer *update_viewer){
	update_viewer_set_in_reply_to_data(NULL, None, NULL, NULL, 0.0, 0.0, FALSE, FALSE, TRUE);
	
	gchar *previous_update=NULL, *previous_update_gconf_path=NULL;
	update_viewer_previous_maxium_updates_validate(update_viewer);
	for(update_viewer->total_updates=0; update_viewer->total_updates<=update_viewer->max_updates; update_viewer->total_updates++){
		if(!( (gconfig_get_string( (previous_update_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "updates", update_viewer->total_updates)), &previous_update )) && G_STR_N_EMPTY(previous_update) )) break;
		
		update_viewer_previous_update_restore(update_viewer, (strstr(previous_update, "->") ?g_strrstr(previous_update, "->")+strlen("->") :previous_update) );
		
		uber_free(previous_update);
		uber_free(previous_update_gconf_path);
	}
	
	update_viewer_previous_update_prepend(update_viewer, "[new update]");
	if(previous_update) uber_free(previous_update);
	if(previous_update_gconf_path) uber_free(previous_update_gconf_path);
}/*update_view_previous_updates_load(update_viewer);*/

static void update_viewer_previous_update_add(UpdateViewer *update_viewer, const gchar *update, gint list_store_index){
	if(G_STR_EMPTY(update)) return;
	
	if(
		G_STR_N_EMPTY(update)
		&&
		g_str_n_equal(update, "[new update]")
		&&
		gconfig_if_bool(PREFS_UPDATES_HISTORY_UNIQUE_ONLY, TRUE)
		&&
		!update_viewer_previous_updates_is_unique(update_viewer, update)
	){
		debug("Update being sent: %s is already in the previous update's list and will not be stored again.", update);
		return;
	}
	
	update_viewer_previous_maxium_updates_validate(update_viewer);
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	if(list_store_index<-1)
		gtk_list_store_append(update_viewer->previous_updates_list_store, iter);
	else if(list_store_index==-1)
		gtk_list_store_prepend(update_viewer->previous_updates_list_store, iter);
	else if(list_store_index>-1)
		gtk_list_store_insert(update_viewer->previous_updates_list_store, iter, (list_store_index<update_viewer->max_updates ?list_store_index :update_viewer->max_updates ));
	gtk_list_store_set(
			update_viewer->previous_updates_list_store, iter,
				GSTRING_UPDATE, g_strdup(update),
			-1
	);
	uber_free(iter);
	
	if( list_store_index==-3 || g_str_equal(update, "[new update]") ) return;
	
	update_viewer_previous_updates_remove(update_viewer, 1);
	update_viewer_previous_update_add(update_viewer, "[new update]", list_store_index);
	if(update_viewer->total_updates<update_viewer->max_updates)
		update_viewer->total_updates++;
	else
		for(; update_viewer->total_updates>=update_viewer->max_updates; update_viewer->total_updates--)
			update_viewer_previous_updates_remove(update_viewer, update_viewer->total_updates);
	
	update_viewer_previous_updates_rotate(update_viewer);
	gchar *previous_update_gconf_path=NULL;
	gconfig_set_string( (previous_update_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "updates", 0)), update);
	uber_free(previous_update_gconf_path);
}/*update_viewer_previous_update_add(update_viewer, GTK_ENTRY(update_viewer->sexy_entry)->text, -2 to prepend|-1 to append|>0 to instert at this index);*/

static gint update_viewer_previous_maxium_updates_validate(UpdateViewer *update_viewer){
	if(!update_viewer) return 0;
	gconfig_get_int_or_default(PREFS_UPDATES_HISTORY_MAXIMUM, &update_viewer->max_updates, 50);
	if(update_viewer->max_updates < 5)
		gconfig_set_int(PREFS_UPDATES_HISTORY_MAXIMUM, (update_viewer->max_updates=5));
	else if(update_viewer->max_updates > 100)
		gconfig_set_int(PREFS_UPDATES_HISTORY_MAXIMUM, (update_viewer->max_updates=100));
	return update_viewer->max_updates;
}/*update_viewer_previous_maxium_updates_validate(update_viewer);*/

static gboolean update_viewer_previous_updates_is_unique(UpdateViewer *update_viewer, const gchar *update){
	if(G_STR_EMPTY(update)) return FALSE;
	
	if(g_str_equal(update, "[new update]"))
		return TRUE;
	
	debug("Checking uniqueness of update.  New update:\t[%s]", update);
	gboolean uniq=TRUE;
	gchar *update_at_index=NULL;
	for(gint i=1; i<=update_viewer->total_updates && uniq; i++) {
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(update_viewer->previous_updates_list_store), iter, path)){
			debug("Checking update, at index: %d, failed to get valid iter for the list store.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gtk_tree_model_get(
				GTK_TREE_MODEL(update_viewer->previous_updates_list_store), iter,
					GSTRING_UPDATE, &update_at_index,
				-1
		);
		
		if(!strcmp(update, update_at_index)){
			debug("Update is not unique.  Duplicate update found at index: %d.", i);
			debug("\tComparing new update: [%s]", update);
			debug("\tAgainst old update: [%s]", update_at_index);
			uniq=FALSE;
		}
		
		gtk_tree_path_free(path);
		uber_free(update_at_index);
		uber_free(iter);
	}
	return uniq;
}/*update_viewer_previous_updates_is_unique(update_viewer, update);*/

static void update_viewer_previous_updates_remove(UpdateViewer *update_viewer, gint list_store_index){
	if(!(list_store_index > 0 && list_store_index <= update_viewer->total_updates)) return;
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	GtkTreePath *path=gtk_tree_path_new_from_indices(list_store_index, -1);
	debug("Removing saved update %d.  Total update saved: %d", list_store_index, update_viewer->total_updates);
	if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(update_viewer->previous_updates_list_store), iter, path))
		debug("Removing saved update, at index: %d, failed to get valid iter for the list store.", list_store_index);
	else{
		debug("Removing iter at index: %d", list_store_index);
		gtk_list_store_remove(update_viewer->previous_updates_list_store, iter);
	}
	gtk_tree_path_free(path);
	uber_free(iter);
}/*update_viewer_previous_updates_remove(update_viewer, update_viewer->total_updates);*/

static void update_viewer_previous_updates_rotate(UpdateViewer *update_viewer){
	gchar *previous_update=NULL, *previous_update_gconf_path=NULL;
	for(gint i=update_viewer->max_updates; i>=0; i--){
		if( (gconfig_get_string( (previous_update_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "updates", i-1)), &previous_update)) && G_STR_N_EMPTY(previous_update) ){
			uber_free(previous_update_gconf_path);
			previous_update_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "updates", i);
			gconfig_set_string(previous_update_gconf_path, previous_update);
		}
		if(previous_update) uber_free(previous_update);
		uber_free(previous_update_gconf_path);
	}
}/*update_viewer_previous_updates_rotate(update_viewer);*/

static void update_viewer_previous_update_selected(GtkComboBoxEntry *sexy_entry_combo_box_entry, UpdateViewer *update_viewer){
	GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
	if(!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(sexy_entry_combo_box_entry), iter)){
		uber_free(iter);
		return;
	}
	
	gchar	*update=NULL;
	gtk_tree_model_get(
				gtk_combo_box_get_model(GTK_COMBO_BOX(sexy_entry_combo_box_entry)), iter,
					GSTRING_UPDATE, &update,
				-1
	);
	
	if( G_STR_EMPTY(update) || g_str_equal(update, "[new update]") ){
		update_viewer_sexy_set(NULL);
		uber_free(update);
		uber_free(iter);
		return;
	}
	
	update_viewer_sexy_set( (strstr(update, "->") ?g_strrstr(update, "->") :update ) );
	uber_free(update);
	uber_free(iter);
}/*update_viewer_previous_update_selected(update_viewer->sexy_entry_combo_box_entry, update_viewer);*/

static void update_viewer_previous_updates_free(UpdateViewer *update_viewer){
	debug("Removing all %d saved updates from this session.", update_viewer->total_updates);
	gtk_list_store_clear(update_viewer->previous_updates_list_store);
}/*update_viewer_previous_updates_free(update_viewer);*/

void update_viewer_beep(void){
	if(!gconfig_if_bool(PREFS_DISABLE_SYSTEM_BELL, FALSE))
		gtk_widget_error_bell(GTK_WIDGET(update_viewer->sexy_entry));
}/*update_viewer_beep();*/

SexySpellEntry *update_viewer_sexy_entry_get_widget(void){
	if(!update_viewer->sexy_entry) return NULL;
	return update_viewer->sexy_entry;
}/*update_viewer_sexy_entry_get_widget();*/

void update_viewer_select_all(void){
	if(gtk_widget_has_focus(GTK_WIDGET(main_window_sexy_search_entry_get_widget()))) return;
	gtk_editable_select_region(GTK_EDITABLE(update_viewer->sexy_entry), 0, -1);
}/*update_viewer_select_all();*/

void update_viewer_sexy_select(void){
	if(gtk_widget_has_focus(GTK_WIDGET(update_viewer->sexy_entry))) return;
	if(gtk_widget_has_focus(GTK_WIDGET(main_window_sexy_search_entry_get_widget()))) return;
	if(gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE))
		window_present(update_viewer->update_viewer, TRUE);
	gtk_widget_grab_focus(GTK_WIDGET(update_viewer->sexy_entry));
	gint sexy_entry_text_position=-1;
	if( update_viewer->sexy_entry_text_position > 0 && update_viewer->sexy_entry_text_position <= gtk_entry_get_text_length((GtkEntry *)update_viewer->sexy_entry) )
		sexy_entry_text_position=update_viewer->sexy_entry_text_position;
	gtk_entry_set_position(GTK_ENTRY(update_viewer->sexy_entry), sexy_entry_text_position);
}/*update_viewer_sexy_select*/

void update_viewer_show_previous_updates(void){
	g_signal_emit_by_name(update_viewer->sexy_entry_combo_box_entry, "popup");
}/*update_viewer_show_previous_updates();*/

void update_viewer_hide_previous_updates(void){
	g_signal_emit_by_name(update_viewer->sexy_entry_combo_box_entry, "popdown");
}/*update_viewer_hide_previous_updates();*/


/*END:UBERCHICK_SEXY_ENTRY_COMPLETION_COMBO_BOX*/



void update_viewer_new_dm(void){
	gtk_toggle_button_set_active(update_viewer->dm_form_active_toggle_button, !gtk_toggle_button_get_active(update_viewer->dm_form_active_toggle_button));
}/*update_viewer_new_dm*/

static void update_viewer_dm_show(GtkToggleButton *toggle_button){
	if(!gtk_toggle_button_get_active(update_viewer->dm_form_active_toggle_button)){
		debug("Disabled UpdateViewer's dm form.");
		update_viewer_dm_form_activate(FALSE);
		return;
	}
	online_service_request_popup_select_service();
	
	if(!( selected_service)) {
		if(gtk_toggle_button_get_active(update_viewer->dm_form_active_toggle_button))
			gtk_toggle_button_set_active(update_viewer->dm_form_active_toggle_button, FALSE);
		return;
	}
	
	debug("Enabling UpdateViewer's dm form.");
	update_viewer_dm_form_activate(TRUE);
	update_viewer_dm_form_set_loading_label(update_viewer);
	users_glist_get(GetFollowers, FALSE, update_viewer_dm_data_fill);
}/*update_viewer_dm_show*/

static void update_viewer_dm_refresh(GtkButton *dm_refresh, UpdateViewer *update_viewer){
	update_viewer_dm_form_set_loading_label(update_viewer);
	online_service_request_popup_select_service();
	users_glist_get(GetFollowers, TRUE, update_viewer_dm_data_fill);
}/*update_viewer_dm_refresh(update_viewer->dm_refresh, update_viewer);*/

static void update_viewer_dm_data_fill(GList *followers){
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
		if(!( (user=(User *)followers->data) )) continue;
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
	gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->dm_frame_label), new_label);
	gtk_label_set_markup(update_viewer->dm_frame_label, new_label);
	gtk_label_set_use_underline(update_viewer->dm_frame_label, TRUE);
	gtk_label_set_single_line_mode(update_viewer->dm_frame_label, TRUE);
	uber_free(new_label);
	
	new_label=g_markup_printf_escaped("<i><b>Select one of your followers to send a Direct/Private message to...</b></i>");
	gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->followers_combo_box), new_label);
	uber_free(new_label);
	
	if(first_fill) first_fill=FALSE;
}/*update_viewer_dm_data_fill( friends );*/

static void update_viewer_dm_form_set_loading_label(UpdateViewer *update_viewer){
	gchar *new_label=g_markup_printf_escaped("<i><b>Please wait while your followers load.  To be able to send a _DM...</b></i>");
	gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->followers_combo_box), new_label);
	gtk_widget_set_tooltip_markup(GTK_WIDGET(update_viewer->dm_frame_label), new_label);
	gtk_label_set_markup(update_viewer->dm_frame_label, new_label);
	gtk_label_set_use_underline(update_viewer->dm_frame_label, TRUE);
	gtk_label_set_single_line_mode(update_viewer->dm_frame_label, TRUE);
	uber_free(new_label);
}/*update_viewer_dm_form_set_loading_label(update_viewer);*/

/********************************************************
 *                       eof                            *
 ********************************************************/
