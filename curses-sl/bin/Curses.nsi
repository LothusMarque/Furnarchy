!define PRODUCT_NAME "Curses"
!define PRODUCT_VERSION "1.0.10"
!define PRODUCT_WEB_SITE "http://proxy.entwinedstudios.com/"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\Furnarchy2Curses"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define SF_SELECTED   1
!define SF_RO         16

SetCompressor lzma

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Curses-${PRODUCT_VERSION}.exe"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\English.nlf"
InstallDir "$PROGRAMFILES\Furnarchy2"
Icon "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
UninstallIcon "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
ComponentText "Check the components you want to install and uncheck the components you don't want to install:"
DirText "Setup will install $(^Name) in the following folder.$\r$\n$\r$\nTo install in a different folder, click Browse and select another folder."
LicenseText "If you accept all the terms of the agreement, choose I Agree to continue. You must accept the agreement to install $(^Name)."
LicenseData "..\eula.txt"
ShowInstDetails show
ShowUnInstDetails show
XPStyle on

Page license
PageEx license
LicenseText "Readme" "Next >"
LicenseData "..\Curses-readme.txt"
Caption ": Readme File"
PageExEnd
PageEx directory
  PageCallbacks initModuleInstall DoNothing dirtest
PageExEnd
Page components
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

Section "Curses" SEC1
  SectionIn RO

  SetOutPath "$INSTDIR"
  SetOverwrite on
  File "Curses.exe"
  File "..\Curses-readme.txt"

  SetOutPath "$INSTDIR"
  CreateDirectory "$SMPROGRAMS\Furnarchy 2"
  CreateShortCut "$SMPROGRAMS\Furnarchy 2\Curses Character Selection.lnk" "$INSTDIR\Curses.exe" ""
SectionEnd

Section "Uninstaller" SEC2
  WriteUninstaller "$INSTDIR\uninst-curses.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "${PRODUCT_NAME} ${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\launch.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$SMPROGRAMS\Furnarchy 2\Website.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\Furnarchy 2\Uninstall Curses.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Function initModuleInstall
#
FunctionEnd

Function DoNothing
#
FunctionEnd

Function dirtest
  StrCmp $INSTDIR $0 0 +4
  IntOp $1 0 | ${SF_RO}
  SectionSetFlags ${SEC2} $1
  return
 SectionSetFlags ${SEC2} ${SF_SELECTED}
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Function .onInit
  ReadRegStr $0 HKLM "Software\Furnarchy2" "path"
IntOp $1 ${SF_SELECTED} | ${SF_RO}
  IfErrors 0 +5
    MessageBox MB_OK|MB_ICONINFORMATION "The installer couldn't detect your Furnarchy2 folder.$\nIf Furnarchy is not installed, remember to check 'Without Furnarchy' inside Curses.$\nOtherwise, Curses MUST be installed to Furnarchy's location to work correctly."
    SectionSetFlags ${SEC1} $1
    SectionSetFlags ${SEC2} ${SF_SELECTED}
    Return
  StrCpy $INSTDIR $0
  SectionSetFlags ${SEC1} $1
FunctionEnd

Section Uninstall
  Delete "$SMPROGRAMS\Furnarchy 2\Website.url"
  Delete "$SMPROGRAMS\Furnarchy 2\Uninstall Curses.lnk"
  Delete "$SMPROGRAMS\Furnarchy 2\Curses Character Selection.lnk"
  RMDir "$SMPROGRAMS\Furnarchy 2"
  Delete "$INSTDIR\Curses.exe"
  Delete "$INSTDIR\Curses-readme.txt"
  RMDir $INSTDIR
  DeleteRegKey HKLM "Software\Furnarchy2"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  SetAutoClose true
SectionEnd