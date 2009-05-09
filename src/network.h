/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright(C) 2007-2008 Daniel Morales <daniminas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or(at your option) any later version.
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
 * Authors: Daniel Morales <daniminas@gmail.com>
 *
 */
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <gtk/gtk.h>
#include <glib.h>
#include <libsoup/soup.h>
#include "config.h"
#include "friends-manager.h"
#include "users.h"
#include "images.h"
#include "online-services.h"

/* Authenication */
#define API_LOGIN		"/account/verify_credentials.xml"

/* Twitter Timelines */
#define API_TIMELINE_PUBLIC	"/statuses/public_timeline.xml"
#define API_TIMELINE_FRIENDS	"/statuses/friends_timeline.xml"
#define API_TIMELINE_MY		"/statuses/user_timeline.xml"
#define API_TIMELINE_USER	"/statuses/user_timeline/%s.xml"
#define API_MENTIONS		"/statuses/mentions.xml"
#define API_REPLIES		"/statuses/replies.xml"
#define API_DIRECT_MESSAGES	"/direct_messages.xml"
#define API_FAVORITES		"/favorites.xml"

/* Twitter actions */
#define API_POST_STATUS		"/statuses/update.xml"
#define API_SEND_MESSAGE	"/direct_messages/new.xml"
#define API_FAVE		"/favorites/create/%s.xml"
#define API_UNFAVE		"/favorites/destroy/%s.xml"


/* Twitter relationships */
#define API_FOLLOWING		"/statuses/friends.xml"
#define API_FOLLOWERS		"/statuses/followers.xml"


#define API_USER_FOLLOW		"/friendships/create/%s.xml"
#define API_USER_UNFOLLOW	"/friendships/destroy/%s.xml"
#define API_USER_BLOCK		"/blocks/create/%s.xml"
#define API_USER_UNBLOCK	"/blocks/destroy/%s.xml"


/* Twitter Details */
#define API_ABOUT_USER		"/users/show/%s.xml"


extern gboolean getting_followers;

gchar *url_encode(const gchar *text);

/* Networking */

/* Verify user credentials */
void network_login(OnlineService **services);

/* Logout current user */
void network_logout( void );

/* Post a new tweet */
void network_post_status(const gchar *text);

/* Post a direct message to a follower */
void network_send_message(OnlineService *service, const gchar *friend, const gchar *text);

/* Get and parse a timeline */
void network_get_timeline(const gchar *uri_timeline);

/* Retrive a user timeline. If user is null, get
 * authenticated user timeline*/
void network_get_user_timeline(OnlineService *service, const gchar *username);

/* Refresh current timeline */
void network_refresh(void);

/* Copyright(C) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com> */
gboolean network_check_http(OnlineService *service, SoupMessage *msg);
GList *network_get_users_glist(gboolean get_friends);
gboolean network_download_avatar(OnlineService *service, const gchar *image_uri);

/* My, Kaity G. B., new stuff ends here. */

/* Get an image from servers */
void network_get_image(OnlineService *service, const gchar *image_uri, GtkTreeIter *iter);

#endif /*  __NETWORK_H__ */
