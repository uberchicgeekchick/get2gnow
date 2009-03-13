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
 *          Richard Hult <richard@imendio.com>
 *          Brian Pepple <bpepple@fedoraproject.org>
 */

#ifndef __SPELL_H__
#define __SPELL_H__

G_BEGIN_DECLS

gboolean     spell_supported           (void);
const gchar *spell_get_language_name   (const gchar *code);
GList       *spell_get_language_codes  (void);
void         spell_free_language_codes (GList       *codes);
gboolean     spell_check               (const gchar *word);
GList *      spell_get_suggestions     (const gchar *word);
void         spell_free_suggestions    (GList       *suggestions);

G_END_DECLS

#endif /* __SPELL_H__ */
