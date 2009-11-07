/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
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
 * Authors: Brian Pepple <bpepple@fedoraproject.org>
 *
 */

#define _GNU_SOURCE
#define _THREAD_SAFE

#include <locale.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <glib/gprintf.h>

#include <libgnomeui/libgnomeui.h>

#include "config.h"
#include "program.h"

const gchar *old_locale=NULL;
gchar **remaining_argv=NULL;
gint remaining_argc=0;
GnomeProgram *program=NULL;

int main(const int argc, const char **argv, const char **envp){
	old_locale=setlocale(LC_TIME, "C");
	GOptionContext *option_context=g_option_context_new(GETTEXT_PACKAGE);
	GOptionEntry option_entries[]={
					{
						G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY,
						&remaining_argv,
						"Special option that collects any remaining arguments for us"
					},
					{ NULL }
	};
	
	g_option_context_add_main_entries(option_context, option_entries, NULL);
	
	program=gnome_program_init(
					GETTEXT_PACKAGE, PACKAGE_VERSION PACKAGE_RELEASE,
					LIBGNOMEUI_MODULE,
					(gint)argc, (gchar **)argv,
					GNOME_PARAM_GOPTION_CONTEXT, option_context,
					GNOME_PARAM_NONE
	);
	
	if(remaining_argv)
		remaining_argc=g_strv_length(remaining_argv)-1;
	
	if(!program_init(remaining_argc, remaining_argv)){
		setlocale(LC_TIME, old_locale);
		exit(0);
	}
	
	gtk_main();
	
	program_deinit();
	
	setlocale(LC_TIME, old_locale);
	
	return 0;
}/*int main(int argc, const char **argv, const char **envp);*/

void get2gnow_program_deinit(void){
	program_deinit();
}/*get2gnow_program_deinit();*/

