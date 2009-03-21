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
#include "gconf.h"

#include "main.h"
#include "geometry.h"

#define DEBUG_DOMAIN "Geometry"

void
geometry_save_for_main_window (gint x, gint y,
									  gint w, gint h)
{
	Conf *conf;

	debug (DEBUG_DOMAIN, "Saving for main window: x:%d, y:%d, w:%d, h:%d",
				  x, y, w, h);

	conf = conf_get ();

	conf_set_int (conf,
						 PREFS_UI_WINDOW_HEIGHT,
						 h);

	conf_set_int (conf,
						 PREFS_UI_WINDOW_WIDTH,
						 w);

	conf_set_int (conf,
						 PREFS_UI_WIN_POS_X,
						 x);

	conf_set_int (conf,
						 PREFS_UI_WIN_POS_Y,
						 y);

}
 
void
geometry_load_for_main_window (GtkWidget *main_window)
{
	Conf *conf;
	gint        x, y, w, h;

	debug (DEBUG_DOMAIN, "Loading window geometry...");

	conf = conf_get ();

	conf_get_int (conf,
						 PREFS_UI_WINDOW_HEIGHT,
						 &h);

	conf_get_int (conf,
						 PREFS_UI_WINDOW_WIDTH,
						 &w);

	conf_get_int (conf,
						 PREFS_UI_WIN_POS_X,
						 &x);

	conf_get_int (conf,
						 PREFS_UI_WIN_POS_Y,
						 &y);

	if (w >=1 && h >= 1) {
		/*
		 * Use the defaults from the glade file
		 * if we don't have good w, h geometry.
		 */
		 debug (DEBUG_DOMAIN,
					   "Configuring window default size w:%d, h: %d", w, h);
		 gtk_window_resize (GTK_WINDOW (main_window), w, h);
	}

	if (x >= 0 && y >= 0) {
		/*
		 * Let the window manager position it
		 * if we don't have good x, y coordinates.
		 */
		debug (DEBUG_DOMAIN,
					  "Configuring window default position x:%d, y:%d", x, y);
		gtk_window_move (GTK_WINDOW (main_window), x, y);
	}
 }
