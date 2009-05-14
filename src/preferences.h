/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2003-2007 Imendio AB
 * Copyright (C) 2007 Brian Pepple
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
 * Authors: Mikael Hallendal <micke@imendio.com>
 *          Richard Hult <richard@imendio.com>
 *          Martyn Russell <martyn@imendio.com>
 *			Brian Pepple <bpepple@fedoraproject.org>
 */

#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

/* GConf Keys */
#ifndef PREFS_PATH
#	if defined GNOME_ENABLE_DEBUG
#		define PREFS_PATH		"/apps/" PACKAGE_TARNAME "/debug"
#	else
#		define	PREFS_PATH		"/apps/" PACKAGE_TARNAME
#	endif
#endif

/* Values used & setup by the prefernces window. */
#define	PREFS_UI_NOTIFICATION			PREFS_PATH "/ui/notify"
#define	PREFS_UI_SOUND				PREFS_PATH "/ui/sound"
#define	PREFS_UI_NO_ALERT			PREFS_PATH "/ui/no_alert"

#define	PREFS_URLS_EXPAND_DISABLED		PREFS_PATH "/ui/urls/expand/disabled"
#define	PREFS_URLS_EXPAND_SELECTED_ONLY		PREFS_PATH "/ui/urls/expand/selected_only"

#define	PREFS_TWEETS_HOME_TIMELINE		PREFS_PATH "/tweets/home_timeline"
#define	PREFS_TWEETS_RELOAD_TIMELINES		PREFS_PATH "/tweets/reload_timeline"


/* UI & notification gconf values. */
#define PREFS_UI_TWEET_VIEW_USE_DIALOG		PREFS_PATH "/ui/use_tweet_dialog"
#define PREFS_UI_HIDDEN				PREFS_PATH "/ui/hidden"

#define PREFS_HINTS_CLOSE_MAIN_WINDOW		PREFS_PATH "/hints/close_main_window"

/* Window height, width, & position gconf values. */
#define PREFS_UI_WIDTH				PREFS_PATH "/ui/widths/%s"
#define PREFS_UI_HEIGHT				PREFS_PATH "/ui/heights/%s"
#define PREFS_UI_POSITION_X			PREFS_PATH "/ui/positions/%s_x"
#define PREFS_UI_POSITION_Y			PREFS_PATH "/ui/positions/%s_y"


void preferences_dialog_show (GtkWindow *parent);

G_END_DECLS

#endif /* __PREFERENCES_H__ */
