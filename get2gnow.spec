Summary:	get2gnow's the ultimate micro-blogging client. Letting you get and do the most on all micro-blogging platforms.
Name:		get2gnow
Version:	"#010001a54"
Release:	20090620
License:	RPL v1.3
URL:		http://local.uberchicgeekchick.com/?projects=get2gnow
Source0:	"%{name}-%{version}.tar.gz"
Group:		Applications/Internet
Packager:	Kaity G. B. <uberChick@uberChicGeekChick.Com>
Requires:	libsexy
Requires:	libnotify
Requires:	gnome-vfs2
Requires:	libxml2 >= 2.6.16
Requires:	gtk2 >= 2.16
Requires:	gconf2 >= 1.2.0
Requires:	gnome-keyring
Requires:	libsexy
BuildRequires:	libgnomeui
BuildRequires:	libopenssl-devel
BuildRequires:	libsoup-devel >= 2.24.0
BuildRequires:	libxml2-devel >= 2.6.16
BuildRequires:	glib2-devel >= @GLIB_VERSION_REQUIRED@
BuildRequires:	libgio-2_0-0 >= 2.15.5
BuildRequires:	gtk2-devel >= 2.16
BuildRequires:	gconf2-devel >= 1.2.0
BuildRequires:	libcanberra-gtk >= 0.4
BuildRequires:	libnotify-devel
BuildRequires:	gnome-vfs2-devel
BuildRequires:	libgobject-2_0-0
BuildRequires:	libgthread-2_0-0 >= @GTHREAD_VERSION_REQUIRED@
BuildRequires:	libgnomeui-devel
BuildRequires:	gnome-keyring-devel

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
%setup -q

%build
%configure
make

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT mandir=%{_mandir}

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_datadir}/applications/get2gnow.desktop
%{_bindir}/get2gnow
%{_datadir}/gnome/help/get2gnow
%{_datadir}/get2gnow/
%{_sysconfdir}/gconf/schemas/get2gnow.schemas
%{_datadir}/locale/ja/LC_MESSAGES/get2gnow.mo
%{_datadir}/locale/sv/LC_MESSAGES/get2gnow.mo
%{_datadir}/locale/es/LC_MESSAGES/get2gnow.mo
%{_datadir}/locale/pt/LC_MESSAGES/get2gnow.mo
%{_datadir}/locale/fr/LC_MESSAGES/get2gnow.mo
%{_datadir}/locale/de/LC_MESSAGES/get2gnow.mo

