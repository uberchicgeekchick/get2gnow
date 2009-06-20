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
 *        Project headers.                              *
 ********************************************************/
#include <stdlib.h>
#include "config.h"
#include "main.h"

#include "timer.h"
#include "main-window.h"

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define DEBUG_DOMAINS "Networking:RateLimitTimer:Requests:Setup:Start-Up:Threads:Headers:Timer.c"
#include "debug.h"

struct _RateLimitTimer{
	gboolean	active;
	GTimer		*gtimer;
	guint		processing;
	gint		requests_remaining;
	gdouble		elapsed;
	gdouble		pause;
};



/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static gboolean timer_check(RateLimitTimer *timer, SoupMessage *msg);
static void timer_process(RateLimitTimer *timer);
static void timer_main_quit(RateLimitTimer *timer);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
RateLimitTimer *timer_new(void){
	debug("Initalizing network rate limit timer.");
	if(!( g_thread_get_initialized() && g_thread_supported() )) g_thread_init(NULL);
	
	/* timer->gtimer is used to avoid Twitter's rate limit. */
	RateLimitTimer *timer=g_new0(RateLimitTimer, 1);
	timer->elapsed=timer->pause=0.0;
	timer->gtimer=g_timer_new();
	timer->active=0;
	timer->processing=FALSE;
	timer->requests_remaining=99;
	g_timer_start(timer->gtimer);
	return timer;
}//timer_new

void timer_main(RateLimitTimer *timer, SoupMessage *msg){
	debug("Processing RateLimit.");
	if(!timer_check(timer, msg)) return;
	
	while(timer->processing){}
	
	timer->pause=0.0;
	
	timer_process(timer);
	
	debug("RateLimitTimer:\t\t\t[done]");
	g_timer_stop(timer->gtimer);
	g_timer_start(timer->gtimer);
}/*timer_main(timer);*/

static gboolean timer_check(RateLimitTimer *timer, SoupMessage *msg){
	if(!g_str_equal("GET", msg->method)){
		debug("RateLimitTimer is skipped for POST requests.");
		return FALSE;
	}
	const char *rate_limit=NULL;
	if(!( rate_limit=soup_message_headers_get_one(msg->response_headers, "X-RateLimit-Remaining") )){
		debug("RateLimitTimer does not need to process this request.  X-RateLimit-Remaining header was not received.");
		return FALSE;
	}
	timer->requests_remaining=atoi(rate_limit);
	
	debug("Running RateLimitTimer.");
	debug("Details: X-RateLimit-Remaining: %s(=%d).", rate_limit, timer->requests_remaining);
	
	return TRUE;
}/*timer_check(timer, msg);*/

static void timer_process(RateLimitTimer *timer){
	gulong request_microseconds=1;
	
	if( timer->requests_remaining < 11 ) timer->pause = 36.0;
	else if( timer->requests_remaining < 21 ) timer->pause = 24.0;
	else if( timer->requests_remaining < 41 ) timer->pause = 12.0;
	else if( timer->requests_remaining < 61 ) timer->pause = 6.0;
	else return;
	
	if( (timer->elapsed=g_timer_elapsed(timer->gtimer, &request_microseconds)) > timer->pause )
		return;
	
	g_timer_stop(timer->gtimer);
	g_timer_start(timer->gtimer);
	
	timer->processing++;
	
	main_window_statusbar_printf("Please wait %i seconds, you have %d before being locked out so pauses %s requests.", (gint)timer->pause, timer->requests_remaining, _(GETTEXT_PACKAGE));
	while( (timer->elapsed=g_timer_elapsed(timer->gtimer, &request_microseconds)) < timer->pause )
		main_window_statusbar_printf("Please wait %i seconds, you have %d before being locked out so pauses %s requests.", (gint)timer->pause, timer->requests_remaining, _(GETTEXT_PACKAGE));
	timer->processing--;
}/*timer_process(timer);*/


static void timer_main_quit(RateLimitTimer *timer){
	debug("Stopping network timer.");
	g_timer_stop(timer->gtimer);
	timer->active=FALSE;
}//timer_main_qu1it



void timer_free(RateLimitTimer *timer){
	if(!timer) return;
	timer_main_quit(timer);
	debug("Shutting down network timer.");
	g_timer_destroy(timer->gtimer);
	uber_free(timer);
}//timer_deinit



