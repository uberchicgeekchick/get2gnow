#!/bin/tcsh -f
if( "${1}" == "" ) goto script_usage;

set extension="`printf '%s' '${1}' | sed -r 's/.*\.([ch])"\$"'`";
if( "${extension} != "c" && "${extension}" != "h" ) goto script_usage;

script_main:
alias	ex	'ex --noplugin -X -n -E';

ex '+1,$s/\v([^(]+)\ \(\ ?(.*)\ ?\)/\1\(\2\)/g' '+1,$s/\v([^(]+)\(\ (.*)\ ?\)/\1\(\2\)/g' '+1,$s/\v([^(]+)\((.*)\ \)/\1\(\2\)/g' "${src}";

script_main_quit:
	exit ${status};

script_usage:
	printf "Usage: %s [source code file.c|header file.h|etc functional script.tcsh|server side scripting.php]" "`basename '${0}'`";
	set status=-1;
	goto script_main_quit;

