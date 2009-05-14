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

#include "app.h"
#include "tweet-view.h"

typedef enum{
	Embed,
	MainWindow,
	FloatingTweetView,
} ViewType;

enum {
	PrefernceWindow,
	PreferenceWidth,
	PreferenceHeight,
	PreferencePositionX,
	PreferencePositionY,
	PreferenceTotal,
};

static GtkWindow *geometry_get_window(ViewType view);
static gchar **geometry_get_prefs_path(ViewType view);
static void geometry_load_for_window(ViewType view);
static void geometry_save_for_window(ViewType view);


#define DEBUG_DOMAINS "Geometry:UI:GtkBuilder:GtkBuildable:Setup:Start-Up"

void geometry_save(void){
	if(!gconfig_if_bool(PREFS_UI_TWEET_VIEW_USE_DIALOG))
		return geometry_save_for_window(Embed);
	
	geometry_save_for_window(MainWindow);
	geometry_save_for_window(FloatingTweetView);
}
 
void geometry_load(void){
	if(!gconfig_if_bool(PREFS_UI_TWEET_VIEW_USE_DIALOG))
		return geometry_load_for_window(Embed);
	
	geometry_load_for_window(MainWindow);
	geometry_load_for_window(FloatingTweetView);
}//geometry_load

static void geometry_load_for_window(ViewType view){
	gint		x=0, y=0, w=0, h=0;
	GtkWindow	*window=geometry_get_window(view);
	gchar		**prefs_path=NULL;
	if(!(prefs_path=geometry_get_prefs_path(view))) return;
	debug(DEBUG_DOMAINS, "Loading %s window geometry.", prefs_path[PrefernceWindow]);
	
	gconfig_get_int(prefs_path[PreferenceWidth], &w);
	gconfig_get_int(prefs_path[PreferenceHeight], &h);
	
	if(!( x >0 && y > 0 )){
		debug(DEBUG_DOMAINS, "Unable to move %s window, either value is less than zero.\n\tposition x: %d, position y: %d", prefs_path[PrefernceWindow], x, y);
	}else{
		debug( DEBUG_DOMAINS, "Moving %s window to: x: %d, y: %d", prefs_path[PrefernceWindow], x, y);
		gtk_window_resize(window, w, h);
	}
	
	gconfig_get_int(prefs_path[PreferencePositionX], &x);
	gconfig_get_int(prefs_path[PreferencePositionY], &y);
	
	if(!( x >0 && y > 0 )){
		debug(DEBUG_DOMAINS, "Unable to move %s window, either value is less than zero.\n\tposition x: %d, position y: %d", prefs_path[PrefernceWindow], x, y);
	}else{
		debug( DEBUG_DOMAINS, "Moving %s window to: x: %d, y: %d", prefs_path[PrefernceWindow], x, y);
		gtk_window_move(window, x, y);
	}
	g_free(prefs_path);
}//geometry_load_for_window

static void geometry_save_for_window(ViewType view){
	gint		x=0, y=0, w=0, h=0;
	GtkWindow	*window=geometry_get_window(view);
	gchar		**prefs_path=NULL;
	if(!(prefs_path=geometry_get_prefs_path(view))) return;
	gtk_window_get_size(window, &w, &h);
	
	if(!( w >0 && h > 0 )){
		debug(DEBUG_DOMAINS, "Unable to save height & width for %s, either value is less than zero.\n\twidth: %d, height: %d", prefs_path[PrefernceWindow], w, h);
	}else{
		debug(DEBUG_DOMAINS, "Saving height & width for %s\n\twidth: %d, height: %d", prefs_path[PrefernceWindow], w, h);
		gconfig_set_int(prefs_path[PreferenceWidth], w);
		gconfig_set_int(prefs_path[PreferenceHeight], h);
	}
	
	gtk_window_get_position(window, &x, &y);
	if(!( x > 0 && y > 0 )){
		debug(DEBUG_DOMAINS, "Unable to save height & width for %s, either value is less than zero.\n\tposition x: %d, position y: %d", prefs_path[PrefernceWindow], x, y);
	}else{
		debug(DEBUG_DOMAINS, "Saving position for: %s\n\tpostition x: %d, position y: %d.", prefs_path[PrefernceWindow], x, y);

		gconfig_set_int(prefs_path[PreferencePositionX], x);
		gconfig_set_int(prefs_path[PreferencePositionY], y);
	}
	g_free(prefs_path);
}//geometry_save_for_window
 
static GtkWindow *geometry_get_window(ViewType view){
	debug(DEBUG_DOMAINS, "Getting window to set geometry for.");
	switch(view){
		case Embed:
		case MainWindow:
			return app_get_window();
		break;
		
		case FloatingTweetView:
			return tweet_view_get_window();
		break;
	}
	return app_get_window();
}//geometry_get_window

static gchar **geometry_get_prefs_path(ViewType view){
	gchar **prefs_path=NULL;
	
	if(!( (prefs_path=g_malloc0(sizeof(G_TYPE_STRING)*PreferenceTotal)) )){
		debug(DEBUG_DOMAINS, "**FATAL-ERROR**: Unable to allocate preference path memory.");
		return NULL;
	}
	
	switch(view){
		case Embed:
			prefs_path[PrefernceWindow]=g_strdup("embed");
		break;
		
		case MainWindow:
			prefs_path[PrefernceWindow]=g_strdup("main_window");
		break;
		
		case FloatingTweetView:
			prefs_path[PrefernceWindow]=g_strdup("tweet_view");
		break;
		
		default:
			debug(DEBUG_DOMAINS, "**ERROR**: Setting gconf preference paths.\n\t\tUnsupported geometry: [%d].", view);
			return NULL;
		break;
	}
	debug(DEBUG_DOMAINS, "Setting gconf preference paths to use for '%s' geometry.", prefs_path[PrefernceWindow]);
	
	if(!(prefs_path[PreferenceWidth]=g_strdup_printf(PREFS_UI_WIDTH, prefs_path[PrefernceWindow]))){
		debug(DEBUG_DOMAINS, "**ERROR:** Loading '%s' width preference path\t[failed].", prefs_path[PrefernceWindow]);
		g_strfreev(prefs_path);
		return NULL;
	}
	if(!(prefs_path[PreferenceHeight]=g_strdup_printf(PREFS_UI_HEIGHT, prefs_path[PrefernceWindow]))){
		debug(DEBUG_DOMAINS, "**ERROR:** Loading '%s' height preference path\t[failed].", prefs_path[PrefernceWindow]);
		g_strfreev(prefs_path);
		return NULL;
	}
	if(!(prefs_path[PreferencePositionX]=g_strdup_printf(PREFS_UI_POSITION_X, prefs_path[PrefernceWindow]))){
		debug(DEBUG_DOMAINS, "**ERROR:** Loading '%s' x position preference path\t[failed].", prefs_path[PrefernceWindow]);
		g_strfreev(prefs_path);
		return NULL;
	}
	
	if(!(prefs_path[PreferencePositionY]=g_strdup_printf(PREFS_UI_POSITION_Y, prefs_path[PrefernceWindow]))){
		debug(DEBUG_DOMAINS, "**ERROR:** Loading '%s' y position preference path\t[failed].", prefs_path[PrefernceWindow]);
		g_strfreev(prefs_path);
		return NULL;
	}
	
	debug(DEBUG_DOMAINS, "Loaded gconf '%s' geometry preference paths.", prefs_path[PrefernceWindow]);
	return prefs_path;
}
