/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2007-2008 Brian Pepple <bpepple@fedoraproject.org>
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
 * Authors: Brian Pepple <bpepple@fedoraproject.org>
 *			Daniel Morales <daniminas@gmail.com>
 *			Kaity G. B. <uberChick@uberChicGeekChick.Com>
 *
 */

#include "config.h"

#include "gconf.h"
#include "debug.h"

#include "main.h"
#include "tweet-list.h"
#include "send-message-dialog.h"
#include "label.h"
#include "app.h"

#define DEBUG_DOMAIN "TweetList"

#define GET_PRIV(obj)           \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TWITUX_TYPE_TWEET_LIST, TwituxTweetListPriv))

struct _TwituxTweetListPriv {
	GtkListStore      *store;
	
	GtkTreeViewColumn *text_column;
	GtkCellRenderer   *text_renderer;
};

static void   twitux_tweet_list_class_init(TwituxTweetListClass *klass);
static void   twitux_tweet_list_init(TwituxTweetList      *tweet);
static void   tweet_list_finalize(GObject              *obj);
static void   tweet_list_create_model(TwituxTweetList      *list);
static void   tweet_list_setup_view(TwituxTweetList      *list);
static void   tweet_list_size_cb(GtkWidget            *widget,
                                            GtkAllocation        *allocation,
                                            gpointer              user_data);
static void   tweet_list_changed_cb(GtkWidget            *widget,
                                            gpointer              user_data);
static void   tweet_list_activated_cb(GtkTreeView          *tree_view,
                                            GtkTreePath          *path,
                                            GtkTreeViewColumn    *column,
                                            gpointer              friends_tweet);
static TwituxTweetList *list = NULL;
static TwituxTweetListPriv *list_priv=NULL;

G_DEFINE_TYPE(TwituxTweetList, twitux_tweet_list, GTK_TYPE_TREE_VIEW);

static void
twitux_tweet_list_class_init(TwituxTweetListClass *klass)
{
	GObjectClass   *object_class = G_OBJECT_CLASS(klass);

	object_class->finalize = tweet_list_finalize;

	g_type_class_add_private(object_class, sizeof(TwituxTweetListPriv));
}

static void
twitux_tweet_list_init(TwituxTweetList *tweet)
{
	
	list=tweet;

	list_priv=GET_PRIV(list);

	tweet_list_create_model(list);
	tweet_list_setup_view(list);

	g_signal_connect(list,
					  "size_allocate",
					  G_CALLBACK(tweet_list_size_cb),
					  list);
	g_signal_connect(list,
					  "cursor-changed",
					  G_CALLBACK(tweet_list_changed_cb),
					  list);
	g_signal_connect(list,
					  "row-activated",
					  G_CALLBACK(tweet_list_activated_cb),
					  list);
	//g_object_unref(priv);
}

static void
tweet_list_finalize(GObject *object)
{
	
	list_priv=GET_PRIV(object);

	if(list_priv->store)
		g_object_unref(list_priv->store);

	G_OBJECT_CLASS(twitux_tweet_list_parent_class)->finalize(object);
	g_object_unref(object);
	//g_object_unref(priv);
}

static void
tweet_list_create_model(TwituxTweetList *list)
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
	//g_object_unref(priv);
}

static void
tweet_list_setup_view(TwituxTweetList *list)
{
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
	//g_object_unref(priv);
}

static void
tweet_list_changed_cb(GtkWidget *widget,
                       gpointer   user_data)
{
	gboolean expand;
	twitux_conf_get_bool(twitux_conf_get(),
						TWITUX_PREFS_UI_EXPAND_MESSAGES,
						&expand);
	if(!expand)
		return;
	
	TwituxTweetList     *t;
		GtkTreeSelection    *sel;
	GtkTreeIter          iter;
	GdkPixbuf           *pixbuf;
	gchar               *name, *tweet, *date;

	t = TWITUX_TWEET_LIST(user_data);
	
	/* Get selected Iter */
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
	
	if(!gtk_tree_selection_get_selected(sel, NULL, &iter))
		return;
	
	gtk_tree_model_get(GTK_TREE_MODEL(list_priv->store),
						&iter,
						STRING_AUTHOR, &name,
						STRING_TWEET, &tweet,
						STRING_DATE, &date,
						PIXBUF_AVATAR, &pixbuf,
						-1);
	
	twitux_app_expand_message(name, date, tweet, pixbuf);
	
	g_free(name);
	g_free(tweet);
	g_free(date);
	g_object_unref(pixbuf);
}

static void
tweet_list_size_cb(GtkWidget     *widget,
                    GtkAllocation *allocation,
                    gpointer       user_data)
{
	TwituxTweetList     *t;
	
	t=TWITUX_TWEET_LIST(user_data);	
	
	g_object_set(list_priv->text_renderer, "wrap-width",((gtk_tree_view_column_get_width(list_priv->text_column))-10), NULL);
}

static void
tweet_list_activated_cb(GtkTreeView       *tree_view,
                         GtkTreePath       *path,
                         GtkTreeViewColumn *column,
                         gpointer           friends_tweet)
{
	TwituxTweetList     *t=NULL;
	gchar               *friend=NULL;
	GtkTreeIter          iter;

	t = TWITUX_TWEET_LIST(friends_tweet);

	gtk_tree_model_get_iter(GTK_TREE_MODEL(list_priv->store),
							 &iter,
							 path);

	gtk_tree_model_get(GTK_TREE_MODEL(list_priv->store),
						&iter,
						STRING_USER, &friend,
						-1);

	twitux_send_message_dialog_show(NULL);
	twitux_message_show_friends(FALSE);
	twitux_message_set_message( (g_strdup_printf("@%s ", friend)) );

	g_free(friend);
}

TwituxTweetList *twitux_tweet_list_new(void){
	return g_object_new(TWITUX_TYPE_TWEET_LIST, NULL);
}

GtkListStore *twitux_tweet_list_get_store(void){
	return list_priv->store;
}
