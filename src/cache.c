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
#define _GNU_SOURCE
#define _THREAD_SAFE



/********************************************************
 *        Project headers, eg #include "config.h"       *
 ********************************************************/
#include <errno.h>
#include <libxml/parser.h>

#include "config.h"
#include "main.h"
#include "cache.h"

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define DEBUG_DOMAINS "OnlineServices:Requests:Tweets:Users:Images:Files:I/O:Setup:Start-Up:Shutdown:Cache"
#include "debug.h"

static gchar *cache_prefix=NULL;
static gchar *unknown_image_filename=NULL;


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static gchar *cache_file_create_online_service_xml_file(OnlineService *service, const gchar *subdir, const gchar *filename);
static gboolean cache_file_save_online_service_xml(OnlineService *service, const gchar *subdir, const gchar *filename, const gchar *xml, goffset length);
static gchar *cache_path_create(const gchar *cache_file, ...);

static void cache_dir_absolute_clean_up(const gchar *cache_dir, gboolean rm_parent);
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
	
	gchar *cache_path_test=NULL;
	if(!(cache_path_test=cache_dir_test("services", TRUE) ))
		g_error("Unable to access config directory: %s.", cache_prefix);
	
	uber_free(cache_path_test);
	
	cache_images_get_unknown_image_filename();
}/*cache_init*/

void cache_deinit(void){
	debug("**SHUTDOWN:** releasing memory of cache prefix: %s.", cache_prefix);
	g_free(cache_prefix);
	
	debug("**SHUTDOWN:** releasing memory of unknown image: %s.", unknown_image_filename);
	g_free(unknown_image_filename);
	unknown_image_filename=NULL;
}/*cache_deinit*/

static gchar *cache_file_create_online_service_xml_file(OnlineService *service, const gchar *subdir, const gchar *filename){
	gchar *filename_xml=g_strdup_printf("%s.xml", filename);
	gchar *cache_file_xml=cache_file_create_file_for_online_service(service, subdir, filename_xml, NULL);
	uber_free(filename_xml);
	return cache_file_xml;
}/*cache_file_create_online_service_xml_file(service, subdir, filename);*/

gboolean cache_save_xml(OnlineService *service, xmlNode *xml_node, const gchar *subdir, const gchar *filename){
	gchar *xml=NULL;
	if( G_STR_EMPTY( (xml=(gchar *)xmlNodeGetContent(xml_node)) ) ) return FALSE;
	if(!( cache_file_save_online_service_xml(service, subdir, filename, xml, g_utf8_strlen(xml, -1)) )){
		debug("**ERROR:** Unable to cache xmlNode's %s contents.  Attempting to cache page to file: %s; subdir: %s.", xml_node->name, filename, subdir);
		return FALSE;
	}
	debug("Saved xmlNode's %s content.  Cached as %s in subdir: %s.", xml_node->name, filename, subdir);
	return TRUE;
}/*cache_xml_save(OnlineService *service, root_element, subdir, filename);*/

gboolean cache_save_page(OnlineService *service, SoupMessageBody *xml, const gchar *subdir, const gchar *filename){
	if(!xml->length) return FALSE;
	if(!( cache_file_save_online_service_xml(service, subdir, filename, xml->data, xml->length) )){
		debug("**ERROR:** Unable to cache page.  Attempting to cache page to file: %s; subdir: %s.", filename, subdir);
		return FALSE;
	}
	debug("Saved page's content.  Cached as %s in subdir: %s.", filename, subdir);
	return TRUE;
}/*void cache_save_page(service, xml, subdir, filename);*/

static gboolean cache_file_save_online_service_xml(OnlineService *service, const gchar *subdir, const gchar *filename, const gchar *xml, goffset length){
	if(!length){
		debug("Unable to save xml cache file: %s; subdir: %s.  Saving would result in an empty file.", filename, subdir);
		return FALSE;
	}
	gchar *cache_file_xml=NULL;
	if(!( cache_file_xml=cache_file_create_online_service_xml_file(service, subdir, filename) )){
		debug("**ERROR:** Unable to create xml cache filename.");
		return FALSE;
	}
	if(!(g_file_set_contents(cache_file_xml, xml, length, NULL))){
		debug("**ERROR:** Unable to save xml contents to cache file: %s.", cache_file_xml);
		uber_free(cache_file_xml);
		return FALSE;
	}
	debug("Cache created: %s.", cache_file_xml);
	uber_free(cache_file_xml);
	return TRUE;
}/*cache_file_save_online_service_xml(service, subdir, filename, xml, length)*/

gchar *cache_dir_test(const gchar *cache_dir, gboolean mkdir){
	gchar *cache_path=cache_path_create(cache_dir, NULL);
	if(!g_file_test(cache_path, G_FILE_TEST_EXISTS|G_FILE_TEST_IS_DIR)){
		if(!mkdir) return NULL;
		
		if(g_mkdir_with_parents(cache_path, S_IRUSR|S_IWUSR|S_IXUSR)){
			debug("***ERROR:*** Unable to create cache directory: [%s].", cache_path);
			g_free(cache_path);
			return NULL;
		}
	}
	return cache_path;
}/*cache_dir_test*/

static void cache_dir_absolute_clean_up(const gchar *cache_dir_path, gboolean rm_parent){
	if(!(g_str_has_prefix(cache_dir_path, cache_prefix)))
		return;
	
	if(!( (g_file_test(cache_dir_path, G_FILE_TEST_EXISTS|G_FILE_TEST_IS_DIR)) ))
		return;
	
	debug("Cleaning-up cache directory: [%s].", cache_dir_path);
	
	const gchar *cache_file=NULL;
	GDir *cache_dir=g_dir_open(cache_dir_path, 0, NULL);
	while( (cache_file=g_dir_read_name(cache_dir)) ){
		gchar *cache_file_path=g_strdup_printf("%s/%s", cache_dir_path, cache_file);
		if(g_file_test(cache_file, G_FILE_TEST_IS_REGULAR))
			cache_file_clean_up(cache_file_path);
		else if(g_file_test(cache_file_path, G_FILE_TEST_IS_DIR)){
			cache_dir_absolute_clean_up(cache_file_path, TRUE);
			if(rm_parent)
				cache_file_clean_up(cache_file_path);
		}
		g_free(cache_file_path);
	}
	g_dir_close(cache_dir);
}/*cache_dir_absolute_clean_up*/

void cache_dir_clean_up(const gchar *cache_subdir, gboolean rm_parent){
	gchar *cache_dir_path=cache_path_create(cache_subdir, NULL);
	cache_dir_absolute_clean_up(cache_dir_path, rm_parent);
	g_free(cache_dir_path);
	if(rm_parent)
		cache_file_clean_up(cache_dir_path);
}/*cache_dir_clean_up*/

static gchar *cache_path_create(const gchar *cache_file, ...){
	gchar *cache_path_full=NULL;
	if(!g_str_has_prefix(cache_file, cache_prefix))
		cache_path_full=g_build_filename(cache_prefix, NULL);
	else
		cache_path_full=g_strdup("");
	
	gchar *cache_path_swap=NULL;
	gchar *cache_dir=NULL;
	
	va_list cache_dirs;
	va_start(cache_dirs, cache_file);
	for(cache_dir=(gchar *)cache_file; cache_dir; cache_dir=va_arg(cache_dirs, gchar *)){
		cache_path_swap=g_build_filename(cache_path_full, cache_dir, NULL);
		g_free(cache_path_full);
		cache_path_full=cache_path_swap;
		cache_path_swap=NULL;
	}
	va_end(cache_dirs);
	
	return cache_path_full;
}/*cache_path_create("cache.xml");*/

gchar *cache_file_touch(const gchar *cache_file){
	gchar *cache_filename=cache_path_create(cache_file, NULL);
	
	if(!(g_file_test(cache_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)))
		if(!(g_creat(cache_filename, S_IRUSR|S_IWUSR))){
			debug("**ERROR:** Failed to create cache file: [%s].", cache_filename);
			g_free(cache_filename);
			return NULL;
		}
	
	return cache_filename;
}/*cache_file_touch("cache.xml");*/

static void cache_file_clean_up(const gchar *cache_file){
	if(!(g_str_has_prefix(cache_file, cache_prefix)))
		return;
	
	GFile *cache_gfile=g_file_new_for_path(cache_file);
	GFileInfo *cache_gfileinfo=g_file_query_info(cache_gfile, G_FILE_ATTRIBUTE_ACCESS_CAN_DELETE, G_FILE_QUERY_INFO_NONE, NULL, NULL);
	if(g_file_info_get_attribute_boolean(cache_gfileinfo, G_FILE_ATTRIBUTE_ACCESS_CAN_DELETE)){
		debug("Cache clean-up, deleting: [%s].", cache_file);
		g_remove(cache_file);
	}
	g_object_unref(cache_gfile);
	g_object_unref(cache_gfileinfo);
}/*cache_file_clean_up*/

gchar *cache_filename_get_from_uri(const gchar *uri){
	gchar *uri_dup=g_strdup(uri);
	gchar *cache_file=g_strdelimit(uri_dup, ":/&?", '_');
	gchar *cache_filename=cache_path_create(cache_file, NULL);
	uber_free(uri_dup);
	uber_free(cache_file);
	return cache_filename;
}/*cache_file_from_uri*/


gchar *cache_file_create_file_for_online_service(OnlineService *service, const gchar *subdir1_or_file, ...){
	gchar	*dir=NULL;
	gchar	*directory=NULL;
	gchar	*cache_subdir_or_file=NULL;
	
	gchar	*file=NULL;
	gchar	*filename=NULL;
	
	dir=cache_path_create("services", online_service_get_uri(service), online_service_get_user_name(service), NULL);
	
	va_list cache_subdirs_and_file;
	va_start(cache_subdirs_and_file, subdir1_or_file);
	for(cache_subdir_or_file=(gchar *)subdir1_or_file; cache_subdir_or_file; cache_subdir_or_file=va_arg(cache_subdirs_and_file, gchar *)){
		if(file){
			g_free(dir);
			dir=cache_path_create(dir, file, NULL);
			g_free(file);
		}
		file=cache_path_create(dir, cache_subdir_or_file);
	}
	va_end(cache_subdirs_and_file);
	
	if(!( (directory=cache_dir_test(dir, TRUE)) && (filename=cache_file_touch(file)) )){
		debug("**ERROR:** Unable to create cache file.");
		debug("**ERROR:** Cache prefix: [%s].", cache_prefix);
		
		if(!directory)
			debug("**ERROR:** Unable to create cache directory: [%s].", dir);
		else{
			uber_free(directory);
			debug("**ERROR:** Unable to create cache file: [%s].", file);
		}
		
		uber_free(dir);
		uber_free(file);
		return NULL;
	}
	
	debug("Created <%s>'s cache file: %s.", online_service_get_key(service), file);
	debug("Directory: [%s].", directory);
	debug("Filename: [%s].", filename);
	
	uber_free(dir);
	uber_free(file);
	uber_free(directory);
	return filename;
}/*cache_file_create_file_for_online_service(service, "users", "uberChick.xml", NULL);*/

gchar *cache_images_get_unknown_image_filename(void){
	if(unknown_image_filename){
		return g_strdup(unknown_image_filename);
	}
	
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
		unknown_image_filename=g_strdup(home_unknown_image_filename);
	}else{
		stock_unknown_image=g_object_ref_sink(stock_unknown_image);
		g_object_get(stock_unknown_image, "file", &unknown_image_filename, NULL );
		g_object_unref(stock_unknown_image);
		if(G_STR_EMPTY(unknown_image_filename)){
			unknown_image_filename=g_strdup(home_unknown_image_filename);
		}
	}
	
	g_free(home_unknown_image_filename);
	
	return cache_images_get_unknown_image_filename();
}/*cache_images_get_unknown_image_filename*/

gchar *cache_images_get_user_avatar_filename(OnlineService *service, const gchar *user_name, const gchar *image_url){
	if(!(G_STR_N_EMPTY(user_name) && G_STR_N_EMPTY(image_url) )){
		debug("**ERROR** Unable to parse an empty url into an image filename.  Attempting to load avatar for user: <%s>; using url: [%s].", user_name, image_url);
		return cache_images_get_unknown_image_filename();
	}
	
	debug("Creating image file name for '%s@%s' from image url: %s.", user_name, online_service_get_uri(service), image_url);
	
	/*
	 * image_name_info[] index explanation:
	 *	0 == the url scheme, e.g. https, http, etc.
	 *	1 == this is empty as a result of the '://' in url scheme.
	 *	2 == the domain name
	 *	n == the finale part of the url, i.e. the actual image's file name.
	 */
	gchar *image_file=NULL, **image_info=NULL;
	image_info=g_strsplit(image_url, "/", -1);
	guint n=g_strv_length(image_info)-1;
	image_file=g_strdup(image_info[n]);
	g_strfreev(image_info);
	
	if(G_STR_EMPTY(image_file)){
		if(image_file) g_free(image_file);
		debug("**WARNING:** Unable to parse url into a valid image filename.  Loading avatar for user: <%s>; using url: [%s].", user_name, image_url);
		return cache_images_get_unknown_image_filename();
	}
	
	gchar *avatar_dir=cache_path_create("services", online_service_get_uri(service), "avatars", user_name, NULL);
	gchar *image_filename=NULL;
	gchar *avatar_path=NULL;
	if(!( (avatar_path=cache_dir_test(avatar_dir, TRUE)) && (image_filename=cache_path_create(avatar_path, image_file, NULL)) ))
		return cache_images_get_unknown_image_filename();
	else if(!( (g_file_test(image_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)) ))
		cache_dir_clean_up(avatar_dir, FALSE);
	
	
	debug("Setting image filename:\n\t\turl: %s\n\t\tfile:%s\n\t\tfull path: %s", image_url, image_file, image_filename);
	
	g_free(avatar_path);
	g_free(avatar_dir);
	g_free(image_file);
	
	return image_filename;
}/*cache_images_get_user_avatar_filename(service, user->user_name, user->image_url);*/


/********************************************************
 *                       eof                            *
 ********************************************************/
