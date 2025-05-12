unit jsonneuralnet;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils,fpjson,jsonparser, math, StdCtrls;

type
  Weights = class //Klasse für die gewichte
  public
    ID: Integer;
    Input :Integer;
    Output :Integer;
    Neuron: ARRAY[0..900] of ARRAY[0..900] of float; //2D Array

    function GetWeight(i,o:Integer):float;
    function GetMax():float;
    function GetMin():float;
  end;

type
  NeuralNet = class //Neural Netz Modell Klasse
  public
    LayerCount: Integer;
    Layer: Array[0..9] of Integer;
    Layers: ARRAY[0..50] of Weights;
    Description: String;
    Test: String;
    Log: TListBox;
    min,max: float;

    constructor Create(LB:TListBox); overload;
    Procedure LoadFromJson(Model : TJSONObject);
    procedure GetMinMax();
  end;

type
  //Eine Klasse Zum Sammeln und organiesieren von mehreren Netzen. In Einer JSON Mehrere Netze

  NNetCluster = class
  public
    NNetCount: Integer; //Menge der gespeicherten Modelle
    LoadetNet: Integer; //Index von Ausgewählten Modell
    NNetCluster: Array[0..900] of NeuralNet;
    Log: TListBox;

    constructor Create(LB:TListBox); overload;
    Procedure LoadFile (FileName : String);
    Procedure Clear();
    Procedure Next();

  private
    Procedure DoParse(P : TJSONParser);
  end;

implementation

constructor NeuralNet.Create(LB:TListBox);//Modell Klassen Builder
begin
  self.Log := LB;
end;

constructor NNetCluster.Create(LB:TListBox);//Listenclassen Builder
begin
  self.Log := LB;
  self.NNetCount:=0;
  self.LoadetNet:=-1;
end;

function Weights.GetMax():float; //Größtes gewicht finden
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

function Weights.GetMin():float;//Kleinstes Gewicht
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

procedure NeuralNet.GetMinMax(); //Suchen und Speichern von Min und Max dur die Layers
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

function Weights.GetWeight(i,o:Integer):float; //Function zum bekommen des Gewichts von Neuronen
begin
  Result := self.Neuron[o-1][i-1];

end;

Procedure NNetCluster.Clear(); //Liste Löschen
VAR i:Integer;
begin
  for i:=0 to self.NNetCount-1 DO self.NNetCluster[i] := NeuralNet.Create;//Objekte Überschreiben
  self.NNetCount:=0;//Zähler auf 0 geladene Modelle setzen
  self.Next(); //Aktualisiren
  self.Log.AddItem('Cleared Model List',self.Log); //Ausführung Loggen
end;

procedure NNetCluster.Next();//Nächstes Modell auswählen
begin
     self.LoadetNet:=self.LoadetNet+1;

     if self.LoadetNet >= self.NNetCount THEN //Wen esnde der Liste ereicht zu 0 springen
       self.LoadetNet:=0;
     if self.NNetCount = 0 THEN self.LoadetNet:=-1;//Wen keine vorhanden sind, Index = -1, kein teil der Liste

     self.Log.AddItem('Model: '+IntToStr(self.LoadetNet+1), self.Log);//Auswahl des moddels Loggen
end;

Procedure NeuralNet.LoadFromJson(Model : TJSONObject);//Modellklasse lähd aus dem übergebennen JSON objekt seine Daten

Var
  JObj: TJSONObject;
  tmp, innerArray: TJSONArray;
  i,k, l : Integer;

begin
    Try//Versuch Datei zu öffnen, falls etwas nicht möglich ist
          self.LayerCount:=Model.Get('LayerCount', 0);
          self.Description:=Model.Get('Description', 'No Name');
          tmp := TJSONArray.Create;
          tmp := Model.Get('LayerSizes',tmp);
          for i:=0 TO tmp.Count-1 DO
              self.Layer[i] := tmp.items[i].AsInteger;//Alle Schichtgrößen Laden
          self.Log.AddItem(IntToStr(self.Layer[0])+IntToStr(self.Layer[1])+IntToStr(self.Layer[2])+IntToStr(self.Layer[3]), self.Log);//Modellaufbau Loggen

          tmp.Clear;
          tmp:=Model.Get('Layers',tmp);
          innerArray:=TJSONArray.Create;
          for i:=0 to tmp.Count-1 DO//Alle Schichten, die Gewichte, Laden
              begin
                JObj:=tmp.items[i] as TJSONObject;
                self.Layers[i] := Weights.Create;
                self.Layers[i].ID:= JObj.Get('ID',-1);//Schicht ID
                self.Layers[i].Input:= JObj.Get('IN',0);//Inputneuronen der Schicht
                self.Layers[i].Output:= JObj.Get('OUT',0);//Ausgangsneuronen der Schicht
                innerArray.Clear;
                innerArray := JObj.Get('weights', innerArray);//Alle Gewichte einzeln Laden
                FOR k:=0 TO innerArray.Count-1 DO
                    FOR l:=0 TO innerArray.Arrays[k].Count-1 DO
                        self.Layers[i].Neuron[k][l]:=innerArray.Arrays[k].items[l].AsFloat;
              end;
    Finally
    end;
end;

Procedure NNetCluster.LoadFile (FileName : String);//Datei öffnen und Modelle Ladne

Var
  F : TFileStream;
  P : TJSONParser;

begin
  F:=TFileStream.Create(FileName,fmopenRead); //Datei Öfnnen
  try //Versuch Datei zu Ladne, falls etwas nicht möglich ist
    P:=TJSONParser.Create(F);
    try
      DoParse(P);//Geladene Datei in JSON vormat bringen und laden
    finally
      FreeAndNil(P);
    end;
  finally
    F.Destroy;
  end;
end;

Procedure NNetCluster.DoParse(P : TJSONParser);//Modelle aus Datei Laden

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
         if tmp.IndexOfName('models') > -1 THEN //Wen h+mehrere Modelle erkannt
           begin
            self.Clear();//Speicher Löschen
            for i := 0 to tmp.Items[0].Count-1 DO//Jedes Modell mit Modellklasse öffnen und in array speichern
                begin
                  self.NNetCluster[i] := NeuralNet.Create(self.Log);
                  self.NNetCluster[i].LoadFromJson(tmp.Items[0].Items[i] as TJSONObject);
                  self.NNetCount:= self.NNetCount+1;//Ein weitere geladen
                end;
           end
         else if tmp.IndexOfName('model') > -1 THEN//Wen nur ein Modell gefunden
           begin
            self.NNetCluster[self.NNetCount] := NeuralNet.Create(self.Log);//Weiteres Modell zu liste hinzufügen und aus JSON Laden
            self.NNetCluster[self.NNetCount].LoadFromJson(tmp.Items[0] as TJSONObject);
            self.NNetCount:= self.NNetCount+1; //ein weiteres
           end
         else Self.Log.AddItem('Found nothing',self.Log);//keine bekannte Dateistruktur


        end
      else
        Writeln('No JSON data available');
    Finally
      FreeAndNil(J);
      Self.Log.AddItem('File Loadet Compleatly', self.Log);//Erfolgreichen Laden Loggen
    end;
  except
    On E : Exception do
      Writeln('An Exception occurred when parsing : ',E.Message);
  end;
end;

end.

