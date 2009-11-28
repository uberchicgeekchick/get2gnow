#!/bin/tcsh -f
set starting_dir=`pwd`;
set project_path=/projects/gtk
set project_name="get2gnow";
set pkg_path=${project_path}/releases;
cd ${project_path}

if(! -d ${pkg_path} ) mkdir -p ${pkg_path};
if(! -d ${pkg_path}/rpms ) mkdir -p ${pkg_path}/rpms;
if(! -d ${pkg_path}/srpms ) mkdir -p ${pkg_path}/srpms;
if(! -d ${pkg_path}/tarballs ) mkdir -p ${pkg_path}/tarballs;
if(! -d ${pkg_path}/specs ) mkdir -p ${pkg_path}/specs;

set package=`/bin/grep PACKAGE_NAME= "${project_name}"/configure | sed 's/.*="\([^"]\+\)"/\1/'`
set release=`/bin/grep PACKAGE_RELEASE= "${project_name}"/configure | sed 's/.*="\([^"]\+\)"/\1/'`
set version=`/bin/grep PACKAGE_VERSION= "${project_name}"/configure | sed 's/.*="\([^"]\+\)"/\1/'`

if(! -e "${pkg_path}/specs/${project_name}.spec" ) then
	cd "${project_name}";
	printf "Running:\n\t./configure --enable-gdb --enable-gtk-icon-update\n\n";
	./configure --enable-gdb --enable-gtk-icon-update;
	cp -vf "./${project_name}.spec" "${pkg_path}/specs/${project_name}.spec";
	make distclean;
	cd ../
endif

set use_sudo="sudo ";
if( "${uid}" == "0" ) set use_sudo="";

ln -vf "${pkg_path}/specs/${project_name}.spec" "${project_name}/${project_name}.spec";

if( ${?1} && "${1}" != "" && "${1}" == "--clean-up" ) then
	rm ${pkg_path}/*/"${project_name}"*;
endif
printf "Creating tarball\t\t\t";
ln -s "${project_name}" "${project_name}"-${version};
tar --dereference -C ${project_path} -czf "${pkg_path}/tarballs/${project_name}-${version}.tar.gz" "${project_name}-${version}";
rm "${project_name}/${project_name}.spec" "${project_name}-${version}";
printf "[done]\n";
${use_sudo}rpmbuild -ta "${pkg_path}/tarballs/${project_name}-$version.tar.gz";
if( "${status}" != "0" ) then
	printf "***********ERROR: rpm build failed****************";
	exit -1;
endif
set local_tarball="${pkg_path}/tarballs/${project_name}-${version}.tar.gz";
#set pkg_path=${project_path}/releases/../nfs/releases/;
mv --verbose "${local_tarball}" "${pkg_path}/tarballs/${project_name}-${version}-${release}.${MACHTYPE}.tar.gz";
cp --verbose "/usr/src/packages/RPMS/${MACHTYPE}/${project_name}-${version}-${release}.${MACHTYPE}.rpm" ./;
${use_sudo}alien --to-deb --keep-version "./${project_name}-${version}-${release}.${MACHTYPE}.rpm";
rm "./${project_name}-${version}-${release}.${MACHTYPE}.rpm";
if( ${MACHTYPE} == "x86_64" ) then
	set deb_arch="amd64";
else
	set deb_arch="i386";
endif
rm --verbose "${pkg_path}/"*/"${project_name}"*.${MACHTYPE}.*;
rm --verbose "${pkg_path}/"*/"${project_name}"*_${deb_arch}.*;
mv --verbose "./${project_name}_${version}-${release}_${deb_arch}.deb" "${pkg_path}/debs/${project_name}_${version}-${release}_${deb_arch}.deb";
${use_sudo}mv --verbose "/usr/src/packages/RPMS/${MACHTYPE}/${project_name}-${version}-${release}.${MACHTYPE}.rpm" "${pkg_path}/rpms/";
${use_sudo}mv --verbose "/usr/src/packages/SRPMS/${project_name}-${version}-${release}.src.rpm" "${pkg_path}/srpms/"${project_name}"-${version}-${release}.${MACHTYPE}.src.rpm";
${use_sudo}chown ${USER}:${GROUP} "${pkg_path}/"*/"${project_name}"*
if( -d /projects/www/uberChicGeekChick.Com/apps/oss-canvas/downloads ) then
	printf "Creating symlinks of our packages for our local uberChicGeekChick.Com mirror.\n"
	rm "/projects/www/uberChicGeekChick.Com/apps/oss-canvas/downloads/${project_name}"*;
	ln -s /projects/gtk/releases/*/${project_name}* /projects/www/uberChicGeekChick.Com/apps/oss-canvas/downloads/;
endif
if(! -d /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads ) then
	printf "Cannot copy our packages to uberChicGeekChick.Com.  sshfs has prolly disconnected.\n"
else
	printf "Copying our packages to uberChicGeekChick.Com.\n"
	rm --verbose "/projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/${project_name}"*.${MACHTYPE}.*;
	rm --verbose "/projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/${project_name}"*_${deb_arch}.*;
	cp --verbose "${pkg_path}/"*/"${project_name}"*.${MACHTYPE}.* /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/;
	cp --verbose "${pkg_path}/"*/"${project_name}"*_${deb_arch}.* /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/;
endif

cd "${starting_dir}";
