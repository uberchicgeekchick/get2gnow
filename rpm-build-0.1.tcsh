#!/bin/tcsh -f
set starting_dir=`pwd`;
set project_path=/projects/gtk
set project_name="get2gnow";
set pkg_path=${project_path}/nfs/packages
cd ${project_path}

if(! -d ${pkg_path} ) mkdir -p ${pkg_path};
if(! -d ${pkg_path}/rpms ) mkdir -p ${pkg_path}/rpms;
if(! -d ${pkg_path}/srpms ) mkdir -p ${pkg_path}/srpms;
if(! -d ${pkg_path}/tarballs ) mkdir -p ${pkg_path}/tarballs;
if(! -d ${pkg_path}/specs ) mkdir -p ${pkg_path}/specs;

set package=`/bin/grep PACKAGE_NAME= "${project_name}"/configure | sed 's/.*="\([^"]\+\)"/\1/'`
set version=`/bin/grep PACKAGE_VERSION= "${project_name}"/configure | sed 's/.*="\([^"]\+\)"/\1/'`
set package_string="${package_name}-${package_version}";

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

if( ${?1} && "${1}" != "" && "${1}" == "--clean-up" ) set clean_up;
if( ${?clean_up} ) then
	rm ${pkg_path}/*/"${project_name}"*;
endif
printf "Creating tarball: [%s-%s.tar.gz]\t\t\t" "${project_name}" "${version}";
ln -s "${project_name}" "${project_name}"-${version};
tar --dereference -C ${project_path} -czf "${pkg_path}/tarballs/${project_string}.tar.gz" "${project_string}";
rm "${project_name}/${project_name}.spec" "${project_string}";
printf "[done]\n";
${use_sudo}rpmbuild -ta "${pkg_path}/tarballs/${project_string}.tar.gz";
if( "${status}" != "0" ) then
	printf "***********ERROR: rpm build failed****************";
	exit -1;
endif
set local_tarball="${pkg_path}/tarballs/${project_string}.tar.gz";
#set pkg_path=${pkg_path}/../nfs/packages;
if( ${?clean_up} ) then
	rm --verbose "${pkg_path}/"*/"${project_name}"*.${MACHTYPE}.*;
	rm --verbose "${pkg_path}/"*/"${project_name}"*_${deb_arch}.*;
endif
mv --verbose "${local_tarball}" "${pkg_path}/tarballs/${project_string}.${MACHTYPE}.tar.gz";
cp --verbose "/usr/src/packages/RPMS/${MACHTYPE}/${project_string}.${MACHTYPE}.rpm" ./;
${use_sudo}alien --to-deb --keep-version "./${project_string}.${MACHTYPE}.rpm";
if( ${?clean_up} ) rm "./${project_string}.${MACHTYPE}.rpm";
if( ${MACHTYPE} == "x86_64" ) then
	set deb_arch="amd64";
else
	set deb_arch="i386";
endif
mv --verbose "./${project_name}_${version}_${deb_arch}.deb" "${pkg_path}/debs/${project_string}_${deb_arch}.deb";
${use_sudo}mv --verbose "/usr/src/packages/RPMS/${MACHTYPE}/${project_string}.${MACHTYPE}.rpm" "${pkg_path}/rpms/";
${use_sudo}mv --verbose "/usr/src/packages/SRPMS/${project_string}.src.rpm" "${pkg_path}/srpms/${project_string}.${MACHTYPE}.src.rpm";
${use_sudo}chown ${USER}:${GROUP} "${pkg_path}/"*/"${project_name}"*
if( -d /projects/www/uberChicGeekChick.Com/apps/oss-canvas/downloads ) then
	printf "Creating symlinks of our packages for our local uberChicGeekChick.Com mirror.\n"
	if( ${?clean_up} ) rm "/projects/www/uberChicGeekChick.Com/apps/oss-canvas/downloads/${project_name}"*;
	ln -s ${pkg_path}/*/${project_name}* /projects/www/uberChicGeekChick.Com/apps/oss-canvas/downloads/;
endif
if(! -d /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads ) then
	printf "Cannot copy our packages to uberChicGeekChick.Com.  sshfs has prolly disconnected.\n"
else
	printf "Copying our packages to uberChicGeekChick.Com.\n"
	if( ${?clean_up} ) then
		rm --verbose "/projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/${project_name}"*.${MACHTYPE}.*;
		rm --verbose "/projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/${project_name}"*_${deb_arch}.*;
	endif
	cp --verbose "${pkg_path}/"*/"${project_name}"*.${MACHTYPE}.* /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/;
	cp --verbose "${pkg_path}/"*/"${project_name}"*_${deb_arch}.* /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/;
endif

cd "${starting_dir}";
