Summary: Just another text editor.
Name: natpad
Version: [%natpad.version%]
Release: 1
Group: Development/Editor
Packager: D.M.Vos (dmvos2000@yahoo.com)
License: GPL
URL: http://www.natpad.net/
BuildArch: [%natpad.os.arch%]

%description
Just another text editor.

%install

%post
exit 0

%postrun

%clean

%files

/usr/bin/natpad
/usr/share/pixmaps/*
/usr/share/applications/kde4/*
/usr/share/natpad/*

