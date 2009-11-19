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
#include "parser.h"
#include "gconfig.h"
#include "online-services.h"
#include "online-service.types.h"
#include "online-service.h"
#include "online-service-request.h"
#include "preferences.h"

#include "main-window.h"
#include "update-viewer.h"

#include "www.h"
#include "uberchick-label.h"

#define	DEBUG_DOMAINS	"UI:Sexy:URLs:URIs:Links:TimelinesSexyTreeView:OnlineServices:Networking:Updates:WWW:XPath:Auto-Magical:label.c"
#include "debug.h"

#define	GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_UBERCHICK_LABEL, UberChickLabelPrivate))

struct _UberChickLabelPrivate{
	OnlineService	*service;
	gdouble		update_id;
};


G_DEFINE_TYPE(UberChickLabel, uberchick_label, SEXY_TYPE_URL_LABEL);



static void uberchick_label_class_init(UberChickLabelClass *klass);
static void uberchick_label_init(UberChickLabel *uberchick_label);
static void uberchick_label_finalize(UberChickLabel *uberchick_label);



static void uberchick_label_class_init(UberChickLabelClass *klass){
	GObjectClass   *object_class=G_OBJECT_CLASS(klass);
	g_type_class_add_private(object_class, sizeof(UberChickLabelPrivate));
	object_class->finalize=(GObjectFinalizeFunc)uberchick_label_finalize;
}

static void uberchick_label_init(UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) )) return;
	GET_PRIVATE(uberchick_label)->update_id=0.0;
	GET_PRIVATE(uberchick_label)->service=NULL;
	
	gtk_label_set_line_wrap(GTK_LABEL(uberchick_label), TRUE);
	g_object_set(uberchick_label, "xalign", 0.0, "yalign", 0.0, "xpad", 0, "ypad", 0, NULL);
	g_signal_connect(uberchick_label, "url-activated", (GCallback)online_services_uri_clicked, NULL);
}/*uberchick_label_init(gobject);*/

UberChickLabel *uberchick_label_new(void){
	return g_object_new(TYPE_UBERCHICK_LABEL, NULL);
}/*uberchick_label_new();*/

static void uberchick_label_finalize(UberChickLabel *uberchick_label){
	GET_PRIVATE(uberchick_label)->update_id=0.0;
	G_OBJECT_CLASS(uberchick_label_parent_class)->finalize(G_OBJECT(uberchick_label));
}/*uberchick_label_finalize(update_viewer->update_label);*/

OnlineService *uberchick_label_get_service(UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) && GET_PRIVATE(uberchick_label)->service )) return NULL;
	return GET_PRIVATE(uberchick_label)->service;
}/*uberchick_label_get_service(widget);*/

gdouble uberchick_label_get_update_id(UberChickLabel *uberchick_label){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) && GET_PRIVATE(uberchick_label)->update_id )) return 0.0;
	return GET_PRIVATE(uberchick_label)->update_id;
}/*uberchick_label_get_service(widget);*/

void uberchick_label_set_text(UberChickLabel *uberchick_label, OnlineService *service, gdouble update_id, const gchar *text, gboolean expand_hyperlinks, gboolean make_hyperlinks){
	if(!( uberchick_label && IS_UBERCHICK_LABEL(uberchick_label) )) return;
	
	if(G_STR_EMPTY(text)){
		GET_PRIVATE(uberchick_label)->service=NULL;
		GET_PRIVATE(uberchick_label)->update_id=0.0;
		gtk_label_set_text(GTK_LABEL(uberchick_label), "");
		return;
	}
	
	GET_PRIVATE(uberchick_label)->service=service;
	GET_PRIVATE(uberchick_label)->update_id=update_id;
	debug("Rendering sexy markup for <%s>'s update's ID: %f; update's text: %s", service->key, update_id, text);
	gchar *sexy_text=www_format_urls(service, text, expand_hyperlinks, make_hyperlinks);
	sexy_url_label_set_markup(SEXY_URL_LABEL(uberchick_label), sexy_text);
	debug("Rendered sexy markup: %s", sexy_text);
	uber_free(sexy_text);
}/*uberchick_label_set_text*/

