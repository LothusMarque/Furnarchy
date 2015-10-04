unit HandlerStuff1;
{
Patchfusion 1.9xx
©2005-2008 Mercenary Enclave Productions
http://syphor.ne1.net
Written by Lothus Marque/Syphor Knight
                               
Container classes, utility functions, and constants.
               

Source Usage rules:
You may NOT claim the work herein is purely your own.
If you modify this software, I would be pleased if you would contact me with any
changes/additions you have made. I'm quite interested in this, and if you allow
they may be added (with full credit) to my 'official' release.
Contact me at supramatrix-AT-gmail.com for this. (Spamtrap added for who knows why)

Additionally, I wish to be informed if you use any code from here in a project of your own.
Should I not be easily available, this does not block you from going ahead and using the
code in your own work.
}
interface

uses
  Windows, Classes, Graphics, Sysutils;

type
 TSelBoxMode = (SEL_Main, SEL_Sub);
 RPicInfo = record  //Basically a raw read of the FSH/FBJ parameters
  height, width : word;
  xpos, ypos : smallint;
  fbjxpos : smallint;
  fbjypos : smallint;
  fbjflags : word;  //Stores raw flag data
  format : integer;   //Hmmm...
  fs2index : smallint;
  end;
 RFrameinfo = record
  fbjflags : word;
  fs2index : smallint;
 end;
 RAnimStep = record
  ruleType : smallint;
  value : smallint;
  counterMax : smallint;
 end;
 RFBJFlags = record //Quick and easy way to keep the set of options together
  sit : boolean;
  walk : boolean;
  get : boolean;
  end;
 TPatchSubFrame = Class(TObject)
   info: RPicInfo;
   picture: TMemoryStream;
   constructor Create;
   destructor Destroy; override;
  end;
 TPatchFrame = class(TObject)
  public
  info: RFrameinfo;
  FrameCount : word;
  Frames: TList;
  Stepcount : word;
  Steps: array of RAnimStep;
  constructor Create;
  destructor Destroy; override;
  end;

 //This function takes an FBJ option byte and expands it into
 //the three boolean values it stores.
 function decodeflags(input : shortint) : RFBJFlags;
 //Due to the way Furcadia stores FBJ data, you must give the encoder
 //the FurreX and FurreY coordinates to correctly convert.
 function encodeflags(input : RFBJFlags; X, Y : shortint) : shortint;

 function FOXdecodeflags(input : shortint) : RFBJFlags;
 function FOXencodeflags(input : RFBJFlags) : smallint;



const
 FT_FS2 = '.FS2';                           
 FT_FSH = '.FSH';
 FT_Fbj = '.fbj';
 FT_FOX = '.FOX';
//Header/footer resources
 RES_FSHFooter = 'FSH1.002';//Data appended to end of FSH files
 RES_FS2HeadV1 = 'FSH2.';
 RES_FS2HWrite = 'FSH2.002';
 RES_FBJHead = 'FO01';//FBJ Header notation
 RES_MaxFS2Ver = 2;
 RES_FBJMaxVer = 1;
 RES_FBJHeadBase = 'FO';
 RES_FS2Hlen = 7;
 RES_FBJHlen = 4;
 RES_FOXHlen = 4;
 RES_FOXHeader = 'FSHX';
 RES_FOXVer = 1;
//System
 K_Delete = 46;//Delete keycode
 MaxVisible = 5;//This should be set to how many columns are visible in the grid
//Palette Handling
 RES_MainRes = 'FURC';
 RES_PalRes = 'PAL';
 PaletteSize = 256;
//Messages
 MSG_EncryptedFS2 = 'This FS2 file is encrypted and cannot be loaded!';
 MSG_BadFBJ = 'The FBJ file exists but is an unrecognised version.'#13'FBJ flags will not transfer.';
 MSG_NewFBJ = 'The FBJ file appears to be newer than supported. Attempt to load it anyway?'#13;
 MSG_BadFS2 = 'Bad or unrecognised FS2 file!';
 MSG_Error = 'Error!';
 MSG_FOX_ERR_Enc = 'Error: Encrypted FOX! This cannot be read.';
 MSG_FOX_ERR_Ver = 'Error: Unknown FOX version';
 MSG_FOX_ERR_Bad = 'Error: Not a Furcadia FOX file!';

const //FBJ handler constants
 FurcFBJWalk = 1;
 FurcFBJGet = 2;
 FurcFBJSit = 4;
 FurcFBJX = 8;
 FurcFBJY = 16;
 FurcFBJFly = 32; //guess
//
 FORMAT_8BIT=1;
 FORMAT_RGB=2;
//
 GEN_FSHED = 0;
 GEN_DUPLOAD = 1;
//
 ENC_NONE = 0;
 ENC_NEW = 2;
//
 RES_BoxWidth = 3; //Width of selection box
 RES_MainBoxColor = CLRED;
 RES_SubBoxColor = CLGREEN;

var
 MainBoxColor: tColor;
 SubBoxColor: tColor;

implementation

//Disabled here for auto-compilation in the DPR
//{$R 'palettes.res' 'palettes.rc'} //Include resource script

//===========TPatchSubFrame methods===========
constructor TPatchSubFrame.Create;
begin
picture := TMemoryStream.Create;
info.fs2index := -1;
info.format := 1;
end;

destructor TPatchSubFrame.Destroy;
begin
picture.free;
inherited;
end;

//===========TPatchFrame methods===========
constructor TPatchFrame.Create;
begin
//picture := TMemoryStream.Create;
//info.format := 1;
info.fs2index := -1;
Frames := TList.Create;
end;

destructor TPatchFrame.Destroy;
var
  I: Integer;
begin
if Frames.count > 0 then
 for I := 0 to {length(frames)}Frames.Count - 1 do
  TPatchSubFrame(Frames[I]).Free;
Frames.Free;
//picture.free;
inherited;
end;

//FBJ Flag handling routines
function decodeflags(input : shortint) : RFBJFlags;
begin
with result do
 begin
 sit := false;
 walk := false;
 get := false;
  walk := (input AND FurcFBJWalk) > 0;
  sit  := (input AND FurcFBJSit) > 0;
  Get  := (input AND FurcFBJGet) > 0;
 //Guestimate
  //fly := (input and FurcFBJFly) > 0;
 end;
end;
              
function encodeflags(input : RFBJFlags; X, Y : shortint) : shortint;
begin
result := 0;
with input do //Bitwise encoding
 begin
 if walk then result := result or FurcFBJWalk;
 if get then result := result or FurcFBJGet;
 if sit then result := result or FurcFBJSit;
 if x <> 0 then result := result or FurcFBJX;
 if y <> 0 then result := result or FurcFBJY;
 //5 bits used, 3 remaining empty
 //Guestimate on upcoming update
 //if fly then result := result or FurcFBJFly;
 end;
end;

//FBJ Flag handling routines
function FOXdecodeflags(input : shortint) : RFBJFlags;
begin
with result do
 begin
 sit := false;
 walk := false;
 get := false;
  walk := (input AND FurcFBJWalk) > 0;
  sit  := (input AND FurcFBJSit) > 0;
  Get  := (input AND FurcFBJGet) > 0;
 //Guestimate
  //fly := (input and FurcFBJFly) > 0;
 end;
end;
              
function FOXencodeflags(input : RFBJFlags) : smallint;
begin
result := 0;
with input do //Bitwise encoding
 begin
 if walk then result := result or FurcFBJWalk;
 if get then result := result or FurcFBJGet;
 if sit then result := result or FurcFBJSit;
 //5 bits used, 3 remaining empty
 //Guestimate on upcoming update
 //if fly then result := result or FurcFBJFly;
 end;
end;

end.
 