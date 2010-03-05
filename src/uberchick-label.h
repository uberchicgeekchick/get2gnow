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


#ifndef __UBERCHICK_LABEL_H__
#define __UBERCHICK_LABEL_H__

#define _GNU_SOURCE
#define _THREAD_SAFE

#include <glib.h>
#include <gtk/gtk.h>
#include <glib-object.h>
#include <libsexy/sexy.h>
#include "online-services.h"

G_BEGIN_DECLS


#define UBERCHICK_TYPE_LABEL         (uberchick_label_get_type())
#define UBERCHICK_LABEL(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), UBERCHICK_TYPE_LABEL, UberChickLabel))
#define UBERCHICK_LABEL_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), UBERCHICK_TYPE_LABEL, UberChickLabelClass))
#define IS_UBERCHICK_LABEL(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), UBERCHICK_TYPE_LABEL))
#define IS_UBERCHICK_LABEL_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), UBERCHICK_TYPE_LABEL))
#define UBERCHICK_LABEL_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), UBERCHICK_TYPE_LABEL, UberChickLabelClass))

typedef struct _UberChickLabel		UberChickLabel;
typedef struct _UberChickLabelClass	UberChickLabelClass;
typedef struct _UberChickLabelPrivate	UberChickLabelPrivate;

struct _UberChickLabel {
	/*GtkLabel           parent;*/
	SexyUrlLabel           parent;
};

struct _UberChickLabelClass {
	/*GtkLabelClass      parent_class;*/
	SexyUrlLabelClass      parent_class;
};

GType uberchick_label_get_type(void) G_GNUC_CONST;



UberChickLabel *uberchick_label_new(void);
OnlineService *uberchick_label_get_service(UberChickLabel *uberchick_label);
const gchar *uberchick_label_get_user_name(UberChickLabel *uberchick_label);
const gchar *uberchick_label_get_markup(UberChickLabel *uberchick_label);
const gchar *uberchick_label_get_text(UberChickLabel *uberchick_label);
gdouble uberchick_label_get_user_id(UberChickLabel *uberchick_label);
UpdateType uberchick_label_get_update_type(UberChickLabel *uberchick_label);
gdouble uberchick_label_get_update_id(UberChickLabel *uberchick_label);

void uberchick_label_set_markup(UberChickLabel *uberchick_label, OnlineService *service, UpdateType update_type, const gchar *user_name, gdouble user_id, gdouble update_id, const gchar *text, gboolean expand_hyperlinks, gboolean make_hyperlinks);

G_END_DECLS
#endif /* __UBERCHICK_LABEL_H__ */

