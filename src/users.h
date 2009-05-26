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
#ifndef __USER_H__
#define __USER_H__


/**********************************************************************
 *        System & library headers, eg #include <gdk/gdkkeysyms.h>    *
 **********************************************************************/
#include <gtk/gtk.h>
#include <libxml/parser.h>
#include <libsoup/soup.h>
#include "online-services.h"


/**********************************************************************
 *        Objects, structures, and etc typedefs                       *
 **********************************************************************/
typedef void (*UsersGListLoadFunc) (GList *users);

typedef struct _User User;
typedef struct _UserStatus UserStatus;
typedef struct _UserRequest UserRequest;

typedef enum _UsersGListGetWhich UsersGListGetWhich;
typedef enum _UserAction UserAction;

#define usrcasecmp	user_sort_by_user_name
#define	usrcmp		user_sort_by_user_name


enum _UserAction{
	SelectService,
	ViewProfile,
	ViewTweets,
	Follow,
	UnFollow,
	Block,
	UnBlock,
	Fave,
	UnFave,
};

enum _UsersGListGetWhich{
	GetFriends,
	GetFollowers,
	GetBoth,
};


struct _UserStatus {
	OnlineService	*service;
	
	User		*user;
	guint		id;
	guint		in_reply_to_status_id;
	
	gchar		*text;
	gchar		*tweet;
	gchar		*sexy_tweet;
	
	gchar		*source;
	
	gchar		*created_at_str;
	gchar		*created_how_long_ago;
	
	guint		created_at;
	guint		created_seconds_ago;
};

struct _User {
	OnlineService		*service;
	
	unsigned long int	id;
	gchar			*user_name;
	gchar			*nick_name;
	
	UserStatus		*status;
	
	gchar			*location;
	gchar			*bio;
	gchar			*url;
	
	gchar			*image_url;
	gchar			*image_filename;
	
	unsigned long int	tweets;
	unsigned long int	following;
	unsigned long int	followers;
	
	gboolean		follower;
};

struct _UserRequest{
	UserAction action;
	RequestMethod method;
	GtkWindow *parent;
	gchar *user_data;
	gchar *message;
	gchar *uri;
};


/**********************************************************************
 *          Global method & function prototypes                      *
 **********************************************************************/
gchar *user_action_to_string(UserAction action);
void user_request_main(OnlineService *service, UserAction action, GtkWindow *parent, const gchar *user_data);
void user_request_main_quit(SoupSession *session, SoupMessage *msg, gpointer user_data);

UserStatus *user_status_new(OnlineService *service, xmlNode *a_node);
void user_status_format_tweet(UserStatus *status, User *user);
void user_status_free(UserStatus *status);

/* Parse a user-list XML( friends, followers,... ) */
GList *users_new(OnlineService *service, SoupMessage *xml);
void users_free(const char *type, GList *users );

/* Get authenticated user's friends(following) */
GList *user_get_friends(gboolean refresh, UsersGListLoadFunc func);
void user_append_friend(User *user);
void user_remove_friend(User *user);


/* Get authenticated user's followers */
GList *user_get_followers(gboolean refresh, UsersGListLoadFunc func);
void user_append_follower(User *user);
void user_remove_follower(User *user);

/* Get authenticated user's friends(following) and followers in a combined & sorted list */
GList *user_get_friends_and_followers(gboolean refresh, UsersGListLoadFunc func);
/* releases all memory & etc associated w/all three list generated by the above methods */
void user_free_lists(OnlineService *service);

/* Parse a xml user node. Ex: add/del users responses */
User *user_parse_new(OnlineService *service, SoupMessage *xml);
User *user_parse_profile(OnlineService *service, xmlNode *a_node);
User *user_fetch_profile(OnlineService *service, const gchar *user_name);
int user_sort_by_user_name(User *a, User *b);
void user_free(User *user);



#endif /*__USER_H__*/
/**********************************************************************
 *                               eof                                  *
 **********************************************************************/

