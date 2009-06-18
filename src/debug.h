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


#ifdef DISABLE_DEBUG
#	define debug
#elif defined(G_HAVE_ISO_VARARGS)
#	define debug(...)	debug_impl(DEBUG_DOMAINS, __VA_ARGS__)
#elif defined(G_HAVE_GNUC_VARARGS)
#	define debug(fmt...)	debug_impl(DEBUG_DOMAINS, fmt)
#else
#	define debug(x)		debug_impl(DEBUG_DOMAINS, x)
#endif


#ifdef DISABLE_DEBUG
#	define IF_DEBUG
#else
#	define IF_DEBUG		debug_if_domain(DEBUG_DOMAINS)
#endif

#define	check_devel()	debug_check_devel()

void debug_init(void);
gboolean debug_check_devel(void);
void debug_impl(const gchar *domain, const gchar *msg, ...) G_GNUC_PRINTF(2, 3);
gboolean debug_if_domain(const gchar *domain);
void debug_deinit(void);

G_END_DECLS

#endif /* __DEBUG_H__ */

