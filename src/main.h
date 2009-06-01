/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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
 * Authors: Kaity G. B. <uberChick@uberChicGeekChick.Com>
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <glib.h>
#include "config.h"

G_BEGIN_DECLS

#define STATUSBAR_DEFAULT "HotKeys: [Ctrl+N] start a new tweet; [Ctrl+D] or [Shift+Return] to DM; [Ctrl+R], [Return], or '@' to reply, [Ctrl+F] or '>' to re-tweet."

#ifndef G_STR_EMPTY
#define G_STR_EMPTY(x) ((x) == NULL || (x)[0] == '\0')
#endif


#ifndef G_STR_N_EMPTY
#define G_STR_N_EMPTY(x) (!((x) == NULL || (x)[0] == '\0'))
#endif

#ifndef uber_free
#define uber_free(mem)	{ g_free(mem); mem=NULL; }
#endif

#ifndef uber_unref
#define uber_unref(o)	{ g_object_unref(o); o=NULL; }
#endif

#ifndef	uber_list_free
#define	uber_list_free(l)	{ g_list_free(l); l=NULL; }
#endif

G_END_DECLS

#endif /* _MAIN_H_ */
