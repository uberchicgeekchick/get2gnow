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
#ifndef __TWITUX_PARSER_H__
#define __TWITUX_PARSER_H__

#include <gtk/gtk.h>

typedef struct {
	gchar	*screen_name;
	gchar	*name;
	gchar	*image_url;
} TwituxUser;


/* Returns a liststore for the main treeview to show tweets */
gboolean twitux_parser_timeline (const gchar *data, 
								 gssize       length);

/* Returns a Glist with friends. Can be used to 
   build the friends menu, on direct messages dialog, etc.. */
GList *twitux_parser_users_list (const gchar *data,
								 gssize       length);

/* Parse a xml user node. Ex: add/del users responses */
TwituxUser *twitux_parser_single_user (const gchar *data,
									   gssize       length);

/* To free a User struct */
void parser_free_user (TwituxUser *user);

/* Restet the ID of the last tweet showed */
void parser_reset_lastid (void);

#endif /*  __TWITUX_PARSER_H__ */
