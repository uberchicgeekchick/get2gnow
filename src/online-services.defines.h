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
#ifndef	__ONLINE_SERVICE_DEFINES_H__
#define	__ONLINE_SERVICE_DEFINES_H__

G_BEGIN_DECLS
/********************************************************************************
 *                        defines, macros, methods, & etc                       *
 ********************************************************************************/
#define API_LOGIN				"/account/verify_credentials.xml"

#define API_DIRECT_MESSAGES			"/direct_messages.xml"
#define API_REPLIES				"/statuses/replies.xml"
#define API_MENTIONS				"/statuses/mentions.xml"
#define API_TIMELINE_FRIENDS			"/statuses/friends_timeline.xml"
#define API_FAVORITES				"/favorites.xml"


#define API_USER_PROFILE			"/users/show/%s.xml"
#define API_TIMELINE_MINE			"/statuses/user_timeline.xml"
#define API_TIMELINE_USER			"/statuses/user_timeline/%s.xml"
#define API_TIMELINE_USER_UNREAD		API_TIMELINE_USER "?since_id=%f"

#define API_FORWARDS_BY_ID			"/statuses/retweets/%s.xml"

#define API_TIMELINE_SEARCH			"/search.atom?q="
#define API_TIMELINE_SEARCH_SUBMIT		"/search.atom?q=%s"

#define API_TIMELINE_GROUP			"/statuses/groups.xml"
#define API_TIMELINE_PUBLIC			"/statuses/public_timeline.xml"

#define API_POST_STATUS				"/statuses/update.xml"
#define API_SEND_MESSAGE			"/direct_messages/new.xml"
#define API_RETWEET_UPDATE			"/statuses/retweet/%s.xml"
/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
G_END_DECLS
#endif /*__ONLINE_SERVICE_DEFINES_H__*/
