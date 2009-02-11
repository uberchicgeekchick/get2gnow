/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2004-2007 Imendio AB
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
 * Authors: Martyn Russell <martyn@imendio.com>
 *          Richard Hult <richard@imendio.com>
 *			Brian Pepple <bpepple@fedoraproject.org>
 */

#include "config.h"

#include <string.h>

#include <glib/gi18n.h>
#include <gtk/gtkbuilder.h>
#include <gtk/gtkcellrenderertext.h>
#include <gtk/gtkdialog.h>
#include <gtk/gtklabel.h>
#include <gtk/gtkliststore.h>
#include <gtk/gtktreeview.h>
#include <gtk/gtktreeselection.h>
#include <gtk/gtksizegroup.h>

#include <libtwitux/twitux-xml.h>

#include "twitux-send-message-dialog.h"
#include "twitux-spell.h"
#include "twitux-spell-dialog.h"

#define XML_FILE "spell_dlg.xml"

typedef struct {
	GtkWidget   *window;
	GtkWidget   *button_replace;
	GtkWidget   *label_word;
	GtkWidget   *treeview_words;

	GtkWidget   *textview;

	gchar       *word;
	GtkTextIter  start;
	GtkTextIter  end;
} TwituxSpellDialog;

enum {
	COL_SPELL_WORD,
	COL_SPELL_COUNT
};

static void spell_dialog_model_populate_columns     (TwituxSpellDialog *dialog);
static void spell_dialog_model_populate_suggestions (TwituxSpellDialog *dialog);
static void spell_dialog_model_row_activated_cb     (GtkTreeView       *tree_view,
													 GtkTreePath       *path,
													 GtkTreeViewColumn *column,
													 TwituxSpellDialog *dialog);
static void spell_dialog_model_selection_changed_cb (GtkTreeSelection  *treeselection,
													 TwituxSpellDialog *dialog);
static void spell_dialog_model_setup                (TwituxSpellDialog *dialog);
static void spell_dialog_response_cb                (GtkWidget         *widget,
													 gint               response,
													 TwituxSpellDialog *dialog);
static void spell_dialog_destroy_cb                 (GtkWidget         *widget,
													 TwituxSpellDialog *dialog);

static void
spell_dialog_model_populate_columns (TwituxSpellDialog *dialog)
{
	GtkTreeModel      *model;
	GtkTreeViewColumn *column;
	GtkCellRenderer   *renderer;
	guint              col_offset;

	model = gtk_tree_view_get_model (GTK_TREE_VIEW (dialog->treeview_words));

	renderer = gtk_cell_renderer_text_new ();
	col_offset = gtk_tree_view_insert_column_with_attributes (
		GTK_TREE_VIEW (dialog->treeview_words),
		-1, _("Word"),
		renderer,
		"text", COL_SPELL_WORD,
		NULL);

	g_object_set_data (G_OBJECT (renderer),
					   "column", GINT_TO_POINTER (COL_SPELL_WORD));

	column = gtk_tree_view_get_column (GTK_TREE_VIEW (dialog->treeview_words), col_offset - 1);
	gtk_tree_view_column_set_sort_column_id (column, COL_SPELL_WORD);
	gtk_tree_view_column_set_resizable (column, FALSE);
	gtk_tree_view_column_set_clickable (GTK_TREE_VIEW_COLUMN (column), TRUE);
}

static void
spell_dialog_model_populate_suggestions (TwituxSpellDialog *dialog)
{
	GtkTreeModel *model;
	GtkListStore *store;
	GList        *suggestions, *l;

	model = gtk_tree_view_get_model (GTK_TREE_VIEW (dialog->treeview_words));
	store = GTK_LIST_STORE (model);

	suggestions = (GList *) twitux_spell_get_suggestions (dialog->word);
	for (l = suggestions; l; l=l->next) {
		GtkTreeIter  iter;
		gchar       *word;

		word = l->data;

		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter,
							COL_SPELL_WORD, word,
							-1);
	}

	twitux_spell_free_suggestions (suggestions);
}

static void
spell_dialog_model_row_activated_cb (GtkTreeView       *tree_view,
									 GtkTreePath       *path,
									 GtkTreeViewColumn *column,
									 TwituxSpellDialog *dialog)
{
	spell_dialog_response_cb (dialog->window, GTK_RESPONSE_OK, dialog);
}

static void
spell_dialog_model_selection_changed_cb (GtkTreeSelection  *treeselection,
										 TwituxSpellDialog *dialog)
{
	gint count;

	count = gtk_tree_selection_count_selected_rows (treeselection);
	gtk_widget_set_sensitive (dialog->button_replace, (count == 1));
}

static void
spell_dialog_model_setup (TwituxSpellDialog *dialog)
{
	GtkTreeView      *view;
	GtkListStore     *store;
	GtkTreeSelection *selection;

	view = GTK_TREE_VIEW (dialog->treeview_words);

	g_signal_connect (view, "row-activated",
					  G_CALLBACK (spell_dialog_model_row_activated_cb),
					  dialog);

	store = gtk_list_store_new (COL_SPELL_COUNT,
								G_TYPE_STRING);   /* word */

	gtk_tree_view_set_model (view, GTK_TREE_MODEL (store));

	selection = gtk_tree_view_get_selection (view);
	gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);

	g_signal_connect (selection, "changed",
					  G_CALLBACK (spell_dialog_model_selection_changed_cb),
					  dialog);

	spell_dialog_model_populate_columns (dialog);
	spell_dialog_model_populate_suggestions (dialog);

	g_object_unref (store);
}

static void
spell_dialog_destroy_cb (GtkWidget         *widget,
						 TwituxSpellDialog *dialog)
{
	g_free (dialog->word);
	g_free (dialog);
}

static void
spell_dialog_response_cb (GtkWidget         *widget,
						  gint               response,
						  TwituxSpellDialog *dialog)
{
	if (response == GTK_RESPONSE_OK) {
		GtkTreeView      *view;
		GtkTreeModel     *model;
		GtkTreeSelection *selection;
		GtkTreeIter       iter;

		gchar            *new_word;

		view = GTK_TREE_VIEW (dialog->treeview_words);
		selection = gtk_tree_view_get_selection (view);

		if (!gtk_tree_selection_get_selected (selection, &model, &iter)) {
			return;
		}

		gtk_tree_model_get (model, &iter, COL_SPELL_WORD, &new_word, -1);

		twitux_message_correct_word (dialog->textview,
									 dialog->start,
									 dialog->end,
									 new_word);

		g_free (new_word);
	}

	gtk_widget_destroy (dialog->window);
}

void
twitux_spell_dialog_show (GtkWidget   *textview,
						  GtkTextIter  start,
						  GtkTextIter  end,
						  const gchar *word)
{
	TwituxSpellDialog *dialog;
	GtkBuilder        *ui;
	gchar             *str;

	g_return_if_fail (textview != NULL);
	g_return_if_fail (word != NULL);

	dialog = g_new0 (TwituxSpellDialog, 1);

	dialog->textview = textview;

	dialog->word = g_strdup (word);

	dialog->start = start;
	dialog->end = end;

	/* Get widgets */
	ui = twitux_xml_get_file (XML_FILE,
						"spell_dialog", &dialog->window,
						"button_replace", &dialog->button_replace,
						"label_word", &dialog->label_word,
						"treeview_words", &dialog->treeview_words,
						NULL);

	/* Connect the signals */
	twitux_xml_connect (ui, dialog,
						"spell_dialog", "destroy", spell_dialog_destroy_cb,
						"spell_dialog", "response", spell_dialog_response_cb,
						NULL);

	g_object_unref (ui);

	/* Set the label */
	str = g_strdup_printf ("%s:\n<b>%s</b>",
						   _("Suggestions for the word"),
						   word);
	gtk_label_set_markup (GTK_LABEL (dialog->label_word), str);
	g_free (str);

	/* Setup the list */
	spell_dialog_model_setup (dialog);

	/* Now let's show it */
	gtk_widget_show (dialog->window);
}
