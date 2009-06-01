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

${my_editor} "./src/online-services.c" "./src/app.c"  "./src/network.c" "./src/network.h" "./src/parser.c" "./src/cache.c" "./src/services-dialog.h" "./src/program.c" "./src/users.c" "./TODO"
