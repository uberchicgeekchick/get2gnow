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
#define	PREFS_NOTIFY_ALL			PREFS_PATH "/notifications/all"
#define	PREFS_NOTIFY_REPLIES			PREFS_PATH "/notifications/replies"
#define	PREFS_NOTIFY_DMS			PREFS_PATH "/notifications/dms"
#define	PREFS_NOTIFY_BEEP			PREFS_PATH "/notifications/sound"

#define	PREFS_URLS_EXPAND_SELECTED_ONLY		PREFS_PATH "/ui/urls/expand/selected_only"
#define PREFS_URLS_EXPAND_REPLACE_WITH_TITLES	PREFS_PATH "/ui/urls/expand/replace_with_titles"
#define	PREFS_URLS_EXPAND_USER_PROFILES		PREFS_PATH "/ui/urls/expand/user_profiles"
#define	PREFS_URLS_EXPAND_DISABLED		PREFS_PATH "/ui/urls/expand/disabled"

#define PREFS_TWEET_VIEW_DIALOG			PREFS_PATH "/ui/tweet_view/use_dialog"
#define	PREFS_TWEET_LENGTH_ALERT		PREFS_PATH "/tbeepweets/no_length_alert"

#define	PREFS_TWEETS_HOME_TIMELINE		PREFS_PATH "/tweets/home_timeline"
#define	PREFS_TWEETS_RELOAD_TIMELINES		PREFS_PATH "/tweets/reload_timeline"
#define PREFS_TWEETS_NO_PROFILE_LINK		PREFS_PATH "/tweets/no_link_in_replies"
#define	PREFS_TWEETS_DIRECT_REPLY_ONLY		PREFS_PATH "/tweets/direct_reply_only"


void preferences_dialog_show (GtkWindow *parent);

G_END_DECLS

#endif /* __PREFERENCES_H__ */
