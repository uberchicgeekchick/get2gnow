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

#include <gtk/gtk.h>

#include "config.h"

#include "gconf.h"
#include "debug.h"

#include "main.h"
#include "tweet-list.h"
#include "send-message-dialog.h"
#include "label.h"
#include "app.h"
#include "tweets.h"

#define DEBUG_DOMAIN "TweetList"

#define GET_PRIV(obj)           \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_TWEET_LIST, TweetListPriv))

struct _TweetListPriv {
	GtkListStore      *store;
	GtkTreeViewColumn *text_column;
	GtkCellRenderer   *text_renderer;
};

static void   tweet_list_class_init(TweetListClass *klass);
static void   tweet_list_init(TweetList      *tweet);
static void   tweet_list_finalize(GObject              *obj);
static void   tweet_list_create_model(TweetList      *list);
static void   tweet_list_setup_view(TweetList      *list);
static void   tweet_list_size_cb(GtkWidget            *widget,
                                            GtkAllocation        *allocation,
                                            gpointer              user_data);
static void   tweet_list_changed_cb(GtkWidget            *widget,
                                            gpointer              user_data);
static void   tweet_list_activated_cb(GtkTreeView          *tree_view,
                                            GtkTreePath          *path,
                                            GtkTreeViewColumn    *column,
                                            gpointer              friends_tweet);
static TweetList *list = NULL;
static TweetListPriv *list_priv=NULL;

G_DEFINE_TYPE(TweetList, tweet_list, GTK_TYPE_TREE_VIEW);

static void
tweet_list_class_init(TweetListClass *klass)
{
	GObjectClass   *object_class = G_OBJECT_CLASS(klass);

	object_class->finalize = tweet_list_finalize;

	g_type_class_add_private(object_class, sizeof(TweetListPriv));
}

static void tweet_list_init(TweetList *tweet){
	
	list=tweet;

	list_priv=GET_PRIV(list);

	tweet_list_create_model(list);
	tweet_list_setup_view(list);

	g_signal_connect(list, "size_allocate", G_CALLBACK(tweet_list_size_cb), list);
	g_signal_connect(list, "cursor-changed", G_CALLBACK(tweet_list_changed_cb), list);
	g_signal_connect(list, "row-activated", G_CALLBACK(tweets_reply), list);
	//g_signal_connect(list, "key-press-event", G_CALLBACK(tweets_key_pressed), list);
}

static void
tweet_list_finalize(GObject *object)
{
	
	list_priv=GET_PRIV(object);

	if(list_priv->store)
		g_object_unref(list_priv->store);

	G_OBJECT_CLASS(tweet_list_parent_class)->finalize(object);
	g_object_unref(object);
}

static void
tweet_list_create_model(TweetList *list)
{
	GtkTreeModel        *model;

	
	if(list_priv->store)
		g_object_unref(list_priv->store);

	list_priv->store=gtk_list_store_new(N_COLUMNS,
							GDK_TYPE_PIXBUF,  /* Avatar pixbuf */
							G_TYPE_STRING,    /* Display string */
							G_TYPE_STRING,    /* Author name string */
							G_TYPE_STRING,    /* Date string */
							G_TYPE_STRING,    /* Tweet string */
							G_TYPE_STRING);   /* Username string */

	/* save normal model */
	model=GTK_TREE_MODEL(list_priv->store);

	gtk_tree_view_set_model(GTK_TREE_VIEW(list), model);
}

static void tweet_list_setup_view(TweetList *list){
	GtkCellRenderer		*renderer;
	GtkTreeViewColumn	*avatar_column;
	GtkTreeViewColumn   *tweet_column;

		
	g_object_set(list, "rules-hint", TRUE, "reorderable", FALSE, "headers-visible", FALSE, NULL);

	renderer=gtk_cell_renderer_pixbuf_new();
	gtk_cell_renderer_set_fixed_size( renderer, 48, 48 );
	avatar_column = gtk_tree_view_column_new_with_attributes( NULL, renderer, "pixbuf", PIXBUF_AVATAR, NULL);
	gtk_tree_view_column_set_sizing( avatar_column, GTK_TREE_VIEW_COLUMN_FIXED );
	gtk_tree_view_column_set_min_width( avatar_column, 48 );
	gtk_tree_view_column_set_fixed_width( avatar_column, 48 );
	gtk_tree_view_append_column( GTK_TREE_VIEW( list ), avatar_column );
	
	renderer=gtk_cell_renderer_text_new();
	tweet_column = gtk_tree_view_column_new_with_attributes( NULL, renderer, "markup", STRING_TEXT, NULL );
	gtk_tree_view_column_set_sizing( tweet_column, GTK_TREE_VIEW_COLUMN_FIXED );
	g_object_set( renderer, "ypad", 0, "xpad", 5, "yalign", 0.0, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL );
	gtk_tree_view_append_column( GTK_TREE_VIEW( list ), tweet_column );
	
	list_priv->text_column=tweet_column;
	list_priv->text_renderer=renderer;
}


static void
tweet_list_changed_cb(GtkWidget *widget,
                       gpointer   user_data)
{
	GtkTreeSelection	*sel;
	GtkTreeIter		iter;
	if(!( (sel=gtk_tree_view_get_selection(GTK_TREE_VIEW(widget))) && gtk_tree_selection_get_selected(sel, NULL, &iter) ))
		return show_tweets_submenu_entries((gboolean)FALSE);
	
	show_tweets_submenu_entries((gboolean)TRUE);
	app_set_statusbar_msg(TWEETS_RETURN_MODIFIERS_STATUSBAR_MSG);
	
	
	gboolean expand;
	conf_get_bool((conf_get()), PREFS_UI_EXPAND_MESSAGES, &expand);
	
	GdkPixbuf	*pixbuf;
	const gchar		*user_name, *tweet, *date, *name;

	gtk_tree_model_get(
				GTK_TREE_MODEL(list_priv->store),
				&iter,
				STRING_AUTHOR, &name,
				STRING_TWEET, &tweet,
				STRING_DATE, &date,
				STRING_USER, &user_name,
				PIXBUF_AVATAR, &pixbuf,
				-1
	);
	
	set_selected_tweet(user_name, tweet);
	
	if(expand) app_expand_message(name, date, tweet, pixbuf);
	
	g_free(name);
	g_free(date);
	g_object_unref(pixbuf);
}

static void
tweet_list_size_cb(GtkWidget     *widget,
                    GtkAllocation *allocation,
                    gpointer       user_data)
{
	TweetList     *t=TWEET_LIST(user_data);	
	
	g_object_set(list_priv->text_renderer, "wrap-width",((gtk_tree_view_column_get_width(list_priv->text_column))-10), NULL);
}

static void
tweet_list_activated_cb(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer friends_tweet){
	tweets_reply();
}

TweetList *tweet_list_new(void){
	return g_object_new(TYPE_TWEET_LIST, NULL);
}

GtkListStore *tweet_list_get_store(void){
	return list_priv->store;
}
