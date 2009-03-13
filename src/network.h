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
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <gtk/gtk.h>
#include "parser.h"

/* Twitter API */
#define API_REPLIES		"https://twitter.com/statuses/replies.xml"
#define API_DIRECT_MESSAGES	"https://twitter.com/direct_messages.xml"

#define API_POST_STATUS		"https://twitter.com/statuses/update.xml"
#define API_SEND_MESSAGE		"https://twitter.com/direct_messages/new.xml"

#define API_FOLLOWING		"https://twitter.com/statuses/friends.xml"
#define API_FOLLOWERS		"https://twitter.com/statuses/followers.xml"

#define API_FOLLOWING_ADD	"https://twitter.com/friendships/create/%s.xml"
#define API_FOLLOWING_DEL	"https://twitter.com/friendships/destroy/%s.xml"

#define API_LOGIN		"https://twitter.com/account/verify_credentials.xml"

/* Verify user credentials */
void network_login		(const char *username, 
								 const char *password);

/* Logout current user */
void network_logout 		(void);

/* Post a new tweet */
void network_post_status		(const gchar *text);

/* Post a direct message to a follower */
void network_send_message	(const gchar *friend,
					 const gchar *text);

/* Get and parse a timeline */
void network_get_timeline 	(const gchar *url_timeline);

/* Retrive a user timeline. If user is null, get
 * authenticated user timeline*/
void network_get_user		(const gchar *username);

/* Refresh current timeline */
void network_refresh			(void);

/* Get authenticating user's friends(following) */
GList *network_get_friends	(void);

/* Get the authenticating user's followers */
GList *network_get_followers	(void);

/* Get an image from servers */
void network_get_image (const gchar *url_image,
							   GtkTreeIter iter);

/* Add a user to follow */
void network_add_user		(const gchar *username);

/* Remove a user */
void network_del_user		(TwituxUser *user);

/* Networking */
void network_new			(void);

void network_close		(void);

void network_stop		(void);


#endif /*  __NETWORK_H__ */
