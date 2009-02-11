/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2004-2007 Imendio AB
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
 * Authors: Martyn Russell <martyn@imendio.com>
 *			Brian Pepple <bpepple@fedoraproject.org>
 */

#ifndef __TWITUX_DBUS_H__
#define __TWITUX_DBUS_H__

#include <glib.h>

gboolean twitux_dbus_nm_init      (void);
void     twitux_dbus_nm_finalize  (void);
gboolean twitux_dbus_nm_get_state (gboolean *connected);

#endif /* __TWITUX_DBUS_H__ */
