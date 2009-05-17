/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2008 Daniel Morales <daniminas@gmail.com>
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

#ifndef __THIS_H__
#define __THIS_H__

#include <glib.h>
#include <glib-object.h>
#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>


G_BEGIN_DECLS

#define	TYPE_THIS		(this_get_type())
#define	THIS(o)			(G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_THIS, This))
#define	THIS_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), TYPE_THIS, ThisClass))
#define	IS_THIS(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_THIS))
#define	IS_THIS_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_THIS))
#define	IS_THIS_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_THIS, ThisClass))

typedef struct{
	GObject			parent; /*What other Extends/Implements/Interfaces.*/
	/* Commonly Used:
	 *	GObject			parent;
	 *	GtkObject		parent;
	 *	GtkWidget		parent;
	 *	GtkContainer		parent;
	 *	GBin			parent;
	 *	GtkWindow		parent;
	 *	GtkItem			parent;
	 *	SexyLabel		parent;
	 */
} This;

typedef struct{
	GObjectClass			parent_class;
	/* Commonly Used:
	 *	GObjectClass		parent_class;
	 *	GtkObjectClass		parent_class;
	 *	GtkWidgetClass		parent_class;
	 *	GtkContainerClass	parent_class;
	 *	GBinClass		parent_class;
	 *	GtkWindowClass		parent_class;
	 *	GtkItemClass		parent_class;
	 *	SexyLabelClass		parent_class;
	 */
} ThisClass;


GType this_get_type(void) G_GNUC_CONST;
This *this_new(void);
This *this_new_from(const gchar *first_property, ...);
void this_show(GtkWindow *parent);

G_END_DECLS

#endif /* __THIS_H__ */
