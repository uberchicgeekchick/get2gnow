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

/********************************************************
 *          My art, code, & programming.                *
 ********************************************************/
#define _GNU_SOURCE
#define _THREAD_SAFE



/********************************************************
 *        Project headers, eg #include "config.h"       *
 ********************************************************/
#include "config.h"
#include "program.h"

#include "gconfig.h"

#include "online-service-request.h"
#include "preferences.h"
#include "geometry.h"

#include "main-window.h"
#include "update-viewer.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
typedef enum{
	Embed,
	MicroBlogging_Viewer_MainWindow,
	FloatingUpdateViewer,
} ViewType;

enum {
	PrefernceWindow,
	PreferenceWidth,
	PreferenceHeight,
	PreferencePositionX,
	PreferencePositionY,
	PreferenceTotal,
};
#define DEBUG_DOMAINS "Geometry:FloatingUpdateViewer:MicroBlogging_Viewer_MainWindow:Paned:UI:GtkBuilder:GtkBuildable:Settings:Setup:Start-Up:MainWindow:geometry.c"
#include"debug.h"

/* Window height, width, & position gconf values. */
#define PREFS_UI_WIDTH				GCONF_PATH "/ui/%s/width"
#define PREFS_UI_HEIGHT				GCONF_PATH "/ui/%s/height"
#define PREFS_UI_POSITIONS			GCONF_PATH "/ui/%s/position_%s"

#define	LOAD_COUNT				2

/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static GtkWindow *geometry_get_window(ViewType view);
static gchar **geometry_get_prefs_path(ViewType view, gint *w, gint *h, gint *x, gint *y);
static void prefs_path_free(gchar **prefs_path);
static void geometry_load_for_window(ViewType view);
static void geometry_save_for_window(ViewType view);
static void geometry_set_paned(GtkPaned *paned, const gchar *widget, ViewType view, gboolean vpaned, gboolean save);

/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
void geometry_load(void){
	if(!gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE))
		return geometry_load_for_window(Embed);
	
	geometry_load_for_window(MicroBlogging_Viewer_MainWindow);
	geometry_load_for_window(FloatingUpdateViewer);
}/*geometry_load*/

void geometry_save(void){
	if(!gconfig_if_bool(PREFS_UPDATE_VIEWER_DIALOG, FALSE))
		return geometry_save_for_window(Embed);
	
	geometry_save_for_window(MicroBlogging_Viewer_MainWindow);
	geometry_save_for_window(FloatingUpdateViewer);
}/*geometry_save();*/

static void geometry_load_for_window(ViewType view){
	if( gconfig_if_bool(  PREFS_UPDATE_VIEWER_COMPACT, TRUE ) && online_service_request_is_update_selected() ) return;
	gint		x, y, w, h;
	gint		default_width=0, default_height=0, default_x=0, default_y=0;
	x=y=w=h=default_width=default_height=default_x=default_y=0;
	GtkWindow	*window=geometry_get_window(view);
	gchar		**prefs_path=NULL;
	if(!(prefs_path=geometry_get_prefs_path(view, &default_width, &default_height, &default_x, &default_y))) return;
	debug("Loading %s's window's geometry.", prefs_path[PrefernceWindow]);
	
	gconfig_get_int_or_default(prefs_path[PreferenceWidth], &w, default_width);
	gconfig_get_int_or_default(prefs_path[PreferenceHeight], &h, default_height);
	
	if(!( w >0 && h > 0 )){
		debug("Unable to resize %s window, either value is less than zero.  width: %d, height: %d", prefs_path[PrefernceWindow], w, h);
	}else{
		debug("Resizing %s window to: width: %d, height: %d", prefs_path[PrefernceWindow], w, h);
		gtk_window_resize(window, w, h);
	}
	
	gconfig_get_int_or_default(prefs_path[PreferencePositionX], &x, default_x);
	gconfig_get_int_or_default(prefs_path[PreferencePositionY], &y, default_y);
	
	if(!( x >0 && y > 0 )){
		debug("Unable to move %s window, either value is less than zero.  position x: %d, position y: %d", prefs_path[PrefernceWindow], x, y);
	}else{
		debug("Moving %s window to: x: %d, y: %d", prefs_path[PrefernceWindow], x, y);
		gtk_window_move(window, x, y);
	}
	prefs_path_free(prefs_path);
	
	if(view!=Embed) return;
	
	geometry_set_paned(main_window_get_timelines_sexy_tree_view_paned(), "timelines_sexy_tree_view", view, FALSE, FALSE);
	geometry_set_paned(main_window_get_main_paned(), "update_viewer", view, TRUE, FALSE);
}//geometry_load_for_window

static void geometry_save_for_window(ViewType view){
	if( gconfig_if_bool(  PREFS_UPDATE_VIEWER_COMPACT, TRUE ) && online_service_request_is_update_selected() ) return;
	static guint	calls=0;
	gint		x, y, w, h;
	gint		default_width=0, default_height=0, default_x=0, default_y=0;
	x=y=w=h=default_width=default_height=default_x=default_y=0;
	GtkWindow	*window=geometry_get_window(view);
	gchar		**prefs_path=NULL;
	if(!(prefs_path=geometry_get_prefs_path(view, &default_width, &default_height, &default_x, &default_y))) return;
	debug("Saving %s's window's geometry.", prefs_path[PrefernceWindow]);
	
	gtk_window_get_size(window, &w, &h);
	if(!( w >0 && h > 0 )){
		debug("Unable to save height & width for %s, either value is less than zero.  width: %d, height: %d", prefs_path[PrefernceWindow], w, h);
	}else{
		debug("Saving height & width for %s  width: %d, height: %d", prefs_path[PrefernceWindow], w, h);
		gconfig_set_int(prefs_path[PreferenceWidth], w);
		gconfig_set_int(prefs_path[PreferenceHeight], h);
	}
	
	gtk_window_get_position(window, &x, &y);
	if(!( x > 0 && y > 0 )){
		debug("Unable to save window positions for %s, either value is less than zero.  position x: %d, position y: %d", prefs_path[PrefernceWindow], x, y);
	}else{
		debug("Saving position for: %s  postition x: %d, position y: %d.", prefs_path[PrefernceWindow], x, y);

		gconfig_set_int(prefs_path[PreferencePositionX], x);
		gconfig_set_int(prefs_path[PreferencePositionY], y);
	}
	prefs_path_free(prefs_path);
	
	if(view!=Embed){
		if(calls) calls=0;
		return;
	}
	
	if(calls<LOAD_COUNT) calls++;
	
	geometry_set_paned(main_window_get_timelines_sexy_tree_view_paned(), "timelines_sexy_tree_view", view, FALSE, (calls==LOAD_COUNT) );
	geometry_set_paned(main_window_get_main_paned(), "update_viewer", view, TRUE, (calls==LOAD_COUNT) );
}/*geometry_save_for_window(view);*/
 
static GtkWindow *geometry_get_window(ViewType view){
	debug("Getting window to set geometry for.");
	switch(view){
		case Embed:
		case MicroBlogging_Viewer_MainWindow:
			return main_window_get_window();
		break;
		
		case FloatingUpdateViewer:
			return update_viewer_get_window();
		break;
	}
	return main_window_get_window();
}//geometry_get_window

static gchar **geometry_get_prefs_path(ViewType view, gint *w, gint *h, gint *x, gint *y){
	gchar **prefs_path=NULL;
	
	if(!( (prefs_path=g_malloc0(sizeof(G_TYPE_STRING)*PreferenceTotal)) )){
		debug("**FATAL-ERROR**: Unable to allocate preference path memory.");
		return NULL;
	}
	
	switch(view){
		case Embed:
			prefs_path[PrefernceWindow]=g_strdup("embed");
			*w=1100, *h=700, *x=100, *y=100;
		break;
		
		case MicroBlogging_Viewer_MainWindow:
			prefs_path[PrefernceWindow]=g_strdup("main_window");
			*w=1100, *h=500, *x=100, *y=100;
		break;
		
		case FloatingUpdateViewer:
			prefs_path[PrefernceWindow]=g_strdup("update_viewer");
			*w=700, *h=350, *x=100, *y=600;
		break;
		
		default:
			debug("**ERROR**: Setting gconf preference paths. Unsupported geometry: [%d].", view);
			return NULL;
		break;
	}
	debug("Setting gconf preference paths to use for '%s' geometry.", prefs_path[PrefernceWindow]);
	
	if(!(prefs_path[PreferenceWidth]=g_strdup_printf(PREFS_UI_WIDTH, prefs_path[PrefernceWindow]))){
		debug("**ERROR:** Loading '%s' width preference path\t[failed].", prefs_path[PrefernceWindow]);
		prefs_path_free(prefs_path);
		return NULL;
	}
	if(!(prefs_path[PreferenceHeight]=g_strdup_printf(PREFS_UI_HEIGHT, prefs_path[PrefernceWindow]))){
		debug("**ERROR:** Loading '%s' height preference path\t[failed].", prefs_path[PrefernceWindow]);
		prefs_path_free(prefs_path);
		return NULL;
	}
	if(!(prefs_path[PreferencePositionX]=g_strdup_printf(PREFS_UI_POSITIONS, prefs_path[PrefernceWindow], "x"))){
		debug("**ERROR:** Loading '%s' x position preference path\t[failed].", prefs_path[PrefernceWindow]);
		prefs_path_free(prefs_path);
		return NULL;
	}
	
	if(!(prefs_path[PreferencePositionY]=g_strdup_printf(PREFS_UI_POSITIONS, prefs_path[PrefernceWindow], "y"))){
		debug("**ERROR:** Loading '%s' y position preference path\t[failed].", prefs_path[PrefernceWindow]);
		prefs_path_free(prefs_path);
		return NULL;
	}
	
	debug("Loaded gconf '%s' geometry preference paths.", prefs_path[PrefernceWindow]);
	return prefs_path;
}

static void prefs_path_free(gchar **prefs_path){
	for(int i=0; i<PreferenceTotal; i++)
		uber_free(prefs_path[i]);
	
	uber_free(prefs_path);
}/*prefs_path_free*/

static void geometry_set_paned(GtkPaned *paned, const gchar *widget, ViewType view, gboolean vpaned, gboolean save){
	GtkWindow	*window=geometry_get_window(view);
	gint		h=0, w=0;
	gtk_window_get_size(window, &w, &h);
	gint position=0, min_position=0, max_position=0, padding=0;/*(h/10)*5;*/
	
	gchar *paned_position_prefs_path=g_strdup_printf(PREFS_UI_POSITIONS, widget, (vpaned ?"vpaned" :"hpaned"));
	
	if(save){
		position=gtk_paned_get_position(paned);
		gconfig_set_int(paned_position_prefs_path, position);
	}else{
		gconfig_get_int_or_default(paned_position_prefs_path, &position, (vpaned ?420 :320 ) );
	}
	uber_free(paned_position_prefs_path);
	
	g_object_get(G_OBJECT(paned), "max-position", &max_position, "min-position", &min_position, NULL);
	max_position-=padding;
	
	debug("%s %s's paned position to: %d.", (save ?_("Saving") :_("Moving")), widget, position);
	debug("Position details: maximum: %d; minimum: %d; padding: %d.", max_position, min_position, padding);
	
	gtk_paned_set_position(paned, position);
}/*geometry_set_paned(main_window_get_main_paned(), "update_viewer", view, TRUE, FALSE);*/

/********************************************************
 *                       eof                            *
 ********************************************************/

