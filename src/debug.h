/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2006 Imendio AB
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


#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifndef	_GNU_SOURCE 
#	define _GNU_SOURCE
#endif

#ifndef _THREAD_SAFE
#	define _THREAD_SAFE
#endif

#include <glib.h>
#include <glib/gprintf.h>


G_BEGIN_DECLS

#ifndef DEBUG_DOMAINS
#	if defined GNOME_ENABLE_DEBUG
#		define DEBUG_DOMAINS "All"
#	else
#		define DEBUG_DOMAINS ""
#	endif
#endif


#ifndef debug
#	if defined(G_HAVE_ISO_VARARGS)
#		define debug(...)	debug_printf(DEBUG_DOMAINS, __FILE__, __func__, __LINE__, __VA_ARGS__)
#	elif defined(G_HAVE_GNUC_VARARGS)
#		define debug(fmt...)	debug_printf(DEBUG_DOMAINS, __FILE__, __func__, __LINE__, fmt)
#	else
#		define debug(x)		debug_printf(DEBUG_DOMAINS, __FILE__, __func__, __LINE__, x)
#	endif
#endif


#ifndef IF_DEBUG
#	define IF_DEBUG		if(debug_if_domain(DEBUG_DOMAINS))
#endif

#ifndef IF_DEVEL
#	define	IF_DEVEL		if(debug_if_devel())
#endif

void debug_init(void);

gboolean debug_if_devel(void);
gboolean debug_check_devel(const gchar *debug_environmental_value);

void debug_printf(const gchar *domain, const gchar *source_code, const gchar *method, const gint line_number, const gchar *msg, ...) G_GNUC_PRINTF(5, 6);

gboolean debug_if_domain(const gchar *domain);

void debug_deinit(void);

G_END_DECLS
#endif /* __DEBUG_H__ */

