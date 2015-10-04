program Curses;

{$R 'resourcepack.res' 'resourcepack.rc'}
{$R 'palette.res' 'palette.rc'}

uses
  Forms,
  CursesU1 in 'CursesU1.pas' {CursesMain},
  ConfigForm1 in 'ConfigForm1.pas' {CFGForm},
  CursesUtils in 'CursesUtils.pas',
  CursesData in 'CursesData.pas',
  Remapping in 'Remapping.pas',
  FoxGraphic in 'FoxGraphic.pas',
  CursesHTMLUtils in 'CursesHTMLUtils.pas',
  Specialpopup in 'Specialpopup.pas' {SpecialFrm};

{$R *.res}

begin
  //ReportMemoryLeaksOnShutdown := DebugHook <> 0;
  Randomize;
  Application.Initialize;
  application.HintPause := 1000;
  Application.CreateForm(TCursesMain, CursesMain);
  Application.CreateForm(TCFGForm, CFGForm);
  Application.Run;
end.
