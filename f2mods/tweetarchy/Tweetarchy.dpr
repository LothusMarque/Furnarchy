library Tweetarchy;
{
"Tweetarchy", a Furnarchy2 Module
by Merc
}
{%File 'Readme.txt'}
{%File '@todo.txt'}
{$R 'imports.res' 'imports.rc'}
{%File 'import_credits.txt'}

uses
  Windows,
  Classes,
  Messages,
  SysUtils,
  Inifiles,
  Furn2 in 'Furn2.pas',
  ConfigFormUnit in 'ConfigFormUnit.pas' {TwitCfgFrm},
  TweetFUnit1 in 'TweetFUnit1.pas' {Twit},
  TwitterResources in 'TwitterResources.pas',
  RegExpr in 'RegExpr.pas',
  OAuth in 'OAuth.pas';

{$R *.res}

//Main
procedure InitMod;
var
 buffer : string;
begin
g_h_client_wnd := 0;
buffer := string(f2_moddocroot(g_mod_index));
inifile := buffer+'\'+RES_INIfile;
Twit := TTwit.Create(nil); //Create our main window

end;


procedure KillMod;
begin
Twit.Free; //Shut down and free all forms
end;



//Furnarchy2 Functions
function on_module(e: Module_event; param: pointer): longint; cdecl;
begin
case (e) of
 MOD_EVENT_START: begin
                  g_mod_index := PModuleInfo(param).module;
                  InitMod;
                  end;
 MOD_EVENT_TOUCH: begin
                  f2_speak(RES_About);
                  end;
 MOD_EVENT_STOP: Begin
                 KillMod;
                 end;
 MOD_EVENT_ENABLE: begin
                   Twit.Show;
                   g_enabled := true;
                   end;
 MOD_EVENT_DISABLE: begin
                    Twit.Hide;
                    g_enabled := false;
                    end;
 end;
result := 0;
end;

function on_client(e: Client_event; param: pointer): longint; cdecl;
begin
result := 0;
end;

function on_wndproc(params: PWNDPROCPARAMS): bool; cdecl;
begin
result := false;
if params.msg = WM_CLSYNCHRONIZE then
 begin
 //f2_Speak('Synchro in main window!');
 result := true;
 end;
end;

function on_world(e: World_event; param: pointer): longint; cdecl;
begin
result := 0;
end;

function on_command(argc: longint; const argv : array of PChar): bool; cdecl;
var
 t1 : string;
 work : string;
 counter : integer;
begin
if argc > 0 then
 begin
 t1 := uppercase(string(pchar(argv[0])));
 if t1 = CMD_Tweet then
  begin
  //f2_speak('[TW] Not yet implemented');
  result := true;
  if argc > 1 then
   begin
   for counter := 1 to argc-1 do
    begin
    if counter <> 1 then work := work + ' ';
    work := work + string(pchar(argv[counter]));
    end;

   if (twit.password <> '') and (twit.username <> '') then
    begin
    Twit.DoPostData(work);
    f2_Speak(pchar(MSG_PostAddedQueue));
    end
   else
    begin
    f2_speak(pchar(MSG_NeedLogin));
    end;
   exit;
   end
  else
   begin
   f2_speak(pchar(MSG_NeedContent));
   exit;
   end;
  exit;
  end;
 end;
result := false;
end;

procedure on_inet(pass: revision; line: pchar; from: longint); cdecl;
begin
end;

procedure on_onet(pass: revision; line: pchar; from: longint); cdecl;
begin
end;

exports
 on_module,
 on_client,
 on_wndproc,
 on_world,
 on_command,
 on_inet,
 on_onet;

begin

end.
