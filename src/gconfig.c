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

/********************************************************
 *          My art, code, & programming.                *
 ********************************************************/
#include <string.h>
#include <gconf/gconf-client.h>


/********************************************************
 *        Project headers, eg #include "config.h"       *
 ********************************************************/
#include "config.h"

#include "gconfig.h"
#include "preferences.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
typedef struct {
	GConfClient *gconf_client;
} GConfigPriv;

typedef struct {
	GConfigNotifyFunc	func;
	gpointer		user_data;
} GConfigNotifyData;

#define DEBUG_DOMAINS "GConfig:OnlineServices:Authentication:Preferences:Settings:Setup:Start-Up"
#include "debug.h"

#define DESKTOP_INTERFACE_ROOT  "/desktop/gnome/interface"

#define	GET_PRIV(obj)	(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_GCONFIG, GConfigPriv))
G_DEFINE_TYPE(GConfig, gconfig, G_TYPE_OBJECT);

static GConfig *gconfig=NULL;
static GConfigPriv *gconfig_priv=NULL;


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static void gconfig_finalize(GObject *object);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
static void gconfig_class_init(GConfigClass *class){
	GObjectClass *object_class;
	object_class=G_OBJECT_CLASS(class);
	object_class->finalize=gconfig_finalize;
	g_type_class_add_private(object_class, sizeof(GConfigPriv));
}

static void gconfig_init(GConfig *gconfig){
	gconfig_priv=GET_PRIV(gconfig);
	gconfig_priv->gconf_client=gconf_client_get_default();

	gconf_client_add_dir(gconfig_priv->gconf_client, PREFS_PATH, GCONF_CLIENT_PRELOAD_ONELEVEL, NULL);
	gconf_client_add_dir(gconfig_priv->gconf_client, DESKTOP_INTERFACE_ROOT, GCONF_CLIENT_PRELOAD_NONE, NULL);
}

static void gconfig_finalize(GObject *object){
	gconf_client_remove_dir(gconfig_priv->gconf_client, PREFS_PATH, NULL);
	gconf_client_remove_dir(gconfig_priv->gconf_client, DESKTOP_INTERFACE_ROOT, NULL);
	g_object_unref(gconfig_priv->gconf_client);
	G_OBJECT_CLASS(gconfig_parent_class)->finalize(object);
}

void gconfig_start(void){
	if(gconfig) return;
	gconfig=g_object_new(TYPE_GCONFIG, NULL);
}

void gconfig_shutdown(void){
	if(!gconfig) return;
	
	g_object_unref(gconfig);
	gconfig=NULL;
}

gboolean gconfig_set_int(const gchar *key, gint value){
	debug("Setting int:'%s' to %d", key, value);
	return gconf_client_set_int(gconfig_priv->gconf_client, key, value, NULL);
}

gboolean gconfig_get_int(const gchar *key, gint *value){
	GError         *error=NULL;
	
	*value=0;
	
	g_return_val_if_fail(value != NULL, FALSE);
	
	*value=gconf_client_get_int(gconfig_priv->gconf_client, key, &error);
	
	debug("Getting int:'%s'(=%d)", key, *value);
	
	if(error){
		debug("\t\t**ERROR:** %s", error->message);
		g_error_free(error);
		return FALSE;
	}
	
	return TRUE;
}

gboolean gconfig_if_bool(const gchar *key, gboolean bool_default){
	GError *error=NULL;
	gboolean value=gconf_client_get_bool(gconfig_priv->gconf_client, key, &error);
	
	debug("Getting boolean: '%s':\n\t\tRetrieved: [%s]\t\tDefault: [%s].", key, (value?"TRUE":"FALSE"), (bool_default?"TRUE":"FALSE"));
	
	if(error) {
		debug("\t\t**ERROR:** %s. Setting default value.", error->message);
		if(!(gconfig_set_bool(key, bool_default)))
			debug("\t\t**ERROR:** failed to set '%s' to default value: '%s'.", error->message, (bool_default ?"TRUE" :"FALSE"));

		g_error_free(error);
		return bool_default;
	}
	
	return value;
}

gboolean gconfig_set_bool(const gchar *key, gboolean value){
	debug("Setting bool:'%s' to %s(=%s).", key, (value ? "true" : "false"), value);
	return gconf_client_set_bool(gconfig_priv->gconf_client, key, value,NULL);
}

gboolean gconfig_get_bool(const gchar *key, gboolean *value){
	GError *error=NULL;
	*value=FALSE;

	g_return_val_if_fail(value != NULL, FALSE);
	*value=gconf_client_get_bool(gconfig_priv->gconf_client, key, &error);

	debug("Getting bool:'%s'(=%d ---> %s).", key, *value, (*value ? "true" : "false") );
	
	if(error) {
		debug("\t\t**ERROR:** %s", error->message);
		g_error_free(error);
		return FALSE;
	}

	return TRUE;
}

gboolean gconfig_set_string(const gchar *key, const gchar *value){
	debug("Setting string:'%s' to '%s'", key, value);
	
	return gconf_client_set_string(gconfig_priv->gconf_client, key, value, NULL);
}

gboolean gconfig_get_string(const gchar *key, gchar **value){
	GError         *error=NULL;

	*value=NULL;
	
	*value=gconf_client_get_string(gconfig_priv->gconf_client, key, &error);

	debug("Getting string: '%s'(='%s').", key, *value);

	if(error) {
		debug("\t\t**ERROR:** %s", error->message);
		g_error_free(error);
		return FALSE;
	}

	return TRUE;
}

gboolean gconfig_set_list_string(const gchar *key, GSList *value){
	return gconfig_set_list(key, value, GCONF_VALUE_STRING);
}//gconfig_set_list_string

gboolean gconfig_get_list_string(const gchar *key, GSList **value){
	return gconfig_get_list(key, value, GCONF_VALUE_STRING);
}//gconfig_set_list_string

/* Possible values for 'list_type' are one of the follwing:
 * 	GCONF_VALUE_STRING, GCONF_VALUE_INT, GCONF_VALUE_BOOL, GCONF_VALUE_FLOAT, GCONF_VALUE_INVALID, GCONF_VALUE_SCHEMA, or 
 */
static void gconfig_print_list_values(GSList *value, GConfValueType list_type){
	GSList *l=NULL;
	if( list_type==GCONF_VALUE_INVALID || list_type==GCONF_VALUE_SCHEMA || list_type==GCONF_VALUE_LIST || list_type==GCONF_VALUE_PAIR ){
		debug("\t\t[undisplayable/mixed values]");
		return;
	}
	
	for(l=value; l; l=l->next)
		switch(list_type){
			case GCONF_VALUE_STRING:
				debug("\t\t\t'%s'",(gchar *)l->data);
				break;
			case GCONF_VALUE_INT:
				debug("\t\t\t'%d'",(gint *)l->data);
				break;
			case GCONF_VALUE_BOOL:
				debug("\t\t\t'%s'",((gboolean *)l->data ?"TRUE" :"FALSE" ));
				break;
			case GCONF_VALUE_FLOAT:
				debug("\t\t\t'%f'",(gfloat *)l->data);
				break;
			case GCONF_VALUE_INVALID:
			case GCONF_VALUE_SCHEMA:
			case GCONF_VALUE_LIST:
			case GCONF_VALUE_PAIR:
			default:
				/* yes we know this is never executed, it catches gcc errors & warning. */
				break;
		}
}//gconfig_print_list

gboolean gconfig_set_list(const gchar *key, GSList *value, GConfValueType list_type){
	if(IF_DEBUG){
		debug("Saving list: '%s', values:(=", key);
		gconfig_print_list_values(value, list_type);
		debug("\t\t)" );
	}
	return gconf_client_set_list(gconfig_priv->gconf_client, key, list_type, value, NULL );
}

gboolean gconfig_get_list(const gchar  *key, GSList **value, GConfValueType list_type){
	GError *error=NULL;
	*value=NULL;
	
	*value=gconf_client_get_list(gconfig_priv->gconf_client, key, list_type, &error);
	if(error){
		debug("\t\t**ERROR:** failed to retrieve list: %s, error: %s.", error->message);
		g_error_free(error);
		return FALSE;
	}else if(IF_DEBUG){
		debug("Retrieved list: '%s':(=", key);
		gconfig_print_list_values(*value, list_type);
		debug("\t)" );
	}
	return TRUE;
}

gboolean gconfig_rm_rf(const gchar *key){
	GError *error=NULL;
	debug("Removing %s and all keys below it.", key);
	gboolean success=gconf_client_recursive_unset(gconfig_priv->gconf_client, key, GCONF_UNSET_INCLUDING_SCHEMA_NAMES, &error);
	if(error){
		debug("\t\t**ERROR**: %s", error->message);
		g_error_free(error);
	}
	return success;
}//gconfig_rm_rf

static void gconfig_notify_data_free(GConfigNotifyData *data){
	g_slice_free(GConfigNotifyData, data);
}

static void gconfig_notify_func(GConfClient *client, guint id, GConfEntry  *entry, gpointer user_data){
	GConfigNotifyData *data;
	data=user_data;
	data->func(gconf_entry_get_key(entry), data->user_data);
}

guint gconfig_notify_add(const gchar *key, GConfigNotifyFunc func, gpointer user_data){
	GConfigNotifyData *data;

	data=g_slice_new0(GConfigNotifyData);
	data->func=func;
	data->user_data=user_data;

	return gconf_client_notify_add(gconfig_priv->gconf_client, key, gconfig_notify_func, data, (GFreeFunc)gconfig_notify_data_free, NULL);
}

gboolean gconfig_notify_remove(guint id){
	gconf_client_notify_remove(gconfig_priv->gconf_client, id);
	return TRUE;
}


/********************************************************
 *                       eof                            *
 ********************************************************/

