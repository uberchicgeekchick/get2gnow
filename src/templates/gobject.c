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

#include "config.h"
#include "this.h"


typedef struct {
	GtkWindow	*window;
} ThisPrivate;

#define	GET_PRIV(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_THIS, ThisPrivate))

#define DEBUG_DOMAINS "This:UI:GtkBuilder:GtkBuildable:Objects:Networking:Requests:Authentication:Setup:Start-Up"
#define GtkBuilderUI "this.ui"

/* GObject methods */
static void this_class_init(ThisClass *klass);
static GObject *this_constructor(GType type, guint n_construct_params, GObjectConstructParam *construct_params){
static void this_init(This *singleton_this);
static void this_constructed(GOject *object);
static void this_finalize(GObject *object);

static This *this=NULL;

G_DEFINE_TYPE(This, this, G_TYPE_OBJECT);
/* G_DEFINE_TYPE's third agument needs to be 'This' parent's type as defined in "this.h". */
/* Commonly Used:
 *	G_TYPE_OBJECT
 *	GTK_TYPE_OBJECT
 *	GTK_TYPE_WIDGET
 *	GTK_TYPE_CONTAINER
 *	GTK_TYPE_BIN
 *	GTK_TYPE_WINDOW
 *	GTK_TYPE_ITEM
 *	SEXY_TYPE_URL_LABEL
*/


static void this_class_init(ThisClass *klass){
	GObjectClass  *object_class=G_OBJECT_CLASS(klass);
	object_class->finalize=this_finalize;
	g_type_class_add_private(object_class, sizeof(ThisPrivate));
}

static GObject *this_constructor(GType type, guint n_construct_params, GObjectConstructParam *construct_params){
	GObject *object;
	
	if(this)
	          object=g_object_ref(G_OBJECT(this));
	else{
		object=G_OBJECT_CLASS(parent_class)->constructor( type, n_construct_params, construct_params);
		this=THIS(object);
	}
}

static void this_init(This *singleton_this){
	this=singleton_this;
}

static void this_constructed(GOject *object){
	/* Add signal hanlers & etc. */
	G_OBJECT_CLASS(parent_class)->constructed(object);
}

static void this_dispose(GObject *object){
	G_OBJECT_CLASS(parent_class)->dispose(object);
}

static void this_finalize(GObject *object){
	G_OBJECT_CLASS(parent_class)->finalize(object);
}

static void this_response(GtkWidget *widget, gint response){
	gtk_widget_destroy(widget);
}//friends_message_response

static void this_destroy(GtkWidget *widget){
	g_object_unref(this);
	this=NULL;
}//this_destroy

This *this_new(void){
	return g_object_new(TYPE_THIS, NULL);
}//this_new

This *this_new_from(const gchar *first_property, ...){
	if(!first_property)
		return g_object_new(TYPE_THIS, NULL);
	
	va_list properties;
	va_start(first_property, properties);
	GObject *this=gobject_new(first_property, properties);
	va_end(properties);
	return this;
}//this_new_from

