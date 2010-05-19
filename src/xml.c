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

#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <libsoup/soup.h>

#include "config.h"
#include "main.h"
#include "program.h"

#include "online-services.typedefs.h"
#include "online-service.types.h"

#include "xml.types.h"
#include "xml.h"

#include "main-window.h"


/********************************************************************************
 *      Project, system, & library headers.  eg #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/


/********************************************************************************
 *                        defines, macros, methods, & etc                       *
 ********************************************************************************/



/********************************************************************************
 *                        objects, structs, and enum typedefs                   *
 ********************************************************************************/


/********************************************************************************
 *                prototypes for private methods & functions                    *
 ********************************************************************************/
static gboolean xml_parser_init(void);

static OnlineServiceXmlDoc *xml_new_online_service_xml_doc(SoupMessage *xml, const gchar *uri);
static void xml_free_online_service_xml_doc(OnlineServiceXmlDoc *xml_doc);
static void xml_verify_online_service_xml_doc(OnlineServiceXmlDoc **xml_doc);
static xmlDoc *xml_create_doc_from_soup_message(SoupMessage *xml);


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/


/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
static gboolean libxml_parser_initalized=FALSE;


#define	DEBUG_DOMAINS	"UI:XML:URLs:URIs:Links:OnlineServices:Networking:Updates:XPath:Auto-Magical:WWW:xml.c"
#include "debug.h"


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
static gboolean xml_parser_init(void){
	if(libxml_parser_initalized)
		return TRUE;
	
	debug("Initalizing libxml2 parser.");
	return (libxml_parser_initalized=TRUE);
}/*xml_parser_init();*/

gboolean xml_parser_deinit(void){
	if(!libxml_parser_initalized)
		return FALSE;
	
	debug("Cleaning up libxml2 parser.  Calling: xmlCleanupParser();");
	xmlCleanupParser();
	return libxml_parser_initalized;
}/*xml_parser_deinit();*/


const gchar *xml_node_type_to_string(xmlElementType type){
	switch(type){
		case XML_ELEMENT_NODE: return _("XML_ELEMENT_NODE");
		case XML_ATTRIBUTE_NODE: return _("XML_ATTRIBUTE_NODE");
		case XML_TEXT_NODE: return _("XML_TEXT_NODE");
		case XML_CDATA_SECTION_NODE: return _("XML_CDATA_SECTION_NODE");
		case XML_ENTITY_REF_NODE: return _("XML_ENTITY_REF_NODE");
		case XML_ENTITY_NODE: return _("XML_ENTITY_NODE");
		case XML_PI_NODE: return _("XML_PI_NODE");
		case XML_COMMENT_NODE: return _("XML_COMMENT_NODE");
		case XML_DOCUMENT_NODE: return _("XML_DOCUMENT_NODE");
		case XML_DOCUMENT_TYPE_NODE: return _("XML_DOCUMENT_TYPE_NODE");
		case XML_DOCUMENT_FRAG_NODE: return _("XML_DOCUMENT_FRAG_NODE");
		case XML_NOTATION_NODE: return _("XML_NOTATION_NODE");
		case XML_HTML_DOCUMENT_NODE: return _("XML_HTML_DOCUMENT_NODE");
		case XML_DTD_NODE: return _("XML_DTD_NODE");
		case XML_ELEMENT_DECL: return _("XML_ELEMENT_DECL");
		case XML_ATTRIBUTE_DECL: return _("XML_ATTRIBUTE_DECL");
		case XML_ENTITY_DECL: return _("XML_ENTITY_DECL");
		case XML_NAMESPACE_DECL: return _("XML_NAMESPACE_DECL");
		case XML_XINCLUDE_START: return _("XML_XINCLUDE_START");
		case XML_XINCLUDE_END: return _("XML_XINCLUDE_END");
		case XML_DOCB_DOCUMENT_NODE: return _("XML_DOCB_DOCUMENT_NODE");
		default: return _("XML_TYPE_UNKNOWN");
	}
}/*xml_node_type_to_string(node->type);*/

static OnlineServiceXmlDoc *xml_new_online_service_xml_doc(SoupMessage *xml, const gchar *uri){
	OnlineServiceXmlDoc *this=g_new0(OnlineServiceXmlDoc, 1);
	this->xml=xml;
	this->doc=NULL;
	
	this->error=FALSE;
	this->error_message=NULL;
	
	this->uri=uri;
	this->content_type=NULL;
	this->char_set=NULL;
	this->encoding=NULL;
	
	this->root_element=NULL;
	return this;
}/*xml_new_online_service_xml_doc(SoupMessage *xml);*/

static void xml_verify_online_service_xml_doc(OnlineServiceXmlDoc **this){
	if(!this) return;
	
	if(!(*this)->error_message)
		(*this)->error_message=g_strdup("");
	
	if(! (*this)->uri)
		(*this)->uri="";
	
	if(! (*this)->content_type)
		(*this)->content_type=g_strdup("");
	
	if(! (*this)->char_set)
		(*this)->char_set=g_strdup("");
	
	if(! (*this)->encoding)
		(*this)->encoding=g_strdup("");
}/*xml_verify_online_service_xml_doc(xml_doc);*/

static void xml_free_online_service_xml_doc(OnlineServiceXmlDoc *this){
	if(!this) return;
	xml_verify_online_service_xml_doc(&this);
	
	if(this->doc)
		xmlFreeDoc(this->doc);
	
	uber_object_free(&this->error_message, &this->uri, &this->content_type, &this->char_set, &this->encoding, &this, NULL);
}/*xml_free_online_service_xml_doc(xml_doc);*/

static xmlDoc *xml_create_doc_from_soup_message(SoupMessage *xml){
	SoupURI	*suri=NULL;
	gchar *uri=NULL;
	if(!((((suri=soup_message_get_uri(xml)))) && ((uri=soup_uri_to_string(suri, FALSE))) && (G_STR_N_EMPTY(uri)))){
		debug("*WARNING*: Unknown XML document URI");
		if(uri) g_free(uri);
		uri=g_strdup("[*unknown*]");
	}
	if(suri) soup_uri_free(suri);
	
	
	if(!(xml->response_headers && xml->response_body && xml->response_body->data && xml->response_body->length)){
		debug("*ERROR:* Cannot parse empty or xml resonse from: %s", uri);
		g_free(uri);
		return NULL;
	}
	
	
	gchar **content_v=NULL;
	debug("Parsing xml document's content-type & DOM information from: %s", uri);
	gchar *content_info=NULL;
	if(!(content_info=g_strdup((gchar *)soup_message_headers_get_one(xml->response_headers, "Content-Type")))){
		debug("*ERROR:* Failed to determine content-type for:  %s", uri);
		g_free(uri);
		return NULL;
	}
	
	debug("Parsing content info: [%s] from: %s", content_info, uri);
	content_v=g_strsplit(content_info, "; ", -1);
	g_free(content_info);
	gchar *content_type=NULL;
	if(!(((content_v[0])) && (content_type=g_strdup(content_v[0])))){
		debug("*ERROR:*: Failed to determine content-type for:  %s", uri);
		g_strfreev(content_v);
		g_free(uri);
		return NULL;
	}
	
	if(!(g_strrstr(content_type, "text") || g_strrstr(content_type, "xml"))){
		debug("*ERROR:* <%s>'s Content-Type: [%s] is not contain text or xml content and cannot be parsed any further", uri, content_type);
		uber_free(content_type);
		return NULL;
	}
	
	debug("Parsed Content-Type: [%s] for: %s", content_type, uri);
	
	gchar *charset=NULL;
	if(!(((content_v[1])) && (charset=g_strdup(content_v[1])))){
		debug("*ERROR:* Failed to determine charset for:  %s", uri);
		g_free(content_type);
		g_strfreev(content_v);
		g_free(uri);
		return NULL;
	}
	g_strfreev(content_v);
	
	
	debug("Parsing charset: [%s] for: %s", charset, uri);
	content_v=g_strsplit(charset, "=", -1);
	g_free(charset);
	if(!(content_v && content_v[1])){
		debug("Defaulting to charset: [utf-8] for: %s", uri);
		charset=g_strdup("utf-8");
	}else{
		charset=g_strdup(content_v[1]);
		debug("Setting parsed charset: [%s] for: %s", charset, uri);
	}
	g_strfreev(content_v);
	
	
	gchar *encoding=NULL;
	if(!g_str_equal(charset, "utf-8"))
		encoding=g_ascii_strup(charset, -1);
	else
		encoding=g_utf8_strup(charset, -1);
	
	content_v=g_strsplit(content_type, "/", -1);
	gchar *dom_base_entity=NULL;
	if(!(((content_v[1])) && (dom_base_entity=g_strdup(content_v[1])))){
		debug("*ERROR:* Failed to determine DOM base entity URL for: %s", uri);
		g_free(uri);
		g_free(content_type);
		g_strfreev(content_v);
		return NULL;
	}
	g_strfreev(content_v);
	
	
	debug("Parsed xml document's information. URI: [%s] content-type: [%s]; charset: [%s]; encoding: [%s]; dom element: [%s]", uri, content_type, charset, encoding, dom_base_entity);
	
	xml_parser_init();	
	debug("Parsing %s document", dom_base_entity);
	xmlDoc *doc=NULL;
	if(!((doc=xmlReadMemory(xml->response_body->data, xml->response_body->length, dom_base_entity, encoding, (XML_PARSE_NOENT | XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING))))){
		debug("*ERROR:* Failed to parse %s document", dom_base_entity);
		g_free(uri);
		g_free(content_type);
		g_free(dom_base_entity);
		g_free(charset);
		return NULL;
	}
	
	g_free(dom_base_entity);
	g_free(encoding);
	g_free(content_type);
	g_free(charset);
	
	debug("XML document parsed.  Returning xmlDoc");
	return doc;
	OnlineServiceXmlDoc *xml_doc=xml_new_online_service_xml_doc(xml, uri);
	xml_free_online_service_xml_doc(xml_doc);
}/*xml_create_doc_from_soup_message(xml);*/

xmlDoc *xml_create_xml_doc_and_get_root_element_from_soup_message(SoupMessage *xml, xmlNode **root_element){
	xmlDoc *doc=NULL;
	if(!(doc=xml_create_doc_from_soup_message(xml))) {
		debug("*ERROR:* failed to create xml doc from soup message");
		return NULL;
	}
	
	debug("Setting XML nodes");
	*root_element=NULL;
	if(!(*root_element=xmlDocGetRootElement(doc))){
		debug("Unknown root element from xml doc");
		xmlFreeDoc(doc);
		return NULL;
	}
	
	return doc;
}/*xml_create_xml_doc_and_get_root_element_from_soup_message(xml, &root_element);*/

gboolean xml_error_check(OnlineService *service, const gchar *uri, SoupMessage *xml, gchar **error_message){
	if(!(SOUP_IS_MESSAGE(xml) && SOUP_STATUS_IS_SUCCESSFUL(xml->status_code) && xml->status_code>99)){
		*error_message=g_strdup_printf("OnlineService: <%s> has returned an invalid or failed libsoup request.  The URI [%s] returned:: %s(%d).", service->key, uri, xml->reason_phrase, xml->status_code);
		return FALSE;
	}
	
	*error_message=g_strdup("");;
	
	if(xml->status_code==100||xml->status_code==200){
		debug("OnlineService: <%s>'s request for: %s has succeed.  HTTP status returned: %s(%d)", service->key, uri, xml->reason_phrase, xml->status_code);
		return TRUE;
	}
	
	if(xml->status_code==401 || xml->status_code==503){
		debug("**ERROR:** Authentication failed and/or access denied.  OnlineService: <%s> returned. %s(%d) when requesting [%s]", service->key, xml->reason_phrase, xml->status_code, uri);
		return FALSE;
	}
	
	xmlDoc		*doc=NULL;
	xmlNode *root_element=NULL;
	
	if(!(doc=xml_create_xml_doc_and_get_root_element_from_soup_message(xml, &root_element))){
		uber_free(*error_message);
		if(!doc)
			*error_message=g_strdup("Unable to parse xml doc.");
		else{
			*error_message=g_strdup("Failed to get the root element from the xml doc.");
			xmlFreeDoc(doc);
		}
		debug(*error_message);
		return FALSE;
	}
	
	gboolean error_free=TRUE;
	xmlNode	*current_node=NULL;
	for(current_node=root_element; current_node; current_node=current_node->next) {
		if(current_node->type != XML_ELEMENT_NODE) continue;
		
		if(!(g_str_equal(current_node->name, "error"))) continue;
		
		error_free=FALSE;
		uber_free(*error_message);
		*error_message=(gchar *)xmlNodeGetContent(current_node);
		break;
	}
	
	if(!error_free){
		gchar *error_message_swap=g_strdup_printf("OnlineService: <%s> returned: %s(%d) with the error: %s.", service->key, xml->reason_phrase, xml->status_code, *error_message);
		uber_free((*error_message));
		*error_message=error_message_swap;
		error_message_swap=NULL;
		debug("%s", *error_message);
		statusbar_printf("%s", *error_message);
	}
	
	xmlFreeDoc(doc);
	
	return error_free;
}/*xml_error_check(service, xml);*/

xmlNode *xml_get_child_element(const gchar *uri, xmlNode *root_element, const gchar *element, const gchar *attribute, ...){
	gchar		*content=NULL;
	xmlAttr		*elements_attributes=NULL;
	xmlNode		*current_element=NULL, *current_elements_attributes=NULL;
	
	for(current_element=root_element; current_element; current_element=current_element->next) {
		debug("Parsing <%s>; looking for <%s>.  Current element: <%s>; type: [%s]", uri, element, current_element->name, xml_node_type_to_string(current_element->type));
		if((current_element->type == XML_ELEMENT_NODE && g_str_equal(current_element->name, element)))
			break;
	}
	
	if(!current_element)
		return NULL;
	
	if(!attribute)
		return current_element;
		
	va_list attributes;
	va_start(attributes, attribute);
	const gchar *value=NULL;
	for(; attribute; attribute=va_arg(attributes, const gchar *)){
		current_elements_attributes=NULL;
		value=va_arg(attributes, const gchar *);
		current_elements_attributes=NULL;
		debug("Parsing <%s> looking: <%s>'s %s", uri, element, attribute);
		for(elements_attributes=current_element->properties; elements_attributes; elements_attributes=elements_attributes->next){
			debug("Parsing element: <%s>; looking: %s value", elements_attributes->name, attribute);
			if(g_str_equal(elements_attributes->name, attribute)) break;
		}
		if(!elements_attributes) break;
		
		current_elements_attributes=elements_attributes->children;
		if(!value){
			current_element=current_elements_attributes;
			break;
		}
		for(; current_elements_attributes; current_elements_attributes=current_elements_attributes->next){
			content=(gchar *)xmlNodeGetContent(current_elements_attributes);
			debug("Parsing Groups node: <%s> looking for <%s %s=\"%s\">", content, element, attribute, value);
			if(g_str_equal(content, value)) break;
			uber_free(content);
		}
		if(content) uber_free(content);
		if(!current_elements_attributes) break;
	}
	va_end(attributes);
	if(!current_elements_attributes)
		return NULL;
	
	return current_element;
}/*xml_get_child_element(uri, root_element, "div", "class", "entity_profile vcard entry-content");*/

/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/
