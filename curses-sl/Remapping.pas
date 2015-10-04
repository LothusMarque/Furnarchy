unit Remapping;

interface
uses
 Windows, sysutils, classes, graphics, {GR32, GR32_Image, GraphicEx,}
 SupraINIv2, CursesUtils, FOXGraphic;

CONST
  PixelCountMax = 32768;

type
  TColormap = array of array [0..1] of longint;
  RGBQuick = packed record
  case Integer of
   0: (rgbBlue: Byte;
       rgbGreen: Byte;
       rgbRed: Byte;
       rgbReserved: Byte);
   1: (bytepack: longint);
  end;
 pRGBQuick = ^RGBQuick;
 TRGBTripleArray = ARRAY[0..PixelCountMax-1] OF TRGBTriple;
 pRGBTripleArray = ^TRGBTripleArray;

resourcestring
 //Resource File
 DATA_Colormap = 'MAP';
 //File Extensions
 EXT_FOX = '.FOX';
 EXT_FSH = '.FSH';

 //Colorcode Functions
 function RandomizeClothes(coredata: TSupraIniV2; input:string):string; //Takes a colorcode, randomizes only clothes entries.
 function RandomizeGender(coredata: TSupraIniV2; input: string):string;
 function RandomizeSpecies(coredata: TSupraIniV2; input: string): string;
 function RandomCC(coredata: TSupraIniV2):string; //Full random colorcode
 //TColorMap Access Functions
 procedure CM_Clear(var cmap:TColormap);
 procedure CM_SetPair(var cmap : TColorMap; key, value: integer);
 function CM_GetValue(var cmap : TColorMap; key: integer):integer;

 //Loading
 procedure GetFurcImg(dest: TBitmap; filename: string); //Wraps LoadFurcImage, takes 'filename,idx' format
 procedure LoadFurcImage(dest: TBitmap; filename: string; index: integer);
 //Remapping
 procedure Remap(ini : TSupraIniV2; pic: TBitmap; cc:string);
 procedure Build8Map(coredata: TSupraINIv2; code : string; var cmap:TColorMap);

implementation

//Colorcode Functions
function RandomCC(coredata: TSupraIniV2):string;
var
 t1 : TStringList;
begin
result := 't';
t1 := TStringList.Create;
 try
 coredata.ReadSection('Fur',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Markings',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Hair',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Eyes',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Badge',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Vest',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Bracers',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Cape',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Boots',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Trousers',t1);
 result := result + makeSingleB220(random(t1.Count));
 finally
 t1.free;
 end;
end;

function RandomizeSpecies(coredata: TSupraIniV2; input: string): string;
var
 t1 : TStringList;
begin
result := copy(input,1,12); //First 12 characters
t1 := TStringList.create;
 try
  coredata.ReadSection('Species', t1);
  result := result + makeSingleB220(random(t1.Count));
  result := result + copy(input,13,length(input));
 finally
 t1.Free;
 end;
end;

function RandomizeGender(coredata: TSupraIniV2; input: string):string;
var
 t1 : TStringList;
begin
result := copy(input,1,11); //First 11 characters
t1 := TStringList.create;
 try
  coredata.ReadSection('Gender', t1);
  result := result + makeSingleB220(random(t1.Count));
  result := result + copy(input,12,length(input));
 finally
 t1.Free;
 end;
end;

function RandomizeClothes(coredata: TSupraIniV2; input:string):string;
var
 t1 : TStringList;
begin
result := copy(input,1,5); //First 5 characters
t1 := TStringList.Create;
 try
 coredata.ReadSection('Badge',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Vest',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Bracers',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Cape',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Boots',t1);
 result := result + makeSingleB220(random(t1.Count));
 t1.Clear;
 coredata.ReadSection('Trousers',t1);
 result := result + makeSingleB220(random(t1.Count));
 result := result + copy(input, length(result)+1,length(input));
 finally
 t1.free;
 end;
end;


//TColormap functions
procedure CM_Clear(var cmap:TColormap);
//Clear TColormap array
begin
setlength(cmap,0);
end;

procedure CM_SetPair(var cmap : TColorMap; key, value: integer);
//Set a Colormap value pair
var
 posi : integer;
begin
for posi := 0 to length(cmap) - 1 do
  begin
  if cmap[posi][0] = key then
   begin
   cmap[posi][1] := value;
   exit;
   end;
  end;
setLength(cmap, length(cmap)+1);
cmap[length(cmap)-1][0] := key;
cmap[length(cmap)-1][1] := value;
end;

function CM_GetValue(var cmap : TColorMap; key: integer):integer;
//Get a colormap value
var
  posi: Integer;
begin
for posi := 0 to length(cmap) - 1 do
 begin
 if cmap[posi][0]=key then
  begin
  result := cmap[posi][1];
  exit;
  end;
 end;
result := -1;
end;
//End Colormap core functions

procedure GetFurcImg(dest: TBitmap; filename: string); //Wraps LoadFurcImage, takes 'filename,idx' format
var
 fil : string;
 tmp : integer;
 index : integer;
begin
tmp := LastPos(',',filename);
fil := copy(filename,1, tmp-1);
index := strtoint2(copy(filename,tmp+1, length(filename)));
LoadFurcImage(dest,fil,index);
end;

procedure LoadFurcImage(dest: TBitmap; filename: string; index: integer);
var
 fsh : TFSHGraphic;
 fox : TFOXGraphic;
 t1 : string;
begin
//Furcadia Object eXtended  (FOX)
t1 := uppercase(extractfileext(filename));
if extractfilepath(filename) = '' then
 begin
 filename := g_FurcPatchFolder + filename;
 end;
if not fileexists(filename) then
 exit; //Yipe, file doesn't exist!

if t1 = EXT_FOX then
 begin
 fox := TFOXGraphic.Create;
  try
  fox.LoadFromFileIndex(filename,index);
  dest.Assign(fox);
  finally
  fox.Free;
  end;
 end;
//Furcadia SHape
if t1 = EXT_FSH then
 begin
 fsh := TFSHGraphic.Create;
  try
  fsh.LoadFromFileIndex(filename,index);
  dest.Assign(fsh);
  finally
  fsh.Free;
  end;
 end;
end;

procedure Remap(ini : TSupraIniV2; pic: TBitmap; cc:string);
var
 x,y : integer;
// test : TStringList;
 cmap : TColorMap;
 c : integer;
 Line : pByteArray;
{ line24 : pRGBTripleArray;
 tmp : RGBQuick; }
begin
//if cc <> '' then
 try
 if pic.PixelFormat = pf8Bit then
  begin
  Build8Map(ini, cc, cmap);
  for X := 0 to pic.Height - 1 do
   begin
   line := pic.ScanLine[x];
   for Y := 0 to pic.Width - 1 do
    begin//Do mapping
    c := CM_GetValue(cmap,line[y]);
    if c > -1 then
     line[y] := c;
    end;
   end;
  exit;
  end; //IF 8bit
 {if pic.PixelFormat = pf24bit then //24Bit Mapping
  begin
  test := TStringList.Create;
  for X := 0 to pic.Width - 1 do
   begin
   line24 := pic.ScanLine[x];
   for Y := 0 to pic.Height - 1 do
    begin//Do mapping
    tmp.rgbBlue := line24[y].rgbtBlue;
    tmp.rgbGreen := line24[y].rgbtGreen;
    tmp.rgbRed := line24[y].rgbtRed;
    test.Values[inttostr(tmp.bytepack)] := inttostr(X)+','+inttostr(Y)+','+inttostr(X*pic.Width+Y);//inttostr(tmp.bytepack);
    //c := CM_GetValue(cmap,line[y]);
    //if c > -1 then
    // line[y] := c;

    end;
   end;
  test.SaveToFile(extractfilepath(paramstr(0))+'dumper.txt');
  test.free;
  exit;
  end; //IF 24bit }
 finally
 end;
end;

procedure Build8Map(coredata: TSupraINIv2; code : string; {cmap : TStringlist}var cmap:TColorMap);
var
 posi : integer;
 tmp : integer;

procedure LoadSection(sect : string; sel : integer);
var
 t : string;
 sectlist : TStringList;
 I: Integer;
begin
 sectlist := TStringlist.create;
 try
 if coredata.ReadString(sect,inttostr(sel),'') = '' then
  sel := 0;
 t := coredata.ReadString(sect,inttostr(sel),'');
 if t <> '' then //make sure it's not actually gone
  begin
  coredata.ReadSection(t, sectlist);
  //OutputDebugString(pchar(sect+'-'+t));
  //t8#'73%@.%%
  for I := 0 to sectlist.count - 1 do
   CM_SetPair(cmap,strtoint2(sectlist[i]),coredata.ReadInteger(t,sectlist[i],0));
  end;
 finally
 sectlist.free;
 end;
end;

begin
//data := TSupraINIv2.Create('');
//data.LoadFromFile(extractfilepath(paramstr(0))+'colordb.ini');

if (length(code) = 0) or (code[1] <> 't') then
 begin
 code := 't################'
 end;

 try
 //cmap.Clear;
 CM_Clear(cmap);
 delete(code,1,1);
 posi := 1;
// if length(code) > 0 then
 while ((posi < 11) or (posi < length(code)+1)) do
  begin
  if posi > length(code) then
   tmp := 0
  else
   tmp := getSingleB220(Code[posi]);
  case posi of
   1: LoadSection('Fur',tmp);
   2: LoadSection('Markings',tmp);
   3: LoadSection('Hair',tmp);
   4: LoadSection('Eyes',tmp);
   5: LoadSection('Badge',tmp);
   6: LoadSection('Vest',tmp);
   7: LoadSection('Bracers',tmp);
   8: LoadSection('Cape',tmp);
   9: LoadSection('Boots',tmp);
  10: LoadSection('Trousers',tmp);
  end;
  posi := posi + 1;
  end;
 finally
 //data.free;
 end;
//cmap.SaveToFile(extractfilepath(paramstr(0))+'dump.txt');
end;
end.
