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

G_BEGIN_DECLS

#define G_STR_EMPTY(x) ((x) == NULL || (x)[0] == '\0')

/* GConf Keys */
#define PREFS_PATH "/apps/greet-tweet-know"

#define PREFS_AUTH_USER_ID             PREFS_PATH "/auth/user_id"
#define PREFS_AUTH_PASSWORD            PREFS_PATH "/auth/password"
#define PREFS_AUTH_AUTO_LOGIN          PREFS_PATH "/auth/auto_login"

#define PREFS_TWEETS_HOME_TIMELINE     PREFS_PATH "/tweets/home_timeline"
#define PREFS_TWEETS_RELOAD_TIMELINES  PREFS_PATH "/tweets/reload_timeline"

#define PREFS_UI_WINDOW_HEIGHT         PREFS_PATH "/ui/main_window_height"
#define PREFS_UI_WINDOW_WIDTH          PREFS_PATH "/ui/main_window_width"
#define PREFS_UI_WIN_POS_X             PREFS_PATH "/ui/main_window_pos_x"
#define PREFS_UI_WIN_POS_Y             PREFS_PATH "/ui/main_window_pos_y"
#define PREFS_UI_MAIN_WINDOW_HIDDEN	  PREFS_PATH "/ui/main_window_hidden"
#define PREFS_UI_NOTIFICATION          PREFS_PATH "/ui/notify"
#define PREFS_UI_SOUND                 PREFS_PATH "/ui/sound"
#define PREFS_UI_SPELL_LANGUAGES       PREFS_PATH "/ui/spell_checker_languages"
#define PREFS_UI_SPELL                 PREFS_PATH "/ui/spell"

#define PREFS_HINTS_CLOSE_MAIN_WINDOW  PREFS_PATH "/hints/close_main_window"

/* Proxy configuration */
#define PROXY                          "/system/http_proxy"
#define PROXY_USE                      PROXY "/use_http_proxy"
#define PROXY_HOST                     PROXY "/host"
#define PROXY_PORT                     PROXY "/port"
#define PROXY_USE_AUTH                 PROXY "/use_authentication"
#define PROXY_USER                     PROXY "/authentication_user"
#define PROXY_PASS                     PROXY "/authentication_password"

/* File storage */
#define DIRECTORY                      "greet-tweet-know"
#define CACHE_IMAGES                   DIRECTORY "/avatars"

G_END_DECLS

#endif /* _MAIN_H_ */
