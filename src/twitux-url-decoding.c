/* -*- Mode: C; tab-width: 8; shiftwidth: 8; indent-tabs-mode: t; c-basic-offset: r -*- */
/*
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
 * Authors: Kaity G. B. <uberChicGeekChick@openSUSE.us>
 */
	
#include "config.h"

#include <string.h>

#include <gtk/gtk.h>

#include <glib/gi18n.h>

#include <libtwitux/twitux-conf.h>
#include <libtwitux/twitux-debug.h>

#include "twitux-url-encoding.h"

#define DEBUG_DOMAIN_SETUP    "URLEncoding"

gchar * url_encode_message(gchar *text){
	char *good="~-._"; // RFC 3986
	char hex[16] = "0123456789ABCDEF";
	GString *result;
	
	if( text == NULL  || *text == '\0' )
		return NULL;
	
	result=g_string_new(NULL);
	while(*text) {
		unsigned char c=*text++;
		
		if (g_ascii_isalnum (c) || strchr(good, c))
			g_string_append_c(result, c);
		else {
			g_string_append_c(result, '%');
			g_string_append_c(result, hex[c >> 4]);
			g_string_append_c(result, hex[c & 0xf]);
		}
	}
	
	return g_string_free( result, FALSE );
}

GString * url_decode_message(gchar *text){
	//const char  hex[16] = "0123456789ABCDEF";
	GString           *result;
	
	g_return_val_if_fail (text != NULL, NULL);
	g_return_val_if_fail (*text != '\0', NULL);
	
	result = g_string_new (NULL);
	while(*text) {
		unsigned char c = *text++;

		if(c!='&'){
			g_string_append_c(result, c);
			continue;
		}

		GString *hex_string=g_string_new(NULL);
		while(c!=';'&&text){
			c=*text++;
			
			g_string_append_c(hex_string, c);
		}
		c=*text++;
		if( (g_str_equal(hex_string, "amp")) ){
			g_string_append_c(result, '&');
		}
		g_string_free(hex_string, TRUE);
	}
	
	return result;
}

