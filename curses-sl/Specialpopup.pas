unit Specialpopup;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls;

type
  TSpecialFrm = class(TForm)
    Image1: TImage;
    procedure Image1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  SpecialFrm: TSpecialFrm;

implementation

{$R *.dfm}

procedure TSpecialFrm.Image1Click(Sender: TObject);
begin
close;
end;

end.
