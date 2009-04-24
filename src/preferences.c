/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2003-2007 Imendio AB
 * Copyright (C) 2007-2008 Brian Pepple
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors: Mikael Hallendal <micke@imendio.com>
 *          Richard Hult <richard@imendio.com>
 *          Martyn Russell <martyn@imendio.com>
 *			Brian Pepple <bpepple@fedoraproject.org>
 *			Daniel Morales <daniminas@gmail.com>
 */

#include "config.h"

#include <string.h>

#include <glib/gi18n.h>

#include "network.h"
#include "gconf.h"
#include "gtkbuilder.h"

#include "main.h"
#include "preferences.h"

#define GtkBuilderUI "preferences.ui"

typedef struct {
	GtkWidget *dialog;
	GtkWidget *notebook;
	GtkWidget *combo_default_timeline;
	GtkWidget *combo_reload;

	/* Checkbuttons */
	GtkWidget *autoconnect;
	GtkWidget *notify;
	GtkWidget *sound;

	GList     *notify_ids;
} Prefs;

typedef struct _reload_time {
	gint   minutes;
	gchar *display_text;
} reload_time;

reload_time reload_list[] = {
	{3, N_("3 minutes")},
	{5, N_("5 minutes")},
	{15, N_("15 minutes")},
	{30, N_("30 minutes")},
	{60, N_("hour")},
	{0, NULL}
};

enum {
	COL_LANG_ENABLED,
	COL_LANG_CODE,
	COL_LANG_NAME,
	COL_LANG_COUNT
};

enum {
	COL_COMBO_VISIBLE_NAME,
	COL_COMBO_NAME,
	COL_COMBO_COUNT
};

static void     preferences_setup_widgets          (Prefs            *prefs);
static void     preferences_timeline_setup         (Prefs            *prefs);
static void     preferences_widget_sync_bool       (const gchar            *key,
													GtkWidget              *widget);
static void   preferences_widget_sync_string_combo (const gchar            *key,
													GtkWidget              *widget);
static void   preferences_widget_sync_int_combo    (const gchar            *key,
													GtkWidget              *widget);
static void     preferences_notify_bool_cb         (Conf             *conf,
													const gchar            *key,
													gpointer                user_data);
static void     preferences_notify_string_combo_cb (Conf             *conf,
													const gchar            *key,
													gpointer                user_data);
static void     preferences_notify_int_combo_cb    (Conf             *conf,
													const gchar            *key,
													gpointer                user_data);
static void     preferences_hookup_toggle_button   (Prefs            *prefs,
													const gchar            *key,
													GtkWidget              *widget);
static void     preferences_hookup_string_combo    (Prefs            *prefs,
													const gchar            *key,
													GtkWidget              *widget);
static void     preferences_hookup_int_combo       (Prefs            *prefs,
													const gchar            *key,
													GtkWidget              *widget);
static void preferences_response_cb                (GtkWidget              *widget,
													gint                    response,
													Prefs            *prefs);
static void preferences_toggle_button_toggled_cb   (GtkWidget              *button,
													gpointer                user_data);
static void preferences_string_combo_changed_cb    (GtkWidget              *button,
													gpointer                user_data);
static void preferences_int_combo_changed_cb       (GtkWidget              *combo,
													gpointer                user_data);
static void preferences_destroy_cb                 (GtkWidget              *widget,
													Prefs            *prefs);

static void preferences_setup_widgets (Prefs *prefs){
	preferences_hookup_toggle_button( prefs, PREFS_UI_NOTIFICATION, prefs->notify );

	preferences_hookup_toggle_button( prefs, PREFS_UI_SOUND, prefs->sound );

	preferences_hookup_string_combo( prefs, PREFS_TWEETS_HOME_TIMELINE, prefs->combo_default_timeline );

	preferences_hookup_int_combo( prefs, PREFS_TWEETS_RELOAD_TIMELINES, prefs->combo_reload );
}

static void preferences_notify_bool_cb (Conf *conf, const gchar *key, gpointer user_data){
	preferences_widget_sync_bool (key, user_data);
}

static void preferences_timeline_setup (Prefs *prefs){
	static const gchar *timelines[] = {
		API_TIMELINE_FRIENDS,	N_("My Friends' Tweets"),
		API_REPLIES,		N_("@ Replies"),
		API_DIRECT_MESSAGES,	N_("My DMs Inbox"),
		API_FAVORITES,		N_("My Favorites"),
		API_TIMELINE_MY,	N_("My Tweets"),
		API_TIMELINE_PUBLIC,	N_("All Public Tweets"),
		NULL
	};

	GtkListStore    *model;
	GtkTreeIter      iter;
	GtkCellRenderer *renderer;
	gint             i;

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (prefs->combo_default_timeline),
								renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (prefs->combo_default_timeline),
									renderer, "text", COL_COMBO_VISIBLE_NAME, NULL);

	model = gtk_list_store_new (COL_COMBO_COUNT,
								G_TYPE_STRING,
								G_TYPE_STRING);

	gtk_combo_box_set_model (GTK_COMBO_BOX (prefs->combo_default_timeline),
							 GTK_TREE_MODEL (model));

	for (i = 0; timelines[i]; i += 2) {
		gtk_list_store_append (model, &iter);
		gtk_list_store_set (model, &iter,
							COL_COMBO_VISIBLE_NAME, _(timelines[i + 1]),
							COL_COMBO_NAME, timelines[i],
							-1);
	}

	g_object_unref (model);
}

static void
preferences_reload_setup (Prefs *prefs)
{
	GtkListStore    *model;
	GtkTreeIter      iter;
	GtkCellRenderer *renderer;
	reload_time     *options;

	options = reload_list;

	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (prefs->combo_reload),
								renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (prefs->combo_reload),
									renderer, "text", COL_COMBO_VISIBLE_NAME, NULL);

	model = gtk_list_store_new (COL_COMBO_COUNT,
								G_TYPE_STRING,
								G_TYPE_INT);

	gtk_combo_box_set_model (GTK_COMBO_BOX (prefs->combo_reload),
							 GTK_TREE_MODEL (model));

	while (options->display_text != NULL) {
		gtk_list_store_append (model, &iter);
		gtk_list_store_set (model, &iter,
							COL_COMBO_VISIBLE_NAME, _(options->display_text),
							COL_COMBO_NAME, options->minutes,
							-1);
		options++;
	}

	g_object_unref (model);
}

static void
preferences_widget_sync_bool (const gchar *key, GtkWidget *widget)
{
	gboolean value;

	if (conf_get_bool (conf_get (), key, &value)) {
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (widget), value);
	}
}

static void
preferences_widget_sync_string_combo (const gchar *key, GtkWidget *widget)
{
	gchar        *value;
	GtkTreeModel *model;
	GtkTreeIter   iter;
	gboolean      found;

	if (!conf_get_string (conf_get (), key, &value)) {
		return;
	}

	model = gtk_combo_box_get_model (GTK_COMBO_BOX (widget));

	found = FALSE;
	if (value && gtk_tree_model_get_iter_first (model, &iter)) {
		gchar *name;

		do {
			gtk_tree_model_get (model, &iter,
								COL_COMBO_NAME, &name,
								-1);

			if (strcmp (name, value) == 0) {
				found = TRUE;
				gtk_combo_box_set_active_iter (GTK_COMBO_BOX (widget), &iter);
				break;
			} else {
				found = FALSE;
			}

			g_free (name);
		} while (gtk_tree_model_iter_next (model, &iter));
	}

	/* Fallback to the first one. */
	if (!found) {
		if (gtk_tree_model_get_iter_first (model, &iter)) {
			gtk_combo_box_set_active_iter (GTK_COMBO_BOX (widget), &iter);
		}
	}

	g_free (value);
}

static void
preferences_widget_sync_int_combo (const gchar *key, GtkWidget *widget)
{
	gint          value;
	GtkTreeModel *model;
	GtkTreeIter   iter;
	gboolean      found;

	if (!conf_get_int (conf_get (), key, &value)) {
		return;
	}

	model = gtk_combo_box_get_model (GTK_COMBO_BOX (widget));

	found = FALSE;
	if (value && gtk_tree_model_get_iter_first (model, &iter)) {
		gint minutes;

		do {
			gtk_tree_model_get (model, &iter,
								COL_COMBO_NAME, &minutes,
								-1);

			if (minutes == value) {
				found = TRUE;
				gtk_combo_box_set_active_iter (GTK_COMBO_BOX (widget), &iter);
				break;
			} else {
				found = FALSE;
			}

		} while (gtk_tree_model_iter_next (model, &iter));
	}

	/* Fallback to the first one. */
	if (!found) {
		if (gtk_tree_model_get_iter_first (model, &iter)) {
			gtk_combo_box_set_active_iter (GTK_COMBO_BOX (widget), &iter);
		}
	}
}

static void
preferences_notify_string_combo_cb (Conf  *conf,
									const gchar *key,
									gpointer     user_data)
{
	preferences_widget_sync_string_combo (key, user_data);
}

static void
preferences_notify_int_combo_cb (Conf  *conf,
								 const gchar *key,
								 gpointer     user_data)
{
	preferences_widget_sync_int_combo (key, user_data);
}

static void
preferences_add_id (Prefs *prefs, guint id)
{
	prefs->notify_ids = g_list_prepend (prefs->notify_ids,
										GUINT_TO_POINTER (id));
}

static void
preferences_hookup_toggle_button (Prefs *prefs,
								  const gchar *key,
								  GtkWidget   *widget)
{
	guint id;

	preferences_widget_sync_bool (key, widget);

	g_object_set_data_full (G_OBJECT (widget), "key",
							g_strdup (key), g_free);

	g_signal_connect (widget,
					  "toggled",
					  G_CALLBACK (preferences_toggle_button_toggled_cb),
					  NULL);

	id = conf_notify_add (conf_get (),
								 key,
								 preferences_notify_bool_cb,
								 widget);

	if (id) {
		preferences_add_id (prefs, id);
	}
}

static void
preferences_hookup_string_combo (Prefs *prefs,
								 const gchar *key,
								 GtkWidget   *widget)
{
	guint id;

	preferences_widget_sync_string_combo (key, widget);

	g_object_set_data_full (G_OBJECT (widget), "key",
							g_strdup (key), g_free);

	g_signal_connect (widget,
					  "changed",
					  G_CALLBACK (preferences_string_combo_changed_cb),
					  NULL);

	id = conf_notify_add (conf_get (),
								 key,
								 preferences_notify_string_combo_cb,
								 widget);
	if (id) {
		preferences_add_id (prefs, id);
	}
}

static void
preferences_hookup_int_combo (Prefs *prefs,
								 const gchar *key,
								 GtkWidget   *widget)
{
	guint id;

	preferences_widget_sync_int_combo (key, widget);

	g_object_set_data_full (G_OBJECT (widget), "key",
							g_strdup (key), g_free);

	g_signal_connect (widget,
					  "changed",
					  G_CALLBACK (preferences_int_combo_changed_cb),
					  NULL);

	id = conf_notify_add (conf_get (),
								 key,
								 preferences_notify_int_combo_cb,
								 widget);

	if (id) {
		preferences_add_id (prefs, id);
	}
}

static void
preferences_toggle_button_toggled_cb (GtkWidget *button,
									  gpointer   user_data)
{
	const gchar *key;

	key = g_object_get_data (G_OBJECT (button), "key");

	conf_set_bool (conf_get (),
						  key,
						  gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)));
}

static void
preferences_string_combo_changed_cb (GtkWidget *combo,
									 gpointer   user_data)
{
	const gchar  *key;
	GtkTreeModel *model;
	GtkTreeIter   iter;
	gchar        *name;

	key = g_object_get_data (G_OBJECT (combo), "key");

	if (gtk_combo_box_get_active_iter (GTK_COMBO_BOX (combo), &iter)) {
		model = gtk_combo_box_get_model (GTK_COMBO_BOX (combo));

		gtk_tree_model_get (model, &iter,
							COL_COMBO_NAME, &name,
							-1);
		conf_set_string (conf_get (), key, name);
		g_free (name);
	}
}

static void
preferences_int_combo_changed_cb (GtkWidget *combo,
								  gpointer   user_data)
{
	const gchar  *key;
	GtkTreeModel *model;
	GtkTreeIter   iter;
	gint          minutes;

	key = g_object_get_data (G_OBJECT (combo), "key");

	if (gtk_combo_box_get_active_iter (GTK_COMBO_BOX (combo), &iter)) {
		model = gtk_combo_box_get_model (GTK_COMBO_BOX (combo));

		gtk_tree_model_get (model, &iter,
							COL_COMBO_NAME, &minutes,
							-1);

		conf_set_int (conf_get (), key, minutes);
	}
}

static void
preferences_response_cb (GtkWidget   *widget,
						 gint         response,
						 Prefs *prefs)
{
	gtk_widget_destroy (widget);
}
static void
preferences_destroy_cb (GtkWidget   *widget,
						Prefs *prefs)
{
	GList *l;

	for (l = prefs->notify_ids; l; l = l->next) {
		guint id;

		id = GPOINTER_TO_UINT (l->data);
		conf_notify_remove (conf_get (), id);
	}

	g_list_free (prefs->notify_ids);
	g_free (prefs);
}

void
preferences_dialog_show (GtkWindow *parent)
{
	static Prefs *prefs;
	GtkBuilder         *ui;

	if (prefs) {
		gtk_window_present (GTK_WINDOW (prefs->dialog));
		return;
	}

	prefs = g_new0 (Prefs, 1);

	/* Get widgets */
	ui = gtkbuilder_get_file (GtkBuilderUI,
							  "preferences_dialog", &prefs->dialog,
							  "preferences_notebook", &prefs->notebook,
							  "combobox_timeline", &prefs->combo_default_timeline,
							  "combobox_reload", &prefs->combo_reload,
							  "autoconnect_checkbutton", &prefs->autoconnect,
							  "notify_checkbutton", &prefs->notify,
							  "sound_checkbutton", &prefs->sound,
							  NULL);

	/* Connect the signals */
	gtkbuilder_connect (ui, prefs,
						"preferences_dialog", "destroy", preferences_destroy_cb,
						"preferences_dialog", "response", preferences_response_cb,
						NULL);

	g_object_unref (ui);

	g_object_add_weak_pointer (G_OBJECT (prefs->dialog), (gpointer) &prefs);
	gtk_window_set_transient_for (GTK_WINDOW (prefs->dialog), parent);

	/* Set up the rest of the widget */
	preferences_timeline_setup (prefs);
	preferences_reload_setup (prefs);

	preferences_setup_widgets (prefs);

	gtk_widget_show (prefs->dialog);
}
