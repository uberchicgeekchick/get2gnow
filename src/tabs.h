/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the Reciprocal Public License (RPL).
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

/********************************************************************************
 *                      My art, code, & programming.                            *
 ********************************************************************************/
#ifndef	__TABS_H__
#define	__TABS_H__

#define _GNU_SOURCE
#define _THREAD_SAFE

/********************************************************************************
 *      Project, system, & library headers.  eg #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/
#include <glib.h>
#include <gtk/gtk.h>

#include "program.h"
#include "online-services-typedefs.h"
#include "timelines-sexy-tree-view.h"


G_BEGIN_DECLS
/********************************************************************************
 *                        defines, macros, methods, & etc                       *
 ********************************************************************************/
#define TIMELINE_SEXY_TREE_VIEW_TOOLBAR_VISIBILITY		GCONF_PATH "/ui/hide/tabs_toolbars"
#define TIMELINE_SEXY_TREE_VIEW_FROM_COLUMN_VISIBILITY		GCONF_PATH "/ui/hide/columns/from"
#define TIMELINE_SEXY_TREE_VIEW_RCPT_COLUMN_VISIBILITY		GCONF_PATH "/ui/hide/columns/rcpt"


/********************************************************************************
 *                        objects, structs, and enum typedefs                   *
 ********************************************************************************/


/********************************************************************************
 *       prototypes for methods, handlers, callbacks, function, & etc           *
 ********************************************************************************/
void tabs_init(GtkNotebook *notebook);

TimelinesSexyTreeView *tabs_open_timeline(const gchar *timeline, OnlineService *service);
void tabs_close_timeline(const gchar *timeline);

TimelinesSexyTreeView *tabs_view_page(gint page);
TimelinesSexyTreeView *tabs_get_page(gint page, gboolean close);
TimelinesSexyTreeView *tabs_get_next(void);
TimelinesSexyTreeView *tabs_get_current(void);
TimelinesSexyTreeView *tabs_get_previous(void);

void tabs_start(void);
void tabs_remove_from_timelines_sexy_tree_views_list_stores( TimelinesSexyTreeViewListStoreColumn timelines_sexy_tree_view_list_store_column, gpointer value );
void tabs_remove_service(OnlineService *service);
void tabs_refresh(void);
void tabs_stop(void);

void tabs_close(void);
void tabs_close_current_page(void);
void tabs_close_page(gint page);

void tabs_toggle_view(void);
void tabs_toggle_toolbars(void);
void tabs_toggle_from_columns(void);
void tabs_toggle_rcpt_columns(void);

void tabs_destroy(void);

G_END_DECLS
/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
#endif /* __TABS_H__*/
