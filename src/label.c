/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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


#define _GNU_SOURCE
#define _THREAD_SAFE

#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <libsexy/sexy.h>

#include "config.h"
#include "program.h"

#include "main-window.h"
#include "label.h"
#include "tweets.h"
#include "network.h"
#include "parser.h"
#include "gconfig.h"
#include "online-services.h"
#include "preferences.h"

#define	DEBUG_DOMAINS	"UI:Parse:Sexy:URLs:TweetView:OnlineServices:Networking:Tweets:XPath:Auto-Magical:Label.c"
#include "debug.h"

static void label_class_init(LabelClass *klass);
static void label_init(Label *label);
static void label_finalize(GObject *object);

static void label_url_activated_cb(GtkWidget *url_label, gchar *url, gpointer user_data);
static gssize find_first_non_user_name(const gchar *str);
static gchar *label_format_user_at_link(OnlineService *service, const gchar *at_url_prefix, gchar *users_at, gboolean expand_profiles, gboolean expand_hyperlinks, gboolean make_hyperlinks, gboolean titles_strip_uris);
static gchar *label_find_user_title(OnlineService *service, const gchar *uri, gboolean expand_hyperlinks, gboolean make_hyperlinks);
static gchar *label_find_uri_title(OnlineService *service, const gchar *uri, gboolean expand_hyperlinks, gboolean make_hyperlinks, gboolean include_uris);


G_DEFINE_TYPE (Label, label, SEXY_TYPE_URL_LABEL);

static void label_class_init(LabelClass *klass){
	GObjectClass   *object_class=G_OBJECT_CLASS(klass);
	/* Add private */
	object_class->finalize=label_finalize;
}

static void label_init(Label *label){
	gtk_label_set_line_wrap(GTK_LABEL (label), TRUE);
	g_object_set(label, "xalign", 0.0, "yalign", 0.0, "xpad", 0, "ypad", 0, NULL);
	g_signal_connect(label, "url-activated", G_CALLBACK(label_url_activated_cb), NULL);
}

Label *label_new(void){
	return g_object_new(TYPE_LABEL, NULL);
}

static void label_finalize(GObject *object){
	/* Cleanup code */
}

static void label_url_activated_cb(GtkWidget *url_label, gchar *url, gpointer user_data){
	if(!g_app_info_launch_default_for_uri(url, NULL, NULL))
		g_warning("Couldn't show URL: '%s'", url);
}

void label_set_text(OnlineService *service, Label *my_sexy_label, const gchar *text, gboolean expand_hyperlinks, gboolean make_hyperlinks){
	if(G_STR_EMPTY(text)){
		gtk_label_set_text(GTK_LABEL(my_sexy_label), "");
		return;
	}
	
	debug("Rendering sexy markup from text: %s", text);
	gchar *sexy_text=label_msg_format_urls(service, text, expand_hyperlinks, make_hyperlinks);
	sexy_url_label_set_markup(SEXY_URL_LABEL(my_sexy_label), sexy_text);
	debug("Rendered sexy markup: %s", sexy_text);
	uber_free(sexy_text);
}/*label_set_text*/

static gboolean url_check_word (char *word, int len){
#define D(x) (x), ((sizeof (x)) - 1)
	static const struct {
		const char *s;
		gint        len;
	}
	
	prefix[] = {
		{ D("ftp.") },
		{ D("www.") },
		{ D("ftp://") },
		{ D("http://") },
		{ D("@") },
		{ D("https://") },
		{ D("irc://") },
		{ D("irc.") },
	};
#undef D

	gint 		i;
	
	for (i = 0; i < G_N_ELEMENTS(prefix); i++) {
		int l;

		l = prefix[i].len;
		if (len > l) {
			gint j;

			/* This is pretty much strncasecmp(). */
			for (j = 0; j < l; j++)	{
				unsigned char c = word[j];
				
				if (g_ascii_tolower (c) != prefix[i].s[j])
					break;
			}
			if (j == l)
				return TRUE;
		}
	}
	
	return FALSE;
}

static gssize find_first_non_user_name(const gchar *str){
	gssize i;

	for (i = 0; str[i]; ++i) {
		if (!(g_ascii_isalnum(str[i]) || str[i] == '_')) {
			return i;
		}
	}
	return -1;
}

gchar *label_msg_format_urls(OnlineService *service, const gchar *message, gboolean expand_hyperlinks, gboolean make_hyperlinks){
	if(G_STR_EMPTY(message)) return g_strdup("");
	
	gchar **tokens=NULL, *result=NULL, *temp=NULL, *at_url_prefix=g_strdup_printf("http%s://%s", (online_service_is_secure(service) ?"s" :""), online_service_get_uri(service) );
	gboolean titles_strip_uris=gconfig_if_bool(PREFS_URLS_EXPANSION_REPLACE_WITH_TITLES, TRUE), expand_profiles=gconfig_if_bool(PREFS_URLS_EXPANSION_USER_PROFILES, TRUE);
	
	tokens=g_strsplit_set(message, " \t\n", 0);
	for(gint i=0; tokens[i]; i++) {
		if(url_check_word(tokens[i], strlen(tokens[i]))) {
			if(tokens[i][0]=='@') {
				debug("Rendering user @ link for user: '%s' on '%s'", tokens[i], online_service_get_uri(service));
				temp=label_format_user_at_link(service, at_url_prefix, tokens[i], expand_profiles, expand_hyperlinks, make_hyperlinks, titles_strip_uris);
				debug("Rendered user @ link.  %s will be replaced with %s.", tokens[i], temp);
			} else {
				debug("Rendering URI for display including title.  URI: '%s'.", tokens[i]);
				temp=label_find_uri_title(service, tokens[i], expand_hyperlinks, make_hyperlinks, titles_strip_uris);
				debug("Rendered URI for display.  %s will be replaced with %s.", tokens[i], temp);
			}
			g_free(tokens[i]);
			tokens[i]=temp;
			temp=NULL;
		}
	}
	result=g_strjoinv(" ", tokens);
	g_strfreev(tokens);
	g_free(at_url_prefix);
	
	return result;
}/*label_msg_format_urls*/

static gchar *label_format_user_at_link(OnlineService *service, const gchar *at_url_prefix, gchar *users_at, gboolean expand_profiles, gboolean expand_hyperlinks, gboolean make_hyperlinks, gboolean titles_strip_uris){
	gchar *user_at_link=NULL;
	gssize end=0;
	gchar delim;
	
	if( (end=(find_first_non_user_name(&users_at[1])+1) ) ){
		delim=users_at[end];
		users_at[end]='\0';
	}
	
	gchar *title=NULL;
	gchar *uri=g_strdup_printf("%s/%s", at_url_prefix, &users_at[1]);
	if(!expand_profiles)
		title=g_strdup("");
	else
		title=label_find_user_title(service, uri, expand_hyperlinks, make_hyperlinks);
	
	const gchar *hyperlink_suffix=( G_STR_N_EMPTY(title) ?"" :users_at );
	if(!make_hyperlinks)
		user_at_link=g_strdup_printf( "<u>%s%s</u>", title, hyperlink_suffix );
	else
		user_at_link=g_strdup_printf( "<a href=\"%s\">%s%s</a>", uri, title, hyperlink_suffix );
	
	if(end){
		gchar *user_at_link2=g_strdup_printf(
				"%s%c%s",
				user_at_link, delim, &users_at[end+1]
		);
		g_free(user_at_link);
		user_at_link=user_at_link2;
		user_at_link2=NULL;
	}
	uber_free(uri);
	uber_free(title);
	
	return user_at_link;
}/*label_format_user_at_link(service, "https://identi.ca/", "@user", expand_profiles, expand_hyperlinks, make_hyperlinks, titles_strip_uris);*/

static gchar *label_find_user_title(OnlineService *service, const gchar *uri, gboolean expand_hyperlinks, gboolean make_hyperlinks){
	gchar *title=NULL;
	gchar *title2=NULL;
	gchar *title_test=NULL;
	
	if(!make_hyperlinks)
		title_test=g_strdup_printf("<u>%s</u>", uri);
	else
		title_test=g_strdup_printf("<a href=\"%s\">%s</a>", uri, uri);
	
	title=label_find_uri_title(service, uri, expand_hyperlinks, make_hyperlinks, FALSE);
	
	if(!g_str_equal(title, title_test)){
		title2=g_strdup_printf("%s", title);
		g_free(title);
		title=title2;
		title2=NULL;
	}else{
		g_free(title);
		title=g_strdup("");
	}
	g_free(title_test);
	
	return title;
}/*label_find_user_title(service, uri, expand_hyperlinks, make_hyperlinks);*/

static gchar *label_find_uri_title(OnlineService *service, const gchar *uri, gboolean expand_hyperlinks, gboolean make_hyperlinks, gboolean include_uris){
	gchar *temp=NULL;
	if(!make_hyperlinks)
		temp=g_strdup_printf("<u>%s</u>", uri);
	else
		temp=g_strdup_printf("<a href=\"%s\">%s</a>", uri, uri);
	
	if(gconfig_if_bool(PREFS_URLS_EXPANSION_DISABLED, FALSE) || !expand_hyperlinks)
		return temp;
	
	SoupMessage *msg=NULL;
	gchar *content_type=NULL;
	
	main_window_statusbar_printf("Please wait while %s's title is found.", uri);
	debug("Attempting to determine content-type for: %s.", uri);
	if(!(content_type=online_service_get_uri_content_type(service, uri, &msg))){
		debug("Unable to determine the content-type from uri: '%s'.", uri);
		return temp;
	}
	
	
	if(!g_str_equal(content_type, "text/html")){
		debug("Non-XHTML content-type from uri: '%s'.", uri);
		g_free(content_type);
		return temp;
	}
	g_free(content_type);
	
	
	gchar *uri_title=NULL;
	debug("Retriving title for uri: '%s'.", uri);
	if(!(uri_title=parse_xpath_content(msg, "html->head->title")))
		return temp;
	
	g_free(temp);
	
	gchar *escaped_title=NULL;
	escaped_title=parser_escape_text(uri_title);
	g_free(uri_title);
	
	debug("Attempting to display link info. title: %s for uri: '%s'.", escaped_title, uri);
	gchar *hyperlink_suffix=NULL;
	if(include_uris)
		hyperlink_suffix=g_strdup_printf(" &lt;- %s", uri);
	
	if(!make_hyperlinks)
		temp=g_strdup_printf("<u>%s%s</u>", escaped_title, (hyperlink_suffix ?hyperlink_suffix :""));
	else
		temp=g_strdup_printf("<a href=\"%s\">%s%s</a>", uri, escaped_title, (hyperlink_suffix ?hyperlink_suffix :""));
	if(hyperlink_suffix) g_free(hyperlink_suffix);
	hyperlink_suffix=NULL;
	
	g_free(escaped_title);
	
	return temp;
}/*label_find_uri_title*/
