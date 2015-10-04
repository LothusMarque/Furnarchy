unit TweetFUnit1;

interface

uses
  {$IFDEF MODULEMODE}Furn2,{$ENDIF}Windows, Messages, SysUtils, Variants, Classes, Graphics, DateUtils, Controls, Forms,
  Dialogs, StdCtrls, IdBaseComponent, IdComponent, IdTCPConnection, IdTCPClient,
  IdHTTP, xmldom, XMLIntf, msxmldom, XMLDoc, TwitterResources,
  ExtCtrls, ComCtrls, ConfigFormUnit, Inifiles, Inetdate2, Menus, mmsystem, Clipbrd, RegExpr;

type
  TTwit = class(TForm)
    Button1: TButton;
    Memo1: TMemo;
    Button2: TButton;
    XMLDocument1: TXMLDocument;
    EntriesList: TListBox;
    DetailsMemo: TMemo;
    Button3: TButton;
    Memo2: TMemo;
    Button4: TButton;
    UpdateBtn: TButton;
    PostMemo: TMemo;
    StatusBar1: TStatusBar;
    CfgBtn: TButton;
    Button5: TButton;
    CheckTimer: TTimer;
    RefreshBtn: TButton;
    TweetMenu: TPopupMenu;
    Reply1: TMenuItem;
    Direct1: TMenuItem;
    ReTweet1: TMenuItem;
    Copy1: TMenuItem;
    procedure Copy1Click(Sender: TObject);
    procedure ReTweet1Click(Sender: TObject);
    procedure Direct1Click(Sender: TObject);
    procedure Reply1Click(Sender: TObject);
    procedure EntriesListContextPopup(Sender: TObject; MousePos: TPoint;
      var Handled: Boolean);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure PostMemoKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure RefreshBtnClick(Sender: TObject);
    procedure CheckTimerTimer(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure UpdateBtnClick(Sender: TObject);
    procedure CfgBtnClick(Sender: TObject);
    procedure EntriesListDrawItem(Control: TWinControl; Index: Integer;
      Rect: TRect; State: TOwnerDrawState);
    procedure EntriesListMeasureItem(Control: TWinControl; Index: Integer;
      var Height: Integer);
    procedure Button4Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure EntriesListClick(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    procedure DoOnWakeMainThread(Sender: TObject);
    procedure WMclSynchronize(var Message: TMessage);
      message WM_CLSYNCHRONIZE;
  public
    password : string;
    username : string;
    tick : integer;
    mainclock : integer;
    clockmins : integer;
    autorefresh : boolean;
    startcheck : boolean;
    showbuffer: boolean;
    NTaudible : boolean;
    NTFile : string;
    NTFileDef : string;
    sounded : boolean;
  //
    Config : TIniFile;
    TweetSeenList : TStringList;
    Tweets : TStringList;
    gets : integer;
    posts : integer;
  //
    procedure StartClock;
    procedure StopClock;
    procedure DoGetTweets(init : boolean);
    procedure DoPostData(data : string);
    procedure SetStatus(text : string);
    procedure TrimSeen;
    procedure TrimDisplay;
    procedure ClearEntriesList;
    procedure ParseSingle(data: string);
    procedure ParseTweets(data: string; initial: boolean);
    procedure NotifyTweet(text, name : string);
  //
    procedure SortList;
    procedure HandleHTTPCode(data : integer);
  end;

 TCommThread = class(TThread)
  private
   status : string;
   code : integer;
   //haltit : boolean;
   procedure SetTheStatus; //Synchro
   procedure SetStatus(stat : string; code : integer);
  protected
  public
   user : string;
   pass : string;
  end;

 TGetHomeThread = class(TCommThread)
  private
   procedure ExpData; //Synchro
  protected
    procedure Execute; override;
  public
   init : boolean;
   data : string;
  end;

 TNewTweetThread = class(TCommThread)
  private
   procedure FinishPost;
  protected
   procedure Execute; override;
  public
   Post : string;
   data : string;
  end;

var
  Twit: TTwit;
  Receiver : TGetHomeThread;
  {$IFDEF MODULEMODE}
  g_h_client_wnd : HWND;
  g_mod_index: index_t;
  g_enabled: bool;
  inifile : string;
  {$ENDIF}

implementation

{$R *.dfm}

function SortByDate(l: TStringList; i1, i2: Integer): Integer;
begin
SortByDate := -CompareDateTime(TTweetData(l.Objects[i1]).Post.timestamp, TTweetData(l.Objects[i2]).Post.timestamp);
end;


{TRecvThread }
procedure TCommThread.SetTheStatus;
var
 tstat : string;
begin
tstat := status;
//Twit.CheckTimer.Enabled := false;
if code <> 0 then
  begin
  case code of
      401 : tstat := STAT_Err + STAT_NoAuth;
      403 : tstat := STAT_Err + STAT_UpdateLimitHit; //Limit hit
      404 : tstat := STAT_Err + STAT_APIGone; //Wtf, API gone?
   end;
  Twit.HandleHTTPCode(code);
  end;
Twit.SetStatus(tstat);
end;

procedure TCommThread.SetStatus(stat : string; code : integer);
begin
status := stat;
self.code := code;
Synchronize(SetTheStatus);
end;

procedure TGetHomeThread.ExpData;
begin
Twit.gets := twit.gets - 1;
//Twit.Memo1.Text := data;
Twit.ParseTweets(data,init);
end;

procedure TGetHomeThread.Execute;
var
 http : TidHTTP;
 buffer : TMemoryStream;
 completed: boolean;
 texbuf : TStringlist;
begin
 texbuf := TStringList.Create;
 http := TidHttp.Create;
 try
 completed := false;
 SetStatus(STAT_CheckingTweets,0);
 buffer := TMemoryStream.Create;
  try
  Http.Request.Clear;
  Http.HTTPOptions := [hoInProcessAuth,hoForceEncodeParams];
  http.ConnectTimeout := 60000;
  Http.Request.Username := user;
  Http.Request.Password := pass;
  Http.Request.UserAgent := RES_UA;
   try
   HTTP.Get(URL_Timeline,buffer);
   //showmessage(inttostr(buffer.Size)+idhttp1.LastCmdResult.DisplayName);
   Buffer.Position := 0;
   TexBuf.LoadFromStream(buffer);
   data := TexBuf.Text;

   synchronize(ExpData);  //SYNC

   completed := true;
   except
    on E: EIdHTTPProtocolException do
     begin
     SetStatus(STAT_Err+E.Message,E.ErrorCode);
     end;
   end;
  finally
  buffer.Free;
  texbuf.Free;
  end;
 if completed then
  SetStatus(STAT_DoneChecking,0);
 finally
 http.Free;
 end;
Terminate;
end;

//TNewTweetThread
procedure TNewTweetThread.FinishPost;
begin
Twit.posts := twit.posts - 1;
Twit.ParseSingle(data);
end;

procedure TNewTweetThread.Execute;
var
 outgoing : TStringList;
 buffer : TMemoryStream;
 texbuf : TStringlist;
 completed: boolean;
 http : TIdHTTP;
begin
http := TidHttp.Create;
 try
completed := false;
SetStatus(STAT_Posting,0);
buffer := TMemoryStream.Create;
outgoing := TStringList.Create;
texbuf := TStringList.create;

//f2_speak(pchar(post));

 try
 Http.Request.Clear;
 Http.Request.Username := user;
 Http.Request.Password := pass;
 Http.HTTPOptions := [hoInProcessAuth,hoForceEncodeParams];
 http.ConnectTimeout := 60000;
 Http.Request.UserAgent := RES_UA;
  try
 Outgoing.Add('status='+post);
 Http.Post(RES_URL_Base+RES_URL_Update, Outgoing, buffer);
 //showmessage(inttostr(buffer.Size)+idhttp1.LastCmdResult.DisplayName);
 Buffer.Position := 0;
 Texbuf.LoadFromStream(buffer); //Should become internal TStringList
 //Debug
 data := texbuf.Text;
 //Debug
 completed := true;
  except
   on E: EIdHTTPProtocolException do
     begin
       SetStatus(STAT_Err+E.Message, E.ErrorCode);
     end;
  end;
 if completed then
  begin
  Synchronize(FinishPost); //SYNC
  SetStatus(STAT_Done,0);
  end;
 finally
 texbuf.free;
 buffer.Free;
 Outgoing.Free;
 end;
 finally
 http.Free;
 end;
end;

procedure TTwit.Button1Click(Sender: TObject);
begin
DoGetTweets(true);
end;

procedure TTwit.NotifyTweet(text, name : string);
var
 R : TRegExpr;
 tmp : string;
begin
r := TRegExpr.Create;
try
 tmp := name+': '+text;
 r.Expression := RGEX_Linkify;
 tmp := r.Replace(tmp, REP_Linkify, true);
{$IFNDEF MODULEMODE}
if showBuffer then
 Memo2.Lines.Add(MSG_Newtweet+tmp);    // MSG_Newtweet+name+': '+text
{$ENDIF}
{$IFDEF MODULEMODE}
 if showBuffer then
  f2_speak(pchar(MSG_Newtweet+tmp));
{$ENDIF}
 if (NTAudible) and (not sounded) then
  begin
  PlaySound(pchar(NTFile),0,SND_Filename or SND_ASYNC);
  end;
 finally
 r.Free;
end;
end;

procedure TTwit.SortList;
var 
  sl: TStringList;
  t : integer;
begin
 LockWindowUpdate(EntriesList.Handle);
 t := EntriesList.ItemIndex;
  EntriesList.Sorted := False;
  sl := TStringList.Create;
  try
  Entrieslist.Items.BeginUpdate;
    sl.Assign(EntriesList.Items);
    sl.CustomSort(SortByDate);
    EntriesList.Items.Assign(sl);
    EntriesList.ItemIndex := t;
  finally
    EntriesList.Items.EndUpdate;
    sl.Free; 
    LockWindowUpdate(0); 
  end;
//showmessage('Sortlist Finished');
end;

procedure TTwit.SetStatus(text : string);
begin
Statusbar1.Panels[0].Text := text;
end;

procedure TTwit.Direct1Click(Sender: TObject);
begin
PostMemo.Text := 'D '+TTweetData(EntriesList.items.Objects[EntriesList.itemindex]).user.screen_name +' '+PostMemo.Text;
end;

procedure TTwit.DoGetTweets(init : boolean);
var
 rec : TGetHomeThread;
begin
if (username <> '') and (password <> '') then
 begin
 rec := TGetHomeThread.Create(true);
 gets := gets + 1;
 rec.user := username;
 rec.pass := password;
 rec.FreeOnTerminate := true;
 rec.init := init;
 rec.Resume;
 end;
end;

procedure TTwit.DoPostData(data : string);
var
 thr : TNewTweetThread;
begin
if (username <> '') and (password <> '') then
 begin
 //f2_speak('Started Post Thread function');
 
 thr := TNewTweetThread.Create(true);
 posts := posts + 1;
 thr.user := username;
 thr.pass := password;
 thr.FreeOnTerminate := true;
 thr.Post := data;
 thr.Resume;
 end;
end;

procedure TTwit.StartClock;
begin
CheckTimer.Enabled := true;
MainClock := 0;
end;

procedure TTwit.StopClock;
begin
CheckTimer.Enabled := false;
MainClock := 0;
end;

procedure TTwit.ParseSingle(data: string);
var
 tmppost: TTweetData;
 completed : boolean;
 t1 : boolean;
begin
 try
XMLDocument1.XML.Text := data;
completed := false;
 try
 XMLDocument1.Active := true; //Execute and parse

  with XMLDocument1.DocumentElement do
   begin
   if NodeName = 'status' then
    begin
    tmppost := TTweetData.create;
     //Post data
     tmppost.Post.text := ChildNodes.FindNode('text').Text;
     tmppost.Post.created_at := ChildNodes.FindNode('created_at').Text;
     tmppost.Post.timestamp := TwitterDateToDateTime(tmppost.Post.created_at);
     tmppost.Post.id := ChildNodes.FindNode('id').Text;

     tmppost.Post.source := ChildNodes.FindNode('source').Text;
     tmppost.Post.truncated := (ChildNodes.FindNode('truncated').Text = 'true');
     tmppost.Post.favorited := (ChildNodes.FindNode('favorited').Text = 'true');
     //Posting User Data
     with ChildNodes.FindNode('user') do
      begin
      tmppost.user.id := ChildNodes.FindNode('id').Text;
      tmppost.user.name := ChildNodes.FindNode('name').Text;
      tmppost.user.screen_name := ChildNodes.FindNode('screen_name').Text;;
      tmppost.user.location := ChildNodes.FindNode('location').Text;//ChildValues['location'];   //problems if empty
      tmppost.user.description := ChildNodes.FindNode('description').Text;
      tmppost.user.profile_image_url := ChildNodes.FindNode('profile_image_url').Text;
      tmppost.user.url := ChildNodes.FindNode('url').Text;
      tmppost.user.uprotected := (ChildNodes.FindNode('protected').Text = 'true');
      end;
 t1 := (TweetSeenList.IndexOf(tmppost.Post.id) = -1);

 if t1 then
  EntriesList.Items.InsertObject(0, FormatTweet(tmppost),tmppost);
    TweetSeenList.Add(tmppost.Post.id);
   if not t1 then
    FreeAndNil(tmppost);
    end;
   end;
 completed := true;
 finally
 XMLDocument1.Active := false; //Okay, we're done with it.
 if not completed then
  tmppost.Free; //Free the last working copy if something went wrong
 TrimDisplay;
 TrimSeen;
 end;
 except
  on E : exception do
  {$IFNDEF MODULEMODE}
 Memo2.Lines.Add('[TW] Internal exception: '+e.Message);
{$ENDIF}
{$IFDEF MODULEMODE}
  f2_errlog(pchar('[TW] Internal exception: '+e.Message));
{$ENDIF}

 end;
end;

procedure TTwit.ParseTweets(data: string; initial: boolean);
var
 i: Integer;
 tmppost: TTweetData;
 completed : boolean;
 t1 : boolean;
begin
try
sounded := false;
XMLDocument1.XML.Text := data;
completed := false;
if initial then ClearEntriesList;
 try
 XMLDocument1.Active := true; //Execute and parse
 for i := XMLDocument1.DocumentElement.ChildNodes.Count-1 downto 0 do
  begin
  with XMLDocument1.DocumentElement do
   begin
   if ChildNodes[i].NodeName = 'status' then
    begin
    tmppost := TTweetData.create;
    with childnodes[i]{.ChildNodes} do
     begin
     //Post data
     tmppost.Post.text := ChildNodes.FindNode('text').Text;
     tmppost.Post.created_at := ChildNodes.FindNode('created_at').Text;
     tmppost.Post.timestamp := TwitterDateToDateTime(tmppost.Post.created_at);
     tmppost.Post.id := ChildNodes.FindNode('id').Text;

     tmppost.Post.source := ChildNodes.FindNode('source').Text;
     tmppost.Post.truncated := (ChildNodes.FindNode('truncated').Text = 'true');
     tmppost.Post.favorited := (ChildNodes.FindNode('favorited').Text = 'true');
     //Posting User Data
     with ChildNodes.FindNode('user') do
      begin
      tmppost.user.id := ChildNodes.FindNode('id').Text;
      tmppost.user.name := ChildNodes.FindNode('name').Text;
      tmppost.user.screen_name := ChildNodes.FindNode('screen_name').Text;;
      tmppost.user.location := ChildNodes.FindNode('location').Text;//ChildValues['location'];   //problems if empty
      tmppost.user.description := ChildNodes.FindNode('description').Text;
      tmppost.user.profile_image_url := ChildNodes.FindNode('profile_image_url').Text;
      tmppost.user.url := ChildNodes.FindNode('url').Text;
      tmppost.user.uprotected := (ChildNodes.FindNode('protected').Text = 'true');
      end;
     end;
   { EntriesList.Items.AddObject(childnodes[i].ChildNodes.FindNode('text').Text +
     #10' ~ ' + childnodes[i].ChildNodes.FindNode('user').ChildNodes.FindNode('screen_name').Text,tmppost);
 }
 //EntriesList.Items.InsertObject();
 t1 := (TweetSeenList.IndexOf(tmppost.Post.id) = -1);

 if t1 then
   EntriesList.Items.InsertObject(0, FormatTweet(tmppost), tmppost);

    if (not initial) and (t1) then
     begin
     TweetSeenList.Add(tmppost.Post.id);
     NotifyTweet(tmppost.Post.text, tmppost.user.screen_name);
     sounded := true;
     end;
   TweetSeenList.Add(tmppost.Post.id);
   if not t1 then
    FreeAndNil(tmppost);
    end;
   end;
  end;
 completed := true;
 finally
 XMLDocument1.Active := false; //Okay, we're done with it.
 if not completed then
  tmppost.Free; //Free the last working copy if something went wrong
 TrimDisplay;
 TrimSeen;
 end;
except
on E : exception do
  {$IFNDEF MODULEMODE}
 Memo2.Lines.Add('[TW] Internal exception: '+e.Message);
{$ENDIF}
{$IFDEF MODULEMODE}
  f2_errlog(pchar('[TW] Internal exception: '+e.Message));
{$ENDIF}
end;
end;

procedure TTwit.PostMemoKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
if key = VK_RETURN then
 UpdateBtnClick(UpdateBtn);
end;

procedure TTwit.RefreshBtnClick(Sender: TObject);
begin
 if (username <> '') and (password <> '') then
  begin
  DoGetTweets(false);
  if (autorefresh) and (not CheckTimer.Enabled) then
   CheckTimer.Enabled := true; //Restart it.
  end
 else
  SetStatus(STAT_NeedAccount)
end;

procedure TTwit.Reply1Click(Sender: TObject);
begin
PostMemo.Text := '@'+TTweetData(EntriesList.items.Objects[EntriesList.itemindex]).user.screen_name +' '+PostMemo.Text;
end;

procedure TTwit.ReTweet1Click(Sender: TObject);
begin
with TTweetData(EntriesList.items.Objects[EntriesList.itemindex]) do
 begin
 PostMemo.Text := 'RT @'+user.screen_name+' '+Post.text;
 end;
end;

procedure TTwit.Button2Click(Sender: TObject);
begin
ParseTweets(Memo1.text, true);
end;

procedure TTwit.Button3Click(Sender: TObject);
begin
DoGetTweets(false);
end;

procedure TTwit.Button4Click(Sender: TObject);
begin
ParseTweets(Memo1.text, false);
end;

procedure TTwit.Button5Click(Sender: TObject);
begin
ParseSingle(memo1.text);
end;

procedure TTwit.EntriesListClick(Sender: TObject);
var
 tmp : TTweetData;
begin
if sender is TListBox then
 with sender as TListBox do
  begin
  if itemindex <> -1 then
   begin
   tmp := TTweetData(items.Objects[itemindex]);
   DetailsMemo.Lines.Clear;
   DetailsMemo.Lines.Add(datetimetostr(tmp.Post.timestamp));
   //DetailsMemo.Lines.add('');
   DetailsMemo.lines.Add(tmp.Post.text);
   DetailsMemo.Lines.Add(' ~ '+tmp.user.name + ' (' + tmp.user.screen_name + ')');
   //DetailsMemo.SelStart := 0;
   DetailsMemo.Perform(EM_SetSel, 0, 0);
   DetailsMemo.Perform(EM_LineScroll, 0, DetailsMemo.Lines.Count * -1);

   //DetailsMemo.Lines.Add(tmp.user.description);
   //DetailsMemo.Lines.Add(tmp.user.url);
   end;
  end;
end;

procedure TTwit.EntriesListContextPopup(Sender: TObject; MousePos: TPoint;
  var Handled: Boolean);
begin
if sender is TListbox then
 with sender as TListbox do
  begin
  Reply1.Enabled := itemindex <> -1;
  Direct1.enabled := Reply1.Enabled;
  ReTweet1.enabled := Reply1.enabled;
  Copy1.Enabled := Reply1.enabled;
  if itemindex <> -1 then
   begin
   
   end;
  end;
end;

procedure TTwit.EntriesListDrawItem(Control: TWinControl; Index: Integer;
  Rect: TRect; State: TOwnerDrawState);
var
  ItemString: string;
begin
  TListBox(Control).Canvas.FillRect(Rect);
  ItemString := TListBox(Control).Items.Strings[Index];
  Rect.Left := Rect.left + 2;
  Rect.Right := Rect.right - 2;
  DrawText(TListBox(Control).Canvas.Handle, PChar(ItemString), - 1, Rect,
   DT_WORDBREAK or DT_LEFT or DT_TOP or DT_NOPREFIX);
end;

procedure TTwit.EntriesListMeasureItem(Control: TWinControl; Index: Integer;
  var Height: Integer);
var
  ItemString: string;
  MyRect: TRect;
  //MyImage: TImage;
  MyCombo: TListBox;
begin
  // Don't waste time with this on Index = -1
  if (Index > -1) then
  begin
    MyCombo := TListBox(Control);
    // Create a temporary canvas to calculate the height
   // MyImage := TImage.Create(MyCombo);
    try
      MyRect := MyCombo.ClientRect;
      MyRect.Left := MyRect.left + 2;
      MyRect.Right := MyRect.right - 2;
      
      ItemString := MyCombo.Items.Strings[Index];
     // MyImage.Canvas.Font := MyCombo.Font;

      // Calc. using this ComboBox's font size
      Height := DrawText({MyImage}MyCombo.Canvas.Handle, PChar(ItemString+#10), //KLUDGE ALERT
        - 1, MyRect, DT_CALCRECT or DT_WORDBREAK or DT_LEFT or DT_TOP or DT_NOPREFIX or DT_NOCLIP);
      height := MyRect.Bottom - MyRect.Top + 8;
      //Height := height+8;
    finally
     // MyImage.Free;
    end;
  end;
end;

procedure TTwit.TrimSeen;
begin
while TweetSeenList.Count > DEF_ReadCacheCount do
 TweetSeenList.Delete(0);
end;

procedure TTwit.UpdateBtnClick(Sender: TObject);
begin
DoPostData(PostMemo.text);
PostMemo.Text := '';
end;

procedure TTwit.CheckTimerTimer(Sender: TObject);
begin
MainClock := MainClock + TTimer(sender).Interval div 1000;
//SetStatus(inttostr(MainClock));
if MainClock > (ClockMins * 60) then
 begin
 MainClock := 0;
 if (username <> '') and (password <> '') and (autorefresh) then
  DoGetTweets(false);
 end;
//SetStatus(inttostr(posts)+':'+inttostr(gets));
end;

procedure TTwit.TrimDisplay;
begin
while EntriesList.items.Count > DEF_DisplayCount do
 begin
 EntriesList.items.objects[EntriesList.items.Count-1].free;
 EntriesList.items.Delete(EntriesList.items.Count-1);
 end;
SortList;
end;

procedure TTwit.CfgBtnClick(Sender: TObject);
var
 cfg: TTwitCfgFrm;
begin
 cfg := TTwitCfgFrm.Create(self);
 try
 cfg.Left := (self.Left + (self.Width div 2)) - (cfg.Width div 2);
 cfg.Top := (self.Top + (self.Height div 2)) - (cfg.Height div 2);
 //Set up values
 cfg.PassEdit.Text := password;
 cfg.AccountEdit.Text := username;
 cfg.CheckTimerEdit.Value := clockmins;
 cfg.StartChk.Checked := startcheck;
 cfg.RefreshChk.Checked := autorefresh;
 cfg.BufDisplayChk.Checked := Showbuffer;

 //New Tweet Sound
 cfg.Snd1Def := NTFileDef;
 cfg.NewTweetEdit.Text := NTFile;
 cfg.NewTweetSndChk.Checked := NTaudible;
 //

 if cfg.ShowModal = MROK then
  begin
  //Retrieve values
  username := cfg.AccountEdit.Text;
  password := cfg.PassEdit.Text;
  clockmins := cfg.CheckTimerEdit.Value;
  startcheck := cfg.StartChk.checked;

  if AutoRefresh <> cfg.RefreshChk.Checked then
   CheckTimer.Enabled := cfg.RefreshChk.Checked;
  autorefresh := cfg.RefreshChk.Checked;
  showbuffer := cfg.BufDisplayChk.Checked;
  //New Tweet Sound
  ntaudible := cfg.NewTweetSndChk.Checked;
  NTFile := cfg.NewTweetEdit.Text;
 //

  //Save it!
  Config.WriteInteger(SEC_Main, KEY_RefTimer, clockmins);
  Config.WriteBool(SEC_Main, KEY_StartCheck, startcheck);
  Config.WriteBool(SEC_Main, KEY_AutoCheck, autorefresh);
  Config.WriteBool(SEC_Main, KEY_ShowBuffer, showbuffer);
  Config.WriteString(SEC_Main, KEY_PW, password);
  Config.WriteString(SEC_Main, KEY_User, username);
  Config.WriteBool(SEC_Main, KEY_NotifyNewTweet, ntaudible);
  Config.WriteString(SEC_Main, KEY_NTSoundFile, NTFile);
  //New Tweet Sound

 //
  end;
 finally
  cfg.Free;
 end;
end;

procedure TTwit.ClearEntriesList;
var
  I: Integer;
begin
for I := 0 to EntriesList.items.count - 1 do
 begin
 EntriesList.Items.Objects[i].Free;
 end;
 EntriesList.Items.Clear;
end;


procedure TTwit.Copy1Click(Sender: TObject);
begin
clipboard.SetTextBuf(pchar(FormatTweet(TTweetData(EntriesList.items.Objects[EntriesList.itemindex]))));
end;

procedure TTwit.FormClose(Sender: TObject; var Action: TCloseAction);
begin
{$IFDEF MODULEMODE}
f2_modenable(g_mod_index, false);
{$ENDIF}
end;

procedure TTwit.FormCreate(Sender: TObject);
begin
Classes.WakeMainThread := DoOnWakeMainThread;
gets := 0;
posts := 0;
Tweets := TStringList.Create;
TweetSeenList := TStringList.Create;
TweetSeenList.Duplicates := dupIgnore;
TweetSeenList.Sorted := true;
//
{$IFDEF MODULEMODE}
NTFileDef := includetrailingpathdelimiter(string(f2_modroot(g_mod_index)))+SND_NewTweet;
{$ELSE}
NTFileDef := ExtractFilePath(paramstr(0))+SND_NewTweet;
{$ENDIF}

{$IFDEF MODULEMODE}
{$ELSE}
self.ClientWidth := 390;
button1.Visible := true;
button2.visible := true;
button3.visible := true;
button4.visible := true;
button5.visible := true;
memo1.Visible := true;
memo2.visible := true;
{$ENDIF}

{$IFDEF MODULEMODE}
Config := TIniFile.Create(inifile);
{$ELSE}
Config := TIniFile.Create(ChangeFileExt(paramstr(0),'.ini'));
{$ENDIF}
password := config.ReadString(SEC_Main, KEY_PW, '');
username := config.ReadString(SEC_Main, KEY_User, '');
clockmins := config.ReadInteger(SEC_Main, KEY_RefTimer, 5);
autorefresh := config.ReadBool(SEC_Main, KEY_AutoCheck, true);
showbuffer := config.ReadBool(SEC_Main, KEY_ShowBuffer, true);

NTaudible := config.ReadBool(SEC_Main, KEY_NotifyNewTweet, true);

NTFile := config.readstring(SEC_Main, KEY_NTSoundFile, NTFileDef);

startcheck := config.ReadBool(SEC_Main, KEY_StartCheck, true);
//
if autorefresh then //start timer
 begin
 Mainclock := 0;
 CheckTimer.Enabled := true;
 end;
if (username <> '') and (password <> '') and (startcheck) then
 begin
 //Start update
 MainClock := 0;
 DoGetTweets(true);
 end;
end;

procedure TTwit.FormDestroy(Sender: TObject);
begin
ClearEntriesList;
TweetSeenList.Free;
Tweets.free;
Config.free;
end;

procedure TTwit.HandleHTTPCode(data : integer);
begin
case data of
      401, 404 : begin
                 CheckTimer.Enabled := false;  //Eep!
                 end;
   end;
end;

procedure TTwit.DoOnWakeMainThread(Sender: TObject);
begin
PostMessage(self.Handle, WM_CLSYNCHRONIZE, 0, 0);
end;

procedure TTwit.WMclSynchronize(var Message: TMessage);
begin
Classes.CheckSynchronize();
inherited;
end;


end.
