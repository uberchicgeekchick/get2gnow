/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright(C) 2002-2007 Imendio AB
 * Copyright(C) 2007-2009 Brian Pepple <bpepple@fedoraproject.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or(at your option) any later version.
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

#include "gconf.h"
#include "debug.h"
#include "gtkbuilder.h"

#include "main.h"
#include "app.h"
#include "send-message-dialog.h"
#include "spell.h"
#include "spell-dialog.h"
#include "network.h"
#include "users.h"
#include "tweets.h"

#define DEBUG_DOMAIN_SETUP    "SendMessage"
#define GtkBuilderUI              "send-message-dialog.ui"

/* Let's use the preferred maximum character count */
#define MAX_CHARACTER_COUNT 140

#define	GET_PRIV(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_MESSAGE, MsgDialogPriv))

typedef struct {
	GtkWidget         *textview;
	gchar             *word;

	GtkTextIter        start;
	GtkTextIter        end;
} MessageSpell;

struct _MsgDialogPriv {
	/* Main widgets */
	GtkWidget         *dialog;
	GtkWidget         *textview;
	GtkWidget         *label;
	GtkWidget         *send_button;
	
	GtkWidget         *friends_combo;
	GtkWidget         *friends_label;
	gboolean           show_friends;
};

static void message_class_init( MsgDialogClass *klass );
static void message_init( MsgDialog *signelton_message );
static void message_finalize( GObject *object );
static void message_setup( GtkWindow *parent );
static void message_set_characters_available( GtkTextBuffer *buffer );
static void message_text_buffer_undo_cb( GtkTextBuffer *buffer );
static void message_text_buffer_changed_cb( GtkTextBuffer *buffer );
static void message_text_check_word_spelling_cb( GtkMenuItem *menuitem, MessageSpell *message_spell );
static MessageSpell *message_spell_new( GtkWidget *window, const gchar *word, GtkTextIter start, GtkTextIter end);
static void message_spell_free( MessageSpell *message_spell );
static void message_destroy_cb( GtkWidget *widget );
static void message_response_cb( GtkWidget *widget, gint response );

static void message_text_button_release_cb( GtkTextView *view, GdkEventButton *event );
static void message_text_populate_popup_cb( GtkTextView *view, GtkMenu *menu );


static GtkTextBuffer *undo_buffer=NULL;
static MsgDialog  *dialog = NULL;
static MsgDialogPriv *dialog_priv=NULL;
G_DEFINE_TYPE(MsgDialog, message, G_TYPE_OBJECT);

static void message_class_init(MsgDialogClass *klass){
	GObjectClass  *object_class=G_OBJECT_CLASS(klass);
	object_class->finalize=message_finalize;
	g_type_class_add_private(object_class, sizeof(MsgDialogPriv));
}

static void message_init( MsgDialog *singleton_message ){
	dialog=singleton_message;
	dialog_priv=GET_PRIV( dialog );
}

static void message_finalize(GObject *object){	
	G_OBJECT_CLASS(message_parent_class)->finalize( object );
}

static void message_setup( GtkWindow  *parent ){
	GtkBuilder            *ui;
	GtkTextBuffer         *buffer;
	const gchar           *standard_msg;
	gchar                 *character_count;
	GtkCellRenderer       *renderer;
	GtkListStore          *model_followers;
	
	/* Set up interface */
	debug(DEBUG_DOMAIN_SETUP, "Initialising message dialog");

	/* Get widgets */
	ui=gtkbuilder_get_file(	GtkBuilderUI,
				"send_message_dialog", &dialog_priv->dialog,
				"send_message_textview", &dialog_priv->textview,
				"char_label", &dialog_priv->label,
				"friends_combo", &dialog_priv->friends_combo,
				"friends_label", &dialog_priv->friends_label,
				"send_button", &dialog_priv->send_button,
				NULL
	);
	
	/* Connect the signals */
	gtkbuilder_connect(	ui, dialog,
			"send_message_dialog", "destroy", message_destroy_cb,
			"send_message_dialog", "response", message_response_cb,
			"send_message_textview", "button_release_event", message_text_button_release_cb,
			"send_message_textview", "populate-popup", message_text_populate_popup_cb,
			NULL
	);

	g_object_unref(ui);

	/* Set the label */
	standard_msg = _("Characters Available");
	character_count = g_markup_printf_escaped("<span size=\"small\">%s: %i</span>", standard_msg, MAX_CHARACTER_COUNT);
	gtk_label_set_markup(GTK_LABEL(dialog_priv->label), character_count);
	g_free(character_count);

	/* Connect the signal to the textview */
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(dialog_priv->textview));
	undo_buffer=gtk_text_view_get_buffer((GTK_TEXT_VIEW( dialog_priv->textview )) );

	/* For spell checking from when the keyboard's 'context menu' key is pressed.
	 * g_signal_connect(buffer, "context-menu", G_CALLBACK(), dialog);
	 */
	g_signal_connect( buffer, "changed", G_CALLBACK(message_text_buffer_changed_cb), dialog);
	g_signal_connect( buffer, "modified-changed", G_CALLBACK( message_text_buffer_undo_cb ), dialog );

	/* Create misspelt words identification tag */
	gtk_text_buffer_create_tag(buffer,
								"misspelled",
								"underline", PANGO_UNDERLINE_ERROR,
								NULL);

	gtk_window_set_transient_for(GTK_WINDOW(dialog_priv->dialog), parent);

	/* Setup followers combobox's model */
	model_followers = gtk_list_store_new(1, G_TYPE_STRING);
	gtk_combo_box_set_model(GTK_COMBO_BOX(dialog_priv->friends_combo),
							 GTK_TREE_MODEL(model_followers));
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(dialog_priv->friends_combo),
								renderer, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(dialog_priv->friends_combo),
									renderer, "text", 0, NULL);

	/* Show the dialog */
	gtk_widget_show(GTK_WIDGET(dialog_priv->dialog));
}

void send_message_dialog_show(GtkWindow *parent){
	if(dialog){
		if(!dialog_priv)
			dialog_priv=GET_PRIV(dialog);
		undo_buffer=gtk_text_view_get_buffer((GTK_TEXT_VIEW( dialog_priv->textview )) );
		gtk_text_buffer_begin_user_action( undo_buffer );
		return gtk_window_present((GTK_WINDOW( dialog_priv->dialog )) );
	}
	
	g_object_new(TYPE_MESSAGE, NULL);
	message_setup(parent);
	if(!dialog_priv)
		dialog_priv=GET_PRIV(dialog);
	undo_buffer=gtk_text_view_get_buffer((GTK_TEXT_VIEW( dialog_priv->textview )) );
	gtk_text_buffer_begin_user_action( undo_buffer );
}

void
message_correct_word(GtkWidget   *textview,
							 GtkTextIter  start,
							 GtkTextIter  end,
							 const gchar *new_word)
{
	g_return_if_fail(textview != NULL);
	g_return_if_fail(new_word != NULL);

	GtkTextBuffer *buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

	gtk_text_buffer_delete(buffer, &start, &end);
	gtk_text_buffer_insert(buffer, &start,
							new_word,
							-1);
}

void message_set_followers(GList *followers){
	GList		*list;
	GtkTreeIter	iter;
	User		*user;
	GtkListStore	*model_followers;

	model_followers=GTK_LIST_STORE( gtk_combo_box_get_model( GTK_COMBO_BOX( dialog_priv->friends_combo ) ) );

	for(list = followers; list; list = list->next) {
		user =(User *)list->data;
		gtk_list_store_append(model_followers, &iter);
		gtk_list_store_set(model_followers,
							&iter,
							0, user->user_name,
							-1);
	}
}

void message_show_friends(gboolean show_friends){
	dialog_priv->show_friends=show_friends;

	if(!show_friends){
		gtk_widget_hide( dialog_priv->friends_combo );
		gtk_widget_hide( dialog_priv->friends_label );
		gtk_widget_grab_focus( GTK_WIDGET( dialog_priv->textview ) );
		return;
	}
	
	GList *followers;
	GdkCursor *cursor;
		
	gtk_widget_show( dialog_priv->friends_combo );
	gtk_widget_show( dialog_priv->friends_label );

	app_set_statusbar_msg(_("Please wait while the list of every one who\'s following you is loaded."));
	cursor=gdk_cursor_new(GDK_WATCH);
	gdk_window_set_cursor(GTK_WIDGET(dialog_priv->dialog)->window, cursor);
	gtk_widget_set_sensitive(dialog_priv->dialog, FALSE);

	/* Let's populate the combobox */
	debug(DEBUG_DOMAIN_SETUP, "Loading followers...");
	if((followers=user_get_followers()) )
		message_set_followers(followers);
	
	gdk_window_set_cursor(GTK_WIDGET(dialog_priv->dialog)->window, NULL);
	gtk_widget_set_sensitive(dialog_priv->dialog, TRUE);
	app_set_statusbar_msg("");
	gtk_widget_grab_focus(GTK_WIDGET(dialog_priv->friends_combo));
}

void message_set_message(const gchar *message){
	GtkTextBuffer *buffer;

	buffer=gtk_text_view_get_buffer( GTK_TEXT_VIEW(dialog_priv->textview) );

	gtk_text_buffer_set_text(buffer, message, -1);

	gtk_window_set_focus(GTK_WINDOW(dialog_priv->dialog), dialog_priv->textview);
}

static void message_set_characters_available( GtkTextBuffer *buffer ){
	gint i;
	gint count;
	const gchar *standard_msg;
	gchar *character_count;

		i = gtk_text_buffer_get_char_count(buffer);

	count = MAX_CHARACTER_COUNT - i;

	standard_msg = _("Characters Available");

	if(i > MAX_CHARACTER_COUNT) {
		character_count =
			g_markup_printf_escaped("<span size=\"small\">%s: <span foreground=\"red\">%i</span></span>",
									 standard_msg, count);
		gtk_widget_set_sensitive(dialog_priv->send_button, FALSE);
	} else {
		character_count =
			g_markup_printf_escaped("<span size=\"small\">%s: %i</span>",
									 standard_msg, count);
		gtk_widget_set_sensitive(dialog_priv->send_button, TRUE);
	}

	gtk_label_set_markup(GTK_LABEL(dialog_priv->label), character_count);
	g_free(character_count);
}

static void message_text_buffer_undo_cb( GtkTextBuffer *buffer ){
	gtk_text_buffer_end_user_action( buffer );
	gtk_text_buffer_begin_user_action( buffer );
}

static void message_text_buffer_changed_cb( GtkTextBuffer *buffer ){
	GtkTextIter            start, end;
	gboolean               spell_checker = FALSE;
	
	message_set_characters_available( buffer );
	conf_get_bool(conf_get(),
						  PREFS_UI_SPELL,
						  &spell_checker);

	gtk_text_buffer_get_start_iter(buffer, &start);

	if(!spell_checker) {
		gtk_text_buffer_get_end_iter(buffer, &end);
		gtk_text_buffer_remove_tag_by_name(buffer, "misspelled", &start, &end);
		return;
	}

	if(!spell_supported())
		return;

	while(TRUE) {
		gboolean correct = FALSE;

		/* if at start */
		if(gtk_text_iter_is_start(&start)) {
			end = start;
			
			/* no whole word yet */
			if(!gtk_text_iter_forward_word_end(&end))
				break;
		} else {
			/* must be the end of the buffer */
			if(!gtk_text_iter_forward_word_end(&end))
				break;
			
			start = end;
			gtk_text_iter_backward_word_start(&start);
		}

		gchar *str=gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
		
		gtk_text_buffer_begin_user_action( undo_buffer );
		
		/* spell check string */
		correct = spell_check(str);

		if(!correct)
			gtk_text_buffer_apply_tag_by_name(buffer, "misspelled", &start, &end);
		else
			gtk_text_buffer_remove_tag_by_name(buffer, "misspelled", &start, &end);

		g_free(str);

		/* set the start iter to the end iters position */
		start = end;
	}
}

static void message_text_button_release_cb( GtkTextView *view, GdkEventButton *event ){
	/*
	 * When the right mouse button is pressed the cursor/insert point needs to be
	 * moved so the context menu is drawn with or without the spell checking entry
	 * based on which word it being right clicked.
	 */
	GtkTextBuffer *buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(dialog_priv->textview));
	
	if( event->button != 3 ) {
		g_signal_emit_by_name(((gpointer *)buffer), "end-user-action" );
		return;
	}
	
	gint x,y;
	GtkTextIter iter;
	
	/* Find where the mouse pointer is at. */
	gtk_widget_get_pointer((GTK_WIDGET( view )), &x, &y );
	gtk_text_view_window_to_buffer_coords((GTK_TEXT_VIEW( view )), GTK_TEXT_WINDOW_WIDGET, x, y, &x, &y );
	gtk_text_view_get_iter_at_location((GTK_TEXT_VIEW( view )), &iter, x, y );
	
	gtk_text_buffer_select_range( buffer, &iter, &iter );
	g_signal_emit_by_name(((gpointer *)dialog), "populate-popup" );
}//message_text_button_release_cb

static void message_text_populate_popup_cb( GtkTextView *view, GtkMenu *menu ){
	GtkTextBuffer *buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(dialog_priv->textview));
	GtkTextTagTable *table=gtk_text_buffer_get_tag_table(buffer);
	GtkTextTag *tag=gtk_text_tag_table_lookup(table, "misspelled");
	GtkTextIter iter, start, end;
	GtkWidget *item;
	gchar *str=NULL;
	MessageSpell *message_spell;

	/* Find where the cursor's location, ie where you're typing. */
	GtkTextMark *cursor_position=gtk_text_buffer_get_insert( buffer );
	gtk_text_buffer_get_iter_at_mark( buffer, &iter, cursor_position );
	
	start = end = iter;

	if((gtk_text_iter_backward_to_tag_toggle( &start, tag )) &&(gtk_text_iter_forward_to_tag_toggle( &end, tag )) )
		str=gtk_text_buffer_get_text( buffer, &start, &end, FALSE );

	if((G_STR_EMPTY( str )) )
		return;

	message_spell=message_spell_new( dialog_priv->textview, str, start, end );

	g_object_set_data_full(	G_OBJECT( menu ),
				"message_spell", message_spell,
				(GDestroyNotify) message_spell_free
	);

	item=gtk_separator_menu_item_new();
	gtk_menu_shell_prepend( GTK_MENU_SHELL( menu ), item );
	gtk_widget_show( item );

	item = gtk_menu_item_new_with_mnemonic(_("_Check Word Spelling..."));
	g_signal_connect(	item,
				"activate",
				G_CALLBACK(message_text_check_word_spelling_cb),
				message_spell
	);
	gtk_menu_shell_prepend( GTK_MENU_SHELL(menu), item );
	gtk_widget_show( item );
}

static void
message_text_check_word_spelling_cb(GtkMenuItem        *menuitem,
									 MessageSpell *chat_spell)
{
	spell_dialog_show(chat_spell->textview,
							  chat_spell->start,
							  chat_spell->end,
							  chat_spell->word);
}

static MessageSpell *
message_spell_new(GtkWidget          *textview,
				   const gchar        *word,
				   GtkTextIter         start,
				   GtkTextIter         end)
{
	MessageSpell *message_spell;

	message_spell = g_new0(MessageSpell, 1);

	message_spell->textview = textview;
	message_spell->word = g_strdup(word);
	message_spell->start = start;
	message_spell->end = end;

	return message_spell;
}

static void
message_spell_free(MessageSpell *message_spell)
{
	if( message_spell->word ) g_free( message_spell->word );
	if(message_spell) g_free(message_spell );
}

static void
message_response_cb( GtkWidget *widget, gint response ){
	if(response != GTK_RESPONSE_OK) {
		gtk_widget_destroy(widget);
		return;
	}

	GtkTextBuffer  *buffer;
	GtkTextIter     start_iter;
	GtkTextIter     end_iter;

		
	debug(DEBUG_DOMAIN_SETUP, "Posting message to Twitter");
	
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(dialog_priv->textview));
	gtk_text_buffer_get_start_iter(buffer, &start_iter);
	gtk_text_buffer_get_end_iter(buffer, &end_iter);
	
	if(gtk_text_iter_equal(&start_iter, &end_iter)) {
		gtk_widget_destroy(widget);
		return;
	}
	gchar          *text;
	gchar          *good_msg;
		
	text = gtk_text_buffer_get_text(buffer, &start_iter, &end_iter, TRUE);
	good_msg=url_encode_message(text);
	g_free(text);
	
	if(!dialog_priv->show_friends) {
		/* Post a tweet */
		gtk_widget_destroy(widget);
		network_post_status(good_msg);
		if(good_msg) g_free(good_msg);
		return;
	}
	
	GtkTreeIter   iter;
	gchar        *to_user;
	GtkComboBox  *combo = GTK_COMBO_BOX(dialog_priv->friends_combo);
	GtkTreeModel *model = gtk_combo_box_get_model(combo);
	/* Send a direct message  */
	if(gtk_combo_box_get_active_iter(combo, &iter)){
		/* Get friend username */
		gtk_tree_model_get(model,
					&iter,
					0, &to_user,
					-1);
		/* Send the message */
		network_send_message(to_user, good_msg);
		if(to_user) g_free(to_user);
	}
	if( good_msg ) g_free(good_msg);
	gtk_widget_destroy(widget);
}

static void message_destroy_cb( GtkWidget *widget ){
	/* Add any clean-up code here */
	gtk_text_buffer_end_user_action( undo_buffer );	
	undo_buffer=NULL;
	g_object_unref(dialog);
	dialog=NULL;
}
