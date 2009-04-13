/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Alacast is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the RPL
 *
 * For more information or to find the latest release, visit our
 * website at: http://uberChicGeekChick.Com/?projects=Alacast
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
#include "config.h"
#include "timer.h"
#include "app.h"
#include "debug.h"

/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
typedef struct {
	gboolean active;
	GTimer *gtimer;
	gdouble limit;
	guint requests;
} RateLimitTimer;
static RateLimitTimer *timer=NULL;

#define DEBUG_DOMAIN "Timer"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/



/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/


void timer_init(void){
	debug(DEBUG_DOMAIN, "Initalizing network rate limit timer.");
	if(!g_thread_supported()) g_thread_init(NULL);
	
	if(timer) timer_deinit();
	
	/* timer->gtimer is used to avoid Twitter's rate limit. */
	timer=g_new0(RateLimitTimer, 1);
	timer->limit=10.0;
	timer->gtimer=g_timer_new();
	timer->active=TRUE;
	timer->requests=0;
	g_timer_start(timer->gtimer);
}//timer_init

void timer_main(SoupMessage *msg){
	gdouble time_elapsed=0.0;
	gulong request_microseconds=1;
	const char *rate_limit=NULL;
	int requests_remaining=99;
	
	if(!( (g_str_equal( "GET", msg->method)) && ((rate_limit=soup_message_headers_get_one( msg->response_headers, "X-RateLimit-Remaining" ))) &&(requests_remaining=atoi( rate_limit )) )){
		debug(DEBUG_DOMAIN, "Skipping network timer.\n\tRequest method: %s\n\tX-RateLimit-Remaininng: %s (%d)", msg->method, rate_limit, requests_remaining );
		return;
	}
	
	debug(DEBUG_DOMAIN, "Running network timer.\n\tRequest method: %s\n\tX-RateLimit-Remaininng: %s\n\tRequests left before the API's RateLimit is reached: %d.", msg->method, rate_limit, requests_remaining );
	
	timer->requests++;
	if(timer->requests==1)
		return;
	
	while( (time_elapsed=g_timer_elapsed(timer->gtimer, &request_microseconds)) < timer->limit ){
		//app_statusbar_printf( "To keep your service from locking you out %s limits your requsts 1 ever %u seconds.  You have %u seconds left to wait.", PACKAGE_NAME, timer->limit, (timer->limit - time_elapsed) );
	}
	g_timer_stop(timer->gtimer);
	
	g_timer_start(timer->gtimer);
	
	debug( DEBUG_DOMAIN, "Requests left before the API's RateLimit is reached: %d.", requests_remaining );
	if( requests_remaining < 11 )
		while( (g_timer_elapsed(timer->gtimer, &request_microseconds)) < 36.0 );
	else if( requests_remaining < 21 )
		while( (g_timer_elapsed(timer->gtimer, &request_microseconds)) < 24.0 );
	else if( requests_remaining < 41 )
		while( (g_timer_elapsed(timer->gtimer, &request_microseconds)) < 12.0 );
	else if( requests_remaining < 61 )
		while( (g_timer_elapsed(timer->gtimer, &request_microseconds)) < 6.0 );
	
	
	g_timer_stop(timer->gtimer);
	g_timer_start(timer->gtimer);
}//timer_main


void timer_main_quit(void){
	debug(DEBUG_DOMAIN, "Stopping network timer.");
	g_timer_stop(timer->gtimer);
	timer->active=FALSE;
}//timer_main_quit



void timer_deinit(void){
	timer_main_quit();
	debug(DEBUG_DOMAIN, "Shutting down network timer.");
	g_timer_destroy(timer->gtimer);
	g_free(timer);
}//timer_deinit



