/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2008 Daniel Morales <daniminas@gmail.com>
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

#ifndef __FRIENDS_MANAGER_H__
#define __FRIENDS_MANAGER_H__

#define _GNU_SOURCE
#define _THREAD_SAFE

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

void friends_manager_show(GtkWindow *parent);
void friends_manager_display_following_and_followers(GList *friends_and_followers);

G_END_DECLS
#endif /* __FRIENDS_MANAGER_H__ */

