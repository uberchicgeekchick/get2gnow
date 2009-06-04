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

#include "network.h"
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

/* ListStore columns */
typedef enum {
		PIXBUF_AVATAR,		/*: Avatar pixbuf.*/
		STRING_TEXT,		/*: Display string.*/
		STRING_NICK,		/*: Author name string.*/
		STRING_DATE,		/*: 'Posted ?(seconds|minutes|hours|day) ago.*/
		STRING_TWEET,		/*: Tweet string.*/
		STRING_USER,		/*: Username string.*/
		SEXY_TWEET,		/*: libsexy formatted Tweet for SexyTreeView's tooltip.*/
		CREATED_DATE,		/*: Date string.*/
		SINCE_CREATED,		/*: Seconds since the post was posted.*/
		CREATED_AT,		/*: unix timestamp of tweet's posted data time for sorting. */
		ULONG_TWEET_ID,		/*: Tweet's ID.*/
		ULONG_USER_ID,		/*: User's ID.*/
		ONLINE_SERVICE,		/*: OnlineService pointer.*/
	N_COLUMNS,		/*Not a column but an incremented value to 'count' the total columns.*/
} TweetListStoreColumns;


static void tweet_list_class_init(TweetListClass *klass);
static void tweet_list_init(TweetList      *tweet);
static void tweet_list_finalize(GObject              *obj);

static void tweet_list_create_model(TweetList      *tweet_list);
static void tweet_list_setup_view(TweetList      *tweet_list);

static void tweet_list_size_cb(GtkWidget *widget, GtkAllocation *allocation, TweetList *tweet_list);
static void tweet_list_changed_cb(GtkWidget *widget, TweetList *tweet);

static void tweet_list_move(GtkWidget *widget, GdkEventKey *event);
static void tweet_list_goto_index(void);

static TweetList *tweet_list=NULL;
static TweetListPriv *tweet_list_priv=NULL;

guint tweet_list_notify_delay=0;


G_DEFINE_TYPE(TweetList, tweet_list, SEXY_TYPE_TREE_VIEW);

static void tweet_list_class_init( TweetListClass *klass ){
	GObjectClass   *object_class=G_OBJECT_CLASS(klass);
	object_class->finalize=tweet_list_finalize;
	g_type_class_add_private(object_class, sizeof(TweetListPriv));
}/* tweet_list_class_init */

TweetList *tweet_list_get(void){
	return tweet_list;
}/*tweet_list_get*/

static void tweet_list_init(TweetList *tweet){
	tweet_list=tweet;
	tweet_list_priv=GET_PRIV(tweet_list);
	tweet_list_create_model(tweet_list);
	tweet_list_setup_view(tweet_list);
	tweet_list->index=tweet_list->total=0;
	g_signal_connect(tweet_list, "size_allocate", G_CALLBACK(tweet_list_size_cb), tweet_list);
	g_signal_connect(tweet_list, "cursor-changed", G_CALLBACK(tweet_list_changed_cb), tweet_list);
	g_signal_connect(tweet_list, "row-activated", G_CALLBACK(tweets_reply), tweet_list);
	g_signal_connect(tweet_list, "key-press-event", G_CALLBACK(tweet_list_key_pressed), NULL);
}/* tweet_list_init */

static void tweet_list_finalize( GObject *object ){
	if(tweet_list_priv->store)
		g_object_unref(tweet_list_priv->store);
	G_OBJECT_CLASS(tweet_list_parent_class)->finalize(object);
}/* tweet_list_finalizd */

static void tweet_list_create_model(TweetList *tweet_list){
	GtkTreeModel *model;
	GtkTreeModel *sort_model;
	
	
	if(tweet_list_priv->store)
		g_object_unref(tweet_list_priv->store);

	tweet_list_priv->store=gtk_list_store_new(
						N_COLUMNS,
							GDK_TYPE_PIXBUF,	/*PIXBUF_AVATAR: Avatar pixbuf.*/
							G_TYPE_STRING,		/*STRING_TEXT: Display string.*/
							G_TYPE_STRING,		/*STRING_NICK: Author name string.*/
							G_TYPE_STRING,		/*STRING_DATE: 'Posted ?(seconds|minutes|hours|day) ago.*/
							G_TYPE_STRING,		/*STRING_TWEET: Tweet string.*/
							G_TYPE_STRING,		/*STRING_USER: Username string.*/
							G_TYPE_STRING,		/*SEXY_TWEET: libsexy formatted Tweet for SexyTreeView's tooltip.*/
							G_TYPE_STRING,		/*CREATED_DATE: Date string.*/
							G_TYPE_UINT,		/*SINCE_CREATED: Seconds since the post was posted.*/
							G_TYPE_ULONG,		/*CREATED_AT: unix timestamp of tweet's posted data time for sorting. */
							G_TYPE_ULONG,		/*ULONG_TWEET_ID: Tweet's ID.*/
							G_TYPE_ULONG,		/*ULONG_USER_ID: User's ID.*/
							G_TYPE_POINTER		/*ONLINE_SERVICE: OnlineService pointer.*/
	);
	
	/* save normal model */
	model=GTK_TREE_MODEL(tweet_list_priv->store);
	sort_model=gtk_tree_model_sort_new_with_model(model);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(sort_model), SINCE_CREATED, GTK_SORT_ASCENDING);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(tweet_list), sort_model);
	tweet_list_priv->model=model;
	tweet_list_priv->sort_model=sort_model;
	
	sexy_tree_view_set_tooltip_label_column( SEXY_TREE_VIEW(tweet_list), STRING_TEXT);
}/*tweet_list_create_model(tweet_list);*/

void tweet_list_store_status(OnlineService *service, UserStatus *status){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		
		debug("Appending tweet to TweetList\n\t\t\tTo: <%s> From: <%s@%s>\n\t\t\tTweet ID: %d; posted on [%s]\n\t\t\tStatus update: %s\n\t\t\tFormatted Tweet: %s", service->decoded_key, status->user->user_name, service->server, status->id, status->created_at_str, status->text, status->sexy_tweet);
		
		/*gtk_list_store_insert(tweet_list_priv->store, iter, tweet_list_store_total);*/
		gtk_list_store_append(tweet_list_priv->store, iter);
		gtk_list_store_set(
				tweet_list_priv->store, iter,
					STRING_TEXT, status->tweet,			/*Display string.*/
					STRING_NICK, status->user->nick_name,		/*Author name string.*/
					STRING_DATE, status->created_how_long_ago,	/*(seconds|minutes|hours|day) ago.*/
					STRING_TWEET, status->text,			/*Tweet string.*/
					STRING_USER, status->user->user_name,		/*Username string.*/
					SEXY_TWEET, status->sexy_tweet,			/*SexyTreeView's tooltip.*/
					CREATED_DATE, status->created_at_str,		/*Date string.*/
					CREATED_AT, status->created_at,			/*Seconds since the post was posted.*/
					SINCE_CREATED, status->created_seconds_ago,	/*How old the post is, in seconds, for sorting.*/
					ULONG_TWEET_ID, status->id,			/*Tweet's ID.*/
					ULONG_USER_ID, status->user->id,		/*User's ID.*/
					ONLINE_SERVICE, service,			/*OnlineService pointer.*/
				-1
		);
		tweet_list->total++;
		
		/* network_get_image, or its callback network_cb_on_image, free's iter once its no longer needed.*/
		network_get_image(status->user, iter);
}/*void tweet_list_store_append(service, status);*/

static void tweet_list_setup_view(TweetList *tweet_list){
	GtkCellRenderer		*renderer;
	GtkTreeViewColumn	*avatar_column;
	GtkTreeViewColumn	*tweet_column;
	
	g_object_set(tweet_list, "rules-hint", TRUE, "reorderable", TRUE, "headers-visible", FALSE, NULL);

	renderer=gtk_cell_renderer_pixbuf_new();
	/*'fixed size' along with x & y padding actually fix the image at 48x48.*/
	gtk_cell_renderer_set_fixed_size( renderer, 58, 55 );
	avatar_column=gtk_tree_view_column_new_with_attributes( NULL, renderer, "pixbuf", PIXBUF_AVATAR, NULL);
	g_signal_connect(avatar_column, "clicked", G_CALLBACK(tweet_list_changed_cb), tweet_list);
	g_object_set( renderer, "ypad", 5, "xpad", 5, "yalign", 0.0, NULL );
	gtk_tree_view_column_set_sizing( avatar_column, GTK_TREE_VIEW_COLUMN_FIXED );
	gtk_tree_view_column_set_min_width( avatar_column, 58 );
	gtk_tree_view_column_set_fixed_width( avatar_column, 58 );
	gtk_tree_view_append_column( GTK_TREE_VIEW( tweet_list ), avatar_column );

	renderer=gtk_cell_renderer_text_new();
	tweet_column=gtk_tree_view_column_new_with_attributes( NULL, renderer, "markup", STRING_TEXT, NULL );
	gtk_tree_view_column_set_sizing( tweet_column, GTK_TREE_VIEW_COLUMN_FIXED );
	g_object_set( renderer, "ypad", 0, "xpad", 5, "yalign", 0.0, "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL );
	gtk_tree_view_append_column( GTK_TREE_VIEW( tweet_list ), tweet_column );
	
	tweet_list_priv->text_column=tweet_column;
	g_signal_connect(tweet_list_priv->text_column, "clicked", G_CALLBACK(tweet_list_changed_cb), tweet_list);
	
	tweet_list_priv->text_renderer=renderer;
}/*tweet_list_setup_view(list);*/

void tweet_list_key_pressed(GtkWidget *widget, GdkEventKey *event){
	if(event->keyval!=GDK_Return) return tweet_list_move(widget, event);
	
	switch(event->state){
		case GDK_CONTROL_MASK:
			tweets_new_tweet();
			break;
		case GDK_MOD1_MASK:
			tweets_retweet();
			break;
		case GDK_SHIFT_MASK:
			tweet_view_new_dm();
			break;
		default:
			tweets_reply();
			break;
	}//switch
}/*tweet_list_key_pressed(widget, event);*/

static void tweet_list_move(GtkWidget *widget, GdkEventKey *event){
	switch(event->keyval){
		case GDK_Tab: case GDK_KP_Tab:
		case GDK_Home: case GDK_KP_Home:
		case GDK_Begin: case GDK_Escape:
			tweet_list->index=0;
			break;
		case GDK_Up: case GDK_KP_Up: case GDK_KP_Prior:
			tweet_list->index--;
			break;
		case GDK_Down: case GDK_KP_Down: case GDK_KP_Next:
			tweet_list->index++;
			break;
		case GDK_End: case GDK_KP_End:
			tweet_list->index=tweet_list->total-1;
			break;
		case GDK_Page_Up:
			tweet_list->index-=9; break;
		case GDK_Page_Down:
			tweet_list->index+=9; break;
		default:
			return;
	}//switch
	
	tweet_list_goto_index();
}/* tweet_list_move */

static void tweet_list_goto_index(void){
	if(tweet_list->index<0) {
		tweets_beep();
		tweet_list->index=0;
	}else if(tweet_list->index>=tweet_list->total){
		tweets_beep();
		tweet_list->index=tweet_list->total-1;
	}
	
	debug("Selecting tweet %d, maximum tweets are: %d.", tweet_list->index, tweet_list->total);
	GtkTreePath *path=gtk_tree_path_new_from_indices(tweet_list->index, -1);
	gtk_tree_view_set_cursor( GTK_TREE_VIEW(tweet_list), path, NULL, FALSE );
	gtk_tree_path_free(path);
	
	tweet_view_sexy_select();
}/*tweet_list_goto_index();*/

void tweet_list_goto_top(void){
	GtkTreePath *path=gtk_tree_path_new_from_indices(0, -1);
	if(GTK_IS_TREE_VIEW(GTK_TREE_VIEW(tweet_list)))
		gtk_tree_view_scroll_to_cell( GTK_TREE_VIEW(tweet_list), path, NULL, FALSE, 0.0, 0.0);
	gtk_tree_path_free(path);
	tweet_list->index=0;
}/* tweet_list_goto_top */

void tweet_list_clear(void){
	debug("Re-setting tweet_list_index.");
	gtk_list_store_clear(tweet_list_priv->store);
	tweet_list->index=0;
	tweet_list->total=0;
	tweet_list_notify_delay=0;
}/* tweet_list_refreshed */

static void tweet_list_changed_cb(GtkWidget *widget, TweetList *friends_tweet){
	GtkTreeSelection	*sel;
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	if(!((sel=gtk_tree_view_get_selection(GTK_TREE_VIEW(tweet_list))) && gtk_tree_selection_get_selected(sel, &tweet_list_priv->sort_model, iter) )){
		g_free(iter);
		tweet_view_sexy_select();
		return;
	}
	
	app_set_statusbar_msg(NULL);
	
	gulong		tweet_id, user_id;
	OnlineService	*service=NULL;
	GdkPixbuf	*pixbuf;
	gchar		*user_name, *user_nick, *date, *sexy_tweet, *text_tweet;

	gtk_tree_model_get(
				GTK_TREE_MODEL(tweet_list_priv->sort_model), iter,
					STRING_NICK, &user_nick,
					STRING_TWEET, &text_tweet,
					SEXY_TWEET, &sexy_tweet,
					STRING_DATE, &date,
					STRING_USER, &user_name,
					PIXBUF_AVATAR, &pixbuf,
					ULONG_TWEET_ID, &tweet_id,
					ULONG_USER_ID, &user_id,
					ONLINE_SERVICE, &service,
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
	g_object_set(GET_PRIV(t)->text_renderer, "wrap-width", ((gtk_tree_view_column_get_width(GET_PRIV(t)->text_column))-10), NULL);
}

TweetList *tweet_list_new(void){
	return g_object_new(TYPE_TWEET_LIST, NULL);
}

void tweet_list_set_image(const gchar *image_filename, GtkTreeIter *iter){
	GdkPixbuf *pixbuf;
	if( !(pixbuf=images_get_pixbuf_from_filename((gchar *)image_filename)) )
		return;
	
	debug("Adding image: '%s' to tweet_view.", image_filename);
	gtk_list_store_set(tweet_list_priv->store , iter, PIXBUF_AVATAR, pixbuf, -1);
	g_object_unref(pixbuf);
}/* tweet_list_set_image */

