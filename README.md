# Natpad

Natpad is a Text-editor written in C using GTK

* **Fast**: loading of the editor
* **Macros**: Allows recording macros and replaying them.
* **Big files**: Allows loading and editing big files.
* **Parser**: Parsers for several language included.

## Versions

[Versions](https://github.com/natpad/VERSIONS.md)

## Building Natpad (LINUX Debian)

Building natpad uses the dung builder. The dung builder can be downloaded here [dung builder](https://github.com/douwevos/dung/releases/download/0.2/dung_0.2.deb). Install the debian package.

```
sudo dpkg -i dung_0.2.dev
```

Dung is a built system using workspaces. A workspace is described in a description file. Dung workspace description files end with the extension '.hil'. If you clone the natpad git repository you will find several .hil files. 

* natpad.hil this is the main workspace description file for natpad. It will build a full production version of Natpad for the Linux platform (64 bit).
* natpad_testing.hil this is used for testing. It will build a test version of Natpad for the Linux platform. The compiler will be compiling in debug mode and natpad will add extensive information about reference-counting and the number of objects created/destroyed. It will also show stacktraces in some cases.
* natpad_windows_amd64.hil this is for building a windows version using mingw. 

```
git clone https://github.com/douwevos/natpad.git
git clone https://github.com/douwevos/gross.git
git clone https://github.com/douwevos/natpad.cup.git 
cd natpad
dung install natpad.hil
dung select natpad
dung natpad
```

The command ```dung install natpad.hil``` will ask you to enter the trunk directory for Natpad. You can simply press enter. It might also ask for your password since it uses **apt** to install additional packages.

## Building packages

To build a Debian package

```
dung natpad#deb
```

To build a RPM package

```
dung natpad#rpm
```

To build a windows setup.exe. (Please make sure you are using the natpad_windows_amd64 dung workspace)

```
dung natpad#nsi
```

