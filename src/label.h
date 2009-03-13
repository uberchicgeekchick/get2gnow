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

#ifndef __LABEL_H__
#define __LABEL_H__

#include <libsexy/sexy-url-label.h>

G_BEGIN_DECLS

/*
 * TwituxLabel
 */ 
#define TYPE_LABEL         (label_get_type ())
#define LABEL(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_LABEL, TwituxLabel))
#define LABEL_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), TYPE_LABEL, TwituxLabelClass))
#define IS_LABEL(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_LABEL))
#define IS_LABEL_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_LABEL))
#define LABEL_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_LABEL, TwituxLabelClass))

typedef struct _TwituxLabel        TwituxLabel;
typedef struct _TwituxLabelClass   TwituxLabelClass;
typedef struct _TwituxLabelPriv    TwituxLabelPriv;

struct _TwituxLabel {
	SexyUrlLabel           parent;
};

struct _TwituxLabelClass {
	SexyUrlLabelClass      parent_class;
};

GType             label_get_type  (void) G_GNUC_CONST;
GtkWidget*        label_new       (void);
void              label_set_text  (TwituxLabel  *nav,
                                          const gchar  *text);
G_END_DECLS

#endif /* __LABEL_H__ */
