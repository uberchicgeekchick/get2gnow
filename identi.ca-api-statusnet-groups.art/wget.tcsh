#!/bin/tcsh -f
set starting_dir="${cwd}";
cd /projects/gtk/get2gnow/identi.ca-api-statusnet-groups/;
foreach list_all( "`/bin/ls --width=1 ./list_all.*`" )
	rm --verbose "${list_all}";
end
wget 'http://identi.ca/api/statusnet/groups/list_all.xml?page=1&count=2&since=Tue%20Dec%2001%2016%3A00%3A00%20%2B0000%202009'
wget 'http://identi.ca/api/statusnet/groups/list_all.xml?count=10'
cd "${starting_dir}";
