#define motor1_pin1 2
#define motor1_pin2 3

#define motor2_pin1 5
#define motor2_pin2 4

#define motor3_pin1 7
#define motor3_pin2 6

#define motor4_pin1 8
#define motor4_pin2 9

#define motorL_speed 10
#define motorR_speed 11

int motor_pin1;
int motor_pin2;
int speed_pin;

int motor_speed;

#include "readserial.h"
#include "gyruskop.h"
#include "movement.h"
#define tx 13
#define rx 12 

char result[7]; // temporary variable used in convert function

/*enum Movement {
  forwards, backwards, stopIt, z0, z1, z2
};
enum Motor {
  one, two, three, four, m1, m2, m3, m4
};

Movement M = Movement(z0);
Motor Mo = Motor(m1);*/

char* toStr(int16_t character) { // converts int16 to string and formatting
  sprintf(result, "%6d", character);
  return result;
}

  gyruskop Gyruskop;
  gyruskop Durchschnit;
  movement Motoren;
void setup(){

  pinMode(0,INPUT);
  pinMode(1,OUTPUT);

  //Motoren.setupMotor(2,3,1);
  Serial.begin(9600);
  //setupGyro();
  
  pinMode(13, INPUT);

  motor_speed = 50;
  

/*
  for(int i=0;i<14;i++){
    digitalWrite(i, LOW);
    analogWrite(i, LOW);
  }*/

  //GReconekt();
  //getdataGyro();
  
  /*Durchschnit.GX = Gyruskop.GX;
  Durchschnit.GY = Gyruskop.GY;
  Durchschnit.GZ = Gyruskop.GZ;
  Durchschnit.Temp = Gyruskop.Temp;
  Durchschnit.AX = Gyruskop.AX;
  Durchschnit.AY = Gyruskop.AY;
  Durchschnit.AZ = Gyruskop.AZ;

for(int i=0;i<20;i++){
  GReconekt();
  getdataGyro();
  Durchschnit.quer(Gyruskop);
  delay(5);
}*/



 while (!Serial) {}
 //pinMode(tx, OUTPUT);
 //pinMode(rx, INPUT);
 //SoftwareSerial configBt(rx, tx); // RX, TX
 
 Serial.println("Start");

  movement Motoren;
  gyruskop Gyruskop;
  gyruskop Durchschnit;
//gyruskop Opti;
//gyruskop D2;
//gyruskop D1;
//gyruskop Werte;

  readserial BT;
}

void Reading(){
  //readserial BT;
  //String IP = BT.GetIP();
  String IP = "";
  char temp = "";
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
    if(IP == "C"){
      Motoren.ReadComands();
    }else if(IP == "J"){
      Motoren.ReadJoyStick();
  }
  }
}

void loop(){

  Reading();
  //Motoren.ReadComands();

  delay(10);
  
}

/*void Opt(){
   Opti.AX = Gyruskop.AX - Durchschnit.AX;
   Opti.AY = Gyruskop.AY - Durchschnit.AY;
   Opti.AZ = Gyruskop.AZ - Durchschnit.AZ;
   Opti.Temp = Gyruskop.Temp - Durchschnit.Temp;
   Opti.GX = Gyruskop.GX - Durchschnit.GX;
   Opti.GY = Gyruskop.GY - Durchschnit.GY;
   Opti.GZ = Gyruskop.GZ - Durchschnit.GZ; 
}*/
