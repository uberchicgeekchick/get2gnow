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
#include "preferences.h"
#include "tabs.h"
#include "update-viewer.h"
#include "control-panel.h"
#include "ui-utils.h"
#include "users.h"


#define	DEBUG_DOMAINS	"OnlineServices:Networking:Tweets:Requests:Users:Updates:hotkeys.c"
#include "debug.h"

static void hotkey_process(GtkWidget *widget, GdkEventKey *event, gpointer user_date);


void hotkey_pressed(GtkWidget *widget, GdkEventKey *event, gpointer user_date){
	hotkey_process(widget, event, user_date);
	control_panel_sexy_select();
}/*void hotkey(widget, event, user_date){*/

static void hotkey_process(GtkWidget *widget, GdkEventKey *event, gpointer user_date){
	if(event->keyval==GDK_Escape){
		control_panel_hide_previous_updates();
		return;
	}
	
	switch(event->state){
		case GDK_MOD1_MASK:
			switch(event->keyval){
				case GDK_Return:	case GDK_KP_Enter:
				case GDK_D:	case GDK_d:
					control_panel_sexy_send_dm();
					return;
				case GDK_S:	case GDK_s:
					control_panel_send(NULL);
					return;
				case GDK_N:	case GDK_n:
					g_signal_emit_by_name(main_window_get_menu("network"), "activate");
					return;
				case GDK_B:	case GDK_b:
					g_signal_emit_by_name(main_window_get_menu("tabs"), "activate");
					return;
				case GDK_E:	case GDK_e:
					g_signal_emit_by_name(main_window_get_menu("edit"), "activate");
					return;
				case GDK_V:	case GDK_v:
					g_signal_emit_by_name(main_window_get_menu("view"), "activate");
					return;
				case GDK_T:	case GDK_t:
					g_signal_emit_by_name(main_window_get_menu("online_service_request"), "activate");
					return;
				case GDK_H:	case GDK_h:
					g_signal_emit_by_name(main_window_get_menu("help"), "activate");
					return;
				case GDK_Down: case GDK_KP_Down:
					control_panel_show_previous_updates();
					return;
				case GDK_R: case GDK_r:
					update_viewer_refresh(tabs_get_current()); 
					return;
				case GDK_I:	case GDK_i:
				case GDK_question:
					online_service_request_selected_update_view_profile();
					return;
				case GDK_U:	case GDK_u:
				case GDK_asciitilde:	case GDK_ampersand:
					online_service_request_selected_update_view_updates();
					return;
				case GDK_F:	case GDK_f:
				case GDK_greater:	case GDK_plus:
					online_service_request_selected_update_follow();
					return;
				case GDK_A:	case GDK_a:
					online_service_request_popup_best_friend_add();
					return;
				case GDK_asterisk:
				case GDK_colon:	case GDK_exclam:
					online_service_request_selected_update_best_friend_add();
					return;
				case GDK_Z:	case GDK_z:
				case GDK_less:		case GDK_minus:
					online_service_request_selected_update_unfollow();
					return;
				case GDK_L:	case GDK_l:
				case GDK_numbersign:	case GDK_semicolon:
					online_service_request_selected_update_block();
					return;
				case GDK_O:	case GDK_o:
					online_service_request_selected_update_unblock();
					return;
				case GDK_Page_Up:
					tabs_get_previous();
					return;
				case GDK_Page_Down:
					tabs_get_next();
					return;
				default: break;
			}
			break;
		case GDK_SHIFT_MASK:
			switch(event->keyval){
				case GDK_Return:	case GDK_KP_Enter:
					control_panel_new_dm();
					return;
				default: break;
			}
			break;
		case GDK_CONTROL_MASK|GDK_SHIFT_MASK:
			switch(event->keyval){
				case GDK_asterisk:
					online_service_request_selected_update_destroy_fave();
				return;
			}
		case GDK_CONTROL_MASK:
			switch(event->keyval){
				case GDK_Return:	case GDK_KP_Enter:
					control_panel_sexy_insert_char('\n');
					return;
				case GDK_Tab:
					control_panel_sexy_insert_char('\t');
					return;
				case GDK_N:	case GDK_n:
					control_panel_new_update();
					return;
				case GDK_Q:	case GDK_q:
					gtk_main_quit();
					return;
				case GDK_S:	case GDK_s:
				case GDK_asterisk:
					online_service_request_selected_update_save_fave();
					return;
				case GDK_F5:
					update_viewer_refresh(tabs_get_current()); 
					return;
				case GDK_Z:	case GDK_z:
					online_service_request_selected_update_destroy_fave();
					return;
				case GDK_R:	case GDK_r:
					online_service_request_selected_update_reply();
					return;
				case GDK_F:	case GDK_f:
					online_service_request_selected_update_forward_update();
					return;
				case GDK_D:	case GDK_d:
					control_panel_new_dm();
					return;
				case GDK_W: case GDK_w:
					tabs_close_current_page();
					return;
				case GDK_Page_Up:
					tabs_get_previous();
					return;
				case GDK_Page_Down:
					tabs_get_next();
					return;
				default: break;
			}
			break;
		default:
			switch(event->keyval){
				case GDK_F1:
					help_show(main_window_get_window());
					return;
				case GDK_greater:
					online_service_request_selected_update_forward_update();
					return;
				case GDK_at:
					online_service_request_selected_update_reply();
					return;
				case GDK_asciitilde:
					control_panel_new_dm();
					return;
				default: break;
			}
			break;
	}
	update_viewer_key_pressed(tabs_get_current(), event);
}/*hotkey_process(widget, event, user_date);*/

/********************************************************
 *                       eof                            *
 ********************************************************/

