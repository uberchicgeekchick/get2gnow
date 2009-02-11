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

#include "config.h"

#include "twitux-avatar.h"

#define AVATAR_SIZE 32

static gboolean
avatar_pixbuf_is_opaque (GdkPixbuf *pixbuf)
{
	int            width;
	int            height;
	int            rowstride; 
	int            i;
	unsigned char *pixels;
	unsigned char *row;

	if (!gdk_pixbuf_get_has_alpha(pixbuf)) {
		return TRUE;
	}

	width     = gdk_pixbuf_get_width (pixbuf);
	height    = gdk_pixbuf_get_height (pixbuf);
	rowstride = gdk_pixbuf_get_rowstride (pixbuf);
	pixels    = gdk_pixbuf_get_pixels (pixbuf);

	row = pixels;
	for (i = 3; i < rowstride; i+=4) {
		if (row[i] != 0xff) {
			return FALSE;
		}
	}

	for (i = 1; i < height - 1; i++) {
		row = pixels + (i * rowstride);
		if (row[3] != 0xff || row[rowstride-1] != 0xff) {
			return FALSE;
		}
	}

	row = pixels + ((height-1) * rowstride);
	for (i = 3; i < rowstride; i+=4) {
		if (row[i] != 0xff) {
			return FALSE;
		}
	}

	return TRUE;
}

/* From Pidgin */
static void
avatar_pixbuf_roundify (GdkPixbuf *pixbuf)
{
	int     width;
	int     height;
	int     rowstride;
	guchar *pixels;

	if (!gdk_pixbuf_get_has_alpha (pixbuf)) {
		return;
	}

	width     = gdk_pixbuf_get_width (pixbuf);
	height    = gdk_pixbuf_get_height (pixbuf);
	rowstride = gdk_pixbuf_get_rowstride (pixbuf);
	pixels    = gdk_pixbuf_get_pixels (pixbuf);

	if (width < 6 || height < 6) {
		return;
	}

	/* Top left */
	pixels[3] = 0;
	pixels[7] = 0x80;
	pixels[11] = 0xC0;
	pixels[rowstride + 3] = 0x80;
	pixels[rowstride * 2 + 3] = 0xC0;

	/* Top right */
	pixels[width * 4 - 1] = 0;
	pixels[width * 4 - 5] = 0x80;
	pixels[width * 4 - 9] = 0xC0;
	pixels[rowstride + (width * 4) - 1] = 0x80;
	pixels[(2 * rowstride) + (width * 4) - 1] = 0xC0;

	/* Bottom left */
	pixels[(height - 1) * rowstride + 3] = 0;
	pixels[(height - 1) * rowstride + 7] = 0x80;
	pixels[(height - 1) * rowstride + 11] = 0xC0;
	pixels[(height - 2) * rowstride + 3] = 0x80;
	pixels[(height - 3) * rowstride + 3] = 0xC0;

	/* Bottom right */
	pixels[height * rowstride - 1] = 0;
	pixels[(height - 1) * rowstride - 1] = 0x80;
	pixels[(height - 2) * rowstride - 1] = 0xC0;
	pixels[height * rowstride - 5] = 0x80;
	pixels[height * rowstride - 9] = 0xC0;
}

GType
twitux_avatar_get_gtype (void)
{
	static GType type_id = 0;

	if (!type_id) {
		type_id = g_boxed_type_register_static ("TwituxAvatar",
												(GBoxedCopyFunc) twitux_avatar_ref,
												(GBoxedFreeFunc) twitux_avatar_unref);
	}

	return type_id;
}

TwituxAvatar *
twitux_avatar_new (guchar *data,
				   gsize   len,
				   gchar  *format)
{
	TwituxAvatar *avatar;

	g_return_val_if_fail (data != NULL, NULL);
	g_return_val_if_fail (len > 0, NULL);
	g_return_val_if_fail (format != NULL, NULL);

	avatar = g_slice_new0 (TwituxAvatar);
	avatar->data = g_memdup (data, len);
	avatar->len = len;
	avatar->format = g_strdup (format);
	avatar->refcount = 1;

	return avatar;
}

static GdkPixbuf *
avatar_create_pixbuf (TwituxAvatar *avatar, gint size)
{
	GdkPixbuf       *tmp_pixbuf;
	GdkPixbuf       *ret_pixbuf;
	GdkPixbufLoader *loader;
	GError          *error = NULL;
	int              orig_width;
	int              orig_height;
	int              scale_width;
	int              scale_height;

	if (!avatar) {
		return NULL;
	}

	loader = gdk_pixbuf_loader_new ();

	if (!gdk_pixbuf_loader_write (loader, avatar->data, avatar->len, &error)) {
		g_warning ("Couldn't write avatar image: %p with "
				   "length: %" G_GSIZE_FORMAT " to pixbuf loader: %s",
				   avatar->data, avatar->len, error->message);
		g_error_free (error);

		return NULL;
	}

	gdk_pixbuf_loader_close (loader, NULL);

	tmp_pixbuf = gdk_pixbuf_loader_get_pixbuf (loader);
	scale_width = orig_width = gdk_pixbuf_get_width (tmp_pixbuf);
	scale_height = orig_height = gdk_pixbuf_get_height (tmp_pixbuf);
	if (scale_height > scale_width) {
		scale_width = (gdouble) size * (gdouble) scale_width / (double) scale_height;
		scale_height = size;
	} else {
		scale_height = (gdouble) size * (gdouble) scale_height / (gdouble) scale_width;
		scale_width = size;
	}
	ret_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, 32, 32);
	gdk_pixbuf_fill (ret_pixbuf, 0x00000000);
	gdk_pixbuf_scale (tmp_pixbuf, ret_pixbuf, 
					  (size - scale_width)/2,
					  (size - scale_height)/2,
					  scale_width,
					  scale_height,
					  (size - scale_width)/2,
					  (size - scale_height)/2,
					  (double)scale_width/(double)orig_width,
					  (double)scale_height/(double)orig_height,
					  GDK_INTERP_BILINEAR);

	if (avatar_pixbuf_is_opaque (ret_pixbuf)) {
		avatar_pixbuf_roundify (ret_pixbuf);
	}

	g_object_unref (loader);

	return ret_pixbuf;
}

GdkPixbuf *
twitux_avatar_get_pixbuf (TwituxAvatar *avatar)
{
	g_return_val_if_fail (avatar != NULL, NULL);

	if (!avatar->pixbuf) {
		avatar->pixbuf = avatar_create_pixbuf (avatar, AVATAR_SIZE);
	}

	return avatar->pixbuf;
}

GdkPixbuf *
twitux_avatar_create_pixbuf_with_size (TwituxAvatar *avatar, gint size)
{
	return avatar_create_pixbuf (avatar, size);
}

void
twitux_avatar_unref (TwituxAvatar *avatar)
{
	g_return_if_fail (avatar != NULL);

	avatar->refcount--;

	if (avatar->refcount == 0) {
		g_free (avatar->data);
		g_free (avatar->format);
		if (avatar->pixbuf) {
			g_object_unref (avatar->pixbuf);
		}

		g_slice_free (TwituxAvatar, avatar);
	}
}

TwituxAvatar *
twitux_avatar_ref (TwituxAvatar *avatar)
{
	g_return_val_if_fail (avatar != NULL, NULL);

	avatar->refcount++;

	return avatar;
}
