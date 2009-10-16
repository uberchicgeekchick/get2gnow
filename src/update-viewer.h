/*
 * Copyright(c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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

#ifndef __UPDATE_VIEWER_H__
#define __UPDATE_VIEWER_H__

#define _GNU_SOURCE
#define _THREAD_SAFE

#include <gtk/gtk.h>
#include <libsexy/sexy.h>
#include "online-services-typedefs.h"
#include "users.types.h"

G_BEGIN_DECLS


#define TYPE_UPDATE_VIEWER			(update_viewer_get_type())
#define UPDATE_VIEWER(o)			(G_TYPE_CHECK_INSTANCE_CAST((o), TYPE_UPDATE_VIEWER, UpdateViewer))
#define UPDATE_VIEWER_CLASS(k)			(G_TYPE_CHECK_CLASS_CAST((k), TYPE_UPDATE_VIEWER, UpdateViewerClass))
#define IS_UPDATE_VIEWER(o)			(G_TYPE_CHECK_INSTANCE_TYPE((o), TYPE_UPDATE_VIEWER))
#define IS_UPDATE_VIEWER_CLASS(k)		(G_TYPE_CHECK_CLASS_TYPE((k), TYPE_UPDATE_VIEWER))
#define UPDATE_VIEWER_GET_CLASS(o)		(G_TYPE_INSTANCE_GET_CLASS((o), TYPE_UPDATE_VIEWER, UpdateViewerClass))

typedef struct _UpdateViewer      UpdateViewer;
typedef struct _UpdateViewerClass UpdateViewerClass;
typedef struct _UpdateViewerPrivate  UpdateViewerPrivate;

struct _UpdateViewer {
	SexyTreeView            parent;
};

struct _UpdateViewerClass {
	SexyTreeViewClass       parent_class;
};


typedef enum _UpdateViewerListStoreColumn{		/********************************************************************************/
	GUINT_UPDATE_VIEWER_INDEX,			/*	The update's index in the list-store.                                   */
	GDOUBLE_UPDATE_ID,				/*	Tweet's ID.								*/
	GDOUBLE_USER_ID,				/*	User's ID.								*/
	STRING_USER,					/*	Username string.							*/
	STRING_NICK,					/*	Author name string.							*/
	STRING_TEXT,					/*	Unformated Tweet string.						*/
	STRING_TWEET,					/*	Update for display string in the UpdateViewer's SexyTreeView.           */
	STRING_SEXY_TWEET,				/*	libsexy formatted Tweet for SexyTreeView's tooltip.			*/
	STRING_CREATED_AGO,				/*	'Posted ?(seconds|minutes|hours|day) ago.				*/
	STRING_CREATED_AT,				/*	Date string.								*/
	GINT_CREATED_AGO,				/*	How old the post is, in seconds, for sorting.				*/
	ULONG_CREATED_AT,				/*	Unix seconds since epoc of how old the tweet's is.			*/
	PIXBUF_AVATAR,					/*	Avatar pixbuf.								*/
	ONLINE_SERVICE,					/*	OnlineService pointer.							*/
	STRING_FROM,					/*	<status's auther @ OnlineService URI>					*/
	STRING_RCPT,					/* 	OnlineService key To: <user@service>					*/
	GINT_SELECTED_INDEX,				/*	The associated order/place this item appears in the acciated tree view. */
	GBOOLEAN_UNREAD,				/*	If the update has been read or not					*/
							/********************************************************************************/
} UpdateViewerListStoreColumn;


GType update_viewer_get_type(void) G_GNUC_CONST;
UpdateViewer *update_viewer_new(const gchar *timeline, OnlineService *service);
const gchar *update_viewer_get_timeline(UpdateViewer *update_viewer);

GtkVBox *update_viewer_get_child(UpdateViewer *update_viewer);
GtkLabel *update_viewer_get_tab(UpdateViewer *update_viewer);
GtkLabel *update_viewer_get_menu(UpdateViewer *update_viewer);

OnlineService *update_viewer_get_service(UpdateViewer *update_viewer);
GtkListStore *update_viewer_get_list_store(UpdateViewer *update_viewer);
GtkTreeModel *update_viewer_get_tree_model(UpdateViewer *update_viewer);

void update_viewer_start(UpdateViewer *update_viewer);
gboolean update_viewer_refresh(UpdateViewer *update_viewer);
void update_viewer_stop(UpdateViewer *update_viewer);
void update_viewer_complete(UpdateViewer *update_viewer);

void update_viewer_store( UpdateViewer *update_viewer, UserStatus *status );
void update_viewer_remove_from_list_store( UpdateViewer *update_viewer, UpdateViewerListStoreColumn update_viewer_list_store_column, gpointer value );
void update_viewer_remove_service(UpdateViewer *update_viewer, OnlineService *service);

void update_viewer_toggle_toolbar(UpdateViewer *update_viewer);

void update_viewer_mark_as_read(UpdateViewer *update_viewer);

gint8 update_viewer_has_loaded(UpdateViewer *update_viewer);

gint update_viewer_get_page(UpdateViewer *update_viewer);
void update_viewer_set_page(UpdateViewer *update_viewer, gint page);

UpdateMonitor update_viewer_get_monitoring(UpdateViewer *update_viewer);
const gchar *update_viewer_get_monitoring_string(UpdateViewer *update_viewer);
const gchar *monitoring_to_string(UpdateMonitor monitoring);

gint update_viewer_get_total(UpdateViewer *update_viewer);
guint update_viewer_get_notify_delay(UpdateViewer *update_viewer);

void update_viewer_key_pressed(UpdateViewer *update_viewer, GdkEventKey *event);
void update_viewer_set_image(UpdateViewer *update_viewer, const gchar *image_filename, GtkTreeIter *iter);


G_END_DECLS
#endif /* __UPDATE_VIEWER_H__ */
