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

#include <errno.h>
#include <stdarg.h>
#include <string.h>

#include <glib.h>
#include <glib/gprintf.h>
#include "main.h"
#include "cache.h"

/*
 * Set DEBUG to a colon/comma/space separated list of domains, or "all"
 * to get all debug output.
 */
#define DEBUG_DOMAINS "All"
#include "debug.h"

static gchar **debug_strv=NULL;
static gboolean all_domains=FALSE;
static gboolean devel=FALSE;
static gchar *debug_last_domain=NULL;
static gchar *debug_environmental_variable=NULL;

static FILE *debug_log_fp=NULL;
static const gchar *debug_log_filename=NULL;
static const gchar *debug_log_filename_swp=NULL;

gboolean debug_check_devel(void){
#ifndef GNOME_ENABLE_DEBUG
	if(!( (g_getenv(debug_environmental_variable)) && (g_str_equal( (g_getenv(debug_environmental_variable)), "GNOME_ENABLE_DEBUG" )) ))
		return FALSE;
	
#define GNOME_ENABLE_DEBUG
#else
	if( (g_getenv(debug_environmental_variable)) && !(g_str_equal( (g_getenv(debug_environmental_variable)), "GNOME_ENABLE_DEBUG" )) )
		return FALSE;
	
#endif
	
	devel=TRUE;
	all_domains=TRUE;
	debug_strv=g_strsplit("All", ":", 1);
	
	return TRUE;
}//debug_check_devel

void debug_init(void){
	gchar *debug_package=g_utf8_strup(PACKAGE_TARNAME, -1);
	debug_environmental_variable=g_strdup_printf("%s_DEBUG", debug_package);
	g_free(debug_package);
	debug_log_filename=cache_file_touch("debug.log");
	debug_log_filename_swp=g_strdup_printf("%s.swp", debug_log_filename);
	debug_log_fp=fopen(debug_log_filename, "w");
	if(debug_check_devel()) return;
	
	const gchar *env;
	gint         i;
	
	if(!(env=g_getenv(debug_environmental_variable))){
		debug_strv=g_strsplit("-", ":", -1);
		return;
	}
	
	debug_strv=g_strsplit(env, ":", -1);
	
	for(i=0; debug_strv && debug_strv[i]; i++)
		if(!(strcasecmp ("All", debug_strv[i])))
			all_domains=TRUE;
}/*debug_init();*/

static void debug_log_rotate(void){
	struct stat debug_log_stat;
	
	if(stat(debug_log_filename, &debug_log_stat)){
		g_error("Failed to stat log file: %s.", debug_log_filename);
		return;
	}
	
	/*Not a mega-byte but close.. cause get2gnow has a shite load of debugging output.*/
	if(debug_log_stat.st_size <= 1000000)
		return;
	
	fclose(debug_log_fp);
	debug_log_fp=fopen(debug_log_filename, "r");
	FILE *debug_log_fp_swap=fopen(debug_log_filename_swp, "w");
	
	/*Save 1/10 of the log so far.*/
	gchar *debug_log_swap_contents=NULL;
	fread(debug_log_swap_contents, sizeof(gchar), 25000, debug_log_fp);
	fwrite(debug_log_swap_contents, sizeof(gchar), 25000, debug_log_fp_swap);
	fclose(debug_log_fp_swap);
	fclose(debug_log_fp);
	g_remove(debug_log_filename);
	g_rename(debug_log_filename_swp, debug_log_filename);
	debug_log_fp=fopen(debug_log_filename, "a");
}/*debug_log_rotate();*/

void debug_impl(const gchar *domain, const gchar *msg, ...){
	g_return_if_fail (domain != NULL);
	g_return_if_fail (msg != NULL);
	
	static gboolean output_started=FALSE;
	FILE *debug_output_fp=NULL;
	
	debug_log_rotate();
	
	gchar **domains=g_strsplit(domain, ":", -1);
	for(gint x=0; domains[x]; x++){
		for(gint i=0; debug_strv && debug_strv[i]; i++) {
			if(!domains[x+1])
				debug_output_fp=debug_log_fp;
			else if(!(all_domains || g_str_equal(domains[x], debug_strv[i]) ))
				continue;
			else{
				debug_output_fp=stdout;
				
				if(!output_started){
					output_started=TRUE;
					g_fprintf(debug_output_fp, "\n");
				}
			}
			
			if(g_str_has_prefix(msg, "**ERROR:**")){
				va_list args;
				va_start(args, msg);
				g_fprintf(stderr, "\n**%s %s %s**:\n\t", GETTEXT_PACKAGE, domains[x], _("error"));
				g_vfprintf(stderr, msg, args);
				va_end(args);
			}
			
			if(!( debug_last_domain && g_str_equal(debug_last_domain, domains[x]) )){
				if(debug_last_domain) g_free(debug_last_domain);
				debug_last_domain=g_strdup(domains[x]);
				g_fprintf(debug_output_fp, "\n%s:\n", domains[x]);
			}
			
			va_list args;
			va_start(args, msg);
			g_fprintf(debug_output_fp, "\t\t");
			g_vfprintf(debug_output_fp, msg, args);
			va_end(args);
			
			g_fprintf(debug_output_fp, "\n");
			
			g_strfreev(domains);
			return;
		}
	}
	g_strfreev(domains);
}

gboolean debug_if_domain(const gchar *domain){
	if(G_STR_EMPTY(domain))
		return FALSE;
	
	gchar **domains=g_strsplit(domain, ":", -1);
	for(gint i=0; debug_strv && debug_strv[i]; i++) {
		for(gint x=0; domains[x]; x++){
			if(!(all_domains || g_str_equal(domains[x], debug_strv[i]) ))
				continue;
			
			g_strfreev(domains);
			return TRUE;
		}
	}
	g_strfreev(domains);
	return FALSE;
}

void debug_deinit(void){
	if(debug_last_domain) g_free(debug_last_domain);
	fclose(debug_log_fp);
	g_free((gchar *)debug_log_filename);
	g_free((gchar *)debug_log_filename_swp);
	g_free(debug_environmental_variable);
	g_strfreev(debug_strv);
}/*debug_deinit*/

