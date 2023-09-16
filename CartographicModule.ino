// Leander 
// Kartographisches Modul zum Ansteuern der Ultraschallsensoren

#define TRIGGER_PIN_1 2 // Pin für den Trigger des ersten Sensors
#define ECHO_PIN_1 3    // Pin für den Echo des ersten Sensors
#define TRIGGER_PIN_2 4 // Pin für den Trigger des zweiten Sensors
#define ECHO_PIN_2 5    // Pin für den Echo des zweiten Sensors

void setup() {
  // Initialisiere die Serielle Kommunikation für Debugging
  Serial.begin(9600);

  // Konfiguriere die Pins für Trigger und Echo als Ein- bzw. Ausgänge
  pinMode(TRIGGER_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIGGER_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);
}

void loop() {
  // Funktion zur Entfernungsmessung aufrufen
  float distance1 = measureDistance(TRIGGER_PIN_1, ECHO_PIN_1);
  float distance2 = measureDistance(TRIGGER_PIN_2, ECHO_PIN_2);

  // Die gemessenen Entfernungen auf der Seriellen Schnittstelle ausgeben
  Serial.print("Entfernung Sensor 1: ");
  Serial.print(distance1);
  Serial.println(" cm");
  
  Serial.print("Entfernung Sensor 2: ");
  Serial.print(distance2);
  Serial.println(" cm");
  
  delay(1000); // Eine Sekunde warten, bevor die nächste Messung durchgeführt wird
}

float measureDistance(int triggerPin, int echoPin) {
  // Sende einen 10us langen Ultraschall-Impuls aus
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Messe die Dauer des Echo-Impulses in Mikrosekunden
  long duration = pulseIn(echoPin, HIGH);

  // Berechne die Entfernung in Zentimetern anhand der Schallgeschwindigkeit (343 m/s)
  float distance = (float)duration * 0.0343 / 2;

  return distance;
}
