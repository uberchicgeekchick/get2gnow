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
#include <glib.h>
#include <libnotify/notify.h>

#include "config.h"
#include "program.h"

#include "online-services.typedefs.h"
#include "online-services.types.h"
#include "online-service.types.h"
#include "update.types.h"
#include "users.types.h"

#include "gconfig.h"
#include "preferences.defines.h"

#include "images.h"

#include "update.h"

#include "main-window.h"
#include "update-viewer.h"


/********************************************************************************
 *      Project, system, & library headers.  eg #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/


/********************************************************************************
 *                        defines, macros, methods, & etc                       *
 ********************************************************************************/


/********************************************************************************
 *                        objects, structs, and enum typedefs                   *
 ********************************************************************************/


/********************************************************************************
 *                prototypes for private methods & functions                    *
 ********************************************************************************/


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/


/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define DEBUG_DOMAINS "OnlineServices:UI:Networking:Updates:Requests:Notification:Settings:Setup:Users:Timelines:update.c"
#include "debug.h"


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
const gchar *update_type_to_string(UpdateType update_type){
	switch(update_type){
		case	Homepage:	return _("homepage");
		case	ReTweets:	return _("friends' retweets");
		case	Replies:	return _("@ replies");
		case	DMs:		return _("direct messages");
		case	BestFriends:	return _("best friend's updates");
		case	Users:		return _("friend's updates");
		case	Faves:		return _("star'd updates");
		case	Searches:	return _("search results");
		case	Groups:		return _("group discussions");
		case	Timelines:	return _("global updates");
		case	Archive:	return _("my updates");
		case	None:	default:
			return _("a lil cracker :-P");
	}
}/*update_type_to_string(update_type);*/



gboolean update_notify_on_timeout(UserStatus *status){
	if(!(status && G_STR_N_EMPTY(status->notification))) return FALSE;
	
	NotifyNotification *notify_notification=NULL;
	GError		*error=NULL;
	const gchar	*notification_icon_file_name=NULL;
	if(!(g_file_test(status->user->image_file, G_FILE_TEST_EXISTS|G_FILE_TEST_IS_REGULAR)))
		notification_icon_file_name=GETTEXT_PACKAGE;
	else
		notification_icon_file_name=status->user->image_file;
	
	if(!main_window_status_icon_is_embedded())
		notify_notification=notify_notification_new(GETTEXT_PACKAGE, status->notification, notification_icon_file_name, NULL);
	else
		notify_notification=notify_notification_new_with_status_icon(GETTEXT_PACKAGE, status->notification, notification_icon_file_name, main_window_status_icon_get());
	
	if(!notify_notification){
		debug("**ERROR:** Failed to create notification of new update from <%s@%s> on <%s>", status->user->user_name, status->service->uri, status->service->key);
		return FALSE;
	}
	
	if(g_file_test(status->user->image_file, G_FILE_TEST_EXISTS|G_FILE_TEST_IS_REGULAR)){
		GdkPixbuf *pixbuf=NULL;
		if( (pixbuf=images_get_default_pixbuf_from_filename(status->user->image_file)) ){
			notify_notification_set_icon_from_pixbuf(notify_notification, pixbuf);
			uber_object_unref(pixbuf);
		}
	}
	notify_notification_set_hint_string(notify_notification, "suppress-sound", "");
	notify_notification_set_timeout(notify_notification, 10000);
	
	if(gconfig_if_bool(PREFS_NOTIFY_BEEP, TRUE))
		update_viewer_beep();
	
	notify_notification_show(notify_notification, &error);
	
	if(error){
		debug("Error displaying status->notification: %s", error->message);
		g_error_free(error);
	}
	
	uber_object_unref(notify_notification);
	
	return FALSE;
}/*update_notify_on_timeout - only used as a callback to g_timer_add_seconds_full - see 'src/parser.c'. */


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

