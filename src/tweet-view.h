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
#ifndef __TWEET_VIEW_H__
#define __TWEET_VIEW_H__


/**********************************************************************
 *        System & library headers, eg #include <gdk/gdkkeysyms.h>    *
 **********************************************************************/
#include <glib.h>
#include <gtk/gtk.h>
#include <libsexy/sexy.h>
#include "label.h"


/*********************************************************************
 *        Objects, structures, and etc typedefs                      *
 *********************************************************************/
/* My, Kaity G. B., new uber tweet viewer. */
typedef struct {
	/* Used to trigger resizing. */
	guint			size_timeout_id;
	
	/* Tweet View Dialog & embeding Widgets */
	GtkWindow		*tweet_view;
	GtkHBox			*tweet_view_embed;
	
	/* Stuff for sending a dm to a selected user. */
	GtkVBox			*user_vbox;
	GtkImage		*user_image;
	
	/* View & Sending Tweets. */
	GtkVBox			*status_vbox;
	
	/* Tweet View */
	GtkVBox			*status_view_vbox;
	Label			*sexy_to;
	Label			*sexy_from;
	GtkLabel		*tweet_datetime_label;
	Label			*sexy_tweet;
	
	/* Tweet, status, & DM writing area & widgets. */
	GtkVBox			*status_compose_vbox;
	GtkHBox			*char_count_hbox;
	GtkLabel		*char_count;
	
	GtkHBox			*tweet_hbox;
	SexySpellEntry		*sexy_entry;
	GtkButton		*sexy_send;
	GtkButton		*sexy_dm;
	GtkVSeparator		*sexy_vseparator;
	GtkButton		*new_tweet_button;
	
	/*The list & actual GtkWidgets for sending DMs.*/
	GtkFrame		*dm_frame;
	GtkLabel		*dm_frame_label;
	GtkHBox			*dm_form_hbox;
	GtkButton		*dm_refresh;
	GtkComboBox		*followers_combo_box;
	GtkListStore		*followers_list_store;
	GtkTreeModel		*followers_model;
	GtkButton		*followers_send_dm;
	
	GtkToggleButton		*dm_form_active_togglebutton;
	GtkImage		*dm_form_active_image;
	
	/* Widgets that are enabled when we a tweet is selected */
	GList			*selected_tweet_buttons;
			
	/* Buttons for viewing details about the user of the current selected/extended Tweet. */
	GtkButton		*view_user_profile_button;
	GtkButton		*view_user_tweets_button;
	
	/* Buttons for viewing details about the user of the current selected/extended Tweet. */
	GtkButton		*user_follow_button;
	GtkButton		*user_unfollow_button;
	GtkButton		*user_block_button;
	
	/* Togglebutton & image used to indicate & switch between embed tweet-view & floating. */
	GtkToggleButton		*embed_togglebutton;
	GtkImage		*embed_image;
	
	/* Buttons for stuff to do with the current selected & extended tweet. */
	GtkButton		*reply_button;
	GtkButton		*retweet_button;
	GtkButton		*make_fave_button;
} TweetView;

/********************************************************
 *          Global method  & function prototypes        *
 ********************************************************/
TweetView *tweet_view_new(GtkWindow *parent);
GtkWindow *tweet_view_get_window(void);

void tweet_view_beep(void);

void tweet_view_show_tweet(OnlineService *service, const gulong id, const gulong user_id, const gchar *user_name, const gchar *user_nick, const gchar *date, const gchar *sexy_tweet, const gchar *text_tweet, GdkPixbuf *pixbuf );

void tweet_view_dm_data_fill(GList *followers);

void tweet_view_set_embed_toggle_and_image(void);

void tweet_view_sexy_select(void);

void tweet_view_sexy_prefix_char(const char c);
void tweet_view_sexy_prefix_string(const gchar *str);
void tweet_view_sexy_set(gchar *tweet);
void tweet_view_sexy_insert_char(const char c);
void tweet_view_sexy_insert_string(const gchar *str);
void tweet_view_sexy_append_char(const char c);
void tweet_view_sexy_append_string(const gchar *str);
gint tweet_view_sexy_puts(const gchar *str, gint position);

void tweet_view_new_dm(void);
void tweet_view_reply(void);
void tweet_view_send(GtkWidget *activated_widget);
void tweet_view_sexy_send(gpointer service, gpointer user_data);

#endif /* __TWEET_VIEW_H__ */

/********************************************************
 *                       eof                            *
 ********************************************************/
