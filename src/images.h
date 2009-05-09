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

#ifndef __IMAGES_H__
#define __IMAGES_H__

#include <glib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>


G_BEGIN_DECLS

enum {
	ImagesMaximum		=	96,
	ImagesExpanded		=	72,
	ImagesDefault		=	48,
	ImagesMinimum		=	24,
};


gchar *images_get_filename(const gchar *url);

GtkImage *images_get_image_from_filename(const gchar *image_filename);
GtkImage *images_get_scaled_image_from_filename(const gchar *image_filename, gint width, gint height );

GtkImage *images_get_expanded_image_from_filename(const gchar *image_filename);
GtkImage *images_get_maximized_image_from_filename(const gchar *image_filename);
GtkImage *images_get_default_image_from_filename(const gchar *image_filename);
GtkImage *images_get_minimized_image_from_filename(const gchar *image_filename);

GdkPixbuf *images_get_expanded_pixbuf_from_filename(const gchar *image_filename);
GdkPixbuf *images_get_maximized_pixbuf_from_filename(const gchar *image_filename);
GdkPixbuf *images_get_default_pixbuf_from_filename(const gchar *image_filename);
GdkPixbuf *images_get_minimized_pixbuf_from_filename(const gchar *image_filename);

GdkPixbuf *images_expand_pixbuf(GdkPixbuf *pixbuf);
GdkPixbuf *images_maximize_pixbuf(GdkPixbuf *pixbuf);
GdkPixbuf *images_normalize_pixbuf(GdkPixbuf *pixbuf);
GdkPixbuf *images_minimize_pixbuf(GdkPixbuf *pixbuf);

GdkPixbuf *images_scale_pixbuf(GdkPixbuf *pixbuf, gint width, gint height);
GdkPixbuf *images_get_pixbuf_from_filename(const gchar *image_filename);
GdkPixbuf *images_get_unscaled_pixbuf_from_filename(const gchar *image_filename);
GdkPixbuf *images_get_scaled_pixbuf_from_filename(const gchar *image_filename, gint width, gint height);
GdkPixbuf *images_get_and_scale_pixbuf_from_filename(const gchar *image_filename, gint width, gint height);

G_END_DECLS

#endif /* __IMAGES_H__ */
