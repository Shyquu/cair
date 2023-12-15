unit jsonneuralnet;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils,fpjson,jsonparser, math, StdCtrls;

type
  Weights = class
  public
    ID: Integer;
    Input :Integer;
    Output :Integer;
    Neuron: ARRAY[0..900] of ARRAY[0..900] of float;

    function GetWeight(i,o:Integer):float;
    function GetMax():float;
    function GetMin():float;
  end;

type
  NeuralNet = class
  public
    LayerCount: Integer;
    Layer: Array[0..9] of Integer;
    Layers: ARRAY[0..50] of Weights;
    Description: String;
    Test: String;
    Log: TListBox;
    min,max: float;

    //Procedure LoadFile (FileName : String);
    constructor Create(LB:TListBox); overload;
    Procedure LoadFromJson(Model : TJSONObject);
    procedure GetMinMax();
  end;

type
  //Eine Classe Zum Sammeln und organiesieren von mehreren Netzen. In Einer JSON Mehrere Netze

  NNetCluster = class
  public
    NNetCount: Integer;
    LoadetNet: Integer; //Index of loadet Net
    NNetCluster: Array[0..900] of NeuralNet;
    Test: String;
    Log: TListBox;

    //procedure Load(Filename: String);
    constructor Create(LB:TListBox); overload;
    Procedure LoadFile (FileName : String);
    Procedure Clear();
    Procedure Next();

  private
    Procedure DoParse(P : TJSONParser);
  end;

implementation

constructor NeuralNet.Create(LB:TListBox);
begin
  self.Log := LB;
end;

constructor NNetCluster.Create(LB:TListBox);
begin
  self.Log := LB;
  self.NNetCount:=0;
  self.LoadetNet:=-1;
end;

function Weights.GetMax():float;
VAR i,o:Integer;
  Max:float;
begin
  Max := 0;
  For i:=1 TO self.Input DO
      begin
        For o:=1 TO self.Output DO
            begin
                 IF self.GetWeight(i,o) > Max THEN Max:=self.GetWeight(i,o);
           end;
      end;
  Result:=Max;
end;

function Weights.GetMin():float;
VAR i,o:Integer;
  min:float;
begin
  min := 0;
  For i:=1 TO self.Input DO
      begin
        For o:=1 TO self.Output DO
            begin
                 IF self.GetWeight(i,o) < min THEN min:=self.GetWeight(i,o);
           end;
      end;
  Result:=min;
end;

procedure NeuralNet.GetMinMax();
VAR i:Integer;
begin
  self.Max := 0;
  self.Min := 0;
  For i:=0 TO self.LayerCount-2 DO
      begin
        IF self.Layers[i].GetMax() > Max THEN Max := self.Layers[i].GetMax();
        IF self.Layers[i].GetMin() < Min THEN Min := self.Layers[i].GetMin();
      end;
end;

function Weights.GetWeight(i,o:Integer):float;
begin
  Result := self.Neuron[o-1][i-1];

end;

Procedure NNetCluster.Clear();
VAR i:Integer;
begin
  for i:=0 to self.NNetCount-1 DO self.NNetCluster[i] := NeuralNet.Create;
  self.NNetCount:=0;
  self.Next();
  self.Log.AddItem('Cleared Model List',self.Log);
end;

procedure NNetCluster.Next();
begin
     self.LoadetNet:=self.LoadetNet+1;

     if self.LoadetNet >= self.NNetCount THEN
       self.LoadetNet:=0;
     if self.NNetCount = 0 THEN self.LoadetNet:=-1;

     self.Log.AddItem('Model: '+IntToStr(self.LoadetNet+1), self.Log);
end;

Procedure NeuralNet.LoadFromJson(Model : TJSONObject);

Var
  //J : TJSONData;
  //Model : TJSONObject;
  JObj: TJSONObject;
  tmp, innerArray: TJSONArray;
  i,k, l : Integer;

begin
    Try
          self.Test:= Model.ClassName;
          self.LayerCount:=Model.Get('LayerCount', 0);
          self.Description:=Model.Get('Description', 'No Name');
          self.Test:=self.Description;
          tmp := TJSONArray.Create;
          tmp := Model.Get('LayerSizes',tmp);
          for i:=0 TO tmp.Count-1 DO
              self.Layer[i] := tmp.items[i].AsInteger;
          self.Log.AddItem(IntToStr(self.Layer[0])+IntToStr(self.Layer[1])+IntToStr(self.Layer[2])+IntToStr(self.Layer[3]), self.Log);

          tmp.Clear;
          tmp:=Model.Get('Layers',tmp);
          //self.Test:=IntToStr(tmp.Count);
          innerArray:=TJSONArray.Create;
          for i:=0 to tmp.Count-1 DO
              begin
                JObj:=tmp.items[i] as TJSONObject;
                self.Layers[i] := Weights.Create;
                self.Layers[i].ID:= JObj.Get('ID',-1);
                self.Layers[i].Input:= JObj.Get('IN',0);
                self.Layers[i].Output:= JObj.Get('OUT',0);
                innerArray.Clear;
                innerArray := JObj.Get('weights', innerArray);
                FOR k:=0 TO innerArray.Count-1 DO
                    FOR l:=0 TO innerArray.Arrays[k].Count-1 DO
                        self.Layers[i].Neuron[k][l]:=innerArray.Arrays[k].items[l].AsFloat;
              end;
        //end

      //else

        //Writeln('No JSON data available');
    Finally
      //FreeAndNil(Model);
    end;
  //except
    //On E : Exception do
      //Writeln('An Exception occurred when parsing : ',E.Message);
  //end;
end;

Procedure NNetCluster.LoadFile (FileName : String);

Var
  F : TFileStream;
  P : TJSONParser;

begin
  //FileName.;
  F:=TFileStream.Create(FileName,fmopenRead);
  try
    // Create parser with Stream as source.
    P:=TJSONParser.Create(F);
    try
      DoParse(P);
    finally
      FreeAndNil(P);
    end;
  finally
    F.Destroy;
  end;
end;

Procedure NNetCluster.DoParse(P : TJSONParser);

Var
  J : TJSONData;
  tmp: TJSONObject;
  i : Integer;
begin
  Try
    J:=P.Parse;
    Try
      self.Log.AddItem('Parse succesful. Dumping JSON data : ',self.Log);
      If Assigned(J) then
        begin
         tmp := J as TJSONObject;
         if tmp.IndexOfName('models') > -1 THEN
           begin
            Test:='Found models';
            self.Clear();
            for i := 0 to tmp.Items[0].Count-1 DO
                begin
                  self.NNetCluster[i] := NeuralNet.Create(self.Log);
                  self.NNetCluster[i].LoadFromJson(tmp.Items[0].Items[i] as TJSONObject);
                  self.NNetCount:= self.NNetCount+1;
                end;
            //tmp
           end
         else if tmp.IndexOfName('model') > -1 THEN
           begin
            //Test:='Found model';
            self.NNetCluster[self.NNetCount] := NeuralNet.Create(self.Log);
            self.NNetCluster[self.NNetCount].LoadFromJson(tmp.Items[0] as TJSONObject);
            self.NNetCount:= self.NNetCount+1;

            Test:=IntToStr(self.NNetCount);
           end
         else Test := 'Found nothing';

        end
      else
        Writeln('No JSON data available');
    Finally
      FreeAndNil(J);
      Self.Log.AddItem('File Loadet Compleatly', self.Log);
    end;
  except
    On E : Exception do
      Writeln('An Exception occurred when parsing : ',E.Message);
  end;
end;

end.

