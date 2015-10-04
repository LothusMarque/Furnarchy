unit ConfigForm1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Menus, CursesUtils, shlobj, ActiveX, ExtCtrls, CursesData;

type
  TCFGForm = class(TForm)
    GroupBox1: TGroupBox;
    PathListBox: TListBox;
    Label1: TLabel;
    PopupMenu1: TPopupMenu;
    AddFolder1: TMenuItem;
    Remove1: TMenuItem;
    Panel1: TPanel;
    Button1: TButton;
    Button2: TButton;
    Panel2: TPanel;
    Label2: TLabel;
    Panel3: TPanel;
    WinPosSaveChk: TCheckBox;
    procedure FormCreate(Sender: TObject);
    procedure AddFolder1Click(Sender: TObject);
    procedure Remove1Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
  private
    { Private declarations }
  public
    pressedok : boolean;
  end;

var
  CFGForm: TCFGForm;

implementation

{$R *.dfm}

procedure TCFGForm.AddFolder1Click(Sender: TObject);
var
 browseinfo: TBrowseInfo;
 pidl: PItemIDList;
 buf: Array[0..MAX_PATH] of Char;
 result : string;
begin
fillchar( browseinfo, sizeof(browseinfo), 0 );
browseinfo.hwndOwner := Handle;
browseinfo.lpszTitle := 'Select folder';
browseinfo.ulFlags  := BIF_NEWDIALOGSTYLE or BIF_VALIDATE or BIF_RETURNONLYFSDIRS; //BIF_RETURNONLYFSDIRS or
browseinfo.lpfn     := BrowserCallback;
pidl := ShBrowseForFolder( browseinfo );
If Assigned( pidl ) Then Begin
 ShGetPathfromIDList( pidl, buf );
 CoTaskMemFree(pidl);
 result := buf;
 PathListBox.Items.Add(IncludeTrailingPathDelimiter(result));
 end;
end;

procedure TCFGForm.Button1Click(Sender: TObject);
begin
pressedok := true;
close;
end;

procedure TCFGForm.Button2Click(Sender: TObject);
begin
pressedok := false;
close;
end;

procedure TCFGForm.FormCreate(Sender: TObject);
begin
Label2.caption := Format('Curses Standalone V%s ©%s Mercenary Enclave Productions && Entwined Studios', [CURSES_Version, CURSES_Date]);
end;

procedure TCFGForm.Remove1Click(Sender: TObject);
begin
if PathListBox.ItemIndex <> -1 then
 PathListBox.Items.Delete(PathListBox.ItemIndex);
end;

initialization 
  OleInitialize(nil);
finalization 
  OleUninitialize;
end.
