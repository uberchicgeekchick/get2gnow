#!/bin/tcsh -f

cd "`dirname '${0}'`";

if( ! -e "./Makefile" ) exit;

set libdirs=("/usr/lib64" "/usr/lib" "/lib");
if( -e "./config.h" && `/bin/grep --perl-regex 'GNOME_ENABLE_DEBUG' config.h | sed 's/.*GNOME_ENABLE_DEBUG\ \?\([0-1]\?\).*/\1/'` != 1 ) then
	foreach libdir( ${libdirs} ) 
		if( -d "${libdir}" ) break
		unset libdir;
	end
	
	if(!(${?libdir})) then
		printf "**ERROR:** Unable to find libdir to use.  Directories tested:\n\t%s\n" "${libdirs}";
		exit -1;
	endif
	printf "Configure will use %s for its libdir.\n" "${libdir}";
	
	printf "Cleaning up previous make & running ./configure with '--enable-debug' set.\n";
	printf "Running %smake distclean%s\n" '`' '`';
	make distclean;
	printf "Running %s./configure --libdir="${libdir}" --prefix=/usr --enable-debug%s\n" '`' '`';
	./configure --libdir="${libdir}" --prefix=/usr --enable-debug;
endif

printf "Debug environment has been setup.  Running make.";
#make;

