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
#ifndef __PREFERENCES_DEFINES_H__
#define __PREFERENCES_DEFINES_H__

#include "config.h"

G_BEGIN_DECLS
#ifndef GCONF_PATH
#	if defined GNOME_ENABLE_DEBUG
#		define	GCONF_PATH				"/apps/" GETTEXT_PACKAGE "-debug"
#	else
#		define	GCONF_PATH				"/apps/" GETTEXT_PACKAGE
#	endif
#endif


#define	PREFS_TIMELINE_RELOAD_MINUTES				GCONF_PATH "/ui/tabs/reload"
#define	PREFS_MAX_UPDATES_TO_SHOW				GCONF_PATH "/ui/tabs%s/max_updates"

#define PREFS_AUTOLOAD_FOLLOWING				GCONF_PATH "/ui/start-up/tabs/following"
#define PREFS_AUTOLOAD_HOMEPAGE					GCONF_PATH "/ui/start-up/tabs/homepage"
#define	PREFS_AUTOLOAD_DMS					GCONF_PATH "/ui/start-up/tabs/dms"
#define	PREFS_AUTOLOAD_REPLIES					GCONF_PATH "/ui/start-up/tabs/replies"
#define PREFS_AUTOLOAD_BEST_FRIENDS				GCONF_PATH "/ui/start-up/tabs/best_friends"

#define	PREFS_UPDATES_HOME_TIMELINE				GCONF_PATH "/ui/start-up/tabs/additional_timeline"

#define PREFS_NOTIFY_BEST_FRIENDS				GCONF_PATH "/notifications/best_friends"
#define	PREFS_NOTIFY_DMS					GCONF_PATH "/notifications/dms"
#define PREFS_NOTIFY_FOLLOWING					GCONF_PATH "/notifications/following"
#define	PREFS_NOTIFY_REPLIES					GCONF_PATH "/notifications/replies"
#define	PREFS_NOTIFY_ALL					GCONF_PATH "/notifications/all"
#define	PREFS_NOTIFY_BEEP					GCONF_PATH "/alerts/beep"

#define	PREFS_URLS_EXPANSION_DISABLED				GCONF_PATH "/ui/update_viewer/uri_expansion/disabled"
#define	PREFS_URLS_EXPANSION_SELECTED_ONLY			GCONF_PATH "/ui/update_viewer/uri_expansion/selected_only"
#define PREFS_URLS_EXPANSION_REPLACE_WITH_TITLES		GCONF_PATH "/ui/update_viewer/uri_expansion/replace_with_titles"
#define	PREFS_URLS_EXPANSION_USER_PROFILES			GCONF_PATH "/ui/update_viewer/uri_expansion/user_profiles"

#define	PREFS_DISABLE_SYSTEM_BELL				GCONF_PATH "/alerts/disable_system_bell"
#define PREFS_UPDATE_VIEWER_DIALOG				GCONF_PATH "/ui/update_viewer/use_dialog"
#define PREFS_UPDATE_VIEWER_COMPACT				GCONF_PATH "/ui/update_viewer/compact_view"
#define PREFS_UPDATE_VIEWER_SHOW_SERVICES			GCONF_PATH "/ui/update_viewer/show_services"

#define COMPACT_VIEW						GCONF_PATH "/ui/view/compact"
#define CONCATENATED_UPDATES					GCONF_PATH "/ui/view/detailed_updates"
#define SCROLL_TO_TOP_WITH_NEW_UPDATES				GCONF_PATH "/ui/uberchick_tree_view/scroll_to_top_with_new_updates"
#define SELECT_UPDATE_BY_DEFAULT				GCONF_PATH "/ui/uberchick_tree_view/select_update_by_default"

#define	PREFS_DISABLE_UPDATE_LENGTH_ALERT			GCONF_PATH "/alerts/disable_update_length_alert"

#define PREFS_ONLINE_SERVICE					GCONF_PATH "/online-services/%s"

#define	ONLINE_SERVICES_ACCOUNTS				GCONF_PATH "/online-services/accounts"
#define	MY_BEST_FRIENDS						GCONF_PATH "/online-services/best_friends"
#define ONLINE_SERVICE_UPDATE_IDS_GCONF_KEY			GCONF_PATH "/online-services/xml-cache/archive/since-ids/%s%s/%s"

#define	PREFS_UPDATES_ARCHIVE_ALL				GCONF_PATH "/online-services/xml-cache/archive/expiration/default"
#define	PREFS_UPDATES_ARCHIVE_DMS				GCONF_PATH "/online-services/xml-cache/archive/expiration/dms"
#define	PREFS_UPDATES_ARCHIVE_REPLIES				GCONF_PATH "/online-services/xml-cache/archive/expiration/replies"
#define	PREFS_UPDATES_ARCHIVE_FAVES				GCONF_PATH "/online-services/xml-cache/archive/expiration/faves"
#define	PREFS_UPDATES_ARCHIVE_BEST_FRIENDS			GCONF_PATH "/online-services/xml-cache/archive/expiration/best_friends"

#define	PREFS_UPDATES_REPLACE_ME_W_NICK				GCONF_PATH "/updates/replacements/me_w_nick"
#define	PREFS_SWAP_OCTOTHORPES_AND_EXCLAIMATIONS		GCONF_PATH "/updates/replacements/swap/octothorpes_and_exclaimations"
#define PREFS_UPDATES_ADD_PROFILE_LINK				GCONF_PATH "/updates/reply/add_profile_link"
#define	PREFS_UPDATES_DIRECT_REPLY_ONLY				GCONF_PATH "/updates/reply/directly_to"

#define	PREFS_SEARCH_HISTORY_UNIQUE_ONLY			GCONF_PATH "/search/history/unique_only"
#define	PREFS_SEARCH_HISTORY_MAXIMUM				GCONF_PATH "/search/history/maximum"

#define	PREFS_UPDATES_HISTORY_UNIQUE_ONLY			GCONF_PATH "/updates/history/unique_only"
#define	PREFS_UPDATES_HISTORY_MAXIMUM				GCONF_PATH "/updates/history/maximum"


/* Window height, width, & position gconf values. */
#define PREFS_UI_WIDTH						GCONF_PATH "/ui/%s/width"
#define PREFS_UI_HEIGHT						GCONF_PATH "/ui/%s/height"
#define PREFS_UI_POSITIONS					GCONF_PATH "/ui/%s/position_%s"

/* ui & notification gconf values. */
#define MAIN_WINDOW_UI_HIDDEN					GCONF_PATH "/ui/hide/main_window"
#define MAIN_WINDOW_BEST_FRIENDS_HIDE_VBOX			GCONF_PATH "/ui/hide/best_friends"

#define MAIN_WINDOW_MAIN_TOOLBAR_HIDE				GCONF_PATH "/ui/hide/main_toolbar"

#define PREFS_SAVED_HISTORY_STRING				GCONF_PATH "/%s/history/saved/%d"

#define TIMELINE_SEXY_TREE_VIEW_TOOLBAR_VISIBILITY		GCONF_PATH "/ui/hide/tabs_toolbars"
#define TIMELINE_SEXY_TREE_VIEW_FROM_COLUMN_VISIBILITY		GCONF_PATH "/ui/hide/columns/from"
#define TIMELINE_SEXY_TREE_VIEW_RCPT_COLUMN_VISIBILITY		GCONF_PATH "/ui/hide/columns/rcpt"

#define PREFS_ACCOUNT_DELETE					GCONF_PATH "/popup_confirmation_dialog/disabled_when/deleting/accounts"
#define PREFS_UPDATE_VIEWER_CLEAR				GCONF_PATH "/popup_confirmation_dialog/disabled_when/clearing/update_viewer"
#define PREFS_HINTS_HIDE_MAIN_WINDOW				GCONF_PATH "/popup_confirmation_dialog/disabled_when/closing/main_window"
#define PREFS_HINTS_CLOSE_MAIN_WINDOW				GCONF_PATH "/popup_confirmation_dialog/disabled_when/hiding/main_window"
#define	ONLINE_SERVICE_CONFIRM_BEST_FRIENDS_CLEAN_UP		GCONF_PATH "/popup_confirmation_dialog/disabled_when/cleaning_up/best_friends"
#define	ONLINE_SERVICE_CONFIRM_BEST_FRIENDS_DELETION		GCONF_PATH "/popup_confirmation_dialog/disabled_when/deleting/best_friends"

G_END_DECLS

#endif /* __PREFERENCES_DEFINES_H__ */
