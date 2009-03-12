/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2007 Brian Pepple <bpepple@fedoraproject.org>
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

#ifndef __TWITUX_AVATAR_H__
#define __TWITUX_AVATAR_H__

#include <glib-object.h>

#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

#define TWITUX_TYPE_AVATAR (twitux_avatar_get_gtype ())

typedef struct _TwituxAvatar TwituxAvatar;

struct _TwituxAvatar {
	guchar    *data;
	gsize      len;
	gchar     *format;
	GdkPixbuf *pixbuf;
	guint      refcount;
};

GType          twitux_avatar_get_gtype               (void) G_GNUC_CONST;
TwituxAvatar * twitux_avatar_new                     (guchar *avatar,
													  gsize len,
													  gchar *format);
GdkPixbuf    * twitux_avatar_get_pixbuf              (TwituxAvatar *avatar);
GdkPixbuf    * twitux_avatar_create_pixbuf_with_size (TwituxAvatar *avatar,
													  gint          size);

TwituxAvatar * twitux_avatar_ref                     (TwituxAvatar *avatar);
void           twitux_avatar_unref                   (TwituxAvatar *avatar);

G_END_DECLS

#endif /* __TWITUX_AVATAR_H__ */
