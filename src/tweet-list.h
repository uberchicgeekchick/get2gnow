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

#include <gtk/gtk.h>
#include <libsexy/sexy.h>
#include "online-services.h"
#include "users.h"

G_BEGIN_DECLS

/*
 * TweetList 
 */ 
#define TYPE_TWEET_LIST			(tweet_list_get_type())
#define TWEET_LIST(o)			(G_TYPE_CHECK_INSTANCE_CAST((o), TYPE_TWEET_LIST, TweetList))
#define TWEET_LIST_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), TYPE_TWEET_LIST, TweetListClass))
#define IS_TWEET_LIST(o)		(G_TYPE_CHECK_INSTANCE_TYPE((o), TYPE_TWEET_LIST))
#define IS_TWEET_LIST_CLASS(k)		(G_TYPE_CHECK_CLASS_TYPE((k), TYPE_TWEET_LIST))
#define TWEET_LIST_GET_CLASS(o)		(G_TYPE_INSTANCE_GET_CLASS((o), TYPE_TWEET_LIST, TweetListClass))

typedef struct _TweetList      TweetList;
typedef struct _TweetListClass TweetListClass;
typedef struct _TweetListPriv  TweetListPriv;

struct _TweetList {
	SexyTreeView            parent;
	
	gint			index;
	guint			total;
};

struct _TweetListClass {
	SexyTreeViewClass       parent_class;
};

extern guint tweet_list_notify_delay;

GType		tweet_list_get_type(void) G_GNUC_CONST;
TweetList	*tweet_list_new(void);
TweetList	*tweet_list_get(void);

void tweet_list_clear(void);

void tweet_list_store_status(OnlineService *service, UserStatus *status);

void tweet_list_key_pressed(GtkWidget *widget, GdkEventKey *event);

void tweet_list_goto_top(void);

void tweet_list_set_image(const gchar *image_filename, GtkTreeIter *iter);

G_END_DECLS

#endif /* __TWEET_LIST_H__ */
