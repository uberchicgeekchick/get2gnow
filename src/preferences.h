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

#include "config.h"
#include "program.h"

G_BEGIN_DECLS

#define	PREFS_NOTIFY_ALL				GCONF_PATH "/notifications/all"
#define PREFS_NOTIFY_FOLLOWING				GCONF_PATH "/notifications/following"
#define PREFS_NOTIFY_BEST_FRIENDS			GCONF_PATH "/notifications/best_friends"
#define	PREFS_NOTIFY_REPLIES				GCONF_PATH "/notifications/replies"
#define	PREFS_NOTIFY_DMS				GCONF_PATH "/notifications/dms"
#define	PREFS_NOTIFY_BEEP				GCONF_PATH "/alerts/beep"

#define	PREFS_URLS_EXPANSION_DISABLED			GCONF_PATH "/tweet_view/url_expansion/disabled"
#define	PREFS_URLS_EXPANSION_SELECTED_ONLY		GCONF_PATH "/tweet_view/url_expansion/selected_only"
#define PREFS_URLS_EXPANSION_REPLACE_WITH_TITLES	GCONF_PATH "/tweet_view/url_expansion/replace_with_titles"
#define	PREFS_URLS_EXPANSION_USER_PROFILES		GCONF_PATH "/tweet_view/url_expansion/user_profiles"

#define	PREFS_DISABLE_SYSTEM_BELL			GCONF_PATH "/alerts/disable_system_bell"
#define PREFS_TWEET_VIEW_DIALOG				GCONF_PATH "/ui/tweet_view_dialog"

#define	PREFS_DISABLE_UPDATE_LENGTH_ALERT		GCONF_PATH "/alerts/disable_update_length_alert"

#define	PREFS_TWEETS_ARCHIVE_DMS			GCONF_PATH "/online-services/xml-cache/archive/expiration/dms"
#define	PREFS_TWEETS_ARCHIVE_REPLIES			GCONF_PATH "/online-services/xml-cache/archive/expiration/replies"
#define	PREFS_TWEETS_ARCHIVE_BEST_FRIENDS		GCONF_PATH "/online-services/xml-cache/archive/expiration/best_friends"

#define	PREFS_TWEETS_HOME_TIMELINE			GCONF_PATH "/tweets/home_timeline"
#define	PREFS_TWEETS_RELOAD_TIMELINES			GCONF_PATH "/tweets/reload_timeline"

#define PREFS_TWEETS_NO_PROFILE_LINK			GCONF_PATH "/tweets/no_link_in_replies"
#define	PREFS_TWEETS_DIRECT_REPLY_ONLY			GCONF_PATH "/tweets/direct_reply_only"


void preferences_dialog_show (GtkWindow *parent);

G_END_DECLS

#endif /* __PREFERENCES_H__ */
