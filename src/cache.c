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
#define DEBUG_DOMAINS "Cache:Requests:OnlineServices:Tweets:Users:Images:Files:I/O:Setup:Start-Up"
#include "debug.h"

static gchar *cache_prefix=NULL;


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static void cache_clean_up_dir_absolute(const gchar *cache_dir);
static void cache_clean_up_file(const gchar *cache_file);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
void cache_init(void){
	cache_prefix=g_build_filename(g_get_home_dir(), ".gnome2", PACKAGE_TARNAME, NULL);
}/*cache_init*/

void cache_deinit(void){
	g_free(cache_prefix);
}/*cache_deinit*/

static void cache_clean_up_dir_absolute(const gchar *cache_dir_path){
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
			cache_clean_up_file(cache_file_path);
		else if(g_file_test(cache_file_path, G_FILE_TEST_IS_DIR)){
			cache_clean_up_dir_absolute(cache_file_path);
			if(depth>1)
				cache_clean_up_file(cache_file_path);
		}
		g_free(cache_file_path);
	}
	g_dir_close(cache_dir);
	depth--;
}/*cache_clean_up_absolute_dir*/

static void cache_clean_up_file(const gchar *cache_file){
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
}/*cache_clean_up_file*/

void cache_clean_up_dir(const gchar *cache_subdir){
	gchar *cache_dir_path=g_build_filename(cache_prefix, cache_subdir, NULL);
	cache_clean_up_dir_absolute(cache_dir_path);
	g_free(cache_dir_path);
}/*cache_clean_up_dir*/

/********************************************************
 *                       eof                            *
 ********************************************************/
