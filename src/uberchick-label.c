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

#define _GNU_SOURCE
#define _THREAD_SAFE


#define _GNU_SOURCE
#define _THREAD_SAFE

#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <libsexy/sexy.h>
#include <libxml/parser.h>

#include "config.h"
#include "program.h"

#include "network.h"
#include "gconfig.h"
#include "update.types.h"
#include "online-services.typedefs.h"
#include "online-services.types.h"
#include "online-services.h"
#include "online-service.types.h"
#include "online-service.h"
#include "online-service-request.h"
#include "preferences.defines.h"

#include "main-window.h"
#include "update-viewer.h"

#include "www.h"

#include "uberchick-label.h"

#define	DEBUG_DOMAINS	"UI:Sexy:URLs:URIs:Links:UberChickTreeView:OnlineServices:Networking:Updates:WWW:XPath:Auto-Magical:label.c"
#include "debug.h"

#define	GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), UBERCHICK_TYPE_LABEL, UberChickLabelPrivate))

struct _UberChickLabel {
#ifndef ENABLE_ALPHA_UI
	SexyUrlLabel		parent;
#else
	GtkLabel		parent;
#endif
};

struct _UberChickLabelClass {
#ifndef ENABLE_ALPHA_UI
	SexyUrlLabelClass	parent_class;
#else
	GtkLabelClass		parent_class;
#endif
};

struct _UberChickLabelPrivate{
	OnlineService	*service;
	UpdateType	update_type;
	
	gchar		*user_name;
	gdouble		user_id;
	gdouble		update_id;
	gchar		*text;
	gchar		*markup;
};


#ifndef ENABLE_ALPHA_UI
	G_DEFINE_TYPE(UberChickLabel, uberchick_label, SEXY_TYPE_URL_LABEL);
#else
	G_DEFINE_TYPE(UberChickLabel, uberchick_label, GTK_TYPE_LABEL);
#endif



static void uberchick_label_class_init(UberChickLabelClass *klass);
static void uberchick_label_init(UberChickLabel *uberchick_label);
static void uberchick_label_finalize(UberChickLabel *uberchick_label);

static void uberchick_label_populate_popup(UberChickLabel *uberchick_label, GtkMenu *menu);
static void uberchick_label_shell_menu_add_action(UberChickLabel *uberchick_label, GtkMenu *menu, const gchar *name_of_action, const gchar *gtk_stock_icon_name, const gchar *tooltip_string, gchar *requested_resource, GCallback callback, gboolean prepend, gboolean add_separator);
static void uberchick_label_shell_menu_add_separator(UberChickLabel *uberchick_label, GtkMenu *menu, gboolean prepend);

static void uberchick_label_forward_text(GtkAction *action, UberChickLabel *uberchick_label);
static void uberchick_label_send_at_reply(GtkAction *action, UberChickLabel *uberchick_label);
static void uberchick_label_search(GtkAction *action, UberChickLabel *uberchick_label);



static void uberchick_label_class_init(UberChickLabelClass *klass){
	GObjectClass   *object_class=G_OBJECT_CLASS(klass);
	g_type_class_add_private(object_class, sizeof(UberChickLabelPrivate));
	object_class->finalize=(GObjectFinalizeFunc)uberchick_label_finalize;
}

static void uberchick_label_init(UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) )) return;
	UberChickLabelPrivate *this=GET_PRIVATE(uberchick_label);
	
	this->service=NULL;
	this->update_type=None;
	this->user_name=NULL;
	this->user_id=0.0;
	this->update_id=0.0;
	this->text=this->markup=NULL;
	
	g_object_set(uberchick_label, "xalign", 0.0, "yalign", 0.0, "xpad", 0, "ypad", 0, "wrap", TRUE, "wrap-mode", PANGO_WRAP_WORD_CHAR, "single-line-mode", FALSE, "use-markup", TRUE, "justify", GTK_JUSTIFY_LEFT, NULL);
#ifndef ENABLE_ALPHA_UI
	g_signal_connect(SEXY_URL_LABEL(uberchick_label), "url-activated", (GCallback)online_service_open_uri, this->service);
#else
	g_object_set(uberchick_label, "selectable", TRUE, NULL);
	g_signal_connect(GTK_LABEL(uberchick_label), "activate-link", (GCallback)online_service_open_uri, this->service);
	g_signal_connect(GTK_LABEL(uberchick_label), "populate-popup", (GCallback)uberchick_label_populate_popup, NULL);
#endif
}/*uberchick_label_init(gobject);*/

UberChickLabel *uberchick_label_new(void){
	return g_object_new(UBERCHICK_TYPE_LABEL, NULL);
}/*uberchick_label_new();*/

static void uberchick_label_finalize(UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) )) return;
	UberChickLabelPrivate *this=GET_PRIVATE(uberchick_label);
	
	if(this->text)
		uber_free(this->text);
	
	if(this->markup)
		uber_free(this->markup);
	
	if(this->user_name)
		uber_free(this->user_name);
	
	G_OBJECT_CLASS(uberchick_label_parent_class)->finalize(G_OBJECT(uberchick_label));
}/*uberchick_label_finalize(update_viewer->update_label);*/

OnlineService *uberchick_label_get_service(UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) && GET_PRIVATE(uberchick_label)->service )) return NULL;
	return GET_PRIVATE(uberchick_label)->service;
}/*uberchick_label_get_service(uberchick_label);*/

const gchar *uberchick_label_get_user_name(UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) && GET_PRIVATE(uberchick_label)->user_name )) return NULL;
	return GET_PRIVATE(uberchick_label)->user_name;
}/*uberchick_label_get_user_name(uberchick_label);*/

const gchar *uberchick_label_get_markup(UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) && GET_PRIVATE(uberchick_label)->markup )) return NULL;
	return GET_PRIVATE(uberchick_label)->markup;
}/*uberchick_label_get_markup(uberchick_label);*/

const gchar *uberchick_label_get_text(UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) && GET_PRIVATE(uberchick_label)->text )) return NULL;
	return GET_PRIVATE(uberchick_label)->text;
}/*uberchick_label_get_text(uberchick_label);*/

gdouble uberchick_label_get_user_id(UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) && GET_PRIVATE(uberchick_label)->user_id )) return 0.0;
	return GET_PRIVATE(uberchick_label)->user_id;
}/*uberchick_label_get_user_id(uberchick_label);*/

UpdateType uberchick_label_get_update_type(UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) && GET_PRIVATE(uberchick_label)->update_type )) return None;
	return GET_PRIVATE(uberchick_label)->update_type;
}/*uberchick_label_get_update_type(uberchick_label);*/

gdouble uberchick_label_get_update_id(UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) && GET_PRIVATE(uberchick_label)->update_id )) return 0.0;
	return GET_PRIVATE(uberchick_label)->update_id;
}/*uberchick_label_get_update_id(uberchick_label);*/

void uberchick_label_set_markup(UberChickLabel *uberchick_label, OnlineService *service, UpdateType update_type, const gchar *user_name, gdouble user_id, gdouble update_id, const gchar *text, gboolean expand_hyperlinks, gboolean make_hyperlinks){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) )) return;
	UberChickLabelPrivate *this=GET_PRIVATE(uberchick_label);
	
	if(this->update_type!=update_type)
		this->update_type=update_type;
	
	if(this->text)
		uber_free(this->text);
	
	if(this->markup)
		uber_free(this->markup);
	
	if(this->user_name)
		uber_free(this->user_name);
	
	if(this->service!=service)
		this->service=service;
	
	if(G_STR_EMPTY(text)){
		this->user_id=0.0;
		this->update_id=0.0;
		gtk_label_set_text(GTK_LABEL(uberchick_label), "");
		return;
	}
	
	if(G_STR_N_EMPTY(user_name))
		this->user_name=g_strdup(user_name);
	
	this->user_id=user_id;
	this->update_id=update_id;
	debug("UberChickLabel: Rendering markup for <%s>'s update's ID: %f; update's text: %s", service->key, update_id, text);
	this->text=g_strdup(text);
	this->markup=www_format_urls(service, text, expand_hyperlinks, make_hyperlinks);
#ifndef ENABLE_ALPHA_UI
	sexy_url_label_set_markup(SEXY_URL_LABEL(uberchick_label), this->markup);
#else
	gtk_label_set_markup(GTK_LABEL(uberchick_label), this->markup);
#endif
	debug("UberChickLabel: Rendered markup: [%s]", this->markup);
}/*uberchick_label_set_markup(uberchick_label, service, update_type, user_name, user_id, id, markup, TRUE|FALSE, TRUE|FALSE);*/


static void uberchick_label_populate_popup(UberChickLabel *uberchick_label, GtkMenu *menu){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) )) return;
	UberChickLabelPrivate *this=GET_PRIVATE(uberchick_label);
	
	const gchar *uri=gtk_label_get_current_uri(GTK_LABEL(uberchick_label));
	if(!(this->service && G_STR_N_EMPTY(this->text) && G_STR_N_EMPTY(uri))) return;
	
	gchar *tooltip_string=NULL;
	
	uberchick_label_shell_menu_add_action(uberchick_label, menu, "uberchick_label_context_menu_forward_text_action", "gtk-media-next", "Forward/ReTweet this update", NULL, (GCallback)uberchick_label_forward_text, TRUE, TRUE);
	
	tooltip_string=g_strdup_printf("@ reply to @%s on <%s>", this->user_name, this->service->uri);
	uberchick_label_shell_menu_add_action(uberchick_label, menu, "uberchick_label_context_menu_at_reply_action", "gtk-undo", tooltip_string, g_strdup(this->user_name), (GCallback)uberchick_label_send_at_reply, TRUE, FALSE);
	uber_free(tooltip_string);
	
	uberchick_label_shell_menu_add_action(uberchick_label, menu, "uberchick_label_context_menu_new_update_action", "gtk-new", "New update", NULL, (GCallback)update_viewer_new_update, TRUE, TRUE);
	
	gchar *requested_resource=NULL;
	if(!(requested_resource=online_service_get_uri_requested_resource(this->service, uri)))
		return;
	
	switch(requested_resource[0]){
		case '%':
			debug("UberChickLabel found %s search link", requested_resource);
			break;
		
		case '@':
			if(g_str_equal(this->user_name, &requested_resource[1])) break;
			
			debug("UberChickLabel found <%s>'s user name on <%s>", requested_resource, this->service->key);
			tooltip_string=g_strdup_printf("@ reply to %s on <%s>", requested_resource, this->service->uri);
			uberchick_label_shell_menu_add_action(uberchick_label, menu, "uberchick_label_context_menu_reply_non_author_action", "gtk-undo", tooltip_string, g_strdup(requested_resource), (GCallback)uberchick_label_send_at_reply, FALSE, TRUE);
			break;
		
		case '#':
		case '!':
			debug("UberChickLabel found search link: for <%s> on <%s>", requested_resource, this->service->key);
			tooltip_string=g_strdup_printf("Search for %s on <%s>", requested_resource, this->service->uri);
			uberchick_label_shell_menu_add_action(uberchick_label, menu, "uberchick_label_context_menu_search_action", "gtk-media-forward", tooltip_string, g_strdup(requested_resource), (GCallback)uberchick_label_search, FALSE, TRUE);
			break;
		
		default:
			debug("UberChickLabel found %s requested_resource", requested_resource);
			break;
	}
	
	if(tooltip_string)
		uber_free(tooltip_string);
	
	uber_free(requested_resource);
}/*uberchick_label_populate_popup(uberchick_label, menu);*/

static void uberchick_label_shell_menu_add_action(UberChickLabel *uberchick_label, GtkMenu *menu, const gchar *name_of_action, const gchar *gtk_stock_icon_name, const gchar *tooltip_string, gchar *requested_resource, GCallback callback, gboolean prepend, gboolean add_separator){
	GtkWidget *w=NULL;
	GtkAction *action=NULL;
	
	if(add_separator)
		uberchick_label_shell_menu_add_separator(uberchick_label, menu, prepend);
	
	gchar *label_string=g_strdup_printf("_%s", tooltip_string);
	action=gtk_action_new(name_of_action, label_string, tooltip_string, gtk_stock_icon_name);
	if(requested_resource)
		g_object_set_data_full(G_OBJECT(action), "requested_resource", requested_resource, g_free);
	g_signal_connect(action, "activate", callback, uberchick_label);
	w=gtk_action_create_menu_item(action);
	
	if(!prepend)
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), w);
	else
		gtk_menu_shell_prepend(GTK_MENU_SHELL(menu), w);
	gtk_widget_show(w);
}/*uberchick_label_shell_menu_add_action(uberchick_label, menu, "uberchick_label_context_menu_name_action", "gtk-", g_strdup(requested_resource), TRUE|FALSE, TRUE|FALSE);*/

static void uberchick_label_shell_menu_add_separator(UberChickLabel *uberchick_label, GtkMenu *menu, gboolean prepend){
	GtkWidget *w=gtk_separator_menu_item_new();
	if(!prepend)
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), w);
	else
		gtk_menu_shell_prepend(GTK_MENU_SHELL(menu), w);
	gtk_widget_show(w);
}/*uberchick_label_shell_menu_add_separator(uberchick_label, menu, TRUE|FALSE);*/

static void uberchick_label_forward_text(GtkAction *action, UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) )) return;
	UberChickLabelPrivate *this=GET_PRIVATE(uberchick_label);
	
	debug("UberChick_Label: Inserting: <%s@%s> in to current update", this->user_name, this->service->uri);
	update_viewer_set_in_reply_to_data(this->service, this->update_type, this->user_name, this->text, this->user_id, this->update_id, TRUE, TRUE, FALSE);
}/*uberchick_label_forward_text(action, user_name);*/

static void uberchick_label_send_at_reply(GtkAction *action, UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) )) return;
	UberChickLabelPrivate *this=GET_PRIVATE(uberchick_label);
	
	gchar *requested_resource=(gchar *)g_object_get_data(G_OBJECT(action), "requested_resource");
	
	debug("UberChick_Label: Inserting: <%s@%s> in to current update", requested_resource, this->service->uri);
	update_viewer_set_in_reply_to_data(this->service, this->update_type, requested_resource, this->text, this->user_id, this->update_id, TRUE, FALSE, FALSE);
}/*uberchick_label_send_at_reply(action, user_name);*/

static void uberchick_label_search(GtkAction *action, UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) )) return;
	UberChickLabelPrivate *this=GET_PRIVATE(uberchick_label);
	
	gchar *requested_resource=(gchar *)g_object_get_data(G_OBJECT(action), "requested_resource");
	
	debug("OnlineServices: Searching for <%s> on <%s>", requested_resource, this->service->key);
	main_window_sexy_search_entry_set(requested_resource, TRUE);
}/*uberchick_label_search(action, user_name);*/

