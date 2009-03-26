#!/bin/tcsh -f
set my_editor = "`printf "\""${0}"\"" | sed 's/.*\/\([^\.]\+\).*/\1/g'`"
switch ( "${my_editor}" )
case "gedit":
	breaksw
case "vi":
case "vim":
case "vim-enhanced":
default:
	set my_editor = `printf "%s -p" "vim-enhanced"`
	breaksw
endsw

${my_editor} "./src/tweet-list.h" "./src/app.h" "./src/tweets.h" "./src/.tweet-list.c.swo" "./src/tweet-list.c" "./src/app.c" "./src/friends-manager.h" "./src/friends-manager.c" "./src/tweets.c" "./ChangeLog"
