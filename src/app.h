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
#include <libsexy/sexy.h>

#include <gtk/gtk.h>
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

struct _App {
        GObject        parent;
};

struct _AppClass {
        GObjectClass parent_class;
};

GType app_get_type(void) G_GNUC_CONST;
void app_create                       (void);
App *app_get(void);
GtkWindow *app_get_window(void);
TweetList *app_get_tweet_list(void);
GtkMenuBar *app_get_main_menu(void);
GtkComboBox *app_get_friends_combo_box(void);
SexySpellEntry *app_get_expand_entry(void);
GtkEntry *app_get_expand_entry_as_gtkentry(void);
void app_set_expand_entry(gchar *tweet);
GtkMenuItem *app_get_menu(const gchar *menu);
void app_refresh_timeline(GtkWidget *window, App *app); 
GList *app_get_widgets_tweet_selected(void);
void app_dm_data_fill(GList *followers);
void app_dm_data_show(void);
void app_dm_data_hide(void);
void app_set_visibility(gboolean visible);
void app_statusbar_printf(const gchar *msg, ...);
void app_set_statusbar_msg(gchar *msg);
void app_notify_sound(gboolean force);
void app_notify(gchar *msg);
void app_state_on_connection(gboolean connected);
void app_set_image( const gchar *file, GtkTreeIter iter );
void app_expand_message( const gchar  *user_name, const gchar  *user_nick, const gchar  *date, const gchar  *tweet, GdkPixbuf    *pixbuf );

G_END_DECLS

#endif /*_APP_H_*/
