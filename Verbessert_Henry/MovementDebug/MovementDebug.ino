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

//Bluetooth
#include <SoftwareSerial.h>

#include "gyruskop.h"
#include "movement.h"
#include "readserial.h"
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
gyruskop Opti;
gyruskop D2;
gyruskop D1;
gyruskop Werte;

void Opt(){
   Opti.AX = Gyruskop.AX - Durchschnit.AX;
   Opti.AY = Gyruskop.AY - Durchschnit.AY;
   Opti.AZ = Gyruskop.AZ - Durchschnit.AZ;
   Opti.Temp = Gyruskop.Temp - Durchschnit.Temp;
   Opti.GX = Gyruskop.GX - Durchschnit.GX;
   Opti.GY = Gyruskop.GY - Durchschnit.GY;
   Opti.GZ = Gyruskop.GZ - Durchschnit.GZ; 
}

SoftwareSerial configBt(rx, tx); // RX, TX

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
 configBt.begin(9600);//38400);

 Serial.println("Start");
 //configBt.println("B_Start");
 //configBt.write("B_Start");
}


void loop(){

  /*if (Serial.available() > 0) {
    // read the incoming byte:
    //Serial.println(Serial.read());
    incomingByte = Serial.read();
    Serial.println("Byte: "+String(incomingByte));
    Buff[BuffI]=int(incomingByte);
    BuffI = BuffI +1;
    if(int(incomingByte) == 10){
      Serial.println("Buff = "+String(Buff[0])+String(Buff[1]));

      if(String(Buff[0]) == "s"){
        if(String(Buff[1]) == "+"){
          motor_speed = motor_speed+10;
          Serial.println("+Speed " + String(motor_speed));
        }else if(String(Buff[1]) == "-"){
          motor_speed = motor_speed-10;
          Serial.println("-Speed " + String(motor_speed));
          }
        analogWrite(motorL_speed, motor_speed);
        analogWrite(motorR_speed, motor_speed);
      }else{
      if(String(Buff[0]) == "0"){
        M = Movement(z0);
        Serial.println("z0");
      }else if(String(Buff[0]) == "1"){
        M = Movement(z1);
        Serial.println("z1");
      }else if(String(Buff[0]) == "2"){
        M = Movement(z2);
        Serial.println("z2");
      }

      if(String(Buff[1]) == "1"){
        Mo = Motor(m1);
        Serial.println("m1");
      }else if(String(Buff[1]) == "2"){
        Mo = Motor(m2);
        Serial.println("m2");
      }else if(String(Buff[1]) == "3"){
        Mo = Motor(m3);
        Serial.println("m3");
      }else if(String(Buff[1]) == "4"){
        Mo = Motor(m4);
        Serial.println("m4");
      }else if(String(Buff[1]) == "*"){
        move(M,Motor(m1),motor_speed);
        move(M,Motor(m2),motor_speed);
        move(M,Motor(m3),motor_speed);
        Mo = Motor(m4);
        Serial.println("m*");
      }else if(String(Buff[1]) == "r"){
        move(M,Motor(m1),motor_speed);
        Mo = Motor(m2);
        Serial.println("mR");
      }else if(String(Buff[1]) == "l"){
        move(M,Motor(m3),motor_speed);
        Mo = Motor(m4);
        Serial.println("mL");
      }
      move(M,Mo,motor_speed);
    }
      BuffI = 0;
    }
     
  }*/
  //configBt.println("Test");
  //configBt.write("Test");
  //Serial.println("test");
  /*incomingByte = configBt.read();
  if(configBt.available()> 0) // if the HC05 is sending something… 
  {
    configBt.write(configBt.read()); // print in serial monitor
    Serial.println("Bt received: ");
    configBt.println("Bt:" + configBt.read()); // print in serial monitor
  }*/
  /*if(Serial.available()) // if serial monitor is outputting something… 
  {
    configBt.write(Serial.read()); // write to Arduino’s Tx pin
  }*/

  delay(10);

  
}
