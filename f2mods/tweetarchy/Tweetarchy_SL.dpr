program Tweetarchy_SL;



{%File '@todo.txt'}
{$R 'imports.res' 'imports.rc'}
{%File 'import_credits.txt'}

uses
  Forms,
  XPMan,
  TweetFUnit1 in 'TweetFUnit1.pas' {Twit},
  TwitterResources in 'TwitterResources.pas',
  ConfigFormUnit in 'ConfigFormUnit.pas' {TwitCfgFrm},
  RegExpr in 'RegExpr.pas',
  OAuth in 'OAuth.pas';

{$R *.res}

begin
  //ReportMemoryLeaksOnShutdown := true;
  Application.Initialize;
  Application.CreateForm(TTwit, Twit);
  Application.Run;
end.
