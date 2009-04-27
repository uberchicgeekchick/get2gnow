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

${my_editor} "./src/app.c" "./src/label.c" "./src/network.c" "./src/network.h" "./src/oauth.c" "./src/oauth.h" "./src/tweet-list.c" "./src/tweet-list.h" "./src/tweets.c" "./src/users.c"
