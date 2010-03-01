#!/bin/tcsh -f
set starting_dir="${cwd}";
set project_path=/projects/gtk
cd "`dirname '${0}'`";
set project_name="`basename '${cwd}'`";
set pkg_path=${project_path}/releases;
cd ${project_path}

if(! -d ${pkg_path} ) mkdir -p ${pkg_path};
if(! -d ${pkg_path}/rpms ) mkdir -p ${pkg_path}/rpms;
if(! -d ${pkg_path}/srpms ) mkdir -p ${pkg_path}/srpms;
if(! -d ${pkg_path}/tarballs ) mkdir -p ${pkg_path}/tarballs;
if(! -d ${pkg_path}/specs ) mkdir -p ${pkg_path}/specs;

set package_name=`/bin/grep PACKAGE_NAME= "${project_name}"/configure | sed 's/.*="\([^"]\+\)"/\1/'`
set package_version=`/bin/grep PACKAGE_VERSION= "${project_name}"/configure | sed 's/.*="\([^"]\+\)"/\1/'`
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
printf "Creating tarball: [%s.tar.gz]\t\t\t" "${package_string}";
ln -s "./${project_name}" "./${package_string}";
tar --dereference -C ${project_path} -czf "${pkg_path}/tarballs/${package_string}.tar.gz" "${package_string}";
rm "${project_name}/${project_name}.spec" "./${package_string}";
printf "[done]\n";
${use_sudo}rpmbuild -ta "${pkg_path}/tarballs/${package_string}.tar.gz";
if( "${status}" != "0" ) then
	printf "***********ERROR: rpm build failed****************";
	exit -1;
endif
set local_tarball="${pkg_path}/tarballs/${package_string}.tar.gz";
#set pkg_path=${project_path}/releases/../nfs/releases/;
if( ${?clean_up} ) then
	rm --verbose "${pkg_path}/"*/"${project_name}"*.${MACHTYPE}.*;
	rm --verbose "${pkg_path}/"*/"${project_name}"*_${deb_arch}.*;
endif
mv --verbose "${local_tarball}" "${pkg_path}/tarballs/${package_string}.${MACHTYPE}.tar.gz";
cp --verbose "/usr/src/packages/RPMS/${MACHTYPE}/${package_string}.${MACHTYPE}.rpm" ./;
if( -x `which alien` ) then
	${use_sudo}alien --to-deb --keep-version "./${package_string}.${MACHTYPE}.rpm";
	rm "./${package_string}.${MACHTYPE}.rpm";
	if( ${MACHTYPE} == "x86_64" ) then
		set deb_arch="amd64";
	else
		set deb_arch="i386";
	endif
	mv --verbose "./${package_string}_${deb_arch}.deb" "${pkg_path}/debs/${package_string}_${deb_arch}.deb";
endif
${use_sudo}mv --verbose "/usr/src/packages/RPMS/${MACHTYPE}/${package_string}.${MACHTYPE}.rpm" "${pkg_path}/rpms/";
${use_sudo}mv --verbose "/usr/src/packages/SRPMS/${package_string}.src.rpm" "${pkg_path}/srpms/${package_string}.${MACHTYPE}.src.rpm";
${use_sudo}chown ${USER}:${GROUP} "${pkg_path}/"*/"${project_name}"*
if( -d /projects/www/uberChicGeekChick.Com/apps/oss-canvas/downloads ) then
	printf "Creating symlinks of our packages for our local uberChicGeekChick.Com mirror.\n"
	if( ${?clean_up} ) rm "/projects/www/uberChicGeekChick.Com/apps/oss-canvas/downloads/${project_name}"*;
	ln -sfv /projects/gtk/releases/*/${project_name}* /projects/www/uberChicGeekChick.Com/apps/oss-canvas/downloads/;
endif
if(! -d /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads ) then
	printf "Cannot copy our packages to uberChicGeekChick.Com.  sshfs has prolly disconnected.\n"
else
	printf "Copying our packages to uberChicGeekChick.Com.\n"
	if( ${?clean_up} ) then
		rm --verbose "/projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/${project_name}"*.${MACHTYPE}.*;
		if( ${?deb_arch} ) rm --verbose "/projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/${project_name}"*_${deb_arch}.*;
	endif
	cp --verbose "${pkg_path}/"*/"${project_name}"*.${MACHTYPE}.* /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/;
	if( ${?deb_arch} ) cp --verbose "${pkg_path}/"*/"${project_name}"*_${deb_arch}.* /projects/ssh/uberChicGeekChick.Com/apps/oss-canvas/downloads/;
endif

cd "${starting_dir}";
