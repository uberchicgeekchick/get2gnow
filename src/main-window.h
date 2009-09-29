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

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#define _GNU_SOURCE
#define _THREAD_SAFE

#include <glib-object.h>
#include <glib.h>
#include <libsexy/sexy.h>

#include <gtk/gtk.h>

#include "online-services-typedefs.h"
#include "tweet-list.h"

G_BEGIN_DECLS

#ifndef statusbar_printf
#define	statusbar_printf		main_window_statusbar_printf
#endif

#define TYPE_MAIN_WINDOW		(main_window_get_type())
#define MAIN_WINDOW(o)			(G_TYPE_CHECK_INSTANCE_CAST((o), TYPE_MAIN_WINDOW, MainWindow))
#define MAIN_WINDOW_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), TYPE_MAIN_WINDOW, MainWindowClass))
#define IS_MAIN_WINDOW(o)		(G_TYPE_CHECK_INSTANCE_TYPE((o), TYPE_MAIN_WINDOW))
#define IS_MAIN_WINDOW_CLASS(k)		(G_TYPE_CHECK_CLASS_TYPE((k), TYPE_MAIN_WINDOW))
#define IS_MAIN_WINDOW_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS((o), TYPE_MAIN_WINDOW, MainWindowClass))

typedef struct _MainWindow      	MainWindow;
typedef struct _MainWindowClass 	MainWindowClass;
typedef struct _MainWindowPrivate  	MainWindowPrivate;

struct _MainWindow {
        GObject			parent;
	MainWindowPrivate	*private;
};

struct _MainWindowClass {
        GObjectClass parent_class;
};

GType main_window_get_type(void) G_GNUC_CONST;
void main_window_disconnect(void);
void main_window_create(void);
MainWindow *main_window_get(void);

void main_window_tabs_init( void );
void main_window_control_panel_set_embed(GtkToggleButton *toggle_button, gpointer user_data);

GtkWindow *main_window_get_window(void);
GtkPaned *main_window_get_tweet_list_paned(void);
GtkPaned *main_window_get_tweet_paned(void);

GtkMenuItem *main_window_get_menu(const gchar *menu);

GtkListStore *main_window_get_best_friends_list_store(void);
GtkTreeModel *main_window_get_best_friends_tree_model(void);
gboolean main_window_best_friends_get_selected( OnlineService **service, gchar **user, gchar **user_name );

void main_window_statusbar_printf(const gchar *msg, ...) G_GNUC_PRINTF(1, 2);
void main_window_set_statusbar_msg(gchar *msg);

gboolean main_window_notify_on_timeout(gpointer data);
const gchar *main_window_tabs_to_right_align(void);

void main_window_state_on_connection(gboolean connected);
void main_window_selected_update_image_menu_items_show(gboolean selected_update);

G_END_DECLS

#endif /*__MAIN_WINDOW_H__*/
