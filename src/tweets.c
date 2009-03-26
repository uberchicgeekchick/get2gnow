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
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>


#include "config.h"
#include "app.h"
#include "tweets.h"
#include "send-message-dialog.h"
#include "tweet-list.h"


typedef struct SelectedTweet {
	const gchar *user_name;
	const gchar *tweet;
} SelectedTweet;//SelectedTweet

static SelectedTweet *selected_tweet=NULL;

static void tweets_include_and_begin_to_send(const gchar *tweet, gboolean release);




void set_selected_tweet(const gchar *user_name, const gchar *tweet){
	if(selected_tweet) g_free(selected_tweet);
	selected_tweet=g_new0(SelectedTweet, 1);
	selected_tweet->user_name=user_name;
	selected_tweet->tweet=tweet;
}//set_selected_tweet

void unset_selected_tweet(void){
	if(!selected_tweet) return;
//	if(selected_tweet->user_name) g_free(selected_tweet->user_name);
//	if(selected_tweet->tweet) g_free(selected_tweet->tweet);
	if(selected_tweet) g_free(selected_tweet);
}//unset_selected_tweet

void show_tweets_submenu_entries(gboolean show){
	for(GList *l = app_priv->widgets_tweet_selected; l; l = l->next)
		g_object_set(l->data, "sensitive", show, NULL);
}//show_tweets_submenu_entries

void tweets_key_pressed(GtkWidget *widget, GdkEventKey *event){
	if(event->keyval !=GDK_Return ) return;
	if(event->state == GDK_CONTROL_MASK ) return tweets_new_tweet();
	if(event->state == GDK_MOD1_MASK ) return tweets_retweet();
	if(event->state == GDK_SHIFT_MASK ) return tweets_new_dm();
	tweets_reply();
}//tweets_key_pressed


void tweets_new_tweet(void){
	send_message_dialog_show(GTK_WINDOW(app_priv->window));
	message_show_friends(FALSE);
}

void tweets_reply(void){
	if(!selected_tweet)
		return;
	const gchar *tweet=g_strdup_printf("@%s ", selected_tweet->user_name);
	tweets_include_and_begin_to_send(tweet, (gboolean)TRUE);
}

void tweets_retweet(void){
	if(!selected_tweet)
		return;
	const gchar *tweet=g_strdup_printf("rt @%s %s", selected_tweet->user_name, selected_tweet->tweet);
	tweets_include_and_begin_to_send(tweet, (gboolean)TRUE);
}

static void tweets_include_and_begin_to_send(const gchar *tweet, gboolean release){
	send_message_dialog_show(GTK_WINDOW(app_priv->window));
	message_show_friends(FALSE);
	if(!tweet) return;
	message_set_message(tweet);
	if(release) g_free((gchar *)tweet);
}//tweets_include_and_begin_to_send

void tweets_new_dm(void){
	send_message_dialog_show(GTK_WINDOW(app_priv->window));
	message_show_friends(TRUE);
}

