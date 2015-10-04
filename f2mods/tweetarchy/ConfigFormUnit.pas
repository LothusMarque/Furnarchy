unit ConfigFormUnit;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, Spin, MMSystem, ShellApi, TwitterResources;

type
  TTwitCfgFrm = class(TForm)
    OKBtn: TButton;
    CancelBtn: TButton;
    GroupBox1: TGroupBox;
    AccountEdit: TLabeledEdit;
    PassEdit: TLabeledEdit;
    GroupBox2: TGroupBox;
    Label1: TLabel;
    CheckTimerEdit: TSpinEdit;
    RefreshChk: TCheckBox;
    StartChk: TCheckBox;
    CredPanel: TPanel;
    GroupBox3: TGroupBox;
    NewTweetEdit: TEdit;
    NewTweetSndChk: TCheckBox;
    NewTweetBrseBtn: TButton;
    NewTweetDefBtn: TButton;
    Label2: TLabel;
    OpenDialog1: TOpenDialog;
    Button1: TButton;
    CreditsTimer: TTimer;
    RecsPanel: TPanel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    Label13: TLabel;
    Button2: TButton;
    Button3: TButton;
    BufDisplayChk: TCheckBox;
    procedure Button3Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Label13Click(Sender: TObject);
    procedure CreditsTimerTimer(Sender: TObject);
    procedure CredPanelClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    Credits : TStringList;
    CredURL : string;
    CredPos : integer;
  public

   Snd1Def : string;
  end;

var
  TwitCfgFrm: TTwitCfgFrm;

implementation

{$R *.dfm}

procedure TTwitCfgFrm.Button1Click(Sender: TObject);
begin
PlaySound(pchar(NewTweetEdit.Text),0,SND_Filename or SND_ASYNC);
end;

procedure TTwitCfgFrm.Button2Click(Sender: TObject);
begin
Recspanel.Visible := false;
end;

procedure TTwitCfgFrm.Button3Click(Sender: TObject);
begin
RecsPanel.Align := alClient;
RecsPanel.visible := true;
end;

procedure TTwitCfgFrm.CreditsTimerTimer(Sender: TObject);
begin
CredPanel.Caption := Credits[CredPos];
CredURL := Credits[CredPos+1];
if CredURL <> '' then
 begin
 CredPanel.Cursor := crHandPoint;
 CredPanel.Font.Color := CLHotLight;
 CredPanel.Font.Style := [fsUnderline];
 end
else
 begin
 CredPanel.Cursor := crDefault;
 CredPanel.Font.Color := CLWindowText;
 CredPanel.Font.Style := [];
 end;
CredPos := CredPos + 2;
if CredPos > Credits.Count-1 then
 CredPos := 0;
end;

procedure TTwitCfgFrm.CredPanelClick(Sender: TObject);
begin
if CredURL <> '' then
 ShellExecute(0, 'open', pchar(CredURL), nil, nil, SW_SHOWNORMAL); 
end;

procedure TTwitCfgFrm.FormCreate(Sender: TObject);
var
 loader : TResourceStream;
begin
Credits := TStringList.Create;
CredPos := 0;
loader := TResourceStream.Create(hinstance, 'CREDS', rt_RCdata);
 try
 Credits.LoadFromStream(loader);
 if odd(credits.Count) then
  Credits.Delete(credits.count-1); //Trim to avoid error messages.
 CreditsTimer.Enabled := true;
 CreditsTimerTimer(self);
 finally
 loader.Free;
 end;
end;

procedure TTwitCfgFrm.FormDestroy(Sender: TObject);
begin
Credits.free;
end;

procedure TTwitCfgFrm.Label13Click(Sender: TObject);
var
 t1 : string;
begin
t1 := TLabel(sender).Caption;
ShellExecute(0, 'open', pchar(t1), nil, nil, SW_SHOWNORMAL);
end;

end.
