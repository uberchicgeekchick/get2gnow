#!/bin/tcsh -f
gconftool-2 --recursive-unset /apps/get2gnow/debug
if( -d ~/.gnome2/get2gnow/debug ) rm -r ~/.gnome2/get2gnow/debug ;
if ( ! -e ./Makefile ) exit;
make distclean
./configure --libdir=/usr/lib64 --prefix=/usr
make

