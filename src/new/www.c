/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the Reciprocal Public License (RPL).
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
/********************************************************************************
 *                      My art, code, & programming.                            *
 ********************************************************************************/
#define _GNU_SOURCE
#define _THREAD_SAFE



/********************************************************************************
 *      Project, system, & library headers.  eg #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/
#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <libxml/parser.h>

#include "program.h"

#include "parser.h"
#include "www.h"

#include "online-service.h"

#include "gconfig.h"
#include "preferences.defines.h"

#include "uberchick-label.h"
#include "main-window.h"
#include "update-viewer.h"


/********************************************************************************
 *                        defines, macros, methods, & etc                       *
 ********************************************************************************/


/********************************************************************************
 *                        objects, structs, and enum typedefs                   *
 ********************************************************************************/
enum {
	COLUMN_URI,
	COLUMN_URI_TITLE,
	COLUMN_INSERT_TIME,
	COLUMN_COUNT,
};


/********************************************************************************
 *                prototypes for private methods & functions                    *
 ********************************************************************************/
static gboolean www_uri_check(gchar *url);
static gssize www_find_first_non_user_name(const gchar *str);

static gchar *www_format_service_hyperlink(OnlineService *service, const gchar *url_prefix, gchar *services_resource, gboolean expand_hyperlinks, gboolean make_hyperlinks);
static gchar *www_find_user_title(OnlineService *service, const gchar *uri, const gchar *services_resource, gboolean expand_hyperlinks, gboolean make_hyperlinks);
static gchar *www_find_uri_pages_title(OnlineService *service, const gchar *uri, const gchar *services_resource, gboolean expand_hyperlinks, gboolean make_hyperlinks);

static gchar *www_uri_title_lookup(const gchar *uri);
static gboolean www_uri_titles_clean_up(void);

/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static void www_format_uri_with_title(gchar **uri_title, const gchar *uri, const gchar *services_resource);
static gchar *www_get_uri_content_type(OnlineService *service, const gchar *uri, SoupMessage **xml);
static void www_uri_title_append(const gchar *uri, const gchar *title);


/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"UI:Sexy:URLs:URIs:Links:UberChickTreeView:OnlineServices:Networking:Updates:XPath:Auto-Magical:WWW:label.c"
#include "debug.h"

static GtkListStore *www_uri_title_lookup_table_list_store;
static gboolean www_uri_title_lookup_table_processing=FALSE;
static guint www_uri_title_lookup_table_clean_up_timeout_id=0;
static guint www_uri_title_list_store_total=0;
static GRegex *number_regex;


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
void www_init(void){
	GError *error=NULL;
	const gchar *g_regex_pattern="&amp;[0-9]+([ \n\r\t]+)?";
	number_regex=g_regex_new(g_regex_pattern, 0, G_REGEX_MATCH_NOTEOL, &error);
	if(error){
		debug("**ERROR:** creating GRegex using the pattern %s.  GError message: %s.", g_regex_pattern, error->message );
		g_error_free(error);
	}
	www_uri_title_lookup_table_list_store=gtk_list_store_new(COLUMN_COUNT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	www_uri_title_lookup_table_clean_up_timeout_id=g_timeout_add_seconds(600, (GSourceFunc)www_uri_titles_clean_up, NULL);
}/*www_init();*/

static gboolean www_uri_titles_clean_up(void){
	if(!www_uri_title_list_store_total) return TRUE;
	www_uri_title_lookup_table_processing=TRUE;
	for(gint i=www_uri_title_list_store_total; i>=0; i--){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(www_uri_title_lookup_table_list_store), iter, path)){
			debug("Retrieving iter from path to index %d failed.  Unable to remove row.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gchar *uri=NULL, *title=NULL;
		const gchar *datetime=NULL;
		gint created_ago=0;
		gtk_tree_model_get(
				GTK_TREE_MODEL(www_uri_title_lookup_table_list_store), iter,
					COLUMN_URI, &uri,
					COLUMN_URI_TITLE, &title,
					COLUMN_INSERT_TIME, &datetime,
				-1
		);
		if( (created_ago=update_convert_datetime_to_seconds_old(datetime, TRUE)) >= 3600 ){
			debug("Removing URIs: <%s>; title: [%s].  Which was stored on %s an is %d seconds old.", uri, title, datetime, created_ago);
			gtk_list_store_remove(www_uri_title_lookup_table_list_store, iter);
			if(www_uri_title_list_store_total)
				www_uri_title_list_store_total--;
		}
		uber_free(iter);
	}
	return !(www_uri_title_lookup_table_processing=FALSE);
}/*www_uri_titles_clean_up();*/

void www_deinit(void){
	g_regex_unref(number_regex);
	program_timeout_remove(&www_uri_title_lookup_table_clean_up_timeout_id, "URI Title clean-up timeout");
	uber_object_unref(www_uri_title_lookup_table_list_store);
}/*www_init();*/

void www_html_entity_escape_status(gchar **status_text){
	gchar *new_status=www_html_entity_escape_text( *status_text );
	uber_free( *status_text );
	*status_text=new_status;
	new_status=NULL;
}/*www_html_entity_escape_status(&status->text);*/

gchar *www_html_entity_escape_text(gchar *status_text){
	gchar *escaped_status=NULL;
	gchar *regex_status=NULL;
	gchar *current_character=escaped_status=g_markup_escape_text(status_text, -1);
	GMatchInfo *match_info;
	if(g_regex_match(number_regex, current_character, 0, &match_info)){
		GError *error=NULL;
		regex_status=g_regex_replace(number_regex, current_character, strlen(current_character), 0, "", 0, &error);
		if(!(error && G_STR_EMPTY(regex_status) )){
			uber_free(escaped_status);
			current_character=escaped_status=regex_status;
		}else{
			debug("**ERROR:** Parsing %s through g_regex_replace.  GError message: %s.", current_character, error->message );
			g_error_free(error);
		}
	}
	while((current_character=strstr(current_character, "&amp;"))) {
		if(!( strncmp(current_character+5, "lt;", 3) && strncmp(current_character+5, "gt;", 3) ))
			g_memmove(current_character+1, current_character+5, strlen(current_character+5)+1);
		else
			current_character+=5;
	}
	return escaped_status;
}/*www_html_entity_escape_text(status->text);*/

gboolean www_xml_error_check(OnlineService *service, const gchar *uri, SoupMessage *xml, xmlDoc **doc, xmlNote **root_element, gchar **error_message){
	if(!( SOUP_IS_MESSAGE(xml) && SOUP_STATUS_IS_SUCCESSFUL(xml->status_code) && xml->status_code>99 )){
		*error_message=g_strdup_printf("OnlineService: <%s> has returned an invalid or failed libsoup request.  The URI [%s] returned:: %s(%d).", service->key, uri, xml->reason_phrase, xml->status_code );
		return FALSE;
	}
	
	if( (*doc)!=NULL ){
		debug("**ERROR:** xmlDoc expects a null xmlDoc object");
		return FALSE;
	}
	
	if( (*root_element)!=NULL ){
		debug("**ERROR:** root_element expects a null xmlNode object");
		return FALSE;
	}
	
	if( (*error_message)!=NULL ){
		debug("**ERROR:** error_message expects a null gchar pointer.");
		return FALSE;
	}
	
	*error_message=g_strdup("");;
	
	if(xml->status_code==100||xml->status_code==200){
		debug("OnlineService: <%s>'s request for: %s has succeed.  HTTP status returned: %s(%d).", service->key, uri, xml->reason_phrase, xml->status_code);
		return TRUE;
	}
	
	if(xml->status_code==401 || xml->status_code==503){
		debug("**ERROR:** Authentication failed and/or access denied.  OnlineService: <%s> returned. %s(%d) when requesting [%s].", service->key, xml->reason_phrase, xml->status_code, uri );
		return FALSE;
	}
	
	debug("Getting content-type from uri: [%s].", uri);
	gchar **content_v=NULL;
	debug("Parsing xml document's content-type & DOM information from: [%s].", uri);
	gchar *content_info=NULL;
	if(!(content_info=g_strdup((gchar *)soup_message_headers_get_one(xml->response_headers, "Content-Type")))){
		debug("*ERROR:* Failed to determine content-type for:  [%s].", uri);
		return FALSE;
	}
	
	debug("Parsing content info: [%s] from: [%s].", content_info, uri);
	content_v=g_strsplit(content_info, "; ", -1);
	uber_free(content_info);
	gchar *content_type=NULL;
	if(!( ((content_v[0])) && (content_type=g_strdup(content_v[0])) )){
		debug("*ERROR:* Failed to determine content-type for:  [%s].", uri);
		g_strfreev(content_v);
		return FALSE;
	}
	debug("Parsed Content-Type: [%s] for: [%s].", content_type, uri);
	
	g_strfreev(content_v);
	if(!g_str_equal(content_type, "application/xml")){
		uber_free(content_type);
		return TRUE;
	}
	uber_free(content_type);
	
	debug("Parsing xml document to find any authentication errors.");
	if(!( (*doc=xmlReadMemory(xml->response_body->data, xml->response_body->length, "xml", "utf-8", (XML_PARSE_NOENT | XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING) )) )){
		debug("Unable to parse xml doc.");
		uber_free( *error_message );
		*error_message=g_strdup("Unable to parse xml doc.");
		xmlCleanupParser();
		return FALSE;
	}
	
	*root_element=xmlDocGetRootElement(*doc);
	if(root_element==NULL) {
		debug("Failed getting first element of xml data.");
		uber_free( *error_message );
		*error_message=g_strdup("Failed getting first element of xml data.");
		xmlFreeDoc(*doc);
		*doc=NULL;
		xmlCleanupParser();
		return FALSE;
	}
	
	gboolean error_free=TRUE;
	xmlNode	*current_node=NULL;
	for(current_node = root_element; current_node; current_node = current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE ) continue;
		
		if(!( g_str_equal(current_node->name, "error") )) continue;
		
		error_free=FALSE;
		uber_free( *error_message );
		*error_message=(gchar *)xmlNodeGetContent(current_node);
		break;
	}
	
	if(error_free)
		return TRUE;
	
	gchar *error_message_swap=g_strdup_printf("OnlineService: <%s> returned: %s(%d) with the error: %s.", service->key, xml->reason_phrase, xml->status_code, *error_message);
	uber_free((*error_message) );
	*error_message=error_message_swap;
	error_message_swap=NULL;
	debug("%s", *error_message);
	statusbar_printf( "%s", *error_message );
	
	xmlFreeDoc(*doc);
	*doc=NULL;
	xmlCleanupParser();
	
	return FALSE;
}/*www_xml_error_check(service, xml);*/

gchar *www_format_urls(OnlineService *service, const gchar *message, gboolean expand_hyperlinks, gboolean make_hyperlinks){
	if(G_STR_EMPTY(message)) return g_strdup("");
	
	gchar *result=NULL, *temp=NULL;
	gchar **words=g_strsplit_set(message, " \t\n\r", 0);
	for(gint i=0; words[i]; i++) {
		if(!( words[i][1] && www_uri_check(words[i]) )) continue;
		
		gboolean searching=FALSE;
		if(words[i][0]!='@' && !(searching=(words[i][0]=='!' || words[i][0]=='#')) ){
			const gchar *domain=g_strrstr( words[i], "@" );
			if(G_STR_N_EMPTY(domain) && strstr(domain, ".")){
				debug("Rendering E-mail address: <%s>", words[i]);
				if(!make_hyperlinks)
					temp=g_strdup_printf("<u>%s</u>", words[i]);
				else
					temp=g_strdup_printf("<a href=\"mailto:%s\">%s</a>", words[i], words[i]);
				uber_free(words[i]);
				words[i]=temp;
				temp=NULL;
				domain=NULL;
				continue;
			}
			domain=NULL;
			
			debug("Rendering URI for display including title.  URI: '%s'.", words[i]);
			temp=www_find_uri_pages_title(service, words[i], NULL, expand_hyperlinks, make_hyperlinks);
			debug("Rendered URI for display.  %s will be replaced with %s.", words[i], temp);
			uber_free(words[i]);
			words[i]=temp;
			temp=NULL;
			continue;
		}
		
		gchar *url_prefix=g_strdup_printf("http%s://%s%s/%s%s%s%s", (words[i][0]=='@'&&service->https ?"s" :""), ( (words[i][0]=='#' && service->micro_blogging_service==Twitter) ?"search." :"" ), service->uri, (searching ?"search" :""), (searching && service->micro_blogging_service!=Twitter ?"/notice" :"" ), (searching ?"?q=" :""), (words[i][0]=='#' ?"%23" :(words[i][0]=='!' ?"%21" :"") ) );
		debug("Rendering OnlineService: <%s>'s %s %c link for: <%s@%s>.", service->key, (words[i][0]=='@' ?"user profile" :"search results"), words[i][0], &words[i][1], service->uri);
		temp=www_format_service_hyperlink(service, url_prefix, words[i], expand_hyperlinks, make_hyperlinks);
		debug("Rendered OnlineService: <%s>'s %s %c link for: <%s@%s> will be replaced by: %s.", service->key, (words[i][0]=='@' ?"user profile" :"search results"), words[i][0], &words[i][1], service->uri, temp);
		uber_free(url_prefix);
		uber_free(words[i]);
		words[i]=temp;
		temp=NULL;
	}
	result=g_strjoinv(" ", words);
	g_strfreev(words);
	
	return result;
}/*www_format_urls(service, "text with http://plan-text.links/", (TRUE|FALSE), (TRUE|FALSE) );*/

static gboolean www_uri_check(gchar *uri){
#define	D(x)	{ (x), ((sizeof(x))-1) }
	static const struct {
		const char	*s;
		guint8		len;
	}
	
	prefix[] = {
		D("@"),
		D("!"),
		D("#"),
		D("ftp."),
		D("www."),
		D("irc."),
		D("ftp://"),
		D("irc://"),
		D("http://"),
		D("https://"),
		D("mailto:"),
	};
#undef D
	guint8 uri_length=strlen(uri);
	for(int i=0; i<G_N_ELEMENTS(prefix); i++){
		if(uri_length<prefix[i].len) continue;
		
		/* This is pretty much case in-sensitive g_str_has_prefix(). */
		int j;
		for(j=0; j<prefix[i].len; j++)
			if(g_ascii_tolower(uri[j])!=prefix[i].s[j]) break;
			
		if(j && j==prefix[i].len) return TRUE;
	}
	
	return FALSE;
}/*www_uri_check("text");*/

static gssize www_find_first_non_user_name(const gchar *str){
	for(gssize i=0; str[i]; ++i)
		if(!(g_ascii_isalnum(str[i]) || str[i] == '_'))
			return i;
	return -1;
}

static gchar *www_format_service_hyperlink(OnlineService *service, const gchar *url_prefix, gchar *services_resource, gboolean expand_hyperlinks, gboolean make_hyperlinks){
	gchar *user_at_link=NULL;
	gssize end=0;
	gchar delim;
	
	if((end=(www_find_first_non_user_name(&services_resource[1])+1) ) ){
		delim=services_resource[end];
		services_resource[end]='\0';
	}
	
	gchar *title=NULL;
	gchar *uri=g_strdup_printf("%s%s", url_prefix, &services_resource[1]);
	if(!gconfig_if_bool(PREFS_URLS_EXPANSION_USER_PROFILES, TRUE))
		title=g_strdup("");
	else
		title=www_find_user_title(service, uri, services_resource, expand_hyperlinks, make_hyperlinks);
	
	const gchar *hyperlink_suffix=( G_STR_N_EMPTY(title) ?"" :services_resource );
	if(!make_hyperlinks)
		user_at_link=g_strdup_printf( "<u>%s%s</u>", title, hyperlink_suffix );
	else if(G_STR_N_EMPTY(title))
		user_at_link=g_strdup(title);
	else
		user_at_link=g_strdup_printf( "<a href=\"%s\">%s%s</a>", uri, title, hyperlink_suffix );
	
	if(end){
		gchar *user_at_link2=g_strdup_printf(
				"%s%c%s",
				user_at_link, delim, &services_resource[end+1]
		);
		uber_free(user_at_link);
		user_at_link=user_at_link2;
		user_at_link2=NULL;
	}
	uber_free(uri);
	uber_free(title);
	
	return user_at_link;
}/*www_format_service_hyperlink(service, "https://identi.ca/", "@user"||"#search||"!tag", expand_hyperlinks, make_hyperlinks);*/

static gchar *www_find_user_title(OnlineService *service, const gchar *uri, const gchar *services_resource, gboolean expand_hyperlinks, gboolean make_hyperlinks){
	gchar *title=NULL;
	gchar *title_test=NULL;
	
	if(!make_hyperlinks)
		title_test=g_strdup_printf("<u>%s</u>", uri);
	else
		title_test=g_strdup_printf("<a href=\"%s\">%s</a>", uri, uri);
	
	title=www_find_uri_pages_title(service, uri, services_resource, expand_hyperlinks, make_hyperlinks);
	
	if(g_str_equal(title, title_test)){
		uber_free(title);
		title=g_strdup("");
	}
	uber_free(title_test);
	
	return title;
}/*www_find_user_title(service, uri, "@user"||"#search||"!tag", expand_hyperlinks, make_hyperlinks);*/

static gchar *www_find_uri_pages_title(OnlineService *service, const gchar *uri, const gchar *services_resource, gboolean expand_hyperlinks, gboolean make_hyperlinks){
	gchar *uri_title=NULL;
	gchar *temp=NULL;
	if( (uri_title=www_uri_title_lookup(uri)) ){
		www_format_uri_with_title(&uri_title, uri, services_resource);
		if(!make_hyperlinks)
			return g_strdup_printf("<u>%s</u>", uri_title);
		
		return g_strdup_printf("<a href=\"%s\">%s</a>", uri, uri_title);
	}
	
	if(!make_hyperlinks)
		temp=g_strdup_printf("<u>%s</u>", uri);
	else
		temp=g_strdup_printf("<a href=\"%s\">%s</a>", uri, uri);
	
	if(gconfig_if_bool(PREFS_URLS_EXPANSION_DISABLED, FALSE) || !expand_hyperlinks)
		return temp;
	
	if(g_str_has_prefix(uri, "ftp://")  || g_str_has_prefix(uri, "irc://") ){
		www_uri_title_append(uri, uri);
		return temp;
	}
	
	SoupMessage *xml=NULL;
	gchar *content_type=NULL;
	
	main_window_statusbar_printf("Please wait while %s's title is found.", uri);
	debug("Attempting to determine content-type for: %s.", uri);
	if(!(content_type=www_get_uri_content_type(service, uri, &xml))){
		debug("Unable to determine the content-type from uri: '%s'.", uri);
		www_uri_title_append(uri, uri);
		return temp;
	}
	
	if(!g_str_equal(content_type, "text/html")){
		debug("Non-XHTML content-type from uri: '%s'.", uri);
		uber_free(content_type);
		www_uri_title_append(uri, uri);
		return temp;
	}
	uber_free(content_type);
	
	gboolean searching=(services_resource && (services_resource[0]=='#' || services_resource[0]=='!' ));
	if(!(uri_title=www_get_uri_dom_xpath_element_content(xml, "html->head->title"))){
		if(searching)
			uri_title=g_strdup(service->uri);
		else if(services_resource && services_resource[0]=='@')
			uri_title=g_strdup_printf("<%s@%s>", &services_resource[1], service->uri);
		else{
			www_uri_title_append(uri, uri);
			return temp;
		}
	}
	uber_free(temp);
	
	www_html_entity_escape_status(&uri_title);
	www_uri_title_append(uri, uri_title);
	
	www_format_uri_with_title(&uri_title, uri, services_resource);
	
	if(!make_hyperlinks)
		temp=g_strdup_printf("<u>%s</u>", uri_title);
	else
		temp=g_strdup_printf("<a href=\"%s\">%s</a>", uri, uri_title);
	
	uber_free(uri_title);
	
	return temp;
}/*www_find_uri_pages_title(service, uri, "@user"||"#search||"!tag", expand_hyperlinks, make_hyperlinks);*/

static void www_format_uri_with_title(gchar **uri_title, const gchar *uri, const gchar *services_resource){
	debug("Attempting to display link info. title: %s for uri: '%s'.", *uri_title, uri);
	gchar *hyperlink_suffix1=NULL;
	gboolean searching=(services_resource && (services_resource[0]=='#' || services_resource[0]=='!' ));
	
	if(services_resource && searching && !g_strrstr(*uri_title, services_resource))
		hyperlink_suffix1=g_strdup_printf("'s search results for %s", services_resource);
	
	gchar *hyperlink_suffix2=NULL;
	if(!gconfig_if_bool(PREFS_URLS_EXPANSION_REPLACE_WITH_TITLES, TRUE))
		hyperlink_suffix2=g_strdup_printf(" &lt;- %s", uri);
	
	gchar *uri_title_swap=g_strdup_printf("%s%s%s", *uri_title, (hyperlink_suffix1 ?hyperlink_suffix1 :""), (hyperlink_suffix2 ?hyperlink_suffix2 :"") );
	uber_free(*uri_title);
	*uri_title=uri_title_swap;
	uri_title_swap=NULL;
	
	if(hyperlink_suffix1)
		uber_free(hyperlink_suffix1);
	if(hyperlink_suffix2)
		uber_free(hyperlink_suffix2);
}/*www_format_uri_with_title(&uri_title, uri, services_resource);*/

static gchar *www_get_uri_content_type(OnlineService *service, const gchar *uri, SoupMessage **xml){
	debug("Downloading headers for: %s to determine its content-type.", uri);
	*xml=online_service_request_uri(service, GET, uri, 0, NULL, NULL, NULL, NULL);
	gchar *error_message=NULL;
	if(!www_xml_error_check(service, uri, *xml, &error_message)){
		uber_free(error_message);
		return NULL;
	}
	
	uber_free(error_message);
	
	debug("Getting content-type from uri: '%s'.", uri);
	gchar **content_v=NULL;
	debug("Parsing xml document's content-type from [%s]'s headers.", uri);
	gchar *content_info=NULL;
	if(!(content_info=g_strdup((gchar *)soup_message_headers_get_one((*xml)->response_headers, "Content-Type")))){
		debug("**ERROR**: Failed to determine content-type for:  %s.", uri);
		return NULL;
	}
	
	debug("Parsing content info: [%s] from: %s", content_info, uri);
	content_v=g_strsplit(content_info, "; ", -1);
	uber_free(content_info);
	gchar *content_type=NULL;
	if(!( ((content_v[0])) && (content_type=g_strdup(content_v[0])) )){
		debug("**ERROR**: Failed to determine content-type for:  %s.", uri);
		g_strfreev(content_v);
		return NULL;
	}
	debug("Parsed Content-Type: [%s] for: %s", content_type, uri);
	
	g_strfreev(content_v);
	
	return content_type;
}/*www_get_uri_content_type*/

gchar *www_get_uri_dom_xpath_element_content(SoupMessage *xml, const gchar *xpath){
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL;
	debug("Parsing xml document before searching for xpath: '%s' content.", xpath);
	if(!(doc=parse_xml_doc(xml, &root_element))){
		debug("Unable to parse xml doc.");
		xmlCleanupParser();
		return NULL;
	}
	
	xmlNode	*current_node=NULL;
	gchar	*xpath_content=NULL;
	
	gchar	**xpathv=g_strsplit(xpath, "->", -1);
	guint	xpath_depth=0, xpath_target_depth=g_strv_length(xpathv)-1;
	
	debug("Searching for xpath: '%s' content.", xpath);
	for(current_node=root_element; current_node; current_node=current_node->next){
		if(current_node->type != XML_ELEMENT_NODE ) continue;
		
		IF_DEBUG
			debug("**NOTICE:** Looking for XPath: %s; current depth: %d; targetted depth: %d.  Comparing against current node: %s.", xpathv[xpath_depth], xpath_depth, xpath_target_depth, current_node->name);
		if( xpath_depth>xpath_target_depth ) break;
		
		if(!g_str_equal(current_node->name, xpathv[xpath_depth])){
			if(xpath_depth==xpath_target_depth && !current_node->next){
				IF_DEBUG
					debug("**NOTICE:** Current node: %s, at depth: %d, is malformated and doesn't close.  Moving back 'up' one layer in the DOM.", current_node->name, xpath_depth);
				current_node=current_node->parent;
				xpath_depth--;
			}
			continue;
		}
		
		if(xpath_depth<xpath_target_depth){
			if(!(current_node->children && current_node->children->next)) continue;
			
			current_node=current_node->children;
			xpath_depth++;
			continue;
		}
		
		xpath_content=(gchar *)xmlNodeGetContent(current_node);
		break;
	}
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	g_strfreev(xpathv);
	
	if(!( ((xpath_content)) && (xpath_content=g_strstrip(xpath_content)) && G_STR_N_EMPTY(xpath_content) )){
		if(xpath_content)
			uber_free(xpath_content);
		return NULL;
	}
	
	return xpath_content;
}/*www_get_uri_dom_xpath_element_content(soup_xml_response, "targetted->xpaths->element", "max_xpath_element");*/

static void www_uri_title_append(const gchar *uri, const gchar *title){
	while(www_uri_title_lookup_table_processing){}
	www_uri_title_list_store_total++;
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	
	time_t current_time=time(NULL);
	const gchar *datetime=ctime(&current_time);
	
	gtk_list_store_append(www_uri_title_lookup_table_list_store, iter);
	gtk_list_store_set(
			www_uri_title_lookup_table_list_store, iter,
				COLUMN_URI, g_strdup(uri),
				COLUMN_URI_TITLE, g_strdup(title),
				COLUMN_INSERT_TIME, g_strdup(datetime),
			-1
	);
	uber_free(iter);
}/*www_uri_title_append("http://twitter.com/uberChick", "uberChick on Twitter");*/

static gchar *www_uri_title_lookup(const gchar *uri){
	if(!www_uri_title_list_store_total) return NULL;
	while(www_uri_title_lookup_table_processing){}
	if(G_STR_EMPTY(uri)) return NULL;
	
	GtkTreeIter *iter=NULL;
	gboolean found=FALSE;
	gchar *key=NULL, *title=NULL;
	for(gint i=www_uri_title_list_store_total; i>=0 && !found; i--){
		iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(www_uri_title_lookup_table_list_store), iter, path)){
			debug("Retrieving iter from path to index %d failed.  Unable to remove row.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gtk_tree_model_get(
				GTK_TREE_MODEL(www_uri_title_lookup_table_list_store), iter,
					COLUMN_URI, &key,
					COLUMN_URI_TITLE, &title,
				-1
		);
		
		if(g_strcasecmp(uri, key)){
			found=TRUE;
			uber_free(key);
			break;
		}
		
		uber_free(title);
		uber_free(key);
		uber_free(iter);
	}
	
	if(!(found && title)){
		if(title) uber_free(title);
		uber_free(iter);
		return NULL;
	}
	
	time_t current_time=time(NULL);
	const gchar *datetime=ctime(&current_time);
	
	gtk_list_store_set(
			www_uri_title_lookup_table_list_store, iter,
				COLUMN_INSERT_TIME, g_strdup(datetime),
			-1
	);
	uber_free(iter);
	return title;
}/*www_uri_title_lookup("http://twitter.com/uberChick");*/

/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
