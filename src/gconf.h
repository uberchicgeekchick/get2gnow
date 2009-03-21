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

#define TYPE_CONF         (conf_get_type ())
#define CONF(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_CONF, Conf))
#define CONF_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST ((k), TYPE_CONF, ConfClass))
#define IS_CONF(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_CONF))
#define IS_CONF_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_CONF))
#define CONF_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_CONF, ConfClass))

typedef struct _Conf      Conf;
typedef struct _ConfClass ConfClass;

struct _Conf  {
	GObject parent;
};

struct _ConfClass {
	GObjectClass parent_class;
};

typedef void (*ConfNotifyFunc) (Conf  *conf, 
									  const gchar *key,
									  gpointer     user_data);

GType       conf_get_type        (void) G_GNUC_CONST;
Conf *conf_get             (void);
void        conf_shutdown        (void);
guint       conf_notify_add      (Conf            *conf,
										 const gchar           *key,
										 ConfNotifyFunc   func,
										 gpointer               data);
gboolean    conf_notify_remove   (Conf            *conf,
										 guint                  id);
gboolean    conf_set_int         (Conf            *conf,
										 const gchar           *key,
										 gint                   value);
gboolean    conf_get_int         (Conf            *conf,
										 const gchar           *key,
										 gint                  *value);
gboolean    conf_set_bool        (Conf            *conf,
										 const gchar           *key,
										 gboolean               value);
gboolean    conf_get_bool        (Conf            *conf,
										 const gchar           *key,
										 gboolean              *value);
gboolean    conf_set_string      (Conf            *conf,
										 const gchar           *key,
										 const gchar           *value);
gboolean    conf_get_string      (Conf            *conf,
										 const gchar           *key,
										 gchar                **value);
gboolean    conf_set_string_list (Conf            *conf,
										 const gchar           *key,
										 GSList                *value);
gboolean    conf_get_string_list (Conf            *conf,
										 const gchar           *key,
										 GSList              **value);

G_END_DECLS

#endif /* __CONF_H__ */

