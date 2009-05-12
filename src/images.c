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


#include "config.h"
#include "images.h"
#include "main.h"
#include "debug.h"

#define DEBUG_DOMAIN	"Images"

static gint images_validate_width( gint width );
static gint images_validate_height( gint height );


gchar *images_get_filename(const gchar *image_url){
	debug(DEBUG_DOMAIN, "Creating image file name from image url: %s.", image_url);
	gchar *image_file, **image_name_info, *image_filename;
	static gchar *stock_unknown_image_filename=NULL;
	
	/**
	 * image_name_info[] index explanation:
	 * 	0 == the url scheme, e.g. https, http, etc.
	 * 	1 == this is empty as a result of the '://' in url scheme.
	 * 	2 == the domain name
	 * 	n || G_N_ELEMENTS == the file part of the url, i.e. the actual image's file name.
	 */
	image_name_info=g_strsplit(image_url, (const gchar *)"/", -1);
	guint n=g_strv_length(image_name_info)-1;
	debug(DEBUG_DOMAIN, "Found %d elements in the image's url.", n+1);
	
	if(image_name_info[2]==image_name_info[n-1])
		image_file=g_strconcat(image_name_info[2], "_", image_name_info[n], NULL);
	else
		image_file=g_strconcat(image_name_info[2], "_", image_name_info[n-1], "_", image_name_info[n], NULL);
	
	if(image_name_info)
		g_strfreev(image_name_info);
	
	if(!image_file){
		debug(DEBUG_DOMAIN, "**ERROR** Unable to parse url into valid image filename.");
		if(!stock_unknown_image_filename){
			GtkImage *stock_unknown_image=(GtkImage *)gtk_image_new_from_icon_name("gtk-missing-image", ImagesDefault);
			g_object_get(
					stock_unknown_image,
						"file", &stock_unknown_image_filename,
					NULL
			);
			g_object_unref(stock_unknown_image);
		}
		
		debug(DEBUG_DOMAIN, "\t\tUsing stock image: %s.", stock_unknown_image_filename);
		return g_strdup(stock_unknown_image_filename);
	}
	
	
	image_filename=g_build_filename( g_get_home_dir(), ".gnome2", CACHE_IMAGES, image_file, NULL );
	
	debug(DEBUG_DOMAIN, "Saving image:\n\t\turl: %s\n\t\tfile:%s\n\t\tfull path: %s", image_url, image_file, image_filename);
	
	g_free(image_file);
	
	return image_filename;
}//images_get_filename



GtkImage *images_get_expanded_image_from_filename( const gchar *image_filename ){
	return images_get_scaled_image_from_filename( image_filename, ImagesExpanded, ImagesExpanded );
}//images_get_expanded_image_from_filename

GtkImage *images_get_maximized_image_from_filename( const gchar *image_filename ){
	return images_get_scaled_image_from_filename( image_filename, ImagesMaximum, ImagesMaximum );
}//images_get_maximize_image_from_filename

GtkImage *images_get_default_image_from_filename( const gchar *image_filename ){
	return images_get_scaled_image_from_filename( image_filename, ImagesDefault, ImagesDefault );
}//images_get_default_image_from_filename

GtkImage *images_get_minimized_image_from_filename( const gchar *image_filename ){
	return images_get_scaled_image_from_filename( image_filename, ImagesMinimum, ImagesMinimum );
}//images_get_minimize_image_from_filename



GtkImage *images_get_image_from_filename( const gchar *image_filename ){
	GdkPixbuf *pixbuf=images_get_pixbuf_from_filename( image_filename );
	GtkImage *image=GTK_IMAGE( gtk_image_new_from_pixbuf( pixbuf ) );
	g_object_unref( pixbuf );
	return image;
}//images_get_image_from_file



GtkImage *images_get_scaled_image_from_filename( const gchar *image_filename, gint width, gint height ){
	width=images_validate_width( width );
	height=images_validate_height( height );
	GdkPixbuf *pixbuf=images_get_scaled_pixbuf_from_filename( image_filename, width, height );
	GtkImage *image=GTK_IMAGE( gtk_image_new_from_pixbuf( pixbuf ) );
	g_object_unref( pixbuf );
	return image;
}//images_get_image_from_file



GdkPixbuf *images_get_maximized_pixbuf_from_filename( const gchar *image_filename ){
	return images_get_scaled_pixbuf_from_filename( image_filename, ImagesMaximum, ImagesMaximum );
}//images_get_maximize_pixbuf_from_filename

GdkPixbuf *images_get_expanded_pixbuf_from_filename( const gchar *image_filename ){
	return images_get_scaled_pixbuf_from_filename( image_filename, ImagesExpanded, ImagesExpanded );
}//images_get_expanded_pixbuf_from_filename

GdkPixbuf *images_get_default_pixbuf_from_filename( const gchar *image_filename ){
	return images_get_scaled_pixbuf_from_filename( image_filename, ImagesDefault, ImagesDefault );
}//images_get_default_pixbuf_from_filename

GdkPixbuf *images_get_minimized_pixbuf_from_filename( const gchar *image_filename ){
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

GdkPixbuf *images_minimize_pixbuf( GdkPixbuf *pixbuf ){
	return images_scale_pixbuf( pixbuf, ImagesMinimum, ImagesMinimum );
}//images_minimize_pixbuf



GdkPixbuf *images_scale_pixbuf( GdkPixbuf *pixbuf, gint width, gint height ){
	width=images_validate_width( width );
	height=images_validate_height( height );
	
	GdkPixbuf *resized=NULL;
	if( (resized=gdk_pixbuf_scale_simple( pixbuf, width, height, GDK_INTERP_BILINEAR )) )
		return resized;
			
	debug(DEBUG_DOMAIN, "Image error: risizing of pixmap to: %d x %d failed.", width, height );
	return NULL;
}//images_resize_pixbuf



GdkPixbuf *images_get_pixbuf_from_filename( const gchar *image_filename ){
	return images_get_scaled_pixbuf_from_filename( image_filename, ImagesDefault, ImagesDefault );
}//images_get_from_filename



static gint images_validate_width( gint width ){
	if( width < ImagesMinimum )
		width=ImagesMinimum;
	
	if( width > ImagesMaximum )
		width=ImagesMaximum;
	
	return width;
}//images_validate_width



static gint images_validate_height( gint height ){
	if( height < ImagesMinimum )
		height=ImagesMinimum;
			
	if( height > ImagesMaximum )
		height=ImagesMaximum;
	
	return height;
}//images_validate_height



GdkPixbuf *images_get_unscaled_pixbuf_from_filename( const gchar *image_filename ){
	GError *error=NULL;
	GdkPixbuf *pixbuf=NULL;
	if( (pixbuf=gdk_pixbuf_new_from_file(image_filename, &error )) )
		return pixbuf;
	
	debug(DEBUG_DOMAIN, "Image error: %s: %s", image_filename, error->message);
	if(error) g_error_free(error);
	return NULL;
}//images_get_full_sized_pixbuf_from_file


/* GNOME 2.6
GdkPixbuf *images_get_scaled_pixbuf_from_filename( const gchar *image_filename, gint width, gint height ){
	if( width == ImagesUnscaled || height == ImagesUnscaled )
		return images_get_unscaled_pixbuf_from_filename( image_filename );
	
	width=images_validate_width( width );
	height=images_validate_height( height );
	
	GError *error=NULL;
	GdkPixbuf *pixbuf;
	
	if( (pixbuf=gdk_pixbuf_new_from_file_at_scale(image_filename, width, height, &error )) )
		return pixbuf;
	
	debug(DEBUG_DOMAIN, "Image error: %s (%d x %d): %s", image_filename, width, height, error->message);
	if(error) g_error_free(error);
	return NULL;
}images_get_scaled_pixbuf_from_file*/

/* I am using these last two methods, okay they're function - but they'll become methods soon enough.
 * Any ways I'm using them in place of the one above, which relies on GNOME 2.6, until GNOME 2.6
 * populates to more distros. */
GdkPixbuf *images_get_scaled_pixbuf_from_filename( const gchar *image_filename, gint width, gint height ){
	return images_get_and_scale_pixbuf_from_filename( image_filename, width, height );
}//images_get_scaled_pixbuf_from_file



GdkPixbuf *images_get_and_scale_pixbuf_from_filename( const gchar *image_filename, gint width, gint height ){
	width=images_validate_width( width );
	height=images_validate_height( height );
	
	GdkPixbuf *pixbuf, *resized;
	if(!(pixbuf=images_get_unscaled_pixbuf_from_filename(image_filename)))
		return NULL;
	
	resized=gdk_pixbuf_scale_simple( pixbuf, width, height, GDK_INTERP_BILINEAR );
	g_object_unref( pixbuf );
	return resized;
}//images_get_and_scale_pixbuf_from_file

