/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * get2gnow is:
 * 	Copyright(c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
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

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <gtk/gtk.h>


#include "config.h"
#include "program.h"

#include "ui-utils.h"
#include "main-window.h"
#include "ipc.h"


#define	MAX_BUFFER_SIZE		4096

typedef struct _Input Input;

struct _Input{
	int		pipe;
	char		*pipe_name;
	GIOChannel	*io;
	guint		io_watch;
	GByteArray	*buffer;
	gboolean	ready;
};

#ifndef IPC_PIPE_PREFIX
#	if defined GNOME_ENABLE_DEBUG
#		define	IPC_PIPE_PREFIX		"%s-debug-%s-"
#	else
#		define	IPC_PIPE_PREFIX		"%s-%s-"
#	endif
#	define	IPC_PIPE_PREFIX_FULL	"%s/" IPC_PIPE_PREFIX "%d"
#endif

#define DEBUG_DOMAINS "Settings:Setup:Start-Up:CLI:Options:IPC:Threads:Pipe:Arguments:Protocol:ipc.c"
#include "debug.h"

static Input *input=NULL;


static void ipc_main(void);
static void ipc_commit(Input *input);
static gboolean ipc_read(G_GNUC_UNUSED GIOChannel *source, GIOCondition condition, Input *input);

gboolean ipc_init_check(int argc, char **argv){
	GDir *dir;
	const char *entry, *user_name, *tmp_path;
	char *prefix;
	guint prefix_len;
	char *cur_dir_tmp;
	char *cur_dir;
	char *to_open;
	
	g_return_val_if_fail(input == NULL, FALSE);
	
	tmp_path=g_get_tmp_dir();
	dir=g_dir_open(tmp_path, 0, NULL);
	g_return_val_if_fail(dir != NULL, FALSE);
	
	user_name=g_get_user_name();
	prefix=g_strdup_printf(IPC_PIPE_PREFIX, user_name, GETTEXT_PACKAGE);
	prefix_len=strlen(prefix);

	cur_dir_tmp=g_get_current_dir();
	cur_dir=g_strdup_printf("%s/", cur_dir_tmp);
	uber_free(cur_dir_tmp);
	
	/* if another process creates a pipe while we are doing this,
	 * we may not get that pipe here. dunno if it's a problem */
	while((entry=g_dir_read_name(dir)) ){
		if(strncmp( entry, prefix, prefix_len ))
			continue;
		
		const char *pid_string;
		pid_t pid;
		char *filename;
		
		errno=0;
		pid_string=entry+prefix_len;
		/* this is not right, but should not cause real problems */
		pid=strtol(pid_string, NULL, 10);
		filename=g_build_filename(tmp_path, entry, NULL);
		
		if(errno && pid <= 0 && kill(pid, 0)){
			unlink(filename);
			uber_free(filename);
			continue;
		}
		/* it would be cool to check that the file is indeed a fifo,
		 * but again, who cares? */
		int fd;
		if((fd=open(filename, O_WRONLY | O_NONBLOCK)) == -1 ){
			perror("open");
			unlink(filename);
			uber_free(filename);
			continue;
		}
		
		/* TODO: validate argumants. */
		gboolean write_check=FALSE;
		if(!(write_check=write(fd, "", 1) )){
			close(fd);
			uber_free(filename);
			g_dir_close(dir);
			uber_free(prefix);
			return FALSE;
		}
		
		for(int i=0; i < argc; ++i) {
			to_open=NULL;
			if(!g_path_is_absolute(argv[i]))
				to_open=g_build_filename(cur_dir, argv[1], NULL);
			else
				to_open=g_build_filename(argv[i], NULL);
			if(!(write_check=write(fd, to_open, strlen(to_open) + 1)) )
				debug("**WARNING:** Failed to write: %s to %s", to_open, filename);
			uber_free(to_open);
		}
		close(fd);
		uber_free(filename);
		g_dir_close(dir);
		uber_free(prefix);
		return TRUE;
	}
	
	g_dir_close(dir);
	uber_free(prefix);
	ipc_main();
	return FALSE;
}


static void ipc_commit(Input *input){
	g_assert(input->buffer->len > 0 && input->buffer->data[input->buffer->len-1] == 0);
	
	window_present(main_window_get_window(), TRUE);
	
	if(input->buffer->len > MAX_BUFFER_SIZE){
		g_byte_array_free(input->buffer, TRUE);
		input->buffer=g_byte_array_new();
	}else{
		g_byte_array_set_size(input->buffer, 0);
	}
	/* Call any methods or ect that you need to handle the input.
	 * 'input->buffer' should be handled just like any element of argv.
	 */
}


static gboolean ipc_read(G_GNUC_UNUSED GIOChannel *source, GIOCondition condition, Input *input ){
	gboolean error_occured=FALSE;
	GError *err=NULL;
	gboolean again=TRUE;
	gboolean got_zero=FALSE;
	
	if(condition &(G_IO_ERR | G_IO_HUP))
		if(errno != EINTR && errno != EAGAIN)
			error_occured=TRUE;
	
	while(again && !error_occured && !err) {
		char c;
		int bytes_read;
		
		struct pollfd fd={input->pipe, POLLIN | POLLPRI, 0};
		
		int res=poll(&fd, 1, 0);
		
		switch(res){
			case -1:
				if(errno != EINTR && errno != EAGAIN)
					error_occured=TRUE;
				perror("poll");
				break;
				
			case 0:
				again=FALSE;
				break;
				
			case 1:
				if(fd.revents &(POLLERR)){
					if(errno != EINTR && errno != EAGAIN)
						error_occured=TRUE;
					perror("poll");
				}else{
					bytes_read=read(input->pipe, &c, 1);
					
					if(bytes_read == 1){
						g_byte_array_append(input->buffer, (guint8 *)&c, 1);
						
						if(!c){
							got_zero=TRUE;
							again=FALSE;
						}
					}else if(bytes_read == -1){
						perror("read");
						if(errno != EINTR && errno != EAGAIN)
							error_occured=TRUE;
					}else{
						again=FALSE;
					}
				}
				break;
				
			default:
				g_assert_not_reached();
		}
	}

	
	if(error_occured || err){
		g_critical("%s: error", G_STRLOC);
		
		if(err){
			g_critical("%s: %s", G_STRLOC, err->message);
			g_error_free(err);
		}
		
		ipc_deinit();
		return FALSE;
	}
	
	if(got_zero) ipc_commit(input);
	return TRUE;
}


static void ipc_main(void){
	g_return_if_fail(input == NULL);
	
	input=g_new0(Input, 1);
	
	input->pipe=-1;
	input->pipe_name=NULL;
	input->io=NULL;
	input->io_watch=0;
	input->ready=FALSE;
	input->buffer=g_byte_array_new();
	
	input->pipe_name=g_strdup_printf(
					IPC_PIPE_PREFIX_FULL,
					g_get_tmp_dir(),
					g_get_user_name(),
					PACKAGE_TARNAME,
					getpid()
	);
	unlink(input->pipe_name);
	
	if((mkfifo( input->pipe_name, S_IRUSR | S_IWUSR )) ){
		perror("mkfifo");
		ipc_deinit();
		return;
	}
	
	if((input->pipe=open(input->pipe_name, O_RDWR | O_NONBLOCK))==-1 ){
		perror("open");
		ipc_deinit();
		return;
	}
	
	input->io=g_io_channel_unix_new(input->pipe);
	g_io_channel_set_encoding(input->io, NULL, NULL);
	input->io_watch=g_io_add_watch(
						input->io,
						G_IO_IN | G_IO_PRI | G_IO_ERR | G_IO_HUP,
						(GIOFunc) ipc_read,
						input
	);
	
	input->ready=TRUE;
}


void ipc_deinit(void){
	if(!input) return;
	
	if(input->io_watch){
		g_source_remove(input->io_watch);
		input->io_watch=0;
	}
	
	if(input->io){
		g_io_channel_shutdown(input->io, TRUE, NULL);
		g_io_channel_unref(input->io);
		input->io=NULL;
	}
	
	if(input->pipe_name){
		input->pipe =-1;
		unlink(input->pipe_name);
		uber_free(input->pipe_name);
		input->pipe_name=NULL;
	}
	
	g_byte_array_free(input->buffer, TRUE);
	uber_free(input);
}/*ipc_deinit();*/

