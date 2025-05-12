unit Unit1;

{$mode objfpc}{$H+}

interface
uses
  Classes, SysUtils, FileUtil, OpenGLContext, Forms, Controls, Graphics,
  Dialogs, StdCtrls, ExtCtrls, ComCtrls, fpjson, jsonparser, math,
  jsonneuralnet, GLU, GL;

type

  { TForm1 }

  TForm1 = class(TForm)
    B_Info: TButton;
    B_Auto: TButton;
    B_Next: TButton;
    B_Print: TButton;
    B_Clear: TButton;
    Label1: TLabel;
    Label2: TLabel;
    L_Auto: TLabel;
    LB_Log: TListBox;
    L_Beschreibung: TLabel;
    M_Info: TMemo;
    OnApp: TApplicationProperties;
    B_Load: TButton;
    E_Input: TEdit;
    L_Out: TLabel;
    OGL_Screen: TOpenGLControl;
    OpenDialog1: TOpenDialog;
    P_Steuerung: TPanel;
    TB_Speed: TTrackBar;
    procedure B_AutoClick(Sender: TObject);
    procedure B_ClearClick(Sender: TObject);
    procedure B_InfoClick(Sender: TObject);
    procedure B_LoadClick(Sender: TObject);
    procedure B_NextClick(Sender: TObject);
    procedure B_PrintClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure OGL_ScreenPaint(Sender: TObject);
    procedure OGL_ScreenResize(Sender: TObject);
    procedure OnAppIdle(Sender: TObject; var Done: Boolean);
    procedure PrintNeuralNet(Netz :NeuralNet);
  private
    { private declarations }
    NN: NNetCluster;
    Clock:Integer;
  public

    { public declarations }
  end;

var
  Form1: TForm1;
  Auto:Boolean = False;

implementation

procedure PrintCircle(r,x,y:float);//Zeichnen eines Kreises mit Dreiecken
VAR I:Integer;
begin
   glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y); // Mittelpunkt
        for I := 0 to 360 DO glVertex2f(r*cos(3.14* I / 180.0) + x, r*sin(3.14 * i / 180.0) + y);//Von Grad zu Grad ein Dreieck bis es ein Kreis ist

   glEnd();

end;

{$R *.lfm}

{ TForm1 }

procedure TForm1.B_LoadClick(Sender: TObject);
var filename : String;
begin

   if OpenDialog1.Execute then //Datei abfragen
   begin
     filename := OpenDialog1.Filename;
   end;
    E_Input.Text:=filename; //Den Edit auf den Dateifad ändern
    NN.LoadFile(E_Input.Text); //Datei in Modellliste laden

    LB_Log.AddItem('Models Loadet: '+IntToStr(NN.NNetCount),LB_Log);//Menge der geladnenen Modelle in den Log schreiben

    NN.Next();//Modellauswahl Aktualisiren
    B_Print.Caption:='Zeichnen '+IntToStr(NN.LoadetNet+1) + '/'+IntToStr(NN.NNetCount); //Angabe des Geladenen netzes und der gesamt Menge

end;

procedure TForm1.B_NextClick(Sender: TObject);
begin
  NN.Next();//Nächstes Modell  Auswählen
  B_Print.Caption:='Zeichnen '+IntToStr(NN.LoadetNet+1) + '/'+IntToStr(NN.NNetCount); //Update
end;

procedure TForm1.B_PrintClick(Sender: TObject);
begin
  B_Print.Caption:='Zeichnen '+IntToStr(NN.LoadetNet+1) + '/'+IntToStr(NN.NNetCount);//Update
  OGL_Screen.Invalidate;//OpenGL Zeichenen
end;

procedure TForm1.B_ClearClick(Sender: TObject);
begin
  NN.Clear(); //Modellliste Löschen
  B_Print.Caption:='Zeichnen '+IntToStr(NN.LoadetNet+1) + '/'+IntToStr(NN.NNetCount);//Update
end;

procedure TForm1.B_InfoClick(Sender: TObject);
begin
  //Toggel der Informationsbox
  IF B_Info.Caption='Erklärung'then
    begin
      B_Info.Caption:='Zurück';
      M_Info.Visible:=true;
    end
  else
      begin
        B_Info.Caption:='Erklärung';
        M_Info.Visible:=False;
      end;
end;

procedure TForm1.B_AutoClick(Sender: TObject);
begin
  //Toggel Autofunktion
  IF Auto THEN Auto := False
  ELSE Auto := True;

  IF Auto THEN L_Auto.Caption:='ON'
  ELSE L_Auto.Caption:='OFF';      //Anzeigen
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  NN := NNetCluster.Create(LB_Log);//Listenklasse Initialisiren
  Clock:=0;//Zeit auf 0 stezen
  Application.AddOnIdleHandler(@OnAppIdle); //AppIdle starten
end;

procedure TForm1.FormResize(Sender: TObject);
begin //Größen anpassen
  M_Info.Width:=Form1.Width-P_Steuerung.Width;
  M_Info.Height:=Form1.Height;
  P_Steuerung.Left:=Form1.Width-P_Steuerung.Width;
  P_Steuerung.Height:=Form1.Height;
  OGL_Screen.Width:=Form1.Width-P_Steuerung.Width;
end;

procedure TForm1.OGL_ScreenPaint(Sender: TObject);
VAR Breite,Hoehe : Integer;

begin
  Breite := OGL_Screen.Width;
  Hoehe := OGL_Screen.Height;
  glViewport(0,0,Breite,Hoehe);          //Viewport einstellungen
  glClearColor(1.0,1.0,1.0,0.0);         // Hintergrundfarbe Weiß
  glClear(GL_COLOR_BUFFER_BIT OR GL_DEPTH_BUFFER_BIT);

  // Normalenbezug und Normalenberechnung
  glEnable(GL_NORMALIZE);

  // Projektionsmatrix laden
  glMatrixMode(GL_PROJECTION);   // Szene
  glLoadIdentity();

  // Festlegung der Projektion
  glOrtho(-1.0,1.0,-1.0,1.0,0.1,100.0);

  // Transformation der Projektionsmatrix

  // Modellmatrix laden (Objekt)
  glMatrixMode(GL_MODELVIEW);   // Objekte
  glLoadIdentity();

  // Tiefenpuffereinstellungen
  glEnable(GL_DEPTH_TEST);

  // Lichteinstellungen

  // Kameraposition festlegen
  gluLookAt( 0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);


  // Ausgewähltes Modell Zeichen

  IF NN.LoadetNet >= 0 THEN //Nur wen auch ein da ist
  begin
   LB_Log.AddItem('Showing Model',LB_Log); //Zeichnug des Models und ID loggen
   PrintNeuralNet(NN.NNetCluster[NN.LoadetNet]);//Procedur zum Zeichenen des Models. Ausgewähltes Modell übergeben
   L_Out.Caption:='Model: '+IntToStr(NN.LoadetNet+1)+'/'+IntToStr(NN.NNetCount); //Anzeigeupdate
  end
  else L_Out.Caption:='Kein Model Geladen'; //Ausgabe des Scheiterns

  // Zeichnen
  glFlush();   // Abbilden auf dem BS
  OGL_Screen.SwapBuffers; //BS aktualisieren
end;

procedure TForm1.PrintNeuralNet(Netz :NeuralNet);//Procedur zum Zeichenen des Übergebennen Models
VAR
  I,J,K:Integer;
  LayerSpace, NeuronSpace, x:float;
begin
  L_Beschreibung.Caption:=Netz.Description; //Beschreibung anzeigen

  LayerSpace:=2.0/(Netz.LayerCount); //Abstand der Schichten erechnen

  for I:=0 To Netz.LayerCount-1 DO
  begin
    x := -1.0+(LayerSpace*(I+0.5));//X wärt für die Schich angeben

    NeuronSpace:=2.0/(Netz.Layer[I]); //Abstand der Neuronnen dieser Schicht erechnen
    for J:=0 TO Netz.Layer[I] DO
    begin
        glColor3f(1.0,1.0,1.0);      // Weiß
        PrintCircle(0.07,x,1.0 - (NeuronSpace*(J+0.5)));//Zeichnen eines Kreises in Weiß mit Radius 0.07. Damit es Ein schwartzer Rand ist.

        glColor3f(0.0,0.0,0.0);      // Schwartz
        PrintCircle(0.08,x,1.0 - (NeuronSpace*(J+0.5)));//Eichenendes Neurons mit Radius 0.08
      end;
  end;

  for I:=0 To Netz.LayerCount-2 DO  //Zeichnen der Gewichte
  begin
    x := -1.0+(LayerSpace*(I+0.5)); //X wärte der ersten Schicht

    for J:=0 TO Netz.Layer[I]-1 DO
    begin
      for K:=0 TO Netz.Layer[I+1]-1 DO
      begin
        glColor3f(0.0,0.0,0.0); // Schwartz
        glLineWidth(0.0);

        Netz.GetMinMax();//Aktualisiren des Größten und des Klensten gewichtes als Referenz zur Liniengröße

        if Netz.Layers[I].GetWeight(J+1,K+1) > 0 THEN //Positives Gewicht
            begin
                 glLineWidth(10.0*((Netz.Layers[I].GetWeight(J+1,K+1)/Netz.Max)));//Einstellen der Dicke Prozentual zum Maximum
                 glColor3f(1.0,0.0,0.0); //Rot
            end;
        if Netz.Layers[I].GetWeight(J+1,K+1) < 0 THEN //Negatives Gewicht
            begin
                 glLineWidth(10.0*(((Netz.Layers[I].GetWeight(J+1,K+1)*-1)/(Netz.Min*-1))));//Einstellen der Dicke Prozentual zum Minimum
                 glColor3f(0.0,0.0,1.0); //Blau
            end;

        glBegin(GL_LINES);
            glVertex2f(-1.0+(LayerSpace*(I+0.5)),1.0 - (2.0/(Netz.Layer[I])*(J+0.5)));   // Angabe des Ersten Neurons
            glVertex2f(-1.0+(LayerSpace*(I+1.5)),1.0 - (2.0/(Netz.Layer[I+1])*(K+0.5)));   // Angabe des Zweiten Neurons
        glEnd;
      end;
    end;
  end;

end;

procedure TForm1.OGL_ScreenResize(Sender: TObject);
begin
     IF OGL_Screen.Height <= 0 THEN Exit; //Wen man nichts mehr sied schliest das Programm sich
end;

procedure TForm1.OnAppIdle(Sender: TObject; var Done: Boolean);//Idel Loop
begin
  Done:=False;

  IF Auto THEN //Wen Automodus Aktiv
      begin
        sleep(100);
        Clock+= 100; //Clock um die gewarteten 100 erwitern

        If (Clock >= 5000-(TB_Speed.Position*50)) THEN //Wen Limmit ereicht. Bestimmt die Geschwindikeit des durhwächseln
            begin
             Clock:=0;//Zeit zurücksetzen
             IF (TB_Speed.Position > -10) THEN //Wen nicht auf Stillstand gestelt
                 begin
                  NN.Next();//Nächstes Modell auswählen
                  OGL_Screen.Invalidate; //OpenGL Aktualisiren
                 end;
            end;
      end;
end;

end.

