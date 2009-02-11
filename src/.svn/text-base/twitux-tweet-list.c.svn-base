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
 */

#include "config.h"

#include <libtwitux/twitux-conf.h>
#include <libtwitux/twitux-debug.h>

#include "twitux.h"
#include "twitux-tweet-list.h"
#include "twitux-send-message-dialog.h"
#include "twitux-label.h"
#include "twitux-app.h"

#define DEBUG_DOMAIN "TweetList"

#define GET_PRIV(obj)           \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), TWITUX_TYPE_TWEET_LIST, TwituxTweetListPriv))

struct _TwituxTweetListPriv {
	GtkListStore      *store;
	
	GtkTreeViewColumn *text_column;
	GtkCellRenderer   *text_renderer;
};

static void   twitux_tweet_list_class_init (TwituxTweetListClass *klass);
static void   twitux_tweet_list_init       (TwituxTweetList      *tweet);
static void   tweet_list_finalize          (GObject              *obj);
static void   tweet_list_create_model      (TwituxTweetList      *list);
static void   tweet_list_setup_view        (TwituxTweetList      *list);
static void   tweet_list_size_cb           (GtkWidget            *widget,
                                            GtkAllocation        *allocation,
                                            gpointer              user_data);
static void   tweet_list_changed_cb        (GtkWidget            *widget,
                                            gpointer              user_data);
static void   tweet_list_activated_cb      (GtkTreeView          *tree_view,
                                            GtkTreePath          *path,
                                            GtkTreeViewColumn    *column,
                                            gpointer              user_data);
static TwituxTweetList *list = NULL;

G_DEFINE_TYPE (TwituxTweetList, twitux_tweet_list, GTK_TYPE_TREE_VIEW);

static void
twitux_tweet_list_class_init (TwituxTweetListClass *klass)
{
	GObjectClass   *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = tweet_list_finalize;

	g_type_class_add_private (object_class, sizeof (TwituxTweetListPriv));
}

static void
twitux_tweet_list_init (TwituxTweetList *tweet)
{
	TwituxTweetListPriv *priv;

	list = tweet;

	priv = GET_PRIV (list);

	tweet_list_create_model (list);
	tweet_list_setup_view (list);

	g_signal_connect (tweet,
					  "size_allocate",
					  G_CALLBACK (tweet_list_size_cb),
					  tweet);
	g_signal_connect (tweet,
					  "cursor-changed",
					  G_CALLBACK (tweet_list_changed_cb),
					  tweet);
	g_signal_connect (tweet,
					  "row-activated",
					  G_CALLBACK (tweet_list_activated_cb),
					  tweet);
}

static void
tweet_list_finalize (GObject *object)
{
	TwituxTweetListPriv *priv;

	priv = GET_PRIV (object);

	g_object_unref (priv->store);

	G_OBJECT_CLASS (twitux_tweet_list_parent_class)->finalize (object);
}

static void
tweet_list_create_model (TwituxTweetList *list)
{
	TwituxTweetListPriv *priv;
	GtkTreeModel        *model;

	priv = GET_PRIV (list);

	if (priv->store) {
		g_object_unref (priv->store);
	}

	priv->store =
		gtk_list_store_new (N_COLUMNS,
							GDK_TYPE_PIXBUF,  /* Avatar pixbuf */
							G_TYPE_STRING,    /* Display string */
							G_TYPE_STRING,    /* Author name string */
							G_TYPE_STRING,    /* Date string */
							G_TYPE_STRING,    /* Tweet string */
							G_TYPE_STRING);   /* Username string */

	/* save normal model */
	model = GTK_TREE_MODEL (priv->store);

	gtk_tree_view_set_model (GTK_TREE_VIEW (list), model);
}

static void
tweet_list_setup_view (TwituxTweetList *list)
{
	TwituxTweetListPriv *priv;
	GtkCellRenderer		*renderer;
	GtkTreeViewColumn	*avatar_column;
	GtkTreeViewColumn   *tweet_column;

	priv = GET_PRIV (list);
	
	g_object_set (list,
				  "rules-hint", TRUE,
				  "reorderable", FALSE,
				  "headers-visible", FALSE,
				  NULL);

	renderer = gtk_cell_renderer_pixbuf_new ();
	avatar_column =
		gtk_tree_view_column_new_with_attributes (NULL,
												  renderer,
												  "pixbuf", PIXBUF_AVATAR,
												  NULL);

	gtk_tree_view_append_column (GTK_TREE_VIEW (list), avatar_column);

	renderer = gtk_cell_renderer_text_new ();
	tweet_column =
		gtk_tree_view_column_new_with_attributes (NULL,
												  renderer,
												  "markup", STRING_TEXT,
												  NULL);
	gtk_tree_view_column_set_sizing (tweet_column,
									 GTK_TREE_VIEW_COLUMN_FIXED);
	g_object_set (renderer,
				  "ypad", 0,
				  "xpad", 5,
				  "yalign", 0.0,
				  "wrap-mode", PANGO_WRAP_WORD_CHAR,
				  NULL);

	gtk_tree_view_append_column (GTK_TREE_VIEW (list), tweet_column);
	
	priv->text_column = tweet_column;
	priv->text_renderer = renderer;
}

static void
tweet_list_changed_cb (GtkWidget *widget,
                       gpointer   user_data)
{
	TwituxTweetList     *t;
	TwituxTweetListPriv *priv;
	GtkTreeSelection    *sel;
	GtkTreeIter          iter;
	GdkPixbuf           *pixbuf;
	gchar               *name, *tweet, *date;
	gboolean             expand;

	t = TWITUX_TWEET_LIST (user_data);
	priv = GET_PRIV (t);

	twitux_conf_get_bool (twitux_conf_get (),
						  TWITUX_PREFS_UI_EXPAND_MESSAGES,
						  &expand);

	if (!expand)
		return;
	
	/* Get selected Iter */
	sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (widget));
	
	if (!gtk_tree_selection_get_selected (sel, NULL, &iter))
		return;

	gtk_tree_model_get (GTK_TREE_MODEL (priv->store),
						&iter,
						STRING_AUTHOR, &name,
						STRING_TWEET, &tweet,
						STRING_DATE, &date,
						PIXBUF_AVATAR, &pixbuf,
						-1);

	twitux_app_expand_message (name, date, tweet, pixbuf);

	g_free (name);
	g_free (tweet);
	g_free (date);
}

static void
tweet_list_size_cb (GtkWidget     *widget,
                    GtkAllocation *allocation,
                    gpointer       user_data)
{
	TwituxTweetList     *t;
	TwituxTweetListPriv *priv;
	gint                 w;

	t = TWITUX_TWEET_LIST (user_data);	
	priv = GET_PRIV (t);

	w = gtk_tree_view_column_get_width (priv->text_column);

	g_object_set (priv->text_renderer,
				  "wrap-width", w-10,
				  NULL);
}

static void
tweet_list_activated_cb (GtkTreeView       *tree_view,
                         GtkTreePath       *path,
                         GtkTreeViewColumn *column,
                         gpointer           user_data)
{
	TwituxTweetList     *t;
	TwituxTweetListPriv *priv;
	gchar               *user;
	gchar               *message;
	GtkTreeIter          iter;

	t = TWITUX_TWEET_LIST (user_data);
	priv = GET_PRIV (t);

	gtk_tree_model_get_iter (GTK_TREE_MODEL (priv->store),
							 &iter,
							 path);

	gtk_tree_model_get (GTK_TREE_MODEL (priv->store),
						&iter,
						STRING_USER, &user,
						-1);

	message = g_strdup_printf ("@%s: ", user);
	twitux_send_message_dialog_show (NULL);
	twitux_message_show_friends (FALSE);
	twitux_message_set_message (message);

	g_free (user);
	g_free (message);
}

TwituxTweetList *
twitux_tweet_list_new (void)
{
	return g_object_new (TWITUX_TYPE_TWEET_LIST, NULL);
}

GtkListStore *
twitux_tweet_list_get_store (void)
{
	TwituxTweetListPriv *priv;
	
	priv = GET_PRIV (list);
	
	return priv->store;
}
