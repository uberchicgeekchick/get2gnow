/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2004-2007 Imendio AB
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
 * Authors: Martyn Russell <martyn@imendio.com>
 *			Brian Pepple <bpepple@fedoraproject.org>
 */

#include <config.h>

#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus-glib-bindings.h>

#include <libtwitux/twitux-debug.h>

#include "twitux-app.h"
#include "twitux-dbus.h"

#define DEBUG_DOMAIN "DBUS"

static const gchar *dbus_nm_state_to_string          (guint32     state);
static void         dbus_nm_state_cb                 (DBusGProxy *proxy,
													  guint       state,
													  gpointer    user_data);
static gboolean     dbus_nm_proxy_restart_timeout_cb (gpointer    user_data);
static void         dbus_nm_proxy_notify_cb          (gpointer    data,
													  GObject    *where_the_object_was);

static DBusGProxy *nm_proxy = NULL;
static gint        nm_proxy_restart_retries = 0;
static guint       nm_proxy_restart_timeout_id = 0;

/*
 * GNOME Network Manager
 */

#define NM_DBUS_SERVICE   "org.freedesktop.NetworkManager"
#define NM_DBUS_PATH      "/org/freedesktop/NetworkManager"
#define NM_DBUS_INTERFACE "org.freedesktop.NetworkManager"

typedef enum NMState {
	NM_STATE_UNKNOWN = 0,
	NM_STATE_ASLEEP,
	NM_STATE_CONNECTING,
	NM_STATE_CONNECTED,
	NM_STATE_DISCONNECTED
} NMState;

static const gchar *
dbus_nm_state_to_string (guint32 state)
{
	switch (state) {
	case NM_STATE_ASLEEP:
		return "asleep";
	case NM_STATE_CONNECTING:
		return "connecting";
	case NM_STATE_CONNECTED:
		return "connected";
	case NM_STATE_DISCONNECTED:
		return "disconnected";
	case NM_STATE_UNKNOWN:
	default:
		return "unknown";
	}
}

static void
dbus_nm_state_cb (DBusGProxy *proxy,
				  guint       state,
				  gpointer    user_data)
{
	twitux_debug (DEBUG_DOMAIN, "New network state:'%s'",
				  dbus_nm_state_to_string (state));

	switch (state) {
	case NM_STATE_ASLEEP:
	case NM_STATE_DISCONNECTED:
	case NM_STATE_CONNECTING:
	case NM_STATE_UNKNOWN:
		twitux_app_state_on_connection (FALSE);
		break;
	case NM_STATE_CONNECTED:
		twitux_app_state_on_connection (TRUE);
		break;
	default:
		break;
	}
}

static gboolean
dbus_nm_proxy_restart_timeout_cb (gpointer user_data)
{
	if (twitux_dbus_nm_init ()) {
		nm_proxy_restart_timeout_id = 0;
		return FALSE;
	}

	nm_proxy_restart_retries--;
	if (nm_proxy_restart_retries == 0) {
		nm_proxy_restart_timeout_id = 0;
		return FALSE;
	}

	return TRUE;
}

static void
dbus_nm_proxy_notify_cb (gpointer  data,
						 GObject  *where_the_object_was)
{
	nm_proxy = NULL;
	nm_proxy_restart_retries = 5;

	if (nm_proxy_restart_timeout_id) {
		g_source_remove (nm_proxy_restart_timeout_id);
	}

	nm_proxy_restart_timeout_id =
		g_timeout_add (10*1000,
					   dbus_nm_proxy_restart_timeout_cb,
					   NULL);
}

void
twitux_dbus_nm_finalize (void)
{
	if (nm_proxy) {
		g_object_weak_unref (G_OBJECT (nm_proxy), dbus_nm_proxy_notify_cb, NULL);
		g_object_unref (nm_proxy);
	}
}

gboolean
twitux_dbus_nm_init (void)
{
	DBusGConnection *bus;
	DBusConnection  *conn;

	if (nm_proxy) {
		return TRUE;
	}

	twitux_debug (DEBUG_DOMAIN, "Initialising Network Manager proxy");

	bus = dbus_g_bus_get (DBUS_BUS_SYSTEM, NULL);
	if (!bus) {
		g_warning ("Could not connect to system bus");
		return FALSE;
	}

	conn = dbus_g_connection_get_connection (bus);
	dbus_connection_set_exit_on_disconnect (conn, FALSE);

	nm_proxy = dbus_g_proxy_new_for_name (bus,
										  NM_DBUS_SERVICE,
										  NM_DBUS_PATH,
										  NM_DBUS_INTERFACE);

	if (!nm_proxy) {
		/* Don't warn since the user might not have nm. */
		return FALSE;
	}

	g_object_weak_ref (G_OBJECT (nm_proxy), dbus_nm_proxy_notify_cb, NULL);

	dbus_g_object_register_marshaller (g_cclosure_marshal_VOID__UINT,
									   G_TYPE_NONE, G_TYPE_UINT, G_TYPE_INVALID);

	dbus_g_proxy_add_signal (nm_proxy, "StateChange",
							 G_TYPE_UINT, G_TYPE_INVALID);

	dbus_g_proxy_connect_signal (nm_proxy, "StateChange",
								 G_CALLBACK (dbus_nm_state_cb),
								 NULL, NULL);

	return TRUE;
}

gboolean
twitux_dbus_nm_get_state (gboolean *connected)
{
	GError  *error = NULL;
	guint32  state;

	g_return_val_if_fail (connected != NULL, FALSE);

	/* Set the initial value of connected in case we have to return */
	*connected = FALSE;

	if (nm_proxy_restart_timeout_id) {
		/* We are still trying to reconnect to the restarted bus */
		return FALSE;
	}

	/* Make sure we have set up Network Manager connections */
	if (!twitux_dbus_nm_init ()) {
		return FALSE;
	}

	if (!dbus_g_proxy_call (nm_proxy, "state", &error,
							G_TYPE_INVALID,
							G_TYPE_UINT, &state, G_TYPE_INVALID)) {
		twitux_debug (DEBUG_DOMAIN, "Failed to complete 'state' request. %s",
					  error->message);
		return FALSE;
	}

	twitux_debug (DEBUG_DOMAIN, "Current network state:'%s'",
				  dbus_nm_state_to_string (state));

	if (connected) {
		*connected = state == NM_STATE_CONNECTED;
	}

	return TRUE;
}
