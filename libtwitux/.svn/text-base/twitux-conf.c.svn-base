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

#include "twitux-conf.h"
#include "twitux-debug.h"

#define DEBUG_DOMAIN "Config"

#define TWITUX_CONF_ROOT        "/apps/twitux"
#define DESKTOP_INTERFACE_ROOT  "/desktop/gnome/interface"

#define GET_PRIV(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TWITUX_TYPE_CONF, TwituxConfPriv))

typedef struct {
	GConfClient *gconf_client;
} TwituxConfPriv;

typedef struct {
	TwituxConf           *conf;
	TwituxConfNotifyFunc  func;
	gpointer              user_data;
} TwituxConfNotifyData;

static void conf_finalize (GObject *object);

G_DEFINE_TYPE (TwituxConf, twitux_conf, G_TYPE_OBJECT);

static TwituxConf *global_conf = NULL;

static void
twitux_conf_class_init (TwituxConfClass *class)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (class);

	object_class->finalize = conf_finalize;

	g_type_class_add_private (object_class, sizeof (TwituxConfPriv));
}

static void
twitux_conf_init (TwituxConf *conf)
{
	TwituxConfPriv *priv;

	priv = GET_PRIV (conf);

	priv->gconf_client = gconf_client_get_default ();

	gconf_client_add_dir (priv->gconf_client,
						  TWITUX_CONF_ROOT,
						  GCONF_CLIENT_PRELOAD_ONELEVEL,
						  NULL);
	gconf_client_add_dir (priv->gconf_client,
						  DESKTOP_INTERFACE_ROOT,
						  GCONF_CLIENT_PRELOAD_NONE,
						  NULL);
}

static void
conf_finalize (GObject *object)
{
	TwituxConfPriv *priv;

	priv = GET_PRIV (object);

	gconf_client_remove_dir (priv->gconf_client,
							 TWITUX_CONF_ROOT,
							 NULL);
	gconf_client_remove_dir (priv->gconf_client,
							 DESKTOP_INTERFACE_ROOT,
							 NULL);

	g_object_unref (priv->gconf_client);

	G_OBJECT_CLASS (twitux_conf_parent_class)->finalize (object);
}

TwituxConf *
twitux_conf_get (void)
{
	if (!global_conf) {
		global_conf = g_object_new (TWITUX_TYPE_CONF, NULL);
	}

	return global_conf;
}

void
twitux_conf_shutdown (void)
{
	if (global_conf) {
		g_object_unref (global_conf);
		global_conf = NULL;
	}
}

gboolean
twitux_conf_set_int (TwituxConf  *conf,
					 const gchar *key,
					 gint         value)
{
	TwituxConfPriv *priv;

	g_return_val_if_fail (TWITUX_IS_CONF (conf), FALSE);

	twitux_debug (DEBUG_DOMAIN, "Setting int:'%s' to %d", key, value);

	priv = GET_PRIV (conf);

	return gconf_client_set_int (priv->gconf_client,
								 key,
								 value,
								 NULL);
}

gboolean
twitux_conf_get_int (TwituxConf  *conf,
					 const gchar *key,
					 gint        *value)
{
	TwituxConfPriv *priv;
	GError         *error = NULL;

	*value = 0;

	g_return_val_if_fail (TWITUX_IS_CONF (conf), FALSE);
	g_return_val_if_fail (value != NULL, FALSE);

	priv = GET_PRIV (conf);

	*value = gconf_client_get_int (priv->gconf_client,
								   key,
								   &error);

	twitux_debug (DEBUG_DOMAIN, "Getting int:'%s' (=%d), error:'%s'",
				  key, *value, error ? error->message : "None");

	if (error) {
		g_error_free (error);
		return FALSE;
	}

	return TRUE;
}

gboolean
twitux_conf_set_bool (TwituxConf  *conf,
					  const gchar *key,
					  gboolean     value)
{
	TwituxConfPriv *priv;

	g_return_val_if_fail (TWITUX_IS_CONF (conf), FALSE);

	twitux_debug (DEBUG_DOMAIN, "Setting bool:'%s' to %d ---> %s",
				  key, value, value ? "true" : "false");

	priv = GET_PRIV (conf);

	return gconf_client_set_bool (priv->gconf_client,
								  key,
								  value,
								  NULL);
}

gboolean
twitux_conf_get_bool (TwituxConf  *conf,
					  const gchar *key,
					  gboolean    *value)
{
	TwituxConfPriv *priv;
	GError         *error = NULL;

	*value = FALSE;

	g_return_val_if_fail (TWITUX_IS_CONF (conf), FALSE);
	g_return_val_if_fail (value != NULL, FALSE);

	priv = GET_PRIV (conf);

	*value = gconf_client_get_bool (priv->gconf_client,
									key,
									&error);

	twitux_debug (DEBUG_DOMAIN, "Getting bool:'%s' (=%d ---> %s), error:'%s'",
				  key, *value, *value ? "true" : "false",
				  error ? error->message : "None");

	if (error) {
		g_error_free (error);
		return FALSE;
	}

	return TRUE;
}

gboolean
twitux_conf_set_string (TwituxConf  *conf,
						const gchar *key,
						const gchar *value)
{
	TwituxConfPriv *priv;

	g_return_val_if_fail (TWITUX_IS_CONF (conf), FALSE);

	twitux_debug (DEBUG_DOMAIN, "Setting string:'%s' to '%s'",
				  key, value);

	priv = GET_PRIV (conf);

	return gconf_client_set_string (priv->gconf_client,
									key,
									value,
									NULL);
}

gboolean
twitux_conf_get_string (TwituxConf   *conf,
						const gchar  *key,
						gchar       **value)
{
	TwituxConfPriv *priv;
	GError         *error = NULL;

	*value = NULL;

	g_return_val_if_fail (TWITUX_IS_CONF (conf), FALSE);

	priv = GET_PRIV (conf);

	*value = gconf_client_get_string (priv->gconf_client,
									  key,
									  &error);

	twitux_debug (DEBUG_DOMAIN, "Getting string:'%s' (='%s'), error:'%s'",
				  key, *value, error ? error->message : "None");

	if (error) {
		g_error_free (error);
		return FALSE;
	}

	return TRUE;
}

gboolean
twitux_conf_set_string_list (TwituxConf  *conf,
							 const gchar *key,
							 GSList      *value)
{
	TwituxConfPriv *priv;

	g_return_val_if_fail (TWITUX_IS_CONF (conf), FALSE);

	priv = GET_PRIV (conf);

	return gconf_client_set_list (priv->gconf_client,
								  key,
								  GCONF_VALUE_STRING,
								  value,
								  NULL);
}

gboolean
twitux_conf_get_string_list (TwituxConf   *conf,
							 const gchar  *key,
							 GSList      **value)
{
	TwituxConfPriv *priv;
	GError         *error = NULL;

	*value = NULL;

	g_return_val_if_fail (TWITUX_IS_CONF (conf), FALSE);

	priv = GET_PRIV (conf);

	*value = gconf_client_get_list (priv->gconf_client,
									key,
									GCONF_VALUE_STRING,
									&error);
	if (error) {
		g_error_free (error);
		return FALSE;
	}

	return TRUE;
}

static void
conf_notify_data_free (TwituxConfNotifyData *data)
{
	g_object_unref (data->conf);
	g_slice_free (TwituxConfNotifyData, data);
}

static void
conf_notify_func (GConfClient *client,
				  guint        id,
				  GConfEntry  *entry,
				  gpointer     user_data)
{
	TwituxConfNotifyData *data;

	data = user_data;

	data->func (data->conf,
				gconf_entry_get_key (entry),
				data->user_data);
}

guint
twitux_conf_notify_add (TwituxConf           *conf,
						const gchar          *key,
						TwituxConfNotifyFunc  func,
						gpointer              user_data)
{
	TwituxConfPriv       *priv;
	guint                 id;
	TwituxConfNotifyData *data;

	g_return_val_if_fail (TWITUX_IS_CONF (conf), 0);

	priv = GET_PRIV (conf);

	data = g_slice_new (TwituxConfNotifyData);
	data->func = func;
	data->user_data = user_data;
	data->conf = g_object_ref (conf);

	id = gconf_client_notify_add (priv->gconf_client,
								  key,
								  conf_notify_func,
								  data,
								  (GFreeFunc) conf_notify_data_free,
								  NULL);

	return id;
}

gboolean
twitux_conf_notify_remove (TwituxConf *conf,
						   guint       id)
{
	TwituxConfPriv *priv;

	g_return_val_if_fail (TWITUX_IS_CONF (conf), FALSE);

	priv = GET_PRIV (conf);

	gconf_client_notify_remove (priv->gconf_client, id);

	return TRUE;
}

