/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Greet-Tweet-Know is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the RPL
 *
 * For more information or to find the latest release, visit our
 * website at: http://uberChicGeekChick.Com/?projects=Greet-Tweet-Know
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
 * License ("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
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
 * user-visible display as defined in Section 6.4 (d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright (C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright (C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright (C) of the User the data is connected to.
 * Users may lisences their data under the terms of an OSI approved or Creative Commons
 * license.  Users must be allowed to select their choice of license for each piece of data
 * on an individual bases and cannot be blanketly applied to all of the Users.  The User may
 * select a default license for their data.  All of the Software's data pertaining to each
 * User must be fully accessible, exportable, and deletable to that User.
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

#define GET_PRIV(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_TWEET_LIST, TweetListPriv))

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
static void tweet_list_size_cb(GtkWidget *widget, GtkAllocation *allocation, TweetList *list);
static void tweet_list_changed_cb(GtkWidget *widget, TweetList *tweet);

static TweetList *list = NULL;
static TweetListPriv *list_priv=NULL;

G_DEFINE_TYPE(TweetList, tweet_list, GTK_TYPE_TREE_VIEW);

static void tweet_list_class_init( TweetListClass *klass ){
	GObjectClass   *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = tweet_list_finalize;
	g_type_class_add_private(object_class, sizeof(TweetListPriv));
}//tweet_list_class_init

static void tweet_list_init(TweetList *tweet){
	list=tweet;
	list_priv=GET_PRIV(list);
	tweet_list_create_model(list);
	tweet_list_setup_view(list);
	g_signal_connect(list, "size_allocate", G_CALLBACK(tweet_list_size_cb), list);
	g_signal_connect(list, "cursor-changed", G_CALLBACK(tweet_list_changed_cb), list);
	g_signal_connect(list, "row-activated", G_CALLBACK(tweets_reply), list);
	//g_signal_connect(list, "key-press-event", G_CALLBACK(tweets_key_pressed), list);
}//tweet_list_init

static void tweet_list_finalize( GObject *object ){
	if(list_priv->store)
		g_object_unref(list_priv->store);
	G_OBJECT_CLASS(tweet_list_parent_class)->finalize(object);
}//tweet_list_finalizd

static void tweet_list_create_model( TweetList *list ){
	GtkTreeModel        *model;

	
	if(list_priv->store)
		g_object_unref(list_priv->store);

	list_priv->store=gtk_list_store_new(N_COLUMNS,
							GDK_TYPE_PIXBUF,	/* Avatar pixbuf */
							G_TYPE_STRING,		/* Display string */
							G_TYPE_STRING,		/* Author name string */
							G_TYPE_STRING,		/* Date string */
							G_TYPE_STRING,		/* Tweet string */
							G_TYPE_STRING,		/* Username string */
							G_TYPE_ULONG		/* Tweet's ID */
	);

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
	tweet_column=gtk_tree_view_column_new_with_attributes( NULL, renderer, "markup", STRING_TEXT, NULL );
	gtk_tree_view_column_set_sizing( tweet_column, GTK_TREE_VIEW_COLUMN_FIXED );
	g_object_set( renderer, "ypad", 0, "xpad", 5, "yalign", 0.0, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL );
	gtk_tree_view_append_column( GTK_TREE_VIEW( list ), tweet_column );
	
	list_priv->text_column=tweet_column;
	list_priv->text_renderer=renderer;
}


static void tweet_list_changed_cb(GtkWidget *widget, TweetList *friends_tweet){
	GtkTreeSelection	*sel;
	GtkTreeIter		iter;
	if(!( (sel=gtk_tree_view_get_selection(GTK_TREE_VIEW(widget))) && gtk_tree_selection_get_selected(sel, NULL, &iter) ))
		return tweets_show_submenu_entries((gboolean)FALSE);
	
	tweets_show_submenu_entries((gboolean)TRUE);
	app_set_statusbar_msg(TWEETS_RETURN_MODIFIERS_STATUSBAR_MSG);
	
	
	gboolean expand;
	conf_get_bool((conf_get()), PREFS_UI_EXPAND_MESSAGES, &expand);
	
	gulong		tweet_id;
	GdkPixbuf	*pixbuf;
	gchar		*user_name, *user_nick, *date, *tweet;

	gtk_tree_model_get(
				GTK_TREE_MODEL(list_priv->store),
				&iter,
				STRING_NICK, &user_nick,
				STRING_TWEET, &tweet,
				STRING_DATE, &date,
				STRING_USER, &user_name,
				PIXBUF_AVATAR, &pixbuf,
				ULONG_TWEET_ID, &tweet_id,
				-1
	);
	
	set_selected_tweet((unsigned long int)tweet_id, user_name, tweet);
	
	if(expand) app_expand_message(user_name, user_nick, date, tweet, pixbuf);
	
	g_free(user_name);
	g_free(tweet);
	g_free(date);
	g_free(user_nick);
	if(pixbuf) g_object_unref(pixbuf);
}

static void tweet_list_size_cb(GtkWidget *widget, GtkAllocation *allocation, TweetList *friends_tweet){
	TweetList *t=TWEET_LIST(friends_tweet);
	g_object_set(list_priv->text_renderer, "wrap-width",((gtk_tree_view_column_get_width(list_priv->text_column))-10), NULL);
}

TweetList *tweet_list_new(void){
	return g_object_new(TYPE_TWEET_LIST, NULL);
}

GtkListStore *tweet_list_get_store(void){
	return list_priv->store;
}
