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

#include "config.h"
#include "program.h"

#include "gtkbuilder.h"
#include "gconfig.h"

#include "update-ids.h"
#include "online-services.rest-uris.defines.h"
#include "online-services.typedefs.h"
#include "online-services.h"
#include "online-service.h"

#include "online-service-request.h"
#include "online-service-wrapper.h"

#include "preferences.defines.h"
#include "main-window.h"
#include "update-viewer.h"

#include "best-friends.h"
/********************************************************************************
 *      Project, system, & library headers.  eg #include <gdk/gdkkeysyms.h>     *
 ********************************************************************************/


/********************************************************************************
 *        typedefs: methods, macros, constants, objects, structs, and enums     *
 ********************************************************************************/
typedef struct _BestFriendWidgets BestFriendWidgets;

typedef enum _OnlineServicesBestFriendsTreeStoreColumns OnlineServicesBestFriendsTreeStoreColumns;

struct _BestFriendWidgets{
	gint			total;
	guint			unread;
	
	GtkWindow		*window;
	
	/* OnlineServices "best friends" stuff. */
	GtkVBox			*vbox;
	GtkScrolledWindow	*scrolled_window;
	
	SexyTreeView		*sexy_tree_view;
	GtkTreeViewColumn	*user_name_tree_view_column;
	GtkCellRendererText	*user_name_cell_renderer_text;
	GtkTreeViewColumn	*online_service_tree_view_column;
	GtkCellRendererText	*online_service_cell_renderer_text;
	
	GtkTreeStore		*tree_store;
	
	GtkButton		*add_button;
	GtkButton		*drop_button;
	
	GtkToolButton		*open_all_unread_updates_tool_button;
	GtkToolButton		*mark_all_unread_updates_as_read_tool_button;
	
	GtkButton		*refresh_button;
	
	GList			*buttons;
	GtkButton		*view_updates_new_button;
	GtkButton		*view_updates_button;
	GtkButton		*view_profile_button;
	GtkButton		*send_at_message_button;
	GtkButton		*send_dm_button;
	GtkButton		*mark_as_read_button;
};

enum _OnlineServicesBestFriendsTreeStoreColumns{
	ONLINE_SERVICE_BEST_FRIEND_ONLINE_SERVICE,
	STRING_BEST_FRIEND_USER,
	STRING_BEST_FRIEND_ONlINE_SERVICE_GUID,
	STRING_BEST_FRIEND_USER_NAME,
	GDOUBLE_BEST_FRIENDS_NEWEST_UPDATE_ID,
	GDOUBLE_BEST_FRIENDS_UNREAD_UPDATE_ID,
	GUINT_BEST_FRIENDS_UNREAD_UPDATES,
	STRING_BEST_FRIEND_USER_NAME_AND_ONlINE_SERVICE_KEY,
};


/********************************************************************************
 *                prototypes for private methods & functions                    *
 ********************************************************************************/
static gint best_friends_tree_store_fill(void);
static gint best_friends_tree_store_validate(void);

static gboolean best_friends_save( OnlineService *service );
static void best_friends_append( OnlineService *service, const gchar *user_name, gdouble user_status_id, gboolean save_gslist);
static gboolean best_friends_confirm_clean_up( OnlineService *service, const gchar *user_name );
static gboolean best_friends_remove( OnlineService *service, const gchar *user_name );
static void best_friends_free(void);


/********************************************************************************
 *               object methods, handlers, callbacks, & etc.                    *
 ********************************************************************************/
static gboolean best_friends_get_selected( OnlineService **service, gchar **user_name, gchar **user_name_with_markup, gdouble *newest_update_id, gdouble *unread_update_id);

static void best_friends_resized(GtkScrolledWindow *best_friends_scrolled_window, GtkAllocation *allocation, MainWindow *main_window);
static void best_friends_tree_store_refresh_clicked(GtkButton *best_friends_refresh_button);
static void best_friends_tree_view_row_activated(GtkTreeView *best_friends_tree_view, BestFriendWidgets *best_friend_widgets);

static void best_friends_open_all_unread_updates(GtkToolButton *open_all_unread_updates_tool_button, BestFriendWidgets *best_friend_widgets);
static void best_friends_mark_all_unread_updates_as_read(GtkToolButton *mark_all_unread_updates_as_read_tool_button, BestFriendWidgets *best_friend_widgets);
static void best_friends_button_clicked(GtkButton *button);



/********************************************************************************
 *              Debugging information static objects, and local defines         *
 ********************************************************************************/
#define	DEBUG_DOMAINS	"OnlineServices:BestFriends:Updates:Requests:Settings:Setup:best-friends.c"
#include "debug.h"

static BestFriendWidgets *best_friend_widgets=NULL;

#define	GTK_BUILDER_UI_FILENAME	"main-window"

#define	ONLINE_SERVICE_BEST_FRIENDS			PREFS_ONLINE_SERVICE "/best_friends"


/********************************************************************************
 *              creativity...art, beauty, fun, & magic...programming            *
 ********************************************************************************/
void best_friends_clean_up(void){
	uber_list_free(best_friend_widgets->buttons);
	best_friends_free();
	gtk_tree_store_clear(best_friend_widgets->tree_store);
	uber_free(best_friend_widgets);
}/*best_friends_clean_up();*/

static void best_friends_free(void){
	OnlineService	*service=NULL;
	GList		*accounts=NULL;
	for(accounts=online_services_get_accounts(); accounts; accounts=accounts->next){
		service=(OnlineService *)accounts->data;
		if(service->best_friends)
			g_slist_foreach(service->best_friends, (GFunc)g_free, NULL);
		service=NULL;
	}
}/*best_friends_free();*/

void best_friends_setup(GtkBuilder *ui){
	best_friend_widgets=g_new0(BestFriendWidgets, 1);
	
	best_friend_widgets->total=0;
	best_friend_widgets->unread=0;
	
	gtkbuilder_get_objects(
				ui, GTK_BUILDER_UI_FILENAME,
					"main_window", &best_friend_widgets->window,
					
					/* best friends elements & buttons */
					"best_friends_vbox", &best_friend_widgets->vbox,
					"best_friends_scrolled_window", &best_friend_widgets->scrolled_window,
					
					"best_friends_tree_store", &best_friend_widgets->tree_store,
					
					"best_friends_sexy_tree_view", &best_friend_widgets->sexy_tree_view,
					"best_friends_user_name_tree_view_column", &best_friend_widgets->user_name_tree_view_column,
					"best_friends_user_name_cell_renderer_text", &best_friend_widgets->user_name_cell_renderer_text,
					"best_friends_online_service_tree_view_column", &best_friend_widgets->online_service_tree_view_column,
					"best_friends_online_service_cell_renderer_text", &best_friend_widgets->online_service_cell_renderer_text,
					
					"best_friends_add_button", &best_friend_widgets->add_button,
					"best_friends_drop_button", &best_friend_widgets->drop_button,
					
					"best_friends_open_all_unread_updates_tool_button", &best_friend_widgets->open_all_unread_updates_tool_button,
					"best_friends_mark_all_unread_updates_as_read_tool_button", &best_friend_widgets->mark_all_unread_updates_as_read_tool_button,
					
					"best_friends_refresh_button", &best_friend_widgets->refresh_button,
					
					"best_friends_view_updates_new_button", &best_friend_widgets->view_updates_new_button,
					"best_friends_view_updates_button", &best_friend_widgets->view_updates_button,
					"best_friends_view_profile_button", &best_friend_widgets->view_profile_button,
					"best_friends_send_at_message_button", &best_friend_widgets->send_at_message_button,
					"best_friends_send_dm_button", &best_friend_widgets->send_dm_button,
					"best_friends_mark_as_read_button", &best_friend_widgets->mark_as_read_button,
				NULL
	);
	
	/* Connect the signals */
	gtkbuilder_connect(ui, best_friend_widgets,
					"best_friends_scrolled_window", "size-allocate", best_friends_resized,
					
					"best_friends_sexy_tree_view", "cursor-changed", best_friends_buttons_set_sensitive,
					"best_friends_sexy_tree_view", "row-activated", best_friends_tree_view_row_activated,
					
					"best_friends_add_button", "clicked", online_service_request_popup_best_friend_add,
					"best_friends_drop_button", "clicked", best_friends_button_clicked,
					
					"best_friends_open_all_unread_updates_tool_button", "clicked", best_friends_open_all_unread_updates,
					"best_friends_mark_all_unread_updates_as_read_tool_button", "clicked", best_friends_mark_all_unread_updates_as_read,
					
					"best_friends_refresh_button", "clicked", best_friends_tree_store_refresh_clicked,
					
					"best_friends_view_updates_new_button", "clicked", best_friends_button_clicked,
					"best_friends_view_updates_button", "clicked", best_friends_button_clicked,
					"best_friends_view_profile_button", "clicked", best_friends_button_clicked,
					"best_friends_send_at_message_button", "clicked", best_friends_button_clicked,
					"best_friends_send_dm_button", "clicked", best_friends_button_clicked,
					"best_friends_mark_as_read_button", "clicked", best_friends_button_clicked,
				NULL
	);
	
	/* TODO: fix online_service_best_friends_list_store_validate(); */
	gtk_widget_set_sensitive(GTK_WIDGET(best_friend_widgets->open_all_unread_updates_tool_button), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(best_friend_widgets->mark_all_unread_updates_as_read_tool_button), FALSE);
	gtk_widget_hide(GTK_WIDGET(best_friend_widgets->refresh_button) );
	
	const gchar *best_friends_buttons[]={
		"best_friends_view_updates_new_button",
		"best_friends_view_updates_button",
		"best_friends_view_profile_button",
		"best_friends_send_at_message_button",
		"best_friends_send_dm_button",
		"best_friends_mark_as_read_button",
	};
	
	GList *list=NULL;
	for(gint i=0; i < G_N_ELEMENTS(best_friends_buttons); i++)
		list=g_list_append(list, (gtk_builder_get_object(ui, best_friends_buttons[i])) );
	best_friend_widgets->buttons=list;
	
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(best_friend_widgets->tree_store), STRING_BEST_FRIEND_USER_NAME_AND_ONlINE_SERVICE_KEY, GTK_SORT_ASCENDING);
	
	sexy_tree_view_set_tooltip_label_column(best_friend_widgets->sexy_tree_view, STRING_BEST_FRIEND_USER_NAME);
	
	best_friends_tree_store_fill();
}/*best_friends_setup(ui);*/

static void best_friends_resized(GtkScrolledWindow *best_friends_scrolled_window, GtkAllocation *allocation, MainWindow *main_window){
	return;
	g_object_set(best_friend_widgets->user_name_cell_renderer_text, "wrap-width", ((gtk_tree_view_column_get_width(best_friend_widgets->user_name_tree_view_column))-10), NULL);
	g_object_set(best_friend_widgets->online_service_cell_renderer_text, "wrap-width", ((gtk_tree_view_column_get_width(best_friend_widgets->online_service_tree_view_column))-10), NULL);
}/*best_friends_resized(widget, allocation, main_window);*/

static void best_friends_tree_store_refresh_clicked(GtkButton *best_friends_refresh_button){
	/* TODO: Add a confirmation dialog that warn of how long this may take. */
	best_friends_tree_store_validate();
}/*best_friends_tree_store_refresh_clicked(button);*/

void best_friends_buttons_set_sensitive(void){
	OnlineService *service=NULL;
	gchar *user_name=NULL;
	gchar *user_name_with_markup=NULL;
	gboolean sensitive;
	gdouble newest_update_id=0.0, unread_update_id=0.0;
	if(!((best_friends_get_selected(&service, &user_name, &user_name_with_markup, &newest_update_id, &unread_update_id)) && service && G_STR_N_EMPTY(user_name) && G_STR_N_EMPTY(user_name_with_markup) )){
		sensitive=FALSE;
		debug("No best friend is selected.  Best Friend buttons will be disabled.");
	}else{
		sensitive=TRUE;
		debug("Selected best friend: %s, on <%s>.  Enabling best friend controls.", user_name, service->guid );
		uber_free(user_name);
		uber_free(user_name_with_markup);
	}
	
	GList *buttons=NULL;
	for(buttons=best_friend_widgets->buttons; buttons; buttons=buttons->next)
		gtk_widget_set_sensitive( (GtkWidget *)buttons->data, sensitive );
	g_list_free(buttons);
	
	update_viewer_sexy_select();
}/*best_friends_buttons_set_sensitive();*/

static void best_friends_tree_view_row_activated(GtkTreeView *best_friends_tree_view, BestFriendWidgets *best_friend_widgets){
	OnlineService *service=NULL;
	gchar *user_name=NULL;
	gchar *user_name_with_markup=NULL;
	gdouble newest_update_id=0.0, unread_update_id=0.0;
	if(!((best_friends_get_selected(&service, &user_name, &user_name_with_markup, &newest_update_id, &unread_update_id)) && service && G_STR_N_EMPTY(user_name) && G_STR_N_EMPTY(user_name) )){
		debug("Cannot load best friends' updates.  Invalid OnlineService or empty user_name.");
		update_viewer_sexy_select();
		return;
	}
	
	if(!g_str_has_prefix(user_name_with_markup, "<b>"))
		online_service_request_view_updates(service, best_friend_widgets->window, user_name);
	else
		online_service_request_view_updates_new(service, best_friend_widgets->window, user_name);
	
	uber_free(user_name);
	uber_free(user_name_with_markup);
	update_viewer_sexy_select();
}/*best_friends_tree_view_row_activated(best_friend_widgets->tree_view, best_friend_widgets);*/

static void best_friends_button_clicked(GtkButton *button){
	OnlineService *service=NULL;
	gchar *user_name=NULL;
	gchar *user_name_with_markup=NULL;
	gdouble newest_update_id=0.0;
	gdouble unread_update_id=0.0;
	if(!((best_friends_get_selected(&service, &user_name, &user_name_with_markup, &newest_update_id, &unread_update_id)) && service && G_STR_N_EMPTY(user_name) && G_STR_N_EMPTY(user_name_with_markup) )){
		if(button==best_friend_widgets->drop_button){
			online_service_request_popup_best_friend_drop();
			return;
		}
		debug("Cannot load best friends request.  Invalid OnlineService or empty user_name.");
		if(user_name) uber_free(user_name);
		if(user_name_with_markup) uber_free(user_name_with_markup);
		update_viewer_sexy_select();
		return;
	}
	uber_free(user_name_with_markup);
	
	if(button==best_friend_widgets->view_updates_button){
		online_service_request_view_updates(service, main_window_get_window(), user_name);
		update_viewer_sexy_select();
		uber_free(user_name);
		return;
	}
	
	if(button==best_friend_widgets->view_updates_new_button){
		online_service_request_view_updates_new(service, main_window_get_window(), user_name);
		update_viewer_sexy_select();
		uber_free(user_name);
		return;
	}
	
	if(button==best_friend_widgets->drop_button){
		best_friends_drop(service, main_window_get_window(), user_name);
		update_viewer_sexy_select();
		uber_free(user_name);
		return;
	}
	
	if(button==best_friend_widgets->view_profile_button){
		online_service_request_view_profile(service, main_window_get_window(), user_name);
		update_viewer_sexy_select();
		uber_free(user_name);
		return;
	}
	
	if(button==best_friend_widgets->send_at_message_button){
		update_viewer_set_in_reply_to_data(service, update_viewer_get_update_monitoring(), user_name, NULL, unread_update_id, unread_update_id, TRUE, FALSE, FALSE);
		uber_free(user_name);
		return;
	}
	
	if(button==best_friend_widgets->send_dm_button){
		update_viewer_best_friends_start_dm(service, user_name);
		update_viewer_sexy_select();
		uber_free(user_name);
		return;
	}
	
	if(button==best_friend_widgets->mark_as_read_button){
		best_friends_tree_store_mark_as_read(service, user_name, newest_update_id);
		update_viewer_sexy_select();
		uber_free(user_name);
		return;
	}
}/*best_friends_button_clicked(button);*/

gboolean best_friends_load(OnlineService *service){
	if(!service) return FALSE;
	gchar *gconf_prefs_path=g_strdup_printf( ONLINE_SERVICE_BEST_FRIENDS, service->key );
	gboolean loaded=gconfig_get_list_string( gconf_prefs_path, &service->best_friends );
	service->best_friends=g_slist_sort( service->best_friends, (GCompareFunc)strcasecmp );
	uber_free(gconf_prefs_path);
	return loaded;
}/*best_friends_load(service);*/

static gboolean best_friends_save(OnlineService *service){
	if(!service) return FALSE;
	gchar *gconf_prefs_path=g_strdup_printf( ONLINE_SERVICE_BEST_FRIENDS, service->key );
	service->best_friends=g_slist_sort( service->best_friends, (GCompareFunc)strcasecmp );
	gboolean saved=gconfig_set_list_string( gconf_prefs_path, service->best_friends );
	uber_free(gconf_prefs_path);
	service->best_friends=g_slist_nth( service->best_friends, 0 );
	return saved;
}/*best_friends_save(service);*/

gboolean best_friends_add(OnlineService *service, const gchar *user_name){
	if(!service) return FALSE;
	if(!(service->best_friends && G_STR_N_EMPTY(user_name) )) return FALSE;
	gboolean found=best_friends_is_user_best_friend( service, user_name );
	
	if(found){
		debug( "Cannot add: %s to <%s>'s best_friends.  %s is already listed in <%s>'s best friends list.", user_name, service->guid, user_name, service->guid );
		statusbar_printf( "%s is already one of your, <%s>'s, best friends.", user_name, service->guid );
	}else{
		debug( "Attempting to load: %s's profile to add them to <%s>'s best_friends.", user_name, service->guid );
		statusbar_printf( "Adding %s's to your, <%s>, best friends.", user_name, service->guid );
		online_service_fetch_profile( service, user_name, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)best_friends_tree_store_update_check );
	}
	
	return !found;
}/*best_friends_add( OnlineService *service, const gchar *user_name );*/

gboolean best_friends_drop(OnlineService *service, GtkWindow *parent, const gchar *user_name){
	if(!service) return FALSE;
	if(!(service->best_friends && G_STR_N_EMPTY(user_name) )) return FALSE;
	gboolean found=best_friends_is_user_best_friend( service, user_name );
	
	if(!found){
		debug( "Cannot remove: %s from <%s>'s best_friends.  %s was not found in <%s>'s best friends list.", user_name, service->guid, user_name, service->guid );
		statusbar_printf( "%s is not one of your, <%s>'s, best friends.", user_name, service->guid );
		online_service_request_best_friend_drop(service, parent, user_name);
	}else{
		debug( "Attempting to load: %s's profile to remove them from <%s>'s best_friends.", user_name, service->guid );
		statusbar_printf( "Removing %s's from your, <%s>, best friends.", user_name, service->guid );
		online_service_fetch_profile( service, user_name, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)best_friends_tree_store_update_check );
	}
	
	return found;
}/*best_friends_drop( service, user_name );*/

void best_friends_tree_store_update_check(OnlineServiceWrapper *online_service_wrapper, SoupMessage *xml, User *user){
	OnlineService *service=online_service_wrapper_get_online_service(online_service_wrapper);
	if(!service) return;
	if(!user){
		debug( "User %s's profile could not be found, on %s.  Their user name has most likely changed.  Though unlikely its possible the netwok connection may have been lost.  Unlikely because prior 'status' checks would have kept this method from being called.", user->user_name, service->guid );
		best_friends_confirm_clean_up( service, user->user_name );
		return;
	}
	
	if(!best_friends_is_user_best_friend(service, user->user_name)){
		debug("Adding best friend %s, on %s to the best friends tree_store & GSList.", user->user_name, service->guid);
		best_friends_append(service, user->user_name, user->status->id, TRUE);
	}else{
		debug("Removing best friend %s, on %s from the best friends tree_store & GSList.", user->user_name, service->guid);
		best_friends_remove(service, user->user_name);
	}
}/*best_friends_tree_store_update_check( online_service_wrapper, xml, user );*/

static void best_friends_append(OnlineService *service, const gchar *user_name, gdouble user_status_id, gboolean save_gslist){
	if(!service) return;
	
	if(save_gslist){
		service->best_friends=g_slist_append( service->best_friends, g_strdup(user_name) );
		best_friends_save(service);
	}
	
	gchar *user_timeline=g_strdup_printf(API_TIMELINE_USER, user_name );
	gdouble newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	update_ids_get(service, user_timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	if(save_gslist)
		update_ids_set( service, user_timeline, ( (newest_update_id>user_status_id) ?newest_update_id :user_status_id ), ( (unread_update_id>user_status_id) ?unread_update_id :user_status_id ), user_status_id );
	uber_free(user_timeline);
	
	gchar *best_friend_user_name_and_onLine_service_key=g_strdup_printf("%s@%s", user_name, service->uri);
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	gtk_tree_store_append(best_friend_widgets->tree_store, iter, NULL);
	gtk_tree_store_set(
				best_friend_widgets->tree_store, iter,
					ONLINE_SERVICE_BEST_FRIEND_ONLINE_SERVICE, service,
					STRING_BEST_FRIEND_USER, user_name,
					STRING_BEST_FRIEND_ONlINE_SERVICE_GUID, service->guid,
					STRING_BEST_FRIEND_USER_NAME, user_name,
					GUINT_BEST_FRIENDS_UNREAD_UPDATES, 0,
					GDOUBLE_BEST_FRIENDS_NEWEST_UPDATE_ID, ( (newest_update_id>user_status_id) ?newest_update_id :user_status_id ),
					GDOUBLE_BEST_FRIENDS_UNREAD_UPDATE_ID, ( (unread_update_id>user_status_id) ?unread_update_id :user_status_id ),
					STRING_BEST_FRIEND_USER_NAME_AND_ONlINE_SERVICE_KEY, best_friend_user_name_and_onLine_service_key,
				-1
	);
	uber_free(best_friend_user_name_and_onLine_service_key);
	uber_free(iter);
	best_friend_widgets->total++;
}/*best_friends_append(service, user->name, user->status->id, TRUE|FALSE);*/

static gboolean best_friends_confirm_clean_up( OnlineService *service, const gchar *user_name ){
	if(!service) return FALSE;
	gchar *message=NULL;
	debug( "Failed to fetch <%s>'s best_friend: %s.", service->guid, user_name );
	statusbar_printf( "<%s> loading best friend from: %s [failed]", service->guid, user_name );
	if(online_service_request_popup_confirmation_dialog(
			ONLINE_SERVICE_CONFIRM_BEST_FRIENDS_CLEAN_UP,
			_("Unable to find a best friend:"),
			(message=g_strdup_printf( "%s was unable to load one of your best friends.\n%s could not be found on <%s>.\nThis usually means you're not connected to <%s>\nOr your best friend may have changed their user name.\n\nWould you like to remove %s from <%s>'s best friends?", _(GETTEXT_PACKAGE), user_name, service->guid, service->guid, user_name, service->guid )),
			NULL, NULL
	)){
		uber_free(message);
		return best_friends_remove( service, user_name );
	}
	
	uber_free(message);
	return FALSE;
}/*best_friends_confirm_clean_up( service, user_name );*/

static gboolean best_friends_remove( OnlineService *service, const gchar *user_name ){
	if(!service) return FALSE;
	
	GSList *best_friends=NULL;
	for(best_friends=g_slist_nth(service->best_friends, 0); best_friends; best_friends=best_friends->next)
		if(g_str_equal((gchar *)best_friends->data, user_name)){
			service->best_friends=g_slist_remove(service->best_friends, best_friends->data);
			break;
		}
	best_friends_save(service);
	
	OnlineService *service_at_index=NULL;
	gchar *user_name_at_index=NULL;
	for(gint i=0; i<=best_friend_widgets->total; i++){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(best_friend_widgets->tree_store), iter, path)){
			debug("Removing iter at index: %d failed.  Unable to retrieve iter from path.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		gtk_tree_model_get(
				GTK_TREE_MODEL(best_friend_widgets->tree_store), iter,
					ONLINE_SERVICE_BEST_FRIEND_ONLINE_SERVICE, &service_at_index,
					STRING_BEST_FRIEND_USER, &user_name_at_index,
				-1
		);
		
		if(!(service==service_at_index && g_str_equal(user_name, user_name_at_index))){
			uber_free(user_name_at_index);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		debug("Removing best friend: %s from iter at index: %d", user_name_at_index, i);
		service->best_friends_total--;
		best_friend_widgets->total--;
		gtk_tree_store_remove(best_friend_widgets->tree_store, iter);
		
		uber_free(user_name_at_index);
		gtk_tree_path_free(path);
		uber_free(iter);
		return TRUE;
	}
	debug("Could not remove %s, on %s.  The user could not be found.", user_name, service->guid);
	return FALSE;
}/*best_friends_remove( service, user_name );*/

static void best_friends_open_all_unread_updates(GtkToolButton *open_all_unread_updates_tool_button, BestFriendWidgets *best_friend_widgets){
	for(gint i=0; i<=best_friend_widgets->total; i++){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(best_friend_widgets->tree_store), iter, path)){
			debug("Removing iter at index: %d failed.  Unable to retrieve iter from path.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		OnlineService *service=NULL;
		gchar *user_name=NULL;
		guint unread_updates=0;
		gtk_tree_model_get(
				GTK_TREE_MODEL(best_friend_widgets->tree_store), iter,
					ONLINE_SERVICE_BEST_FRIEND_ONLINE_SERVICE, &service,
					STRING_BEST_FRIEND_USER, &user_name,
					GUINT_BEST_FRIENDS_UNREAD_UPDATES, &unread_updates,
				-1
		);
		if(!unread_updates){
			uber_free(user_name);
			continue;
		}
		
		online_service_request_view_updates_new(service, main_window_get_window(), user_name);
		uber_free(user_name);
	}
	update_viewer_sexy_select();
}/*best_friends_open_all_unread_updates(best_friend_widgets->open_all_as_read_button, best_friend_widgets);*/

static void best_friends_mark_all_unread_updates_as_read(GtkToolButton *mark_all_unread_updates_as_read_tool_button, BestFriendWidgets *best_friend_widgets){
	for(gint i=0; i<=best_friend_widgets->total; i++){
		GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter(GTK_TREE_MODEL(best_friend_widgets->tree_store), iter, path)){
			debug("Removing iter at index: %d failed.  Unable to retrieve iter from path.", i);
			gtk_tree_path_free(path);
			uber_free(iter);
			continue;
		}
		
		OnlineService *service=NULL;
		gchar *user_name=NULL;
		guint unread_updates=0;
		gdouble newest_update_id=0.0;
		gtk_tree_model_get(
				GTK_TREE_MODEL(best_friend_widgets->tree_store), iter,
					ONLINE_SERVICE_BEST_FRIEND_ONLINE_SERVICE, &service,
					STRING_BEST_FRIEND_USER, &user_name,
					GDOUBLE_BEST_FRIENDS_NEWEST_UPDATE_ID, &newest_update_id,
					GUINT_BEST_FRIENDS_UNREAD_UPDATES, &unread_updates,
				-1
		);
		if(!unread_updates){
			uber_free(user_name);
			continue;
		}
		
		best_friends_tree_store_mark_as_read(service, user_name, newest_update_id);
		uber_free(user_name);
	}
	update_viewer_sexy_select();
}/*best_friends_mark_all_unread_updates_as_read(best_friend_widgets->mark_all_unread_updates_as_read_tool_button, best_friend_widgets);*/


gint best_friends_adjust_total_by(gint best_friends_adjustment){
	return (best_friend_widgets->total+=best_friends_adjustment);
}/*best_friends_adjust_total_by( 1 || -1);*/


static gint best_friends_tree_store_fill(void){
	GList		*accounts=NULL;
	gtk_tree_store_clear(best_friend_widgets->tree_store);
	best_friend_widgets->total=0;
	for(accounts=online_services_get_accounts(); accounts; accounts=accounts->next){
		GSList *best_friends=NULL;
		OnlineService *service=(OnlineService *)accounts->data;
		debug("Loading <%s>'s best_friends.", service->key );
		service->best_friends_total=0;
		for(best_friends=g_slist_nth(service->best_friends, 0); best_friends; best_friends=best_friends->next){
			best_friends_append( service, (const gchar *)best_friends->data, 0.0, FALSE);
			service->best_friends_total++;
		}
		uber_slist_free(best_friends);
		best_friend_widgets->total+=service->best_friends_total;
	}
	return best_friend_widgets->total;
}/*best_friends_tree_store_fill(void);*/


static gint best_friends_tree_store_validate(void){
	GList		*accounts=NULL;
	gtk_tree_store_clear(best_friend_widgets->tree_store);
	best_friend_widgets->total=0;
	for(accounts=online_services_get_accounts(); accounts; accounts=accounts->next){
		OnlineService *service=(OnlineService *)accounts->data;
		debug("Validating <%s>'s best_friends.", service->key );
		service->best_friends_total=0;
		GSList *best_friends=NULL;
		for( best_friends=g_slist_nth( service->best_friends, 0 ); best_friends; best_friends=best_friends->next)
			online_service_fetch_profile( service, (const gchar *)best_friends->data, (OnlineServiceSoupSessionCallbackReturnProcessorFunc)best_friends_tree_store_update_check );
	}
	return best_friend_widgets->total;
}/*best_friends_tree_store_validate();*/

static gboolean best_friends_tree_store_get_user_iter(OnlineService *service, const gchar *user_name, GtkTreeIter **iter){
	if(!(best_friend_widgets->total && G_STR_N_EMPTY(user_name))) return FALSE;
	
	OnlineService *service_at_index=NULL;
	gchar *user_at_index=NULL;
	for(gint i=0; i<=best_friend_widgets->total; i++){
		*iter=g_new0(GtkTreeIter, 1);
		GtkTreePath *path=gtk_tree_path_new_from_indices(i, -1);
		if(!gtk_tree_model_get_iter( GTK_TREE_MODEL(best_friend_widgets->tree_store), *iter, path)){
			debug("Failed to get best friend: %s, on %s aned index: %d, as read has failed.  Unable to retrieve iter from path.", user_name, service->guid, i);
			gtk_tree_path_free(path);
			uber_free(*iter);
			continue;
		}
		
		gtk_tree_model_get(
				GTK_TREE_MODEL(best_friend_widgets->tree_store), *iter,
					ONLINE_SERVICE_BEST_FRIEND_ONLINE_SERVICE, &service_at_index,
					STRING_BEST_FRIEND_USER, &user_at_index,
				-1
		);
		if(!( service==service_at_index && !strcasecmp(user_name, user_at_index))){
			gtk_tree_path_free(path);
			uber_free(user_at_index);
			uber_free(*iter);
			continue;
		}
		
		debug("Found best friend iter for best friend: %s, on service: <%s>, at index: %d.", user_at_index, service_at_index->guid, i);
		gtk_tree_path_free(path);
		uber_free(user_at_index);
		return TRUE;
	}
	debug("Unable to find best friend iter for best friend: %s, on service: <%s>.", user_at_index, service_at_index->guid);
	uber_free(user_at_index);
	if(*iter) uber_free(*iter);
	return FALSE;
}/*best_friends_tree_store_get_user_iter(service, user_name, tree_store, &iter);*/

gdouble best_friends_tree_store_mark_as_unread(OnlineService *service, const gchar *user_name, gdouble update_id){
	if(!(best_friend_widgets->total && G_STR_N_EMPTY(user_name))) return FALSE;
	
	GtkTreeIter *iter=NULL;
	if(!best_friends_tree_store_get_user_iter(service, user_name, &iter)){
		debug("User: %s  on service: <%s>, could not be found in your best friends list store.", user_name, service->guid);
		return 0.0;
	}
	
	guint unread_updates=0;
	gdouble newest_update_id=0.0, unread_update_id=0.0;
	gchar *user_at_index=NULL, *user_name_at_index=NULL;
	gtk_tree_model_get(
			GTK_TREE_MODEL(best_friend_widgets->tree_store), iter,
				STRING_BEST_FRIEND_USER, &user_at_index,
				STRING_BEST_FRIEND_USER_NAME, &user_name_at_index,
				GDOUBLE_BEST_FRIENDS_NEWEST_UPDATE_ID, &newest_update_id,
				GDOUBLE_BEST_FRIENDS_UNREAD_UPDATE_ID, &unread_update_id,
				GUINT_BEST_FRIENDS_UNREAD_UPDATES, &unread_updates,
			-1
	);
	
	if(!(unread_update_id && update_id && update_id > unread_update_id )) return unread_update_id;
	
	unread_updates++;
	if(!g_str_has_prefix(user_name_at_index, "<b>")){
		if(!best_friend_widgets->unread){
			gtk_widget_set_sensitive(GTK_WIDGET(best_friend_widgets->open_all_unread_updates_tool_button), TRUE);
			gtk_widget_set_sensitive(GTK_WIDGET(best_friend_widgets->mark_all_unread_updates_as_read_tool_button), TRUE);
		}
		best_friend_widgets->unread++;
		uber_free(user_name_at_index);
		user_name_at_index=g_strdup_printf("<b>%s</b>", user_at_index);
	}
	
	debug("Marking best friend: %s(%s), on service <%s>, as having %d unread updates.  Last read update: %f; current update ID: %f..", user_at_index, user_name_at_index, service->guid, unread_updates, unread_update_id, update_id);
	gtk_tree_store_set(
			best_friend_widgets->tree_store, iter,
				STRING_BEST_FRIEND_USER_NAME, user_name_at_index,
				GDOUBLE_BEST_FRIENDS_NEWEST_UPDATE_ID, (update_id>newest_update_id ?update_id :newest_update_id),
				GDOUBLE_BEST_FRIENDS_UNREAD_UPDATE_ID, unread_update_id,
				GUINT_BEST_FRIENDS_UNREAD_UPDATES, unread_updates,
			-1
	);
	
	uber_free(iter);
	uber_free(user_at_index);
	uber_free(user_name_at_index);
	return update_id;
}/*best_friends_tree_store_mark_as_unread(service, user_name, unread_update_id);*/

gboolean best_friends_tree_store_mark_as_read(OnlineService *service, const gchar *user_name, gdouble update_id){
	if(!(best_friend_widgets->total && G_STR_N_EMPTY(user_name))) return FALSE;
	
	GtkTreeIter *iter=NULL;
	if(!best_friends_tree_store_get_user_iter(service, user_name, &iter)){
		debug("User: %s  on service: <%s>, could not be found in your best friends list store.", user_name, service->guid);
		return FALSE;
	}
	
	guint unread_updates=0;
	gchar *user_at_index=NULL;
	gtk_tree_model_get(
			GTK_TREE_MODEL(best_friend_widgets->tree_store), iter,
				STRING_BEST_FRIEND_USER, &user_at_index,
				GUINT_BEST_FRIENDS_UNREAD_UPDATES, &unread_updates,
			-1
	);
	
	gchar		*user_timeline=g_strdup_printf("/%s.xml", user_at_index);
	gdouble		newest_update_id=0.0, unread_update_id=0.0, oldest_update_id=0.0;
	update_ids_get(service, user_timeline, &newest_update_id, &unread_update_id, &oldest_update_id);
	update_ids_set(service, user_timeline, ( (update_id>newest_update_id) ?update_id :newest_update_id) , ( (update_id>unread_update_id) ?(unread_update_id=update_id) :unread_update_id ), ( (update_id>oldest_update_id) ?update_id :oldest_update_id));
	uber_free(user_timeline);
	
	debug("Marking best friend: %s, on service <%s>, as having all of their updates as read.", user_at_index, service->guid);
	gtk_tree_store_set(
			best_friend_widgets->tree_store, iter,
				STRING_BEST_FRIEND_USER_NAME, user_at_index,
				GUINT_BEST_FRIENDS_UNREAD_UPDATES, 0,
				GDOUBLE_BEST_FRIENDS_UNREAD_UPDATE_ID, unread_update_id,
			-1
	);
	
	if(best_friend_widgets->unread)
		best_friend_widgets->unread--;
	if(!best_friend_widgets->unread){
		gtk_widget_set_sensitive(GTK_WIDGET(best_friend_widgets->open_all_unread_updates_tool_button), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(best_friend_widgets->mark_all_unread_updates_as_read_tool_button), FALSE);
	}
	
	uber_free(iter);
	uber_free(user_at_index);
	return TRUE;
}/*best_friends_tree_store_mark_as_read(service, user_name, unread_update_id);*/

gboolean best_friends_is_user_best_friend(OnlineService *service, const gchar *user_name){
	if(G_STR_EMPTY(user_name)) return FALSE;
	GList		*accounts=NULL;
	for(accounts=online_services_get_accounts(); accounts; accounts=accounts->next){
		if(service!=(OnlineService *)accounts->data) continue;
		GSList *best_friends=NULL;
		for( best_friends=((OnlineService *)accounts->data)->best_friends; best_friends; best_friends=best_friends->next )
			if(!strcasecmp( user_name, (gchar *)best_friends->data ))
				return TRUE;
	}
	g_list_free(accounts);
	return FALSE;
}/*best_friends_is_user_best_friend(service, user_name);*/

gboolean best_friends_get_selected(OnlineService **service, gchar **user_name, gchar **user_name_with_markup, gdouble *newest_update_id, gdouble *unread_update_id ){
	GtkTreeIter *iter=g_new0(GtkTreeIter, 1);
	gboolean found=FALSE;
	OnlineService *selected_service=NULL;
	gchar *selected_user_name=NULL;
	gchar *selected_user_name_with_markup=NULL;
	gdouble selected_newest_update_id=0.0;
	gdouble selected_update_id=0.0;
	GtkTreeSelection *sel=gtk_tree_view_get_selection( (GtkTreeView *)best_friend_widgets->sexy_tree_view );
	if(gtk_tree_selection_get_selected(sel, NULL, iter))
		gtk_tree_model_get(
				GTK_TREE_MODEL(best_friend_widgets->tree_store), iter,
					ONLINE_SERVICE_BEST_FRIEND_ONLINE_SERVICE, &selected_service,
					STRING_BEST_FRIEND_USER, &selected_user_name,
					STRING_BEST_FRIEND_USER_NAME, &selected_user_name_with_markup,
					GDOUBLE_BEST_FRIENDS_NEWEST_UPDATE_ID, &selected_newest_update_id,
					GDOUBLE_BEST_FRIENDS_UNREAD_UPDATE_ID, &selected_update_id,
				-1
		);
	if(!( selected_service && selected_service->connected && G_STR_N_EMPTY(selected_user_name) && G_STR_N_EMPTY(selected_user_name_with_markup) )){
		if(selected_user_name) uber_free(selected_user_name);
		if(selected_user_name_with_markup) uber_free(selected_user_name_with_markup);
		*service=NULL;
		*user_name=NULL;
		*user_name_with_markup=NULL;
		*unread_update_id=0.0;
		*newest_update_id=0.0;
	}else{
		found=TRUE;
		*user_name=g_strdup(selected_user_name);
		uber_free(selected_user_name);
		*user_name_with_markup=g_strdup(selected_user_name_with_markup);
		uber_free(selected_user_name_with_markup);
		*service=selected_service;
		if(selected_update_id) *unread_update_id=selected_update_id;
		else *unread_update_id=0.0;
		if(selected_newest_update_id) *newest_update_id=selected_newest_update_id;
		else *newest_update_id=0.0;
	}
	
	uber_free(iter);
	return found;
}/*best_friends_get_selected(&service, &user_name, &user_name_with_markup, &newest_update_id, &unread_update_id);*/

gboolean best_friends_check_update_ids(OnlineService *service, const gchar *best_friends_user_name, gdouble update_id){
	gdouble unread_update_id=0.0;
	gchar *user_timeline=g_strdup_printf(API_TIMELINE_USER, best_friends_user_name );
	gdouble best_friends_newest_update_id=0.0, best_friends_unread_update_id=0.0, best_friends_oldest_update_id=0.0;
	update_ids_get( service, user_timeline, &best_friends_newest_update_id, &best_friends_unread_update_id, &best_friends_oldest_update_id );
	gboolean notify_of_new_update=FALSE, save_update_ids=FALSE;;
	unread_update_id=best_friends_tree_store_mark_as_unread(service, best_friends_user_name, update_id);
	if( unread_update_id > best_friends_unread_update_id ){
		debug("OnlineService: <%s>'s Best Friend: <%s@%s> has a new unread update.  Update->ID: %f.  Previous unread_update_id: %f.", service->guid, best_friends_user_name, service->uri, unread_update_id, best_friends_unread_update_id);
		notify_of_new_update=TRUE;
		save_update_ids=TRUE;
		best_friends_unread_update_id=unread_update_id;
	}
	
	if( update_id > best_friends_newest_update_id ){
		debug("OnlineService: <%s>'s Best Friend: <%s@%s> has a new newest update.  Update->ID: %f.  Previous newest_update_id: %f.", service->guid, best_friends_user_name, service->uri, update_id, best_friends_newest_update_id);
		if(!notify_of_new_update) notify_of_new_update=TRUE;
		if(!save_update_ids) save_update_ids=TRUE;
		best_friends_newest_update_id=update_id;
	}
	
	if( update_id < best_friends_oldest_update_id ){
		debug("OnlineService: <%s>'s Best Friend: <%s@%s> has a new oldest update.  Update->ID: %f.  Previous oldest_update_id: %f.", service->guid, best_friends_user_name, service->uri, update_id, best_friends_oldest_update_id);
		if(!save_update_ids) save_update_ids=TRUE;
		best_friends_oldest_update_id=update_id;
	}
	
	uber_free(user_timeline);
	return notify_of_new_update;
}/*best_friends_check_update_ids( service, update->user->user_name, update->id);*/

gboolean best_friends_new_update_notify(void){
	return gconfig_if_bool(PREFS_NOTIFY_BEST_FRIENDS, TRUE);
}/*best_friends_new_update_notify();*/
/********************************************************************************
 *                                    eof                                       *
 ********************************************************************************/

