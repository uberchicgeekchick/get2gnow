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

#include "preferences.defines.h"

#include "hotkeys.h"

#include "gconfig.h"
#include "gtkbuilder.h"

#include "uberchick-sexy-entry-completion.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
#define	GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_UBERCHICK_SEXY_ENTRY_COMPLETION, UberChickSexyEntryCompletionPrivate))

struct _UberChickSexyEntryCompletionPrivate {
	gint			total;
	gint			maximum;
	
	GtkHBox			*hbox;
	
	GtkFrame		*frame;
	GtkAlignment		*alignment;
	GtkLabel		*label;
	GtkComboBoxEntry	*combo_box_entry;
	
	SexySpellEntry		*sexy_entry;
	gint			position;
	
	GtkEntryCompletion	*entry_completion;
	GtkListStore		*list_store;
	GtkTreeModel		*tree_model;
	GtkButton		*submit_button;
};

typedef enum{
	UBER_CHICK_SEXY_ENTRY_COMPLETION_PHRASE = 0,
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

static void uberchick_sexy_entry_completion_submitted(GtkWidget *sexy_entry, UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);

/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
/* BEGIN: GObject core methods. */
static void uberchick_sexy_entry_completion_class_init(UberChickSexyEntryCompletionClass *klass){
	GObjectClass	*object_class=G_OBJECT_CLASS(klass);
	
	object_class->finalize=(GObjectFinalizeFunc)uberchick_sexy_entry_completion_finalize;
	
	g_type_class_add_private(object_class, sizeof(UberChickSexyEntryCompletionPrivate));
}/* uberchick_sexy_entry_completion_class_init */

static void uberchick_sexy_entry_completion_init(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	uberchick_sexy_entry_completion_setup(uberchick_sexy_entry_completion);
	this->maximum=this->total=0;
	g_object_set(uberchick_sexy_entry_completion, "expand", TRUE, "fill", TRUE, NULL);
}/* uberchick_sexy_entry_completion_init */

UberChickSexyEntryCompletion *uberchick_sexy_entry_completion_new(void){
	return g_object_new(TYPE_UBERCHICK_SEXY_ENTRY_COMPLETION, NULL);
}/*uberchick_sexy_entry_completion_new(timeline);*/

static void uberchick_sexy_entry_completion_finalize(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	gtk_list_store_clear(this->list_store);
	
	gtk_widget_destroy(GTK_WIDGET(this->sexy_entry));
	gtk_widget_destroy(GTK_WIDGET(this->entry_completion));
	
	G_OBJECT_CLASS(uberchick_sexy_entry_completion_parent_class)->finalize(G_OBJECT(uberchick_sexy_entry_completion));
}/* uberchick_sexy_entry_completion_finalized */


/*BEGIN: Custom UberChickSexyEntryCompletion methods.*/
static void uberchick_sexy_entry_completion_setup(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!( uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion) )) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	GtkBuilder *ui=gtkbuilder_get_file(
				GTK_BUILDER_UI_FILENAME,
					"uberchick_sexy_entry_completion_hbox", &this->hbox,
					"uberchick_sexy_entry_completion_frame", &this->frame,
					"uberchick_sexy_entry_completion_entry_alignment", &this->alignment,
                              		"uberchick_sexy_entry_completion_label", &this->label,
					"uberchick_sexy_entry_completion_history_combo_box_entry", &this->combo_box_entry,
					"uberchick_sexy_entry_completion_history_list_store", &this->list_store,
					"uberchick_sexy_entry_completion_submit_button", &this->submit_button,
				NULL
			);
	
	uber_object_unref(ui);
	uberchick_sexy_entry_completion_sexy_setup(uberchick_sexy_entry_completion);
}/*uberchick_sexy_entry_completion_setup(uberchick_sexy_entry_completion);*/

static void uberchick_sexy_entry_completion_sexy_setup(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!( uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion) )) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	this->tree_model=gtk_combo_box_get_model( GTK_COMBO_BOX(this->combo_box_entry) );
	
	this->sexy_entry=SEXY_SPELL_ENTRY(sexy_spell_entry_new());
	this->sexy_entry=g_object_ref_sink(this->sexy_entry);
	
	gtk_container_remove(GTK_CONTAINER(this->combo_box_entry), gtk_bin_get_child(GTK_BIN(this->combo_box_entry)));
	gtk_container_add(GTK_CONTAINER(this->combo_box_entry), GTK_WIDGET(this->sexy_entry));
	
	this->entry_completion=gtk_entry_completion_new();
	gtk_entry_completion_set_model(this->entry_completion, this->tree_model);
	gtk_entry_set_completion(GTK_ENTRY(this->sexy_entry), this->entry_completion);
	
	gtk_combo_box_entry_set_text_column(this->combo_box_entry, UBER_CHICK_SEXY_ENTRY_COMPLETION_PHRASE);
	gtk_entry_completion_set_text_column(this->entry_completion, UBER_CHICK_SEXY_ENTRY_COMPLETION_PHRASE);
	uberchick_sexy_entry_completion_load(uberchick_sexy_entry_completion);
	gtk_widget_show(GTK_WIDGET(this->sexy_entry));
	
	g_signal_connect_after(this->sexy_entry, "key-press-event", G_CALLBACK(hotkey_pressed), NULL);
	g_signal_connect(this->sexy_entry, "activate", G_CALLBACK(uberchick_sexy_entry_completion_submitted), uberchick_sexy_entry_completion);
}/*uberchick_sexy_entry_completion_sexy_setup(uberchick_sexy_entry_completion);*/

static void uberchick_sexy_entry_completion_load(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!( uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion) )) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);

	gchar *previous_search=NULL, *search_history_gconf_path=NULL;
	uberchick_sexy_entry_completion_validate_maxium(uberchick_sexy_entry_completion);
	for(this->total=0; this->total<=this->maximum; this->total++){
		if(!( (gconfig_get_string( (search_history_gconf_path=g_strdup_printf(PREFS_SAVED_HISTORY_STRING, "search", this->total)), &previous_search )) && G_STR_N_EMPTY(previous_search) )) break;
		
		uberchick_sexy_entry_completion_restore(uberchick_sexy_entry_completion, previous_search);
		uber_free(previous_search);
		uber_free(search_history_gconf_path);
	}

	uberchick_sexy_entry_completion_prepend(uberchick_sexy_entry_completion, "[new search]");
	if(previous_search) uber_free(previous_search);
	if(search_history_gconf_path) uber_free(search_history_gconf_path);
}/*uberchick_sexy_entry_completion_load(uberchick_sexy_entry_completion)*/

static void uberchick_sexy_entry_completion_add(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion, const gchar *update, gint list_store_index){
	if(!( uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion) )) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	this->total++;
}/*uberchick_sexy_entry_completion_add(uberchick_sexy_entry_completion, GTK_ENTRY(this->sexy_entry)->text, [-3 to prepend w/o saving update into gconfig|-2 to prepend|-1 to append|>0 to instert at this index]);*/

static void uberchick_sexy_entry_completion_submitted(GtkWidget *sexy_entry, UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!( uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion) )) return;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	this->total++;
}/*uberchick_sexy_entry_completion_submitted(GtkWidget *sexy_entry, UberChickSexyEntryCompletion *uberchick_sexy_entry_completion);*/

static gint uberchick_sexy_entry_completion_validate_maxium(UberChickSexyEntryCompletion *uberchick_sexy_entry_completion){
	if(!( uberchick_sexy_entry_completion && IS_UBERCHICK_SEXY_ENTRY_COMPLETION(uberchick_sexy_entry_completion) )) return 0;
	UberChickSexyEntryCompletionPrivate *this=GET_PRIVATE(uberchick_sexy_entry_completion);
	
	gconfig_get_int_or_default(PREFS_SEARCH_HISTORY_MAXIMUM, &this->maximum, 50);
	if(this->maximum < 5)
		gconfig_set_int(PREFS_SEARCH_HISTORY_MAXIMUM, (this->maximum=5));
	else if(this->maximum > 100)
		gconfig_set_int(PREFS_SEARCH_HISTORY_MAXIMUM, (this->maximum=100));
	return this->maximum;
}/*main_window_search_history_validate_maxium(main_window->private);*/
/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/


