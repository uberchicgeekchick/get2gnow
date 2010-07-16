/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright(c) 2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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
 * License("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
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
 * user-visible display as defined in Section 6.4(d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright(C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright(C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright(C) of the User the data is connected to.
 * Users may lisences their data under the terms of an OSI approved or Creative Commons
 * license.  Users must be allowed to select their choice of license for each piece of data
 * on an individual bases and cannot be blanketly applied to all of the Users.  The User may
 * select a default license for their data.  All of the Software's data pertaining to each
 * User must be fully accessible, exportable, and deletable to that User.
 */

#define _GNU_SOURCE
#define _THREAD_SAFE


#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <glib/gprintf.h>


/*******************************************************************************
 * keyboard & mouse event structs, enums, & defines.                           *
 *          /usr/include/gtk-2.0/gdk/gdkevents.h                               *
 * keyboard enum mappings are found in:                                        *
 *          /usr/include/gtk-2.0/gdk/gdkkeysyms.h                              *
 *******************************************************************************/
#include <gdk/gdk.h>
#include <gdk/gdkevents.h>
#include <gdk/gdkkeys.h>
#include <gdk/gdktypes.h>
#include <gdk/gdkkeysyms.h>



#include "config.h"
#include "program.h"

#include "main-window.h"
#include "geometry.h"
#include "hotkeys.h"

#include "online-service.h"
#include "online-service-request.h"
#include "network.h"

#include "gconfig.h"
#include "preferences.defines.h"
#include "tabs.h"
#include "uberchick-tree-view.types.h"
#include "uberchick-tree-view.h"
#include "update-viewer.h"
#include "ui-utils.h"
#include "users.h"


#define	DEBUG_DOMAINS	"OnlineServices:Networking:Updates:Requests:Users:Updates:hotkeys.c"
#include "debug.h"

static gboolean hotkey_process(GtkWidget *widget, GdkEventKey *event);
static gboolean hotkeys_display_menu(const gchar *menu_name, const gint event_time);


void hotkey_pressed(GtkWidget *widget, GdkEventKey *event){
	gboolean gui_displayed=FALSE;
	if(!(gui_displayed=hotkey_process(widget, event)))
		update_viewer_sexy_select();
}/*hotkey(widget, event);*/

static gboolean hotkey_process(GtkWidget *widget, GdkEventKey *event){
	if(event->keyval==GDK_Escape){
		if(gtk_widget_has_focus(GTK_WIDGET(main_window_sexy_search_entry_get_widget()))){
			main_window_show_search_history();
			return TRUE;
		}else if(gtk_widget_has_focus(GTK_WIDGET(update_viewer_sexy_entry_get_widget()))){
			update_viewer_show_previous_updates();
			return TRUE;
		}
	}
	
	switch(event->state){
		case GDK_SHIFT_MASK|GDK_CONTROL_MASK:
			switch(event->keyval){
				case GDK_asterisk:
					online_service_request_selected_update_unfave();
					return FALSE;
				
				case GDK_F:	case GDK_f:
				case GDK_less:		case GDK_minus:
					online_service_request_selected_update_unfollow();
					return FALSE;
				
				case GDK_Insert:	case GDK_KP_Insert:
				case GDK_I:		case GDK_i:
					online_service_request_popup_shorten_uri();
					return TRUE;
				
				case GDK_Return:	case GDK_KP_Enter:
					update_viewer_sexy_insert_char('\n');
					return FALSE;
				
				case GDK_Tab:		case GDK_KP_Tab:
					update_viewer_sexy_insert_char('\t');
					return FALSE;
#ifdef GNOME_ENABLE_DEBUG
				case GDK_R:		case GDK_r:
					if(online_service_request_isset_selected_update()){
						tabs_remove_from_uberchick_tree_views_tree_stores(STRING_USER, (gchar *)online_service_request_selected_update_get_user_name());
						return FALSE;
					}
#endif
			}
		case GDK_MOD1_MASK:
			switch(event->keyval){
				case GDK_Return:	case GDK_KP_Enter:
				case GDK_D:	case GDK_d:
					update_viewer_send(GTK_WIDGET(update_viewer_get_sexy_dm_button()));
					return FALSE;
				
				case GDK_S:	case GDK_s:
					update_viewer_send(NULL);
					return FALSE;
				
				case GDK_N:	case GDK_n:
					return hotkeys_display_menu("network", event->time);
				
				case GDK_B:	case GDK_b:
					return hotkeys_display_menu("tabs", event->time);
				
				case GDK_E:	case GDK_e:
					return hotkeys_display_menu("edit", event->time);
				
				case GDK_V:	case GDK_v:
					return hotkeys_display_menu("view", event->time);
				
				case GDK_T:	case GDK_t:
					return hotkeys_display_menu("online_service_request", event->time);
				
				case GDK_H:	case GDK_h:
					return hotkeys_display_menu("help", event->time);
				
				case GDK_Up: case GDK_KP_Up:
					if(gtk_widget_has_focus(GTK_WIDGET(main_window_sexy_search_entry_get_widget())))
						main_window_hide_search_history();
					else /*if(gtk_widget_has_focus(GTK_WIDGET(update_viewer_sexy_entry_get_widget())))*/
						update_viewer_hide_previous_updates();
					return TRUE;
				
				case GDK_Down: case GDK_KP_Down:
					if(gtk_widget_has_focus(GTK_WIDGET(main_window_sexy_search_entry_get_widget())))
						main_window_show_search_history();
					else /*if(gtk_widget_has_focus(GTK_WIDGET(update_viewer_sexy_entry_get_widget())))*/
						update_viewer_show_previous_updates();
					return TRUE;
				
				case GDK_R: case GDK_r:
					uberchick_tree_view_refresh(tabs_get_current()); 
					return FALSE;
				
				case GDK_P:	case GDK_p:
				case GDK_question:
					online_service_request_selected_update_view_profile();
					return TRUE;
				
				case GDK_U:	case GDK_u:
					/* calls: update_viewer_sexy_select(); */
					gtk_widget_grab_focus(GTK_WIDGET(update_viewer_sexy_entry_get_widget()));
					return FALSE;
				
				case GDK_asciitilde:	case GDK_ampersand:
					online_service_request_selected_update_view_updates();
					return FALSE;
				
				case GDK_F:	case GDK_f:
				case GDK_greater:	case GDK_plus:
					online_service_request_selected_update_follow();
					return FALSE;
				
				case GDK_A:	case GDK_a:
					online_service_request_popup_best_friend_add();
					return TRUE;
				
				case GDK_asterisk:
				case GDK_colon:	case GDK_exclam:
					update_viewer_best_friend_emulate_toggle_click();
					return FALSE;
				
				case GDK_L:	case GDK_l:
				case GDK_numbersign:	case GDK_semicolon:
					online_service_request_selected_update_block();
					return FALSE;
				
				case GDK_O:	case GDK_o:
					online_service_request_selected_update_unblock();
					return FALSE;
				
				case GDK_Insert:	case GDK_KP_Insert:
				case GDK_I:		case GDK_i:
					online_service_request_popup_shorten_uri();
					return TRUE;
				
				case GDK_Tab:		case GDK_KP_Tab:
					update_viewer_sexy_insert_char('\t');
					return FALSE;
				
				case GDK_Page_Up:
					tabs_get_previous();
					return FALSE;
				
				case GDK_Page_Down:
					tabs_get_next();
					return FALSE;
				
				default: break;
			}
			break;
		
		case GDK_SHIFT_MASK:
			switch(event->keyval){
				case GDK_Return:	case GDK_KP_Enter:
					update_viewer_new_dm();
					return FALSE;
				
				case GDK_Insert:	case GDK_KP_Insert:
					online_service_request_popup_shorten_uri();
					return TRUE;
				
				case GDK_Tab:		case GDK_KP_Tab:
					update_viewer_sexy_insert_char('\t');
					return FALSE;
				
				default: break;
			}
			break;
		
		case GDK_CONTROL_MASK:
			switch(event->keyval){
				case GDK_Return:	case GDK_KP_Enter:
					update_viewer_sexy_insert_char('\n');
					return FALSE;
				
				case GDK_Insert:	case GDK_KP_Insert:
				case GDK_I:		case GDK_i:
					online_service_request_popup_shorten_uri();
					return TRUE;
				
				case GDK_Tab:		case GDK_KP_Tab:
					update_viewer_sexy_insert_char('\t');
					return FALSE;
				
				case GDK_N:	case GDK_n:
					update_viewer_new_update();
					return TRUE;
				
				case GDK_K:	case GDK_k:
					update_viewer_sexy_entry_clear();
					return TRUE;
				
				case GDK_L:	case GDK_l:
					update_viewer_select_all();
					return TRUE;
				
				case GDK_Q:	case GDK_q:
					gtk_main_quit();
					return TRUE;
				
				case GDK_S:	case GDK_s:
				case GDK_asterisk:
					online_service_request_selected_update_fave();
					return FALSE;
				
				case GDK_F5:
					uberchick_tree_view_refresh(tabs_get_current()); 
					return FALSE;
				
				case GDK_Z:	case GDK_z:
					online_service_request_selected_update_unfave();
					return FALSE;
				
				case GDK_R:	case GDK_r:
					online_service_request_selected_update_reply();
					return FALSE;
				
				case GDK_F:	case GDK_f:
					online_service_request_selected_update_forward();
					return FALSE;
				
				case GDK_D:	case GDK_d:
					update_viewer_new_dm();
					return TRUE;
				
				case GDK_W: case GDK_w:
					tabs_close_current_page();
					return FALSE;
				
				case GDK_Page_Up:
					tabs_get_previous();
					return FALSE;
				
				case GDK_Page_Down:
					tabs_get_next();
					return FALSE;
				
				default: break;
			}
			break;
		
		default:
			switch(event->keyval){
				case GDK_F1:
					help_show(main_window_get_window());
					return TRUE;
				
				case GDK_greater:
					online_service_request_selected_update_forward();
					return FALSE;
				
				case GDK_at:
					online_service_request_selected_update_reply();
					return FALSE;
				
				case GDK_asciitilde:
					update_viewer_new_dm();
					return FALSE;
				
				default: break;
			}
			break;
	}
	uberchick_tree_view_key_pressed(tabs_get_current(), event);
	return FALSE;
}/*hotkey_process(widget, event);*/

static gboolean hotkeys_display_menu(const gchar *menu_name, const gint event_time){
	GtkMenu *menu=main_window_get_menu(menu_name);
	GtkMenuItem *menu_item=main_window_get_menu_item(menu_name);
	if(!(menu && menu_item))
		return FALSE;
	debug("Displaying main_window's [%s] menu via hotkey popup", menu_name);
	gtk_menu_popup(menu, GTK_WIDGET(GTK_MENU_SHELL(main_menu_get_main_menubar())), GTK_WIDGET(menu_item), NULL, NULL, 0, event_time);
	return TRUE;
	g_signal_emit_by_name(main_window_get_menu_item("network"), "popup");
}/*hotkeys_display_menu("network");*/

/********************************************************
 *                       eof                            *
 ********************************************************/

