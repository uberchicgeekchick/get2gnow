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
/*#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
*/
#include "config.h"
#include "main.h"
#include "program.h"
#include "datetime.h"


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
#define DEBUG_DOMAINS "System:locale:Date:Time:Datetime:Sorting:datetime.c"
#include "debug.h"

const gchar *old_locale=NULL;


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
void datetime_locale_init(void){
	old_locale=setlocale(LC_TIME, "C");
}/*datetime_locale_init();*/

void datetime_locale_deinit(void){
	if(old_locale)
		setlocale(LC_TIME, old_locale);
}/*datetime_locale_deinit();*/

void datetime_strp_ages(gchar *datetime, gulong *seconds_since_epoc, gchar **age, gint *seconds_old, gboolean use_gmt){
	tzset();
	time_t		t=time(NULL);
	struct tm	*ta;
	if(use_gmt)
		ta=gmtime(&t);
	else
		ta=localtime(&t);
	ta->tm_isdst=-1;
	
	struct tm	post;
	strptime(datetime, "%s", &post);
	post.tm_isdst=-1;
	*seconds_since_epoc=mktime(&post);
	
	debug("Setting age of datetime: [%s]; seconds since epoc: [%lu]", datetime, *seconds_since_epoc);
	datetime_strp_age(datetime, age, seconds_old, use_gmt);
	debug("Display time set to: %s, %d", *age, *seconds_old);
}/*datetime_strp_ages(status, TRUE|FALSE);*/

void datetime_strp_age(const gchar *datetime, gchar **age, gint *my_diff, gboolean use_gmt){
	gint diff=0.0;
	datetime_age(datetime, &diff, use_gmt);
	if(diff < 0) *my_diff=0;
	else *my_diff=diff;
	
	/* Up to one minute ago. */
	
	if(diff < 2){
		*age=g_strdup(_("1 second ago"));
		return;
	}
	if(diff < 60){
		*age=g_strdup_printf(_("%d seconds ago"), diff);
		return;
	}
	if(diff < 120){
		*age=g_strdup(_("1 minute ago"));
		return;
	}
	
	/* Minutes */
	diff=diff/60;
	if(diff < 60){
		*age=g_strdup_printf(_("%d minutes ago"), diff);
		return;
	}
	if(diff < 120){
		*age=g_strdup(_("1 hour ago"));
		return;
	}
	
	/* Hours */
	diff=diff/60;
	if(diff < 24){
		*age=g_strdup_printf(_("%d hours ago"), diff);
		return;
	}
	if(diff < 48){
		*age=g_strdup(_("1 day ago"));
		return;
	}
	
	/* Days */
	diff=diff/24;
	if(diff < 30){
		*age=g_strdup_printf(_("%d days ago"), diff);
		return;
	}
	if(diff < 365){
		*age=g_strdup_printf(_("%d months ago"), (diff/30));
		return;
	}
	
	diff=diff/365;
	*age=g_strdup_printf(_("%d year%s ago"), diff, (diff==1 ?"" :"s"));
	/* NOTE:
	 * 	About time, month, & year precision, "years aren't...blah blah".
	 * 	yeah well I agree!
	 * 	but I'm dealing w/integers not floating point arthmatic.
	 * 	so we'll all just have to get over is.
	 */
}/*datetime_strp_age(&age, datetime, &seconds_old, TRUE|FALSE);*/

void datetime_age(const gchar *datetime, gint *age, gboolean use_gmt){
	struct tm	*locale_datetime;
	struct tm	post;
	int		seconds_local;
	int		seconds_post;
	
	/*if(use_gmt)
		setlocale(LC_TIME, old_locale);*/
	
	time_t		t=time(NULL);
	
	tzset();
	if(use_gmt){
		locale_datetime=gmtime(&t);
		strptime(datetime, "%a %b %d %T %z %Y", &post);
	}else{
		locale_datetime=localtime(&t);
		strptime(datetime, "%a %b %d %T +0000 %Y", &post);
	}
	locale_datetime->tm_isdst=-1;
	
	seconds_local=mktime(locale_datetime);
	
	post.tm_isdst=-1;
	seconds_post=mktime(&post);
	
	/*if(use_gmt)
		old_locale=setlocale(LC_TIME, "C");*/
	
	*age=difftime(seconds_local, seconds_post);
}/*
	datetime_age(&age, "Fri Nov  6 16:30:31 -0000 2009", TRUE|FALSE);
	datetime_age(&age, datetime, TRUE|FALSE);
*/


/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
