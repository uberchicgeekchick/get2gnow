/*
 * Copyright(c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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

#ifndef __TWEET_LIST_H__
#define __TWEET_LIST_H__

#define _GNU_SOURCE
#define _THREAD_SAFE

#include <gtk/gtk.h>
#include <libsexy/sexy.h>
#include "online-services-typedefs.h"
#include "users.types.h"

G_BEGIN_DECLS


#define TYPE_TWEET_LIST			(tweet_list_get_type())
#define TWEET_LIST(o)			(G_TYPE_CHECK_INSTANCE_CAST((o), TYPE_TWEET_LIST, TweetList))
#define TWEET_LIST_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), TYPE_TWEET_LIST, TweetListClass))
#define IS_TWEET_LIST(o)		(G_TYPE_CHECK_INSTANCE_TYPE((o), TYPE_TWEET_LIST))
#define IS_TWEET_LIST_CLASS(k)		(G_TYPE_CHECK_CLASS_TYPE((k), TYPE_TWEET_LIST))
#define TWEET_LIST_GET_CLASS(o)		(G_TYPE_INSTANCE_GET_CLASS((o), TYPE_TWEET_LIST, TweetListClass))

typedef struct _TweetList      TweetList;
typedef struct _TweetListClass TweetListClass;
typedef struct _TweetListPrivate  TweetListPrivate;

struct _TweetList {
	SexyTreeView            parent;
};

struct _TweetListClass {
	SexyTreeViewClass       parent_class;
};


enum TweetListStoreColumns{		/********************************************************************************/
	GUINT_TWEET_LIST_INDEX,		/*	The update's index in the list-store.                                   */
	GDOUBLE_TWEET_ID,		/*	Tweet's ID.								*/
	GDOUBLE_USER_ID,		/*	User's ID.								*/
	STRING_USER,			/*	Username string.							*/
	STRING_NICK,			/*	Author name string.							*/
	STRING_TEXT,			/*	Unformated Tweet string.						*/
	STRING_TWEET,			/*	Tweet for display string in the TweetList, SexyTreeView, GtkTreeView.	*/
	STRING_SEXY_TWEET,		/*	libsexy formatted Tweet for SexyTreeView's tooltip.			*/
	STRING_CREATED_AGO,		/*	'Posted ?(seconds|minutes|hours|day) ago.				*/
	STRING_CREATED_AT,		/*	Date string.								*/
	GINT_CREATED_AGO,		/*	How old the post is, in seconds, for sorting.				*/
	ULONG_CREATED_AT,		/*	Unix seconds since epoc of how old the tweet's is.			*/
	PIXBUF_AVATAR,			/*	Avatar pixbuf.								*/
	ONLINE_SERVICE,			/*	OnlineService pointer.							*/
	STRING_FROM,			/*	<status's auther @ OnlineService URI>					*/
	STRING_RCPT,			/* 	OnlineService key To: <user@service>					*/
	GUINT_SELECTED_INDEX,		/*	The associated order/place this item appears in the acciated tree view. */
};					/********************************************************************************/


GType tweet_list_get_type(void) G_GNUC_CONST;
TweetList *tweet_list_new(const gchar *timeline, OnlineService *service);
const gchar *tweet_list_get_timeline(TweetList *tweet_list);

GtkVBox *tweet_list_get_child(TweetList *tweet_list);
GtkLabel *tweet_list_get_tab(TweetList *tweet_list);
GtkLabel *tweet_list_get_menu(TweetList *tweet_list);

OnlineService *tweet_list_get_service(TweetList *tweet_list);
GtkListStore *tweet_list_get_list_store(TweetList *tweet_list);
GtkTreeModel *tweet_list_get_tree_model(TweetList *tweet_list);

void tweet_list_start(TweetList *tweet_list);
gboolean tweet_list_refresh(TweetList *tweet_list);
void tweet_list_stop(TweetList *tweet_list);
void tweet_list_complete(TweetList *tweet_list);

void tweet_list_store( TweetList *tweet_list, UserStatus *status );
void tweet_list_remove_service(TweetList *tweet_list, OnlineService *service);

void tweet_list_toggle_toolbar(TweetList *tweet_list);

void tweet_list_mark_as_read(TweetList *tweet_list);

gint8 tweet_list_has_loaded(TweetList *tweet_list);

gint tweet_list_get_page(TweetList *tweet_list);
void tweet_list_set_page(TweetList *tweet_list, gint page);

UpdateMonitor tweet_list_get_monitoring(TweetList *tweet_list);
const gchar *tweet_list_get_monitoring_string(TweetList *tweet_list);
const gchar *monitoring_to_string(UpdateMonitor monitoring);

gint tweet_list_get_total(TweetList *tweet_list);
guint tweet_list_get_notify_delay(TweetList *tweet_list);

void tweet_list_key_pressed(TweetList *tweet_list, GdkEventKey *event);
void tweet_list_set_image(TweetList *tweet_list, const gchar *image_filename, GtkTreeIter *iter);


G_END_DECLS
#endif /* __TWEET_LIST_H__ */
