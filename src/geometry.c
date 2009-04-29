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
 *
 * Authors: Brian Pepple <bpepple@fedoraproject.org>
 */

#include "config.h"

#include "debug.h"
#include "gconfig.h"

#include "preferences.h"
#include "geometry.h"

#define DEBUG_DOMAIN "Geometry"

void
geometry_save_for_main_window (gint x, gint y,
									  gint w, gint h)
{
	GConfig *gconfig=gconfig_get();

	debug (DEBUG_DOMAIN, "Saving for main window: x:%d, y:%d, w:%d, h:%d",
				  x, y, w, h);

	gconfig_set_int (gconfig,
						 PREFS_UI_WINDOW_HEIGHT,
						 h);

	gconfig_set_int (gconfig,
						 PREFS_UI_WINDOW_WIDTH,
						 w);

	gconfig_set_int (gconfig,
						 PREFS_UI_WIN_POS_X,
						 x);

	gconfig_set_int (gconfig,
						 PREFS_UI_WIN_POS_Y,
						 y);

}
 
void
geometry_load_for_main_window (GtkWindow *main_window)
{
	GConfig	*gconfig=gconfig_get();
	gint	x=0, y=0, w=0, h=0;

	debug (DEBUG_DOMAIN, "Loading window geometry...");
	

	gconfig_get_int(gconfig, PREFS_UI_WINDOW_HEIGHT, &h);
	gconfig_get_int(gconfig, PREFS_UI_WINDOW_WIDTH, &w);

	gconfig_get_int(gconfig, PREFS_UI_WIN_POS_X, &x);
	gconfig_get_int(gconfig, PREFS_UI_WIN_POS_Y, &y);

	if(!(w >0 && h > 0 && x > 0 && y > 0))
		return;

	debug(DEBUG_DOMAIN, "Resizing window:  width:%d, height: %d", w, h);
	gtk_window_resize( GTK_WINDOW (main_window), w, h);

	debug( DEBUG_DOMAIN, "Moving window to: x:%d, y:%d", x, y);
	gtk_window_move( GTK_WINDOW (main_window), x, y);
 }
