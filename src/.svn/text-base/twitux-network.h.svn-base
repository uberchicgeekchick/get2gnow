/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2007-2008 Daniel Morales <daniminas@gmail.com>
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
 * Authors: Daniel Morales <daniminas@gmail.com>
 *
 */
#ifndef __TWITUX_NETWORK_H__
#define __TWITUX_NETWORK_H__

#include <gtk/gtk.h>
#include "twitux-parser.h"

/* Twitter API */
#define TWITUX_API_REPLIES		"https://twitter.com/statuses/replies.xml"
#define TWITUX_API_DIRECT_MESSAGES	"https://twitter.com/direct_messages.xml"

#define TWITUX_API_POST_STATUS		"https://twitter.com/statuses/update.xml"
#define TWITUX_API_SEND_MESSAGE		"https://twitter.com/direct_messages/new.xml"

#define TWITUX_API_FOLLOWING		"https://twitter.com/statuses/friends.xml"
#define TWITUX_API_FOLLOWERS		"https://twitter.com/statuses/followers.xml"

#define TWITUX_API_FOLLOWING_ADD	"https://twitter.com/friendships/create/%s.xml"
#define TWITUX_API_FOLLOWING_DEL	"https://twitter.com/friendships/destroy/%s.xml"

#define TWITUX_API_LOGIN		"https://twitter.com/account/verify_credentials.xml"

/* Verify user credentials */
void twitux_network_login		(const char *username, 
								 const char *password);

/* Logout current user */
void twitux_network_logout 		(void);

/* Post a new tweet */
void twitux_network_post_status		(const gchar *text);

/* Post a direct message to a follower */
void twitux_network_send_message	(const gchar *friend,
					 const gchar *text);

/* Get and parse a timeline */
void twitux_network_get_timeline 	(const gchar *url_timeline);

/* Retrive a user timeline. If user is null, get
 * authenticated user timeline*/
void twitux_network_get_user		(const gchar *username);

/* Refresh current timeline */
void twitux_network_refresh			(void);

/* Get authenticating user's friends(following) */
GList *twitux_network_get_friends	(void);

/* Get the authenticating user's followers */
GList *twitux_network_get_followers	(void);

/* Get an image from servers */
void twitux_network_get_image (const gchar *url_image,
							   GtkTreeIter iter);

/* Add a user to follow */
void twitux_network_add_user		(const gchar *username);

/* Remove a user */
void twitux_network_del_user		(TwituxUser *user);

/* Networking */
void twitux_network_new			(void);

void twitux_network_close		(void);

void twitux_network_stop		(void);


#endif /*  __TWITUX_NETWORK_H__ */
