/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Greet-Tweet-Know is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the RPL
 *
 * For more information or to find the latest release, visit our
 * website at: http://uberChicGeekChick.Com/?projects=Greet-Tweet-Know
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


#include <strings.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "debug.h"
#include "friends-manager.h"
#include "users.h"
#include "gtkbuilder.h"
#include "network.h"
#include "images.h"
#include "parser.h"

static void users_free_foreach( gpointer user, gpointer data );

#define GtkBuilderUI "user-profile.ui"

#define DEBUG_DOMAIN "Users"


/* Parse a xml user node. Ex: user's profile & add/del/block users responses */
User *user_parse_new( const gchar *data, gssize length ){
	xmlDoc *doc=NULL;
	xmlNode *root_element=NULL;
	User *user=NULL;
	
	/* parse the xml */
	if(!( (doc=parser_parse(data, length, &root_element )) )){
		xmlCleanupParser();
		return NULL;
	}
	
	if(g_str_equal(root_element->name, "user"))
		user=user_new(root_element->children);
	
	/* Free memory */
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return user;
}


User *user_new(xmlNode *a_node){
	xmlNode		   *cur_node = NULL;
	xmlBufferPtr	buffer;
	User     *user;
	
	buffer = xmlBufferCreate();
	user = g_new0(User, 1);
	
	user->follower=getting_followers;
	
	/* Begin 'users' node loop */
	for(cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if(cur_node->type != XML_ELEMENT_NODE)
			continue;
		if(xmlNodeBufGetContent(buffer, cur_node) != 0)
			continue;
		
		const xmlChar *tmp=xmlBufferContent(buffer);
		
		if(g_str_equal(cur_node->name, "id" ))
			user->id=strtoul( ((const char *)g_strdup((const gchar *)tmp)), NULL, 0 );
		
		else if(g_str_equal(cur_node->name, "name" ))
			user->nick_name=g_strdup((const gchar *)tmp);
		
		else if(g_str_equal(cur_node->name, "screen_name" ))
			user->user_name=g_strdup((const gchar *)tmp);
		
		else if(g_str_equal(cur_node->name, "location" ))
			user->location=g_strdup((const gchar *)tmp);
		
		else if(g_str_equal(cur_node->name, "description" ))
			user->bio=g_markup_printf_escaped( "%s", (const gchar *)tmp );
		
		else if(g_str_equal(cur_node->name, "url" ))
			user->url=g_strdup((const gchar *)tmp);
		
		else if(g_str_equal(cur_node->name, "followers_count" ))
			user->followers=strtoul( ((const char *)g_strdup((const gchar *)tmp)), NULL, 0 );
		
		else if(g_str_equal(cur_node->name, "friends_count" ))
			user->friends=strtoul( ((const char *)g_strdup((const gchar *)tmp)), NULL, 0 );
		
		else if(g_str_equal(cur_node->name, "statuses_count" ))
			user->tweets=strtoul( ((const char *)g_strdup((const gchar *)tmp)), NULL, 0 );
		
		else if(g_str_equal(cur_node->name, "profile_image_url"))
			user->image_filename=images_get_filename( (user->image_url=g_strdup((const gchar *)tmp)) );
		
		/* Free buffer content */
		xmlBufferEmpty(buffer);
		
	} /* End of loop */
	
	/* Free buffer pointer */
	xmlBufferFree(buffer);
	
	return user;
}


int user_sort(User *a, User *b){
	return strcasecmp( (const char *)a->user_name, (const char *)b->user_name );
}


/* Parse a user-list XML( friends, followers,... ) */
GList *users_new(const gchar *data, gssize length){
	xmlDoc		*doc = NULL;
	xmlNode		*root_element = NULL;
	xmlNode		*cur_node = NULL;
	
	GList		*friends = NULL;
	
	User 	*user;
	
	/* parse the xml */
	if(!( (doc=parser_parse(data, length, &root_element)) )){
		xmlCleanupParser();
		return NULL;
	}
	
	/* get users */
	for(cur_node = root_element; cur_node; cur_node = cur_node->next) {
		if(cur_node->type != XML_ELEMENT_NODE)
			continue;
		if(g_str_equal(cur_node->name, "user")){
			/* parse user */
			user=user_new(cur_node->children);
			/* add to list */
			friends=g_list_append(friends, user);
		} else if(g_str_equal(cur_node->name, "users")){
			cur_node = cur_node->children;
		}
	} /* End of loop */
	
	/* Free memory */
	xmlFreeDoc(doc);
	xmlCleanupParser();

	return friends;
}


void users_free( GList *users ){
	g_list_foreach(users, users_free_foreach, NULL );
	g_list_free(users);
	users=NULL;
	debug( DEBUG_DOMAIN, "Friends freed" );
}//user_free_glist



/* Callback to free every element on a User list */
static void users_free_foreach( gpointer user, gpointer data ){
	if(!user) return;
	user_free( (User *)user );
}

/* Free a user struct */
void user_free(User *user){
	if(!user)
		return;

	if(user->user_name) g_free(user->user_name);
	
	if(user->nick_name) g_free(user->nick_name);
	
	if(user->location) g_free(user->location);
	
	if(user->bio) g_free(user->bio);
	
	if(user->url) g_free(user->url);
	
	if(user->image_url) g_free(user->image_url);
	
	if(user->image_filename) g_free(user->image_filename);
	
	g_free(user);
}//user_free



void user_popup_profile( gchar *user_name ){
	GtkImage		*image;
	GtkMessageDialog	*profile_dialog;
	
	GtkBuilder *ui=gtkbuilder_get_file(
					GtkBuilderUI,
					"profile_dialog", &profile_dialog,
					NULL
	);
	g_signal_connect( profile_dialog, "response", G_CALLBACK(gtk_widget_destroy), profile_dialog );
			
	User *profile=network_fetch_profile(user_name);
	gchar *message=g_strdup_printf(
					"<u><b>%s:</b> %s</u>\n\t<b>Tweets:</b> %lu\n\t<b>Friends:</b> %lu\n\t<b>Followers:</b> %lu\n\t<b>Location:</b> %s\n\t<b>URL:</b>%s\n\t<b>Bio:</b>\n\t\t%s\n",
					profile->user_name, profile->nick_name,
					profile->tweets,
					profile->friends,
					profile->followers,
					profile->location,
					profile->url,
					profile->bio
	);

	gtk_message_dialog_set_markup( profile_dialog, message );
	
	network_download_avatar( profile->image_url );
	if(!(g_str_equal("unknown_image", profile->image_filename)))
		gtk_message_dialog_set_image( profile_dialog, GTK_WIDGET(image=images_get_maximized_image_from_filename( profile->image_filename )) );
	
	
	gtk_widget_show_all( GTK_WIDGET( profile_dialog ) );
	
	
	//if( image ) g_object_unref( image );
	g_free( message );
	user_free( profile );
}//user_popup_profile

