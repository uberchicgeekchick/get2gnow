#!/bin/tcsh -f
set my_editor = "`printf "\""${0}"\"" | sed 's/.*\/\([^\.]\+\).*/\1/g'`"
switch ( "${my_editor}" )
case "connectED":
case "gedit":
	breaksw
case "vi":
case "vim":
case "vim-enhanced":
default:
	set my_editor = `printf "%s -p" "vim-enhanced"`
	breaksw
endsw

${my_editor} "./TODO" "./src/online-services.c" "./src/network.c" "./src/parser.c" "./src/popup-dialog.c" "./src/cache.c" "./src/oauth.c" "./dev/oauth.c.dev" "./src/friends-manager.c"
