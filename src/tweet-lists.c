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
#include "tweet-lists.h"


/********************************************************************************
 *                        objects, structs, and enum typedefs                   *
 ********************************************************************************/
typedef struct _TweetLists{
	GList *tabs;
	
	/* These are the 3 default tabs & all other timlines.
	 * Each page is an embed 'control-panel'
	 * See: 'data/control-panel.ui', 'src/control-panel.c', & 'src/control-panel.h'.*/
	GtkNotebook *notebook;
} TweetLists;

static TweetLists *tweet_lists;


/********************************************************************************
 *                prototypes for private methods & functions                    *
 ********************************************************************************/
static TweetList *tweet_lists_new_tab(const gchar *timeline, OnlineService *service);


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static void tweet_lists_mark_as_read(GtkNotebook *notebook, GtkNotebookPage *page, guint page_num);


/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"UI:GtkBuilder:GtkBuildable:OnlineServices:Networking:Tweets:Requests:Users:Authentication:Preferences:Settings:Setup:Start-Up:tweet-lists.c"
#include "debug.h"


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
void tweet_lists_init( GtkNotebook *notebook ){
	tweet_lists=g_new0( TweetLists, 1 );
	
	tweet_lists->notebook=notebook;
	if( gtk_notebook_get_n_pages( tweet_lists->notebook ) )
		gtk_notebook_remove_page(tweet_lists->notebook, 0);
	
	g_signal_connect( tweet_lists->notebook, "switch-page", (GCallback)tweet_lists_mark_as_read, NULL );
	tweet_lists->tabs=NULL;
}/* tweet_lists_init( main_window->private->tweet_lists_notebook ); */

static TweetList *tweet_lists_new_tab(const gchar *timeline, OnlineService *service ){
	TweetList *tweet_list=tweet_list_new( timeline, service );
	
	tweet_lists->tabs=g_list_append(tweet_lists->tabs, tweet_list);
	tweet_lists->tabs=g_list_last( tweet_lists->tabs );
	tweet_list=tweet_lists->tabs->data;
	tweet_lists->tabs=g_list_first( tweet_lists->tabs );
	
	gint page=gtk_notebook_append_page_menu(tweet_lists->notebook, GTK_WIDGET( tweet_list_get_child( tweet_list )), GTK_WIDGET( tweet_list_get_tab( tweet_list )), GTK_WIDGET( tweet_list_get_menu( tweet_list )) );
	tweet_list_set_page(tweet_list, page);
	
	return tweet_list;
}/*tweet_lists_new_tab( "/replies.xml" );*/

TweetList *tweet_lists_get_timeline(const gchar *timeline, OnlineService *service){
	if(G_STR_EMPTY(timeline)) return NULL;
	GList *tabs=NULL;
	TweetList *tweet_list=NULL;
	for(tabs=tweet_lists->tabs; tabs; tabs=tabs->next)
		if(g_str_equal(tweet_list_get_timeline((TweetList *)tabs->data), timeline)){
			tweet_list=(TweetList *)tabs->data;
			gtk_notebook_set_current_page(tweet_lists->notebook, tweet_list_get_page( tweet_list ));
			return tweet_list;
		}
	g_list_free(tabs);
	return tweet_lists_new_tab( timeline, service );
}/*main_window_tweets_list_get( "/direct_messages.xml", (NULL|service) );*/

static void tweet_lists_mark_as_read(GtkNotebook *notebook, GtkNotebookPage *page, guint page_num){
	tweet_list_mark_as_read(tweet_lists_get_page(page_num, FALSE));
}/*tweet_lists_mark_as_read(tweet_lists->notebook, page, 0, main_window);*/

TweetList *tweet_lists_get_next( void ){
	if( gtk_notebook_get_n_pages( tweet_lists->notebook )==1) return tweet_lists_get_current();
	TweetList *current=tweet_lists_get_current();
	gtk_notebook_next_page( tweet_lists->notebook );
	TweetList *next=tweet_lists_get_current();
	if( current!=next ) return next;
	gtk_notebook_set_current_page(tweet_lists->notebook, 0);
	return tweet_lists_get_current();
}/*tweet_lists_current()*/

TweetList *tweet_lists_get_current( void ){
	return tweet_lists_get_page( gtk_notebook_get_current_page( tweet_lists->notebook ), FALSE);
}/*tweet_lists_current()*/

TweetList *tweet_lists_get_previous( void ){
	if( gtk_notebook_get_n_pages( tweet_lists->notebook )==1) return tweet_lists_get_current();
	TweetList *current=tweet_lists_get_current();
	gtk_notebook_prev_page( tweet_lists->notebook );
	TweetList *previous=tweet_lists_get_current();
	if( current!=previous ) return previous;
	gtk_notebook_set_current_page(tweet_lists->notebook,( gtk_notebook_get_n_pages( tweet_lists->notebook )-1));
	return tweet_lists_get_current();
}/*tweet_lists_current()*/

TweetList *tweet_lists_get_page(gint page, gboolean close){
	GList *t=NULL;
	gint tweet_list_page=0;
	TweetList *tweet_list=NULL;
	for(t=tweet_lists->tabs; t; t=t->next){
		tweet_list_page=tweet_list_get_page((TweetList *)t->data);
		if( tweet_list_page==page ){
			tweet_list=(TweetList *)t->data;
			if( !close ) return tweet_list;
		}else if( tweet_list_page > page )
			tweet_list_set_page( (TweetList *)t->data, page-1 );
	}
	return tweet_list;
}/*tweet_lists_get_page(0, TRUE|FALSE);*/

void tweet_lists_start( void ){
	GList *tabs=NULL;
	for(tabs=tweet_lists->tabs; tabs; tabs=tabs->next)
		tweet_list_start((TweetList *)tabs->data);
	g_list_free(tabs);
}/*tweet_lists_start();*/

void tweet_lists_refresh( void ){
	GList *tabs=NULL;
	for(tabs=tweet_lists->tabs; tabs; tabs=tabs->next)
		tweet_list_refresh((TweetList *)tabs->data);
	g_list_free(tabs);
}/*tweet_lists_refresh();*/

void tweet_lists_stop( void ){
	GList *tabs=NULL;
	for(tabs=tweet_lists->tabs; tabs; tabs=tabs->next)
		tweet_list_stop((TweetList *)tabs->data);
	g_list_free(tabs);
}/*tweet_lists_stop();*/

void tweet_lists_close( void ){
	GList *tabs=NULL;
	for(tabs=tweet_lists->tabs; tabs; tabs=tabs->next)
		tweet_lists_close_page(tweet_list_get_page((TweetList *)tabs->data));
	g_list_free(tabs);
}/*tweet_lists_stop();*/

void tweet_lists_close_current_page( void ){
	tweet_lists_close_page( gtk_notebook_get_current_page( tweet_lists->notebook ));
}/*tweet_lists_close_current_page();*/

void tweet_lists_close_page(gint page){
	TweetList *tweet_list=tweet_lists_get_page(page, TRUE);
	gtk_notebook_remove_page(tweet_lists->notebook, page);
	tweet_lists->tabs=g_list_remove(tweet_lists->tabs, tweet_list);
	g_object_unref( tweet_list );
}/*void tweet_lists_close_page(0);*/

void tweet_lists_toggle_toolbars( void ){
	GList *tabs=NULL;
	for(tabs=tweet_lists->tabs; tabs; tabs=tabs->next)
		tweet_list_toggle_toolbar((TweetList *)tabs->data);
	g_list_free(tabs);
}/*tweet_lists_toggle_toolbars();*/

void tweet_lists_destroy( void ){
	TweetList *tweet_list=NULL;
	GList *tabs=NULL;
	for(tabs=tweet_lists->tabs; tabs; tabs=tabs->next){
		tweet_list=(TweetList *)tabs->data;
		tweet_list_stop( tweet_list );
		gtk_notebook_remove_page(tweet_lists->notebook, tweet_list_get_page( tweet_list ));
		g_object_unref( tweet_list );
	}
	g_list_free( tabs );
	g_list_free( tweet_lists->tabs );
	
	tweet_lists->notebook=NULL;
	uber_free( tweet_lists );
}/*tweet_lists_destroy();*/


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
