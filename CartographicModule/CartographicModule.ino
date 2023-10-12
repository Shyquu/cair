#include <NewPing.h>
#include <SPI.h>
#include <SD.h>

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

void setup() {
  // Initialisiere die Serielle Kommunikation für Debugging
  Serial.begin(9600); 

  SD.begin();
  while (!Serial) {}

  Serial.print("\nInitialisieren des SD-Speichers...");

  coords = SD.open("coords.csv", FILE_WRITE);

}

void loop() {

  for (uint8_t i = 0; i < 2; i++) {
    delay(100);
    // Debug only: Werte ausgeben lassen
    int centi = sonar[i].ping_cm();
    Serial.println(String(i) + ": " + String(centi) + "cm");


    coords.println(String(centi));
    //Serial.println("printed.");
    if(centi < 30) {
      Serial.println("T");
    }
  }
  coords.flush();

}

void processGyroData(float x, float y, float z) {
  
}

String mapCoords(float x, float y, float z) {
  return crds = x + "," + y + "," + z;
}