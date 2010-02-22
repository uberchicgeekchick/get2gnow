#!/bin/tcsh -f
set objects=( "libpthread.so" "libpthread.a" "libdl.so" "libdl.a" "libm.so" "libm.a" "librt.so" "librt.a" "libpthread.so" "libpthread.a" "libc.so" "libc.a" );

foreach object ( ${objects} )
	mv "/usr/lib/${object}" "/usr/lib/${object}.32bit.bck";
	ln -s "/usr/lib64/${object}" "/usr/lib/${object}";
end
