/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2002-2007 Imendio AB
 * Copyright (C) 2007-2008 Brian Pepple <bpepple@fedoraproject.org>
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
 * Authors: Mikael Hallendal <micke@imendio.com>
 *          Richard Hult <richard@imendio.com>
 *          Martyn Russell <martyn@imendio.com>
 *          Geert-Jan Van den Bogaerde <geertjan@gnome.org>
 *          Brian Pepple <bpepple@fedoraproject.org>
 *          Daniel Morales <daniminas@gmail.com>
 */

#ifndef __SEND_MESSAGE_DIALOG_H__
#define __SEND_MESSAGE_DILAOG_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TYPE_MESSAGE         	(message_get_type ())
#define MESSAGE(o)		    	(G_TYPE_CHECK_INSTANCE_CAST ((o), TYPE_MESSAGE, MsgDialog))
#define MESSAGE_CLASS(k)	   	 	(G_TYPE_CHECK_CLASS_CAST((k), TYPE_MESSAGE, MsgDialogClass))
#define IS_MESSAGE(o)	    	(G_TYPE_CHECK_INSTANCE_TYPE ((o), TYPE_MESSAGE))
#define IS_MESSAGE_CLASS(k)  	(G_TYPE_CHECK_CLASS_TYPE ((k), TYPE_MESSAGE))
#define IS_MESSAGE_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), TYPE_MESSAGE, MsgDialogClass))

typedef struct _MsgDialog      	MsgDialog;
typedef struct _MsgDialogClass 	MsgDialogClass;
typedef struct _MsgDialogPriv  	MsgDialogPriv;

struct _MsgDialog {
        GObject        parent;
};

struct _MsgDialogClass {
        GObjectClass parent_class;
};

GType             message_get_type         (void) G_GNUC_CONST;
void              send_message_dialog_show (GtkWindow   *parent);
void              message_correct_word     (GtkWidget   *textview,
									               GtkTextIter  start,
									               GtkTextIter  end,
									               const gchar *new_word);
void              message_set_followers    (GList       *followers);
void              message_show_friends     (gboolean     show_friends);
void              message_set_message      (const gchar *message);
G_END_DECLS

#endif /* __SEND_MESSAGE_DIALOG_H__ */
