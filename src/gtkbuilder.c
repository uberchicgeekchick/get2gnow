/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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
 * License( "RPL" ) Version 1.5, or subsequent versions as allowed by the RPL,
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

#define _GNU_SOURCE
#define _THREAD_SAFE


#include "config.h"
#include "program.h"

#define	DEBUG_DOMAINS	"Debug:UI:GtkBuilder:GtkBuildable:Setup"
#include "debug.h"
#include "gtkbuilder.h"

static gchar *gtkbuilder_get_path( const gchar *filename );
static gchar *gtkbuilder_ui_test_filename(gchar *gtkbuilder_ui_filename);

static gchar *gtkbuilder_get_path(const gchar *base_filename){
	gchar *gtkbuilder_ui_file=NULL, *gtkbuilder_ui_filename=NULL;
	
#ifndef	GNOME_ENABLE_DEBUG
	gtkbuilder_ui_file=g_strdup_printf("%s.ui", base_filename);
	gtkbuilder_ui_filename=g_build_filename( DATADIR, PACKAGE_TARNAME, gtkbuilder_ui_file, NULL );
	if( gtkbuilder_ui_test_filename( gtkbuilder_ui_filename )){
		uber_free( gtkbuilder_ui_file );
		return gtkbuilder_ui_filename;
	}
#endif
	
	gtkbuilder_ui_file=g_strdup_printf("%s.ui", base_filename);
	gtkbuilder_ui_filename=g_build_filename( BUILDDIR, "data", gtkbuilder_ui_file, NULL );
	uber_free( gtkbuilder_ui_file );
	if( gtkbuilder_ui_test_filename( gtkbuilder_ui_filename ))
		return gtkbuilder_ui_filename;
	
	gtkbuilder_ui_filename=g_build_filename( BUILDDIR, "data", base_filename, NULL );
	uber_free( gtkbuilder_ui_file );
	
	if( gtkbuilder_ui_test_filename( gtkbuilder_ui_filename )){
		debug( "**ERROR:** GtkBuilderUI [%s] needs converted.", gtkbuilder_ui_filename );
		debug( "**ERROR:** GtkBuilderUI template file: [data/%s.in.ui] exists; needs converted to [data/%s.ui].", base_filename, base_filename );
		debug( "**ERROR:** GtkBuilderUI can be converted by running: (cd data/ ; make %s.ui).  Or re-run `make`.", base_filename );
	}
	
	debug("**ERROR:** Unable to load gtkbuilder ui: %s.ui", gtkbuilder_ui_filename);
	uber_free( gtkbuilder_ui_filename );
	
	return NULL;
}/*gtkbuilder_get_path( "tweet-view" );*/

static gchar *gtkbuilder_ui_test_filename(gchar *gtkbuilder_ui_filename){
	debug("Checking existance of for GtkBuilder UI filename: %s", gtkbuilder_ui_filename);
	if(!g_file_test( gtkbuilder_ui_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR )){
		debug("Unable to load GtkBuilder UI filename: %s", gtkbuilder_ui_filename);
		uber_free( gtkbuilder_ui_filename );
		
		/* uber_free releases gtkbuilder_ui_filename's memory & sets it to NULL.
		 * So it will end up with this function returning NULL as well.
		 */
	}
	
	debug("GtkBuilder UI found filename: %s.", gtkbuilder_ui_filename);
	return gtkbuilder_ui_filename;
}/*gtkbuilder_ui_test_file( gtkbuilder_ui_filename );*/



static GtkBuilder *gtkbuilder_load_file( const gchar *filename, const gchar *first_widget, va_list args ){
	GtkBuilder  *ui = NULL;
	GObject    **pointer;
	const char  *name;
	gchar       *path;
	GError      *error=NULL;

	/* Create gtkbuilder & load the xml file */
	ui=gtk_builder_new ();
	gtk_builder_set_translation_domain (ui, GETTEXT_PACKAGE);
	path=gtkbuilder_get_path( filename );
	if(!gtk_builder_add_from_file (ui, path, &error)){
		g_warning ("XML file error: %s", error->message);
		g_error_free( error );
		g_free( path );
		return NULL;
	}
	
	for(name=first_widget; name; name=va_arg (args, char *)){
		pointer=va_arg(args, void *);
		if(!( *pointer=gtk_builder_get_object(ui, name) ))
			g_warning ("Widget '%s' at '%s' is missing.", name, path);
	}
	
	uber_free( path );
	
	return ui;
}/*gtkbuilder_load_file(GtkBuilderUI, "widget", private_objects_ui->widget, NULL);*/

GtkBuilder *gtkbuilder_get_file (const gchar *filename, const gchar *first_widget, ...){
	va_list args;

	va_start(args, first_widget);
	GtkBuilder *ui=gtkbuilder_load_file(filename, first_widget, args);
	va_end( args );

	return ( ui ? ui : NULL );
}

void gtkbuilder_connect(GtkBuilder *ui, gpointer user_data, gchar *first_widget, ...){
	GObject     *instance;
	gpointer    *callback;
	const gchar *signal;
	const gchar *name;
	va_list      args;

	va_start (args, first_widget);
	
	for (name = first_widget; name; name = va_arg (args, char *)) {
		signal = va_arg (args, void *);
		callback = va_arg (args, void *);

		instance=gtk_builder_get_object(ui, name);
		if( !instance ) {
			g_warning ("Missing widget '%s'", name);
			continue;
		}

		g_signal_connect(instance, signal, G_CALLBACK( callback ), user_data);
	}

	va_end( args );
}

