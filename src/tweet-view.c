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
#include "debug.h"
#include "network.h"
#include "gtkbuilder.h"
#include "tweets.h"
#include "app.h"
#include "tweet-view.h"
#include "preferences.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define DEBUG_DOMAIN "Tweets"
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
static void tweet_view_sexy_init(void);
static void tweet_view_reorder(void);

static void tweet_view_tweet_selected_buttons_setup(GtkBuilder *ui);
static void tweet_view_tweet_selected_buttons_show(gboolean show);

static void tweet_view_count_tweet_char(GtkEntry *entry, GdkEventKey *event, GtkLabel *tweet_character_counter);
static void tweet_view_sexy_send(gpointer service, gpointer user_data);

static void tweet_view_dm_data_fill(GList *followers);
static void tweet_view_dm_data_set_sensitivity(GtkButton *button);
static void tweet_view_dm_data_show(void);
static void tweet_view_dm_data_hide(void);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
TweetView *tweet_view_new(GtkWindow *parent){
	GtkBuilder	*ui;
	
	tweet_view=g_new0(TweetView, 1);
	debug(DEBUG_DOMAIN, "Building Tweet View");
	ui=gtkbuilder_get_file( GtkBuilderUI,
				"tweet_view", &tweet_view->tweet_view,
				"tweet_view_embed", &tweet_view->tweet_view_embed,
				
				"user_vbox", &tweet_view->user_vbox,
				"user_image", &tweet_view->user_image,
				
				"status_view_vbox", &tweet_view->status_view_vbox,
				
				"view_user_profile_button", &tweet_view->view_user_profile_button,
				"view_user_tweets_button", &tweet_view->view_user_tweets_button,
				
				"user_follow_button", &tweet_view->user_follow_button,
				"user_unfollow_button", &tweet_view->user_unfollow_button,
				"user_block_button", &tweet_view->user_block_button,
				
				"char_count_hbox", &tweet_view->char_count_hbox,
				"char_count", &tweet_view->char_count,
				
				"tweet_hbox", &tweet_view->tweet_hbox,
				"sexy_send", &tweet_view->sexy_send,
				"sexy_dm", &tweet_view->sexy_dm,
				
				"dm_frame", &tweet_view->dm_frame,
				"dm_form_hbox", &tweet_view->dm_form_hbox,
				"dm_frame_label", &tweet_view->dm_frame_label,
				"dm_refresh", &tweet_view->dm_refresh,
				"friends_combo_box", &tweet_view->friends_combo_box,
				"friends_liststore", &tweet_view->friends_liststore,
				"friends_send_dm", &tweet_view->friends_send_dm,
				"dm_form_hide", &tweet_view->dm_form_hide,
				"dm_form_show", &tweet_view->dm_form_show,
				
				"reply_button", &tweet_view->reply_button, 
				"retweet_button", &tweet_view->retweet_button,
				"make_fave_button", &tweet_view->make_fave_button,
			NULL
	);
	tweet_view->friends_model=gtk_combo_box_get_model(tweet_view->friends_combo_box);
	
	debug(DEBUG_DOMAIN, "Building & setting up new Tweet entry area.");
	tweet_view_sexy_init();
	tweet_view_reorder();
	debug(DEBUG_DOMAIN, "TweetView view & entry area setup.  Grabbing selected widgets.");
	tweet_view_tweet_selected_buttons_setup(ui);
	
	/* Connect the signals */
	debug(DEBUG_DOMAIN, "TweetView interface created & setup.  Setting signal handlers.");
	g_signal_connect_after(tweet_view->tweet_view_embed, "key-press-event", G_CALLBACK(tweets_hotkey), NULL);
	gtkbuilder_connect( ui, tweet_view,
				"view_user_profile_button", "clicked", tweets_user_view_profile,
				"view_user_tweets_button", "clicked", tweets_user_view_tweets,
				
				"user_follow_button", "clicked", tweets_user_follow,
				"user_unfollow_button", "clicked", tweets_user_unfollow,
				"user_block_button", "clicked", tweets_user_block,
				
				"friends_send_dm", "clicked", tweet_view_send,
				"sexy_send", "clicked", tweet_view_send,
				"sexy_dm", "clicked", tweet_view_send,
				
				"dm_form_hide", "clicked", tweet_view_dm_data_set_sensitivity,
				"dm_form_show", "clicked", tweet_view_new_dm,
				
				"reply_button", "clicked", tweets_reply,
				"retweet_button", "clicked", tweets_retweet,
				"make_fave_button", "clicked", tweets_save_fave,
			NULL
	);
	
	/*
	GtkCellRenderer *renderer=gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(tweet_view->friends_combo_box), renderer, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(tweet_view->friends_combo_box), renderer, "text", 0, NULL);
	*/
	debug(DEBUG_DOMAIN, "Disabling 'selected widgets' since no tweet could be selected when we 1st start.");
	
	if(!( parent && gconfig_if_bool(PREFS_UI_USE_TWEET_DIALOG) )){
		debug(DEBUG_DOMAIN, "TweetView's set to be embed, no further setup needed.");
	}else{
		debug(DEBUG_DOMAIN, "Displaying TweetView as a stand alone dialog & setting TweetView's parent window..");
		gtk_widget_show_all(GTK_WIDGET(tweet_view->tweet_view));
		g_object_add_weak_pointer(G_OBJECT(tweet_view->tweet_view),(gpointer) &tweet_view);
		gtk_window_set_transient_for(GTK_WINDOW(tweet_view->tweet_view), parent);
	}
	
	tweet_view_tweet_selected_buttons_show(FALSE);
	tweet_view_dm_data_set_sensitivity(tweet_view->dm_form_hide);
	
	unset_selected_tweet();
	
	return tweet_view;
}

static void tweet_view_sexy_init(void){
	/* Set-up expand tweet view.  Used to view tweets in detailed & send tweets and DMs. */
	debug(DEBUG_DOMAIN, "Creating Tweet's title area, 'tweet_view->sexy_title', using sexy label interface.");
	tweet_view->sexy_title=label_new();
	gtk_widget_show(GTK_WIDGET(tweet_view->sexy_title));
	gtk_box_pack_start(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->sexy_title),
			TRUE, TRUE, 0
	);
	
	debug(DEBUG_DOMAIN, "Creating Tweet's view area, 'tweet_view->sexy_tweet', using sexy label interface.");
	tweet_view->sexy_tweet=label_new();
	gtk_widget_show(GTK_WIDGET(tweet_view->sexy_tweet));
	gtk_box_pack_start(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->sexy_tweet),
			TRUE, TRUE, 0
	);
	
	debug(DEBUG_DOMAIN, "Creating Tweet's service link area, 'tweet_view->sexy_service', using sexy label interface.");
	tweet_view->sexy_service=label_new();
	g_object_set( tweet_view->sexy_service, "yalign", 1.0, "xalign", 1.0, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL );
	gtk_widget_show(GTK_WIDGET(tweet_view->sexy_service));
	gtk_box_pack_start(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->sexy_service),
			TRUE, TRUE, 0
	);
	
	debug(DEBUG_DOMAIN, "Creating Tweet's entry, 'tweet_view->sexy_entry', using sexy entry.");
	tweet_view->sexy_entry=(SexySpellEntry *)sexy_spell_entry_new();
	gtk_widget_show(GTK_WIDGET(tweet_view->sexy_entry));
	gtk_box_pack_start(
			GTK_BOX(tweet_view->tweet_hbox),
			GTK_WIDGET(tweet_view->sexy_entry),
			TRUE, TRUE, 0
	);
	
	debug(DEBUG_DOMAIN, "Reording Tweet's entry area.");
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
	
	g_signal_connect_after(tweet_view->sexy_entry, "key-press-event", G_CALLBACK(tweets_hotkey), NULL);
	g_signal_connect_after(tweet_view->sexy_entry, "key-release-event", G_CALLBACK(tweet_view_count_tweet_char), tweet_view->char_count);
	g_signal_connect(tweet_view->sexy_entry, "activate", G_CALLBACK(tweet_view_send), NULL);
	g_signal_connect_after(tweet_view->friends_combo_box, "changed", G_CALLBACK(tweet_view_sexy_select), tweet_view->friends_combo_box);
}//tweet_view_sexy_init

static void tweet_view_tweet_selected_buttons_setup(GtkBuilder *ui){
	const gchar *tweet_selected_buttons[]={
		"view_user_profile_button",
		"view_user_tweets_button",
		
		"user_follow_button",
		"user_unfollow_button",
		"user_block_button",
		
		"sexy_dm",
		
		"reply_button",
		"retweet_button",
		"make_fave_button",
	};
	
	GList *list=NULL;
	for(int i=0; i < G_N_ELEMENTS(tweet_selected_buttons); i++)
		list=g_list_append(list,(gtk_builder_get_object(ui, tweet_selected_buttons[i])) );
	tweet_view->tweet_selected_buttons=list;
}//tweet_view_selected_widgets_setup

static void tweet_view_tweet_selected_buttons_show(gboolean show){
	GList *l=NULL;
	for(l=tweet_view->tweet_selected_buttons; l; l=l->next)
		gtk_widget_set_sensitive( GTK_WIDGET(l->data), show );
}//tweet_view_selected_widgets_show

static void tweet_view_reorder(void){
	debug(DEBUG_DOMAIN, "Reording TweetView.");
	debug(DEBUG_DOMAIN, "Setting 'sexy_title' as 'status_view_vbox' 1st widget.");
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->sexy_title),
			0
	);
	
	debug(DEBUG_DOMAIN, "Setting 'sexy_tweet' as 'status_view_vbox' 2nd widget.");
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->sexy_tweet),
			1
	);
	
	debug(DEBUG_DOMAIN, "Setting 'sexy_title' as 'status_view_vbox' 3rd widget.");
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->sexy_service),
			2
	);
					
	debug(DEBUG_DOMAIN, "Setting 'char_count_hbox' as 'status_view_vbox' 4th widget.");
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->char_count_hbox),
			3
	);
	
	debug(DEBUG_DOMAIN, "Setting 'tweet_hbox' as 'status_view_vbox' 5th widget.");
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->tweet_hbox),
			4
	);
	
	debug(DEBUG_DOMAIN, "Setting 'dm_frame' as 'status_view_vbox' 6th widget.");
	gtk_box_reorder_child(
			GTK_BOX(tweet_view->status_view_vbox),
			GTK_WIDGET(tweet_view->dm_frame),
			5
	);
}//tweet_view_reorder


void tweet_view_show_tweet(OnlineService *service, unsigned long int id, unsigned long int user_id, const gchar *user_name, const gchar *user_nick, const gchar *date, const gchar *tweet, GdkPixbuf *pixbuf){
	if(!id)
		unset_selected_tweet();
	else
		set_selected_tweet(service, id, user_id, user_name, tweet);
	
	debug(DEBUG_DOMAIN, "%sabling 'selected_tweet_buttons'.", (id ?"En" :"Dis") );
	tweet_view_tweet_selected_buttons_show( (id ?TRUE :FALSE ) );

	debug(DEBUG_DOMAIN, "Setting 'sexy_service' to 'sexy_service_text' of 'selected_tweet'.");
	gchar *sexy_service_text=NULL;
	if(!id)
		sexy_service_text=g_strdup("");
	else
		sexy_service_text=g_strdup_printf("<span size=\"small\" weight=\"light\" variant=\"smallcaps\">from: [<a href=\"https://%s/\">%s</a>]</span>", service->url, service->decoded_key);
	label_set_text(tweet_view->sexy_service, sexy_service_text);
	
	debug(DEBUG_DOMAIN, "Setting 'title_text' of 'selected_tweet'.");
	gchar *title_text=NULL;
	if(!( G_STR_EMPTY(user_nick) && G_STR_EMPTY(user_name) && G_STR_EMPTY(date) ))
		title_text=g_strdup_printf("<b>%s( @%s )</b> - %s", user_nick, user_name, date);
	else
		title_text=g_strdup("");
	label_set_text(tweet_view->sexy_title, title_text);
	
	debug(DEBUG_DOMAIN, "Setting 'tweet' of 'selected_tweet'.");
	label_set_text(tweet_view->sexy_tweet, tweet);
	g_free(title_text);
	
	if(!pixbuf)
		gtk_image_set_from_icon_name(tweet_view->user_image, PACKAGE_NAME, ImagesExpanded);
	else{
		debug(DEBUG_DOMAIN, "Setting avatar for the user who wrote the 'selected_tweet'.");
		GdkPixbuf *resized=NULL;
		resized=images_expand_pixbuf( pixbuf );
		gtk_image_set_from_pixbuf(tweet_view->user_image, resized);
		if(resized)
			g_object_unref(resized);
	}
	
	debug(DEBUG_DOMAIN, "Selecting 'sexy_entry' for entering a new tweet.");
	tweet_view_sexy_select();
}//tweet_view_show_tweet

static gshort tweetlen(gchar *tweet){
	gushort character_count=0;
	while(*tweet){
		unsigned char l=*tweet++;
		if(l=='<' || l=='>')
			character_count+=3;
		character_count++;
	}
	
	return TWEET_MAX_CHARS-character_count;
}//tweetlen

static void tweet_view_count_tweet_char(GtkEntry *entry, GdkEventKey *event, GtkLabel *tweet_character_counter){
	gshort character_count=tweetlen(entry->text);
	gchar *remaining_characters=NULL;
	if(character_count < 0){
		if(!gconfig_if_bool(PREFS_UI_NO_ALERT))
			gtk_widget_error_bell(GTK_WIDGET(entry));
		remaining_characters=g_markup_printf_escaped("<span size=\"small\" foreground=\"red\">%i</span>", character_count);
	}else
		remaining_characters=g_markup_printf_escaped("<span size=\"small\" foreground=\"green\">%i</span>", character_count);
	
	gtk_label_set_markup( tweet_character_counter, remaining_characters );
	g_free(remaining_characters);
}//tweet_view_count_tweet_char

void tweet_view_sexy_select(void){
	gtk_widget_grab_focus(GTK_WIDGET(tweet_view->sexy_entry));
	gtk_entry_set_position(GTK_ENTRY(tweet_view->sexy_entry), -1 );
}//tweet_view_sexy_select

void tweet_view_sexy_prefix_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	tweet_view_sexy_prefix_string((const gchar *)str);
	g_free(str);
}//tweet_view_sexy_prefix_char

void tweet_view_sexy_prefix_string(const gchar *str){
	tweet_view_sexy_puts(str, 0);
}//tweet_view_sexy_prefix_string

void tweet_view_sexy_set(gchar *tweet){
	gtk_entry_set_text(GTK_ENTRY(tweet_view->sexy_entry), tweet);
	tweet_view_sexy_select();
}//tweet_view_sexy_set

void tweet_view_sexy_insert_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	tweet_view_sexy_insert_string((const gchar *)str);
	g_free(str);
}//tweet_view_sexy_insert_char

void tweet_view_sexy_insert_string(const gchar *str){
	tweet_view_sexy_puts(str, gtk_editable_get_position(GTK_EDITABLE(tweet_view->sexy_entry)) );
}//tweet_view_sexy_insert_string

void tweet_view_sexy_append_char(const char c){
	gchar *str=g_strdup_printf("%c", c);
	tweet_view_sexy_append_string((const gchar *)str);
	g_free(str);
}//tweet_view_sexy_append_char

void tweet_view_sexy_append_string(const gchar *str){
	tweet_view_sexy_puts(str,(gint)gtk_entry_get_text_length(GTK_ENTRY(tweet_view->sexy_entry)) );
}//tweet_view_sexy_append_string

gint tweet_view_sexy_puts(const gchar *str, gint position){
	gtk_editable_insert_text(GTK_EDITABLE(tweet_view->sexy_entry), str, -1, &position );
	gtk_entry_set_position(GTK_ENTRY(tweet_view->sexy_entry), position );
	tweet_view_sexy_select();
	return position;
}//tweet_view_sexy_puts

void tweet_view_send(GtkWidget *activated_widget){
	gchar *user_name=NULL;
	
	gchar *text=GTK_ENTRY(tweet_view->sexy_entry)->text;
	if(G_STR_EMPTY(text)){
		if(!selected_tweet_get_user_name())
			gtk_widget_error_bell(GTK_WIDGET(activated_widget));
		else if(g_str_equal(text, selected_tweet_get_reply_to_string()))
			gtk_widget_error_bell(GTK_WIDGET(activated_widget));
		else
			tweets_reply();
		return;
	}
	
	if( activated_widget==GTK_WIDGET(tweet_view->sexy_dm) )
		tweet_view_sexy_send(selected_tweet_get_service(), selected_tweet_get_user_name() );
	
	else if( (activated_widget==GTK_WIDGET(tweet_view->friends_send_dm)) && (GTK_WIDGET_IS_SENSITIVE(tweet_view->friends_combo_box)) && (user_name=gtk_combo_box_get_active_text(tweet_view->friends_combo_box)) ){
		g_free(user_name);
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		User *user=NULL;
		gtk_combo_box_get_active_iter(tweet_view->friends_combo_box, iter);
		gtk_tree_model_get(
					tweet_view->friends_model,
					iter,
						USER_POINTER, &user,
					-1
		);
		tweet_view_sexy_send(user->service, user->user_name);
		g_free(iter);
		gtk_combo_box_set_active(tweet_view->friends_combo_box, 0);
	}else
		tweet_view_sexy_send(NULL, NULL);
}//tweet_view_send
	
static void tweet_view_sexy_send(gpointer service, gpointer user_data){
	if(!( (GTK_ENTRY(tweet_view->sexy_entry)->text) && (tweetlen(GTK_ENTRY(tweet_view->sexy_entry)->text) <= TWEET_MAX_CHARS) )){
		if(!gconfig_if_bool(PREFS_UI_NO_ALERT))
			gtk_widget_error_bell(GTK_WIDGET(tweet_view->sexy_entry));
		return;
	}
	
	gchar *tweet=url_encode(GTK_ENTRY(tweet_view->sexy_entry)->text);
	const gchar *user_name=(const gchar *)user_data;
	if(G_STR_EMPTY(user_name))
		network_post_status(tweet);
	else
		network_send_message((OnlineService *)service, user_name, (const gchar *)tweet);
	
	g_free(tweet);
	tweet_view_sexy_set((gchar *)"");
}//tweet_view_sexy_send

void tweet_view_new_dm(void){
	tweet_view_dm_data_set_sensitivity(tweet_view->dm_form_show);
	tweet_view_dm_data_fill( user_get_followers(FALSE) );
}//tweets_new_dm

void tweet_view_dm_data_fill(GList *followers){
	GList		*list;
	User		*user;
	GtkListStore	*model_followers;
	gchar		*null_friend=g_strdup("");
	GtkTreeIter	*iter=g_new0(GtkTreeIter, 1);
	
	model_followers=GTK_LIST_STORE( gtk_combo_box_get_model( tweet_view->friends_combo_box ) );
	gtk_list_store_append(model_followers, iter);
	gtk_list_store_set(
				model_followers, iter,
					USER_LABEL, null_friend,
					USER_POINTER, NULL,
				-1
	);
	for(list=followers; list; list=list->next) {
		user=(User *)list->data;
		gchar *user_label=g_strdup_printf("%s from %s", user->user_name, user->service->decoded_key);
		gtk_list_store_append(model_followers, iter);
		gtk_list_store_set(
				model_followers,
				iter,
					USER_LABEL, user_label,
					USER_POINTER, user,
				-1
		);
		g_free(user_label);
		g_free(iter);
		iter=NULL;
	}
}//tweet_view_dm_data_fill

static void tweet_view_dm_data_set_sensitivity(GtkButton *button){
	gboolean dm_activate=( button==tweet_view->dm_form_show ?TRUE :FALSE );
	gtk_widget_set_sensitive( GTK_WIDGET(tweet_view->dm_form_hbox), dm_activate );
	gtk_widget_set_sensitive( GTK_WIDGET(tweet_view->friends_combo_box), dm_activate );
	gtk_widget_set_sensitive( GTK_WIDGET(tweet_view->dm_frame_label), dm_activate );
	gtk_widget_set_sensitive( GTK_WIDGET(tweet_view->friends_send_dm), dm_activate );
	gtk_widget_set_sensitive( GTK_WIDGET(tweet_view->dm_form_hide), dm_activate );
	gtk_widget_set_sensitive( GTK_WIDGET(tweet_view->dm_form_show), !dm_activate );
	return (dm_activate ?tweet_view_dm_data_show() :tweet_view_dm_data_hide() );
}//tweet_view_dm_data_set_sensitivity

static void tweet_view_dm_data_show(void){
	gtk_widget_show( GTK_WIDGET(tweet_view->dm_form_hbox) );
	gtk_widget_show( GTK_WIDGET(tweet_view->friends_combo_box) );
	gtk_widget_show( GTK_WIDGET(tweet_view->dm_frame_label) );
	gtk_widget_show( GTK_WIDGET(tweet_view->friends_send_dm) );
	gtk_widget_show( GTK_WIDGET(tweet_view->dm_form_hide) );
	gtk_widget_hide( GTK_WIDGET(tweet_view->dm_form_show) );
	gtk_widget_grab_focus(GTK_WIDGET(tweet_view->friends_combo_box));
}//tweet_view_dm_data_show

static void tweet_view_dm_data_hide(void){
	gtk_widget_hide( GTK_WIDGET(tweet_view->dm_form_hbox) );
	gtk_widget_hide( GTK_WIDGET(tweet_view->friends_combo_box) );
	gtk_widget_hide( GTK_WIDGET(tweet_view->dm_frame_label) );
	gtk_widget_hide( GTK_WIDGET(tweet_view->friends_send_dm) );
	gtk_widget_hide( GTK_WIDGET(tweet_view->dm_form_hide) );
	gtk_widget_show( GTK_WIDGET(tweet_view->dm_form_show) );
	gtk_widget_grab_focus(GTK_WIDGET(tweet_view->sexy_entry));
}//tweet_view_dm_data_hide

/********************************************************
 *                       eof                            *
 ********************************************************/
