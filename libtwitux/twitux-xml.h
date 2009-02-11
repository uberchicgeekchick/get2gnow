/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2002-2007 Imendio AB
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
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __TWITUX_XML_H__
#define __TWITUX_XML_H__

#include <gtk/gtk.h>

GtkBuilder *twitux_xml_get_file (const gchar *filename,
                                 const gchar *first_widget,
                                 ...);

void        twitux_xml_connect  (GtkBuilder  *gui,
                                 gpointer     user_data,
                                 gchar       *first_widget,
                                 ...);

#endif /*  __TWITUX_XML_H__ */

