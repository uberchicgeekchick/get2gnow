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
#ifndef __PARSER_H__
#define __PARSER_H__

#include <gtk/gtk.h>
#include <libxml/parser.h>

#include <libsoup/soup-message.h>

#include "online-services.h"

gchar *parser_get_cache_file_from_uri(const gchar *uri);

xmlDoc *parser_parse(SoupMessage *xml, xmlNode **first_element);

gboolean parser_timeline(OnlineService *service, SoupMessage *xml);
gchar *parser_parse_xpath_content(SoupMessage *xml, const gchar *xpath);

/* Restet the ID of the last tweet showed */
void parser_reset_lastid( void );

#endif /*  __PARSER_H__ */
