#!/bin/tcsh -f
set my_editor = "`printf "${0}" | sed 's/.*\/\([^\.]\+\).*/\1/g'`"
switch ( "${my_editor}" )
case "gedit":
	breaksw
case "vim":
default:
	set my_editor = `printf "%s -p %s" "vim" '+tabdo$-2'`
	breaksw
endsw

${my_editor} "./src/twitux-tweet-list.c" "./src/Makefile.in" "./src/twitux-url-encoding.h" "./src/twitux-send-message-dialog.c" "./src/twitux-url-encoding.c" "./src/Makefile.am"
