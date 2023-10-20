#include <NewPing.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <math.h>

// Leander 
// Kartographisches Modul zum Ansteuern der Ultraschallsensoren

#define TRIGGER_PIN_1 2 // Pin für den Trigger des ersten Sensors
#define ECHO_PIN_1 3    // Pin für den Echo des ersten Sensors
#define TRIGGER_PIN_2 7 // Pin für den Trigger des zweiten Sensors
#define ECHO_PIN_2 6    // Pin für den Echo des zweiten Sensors
#define LIMIT 30        // Limit für das Auto zum wenden

NewPing sonar[2] = {
  NewPing(TRIGGER_PIN_1, ECHO_PIN_1),
  NewPing(TRIGGER_PIN_2, ECHO_PIN_2)
};

Sd2Card card;
SdVolume volume;
SdFile root;
File coords;

// change this to match your SD shield or module;
// Default SPI on Uno and Nano: pin 10
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// MKRZero SD: SDCARD_SS_PIN

const int chipSelect = 10;
int centi1, centi2;

void setup() {
  // Initialisiere die Serielle Kommunikation für Debugging
  Serial.begin(9600); 

  SD.begin();
  while (!Serial) {}

  Serial.print("\nInitialisieren des SD-Speichers...");

  coords = SD.open("coords.csv", FILE_WRITE);

}

String values;

void loop() {

  for (uint8_t i = 0; i < 2; i++) {

    // Verzögerung, damit der nächste Sensor reagieren kann
    delay(100);

    // Debug only: Werte ausgeben lassen
    if(i = 0) centi1 = sonar[0].ping_cm();
    if(i = 1) centi2 = sonar[0].ping_cm();

    if((centi1 || centi2) < 30) {
      Serial.println("T");
    }
  }

  values = String(centi1) + "," + String(centi2);
  Serial.println(values);
  coords.println(values);
  coords.flush();

}

void processGyroData(float x, float y, float z) {

  // input : geschwindigkeit und weg
  
}

String mapCoords(float x, float y, float z) {
  return String(x) + "," + String(y) + "," + String(z);
}

float prevAccelX=0,prevAccelY=0,prevAccelZ=0;
double prevTime;
double prevVelocityX=0,prevVelocityY=0,prevVelocityZ=0;
float prevCoordinateX=0,prevCoordinateY=0,prevCoordinateZ=0;
int i=1;
#define PI 3.14159265

void fixAccelaration(float *currAccel){
  if ((*currAccel>-0.07)&&(*currAccel<0.07))
    *currAccel=0;
}
void convertAcceleration(float *AccelX,float *AccelY,float *AccelZ,float currAccelX,float currAccelY,float currAccelZ){
 
  while (i){
    prevAccelX=currAccelX;
    prevAccelY=currAccelY;
    prevAccelZ=currAccelZ;
    i=0;
  }
  *AccelX= currAccelX-prevAccelX;
  prevAccelX=currAccelX;
  *AccelY= currAccelY-prevAccelY;
  prevAccelY=currAccelY;
  *AccelZ= currAccelZ-prevAccelZ;
  prevAccelZ=currAccelZ;
  fixAccelaration(AccelX);  fixAccelaration(AccelY);  fixAccelaration(AccelZ);
}

float convertMagentic(float currMagnet,float currAccelVector,float Accel){
  float param;
  param=(currAccelVector-Accel)/9.8;
  currMagnet= acos(param)*180.0/PI;
  if (isnan(currMagnet)){
    if (param<0)
      currMagnet=180;
    else
      currMagnet=0;
  }
  return currMagnet;
}


void correctDegrees (float *DegreesX,float *DegreesY,float *DegreesZ){
  float uncorrDegreesX, uncorrDegreesY, uncorrDegreesZ;
  uncorrDegreesX=*DegreesX;
  uncorrDegreesY=*DegreesY;
  uncorrDegreesZ=*DegreesZ;
  if ((uncorrDegreesY>=90)&&(uncorrDegreesY<=180))
    *DegreesZ=360-uncorrDegreesZ;
  if ((uncorrDegreesX>=90)&&(uncorrDegreesX<=180))
    *DegreesY=360-uncorrDegreesY;
  if ((uncorrDegreesY>=90)&&(uncorrDegreesY<=180))
    *DegreesX=360-uncorrDegreesX;
}

void getCoordinate(double *currCoordinateX,double *currCoordinateY,double *currCoordinateZ,float AccelX,float AccelY,float AccelZ){
  double currentTime;
  currentTime=(millis()-prevTime)/1000;
  prevTime=millis();
  *currCoordinateX=prevCoordinateX+(prevVelocityX*currentTime)+(AccelX*currentTime*currentTime*0.5);
  prevVelocityX=prevVelocityX+(AccelX*currentTime);
  prevCoordinateX=*currCoordinateX;
  *currCoordinateY=prevCoordinateY+(prevVelocityY*currentTime)+(AccelY*currentTime*currentTime*0.5);
  prevVelocityY=prevVelocityY+(AccelY*currentTime);
  prevCoordinateY=*currCoordinateY;
  //Calculate Z coodinate
  *currCoordinateZ=prevCoordinateZ+(prevVelocityZ*currentTime)+(AccelZ*currentTime*currentTime*0.5);
  prevVelocityZ=prevVelocityZ+(AccelZ*currentTime);
  prevCoordinateZ=*currCoordinateZ;
}