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
#include "online-service.types.h"
#include "online-services.types.h"
#include "online-services-typedefs.h"

#include "main-window.h"
#include "update-viewer.h"
#include "tabs.h"


/********************************************************************************
 *                        defines, macros, methods, & etc                       *
 ********************************************************************************/


/********************************************************************************
 *                        objects, structs, and enum typedefs                   *
 ********************************************************************************/
typedef struct _TimelineTabs{
	GList *tabs;
	
	/* These are the 3 default tabs & all other timlines.
	 * Each page is an embed 'control-panel'
	 * See: 'data/control-panel.ui', 'src/control-panel.c', & 'src/control-panel.h'.*/
	GtkNotebook *notebook;
} TimelineTabs;

static TimelineTabs *tabs;


/********************************************************************************
 *                prototypes for private methods & functions                    *
 ********************************************************************************/
static UpdateViewer *tabs_new_tab(const gchar *timeline, OnlineService *service);


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static void tabs_mark_as_read(GtkNotebook *notebook, GtkNotebookPage *page, guint page_num);


/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Tweets:Requests:Users:Authentication:Preferences:Settings:Setup:Start-Up:tabs.c"
#include "debug.h"


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
void tabs_init(GtkNotebook *notebook){
	tabs=g_new0(TimelineTabs, 1);
	
	tabs->notebook=notebook;
	if(gtk_notebook_get_n_pages(tabs->notebook) )
		gtk_notebook_remove_page(tabs->notebook, 0);
	
	g_signal_connect( tabs->notebook, "switch-page", (GCallback)tabs_mark_as_read, NULL );
	tabs->tabs=NULL;
}/* tabs_init(main_window->private->tabs_notebook); */

static UpdateViewer *tabs_new_tab(const gchar *timeline, OnlineService *service ){
	UpdateViewer *update_viewer=update_viewer_new(timeline, service);
	
	tabs->tabs=g_list_append(tabs->tabs, update_viewer);
	tabs->tabs=g_list_last(tabs->tabs);
	update_viewer=tabs->tabs->data;
	tabs->tabs=g_list_first(tabs->tabs);
	
	gint page=gtk_notebook_append_page_menu(tabs->notebook, GTK_WIDGET(update_viewer_get_child(update_viewer)), GTK_WIDGET(update_viewer_get_tab(update_viewer)), GTK_WIDGET(update_viewer_get_menu(update_viewer)) );
	update_viewer_set_page(update_viewer, page);
	gtk_notebook_set_current_page( tabs->notebook, page );
	
	return update_viewer;
}/*tabs_new_tab("/replies.xml");*/

UpdateViewer *tabs_open_timeline(const gchar *timeline, OnlineService *service){
	if(G_STR_EMPTY(timeline)) return NULL;
	GList *t=NULL;
	UpdateViewer *update_viewer=NULL;
	for(t=tabs->tabs; t; t=t->next)
		if(g_str_equal(update_viewer_get_timeline((UpdateViewer *)t->data), timeline)){
			update_viewer=(UpdateViewer *)t->data;
			gtk_notebook_set_current_page(tabs->notebook, update_viewer_get_page(update_viewer));
			return update_viewer;
		}
	g_list_free(t);
	return tabs_new_tab(timeline, service);
}/*main_window_tweets_list_get( "/direct_messages.xml", (NULL|service) );*/

void tabs_close_timeline(const gchar *timeline){
	if(G_STR_EMPTY(timeline)) return;
	GList *t=NULL;
	gboolean timeline_found=FALSE;
	UpdateViewer *update_viewer=NULL;
	for(t=tabs->tabs; t && !timeline_found; t=t->next)
		if(g_str_equal(update_viewer_get_timeline((UpdateViewer *)t->data), timeline)){
			update_viewer=(UpdateViewer *)t->data;
			tabs_close_page(update_viewer_get_page(update_viewer));
			timeline_found=TRUE;
		}
	g_list_free(t);
}/*main_window_tweets_list_get( "/direct_messages.xml", (NULL|service) );*/

static void tabs_mark_as_read(GtkNotebook *notebook, GtkNotebookPage *page, guint page_num){
	update_viewer_mark_as_read(tabs_get_page(page_num, FALSE));
}/*tabs_mark_as_read(tabs->notebook, page, 0, main_window);*/

UpdateViewer *tabs_get_next(void){
	if(gtk_notebook_get_n_pages(tabs->notebook)==1) return tabs_get_current();
	UpdateViewer *current=tabs_get_current();
	gtk_notebook_next_page(tabs->notebook);
	UpdateViewer *next=tabs_get_current();
	if(current!=next) return next;
	gtk_notebook_set_current_page(tabs->notebook, 0);
	return tabs_get_current();
}/*tabs_current()*/

UpdateViewer *tabs_get_current(void){
	return tabs_get_page(gtk_notebook_get_current_page(tabs->notebook), FALSE);
}/*tabs_current()*/

UpdateViewer *tabs_get_previous(void){
	if(gtk_notebook_get_n_pages(tabs->notebook)==1) return tabs_get_current();
	UpdateViewer *current=tabs_get_current();
	gtk_notebook_prev_page(tabs->notebook);
	UpdateViewer *previous=tabs_get_current();
	if(current!=previous) return previous;
	gtk_notebook_set_current_page(tabs->notebook, (gtk_notebook_get_n_pages(tabs->notebook)-1));
	return tabs_get_current();
}/*tabs_current()*/

UpdateViewer *tabs_view_page(gint page){
	GList *t=NULL;
	gint update_viewer_page=0;
	UpdateViewer *update_viewer=NULL;
	for(t=tabs->tabs; t; t=t->next){
		update_viewer_page=update_viewer_get_page((UpdateViewer *)t->data);
		if(update_viewer_page==page){
			update_viewer=(UpdateViewer *)t->data;
			gtk_notebook_set_current_page( tabs->notebook, page );
			return update_viewer;
		}
	}
	return NULL;
}/*tabs_view_page(0);*/

UpdateViewer *tabs_get_page(gint page, gboolean close){
	GList *t=NULL;
	gint update_viewer_page=0;
	UpdateViewer *update_viewer=NULL;
	for(t=tabs->tabs; t; t=t->next){
		update_viewer_page=update_viewer_get_page((UpdateViewer *)t->data);
		if(update_viewer_page==page){
			update_viewer=(UpdateViewer *)t->data;
			if(!close) return update_viewer;
		}else if(update_viewer_page > page)
			update_viewer_set_page( (UpdateViewer *)t->data, page-1 );
	}
	return update_viewer;
}/*tabs_get_page(0, TRUE|FALSE);*/

void tabs_start(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		update_viewer_start((UpdateViewer *)t->data);
	g_list_free(t);
}/*tabs_start();*/

void tabs_remove_from_update_viewers_list_stores( UpdateViewerListStoreColumn update_viewer_list_store_column, gpointer value ){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		update_viewer_remove_from_list_store((UpdateViewer *)t->data, update_viewer_list_store_column, value);
	
	g_list_free(t);
}/*tabs_remove_from_update_viewers_list_stores( UpdateViewerStoreColumn, value );*/

void tabs_remove_service(OnlineService *service){
	GList *t=NULL;
	UpdateViewer *update_viewer;
	for(t=tabs->tabs; t; t=t->next){
		update_viewer=(UpdateViewer *)t->data;
		if(update_viewer_get_service(update_viewer) == service)
			tabs_close_page(update_viewer_get_page(update_viewer));
		else
			update_viewer_remove_service(update_viewer, service);
	}
	g_list_free(t);
}/*tabs_remove_service(service);*/

void tabs_refresh(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		update_viewer_refresh((UpdateViewer *)t->data);
	g_list_free(t);
}/*tabs_refresh();*/

void tabs_stop(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		update_viewer_stop((UpdateViewer *)t->data);
	g_list_free(t);
}/*tabs_stop();*/

void tabs_close(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next){
		UpdateViewer *update_viewer=(UpdateViewer *)t->data;
		main_window_tabs_menu_set_active(update_viewer_get_timeline(update_viewer), FALSE);
		tabs_close_page(update_viewer_get_page(update_viewer));
	}
	g_list_free(t);
}/*tabs_close();*/

void tabs_close_current_page(void){
	tabs_close_page(gtk_notebook_get_current_page(tabs->notebook));
}/*tabs_close_current_page();*/

void tabs_close_page(gint page){
	UpdateViewer *update_viewer=tabs_get_page(page, TRUE);
	gtk_notebook_remove_page(tabs->notebook, page);
	tabs->tabs=g_list_remove(tabs->tabs, update_viewer);
	g_object_unref(update_viewer);
}/*void tabs_close_page(0);*/

void tabs_toggle_toolbars(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		update_viewer_toggle_toolbar((UpdateViewer *)t->data);
	g_list_free(t);
}/*tabs_toggle_toolbars();*/

void tabs_toggle_from_columns(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		update_viewer_toggle_from_column((UpdateViewer *)t->data);
	g_list_free(t);
}/*tabs_toggle_from_columns();*/

void tabs_toggle_rcpt_columns(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		update_viewer_toggle_rcpt_column((UpdateViewer *)t->data);
	g_list_free(t);
}/*tabs_toggle_rcpt_columns();*/

void tabs_destroy(void){
	UpdateViewer *update_viewer=NULL;
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next){
		update_viewer=(UpdateViewer *)t->data;
		update_viewer_stop(update_viewer);
		gtk_notebook_remove_page(tabs->notebook, update_viewer_get_page(update_viewer));
		g_object_unref(update_viewer);
	}
	g_list_free(t);
	g_list_free(tabs->tabs);
	
	tabs->notebook=NULL;
	uber_free(tabs);
}/*tabs_destroy();*/


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
