/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberchick@uberChicGeekChick.Com>
 * 	Released under the terms of the Reciprocal Public License (RPL).
 *
 * For more information or to find the latest release, visit our
 * website at: http://uberChicGeekChick.Com/?projects=get2gnow
 *
 * Writen by an uberchick, other uberchicks please meet me & others @:
 * 	http://uberchicks.Net/
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
 *      Project, system, & library headers.  eg #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/
#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <libsexy/sexy.h>

#include "config.h"
#include "program.h"

#include "online-services.h"
#include "online-service-request.h"

#include "preferences.defines.h"

#include "hotkeys.h"

#include "gconfig.h"
#include "gtkbuilder.h"

#include "uberchick-sexy-entry-completion.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
#define	GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), UBERCHICK_TYPE_SEXY_ENTRY_COMPLETION, UberChickSexyEntryCompletionPrivate))

struct _UberChickSexyEntryCompletionPrivate {
	gint			total;
	gint			maximum;
	
	GtkHBox			*hbox;
	
	GtkFrame		*frame;
	GtkAlignment		*alignment;
	GtkLabel		*label;
	GtkLabel		*remaining_characters_count_label;
	GtkComboBoxEntry	*sexy_entry_combo_box_entry;
	
	GRegex			*upper_case_regex;
	GRegex			*username_regex;
	SexySpellEntry		*sexy_entry;
	gint			sexy_entry_text_position;
	guint			max_length;
	
	GtkEntryCompletion	*entry_completion;
	GtkListStore		*list_store;
	GtkTreeModel		*tree_model;
	GtkButton		*submit_button;
};

typedef enum{
	GSTRING_UBER_CHICK_SEXY_ENTRY_COMPLETION_PHRASE = 0,
} UberChickSexyEntryCompletionListStoreColumns;


/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define DEBUG_DOMAINS "{Stuff}:GtkBuilder:GtkBuildable:Settings:Setup:Start-Up:UberChickSexyEntryCompletion.c"
#include "debug.h"

#define GTK_BUILDER_UI_FILENAME "uberchick-sexy-entry-completion"


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static void uberchick_sexy_entry_completion_class_init(UberChickSexyEntryCompletionClass *klass);
static void uberchick_sexy_entry_completion_init(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);
static void uberchick_sexy_entry_completion_finalize(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);

G_DEFINE_TYPE(UberChickSexyEntryCompletion, uberchick_sexy_entry_completion, G_TYPE_OBJECT);

static void uberchick_sexy_entry_completion_setup(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);
static void uberchick_sexy_entry_completion_sexy_setup(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);
static void uberchick_sexy_entry_completion_load(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);

static void uberchick_sexy_entry_completion_add(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const gchar *update, gint list_store_index);
#define uberchick_sexy_entry_completion_restore(uberchick_sexy_entry_completion, update)						\
					uberchick_sexy_entry_completion_add(uberchick_sexy_entry_completion, update, -3)
#define uberchick_sexy_entry_completion_apppend(uberchick_sexy_entry_completion, update)						\
					uberchick_sexy_entry_completion_add(uberchick_sexy_entry_completion, update, -2)
#define uberchick_sexy_entry_completion_prepend(uberchick_sexy_entry_completion, update)						\
					uberchick_sexy_entry_completion_add(uberchick_sexy_entry_completion, update, -1)
#define uberchick_sexy_entry_completion_insert(uberchick_sexy_entry_completion, update, list_store_index)				\
					uberchick_sexy_entry_completion_add(uberchick_sexy_entry_completion, update, list_store_index)

static gint uberchick_sexy_entry_completion_validate_maxium(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);
static gboolean uberchick_sexy_entry_completion_is_unique(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const gchar *update);
static void uberchick_sexy_entry_completion_remove(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, gint list_store_index);
static void uberchick_sexy_entry_completion_rotate(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);

static void uberchick_sexy_entry_completion_submitted(GtkWidget *sexy_entry, UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);

static gint uberchick_sexy_entry_completion_update_length(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, gchar *update);
static void uberchick_sexy_entry_completion_update_remaining_character_count(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);
static gboolean uberchick_sexy_entry_completion_puts(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const gchar *str, gint position, gboolean to_lower, gboolean uniq);
static gboolean uberchick_sexy_entry_completion_confirm_clear(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);
static gboolean uberchick_sexy_entry_completion_clear(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);
static void uberchick_sexy_entry_completion_set(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, gchar *text);
void uberchick_sexy_entry_completion_beep(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);



/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
/* BEGIN: GObject core methods. */
static void uberchick_sexy_entry_completion_class_init(UberChickSexyEntryCompletionClass *klass){
	GObjectClass	*object_class=G_OBJECT_CLASS(klass);
	
	object_class->finalize=(GObjectFinalizeFunc)uberchick_sexy_entry_completion_finalize;
	
	g_type_class_add_private(object_class, sizeof(UberChickSexyEntryCompletionPrivate));
}/* uberchick_sexy_entry_completion_class_init(uberchick_sexy_entry_completion_class);*/

static void uberchick_sexy_entry_completion_init(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	uberchick_sexy_entry_completion_setup(uberchick_sexy_entry_completion);
	this->maximum=this->total=0;
	
	GError *error=NULL;
	const gchar *g_regex_pattern="[A-Z]+";
	this->upper_case_regex=g_regex_new(g_regex_pattern, G_REGEX_DOLLAR_ENDONLY|G_REGEX_OPTIMIZE, G_REGEX_MATCH_NEWLINE_ANY, &error);
	if(error){
		debug("**ERROR:** creating case-matching GRegex using the pattern %s.  GError message: %s", g_regex_pattern, error->message);
		g_error_free(error);
		uber_regex_unref(this->upper_case_regex);
	}
	
	error=NULL;
	g_regex_pattern="^(@[A-Za-z0-9_]+) .+$";
	this->username_regex=g_regex_new(g_regex_pattern, G_REGEX_DOLLAR_ENDONLY|G_REGEX_OPTIMIZE, G_REGEX_MATCH_NEWLINE_ANY, &error);
	if(error){
		this->username_regex=NULL;
		debug("**ERROR:** creating username GRegex using the pattern %s.  GError message: %s", g_regex_pattern, error->message);
		g_error_free(error);
		uber_regex_unref(this->username_regex);
	}
	
	g_object_set(uberchick_sexy_entry_completion, "expand", TRUE, "fill", TRUE, NULL);
}/* uberchick_sexy_entry_completion_init(uberchick_sexy_entry_completion);*/

UberChickSexyEntryCompletion *uberchick_sexy_entry_completion_new(void){
	return g_object_new(UBERCHICK_TYPE_SEXY_ENTRY_COMPLETION, NULL);
}/*uberchick_sexy_entry_completion_new(timeline);*/

static void uberchick_sexy_entry_completion_finalize(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	if(this->list_store)
		gtk_list_store_clear(this->list_store);
	
	uber_regex_unref(this->upper_case_regex);
	uber_regex_unref(this->username_regex);
	
	if(this->sexy_entry)
		gtk_widget_destroy(GTK_WIDGET(this->sexy_entry));
	if(this->entry_completion)
		gtk_widget_destroy(GTK_WIDGET(this->entry_completion));
	
	G_OBJECT_CLASS(uberchick_sexy_entry_completion_parent_class)->finalize(G_OBJECT(uberchick_sexy_entry_completion));
}/* uberchick_sexy_entry_completion_finalized(uberchick_sexy_entry_completion);*/


/*BEGIN: Custom UberChickSexyEntryCompletion methods.*/
static void uberchick_sexy_entry_completion_setup(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	GtkBuilder *ui=gtkbuilder_get_file(
				GTK_BUILDER_UI_FILENAME,
					"uberchick_sexy_entry_completion_hbox", &this->hbox,
					"uberchick_sexy_entry_completion_frame", &this->frame,
					"uberchick_sexy_entry_completion_alignment", &this->alignment,
                              		"uberchick_sexy_entry_completion_label", &this->label,
					"uberchick_sexy_entry_completion_remaining_characters_count_label", &this->remaining_characters_count_label,
					"uberchick_sexy_entry_completion_combo_box_entry", &this->sexy_entry_combo_box_entry,
					"uberchick_sexy_entry_completion_list_store", &this->list_store,
					"uberchick_sexy_entry_completion_submit_button", &this->submit_button,
				NULL
			);
	
	uber_object_unref(ui);
	uberchick_sexy_entry_completion_sexy_setup(uberchick_sexy_entry_completion);
}/*uberchick_sexy_entry_completion_setup(uberchick_sexy_entry_completion);*/

static void uberchick_sexy_entry_completion_sexy_setup(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	this->tree_model=gtk_combo_box_get_model(GTK_COMBO_BOX(this->sexy_entry_combo_box_entry));
	
	this->sexy_entry=SEXY_SPELL_ENTRY(sexy_spell_entry_new());
	this->sexy_entry=g_object_ref_sink(this->sexy_entry);
	
	gtk_container_remove(GTK_CONTAINER(this->sexy_entry_combo_box_entry), gtk_bin_get_child(GTK_BIN(this->sexy_entry_combo_box_entry)));
	gtk_container_add(GTK_CONTAINER(this->sexy_entry_combo_box_entry), GTK_WIDGET(this->sexy_entry));
	
	this->entry_completion=gtk_entry_completion_new();
	gtk_entry_completion_set_model(this->entry_completion, this->tree_model);
	gtk_entry_set_completion(GTK_ENTRY(this->sexy_entry), this->entry_completion);
	
	gtk_combo_box_entry_set_text_column(this->sexy_entry_combo_box_entry, GSTRING_UBER_CHICK_SEXY_ENTRY_COMPLETION_PHRASE);
	gtk_entry_completion_set_text_column(this->entry_completion, GSTRING_UBER_CHICK_SEXY_ENTRY_COMPLETION_PHRASE);
	uberchick_sexy_entry_completion_load(uberchick_sexy_entry_completion);
	gtk_widget_show(GTK_WIDGET(this->sexy_entry));
	
	g_signal_connect_after(this->sexy_entry, "key-press-event", G_CALLBACK(hotkey_pressed), NULL);
	g_signal_connect(this->sexy_entry, "activate", G_CALLBACK(uberchick_sexy_entry_completion_submitted), uberchick_sexy_entry_completion);
}/*uberchick_sexy_entry_completion_sexy_setup(uberchick_sexy_entry_completion);*/

static void uberchick_sexy_entry_completion_load(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);

	gchar *previous_search=NULL, *search_history_gconf_path=NULL;
	uberchick_sexy_entry_completion_validate_maxium(uberchick_sexy_entry_completion);
	for(this->total=0; this->total<=this->maximum; this->total++){
		if(!((gconfig_get_string((search_history_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "search", this->total)), &previous_search)) && G_STR_N_EMPTY(previous_search))) break;
		
		uberchick_sexy_entry_completion_restore(uberchick_sexy_entry_completion, previous_search);
		uber_free(previous_search);
		uber_free(search_history_gconf_path);
	}

	uberchick_sexy_entry_completion_prepend(uberchick_sexy_entry_completion, "[new search]");
	if(previous_search) uber_free(previous_search);
	if(search_history_gconf_path) uber_free(search_history_gconf_path);
}/*uberchick_sexy_entry_completion_load(uberchick_sexy_entry_completion)*/

static void uberchick_sexy_entry_completion_add(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const gchar *update, gint list_store_index){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	if(G_STR_EMPTY(update)) return;
	
	if(
		G_STR_N_EMPTY(update)
		&&
		g_str_n_equal(update, "[new update]")
		&&
		gconfig_if_bool(PREFS_UPDATES_HISTORY_UNIQUE_ONLY, TRUE)
		&&
		!uberchick_sexy_entry_completion_is_unique(uberchick_sexy_entry_completion, update)
	){
		debug("Update being sent: %s is already in the previous update's list and will not be stored again", update);
		return;
	}
	
	uberchick_sexy_entry_completion_validate_maxium(uberchick_sexy_entry_completion);
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	if(list_store_index<-1){
		debug("Appending <%s> to UpdateViewer's previous_updates", update);
		gtk_list_store_append(this->list_store, iter);
	}else if(list_store_index==-1){
		debug("Prepending <%s> to UpdateViewer's previous_updates", update);
		gtk_list_store_prepend(this->list_store, iter);
	}else if(list_store_index>-1){
		if(list_store_index>this->maximum)
			list_store_index=this->maximum;
		debug("Inserting <%s> to UpdateViewer's previous_updates; index: %i", update, list_store_index);
		gtk_list_store_insert(this->list_store, iter, list_store_index);
	}
	gtk_list_store_set(
			this->list_store, iter,
				GSTRING_UBER_CHICK_SEXY_ENTRY_COMPLETION_PHRASE, g_strdup(update),
			-1
	);
	uber_free(iter);
	
	gchar *user_name=NULL;
	GMatchInfo *match_info=NULL;
	if(((g_regex_match(this->username_regex, update, 0, &match_info))) && (user_name=g_regex_replace(this->username_regex, update, -1, 0, "\\1", 0, NULL)) && G_STR_N_EMPTY(user_name)){
		debug("Appending user name <%s> to UpdateViewer's previous_updates", user_name);
		/*uberchick_sexy_entry_completion_add(uberchick_sexy_entry_completion, user_name, this->maximum/2);*/
		uberchick_sexy_entry_completion_add(uberchick_sexy_entry_completion, user_name, list_store_index);
	}
	if(user_name)
		uber_free(user_name);
	if(match_info)
		g_match_info_free(match_info);
	
	if(list_store_index==-3 || g_str_equal(update, "[new update]")) return;
	
	uberchick_sexy_entry_completion_remove(uberchick_sexy_entry_completion, 1);
	uberchick_sexy_entry_completion_add(uberchick_sexy_entry_completion, "[new update]", list_store_index);
	
	if(this->total<this->maximum)
		this->total++;
	else
		for(; this->total>=this->maximum; this->total--)
			uberchick_sexy_entry_completion_remove(uberchick_sexy_entry_completion, this->total);
	
	uberchick_sexy_entry_completion_rotate(uberchick_sexy_entry_completion);
	gchar *previous_update_gconf_path=NULL;
	gconfig_set_string((previous_update_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "updates", 0)), update);
	uber_free(previous_update_gconf_path);
}/*uberchick_sexy_entry_completion_add(uberchick_sexy_entry_completion, GTK_ENTRY(this->sexy_entry)->text, [-3 to prepend w/o saving update into gconfig|-2 to prepend|-1 to append|>0 to instert at this index]);*/

static gboolean uberchick_sexy_entry_completion_is_unique(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const gchar *update){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return FALSE;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	if(G_STR_EMPTY(update)) return FALSE;
	
	if(g_str_equal(update, "[new update]"))
		return TRUE;
	
	debug("Checking uniqueness of update.  New update:\t[%s]", update);
	gboolean uniq=TRUE;
	gchar *update_at_index=NULL;
	for(gint i=1; i<=this->total && uniq; i++) {
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(this->list_store), iter, path)){
			debug("Checking update, at index: %d, failed to get valid iter for the list store", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gtk_tree_model_get(
				GTK_TREE_MODEL(this->list_store), iter,
					GSTRING_UBER_CHICK_SEXY_ENTRY_COMPLETION_PHRASE, &update_at_index,
				-1
		);
		
		if(!strcmp(update, update_at_index)){
			debug("Update is not unique.  Duplicate update found at index: %d", i);
			debug("\tComparing new update: [%s]", update);
			debug("\tAgainst old update: [%s]", update_at_index);
			uniq=FALSE;
		}
		
		gtk_tree_path_free(path);
		uber_free(update_at_index);
		uber_free(iter);
	}
	return uniq;
}/*uberchick_sexy_entry_completion_is_unique(uberchick_sexy_entry_completion, update);*/

static void uberchick_sexy_entry_completion_remove(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, gint list_store_index){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	if(!(list_store_index > 0 && list_store_index <= this->total)) return;
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	GtkTreePath *path=gtk_tree_path_new_from_indices(list_store_index, -1);
	debug("Removing saved update %d.  Total update saved: %d", list_store_index, this->total);
	if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(this->list_store), iter, path))
		debug("Removing saved update, at index: %d, failed to get valid iter for the list store", list_store_index);
	else{
		debug("Removing iter at index: %d", list_store_index);
		gtk_list_store_remove(this->list_store, iter);
	}
	gtk_tree_path_free(path);
	uber_free(iter);
}/*uberchick_sexy_entry_completion_remove(uberchick_sexy_entry_completion, GET_PRIVATE(uberchick_sexy_entry_completion)->total);*/

static void uberchick_sexy_entry_completion_rotate(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	gchar *previous_update=NULL, *previous_update_gconf_path=NULL;
	for(gint i=this->maximum; i>=0; i--){
		if((gconfig_get_string((previous_update_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "updates", i-1)), &previous_update)) && G_STR_N_EMPTY(previous_update)){
			uber_free(previous_update_gconf_path);
			previous_update_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "updates", i);
			gconfig_set_string(previous_update_gconf_path, previous_update);
		}
		if(previous_update) uber_free(previous_update);
		uber_free(previous_update_gconf_path);
	}
}/*uberchick_sexy_entry_completion_rotate(uberchick_sexy_entry_completion);*/

static void uberchick_sexy_entry_completion_submitted(GtkWidget *sexy_entry, UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	this->total++;
}/*uberchick_sexy_entry_completion_submitted(GtkWidget *sexy_entry, UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);*/

static gint uberchick_sexy_entry_completion_validate_maxium(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return 0;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	gconfig_get_int_or_default(PREFS_SEARCH_HISTORY_MAXIMUM, &this->maximum, 50);
	if(this->maximum < 5)
		gconfig_set_int(PREFS_SEARCH_HISTORY_MAXIMUM, (this->maximum=5));
	else if(this->maximum > 100)
		gconfig_set_int(PREFS_SEARCH_HISTORY_MAXIMUM, (this->maximum=100));
	return this->maximum;
}/*uberchick_sexy_entry_completion_validate_maxium(uberchick_sexy_entry_completion);*/

void uberchick_sexy_entry_completion_show_entries(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	g_signal_emit_by_name(GET_PRIVATE(uberchick_sexy_entry_completion)->sexy_entry_combo_box_entry, "popup");
}/*uberchick_sexy_entry_completion_show_previous_updates(uberchick_sexy_entry_completion);*/

void uberchick_sexy_entry_completion_hide_entries(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	g_signal_emit_by_name(GET_PRIVATE(uberchick_sexy_entry_completion)->sexy_entry_combo_box_entry, "popdown");
}/*uberchick_sexy_entry_completion_hide_previous_updates(uberchick_sexy_entry_completion);*/

static gint uberchick_sexy_entry_completion_update_length(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, gchar *update){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return 0;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	guint16 character_count=0;
	gint replace_me=0;
	gconfig_get_int_or_default(PREFS_UPDATES_REPLACE_ME_W_NICK, &replace_me, 2);
	for(guint16 i=0; update[i]; i++){
		if(update[i]=='<' || update[i]=='>')
			character_count+=3;
		if(replace_me!=-1 && update[i]=='/' && update[i+1] && update[i+1]=='m' && update[i+2] && update[i+2]=='e'){
			character_count+=online_services_get_length_of_longest_replacement();
			i+=2;
		}
		character_count++;
	}
	/*
	   if(!character_count)
		if(in_reply_to_service==this->viewing_service && in_reply_to_status_id==this->viewing_update_id){
			in_reply_to_service=NULL;
			in_reply_to_status_id=0.0;
		}
	*/
	
	return (this->max_length-character_count);
}/*uberchick_sexy_entry_completion_update_length(uberchick_sexy_entry_completion, update);*/

static gboolean uberchick_sexy_entry_completion_confirm_clear(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return FALSE;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	if(G_STR_EMPTY(GTK_ENTRY(this->sexy_entry)->text)) return TRUE;
	return online_service_request_popup_confirmation_dialog(
			PREFS_UPDATE_VIEWER_CLEAR,
			_("You are about to empty your current update:"),
			_("Are you sure you want to delete your current update?"),
			NULL, NULL
	);
	uberchick_sexy_entry_completion_clear(uberchick_sexy_entry_completion);
}/*uberchick_sexy_entry_completion_confirm_clear(uberchick_sexy_entry_completion);*/

static gboolean uberchick_sexy_entry_completion_clear(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return FALSE;
	
	if(!(uberchick_sexy_entry_completion_confirm_clear(uberchick_sexy_entry_completion)))
		return FALSE;
	
	uberchick_sexy_entry_completion_set(uberchick_sexy_entry_completion, NULL);
	return TRUE;
}/*uberchick_sexy_entry_completion_clear(uberchick_sexy_entry_completion);*/

static void uberchick_sexy_entry_completion_set(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, gchar *text){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	gtk_entry_set_text(GTK_ENTRY(this->sexy_entry), (text ?text :(gchar *)""));
	uberchick_sexy_entry_completion_update_remaining_character_count(uberchick_sexy_entry_completion);
	uberchick_sexy_entry_completion_select(uberchick_sexy_entry_completion);
}/*uberchick_sexy_entry_completion_set(NULL|"an phase or update");*/

void uberchick_sexy_entry_completion_prefix_char(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const char c){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	gchar *str=g_strdup_printf("%c", c);
	uberchick_sexy_entry_completion_prefix_string(uberchick_sexy_entry_completion, (const gchar *)str, FALSE, FALSE);
	uber_free(str);
}/*uberchick_sexy_entry_completion_prefix_char('\t', TRUE|FALSE, TRUE|FALSE);*/

gboolean uberchick_sexy_entry_completion_prefix_string(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const gchar *str, gboolean to_lower, gboolean uniq){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return FALSE;
	return uberchick_sexy_entry_completion_puts(uberchick_sexy_entry_completion, str, 0, to_lower, uniq);
}/*uberchick_sexy_entry_completion_prefix_string("string", TRUE|FALSE, TRUE|FALSE);*/

void uberchick_sexy_entry_completion_insert_char(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const char c){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	gchar *str=g_strdup_printf("%c", c);
	uberchick_sexy_entry_completion_insert_string(uberchick_sexy_entry_completion, (const gchar *)str, FALSE, FALSE);
	uber_free(str);
}/*uberchick_sexy_entry_completion_insert_char('\t', TRUE|FALSE, TRUE|FALSE);*/

gboolean uberchick_sexy_entry_completion_insert_string(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const gchar *str, gboolean to_lower, gboolean uniq){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return FALSE;
	return uberchick_sexy_entry_completion_puts(uberchick_sexy_entry_completion, str, gtk_editable_get_position(GTK_EDITABLE(GTK_ENTRY(GET_PRIVATE(uberchick_sexy_entry_completion)->sexy_entry))), to_lower, uniq);
}/*uberchick_sexy_entry_completion_insert_string("string", TRUE|FALSE, TRUE|FALSE);*/

void uberchick_sexy_entry_completion_append_char(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const char c){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	gchar *str=g_strdup_printf("%c", c);
	uberchick_sexy_entry_completion_append_string(uberchick_sexy_entry_completion, (const gchar *)str, FALSE, FALSE);
	uber_free(str);
}/*uberchick_sexy_entry_completion_append_char('\t', TRUE|FALSE, TRUE|FALSE);*/

gboolean uberchick_sexy_entry_completion_append_string(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const gchar *str, gboolean to_lower, gboolean uniq){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return FALSE;
	return uberchick_sexy_entry_completion_puts(uberchick_sexy_entry_completion, str, (gint)gtk_entry_get_text_length(GTK_ENTRY(GET_PRIVATE(uberchick_sexy_entry_completion)->sexy_entry)), to_lower, uniq);
}/*uberchick_sexy_entry_completion_append_string("string", TRUE|FALSE, TRUE|FALSE);*/

static gboolean uberchick_sexy_entry_completion_puts(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const gchar *str, gint position_after, gboolean to_lower, gboolean uniq){
	if(G_STR_EMPTY(str)) return FALSE;
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return FALSE;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	if(gtk_widget_is_sensitive(GTK_WIDGET(this->sexy_entry)))
		gtk_widget_set_sensitive(GTK_WIDGET(this->sexy_entry), TRUE);
	
	gchar *string=NULL;
	gboolean free_string=FALSE;
	GMatchInfo *match_info=NULL;
	if(to_lower && g_regex_match(this->upper_case_regex, str, 0, &match_info)){
		free_string=TRUE;
		string=g_utf8_strdown(str, -1);
	}else
		string=(gchar *)str;
	g_match_info_free(match_info);
	
	if(uniq && G_STR_N_EMPTY(GTK_ENTRY(this->sexy_entry)->text) && g_strrstr(GTK_ENTRY(this->sexy_entry)->text, string)){
		if(free_string) uber_free(string);
		return FALSE;
	}
	
	if(uniq && GTK_ENTRY(this->sexy_entry)->text[0]=='@' && string[0]=='@'){
		if(free_string) uber_free(string);
		return FALSE;
	}
	
	gint position_prior=gtk_editable_get_position(GTK_EDITABLE(this->sexy_entry))+strlen(string);
	gtk_editable_insert_text(GTK_EDITABLE(this->sexy_entry), string, -1, &position_after);
	if(position_after<position_prior)
		this->sexy_entry_text_position=position_prior;
	else
		this->sexy_entry_text_position=position_after;
	
	gtk_entry_set_position(GTK_ENTRY(this->sexy_entry), this->sexy_entry_text_position);
	
	if(free_string) uber_free(string);
	
	uberchick_sexy_entry_completion_update_remaining_character_count(uberchick_sexy_entry_completion);
	uberchick_sexy_entry_completion_select(uberchick_sexy_entry_completion);
	return TRUE;
}/*uberchick_sexy_entry_completion_puts("string", 0...139, TRUE|FALSE, TRUE|FALSE);*/

static void uberchick_sexy_entry_completion_update_remaining_character_count(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	this->sexy_entry_text_position=gtk_editable_get_position(GTK_EDITABLE(GTK_ENTRY(this->sexy_entry)));
	gshort remaining_character_count=uberchick_sexy_entry_completion_update_length(uberchick_sexy_entry_completion, GTK_ENTRY(this->sexy_entry)->text);
	gchar *remaining_characters_markup_label=NULL;
	if(remaining_character_count < 0){
		if(!gconfig_if_bool(PREFS_DISABLE_UPDATE_LENGTH_ALERT, FALSE))
			uberchick_sexy_entry_completion_beep(uberchick_sexy_entry_completion);
		remaining_characters_markup_label=g_strdup_printf("<span size=\"small\" foreground=\"red\">%d</span>", remaining_character_count);
	}else{
		remaining_characters_markup_label=g_strdup_printf("<span size=\"small\" foreground=\"green\">%d</span>", remaining_character_count);
	}
	
	gtk_label_set_markup(this->remaining_characters_count_label, remaining_characters_markup_label);
	uber_free(remaining_characters_markup_label);
}/*uberchick_sexy_entry_completion_update_remaining_character_count(uberchick_sexy_entry_completion);*/

void uberchick_sexy_entry_completion_select(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	if(gtk_widget_has_focus(GTK_WIDGET(this->sexy_entry))) return;
	/*if(gtk_widget_has_focus(GTK_WIDGET(main_window_sexy_search_entry_get_widget()))) return;*/
	gtk_widget_grab_focus(GTK_WIDGET(this->sexy_entry));
	gint position=-1;
	if(this->sexy_entry_text_position > 0 && this->sexy_entry_text_position <= gtk_entry_get_text_length((GtkEntry *)this->sexy_entry))
		position=this->sexy_entry_text_position;
	gtk_entry_set_position(GTK_ENTRY(this->sexy_entry), position);
}/*uberchick_sexy_entry_completion_select(uberchick_sexy_entry_completion);*/

void uberchick_sexy_entry_completion_beep(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(gconfig_if_bool(PREFS_DISABLE_SYSTEM_BELL, FALSE)) return;
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	gtk_widget_error_bell(GTK_WIDGET(GET_PRIVATE(uberchick_sexy_entry_completion)->sexy_entry));
}/*uberchick_sexy_entry_completion_beep(uberchick_sexy_entry_completion);*/

SexySpellEntry *uberchick_sexy_entry_get_widget(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return NULL;
	return GET_PRIVATE(uberchick_sexy_entry_completion)->sexy_entry;
}/*uberchick_sexy_entry_get_widget(uberchick_sexy_entry_completion);*/

void uberchick_sexy_entry_completion_select_all(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!(uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion))) return;
	/*if(gtk_widget_has_focus(GTK_WIDGET(main_window_sexy_search_entry_get_widget()))) return;*/
	gtk_editable_select_region(GTK_EDITABLE(GET_PRIVATE(uberchick_sexy_entry_completion)->sexy_entry), 0, -1);
}/*uberchick_sexy_entry_completion_select_all(uberchick_sexy_entry_completion);*/


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
