#!/bin/tcsh -f
cd /projects/gtk;
set version=`/bin/grep PACKAGE_VERSION= get2gnow/configure | sed 's/.*="\([^"]\+\)"/\1/'`
cd get2gnow;
./configure --libdir=/usr/lib64 --prefix=/usr;
cd ..;
printf "Creating tarball\t\t\t";
ln -s get2gnow get2gnow-$version;
tar --dereference -czf releases/tarballs/get2gnow-$version.tar.gz get2gnow-$version;
rm get2gnow-$version;
printf "[done]";
cd get2gnow;
make distclean;
sudo rpmbuild -ta releases/tarballs/get2gnow-$version.tar.gz;
if( "${status}" != "0" ) then
	printf "***********ERROR: rpm build failed****************";
	exit -1;
endif
rm releases/rpms/get2gnow*;
cp /usr/src/packages/RPMS/x86_64/get2gnow*.rpm releases/rpms/;
cp /usr/src/packages/SRPMS/get2gnow*.rpm releases/rpms/;
if( ! -d /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads ) then
	printf "Cannot copy our packages to uberChicGeekChick.Com.  sshfs has prolly disconnected."
else
	printf "Copying our packages to uberChicGeekChick.Com."
	cp releases/*/get2gnow*  /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/ &;
endif

