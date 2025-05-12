unit jsonneuralnet;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils,fpjson, jsonConf,jsonparser, math;

type
  Weights = class
  public
    ID: Integer;
    Input :Integer;
    Output :Integer;
    Neuron: ARRAY[0..900] of ARRAY[0..900] of float;

    function GetWeight(i,o:Integer):float;
  end;

type
  NeuralNet = class
  public
    LayerCount: Integer;
    Layer: Array[0..9] of Integer;
    Layers: ARRAY[0..50] of Weights;
    Description: String;
    Test: String;

    //Procedure LoadFile (FileName : String);
    Procedure LoadFromJson(Model : TJSONObject);

  end;

type
  //Eine Classe Zum Sammeln und organiesieren von mehreren Netzen. In Einer JSON Mehrere Netze

  NNetCluster = class
  public
    NNetCount: Integer;
    LoadetNet: Integer; //Index of loadet Net
    NNetCluster: Array[0..900] of NeuralNet;
    Test: String;

    //procedure Load(Filename: String);
    Procedure LoadFile (FileName : String);
    Procedure Clear();

  private
    Procedure DoParse(P : TJSONParser);
  end;

implementation

function Weights.GetWeight(i,o:Integer):float;
begin
  Result := self.Neuron[o-1][i-1];

end;

Procedure NNetCluster.Clear();
VAR i:Integer;

begin
  for i:=0 to self.LoadetNet-1 DO self.NNetCluster[i] := NeuralNet.Create;
  self.LoadetNet:=0;
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
          //self.Test:=IntToStr(self.Layer[0])+IntToStr(self.Layer[1])+IntToStr(self.Layer[2])+IntToStr(self.Layer[3]);

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
  Arr :TJSONArray;
  typ :Boolean;
  i : Integer;
begin
  Try
    J:=P.Parse;
    Try
      //Writeln('Parse succesful. Dumping JSON data : ');
      If Assigned(J) then
        begin
         tmp := J as TJSONObject;
         if tmp.IndexOfName('models') > -1 THEN
           begin
            Test:='Found models';
            self.LoadetNet:=0;
            for i := 0 to tmp.Items[0].Count-1 DO
                begin
                  self.NNetCluster[i] := NeuralNet.Create;
                  self.NNetCluster[i].LoadFromJson(tmp.Items[0].Items[i] as TJSONObject);
                  self.LoadetNet:= self.LoadetNet+1;
                end;
            //tmp
           end
         else if tmp.IndexOfName('model') > -1 THEN
           begin
            //Test:='Found model';
            if not(self.LoadetNet > 0) then self.LoadetNet:=0;
            self.NNetCluster[self.LoadetNet] := NeuralNet.Create;
            self.NNetCluster[self.LoadetNet].LoadFromJson(tmp.Items[0] as TJSONObject);
            self.LoadetNet:= self.LoadetNet+1;
            Test:=IntToStr(self.LoadetNet);
           end
         else Test := 'Found nothing';

        end
      else
        Writeln('No JSON data available');
    Finally
      FreeAndNil(J);
    end;
  except
    On E : Exception do
      Writeln('An Exception occurred when parsing : ',E.Message);
  end;
end;

end.

