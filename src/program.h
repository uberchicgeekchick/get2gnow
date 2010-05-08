/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the RPL
 *
 * For more information or to find the latest release, visit our
 * website at: http://uberChicGeekChick.Com/?projects=get2gnow
 *
 * Writen by an uberChick, other uberChicks please meet me & others @:
 * 	http://uberChicks.Net/
 *
 * I'm also disabled. I live with a progressive neuro-muscular disease.
 * DYT1+ Early-Onset Generalized Dystonia, a type of Generalized Dystonia.
 * 	http://Dystonia-DREAMS.Org/
 *
 *
 *
 * Unless explicitly acquired and licensed from Licensor under another
 * license, the contents of this file are subject to the Reciprocal Public
 * License ("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
 * and You may not copy or use this file in either source code or executable
 * form, except in compliance with the terms and conditions of the RPL.
 *
 * All software distributed under the RPL is provided strictly on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, AND
 * LICENSOR HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, QUIET ENJOYMENT, OR NON-INFRINGEMENT. See the RPL for specific
 * language governing rights and limitations under the RPL.
 *
 * The User-Visible Attribution Notice below, when provided, must appear in each
 * user-visible display as defined in Section 6.4 (d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright (C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright (C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright (C) of the User the data is connected to.
 * Users may lisences their data under the terms of an OSI approved or Creative Commons
 * license.  Users must be allowed to select their choice of license for each piece of data
 * on an individual bases and cannot be blanketly applied to all of the Users.  The User may
 * select a default license for their data.  All of the Software's data pertaining to each
 * User must be fully accessible, exportable, and deletable to that User.
 */

/**********************************************************************
 *          My art, code, & programming.                              *
 **********************************************************************/
#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#ifndef	_GNU_SOURCE 
#	define _GNU_SOURCE
#endif

#ifndef _THREAD_SAFE
#	define _THREAD_SAFE
#endif


/**********************************************************************
 *        System & library headers, eg #include <gdk/gdkkeysyms.h>    *
 **********************************************************************/

#include <glib.h>
#include <glib/gi18n.h>

#include <libgnome/libgnome.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "config.h"
#include "main.h"

G_BEGIN_DECLS
/**********************************************************************
 *        Objects, structures, and etc typedefs                       *
 **********************************************************************/
#ifndef CONFIG_SUBDIR
#	if defined GNOME_ENABLE_DEBUG
#		define	CONFIG_SUBDIR		PACKAGE_TARNAME "-debug"
#	else
#		define	CONFIG_SUBDIR		PACKAGE_TARNAME
#	endif
#endif

#ifndef	G_STR_EMPTY
#	define		G_STR_EMPTY(string)			(((string)==NULL) || (string)[0]=='\0')
#endif


#ifndef	G_STR_N_EMPTY
#	define		G_STR_N_EMPTY(string)			(((string)!=NULL) && (string)[0]!='\0')
#endif

#ifndef	uber_new
#	define	uber_new(mem, type)					\
	G_STMT_START{							\
		if(mem!=NULL){ g_free(mem); mem=NULL; }			\
		mem=g_new0(type, 1);					\
	}G_STMT_END
#endif

#ifndef uber_free
#	define	uber_free(mem)						\
	G_STMT_START{							\
		if(mem!=NULL){ g_free(mem); mem=NULL; }			\
	}G_STMT_END
#endif

#ifndef uber_list_free
#	define uber_list_free(l)					\
	G_STMT_START{							\
		if(l!=NULL){ g_list_free(l); l=NULL; }			\
	}G_STMT_END
#endif

#ifndef uber_slist_free
#	define uber_slist_free(sl)					\
	G_STMT_START{							\
		if(sl!=NULL){ g_slist_free(sl); sl=NULL; }		\
	}G_STMT_END
#endif

#ifndef uber_error_free
#	define	uber_error_free(error)					\
	G_STMT_START{							\
		if(error!=NULL){ g_error_free(error); error=NULL; }	\
	}G_STMT_END
#endif

#ifndef uber_object_unref
#	define	uber_object_unref(o)					\
	G_STMT_START{							\
		if(o!=NULL){ g_object_unref(o); o=NULL; }		\
	}G_STMT_END
#endif

#ifndef uber_regex_unref
#	define	uber_regex_unref(regex)					\
	G_STMT_START{							\
		if(regex!=NULL){ g_regex_unref(regex); regex=NULL; }	\
	}G_STMT_END
#endif

#ifndef	g_strcasecmp
#	define	g_strcasecmp(str1, str2)			(gboolean)!strcasecmp(str1, str2)
#endif

#ifndef g_str_n_equal
#	define	g_str_n_equal(string1, string2)			!g_str_equal(string1, string2)
#endif

#ifndef	gtk_widget_is_visible
#	define	gtk_widget_is_visible(widget)			program_gtk_widget_get_gboolean_property_value(GTK_WIDGET(widget), "visible")
#endif

#ifndef	gtk_widget_is_sensitive
#	define	gtk_widget_is_sensitive(widget)			program_gtk_widget_get_gboolean_property_value(GTK_WIDGET(widget), "sensitive")
#endif

#ifndef	gtk_widget_has_focus
#	define	gtk_widget_has_focus(widget)			program_gtk_widget_get_gboolean_property_value(GTK_WIDGET(widget), "has-focus")
#endif

#ifndef	gtk_widget_toggle_visibility
#	define	gtk_widget_toggle_visibility(widget)											\
	G_STMT_START{															\
		if(widget && GTK_IS_WIDGET(widget))											\
			if(!(gtk_widget_is_visible(widget)))										\
				gtk_widget_show(widget);										\
			else														\
				gtk_widget_hide(widget);										\
	}G_STMT_END
#endif

#ifndef	uber_object_free
#	if defined(G_HAVE_ISO_VARARGS)
#		define	uber_object_free(...)			program_uber_object_free(__VA_ARGS__)
#	elif defined(G_HAVE_GNUC_VARARGS)
#		define	uber_object_free(fmt...)		program_uber_object_free(fmt)
#	else
#		define	uber_object_free(x)			program_uber_object_free(x)
#	endif
#endif


#ifndef	uber_str_equal
#	if defined(G_HAVE_ISO_VARARGS)
#		define uber_str_equal(...)			program_uber_g_str_equal(__VA_ARGS__)
#	elif defined(G_HAVE_GNUC_VARARGS)
#		define uber_str_equal(fmt...)			program_uber_g_str_equal(fmt)
#	else
#		define uber_str_equal(x)			program_uber_g_str_equal(x)
#	endif
#endif

#ifndef uber_g_str_n_equal
#	define		uber_str_n_equal			!uber_str_equal
#endif

#ifndef	gdouble_to_str
#	define	gdouble_to_str					program_gdouble_drop_precision
#endif


/**********************************************************************
 *          Global method  & function prototypes                      *
 **********************************************************************/
gboolean program_init(int argc, char **argv);

const gchar *program_gtk_response_to_string(gint response);

void program_uber_object_free(gpointer pointer1, ...) G_GNUC_NULL_TERMINATED;
gchar *program_gdouble_drop_precision(const gdouble gdouble_value);
gboolean program_uber_g_str_equal(gchar *string_cmp_against, gchar *string_cmp1, ...);

gboolean program_gtk_widget_get_gboolean_property_value(GtkWidget *widget, const gchar *property);
void program_timeout_remove(guint *id, const gchar *usage);

void program_deinit(void);


G_END_DECLS
#endif /* __PROGRAM_H__ */
/**********************************************************************
 *                               eof                                  *
 **********************************************************************/

