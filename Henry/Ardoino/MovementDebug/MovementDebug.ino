#include "readserial.h"
#include "gyruskop.h"
#include "movement.h"
#define tx 13
#define rx 12 

char result[7]; // temporary variable used in convert function


/*char* toStr(int16_t character) { // converts int16 to string and formatting
  sprintf(result, "%6d", character);
  return result;
}*/

gyruskop Gyruskop;
//gyruskop Durchschnit;
movement Motoren();
void setup(){

  pinMode(0,INPUT);
  pinMode(1,OUTPUT);

  //Motoren.setupMotor(2,3,1);
  Serial.begin(9600);
  //setupGyro();
  
  //pinMode(13, INPUT);

  while (!Serial) {}
  //pinMode(tx, OUTPUT);
  //pinMode(rx, INPUT);
  //SoftwareSerial configBt(rx, tx); // RX, TX
  
  Serial.println("\nStart");

  //movement Motoren;
  //gyruskop Gyruskop = gyruskop();
  //gyruskop Durchschnit;

  //readserial BT;
}

/*void Reading(){
  //readserial BT;
  //String IP = BT.GetIP();
  String IP = "";
  char temp = "";
  if (Motoren.OptimComands){
    Motoren.ReadOptimisedDirektionComands();
  }

  if (Serial.available() > 0){
    //Serial.println("Read");
    temp = Serial.read();
    if(String(temp) == "#"){
      resetFunc();
    }
    IP = String(temp);
  }

  if(IP == ""){
    return;
  }else{
    Serial.println(IP);
    if(IP == "c"){
      Motoren.ReadComands();
    }else if(IP == "j"){
      Motoren.ReadJoyStick();
    }else if(IP == "o"){
      Motoren.OptimComands = true;
    }
  }
}*/

void loop(){
  //Serial.println("W");
  Gyruskop.Update();
  Gyruskop.Draw();
  //Reading();
  //Motoren.ReadComands();
  //Serial.println("W");
  delay(100);
  
}
