unit FoxGraphic;
{
FOXGraphic
Copyright Mercenary Enclave Productions

==Description==
This unit provides TGraphic objects to load single frames from FOX1
and FSH formats like any normal image.

==Changelog==
--V1.0--
Added: Everything

--V1.0.1--
Added: LoadFromFileIndex function, loads specific image out of patch file
Changed: Moved real work to LoadFromFileIndex, LoadFromFile is a simple wrapper

}
interface
uses  Windows, graphics, classes, HandlerStuff1, sysutils;

type
 TFoxGraphic = class(TBitmap)
  private
  overpal : TMaxLogPalette;
  public
  //alpha : TBitmap;
  //remap : TBitmap;
  procedure LoadFromFile(const filename: string); override;
  procedure LoadFromFileIndex(const filename: string; index:integer = 0);
  constructor Create; override;
  end;

 TFSHGraphic = class(TBitmap)
  private
  overpal : TMaxLogPalette;
  public
  procedure LoadFromFile(const filename: string); override;
  procedure LoadFromFileIndex(const filename: string; index:integer = 0); //override;
  constructor Create; override;
  end;

const
 RES_Palette = 'PAL';

implementation

procedure TFoxgraphic.LoadFromFile(const filename: string);
begin
LoadFromFileIndex(filename);
end;

procedure TFoxGraphic.LoadFromFileIndex(const filename: string; index:integer = 0);
var
 inp : TFileStream; //FOX Input file
 numshapes: integer;
 b_u16 : word; //Size-named reader variables. xD
 b_32: longint;
 b_16: smallint;
 b_u32: longword;
// holder : TPatchFrame; //Structure for allocating memory
framecount : integer;
framelimit : integer;
stepcount,steppos : integer;
 subholder : TPatchSubFrame;
 verheader : ansistring;
 I, FramePos: Integer;
 c1, c2 : integer;
 n2 : byte;
 Line : pByteArray;
begin
//Load the first frame of a FOX
inp := Tfilestream.create(filename,fmOpenRead);
subholder := TPatchSubFrame.Create; //We'll only ever need one...
try
 setlength(verheader,RES_FOXHlen);
 inp.Read(PAnsiChar(verheader)^,length(verheader));
 if verheader <> RES_FOXHeader then
  begin
  //Throw an error!
  OutputDebugString(MSG_FOX_ERR_Bad);
  exit;
  end;
inp.Read(b_32, sizeof(b_32)); //FOX Version
if b_32 <> RES_FOXVer then
 begin
  //Throw an error!
  OutputDebugString(MSG_FOX_ERR_Ver);
  exit;
 end;
inp.Read(b_32, sizeof(b_32)); //Number of image blocks
numshapes := b_32;
inp.Read(b_32, sizeof(b_32)); //FOX Type
inp.Read(b_32, sizeof(b_32)); //Encoding
if b_32 <> ENC_NONE then
 begin
 OutputDebugString(MSG_FOX_ERR_Enc);
 exit;
 end;
inp.Read(b_32, sizeof(b_32)); //Reserved 1
inp.Read(b_32, sizeof(b_32)); //Reserved 2

//Sanity Clause EXISTS!
if index+1 > numshapes then
 index := 0;


if numshapes >= 1 then
for I := 1 to index+1 do //numshapes do
 begin //Load FOX ImageBlock
 //holder := TPatchFrame.Create;
 inp.Read(b_u16, sizeof(b_u16));  //Object Flags
 //holder.info.fbjflags := b_u16;
 inp.Read(b_16, sizeof(b_16)); //Replacement Index
 //holder.info.fs2index := b_16;
 inp.Read(b_u16, sizeof(b_u16)); //number of Frames
 //holder.FrameCount := b_u16;
 framecount := b_u16;
 inp.Read(b_u16, sizeof(b_u16)); //number of Steps
 StepCount := b_u16;
 framelimit := 1;
 if FrameCount >= 1 then
  begin
  if I < index+1 then
   framelimit := framecount;
  end;
 if FrameCount >= 1 then
 for FramePos := 1 to framelimit{index+}{1}{holder.Framecount} do //Load Image frame(s)
  begin
   inp.Read(b_16, sizeof(b_16)); //Shape Format

   try
   subholder.info.format := b_16;
   inp.Read(b_u16, sizeof(b_u16)); //Shape Width
   SubHolder.info.width := b_u16;
   inp.Read(b_u16, sizeof(b_u16)); //Shape Height
   SubHolder.info.height := b_u16;
   inp.Read(b_16, sizeof(b_16)); //Shape PosX
   SubHolder.info.xpos := b_16;
   inp.Read(b_16, sizeof(b_16)); //Shape PosY
   SubHolder.info.ypos := b_16;
   inp.Read(b_16, sizeof(b_16)); //Shape FurrePosX
   SubHolder.info.fbjxpos := b_16;
   inp.Read(b_16, sizeof(b_16)); //Shape FurrePosY
   SubHolder.info.fbjypos := b_16;
   //Image data size
   inp.Read(b_u32, sizeof(b_u32)); //ImageData Size
   if I = index+1 then //If we're on target, actually allocate and load it.
    begin
    //Outputdebugstring(pchar('Image Size: '+inttostr(b_u32)));
    SubHolder.picture.SetSize(b_u32);
    Subholder.picture.Position := 0;
    if Subholder.picture.Size <> 0 then  //Don't even try if it's 0. Otherwise TMemoryStream explodes silently.
     Subholder.picture.CopyFrom(inp, subholder.picture.Size); //Load image data
    break;
    //Holder.Frames.Add(Subholder);
    //Subholder := nil;
    end
   else
    begin
    inp.Seek(b_u32,soFromCurrent); //Just move ahead
    end;
   finally
    //We need a better failure mode...
    //outputDebugString(
    end;//Finally
   //OutputDebugString(pchar('Animation Steps read: '+inttostr(stepcount)));
   end;//For Framepos
  if subholder.picture.Size = 0 then for StepPos := 0 to stepcount-1 do
    begin
    inp.Read(b_16, sizeof(b_16));
    inp.Read(b_16, sizeof(b_16));
    inp.Read(b_16, sizeof(b_16));
    end;
 end; //For Numshapes


//At this point we have one TPatchSubFrame with an image
//Now to load it into the TBitmap we inherited from.
Pixelformat := pf8Bit;
Palette := CreatePalette(TLogPalette((@overpal)^)); //Set up the windows Palette from our memory cache
if subholder.info.format <> FORMAT_8BIT then
 begin
 OutputDebugString('Unsupported Frame Format!');
 end
else
 begin
 subholder.picture.position := 0;//Get ready to copy raw data
 width := subholder.info.width;
 Height := subholder.info.height;
 if (Height <> 0) or (Width <> 0) then
 for c1 := subholder.info.height-1 downto 0 do
  begin //Load raw data into the prepared bitmap image
  Line:=Scanline[c1];
  for c2 := 0 to subholder.info.width-1 do
   begin
   subholder.picture.Read(n2,sizeof(n2));
   line[c2] := n2;
   end;
  end;
 end;
finally
subholder.Free;
inp.free;
end;

end;


constructor TFoxGraphic.Create;
var
 i : integer;
 pal : TResourceStream;
 value : byte;
begin
inherited; //Initial setup
//Load palette entries!
pal := TResourceStream.Create(hInstance,Pchar(RES_MainRes),pchar(RES_PalRes));
try
pal.Position := 0;
with overpal do
 begin
 palVersion:= $300; //Standard structure version number
 palNumEntries:= 256; //How many palette items
 for i := 0 to 255 do //Read them one by one into memory
  begin
  pal.read(value, 1);
  palPalEntry[i].peRed:= value;
  pal.read(value, 1);
  palPalEntry[i].peGreen:= value;
  pal.read(value, 1);
  palPalEntry[i].peBlue:= value;
  end;
 end;
finally
 pal.free;  //Free the loader stream
 end;
end;

//==TFSHGraphic==
constructor TFSHGraphic.Create;
var
 i : integer;
 pal : TResourceStream;
 value : byte;
begin
inherited; //Initial setup
//Load palette entries!
pal := TResourceStream.Create(hInstance,Pchar(RES_MainRes),pchar(RES_PalRes));
try
pal.Position := 0;
with overpal do
 begin
 palVersion:= $300; //Standard structure version number
 palNumEntries:= 256; //How many palette items
 for i := 0 to 255 do //Read them one by one into memory
  begin
  pal.read(value, 1);
  palPalEntry[i].peRed:= value;
  pal.read(value, 1);
  palPalEntry[i].peGreen:= value;
  pal.read(value, 1);
  palPalEntry[i].peBlue:= value;
  end;
 end;
finally
 pal.free;  //Free the loader stream
 end;
end;

procedure TFSHGraphic.LoadFromFile(const filename: string);
begin
LoadFromFileIndex(filename);
end;

procedure TFSHGraphic.LoadFromFileIndex(const filename: string; index:integer = 0);
var
 inp : TFileStream; //FSH/FS2 Input file
 c1, chunknum, chunkpos, c2 : integer;
 n1 : word; //Badly named reader variables. xD
 n2 : byte;
 n3 : shortint;
// holder : TPatchFrame; //Structure for allocating memory
 subholder : TPatchSubFrame;
 chunksizes : TList; //using pointers as integer storage should work just fine
 Line : pByteArray;
begin
inp := Tfilestream.create(filename,fmOpenRead);
Subholder := TPatchSubFrame.Create;
chunksizes := TList.Create;
try
inp.Read(n1,sizeof(n1)); //Number of chunks in the file
chunknum := n1;
for c1 := 1 to chunknum do
 begin
 //Collect chunksizes. Under normal conditions they aren't needed
 //however, sometimes image sizes are smaller than a chunk has allocated.
 inp.Read(n1,sizeof(n1));
 //Store them into a list
 chunksizes.Add(Pointer(n1));
 end;

//Sanity Clause EXISTS!
if index+1 > chunknum then
 index := 0;

if chunknum > 0 then
for chunkpos := 1 to index+1{chunknum} do
 begin
 inp.Read(n2,sizeof(n2)); //Image width
 subholder.info.width := n2;
 inp.Read(n2,sizeof(n2)); //Image height
 subholder.info.height := n2;
 inp.Read(n3,sizeof(n3)); //X Position
 subholder.info.xpos := n3;
 inp.Read(n3,sizeof(n3)); //Y Position
 subholder.info.ypos := n3;
 if chunkpos = index+1 then //If we're on target, actually allocate and load it.
    begin
    subholder.picture.SetSize(subholder.info.height*subholder.info.width); //Allocate memory for the image
    subholder.picture.position := 0; //Reset write position, just in case.
    if subholder.picture.Size <> 0 then
      subholder.picture.CopyFrom(inp,subholder.picture.Size); //Copy raw image data
    end
   else
    begin
    inp.Seek((subholder.info.height*subholder.info.width),soFromCurrent); //Just move ahead
    end;

 //if subholder.picture.Size <> 0 then
 // subholder.picture.CopyFrom(inp,subholder.picture.Size); //Copy raw image data
                                                  //(Debug) listbox1.Items.add(inttostr((holder.picture.size - (longint(chunksizes[chunkpos-1])-4))));
 //At the moment of writing, FSH files are 8-bit image data. If the image doesn't fill the whole chunk,
 //the rest is unused. This should skip beyond that.
 //Only FSH files have a chunk size at the moment.
 n3 := ((longint(chunksizes[chunkpos-1])-4) - {subholder.picture.size}(subholder.info.height*subholder.info.width));
 if n3 > 0 then inp.Seek(n3,soFromCurrent);
 end;

//At this point we have one TPatchSubFrame with an image
//Now to load it into the TBitmap we inherited from.
Pixelformat := pf8Bit;
Palette := CreatePalette(TLogPalette((@overpal)^)); //Set up the windows Palette from our memory cache
if subholder.info.format <> FORMAT_8BIT then
 begin
 OutputDebugString('Unsupported Frame Format!');
 end
else
 begin
 subholder.picture.position := 0;//Get ready to copy raw data
 width := subholder.info.width;
 Height := subholder.info.height;
 if (Height <> 0) or (Width <> 0) then
 for c1 := subholder.info.height-1 downto 0 do
  begin //Load raw data into the prepared bitmap image
  Line:=Scanline[c1];
  for c2 := 0 to subholder.info.width-1 do
   begin
   subholder.picture.Read(n2,sizeof(n2));
   line[c2] := n2;
   end;
  end;
 end;
finally
inp.free;
subholder.free;
chunksizes.free;
end;//Finally
end;

(*
procedure TPatchMain.LoadFOX(filename: string; destlist : TList; grid : TDrawGrid);
var
 inp : TFileStream; //FOX Input file
 numshapes: integer;
 b_u16 : word; //Size-named reader variables. xD
 b_32: longint;
 b_16: smallint;
 b_u32: longword;
 holder : TPatchFrame; //Structure for allocating memory
 subholder : TPatchSubFrame; //Maintains subframes
 verheader : string;
 complete : boolean;
 I, FramePos, StepPos: Integer;
begin
{$IFDEF LOGGINGMODE}
   LogText('===FOX LOAD OPERATION BEGINS===');
{$ENDIF}
complete := false;
holder := nil;
subholder := nil;
if destlist.count > MaxVisible then //Apparently it loses cell coordinates if you pull this when the scrollbar isn't there.
 Grid.Perform(WM_HScroll,SB_ThumbPosition,0);//Scroll back to home position
CleanImages(destlist, grid); //Clean up any existing data first
inp := Tfilestream.create(filename,fmOpenRead);
try
 setlength(verheader,RES_FOXHlen);
 inp.Read(PChar(verheader)^,length(verheader));
 if verheader <> RES_FOXHeader then
  begin
  //Throw an error!
  Showmessage(MSG_FOX_ERR_Bad);
  exit;
  end;
inp.Read(b_32, sizeof(b_32)); //FOX Version
if b_32 <> RES_FOXVer then
 begin
  //Throw an error!
  Showmessage(MSG_FOX_ERR_Ver);
  exit;
 end;
inp.Read(b_32, sizeof(b_32)); //Number of image blocks
numshapes := b_32;
inp.Read(b_32, sizeof(b_32)); //FOX Type
inp.Read(b_32, sizeof(b_32)); //Encoding
if b_32 <> ENC_NONE then
 begin
 Showmessage(MSG_FOX_ERR_Enc);
 exit;
 end;
inp.Read(b_32, sizeof(b_32)); //Reserved 1
inp.Read(b_32, sizeof(b_32)); //Reserved 2
for I := 1 to numshapes do
 begin //Load FOX ImageBlock
 holder := TPatchFrame.Create;
 complete := false;
 inp.Read(b_u16, sizeof(b_u16));  //Object Flags
 holder.info.fbjflags := b_u16;
 inp.Read(b_16, sizeof(b_16)); //Replacement Index
 holder.info.fs2index := b_16;
 inp.Read(b_u16, sizeof(b_u16)); //number of Frames
 holder.FrameCount := b_u16;
 inp.Read(b_u16, sizeof(b_u16)); //number of Steps
 holder.StepCount := b_u16;
 for FramePos := 1 to holder.Framecount do //Load Image frames
  begin
   inp.Read(b_16, sizeof(b_16)); //Shape Format
   try
   subholder := TPatchSubFrame.Create;
   subholder.info.format := b_16;
   inp.Read(b_u16, sizeof(b_u16)); //Shape Width
   SubHolder.info.width := b_u16;
   inp.Read(b_u16, sizeof(b_u16)); //Shape Height
   SubHolder.info.height := b_u16;
   inp.Read(b_16, sizeof(b_16)); //Shape PosX
   SubHolder.info.xpos := b_16;
   inp.Read(b_16, sizeof(b_16)); //Shape PosY
   SubHolder.info.ypos := b_16;
   inp.Read(b_16, sizeof(b_16)); //Shape FurrePosX
   SubHolder.info.fbjxpos := b_16;
   inp.Read(b_16, sizeof(b_16)); //Shape FurrePosY
   SubHolder.info.fbjypos := b_16;
   //Image data size
   inp.Read(b_u32, sizeof(b_u32)); //ImageData Size
   SubHolder.picture.SetSize(b_u32);
   Subholder.picture.Position := 0;
   if Subholder.picture.Size <> 0 then  //Don't even try if it's 0. Otherwise TMemoryStream explodes silently.
    Subholder.picture.CopyFrom(inp, subholder.picture.Size); //Load image data
   Holder.Frames.Add(Subholder);
   Subholder := nil;
   finally
    if subholder <> nil then
     begin
     subholder.free;
     outputdebugstring(pchar('Fatal loading error at: '+inttostr(I)));
     end;
    end;
   end;
 //Load Steps
 setLength(holder.Steps,Holder.stepCount);

 outputdebugstring(pchar('KITTERSPEAK STEPS: '+inttostr(length(holder.steps))+' - '+inttostr(holder.Stepcount)));

 for StepPos := 0 to Holder.stepcount-1 do
  begin
  inp.Read(b_16, sizeof(b_16));
  holder.steps[Steppos].ruleType := b_16;
  inp.Read(b_16, sizeof(b_16));
  holder.steps[Steppos].value := b_16;
  inp.Read(b_16, sizeof(b_16));
  holder.steps[Steppos].counterMax := b_16;
  end;
 //Finish
 destlist.Add(Holder);
 complete := true;
 end;
finally
  if not complete then
   begin
   //outputDebugString(pchar('Step value: '+inttostr(StepPos)));
   holder.free;
   showmessage(MSG_LoadError);
   end;
  inp.Free;
  if destlist.count > 0 then
  Grid.ColCount := destlist.count else
   grid.colcount := 1; //Set up viewer grid
 grid.col := 0;
 if destlist.count > MaxVisible then //Apparently it loses cell coordinates if you pull this when the scrollbar isn't there.
  Grid.Perform(WM_HScroll,SB_ThumbPosition,0);//Scroll back to home position
 Grid.OnClick(Grid); //Reset the grid
 Grid.Repaint;
 SetStatus; //Set all status displays
 end;

end;
*)
initialization
TPicture.RegisterFileFormat('fox', 'Furcadia FOX File', TFOXGraphic);
TPicture.RegisterFileFormat('fsh', 'Furcadia FSH File', TFSHGraphic);
finalization
TPicture.UnregisterGraphicClass(TFoxGraphic);
TPicture.UnregisterGraphicClass(TFshGraphic);
end.
