/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2006 Imendio AB
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
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
	GConfig			*gconfig;
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
	object_class->finalize = gconfig_finalize;
	g_type_class_add_private(object_class, sizeof(GConfigPriv));
}

static void gconfig_init(GConfig *gconfig){
	gconfig_priv=GET_PRIV(gconfig);
	gconfig_priv->gconf_client = gconf_client_get_default();

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

GConfig *gconfig_get(void){
	if(!gconfig)
		gconfig=g_object_new(TYPE_GCONFIG, NULL);

	return gconfig;
}

void gconfig_shutdown(void){
	if(!gconfig) return;
	
	g_object_unref(gconfig);
	gconfig=NULL;
}

gboolean gconfig_set_int(GConfig *gconfig,
					 const gchar *key,
					 gint         value)
{
	
	g_return_val_if_fail(IS_GCONFIG(gconfig), FALSE);

	debug(DEBUG_DOMAIN, "Setting int:'%s' to %d", key, value);

	
	return gconf_client_set_int(gconfig_priv->gconf_client,
								 key,
								 value,
								 NULL);
}

gboolean
gconfig_get_int(GConfig *gconfig,
					 const gchar *key,
					 gint        *value)
{
		GError         *error = NULL;

	*value = 0;

	g_return_val_if_fail(IS_GCONFIG(gconfig), FALSE);
	g_return_val_if_fail(value != NULL, FALSE);

	
	*value = gconf_client_get_int(gconfig_priv->gconf_client,
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

gboolean
gconfig_set_bool(GConfig *gconfig,
					  const gchar *key,
					  gboolean     value)
{
	
	g_return_val_if_fail(IS_GCONFIG(gconfig), FALSE);

	debug(DEBUG_DOMAIN, "Setting bool:'%s' to %d ---> %s",
				  key, value, value ? "true" : "false");

	
	return gconf_client_set_bool(gconfig_priv->gconf_client,
								  key,
								  value,
								  NULL);
}

gboolean
gconfig_get_bool(GConfig *gconfig,
					  const gchar *key,
					  gboolean    *value)
{
		GError         *error = NULL;

	*value = FALSE;

	g_return_val_if_fail(IS_GCONFIG(gconfig), FALSE);
	g_return_val_if_fail(value != NULL, FALSE);

	
	*value = gconf_client_get_bool(gconfig_priv->gconf_client,
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
gconfig_set_string(GConfig *gconfig,
						const gchar *key,
						const gchar *value)
{
	
	g_return_val_if_fail(IS_GCONFIG(gconfig), FALSE);

	debug(DEBUG_DOMAIN, "Setting string:'%s' to '%s'",
				  key, value);

	
	return gconf_client_set_string(gconfig_priv->gconf_client,
									key,
									value,
									NULL);
}

gboolean
gconfig_get_string(GConfig *gconfig,
						const gchar  *key,
						gchar       **value)
{
	GError         *error = NULL;

	*value = NULL;

	g_return_val_if_fail(IS_GCONFIG(gconfig), FALSE);

	
	*value = gconf_client_get_string(gconfig_priv->gconf_client,
									  key,
									  &error);

	debug(DEBUG_DOMAIN, "Getting string:'%s'(='%s'), error:'%s'",
				  key, *value, error ? error->message : "None");

	if(error) {
		g_error_free(error);
		return FALSE;
	}

	return TRUE;
}

gboolean
gconfig_set_string_list(GConfig *gconfig,
							 const gchar *key,
							 GSList      *value)
{
	
	g_return_val_if_fail(IS_GCONFIG(gconfig), FALSE);

	
	return gconf_client_set_list(gconfig_priv->gconf_client,
								  key,
								  GCONF_VALUE_STRING,
								  value,
								  NULL);
}

gboolean
gconfig_get_string_list(GConfig *gconfig,
							 const gchar  *key,
							 GSList      **value)
{
		GError         *error = NULL;

	*value = NULL;

	g_return_val_if_fail(IS_GCONFIG(gconfig), FALSE);

	
	*value = gconf_client_get_list(gconfig_priv->gconf_client,
									key,
									GCONF_VALUE_STRING,
									&error);
	if(error) {
		g_error_free(error);
		return FALSE;
	}

	return TRUE;
}

static void gconfig_notify_data_free(GConfigNotifyData *data){
	g_object_unref(data->gconfig);
	g_slice_free(GConfigNotifyData, data);
}

static void gconfig_notify_func(GConfClient *client, guint id, GConfEntry  *entry, gpointer user_data){
	GConfigNotifyData *data;
	data = user_data;
	data->func(data->gconfig, gconf_entry_get_key(entry), data->user_data);
}

guint
gconfig_notify_add(GConfig *gconfig,
						const gchar          *key,
						GConfigNotifyFunc  func,
						gpointer              user_data)
{
		guint                 id;
	GConfigNotifyData *data;

	g_return_val_if_fail(IS_GCONFIG(gconfig), 0);

	
	data = g_slice_new(GConfigNotifyData);
	data->func = func;
	data->user_data = user_data;
	data->gconfig = g_object_ref(gconfig);

	id = gconf_client_notify_add(gconfig_priv->gconf_client,
								key,
								gconfig_notify_func,
								data,
								(GFreeFunc) gconfig_notify_data_free,
								NULL);

	return id;
}

gboolean
gconfig_notify_remove(GConfig *gconfig,
						   guint       id)
{
	g_return_val_if_fail(IS_GCONFIG(gconfig), FALSE);

	
	gconf_client_notify_remove(gconfig_priv->gconf_client, id);

	return TRUE;
}

