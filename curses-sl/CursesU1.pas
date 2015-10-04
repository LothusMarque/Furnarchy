unit CursesU1;
{$WARN UNIT_PLATFORM OFF} //FileCtrl, as this is a Windows-only application.
interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ComCtrls, StdCtrls, ExtCtrls, SupraINIv2, XPMan, ConfigForm1,
  FileCtrl, Inifiles, CursesUtils, CursesData, ShellAPI, Spin, ExtDlgs,
  FoxGraphic, Remapping, Menus, pngimage, CursesHTMLUtils, Specialpopup;

type
  TCursesMain = class(TForm)
    TabSet: TPageControl;
    DescTab: TTabSheet;
    AFKDescTab: TTabSheet;
    AfkWhisperTab: TTabSheet;
    SettingsTab: TTabSheet;
    AvSettings: TGroupBox;
    CB_Color: TComboBox;
    CB_ColorSeg: TComboBox;
    Label3: TLabel;
    Label4: TLabel;
    Label1: TLabel;
    Label2: TLabel;
    CB_Gender: TComboBox;
    CB_Species: TComboBox;
    GroupBox1: TGroupBox;
    CharacterList: TListBox;
    FileScanner: TFileListBox;
    StatusBar1: TStatusBar;
    DescLengthLabel: TLabel;
    DescMemo: TMemo;
    AFKDescLengthLabel: TLabel;
    AwayWhisperLabel: TLabel;
    AFKDescMemo: TMemo;
    AwayWhisperMemo: TMemo;
    GroupBox3: TGroupBox;
    GroupBox4: TGroupBox;
    GroupBox5: TGroupBox;
    NameEdit: TLabeledEdit;
    PassEdit: TLabeledEdit;
    SaveBtn: TButton;
    SaveAsBtn: TButton;
    PlayBtn: TButton;
    INISettingsBtn: TButton;
    GroupBox2: TGroupBox;
    PortPrevPanel: TPanel;
    PortDisplay: TImage;
    PrevAvaPanel: TPanel;
    AvaDisplay: TImage;
    DigoCB: TComboBox;
    Label6: TLabel;
    CCEdit: TLabeledEdit;
    SettingsSB: TScrollBox;
    Label5: TLabel;
    AFKTimerSpin: TSpinEdit;
    Label10: TLabel;
    Label7: TLabel;
    DisConnSpin: TSpinEdit;
    Label8: TLabel;
    Label9: TLabel;
    Label11: TLabel;
    AwayPortSpin: TSpinEdit;
    Label12: TLabel;
    ReturnPortSpin: TSpinEdit;
    Label13: TLabel;
    AutoResCB: TCheckBox;
    AutoResponse: TTabSheet;
    AutoResMemo: TMemo;
    GroupBox6: TGroupBox;
    AutoResLabel: TLabel;
    WOFurnarchy: TCheckBox;
    XPManifest1: TXPManifest;
    SaveDialog1: TSaveDialog;
    OpenPictureDialog1: TOpenPictureDialog;
    CustPortMenu: TPopupMenu;
    ChooseCustomPort1: TMenuItem;
    ClearCustomPort1: TMenuItem;
    FurnConfigBtn: TButton;
    LogoImg: TImage;
    LogoImg2: TImage;
    CharListPopup: TPopupMenu;
    Setasdefault1: TMenuItem;
    N1: TMenuItem;
    ShowCustomPort1: TMenuItem;
    MainBackground: TImage;
    HelpMemo1: TMemo;
    HelpMemo2: TMemo;
    HelpMemo3: TMemo;
    Button1: TButton;
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    Exit1: TMenuItem;
    N2: TMenuItem;
    Save1: TMenuItem;
    SaveAs1: TMenuItem;
    N3: TMenuItem;
    Play1: TMenuItem;
    RunCharacterCreation1: TMenuItem;
    Options1: TMenuItem;
    CursesConfiguration1: TMenuItem;
    FurnarchyConfiguration1: TMenuItem;
    N4: TMenuItem;
    WithoutFurnarchy1: TMenuItem;
    DropPopMenu: TPopupMenu;
    RunCharacterCreation2: TMenuItem;
    RandClothesBtn: TButton;
    RandomTypeMenu: TPopupMenu;
    RandomClothes1: TMenuItem;
    RandomColors1: TMenuItem;
    RandomEverything1: TMenuItem;
    DigoDropPop: TPopupMenu;
    N5: TMenuItem;
    Delete1: TMenuItem;
    SavedigoselectionThisINI1: TMenuItem;
    N6: TMenuItem;
    ClearALLDigoselections1: TMenuItem;
    AvaPrevBtn: TButton;
    AvaNextBtn: TButton;
    Button2: TButton;
    PopMsg1: TImage;
    procedure RunCharacterCreation2Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure WOFurnarchyClick(Sender: TObject);
    procedure ClearCustomPort1Click(Sender: TObject);
    procedure ShowCustomPort1Click(Sender: TObject);
    procedure ChooseCustomPort1Click(Sender: TObject);
    procedure FurnConfigBtnClick(Sender: TObject);
    procedure SetDefINIBtnClick(Sender: TObject);
    procedure DigoCBChange(Sender: TObject);
    procedure AutoResCBClick(Sender: TObject);
    procedure AFKTimerSpinChange(Sender: TObject);
    procedure SaveAsBtnClick(Sender: TObject);
    procedure AutoResMemoChange(Sender: TObject);
    procedure SaveBtnClick(Sender: TObject);
    procedure PlayBtnClick(Sender: TObject);
    procedure AwayWhisperMemoChange(Sender: TObject);
    procedure CCEditChange(Sender: TObject);
    procedure AFKDescMemoChange(Sender: TObject);
    procedure DescMemoChange(Sender: TObject);
    procedure CharacterListClick(Sender: TObject);
    procedure INISettingsBtnClick(Sender: TObject);
    procedure CB_GenderChange(Sender: TObject);
    procedure CB_SpeciesChange(Sender: TObject);
    procedure CB_ColorSegChange(Sender: TObject);
    procedure CB_ColorDrawItem(Control: TWinControl; Index: Integer;
      Rect: TRect; State: TOwnerDrawState);
    procedure FormDestroy(Sender: TObject);
    procedure CB_ColorChange(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure RandClothesBtnClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure LogoImg2Click(Sender: TObject);
    procedure CharacterListKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure RandomClothes1Click(Sender: TObject);
    procedure Delete1Click(Sender: TObject);
    procedure ClearALLDigoselections1Click(Sender: TObject);
    procedure AvaNextBtnClick(Sender: TObject);
    procedure Button2MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Button2Click(Sender: TObject);
    procedure AvaPrevBtnClick(Sender: TObject);
  private
    { Private declarations }
  public
    startup : boolean;
    modified : boolean;
    restoreWinpos : boolean;
    //Core Dataloaders
    inireader : TSupraIniV2;
    DigoInfo : TSupraInIV2;
    ColorMap : TSupraIniV2;
    //
    CustomData : TIniFile;
    //INI Listing
    inifiles : TStringList;
    inipaths : TStringList;
    workingpath : string;
    MainCfg : string;
    CurrentCharIni : string;
    CurrentIndex: integer;
    //Colorcode storage
    Character: TFurcAvatar;
    //
    avatarPositions : TStringList;
    avatarPosition : integer;
    //
    procedure populateDynamics;
    procedure LoadColors(dest: TComboBox; name: string);
    procedure LoadCharINI(name : string);
    procedure SaveCharINI(name : string);
    //procedure BuildCC(data : TFurcAvatar); //Build full colorcode
    procedure DecodeCC(data: string);
    function GetSelCodeNum: integer;
    procedure SetSelCodeNum(Data: integer);
    //
    procedure SetCustomDigo(digonum: integer);
    function GetCustomDigo(character : string): integer;
    procedure ClearCustomDigo(character : string);
    function GetCustomPort(character : string): string;
    procedure ClearCustomPort(character : string);
    procedure SetCustomPort(filename : string);
    procedure UpdateRemaps;
    procedure AdjustTransparency(data : TBitmap);
    //
    procedure LoadINIList;
    procedure SelectDefaultIni(choice : string = '');
    procedure SavePathList;
  end;

  procedure DeleteToRecycleBin(Filename:String);

var
  CursesMain: TCursesMain;

implementation

{$R *.dfm}

procedure DeleteToRecycleBin(Filename:String);
Var SHFileOpStruct:TSHFileOpStruct;
    P:String;
begin
  P:=Filename+#0;
  FillChar(SHFileOpStruct,SizeOf(TSHFileOpStruct),0);
  with SHFileOpStruct do
  begin
    Wnd:=0;
    wFunc:=FO_DELETE;
    pFrom:=Pchar(P);
    fFlags:=FOF_ALLOWUNDO;
  end;
  SHFileOperation(SHFileOpStruct);
end;

//Form
procedure TCursesMain.populateDynamics;
//Populates the list of dynamic avatars.
//Current version reads the dynamic avatar file in a very simple fashion.
var
 i : integer;
 loader : TStringList;
begin
I := 1;
if fileexists(g_GlobalDataRoot+RES_DynamicAvas+RES_DynList) then
 begin
 loader := TStringlist.Create;
  try
  loader.LoadFromFile(g_GlobalDataRoot+RES_DynamicAvas+RES_DynList);
  while I < loader.Count do
   begin
   if (length(loader[I]) > 0) and (loader[I][1] <> '0')  then
    DigoCB.Items.Add(format(RES_DynAva,[I]));
   I := I + 1;
   end;
  finally
  loader.Free;
  end;
 end;
end;
//
procedure TCursesMain.LoadColors(dest: TComboBox; name: string);
var
 tmp : TStringlist;
 c : integer;
begin
dest.Items.BeginUpdate;
tmp := TStringlist.create;
 try
 dest.Items.Clear;
 inireader.ReadSectionValues(name,tmp);
 for c := 0 to tmp.Count - 1 do
   begin
   dest.Items.addobject(tmp.Names[c],pointer(strtoint(tmp.Values[tmp.Names[c]])));
   end;
 finally
 tmp.free;
 end;
if dest.Items.Count > 0 then
 dest.ItemIndex := 0;
dest.Items.endupdate;
end;

procedure TCursesMain.SaveCharINI(name : string);
var
 io : TStringList;
begin
io := TStringList.Create;
  try
  if fileexists(name) then
    io.LoadFromFile(name) //Load/update current file
  else
    io.Add(RES_NewINIHeader); //Start a new one

  //Write Data!
  io.Values[KEY_Colors] := CCEdit.text;
  io.Values[KEY_Description] := stripReturns(DescMemo.Text);
  io.Values[KEY_AFKDescription] := stripReturns(AFKDescMemo.Text);
  io.Values[KEY_Name] := NameEdit.Text;
//  if Savepassword.checked then
   io.Values[KEY_Password] := PassEdit.Text;
  io.Values[KEY_AFKTime] := inttostr(AFKTimerSpin.Value);
  io.Values[KEY_AFKDisconnectTime] := inttostr(DisConnSpin.Value);
  io.Values[KEY_AutoResMessage] := stripReturns(AutoResMemo.Text);
  io.Values[KEY_AFKMessage] := stripReturns(AwayWhisperMemo.Text);
  if AutoResCB.Checked then
   io.Values[KEY_AutoResponse] := RES_Yes
  else
   io.Values[KEY_AutoResponse] := RES_No;
  io.Values[KEY_AFKPortrait] := inttostr(AwayPortSpin.Value);
  io.Values[KEY_DefaultPortrait] := inttostr(ReturnPortSpin.value);
  io.SaveToFile(name);
  modified := false;
  finally
  io.free;
  end;
end;


procedure TCursesMain.LoadCharINI(name : string);
var
 reader : TStringList;
begin
reader := TStringList.create;
 try
 reader.LoadFromFile(name);
 CCEdit.Text := reader.values[KEY_Colors];
 DescMemo.Text := reader.Values[KEY_Description];
 AFKDescMemo.Text := reader.Values[KEY_AFKDescription];
 NameEdit.Text := Reader.values[KEY_Name];
 PassEdit.Text := Reader.values[KEY_Password];
 AFKTimerSpin.Value := strtoint2(reader.Values[KEY_AFKTime]);
 DisConnSpin.Value := strtoint2(reader.Values[KEY_AFKDisconnectTime]);
 AutoResMemo.text := Reader.values[KEY_AutoResMessage];
 AwayWhisperMemo.Text := Reader.Values[KEY_AFKMessage];
 AutoResCB.Checked := (Reader.Values[KEY_AutoResponse] = '1');
 AwayPortSpin.Value := strtoint2(reader.Values[KEY_AFKPortrait]);
 ReturnPortSpin.Value := strtoint2(reader.Values[KEY_DefaultPortrait]);
 modified := false;
 finally
 reader.free;
 end;
end;

procedure TCursesMain.DecodeCC(data: string);
var
 charc : integer;
 tmp : integer;
begin
Startup := true;
fillchar(Character,sizeof(character),0); //NUKE IT
if length(data) > 0 then
if data[1] = 't' then
 for charc := 1 to length(data) do
  begin
  case charc of
   1:;
   2: Character.colors.Fur := GetSingleB220(data[charc]);
   3: Character.colors.Markings := GetSingleB220(data[charc]);
   4: Character.colors.Hair := GetSingleB220(data[charc]);
   5: Character.colors.Eye := GetSingleB220(data[charc]);
   6: Character.colors.Badge := GetSingleB220(data[charc]);
   7: Character.colors.Vest := GetSingleB220(data[charc]);
   8: Character.colors.Bracer := GetSingleB220(data[charc]);
   9: Character.colors.Cape := GetSingleB220(data[charc]);
   10: Character.colors.Boot := GetSingleB220(data[charc]);
   11: Character.colors.Trousers := GetSingleB220(data[charc]);
   12: Character.Gender := GetSingleB220(data[charc]);
   13: Character.Species := GetSingleB220(data[charc]);
   14: Character.specialavatar := GetSingleB220(data[charc]);
   end;
  end;
//if charc[1] = '' then

//CB_ColorSegChange(CB_Color);
tmp := GetSelCodeNum;
if tmp > CB_Color.items.count-1 then
 tmp := 0;
CB_Color.ItemIndex := tmp;

if (Character.Gender < CB_Gender.Items.Count)  and (Character.Gender > -1) then
 CB_Gender.ItemIndex := Character.Gender
 else
  CB_Gender.ItemIndex := 0;
if (Character.Species < CB_Species.Items.Count) and (Character.Species > -1) then
 CB_Species.ItemIndex := Character.Species
 else
  CB_Species.ItemIndex := 0;
startup := false;
end;

procedure TCursesMain.Delete1Click(Sender: TObject);
begin
if CurrentCharIni <> '' then
 DeleteToRecycleBin(CurrentCharIni);
LoadINIList;
SelectDefaultINI();
end;

procedure TCursesMain.DescMemoChange(Sender: TObject);
begin
DescLengthLabel.Caption := inttostr(length(DescMemo.text))+' of '+inttostr(DescMemo.MaxLength)+' characters.';
if DescMemo.Modified then
 modified := true;
end;

procedure TCursesMain.DigoCBChange(Sender: TObject);
begin
//showmessage(DigoCB.Text);
UpdateRemaps;
end;

procedure TCursesMain.Exit1Click(Sender: TObject);
begin
close;
end;

function TCursesMain.GetSelCodeNum: integer;
begin
result := 0;
case CB_ColorSeg.ItemIndex of
 0: result := Character.colors.Fur;
 1: result := Character.colors.Markings;
 2: result := Character.colors.Hair;
 3: result := Character.colors.Eye;
 4: result := Character.colors.Badge;
 5: result := Character.colors.Vest;
 6: result := Character.colors.Bracer;
 7: result := Character.colors.Cape;
 8: result := Character.colors.Boot;
 9: result := Character.colors.Trousers;
 end;
end;

procedure TCursesMain.SetDefINIBtnClick(Sender: TObject);
var
 data : TIniFile;
 //tmp : string;
begin
//Select current default INI, alternatively select item 0.
if (CharacterList.Items.count > 0) and (CharacterList.ItemHeight <> -1) then
  begin
  data := TIniFile.Create(g_ClientDataRoot+RES_SettingsINI);
    try
    //CurrentCharINI := inifiles[integer(items.Objects[itemindex])];
    data.WriteString(SEC_General, KEY_DefaultINI, CurrentCharIni);
    finally
    data.Free;
    end;
  end;
end;

procedure TCursesMain.SetSelCodeNum(Data: integer);
begin
case CB_ColorSeg.ItemIndex of
 0: Character.colors.Fur := data;
 1: Character.colors.Markings := data;
 2: Character.colors.Hair := data;
 3: Character.colors.Eye := data;
 4: Character.colors.Badge := data;
 5: Character.colors.Vest := data;
 6: Character.colors.Bracer := data;
 7: Character.colors.Cape := data;
 8: Character.colors.Boot := data;
 9: Character.colors.Trousers := data;
 end;
end;

procedure TCursesMain.ShowCustomPort1Click(Sender: TObject);
begin
UpdateRemaps;
end;

{$HINTS OFF} //fBitmapPalEntries is tossed
procedure TCursesMain.AdjustTransparency(data : TBitmap);
var
 fPal: PLogPalette;
 fBitmapPalEntries: Cardinal;
begin
GetMem( fPal, Sizeof( TLogPalette ) + Sizeof( TPaletteEntry ) * 1 );
 try
  fPal.palversion := $300;
  fPal.palnumentries := 1;
  fBitmapPalEntries := GetPaletteEntries( data.Palette, 0, 1, fPal.palPalEntry[ 0 ] );
  data.Canvas.Brush.Color := fPal.PalPalEntry[ 0 ].PeBlue shl 16
              + fPal.PalPalEntry[ 0 ].PeGreen shl 8
              + fPal.PalPalEntry[ 0 ].PeRed;
  //outputDebugString(pchar('Color: '+inttostr(data.Canvas.Brush.Color)));
   data.Height := data.Height+1;
 data.Canvas.FloodFill(0,data.Height-1,data.Canvas.Pixels[0,data.Height-1],fsSurface);
 finally
 freeMem(fpal);
 end;
end;
{$HINTS ON}

procedure TCursesMain.UpdateRemaps;
var
 tmp : string;
 tbitm{, bit2 }: TBitmap;

begin
tmp := '';
if showcustomport1.checked then
 tmp := GetCustomPort(CurrentCharIni);

if tmp = '' then
 tmp := GetPortraitFile(digoinfo,Character.Species, Character.Gender,DigoCB.Text);
//PortPrevPanel.Caption := tmp;
 tbitm := TBitmap.Create;
 try
 GetFurcImg(TBitm,tmp);
 Remap(ColorMap,tBitm,CCEdit.text);
 PortDisplay.Picture.Assign(TBitm);
 finally
 tbitm.Free;
 end;
tmp := GetAvatarFile(digoinfo,Character.Species, Character.Gender, DigoCB.Text);
//outputDebugString(pchar('tmp: '+tmp));
avatarpositions.CommaText := tmp;
//avatarposition := 1;
//PrevAvaPanel.Caption := tmp;
 tbitm := TBitmap.Create;
 //bit2 := TBitmap.Create;
 try
 if avatarPositions.Count > 1 then
  GetFurcImg(TBitm,{tmp}AvatarPositions[0]+','+Avatarpositions[avatarposition])
 else GetFurcImg(TBitm,tmp); //Fall back on classic error handling
 Remap(ColorMap,tBitm,CCEdit.text);

 AdjustTransparency(tBitm);
 //Tbitm.Canvas.Brush.Color := Tbitm.Palette;
 AvaDisplay.Picture.Assign(TBitm);
 finally
 tbitm.Free;
// bit2.Free;
 end;
PortDisplay.Repaint;
AvaDisplay.Repaint;
end;

procedure TCursesMain.WOFurnarchyClick(Sender: TObject);
begin
if not startup then
 begin
 if sender is TCheckBox then
  with sender as TCheckbox do
   begin
   CustomData.WriteBool(SEC_Main, KEY_WithOFurn, checked);
   WithoutFurnarchy1.Checked := checked;
   end;
 if sender is TMenuItem then
 with sender as TMenuItem do
  begin
  CustomData.WriteBool(SEC_Main, KEY_WithOFurn, checked);
  WOFurnarchy.Checked := checked;
  end;
 end;
end;

function TCursesMain.GetCustomDigo(character : string): integer;
begin
result := CustomData.ReadInteger(SEC_CDigos, character, -1);
end;

procedure TCursesMain.SetCustomDigo(digonum: integer);
begin
if CurrentCharIni <> '' then
  CustomData.WriteInteger(SEC_Ports, CurrentCharIni, digonum)
 else
  Application.MessageBox(pchar(ERR_IniSave), pchar(TTL_Error), MB_OK or MB_ICONINFORMATION);
end;

procedure TCursesMain.ClearCustomDigo(character : string);
begin
CustomData.DeleteKey(SEC_CDigos, character);
end;

function TCursesMain.GetCustomPort(character : string): string;
begin
result := CustomData.ReadString(SEC_Ports, character, '')
end;

procedure TCursesMain.ClearALLDigoselections1Click(Sender: TObject);
begin
if Application.MessageBox(pchar(RES_ClearDigoSelections),pchar(TTL_Query), MB_OKCANCEL or MB_ICONWARNING) = IDOK then
 CustomData.EraseSection(SEC_CDigos);
end;

procedure TCursesMain.ClearCustomPort(character : string);
begin
CustomData.DeleteKey(SEC_Ports, character);
end;

procedure TCursesMain.SetCustomPort(filename : string);
begin
if CurrentCharIni <> '' then
  CustomData.WriteString(SEC_Ports, CurrentCharIni, filename)
 else
  Application.MessageBox(pchar(ERR_IniSave), pchar(TTL_Error), MB_OK or MB_ICONINFORMATION);
end;

procedure TCursesMain.LoadINIList;
var
  I, Posi: Integer;
  treader: Tstringlist;

procedure AddFile(item : string);
begin
if fileexists(item) then
 begin
 treader.LoadFromFile(item);
 if treader.Count > 0 then
  if (treader[0] = RES_Header2) or (treader[0] = RES_Header3) or
   (treader[0] = RES_Header1_6) then
   begin
   inifiles.Add(item);
   //Characterlist.items.AddObject(StringReplace(treader.Values['Name'],'|',' ',[rfReplaceAll]),pointer(inifiles.count-1));
   Characterlist.items.AddObject(ALUTF8XMLTextElementDecode(StringReplace(treader.Values['Name'],'|',' ',[rfReplaceAll])),pointer(inifiles.count-1));
   end;
 end;
end;

begin
inifiles.clear;
Characterlist.Items.Clear;
treader := TStringList.Create;
try
for I := 0 to inipaths.count - 1 do
 begin
 if DirectoryExists(inipaths[I]) then
  begin
  FileScanner.Directory := inipaths[I];
  Outputdebugstring(pchar(inipaths[I]));
  for posi := 0 to fileScanner.Items.Count - 1 do
   begin
   AddFile(inipaths[I]+FileScanner.Items[posi]);
   //Outputdebugstring(pchar(FileScanner.Items[posi]));
   end;
  end;
 end;
finally
treader.Free;
end;
end;

procedure TCursesMain.LogoImg2Click(Sender: TObject);
{var
 Frm: TSpecialFrm;
 png: TPngImage;
 Dataloader : TResourceStream; }
begin
{frm := TSpecialFrm.Create(self);
try
 png := TPngImage.Create;//TPngObject.Create;
 DataLoader := TResourceStream.Create(hinstance,pchar(DAT_UC),pchar(DATG_Art));
 try
 png.LoadFromStream(DataLoader);
 frm.Image1.Picture.Assign(png);
 finally
 png.Free;
 dataloader.free;
 end;
 frm.ClientWidth := frm.Image1.Width;
 frm.ClientHeight := frm.Image1.Height;
 frm.Left := self.Left + (self.width div 2) - (frm.Width div 2);
 frm.Top := self.Top + (self.Height div 2) - (frm.Height div 2);
 frm.ShowModal;
finally
 frm.Free;
 end;   }
end;

procedure TCursesMain.SelectDefaultIni(choice : string = '');
var
 data : TIniFile;
 tmp : string;
 I : integer;
begin
//Select current default INI, alternatively select item 0.
if CharacterList.Items.count > 0 then
  begin
  data := TIniFile.Create(g_ClientDataRoot+RES_SettingsINI);
    try
    tmp := choice;
    if tmp = '' then
     tmp := data.ReadString(SEC_General,KEY_DefaultINI,RES_NoData);
    if tmp <> '' then
     begin
     tmp := lowercase(tmp);
     for I := 0 to CharacterList.items.count - 1 do
      begin
      //OutPutDebugString(pchar('Scanning: '+IniFiles[integer(CharacterList.items.Objects[i])]));
      if tmp = lowercase(IniFiles[integer(CharacterList.items.Objects[i])]) then
       begin
       CharacterList.ItemIndex := I;
       CharacterListClick(CharacterList);
       exit;
       end;
      end;
     end;
    //Select 0 - This will never be executed if a match is made above.
    CharacterList.ItemIndex := 0;
    CharacterListClick(CharacterList)
    finally
    data.Free;
    end;
  end
else
 begin
 //If there aren't any to select, we sort of sit there on default/Nothing.
 CB_ColorSegChange(CB_Color);
 CCEdit.text := BuildCC(Character);
 UpdateRemaps;
 end;

end;

procedure TCursesMain.SavePathList;
var
 ifile : TIniFile;
  I: Integer;
begin
ifile := TIniFile.Create(MainCFG);
 try
 ifile.EraseSection(SEC_Folders);
 for I := 0 to inipaths.Count - 1 do
  begin
  ifile.WriteString(SEC_Folders,inttostr(i),inipaths[i]);
  end;
 finally
 ifile.free;
 end;

end;

procedure TCursesMain.AFKDescMemoChange(Sender: TObject);
begin
AFKDescLengthLabel.Caption := inttostr(length(AFKDescMemo.text))+' of '+inttostr(AFKDescMemo.MaxLength)+' characters.';
if AFKDescMemo.Modified then
 modified := true;
end;

procedure TCursesMain.AFKTimerSpinChange(Sender: TObject);
begin
modified := true;
end;

procedure TCursesMain.AutoResCBClick(Sender: TObject);
begin
modified := true;
end;

procedure TCursesMain.AutoResMemoChange(Sender: TObject);
begin
AutoResLabel.Caption := inttostr(length(AutoResMemo.text))+' of '+inttostr(AutoResMemo.MaxLength)+' characters.';
if AutoResMemo.Modified then
 modified := true;
end;

procedure TCursesMain.AvaNextBtnClick(Sender: TObject);
var
 tbitm : TBitmap;
begin
//Pick the next available avatar mode frame and load/display it.
avatarPosition := avatarPosition + 1;
if avatarPosition > AvatarPositions.Count-1 then
 avatarPosition := 1;
if avatarPosition > AvatarPositions.Count-1 then
 exit; //Um. Normally we should have at least two here.
tbitm := TBitmap.Create;
 try
 GetFurcImg(TBitm,{tmp}AvatarPositions[0]+','+Avatarpositions[avatarPosition]);
 Remap(ColorMap,tBitm,CCEdit.text);
 AdjustTransparency(tBitm);
 AvaDisplay.Picture.Assign(TBitm);
 finally
 tbitm.Free;
 end;
//
end;

procedure TCursesMain.AvaPrevBtnClick(Sender: TObject);
var
 tbitm : TBitmap;
begin
//Pick the next available avatar mode frame and load/display it.
avatarPosition := avatarPosition - 1;
if avatarPosition < 1 then
 avatarPosition := AvatarPositions.Count-1;
if avatarposition < 1 then
 exit; //houston, we have a problem!
tbitm := TBitmap.Create;
 try
 GetFurcImg(TBitm,{tmp}AvatarPositions[0]+','+Avatarpositions[avatarPosition]);
 Remap(ColorMap,tBitm,CCEdit.text);
 AdjustTransparency(tBitm);
 AvaDisplay.Picture.Assign(TBitm);
 finally
 tbitm.Free;
 end;
end;

procedure TCursesMain.AwayWhisperMemoChange(Sender: TObject);
begin
AwayWhisperLabel.Caption := inttostr(length(AwayWhisperMemo.text))+' of '+inttostr(AwayWhisperMemo.MaxLength)+' characters.';
if AwayWhisperMemo.Modified then
 modified := true;
end;

procedure TCursesMain.Button1Click(Sender: TObject);
begin
DropPopMenu.Popup(mouse.CursorPos.X, mouse.CursorPos.Y);
end;

procedure TCursesMain.Button2Click(Sender: TObject);
var
 t : TPoint;
begin
//CustPortMenu.Popup(mouse.CursorPos.X,mouse.CursorPos.Y);
if sender is TButton then
with sender as TButton do
  begin
  t.X := Left + (Width DIV 2);
  t.Y := {Top + }(Height DIV 2);

  t := clienttoscreen(t);
  CustPortMenu.Popup(t.X,t.Y);
  end;
end;

procedure TCursesMain.Button2MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
 t : TPoint;
begin
with sender as TButton do
  begin
{t.X := X;
t.Y := Y;
//t.X := Left + (Width DIV 2);
//t.Y := Top + (Height DIV 2);

  t := clienttoscreen(t); }
  //CustPortMenu.Popup(t.X,t.Y);
  //CustPortMenu.Popup(mouse.CursorPos.X,mouse.CursorPos.Y);
  end;
end;

procedure TCursesMain.SaveBtnClick(Sender: TObject);
begin
CCEdit.text := BuildCC(Character);
if CurrentCharINI <> '' then
 SaveCharINI(CurrentCharINI)
else
 SaveAsBtnClick(SaveAsBtn);
end;

procedure TCursesMain.SaveAsBtnClick(Sender: TObject);
begin
with SaveDialog1 do
 if execute then
  begin
  SaveCharINI(filename);
  LoadINIList;
  SelectDefaultINI(filename);
  end;
end;

procedure TCursesMain.PlayBtnClick(Sender: TObject);
var
 tmp : string;
 workdir: string;
 t1 : integer;
begin
//Currently only
if CharacterList.ItemIndex <> -1 then
 begin
 if modified then
   begin
   t1 := application.MessageBox(pchar(RES_SaveChanges),pchar(TTL_Query), MB_YESNOCANCEL or MB_ICONWARNING);
   case t1 of
    IDYES: begin
           if CurrentCharINI <> '' then
             SaveCharINI(CurrentCharINI)
           else
             SaveAsBtnClick(SaveAsBtn);
           end;
    IDCANCEL: begin
              exit;
              end;
    end;
   end;
 if WOFurnarchy.Checked then
   begin
  tmp := g_FurcFolder+RES_FurcEXE;
  workdir := g_FurcFolder;
  end
 else
   begin
   tmp := extractfilepath(paramstr(0))+RES_LaunchEXE;
   workdir := extractfilepath(paramstr(0));
   end;
 ShellExecute(application.Handle,nil,pchar(tmp),
   pchar('"'+inifiles[integer(CharacterList.Items.Objects[CharacterList.itemindex])]+'"'),pchar(workdir),SW_SHOWNORMAL);
 close;
 end
else
 begin
 Application.MessageBox(pchar(ERR_NoINISelected),pchar(TTL_Error),MB_OK or MB_ICONERROR);
 end;
end;

procedure TCursesMain.RandClothesBtnClick(Sender: TObject);
begin
//Randomize Mode
if RandomClothes1.Checked then CCEdit.Text := RandomizeClothes(ColorMap,CCEdit.Text);
if RandomColors1.Checked then CCEdit.Text := RandomCC(ColorMap)+Copy(CCEdit.Text,12,length(ccedit.Text));
if RandomEverything1.Checked then CCEdit.Text := RandomizeSpecies(Colormap, RandomizeGender(Colormap, RandomCC(Colormap)))+'#';
modified := true;
end;

procedure TCursesMain.RandomClothes1Click(Sender: TObject);
begin
//
if sender is TMenuItem then
 with sender as TMenuitem do
  checked := true;
end;

procedure TCursesMain.RunCharacterCreation2Click(Sender: TObject);
var
 tmp : string;
 workdir : string;
begin
///
///
if WOFurnarchy.Checked then
 begin
 tmp := g_FurcFolder+RES_FurcEXE;
 workdir := g_FurcFolder;
 end
else
  begin
  tmp := extractfilepath(paramstr(0))+RES_LaunchEXE;
  workdir := extractfilepath(paramstr(0));
  end;
 ShellExecute(application.Handle,nil,pchar(tmp),
   pchar('-create'),pchar(workdir),SW_SHOWNORMAL); //pchar(g_FurcFolder)
 close;
end;

procedure TCursesMain.INISettingsBtnClick(Sender: TObject);
var
 tmp : TCFGForm;
begin
tmp := TCFGForm.Create(self);
 try
 tmp.WinPosSaveChk.Checked := RestoreWinPos;
 tmp.Pathlistbox.items.addStrings(inipaths);
 tmp.Left := (CursesMain.Width div 2) - (tmp.Width div 2) + CursesMain.Left;
 tmp.Top := (CursesMain.Height div 2) - (tmp.Height div 2) + CursesMain.Top;
 tmp.ShowModal;
 if tmp.pressedok then
  begin
  inipaths.Clear;
  inipaths.Sorted := true;
  inipaths.Duplicates := dupIgnore;
  inipaths.AddStrings(tmp.PathListBox.Items);
  inipaths.Sorted := false;
  //Application.MessageBox('Pressed ok!','Info', MB_OK or MB_ICONINFORMATION);
  SavePathList;
  RestoreWinPos := tmp.WinPosSaveChk.Checked;
  CustomData.WriteBool(SEC_Main, KEY_RestoreWinPos, restoreWinPos);
  LoadIniList;
  SelectDefaultIni;
  end;
 finally
 tmp.Free;
 end;
end;

procedure TCursesMain.CB_ColorChange(Sender: TObject);
begin
if not startup then
 begin
 SetSelCodeNum(TComboBox(sender).ItemIndex);
 CCEdit.text := BuildCC(Character);
 UpdateRemaps;
 modified := true;
 //DoRemap(CCEdit.Text);
 end;
end;

procedure TCursesMain.CB_ColorDrawItem(Control: TWinControl; Index: Integer;
  Rect: TRect; State: TOwnerDrawState);
var
 tmp : TColor;
 lum : real;
begin
with (control as TComboBox).canvas do
 begin
 tmp := brush.color;
 Brush.color := TColor(TComboBox(control).Items.Objects[index]);
 Fillrect(rect);
 lum := (0.30 * GetRvalue(brush.color)) + (0.59 * GetGvalue(brush.color)) + (0.11 * GetBvalue(brush.color));
 if lum > 150 then
  font.color := clblack
 else
  font.color := clWhite;
 rect.Left := rect.Left + 1;
 DrawText(Handle, pChar((Control as TComboBox).Items[index]), -1, Rect, DT_LEFT);
 brush.Color := tmp;
 end;
end;

procedure TCursesMain.CB_ColorSegChange(Sender: TObject);
var
 tmp : integer;
begin
//startup := true;
CB_Color.items.clear;
LoadColors(CB_Color, CB_ColorSeg.Text);
tmp := GetSelCodeNum;
if tmp > CB_Color.items.count-1 then
 tmp := 0;
CB_Color.ItemIndex := tmp;
//startup := false;
end;

procedure TCursesMain.CB_GenderChange(Sender: TObject);
begin
character.Gender := CB_Gender.ItemIndex;
CCEdit.text := BuildCC(Character);
UpdateRemaps;
modified := true;
end;

procedure TCursesMain.CB_SpeciesChange(Sender: TObject);
begin
character.Species := CB_Species.ItemIndex;
CCEdit.text := BuildCC(Character);
UpdateRemaps;
modified := true;
end;

procedure TCursesMain.CCEditChange(Sender: TObject);
begin
if not startup then
 begin
 DecodeCC(CCEdit.Text);
 UpdateRemaps;
 end;
end;

procedure TCursesMain.CharacterListClick(Sender: TObject);
var
 t1 : integer;
begin
if sender is TListBox then
 with sender as TListBox do
  begin
  if modified then
   begin
   t1 := application.MessageBox(pchar(RES_SaveChanges),pchar(TTL_Query), MB_YESNOCANCEL or MB_ICONWARNING);
   case t1 of
    IDYES: begin
           if CurrentCharINI <> '' then
             SaveCharINI(CurrentCharINI)
           else
             SaveAsBtnClick(SaveAsBtn);
           end;
    IDCANCEL: begin
              itemindex := currentindex;
              exit;
              end;
    end;
   end;
  if itemindex <> -1 then
   begin
   CurrentIndex := itemindex;
   StatusBar1.Panels[0].Text := MinimizePathName(StatusBar1.Handle,inifiles[integer(items.Objects[itemindex])]);
   CurrentCharINI := inifiles[integer(items.Objects[itemindex])];
   LoadCharINI(CurrentCharINI);
   DecodeCC(CCEdit.Text);
   UpdateRemaps;
   if SavedigoselectionThisINI1.Checked then
    begin
    t1 := GetCustomDigo(CurrentCharIni);
    if t1 <> -1 then
     if t1 < DigoCB.Items.Count then
      DigoCB.ItemIndex := t1;
    end;
   end;
{  else
   begin
   PlayBtn.Enabled := false;
   end; }
  end;
end;

procedure TCursesMain.CharacterListKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
if key = VK_RETURN then
 PlayBtnClick(sender);
end;

procedure TCursesMain.ChooseCustomPort1Click(Sender: TObject);
begin
with OpenPictureDialog1 do
 if execute then
  begin
  SetCustomPort(filename+',0');
  ShowCustomPort1.Checked := true;
  UpdateRemaps;
  end;
end;

procedure TCursesMain.ClearCustomPort1Click(Sender: TObject);
begin
ClearCustomPort(CurrentCharIni);
UpdateRemaps;
end;

procedure TCursesMain.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
var
 t1 : integer;
begin
if modified then
   begin
   t1 := application.MessageBox(pchar(RES_SaveChanges),pchar(TTL_Query), MB_YESNOCANCEL or MB_ICONWARNING);
   case t1 of
    IDYES: begin
           if CurrentCharINI <> '' then
             SaveCharINI(CurrentCharINI)
           else
             SaveAsBtnClick(SaveAsBtn);
           end;
    IDCANCEL: begin
              canclose := false;
              exit;
              end;
    end;
   end;
end;

procedure TCursesMain.FormCreate(Sender: TObject);
//Let's load up all the data!
var
 dataloader: TStream;// : TResourceStream;
 cfgload : TIniFile;
 newfile : boolean;
 tmp : string;
 cnt : integer;
 tmplist : TStringList;
 lastdigo : integer;
 //png : TPngObject;
 png: TPngImage;
 T, L : integer;
 tmpstart : boolean;
begin
startup := true;
//Self.ClientHeight := 480;

avatarPositions := TStringList.Create;
avatarPosition := 1;
tmpstart := not FileExists(extractfilepath(paramstr(0))+RES_LaunchEXE);
inipaths := TStringList.Create;
 try
 ForceDirectories(GetWorkingPath);
 except
 on e: Exception do
  OutputDebugString(pchar(e.Message)); //Not good but wtf...
 end;
newfile := not fileexists(GetWorkingPath+RES_CursesINI);
//showmessage(GetWorkingPath+RES_CursesINI);
MainCFG := GetWorkingPath+RES_CursesINI;
  cfgload := TIniFile.Create(MainCFG);
   try
   RestoreWinPos := cfgload.ReadBool(SEC_Main, KEY_RestoreWinPos, false);
   if RestoreWinPos then
    begin
    T := cfgload.ReadInteger(SEC_Main, KEY_WinTop, -1);
    L := cfgload.ReadInteger(SEC_Main, KEY_WinLeft, -1);
    if (t <> -1) and (L <> -1) then
     begin
     Position := poDesigned;
     left := L;
     top := T;
     end;
    end;
   WOFurnarchy.Checked := cfgload.ReadBool(SEC_Main, KEY_WithOFurn, tmpstart);
   WithoutFurnarchy1.Checked := WOFurnarchy.Checked;
   case cfgLoad.ReadInteger(SEC_Main, KEY_RandomMode, DEF_RandomMode) of
    0: RandomClothes1.Checked := true;
    1: RandomColors1.Checked := true;
    2: RandomEverything1.Checked := true;
   end;

   SavedigoselectionThisINI1.Checked := cfgload.ReadBool(SEC_Main, KEY_DigoSelect, DEF_Digoselect);

   //Load this thing here for later.
   lastdigo := cfgload.ReadInteger(SEC_Main, KEY_LastDigo, 0);

   if newfile then //Populate the defaults
    begin
    cfgload.WriteString(SEC_Folders, '0', g_ClientDocRoot+RES_FurcadiaCharacters);
    inipaths.Add(g_ClientDocRoot+RES_FurcadiaCharacters);
    cfgload.WriteString(SEC_Folders, '1', g_FurcFolder);
    inipaths.Add(GetFurcadiaFolder);
    end
   else
    begin
    //showmessage('Loading...');
    cnt := 0;
    tmp := cfgload.ReadString(SEC_Folders,inttostr(cnt),RES_NoData);
    while tmp <> RES_NoData do
     begin
     inipaths.Add(includetrailingpathdelimiter(tmp));
     cnt := cnt + 1;
     tmp := cfgload.ReadString(SEC_Folders,inttostr(cnt),RES_NoData);
     end;
    end;
   if inipaths.Count > 0 then
    begin
    SaveDialog1.InitialDir := inipaths[0];
    openpicturedialog1.InitialDir := inipaths[0];
    end;
   finally
   cfgload.free;
   end;

inifiles := TStringList.Create;


//Logo
png := TPngImage.Create;//TPngObject.Create;
DataLoader := TResourceStream.Create(hinstance,pchar(DAT_Logo),pchar(DATG_Art));
 try
 png.LoadFromStream(DataLoader);
 LogoImg.Picture.Assign(png);
 finally
 png.Free;
 dataloader.free;
 end;
//Secondary graphic
png := TPngImage.Create;
DataLoader := TResourceStream.Create(hinstance,pchar(DAT_Logo2),pchar(DATG_Art));
 try
 png.LoadFromStream(DataLoader);
 LogoImg2.Picture.Assign(png);
 finally
 png.Free;
 dataloader.free;
 end;
//Hidden Image
{png := TPngImage.Create;
DataLoader := TResourceStream.Create(hinstance,pchar(DAT_ImgMessage),pchar(DATG_Art));
 try
 png.LoadFromStream(DataLoader);
 PopMsg1.Picture.Assign(png);
 finally
 png.Free;
 dataloader.free;
 end;  }

{png := TPngObject.Create;
 try
 png.LoadFromFile(extractFilePath(paramstr(0))+'testbg.png');
 MainBackground.Picture.Assign(png);
 finally
 png.Free;
 end;  }

//Color-Selector mapping info
inireader := TSupraIniv2.Create('');
 DataLoader := TResourceStream.Create(hinstance,SelData,RT_RCData);
 try
 inireader.LoadFromStream(dataloader);
 finally
 dataloader.Free;
 end;

//Color-Remapping Info
ColorMap := TSupraIniv2.Create('');
 DataLoader := TResourceStream.Create(hinstance,DATA_ColorMap,RT_RCData);
 try
 ColorMap.LoadFromStream(dataloader);
 finally
 dataloader.Free;
 end;

//Help Data
DataLoader := TResourceStream.Create(hinstance, RES_TagHelpText, RT_RCData);
tmplist := TStringList.Create;
  try
  tmplist.LoadFromStream(dataloader);
  HelpMemo1.Text := tmplist.Text;
  HelpMemo2.Text := tmplist.text;
  HelpMemo3.Text := tmplist.text;
  finally
  dataloader.free;
  tmplist.Free;
  end;

//Load Digo Information
if fileexists(extractfilepath(paramstr(0))+RES_DigoDataFile) then
  DataLoader := TFileStream.Create(extractfilepath(paramstr(0))+RES_DigoDataFile, fmOpenRead)
 else
  DataLoader := TResourceStream.Create(hinstance, RES_DigoData, RT_RCData);
DigoInfo := TSupraINIV2.Create('');
 try
 DigoInfo.LoadFromStream(Dataloader);
// Memo1.Lines.AddStrings(DigoInfo.MainData);
 DigoCB.items.clear;
 DigoInfo.ReadSection(SEC_Digos,DigoCB.Items);

 //Dynamic Avatars
 populateDynamics();

 //

 if DigoCB.Items.count > 0 then
  DigoCB.ItemIndex := 0;
 //Finally, now we have a chance to set the combobox's default.
 if (lastdigo > -1) and (lastdigo < DigoCB.Items.Count) then
     DigoCB.ItemIndex := lastdigo;

 finally
 DataLoader.free;
 end;

//Custom Data
CustomData := TIniFile.Create(MainCfg);

//Ini List
LoadIniList;
SelectDefaultIni;

//Final finishing touches
Tabset.ActivePageIndex := 0;
SettingsSB.VertScrollBar.Position := 0;

CB_ColorSegChange(CB_Color);

startup := false;
end;

procedure TCursesMain.FormDestroy(Sender: TObject);
var
 t1 : integer;
begin
t1 := 0;
if windowstate = wsNormal then //If somehow it's not normal, we don't want to save.
 begin
 if RandomClothes1.Checked then t1 := 0;
 if RandomColors1.Checked then t1 := 1;
 if RandomEverything1.Checked then t1 := 2;
 CustomData.WriteInteger(SEC_Main, KEY_WinTop, top);
 CustomData.WriteInteger(SEC_Main, KEY_WinLeft, left);
 CustomData.WriteInteger(SEC_Main, KEY_RandomMode, t1);
 //CustomData.WriteBool(SEC_Main, KEY_DigoSelect, SavedigoselectionThisINI1.Checked);
 CustomData.WriteInteger(SEC_Main, KEY_LastDigo, DigoCB.ItemIndex);
 end;
//Trash all our temporary data handles
inireader.Free;
inipaths.free;
inifiles.free;
DigoInfo.Free;
ColorMap.Free;
CustomData.Free;
avatarPositions.Free;
end;

procedure TCursesMain.FormShow(Sender: TObject);
begin
CharacterList.SetFocus;
end;

procedure TCursesMain.FurnConfigBtnClick(Sender: TObject);
begin
ShellExecute(application.Handle,nil,pchar(extractfilepath(paramstr(0))+RES_F2CFGEXE),
   '',pchar(extractfilepath(paramstr(0))),SW_SHOWNORMAL);
end;

end.
