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

#ifndef __TWEET_LIST_H__
#define __TWEET_LIST_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

/*
 * TwituxTweetList 
 */ 
#define TYPE_TWEET_LIST         (tweet_list_get_type ())
#define TWEET_LIST(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_TWEET_LIST, TwituxTweetList))
#define TWEET_LIST_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), TYPE_TWEET_LIST, TwituxTweetListClass))
#define IS_TWEET_LIST(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_TWEET_LIST))
#define IS_TWEET_LIST_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_TWEET_LIST))
#define TWEET_LIST_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_TWEET_LIST, TwituxTweetListClass))

typedef struct _TwituxTweetList      TwituxTweetList;
typedef struct _TwituxTweetListClass TwituxTweetListClass;
typedef struct _TwituxTweetListPriv  TwituxTweetListPriv;

struct _TwituxTweetList {
	GtkTreeView            parent;
};

struct _TwituxTweetListClass {
	GtkTreeViewClass       parent_class;
};

GType                 tweet_list_get_type           (void) G_GNUC_CONST;
TwituxTweetList *     tweet_list_new                (void);
GtkListStore *        tweet_list_get_store          (void);

G_END_DECLS

#endif /* __TWEET_LIST_H__ */
