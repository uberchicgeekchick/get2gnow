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
#define _GNU_SOURCE
#define _THREAD_SAFE

/********************************************************************************
 *      Project, system, & library headers.  eg #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/
#include "program.h"
#include "online-service.typedefs.h"
#include "online-services.types.h"
#include "online-services.typedefs.h"

#include "main-window.h"
#include "uberchick-tree-view.h"
#include "tabs.h"


/********************************************************************************
 *                        defines, macros, methods, & etc                       *
 ********************************************************************************/


/********************************************************************************
 *                        objects, structs, and enum typedefs                   *
 ********************************************************************************/
typedef struct _TimelineTabs{
	GList		*tabs;
	
	/* These are the 3 default tabs & all other timlines.
	 * Each page is an embed 'uberchick-tree-view' widget.
	 * See: 'data/uberchick-tree-view.in.ui', 'src/uberchick-tree-view.c', & 'src/uberchick-tree-view.h'.*/
	GtkNotebook	*notebook;
	gint		previous_page;
	gint		current_page;
	gint		pages;
} TimelineTabs;

static TimelineTabs *tabs;


/********************************************************************************
 *                prototypes for private methods & functions                    *
 ********************************************************************************/
static UberChickTreeView *tabs_get_page(gint page, gboolean close);
static UberChickTreeView *tabs_new_tab(const gchar *timeline, OnlineService *service);
static void tabs_set_page(gint page);


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static void tabs_mark_as_read(GtkNotebook *notebook, GtkNotebookPage *page, guint page_num);


/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Updates:Requests:Users:Authentication:Preferences:Settings:Setup:Start-Up:tabs.c"
#include "debug.h"


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
void tabs_init(GtkNotebook *notebook){
	tabs=g_new0(TimelineTabs, 1);
	
	tabs->notebook=notebook;
	if(gtk_notebook_get_n_pages(tabs->notebook))
		gtk_notebook_remove_page(tabs->notebook, 0);
	
	g_signal_connect( tabs->notebook, "switch-page", (GCallback)tabs_mark_as_read, NULL );
	tabs->tabs=NULL;
	
	tabs->previous_page=tabs->current_page=-1;
	tabs->pages=0;
}/* tabs_init(main_window->private->tabs_notebook); */

static UberChickTreeView *tabs_new_tab(const gchar *timeline, OnlineService *service){
	gint page=tabs->pages;
	UberChickTreeView *uberchick_tree_view=uberchick_tree_view_new(page, timeline, service);
	
	tabs->tabs=g_list_append(tabs->tabs, uberchick_tree_view);
	tabs->tabs=g_list_last(tabs->tabs);
	uberchick_tree_view=tabs->tabs->data;
	tabs->tabs=g_list_first(tabs->tabs);
	
	GtkWidget *uberchick_tree_view_child=uberchick_tree_view_get_child_widget(uberchick_tree_view);
	GtkWidget *uberchick_tree_view_tab=uberchick_tree_view_get_tab_widget(uberchick_tree_view);
	GtkWidget *uberchick_tree_view_menu=uberchick_tree_view_get_menu_widget(uberchick_tree_view);
	
	tabs->pages++;
	
	gtk_notebook_append_page_menu(tabs->notebook, uberchick_tree_view_child, uberchick_tree_view_tab, uberchick_tree_view_menu);
	GtkWidget *tab_label=gtk_notebook_get_tab_label(tabs->notebook, uberchick_tree_view_child);
	gtk_notebook_set_tab_label_packing(tabs->notebook, tab_label, FALSE, FALSE, GTK_PACK_START);
	tabs_set_page(page);
	
	return uberchick_tree_view;
}/*tabs_new_tab("/replies.xml");*/

UberChickTreeView *tabs_open_timeline(const gchar *timeline, OnlineService *service){
	if(G_STR_EMPTY(timeline)) return NULL;
	GList *t=NULL;
	UberChickTreeView *uberchick_tree_view=NULL;
	for(t=tabs->tabs; t; t=t->next){
		if(!g_str_equal(uberchick_tree_view_get_timeline(UBERCHICK_TREE_VIEW(t->data)), timeline))
			continue;
		
		uberchick_tree_view=UBERCHICK_TREE_VIEW(t->data);
		tabs_set_page(uberchick_tree_view_get_page(uberchick_tree_view));
		return uberchick_tree_view;
	}
	g_list_free(t);
	return (uberchick_tree_view ? uberchick_tree_view :tabs_new_tab(timeline, service) );
}/*tabs_open_timeline( "/direct_messages.xml", (NULL|service) );*/

void tabs_close_timeline(const gchar *timeline){
	if(G_STR_EMPTY(timeline)) return;
	GList *t=NULL;
	UberChickTreeView *uberchick_tree_view=NULL;
	for(t=tabs->tabs; t; t=t->next){
		if(!g_str_equal(uberchick_tree_view_get_timeline(UBERCHICK_TREE_VIEW(t->data)), timeline))
			continue;
		uberchick_tree_view=UBERCHICK_TREE_VIEW(t->data);
		tabs_close_page(uberchick_tree_view_get_page(uberchick_tree_view));
	}
	g_list_free(t);
}/*tabs_close_timeline( "/direct_messages.xml", (NULL|service) );*/

static void tabs_mark_as_read(GtkNotebook *notebook, GtkNotebookPage *page, guint page_num){
	uberchick_tree_view_labels_mark_as_read(tabs_get_page(page_num, FALSE));
}/*tabs_mark_as_read(tabs->notebook, uberchick_tree_view, 0);*/

UberChickTreeView *tabs_get_next(void){
	if(tabs->pages==1) return tabs_get_current();
	UberChickTreeView *current=tabs_get_current();
	gtk_notebook_set_current_page(tabs->notebook, uberchick_tree_view_get_page(current)+1);
	UberChickTreeView *next=tabs_get_current();
	if(current!=next){
		tabs_set_page(uberchick_tree_view_get_page(next));
		return next;
	}
	tabs_set_page(0);
	return tabs_get_current();
}/*tabs_get_next()*/

UberChickTreeView *tabs_get_current(void){
	return tabs_get_page(gtk_notebook_get_current_page(tabs->notebook), FALSE);
}/*tabs_get_current()*/

UberChickTreeView *tabs_get_previous(void){
	if(tabs->pages==1) return tabs_get_current();
	UberChickTreeView *current=tabs_get_current();
	gtk_notebook_set_current_page(tabs->notebook, uberchick_tree_view_get_page(current)-1);
	UberChickTreeView *previous=tabs_get_current();
	if(current!=previous){
		tabs_set_page(uberchick_tree_view_get_page(previous));
		return previous;
	}
	tabs_set_page(tabs->pages-1);
	return tabs_get_current();
}/*tabs_get_previous()*/

UberChickTreeView *tabs_view_page(gint page){
	GList *t=NULL;
	UberChickTreeView *uberchick_tree_view=NULL;
	for(t=tabs->tabs; t; t=t->next){
		if(page!=uberchick_tree_view_get_page(UBERCHICK_TREE_VIEW(t->data)))
			continue;
		
		uberchick_tree_view=UBERCHICK_TREE_VIEW(t->data);
		tabs_set_page(page);
	}
	g_list_free(t);
	return uberchick_tree_view;
}/*tabs_view_page(0);*/

static UberChickTreeView *tabs_get_page(gint page, gboolean close){
	GList *t=NULL;
	gint uberchick_tree_view_page=0;
	UberChickTreeView *uberchick_tree_view=NULL;
	for(t=tabs->tabs; t; t=t->next){
		uberchick_tree_view_page=uberchick_tree_view_get_page(UBERCHICK_TREE_VIEW(t->data));
		if(uberchick_tree_view_page==page){
			uberchick_tree_view=UBERCHICK_TREE_VIEW(t->data);
			if(!close) return uberchick_tree_view;
		}else if(uberchick_tree_view_page > page)
			uberchick_tree_view_set_page(UBERCHICK_TREE_VIEW(t->data), uberchick_tree_view_page-1);
	}
	g_list_free(t);
	return uberchick_tree_view;
}/*tabs_get_page(0, TRUE|FALSE);*/

void tabs_start(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		uberchick_tree_view_start(UBERCHICK_TREE_VIEW(t->data));
	g_list_free(t);
}/*tabs_start();*/

void tabs_remove_from_uberchick_tree_views_tree_stores( UberChickTreeViewListStoreColumn uberchick_tree_view_tree_store_column, gpointer value ){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		uberchick_tree_view_remove_from_tree_store(UBERCHICK_TREE_VIEW(t->data), uberchick_tree_view_tree_store_column, value);
	
	g_list_free(t);
}/*tabs_remove_from_uberchick_tree_views_tree_stores( UberChickTreeViewStoreColumn, value );*/

void tabs_remove_service(OnlineService *service){
	GList *t=NULL;
	UberChickTreeView *uberchick_tree_view;
	for(t=tabs->tabs; t; t=t->next){
		uberchick_tree_view=UBERCHICK_TREE_VIEW(t->data);
		if(uberchick_tree_view_get_service(uberchick_tree_view) == service)
			tabs_close_page(uberchick_tree_view_get_page(uberchick_tree_view));
		else
			uberchick_tree_view_remove_service(uberchick_tree_view, service);
	}
	g_list_free(t);
}/*tabs_remove_service(service);*/

void tabs_refresh(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		uberchick_tree_view_refresh(UBERCHICK_TREE_VIEW(t->data));
	g_list_free(t);
}/*tabs_refresh();*/

void tabs_stop(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		uberchick_tree_view_stop(UBERCHICK_TREE_VIEW(t->data));
	g_list_free(t);
}/*tabs_stop();*/

void tabs_close(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		tabs_close_page(uberchick_tree_view_get_page(UBERCHICK_TREE_VIEW(t->data)));
	
	g_list_free(t);
}/*tabs_close();*/

void tabs_close_current_page(void){
	gint page=gtk_notebook_get_current_page(tabs->notebook);
	UberChickTreeView *uberchick_tree_view=tabs_get_page(page, FALSE);
	if(!main_window_tabs_menu_set_active(uberchick_tree_view_get_timeline(uberchick_tree_view), FALSE))
		tabs_close_page(page);
}/*tabs_close_current_page();*/

void tabs_close_page(gint page){
	UberChickTreeView *uberchick_tree_view=tabs_get_page(page, TRUE);
	uberchick_tree_view_stop(uberchick_tree_view);
	gtk_notebook_remove_page(tabs->notebook, page);
	
	tabs->pages--;
	if(tabs->previous_page <= tabs->pages)
		tabs_set_page(tabs->previous_page);
	
	tabs->tabs=g_list_remove(tabs->tabs, uberchick_tree_view);
	gtk_widget_destroy(GTK_WIDGET(uberchick_tree_view));
}/*void tabs_close_page(0);*/

static void tabs_set_page(gint page){
	if(tabs->current_page > -1)
		if(tabs->current_page <= tabs->pages)
			tabs->previous_page=tabs->current_page;
		else
			tabs->previous_page=0;
	
	tabs->current_page=page;
	
	gtk_notebook_set_current_page(tabs->notebook, page);
}/*tabs_set_page(0);*/

void tabs_toggle_view(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		uberchick_tree_view_toggle_view(UBERCHICK_TREE_VIEW(t->data));
	g_list_free(t);
}/*tabs_toggle_view();*/

void tabs_toggle_toolbars(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		uberchick_tree_view_toggle_toolbar(UBERCHICK_TREE_VIEW(t->data));
	g_list_free(t);
}/*tabs_toggle_toolbars();*/

void tabs_toggle_from_columns(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		uberchick_tree_view_toggle_from_column(UBERCHICK_TREE_VIEW(t->data));
	g_list_free(t);
}/*tabs_toggle_from_columns();*/

void tabs_toggle_rcpt_columns(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		uberchick_tree_view_toggle_rcpt_column(UBERCHICK_TREE_VIEW(t->data));
	g_list_free(t);
}/*tabs_toggle_rcpt_columns();*/

void tabs_destroy(void){
	tabs_close();
	g_list_free(tabs->tabs);
	
	tabs->notebook=NULL;
	uber_free(tabs);
}/*tabs_destroy();*/


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
