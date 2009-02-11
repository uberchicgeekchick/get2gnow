/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2007-2008 Daniel Morales <daniminas@gmail.com>
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
 * Authors: Daniel Morales <daniminas@gmail.com>
 *
 */

#ifndef __TWITUX_LABEL_H__
#define __TWITUX_LABEL_H__

#include <libsexy/sexy-url-label.h>

G_BEGIN_DECLS

/*
 * TwituxLabel
 */ 
#define TWITUX_TYPE_LABEL         (twitux_label_get_type ())
#define TWITUX_LABEL(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), TWITUX_TYPE_LABEL, TwituxLabel))
#define TWITUX_LABEL_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), TWITUX_TYPE_LABEL, TwituxLabelClass))
#define TWITUX_IS_LABEL(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), TWITUX_TYPE_LABEL))
#define TWITUX_IS_LABEL_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), TWITUX_TYPE_LABEL))
#define TWITUX_LABEL_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TWITUX_TYPE_LABEL, TwituxLabelClass))

typedef struct _TwituxLabel        TwituxLabel;
typedef struct _TwituxLabelClass   TwituxLabelClass;
typedef struct _TwituxLabelPriv    TwituxLabelPriv;

struct _TwituxLabel {
	SexyUrlLabel           parent;
};

struct _TwituxLabelClass {
	SexyUrlLabelClass      parent_class;
};

GType             twitux_label_get_type  (void) G_GNUC_CONST;
GtkWidget*        twitux_label_new       (void);
void              twitux_label_set_text  (TwituxLabel  *nav,
                                          const gchar  *text);
G_END_DECLS

#endif /* __TWITUX_LABEL_H__ */
