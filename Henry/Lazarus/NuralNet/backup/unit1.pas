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
    B_Auto: TButton;
    B_Next: TButton;
    B_Print: TButton;
    B_Clear: TButton;
    Label1: TLabel;
    Label2: TLabel;
    L_Auto: TLabel;
    LB_Log: TListBox;
    L_Beschreibung: TLabel;
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
    procedure B_LoadClick(Sender: TObject);
    procedure B_NextClick(Sender: TObject);
    procedure B_PrintClick(Sender: TObject);
    procedure E_InputChange(Sender: TObject);
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

procedure PrintCircle(r,x,y:float);
VAR I:Integer;
begin
   glBegin(GL_TRIANGLE_FAN);

        glVertex2f(x, y); // Center
        for I := 0 to 360 DO glVertex2f(r*cos(3.14* I / 180.0) + x, r*sin(3.14 * i / 180.0) + y);

   glEnd();

end;

{$R *.lfm}

{ TForm1 }

procedure TForm1.B_LoadClick(Sender: TObject);
var filename : String;
begin

   if OpenDialog1.Execute then
   begin
     filename := OpenDialog1.Filename;
     //ShowMessage(filename);
   end;
    E_Input.Text:=filname;
    NN.LoadFile(E_Input.Text);

    LB_Log.AddItem('Models Loadet: '+IntToStr(NN.NNetCount),LB_Log);
    //LB_Log.AddItem(FloatToStr(NN.NNetCluster[0].Layers[0].GetWeight(1,2)),LB_Log);

    NN.Next();
    B_Print.Caption:='Lade '+IntToStr(NN.LoadetNet+1) + '/'+IntToStr(NN.NNetCount);

end;

procedure TForm1.B_NextClick(Sender: TObject);
begin
  NN.Next();
  B_Print.Caption:='Lade '+IntToStr(NN.LoadetNet+1) + '/'+IntToStr(NN.NNetCount);
end;

procedure TForm1.B_PrintClick(Sender: TObject);
begin
  B_Print.Caption:='Lade '+IntToStr(NN.LoadetNet+1) + '/'+IntToStr(NN.NNetCount);
  OGL_Screen.Invalidate;
end;

procedure TForm1.B_ClearClick(Sender: TObject);
begin
  NN.Clear();
  B_Print.Caption:='Lade '+IntToStr(NN.LoadetNet+1) + '/'+IntToStr(NN.NNetCount);
end;

procedure TForm1.B_AutoClick(Sender: TObject);
begin
  IF Auto THEN Auto := False
  ELSE Auto := True;

  IF Auto THEN L_Auto.Caption:='ON'
  ELSE L_Auto.Caption:='OFF';
end;

procedure TForm1.E_InputChange(Sender: TObject);
begin

end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  NN := NNetCluster.Create(LB_Log);
  Clock:=0;
  Application.AddOnIdleHandler(@OnAppIdle);
end;

procedure TForm1.FormResize(Sender: TObject);
begin
   P_Steuerung.Left:=Form1.Width-P_Steuerung.Width;
   P_Steuerung.Height:=Form1.Height;
   OGL_Screen.Width:=Form1.Width-P_Steuerung.Width;
end;

procedure TForm1.OGL_ScreenPaint(Sender: TObject);
VAR Breite,Hoehe : Integer;

begin
  Breite := OGL_Screen.Width;
  Hoehe := OGL_Screen.Height;
  glViewport(0,0,Breite,Hoehe);
  glClearColor(1.0,1.0,1.0,0.0);         // Hintergrundfarbe Weiß
  glClear(GL_COLOR_BUFFER_BIT OR GL_DEPTH_BUFFER_BIT);

  // Normalenbezug und Normalenberechnung
  glEnable(GL_NORMALIZE);

  // Projektionsmatrix laden
  glMatrixMode(GL_PROJECTION);   // Szene
  glLoadIdentity();

  // Festlegung der Projektion
  glOrtho(-1.0,1.0,-1.0,1.0,0.1,100.0);
  //OR
  //gluPerspective(90,Breite/Hoehe,0.1,100);


  // Transformation der Projektionsmatrix

  // Modellmatrix laden (Objekt)
  glMatrixMode(GL_MODELVIEW);   // Objekte
  glLoadIdentity();

  // Tiefenpuffereinstellungen
  glEnable(GL_DEPTH_TEST);

  // Lichteinstellungen

  // Kameraposition festlegen
  gluLookAt( 0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);

  // Objekttransformation
  //glRotatef(WinkelX,1.0,0.0,0.0);    // Drehung um X-Achse mit Winkel
  //glRotatef(WinkelY,0.0,1.0,0.0);    // Drehung um Y-Achse mit Winkel
  //glRotatef(WinkelZ,0.0,0.0,1.0);    // Drehung um Z-Achse mit Winkel
  //glTranslatef(X,Y,Z);               // Verschiebung zum Punkt
  //glScalef(ScaleX,ScaleY,ScaleZ);    // gleichmäßige Skalierung nach X, Y und Z

  // Polygonmoduseinstellen
  //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  //OR
  //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  //OR
  //glPointSize(3);
  //glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);




  // -- Vordergrund

  IF NN.LoadetNet >= 0 THEN
  begin
   LB_Log.AddItem('Showing Model',LB_Log);
  PrintNeuralNet(NN.NNetCluster[NN.LoadetNet]);
  L_Out.Caption:='Model: '+IntToStr(NN.LoadetNet+1)+'/'+IntToStr(NN.NNetCount);
  end
  else L_Out.Caption:='Kein Model Geladen';

 {
    glVertex3f(-0.2, 0.2, 0.2);  // E
    glVertex3f( 0.2, 0.2, 0.2);  // F
    glVertex3f( 0.2, 0.2,-0.2);  // G
    glVertex3f(-0.2, 0.2,-0.2);  // H
  glEnd;
  // -- Seite rechts
  glBegin(GL_QUADS);
    glColor3f(1.0,0.5,0.25);      // Orange
    glVertex3f( 0.2,-0.2, 0.2);  // B
    glVertex3f( 0.2,-0.2,-0.2);  // C
    glVertex3f( 0.2, 0.2,-0.2);  // G
    glVertex3f( 0.2, 0.2, 0.2);  // F
  glEnd;
  // -- Seite links
  glBegin(GL_QUADS);
    glColor3f(1.0,0.0,1.0);      // Pink
    glVertex3f(-0.2,-0.2, 0.2);  // A
    glVertex3f(-0.2,-0.2,-0.2);  // D
    glVertex3f(-0.2, 0.2,-0.2);  // H
    glVertex3f(-0.2, 0.2, 0.2);  // E
  glEnd;
  // -- Hintergrund
  glBegin(GL_QUADS);
    glColor3f(0.5,0.5,0.5);      // Grau
    glVertex3f(-0.2,-0.2,-0.2);  // D
    glVertex3f( 0.2,-0.2,-0.2);  // C
    glVertex3f( 0.2, 0.2,-0.2);  // G
    glVertex3f(-0.2, 0.2,-0.2);  // H
  glEnd;
   }
  // Textur löschen

  // Transformationsänderung
  // --- Auslagerung in die Buttons

  // Zeichnen
  glFlush();   // Abbilden auf dem BS
  OGL_Screen.SwapBuffers; //BS aktualisieren

end;

procedure TForm1.PrintNeuralNet(Netz :NeuralNet);
VAR
  I,J,K:Integer;
  LayerSpace, NeuronSpace, x:float;
begin
  L_Beschreibung.Caption:=Netz.Description;

  LayerSpace:=2.0/(Netz.LayerCount);
  //LB_Log.AddItem('LayerSpace:'+FloatToStr(LayerSpace), LB_Log);

  for I:=0 To Netz.LayerCount-1 DO
  begin
    x := -1.0+(LayerSpace*(I+0.5));
    //LB_Log.AddItem('X:'+FloatToStr(x), LB_Log);
    {glColor3f(1.0,1.0,1.0);      // Weiß
    PrintCircle(0.07,x,0.0);

    glColor3f(0.0,0.0,0.0);      // Schwartz
    PrintCircle(0.08,x,0.0);}

    NeuronSpace:=2.0/(Netz.Layer[I]);
    for J:=0 TO Netz.Layer[I] DO
    begin
        glColor3f(1.0,1.0,1.0);      // Weiß
        PrintCircle(0.07,x,1.0 - (NeuronSpace*(J+0.5)));

        glColor3f(0.0,0.0,0.0);      // Schwartz
        PrintCircle(0.08,x,1.0 - (NeuronSpace*(J+0.5)));
      end;
  end;

  LayerSpace:=2.0/(Netz.LayerCount);
  //LB_Log.AddItem('LayerSpace:'+FloatToStr(LayerSpace), LB_Log);

  for I:=0 To Netz.LayerCount-2 DO
  begin
    x := -1.0+(LayerSpace*(I+0.5));

    //NeuronSpace:=2.0/(Netz.Layer[I]);
    for J:=0 TO Netz.Layer[I]-1 DO
    begin
      for K:=0 TO Netz.Layer[I+1]-1 DO
      begin
        glColor3f(0.0,0.0,0.0); // Schwartz
        glLineWidth(0.0);

        Netz.GetMinMax();
        //LB_Log.AddItem('Max/Min: '+FloatToStr(Netz.Max)+'/'+FloatToStr(Netz.Min),LB_Log);
        if Netz.Layers[I].GetWeight(J+1,K+1) > 0 THEN
            begin
                 glLineWidth(10.0*((Netz.Layers[I].GetWeight(J+1,K+1)/Netz.Max)));
                 glColor3f(1.0,0.0,0.0); //Rot
            end;
        if Netz.Layers[I].GetWeight(J+1,K+1) < 0 THEN
            begin
                 glLineWidth(10.0*(((Netz.Layers[I].GetWeight(J+1,K+1)*-1)/(Netz.Min*-1))));
                 glColor3f(0.0,0.0,1.0); //Blau
            end;

        glBegin(GL_LINES);
        glVertex2f(-1.0+(LayerSpace*(I+0.5)),1.0 - (2.0/(Netz.Layer[I])*(J+0.5)));   // IN
        glVertex2f(-1.0+(LayerSpace*(I+1.5)),1.0 - (2.0/(Netz.Layer[I+1])*(K+0.5)));   // OUT
        glEnd;
      end;
    end;
  end;

  {glColor3f(1.0,1.0,1.0);      // Weiß
  PrintCircle(0.07,0,0);

  glColor3f(0.0,0.0,0.0);      // Schwartz
  PrintCircle(0.08,0,0);        }

 { glBegin(GL_LINES);
    glColor3f(1.0,1.0,1.0); // Weiß
    glVertex2f(-1.0,0);   // x-Achse -x
    glVertex2f( 1.0,0);   // x-Achse +x
  glEnd;}

end;

procedure TForm1.OGL_ScreenResize(Sender: TObject);
begin
     IF OGL_Screen.Height <= 0 THEN Exit;
end;

procedure TForm1.OnAppIdle(Sender: TObject; var Done: Boolean);
begin
  Done:=False;

  IF Auto THEN
      begin
        sleep(100);
        Clock+= 100;

        If (Clock >= 5000-(TB_Speed.Position*50)) THEN
            begin
             Clock:=0;
             IF (TB_Speed.Position > -10) THEN
                 begin
                  NN.Next();
                  OGL_Screen.Invalidate;
                 end;
            end;
      end;
end;

end.

