# declare name of installer file
!include "MUI.nsh"

outfile "natpad-setup.exe"
RequestExecutionLevel admin
InstallDir "$PROGRAMFILES\Natpad"

!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Natpad"

Var StartMenuFolder

!insertmacro MUI_PAGE_DIRECTORY

!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

!insertmacro MUI_PAGE_INSTFILES 


# open section
section

    StrCpy $StartMenuFolder "Natpad"

	setOutPath "$WINDIR\Natpad\"
	File /r "usr"

	setOutPath $INSTDIR

	File "natpad.exe"
	File "*.dll"
	File /r "share"

	writeUninstaller "$INSTDIR\uninstall.exe"



 !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END


	CreateShortCut "$SMPROGRAMS\Natpad\Uninstall-natpad.lnk" "$INSTDIR\uninstall.exe"
	
	CreateShortCut "$SMPROGRAMS\Natpad\natpad.lnk" "$INSTDIR\natpad.exe"

 
sectionEnd

section "uninstall"

	delete "$INSTDIR\uninstall.exe"
	delete "$SMPROGRAMS\Natpad\Uninstall-natpad.lnk"
	delete "$SMPROGRAMS\Natpad\natpad.lnk"


sectionEnd
