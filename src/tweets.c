/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2009 Brian Pepple <bpepple@fedoraproject.org>
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
 * Authors: Kaity G. B. <uberChick@uberChicGeekChick.com>
 */

#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <glib/gprintf.h>


#include "debug.h"
#include "config.h"
#include "app.h"
#include "tweets.h"
#include "network.h"
#include "send-message-dialog.h"
#include "tweet-list.h"
#include "profile-viewer.h"


typedef struct SelectedTweet {
	unsigned long int id;
	gchar *user_name;
	gchar *tweet;
} SelectedTweet;//SelectedTweet

static void tweets_include_and_begin_to_send(const gchar *tweet, gboolean release);


unsigned long int in_reply_to_status_id=0;
static SelectedTweet *selected_tweet=NULL;


void set_selected_tweet(unsigned long int id, const gchar *user_name, const gchar *tweet){
	/*	id=strtoul( char id, NULL, 0 );	*/
	if(selected_tweet) g_free(selected_tweet);
	selected_tweet=g_new0(SelectedTweet, 1);
	selected_tweet->id=id;
	selected_tweet->user_name=g_strdup(user_name);
	selected_tweet->tweet=g_strdup(tweet);
}//set_selected_tweets

gchar *tweets_get_selected_user_name(void){
	if(!selected_tweet->user_name)
			return NULL;
	return g_strdup(selected_tweet->user_name);
}//tweets_get_selected_user_name

void tweets_show_submenu_entries(gboolean show){
	for(GList *l=app_get_widgets_tweet_selected(); l; l = l->next)
		g_object_set(l->data, "sensitive", show, NULL);
}//tweets_show_submenu_entries

void tweets_new_tweet(void){
	if(in_reply_to_status_id) in_reply_to_status_id=0;
	send_message_dialog_show(GTK_WINDOW(app_get_window()));
	message_show_friends(FALSE);
}//tweets_new_tweet

void tweets_reply(void){
	if(!selected_tweet)
		return;
	const gchar *tweet=g_strdup_printf("@%s ", selected_tweet->user_name);
	in_reply_to_status_id=selected_tweet->id;
	tweets_include_and_begin_to_send(tweet, (gboolean)TRUE);
}//tweets_reply

void tweets_retweet(void){
	if(!selected_tweet)
		return;
	const gchar *tweet=g_strdup_printf("RT @%s %s", selected_tweet->user_name, selected_tweet->tweet);
	in_reply_to_status_id=selected_tweet->id;
	tweets_include_and_begin_to_send(tweet, (gboolean)TRUE);
}//tweets_retweet

static void tweets_include_and_begin_to_send(const gchar *tweet, gboolean release){
	send_message_dialog_show(GTK_WINDOW(app_get_window()));
	message_show_friends(FALSE);
	if(!tweet) return;
	message_set_message(tweet);
	if(release) g_free((gchar *)tweet);
}//tweets_include_and_begin_to_send

void tweets_new_dm(void){
	send_message_dialog_show(GTK_WINDOW(app_get_window()));
	message_show_friends(TRUE);
}//tweets_new_dm

void tweets_make_fave(void){
	if(!selected_tweet)
		return;
	gchar *url=NULL;
	network_get( (url=g_strdup_printf( API_TWITTER_SAVE_FAVE, selected_tweet->id )) );
	g_free(url);
	app_set_statusbar_msg(_("Star'd Tweet."));
}//tweets_save_fave

void unset_selected_tweet(void){
	if(!selected_tweet) return;
	if(selected_tweet->user_name) g_free(selected_tweet->user_name);
	if(selected_tweet->tweet) g_free(selected_tweet->tweet);
	if(selected_tweet) g_free(selected_tweet);
}//unset_selected_tweet

void tweets_view_selected_timeline(void){
	if(!selected_tweet->user_name) return;
	network_get_user_timeline(selected_tweet->user_name);
}//tweets_view_selected_timeline

void tweets_view_selected_profile(void){
	if(!selected_tweet->user_name) return;
	view_profile( selected_tweet->user_name, GTK_WINDOW(app_get_window()) );
}//tweets_view_selected_profile

