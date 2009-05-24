/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 * Copyright (C) 2007-2008 Daniel Morales <daniminas@gmail.com>
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
 * Authors: Daniel Morales <daniminas@gmail.com>
 *
 */

#include "config.h"

#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <libsexy/sexy.h>

#include "main.h"
#include "app.h"
#include "label.h"
#include "tweets.h"
#include "network.h"
#include "parser.h"
#include "gconfig.h"
#include "online-services.h"
#include "preferences.h"

#define	DEBUG_DOMAIN	"UI:Lable"
#include "debug.h"

static void label_class_init(LabelClass *klass);
static void label_init(Label *label);
static void label_finalize(GObject *object);

static void label_url_activated_cb(GtkWidget *url_label, gchar *url, gpointer user_data);
static gchar *label_find_uri_title(OnlineService *service, const gchar *uri, gboolean expand_hyperlinks, gboolean make_hyperlinks);


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

static void label_finalize(GObject *object){
	/* Cleanup code */
}

static void label_url_activated_cb(GtkWidget *url_label, gchar *url, gpointer user_data){
	if(!g_app_info_launch_default_for_uri(url, NULL, NULL))
		g_warning("Couldn't show URL: '%s'", url);
}

Label *label_new (void){
	return g_object_new(TYPE_LABEL, NULL);
}

void label_set_text(OnlineService *service, Label *my_sexy_label, const gchar  *text, gboolean expand_hyperlinks, gboolean make_hyperlinks){
	if(G_STR_EMPTY(text)){
		sexy_url_label_set_markup(SEXY_URL_LABEL(my_sexy_label), "");
		return;
	}
	
	gchar *parsed_text=label_msg_format_urls(service, text, expand_hyperlinks, make_hyperlinks);
	sexy_url_label_set_markup(SEXY_URL_LABEL(my_sexy_label), parsed_text);
	g_free(parsed_text);
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
		{ D("https://") },
		{ D("@") },
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

static gssize find_first_non_username(const char *str){
	gssize i;

	for (i = 0; str[i]; ++i) {
		if (!(g_ascii_isalnum(str[i]) || str[i] == '_')) {
			return i;
		}
	}
	return -1;
}

gchar *label_msg_format_urls(OnlineService *service, const char *message, gboolean expand_hyperlinks, gboolean make_hyperlinks){
	if(G_STR_EMPTY(message)) return g_strdup("");
	
	gchar **tokens;
	gchar  *result;
	gchar  *temp;
	gint 	i;
	
	/* TODO: Do we need to escape out <>&"' so that pango markup doesn't get confused? */
	gchar *at_url_prefix=g_strdup_printf("http://%s", ( (service && service->uri) ?service->uri :"twitter.com" ));
	
	/* surround urls with <a> markup so that sexy-url-label can link it */
	tokens=g_strsplit_set(message, " \t\n", 0);
	for(i=0; tokens[i]; i++) {
		if(url_check_word(tokens[i], strlen(tokens[i]))) {
			if(tokens[i][0]=='@') {
				gssize end;
				gchar *title2=NULL;
				gchar *uri=g_strdup_printf("%s/%s", at_url_prefix, &tokens[i][1]);
				gchar *title_test=NULL;
				if(!make_hyperlinks)
					title_test=g_strdup_printf("<u>%s</u>", uri);
				else
					title_test=g_strdup_printf("<a href=\"%s\">%s</a>", uri, uri);
				
				gchar *title=label_find_uri_title(service, uri, expand_hyperlinks, make_hyperlinks);
				if(!g_str_equal(title, title_test)){
					title2=g_strdup_printf("%s ", title);
					g_free(title);
					title=title2;
					title2=NULL;
				}else{
					g_free(title);
					title=g_strdup("");
				}
				g_free(title_test);
				if(!make_hyperlinks)
					temp=g_strdup_printf("<u>%s%s</u>", title, tokens[i]);
				else
					temp=g_strdup_printf(
								"<a href=\"%s\">%s%s</a>",
									uri, title, tokens[i]
					);
				
				if( (end=( find_first_non_username(&tokens[i][1])+1 ) )){
					gchar delim=tokens[i][end];
					tokens[i][end]='\0';
					gchar *temp2=g_strdup_printf(
									"%s%c%s",
									temp, delim, &tokens[i][end+1]
					);
					g_free(temp);
					temp=temp2;
					temp2=NULL;
				}
				g_free(uri);
				g_free(title);
			} else {
				debug("Attempting to display title for uri: '%s'.", tokens[i]);
				temp=label_find_uri_title(service, tokens[i], expand_hyperlinks, make_hyperlinks);
			}
			app_set_statusbar_msg(TWEETS_RETURN_MODIFIERS_STATUSBAR_MSG);
			g_free(tokens[i]);
			tokens[i]=temp;
		}
	}
	result = g_strjoinv(" ", tokens);
	g_strfreev (tokens);
	g_free(at_url_prefix);
	
	return result;	
}/*label_msg_format_urls*/

static gchar *label_find_uri_title(OnlineService *service, const gchar *uri, gboolean expand_hyperlinks, gboolean make_hyperlinks){
	gchar *temp=NULL;
	if(!(make_hyperlinks))
		temp=g_strdup_printf("<u>%s</u>", uri);
	else
		temp=g_strdup_printf("<a href=\"%s\">%s</a>", uri, uri);
	
	if(gconfig_if_bool(PREFS_URLS_EXPAND_DISABLED, FALSE) || !expand_hyperlinks)
		return temp;
	
	
	SoupMessage *msg=NULL;
	gchar *content_type=NULL;
	
	
	app_statusbar_printf("Please wait while %s's title is found.", uri);
	if(!(content_type=online_service_get_url_content_type(service, uri, &msg))){
		debug("\t\tUnable to determine the content-type from uri: '%s'.", uri);
		return temp;
	}
	
	
	if(!g_str_equal(content_type, "text/html")){
		debug("\t\tNon-XHTML content-type from uri: '%s'.", uri);
		g_free(content_type);
		return temp;
	}
	g_free(content_type);
	
	
	gchar *uri_title=NULL;
	debug("Retriving title for uri: '%s'.", uri);
	if(!(uri_title=parser_parse_xpath_content(msg, "html->head->title")))
		return temp;
	
	g_free(temp);
	
	gchar *escaped_title=NULL;
	gchar *cur=escaped_title=g_markup_escape_text(uri_title, -1);
	g_free(uri_title);
	while((cur = strstr(cur, "&amp;"))){
		if(strncmp(cur + 5, "lt;", 3) == 0 || strncmp(cur + 5, "gt;", 3) == 0)
			g_memmove(cur + 1, cur + 5, strlen(cur + 5) + 1);
		else
			cur += 5;
	}
	
	debug("Attempting to display link info.\n\t\t\ttitle: %s\n\t\t\tfor uri: '%s'.", escaped_title, uri);
	if(!make_hyperlinks)
		temp=g_strdup_printf("<u>%s &lt;- %s</u>", escaped_title, uri);
	else
		temp=g_strdup_printf("<a href=\"%s\">%s &lt;- %s</a>", uri, escaped_title, uri);
	
	g_free(escaped_title);
	
	return temp;
}/*label_find_uri_title*/
