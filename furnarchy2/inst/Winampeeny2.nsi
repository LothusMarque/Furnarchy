!define PRODUCT_NAME "Winampeeny2"
!define PRODUCT_VERSION "2.1-r1"
!define PRODUCT_WEB_SITE "http://proxy.entwinedstudios.com/"

SetCompressor lzma

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}.exe"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\English.nlf"
InstallDir "$PROGRAMFILES\Furnarchy2"
Icon "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
UninstallIcon "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
ComponentText "Check the components you want to install and uncheck the components you don't want to install:"
DirText "Setup will install $(^Name) in the following folder.$\r$\n$\r$\nTo install in a different folder, click Browse and select another folder."
LicenseText "If you accept all the terms of the agreement, choose I Agree to continue. You must accept the agreement to install $(^Name)."
LicenseData "..\COPYING"
ShowInstDetails show
ShowUnInstDetails show
XPStyle on

Page license
PageEx directory
  PageCallbacks initModuleInstall
PageExEnd
Page instfiles

Section "Main"
  SectionIn RO
  
    SetOutPath "$INSTDIR\modules\winampeeny2"
    SetOverwrite on
    File "..\bin\modules\winampeeny2\winampeeny2.dll"
    File "..\bin\modules\winampeeny2\module.ini"
    File "..\bin\modules\winampeeny2\interop.txt"
    File "..\bin\modules\winampeeny2\COPYING"
    File "..\bin\modules\winampeeny2\ChangeLog"
    SetOverwrite off
    File "..\bin\modules\winampeeny2\strings.txt"
    File "..\bin\modules\winampeeny2\thumbnail.tga"

SectionEnd

Function initModuleInstall
  ReadRegStr $0 HKLM "Software\Furnarchy2" "path"
  IfErrors 0 +3
    MessageBox MB_OK|MB_ICONINFORMATION "The installer couldn't detect your Furnarchy2 folder. Please enure the path is correct."
    Return
  StrCpy $INSTDIR $0
FunctionEnd