#!/bin/tcsh -f

@ errno=0;

set starting_dir="${cwd}";
cd "`dirname '${0}'`";
set project_path="${cwd}";
cd starting_dir;
unset starting_dir;
set projects_path="${project_path}/..";

if( "${cwd}" != "${project_path}" ) then
	set starting_dir="${cwd}";
	cd "${project_path}";
endif

set project_name="`basename '${cwd}'`";

set package_name=`/bin/grep PACKAGE_NAME= "${project_path}"/configure | sed 's/.*="\([^"]\+\)"/\1/'`
set package_version=`/bin/grep PACKAGE_VERSION= "${project_path}"/configure | sed 's/.*="\([^"]\+\)"/\1/'`
set package_release=`/bin/grep PACKAGE_RELEASE= "${project_path}"/configure | sed 's/.*="\([^"]\+\)"/\1/'`
set package_string="${package_name}-${package_version}${package_release}";

set pkg_path=${projects_path}/packages;

if(! -d ${pkg_path} )	\
	mkdir -p ${pkg_path};
if(! -d ${pkg_path}/rpms )	\
	mkdir -p ${pkg_path}/rpms;
if(! -d ${pkg_path}/srpms )	\
	mkdir -p ${pkg_path}/srpms;
if(! -d ${pkg_path}/tarballs )	\
	mkdir -p ${pkg_path}/tarballs;
if(! -d ${pkg_path}/specs )	\
	mkdir -p ${pkg_path}/specs;

set use_sudo="sudo ";
if( "${uid}" == "0" )	\
	set use_sudo="";


create_tarballs:
	cd "${project_path}";
	if(! -e "./${project_name}.spec" ) then
		printf "Running:\n\t./configure --enable-gdb --enable-gtk-icon-update\n\n";
		./configure --enable-gdb --enable-gtk-icon-update;
	endif
	cp -vf "./${project_name}.spec" "${pkg_path}/specs/${project_name}.spec";
	make distclean;
	cd "${projects_path}";
	
	ln -vf "${pkg_path}/specs/${project_name}.spec" "${project_name}/${project_name}.spec";
	
	if( ${?1} && "${1}" != "" && "${1}" == "--clean-up" )	\
		set clean_up;
	if( ${?clean_up} ) then
		rm ${pkg_path}/*/"${project_name}"*;
	endif
	
	printf "Creating tarball: [%s.tar.gz]\t\t\t" "${package_string}";
	ln -s "./${project_name}" "./${package_string}";
	tar --dereference -C ${projects_path} -czf "${pkg_path}/tarballs/${package_string}.tar.gz" "${package_string}";
	rm "${project_name}/${project_name}.spec" "./${package_string}";
	printf "[done]\n";
#create_tarballs:


create_rpm:
	${use_sudo}rpmbuild -ta "${pkg_path}/tarballs/${package_string}.tar.gz";
	if( "${status}" != "0" ) then
		printf "***********ERROR: rpm build failed****************";
		goto exit_script;
	endif
#create_rpm:


archive_rpm:
	set local_tarball="${pkg_path}/tarballs/${package_string}.tar.gz";
	#set pkg_path="${pkg_path}/../nfs/packages";
	if( ${?clean_up} ) then
		rm --verbose "${pkg_path}/"*/"${project_name}"*.${MACHTYPE}.*;
		rm --verbose "${pkg_path}/"*/"${project_name}"*_${deb_arch}.*;
	endif
	mv --verbose "${local_tarball}" "${pkg_path}/tarballs/${package_string}.${MACHTYPE}.tar.gz";
	${use_sudo}mv --verbose "/usr/src/packages/RPMS/${MACHTYPE}/${package_string}-${package_release}.${MACHTYPE}.rpm" ./;
#archive_rpm:


create_deb:
	cd "${projects_path}";
	if(! -x `which alien` )	\
		goto store_packages;
	
	${use_sudo}alien --to-deb --keep-version "./${package_string}-${package_release}.${MACHTYPE}.rpm";
	rm "./${package_string}.${MACHTYPE}.rpm";
	if( ${MACHTYPE} == "x86_64" ) then
		set deb_arch="amd64";
	else
		set deb_arch="i386";
	endif
	mv --verbose "./${package_name}_${package_version}${package_release}-${package_release}_${deb_arch}.deb" "${pkg_path}/debs/${package_string}_${deb_arch}.deb";
#create_deb:


store_packages:
	${use_sudo}mv --verbose "./${package_string}-${package_release}.${MACHTYPE}.rpm" "${pkg_path}/rpms/${package_string}.${MACHTYPE}.rpm";
	${use_sudo}mv --verbose "/usr/src/packages/SRPMS/${package_string}-${package_release}.src.rpm" "${pkg_path}/srpms/${package_string}.${MACHTYPE}.src.rpm";
	${use_sudo}chown ${USER}:${GROUP} "${pkg_path}/"*/"${project_name}"*
#store_packages:

exit_script:
	if( ${?starting_dir} ) then
		cd "${starting_dir}";
		unset starting_dir;
	endif
	
	if( ${?errno} ) then
		set status=${errno};
		unset errno;
	endif
	exit ${status};
#exit_script:

