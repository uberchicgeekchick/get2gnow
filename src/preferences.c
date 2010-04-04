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
#include <string.h>
#include <glib/gi18n.h>

#include "config.h"
#include "program.h"

#include "online-services.rest-uris.defines.h"
#include "online-services.typedefs.h"
#include "online-services.h"

#include "network.h"
#include "gconfig.h"
#include "gtkbuilder.h"

#include "main-window.h"
#include "update-viewer.h"
#include "preferences.defines.h"
#include "preferences.h"

#include "ui-utils.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
typedef struct _PreferencesDialog PreferencesDialog;
typedef struct _replace_me_with replace_me_with;
typedef struct _reload_time reload_time;

struct _PreferencesDialog{
	GtkDialog	*dialog;
	GtkNotebook	*notebook;
	GtkComboBox	*combo_default_timeline;
	GtkComboBox	*combo_reload;
	
	/* Checkbuttons */
	GtkCheckButton	*disable_update_length_alert_check_button;
	GtkComboBox	*replace_me_with_combo_box;
	
	GtkCheckButton	*autoload_best_friends_updates_check_button;
	GtkCheckButton	*autoload_dms_check_button;
	GtkCheckButton	*autoload_replies_check_button;
	GtkCheckButton	*autoload_homepage_check_button;
	
	GtkCheckButton	*notify_dms_check_button;
	GtkCheckButton	*notify_at_mentions_check_button;
	GtkCheckButton	*notify_following_updates_check_button;
	GtkCheckButton	*notify_best_friends_updates_check_button;
	GtkCheckButton	*notify_all_new_updates;
	GtkCheckButton	*notify_beep_updates_check_button;
	
	GtkCheckButton	*disable_system_bell;
	
	GtkCheckButton	*compact_entire_view_check_button;
	GtkCheckButton	*concatenated_updates_check_button;
	GtkCheckButton	*use_dialog_toggle_button;
	GtkCheckButton	*compact_view_toggle_button;
	
	GtkCheckButton	*expand_urls_disabled_checkbutton;
	GtkCheckButton	*expand_urls_selected_only_checkbutton;
	GtkCheckButton	*titles_only_checkbutton;
	GtkCheckButton	*expand_users_checkbutton;
	
	GtkCheckButton	*updates_add_profile_link_checkbutton;
	GtkCheckButton	*post_reply_to_service_only_checkbutton;
	
	GtkCheckButton	*previous_updates_uniq_check_button;
	GtkComboBox	*previous_updates_maximum_combo_box;
	
	GtkCheckButton	*search_history_uniq_check_button;
	GtkComboBox	*search_history_maximum_combo_box;
	
	GList		*notify_ids;
};

struct _reload_time{
	gint   minutes;
	gchar *display_text;
};

reload_time reload_list[]={
	{5, N_("5 minutes")},
	{10, N_("10 minutes")},
	{15, N_("15 minutes")},
	{30, N_("30 minutes")},
	{45, N_("45 minutes")},
	{60, N_("1 hour")},
	{0, NULL}
};

static const struct {
	gint value;
}

maximum_entries[]={
	{5},
	{10},
	{15},
	{25},
	{35},
	{45},
	{50},
	{55},
	{60},
	{65},
	{70},
	{75},
	{80},
	{85},
	{90},
	{95},
	{100},
};

struct _replace_me_with{
	gint value;
	gchar *label;
};

replace_me_with replace_me_list[]={
	{2, N_("My user name, i.e. the user name I use login.")},
	{1, N_("My nick name, i.e. my screen name / full name.")},
	{-1, N_("Nothing")},
	{0, NULL}
};

enum {
	COLUMN_COMBO_NAME,
	COLUMN_COMBO_VALUE,
	COLUMN_COMBO_COUNT,
};

/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define GTK_BUILDER_UI_FILENAME "preferences"

#define DEBUG_DOMAINS "UI:GtkBuilder:GtkBuildable:OnlineServices:Updates:Notification:Settings:Setup:Start-Up:Preferences:preferences.c"
#include "debug.h"

static PreferencesDialog *prefs=NULL;


/********************************************************************************
 *                    prototypes for private method & function                  *
 ********************************************************************************/
static void preferences_setup_widgets(PreferencesDialog *prefs);
static void preferences_timeline_setup(PreferencesDialog *prefs);
static void preferences_replace_with_setup(PreferencesDialog *prefs);
static void preferences_max_int_combo_box_setup(PreferencesDialog *prefs, GtkComboBox *int_combo_box, gint max_int_value, const gchar *int_label);

static void preferences_widget_sync_bool(const gchar *key, GtkCheckButton *check_button);
static void preferences_widget_sync_string_combo(const gchar *key, GtkComboBox *combo_box);
static void preferences_widget_sync_int_combo(const gchar *key, GtkComboBox *combo_box);

static void preferences_notify_bool_cb(const gchar *key, gpointer user_data);
static void preferences_notify_string_combo_cb(const gchar *key, gpointer user_data);
static void preferences_notify_int_combo_cb(const gchar *key, gpointer user_data);

static void preferences_hookup_toggle_button(PreferencesDialog *prefs, const gchar *key, gboolean bool_default, GtkCheckButton *check_button);
static void preferences_hookup_string_combo(PreferencesDialog *prefs, const gchar *key, GtkComboBox *combo_box);
static void preferences_hookup_int_combo(PreferencesDialog *prefs, const gchar *key, const gint default_int, GtkComboBox *combo_box);

static void preferences_toggle_button_toggled_cb(GtkCheckButton *check_button, PreferencesDialog *prefs);
static void preferences_string_combo_changed_cb(GtkComboBox *combo_box, PreferencesDialog *prefs);
static void preferences_int_combo_changed_cb(GtkComboBox *combo_box, PreferencesDialog *prefs);

static void preferences_response_cb(GtkDialog *dialog, gint response, PreferencesDialog *prefs);
static void preferences_destroy_cb(GtkDialog *dialog, PreferencesDialog *prefs);



/********************************************************************************
 *                  'Here be Dragons'...art, beauty, fun, & magic.              *
 ********************************************************************************/
static void preferences_setup_widgets(PreferencesDialog *prefs){
	debug("Binding widgets to preferences");
	preferences_hookup_toggle_button(prefs, PREFS_AUTOLOAD_BEST_FRIENDS, TRUE, prefs->autoload_best_friends_updates_check_button);
	preferences_hookup_toggle_button(prefs, PREFS_AUTOLOAD_DMS, TRUE, prefs->autoload_dms_check_button);
	preferences_hookup_toggle_button(prefs, PREFS_AUTOLOAD_REPLIES, TRUE, prefs->autoload_replies_check_button);
	preferences_hookup_toggle_button(prefs, PREFS_AUTOLOAD_HOMEPAGE, TRUE, prefs->autoload_homepage_check_button);
	
	preferences_hookup_toggle_button(prefs, PREFS_NOTIFY_DMS, TRUE, prefs->notify_dms_check_button);
	preferences_hookup_toggle_button(prefs, PREFS_NOTIFY_REPLIES, TRUE, prefs->notify_at_mentions_check_button);
	preferences_hookup_toggle_button(prefs, PREFS_NOTIFY_FOLLOWING, TRUE, prefs->notify_following_updates_check_button);
	preferences_hookup_toggle_button(prefs, PREFS_NOTIFY_BEST_FRIENDS, TRUE, prefs->notify_best_friends_updates_check_button);
	preferences_hookup_toggle_button(prefs, PREFS_NOTIFY_ALL, TRUE, prefs->notify_all_new_updates);
	preferences_hookup_toggle_button(prefs, PREFS_NOTIFY_BEEP, TRUE, prefs->notify_beep_updates_check_button);
	
	preferences_hookup_toggle_button(prefs, PREFS_DISABLE_UPDATE_LENGTH_ALERT, TRUE, prefs->disable_update_length_alert_check_button);
	
	preferences_hookup_toggle_button(prefs, COMPACT_VIEW, FALSE, prefs->compact_entire_view_check_button);
	preferences_hookup_toggle_button(prefs, CONCATENATED_UPDATES, FALSE, prefs->concatenated_updates_check_button);
	preferences_hookup_toggle_button(prefs, PREFS_UPDATE_VIEWER_COMPACT, FALSE, prefs->compact_view_toggle_button);
	preferences_hookup_toggle_button(prefs, PREFS_UPDATE_VIEWER_DIALOG, FALSE, prefs->use_dialog_toggle_button);
	
	preferences_hookup_toggle_button(prefs, PREFS_DISABLE_SYSTEM_BELL, FALSE, prefs->disable_system_bell);
	
	preferences_hookup_toggle_button(prefs, PREFS_URLS_EXPANSION_USER_PROFILES, TRUE, prefs->expand_users_checkbutton);
	preferences_hookup_toggle_button(prefs, PREFS_URLS_EXPANSION_SELECTED_ONLY, TRUE, prefs->expand_urls_selected_only_checkbutton);
	preferences_hookup_toggle_button(prefs, PREFS_URLS_EXPANSION_REPLACE_WITH_TITLES, TRUE, prefs->titles_only_checkbutton);
	preferences_hookup_toggle_button(prefs, PREFS_URLS_EXPANSION_DISABLED, FALSE, prefs->expand_urls_disabled_checkbutton);
	
	preferences_hookup_toggle_button(prefs, PREFS_UPDATES_ADD_PROFILE_LINK, FALSE, prefs->updates_add_profile_link_checkbutton);
	preferences_hookup_toggle_button(prefs, PREFS_UPDATES_DIRECT_REPLY_ONLY, FALSE, prefs->post_reply_to_service_only_checkbutton);
	
	preferences_hookup_string_combo(prefs, PREFS_UPDATES_HOME_TIMELINE, prefs->combo_default_timeline);
	
	preferences_hookup_toggle_button(prefs, PREFS_UPDATES_HISTORY_UNIQUE_ONLY, TRUE, prefs->previous_updates_uniq_check_button);
	preferences_hookup_int_combo(prefs, PREFS_UPDATES_HISTORY_MAXIMUM, 50, prefs->previous_updates_maximum_combo_box);
	
	preferences_hookup_toggle_button(prefs, PREFS_SEARCH_HISTORY_UNIQUE_ONLY, TRUE, prefs->search_history_uniq_check_button);
	preferences_hookup_int_combo(prefs, PREFS_SEARCH_HISTORY_MAXIMUM, 25, prefs->search_history_maximum_combo_box);
	
	preferences_hookup_int_combo(prefs, PREFS_TIMELINE_RELOAD_MINUTES, 5, prefs->combo_reload);
	
	preferences_hookup_int_combo(prefs, PREFS_UPDATES_REPLACE_ME_W_NICK, 2, prefs->replace_me_with_combo_box);
}/*preferences_setup_widgets(prefs);*/

static void preferences_notify_bool_cb(const gchar *key, gpointer user_data){
	debug("Saving preference: %s", key );
	preferences_widget_sync_bool(key, user_data);
}

static void preferences_notify_string_combo_cb(const gchar *key, gpointer user_data){
	debug("Saving preference: %s", key );
	preferences_widget_sync_string_combo(key, user_data);
}

static void preferences_notify_int_combo_cb(const gchar *key, gpointer user_data){
	debug("Saving preference: %s", key );
	preferences_widget_sync_int_combo(key, user_data);
}

static void preferences_timeline_setup(PreferencesDialog *prefs){
	debug("Binding timelines to preference");
	static const gchar *timelines[] = {
		"",			N_("Nothing"),
		API_TIMELINE_HOMEPAGE,	N_("My Homepage"),
		API_REPLIES,		N_("@ Replies"),
		API_DIRECT_MESSAGES,	N_("My DMs Inbox"),
		API_FAVORITES,		N_("My Favorites"),
		API_TIMELINE_MINE,	N_("My Updates"),
		API_RETWEETED_TO_ME,	N_("My Friends' ReTweets"),
		API_RETWEETS_OF_ME,	N_("ReTweets of My Updates"),
		API_TIMELINE_PUBLIC,	N_("All Public Updates"),
		NULL
	};
	
	GtkCellRenderer *renderer=gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(prefs->combo_default_timeline), renderer, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(prefs->combo_default_timeline), renderer, "text", COLUMN_COMBO_NAME, NULL);
	
	GtkListStore *list_store=gtk_list_store_new(COLUMN_COMBO_COUNT, G_TYPE_STRING, G_TYPE_STRING);
	
	gtk_combo_box_set_model(GTK_COMBO_BOX(prefs->combo_default_timeline), GTK_TREE_MODEL(list_store));
	
	for(gint i=0; timelines[i]; i+=2) {
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		gtk_list_store_append(list_store, iter);
		gtk_list_store_set(
					list_store, iter,
						COLUMN_COMBO_NAME, timelines[i+1],
						COLUMN_COMBO_VALUE, timelines[i],
					-1
		);
		uber_free(iter);
	}
	
	uber_object_unref(list_store);
}/*static void preferences_timeline_setup(prefs);*/

static void preferences_replace_with_setup(PreferencesDialog *prefs){
	debug("Setting-up /me replacement preference");
	
	GtkListStore *list_store=gtk_list_store_new(COLUMN_COMBO_COUNT, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_combo_box_set_model(GTK_COMBO_BOX(prefs->replace_me_with_combo_box), GTK_TREE_MODEL(list_store));
	
	GtkCellRenderer *renderer=gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(prefs->replace_me_with_combo_box), renderer, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(prefs->replace_me_with_combo_box), renderer, "text", COLUMN_COMBO_NAME, NULL);
	
	replace_me_with	*replacements=replace_me_list;
	while(replacements->label){
		GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
		gtk_list_store_append(list_store, iter);
		gtk_list_store_set(
					list_store, iter,
						COLUMN_COMBO_NAME, replacements->label,
						COLUMN_COMBO_VALUE, replacements->value,
					-1
		);
		replacements++;
		uber_free(iter);
	}
	
	uber_object_unref(list_store);
}/*static void preferences_replace_with_setup(prefs);*/

static void preferences_max_int_combo_box_setup(PreferencesDialog *prefs, GtkComboBox *int_combo_box, gint max_int_value, const gchar *int_label){
	debug("Setting-up maximum %s preference combo box", int_label);
	
	GtkListStore *list_store=gtk_list_store_new(COLUMN_COMBO_COUNT, G_TYPE_STRING, G_TYPE_INT);
	
	gtk_combo_box_set_model(GTK_COMBO_BOX(int_combo_box), GTK_TREE_MODEL(list_store));
	
	GtkCellRenderer *renderer=gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(int_combo_box), renderer, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(int_combo_box), renderer, "text", COLUMN_COMBO_NAME, NULL);
	
	if(max_int_value < maximum_entries[0].value)
		max_int_value=maximum_entries[G_N_ELEMENTS(maximum_entries)-1].value;
	
	for(gint i=0; i<G_N_ELEMENTS(maximum_entries) && maximum_entries[i].value <= max_int_value; i++){
		GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
		gtk_list_store_append(list_store, iter);
		gchar *combo_box_rows_label=g_strdup_printf("%d %s", maximum_entries[i].value, int_label);
		gtk_list_store_set(
					list_store, iter,
						COLUMN_COMBO_NAME, combo_box_rows_label,
						COLUMN_COMBO_VALUE, maximum_entries[i].value,
					-1
		);
		uber_free(combo_box_rows_label);
		uber_free(iter);
	}
	
	uber_object_unref(list_store);
}/*preferences_max_int_combo_box_setup(prefs, GtkComboBox *int_combo_box, const gchar *int_label);*/

static void preferences_widget_sync_bool(const gchar *key, GtkCheckButton *check_button){
	gchar *pref_bool_default=(gchar *)g_object_get_data(G_OBJECT(check_button), "bool_default");
	gboolean bool_default=g_str_equal(pref_bool_default, "TRUE");
	debug("Binding CheckButton: %s to preference: %s; default value: %s", gtk_button_get_label(GTK_BUTTON(check_button)), key, (bool_default?"TRUE":"FALSE"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (check_button), gconfig_if_bool(key, bool_default));
}

static void preferences_widget_sync_string_combo(const gchar *key, GtkComboBox *combo_box){
	debug("Binding ComboBox to preference: %s", key );
	gchar        *value;
	
	if (!gconfig_get_string(key, &value)) return;
	
	GtkTreeModel *model=gtk_combo_box_get_model(combo_box);
	
	gboolean found=FALSE;
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	if(value && gtk_tree_model_get_iter_first(model, iter)) {
		gchar *name=NULL;
		
		do{
			gtk_tree_model_get(
					model, iter,
						COLUMN_COMBO_VALUE, &name,
					-1
			);
			
			if(!strcmp(name, value)){
				found=TRUE;
				gtk_combo_box_set_active_iter(combo_box, iter);
			}
			
			uber_free(name);
		}while(!found && gtk_tree_model_iter_next(model, iter));
	}
	
	/* Fallback to the first one. */
	if(!found){
		uber_free(iter);
		iter=g_new0(GtkTreeIter, 1);
		if(gtk_tree_model_get_iter_first(model, iter))
			gtk_combo_box_set_active_iter(combo_box, iter);
	}
	
	uber_free(value);
	uber_free(iter);
}

static void preferences_widget_sync_int_combo(const gchar *key, GtkComboBox *combo_box){
	debug("Binding ComboBox to preference: %s", key );
	gint          value=0, default_int=GPOINTER_TO_INT( g_object_get_data(G_OBJECT(combo_box), "default_int") );
	if(!gconfig_get_int_or_default(key, &value, default_int)) return;
	GtkTreeModel *model=gtk_combo_box_get_model(combo_box);;
	GtkTreeIter   *iter=g_new0(GtkTreeIter, 1);
	gboolean      found=FALSE;
	
	if(!( value && gtk_tree_model_get_iter_first(model, iter) )){
		uber_free(iter);
		return;
	}
	
	gint option;
	do{
		gtk_tree_model_get(model, iter,
					COLUMN_COMBO_VALUE, &option,
				-1
		);
		
		if(option==value) {
			found=TRUE;
			gtk_combo_box_set_active_iter(combo_box, iter);
			break;
		}
	}while( gtk_tree_model_iter_next(model, iter));
	/* Fallback to the first one. */
	uber_free(iter);
	iter=g_new0(GtkTreeIter, 1);
	if( !found && gtk_tree_model_get_iter_first(model, iter) )
		gtk_combo_box_set_active_iter(combo_box, iter);
	
	uber_free(iter);
}

static void preferences_add_id (PreferencesDialog *prefs, guint id){
	prefs->notify_ids = g_list_prepend (prefs->notify_ids, GUINT_TO_POINTER (id));
}

static void preferences_hookup_toggle_button(PreferencesDialog *prefs, const gchar *key, gboolean bool_default, GtkCheckButton *check_button){
	guint id;
	
	g_object_set_data_full(G_OBJECT(check_button), "key", g_strdup(key), g_free);
	g_object_set_data_full(G_OBJECT(check_button), "bool_default", g_strdup( (bool_default?"TRUE" :"FALSE") ), g_free);
	
	g_signal_connect(check_button, "toggled", (GCallback)preferences_toggle_button_toggled_cb, prefs);
	preferences_widget_sync_bool(key, check_button);
	
	if((id=gconfig_notify_add(g_strdup(key), preferences_notify_bool_cb, check_button)) )
		preferences_add_id(prefs, id);
}

static void preferences_hookup_string_combo(PreferencesDialog *prefs, const gchar *key, GtkComboBox *combo_box){
	guint id;

	preferences_widget_sync_string_combo(key, combo_box);

	g_object_set_data_full(G_OBJECT(combo_box), "key", g_strdup(key), g_free);

	g_signal_connect(combo_box, "changed", (GCallback)preferences_string_combo_changed_cb, prefs);

	if((id=gconfig_notify_add(g_strdup(key), preferences_notify_string_combo_cb, combo_box)) )
		preferences_add_id(prefs, id);
}

static void preferences_hookup_int_combo(PreferencesDialog *prefs, const gchar *key,const gint default_int,  GtkComboBox *combo_box){
	guint id;
	
	g_object_set_data_full(G_OBJECT(combo_box), "key", g_strdup(key), g_free);
	g_object_set_data(G_OBJECT(combo_box), "default_int", GINT_TO_POINTER(default_int) );
	
	preferences_widget_sync_int_combo(key, combo_box);
	
	g_signal_connect(combo_box, "changed", (GCallback)preferences_int_combo_changed_cb, prefs);
	
	if( (id=gconfig_notify_add(key, preferences_notify_int_combo_cb, combo_box)) )
		preferences_add_id (prefs, id);
}

static void preferences_toggle_button_toggled_cb(GtkCheckButton *check_button, PreferencesDialog *prefs){
	const gchar *key;
	key=g_object_get_data(G_OBJECT(check_button), "key");
	gconfig_set_bool(key, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (check_button)));
}

static void preferences_string_combo_changed_cb (GtkComboBox *combo_box, PreferencesDialog *prefs){
	GtkTreeModel	*model;
	GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
	gchar		*name;

	const gchar *key=g_object_get_data(G_OBJECT(combo_box), "key");

	if(gtk_combo_box_get_active_iter(combo_box, iter)) {
		model=gtk_combo_box_get_model(combo_box);

		gtk_tree_model_get(model, iter, COLUMN_COMBO_VALUE, &name, -1);
		gconfig_set_string(key, name);
		g_free(name);
		g_free(iter);
	}
}

static void preferences_int_combo_changed_cb(GtkComboBox *combo_box, PreferencesDialog *prefs){
	GtkTreeModel *model;
	GtkTreeIter   *iter=g_new0(GtkTreeIter, 1);
	gint          value=0;
	
	const gchar *key=g_object_get_data(G_OBJECT(combo_box), "key");
	if(combo_box==prefs->replace_me_with_combo_box)
		online_services_reset_length_of_longest_replacement();
	
	if(gtk_combo_box_get_active_iter(combo_box, iter)) {
		model=gtk_combo_box_get_model(combo_box);
		gtk_tree_model_get(model, iter, COLUMN_COMBO_VALUE, &value, -1);
		
		gconfig_set_int(key, value);
	}
	uber_free(iter);
}

static void preferences_response_cb(GtkDialog *dialog, gint response, PreferencesDialog *prefs){
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

static void preferences_destroy_cb(GtkDialog *dialog, PreferencesDialog *prefs){
	GList *l;

	for(l=prefs->notify_ids; l; l = l->next) {
		guint id;

		id=GPOINTER_TO_UINT(l->data);
		gconfig_notify_remove(id);
	}
	
	g_list_free(prefs->notify_ids);
	uber_free(prefs);
}

void preferences_dialog_show(GtkWindow *parent){
	if(prefs){
		window_present(GTK_WINDOW (prefs->dialog), TRUE);
		return;
	}

	prefs=g_new0(PreferencesDialog, 1);

	/* Get widgets */
	GtkBuilder *ui=gtkbuilder_get_file(
				GTK_BUILDER_UI_FILENAME,
					"preferences_dialog", &prefs->dialog,
					"preferences_notebook", &prefs->notebook,
					
					"compact_entire_view_check_button", &prefs->compact_entire_view_check_button,
					"uberchick_tree_view_concatenated_update_check_button", &prefs->concatenated_updates_check_button,
					"update_viewer_use_dialog_toggle_button", &prefs->use_dialog_toggle_button,
					"update_viewer_compact_view_toggle_button", &prefs->compact_view_toggle_button,
					
					"general_look_and_feel_disable_system_bell_check_button", &prefs->disable_system_bell,
					
					"titles_only_checkbutton", &prefs->titles_only_checkbutton,
					"expand_urls_selected_only_checkbutton", &prefs->expand_urls_selected_only_checkbutton,
					"expand_users_checkbutton", &prefs->expand_users_checkbutton,
					"expand_urls_disabled_checkbutton", &prefs->expand_urls_disabled_checkbutton,
					
					"autoload_addition_timeline_combo_box", &prefs->combo_default_timeline,
					"refresh_timeline_initial_interval_combo_box", &prefs->combo_reload,
					
					"autoload_best_friends_updates_check_button", &prefs->autoload_best_friends_updates_check_button,
					"autoload_dms_check_button", &prefs->autoload_dms_check_button,
					"autoload_replies_check_button", &prefs->autoload_replies_check_button,
					"autoload_homepage_check_button", &prefs->autoload_homepage_check_button,
					
					"notify_dms_check_button", &prefs->notify_dms_check_button,
					"notify_at_mentions_check_button", &prefs->notify_at_mentions_check_button,
					"notify_following_updates_check_button", &prefs->notify_following_updates_check_button,
					"notify_best_friends_updates_check_button", &prefs->notify_best_friends_updates_check_button,
					"notify_all_new_updates_check_button", &prefs->notify_all_new_updates,
					"notify_beep_updates_check_button", &prefs->notify_beep_updates_check_button,
					
					"disable_update_length_alert_check_button", &prefs->disable_update_length_alert_check_button,
					"replace_me_with_combo_box", &prefs->replace_me_with_combo_box,
					
					"updates_add_profile_link_checkbutton", &prefs->updates_add_profile_link_checkbutton,
					"post_reply_to_service_only_checkbutton", &prefs->post_reply_to_service_only_checkbutton,
					
					"previous_updates_uniq_check_button", &prefs->previous_updates_uniq_check_button,
					"previous_updates_maximum_combo_box", &prefs->previous_updates_maximum_combo_box,
					
					"search_history_uniq_check_button", &prefs->search_history_uniq_check_button,
					"search_history_maximum_combo_box", &prefs->search_history_maximum_combo_box,
				NULL
	);

	gchar *window_title=g_strdup_printf("%s - %s", _(GETTEXT_PACKAGE), _("Preferences"));
	gtk_window_set_title(GTK_WINDOW(prefs->dialog), window_title);
	uber_free(window_title);
	
	/* Connect the signals */
	gtkbuilder_connect(
				ui, prefs,
					"preferences_dialog", "destroy", preferences_destroy_cb,
					"preferences_dialog", "response", preferences_response_cb,
				NULL
	);
	
	g_signal_connect( (GtkToggleButton *)prefs->use_dialog_toggle_button, "toggled", (GCallback)main_window_update_viewer_set_embed, NULL);
	g_signal_connect_after( (GtkToggleButton *)prefs->compact_view_toggle_button, "toggled", (GCallback)update_viewer_compact_view_toggled, NULL);
	g_signal_connect_after( (GtkToggleButton *)prefs->concatenated_updates_check_button, "toggled", (GCallback)main_window_concatenate_timeline_columns, NULL);
	g_signal_connect_after( (GtkToggleButton *)prefs->compact_entire_view_check_button, "toggled", (GCallback)main_window_compact_ui, NULL);
	
	uber_object_unref(ui);
	
	g_object_add_weak_pointer(G_OBJECT (prefs->dialog), (gpointer) &prefs);
	gtk_window_set_transient_for(GTK_WINDOW (prefs->dialog), parent);
	
	preferences_timeline_setup(prefs);
	preferences_replace_with_setup(prefs);
	
	preferences_max_int_combo_box_setup(prefs, prefs->combo_reload, 60, "Minutes");
	preferences_max_int_combo_box_setup(prefs, prefs->previous_updates_maximum_combo_box, 0, "Updates");
	preferences_max_int_combo_box_setup(prefs, prefs->search_history_maximum_combo_box, 0, "Searches");
	
	preferences_setup_widgets(prefs);
	
	gtk_widget_show(GTK_WIDGET(prefs->dialog));
}/*preferences_dialog_show(parent);*/


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

