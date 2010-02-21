#!/bin/tcsh -f

if( "`/bin/ls ./list_all.xml*`" != "" ) rm "./list_all.xml"*;

printf "Downloading: list_xml?page=1 POST: page=1\n";
curl --output "./list_all.xml:get-and-post:page=1" --data "page=1" "http://identi.ca/api/statusnet/groups/list_all.xml?page=1";
printf "\n";

printf "Downloading: list_xml?page=1&count=10 POST: page=1&count=10\n";
curl --output "./list_all.xml:get-and-post:page=1&count=10" --data "page=1&count=10" "http://identi.ca/api/statusnet/groups/list_all.xml?page=1&count=10";
printf "\n";

printf "Downloading: list_xml?page=1\n";
curl --output "./list_all.xml:get:page=1" "http://identi.ca/api/statusnet/groups/list_all.xml?page=1";
printf "\n";

printf "Downloading: list_xml?count=10\n";
curl --output "./list_all.xml:get:count=10" "http://identi.ca/api/statusnet/groups/list_all.xml?count=10";
printf "\n";

printf "Downloading: list_xml POST: page=1\n";
curl --output "./list_all.xml:post:page=1" --data "page=1" "http://identi.ca/api/statusnet/groups/list_all.xml";
printf "\n";

printf "Downloading: list_xml POST: count=10\n";
curl --output "./list_all.xml:post:count=10" --data "count=10" "http://identi.ca/api/statusnet/groups/list_all.xml";
printf "\n";


