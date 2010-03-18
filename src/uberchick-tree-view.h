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

#ifndef __UBERCHICK_TREE_VIEW_H__
#define __UBERCHICK_TREE_VIEW_H__

#define _GNU_SOURCE
#define _THREAD_SAFE

#include <gtk/gtk.h>
#include <libsexy/sexy.h>
#include "online-services.typedefs.h"

G_BEGIN_DECLS


#define UBERCHICK_TYPE_TREE_VIEW		(uberchick_tree_view_get_type())
#define UBERCHICK_TREE_VIEW(o)			(G_TYPE_CHECK_INSTANCE_CAST((o), UBERCHICK_TYPE_TREE_VIEW, UberChickTreeView))
#define UBERCHICK_TREE_VIEW_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), UBERCHICK_TYPE_TREE_VIEW, UberChickTreeViewClass))
#define IS_UBERCHICK_TREE_VIEW(o)		(G_TYPE_CHECK_INSTANCE_TYPE((o), UBERCHICK_TYPE_TREE_VIEW))
#define IS_UBERCHICK_TREE_VIEW_CLASS(k)		(G_TYPE_CHECK_CLASS_TYPE((k), UBERCHICK_TYPE_TREE_VIEW))
#define UBERCHICK_TREE_VIEW_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS((o), UBERCHICK_TYPE_TREE_VIEW, UberChickTreeViewClass))

typedef struct _UberChickTreeView      UberChickTreeView;
typedef struct _UberChickTreeViewClass UberChickTreeViewClass;
typedef struct _UberChickTreeViewPrivate  UberChickTreeViewPrivate;

typedef enum _UberChickTreeViewListStoreColumn UberChickTreeViewListStoreColumn;

struct _UberChickTreeView {
	SexyTreeView            parent;
};

struct _UberChickTreeViewClass {
	SexyTreeViewClass       parent_class;
};


GType uberchick_tree_view_get_type(void) G_GNUC_CONST;



UberChickTreeView *uberchick_tree_view_new(gint page, const gchar *timeline, OnlineService *service);

const gchar *uberchick_tree_view_get_timeline(UberChickTreeView *uberchick_tree_view);

GtkWidget *uberchick_tree_view_get_child_widget(UberChickTreeView *uberchick_tree_view);
GtkWidget *uberchick_tree_view_get_tab_widget(UberChickTreeView *uberchick_tree_view);
GtkWidget *uberchick_tree_view_get_menu_widget(UberChickTreeView *uberchick_tree_view);

OnlineService *uberchick_tree_view_get_service(UberChickTreeView *uberchick_tree_view);
GtkTreeStore *uberchick_tree_view_get_tree_store(UberChickTreeView *uberchick_tree_view);

void uberchick_tree_view_start(UberChickTreeView *uberchick_tree_view);
gboolean uberchick_tree_view_refresh(UberChickTreeView *uberchick_tree_view);
void uberchick_tree_view_stop(UberChickTreeView *uberchick_tree_view);
void uberchick_tree_view_complete(UberChickTreeView *uberchick_tree_view);



void uberchick_tree_view_store_update( UberChickTreeView *uberchick_tree_view, UserStatus *status );
void uberchick_tree_view_store_group(UberChickTreeView *uberchick_tree_view, StatusNetGroup *group);



void uberchick_tree_view_update_tree_store( UberChickTreeView *uberchick_tree_view, UberChickTreeViewListStoreColumn uberchick_tree_view_tree_store_column, gpointer value );
void uberchick_tree_view_remove_from_tree_store( UberChickTreeView *uberchick_tree_view, UberChickTreeViewListStoreColumn uberchick_tree_view_tree_store_column, gpointer value );
void uberchick_tree_view_remove_service(UberChickTreeView *uberchick_tree_view, OnlineService *service);

gboolean uberchick_tree_view_toggle_view(UberChickTreeView *uberchick_tree_view);
gboolean uberchick_tree_view_toggle_toolbar(UberChickTreeView *uberchick_tree_view);
gboolean uberchick_tree_view_toggle_from_column(UberChickTreeView *uberchick_tree_view);
gboolean uberchick_tree_view_toggle_rcpt_column(UberChickTreeView *uberchick_tree_view);

void uberchick_tree_view_labels_mark_as_read(UberChickTreeView *uberchick_tree_view);

gint8 uberchick_tree_view_has_loaded(UberChickTreeView *uberchick_tree_view);

gint uberchick_tree_view_get_page(UberChickTreeView *uberchick_tree_view);
void uberchick_tree_view_set_page(UberChickTreeView *uberchick_tree_view, gint page);

UpdateType uberchick_tree_view_get_update_type(UberChickTreeView *uberchick_tree_view);
const gchar *uberchick_tree_view_get_update_type_string(UberChickTreeView *uberchick_tree_view);
const gchar *update_type_to_string(UpdateType update_type);

gint uberchick_tree_view_get_total(UberChickTreeView *uberchick_tree_view);
guint uberchick_tree_view_get_notify_delay(UberChickTreeView *uberchick_tree_view);

void uberchick_tree_view_key_pressed(UberChickTreeView *uberchick_tree_view, GdkEventKey *event);
void uberchick_tree_view_set_image(UberChickTreeView *uberchick_tree_view, const gchar *image_filename, GtkTreeIter *iter);


G_END_DECLS
#endif /* __UBERCHICK_TREE_VIEW_H__ */
