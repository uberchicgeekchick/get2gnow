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
 */

#ifndef __POPUP_DIALOG_H__
#define __POPUP_DIALOG_H__

#include <gtk/gtk.h>

void popup_select_service( GtkWindow *parent );
void popup_friend_profile( GtkWindow *parent );
void popup_friend_tweets( GtkWindow *parent );
void popup_friend_follow( GtkWindow *parent );
void popup_friend_unfollow( GtkWindow *parent );
void popup_friend_block( GtkWindow *parent );
void popup_friend_unblock( GtkWindow *parent );

#endif /* __POPUP_DIALOG_H__ */
