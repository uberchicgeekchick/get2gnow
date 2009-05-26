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
#include <libnotify/notify.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <glib/gprintf.h>

#include "config.h"
#include "program.h"
#include "debug.h"
#include "gconfig.h"
#include "app.h"
#include "online-services.h"
#include "ipc.h"
#include "tweet-list.h"
#include "images.h"
#include "cache.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
static gboolean notifing=FALSE;

/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
void get2gnow_init(int argc, char **argv){
	if( (ipc_init_check( argc-1, argv-1)) ){
		g_printf( "%s is already running.  Be sure to check system try for %s's icon.\n", PACKAGE_NAME, PACKAGE_NAME );
		ipc_deinit();
		exit(0);
	}
	
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);
	
	g_set_application_name(GETTEXT_PACKAGE);
	
	if(!g_thread_supported()) g_thread_init(NULL);
	
	gtk_init(&argc, &argv);
	
	gtk_window_set_default_icon_name(PACKAGE_NAME);
	
	debug_init();
	
	cache_init();
	
	/* Connect to gconf */
	gconfig_start();
	
	/* Start's the network & sets extern OnlineServices *online_services; from 'online-services.h'. */
	online_services_init();

	/* Start libnotify */
	notifing=notify_init(PACKAGE_NAME);
	
	/* Create the ui */
	app_create();
}/*start_up*/


void get2gnow_deinit(void){
	/* Close libnotify */
	if(notifing) notify_uninit();
	
	/* Close the network */
	online_services_deinit(online_services);
	
	ipc_deinit();
	
	/* Clean up the ui */
	g_object_unref(tweet_list_get());
	g_object_unref(app_get());
	
	gconfig_shutdown();
	
	cache_deinit();

	debug_deinit();
}/*shutdown*/


/********************************************************
 *                       eof                            *
 ********************************************************/
