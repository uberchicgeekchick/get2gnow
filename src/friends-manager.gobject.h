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

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define	TYPE_FRIENDS_MANAGER		(friends_manager_get_type())
#define	FRIENDS_MANAGER(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_FRIENDS_MANAGER, FriendsManager))
#define	FRIENDS_MANAGER_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), TYPE_FRIENDS_MANAGER, FriendsManagerClass))
#define	IS_FRIENDS_MANAGER(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_FRIENDS_MANAGER))
#define	IS_FRIENDS_MANAGER_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_FRIENDS_MANAGER))
#define	IS_FRIENDS_MANAGER_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_FRIENDS_MANAGER, FriendsManagerClass))

typedef struct{
	GObject        parent;
} FriendsManager;

typedef struct {
	GObjectClass parent_class;
} FriendsManagerClass;

GType friends_manager_get_type(void) G_GNUC_CONST;
void friends_manager_show(GtkWindow *parent);

G_END_DECLS

#endif /* __FRIENDS_MANAGER_H__ */
