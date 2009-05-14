/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright(c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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
 * License("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
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
 * user-visible display as defined in Section 6.4(d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright(C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright(C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright(C) of the User the data is connected to.
 * Users may lisences their data under the terms of an OSI approved or Creative Commons
 * license.  Users must be allowed to select their choice of license for each piece of data
 * on an individual bases and cannot be blanketly applied to all of the Users.  The User may
 * select a default license for their data.  All of the Software's data pertaining to each
 * User must be fully accessible, exportable, and deletable to that User.
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

#define TYPE_APP		(app_get_type())
#define APP(o)			(G_TYPE_CHECK_INSTANCE_CAST((o), TYPE_APP, App))
#define APP_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), TYPE_APP, AppClass))
#define IS_APP(o)		(G_TYPE_CHECK_INSTANCE_TYPE((o), TYPE_APP))
#define IS_APP_CLASS(k)		(G_TYPE_CHECK_CLASS_TYPE((k), TYPE_APP))
#define IS_APP_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS((o), TYPE_APP, AppClass))

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
void app_create(void);
App *app_get(void);

void app_tweet_view_set_embed(GtkCheckButton *check_button, gpointer user_data);

GtkWindow *app_get_window(void);
TweetList *app_get_tweet_list(void);
GtkMenuBar *app_get_main_menu(void);
GtkMenuItem *app_get_menu(const gchar *menu);

void app_refresh_timeline(GtkWidget *window, App *app); 

gboolean app_has_status_icon(void);
void app_set_visibility(gboolean visible);

void app_statusbar_printf(const gchar *msg, ...);
void app_set_statusbar_msg(gchar *msg);


gboolean app_notify_on_timeout(gpointer tweet);
void app_notify_sound(gboolean force);
void app_notify(gchar *msg);

void app_state_on_connection(gboolean connected);

G_END_DECLS

#endif /*_APP_H_*/
