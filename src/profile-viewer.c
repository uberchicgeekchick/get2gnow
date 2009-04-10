/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Greet-Tweet-Know is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the RPL
 *
 * For more information or to find the latest release, visit our
 * website at: http://uberChicGeekChick.Com/?projects=Greet-Tweet-Know
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

#include <glib.h>
#include <strings.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "debug.h"
#include "friends-manager.h"
#include "users.h"
#include "gtkbuilder.h"
#include "network.h"
#include "images.h"
#include "parser.h"
#include "label.h"
#include "profile-viewer.h"

#define GtkBuilderUI "profile-viewer.ui"

#define DEBUG_DOMAIN "Profile-Viewer"

typedef struct {
	User			*user;
	GtkMessageDialog	*dialog;
	GtkImage		*image;
	GtkLabel		*loading_label;
	GtkWidget		*title_vbox;
	GtkWidget		*user_title;
	GtkWidget		*profile_vbox;
	GtkWidget		*url_hyperlink;
	GtkWidget		*bio_html;
} ProfileViewer;



void view_profile( gchar *user_name, GtkWindow *parent ){
	gchar	*profile_details;
	
	ProfileViewer *profile_viewer=g_new( ProfileViewer, 1 );
	
	GtkBuilder *ui=gtkbuilder_get_file(
					GtkBuilderUI,
					"profile_dialog", &profile_viewer->dialog,
					"loading_label", &profile_viewer->loading_label,
					"title_vbox", &profile_viewer->title_vbox,
					"profile_vbox", &profile_viewer->profile_vbox,
					NULL
	);
	
	gtkbuilder_connect(ui, profile_viewer,
					"profile_dialog", "destroy", gtk_widget_destroy,
					"profile_dialog", "response", gtk_widget_destroy,
					NULL
	);
	
	gtk_window_set_transient_for(GTK_WINDOW(profile_viewer->dialog), parent);
	
	gtk_widget_show_all( GTK_WIDGET( profile_viewer->dialog ) );
	
	profile_viewer->user=network_fetch_profile(user_name);
	profile_details=g_strdup_printf(
					"\t<u><b>%s:</b> @%s</u>\n",
					profile_viewer->user->nick_name, profile_viewer->user->user_name
	);
	
	
	profile_viewer->user_title=label_new();
	gtk_widget_show(GTK_WIDGET(profile_viewer->user_title));
	gtk_box_pack_start(
				GTK_BOX(profile_viewer->title_vbox),
				GTK_WIDGET(profile_viewer->user_title),
				TRUE, TRUE, 0
	);
	label_set_text(LABEL(profile_viewer->user_title), profile_details);
	g_free( profile_details );
	
	profile_details=g_strdup_printf(
					"\t<b>Tweets:</b> %lu\n\t<b>Friends:</b> %lu\n\t<b>Followers:</b> %lu\n\t<b>Location:</b> %s\n",
					profile_viewer->user->tweets,
					profile_viewer->user->friends,
					profile_viewer->user->followers,
					profile_viewer->user->location
	);
	gtk_message_dialog_set_markup( profile_viewer->dialog, profile_details );
	g_free( profile_details );
	
	profile_details=g_strdup_printf( "\t<b>URL:</b>\t<a href=\"%s\">%s</a>\n", profile_viewer->user->url, profile_viewer->user->url );
	profile_viewer->url_hyperlink=label_new();
	gtk_widget_show(GTK_WIDGET(profile_viewer->url_hyperlink));
	gtk_box_pack_start(
				GTK_BOX(profile_viewer->profile_vbox),
				GTK_WIDGET(profile_viewer->url_hyperlink),
				TRUE, TRUE, 0
	);
	label_set_text(LABEL(profile_viewer->url_hyperlink), profile_details);
	g_free( profile_details );
	
	profile_details=g_strdup_printf( "\t<b>Bio:</b>\n\t\t%s\n", profile_viewer->user->bio );
	profile_viewer->bio_html=label_new();
	gtk_widget_show(GTK_WIDGET(profile_viewer->bio_html));
	gtk_box_pack_end(
				GTK_BOX(profile_viewer->profile_vbox),
				GTK_WIDGET(profile_viewer->bio_html),
				TRUE, TRUE, 0
	);
	label_set_text(LABEL(profile_viewer->bio_html), profile_details);
	g_free( profile_details );

	network_download_avatar( profile_viewer->user->image_url );
	if(!(g_str_equal("unknown_image", profile_viewer->user->image_filename)))
		gtk_message_dialog_set_image( profile_viewer->dialog, GTK_WIDGET(profile_viewer->image=images_get_maximized_image_from_filename( profile_viewer->user->image_filename )) );
	
	
	gtk_widget_show_all( GTK_WIDGET( profile_viewer->dialog ) );
	gtk_widget_hide( GTK_WIDGET( profile_viewer->loading_label ) );
	
	user_free( profile_viewer->user );
	
}//user_popup_profile

