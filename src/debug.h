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

G_BEGIN_DECLS

#ifdef G_HAVE_ISO_VARARGS
#  ifdef DISABLE_DEBUG
#    define debug(...)
#  else
#    define debug(...) debug_impl (__VA_ARGS__)
#  endif
#elif defined(G_HAVE_GNUC_VARARGS)
#  if DISABLE_DEBUG
#    define debug(fmt...)
#  else
#    define debug(fmt...) debug_impl(fmt)
#  endif
#else
#  if DISABLE_DEBUG
#    define debug(x)
#  else
#    define debug debug_impl
#  endif
#endif

void debug_impl (const gchar *domain, const gchar *msg, ...);

G_END_DECLS

#endif /* __DEBUG_H__ */

