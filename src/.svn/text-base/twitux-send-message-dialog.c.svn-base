/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2002-2007 Imendio AB
 * Copyright (C) 2007-2009 Brian Pepple <bpepple@fedoraproject.org>
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
 *          Geert-Jan Van den Bogaerde <geertjan@gnome.org>
 *          Brian Pepple <bpepple@fedoraproject.org>
 *          Daniel Morales <daniminas@gmail.com>
 */

#include "config.h"

#include <string.h>

#include <glib/gi18n.h>

#include <libtwitux/twitux-conf.h>
#include <libtwitux/twitux-debug.h>
#include <libtwitux/twitux-xml.h>

#include "twitux.h"
#include "twitux-send-message-dialog.h"
#include "twitux-spell.h"
#include "twitux-spell-dialog.h"
#include "twitux-network.h"

#define DEBUG_DOMAIN_SETUP    "SendMessage"
#define XML_FILE              "send_message_dlg.xml"

/* Let's use the preferred maximum character count */
#define MAX_CHARACTER_COUNT 140

#define GET_PRIV(obj)          \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), TWITUX_TYPE_MESSAGE, TwituxMsgDialogPriv))

typedef struct {
	GtkWidget         *textview;
	gchar             *word;

	GtkTextIter        start;
	GtkTextIter        end;
} TwituxMessageSpell;

struct _TwituxMsgDialogPriv {
	/* Main widgets */
	GtkWidget         *dialog;
	GtkWidget         *textview;
	GtkWidget         *label;
	GtkWidget         *send_button;
	
	GtkWidget         *friends_combo;
	GtkWidget         *friends_label;
	gboolean           show_friends;
};

static void	twitux_message_class_init		     (TwituxMsgDialogClass *klass);
static void twitux_message_init			         (TwituxMsgDialog      *dialog);
static void message_finalize                     (GObject              *object);
static void message_setup                        (GtkWindow            *parent);
static void message_set_characters_available     (GtkTextBuffer        *buffer,
												  TwituxMsgDialog      *dialog);
static void message_text_buffer_changed_cb       (GtkTextBuffer        *buffer,
											      TwituxMsgDialog      *dialog);
static void message_text_populate_popup_cb       (GtkTextView          *view,
												  GtkMenu              *menu,
												  TwituxMsgDialog      *dialog);
static void message_text_check_word_spelling_cb  (GtkMenuItem          *menuitem,
												  TwituxMessageSpell   *message_spell);
static TwituxMessageSpell *message_spell_new     (GtkWidget            *window,
												  const gchar          *word,
												  GtkTextIter           start,
												  GtkTextIter           end);
static void message_spell_free                   (TwituxMessageSpell   *message_spell);
static void message_destroy_cb                   (GtkWidget            *widget,
												  TwituxMsgDialog      *dialog);
static void message_response_cb                  (GtkWidget            *widget,
												  gint                  response,
												  TwituxMsgDialog      *dialog);

static TwituxMsgDialog  *dialog = NULL;

G_DEFINE_TYPE (TwituxMsgDialog, twitux_message, G_TYPE_OBJECT);

static void
twitux_message_class_init (TwituxMsgDialogClass *klass)
{
	GObjectClass  *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = message_finalize;

	g_type_class_add_private (object_class, sizeof (TwituxMsgDialogPriv));
}

static void
twitux_message_init (TwituxMsgDialog *singleton_message)
{
	dialog = singleton_message;
}

static void
message_finalize (GObject *object)
{	
	G_OBJECT_CLASS (twitux_message_parent_class)->finalize (object);
}

static void
message_setup (GtkWindow  *parent)
{
	TwituxMsgDialogPriv   *priv;
	GtkBuilder            *ui;
	GtkTextBuffer         *buffer;
	const gchar           *standard_msg;
	gchar                 *character_count;
	GtkCellRenderer       *renderer;
	GtkListStore          *model_followers;
	
	priv = GET_PRIV (dialog);

	/* Set up interface */
	twitux_debug (DEBUG_DOMAIN_SETUP, "Initialising message dialog");

	/* Get widgets */
	ui =
		twitux_xml_get_file (XML_FILE,
							 "send_message_dialog", &priv->dialog,
							 "send_message_textview", &priv->textview,
							 "char_label", &priv->label,
							 "friends_combo", &priv->friends_combo,
							 "friends_label", &priv->friends_label,
							 "send_button", &priv->send_button,
							 NULL);
	
	/* Connect the signals */
	twitux_xml_connect (ui, dialog,
						"send_message_dialog", "destroy", message_destroy_cb,
						"send_message_dialog", "response", message_response_cb,
						"send_message_textview", "populate_popup", message_text_populate_popup_cb,
						NULL);

	g_object_unref (ui);

	/* Set the label */
	standard_msg = _("Characters Available");
	character_count =
		g_markup_printf_escaped ("<span size=\"small\">%s: %i</span>",
								 standard_msg, MAX_CHARACTER_COUNT);
	gtk_label_set_markup (GTK_LABEL (priv->label), character_count);
	g_free (character_count);

	/* Connect the signal to the textview */
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->textview));
	g_signal_connect (buffer,
					  "changed",
					  G_CALLBACK (message_text_buffer_changed_cb),
					  dialog);

	/* Create misspelt words identification tag */
	gtk_text_buffer_create_tag (buffer,
								"misspelled",
								"underline", PANGO_UNDERLINE_ERROR,
								NULL);

	gtk_window_set_transient_for (GTK_WINDOW (priv->dialog), parent);

	/* Setup followers combobox's model */
	model_followers = gtk_list_store_new (1, G_TYPE_STRING);
	gtk_combo_box_set_model (GTK_COMBO_BOX (priv->friends_combo),
							 GTK_TREE_MODEL (model_followers));
	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (priv->friends_combo),
								renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (priv->friends_combo),
									renderer, "text", 0, NULL);

	gtk_widget_grab_focus(GTK_WIDGET (priv->textview));
	
	/* Show the dialog */
	gtk_widget_show (GTK_WIDGET (priv->dialog));
}

void
twitux_send_message_dialog_show (GtkWindow *parent)
{
	TwituxMsgDialogPriv   *priv;
	
	if (dialog){
		priv = GET_PRIV (dialog);
		gtk_window_present (GTK_WINDOW (priv->dialog));
		return;
	}

	g_object_new (TWITUX_TYPE_MESSAGE, NULL);

	message_setup (parent);
}

void
twitux_message_correct_word (GtkWidget   *textview,
							 GtkTextIter  start,
							 GtkTextIter  end,
							 const gchar *new_word)
{
	GtkTextBuffer *buffer;

	g_return_if_fail (textview != NULL);
	g_return_if_fail (new_word != NULL);

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));

	gtk_text_buffer_delete (buffer, &start, &end);
	gtk_text_buffer_insert (buffer, &start,
							new_word,
							-1);
}

void
twitux_message_set_followers (GList *followers)
{
	TwituxMsgDialogPriv *priv;
	GList               *list;
	GtkTreeIter          iter;
	TwituxUser          *user;
	GtkListStore        *model_followers;

	priv = GET_PRIV (dialog);

	model_followers =
		GTK_LIST_STORE (gtk_combo_box_get_model (
							GTK_COMBO_BOX (priv->friends_combo)));

	for (list = followers; list; list = list->next) {
		user = (TwituxUser *)list->data;
		gtk_list_store_append (model_followers, &iter);
		gtk_list_store_set (model_followers,
							&iter,
							0, user->screen_name,
							-1);
	}
}

void
twitux_message_show_friends (gboolean show_friends)
{
	TwituxMsgDialogPriv *priv = GET_PRIV (dialog);
	priv->show_friends = show_friends;

	if (show_friends){
		GList *followers;
		gtk_widget_show (priv->friends_combo);
		gtk_widget_show (priv->friends_label);
		
		/* Let's populate the combobox */
		followers = twitux_network_get_followers ();
		if (followers){
			twitux_debug (DEBUG_DOMAIN_SETUP, "Loaded previous followers list");
			twitux_message_set_followers (followers);
		} else {
			twitux_debug (DEBUG_DOMAIN_SETUP, "Fetching followers...");
		}
		return;
	}
	gtk_widget_hide (priv->friends_combo);
	gtk_widget_hide (priv->friends_label);
}

void
twitux_message_set_message (const gchar *message)
{
	TwituxMsgDialogPriv *priv = GET_PRIV (dialog);
	GtkTextBuffer *buffer;

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->textview));

	gtk_text_buffer_set_text (buffer, message, -1);

	gtk_window_set_focus (GTK_WINDOW (priv->dialog), priv->textview);
}

static gchar *
url_encode_message (gchar *text)
{
	const char        *good;
	static const char  hex[16] = "0123456789ABCDEF";
	GString           *result;

	g_return_val_if_fail (text != NULL, NULL);
	g_return_val_if_fail (*text != '\0', NULL);

	/* RFC 3986 */ 
	good = "~-._";

	result = g_string_new (NULL);
	while (*text) {
		unsigned char c = *text++;

		if (g_ascii_isalnum (c) || strchr (good, c))
			g_string_append_c (result, c);
		else {
			g_string_append_c (result, '%');
			g_string_append_c (result, hex[c >> 4]);
			g_string_append_c (result, hex[c & 0xf]);
		}
	}

	return g_string_free (result, FALSE);
}

static void
message_set_characters_available (GtkTextBuffer     *buffer,
								  TwituxMsgDialog   *dialog)
{
	TwituxMsgDialogPriv *priv;
	gint i;
	gint count;
	const gchar *standard_msg;
	gchar *character_count;

	priv = GET_PRIV (dialog);

	i = gtk_text_buffer_get_char_count (buffer);

	count = MAX_CHARACTER_COUNT - i;

	standard_msg = _("Characters Available");

	if (i > MAX_CHARACTER_COUNT) {
		character_count =
			g_markup_printf_escaped ("<span size=\"small\">%s: <span foreground=\"red\">%i</span></span>",
									 standard_msg, count);
		gtk_widget_set_sensitive (priv->send_button, FALSE);
	} else {
		character_count =
			g_markup_printf_escaped ("<span size=\"small\">%s: %i</span>",
									 standard_msg, count);
		gtk_widget_set_sensitive (priv->send_button, TRUE);
	}

	gtk_label_set_markup (GTK_LABEL (priv->label), character_count);
	g_free (character_count);
}

static void
message_text_buffer_changed_cb (GtkTextBuffer    *buffer,
                                TwituxMsgDialog  *dialog)
{
	TwituxMsgDialogPriv   *priv;
	GtkTextIter            start;
	GtkTextIter            end;
	gchar                 *str;
	gboolean               spell_checker = FALSE;

	priv = GET_PRIV (dialog);

	message_set_characters_available (buffer, dialog);

	twitux_conf_get_bool (twitux_conf_get (),
						  TWITUX_PREFS_UI_SPELL,
						  &spell_checker);

	gtk_text_buffer_get_start_iter (buffer, &start);

	if (!spell_checker) {
		gtk_text_buffer_get_end_iter (buffer, &end);
		gtk_text_buffer_remove_tag_by_name (buffer, "misspelled", &start, &end);
		return;
	}

	if (!twitux_spell_supported ()) {
		return;
	}

	while (TRUE) {
		gboolean correct = FALSE;

		/* if at start */
		if (gtk_text_iter_is_start (&start)) {
			end = start;

			if (!gtk_text_iter_forward_word_end (&end)) {
				/* no whole word yet */
				break;
			}
		} else {
			if (!gtk_text_iter_forward_word_end (&end)) {
				/* must be the end of the buffer */
				break;
			}

			start = end;
			gtk_text_iter_backward_word_start (&start);
		}

		str = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

		/* spell check string */
		correct = twitux_spell_check (str);

		if (!correct) {
			gtk_text_buffer_apply_tag_by_name (buffer, "misspelled",
											   &start, &end);
		} else {
			gtk_text_buffer_remove_tag_by_name (buffer, "misspelled",
												&start, &end);
		}

		g_free (str);

		/* set the start iter to the end iters position */
		start = end;
	}
}

static void
message_text_populate_popup_cb (GtkTextView        *view,
								GtkMenu            *menu,
								TwituxMsgDialog    *dialog)
{
	TwituxMsgDialogPriv   *priv;
	GtkTextBuffer         *buffer;
	GtkTextTagTable       *table;
	GtkTextTag            *tag;
	gint                   x,y;
	GtkTextIter            iter, start, end;
	GtkWidget             *item;
	gchar                 *str = NULL;
	TwituxMessageSpell    *message_spell;

	priv = GET_PRIV (dialog);

	/* Add the spell check menu item */
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->textview));
	table = gtk_text_buffer_get_tag_table (buffer);

	tag = gtk_text_tag_table_lookup (table, "misspelled");

	gtk_widget_get_pointer (GTK_WIDGET (view), &x, &y);

	gtk_text_view_window_to_buffer_coords (GTK_TEXT_VIEW (view),
										   GTK_TEXT_WINDOW_WIDGET,
										   x, y,
										   &x, &y);

	gtk_text_view_get_iter_at_location (GTK_TEXT_VIEW (view), &iter, x, y);

	start = end = iter;

	if (gtk_text_iter_backward_to_tag_toggle (&start, tag) &&
		gtk_text_iter_forward_to_tag_toggle (&end, tag)) {
		str = gtk_text_buffer_get_text (buffer,
										&start, &end, FALSE);
	}

	if (G_STR_EMPTY (str)) {
		return;
	}

	message_spell = message_spell_new (priv->textview, str, start, end);

	g_object_set_data_full (G_OBJECT (menu),
							"message_spell", message_spell,
							(GDestroyNotify) message_spell_free);

	item = gtk_separator_menu_item_new ();
	gtk_menu_shell_prepend (GTK_MENU_SHELL (menu), item);
	gtk_widget_show (item);

	item = gtk_menu_item_new_with_mnemonic (_("_Check Word Spelling..."));
	g_signal_connect (item,
					  "activate",
					  G_CALLBACK (message_text_check_word_spelling_cb),
					  message_spell);
	gtk_menu_shell_prepend (GTK_MENU_SHELL (menu), item);
	gtk_widget_show (item);
}

static void
message_text_check_word_spelling_cb (GtkMenuItem        *menuitem,
									 TwituxMessageSpell *chat_spell)
{
	twitux_spell_dialog_show (chat_spell->textview,
							  chat_spell->start,
							  chat_spell->end,
							  chat_spell->word);
}

static TwituxMessageSpell *
message_spell_new (GtkWidget          *textview,
				   const gchar        *word,
				   GtkTextIter         start,
				   GtkTextIter         end)
{
	TwituxMessageSpell *message_spell;

	message_spell = g_new0 (TwituxMessageSpell, 1);

	message_spell->textview = textview;
	message_spell->word = g_strdup (word);
	message_spell->start = start;
	message_spell->end = end;

	return message_spell;
}

static void
message_spell_free (TwituxMessageSpell *message_spell)
{
	g_free (message_spell->word);
	g_free (message_spell);
}

static void
message_response_cb (GtkWidget          *widget,
					 gint                response,
					 TwituxMsgDialog    *dialog)
{
	TwituxMsgDialogPriv   *priv;

	if (response == GTK_RESPONSE_OK) {	  
		GtkTextBuffer  *buffer;
		GtkTextIter     start_iter;
		GtkTextIter     end_iter;

		priv = GET_PRIV (dialog);

		twitux_debug (DEBUG_DOMAIN_SETUP, "Posting message to Twitter");

		buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (priv->textview));
		gtk_text_buffer_get_start_iter (buffer, &start_iter);
		gtk_text_buffer_get_end_iter (buffer, &end_iter);

		if (!gtk_text_iter_equal (&start_iter, &end_iter)) {
				gchar          *text;
				gchar          *good_msg;

				text = gtk_text_buffer_get_text (buffer, &start_iter, &end_iter, TRUE);
				good_msg = url_encode_message (text);
				
				if (priv->show_friends)
				{
					GtkTreeIter   iter;
					gchar        *to_user;
					GtkComboBox  *combo = GTK_COMBO_BOX (priv->friends_combo);
					GtkTreeModel *model = gtk_combo_box_get_model (combo);
					/* Send a direct message  */
					if (gtk_combo_box_get_active_iter (combo, &iter)){
						/* Get friend username */
						gtk_tree_model_get (model,
											&iter,
											0, &to_user,
											-1);
						/* Send the message */
						twitux_network_send_message (to_user, good_msg);
						g_free (to_user);
					}
				} else {
					/* Post a tweet */
					twitux_network_post_status (good_msg);
				}

				g_free (text);
				g_free (good_msg);
		}
	}
	gtk_widget_destroy (widget);
}

static void
message_destroy_cb (GtkWidget         *widget,
					TwituxMsgDialog   *dialoga)
{
	TwituxMsgDialogPriv *priv;

	priv = GET_PRIV (dialog);

	/* Add any clean-up code here */
	
	g_object_unref (dialog);
	dialog = NULL;
}
