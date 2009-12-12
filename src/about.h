/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
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
 */
#ifndef __ABOUT_DIALOG_H__
#define __ABOUT_DIALOG_H__

#ifndef	_GNU_SOURCE 
#	define _GNU_SOURCE
#endif

#ifndef _THREAD_SAFE
#	define _THREAD_SAFE
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

void about_dialog_new (GtkWindow *parent);

G_END_DECLS
#endif /*  __ABOUT_DIALOG_H__ */
