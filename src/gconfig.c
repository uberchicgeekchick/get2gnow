/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright(C) 2006 Imendio AB
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or(at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors: Richard Hult <richard@imendio.com>
 *			Brian Pepple <bpepple@fedoraproject.org>
 */

#include "config.h"

#include <string.h>

#include <gconf/gconf-client.h>

#include "gconfig.h"
#include "debug.h"
#include "preferences.h"


#define DEBUG_DOMAIN "GConfig"

#define DESKTOP_INTERFACE_ROOT  "/desktop/gnome/interface"

#define GET_PRIV(obj)(G_TYPE_INSTANCE_GET_PRIVATE((obj), TYPE_GCONFIG, GConfigPriv))

typedef struct {
	GConfClient *gconf_client;
} GConfigPriv;

typedef struct {
	GConfigNotifyFunc	func;
	gpointer		user_data;
} GConfigNotifyData;

static void gconfig_finalize(GObject *object);

G_DEFINE_TYPE(GConfig, gconfig, G_TYPE_OBJECT);

static GConfig *gconfig=NULL;
static GConfigPriv *gconfig_priv=NULL;

static void gconfig_class_init(GConfigClass *class){
	GObjectClass *object_class;
	object_class=G_OBJECT_CLASS(class);
	object_class->finalize=gconfig_finalize;
	g_type_class_add_private(object_class, sizeof(GConfigPriv));
}

static void gconfig_init(GConfig *gconfig){
	gconfig_priv=GET_PRIV(gconfig);
	gconfig_priv->gconf_client=gconf_client_get_default();

	gconf_client_add_dir(gconfig_priv->gconf_client,
						  PREFS_PATH,
						  GCONF_CLIENT_PRELOAD_ONELEVEL,
						  NULL);
	gconf_client_add_dir(gconfig_priv->gconf_client,
						  DESKTOP_INTERFACE_ROOT,
						  GCONF_CLIENT_PRELOAD_NONE,
						  NULL);
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

gboolean gconfig_set_int(const gchar *key,
					 gint         value)
{
	

	debug(DEBUG_DOMAIN, "Setting int:'%s' to %d", key, value);

	
	return gconf_client_set_int(gconfig_priv->gconf_client,
								 key,
								 value,
								 NULL);
}

gboolean
gconfig_get_int(const gchar *key,
					 gint        *value)
{
		GError         *error=NULL;

	*value=0;

	g_return_val_if_fail(value != NULL, FALSE);

	
	*value=gconf_client_get_int(gconfig_priv->gconf_client,
								   key,
								   &error);

	debug(DEBUG_DOMAIN, "Getting int:'%s'(=%d), error:'%s'",
				  key, *value, error ? error->message : "None");

	if(error) {
		g_error_free(error);
		return FALSE;
	}

	return TRUE;
}

gboolean gconfig_if_bool(const gchar *key){
	GError *error=NULL;
	gboolean value=gconf_client_get_bool(gconfig_priv->gconf_client, key, &error);
	
	debug(DEBUG_DOMAIN, "Getting bool:'%s'(=%d ---> %s), error:'%s'",
			key, value,(value ?"true" :"false"),
			error ? error->message : "None");
	
	if(error) {
		g_error_free(error);
		return FALSE;
	}
	
	return value;
}

gboolean gconfig_set_bool(const gchar *key, gboolean value){
	debug(DEBUG_DOMAIN, "Setting bool:'%s' to %d ---> %s",
				  key, value, value ? "true" : "false");

	
	return gconf_client_set_bool(gconfig_priv->gconf_client,
								  key,
								  value,
								  NULL);
}

gboolean gconfig_get_bool(const gchar *key, gboolean *value){
	GError *error=NULL;
	*value=FALSE;

	g_return_val_if_fail(value != NULL, FALSE);
	*value=gconf_client_get_bool(gconfig_priv->gconf_client,
									key,
									&error);

	debug(DEBUG_DOMAIN, "Getting bool:'%s'(=%d ---> %s), error:'%s'",
				  key, *value, *value ? "true" : "false",
				  error ? error->message : "None");

	if(error) {
		g_error_free(error);
		return FALSE;
	}

	return TRUE;
}

gboolean
gconfig_set_string(const gchar *key,
						const gchar *value)
{
	

	debug(DEBUG_DOMAIN, "Setting string:'%s' to '%s'",
				  key, value);

	
	return gconf_client_set_string(gconfig_priv->gconf_client,
									key,
									value,
									NULL);
}

gboolean
gconfig_get_string(const gchar  *key,
						gchar       **value)
{
	GError         *error=NULL;

	*value=NULL;


	
	*value=gconf_client_get_string(gconfig_priv->gconf_client,
									  key,
									  &error);

	debug(DEBUG_DOMAIN, "Getting string:'%s'(='%s'), error:'%s'", key, *value,(error ?error->message :"None") );

	if(error) {
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
		debug(DEBUG_DOMAIN, "\t\t[undisplayable/mixed values]");
		return;
	}
	
	for(l=value; l; l=l->next)
		switch(list_type){
			case GCONF_VALUE_STRING:
				debug(DEBUG_DOMAIN, "\t\t'%s'",(gchar *)l->data);
				break;
			case GCONF_VALUE_INT:
				debug(DEBUG_DOMAIN, "\t\t'%d'",(gint *)l->data);
				break;
			case GCONF_VALUE_BOOL:
				debug(DEBUG_DOMAIN, "\t\t'%s'",((gboolean *)l->data ?"TRUE" :"FALSE" ));
				break;
			case GCONF_VALUE_FLOAT:
				debug(DEBUG_DOMAIN, "\t\t'%f'",(gfloat *)l->data);
				break;
			case GCONF_VALUE_INVALID:
			case GCONF_VALUE_SCHEMA:
			case GCONF_VALUE_LIST:
			case GCONF_VALUE_PAIR:
				/* yes we know this is never executed, it catches gcc errors & warning. */
				break;
		}
}//gconfig_print_list

gboolean gconfig_set_list(const gchar *key, GSList *value, GConfValueType list_type){
	debug(DEBUG_DOMAIN, "Saving list: '%s', values:(=", key);
	gconfig_print_list_values(value, list_type);
	debug(DEBUG_DOMAIN, "\t)" );
	return gconf_client_set_list(
					gconfig_priv->gconf_client,
					key,
					list_type,
					value,
				NULL
	);
}

gboolean gconfig_get_list(const gchar  *key, GSList **value, GConfValueType list_type){
	GError *error=NULL;
	*value=NULL;
	
	*value=gconf_client_get_list(
					gconfig_priv->gconf_client,
					key,
					list_type,
					&error
	);
	debug(DEBUG_DOMAIN, "Retrieved list: '%s', error: '%s', values:(=", key,(error ?error->message :"None"));
	if(error){
		debug(DEBUG_DOMAIN, "\tan error has occured)" );
		g_error_free(error);
		return FALSE;
	}
	
	gconfig_print_list_values(*value, list_type);
	debug(DEBUG_DOMAIN, "\t)" );
	return TRUE;
}

gboolean gconfig_rm_rf(const gchar *key){
	GError *error=NULL;
	debug(DEBUG_DOMAIN, "Removing %s and all keys below it.", key);
	gboolean success=gconf_client_recursive_unset(gconfig_priv->gconf_client, key, GCONF_UNSET_INCLUDING_SCHEMA_NAMES, &error);
	if(error)
		debug(DEBUG_DOMAIN, "**ERROR**: %s", error->message);
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
	guint                 id;
	GConfigNotifyData *data;

	data=g_slice_new0(GConfigNotifyData);
	data->func=func;
	data->user_data=user_data;

	id=gconf_client_notify_add(gconfig_priv->gconf_client,
								key,
								gconfig_notify_func,
								data,
								(GFreeFunc) gconfig_notify_data_free,
								NULL);
	
	return id;
}

gboolean gconfig_notify_remove(guint id){
	gconf_client_notify_remove(gconfig_priv->gconf_client, id);
	return TRUE;
}

