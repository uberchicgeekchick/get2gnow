/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright(c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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
 * License("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
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
 * user-visible display as defined in Section 6.4(d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright(C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright(C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright(C) of the User the data is connected to.
 * Users may lisences their data under the terms of an OSI approved or Creative Commons
 * license.  Users must be allowed to select their choice of license for each piece of data
 * on an individual bases and cannot be blanketly applied to all of the Users.  The User may
 * select a default license for their data.  All of the Software's data pertaining to each
 * User must be fully accessible, exportable, and deletable to that User.
 */

#ifndef __GCONFIG_H__
#define __GCONFIG_H__

#include <glib-object.h>
#include <gconf/gconf-client.h>

G_BEGIN_DECLS

#define TYPE_GCONFIG		(gconfig_get_type())
#define GCONFIG(o)		(G_TYPE_CHECK_INSTANCE_CAST((o), TYPE_GCONFIG, GConfig))
#define GCONFIG_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), TYPE_GCONFIG, GConfigClass))
#define IS_GCONFIG(o)		(G_TYPE_CHECK_INSTANCE_TYPE((o), TYPE_GCONFIG))
#define IS_GCONFIG_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE((k), TYPE_GCONFIG))
#define GCONFIG_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS((o), TYPE_GCONFIG, GConfClass))


typedef struct{
	GObject parent;
} GConfig;

typedef struct{
	GObjectClass parent_class;
} GConfigClass;

typedef void (*GConfigNotifyFunc) (const gchar *key, gpointer user_data);

GType gconfig_get_type(void) G_GNUC_CONST;
void gconfig_start(void);
void gconfig_shutdown(void);
guint gconfig_notify_add(const gchar *key, GConfigNotifyFunc func, gpointer user_data);
gboolean gconfig_notify_remove(guint                  id);
gboolean    gconfig_set_int(const gchar           *key,
										 gint                   value);
gboolean    gconfig_get_int(const gchar           *key,
										 gint                  *value);
gboolean    gconfig_if_bool(const gchar *key, gboolean default_bool);
gboolean    gconfig_set_bool(const gchar           *key, gboolean               value);
gboolean    gconfig_get_bool(const gchar           *key, gboolean              *value);
gboolean    gconfig_set_string(const gchar           *key,
										 const gchar           *value);
gboolean    gconfig_get_string(const gchar           *key,
										 gchar                **value);
gboolean gconfig_set_list_string(const gchar *key, GSList *value);
gboolean gconfig_get_list_string(const gchar *key, GSList **value);
/* Possible values for 'list_type' are one of the follwing:
 *  * 	GCONF_VALUE_STRING, GCONF_VALUE_INT, GCONF_VALUE_BOOL, or GCONF_VALUE_FLOAT
 *   */
gboolean gconfig_set_list(const gchar *key, GSList *value, GConfValueType list_type);
gboolean gconfig_get_list(const gchar *key, GSList **value, GConfValueType list_type);

gboolean gconfig_rm_rf(const gchar *key);

G_END_DECLS

#endif /* __GCONFIG_H__ */

