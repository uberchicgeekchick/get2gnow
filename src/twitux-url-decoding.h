/* -*- Mode: C; tab-width: 8; shiftwidth: 8; indent-tabs-mode: t; c-basic-offset: r -*- */
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
 * Authors: Kaity G. B. <uberChicGeekChick@openSUSE.us>
 */
	
#ifndef __TWITUX_URL_ENCODING_H__
#define __TWITUX_URL_ENCODING_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

gchar * url_encode_message(gchar *text);
GString * url_decode_message(gchar *text);

G_END_DECLS

#endif /* __TWITUX_URL_ENCODING_H__ */

