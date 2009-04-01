/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2006-2007 Imendio AB
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
 * Authors: Richard Hult <richard@imendio.com>
 *	    Brian Pepple <bpepple@fedoraproject.org>
 */

#include "config.h"
#include "paths.h"

gchar * paths_get_gtkbuilder_path (const gchar *filename){
	/* TODO:
	 *	have this test for the file in DATADIR & if its not there yet
	 *	than hade it look for the relative path, from tha binary
	 *	to the gtkbuilder UI files.  Usually its "../data/", filename.
	 *	Though I may just add 'topsrcdir to the command line defines
	 *	while compiling.
	 */
	return g_build_filename( DATADIR, PACKAGE_TARNAME, filename, NULL );
}

gchar *paths_get_locale_path(){
	return g_strdup( LOCALEDIR );
}
