#!/bin/tcsh -f
set starting_dir=`pwd`;
set project_path=/projects/gtk
set pkg_path=${project_path}/releases;
cd ${project_path}

if(! -d ${pkg_path}/ ) mkdir -p ${pkg_path}/;
if(! -d ${pkg_path}/rpms/ ) mkdir -p ${pkg_path}/rpms/;
if(! -d ${pkg_path}/srpms/ ) mkdir -p ${pkg_path}/rpms/;
if(! -d ${pkg_path}/tarballs/ ) mkdir -p ${pkg_path}/tarballs/;

set release=`/bin/grep PACKAGE_RELEASE= get2gnow/configure | sed 's/.*="\([^"]\+\)"/\1/'`
set version=`/bin/grep PACKAGE_VERSION= get2gnow/configure | sed 's/.*="\([^"]\+\)"/\1/'`

cd get2gnow;
set libdir="";
if( "${MACHTYPE}" == "x86_64" ) set libdir=" --libdir=/usr/lib64";
printf "Running:\n\t./configure --enable-gdb --enable-gtk-icon-update%s\n\n" "${libdir}";
./configure --enable-gdb --enable-gtk-icon-update${libdir};
if( -e src/get2gnow ) make clean;
cd ..;
if( ${?1} && "${1}" != "" && "${1}" == "--clean-up" ) then
	rm ${pkg_path}/*/get2gnow*;
endif
printf "Creating tarball\t\t\t";
ln -s get2gnow get2gnow-${version};
tar --dereference -C ${project_path} -czf ${pkg_path}/tarballs/get2gnow-${version}.tar.gz get2gnow-${version};
rm get2gnow-${version};
printf "[done]";
sudo rpmbuild -ta "${pkg_path}/tarballs/get2gnow-$version.tar.gz";
if( "${status}" != "0" ) then
	printf "***********ERROR: rpm build failed****************";
	exit -1;
endif
mv --verbose "${pkg_path}/tarballs/get2gnow-${version}.tar.gz" "${pkg_path}/tarballs/get2gnow-${version}-${release}.${MACHTYPE}.tar.gz";
sudo mv --verbose /usr/src/packages/RPMS/*/get2gnow*.rpm "${pkg_path}/rpms/";
sudo mv --verbose /usr/src/packages/SRPMS/get2gnow*.rpm "${pkg_path}/srpms/";
sudo chown ${USER}:${GROUP} "${pkg_path}/"*/get2gnow*
if( ! -d /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads ) then
	printf "Cannot copy our packages to uberChicGeekChick.Com.  sshfs has prolly disconnected."
else
	printf "Copying our packages to uberChicGeekChick.Com."
	rm /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/get2gnow*;
	cp --verbose ${pkg_path}/*/get2gnow*  /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/;
endif

cd "${starting_dir}";
