unit CursesUtils;

interface
uses
 Sysutils, Windows, ShellAPI, shFolder, shlobj, Messages,
 ActiveX, Forms, Classes, Registry, CursesData, SupraINIv2, strutils;

type
   TFurcColors = packed record
    Fur : integer;
    Markings : integer;
    Hair : integer;
    Eye : integer;
    Badge : integer;
    Vest : integer;
    Bracer : integer;
    Cape : integer;
    Boot : integer;
    Trousers : integer;
   end;
  TFurcAvatar = packed record
   colors: TFurcColors;
   Species: integer;
   Gender: integer;
   specialavatar : integer;
   end;

//Misc
function LastPos(const SubStr: String; const S: String): Integer;
function StrToInt2(data : string):integer; //Non-exception causing strtoint
function StripReturns(data: string):string; //Eat all linebreaks from input

//Base220 Utility Functions
function getSingleB220(i : char): integer;
function makeSingleB220(i : byte): char;

//Path Gatherers
function CheckLocalMode : boolean;
function getFurcadiaFolder:string;
function GlobalAppDataPath: string;
function MyDocsDataPath : string;
function GetWorkingPath: string;
function GetFurcDocFolder: string;
function BrowserCallback(Wnd: HWND; uMsg: UINT; lParam, lpData: LPARAM):Integer stdcall; //Callback for Folder Window

//Fileshortening for display - two different methods
function GetShortPath(Path: string; Count: Integer): string;
function MinimizePathName(Wnd: HWND; const Filename: string): string;

//Colorcode
function BuildCC(data : TFurcAvatar) : string;

//Graphics
function GetPortraitFile(io: TSupraINIv2; species, gendr: integer; digo : string): string;
function GetAvatarFile(io: TSupraINIv2; species, gendr: integer; digo : string): string;

var
 g_localmode : boolean;
 g_ClientDataRoot : string;
 g_ClientDocRoot : string;
 g_GlobalDataRoot : string;
 g_FurcFolder : string;
 g_FurcPatchFolder : string;

implementation
uses CursesU1; //Bad move, but it'll do for now.

//Utility Functions
function LastPos(const SubStr: String; const S: String): Integer;
begin
   result := Pos(ReverseString(SubStr), ReverseString(S)) ;

   if (result <> 0) then
     result := ((Length(S) - Length(SubStr)) + 1) - result + 1;
end;


function StrToInt2(data : string):integer;
var
 t1, c1 : integer;
begin
val(data,t1,c1);
result := t1;
end;

function StripReturns(data: string):string;
begin
result := StringReplace(StringReplace(data,#13,'',[rfReplaceAll]),#10,'',[rfReplaceAll]);
end;

//Base220
function getSingleB220(i : char): integer;
begin
result := ord(i) - 35;
end;

function makeSingleB220(i : byte): char;
begin
result := chr(i + 35);
end;

function GetShortPath(Path: string; Count: Integer): string;
  procedure Slashes(var Str: string; var Num: Integer);
  var
    Position, Index: Integer;
  begin
    Index:=0;
    repeat
      Position:=Pos('\', Str);
      Delete(Str,1,Position);
      if Position<>0 then Inc(Index);
      if (Index=Num)and(Num<>0) then break;
    until Position=0;
    Num:=Index;
  end;
var
  Num, NewNum, P: Integer;
  Str: string;
begin
  Str:=Path;
  Num:=0;
  Slashes(Path, Num);
  while (Length(Str)>Count)and(Num>2) do
  begin
    NewNum:=Num div 2;
    Path:=Str;
    Slashes(Path, NewNum);
    P:=Pos(Path, Str);
    Delete(Str,P, Length(Path));
    NewNum:=2;
    Slashes(Path, NewNum);
    Str:=Str+'...\'+Path;
    Dec(Num);
  end;
  Result:=Str;
end;

function MinimizePathName(Wnd: HWND; const Filename: string): string;
{ func to shorten the long path name with an ellipses '...' to fit }
{ in whatever control is passed to the Wnd parameter.              }
{ Usage: Panel1.Caption := MinimizePathName(Panel1.Handle, DirectoryOutline1.Directory) }
{        This will shorten the path if necessary to fit in Panel1.                      }
  procedure CutFirstDirectory(var S: string);
  var
    Root: Boolean;
    P: Integer; 
  begin
    if S = '\' then
      S := ''
    else 
      begin
        if S[1] = '\' then
          begin
            Root := True;
            Delete(S, 1, 1);
          end
        else
          Root := False;
        if S[1] = '.' then
          Delete(S, 1, 4);
        P := Pos('\',S);
        if P <> 0 then
          begin
            Delete(S, 1, P);
            S := '...\' + S;
          end
        else
          S := '';
        if Root then
          S := '\' + S;
      end;
  end;

  function GetTextWidth(DC: HDC; const Text: string): Integer;
  var
    Extent: TSize;
  begin
    if GetTextExtentPoint(DC, PChar(Text), Length(Text), Extent) then 
      Result := Extent.cX
    else 
      Result := 0;
  end;

var
  Drive,
  Dir,
  Name: string; 
  R: TRect;
  DC: HDC;
  MaxLen: integer;
  OldFont, Font: HFONT; 
begin
  Result := FileName;

  if Wnd = 0 then
    Exit;

  DC := GetDC(Wnd); 
  if DC = 0 then
    Exit;

  Font := HFONT(SendMessage(Wnd, WM_GETFONT, 0, 0));
  OldFont := SelectObject(DC, Font);
  try
    GetWindowRect(Wnd, R);
    MaxLen := R.Right - R.Left;

    Dir := ExtractFilePath(Result);
    Name := ExtractFileName(Result);

    if (Length(Dir) >= 2) and (Dir[2] = ':') then
      begin
        Drive := Copy(Dir, 1, 2);
        Delete(Dir, 1, 2);
      end
    else 
      Drive := '';

    while ((Dir <> '') or (Drive <> '')) and (GetTextWidth(DC, Result) > MaxLen) do
      begin
        if Dir = '\...\' then
          begin
            Drive := '';
            Dir := '...\';
          end
        else
          if Dir = '' then
            Drive := ''
          else
            CutFirstDirectory(Dir);
        Result := Drive + Dir + Name;
      end;
  finally
    SelectObject(DC, OldFont);
    ReleaseDC(Wnd, DC);
  end;
end;

function BuildCC(data : TFurcAvatar): string;
var
 code : string;
begin
setlength(code, 21);
code := 't';
code := code + makeSingleB220(data.colors.Fur);
code := code + makeSingleB220(data.colors.Markings);
code := code + makeSingleB220(data.colors.Hair);
code := code + makeSingleB220(data.colors.Eye);
code := code + makeSingleB220(data.colors.Badge);
code := code + makeSingleB220(data.colors.Vest);
code := code + makeSingleB220(data.colors.Bracer);
code := code + makeSingleB220(data.colors.Cape);
code := code + makeSingleB220(data.colors.Boot);
code := code + makeSingleB220(data.colors.Trousers);
code := code + makeSingleB220(data.Gender);
code := code + makeSingleB220(data.Species);
code := code + makeSingleB220(data.specialavatar);
result := code;
end;

Function CenterVertical( const rect: TRect; h: Integer ): Integer;
Begin
Result := (rect.bottom + rect.top - h) div 2;
End;

Function CenterHorizontal( const rect: TRect; w: Integer ): Integer;
Begin
Result := (rect.right + rect.left - w) div 2;
End;

function BrowserCallback(Wnd: HWND; uMsg: UINT; lParam, lpData: LPARAM):Integer stdcall;
var
  r1, r2: TRect;
  PathName: array[0..MAX_PATH] of char;
begin
  result := 0;
  If uMsg = BFFM_INITIALIZED Then Begin
    GetWindowRect( wnd, r1 );
    r2 := Rect( 0, 0, Screen.Width, Screen.Height );
    MoveWindow( wnd, CenterHorizontal( r2, r1.Right-r1.left ),
                  CenterVertical(r2, r1.Bottom-r1.Top),
                  r1.Right-r1.Left,
                  r1.Bottom-r1.Top,
                  false );
  End;
  if uMsg = BFFM_SELCHANGED then
   begin
   SHGetPathFromIDList(PItemIDList(lParam), @PathName);
   if DirectoryExists(PathName) then
    SendMessage(Wnd, BFFM_ENABLEOK, 1, 1)
   else
    SendMessage(Wnd, BFFM_ENABLEOK, 0, 0);
   end;
end;

function getFurcadiaFolder:string;
var
 registry : TRegistry;
 tmp : TStringList;
 temp : string;
begin
result := '';
//Launch.ini overrides!
if fileexists(GetWorkingPath+RES_LaunchINI) then
 begin
 tmp := TStringList.Create;
  try
  tmp.LoadFromFile(GetWorkingPath+RES_LaunchINI);
  temp := tmp.Values[KEY_ClientEXE];
  if temp <> '' then
   begin
   result := includetrailingpathdelimiter(extractfilepath(temp));
   exit;
   end;
  finally
  tmp.Free;
  end;
 end;
//if fileexists(MyDocsDataPath+RES_F2Folder) then

//
  Registry := TRegistry.Create(KEY_READ);
  try
  //First off, we check the current user location.
  Registry.RootKey := HKEY_CURRENT_USER;  //HKEY_CURRENT_USER
  // False because we do not want to create it if it doesn't exist
  Registry.OpenKey(RES_FurcKey, False);
  Temp := Registry.ReadString('Path');
  if length(temp) > 0 then
   if temp[length(temp)] <> '\' then
    temp := temp + '\';
   result := temp;
  OutputDebugString('Read Current User');
  if result = '' then //Check Local Machine
   begin
   Registry.RootKey := HKEY_LOCAL_MACHINE;  //HKEY_LOCAL_MACHINE
   // False because we do not want to create it if it doesn't exist
   Registry.OpenKey(RES_FurcKey, False);
   Temp := Registry.ReadString('Path');
  { if length(temp) > 0 then
    if temp[length(temp)] <> '\' then
     temp := temp + '\';  }
    result := includetrailingpathdelimiter(Temp);//temp;
   OutputDebugString('Read Local Machine');
   end;
  if result = '' then //Default in hopes that we look at the right folder
   begin
   result := 'C:\Program Files\Furcadia\';
   OutputDebugString('Warning: Defaulted to Program Files');
   end;
  finally
    Registry.Free;
  end;
end;


function GetWorkingPath: string;
begin
result := '';
//showmessage(extractfilepath(paramstr(0))+'localdir.ini');
//if fileexists(extractfilepath(paramstr(0))+'localdir.ini') then
if g_localmode then
 begin
 //g_localmode := true;
 result := extractfilepath(paramstr(0));
 exit;
 end;
g_localmode := false;
result := MyDocsDataPath+RES_F2Folder;
//OutputDebugString(pchar(result));
end;

function GetFurcDocFolder: string;
begin
result := '';
//showmessage(extractfilepath(paramstr(0))+'localdir.ini');
//Not sure yet how F2 marks local use
//if fileexists(includetrailingpathdelimiter(extractfilepath(paramstr(0)))+'localdir.ini') then
if g_localmode then
 begin
 //g_localmode := true;
 //result := includetrailingpathdelimiter(extractfilepath(paramstr(0)));
 result := g_FurcFolder;
 exit;
 end;
g_localmode := false;
result := G_ClientDocRoot;
//showmessage(result);
//OutputDebugString(pchar(result));
end;

function GlobalAppDataPath: string;
const
   SHGFP_TYPE_CURRENT = 0;
var
   path: array [0..MAX_PATH] of char;
begin
SHGetSpecialFolderPath(0,path,CSIDL_COMMON_APPDATA,false);
result := path;
end;

function LocalAppDataPath : string;
const
   SHGFP_TYPE_CURRENT = 0;
var
   path: array [0..MAX_PATH] of char;
begin
   SHGetFolderPath(0,CSIDL_LOCAL_APPDATA,0,SHGFP_TYPE_CURRENT,@path[0]) ;
   Result := IncludeTrailingPathDelimiter(path);
end;

function MyDocsDataPath : string;
const
   SHGFP_TYPE_CURRENT = 0;
var
   path: array [0..MAX_PATH] of char;
begin
   SHGetFolderPath(0,CSIDL_PERSONAL,0,SHGFP_TYPE_CURRENT,@path[0]) ;
   Result := IncludeTrailingPathDelimiter(path);
end;

function GetPortraitFile(io: TSupraINIv2; species, gendr: integer; digo : string): string;
var
 tmp : string;
 gender : string;
begin
//Dynamic Avatars
if pos(RES_DynAvaTest,digo) = 1 then
 begin //We're running a dynamic avatar here. Things change.
 tmp := copy(digo,length(RES_DynAvaTest)+1,length(digo));
 //outputdebugstring(pchar('DynPort#: '+tmp));
 result := g_GlobalDataRoot+RES_DynamicAvas+format(RES_DynPort,[strtoint2(tmp)]);
  case gendr of
   0:result := result + ',1'; //Female
   1:result := result + ',2'; //Male
   2:result := result + ',0'; //Unspec
  end;
 //outputdebugstring(pchar(result));
 exit;
 end;
//Normal!
case gendr of
 0: gender := KEY_PortF;
 1: gender := KEY_PortM;
 2: gender := KEY_PortN;
 end;
//Species-specific
tmp := io.ReadString(digo,gender+inttostr(species),RES_NoData);
//Species-Specific Male default
if tmp = RES_NoData then
 tmp := io.ReadString(digo, KEY_PortM+inttostr(species), RES_NoData);
//Full-replace Gender
if tmp = RES_NoData then
 tmp := io.ReadString(digo, gender, RES_NoData);
//Default to full-replace Male
if tmp = RES_NoData then
 tmp := io.ReadString(digo, KEY_PortM, RES_NoData);
if tmp = RES_NoData then  //Still nothing? Return blank and exit.
 begin
 result := '';
 exit;
 end;
result := tmp;
end;

function GetAvatarFile(io: TSupraINIv2; species, gendr: integer; digo : string): string;
var
 tmp : string;
 gender : string;
begin
//Dynamic Avatars
if pos(RES_DynAvaTest,digo) = 1 then
 begin //We're running a dynamic avatar here. Things change.
 tmp := copy(digo,length(RES_DynAvaTest)+1,length(digo));
 //outputdebugstring(pchar('DynAvatar#: '+tmp));
 result := '"'+g_GlobalDataRoot+RES_DynamicAvas+format(RES_DynPlayer,[strtoint2(tmp)])+'",'+RES_DynamicPoses;
 //outputdebugstring(pchar(result));
 exit;
 end;
//Normal
case gendr of
 0: gender := KEY_AvaF;
 1: gender := KEY_AvaM;
 2: gender := KEY_AvaN;
 end;
//Species-specific
tmp := io.ReadString(digo,gender+inttostr(species),RES_NoData);
//Species-Specific Male default
if tmp = RES_NoData then
 tmp := io.ReadString(digo, KEY_AvaM+inttostr(species), RES_NoData);
//Full-replace Gender
if tmp = RES_NoData then
 tmp := io.ReadString(digo, gender, RES_NoData);
//Default to full-replace Male
if tmp = RES_NoData then
 tmp := io.ReadString(digo, KEY_AvaM, RES_NoData);
if tmp = RES_NoData then  //Still nothing? Return blank and exit.
 begin
 result := '';
 exit;
 end;
result := tmp;
end;

function CheckLocalMode : boolean;
var
 F: Text;
 t1,t2 : string;
begin
result := false;
t1 := extractfilepath(paramstr(0))+RES_FurnarchyINI;
if fileexists(t1) then
 begin
 AssignFile(F,t1);
 Reset(F);
  try
  while not EOF(F) do
   begin
   ReadLn(F, t1);
   OutputDebugString(pchar(t1));
   if pos('<singleUserMode>true</singleUserMode>',t1) <> 0 then
    begin
    result := true;
    exit;
    end;
   end;
  finally
  closeFile(F);
  end;
 end;
end;

begin
//Unit initialization - let's prepare path variables
g_LocalMode := CheckLocalMode;
g_ClientDataRoot := IncludeTrailingPathDelimiter(LocalAppDataPath + RES_FurcDataPath);
g_ClientDocRoot := IncludeTrailingPathDelimiter(MyDocsDataPath + RES_FurcDocPath);
g_GlobalDataRoot := IncludeTrailingPathDelimiter(GlobalAppDataPath + RES_FurcDataPath);
g_FurcFolder := GetFurcadiaFolder;
g_FurcPatchFolder := includetrailingpathdelimiter(g_FurcFolder)+'patches\Default\';
//showmessage('Furcadia Folder: '+g_FurcFolder);
//if g_localmode then showmessage('Localmode!');
end.
