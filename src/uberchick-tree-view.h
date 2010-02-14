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
#include "users.types.h"

G_BEGIN_DECLS


#define TYPE_UBERCHICK_TREE_VIEW		(uberchick_tree_view_get_type())
#define UBERCHICK_TREE_VIEW(o)			(G_TYPE_CHECK_INSTANCE_CAST((o), TYPE_UBERCHICK_TREE_VIEW, UberChickTreeView))
#define UBERCHICK_TREE_VIEW_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), TYPE_UBERCHICK_TREE_VIEW, UberChickTreeViewClass))
#define IS_UBERCHICK_TREE_VIEW(o)		(G_TYPE_CHECK_INSTANCE_TYPE((o), TYPE_UBERCHICK_TREE_VIEW))
#define IS_UBERCHICK_TREE_VIEW_CLASS(k)		(G_TYPE_CHECK_CLASS_TYPE((k), TYPE_UBERCHICK_TREE_VIEW))
#define UBERCHICK_TREE_VIEW_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS((o), TYPE_UBERCHICK_TREE_VIEW, UberChickTreeViewClass))

typedef struct _UberChickTreeView      UberChickTreeView;
typedef struct _UberChickTreeViewClass UberChickTreeViewClass;
typedef struct _UberChickTreeViewPrivate  UberChickTreeViewPrivate;

struct _UberChickTreeView {
	SexyTreeView            parent;
};

struct _UberChickTreeViewClass {
	SexyTreeViewClass       parent_class;
};


typedef enum _UberChickTreeViewListStoreColumn{	/********************************************************************************/
	GUINT_UBERCHICK_TREE_VIEW_INDEX,		/*	The update's index in the list-store.                                   */
	GDOUBLE_UPDATE_ID,				/*	Update's ID.								*/
	GSTRING_UPDATE_ID_STR,				/*	Update's ID string.							*/
	GDOUBLE_USER_ID,				/*	User's ID.								*/
	STRING_USER,					/*	Username string.							*/
	STRING_NICK,					/*	Author name string.							*/
	STRING_TEXT,					/*	Unformated Update string.						*/
	STRING_UPDATE,					/*	Update for display string in the UberChickTreeView's SexyTreeView.  */
	STRING_SEXY_UPDATE,				/*	libsexy formatted Update for SexyTreeView's tooltip.			*/
	STRING_DETAILED_UPDATE,				/*	Upate's string as markup w/full details..				*/
	STRING_CREATED_AGO,				/*	'Posted ?(seconds|minutes|hours|day) ago.				*/
	STRING_CREATED_AT,				/*	Date string.								*/
	GINT_CREATED_AGO,				/*	How old the post is, in seconds, for sorting.				*/
	ULONG_CREATED_AT,				/*	Unix seconds since epoc of how old the update is.			*/
	PIXBUF_AVATAR,					/*	Avatar pixbuf.								*/
	ONLINE_SERVICE,					/*	OnlineService pointer.							*/
	STRING_FROM,					/*	<status's auther @ OnlineService URI>					*/
	STRING_RCPT,					/* 	OnlineService's key this update's from. E.G. To: <user@service>		*/
	GINT_LIST_STORE_INDEX,				/*	The associated order/place this item appears in the list store.         */
	GBOOLEAN_UNREAD,				/*	If the update has been read or not					*/
	GBOOLEAN_RETWEET,				/*	If the update's a retweet or not					*/
	GCHARARRY_RETWEETED_BY,				/*	Who retweeted this update.  Its: "" if this update's not a retweet.	*/
	STRING_SEXY_STATUS_UPDATE,			/*	Update's string as markup for display.					*/
							/********************************************************************************/
} UberChickTreeViewListStoreColumn;


GType uberchick_tree_view_get_type(void) G_GNUC_CONST;



UberChickTreeView *uberchick_tree_view_new(gint page, const gchar *timeline, OnlineService *service);
void uberchick_tree_view_null_and_void_catch_all_method(UberChickTreeView *uberchick_tree_view);

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

UpdateMonitor uberchick_tree_view_get_monitoring(UberChickTreeView *uberchick_tree_view);
const gchar *uberchick_tree_view_get_monitoring_string(UberChickTreeView *uberchick_tree_view);
const gchar *monitoring_to_string(UpdateMonitor monitoring);

gint uberchick_tree_view_get_total(UberChickTreeView *uberchick_tree_view);
guint uberchick_tree_view_get_notify_delay(UberChickTreeView *uberchick_tree_view);

void uberchick_tree_view_key_pressed(UberChickTreeView *uberchick_tree_view, GdkEventKey *event);
void uberchick_tree_view_set_image(UberChickTreeView *uberchick_tree_view, const gchar *image_filename, GtkTreeIter *iter);


G_END_DECLS
#endif /* __UBERCHICK_TREE_VIEW_H__ */
