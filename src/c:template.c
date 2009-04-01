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

/********************************************************
 *          My art, code, & programming.                *
 ********************************************************/

/********************************************************
 *        Project headers.                              *
 ********************************************************/
#include "config.h"
#include "new-object.h"

/********************************************************
 *         typedefs: objects, structures, and etc       *
 ********************************************************/
struct {
	gchar		*gtkbuilder_ui_file;
	GtkWindow	*dialog;
	GtkButton	*yes;
	GtkButton	*no;
} ThisObjectPriv;


/********************************************************
 *          Objects and handlers.                       *
 ********************************************************/
#define GET_PRIV(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_OF_THIS_OBJECT, ThisObjectPriv))

static ThisObject *this=NULL;

static void this_object_class_init( ThisObjectClass *goclass );
static void this_object_init( ThisObject *new_object );
static void this_object_finalize( GObject *object );


/********************************************************
 *          Method  & function prototypes               *
 ********************************************************/


ThisObject *this_object_class_new(void){
	return g_object_new(TYPE_OF_NEW_OBJECT, NULL);
}//new_object_class_new


static void this_object_class_init( ThisObjectClass *kclass ){
	GObjectClass *this=G_OBJECT_CLASS(golass);
	this->finalize=new_object_finalize;
	g_type_class_add_private(this, sizeof(ThisObjectPriv));
}//new_object_class_init

static void this_object_init(ThisObject *this_object){
	this=this_object;
	g_signal_connect(this, "size_allocate", G_CALLBACK(this_object_resize), this);
	g_signal_connect(this, "activated", G_CALLBACK(this_object_clicked), this);
}//new_class_init

static void this_object_create( GtkWindow *parent ){
	this=g_new0(ThisObject, 1);
	this->gtkbuilder_ui_file=g_strdup_printf( "%sthis-object.ui", PACKAGE_PREFIX );


}//void this_object_create

void this_object_show( GtkWindow *parent ){
	if(!this) this_object_create( parent );
	
	gtk_widget_show( GET_PRIV(this)->window );
}//void this_object_show

static void this_object_finalize( GObject *object ){
	ThisObjectPrivate *private=GET_PRIV(object);
	G_OBJECT_CLASS(private_parent_class)->finalize(object);
}//this_object_finalize

