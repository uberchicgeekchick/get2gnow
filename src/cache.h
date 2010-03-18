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

/**********************************************************************
 *          My art, code, & programming.                              *
 **********************************************************************/
#ifndef __CACHE_H__
#define __CACHE_H__

#ifndef	_GNU_SOURCE 
#	define _GNU_SOURCE
#endif

#ifndef _THREAD_SAFE
#	define _THREAD_SAFE
#endif


/**********************************************************************
 *        System & library headers, eg #include <gdk/gdkkeysyms.h>    *
 **********************************************************************/
#include <glib.h>
#include <libsoup/soup-message-body.h>
#include <libxml/tree.h>

#include "online-services.typedefs.h"
#include "users.h"
#include "images.h"

/**********************************************************************
 *        Objects, structures, and etc typedefs                       *
 **********************************************************************/
#ifndef cache_images_get_user_avatar_filename
#	define	cache_images_get_user_avatar_filename(service, user_name, image_uri)				\
		cache_create_image_filename_from_uri(service, "avatars", user_name, image_uri)
#endif

#ifndef cache_images_get_group_image_filename
#	define	cache_images_get_group_image_filename(service, group, image_uri)				\
		cache_create_image_filename_from_uri(service, "groups", group, image_uri)
#endif

#ifndef cache_images_get_user_profile_background_filename
#	define	cache_images_get_user_profile_background_filename(service, user_name, image_uri)		\
		cache_create_image_filename_from_uri(service, "profile_backgrounds", user_name, image_uri)
#endif


/**********************************************************************
 *          Global method  & function prototypes                      *
 **********************************************************************/
G_BEGIN_DECLS

void cache_init(void);
void cache_deinit(void);

gchar *cache_dir_test(const gchar *cache_dir, gboolean mkdir);
void cache_dir_clean_up(const gchar *cache_subdir, gboolean rm_parent);

void cache_get_uri_filename(const gchar *uri, gboolean set_subdir, gchar **subdir, gboolean set_filename, gchar **filename, gboolean set_query_string, gchar **query_string);
gchar *cache_file_touch(const gchar *cache_file);

gboolean cache_save_page(OnlineService *service, const gchar *uri, SoupMessageBody *page);

gchar *cache_file_create_file_for_online_service(OnlineService *service, const gchar *subdir1_or_file, ...);

gchar *cache_create_image_filename_from_uri(OnlineService *service, const gchar *image_type, const gchar *user_name, const gchar *image_uri);

G_END_DECLS
#endif /* __CACHE_H__ */
/**********************************************************************
 *                               eof                                  *
 **********************************************************************/
