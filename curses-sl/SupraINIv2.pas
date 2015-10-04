unit SupraINIv2;
{
TSupraINIv2
Version 2.00 For all Delphi versions
Written By Eric A. Pettersen, A.K.A. Supralight Kaiilridge
Email: androndax@hotmail.com

Description:
INI file reader with core structure based on the description of TMemINIFile
from Delphi 4+. Created for Delphi 3 with streaming support. 

Features:
All standard TINIFile functions.
Streaming support.

Warnings:
In Win9x/ME don't load or save a single INI file larger than 32k (maybe 64k),
although you can merge multiple files of that size into the database as long
as neither, any single section gets above 32/64k, or the section list gets
bigger than 32/64k. Under Windows NT/2000/XP this should not be a problem.
I'm not sure if this applies actually, but if you get errors, I suggest
checking it.

Main Differences:
-Doesn't like empty spaces inside a section. the values will be stored, but
 only as a comment in the main list. They will not be accessible.
 Haven't figured out how to handle this one yet... :(
-Not Case-Sensitive, like TSupraINI.
-Parse function requires a parameter - (Merge : boolean).


Terms of Use:
You may freely use this unit in any project you create, commercial, shareware,
or freeware with the following strings attached:
Please inform me that you're using it and in what. :)
If you modify or improve upon it, please send me the modified unit.
If you really feel generous, a finished copy of whatever the program was. :P
Honestly, the third's a joke but if you want to...I won't complain. ;)

Changes from TSupraINI:
Doesn't have RAW database load/save.
 (The data is stored as linked lists, not a flat formatted database,
 so there is no need for these functions.)

TODO:


Version History: (started at V2.0.10b)

TSupraINIv2 V2.0.10b
 (+) Started building parsing/storage core from scratch.

TSupraINIv2 V2.0.20b
 (+) INI file read/write with Streams and Files
 (+) Value reading/writing is implemented.
 (+) Section tools are implemented

TSupraINIv2 V2.0.30b
 (+) Management functions completed.
 (+) Merging functions completed. Has a few glitches,
     but nothing that should mess data reading - though if you dump the
     file back out it might look kinda strange depending on the input files. 
 (+) Custom Boolean support added.

TSupraINIv2 V2.0.35b
 (F) Apparently fixed the silly little extra space at the top when creating a new INI file.
 (M) Fixed a constant string issue oversight in the code.
}
interface
uses
// Windows,
// Messages, dialogs,
 SysUtils, Classes;
 
type
 TSupraINIBoolType = (siNum, siTrueFalse, siYesNo, siYN, siCustom); {Boolean storage config}
 TSupraINIv2 = class(TObject)
  private
   FBoolType : TSupraINIBoolType;
   FAboutText : String;
   FVersion : longint;
   FTrue : string;
   FFalse : string;
   FMainData : TStringList; {Main database }
   FTempData : TStringList; {Temporary storage for I/O}
   function GetValue(section, key : string; index : longint) : string; {Retrieves a value from the database}
   procedure PutValue(section, key, value : string; index : longint); {Stores a value in the database}
   function ReadTempData(Merge : boolean) : integer;
   function BuildINIData : integer; {Build valid INI file and place it in FTempData}
  protected
   function CharPos(Chr : AnsiChar; const Str : AnsiString) : Cardinal;
  public
   constructor Create(filename : string);
   destructor Destroy; override;
   function Parse(Merge : boolean) : integer;{Parse import buffer}
   {INI Load/Save functions
    Save creates a completely valid INI file.}
   procedure LoadFromStream(thestream : TStream);
   procedure LoadFromFile(filename : String);
   procedure AddFromStream(thestream : TStream);  {Add INI data to current database}
   procedure AddFromFile(filename : String);
   procedure SaveToStream(thestream : TStream);
   procedure SaveToFile(filename : String);
   {Management functions}
   procedure ClearDatabase;
   procedure EraseSection(Section: string);   {Erases a section from the database}
   procedure DeleteKey(Section, Key: string); {Delete a single key from the database}
   {Indexing Functions}
   procedure ReadSections(Strings: TStrings); {Retrieve the names of all sections in an INI file into a string list object. }
   procedure ReadSection (Section: string; Strings: TStrings); {Read all the key names in a section into a string list}
   procedure ReadSectionValues(const Section: string; Strings: TStrings);
             {Read all the values from a section's keys into a string list}
   {Data handling functions}
   function ReadString(Section, Key, Vdefault : string): string; {Read a string value }
//function ReadStringIndex(Section, Key, VDefault : String; index : integer); {Read a string value duplicate, index 0+}
   function ReadInteger(Section, Key : string; Vdefault : integer): integer; {Read an integer value}
   function ReadBool(Section, Key : string): boolean; {Read a Boolean value }
   function ReadFloat(Section, Key : string; Vdefault : extended): extended; {Read a Floating-point value}
   procedure WriteString(Section, Key, value : string); {Write a string value}
   procedure WriteInteger(Section, Key : string; value : integer); {Write an integer value}
   procedure WriteBool(Section, Key : string; value : boolean); {Write a boolean value}
   procedure WriteFloat(Section, Key : string; value : extended); {Write a floating-point value}
   {Properties}
   property About : String read FAboutText;
   property Version : longint read FVersion;
   property BoolType : TSupraINIBoolType read FBoolType write FBoolType;
   property BoolTrue : string read FTrue write FTrue;
   property BoolFalse : string read FFalse write FFalse;
   property TempData : TStringList read FTempData; {Direct access to import cache}
   property MainData : TStringList read FMainData;
  end;

implementation
const
{Internal copyright}
 copyrighttext = 'SupraINI Version 2.0.35b by Supralight Kaiilridge';
 versionnum = 20000;
 SectionOrd = 91; {[}

ResourceString
{Database parsing and storage constants}
 SectionChar = '[';
 SectionEnd  = ']';
 ValueEquals = '=';
 CommentChar = ';';
 Delimiter   = ',';
{Boolean types for output}
 BoolOne     = '1';
 BoolZero    = '0';
 BoolTrue    = 'TRUE';
 BoolFalse   = 'FALSE';
 BoolY       = 'Y';
 BoolN       = 'N';
 BoolYes     = 'YES';
 BoolNo      = 'NO';
{Other}
 StartDefault = 'SupraINIv2: No Entry (3946DFGHI378834SUPRA872385128INI7659283476)';

{
------------------------------------------------------------
--------------------Creation/Destruction--------------------
------------------------------------------------------------
}
constructor TSupraINIv2.Create(filename : string);
begin
FAboutText := copyrighttext;
FVersion := Versionnum;
FBoolType := siNum;
FMainData := TStringList.Create;
FTempData := TStringList.Create;
FFalse := BoolFalse;
FTrue := BoolTrue;
if filename <> '' then LoadFromFile(filename);
end;

destructor TSupraINIv2.Destroy;
begin
ClearDatabase;
FMainData.Free;
FTempData.Free;
//{$IFNDEF VER80} {note: Unknown if D2 TObject has destructor...Delphi 2=VER90}
inherited;
//{$ENDIF}
end;

{
------------------------------------------------------------
-------------------Library-Type Functions-------------------
------------------------------------------------------------
}
function TSupraINIv2.CharPos(Chr : AnsiChar; const Str : AnsiString) : Cardinal;
//(C) Eric Grange
var
   locChr : AnsiChar;
   i : Integer;
begin
   if Str<>'' then begin
      locChr:=Chr;
      for i:=0 to Length(Str)-1 do begin
         if Str[i+1]=locChr then begin
            Result:=i+1;
            Exit;
         end;
      end;
   end;
   Result:=0;
end;

{
------------------------------------------------------------
----------------------Parsing Functions---------------------
------------------------------------------------------------
}
function TSupraINIv2.ReadTempData(Merge : boolean) : integer;
{Parse import buffer}
{NOTE: Write merge parse code later!}
var
 ValueList : TStringList;
 position : longint;
 ScanStr : string;

 function AddSection: longint;
 {Internal routine to add a section.
  Sure, I know it probably isn't as efficient as it could be becuase
  it's a sub-procedure, but it's easier to keep track of.}
 var
  posi : integer;
  teststr : string;
 begin
// showmessage('Section ID: '+Scanstr);
//OutputDebugString(PChar(ScanStr));
 FMainData.Add(Scanstr);
 ValueList := TStringList.Create;
 posi := position+1;
 FMaindata.Objects[FMainData.count-1] := ValueList;
 while (posi <= FTempData.Count-1) do
  begin
  teststr := FTempdata.strings[posi];
   if length(teststr) > 0 then
    case ord(teststr[1]) of
     SectionOrd : Break;
     else
      TStringList(FMainData.Objects[FMainData.Count-1]).Add(teststr);
    end
   else
    begin
    posi := posi-1;
    break;
    end;
  posi := posi + 1;
  end;
 if length(teststr) > 0 then
  if teststr[1] = SectionChar then
   result := position + ((posi-1) - position)
  else result := position + (posi - position)
 else result := position + (posi - position);
 end;

 function MergeSection: longint;
 {Internal routine to merge or add.
 Calls AddSection if the section does not exist.}
 var
  secindex : longint;
  secpos : longint;
  teststr : string;
  posi : longint;
 begin
 posi := position+1;
 secindex := FMainData.IndexOf(ScanStr);
 if Secindex = -1 then
  begin
  //Do a standard AddSection
  result := AddSection;
//showmessage('Section Added');
  end
 else
  begin
//showmessage('Section Merged - '+Scanstr);
  //Do Merge
  ValueList := TStringList.Create;
  try
  //Fill section data to merge
   while (posi <= FTempData.Count-1) do
    begin
    teststr := FTempdata.strings[posi];
    if length(teststr) > 0 then
     case ord(teststr[1]) of
      SectionOrd : Break;
      else
       ValueList.Add(teststr);
     end
    else
     begin
     posi := posi-1;
     break;
     end;
    posi := posi + 1;
    end;
  //Merge data
   for secpos := 0 to Valuelist.count-1 do
    begin
    if Valuelist.Names[secpos] <> '' then
     begin
     TStringList(FMainData.Objects[secindex]).Values[ValueList.Names[secpos]] := ValueList.Values[Valuelist.Names[secpos]];
     end;
    end;
  finally
  valuelist.free;
   end;
  if length(teststr) > 0 then
   if teststr[1] = SectionChar then
    result := position + ((posi-1) - position)
   else result := position + (posi - position)
  else result := position + (posi - position);
  end;
 end;

begin
position := 0;
while (position <= FTempData.Count-1) do
 begin
 Scanstr := FTempData.Strings[position];
 if length(scanstr) > 0 then
  case ord(scanstr[1]) of
   SectionOrd : begin
                if not merge then Position := AddSection
                 else Position := MergeSection;
                end;
   else FMainData.Add(Scanstr);
  end
   else FMainData.Add(Scanstr);
 position := position + 1;
 end;
result := 0;
{;}{59 : showmessage('";" :'+Scanstr);}
end;

function TSupraINIv2.Parse(Merge : boolean) : integer;
begin
Result := ReadTempData(Merge);
end;

function TSupraINIv2.BuildINIData : integer;
{Build valid INI file and place it in FTempData}
var
 position : longint;
begin
FTempData.Clear;
for Position := 0 to FMainData.Count-1 do
 begin
 FTempData.Add(FMainData.strings[position]);
 if FMainData.Objects[Position] <> nil then
  begin
  FTempData.AddStrings(TStringList(FMainData.Objects[Position]));
  end;
 end;
result := 1;
end;

{
------------------------------------------------------------
------------------Data Retrieval/Storage--------------------
------------------------------------------------------------
}
function TSupraINIv2.GetValue(section, key : string; index : longint) : string;
{Retrieves a value from the database}
var
 fullsec : string;
 tempvalue : string;
 Secindex : integer;
 position : integer;
 found : boolean;
begin
found := false;
FullSec := SectionChar+Section+SectionEnd;
secindex := FMainData.IndexOf(FullSec);
if Secindex <> -1 then
 begin
 tempvalue := TStringList(FMainData.Objects[secindex]).Values[Key];
 if tempvalue = '' then
 {Quick name scan to make certain that an key returned as empty is
 actually in the database, which should result in a blank return}
  for Position := 0 to TStringList(FMainData.Objects[secindex]).count-1 do
   begin
   if (uppercase(TStringList(FMainData.Objects[secindex]).names[position]) = uppercase(key)) then
    begin
    found := true;
    break;
    end;
   end
   {end of scan code}
  else found := true;
 end;
if found = false then result := StartDefault
 else result := tempvalue;
end;

procedure TSupraINIv2.PutValue(section, key, value : string; index : longint);
{Stores a value in the database}
var
 fullsec : string;
 Secindex : integer;
 ValueList : TStringList;
begin
FullSec := SectionChar+Section+SectionEnd;
secindex := FMainData.IndexOf(FullSec);
if Secindex <> -1 then
 begin
 TStringList(FMainData.Objects[secindex]).Values[key] := value;
 end
else
 begin
 if FMainData.Count <> 0 then FMainData.Add('');
 FMainData.Add(SectionChar+section+SectionEnd);
 ValueList := TStringList.Create;
 FMaindata.Objects[FMainData.count-1] := ValueList;
 TStringList(FMaindata.Objects[FMainData.count-1]).Values[key] := value;
 end;
end;

{
------------------------------------------------------------
---------------------Saving and Loading---------------------
------------------------------------------------------------
}
procedure TSupraINIv2.LoadFromStream(thestream : TStream);
{Load and parse INI data from a generic stream interface, clearing out
whatever's in the database right now.  }
begin
ClearDatabase;
FTempData.LoadFromStream(thestream);
Parse(false);
end;

procedure TSupraINIv2.LoadFromFile(filename : String);
{Load and parse INI data directly from a file, clearing out whatever's
in the database right now.}
begin
ClearDatabase;
FTempData.LoadFromFile(filename);
Parse(false);
end;

procedure TSupraINIv2.AddFromStream(thestream : TStream);
{Load and parse INI data from a generic stream interface, adding to what's
in the current database.}
begin
FTempData.LoadFromStream(thestream);
Parse(true);
end;

procedure TSupraINIv2.AddFromFile(filename : String);
{Load and parse INI data directly from a file, adding to what's in the
current database.}
begin
FTempData.LoadFromFile(filename);
Parse(true);
end;

procedure TSupraINIv2.SaveToStream(thestream : TStream);
{Save encoded INI data to a Delphi stream}
begin
BuildINIData;
FTempData.SaveToStream(theStream);
end;

procedure TSupraINIv2.SaveToFile(filename : String);
{Save encoded INI data to a file}
begin
BuildINIData;
FTempData.SaveToFile(filename);
end;
{
------------------------------------------------------------
---------------------Management Functions-------------------
------------------------------------------------------------
}

procedure TSupraINIv2.ClearDatabase;
{Clear the database}
var
 position : longint;
begin
for position := 0 to (FMainData.Count - 1) do
 begin
 FMainData.Objects[position].Free;
 end;
FMainData.clear;
FTempData.clear;
end;

procedure TSupraINIv2.EraseSection(Section: string);
{Erases a section from the database}
var
 fullsec : string;
 Secindex : integer;
begin
FullSec := SectionChar+Section+SectionEnd;
secindex := FMainData.IndexOf(FullSec);
if Secindex <> -1 then
 begin
 //Free Sublist
 FMainData.Objects[secindex].Free;
 //Clear entry
 FMainData.Delete(secindex);
 //Clean out the next empty space, if there is one.
 //(Becuase SupraINI, when adding sections with PutValue
 //adds spaces between the sections.
 if secindex > FMainData.Count-1 then secindex := secindex-1;
 if (FMainData.Strings[secindex] = '') then
  FMainData.Delete(Secindex);
 end;
end;

procedure TSupraINIv2.DeleteKey(Section, Key: string);
{Delete a single key from the database}
var
 fullsec : string;
// tempvalue : string;
 Secindex : integer;
 position : integer;
// found : boolean;
begin
//found := false;
FullSec := SectionChar+Section+SectionEnd;
secindex := FMainData.IndexOf(FullSec);
if Secindex <> -1 then
 begin
  for Position := 0 to TStringList(FMainData.Objects[secindex]).count-1 do
   begin
   if (uppercase(TStringList(FMainData.Objects[secindex]).names[position]) = uppercase(key)) then
    begin
    TStringList(FMainData.Objects[secindex]).delete(position);//TStringList(FMainData.Objects[secindex]).names[position])
//    showmessage(inttostr(position));
    //found := true;
    break;
    end;
   end
 end;
end;

{
------------------------------------------------------------
--------------------Indexing Functions----------------------
------------------------------------------------------------
}
procedure TSupraINIv2.ReadSections(Strings: TStrings);
{Return complete list of all available sections}
var
 position : longint;
 test : string;
begin
 for position := 0 to FMainData.count-1 do
  begin
  test := FMainData[position];
  if length(test) > 0 then
   if test[1] = SectionChar then
    begin
    delete(test,1,1);
    delete(test,length(test),1);
    Strings.Add(test);
    end;
  end;
end;

procedure TSupraINIv2.ReadSection(Section: string; Strings: TStrings);
{Read all the key names in a section into a string list}
var
 fullsec : string;
 tempvalue : string;
 Secindex : integer;
 position : integer;
begin
FullSec := SectionChar+Section+SectionEnd;
secindex := FMainData.IndexOf(FullSec);
if Secindex <> -1 then
 begin
 for position := 0 to TStringList(FMainData.Objects[secindex]).count-1 do
  begin
  tempvalue := TStringList(FMainData.Objects[secindex]).Strings[position];
  if (length(tempvalue) > 0) then
   if (pos(ValueEquals, tempvalue) > 0) then
    begin
    delete(tempvalue,pos(ValueEquals, Tempvalue),length(tempvalue));
    Strings.Add(tempvalue);
    end;
  end;
 end;
end;

procedure TSupraINIv2.ReadSectionValues(const Section: string; Strings: TStrings);
{Read all the keys+values from a section's keys into a string list}
var
 fullsec : string;
 tempvalue : string;
 Secindex : integer;
 position : integer;
begin
FullSec := SectionChar+Section+SectionEnd;
secindex := FMainData.IndexOf(FullSec);
if Secindex <> -1 then
 begin
 for position := 0 to TStringList(FMainData.Objects[secindex]).count-1 do
  begin
  tempvalue := TStringList(FMainData.Objects[secindex]).Strings[position];
  if (length(tempvalue) > 0) then
   if (pos(ValueEquals, tempvalue) > 0) then
    begin
    Strings.Add(tempvalue);
    end;
  end;
 end;
end;

{
------------------------------------------------------------
------------------Read/Write Functions----------------------
------------------------------------------------------------
}
function TSupraINIv2.ReadString(Section, Key, Vdefault : string): string;
var temp : string;
begin
temp := GetValue(Section,Key,0);
if temp = StartDefault then temp := VDefault;
Result := temp;
end;

function TSupraINIv2.ReadInteger(Section, Key : string; Vdefault : integer): integer;
{Read an integer value}
var
temp : string;
int : integer;
begin
temp := GetValue(Section,Key,0);
if temp = StartDefault then int := VDefault
 else int := strtoint(temp);
Result := int;
end;

function TSupraINIv2.ReadBool(Section, Key : string): boolean;
{Read a boolean value.
Supports YES,Y,1,TRUE, everything else is false.}
var
 temp : string;
begin
temp := GetValue(Section,Key,0);
Temp := Uppercase(temp);
if (temp = BoolOne)
 or (temp = BoolTrue)
 or (temp = BoolY)
 or (temp = BoolYes)
 or (temp = FTrue)
  then result := true
 else Result := false;
end;

function TSupraINIv2.ReadFloat(Section, Key : string; Vdefault : extended): extended;
{Read a floating-point value }
var
 temp : string;
 int : extended;
begin
temp := GetValue(Section,Key,0);
if temp = StartDefault then int := VDefault
 else
  try {Exception handler for converter}
  int := strtofloat(temp);
  except
  on EConvertError do int := 0;
  end;
Result := int;
end;

procedure TSupraINIv2.WriteString(Section, Key, value : string);
{Write a value into the database}
begin
PutValue(section, key, value, 0);
end;

procedure TSupraINIv2.WriteInteger(Section, Key : string; value : integer);
{Write an integer value}
var
 temp : string;
begin
{Convert value}
temp := inttostr(value);
{check for and write value}
PutValue(section, key, temp, 0);
end;

procedure TSupraINIv2.WriteBool(Section, Key : string; value : boolean);
{Write a boolean value into the database}
var
 temp : string;
begin
{Convert value}
if FBoolType = siNum then      {numbers, 0=false}
 begin
 if value then temp := BoolOne
  else temp := BoolZero;
 end else
if FBoolType = siTrueFalse then {True/False}
 begin
 if value then temp := BoolTrue
  else temp := BoolFalse;
 end else
if FBoolType = siYN then       {Y/N}
 begin
 if value then temp := BoolY
  else temp := BoolN;
 end else
if FBoolType = siYesNo then    {Yes/No}
 begin
 if value then temp := BoolYes
  else temp := BoolNo;
 end else
if FBoolType = siCustom then    {Custom}
 begin
 if value then temp := FTrue
  else temp := FFalse;
 end;
{check for and write value}
PutValue(section, key, temp, 0);
end;

procedure TSupraINIv2.WriteFloat(Section, Key : string; value : extended);
{Write a floating-point value}
var
 temp : string;
begin
{Convert value}
temp := FloatToStrF(value, ffGeneral, 18, 0);
{check for and write value}
PutValue(section, key, temp, 0);
end;

end.
