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
#define _GNU_SOURCE
#define _THREAD_SAFE



#include "config.h"

#include <gio/gio.h>
#include <glib/gi18n.h>

#include "online-services.h"
#include "about.h"


#define	DEBUG_DOMAINS	"ui:gtkbuilder:gtkbuildable:information:settings:artistry:graphics:programming:code:about.c"
#include "debug.h"

const gchar *authors[] = {
	"Kaity G. B. <uberChick@uberChicGeekChick.Com>",
	"Aaron Taddei <aarontaddei@gmail.com>",
	"Lord Drachenblut <lord.drachenblut@gmail.com>",
	NULL
};

const gchar *artists[] = {
	"Kaity G. B. <uberChick@uberChicGeekChick.Com>",
	NULL
};

const gchar *license[] = {
	N_("Unless explicitly acquired and licensed from Licensor under another"
		"license, the contents of this file are subject to the Reciprocal Public"
		"License (\"RPL\") Version 1.5, or subsequent versions as allowed by the RPL,"
		"and You may not copy or use this file in either source code or executable"
		"form, except in compliance with the terms and conditions of the RPL."),
	N_("All software distributed under the RPL is provided strictly on an"
		"\"AS IS\" basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, AND"
		"LICENSOR HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT"
		"LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR"
		"PURPOSE, QUIET ENJOYMENT, OR NON-INFRINGEMENT. See the RPL for specific"
		"language governing rights and limitations under the RPL."),
	N_("The User-Visible Attribution Notice below, when provided, must appear in each"
		"user-visible display as defined in Section 6.4 (d):"),
	N_("Initial art work including: design, logic, programming, and graphics are"
		"Copyright (C) 2009 Kaity G. B. and released under the RPL where sapplicable."
		"All materials not covered under the terms of the RPL are all still"
		"Copyright (C) 2009 Kaity G. B. and released under the terms of the"
		"Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license."),
	N_("Any & all data stored by this Software created, generated and/or uploaded by any User"
		"and any data gathered by the Software that connects back to the User.  All data stored"
		"by this Software is Copyright (C) of the User the data is connected to."
		"Users may lisences their data under the terms of an OSI approved or Creative Commons"
		"license.  Users must be allowed to select their choice of license for each piece of data"
		"on an individual bases and cannot be blanketly applied to all of the Users.  The User may"
		"select a default license for their data.  All of the Software's data pertaining to each"
		"User must be fully accessible, exportable, and deletable to that User."),
	NULL
};


void about_dialog_new (GtkWindow *parent){
	gtk_about_dialog_set_url_hook((GtkAboutDialogActivateLinkFunc)online_services_open_uri, NULL, NULL);
	
	gchar *license_trans=g_strconcat( _(license[0]), "\n\n", _(license[1]), "\n\n", _(license[2]), "\n\n", _(license[3]), "\n\n", _(license[4]), NULL );
	gtk_show_about_dialog(
				parent,
					"authors", authors,
					"artists", artists,
					"comments", _("GNOME's micro-blogging client with support for Twitter.com and any StatusNet instance, e.g. Identi.ca(of course)."),
					"copyright", _("Copyright \xc2\xa9 2009 Kaity G. B."),
					"license", license_trans,
					"translator-credits", _("translator-credits"),
					"version", PACKAGE_VERSION,
					"website", PACKAGE_BUGREPORT,
					"wrap-license", TRUE,
					"logo-icon-name", GETTEXT_PACKAGE,
				NULL
	);
	g_free(license_trans);
}
