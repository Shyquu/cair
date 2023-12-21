#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SD.h>
#include <math.h>
#include <readserial.h>
#include <NewPing.h>

// LINKS
#define TRIGGER_PIN_1 4                               // Pin für den Trigger des ersten Sensors
#define ECHO_PIN_1 3                                  // Pin für den Echo des ersten Sensors
// MITTE
#define TRIGGER_PIN_2 6                               // Pin für den Trigger des zweiten Sensors
#define ECHO_PIN_2 5                                  // Pin für den Echo des zweiten Sensors
// RECHTS
#define TRIGGER_PIN_3 8                               // Pin für den Trigger des dritten Sensors
#define ECHO_PIN_3 7                                  // Pin für den Echo des dritten Sensors

#define LIMIT 30                                      // Limit für das Auto zum wenden

Adafruit_MPU6050 mpu;
Sd2Card card;
SdVolume volume;
SdFile root;
File coords;
File distance;
bool check;
const int chip = 10;
String final_values;

int i;                                                // Iterationsvariable noetig fuer Matlab und fuer Anzahl der Samples
int leftData;                                         // linkes Sonar
int midData;                                          // Mittleres Sonar
int rightData;                                        // rechtes Sonar

NewPing sonar[3] = {                                  // Sonararray mit 2 Plaetzen, beide Sonare werden mit der NewPing Bibliothek angesprochen
  NewPing(TRIGGER_PIN_1, ECHO_PIN_1),
  NewPing(TRIGGER_PIN_2, ECHO_PIN_2),
  NewPing(TRIGGER_PIN_3, ECHO_PIN_3)
};

void setup() {
  Serial.begin(9600);

  Wire.begin();                                       // Initialisieren der Wire Bibliothek, damit man auf den I2C Bus zugreifen kann

  if (!mpu.begin()) {                                 // Initialisieren der MPU6050 Bibliothek
    Serial.println("MPU6050 NICHT ERKANNT...");
    while (1) {
      delay(10);
    }
  }

  Serial.println("MPU6050 GEFUNDEN...");

  SD.begin();                                         // Initialisieren der SD-Karten Bibliothek damit man auf die Karte zugreifen kann
  Serial.println("INITIALISIEREN DES SD-SPEICHERS...");

  // Spaeter umbennen zu LoggedData_CalInertialAndMag
  coords = SD.open("coords.csv", FILE_WRITE);         // Karte auf der SD-Karte erstellen/oeffnen und auf Schreibmodus stellen
  distance = SD.open("distance.csv", FILE_WRITE);
  check = true;                                       // Check-Boolean auf false setzen, damit kein Code eingelesen wird (nur noetig fuer das testen, unnoetig fuer spaeter)

  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);       // Reichweite auf 4G => Werteabweichung verhindern
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);           // Maximale Drehung auf 2000 Grad

  Serial.println("LESEN...");

  // Initialisieren der Sonar und Iterationswerte
  i = 0;
  leftData = 0;
  rightData = 0;

  delay(100);
}

void loop() {

  i++;
  if(i>9999) return;                                  // Limitierung der Sampling Rate

  delay(7);                                           // Damit die Sampling Rate um 100hz betraegt


  // Erstellung von coords.csv

  sensors_event_t a, g, temp;                         // Variable für die Sensordaten initialisieren
  mpu.getEvent(&a, &g, &temp);                        // Beschleunigungs- und Gyrowerte vom MPU6050 ablesen

  final_values = String(i) + ", " 
  + String(g.gyro.x) + ", " + String(g.gyro.y) + ", " + String(g.gyro.z) + ", " 
  + String(a.acceleration.x) + ", " + String(a.acceleration.y) + ", " + String(a.acceleration.z) 
  + ", 0, 0, 0";

  //Serial.println(final_values);                       // Finalen Werte im CSV-fertigen Format an den Serial Port senden
  //Serial.println(String(i/(millis()/1000)) + " Hz");  // Frequenz ausgeben

  coords.println(final_values);                       // Finalen Werte in die CSV-Datei schreiben
  coords.flush();                                     // Datei speichern

  // Erstellung von distance.csv

  for (int i = 0; i < 3; i++) {

    if(i == 0) leftData = sonar[0].ping_cm();  // LINKS
    if(i == 1) midData = sonar[1].ping_cm();   // MITTE
    if(i == 2) rightData = sonar[2].ping_cm();   // RECHTS

  }

  Serial.println(String(leftData) + "," + String(midData) + "," + String(rightData));


}

