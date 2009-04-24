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

#include <stdarg.h>
#include <string.h>

#include <glib.h>
#include <glib/gprintf.h>

/*
 * Set DEBUG to a colon/comma/space separated list of domains, or "all"
 * to get all debug output.
 */

#include "debug.h"

static gchar    **debug_strv=NULL;
static gboolean   all_domains = FALSE;
static gboolean devel=FALSE;

gboolean debug_check_devel(void){
#ifndef GNOME_ENABLE_DEBUG
	if(!( (g_getenv("DEBUG")) && (g_str_equal( (g_getenv("DEBUG")), "GNOME_ENABLE_DEBUG" )) ))
		return FALSE;
	
#define GNOME_ENABLE_DEBUG
#else
	if( (g_getenv("DEBUG")) && !(g_str_equal( (g_getenv("DEBUG")), "GNOME_ENABLE_DEBUG" )) )
		return FALSE;
	
#endif
	
	devel=TRUE;
	all_domains=TRUE;
	debug_strv=g_strsplit_set("all", ":, ", 0);
	
	return TRUE;
}//debug_check_devel

static void debug_init (void){
	static gboolean inited=FALSE;
	if(inited) return;
	inited=TRUE;
	if(debug_check_devel()) return;
	
	const gchar *env;
	gint         i;
	
	if(!(env=g_getenv("DEBUG")))
		return;
	
	debug_strv=g_strsplit_set (env, ":, ", 0);
	
	for(i=0; debug_strv && debug_strv[i]; i++)
		if (!(strcmp ("all", debug_strv[i])))
			all_domains=TRUE;
}

void debug_impl(const gchar *domain, const gchar *msg, ...){
	static gboolean output_started=FALSE;
	gint i;

	g_return_if_fail (domain != NULL);
	g_return_if_fail (msg != NULL);

	debug_init();

	for (i = 0; debug_strv && debug_strv[i]; i++) {
		if (all_domains || strcmp (domain, debug_strv[i]) == 0) {
			va_list args;
			if(!output_started){
				g_print("\n");
				output_started=TRUE;
			}
			
			g_print ("%s: ", domain);
			
			va_start(args, msg);
			g_vprintf(msg, args);
			va_end(args);
			
			g_print ("\n");
			break;
		}
	}
}

