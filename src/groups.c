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
/********************************************************************************
 *                      My art, code, & programming.                            *
 ********************************************************************************/
#define _GNU_SOURCE
#define _THREAD_SAFE



/********************************************************************************
 *      Project, system & library headers, eg #include <gdk/gdkkeysyms.h>       *
 ********************************************************************************/
#include <glib/gi18n.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <libsoup/soup-message.h>

#include "config.h"
#include "program.h"

#include "online-services.typedefs.h"
#include "online-services.types.h"

#include "online-service.types.h"

#include "online-services.h"
#include "online-service.h"
#include "update-ids.h"
#include "network.h"

#include "update.types.h"
#include "users.types.h"
#include "users.h"

#include "main-window.h"
#include "uberchick-tree-view.h"
#include "preferences.defines.h"
#include "images.h"
#include "cache.h"
#include "gconfig.h"

#include "xml.h"
#include "www.h"

#include "groups.types.h"
#include "groups.h"


/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"Groups:Parser:Requests:OnlineServices:Updates:UI:Refreshing:Dates:Times:groups.c"
#include "debug.h"

static GRegex *groups_id_regex=NULL;

/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static StatusNetGroup *groups_new_group(OnlineService *service);
static void groups_verify_group(StatusNetGroup **group);
static void groups_free_group(StatusNetGroup *group);
static StatusNetGroup *groups_new_group_from_node(OnlineService *service, const gchar *uri, xmlNode *root_element);
static void groups_format_group_note(StatusNetGroup *group);
static gboolean groups_notify(StatusNetGroup *group);

/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
void groups_init(void){
	GError *error=NULL;
	const gchar *g_regex_pattern="^group-([0-9]+)$";
	groups_id_regex=g_regex_new(g_regex_pattern, G_REGEX_DOLLAR_ENDONLY|G_REGEX_OPTIMIZE, G_REGEX_MATCH_NEWLINE_ANY, &error);
	if(error){
		debug("**ERROR:** creating GRegex using the pattern %s.  GError message: %s", g_regex_pattern, error->message );
		g_error_free(error);
		uber_regex_unref(groups_id_regex);
	}
}/*groups_init();*/

void groups_deinit(void){
	uber_regex_unref(groups_id_regex);
}/*groups_deinit();*/

static StatusNetGroup *groups_new_group(OnlineService *service){
	StatusNetGroup *group=g_new0(StatusNetGroup, 1);
	
	group->service=service;
	group->type=Groups;
	
	group->id=0.0;
	
	group->name=group->title=group->id_str=group->uri=group->homepage=group->image_uri=group->image_file=group->note=group->note_tooltip=group->note_markup=group->notification=NULL;
	
	group->created_at=0;
	group->created_seconds_ago=0;
	
	return group;
}/*groups_new_group(service);*/

StatusNetGroup *groups_new_group_from_node(OnlineService *service, const gchar *uri, xmlNode *root_element){
	if(!( service && root_element && root_element->children && root_element->properties )) return NULL;
	
	StatusNetGroup	*group=groups_new_group(service);
	xmlNode		*current_element=NULL, *contents_element=NULL, *current_elements_attributes=NULL;
	xmlAttr		*elements_attributes=NULL;
	gchar		*content=NULL;
	
	debug("Parsing Group's ID within this node, i.e. <li class=\"profile hentry\" id=\"group-[0-9]+\">");
	for(elements_attributes=root_element->properties; elements_attributes; elements_attributes=elements_attributes->next){
		debug("Parsing Group node: <%s>; finding id attribute", elements_attributes->name );
		if(g_str_equal(elements_attributes->name, "id")) break;
	}
	if(!elements_attributes) return NULL;
	
	for(current_elements_attributes=elements_attributes->children; current_elements_attributes; current_elements_attributes=current_elements_attributes->next){
		content=(gchar *)xmlNodeGetContent(current_elements_attributes);
		debug("Parsing Groups node: <%s> looking for id=\"group-[0-9]+\"", content);
		GMatchInfo *match_info=NULL;
		if(g_regex_match(groups_id_regex, content, 0, &match_info)){
			g_match_info_free(match_info);
			break;
		}
		g_match_info_free(match_info);
		uber_free(content);
	}
	if(!content)
		return NULL;
	
	gchar *group_id=NULL;
	group->id=g_ascii_strtod((group_id=g_strrstr(content, "-")+1), NULL);
	group->id_str=g_strdup(group_id);
	debug("Parsing Group. ID: %s(=%f)'s Hypercard", group->id_str, group->id);
	if(content) uber_free(content);
	
	/*
	 * current_element=root_element->children->children
	 * get's inside the Groups hypercard <div>
	 *
	 */
	if(!(root_element=xml_get_child_element(uri, root_element->children, "div", "class", "entity_profile vcard entry-content", NULL ))){
		debug("Could not find Groups <div class=\"entity_profile vcard entry-content\">");
		return NULL;
	}
	
	for(current_element=root_element->children; current_element; current_element=current_element->next) {
		debug("Parsing Group from xhtml <li> element.  Current element: <%s>; type: [%s]", current_element->name, xml_node_type_to_string(current_element->type));
		if(current_element->type != XML_ELEMENT_NODE && current_element->type != XML_ATTRIBUTE_NODE ) continue;
		
		if(g_str_equal(current_element->name, "a")){
			if((contents_element=xml_get_child_element(uri, current_element, "a", "class", "url entry-title", "href", NULL))){
				content=(gchar *)xmlNodeGetContent(contents_element);
				group->uri=g_strdup(content);
				debug("**DEBUG:** Parsed Group's URI: <%s>", group->uri);
				uber_free(content);
			}else if((contents_element=xml_get_child_element(uri, current_element, "a", "class", "url", "href", NULL))){
				content=(gchar *)xmlNodeGetContent(contents_element);
				group->homepage=g_strdup(content);
				debug("**DEBUG:** Parsed Group's Homepage: <%s>", group->homepage);
				uber_free(content);
				continue;
			}else
				continue;
			
			for(current_elements_attributes=current_element->children; current_elements_attributes; current_elements_attributes=current_elements_attributes->next) {
				debug("Parsing current element: <%s>; type: [%s]", current_elements_attributes->name, xml_node_type_to_string(current_elements_attributes->type));
				if(g_str_equal(current_elements_attributes->name, "img")){
					if((contents_element=xml_get_child_element(uri, current_elements_attributes, "img", "class", "photo avatar", "src", NULL))){
						content=(gchar *)xmlNodeGetContent(contents_element);
						group->image_uri=g_strdup(content);
						debug("**DEBUG:** Parsed Group's Image URI: <%s>", group->image_uri);
						uber_free(content);
					}
					
				}else if(g_str_equal(current_elements_attributes->name, "span")){
					if((contents_element=xml_get_child_element(uri, current_elements_attributes, "span", "class", "nickname", NULL))){
						content=(gchar *)xmlNodeGetContent(contents_element);
						group->name=g_markup_printf_escaped(content);
						uber_free(content);
						debug("**DEBUG:** Parsed Group's Name: <%s>", group->name);
					}
				}
			}
		}else if(g_str_equal(current_element->name, "span")){
			if((contents_element=xml_get_child_element(uri, current_element, "span", "class", "fn org", NULL))){
				content=(gchar *)xmlNodeGetContent(contents_element);
				group->title=g_markup_printf_escaped(content);
				uber_free(content);
				debug("**DEBUG:** Parsed Group's Title: <%s>", group->title);
			}
		}else if(g_str_equal(current_element->name, "p")){
			if((contents_element=xml_get_child_element(uri, current_element, "p", "class", "note", NULL))){
				content=(gchar *)xmlNodeGetContent(contents_element);
				group->note=g_markup_printf_escaped(content);
				uber_free(content);
				debug("**DEBUG:** Parsed Group's Note: <%s>", group->note);
			}
		}
	}
	
	group->image_file=cache_images_get_group_image_filename(service, group->name, group->image_uri);
	groups_verify_group(&group);
	groups_format_group_note(group);
	return group;
}/*groups_new_group_from_node(service, uri, root_element);*/

static void groups_format_group_note(StatusNetGroup *group){
	if(!(group && group->id && G_STR_N_EMPTY(group->note) )) return;
	
	group->note_tooltip=www_html_entity_escape_text(group->note);
	group->note_markup=www_format_urls(group->service, group->note_tooltip, FALSE, FALSE);
}/*groups_format_group_note(group);*/

static void groups_verify_group(StatusNetGroup **group){
	if(!group) return;
	
	if(! (*group)->id_str ) (*group)->id_str=g_strdup("");
	
	if(! (*group)->uri ) (*group)->uri=g_strdup("");
	if(! (*group)->homepage ) (*group)->homepage=g_strdup("");
	
	if(! (*group)->name ) (*group)->name=g_strdup("");
	if(! (*group)->title ) (*group)->title=g_strdup("");
	
	if(! (*group)->image_uri ) (*group)->image_uri=g_strdup("");
	if(! (*group)->image_file ) (*group)->image_file=g_strdup("");
	
	if(! (*group)->note ) (*group)->note=g_strdup("");
	if(! (*group)->note_tooltip ) (*group)->note_tooltip=g_strdup("");
	if(! (*group)->note_markup ) (*group)->note_markup=g_strdup("");
	if(! (*group)->notification ) (*group)->notification=g_strdup("");
}/*groups_verify_group(group);*/

static void groups_free_group(StatusNetGroup *group){
	if(!group) return;
	groups_verify_group( &group );
	debug("Freeing StatusNetGroup: <%s> [id: %f]", group->name, group->id);
	
	group->service=NULL;
	
	uber_object_free( &group->id_str, &group->name, &group->title, &group->uri, &group->homepage, &group->image_uri, &group->image_file, &group->note, &group->note_tooltip, &group->note_markup, &group->notification, &group, NULL );
}/*groups_free_group(StatusNetGroup *group);*/

/* Parse a timeline XML file */
guint groups_parse_conversation(OnlineService *service, SoupMessage *xml, const gchar *uri, UberChickTreeView *uberchick_tree_view, UpdateType update_type){
	const gchar	*timeline=g_strrstr(uri, "/");
	
	xmlDoc		*doc=NULL;
	xmlNode		*root_element=NULL, *current_element=NULL, *current_elements_attributes=NULL;
	xmlAttr		*elements_attributes=NULL;
	
	if(!(doc=xml_create_xml_doc_and_get_root_element_from_soup_message(xml, &root_element))){
		debug("Failed to parse xml document, timeline: %s; uri: %s", timeline, uri);
		xmlCleanupParser();
		return 0;
	}
	
	gboolean found_body=FALSE, found_wrap=FALSE, found_core=FALSE, found_content=FALSE, found_content_inner=FALSE, found_profiles=FALSE;
	const gchar	*element="body", *attribute="id", *value="groupsearch";
	gchar		*content=NULL;
	for(root_element=root_element->children; root_element; root_element=root_element->next){
		if(!( root_element->type == XML_ELEMENT_NODE && g_str_equal(root_element->name, element) ))
			continue;
		
		if(!found_body){
			found_body=TRUE;
			element="div";
			value="wrap";
			root_element=root_element->children;
			continue;
		}
		if(!(root_element->properties && root_element->properties->name)) continue;
		for(elements_attributes=root_element->properties; elements_attributes; elements_attributes=elements_attributes->next){
			if(!elements_attributes->name) continue;
			debug("Parsing Groups from <%s>'s looking for <%s %s=\"%s\">; at node: <%s %s=\"%s\">", uri, element, attribute, value, root_element->name, elements_attributes->name, content);
			if(g_str_equal(elements_attributes->name, attribute)) break;
		}
		if(!elements_attributes) continue;
		
		for(current_elements_attributes=elements_attributes->children; current_elements_attributes; current_elements_attributes=current_elements_attributes->next){
			content=(gchar *)xmlNodeGetContent(current_elements_attributes);
			debug("Parsing Groups from <%s>'s looking for <%s %s=\"%s\">; at node: <%s %s=\"%s\">", uri, element, attribute, value, root_element->name, elements_attributes->name, content);
			if(g_str_equal(content, "wrap")){
				found_wrap=TRUE;
				value="core";
			}else if(g_str_equal(content, "core")){
				found_core=TRUE;
				value="content";
			}else if(g_str_equal(content, "content")){
				found_content=TRUE;
				value="content_inner";
			}else if(g_str_equal(content, "content_inner")){
				found_content_inner=TRUE;
				element="ul";
				attribute="class";
				value="profiles groups xoxo";
			}else if(g_str_equal(content, "profiles groups xoxo")){
				found_profiles=TRUE;
				element="li";
				attribute="class";
				value="profile hentry";
			}else{
				uber_free(content);
			}
			
			if(content){
				root_element=root_element->children;
				uber_free(content);
			}
			
			if(found_profiles)
				break;
		}
		if(found_profiles)
			break;
	}
	if(!root_element){
		debug("Failed to find Groups' <ul class=\"profiles groups xoxo\">, timeline: %s; uri: %s", timeline, uri);
		xmlCleanupParser();
		return 0;
	}
	
	StatusNetGroup 	*group=NULL;
	
	/* Count new tweets */
	guint		new_updates=0, notified_updates=0;
	
	gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	
	update_ids_get(service, timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	
	gdouble	last_notified_update=newest_update_id;
	newest_update_id=0.0;
	
	gboolean	has_loaded=uberchick_tree_view_has_loaded(uberchick_tree_view);
	gboolean	notify=( (oldest_update_id && has_loaded) ?gconfig_if_bool(PREFS_NOTIFY_ALL, TRUE) :FALSE );
	gboolean	save_oldest_id=(has_loaded?FALSE:TRUE);
	
	guint		update_notification_delay=uberchick_tree_view_get_notify_delay(uberchick_tree_view);
	const gint	update_notification_interval=10;
	const gint	notify_priority=(uberchick_tree_view_get_page(uberchick_tree_view)+1)*100;
	
	/* parse Groups from search results. */
	//debug("Parsing Groups from xhtml search results; starting with: <%s> node", root_element->name);
	for(current_element=root_element->next; current_element; current_element=current_element->next) {
		debug("Parsing Groups from <%s>'s looking for <li>; at node: <%s>; type: [%s]", uri, current_element->name, xml_node_type_to_string(current_element->type));
		if(!( current_element->type == XML_ELEMENT_NODE && g_str_equal(current_element->name, "li") ))
			continue;
		
		if(!current_element->children){
			debug("*WARNING:* Cannot parse %s. Its missing children nodes", current_element->name);
			continue;
		}
		if(content) uber_free(content);
		debug("Parsing Groups node looking for each Group's node, i.e. <li class=\"profile hentry\">");
		for(elements_attributes=current_element->properties; elements_attributes; elements_attributes=elements_attributes->next){
			debug("Parsing Groups at node: %s checking class value", elements_attributes->name);
			if(g_str_equal(elements_attributes->name, "class")) break;
		}
		if(!elements_attributes) continue;
		
		for(current_elements_attributes=elements_attributes->children; current_elements_attributes; current_elements_attributes=current_elements_attributes->next){
			content=(gchar *)xmlNodeGetContent(current_elements_attributes);
			debug("Parsing Groups node: <%s> looking for <li class=\"profile hentry\">", content);
			if(g_str_equal(content, "profile hentry")) break;
			uber_free(content);
		}
		if(content) uber_free(content);
		if(!current_elements_attributes) continue;
		
		debug("Parsing Groups result");
		group=NULL;
		if(!( (( group=groups_new_group_from_node(service, uri, current_element) )) && group->id )){
			if(group) groups_free_group(group);
			continue;
		}
		
		new_updates++;
		gboolean notify_of_new_update=FALSE;
		debug("Adding StatusNetGroup ID: %f, on <%s> to UberChickTreeView", group->id, service->key);
		uberchick_tree_view_store_group(uberchick_tree_view, group);
		
		if(notify && !save_oldest_id && group->id > last_notified_update)
			notify_of_new_update=TRUE;
		
		if(!newest_update_id && group->id) newest_update_id=group->id;
		
		if(save_oldest_id && group->id)
			oldest_update_id=group->id;
		
		if(!notify_of_new_update)
			groups_free_group(group);
		else{
			g_timeout_add_seconds_full(notify_priority, update_notification_delay, (GSourceFunc)groups_notify, group, (GDestroyNotify)groups_free_group);
			update_notification_delay-=update_notification_interval;
			notified_updates++;
		}
	}
	
	if(new_updates && newest_update_id){
		/*TODO implement this only once it won't ending up causing bloating.
		 *cache_save_page(service, uri, xml->response_body);
		 */
		debug("Processing <%s>'s requested URI's: [%s] Group IDs", service->guid, uri);
		debug("Saving <%s>'s; Group IDs for [%s];  newest ID: %f; unread ID: %f; oldest ID: %f", service->guid, timeline, newest_update_id, unread_update_id, oldest_update_id );
		update_ids_set(service, timeline, newest_update_id, unread_update_id, oldest_update_id);
	}
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return new_updates;
}/*search_parse_results(service, xml, uri, uberchick_tree_view);*/

static gboolean groups_notify(StatusNetGroup *group){
	groups_free_group(group);
	return FALSE;
}/*groups_notify(group);*/
/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

