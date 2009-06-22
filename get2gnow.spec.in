Summary: get2gnow's the ultimate micro-blogging client. Letting you get and do the most on all micro-blogging platforms.
Name: get2gnow
Version: 0000F9aEC
Release: 20090620
License: RPL v1.3
BuildRequires: libsexy-devel libopenssl-devel libsoup-devel libnotify-devel gnome-vfs2-devel gtk2-devel glib2-devel libgio-2_0-0 libcanberra-gtk libxml2-devel libgobject-2_0-0 libgthread-2_0-0 gconf2-devel gnome-keyring-devel
Group: Applications/Internet/Chat
Source: get2gnow.tar.bz2
URL: http://local.uberchicgeekchick.com/?projects=get2gnow
Packager: Kaity G. B. <uberChick@uberChicGeekChick.Com>

%description
get2gnow's the ultimate micro-blogging client.
Letting you get and do the most on all micro-blogging platforms.
which just so happens to be exclusively available on
Linux, GTK+, & GNOME.  With our goal for get2gnow being
to create the very best, most robust, useful, beautiful,
and feature rich communication platform imaginable while
remaining yet straight forward & easy to use.
get2gnow's focus is to createg the most full featured
online communication platform available one any platform,
And for Linux/GTK/GNOME open source artists:
	get2gnow can easily be embeddable into other applications

Q.) What is 'micro-blogging'?

A.) Micro-blogging is a way of quickly communicating
any thing from complex ideas to the very simple:
'What are you doing?'

%prep
rm -rf get2gnow
tar -xjf get2gnow.tar.bz2

%build
cd get2gnow
./configure
make

%install
cd get2gnow
make install

%files
/usr/share/applications/get2gnow.desktop
/usr/share/locale/ja/LC_MESSAGES/get2gnow.mo
/usr/share/locale/sv/LC_MESSAGES/get2gnow.mo
/usr/share/locale/es/LC_MESSAGES/get2gnow.mo
/usr/share/locale/pt/LC_MESSAGES/get2gnow.mo
/usr/share/locale/fr/LC_MESSAGES/get2gnow.mo
/usr/share/locale/de/LC_MESSAGES/get2gnow.mo
/usr/share/gnome/help/get2gnow
/usr/share/gnome/help/get2gnow/C
/usr/share/gnome/help/get2gnow/C/get2gnow.xml
/usr/share/gnome/help/get2gnow/C/figures
/usr/share/gnome/help/get2gnow/C/figures/client.png
/usr/share/gnome/help/get2gnow/C/legal.xml
/usr/share/get2gnow
/usr/share/get2gnow/friends-manager.ui
/usr/share/get2gnow/user-profile-viewer.ui
/usr/share/get2gnow/tweet-list.ui
/usr/share/get2gnow/following-viewer.ui
/usr/share/get2gnow/tweet-view.ui
/usr/share/get2gnow/online-services-dialog.ui
/usr/share/get2gnow/preferences.ui
/usr/share/get2gnow/online-services-request-popup.ui
/usr/share/get2gnow/main-window.ui
/usr/bin/get2gnow
/usr/etc/gconf/schemas/get2gnow.schemas

%clean
cd get2gnow
make distclean
cd ..
rm -rf get2gnow

