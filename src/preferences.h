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

#define _GNU_SOURCE
#define _THREAD_SAFE

#include <glib.h>
#include <gtk/gtk.h>

#include "main.h"

G_BEGIN_DECLS

/* Values used & setup by the prefernces window. */
#define	PREFS_NOTIFY_ALL			GCONF_PATH "/notifications/all"
#define PREFS_NOTIFY_MY_FRIENDS_TWEETS		GCONF_PATH "/notifications/my_friends_tweets"
#define	PREFS_NOTIFY_REPLIES			GCONF_PATH "/notifications/replies"
#define	PREFS_NOTIFY_DMS			GCONF_PATH "/notifications/dms"
#define	PREFS_NOTIFY_BEEP			GCONF_PATH "/alerts/beep"

#define	PREFS_URLS_EXPAND_SELECTED_ONLY		GCONF_PATH "/ui/urls/expand/selected_only"
#define PREFS_URLS_EXPAND_REPLACE_WITH_TITLES	GCONF_PATH "/ui/urls/expand/replace_with_titles"
#define	PREFS_URLS_EXPAND_USER_PROFILES		GCONF_PATH "/ui/urls/expand/user_profiles"
#define	PREFS_URLS_EXPAND_DISABLED		GCONF_PATH "/ui/urls/expand/disabled"

#define	PREFS_DISABLE_SYSTEM_BELL		GCONF_PATH "/alerts/disable_system_bell"
#define PREFS_TWEET_VIEW_DIALOG			GCONF_PATH "/ui/tweet_view/use_dialog"

#define	PREFS_TWEET_LENGTH_ALERT		GCONF_PATH "/alerts/no_length_beep"

#define	PREFS_TWEETS_HOME_TIMELINE		GCONF_PATH "/tweets/home_timeline"
#define	PREFS_TWEETS_RELOAD_TIMELINES		GCONF_PATH "/tweets/reload_timeline"
#define PREFS_TWEETS_NO_PROFILE_LINK		GCONF_PATH "/tweets/no_link_in_replies"
#define	PREFS_TWEETS_DIRECT_REPLY_ONLY		GCONF_PATH "/tweets/direct_reply_only"


void preferences_dialog_show (GtkWindow *parent);

G_END_DECLS

#endif /* __PREFERENCES_H__ */
