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


#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <libsexy/sexy.h>

#include "config.h"

#include "main.h"
#include "tweet-list.h"
#include "label.h"
#include "app.h"
#include "tweets.h"
#include "tweet-view.h"

#define DEBUG_DOMAINS "TweetList:UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Files:I/O:Tweets:Requests:Users:Notification:Authentication:Preferences:Settings:Setup:Start-Up"
#include "debug.h"

#define	GET_PRIV(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_TWEET_LIST, TweetListPriv))

struct _TweetListPriv {
	GtkListStore		*store;
	GtkTreeModel		*model;
	GtkTreeModel		*sort_model;
	GtkTreeViewColumn	*text_column;
	GtkCellRenderer		*text_renderer;
};

static void tweet_list_class_init(TweetListClass *klass);
static void tweet_list_init(TweetList      *tweet);
static void tweet_list_finalize(GObject              *obj);

static void tweet_list_create_model(TweetList      *list);
static void tweet_list_setup_view(TweetList      *list);

static void tweet_list_size_cb(GtkWidget *widget, GtkAllocation *allocation, TweetList *list);
static void tweet_list_changed_cb(GtkWidget *widget, TweetList *tweet);

static void tweet_list_move(GdkEventKey *event, TweetList *list);


static TweetList *list=NULL;
static TweetListPriv *list_priv=NULL;
static gint tweet_list_index=0;

G_DEFINE_TYPE(TweetList, tweet_list, SEXY_TYPE_TREE_VIEW);

static void tweet_list_class_init( TweetListClass *klass ){
	GObjectClass   *object_class=G_OBJECT_CLASS(klass);
	object_class->finalize=tweet_list_finalize;
	g_type_class_add_private(object_class, sizeof(TweetListPriv));
}/* tweet_list_class_init */

TweetList *tweet_list_get(void){
	return list;
}/*tweet_list_get*/

static void tweet_list_init(TweetList *tweet){
	list=tweet;
	list_priv=GET_PRIV(list);
	tweet_list_create_model(list);
	tweet_list_setup_view(list);
	g_signal_connect(list, "size_allocate", G_CALLBACK(tweet_list_size_cb), list);
	g_signal_connect(list, "cursor-changed", G_CALLBACK(tweet_list_changed_cb), list);
	g_signal_connect(list, "row-activated", G_CALLBACK(tweets_reply), list);
	g_signal_connect(list, "key-press-event", G_CALLBACK(tweet_list_key_pressed), list);
	//sexy_tree_view_set_tooltip_label_column( SEXY_TREE_VIEW( list ), STRING_TWEET );
}/* tweet_list_init */

static void tweet_list_finalize( GObject *object ){
	if(list_priv->store)
		g_object_unref(list_priv->store);
	G_OBJECT_CLASS(tweet_list_parent_class)->finalize(object);
}/* tweet_list_finalizd */

static void tweet_list_create_model( TweetList *list ){
	GtkTreeModel *model;
	GtkTreeModel *sort_model;
	
	
	if(list_priv->store)
		g_object_unref(list_priv->store);

	list_priv->store=gtk_list_store_new(
						N_COLUMNS,
							GDK_TYPE_PIXBUF,	/*PIXBUF_AVATAR: Avatar pixbuf */
							G_TYPE_STRING,		/*STRING_TEXT: Display string */
							G_TYPE_STRING,		/*STRING_NICK: Author name string */
							G_TYPE_STRING,		/*STRING_DATE: 'Posted ?(seconds|minutes|hours|day) ago */
							G_TYPE_STRING,		/*STRING_TWEET: Tweet string */
							G_TYPE_STRING,		/*STRING_USER: Username string */
							G_TYPE_STRING,		/*SEXY_TWEET: libsexy formatted Tweet for SexyTreeView's tooltip */
							G_TYPE_STRING,		/*CREATED_DATE: Date string */
							G_TYPE_ULONG,		/*CREATED_AT: unix timestamp of tweet's posted data time for sorting. */
							G_TYPE_ULONG,		/*ULONG_TWEET_ID: Tweet's ID */
							G_TYPE_ULONG,		/*ULONG_USER_ID: User's ID */
							G_TYPE_POINTER		/*SERVICE_POINTER: Service pointer */
	);

	/* save normal model */
	model=GTK_TREE_MODEL(list_priv->store);
	sort_model=gtk_tree_model_sort_new_with_model(model);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(sort_model), CREATED_AT, GTK_SORT_DESCENDING);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(list), sort_model);
	list_priv->model=model;
	list_priv->sort_model=sort_model;
	
	sexy_tree_view_set_tooltip_label_column( SEXY_TREE_VIEW(list), STRING_TEXT);
}

static void tweet_list_setup_view(TweetList *list){
	GtkCellRenderer		*renderer;
	GtkTreeViewColumn	*avatar_column;
	GtkTreeViewColumn	*tweet_column;
	
	g_object_set(list, "rules-hint", TRUE, "reorderable", TRUE, "headers-visible", FALSE, NULL);

	renderer=gtk_cell_renderer_pixbuf_new();
	/*'fixed size' along with x & y padding actually fix the image at 48x48.*/
	gtk_cell_renderer_set_fixed_size( renderer, 58, 55 );
	avatar_column=gtk_tree_view_column_new_with_attributes( NULL, renderer, "pixbuf", PIXBUF_AVATAR, NULL);
	g_signal_connect(avatar_column, "clicked", G_CALLBACK(tweet_list_changed_cb), list);
	g_object_set( renderer, "ypad", 5, "xpad", 5, "yalign", 0.0, NULL );
	gtk_tree_view_column_set_sizing( avatar_column, GTK_TREE_VIEW_COLUMN_FIXED );
	gtk_tree_view_column_set_min_width( avatar_column, 58 );
	gtk_tree_view_column_set_fixed_width( avatar_column, 58 );
	gtk_tree_view_append_column( GTK_TREE_VIEW( list ), avatar_column );
	
	renderer=gtk_cell_renderer_text_new();
	tweet_column=gtk_tree_view_column_new_with_attributes( NULL, renderer, "markup", STRING_TEXT, NULL );
	gtk_tree_view_column_set_sizing( tweet_column, GTK_TREE_VIEW_COLUMN_FIXED );
	g_object_set( renderer, "ypad", 0, "xpad", 5, "yalign", 0.0, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL );
	gtk_tree_view_append_column( GTK_TREE_VIEW( list ), tweet_column );
	
	list_priv->text_column=tweet_column;
	g_signal_connect(list_priv->text_column, "clicked", G_CALLBACK(tweet_list_changed_cb), list);
	
	list_priv->text_renderer=renderer;
}


static void tweet_list_move(GdkEventKey *event, TweetList *list){
	switch(event->keyval){
		case GDK_Tab: case GDK_KP_Tab:
		case GDK_Home: case GDK_KP_Home:
		case GDK_Begin: case GDK_Escape:
			tweet_list_index=0;
			break;
		case GDK_Up: case GDK_KP_Up: case GDK_KP_Prior:
			tweet_list_index--;
			break;
		case GDK_Down: case GDK_KP_Down: case GDK_KP_Next:
			tweet_list_index++;
			break;
		case GDK_End: case GDK_KP_End:
			tweet_list_index=19;
			break;
		case GDK_Page_Up: tweet_list_index-=10; break;
		case GDK_Page_Down: tweet_list_index+=10; break;
		default: return;
	}//switch
	
	tweet_list_move_to(tweet_list_index);
}/* tweet_list_move */

void tweet_list_goto_top(void){
	GtkTreePath *path=gtk_tree_path_new_from_indices(0, -1);
	if(GTK_IS_TREE_VIEW(GTK_TREE_VIEW(list)))
		gtk_tree_view_scroll_to_cell( GTK_TREE_VIEW(list), path, NULL, FALSE, 0.0, 0.0);
	gtk_tree_path_free(path);
	tweet_list_index=0;
}/* tweet_list_goto_top */

void tweet_list_move_to(gint row_index){
	guint max_tweets=((online_services_count_connections(online_services))*20)-1;
	if( row_index<0 ) row_index=0;
	else if( row_index>max_tweets ) row_index=max_tweets;
	
	debug("Selecting tweet %d, maximum tweets are: %d.", row_index, max_tweets);
	GtkTreePath *path=gtk_tree_path_new_from_indices(row_index, -1);
	gtk_tree_view_set_cursor( GTK_TREE_VIEW(list), path, NULL, FALSE );
	gtk_tree_path_free(path);
	
	tweet_view_sexy_select();
}/* tweet_list_move_to */

void tweet_list_refresh(void){
	debug("Re-setting tweet_list_index.");
	gtk_list_store_clear(list_priv->store);
	tweet_list_index=0;
}/* tweet_list_refreshed */


void tweet_list_key_pressed(GtkWidget *widget, GdkEventKey *event, TweetList *list){
	if(event->state == GDK_CONTROL_MASK) return tweets_hotkey(widget, event);
	if(event->keyval != GDK_Return) return tweet_list_move(event, list);
	
	switch(event->state){
		case GDK_CONTROL_MASK: return tweets_new_tweet();
		case GDK_MOD1_MASK: return tweets_retweet();
		case GDK_SHIFT_MASK: return tweet_view_new_dm();
		default: tweets_reply();
	}//switch
}/* tweet_list_key_pressed */


static void tweet_list_changed_cb(GtkWidget *widget, TweetList *friends_tweet){
	GtkTreeSelection	*sel;
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	if(!((sel=gtk_tree_view_get_selection(GTK_TREE_VIEW(list))) && gtk_tree_selection_get_selected(sel, &list_priv->sort_model, iter) )){
		g_free(iter);
		tweet_view_sexy_select();
		return;
	}
	
	app_set_statusbar_msg(TWEETS_RETURN_MODIFIERS_STATUSBAR_MSG);
	
	gulong		tweet_id, user_id;
	OnlineService	*service=NULL;
	GdkPixbuf	*pixbuf;
	gchar		*user_name, *user_nick, *date, *sexy_tweet, *text_tweet;

	gtk_tree_model_get(
				GTK_TREE_MODEL(list_priv->sort_model),
				iter,
				STRING_NICK, &user_nick,
				STRING_TWEET, &text_tweet,
				SEXY_TWEET, &sexy_tweet,
				STRING_DATE, &date,
				STRING_USER, &user_name,
				PIXBUF_AVATAR, &pixbuf,
				ULONG_TWEET_ID, &tweet_id,
				ULONG_USER_ID, &user_id,
				SERVICE_POINTER, &service,
			-1
	);
	
	debug("Displaying tweet: #%lu from '%s'.", tweet_id, service->decoded_key);
	tweet_view_show_tweet(service, (unsigned long int)tweet_id, user_id, user_name, user_nick, date, sexy_tweet, text_tweet, pixbuf);
	
	g_free(user_name);
	g_free(sexy_tweet);
	g_free(text_tweet);
	g_free(date);
	g_free(user_nick);
	if(pixbuf) g_object_unref(pixbuf);
	g_free(iter);
	
	tweet_view_sexy_select();
}

static void tweet_list_size_cb(GtkWidget *widget, GtkAllocation *allocation, TweetList *friends_tweet){
	TweetList *t=TWEET_LIST(friends_tweet);
	g_object_set(GET_PRIV(t)->text_renderer, "wrap-width",((gtk_tree_view_column_get_width(GET_PRIV(t)->text_column))-10), NULL);
}

TweetList *tweet_list_new(void){
	return g_object_new(TYPE_TWEET_LIST, NULL);
}

GtkListStore *tweet_list_get_store(void){
	return list_priv->store;
}

void tweet_list_set_image(const gchar *image_filename, GtkTreeIter *iter){
	GdkPixbuf *pixbuf;
	if( !(pixbuf=images_get_pixbuf_from_filename((gchar *)image_filename)) )
		return;
	
	debug("Adding image: '%s' to tweet_view.", image_filename);
	gtk_list_store_set(list_priv->store , iter, PIXBUF_AVATAR, pixbuf, -1);
	g_object_unref(pixbuf);
}/* tweet_list_set_image */

