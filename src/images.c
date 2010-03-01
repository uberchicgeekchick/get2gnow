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

#define _GNU_SOURCE
#define _THREAD_SAFE

#include <sys/stat.h>

#include "config.h"
#include "program.h"

#include "www.h"
#include "cache.h"
#include "images.h"


#define DEBUG_DOMAINS "Images:UI:Requests:Files:I/O:Setup:Start-Up:Cache:UpdateViewer:Graphics:images.c"
#include "debug.h"

static gchar *unknown_image_file=NULL;


static void images_validate_width(gint *width);
static void images_validate_height(gint *height);
static void images_validate_filename(gchar **image_filename);
static GdkPixbuf *images_get_scaled_pixbuf_from_filename(gchar *image_filename, gint width, gint height);


gchar *images_get_unknown_image_file(void){
	if(!unknown_image_file)
		images_set_unknown_image_file();
	return g_strdup(unknown_image_file);
}/*images_get_unknown_image_file();*/

void images_set_unknown_image_file(void){
	GtkImage *stock_unknown_image=NULL;
	gchar *unknown_image_filename=NULL;
	if((stock_unknown_image=GTK_IMAGE(gtk_image_new_from_icon_name("gtk-missing-image", ImagesDialog)))){
		stock_unknown_image=g_object_ref_sink(stock_unknown_image);
		g_object_get(stock_unknown_image, "file", &unknown_image_filename, NULL );
		g_object_unref(stock_unknown_image);
		if(!G_STR_EMPTY(unknown_image_filename)){
			debug("**notice:** images_unknown_image_file has been set to <%s>.", unknown_image_filename);
			unknown_image_file=unknown_image_filename;
			return;
		}
		
		if(unknown_image_filename)
			uber_free(unknown_image_filename);
	}
	
	unknown_image_filename=g_build_filename(DATADIR, "icons", "gnome", "scalable", "status", "gtk-missing-image.svg", NULL);
	if(!(g_file_test(unknown_image_filename, G_FILE_TEST_EXISTS|G_FILE_TEST_IS_REGULAR ))){
		uber_free(unknown_image_filename);
		unknown_image_filename=g_strdup("");
		debug("**error:** images_unknown_image_file could not be set.");
	}else
		debug("**notice:** images_unknown_image_file has been set to <%s>.", unknown_image_file);
	
	unknown_image_file=unknown_image_filename;
}/*images_get_unknown_image_file();*/

void images_unset_unknown_image_file(void){
	if(!unknown_image_file) return;
	
	debug("**SHUTDOWN:** releasing memory of unknown image: %s.", unknown_image_file);
	uber_free(unknown_image_file);
}/*images_unset_unknown_image_file();*/

gboolean images_save_image(OnlineService *service, SoupMessage *xml, const gchar *image_uri, const gchar *image_file, gchar **image_filename){
	gchar *error_message=NULL;
	if(!(www_xml_error_check(service, image_uri, xml, &error_message))){
		debug("Failed to download and save image: <%s> to file: [%s].", image_uri, image_file);
		debug("Detailed error message: %s.", error_message);
		uber_free(error_message);
		(*image_filename)=images_get_unknown_image_file();
		return FALSE;
	}
	uber_free(error_message);
	
	if(!( g_strrstr( soup_message_headers_get_one( xml->response_headers, "Content-Type"), "image" ) )){
		debug("Downloaded content does not appear to be an image: <%s>.  GNOME's unknown-image will be used instead.", image_uri);
		(*image_filename)=images_get_unknown_image_file();
		return FALSE;
	}
	
	if(!(g_file_set_contents(
				image_file,
					xml->response_body->data,
					xml->response_body->length,
				NULL
	))){
		debug("Failed to download and save image: <%s> to file: [%s].", image_uri, image_file);
		(*image_filename)=images_get_unknown_image_file();
		return FALSE;
	}
	
	debug("Saved image: <%s> to file: [%s]", image_uri, image_file);
	(*image_filename)=g_strdup(image_file);
	return TRUE;
}/*images_save_image(service, xml, image_uri, image_file);*/

static void images_validate_filename(gchar **image_filename){
	if(!G_STR_EMPTY(*image_filename))
		return;
	
	if(*image_filename) g_free(*image_filename);
	*image_filename=images_get_unknown_image_file();
}/*images_validate_filename*/

GtkImage *images_get_expanded_image_from_filename( gchar *image_filename ){
	return images_get_scaled_image_from_filename( image_filename, ImagesExpanded, ImagesExpanded );
}//images_get_expanded_image_from_filename

GtkImage *images_get_maximized_image_from_filename( gchar *image_filename ){
	return images_get_scaled_image_from_filename( image_filename, ImagesMaximum, ImagesMaximum );
}//images_get_maximize_image_from_filename

GtkImage *images_get_default_image_from_filename( gchar *image_filename ){
	return images_get_scaled_image_from_filename( image_filename, ImagesDefault, ImagesDefault );
}//images_get_default_image_from_filename

GtkImage *images_get_dialog_image_from_filename( gchar *image_filename ){
	return images_get_scaled_image_from_filename( image_filename, ImagesDialog, ImagesDialog );
}//images_get_dialog_image_from_filename

GtkImage *images_get_expanded_image_from_stock( const gchar *stock_image_name ){
	return (GtkImage *)gtk_image_new_from_stock( stock_image_name, ImagesExpanded );
}//images_get_expanded_image_from_stock

GtkImage *images_get_maximized_image_from_stock( const gchar *stock_image_name ){
	return GTK_IMAGE( gtk_image_new_from_stock( stock_image_name, ImagesMaximum ) );
}/*images_get_maximized_image_from_stock("gtk-dialog-info");*/

GtkImage *images_get_default_image_from_stock( const gchar *stock_image_name ){
	return (GtkImage *)gtk_image_new_from_stock( stock_image_name, ImagesDefault );
}//images_get_default_image_from_stock

GtkImage *images_get_dialog_image_from_stock( const gchar *stock_image_name ){
	return (GtkImage *)gtk_image_new_from_stock( stock_image_name, ImagesDialog );
}/*images_get_dialog_image_from_stock("gtk-dialog-info");*/

GtkImage *images_get_minimized_image_from_stock( const gchar *stock_image_name ){
	return (GtkImage *)gtk_image_new_from_stock( stock_image_name, ImagesMinimum );
}//images_get_minimize_image_from_stock

GtkImage *images_get_image_from_filename( gchar *image_filename ){
	images_validate_filename(&image_filename);
	
	GdkPixbuf *pixbuf=images_get_pixbuf_from_filename(image_filename);
	GtkImage *image=GTK_IMAGE( gtk_image_new_from_pixbuf(pixbuf) );
	g_object_unref(pixbuf);
	return image;
}//images_get_image_from_file



GtkImage *images_get_scaled_image_from_filename( gchar *image_filename, gint width, gint height ){
	images_validate_filename(&image_filename);
	
	images_validate_width(&width);
	images_validate_height(&height);
	GdkPixbuf *pixbuf=images_get_scaled_pixbuf_from_filename( image_filename, width, height );
	GtkImage *image=GTK_IMAGE( gtk_image_new_from_pixbuf(pixbuf) );
	g_object_unref(pixbuf);
	return image;
}//images_get_image_from_file


GdkPixbuf *images_get_maximized_pixbuf_from_filename( gchar *image_filename ){
	return images_get_scaled_pixbuf_from_filename( image_filename, ImagesMaximum, ImagesMaximum );
}//images_get_maximize_pixbuf_from_filename

GdkPixbuf *images_get_expanded_pixbuf_from_filename( gchar *image_filename ){
	return images_get_scaled_pixbuf_from_filename( image_filename, ImagesExpanded, ImagesExpanded );
}//images_get_expanded_pixbuf_from_filename

GdkPixbuf *images_get_default_pixbuf_from_filename( gchar *image_filename ){
	return images_get_scaled_pixbuf_from_filename( image_filename, ImagesDefault, ImagesDefault );
}//images_get_default_pixbuf_from_filename

GdkPixbuf *images_get_dialog_pixbuf_from_filename( gchar *image_filename ){
	return images_get_scaled_pixbuf_from_filename( image_filename, ImagesDialog, ImagesDialog );
}//images_get_dialog_pixbuf_from_filename

GdkPixbuf *images_get_minimized_pixbuf_from_filename( gchar *image_filename ){
	return images_get_scaled_pixbuf_from_filename( image_filename, ImagesMinimum, ImagesMinimum );
}//images_get_minimize_pixbuf_from_filename



GdkPixbuf *images_maximize_pixbuf( GdkPixbuf *pixbuf ){
	return images_scale_pixbuf( pixbuf, ImagesMaximum, ImagesMaximum );
}//images_maximize_pixbuf

GdkPixbuf *images_expand_pixbuf( GdkPixbuf *pixbuf ){
	return images_scale_pixbuf( pixbuf, ImagesExpanded, ImagesExpanded );
}//images_get_expand_pixbuf

GdkPixbuf *images_normalize_pixbuf( GdkPixbuf *pixbuf ){
	return images_scale_pixbuf( pixbuf, ImagesDefault, ImagesDefault );
}//images_normalize_pixbuf

GdkPixbuf *images_dialogize_pixbuf( GdkPixbuf *pixbuf ){
	return images_scale_pixbuf( pixbuf, ImagesDialog, ImagesDialog );
}//images_normalize_pixbuf

GdkPixbuf *images_minimize_pixbuf( GdkPixbuf *pixbuf ){
	return images_scale_pixbuf( pixbuf, ImagesMinimum, ImagesMinimum );
}//images_minimize_pixbuf



GdkPixbuf *images_scale_pixbuf( GdkPixbuf *pixbuf, gint width, gint height ){
	images_validate_width(&width);
	images_validate_height(&height);
	
	GdkPixbuf *resized=NULL;
	if( (resized=gdk_pixbuf_scale_simple( pixbuf, width, height, GDK_INTERP_BILINEAR )) )
		return resized;
	
	debug("Image error: risizing of pixmap to: %d x %d failed.", width, height );
	return NULL;
}//images_resize_pixbuf



GdkPixbuf *images_get_pixbuf_from_filename( gchar *image_filename ){
	return images_get_scaled_pixbuf_from_filename( image_filename, ImagesDefault, ImagesDefault );
}//images_get_from_filename



static void images_validate_width(gint *width){
	if(*width<ImagesMinimum)
		*width=ImagesMinimum;
	
	if(*width>ImagesMaximum)
		*width=ImagesMaximum;
}//images_validate_width



static void images_validate_height(gint *height){
	if(*height<ImagesMinimum)
		*height=ImagesMinimum;
		
	if(*height>ImagesMaximum)
		*height=ImagesMaximum;
}//images_validate_height



GdkPixbuf *images_get_unscaled_pixbuf_from_filename( gchar *image_filename ){
	images_validate_filename(&image_filename);
	
	GError *error=NULL;
	GdkPixbuf *pixbuf=NULL;
	if( (pixbuf=gdk_pixbuf_new_from_file(image_filename, &error )) )
		return pixbuf;
	
	debug("Image error: %s: %s", image_filename, error->message);
	if(error) g_error_free(error);
	return NULL;
}//images_get_full_sized_pixbuf_from_file


/* GNOME 2.6 - future prep. but GNOME 2.6 isn't in enough distros yet */

static GdkPixbuf *images_get_scaled_pixbuf_from_filename( gchar *image_filename, gint width, gint height ){
	images_validate_filename(&image_filename);
	
	if( width == ImagesUnscaled || height == ImagesUnscaled )
		return images_get_unscaled_pixbuf_from_filename(image_filename);
	
	images_validate_width(&width);
	images_validate_height(&height);
	
	GError *error=NULL;
	GdkPixbuf *pixbuf;
	
	if( (pixbuf=gdk_pixbuf_new_from_file_at_scale(image_filename, width, height, TRUE, &error )) )
		return pixbuf;
	
	debug("Image error: %s (%d x %d): %s", image_filename, width, height, error->message);
	if(error) g_error_free(error);
	return NULL;
}/*images_get_scaled_pixbuf_from_file(image_filename, width, height);*/

gboolean images_set_file_chooser_preview_image(GtkImage *preview_image, GtkFileChooser *file_chooser){
	gchar *filename=gtk_file_chooser_get_preview_filename(file_chooser);
	if(G_STR_EMPTY(filename)){
		if(filename)
			uber_free(filename);
		filename=gtk_file_chooser_get_filename(file_chooser);
		
		if(G_STR_EMPTY(filename)){
			if(filename)
				uber_free(filename);
			return FALSE;
		}
	}
	
	GdkPixbuf *pixbuf=gdk_pixbuf_new_from_file_at_size(filename, 128, 128, NULL);
	uber_free(filename);
	if(!pixbuf)
		return FALSE;
	
	gtk_image_set_from_pixbuf(preview_image, pixbuf);
	g_object_unref(pixbuf);
	gtk_file_chooser_set_preview_widget_active(file_chooser, TRUE);
	
	return TRUE;
}/*images_set_file_chooser_preview_image(preview_image, file_chooser);*/
