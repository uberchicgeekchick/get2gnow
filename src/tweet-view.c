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



/********************************************************
 *        Project headers, eg #include "config.h"       *
 ********************************************************/
#include <glib.h>

#include "config.h"
#include "gconfig.h"
#include "main.h"
#include "program.h"

#include "network.h"
#include "gtkbuilder.h"
#include "tweets.h"
#include "app.h"
#include "geometry.h"
#include "preferences.h"
#include "tweet-view.h"
#include "popup-dialog.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define	DEBUG_DOMAINS	"TweetView:UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Tweets:Requests:Users:Start-Up"
#include "debug.h"

#define GtkBuilderUI "tweet-view.ui"
#define	TWEET_MAX_CHARS	140

static TweetView *tweet_view=NULL;

typedef enum{
	USER_LABEL,
	USER_POINTER,
} FriendsDMColumns;

/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static void tweet_view_destroy_cb(GtkWidget *window, TweetView *tweet_view); 
static gboolean tweet_view_delete_event_cb(GtkWidget *window, GdkEvent *event, TweetView *tweet_view);
static gboolean tweet_view_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, TweetView *tweet_view);
static gboolean tweet_view_configure_event_timeout_cb(GtkWidget *widget);

static void tweet_view_dm_show(GtkToggleButton *toggle_button);
static void tweet_view_dm_form_activate(gboolean dm_activate);
static void tweet_view_dm_refresh(void);

static void tweet_view_sexy_init(void);
static void tweet_view_reorder(void);

static void tweet_view_selected_tweet_buttons_setup(GtkBuilder *ui);
static void tweet_view_selected_tweet_buttons_show(gboolean show);

static void tweet_view_count_tweet_char(GtkEntry *entry, GdkEventKey *event, GtkLabel *tweet_character_counter);
static void tweet_view_sexy_send(gpointer service, gpointer user_data);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
static void tweet_view_destroy_cb(GtkWidget *window, TweetView *tweet_view){
	debug("Destroying Tweet View & freeing resources");
	gtk_widget_destroy( GTK_WIDGET(tweet_view->tweet_view) );
	g_free(tweet_view);
}/*tweet_view_destroy_cb*/

static gboolean tweet_view_delete_event_cb(GtkWidget *window, GdkEvent *event, TweetView *tweet_view){
	gtk_toggle_button_set_active(tweet_view->embed_togglebutton, FALSE);
	
	return TRUE;
}/*tweet_view_delete_event_cb*/

static gboolean tweet_view_configure_event_timeout_cb(GtkWidget *widget){
	if(!gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE))
		return FALSE;
	
	geometry_save();
	
	tweet_view->size_timeout_id=0;
	
	return FALSE;
}

static gboolean tweet_view_configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, TweetView *tweet_view){
	program_timeout_remove(&tweet_view->size_timeout_id, _("main window configuration"));
	
	tweet_view->size_timeout_id=g_timeout_add(500, (GSourceFunc) tweet_view_configure_event_timeout_cb, widget );
	
	return FALSE;
}

TweetView *tweet_view_new(GtkWindow *parent){
	GtkBuilder	*ui;
	
	tweet_view=g_new0(TweetView, 1);
	debug("Building Tweet View");
	ui=gtkbuilder_get_file( GtkBuilderUI,
				"tweet_view", &tweet_view->tweet_view,
				"tweet_view_embed", &tweet_view->tweet_view_embed,
				
				"user_vbox", &tweet_view->user_vbox,
				"user_image", &tweet_view->user_image,
				"view_user_profile_button", &tweet_view->view_user_profile_button,
				"view_user_tweets_button", &tweet_view->view_user_tweets_button,
				"user_follow_button", &tweet_view->user_follow_button,
				"user_unfollow_button", &tweet_view->user_unfollow_button,
				"user_block_button", &tweet_view->user_block_button,
				
				"status_view_vbox", &tweet_view->status_view_vbox,
				"tweet_datetime_label", &tweet_view->tweet_datetime_label,
				
				"status_compose_vbox", &tweet_view->status_compose_vbox,
				"char_count_hbox", &tweet_view->char_count_hbox,
				"char_count", &tweet_view->char_count,
				
				"tweet_hbox", &tweet_view->tweet_hbox,
				"sexy_send", &tweet_view->sexy_send,
				"sexy_dm", &tweet_view->sexy_dm,
				"sexy_vseparator", &tweet_view->sexy_vseparator,
				"new_tweet_button", &tweet_view->new_tweet_button,
				
				"dm_frame", &tweet_view->dm_frame,
				"dm_form_hbox", &tweet_view->dm_form_hbox,
				"dm_frame_label", &tweet_view->dm_frame_label,
				"dm_refresh", &tweet_view->dm_refresh,
				"followers_combo_box", &tweet_view->followers_combo_box,
				"followers_list_store", &tweet_view->followers_list_store,
				"followers_send_dm", &tweet_view->followers_send_dm,
				
				"dm_form_active_togglebutton", &tweet_view->dm_form_active_togglebutton,
				"dm_form_active_image", &tweet_view->dm_form_active_image,
				
				"tweet_view_embed_togglebutton", &tweet_view->embed_togglebutton,
				"tweet_view_embed_image", &tweet_view->embed_image,
				
				"reply_button", &tweet_view->reply_button, 
				"retweet_button", &tweet_view->retweet_button,
				"make_fave_button", &tweet_view->make_fave_button,
			NULL
	);
	tweet_view->followers_model=gtk_combo_box_get_model(tweet_view->followers_combo_box);
	
	debug("Building & setting up new Tweet entry area.");
	tweet_view_sexy_init();
	tweet_view_reorder();
	debug("TweetView view & entry area setup.  Grabbing selected widgets.");
	tweet_view_selected_tweet_buttons_setup(ui);
	
	/* Connect the signals */
	debug("TweetView interface created & setup.  Setting signal handlers.");
	g_signal_connect_after(tweet_view->tweet_view_embed, "key-press-event", G_CALLBACK(tweets_hotkey), NULL);
	gtkbuilder_connect( ui, tweet_view,
				"tweet_view", "destroy", tweet_view_destroy_cb,
				"tweet_view", "delete_event", tweet_view_delete_event_cb,
				"tweet_view", "configure_event", tweet_view_configure_event_cb,

				"view_user_profile_button", "clicked", tweets_user_view_profile,
				"view_user_tweets_button", "clicked", tweets_user_view_tweets,
				
				"user_follow_button", "clicked", tweets_user_follow,
				"user_unfollow_button", "clicked", tweets_user_unfollow,
				"user_block_button", "clicked", tweets_user_block,
				
				"followers_send_dm", "clicked", tweet_view_send,
				"sexy_send", "clicked", tweet_view_send,
				"sexy_dm", "clicked", tweet_view_send,
				"new_tweet_button", "clicked", tweets_new_tweet,
				
				"dm_refresh", "clicked", tweet_view_dm_refresh,
				"dm_form_active_togglebutton", "toggled", tweet_view_dm_show,
				
				"reply_button", "clicked", tweets_reply,
				"retweet_button", "clicked", tweets_retweet,
				"make_fave_button", "clicked", tweets_save_fave,
			NULL
	);
	
	g_signal_connect_after(tweet_view->embed_togglebutton, "toggled", (GCallback)app_tweet_view_set_embed, NULL);
	
	gchar *tweet_view_title=g_strdup_printf("%s - TweetView", _(GETTEXT_PACKAGE));
	gtk_window_set_title(tweet_view->tweet_view, tweet_view_title);
	g_free(tweet_view_title);
	
	if(!( parent && gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE) )){
		debug("TweetView's set to be embed, no further setup needed.");
	}else{
		debug("Displaying TweetView as a stand alone dialog & setting TweetView's parent window..");
		gtk_widget_show_all(GTK_WIDGET(tweet_view->tweet_view));
		g_object_add_weak_pointer(G_OBJECT(tweet_view->tweet_view), (gpointer)&tweet_view);
		gtk_window_set_transient_for(GTK_WINDOW(tweet_view->tweet_view), parent);
	}
	
	debug("Setting TweetView's embed state indicators.");
	tweet_view_set_embed_toggle_and_image();
	
	debug("Disabling 'selected widgets' since no tweet could be selected when we 1st start.");
	tweet_view_selected_tweet_buttons_show(FALSE);
	
	debug("Disabling & hiding tweet view's dm form since friends have not yet been loaded.");
	tweet_view_dm_form_activate(FALSE);
	
	unset_selected_tweet();
	
	return tweet_view;
}/*tweet_view_new*/

GtkWindow *tweet_view_get_window(void){
	return tweet_view->tweet_view;
}/*tweet_view_get_window*/

void tweet_view_set_embed_toggle_and_image(void){
	if(!gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE)){
		debug("Setting TweetView's embed state indicators to split Tweet View off into a floating window.");
		gtk_toggle_button_set_active(tweet_view->embed_togglebutton, FALSE);
		gtk_widget_set_tooltip_markup(GTK_WIDGET(tweet_view->embed_togglebutton), "<span weight=\"bold\">Split Tweet View into its own window.</span>");
		gtk_image_set_from_icon_name(tweet_view->embed_image, "gtk-goto-top", ImagesMinimum);
	}else{
		debug("Setting TweetView's embed state indicators to embed Tweet View into %s's main window.", PACKAGE_NAME);
		gtk_toggle_button_set_active(tweet_view->embed_togglebutton, TRUE);
		gchar *tooltip_markup=g_strdup_printf("<span weight=\"light\">Move Tweet View back into %s's main window.</span>", PACKAGE_NAME);
		gtk_widget_set_tooltip_markup(GTK_WIDGET(tweet_view->embed_togglebutton), tooltip_markup );
		g_free(tooltip_markup);
		gtk_image_set_from_icon_name(tweet_view->embed_image, "gtk-goto-bottom", ImagesMinimum);
	}
}/*tweet_view_set_embed_toggle_and_image*/

static void tweet_view_selected_tweet_buttons_setup(GtkBuilder *ui){
	const gchar *selected_tweet_buttons[]={
		"view_user_profile_button",
		"view_user_tweets_button",
		
		"user_follow_button",
		"user_unfollow_button",
		"user_block_button",
		
		"sexy_dm",
		"new_tweet_button",
		
		"reply_button",
		"retweet_button",
		"make_fave_button",
	};
	
	GList *list=NULL;
	for(int i=0; i < G_N_ELEMENTS(selected_tweet_buttons); i++)
		list=g_list_append(list, (gtk_builder_get_object(ui, selected_tweet_buttons[i])) );
	tweet_view->selected_tweet_buttons=list;
}/*tweet_view_selected_widgets_setup*/

static void tweet_view_selected_tweet_buttons_show(gboolean show){
	GList *l=NULL;
	for(l=tweet_view->selected_tweet_buttons; l; l=l->next)
		gtk_widget_set_sensitive( GTK_WIDGET(l->data), show );
}/*tweet_view_selected_widgets_show*/

static void tweet_view_dm_form_set_toggle_and_image(void){
	if(!gtk_toggle_button_get_active(tweet_view->dm_form_active_togglebutton)){
		debug("Setting TweetView's dm form toggle button to enable the DM form.");
		gtk_widget_set_tooltip_markup(GTK_WIDGET(tweet_view->dm_form_active_togglebutton), "<span weight=\"bold\">Compose DM's to any of your friends.</span>");
		gtk_image_set_from_icon_name(tweet_view->dm_form_active_image, "gtk-fullscreen", ImagesMinimum);
	}else{
		debug("Setting TweetView's dm form toggle button to disable the DM form.");
		gtk_widget_set_tooltip_markup(GTK_WIDGET(tweet_view->dm_form_active_togglebutton), "<span weight=\"light\">Hide the DM form &amp; area.  You'll still be able to send DMs in the future by clicking this button again.</span>");
		gtk_image_set_from_icon_name(tweet_view->dm_form_active_image, "gtk-close", ImagesMinimum);
	}
}/*tweet_view_dm_form_set_toggle_and_image*/

static void tweet_view_dm_form_activate(gboolean dm_activate){
	tweet_view_dm_form_set_toggle_and_image();
	
	if(!dm_activate){
		gtk_widget_hide( GTK_WIDGET(tweet_view->dm_frame) );
		gtk_widget_grab_focus(GTK_WIDGET(tweet_view->sexy_entry));
	}else{
		gtk_widget_show( GTK_WIDGET(tweet_view->dm_frame) );
		gtk_widget_grab_focus(GTK_WIDGET(tweet_view->followers_combo_box));
	}
}/*tweet_view_dm_form_activate*/

static void tweet_view_sexy_init(void){
	/* Set-up expand tweet view.  Used to view tweets in detailed & send tweets and DMs. */
	debug("Creating Tweet's service link area, 'tweet_view->sexy_to', using sexy label interface.");
	tweet_view->sexy_to=label_new();
	gtk_box_pack_start(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->sexy_to),
			FALSE, FALSE, 5
	);
	g_object_set( tweet_view->sexy_to, "yalign", 0.00, "xalign", 1.00, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL );
	gtk_widget_show(GTK_WIDGET(tweet_view->sexy_to));
	
	debug("Creating Tweet's title area, 'tweet_view->sexy_from', using sexy label interface.");
	tweet_view->sexy_from=label_new();
	gtk_box_pack_start(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->sexy_from),
			FALSE, FALSE, 5
	);
	
	debug("Aligning Tweet's view area, 'tweet_view->tweet_datetime_label',");
	g_object_set( tweet_view->tweet_datetime_label, "yalign", 0.00, "xalign", 1.00, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL );
	
	g_object_set( tweet_view->sexy_from, "yalign", 0.00, "xalign", 0.00, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL );
	gtk_widget_show(GTK_WIDGET(tweet_view->sexy_from));
	
	debug("Creating Tweet's view area, 'tweet_view->sexy_tweet', using sexy label interface.");
	tweet_view->sexy_tweet=label_new();
	gtk_box_pack_start(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->sexy_tweet),
			TRUE, TRUE, 5
	);
	g_object_set( tweet_view->sexy_tweet, "yalign", 0.00, "xalign", 0.00, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL );
	gtk_widget_show(GTK_WIDGET(tweet_view->sexy_tweet));
	
	debug("Creating Tweet's entry, 'tweet_view->sexy_entry', using sexy entry.");
	tweet_view->sexy_entry=(SexySpellEntry *)sexy_spell_entry_new();
	gtk_widget_show(GTK_WIDGET(tweet_view->sexy_entry));
	gtk_box_pack_start(
			GTK_BOX(tweet_view->tweet_hbox),
			GTK_WIDGET(tweet_view->sexy_entry),
			TRUE, TRUE, 0
	);
	
	debug("Reording Tweet's entry area.");
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->tweet_hbox),
			GTK_WIDGET(tweet_view->sexy_entry),
			1
	);
	
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->tweet_hbox),
			GTK_WIDGET(tweet_view->sexy_send),
			2
	);
	
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->tweet_hbox),
			GTK_WIDGET(tweet_view->sexy_dm),
			3
	);
	
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->tweet_hbox),
			GTK_WIDGET(tweet_view->sexy_vseparator),
			4
	);
	
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->tweet_hbox),
			GTK_WIDGET(tweet_view->new_tweet_button),
			5
	);
	
	g_signal_connect_after(tweet_view->sexy_entry, "key-press-event", G_CALLBACK(tweets_hotkey), NULL);
	g_signal_connect_after(tweet_view->sexy_entry, "key-release-event", G_CALLBACK(tweet_view_count_tweet_char), tweet_view->char_count);
	g_signal_connect(tweet_view->sexy_entry, "activate", G_CALLBACK(tweet_view_send), NULL);
	g_signal_connect_after(tweet_view->followers_combo_box, "changed", G_CALLBACK(tweet_view_sexy_select), tweet_view->followers_combo_box);
}/*tweet_view_sexy_init*/

static void tweet_view_reorder(void){
	debug("Ordering TweetView.");
	debug("Setting 'sexy_to' as 'status_view_vbox' 1st widget.");
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->sexy_to),
			0
	);
	
	debug("Setting 'sexy_from' as 'status_view_vbox' 2nd widget.");
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->sexy_from),
			1
	);
	
	debug("Setting 'tweet_datetime_label' as 'status_view_vbox' 3rd widget.");
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->tweet_datetime_label),
			2
	);
		
	debug("Setting 'sexy_tweet' as 'status_view_vbox' 4th widget.");
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->sexy_tweet),
			3
	);
}/*tweet_view_reorder*/


void tweet_view_show_tweet(OnlineService *service, unsigned long int id, unsigned long int user_id, const gchar *user_name, const gchar *user_nick, const gchar *date, const gchar *sexy_tweet, const gchar *text_tweet, GdkPixbuf *pixbuf){
	if(!id)
		unset_selected_tweet();
	else
		set_selected_tweet(service, id, user_id, user_name, text_tweet);
	
	debug("%sabling 'selected_tweet_buttons'.", (id ?"En" :"Dis") );
	tweet_view_selected_tweet_buttons_show( (id ?TRUE :FALSE ) );

	gchar *sexy_text=NULL;
	if(!id)
		sexy_text=g_strdup("");
	else
		sexy_text=g_strdup_printf("<span size=\"small\" weight=\"light\" variant=\"smallcaps\">To: [<a href=\"https://%s/%s\">%s</a>]</span>", service->uri, service->username, service->decoded_key);
	debug("Setting 'sexy_to' for 'selected_tweet':\n\t\t%s.", sexy_text);
	label_set_text(service, tweet_view->sexy_to, sexy_text, FALSE, TRUE);
	g_free(sexy_text);
	
	
	if(!( G_STR_EMPTY(user_nick) && G_STR_EMPTY(user_name) ))
		sexy_text=g_strdup_printf("<b>From: %s &lt; @%s on <a href=\"http://%s/%s\">%s</a> &gt;</b>", user_nick, user_name, service->uri, user_name, service->uri);
	else
		sexy_text=g_strdup("");
	debug("Setting 'sexy_from' for 'selected_tweet':\n\t\t%s.", sexy_text);
	label_set_text(service, tweet_view->sexy_from, sexy_text, FALSE, TRUE);
	g_free(sexy_text);
	
	if(!(G_STR_EMPTY(date)))
		sexy_text=g_markup_printf_escaped("<span style=\"italic\">[%s]</span>", date);
	else
		sexy_text=g_strdup("");
	gtk_label_set_markup(tweet_view->tweet_datetime_label, sexy_text);
	g_free(sexy_text);
	
	
	/*
	 * gchar sexy_tweet has already been formatted to include urls, hyperlinks, titles, & etc.
	 * So we just set it as a SexyLable & bypass Label
	 */
	debug("Setting 'sexy_tweet' for 'selected_tweet':\n\t\t%s.", sexy_tweet);
	if(!(gconfig_if_bool(PREFS_URLS_EXPAND_SELECTED_ONLY, TRUE)))
		sexy_url_label_set_markup(SEXY_URL_LABEL(tweet_view->sexy_tweet), sexy_tweet);
	else
		label_set_text(service, tweet_view->sexy_tweet, sexy_tweet, TRUE, TRUE);
	
	if(!pixbuf)
		gtk_image_set_from_icon_name(tweet_view->user_image, PACKAGE_NAME, ImagesExpanded);
	else{
		debug("Setting avatar for the user who wrote the 'selected_tweet'.");
		GdkPixbuf *resized=NULL;
		resized=images_expand_pixbuf( pixbuf );
		gtk_image_set_from_pixbuf(tweet_view->user_image, resized);
		if(resized)
			g_object_unref(resized);
	}
	
	debug("Selecting 'sexy_entry' for entering a new tweet.");
	tweet_view_sexy_select();
}/*tweet_view_show_tweet*/

static gshort tweetlen(gchar *tweet){
	gushort character_count=0;
	while(*tweet){
		unsigned char l=*tweet++;
		if(l=='<' || l=='>')
			character_count+=3;
		character_count++;
	}
	
	return TWEET_MAX_CHARS-character_count;
}/*tweetlen*/

static void tweet_view_count_tweet_char(GtkEntry *entry, GdkEventKey *event, GtkLabel *tweet_character_counter){
	gshort character_count=tweetlen(entry->text);
	gchar *remaining_characters=NULL;
	if(character_count < 0){
		if(!gconfig_if_bool(PREFS_TWEET_LENGTH_ALERT, FALSE))
			tweet_view_beep();
		remaining_characters=g_markup_printf_escaped("<span size=\"small\" foreground=\"red\">%i</span>", character_count);
	}else
		remaining_characters=g_markup_printf_escaped("<span size=\"small\" foreground=\"green\">%i</span>", character_count);
	
	gtk_label_set_markup( tweet_character_counter, remaining_characters );
	g_free(remaining_characters);
}/*tweet_view_count_tweet_char*/

void tweet_view_beep(void){
	gtk_widget_error_bell(GTK_WIDGET(tweet_view->sexy_entry));
}/*tweet_view_beep*/

void tweet_view_sexy_select(void){
	gtk_widget_grab_focus(GTK_WIDGET(tweet_view->sexy_entry));
	gtk_entry_set_position(GTK_ENTRY(tweet_view->sexy_entry), -1 );
}/*tweet_view_sexy_select*/

void tweet_view_sexy_prefix_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	tweet_view_sexy_prefix_string((const gchar *)str);
	g_free(str);
}/*tweet_view_sexy_prefix_char*/

void tweet_view_sexy_prefix_string(const gchar *str){
	tweet_view_sexy_puts(str, 0);
}/*tweet_view_sexy_prefix_string*/

void tweet_view_sexy_set(gchar *tweet){
	gtk_entry_set_text(GTK_ENTRY(tweet_view->sexy_entry), tweet);
	tweet_view_sexy_select();
}/*tweet_view_sexy_set*/

void tweet_view_sexy_insert_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	tweet_view_sexy_insert_string((const gchar *)str);
	g_free(str);
}/*tweet_view_sexy_insert_char*/

void tweet_view_sexy_insert_string(const gchar *str){
	tweet_view_sexy_puts(str, gtk_editable_get_position(GTK_EDITABLE(tweet_view->sexy_entry)) );
}/*tweet_view_sexy_insert_string*/

void tweet_view_sexy_append_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	tweet_view_sexy_append_string((const gchar *)str);
	g_free(str);
}/*tweet_view_sexy_append_char*/

void tweet_view_sexy_append_string(const gchar *str){
	tweet_view_sexy_puts(str, (gint)gtk_entry_get_text_length(GTK_ENTRY(tweet_view->sexy_entry)) );
}/*tweet_view_sexy_append_string*/

gint tweet_view_sexy_puts(const gchar *str, gint position){
	gtk_editable_insert_text(GTK_EDITABLE(tweet_view->sexy_entry), str, -1, &position );
	gtk_entry_set_position(GTK_ENTRY(tweet_view->sexy_entry), position );
	tweet_view_sexy_select();
	return position;
}/*tweet_view_sexy_puts*/

void tweet_view_send(GtkWidget *activated_widget){
	gchar *user_name=NULL;
	
	gchar *text=GTK_ENTRY(tweet_view->sexy_entry)->text;
	if(G_STR_EMPTY(text)){
		gchar *reply_to_string=selected_tweet_reply_to_strdup();
		if(!selected_tweet_get_user_name())
			gtk_widget_error_bell(GTK_WIDGET(activated_widget));
		else if(g_str_equal(text, reply_to_string))
			gtk_widget_error_bell(GTK_WIDGET(activated_widget));
		else
			tweets_reply();
		uber_free(reply_to_string);
		return;
	}
	
	if( activated_widget==GTK_WIDGET(tweet_view->sexy_dm) )
		tweet_view_sexy_send(selected_tweet_get_service(), selected_tweet_get_user_name() );
	
	else if( (activated_widget==GTK_WIDGET(tweet_view->followers_send_dm)) && (GTK_WIDGET_IS_SENSITIVE(tweet_view->followers_combo_box)) && (user_name=gtk_combo_box_get_active_text(tweet_view->followers_combo_box)) ){
		g_free(user_name);
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		User *user=NULL;
		gtk_combo_box_get_active_iter(tweet_view->followers_combo_box, iter);
		gtk_tree_model_get(
					tweet_view->followers_model,
					iter,
						USER_POINTER, &user,
					-1
		);
		tweet_view_sexy_send(user->service, user->user_name);
		g_free(iter);
		gtk_combo_box_set_active(tweet_view->followers_combo_box, 0);
	}else
		tweet_view_sexy_send(NULL, NULL);
}/*tweet_view_send*/
	
static void tweet_view_sexy_send(gpointer service, gpointer user_data){
	if(!( (GTK_ENTRY(tweet_view->sexy_entry)->text) && (tweetlen(GTK_ENTRY(tweet_view->sexy_entry)->text) <= TWEET_MAX_CHARS) )){
		if(!gconfig_if_bool(PREFS_TWEET_LENGTH_ALERT, FALSE))
			gtk_widget_error_bell(GTK_WIDGET(tweet_view->sexy_entry));
		return;
	}
	
	gchar *tweet=g_uri_escape_string(GTK_ENTRY(tweet_view->sexy_entry)->text, NULL, TRUE);
	const gchar *user_name=(const gchar *)user_data;
	if(G_STR_EMPTY(user_name))
		network_post_status(tweet);
	else
		network_send_message((OnlineService *)service, user_name, (const gchar *)tweet);
	
	g_free(tweet);
	tweet_view_sexy_set((gchar *)"");
}/*tweet_view_sexy_send*/

void tweet_view_new_dm(void){
	gtk_toggle_button_set_active(tweet_view->dm_form_active_togglebutton, !gtk_toggle_button_get_active(tweet_view->dm_form_active_togglebutton));
}/*tweet_view_new_dm*/

static void tweet_view_dm_show(GtkToggleButton *toggle_button){
	if(!gtk_toggle_button_get_active(tweet_view->dm_form_active_togglebutton)){
		debug("Disabled TweetView's dm form.");
		tweet_view_dm_form_activate(FALSE);
		return;
	}
	popup_select_service( (gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE) ?tweet_view->tweet_view :app_get_window()) );
	
	if(!(selected_service)) {
		if(gtk_toggle_button_get_active(tweet_view->dm_form_active_togglebutton))
			gtk_toggle_button_set_active(tweet_view->dm_form_active_togglebutton, FALSE);
		return;
	}
	
	debug("Enabling TweetView's dm form.");
	tweet_view_dm_form_activate(TRUE);
	network_users_glist_get(GetFollowers, FALSE, tweet_view_dm_data_fill);
}/*tweet_view_dm_show*/

static void tweet_view_dm_refresh(void){
	popup_select_service( (gconfig_if_bool(PREFS_TWEET_VIEW_DIALOG, FALSE) ?tweet_view->tweet_view :app_get_window()) );
	network_users_glist_get(GetFollowers, TRUE, tweet_view_dm_data_fill);
}/*tweet_view_dm_refresh*/

void tweet_view_dm_data_fill(GList *followers){
	if(!(followers)) return;
	
	GList		*list;
	User		*user;
	gchar		*null_friend=g_strdup("");
	GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
	
	gchar *new_label=NULL;
	
	gtk_list_store_clear(tweet_view->followers_list_store);
	
	gtk_list_store_append(tweet_view->followers_list_store, iter);
	gtk_list_store_set(
				tweet_view->followers_list_store, iter,
					USER_LABEL, null_friend,
					USER_POINTER, NULL,
				-1
	);
	g_free(iter);
	
	for(list=followers; list; list=list->next) {
		user=(User *)list->data;
		if(!new_label){
			new_label=g_markup_printf_escaped("<b>_DM one of your, &lt;%s&gt;, followers:</b>", user->service->decoded_key);
			gtk_label_set_markup(tweet_view->dm_frame_label, new_label);
			gtk_label_set_use_underline(tweet_view->dm_frame_label, TRUE);
			gtk_label_set_single_line_mode(tweet_view->dm_frame_label, TRUE);
		}
		gchar *user_label=g_strdup_printf("%s <%s>", user->user_name, user->nick_name);
		iter=g_new0(GtkTreeIter, 1);
		gtk_list_store_append(tweet_view->followers_list_store, iter);
		gtk_list_store_set(
				tweet_view->followers_list_store,
				iter,
					USER_LABEL, user_label,
					USER_POINTER, user,
				-1
		);
		g_free(user_label);
		g_free(iter);
		iter=NULL;
	}
	if(new_label) uber_free(new_label);
}/*tweet_view_dm_data_fill*/

/********************************************************
 *                       eof                            *
 ********************************************************/
