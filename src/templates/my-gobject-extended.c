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
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <libsexy/sexy.h>

#include "config.h"
#include "main.h"
#include "my-gobject.h"


/********************************************************************************
 *        Methods, macros, constants, objects, structs, and enum typedefs       *
 ********************************************************************************/
#define	GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_MY_GOBJECT, MyGObjectPrivate))

typedef struct _MyGObjectTitles MyGObjectTitles;
typedef enum _MyGObject_ListStore_Rows MyGObject_ListStore_Rows;

struct _MyGObjectTitles{
	guint		page;
	const gchar	*uri;
	const gchar	*title;
};

MyGObjectTitles MyGObjectTitlesList[]={
	{1,	"http://uberchicgeekchick.com/",			N_("Expressive Programming")},
	{2,	"http://uberchicgeekchick.com/?episodes=0008",		N_("Expressive Programming - episode 0008 - if(!g_thread_supported()) g_thread_init(NULL);")},
	{3,	"http://uberchicgeekchick.com/?projects=get2gnow",	N_("Expressive Programming - My OSS projects: get2gnow")},
	{4,	"http://uberchicgeekchick.com/?projects=connectED",	N_("Expressive Programming - My OSS projects: connectED")},
	{5,	"http://uberchicgeekchick.com/?projects=AOPHP",		N_("Expressive Programming - My OSS projects: AOPHP")},
	{6,	"http://uberchicgeekchick.com/?blogs=0001",		N_("Expressive Programming - My writings, ramblings, &amp; rantings: About my life...")},
	{7,	"http://uberchicgeekchick.com/?specials=0001",		N_("Expressive Programming - Events, causes, issues, &amp; changing our world: Podcast-a-thon!")},
	{8,	"http://uberchicks.net/",				N_("uberChicks!")},
	{9,	"http://dystonia-dreams.org/",				N_("the story so far...my life's graphic novel memior...provide *AS IS*: Dystonia-DREAMS!")},
	{0,	NULL,							N_("Unknown timeline")}
};

struct _MyGObjectPrivate {
	guint			timeout_id;
	
	gint			index;
	guint			total;
	
	gint			page;
	gchar			*uri;
	gchar			*title
	
	GtkWidget		*widget;
	
	GtkEntry		*uri_entry;
	GtkLabel		*title_label;
	
	GtkListStore		*list_store;
	GtkTreeModel		*tree_model;
	GtkTreeModel		*tree_model_sort;
	
	GtkTreeView		*tree_view;
	
	GtkTreeViewColumn	*pixbuf_tree_view_column;
	GtkCellRendererPixbuf	*pixbuf_cell_renderer_pixbuf;
	
	GtkTreeViewColumn	*string_tree_view_column;
	GtkCellRendererText	*string_cell_renderer_text;
};

enum _MyGObject_ListStore_Rows{
	ROW_ULONG_ID,
	ROW_ULONG_AGE,
	ROW_STRING_TITLE,
	ROW_PIXBUF,
	ROW_POINTER,
};



/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define DEBUG_DOMAINS "{Stuff}:GtkBuilder:GtkBuildable:Settings:Setup:Start-Up:MyGObject.c"
#include "debug.h"

#define GtkBuilderUI "my-gobject.ui"


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static void my_gobject_class_init(MyGObjectClass *klass);
static void my_gobject_init(MyGObject *my_gobject);
static void my_gobject_finalize(MyGObject *my_gobject);

static gchar *my_gobject_gtkbuilder_ui_get_path(const gchar *filename);
static GtkBuilder *my_gobject_gtkbuilder_ui_load_file( const gchar *filename, const gchar *first_widget, va_list widgets);
static GtkBuilder *my_gobject_gtk_builder_ui_load(const gchar *filename, const gchar *first_widget, ...);

static void my_gobject_setup(MyGObject *my_gobject);

static void my_gobject_set_uri_title(MyGObject *my_gobject, const gchar *uri);

static gboolean my_gobject_update(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, MyGObject *my_gobject);

static void my_gobject_key_pressed(GtkWidget *widget, GdkEventKey *event, MyGObject *my_gobject);
static void my_gobject_move(GtkWidget *widget, GdkEventKey *event, MyGObject *my_gobject);

static void my_gobject_pixbuf_get(MyGObject *my_gobject, const gchar *image_filename);
static void my_gobject_pixbuf_scale(MyGObject *my_gobject, GdkPixbuf *pixbuf);

static void my_gobject_changed_cb(GtkTreeView *my_gobject_tree_view, MyGObject *my_gobject);

static void my_gobject_grab_focus_cb(GtkWidget *widget, MyGObject *my_gobject);
static void my_gobject_size_cb(GtkWidget *widget, GtkAllocation *allocation, MyGObject *my_gobject);

/*G_DEFINE_TYPE(MyGObject, my_gobject, G_TYPE_OBJECT);*/
G_DEFINE_TYPE(MyGObject, my_gobject, GTK_WIDGET);


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
/* BEGIN: GObject core methods. */
static void my_gobject_class_init(MyGObjectClass *klass){
	GObjectClass	*object_class=G_OBJECT_CLASS(klass);
	
	object_class->finalize=(GObjectFinalizeFunc)my_gobject_finalize;
	
	g_type_class_add_private(object_class, sizeof(MyGObjectPrivate));
}/* my_gobject_class_init */

static void my_gobject_init(MyGObject *my_gobject){
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	my_gobject_setup(my_gobject);
	this->index=this->total=0;
	g_object_set(my_gobject, "expand", TRUE, "fill", TRUE, NULL);
	g_signal_connect(my_gobject, "size-allocate", G_CALLBACK(my_gobject_size_cb), my_gobject);
	g_signal_connect(my_gobject, "grab-focus", G_CALLBACK(my_gobject_grab_focus_cb), my_gobject);
}/* my_gobject_init */

MyGObject *my_gobject_new(void){
	MyGObject *my_gobject=g_object_new(TYPE_MY_GOBJECT, NULL);
	my_gobject_set_uri_title(my_gobject, uri);
	gtk_widget_show_all(GTK_WIDGET(this->widget));
	gtk_widget_show_all(GTK_WIDGET(my_gobject));
	my_gobject_start(my_gobject);
	return my_gobject;
}/*my_gobject_new(timeline);*/

static void my_gobject_finalize(MyGObject *my_gobject){
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	program_timeout_remove(&this->timeout_id, g_strrstr(this->string, "/"));
	
	if(this->uri) uber_free(this->uri);
	if(this->title) uber_free(this->title);

	if(this->widget) gtk_widget_destroy(GTK_WIDGET(this->widget));
	if(this->tree_model_sort) g_object_unref(this->tree_model_sort);
	
	G_OBJECT_CLASS(my_gobject_parent_class)->finalize(G_OBJECT(my_object));
}/* my_gobject_finalized */


/*BEGIN: Custom MyGObject methods.*/
GtkWidget *my_gobject_get_widget(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) ))	return NULL;
	return GTK_WIDGET( GET_PRIVATE(my_gobject)->widget );
}/*my_gobject_get_child(my_gobject);*/

const gchar *my_gobject_get_uri(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) ))	return NULL;
	return GET_PRIVATE(my_gobject)->uri;
}/*my_gobject_get_uri(my_gobject);*/

const gchar *my_gobject_get_title(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) ))	return NULL;
	return GET_PRIVATE(my_gobject)->title;
}/*my_gobject_get_uri(my_gobject);*/

gint my_gobject_get_page(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) ))	return -1;
	return GET_PRIVATE(my_gobject)->page;
}/*my_gobject_get_page(my_gobject);*/

void my_gobject_set_page(MyGObject *my_gobject, gint page){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) ))	return;
	GET_PRIVATE(my_gobject)->page=page;
}/*my_gobject_set_page(my_gobject, 0);*/

void my_gobject_start(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) ))	return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	debug("Creating timeout to reload %s.", this->timeline_menu_label);
	guint reload=60000;
	this->timeout_id=g_timeout_add(reload, (GSourceFunc)my_gobject_refresh, my_gobject);
}/*my_gobject_start(MyGObject *my_gobject);*/

void my_gobject_stop(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) ))	return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	program_timeout_remove(&this->timeout_id, g_strrstr(this->timeline, "/"));
}/*my_gobject_stop(my_gobject);*/

gboolean my_gobject_refresh(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) ))	return FALSE;
	my_gobject_stop(my_gobject);
	my_gobject_start(my_gobject);
	return FALSE;
}/*my_gobject_refresh(my_gobject);*/

static void my_gobject_sort(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) ))	return;
	gtk_tree_model_foreach(GTK_TREE_MODEL(( GET_PRIVATE(my_gobject)->tree_model )), (GtkTreeModelForeachFunc)my_gobject_update, my_gobject);
}/*my_gobject_sort(my_gobject);*/

static gboolean my_gobject_update(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) ))	return FALSE;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	gulong 		created_seconds_ago=0;
	gchar		*created_at_str=NULL, *created_how_long_ago=NULL;
	
	gtk_tree_model_get(
				this->tree_model, iter,
					STRING_CREATED_AT, &created_at_str,
				-1
	);
	if(!gtk_list_store_iter_is_valid(this->list_store, iter)){
		if(created_at_str) uber_free(created_at_str);
		return FALSE;
	}
	created_how_long_ago=parser_convert_time(created_at_str, &created_seconds_ago);
	gtk_list_store_set(
				this->list_store, iter,
					STRING_CREATED_AGO, created_how_long_ago,	/*(seconds|minutes|hours|day) ago.*/
					ULONG_CREATED_AGO, created_seconds_ago,		/*How old the post is, in seconds, for sorting.*/
				-1
	);
	uber_free(created_at_str);
	uber_free(created_how_long_ago);
	return TRUE;
}/*static gboolean my_gobject_update(model, path, iter, my_gobject);*/

static void my_gobject_set_uri_title(MyGObject *my_gobject, const gchar *uri){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) ))	return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	MyGObjectTitles *titles=MyGObjectTitlesList;
	while(labels->page){
		if(g_str_has_prefix(uri, labels->uri )){
			this->monitoring=labels->monitoring;
			this->uri=g_strdup(uri);
			this->string=g_strdup(labels->string);
		}
		timeline_labels++;
	}
}/*my_gobject_set_uri_title(my_gobject, uri);*/

static gchar *my_gobject_gtkbuilder_ui_get_path(const gchar *filename){
#ifndef GNOME_ENABLE_DEBUG
	gchar *gtkbuilder_ui_file=NULL;
	if( (g_file_test( (gtkbuilder_ui_file=g_build_filename( DATADIR, PACKAGE_TARNAME, filename, NULL )), G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR )) )
		return gtkbuilder_ui_file;
	
	g_free( gtkbuilder_ui_file );
#endif
	
	return g_build_filename( BUILDDIR, "data", filename, NULL );
}/*my_gobject_gtkbuilder_ui_get_path(filename);*/

static GtkBuilder *my_gobject_gtkbuilder_ui_load_file( const gchar *filename, const gchar *first_widget, va_list widgets){
	/* Create gtkbuilder & load the xml file */
	GtkBuilder *my_gobject_ui=gtk_builder_new();
	gtk_builder_set_translation_domain(my_gobject_ui, GETTEXT_PACKAGE);
	gchar *path=my_gobject_gtkbuilder_ui_get_path(filename);
	GError *error=NULL;
	if(!gtk_builder_add_from_file(my_gobject_ui, path, &error)) {
		debug("**ERROR:** XML file error: %s", error->message);
		g_error_free(error);
		g_free(path);
		return NULL;
	}
	g_free(path);
	
	GObject    **pointer;
	const char  *name;
	for(name=first_widget; name; name=va_arg (args, char *)){
		pointer=va_arg(args, void *);
		if(!( *pointer=gtk_builder_get_object(ui, name) ))
			g_warning ("Widget '%s' at '%s' is missing.", name, filename);
	}

	return my_gobject_ui;
}/*my_gobject_gtkbuilder_ui_load_file(filename, first_widget, args);*/

static GtkBuilder *my_gobject_gtk_builder_ui_load(const gchar *filename, const gchar *first_widget, ...){
	va_list args;
	va_start(args, first_widget);
	GtkBuilder *my_gobject_ui=my_gobject_gtkbuilder_ui_load_file(filename, first_widget, args);
	va_end(args);
}/*my_gobject_gtkbuilder_ui_load(GtkBuilderUI, "my_gobject_widget", &this->widget, NULL*/

static void my_gobject_setup(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) ))	return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	GtkBuilder *my_gobject_ui=my_gobject_gtkbuilder_ui_load(
							GtkBuilderUI,
								"my_gobject_widget", &this->widget,
								
								"my_gobject_uri_entry", &this->uri_entry,
								"my_gobject_title_label", &this->title_label,
								
								"my_gobject_tree_view", &this->tree_view,
								
								"my_gobject_list_store", &this->list_store,
								
								"my_gobject_pixbuf_tree_view_column", &this->pixbuf_tree_view_column,
								"my_gobject_pixbuf_cell_renderer_pixbuf", &this->pixbuf_cell_renderer_pixbuf,
								
								"my_gobject_string_tree_view_column", &this->string_tree_view_column,
								"my_gobject_string_cell_renderer_text", &this->string_cell_renderer_text,
							NULL
	);
	
	gtkbuilder_connect(gtk_builder_ui, my_gobject,
								"my_gobject_tree_view", "grab-focus", my_gobject_grab_focus_cb,
								"my_gobject_tree_view", "cursor-changed", my_gobject_changed_cb,
								"my_gobject_tree_view", "size-allocate", my_gobject_size_cb,
								"my_gobject_tree_view", "key-press-event", my_gobject_key_pressed,
							NULL
	);
	
	this->tree_model=GTK_TREE_MODEL(this->list_store);
	this->tree_model_sort=gtk_tree_model_sort_new_with_model(this->tree_model);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(this->tree_model_sort), ROW_ULONG_AGE, GTK_SORT_ASCENDING);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(my_gobject), this->tree_model_sort);
	gtk_tree_view_set_model(this->tree_view, this->tree_model_sort);
}/*my_gobject_setup(my_gobject);*/

static void my_gobject_key_pressed(GtkWidget *widget, GdkEventKey *event, MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) )) return;
	if(event->keyval!=GDK_Return && event->keyval!=GDK_KP_Enter) return my_gobject_move(widget, event, my_gobject);
	
	switch(event->state){
		case GDK_CONTROL_MASK:
			return;
		case GDK_MOD1_MASK:
			return;
		case GDK_SHIFT_MASK:
			return;
		default:
			return;
	}//switch
}/*my_gobject_key_pressed(widget, event);*/

static void my_gobject_move(GtkWidget *widget, GdkEventKey *event, MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) )) return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	switch(event->keyval){
		case GDK_Tab: case GDK_KP_Tab:
		case GDK_Home: case GDK_KP_Home:
		case GDK_Begin: case GDK_Escape:
			this->index=0;
			break;
		case GDK_Up: case GDK_KP_Up: case GDK_KP_Prior:
			this->index--;
			break;
		case GDK_Down: case GDK_KP_Down: case GDK_KP_Next:
			this->index++;
			break;
		case GDK_End: case GDK_KP_End:
			this->index=this->total-1;
			break;
		case GDK_Page_Up:
			this->index-=9; break;
		case GDK_Page_Down:
			this->index+=9; break;
		default:
			return;
	}//switch
	
	my_gobject_goto_index(my_gobject);
}/* my_gobject_move */

static void my_gobject_goto_index(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) )) return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	if(this->index<0) {
		gtk_widget_error_bell(GTK_WIDGET(my_gobject));
		this->index=0;
	}else if(this->index>=this->total){
		gtk_widget_error_bell(GTK_WIDGET(my_gobject));
		this->index=this->total-1;
	}
	
	debug("Selecting #%d, out of : %d.", this->index, this->total);
	GtkTreePath *path=gtk_tree_path_new_from_indices(this->index, -1);
	gtk_tree_view_set_cursor(this->tree_view, path, NULL, FALSE);
	gtk_tree_path_free(path);
}/*my_gobject_goto_index();*/

static void my_gobject_scroll_to_top(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) )) return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
		
	if(!(GTK_TREE_VIEW(this->tree_view))){
		debug("**ERROR:** MyGObject cannot be cast to GtkTreeView.  Unable to move to top.");
		return;
	}
	GtkTreePath *path=gtk_tree_path_new_from_indices(0, -1);
	if(GTK_IS_TREE_VIEW(GTK_TREE_VIEW(this->tree_view)))
		gtk_tree_view_scroll_to_cell(this->tree_view, path, NULL, FALSE, 0.0, 0.0);
	gtk_tree_path_free(path);
}/* my_gobject_scroll_to_top */

static void my_gobject_clear(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) )) return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	debug("Re-setting my_gobject_index.");
	gtk_list_store_clear(this->list_store);
	this->index=0;
	this->total=0;
}/*my_gobject_clear(my_gobject);*/

static void my_gobject_grab_focus_cb(GtkWidget *widget, MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) )) return;
	my_gobject_mark_as_read(my_gobject);
}/*my_gobject_grab_focus_cb(widget, event, my_gobject);*/

void my_gobject_mark_as_read(MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) )) return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	gtk_entry_set_text(this->uri_entry, this->uri);
	gtk_label_set_markup_with_mnemonic(this->title_label, this->title);
}/*my_gobject_mark_as_read(my_gobject);*/

void my_gobject_append(MyGObject *my_gobject, gulong id, gulong age, gchar *title, const gchar *image_filename, gpointer *pointer){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) )) return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	
	GdkPixbuf *pixbuf=NULL;
	if(g_file_test(image_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR))
		pixbuf=my_gobject_pixbuf_get(my_gobject, image_filename);
	
	gtk_list_store_append( this->ist_store, iter);
	gtk_list_store_set(
				this->list_store, iter,
					ROW_ULONG_ID, id,
					ROW_ULONG_AGE, age,
					ROW_STRING_TITLE, title,
					ROW_PIXBUF, pixbuf,
					ROW_POINTER, pointer,
				-1
	);
	g_object_unref(pixbuf);
	this->total++;
}/*void my_gobject_append(my_gobject, gulong id, gulong age, title, pixbuf, pointer);*/

static void my_gobject_pixbuf_get(MyGObject *my_gobject, const gchar *image_filename){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) )) return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	GError *error=NULL;
	GdkPixbuf *pixbuf=NULL;
	if( (pixbuf=gdk_pixbuf_new_from_file(image_filename, &error )) )
		return my_gobject_pixbuf_scale(my_gobject, pixbuf);
	
	debug("Image error: %s: %s", image_filename, error->message);
	if(error) g_error_free(error);
	return NULL;
}/*my_gobject_get_pixbuf(image_filename);*/

static void my_gobject_pixbuf_scale(MyGObject *my_gobject, GdkPixbuf *pixbuf){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) )) return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	gint size=0;
	size=gtk_tree_view_column_get_fixed_width(this->pixbuf_tree_view_column);
	if( size < 0 )
		gtk_tree_view_column_get_max_width(this->pixbuf_tree_view_column);
	if( size < 0 )
		size=gtk_tree_view_column_get_min_width(this->pixbuf_tree_view_column);
	if( size < 0 )
		size=gtk_tree_view_column_get_width(this->pixbuf_tree_view_column);
	
	GdkPixbuf *resized=gdk_pixbuf_scale_simple(pixbuf, size, size, GDK_INTERP_BILINEAR);
	g_object_unref(pixbuf);
	return resized;
}/*my_gobject_pixbuf_scale(my_gobject, pixbuf);*/

static void my_gobject_changed_cb(GtkTreeView *my_gobject_tree_view, MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) )) return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	GtkTreeSelection	*sel;
	GtkTreeIter		*iter=g_new0(GtkTreeIter, 1);
	if(!((sel=gtk_tree_view_get_selection(this->tree_view)) && gtk_tree_selection_get_selected(sel, &this->tree_model_sort, iter) )){
		g_free(iter);
		return;
	}
	
	
	gulong		id, age;
	gpointer	*pointer=NULL;
	GdkPixbuf	*pixbuf;
	gchar		*title;

	gtk_tree_model_get(
				GTK_TREE_MODEL(this->tree_model_sort), iter,
					ROW_ULONG_ID, &id,
					ROW_ULONG_AGE, &age,
					ROW_STRING_TITLE, &title,
					ROW_PIXBUF, &pixbuf,
					ROW_POINTER, &pointer,
				-1
	);
	
	debug("Starting to do something with: #%lu from '%s'.", id, this->title);
	
	
	
	
	debug("Finished doing something with: #%lu from '%s'.", id, this->title);
	pointer=NULL;
	if(title) g_free(title);
	if(pixbuf) g_object_unref(pixbuf);
	g_free(iter);
}/*my_gobject_changed_cb(my_gobject_tree_view, my_gobject);*/

static void my_gobject_size_cb(GtkWidget *widget, GtkAllocation *allocation, MyGObject *my_gobject){
	if(!( my_gobject && IS_MY_GOBJECT(my_gobject) )) return;
	MyGObjectPrivate *this=GET_PRIVATE(my_gobject);
	
	g_object_set(this->string_cell_renderer_text, "wrap-width", ((gtk_tree_view_column_get_width(this->string_tree_view_column))-10), NULL);
}/*my_gobject_size_cb(widget, allocation, my_gobject);*/


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

