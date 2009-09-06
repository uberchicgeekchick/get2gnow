/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the RPL
 *
 * For more information or to find the latest release, visit our
 * website at: http://uberChicGeekChick.Com/?projects=get2gnow
 *
 * Writen by an uberChick, other uberChicks please meet me & others @:
 * 	http://uberChicks.Net/
 *
 * I'm also disabled. I live with a progressive neuro-muscular disease.
 * DYT1+ Early-Onset Generalized Dystonia, a type of Generalized Dystonia.
 * 	http://Dystonia-DREAMS.Org/
 *
 *
 *
 * Unless explicitly acquired and licensed from Licensor under another
 * license, the contents of this file are subject to the Reciprocal Public
 * License ("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
 * and You may not copy or use this file in either source code or executable
 * form, except in compliance with the terms and conditions of the RPL.
 *
 * All software distributed under the RPL is provided strictly on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, AND
 * LICENSOR HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, QUIET ENJOYMENT, OR NON-INFRINGEMENT. See the RPL for specific
 * language governing rights and limitations under the RPL.
 *
 * The User-Visible Attribution Notice below, when provided, must appear in each
 * user-visible display as defined in Section 6.4 (d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright (C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright (C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright (C) of the User the data is connected to.
 * Users may lisences their data under the terms of an OSI approved or Creative Commons
 * license.  Users must be allowed to select their choice of license for each piece of data
 * on an individual bases and cannot be blanketly applied to all of the Users.  The User may
 * select a default license for their data.  All of the Software's data pertaining to each
 * User must be fully accessible, exportable, and deletable to that User.
 */
#define _GNU_SOURCE
#define _THREAD_SAFE


#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#define _GNU_SOURCE
#define _THREAD_SAFE

#include <glib.h>
#include <gtk/gtk.h>

#include "config.h"
#include "program.h"

G_BEGIN_DECLS

#define PREFS_AUTOLOAD_BEST_FRIENDS			GCONF_PATH "/ui/start-up/tabs/best_friends"
#define	PREFS_AUTOLOAD_DMS				GCONF_PATH "/ui/start-up/tabs/dms"
#define	PREFS_AUTOLOAD_REPLIES				GCONF_PATH "/ui/start-up/tabs/replies"
#define PREFS_AUTOLOAD_FOLLOWING			GCONF_PATH "/ui/start-up/tabs/following"

#define PREFS_NOTIFY_BEST_FRIENDS			GCONF_PATH "/notifications/best_friends"
#define	PREFS_NOTIFY_DMS				GCONF_PATH "/notifications/dms"
#define PREFS_NOTIFY_FOLLOWING				GCONF_PATH "/notifications/following"
#define	PREFS_NOTIFY_REPLIES				GCONF_PATH "/notifications/replies"
#define	PREFS_NOTIFY_ALL				GCONF_PATH "/notifications/all"
#define	PREFS_NOTIFY_BEEP				GCONF_PATH "/alerts/beep"

#define	PREFS_URLS_EXPANSION_DISABLED			GCONF_PATH "/control_panel/url_expansion/disabled"
#define	PREFS_URLS_EXPANSION_SELECTED_ONLY		GCONF_PATH "/control_panel/url_expansion/selected_only"
#define PREFS_URLS_EXPANSION_REPLACE_WITH_TITLES	GCONF_PATH "/control_panel/url_expansion/replace_with_titles"
#define	PREFS_URLS_EXPANSION_USER_PROFILES		GCONF_PATH "/control_panel/url_expansion/user_profiles"

#define	PREFS_DISABLE_SYSTEM_BELL			GCONF_PATH "/alerts/disable_system_bell"
#define PREFS_CONTROL_PANEL_DIALOG			GCONF_PATH "/ui/control_panel/use_dialog"
#define PREFS_CONTROL_PANEL_COMPACT			GCONF_PATH "/ui/control_panel/compact_view"

#define	PREFS_DISABLE_UPDATE_LENGTH_ALERT		GCONF_PATH "/alerts/disable_update_length_alert"

#define	PREFS_TWEETS_ARCHIVE_DMS			GCONF_PATH "/online-services/xml-cache/archive/expiration/dms"
#define	PREFS_TWEETS_ARCHIVE_REPLIES			GCONF_PATH "/online-services/xml-cache/archive/expiration/replies"
#define	PREFS_TWEETS_ARCHIVE_FAVES			GCONF_PATH "/online-services/xml-cache/archive/expiration/best_friends"
#define	PREFS_TWEETS_ARCHIVE_BEST_FRIENDS		GCONF_PATH "/online-services/xml-cache/archive/expiration/best_friends"

#define	PREFS_TWEETS_HOME_TIMELINE			GCONF_PATH "/updates/home_timeline"
#define	PREFS_TWEETS_RELOAD_TIMELINES			GCONF_PATH "/updates/reload_timeline"

#define PREFS_TWEETS_NO_PROFILE_LINK			GCONF_PATH "/updates/no_link_in_replies"
#define	PREFS_TWEETS_DIRECT_REPLY_ONLY			GCONF_PATH "/updates/direct_reply_only"


void preferences_dialog_show (GtkWindow *parent);

G_END_DECLS

#endif /* __PREFERENCES_H__ */
