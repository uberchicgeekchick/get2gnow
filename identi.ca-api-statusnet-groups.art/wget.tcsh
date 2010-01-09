#!/bin/tcsh -f
set starting_dir="${cwd}";
cd "`dirname '${0}'`";

if( "`/bin/ls --width=1 ./list_all.*`" != "" ) rm --verbose "./list_all."*;

wget "http://identi.ca/api/statusnet/groups/list_all.xml?page=1&count=2&since=`date '+%c'`";
wget "http://identi.ca/api/statusnet/groups/list_all.xml?count=10";

cd "${starting_dir}";
