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

#ifndef __TWITUX_APP_H__
#define __TWITUX_APP_H__

#include <glib-object.h>
#include <glib.h>

#include <gtk/gtk.h>

#include <libtwitux/twitux-paths.h>

#include "twitux-parser.h"

G_BEGIN_DECLS

#define TWITUX_TYPE_APP             (twitux_app_get_type ())
#define TWITUX_APP(o)			    (G_TYPE_CHECK_INSTANCE_CAST ((o), TWITUX_TYPE_APP, TwituxApp))
#define TWITUX_APP_CLASS(k)		    (G_TYPE_CHECK_CLASS_CAST((k), TWITUX_TYPE_APP, TwituxAppClass))
#define TWITUX_IS_APP(o)		    (G_TYPE_CHECK_INSTANCE_TYPE ((o), TWITUX_TYPE_APP))
#define TWITUX_IS_APP_CLASS(k)	    (G_TYPE_CHECK_CLASS_TYPE ((k), TWITUX_TYPE_APP))
#define TWITUX_IS_APP_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), TWITUX_TYPE_APP, TwituxAppClass))

typedef struct _TwituxApp      	TwituxApp;
typedef struct _TwituxAppClass 	TwituxAppClass;
typedef struct _TwituxAppPriv  	TwituxAppPriv;

struct _TwituxApp {
        GObject        parent;
};

struct _TwituxAppClass {
        GObjectClass parent_class;
};

GType               twitux_app_get_type                     (void) G_GNUC_CONST;
void                twitux_app_create                       (void);
TwituxApp *	        twitux_app_get                          (void);
GtkWidget *         twitux_app_get_window                   (void);
void				twitux_app_set_visibility				(gboolean	   visible);
void				twitux_app_set_statusbar_msg	        (gchar        *message);
void				twitux_app_set_friends	                (GList        *friends);
void				twitux_app_add_friend                   (TwituxUser   *user);
void                twitux_app_notify_sound                 (void);
void				twitux_app_notify                       (gchar        *msg);
void                twitux_app_state_on_connection          (gboolean      connected);
void                twitux_app_set_image                    (const gchar  *file,
                                                             GtkTreeIter   iter);
void                twitux_app_expand_message               (const gchar  *name,
                                                             const gchar  *date,
                                                             const gchar  *tweet,
                                                             GdkPixbuf    *pixbuf);
G_END_DECLS

#endif /*_TWITUX_APP_H_*/
