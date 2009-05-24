/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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

/********************************************************
 *          My art, code, & programming.                *
 ********************************************************/


/********************************************************
 *        Project headers, eg #include "config.h"       *
 ********************************************************/
#include "config.h"
#include "main.h"
#include "cache.h"

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define DEBUG_DOMAINS "Cache:Requests:OnlineServices:Tweets:Users:Images:Files:I/O:Setup:Start-Up:Shutdown"
#include "debug.h"

static gchar *cache_prefix=NULL;
static gchar *unknown_image_filename=NULL;


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static void cache_dir_clean_up(const gchar *cache_subdir);
static void cache_dir_absolute_clean_up(const gchar *cache_dir);
static void cache_file_clean_up(const gchar *cache_file);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
void cache_init(void){
#ifndef GNOME_ENABLE_DEBUG
	cache_prefix=g_build_filename(g_get_home_dir(), ".gnome2", PACKAGE_TARNAME, NULL);
#else
	cache_prefix=g_build_filename(g_get_home_dir(), ".gnome2", PACKAGE_TARNAME, "debug", NULL);
#endif
	
	cache_images_get_unknown_image_filename();
}/*cache_init*/

void cache_deinit(void){
	debug("**SHUTDOWN:** releasing memory of cache prefix: %s.", cache_prefix);
	g_free(cache_prefix);
	
	debug("**SHUTDOWN:** releasing memory of unknown image: %s.", unknown_image_filename);
	g_free(unknown_image_filename);
	unknown_image_filename=NULL;
}/*cache_deinit*/

gchar *cache_dir_test(const gchar *cache_dir, gboolean mkdir){
	gchar *cache_path=g_build_filename(cache_prefix, cache_dir, NULL);
	if(!g_file_test(cache_path, G_FILE_TEST_EXISTS|G_FILE_TEST_IS_DIR)){
		debug("\t\t*NOTICE:* Creating avatars directory: %s", cache_path);
		if(!mkdir) return NULL;
		
		if(g_mkdir_with_parents(cache_path, S_IRUSR|S_IWUSR|S_IXUSR)){
			debug("***ERROR:*** Unable to create cache directory: [%s].", cache_path);
			g_free(cache_path);
			return NULL;
		}
	}
	return cache_path;
}/*cache_dir_test*/

static void cache_dir_absolute_clean_up(const gchar *cache_dir_path){
	static guint depth=0;
	if(!(g_str_has_prefix(cache_dir_path, cache_prefix)))
		return;
	
	if(!( (g_file_test(cache_dir_path, G_FILE_TEST_EXISTS|G_FILE_TEST_IS_DIR)) ))
		return;
	
	debug("Cleaning-up cache directory: [%s].", cache_dir_path);
	
	depth++;
	const gchar *cache_file=NULL;
	GDir *cache_dir=g_dir_open(cache_dir_path, 0, NULL);
	while( (cache_file=g_dir_read_name(cache_dir)) ){
		gchar *cache_file_path=g_strdup_printf("%s/%s", cache_dir_path, cache_file);
		if(g_file_test(cache_file, G_FILE_TEST_IS_REGULAR))
			cache_file_clean_up(cache_file_path);
		else if(g_file_test(cache_file_path, G_FILE_TEST_IS_DIR)){
			cache_dir_absolute_clean_up(cache_file_path);
			if(depth>1)
				cache_file_clean_up(cache_file_path);
		}
		g_free(cache_file_path);
	}
	g_dir_close(cache_dir);
	depth--;
}/*cache_dir_absolute_clean_up*/

gchar *cache_file_touch(const gchar *cache_file){
	gchar *cache_filename=NULL;
	
	if(!(g_str_has_prefix(cache_file, cache_prefix)))
		cache_filename=g_build_filename(cache_prefix, cache_file, NULL);
	else
		cache_filename=g_strdup(cache_file);
	
	if(!(g_file_test(cache_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)))
		if(!(g_creat(cache_filename, S_IRUSR|S_IWUSR))){
			debug("\t\t**ERROR:** Failed to create cache file.\n\t\t\tFilename: [%s].", cache_filename);
			g_free(cache_filename);
			return NULL;
		}
	
	return cache_filename;
}/*cache_file_touch*/

static void cache_file_clean_up(const gchar *cache_file){
	if(!(g_str_has_prefix(cache_file, cache_prefix)))
		return;
	
	GFile *cache_gfile=g_file_new_for_path(cache_file);
	GFileInfo *cache_gfileinfo=g_file_query_info(cache_gfile, G_FILE_ATTRIBUTE_ACCESS_CAN_DELETE, G_FILE_QUERY_INFO_NONE, NULL, NULL);
	if(g_file_info_get_attribute_boolean(cache_gfileinfo, G_FILE_ATTRIBUTE_ACCESS_CAN_DELETE)){
		debug("\t\tCache clean-up, deleting: [%s].", cache_file);
		g_file_delete(cache_gfile, NULL, NULL);
	}
	g_object_unref(cache_gfile);
	g_object_unref(cache_gfileinfo);
}/*cache_file_clean_up*/

static void cache_dir_clean_up(const gchar *cache_subdir){
	gchar *cache_dir_path=g_build_filename(cache_prefix, cache_subdir, NULL);
	cache_dir_absolute_clean_up(cache_dir_path);
	g_free(cache_dir_path);
}/*cache_dir_clean_up*/

gchar *cache_images_get_unknown_image_filename(void){
	if(unknown_image_filename){
		debug("Using unkown image: %s.", unknown_image_filename);
		return g_strdup(unknown_image_filename);
	}
	
	debug("**NOTICE:** Setting inital unknown image.");
	
	gchar *home_unknown_image_filename=NULL;
#ifndef GNOME_ENABLE_DEBUG
	home_unknown_image_filename=g_build_filename(DATADIR, "icons", "gnome", "scalable", "status", "gtk-missing-image.svg", NULL);
	if(!(g_file_test(home_unknown_image_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR ))){
		g_free(home_unknown_image_filename);
#endif
		home_unknown_image_filename=g_build_filename(BUILDDIR, "data", "gnome", "scalable", "status", "gtk-missing-image.svg", NULL);
#ifndef GNOME_ENABLE_DEBUG
	}
#endif
	
	
	GtkImage *stock_unknown_image=NULL;
	if(!( (stock_unknown_image=(GtkImage *)gtk_image_new_from_stock(GTK_STOCK_MISSING_IMAGE, ImagesDialog)) )){
		debug("\t\t**WARNING:** Unable to load stock icon: GTK_STOCK_MISSING_IMAGE(%d).", GTK_STOCK_MISSING_IMAGE);
		unknown_image_filename=g_strdup(home_unknown_image_filename);
	}else{
		stock_unknown_image=g_object_ref_sink(stock_unknown_image);
		g_object_get(stock_unknown_image, "file", &unknown_image_filename, NULL );
		g_object_unref(stock_unknown_image);
		if(G_STR_EMPTY(unknown_image_filename)){
			debug("\t\t**WARNING:** Unable to get 'file' from stock icon: GTK_STOCK_MISSING_IMAGE(%d).", GTK_STOCK_MISSING_IMAGE);
			unknown_image_filename=g_strdup(home_unknown_image_filename);
		}
	}
	
	g_free(home_unknown_image_filename);
	
	return cache_images_get_unknown_image_filename();
}/*cache_images_get_unknown_image_filename*/

gchar *cache_images_get_filename(User *user){
	if(G_STR_EMPTY(user->user_name) || G_STR_EMPTY(user->image_url)){
		debug("**ERROR** Unable to parse an empty url into an image filename.");
		return cache_images_get_unknown_image_filename();
	}
	
	debug("Creating image file name for '%s@%s' from image url: %s.", user->user_name, user->service->server, user->image_url);
	
	/*
	 * image_name_info[] index explanation:
	 *	0 == the url scheme, e.g. https, http, etc.
	 *	1 == this is empty as a result of the '://' in url scheme.
	 *	2 == the domain name
	 *	n == the finale part of the url, i.e. the actual image's file name.
	 */
	gchar *image_file=NULL, **image_info=NULL;
	image_info=g_strsplit(user->image_url, "/", -1);
	guint n=g_strv_length(image_info)-1;
	image_file=g_strdup(image_info[n]);
	g_strfreev(image_info);
	
	if(G_STR_EMPTY(image_file)){
		if(image_file) g_free(image_file);
		debug("\t\t**WARNING:** Unable to parse url into a valid image filename.\n\t\tURL: [%s]", user->image_url);
		return cache_images_get_unknown_image_filename();
	}
	
	gchar *avatar_dir=g_build_filename("services", user->service->uri, "avatars", user->user_name, NULL);
	gchar *image_filename=NULL;
	gchar *avatar_path=NULL;
	if(!( (avatar_path=cache_dir_test(avatar_dir, TRUE)) && (image_filename=g_build_filename(avatar_path, image_file, NULL)) ))
		return cache_images_get_unknown_image_filename();
	else if(!( (g_file_test(image_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)) ))
		cache_dir_clean_up(avatar_dir);
	
	
	debug("\t\tSetting image filename:\n\t\turl: %s\n\t\tfile:%s\n\t\tfull path: %s", user->image_url, image_file, image_filename);
	
	g_free(avatar_path);
	g_free(avatar_dir);
	g_free(image_file);
	
	return image_filename;
}/*cache_images_get_filename*/


/********************************************************
 *                       eof                            *
 ********************************************************/
