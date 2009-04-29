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
 */

#ifndef __CONF_H__
#define __CONF_H__

#include <glib-object.h>

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

typedef void (*GConfigNotifyFunc) (GConfig  *gconfig, const gchar *key, gpointer user_data);

GType gconfig_get_type(void) G_GNUC_CONST;
GConfig *gconfig_get(void);
void gconfig_shutdown(void);
guint       gconfig_notify_add      (GConfig *gconfig,
										 const gchar           *key,
										 GConfigNotifyFunc   func,
										 gpointer               data);
gboolean    gconfig_notify_remove   (GConfig *gconfig,
										 guint                  id);
gboolean    gconfig_set_int         (GConfig *gconfig,
										 const gchar           *key,
										 gint                   value);
gboolean    gconfig_get_int         (GConfig *gconfig,
										 const gchar           *key,
										 gint                  *value);
gboolean    gconfig_set_bool        (GConfig *gconfig,
										 const gchar           *key,
										 gboolean               value);
gboolean    gconfig_get_bool        (GConfig *gconfig,
										 const gchar           *key,
										 gboolean              *value);
gboolean    gconfig_set_string      (GConfig *gconfig,
										 const gchar           *key,
										 const gchar           *value);
gboolean    gconfig_get_string      (GConfig *gconfig,
										 const gchar           *key,
										 gchar                **value);
gboolean    gconfig_set_string_list (GConfig *gconfig,
										 const gchar           *key,
										 GSList                *value);
gboolean    gconfig_get_string_list (GConfig *gconfig,
										 const gchar           *key,
										 GSList              **value);

G_END_DECLS

#endif /* __CONF_H__ */

