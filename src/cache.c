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
#include <sys/stat.h>
#include <gio/gio.h>
#include <glib/gstdio.h>

#include <errno.h>
#include <libxml/parser.h>

#include "config.h"
#include "program.h"

#include "online-services.types.h"
#include "online-service.types.h"
#include "online-service.h"

#include "cache.h"

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define DEBUG_DOMAINS "OnlineServices:Requests:Updates:Users:Images:Files:I/O:Setup:Start-Up:Shutdown:Cache:cache.c"
#include "debug.h"

static gchar *cache_prefix=NULL;


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static gchar *cache_file_create_online_service_xml_file(OnlineService *service, const gchar *uri);
static gboolean cache_file_save_online_service_xml(OnlineService *service, const gchar *uri, const gchar *xml, goffset length);
static gchar *cache_path_create(const gchar *cache_file, ...);

static void cache_dir_absolute_clean_up(const gchar *cache_dir, gboolean rm_parent);
static void cache_file_clean_up(const gchar *cache_file);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
void cache_init(void){
	cache_prefix=g_build_filename(g_get_home_dir(), ".gnome2", CONFIG_SUBDIR, NULL);
	
	gchar *cache_path_test=NULL;
	if(!(cache_path_test=cache_dir_test("services", TRUE) ))
		g_error("Unable to access config directory: %s.", cache_prefix);
	
	uber_free(cache_path_test);
}/*cache_init();*/

void cache_deinit(void){
	debug("**SHUTDOWN:** releasing memory of cache prefix: %s", cache_prefix);
	uber_free(cache_prefix);
}/*cache_deinit();*/

static gchar *cache_file_create_online_service_xml_file(OnlineService *service, const gchar *uri){
	gchar *subdir, *filename, *query_string;
	cache_get_uri_filename(uri, TRUE, &subdir, TRUE, &filename, TRUE, &query_string);
	const gchar *page;
	if(!( ((query_string)) && (page=g_strrstr(query_string, "page=")) ))
		page="0";
	gchar *filename_xml=g_strdup_printf("%s/page_%s", subdir, page);
	
	
	gchar *cache_file_xml=NULL;
	debug("Attempting to cache %s in subdir: %s as file: %s", uri, subdir, filename_xml);
	if(!(cache_file_xml=cache_file_create_file_for_online_service(service, subdir, filename_xml, NULL)))
		debug("**ERROR:** caching failed");
	else
		debug("Cached URI: %s to: %s", uri, cache_file_xml);
	
	uber_free(query_string);
	uber_free(subdir);
	uber_free(filename_xml);
	return cache_file_xml;
}/*cache_file_create_online_service_xml_file(service, uri);*/

gboolean cache_save_page(OnlineService *service, const gchar *uri, SoupMessageBody *xml){
	debug("Caching page");
	if(!( cache_file_save_online_service_xml(service, uri, xml->data, xml->length) ))
		return FALSE;
	
	debug("Cached page's content");
	return TRUE;
}/*void cache_save_page(service, xml, subdir, filename);*/

static gboolean cache_file_save_online_service_xml(OnlineService *service, const gchar *uri, const gchar *xml, goffset length){
	if(!length){
		debug("Stubornly refusing to save an empty xml file");
		return FALSE;
	}
	gchar *cache_file_xml=NULL;
	if(!( cache_file_xml=cache_file_create_online_service_xml_file(service, uri) )){
		debug("**ERROR:** Unable to create xml cache filename");
		return FALSE;
	}
	if(!(g_file_set_contents(cache_file_xml, xml, length, NULL))){
		debug("**ERROR:** Unable to save xml contents to cache file: %s", cache_file_xml);
		uber_free(cache_file_xml);
		return FALSE;
	}
	debug("Cache created: %s", cache_file_xml);
	uber_free(cache_file_xml);
	return TRUE;
}/*cache_file_save_online_service_xml(service, subdir, filename, xml, length);*/

gchar *cache_dir_test(const gchar *cache_dir, gboolean mkdir){
	gchar *cache_path=cache_path_create(cache_dir, NULL);
	if(!g_file_test(cache_path, G_FILE_TEST_EXISTS|G_FILE_TEST_IS_DIR)){
		if(!mkdir) return NULL;
		
		if(g_mkdir_with_parents(cache_path, S_IRUSR|S_IWUSR|S_IXUSR)){
			debug("**ERROR:** Unable to create cache directory: [%s]", cache_path);
			uber_free(cache_path);
			return NULL;
		}
	}
	return cache_path;
}/*cache_dir_test("~/gnome2/get2gnow/cache/images", TRUE|FALSE);*/

static void cache_dir_absolute_clean_up(const gchar *cache_dir_path, gboolean rm_parent){
	if(!(g_str_has_prefix(cache_dir_path, cache_prefix)))
		return;
	
	if(!( (g_file_test(cache_dir_path, G_FILE_TEST_EXISTS|G_FILE_TEST_IS_DIR)) )){
		cache_file_clean_up(cache_dir_path);
		return;
	}
	
	debug("Cleaning-up cache directory: [%s]", cache_dir_path);
	
	const gchar *cache_file=NULL;
	GDir *cache_dir=g_dir_open(cache_dir_path, 0, NULL);
	while((cache_file=g_dir_read_name(cache_dir)) ){
		gchar *cache_file_path=g_strdup_printf("%s/%s", cache_dir_path, cache_file);
		if(g_file_test(cache_file_path, G_FILE_TEST_IS_DIR)){
			cache_dir_absolute_clean_up(cache_file_path, TRUE);
			if(rm_parent)
				cache_file_clean_up(cache_file_path);
		}else
			cache_file_clean_up(cache_file_path);
		uber_free(cache_file_path);
	}
	g_dir_close(cache_dir);
	if(rm_parent)
		cache_file_clean_up(cache_dir_path);
}/*cache_dir_absolute_clean_up("~/gnome2/get2gnow/cache/images/", TRUE|FALSE);*/

void cache_dir_clean_up(const gchar *cache_subdir, gboolean rm_parent){
	gchar *cache_dir_path=cache_path_create(cache_subdir, NULL);
	debug("%s cache directory: [%s]", (rm_parent ?"Removing directory and its contents" :"Cleaning-up"), cache_dir_path);
	cache_dir_absolute_clean_up(cache_dir_path, rm_parent);
	uber_free(cache_dir_path);
}/*cache_dir_clean_up("~/gnome2/get2gnow/cache/images/", TRUE|FALSE);*/

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
		uber_free(cache_path_full);
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
			debug("**ERROR:** Failed to create cache file: [%s]", cache_filename);
			uber_free(cache_filename);
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
		debug("Cache clean-up, deleting: [%s]", cache_file);
		g_remove(cache_file);
	}
	uber_object_unref(cache_gfile);
	uber_object_unref(cache_gfileinfo);
}/*cache_file_clean_up("friends001.xml");*/

/**
 * Do not initalize any of the pointers you want to use prior to sending them as parameters.
 */
void cache_get_uri_filename(const gchar *uri, gboolean set_subdir, gchar **subdir, gboolean set_filename, gchar **filename, gboolean set_query_string, gchar **query_string){
	/*
	 * uri_info[] index explanation:
	 * 	0 == the uri scheme, e.g. https, http, etc.
	 * 	1 == this is empty as a result of the '://' in uri scheme.
	 * 	2 == the domain name
	 * 	3-n == the finale part of the uri, e.g. an image's file name or xml page w/its query string.
	 */
	gchar **uri_info=g_strsplit_set(uri, "/?", -1);
	guint n=0;
	if(!g_strrstr(uri, "?"))
		n=g_strv_length(uri_info)-1;
	else
		n=g_strv_length(uri_info)-2;
	
	if(set_query_string)
		(*query_string)=g_strdup(g_strrstr(uri, "?"));
	
	if(set_subdir)
		(*subdir)=g_strdup(uri_info[n-1]);
	
	if(set_filename)
		(*filename)=g_strdup(uri_info[n]);
	
	g_strfreev(uri_info);
}/*gchar *filename; cache_get_uri_filename(const gchar *uri, NULL, &filename, NULL);*/


gchar *cache_file_create_file_for_online_service(OnlineService *service, const gchar *subdir1_or_file, ...){
	gchar	*subdir_or_filename=NULL;
	
	gchar	*dir=NULL;
	gchar	*directory=NULL;
	
	gchar	*file=NULL;
	gchar	*filename=NULL;
	
	dir=cache_path_create("services", service->uri, service->user_name, NULL);
	
	va_list cache_subdirs_and_file;
	va_start(cache_subdirs_and_file, subdir1_or_file);
	for(subdir_or_filename=(gchar *)subdir1_or_file; subdir_or_filename; subdir_or_filename=va_arg(cache_subdirs_and_file, gchar *)){
		if(file) uber_free(file);
		file=cache_path_create(dir, subdir_or_filename, NULL);
		
		if(!(subdir_or_filename=va_arg(cache_subdirs_and_file, gchar *))) break;
		
		if(directory) uber_free(directory);
		directory=cache_path_create(dir, subdir_or_filename, NULL);
		dir=directory;
	}
	va_end(cache_subdirs_and_file);
	
	if(!( (directory=cache_dir_test(dir, TRUE)) && (filename=cache_file_touch(file)) )){
		debug("**ERROR:** Unable to create cache file");
		debug("**ERROR:** Cache prefix: [%s]", cache_prefix);
		
		if(!directory)
			debug("**ERROR:** Unable to create cache directory: [%s]", dir);
		else{
			uber_free(directory);
			debug("**ERROR:** Unable to create cache file: [%s]", file);
		}
		
		uber_free(dir);
		uber_free(file);
		return NULL;
	}
	
	debug("Created <%s>'s cache filename: [%s]", service->key, filename);
	debug("Directory: [%s]", directory);
	debug("File: [%s]", file);
	
	uber_free(dir);
	uber_free(file);
	uber_free(directory);
	return filename;
}/*cache_file_create_file_for_online_service(service, "users", "uberChick.xml", NULL);*/

gchar *cache_create_image_filename_from_uri(OnlineService *service, const gchar *image_type, const gchar *user_name, const gchar *image_uri){
	if(!(G_STR_N_EMPTY(user_name) && G_STR_N_EMPTY(image_uri) )){
		debug("**ERROR** Unable to parse an empty uri into an image filename.  Attempting to load %s for user: <%s>; using uri: [%s]", image_type, user_name, image_uri);
		return images_get_unknown_image_filename();
	}
	
	debug("Creating image file name for '%s@%s' from image uri: %s", user_name, service->uri, image_uri);
	
	gchar *image_file;
	cache_get_uri_filename(image_uri, FALSE, NULL, TRUE, &image_file, FALSE, NULL);
	if(G_STR_EMPTY(image_file)){
		if(image_file) uber_free(image_file);
		debug("**WARNING:** Unable to parse uri into a valid image filename.  Loading %s for user: <%s>; using uri: [%s]", image_type, user_name, image_uri);
		return images_get_unknown_image_filename();
	}
	
	gchar *image_dir=cache_path_create("services", service->uri, image_type, user_name, NULL);
	gchar *image_filename=NULL;
	gchar *image_path=NULL;
	if(!( ((image_path=cache_dir_test(image_dir, TRUE))) && (image_filename=cache_path_create(image_path, image_file, NULL)) )){
		uber_free(image_file);
		uber_free(image_dir);
		if(image_path) uber_free(image_path);
		return images_get_unknown_image_filename();
	}
	
	if(!( (g_file_test(image_filename, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)) )){
		debug("Cleaning up old %s from: [%s]", image_type, image_dir);
		cache_dir_clean_up(image_dir, FALSE);
	}
	
	
	debug("Setting image filename:\n\t\t[%s]", image_filename);
	
	uber_free(image_file);
	uber_free(image_dir);
	uber_free(image_path);
	
	return image_filename;
}/*cache_create_image_filename_from_uri(service, user->user_name, user->image_uri);*/


/********************************************************
 *                       eof                            *
 ********************************************************/
