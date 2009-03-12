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

#ifndef __TWITUX_SEND_MESSAGE_DIALOG_H__
#define __TWITUX_SEND_MESSAGE_DILAOG_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TWITUX_TYPE_MESSAGE         	(twitux_message_get_type ())
#define TWITUX_MESSAGE(o)		    	(G_TYPE_CHECK_INSTANCE_CAST ((o), TWITUX_TYPE_MESSAGE, TwituxMsgDialog))
#define TWITUX_MESSAGE_CLASS(k)	   	 	(G_TYPE_CHECK_CLASS_CAST((k), TWITUX_TYPE_MESSAGE, TwituxMsgDialogClass))
#define TWITUX_IS_MESSAGE(o)	    	(G_TYPE_CHECK_INSTANCE_TYPE ((o), TWITUX_TYPE_MESSAGE))
#define TWITUX_IS_MESSAGE_CLASS(k)  	(G_TYPE_CHECK_CLASS_TYPE ((k), TWITUX_TYPE_MESSAGE))
#define TWITUX_IS_MESSAGE_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), TWITUX_TYPE_MESSAGE, TwituxMsgDialogClass))

typedef struct _TwituxMsgDialog      	TwituxMsgDialog;
typedef struct _TwituxMsgDialogClass 	TwituxMsgDialogClass;
typedef struct _TwituxMsgDialogPriv  	TwituxMsgDialogPriv;

struct _TwituxMsgDialog {
        GObject        parent;
};

struct _TwituxMsgDialogClass {
        GObjectClass parent_class;
};

GType             twitux_message_get_type         (void) G_GNUC_CONST;
void              twitux_send_message_dialog_show (GtkWindow   *parent);
void              twitux_message_correct_word     (GtkWidget   *textview,
									               GtkTextIter  start,
									               GtkTextIter  end,
									               const gchar *new_word);
void              twitux_message_set_followers    (GList       *followers);
void              twitux_message_show_friends     (gboolean     show_friends);
void              twitux_message_set_message      (const gchar *message);
G_END_DECLS

#endif /* __TWITUX_SEND_MESSAGE_DIALOG_H__ */
