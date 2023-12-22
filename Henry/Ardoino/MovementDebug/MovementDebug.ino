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
movement Motoren;
void setup(){

  pinMode(0,INPUT);
  pinMode(1,OUTPUT);

  Motoren.setupMotor(2,3,1);
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

void Reading(){
  readserial BT;
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
    delay(10);
    if(IP == "c"){
      Motoren.ReadComands();
    }else if(IP == "j"){
      //Serial.println("Joy");
      //Cords L = BT.readCords();
      Motoren.ReadJoyStick();
    }else if(IP == "o"){
      Serial.println("Opt");
      Motoren.OptimComands = true;
    }
  }
}

void PPing(){
  String H = readserial().read();
  if (H == ""){return;}
  
  Serial.println(H.toInt()+1);
}

void loop(){
  //Serial.println("W");
  //Gyruskop.Update();
  //Gyruskop.Draw();
  Reading();

  //PPing();
  //Cords C = readserial().readCords();
  //Serial.println();
  //Motoren.ReadJoyStick();
  //Motoren.ReadComands();
  //Serial.println("W");
  //delay(100);
  
}
