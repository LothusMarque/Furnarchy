unit CursesData;

interface

resourcestring
 //
 DAT_Logo = 'LOGO';
 DAT_Logo2 = 'LOGO2';
 DAT_UC = 'UC';
 DAT_ImgMessage = 'MSG';
 DATG_Art = 'ART';
 //INI Sections
 SEC_Folders = 'Folders';
 SEC_Digos = '!Digos!';
 SEC_General = 'general';
 SEC_Ports = 'CustomPorts';
 SEC_CDigos = 'CustomDigo';
 SEC_Main = 'Main';
 //INI Keys
 KEY_WithOFurn = 'WithoutFurnarchy';
 KEY_ClientEXE = 'ClientExe';
 KEY_RestoreWinPos = 'RestoreWinPosition';
 KEY_WinTop = 'WindowTop';
 KEY_WinLeft = 'WindowLeft';
 KEY_AvaM = 'M';
 KEY_PortM = 'PM';
 KEY_AvaN = 'N';
 KEY_PortN = 'PN';
 KEY_AvaF = 'F';
 KEY_PortF = 'PF';
 KEY_LastDigo = 'LastDigo';
 //KEY_Digo
 KEY_RandomMode = 'RandomMode';
 KEY_DigoSelect = 'RememberDigoSelect';
 //Character INI Starts
 RES_NewINIHeader = 'V3.0 character';
 RES_Header1_6 = 'V1.6 character';
 RES_Header2 = 'V2.0 character';
 RES_Header3 = 'V3.0 character';
 //Character INI Keys
 KEY_Colors = 'Colors';
 KEY_Description = 'Desc';
 KEY_AFKDescription = 'AFKDescription';
 KEY_Name = 'Name';
 KEY_Password = 'Password';
 KEY_AFKTime = 'AFKTime';
 KEY_AFKDisconnectTime = 'AFKDisconnectTime';
 KEY_AutoResMessage = 'AutoResponseMessage';
 KEY_AFKMessage = 'AFKMessage';
 KEY_AutoResponse = 'AutoResponse';
 KEY_AFKPortrait = 'AFKPortrait';
 KEY_DefaultPortrait = 'DefaultPortrait';
 //Paths
 RES_Docfolder = 'Furcadia\';
 RES_F2Folder = 'Furnarchy2\';
 RES_FurcadiaCharacters = 'Furcadia Characters\';
 RES_FurcDataPath = '\Dragon''s Eye Productions\Furcadia\';
 RES_DynamicAvas = 'Dynamic Avatars\';
 RES_FurcDocPath = 'Furcadia\';
 KEY_DefaultINI = 'DefaultINI';
 //Files
 RES_FurnarchyINI = 'options.xml';
 RES_CursesINI = 'curses.ini';
 RES_LaunchINI = 'launch.ini';
 RES_FurcEXE = 'furcadia.exe';
 RES_LaunchEXE = 'launch.exe';
 RES_SettingsINI = 'settings.ini';
 RES_F2CFGEXE = 'config.exe';
 //Dynamic files
 RES_DynPlayer = 'dplayer%d.fox';//Format
 RES_DynPort = 'dport%d.fox';
 RES_DynTag = 'DSpeci%d.fox';
 RES_DynList = 'dynamicavatars.dat';

 //Other
 RES_NoData = '{95720326872368760239}';
 RES_Yes = '1';
 RES_No = '0';
 //Resources
 SelData = 'GENSELECT';
 RES_TagHelpText = 'TAGHELP';
 RES_DigoData = 'DIGODATA';
 RES_DigoDataFile = 'digodata.ini';
 //Registry paths
 RES_FurcKey = 'SOFTWARE\Dragon''s Eye Productions\Furcadia\Programs\';
 //Errors
 ERR_NoINISelected = 'No INI is selected to play!';
 ERR_INISave = 'You need to save your character first!';
 //Window Titles
 TTL_Error = 'Error';
 TTL_Query = 'Query';
 //Messages
 RES_ClearDigoSelections = 'This will clear ALL digo display selections. Are you sure?';
 RES_SaveChanges = 'The character has been modified. Do you want to save the changes?';
 //Dynamic Data
 RES_DynAva = 'Dynamic Avatar %d';
 RES_DynAvaTest = 'Dynamic Avatar ';
 RES_DynamicPoses = '1,5,13,9,0,4,12,8,16,17,18,19'; //Dynamic pose list

const
 DEF_RandomMode = 0;
 DEF_DigoSelect = false;

const
 CURSES_Version = '1.0.10';
 CURSES_Date = '2009-2013';

implementation

end.
