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
#include "timelines-sexy-tree-view.h"
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
	 * Each page is an embed 'update-viewer'
	 * See: 'data/update-viewer.ui', 'src/update-viewer.c', & 'src/update-viewer.h'.*/
	GtkNotebook *notebook;
} TimelineTabs;

static TimelineTabs *tabs;


/********************************************************************************
 *                prototypes for private methods & functions                    *
 ********************************************************************************/
static TimelinesSexyTreeView *tabs_new_tab(const gchar *timeline, OnlineService *service);


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

static TimelinesSexyTreeView *tabs_new_tab(const gchar *timeline, OnlineService *service ){
	TimelinesSexyTreeView *timelines_sexy_tree_view=timelines_sexy_tree_view_new(timeline, service);
	
	tabs->tabs=g_list_append(tabs->tabs, timelines_sexy_tree_view);
	tabs->tabs=g_list_last(tabs->tabs);
	timelines_sexy_tree_view=tabs->tabs->data;
	tabs->tabs=g_list_first(tabs->tabs);
	
	gint page=gtk_notebook_append_page_menu(tabs->notebook, GTK_WIDGET(timelines_sexy_tree_view_get_child(timelines_sexy_tree_view)), GTK_WIDGET(timelines_sexy_tree_view_get_tab(timelines_sexy_tree_view)), GTK_WIDGET(timelines_sexy_tree_view_get_menu(timelines_sexy_tree_view)) );
	timelines_sexy_tree_view_set_page(timelines_sexy_tree_view, page);
	gtk_notebook_set_current_page( tabs->notebook, page );
	
	return timelines_sexy_tree_view;
}/*tabs_new_tab("/replies.xml");*/

TimelinesSexyTreeView *tabs_open_timeline(const gchar *timeline, OnlineService *service){
	if(G_STR_EMPTY(timeline)) return NULL;
	GList *t=NULL;
	TimelinesSexyTreeView *timelines_sexy_tree_view=NULL;
	for(t=tabs->tabs; t; t=t->next)
		if(g_str_equal(timelines_sexy_tree_view_get_timeline((TimelinesSexyTreeView *)t->data), timeline)){
			timelines_sexy_tree_view=(TimelinesSexyTreeView *)t->data;
			gtk_notebook_set_current_page(tabs->notebook, timelines_sexy_tree_view_get_page(timelines_sexy_tree_view));
			return timelines_sexy_tree_view;
		}
	g_list_free(t);
	return tabs_new_tab(timeline, service);
}/*main_window_tweets_list_get( "/direct_messages.xml", (NULL|service) );*/

void tabs_close_timeline(const gchar *timeline){
	if(G_STR_EMPTY(timeline)) return;
	GList *t=NULL;
	gboolean timeline_found=FALSE;
	TimelinesSexyTreeView *timelines_sexy_tree_view=NULL;
	for(t=tabs->tabs; t && !timeline_found; t=t->next)
		if(g_str_equal(timelines_sexy_tree_view_get_timeline((TimelinesSexyTreeView *)t->data), timeline)){
			timelines_sexy_tree_view=(TimelinesSexyTreeView *)t->data;
			tabs_close_page(timelines_sexy_tree_view_get_page(timelines_sexy_tree_view));
			timeline_found=TRUE;
		}
	g_list_free(t);
}/*main_window_tweets_list_get( "/direct_messages.xml", (NULL|service) );*/

static void tabs_mark_as_read(GtkNotebook *notebook, GtkNotebookPage *page, guint page_num){
	timelines_sexy_tree_view_mark_as_read(tabs_get_page(page_num, FALSE));
}/*tabs_mark_as_read(tabs->notebook, page, 0, main_window);*/

TimelinesSexyTreeView *tabs_get_next(void){
	if(gtk_notebook_get_n_pages(tabs->notebook)==1) return tabs_get_current();
	TimelinesSexyTreeView *current=tabs_get_current();
	gtk_notebook_next_page(tabs->notebook);
	TimelinesSexyTreeView *next=tabs_get_current();
	if(current!=next) return next;
	gtk_notebook_set_current_page(tabs->notebook, 0);
	return tabs_get_current();
}/*tabs_current()*/

TimelinesSexyTreeView *tabs_get_current(void){
	return tabs_get_page(gtk_notebook_get_current_page(tabs->notebook), FALSE);
}/*tabs_current()*/

TimelinesSexyTreeView *tabs_get_previous(void){
	if(gtk_notebook_get_n_pages(tabs->notebook)==1) return tabs_get_current();
	TimelinesSexyTreeView *current=tabs_get_current();
	gtk_notebook_prev_page(tabs->notebook);
	TimelinesSexyTreeView *previous=tabs_get_current();
	if(current!=previous) return previous;
	gtk_notebook_set_current_page(tabs->notebook, (gtk_notebook_get_n_pages(tabs->notebook)-1));
	return tabs_get_current();
}/*tabs_current()*/

TimelinesSexyTreeView *tabs_view_page(gint page){
	GList *t=NULL;
	gint timelines_sexy_tree_view_page=0;
	TimelinesSexyTreeView *timelines_sexy_tree_view=NULL;
	for(t=tabs->tabs; t; t=t->next){
		timelines_sexy_tree_view_page=timelines_sexy_tree_view_get_page((TimelinesSexyTreeView *)t->data);
		if(timelines_sexy_tree_view_page==page){
			timelines_sexy_tree_view=(TimelinesSexyTreeView *)t->data;
			gtk_notebook_set_current_page( tabs->notebook, page );
			return timelines_sexy_tree_view;
		}
	}
	return NULL;
}/*tabs_view_page(0);*/

TimelinesSexyTreeView *tabs_get_page(gint page, gboolean close){
	GList *t=NULL;
	gint timelines_sexy_tree_view_page=0;
	TimelinesSexyTreeView *timelines_sexy_tree_view=NULL;
	for(t=tabs->tabs; t; t=t->next){
		timelines_sexy_tree_view_page=timelines_sexy_tree_view_get_page((TimelinesSexyTreeView *)t->data);
		if(timelines_sexy_tree_view_page==page){
			timelines_sexy_tree_view=(TimelinesSexyTreeView *)t->data;
			if(!close) return timelines_sexy_tree_view;
		}else if(timelines_sexy_tree_view_page > page)
			timelines_sexy_tree_view_set_page( (TimelinesSexyTreeView *)t->data, page-1 );
	}
	return timelines_sexy_tree_view;
}/*tabs_get_page(0, TRUE|FALSE);*/

void tabs_start(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		timelines_sexy_tree_view_start((TimelinesSexyTreeView *)t->data);
	g_list_free(t);
}/*tabs_start();*/

void tabs_remove_from_timelines_sexy_tree_views_list_stores( TimelinesSexyTreeViewListStoreColumn timelines_sexy_tree_view_list_store_column, gpointer value ){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		timelines_sexy_tree_view_remove_from_list_store((TimelinesSexyTreeView *)t->data, timelines_sexy_tree_view_list_store_column, value);
	
	g_list_free(t);
}/*tabs_remove_from_timelines_sexy_tree_views_list_stores( TimelinesSexyTreeViewStoreColumn, value );*/

void tabs_remove_service(OnlineService *service){
	GList *t=NULL;
	TimelinesSexyTreeView *timelines_sexy_tree_view;
	for(t=tabs->tabs; t; t=t->next){
		timelines_sexy_tree_view=(TimelinesSexyTreeView *)t->data;
		if(timelines_sexy_tree_view_get_service(timelines_sexy_tree_view) == service)
			tabs_close_page(timelines_sexy_tree_view_get_page(timelines_sexy_tree_view));
		else
			timelines_sexy_tree_view_remove_service(timelines_sexy_tree_view, service);
	}
	g_list_free(t);
}/*tabs_remove_service(service);*/

void tabs_refresh(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		timelines_sexy_tree_view_refresh((TimelinesSexyTreeView *)t->data);
	g_list_free(t);
}/*tabs_refresh();*/

void tabs_stop(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		timelines_sexy_tree_view_stop((TimelinesSexyTreeView *)t->data);
	g_list_free(t);
}/*tabs_stop();*/

void tabs_close(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next){
		TimelinesSexyTreeView *timelines_sexy_tree_view=(TimelinesSexyTreeView *)t->data;
		main_window_tabs_menu_set_active(timelines_sexy_tree_view_get_timeline(timelines_sexy_tree_view), FALSE);
		tabs_close_page(timelines_sexy_tree_view_get_page(timelines_sexy_tree_view));
	}
	g_list_free(t);
}/*tabs_close();*/

void tabs_close_current_page(void){
	tabs_close_page(gtk_notebook_get_current_page(tabs->notebook));
}/*tabs_close_current_page();*/

void tabs_close_page(gint page){
	TimelinesSexyTreeView *timelines_sexy_tree_view=tabs_get_page(page, TRUE);
	gtk_notebook_remove_page(tabs->notebook, page);
	tabs->tabs=g_list_remove(tabs->tabs, timelines_sexy_tree_view);
	g_object_unref(timelines_sexy_tree_view);
}/*void tabs_close_page(0);*/

void tabs_toggle_toolbars(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		timelines_sexy_tree_view_toggle_toolbar((TimelinesSexyTreeView *)t->data);
	g_list_free(t);
}/*tabs_toggle_toolbars();*/

void tabs_toggle_from_columns(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		timelines_sexy_tree_view_toggle_from_column((TimelinesSexyTreeView *)t->data);
	g_list_free(t);
}/*tabs_toggle_from_columns();*/

void tabs_toggle_rcpt_columns(void){
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next)
		timelines_sexy_tree_view_toggle_rcpt_column((TimelinesSexyTreeView *)t->data);
	g_list_free(t);
}/*tabs_toggle_rcpt_columns();*/

void tabs_destroy(void){
	TimelinesSexyTreeView *timelines_sexy_tree_view=NULL;
	GList *t=NULL;
	for(t=tabs->tabs; t; t=t->next){
		timelines_sexy_tree_view=(TimelinesSexyTreeView *)t->data;
		timelines_sexy_tree_view_stop(timelines_sexy_tree_view);
		gtk_notebook_remove_page(tabs->notebook, timelines_sexy_tree_view_get_page(timelines_sexy_tree_view));
		g_object_unref(timelines_sexy_tree_view);
	}
	g_list_free(t);
	g_list_free(tabs->tabs);
	
	tabs->notebook=NULL;
	uber_free(tabs);
}/*tabs_destroy();*/


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
