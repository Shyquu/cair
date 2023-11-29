unit Unit1;

{$mode objfpc}{$H+}

interface
uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls,fpjson,jsonparser, jsonConf, math, jsonneuralnet;

type

  { TForm1 }

  TForm1 = class(TForm)
    B_Load: TButton;
    E_Input: TEdit;
    L_Out: TLabel;
    procedure B_LoadClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { private declarations }
  public
    NN: NNetCluster;
    { public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.lfm}

{ TForm1 }

procedure TForm1.B_LoadClick(Sender: TObject);
begin

    NN.LoadFile(E_Input.Text);

    L_Out.Caption:=NN.NNetCluster[0].Test;//IntToStr(NN.LayerCount); //
    L_Out.Caption:=FloatToStr(NN.NNetCluster[0].Layers[0].GetWeight(1,2));

end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  NN := NNetCluster.Create;
end;

end.

