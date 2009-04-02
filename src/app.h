/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2002-2007 Imendio AB
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
 *			Daniel Morales <daniminas@gmail.com>
 */

#ifndef __APP_H__
#define __APP_H__

#include <glib-object.h>
#include <glib.h>

#include <gtk/gtk.h>
#include "paths.h"
#include "parser.h"
#include "images.h"

#include "tweet-list.h"


G_BEGIN_DECLS

#define TYPE_APP             (app_get_type ())
#define APP(o)			    (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_APP, App))
#define APP_CLASS(k)		    (G_TYPE_CHECK_CLASS_CAST((k), TYPE_APP, AppClass))
#define IS_APP(o)		    (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_APP))
#define IS_APP_CLASS(k)	    (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_APP))
#define IS_APP_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_APP, AppClass))

typedef struct _App      	App;
typedef struct _AppClass 	AppClass;
typedef struct _AppPriv  	AppPriv;

struct _AppPriv {
	/* Main widgets */
	GtkWidget		*window;
	TweetList		*listview;
	GtkWidget		*statusbar;

	/*
	 *		Widgets that are enabled when
	 *		we are connected/disconnected
	 */
	GList			*widgets_connected;
	GList			*widgets_disconnected;
	GList			*widgets_tweet_selected;

	/* Timeline menu items */
	GSList			*group;
	GtkRadioAction		*menu_combined;
	GtkRadioAction		*menu_public;
	GtkRadioAction		*menu_friends;
	GtkRadioAction		*menu_mine;
	GtkRadioAction		*menu_twitux;
	GtkRadioAction		*menu_direct_messages;
	GtkRadioAction		*menu_direct_replies;

	GtkAction		*view_friends;

	/* Status Icon */
	GtkStatusIcon		*status_icon;

	/* Status Icon Popup Menu */
	GtkWidget		*popup_menu;
	GtkToggleAction		*popup_menu_show_app;

	/* Account related data */
	char			*username;
	char			*password;

	/* Misc */
	guint			size_timeout_id;
	
	/* Expand messages widgets */
	GtkWidget		*expand_box;
	GtkWidget		*expand_image;
	GtkWidget		*expand_title;
	GtkWidget		*expand_label;
};

extern AppPriv *app_priv;

struct _App {
        GObject        parent;
};

struct _AppClass {
        GObjectClass parent_class;
};

GType               app_get_type                     (void) G_GNUC_CONST;
void                app_create                       (void);
App *	        app_get                          (void);
GtkWidget *         app_get_window                   (void);
void				app_set_visibility				(gboolean	   visible);
void				app_set_statusbar_msg	        (gchar        *message);
void                app_notify_sound                 (void);
void				app_notify                       (gchar        *msg);
void                app_state_on_connection          (gboolean      connected);
void app_set_image ( const gchar *file, GtkTreeIter iter );
void                app_expand_message               (const gchar  *name,
                                                             const gchar  *date,
                                                             const gchar  *tweet,
                                                             GdkPixbuf    *pixbuf);
G_END_DECLS

#endif /*_APP_H_*/
