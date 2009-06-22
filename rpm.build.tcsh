#!/bin/tcsh -f
cd /projects/gtk;
printf "Creating tarball\t\t\t";
tar -cjf releases/tarballs/get2gnow.tar.bz2 get2gnow
printf "[done]\n";
cd get2gnow;
sudo rpmbuild -ba get2gnow.spec;
cp /usr/src/packages/RPMS/x86_64/get2gnow*.rpm ../releases/rpms/;
cp /usr/src/packages/SRPMS/get2gnow*.rpm ../releases/rpms/;
cp ../releases/rpms/get2gnow*  /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/ &;

