/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2002-2007 Imendio AB
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
 */
 
#ifndef __TWITUX_HINT_H__
#define __TWITUX_HINT_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

gboolean   twitux_hint_dialog_show	(const gchar         *conf_path,
									 const gchar         *message1,
									 const gchar         *message2,
									 GtkWindow           *parent,
									 GFunc                func,
									 gpointer             user_data);
gboolean   twitux_hint_show			(const gchar         *conf_path,
									 const gchar         *message1,
									 const gchar         *message2,
									 GtkWindow           *parent,
									 GFunc                func,
									 gpointer             user_data);

G_END_DECLS

#endif /* __TWITUX_HINT_H__ */
