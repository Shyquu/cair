#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

float elapsedTime, currentTime, previousTime;
float accX, accY, accZ;
float velX = 0, velY = 0, velZ = 0;
float posX = 0, posY = 0, posZ = 0;

// Kalman-Filter Variablen
float Q_angle = 0.001; // Prozessrauschen für den Winkel (beschleunigung)
float Q_gyro = 0.003;  // Prozessrauschen für den Gyro
float R_angle = 0.03;  // Messrauschen für den Winkel

float angle = 0; // Geschätzter Winkel

void setup() {
  Serial.begin(9600);

  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 Verbindung fehlgeschlagen");
    while (1);
  }
}

void loop() {
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000.0; // in seconds

  mpu.getMotion6();

  accX = mpu.getAccelerationX();
  accY = mpu.getAccelerationY();
  accZ = mpu.getAccelerationZ();

  // Kalman-Filter zur Schätzung des Winkels anwenden
  float gyroRate = mpu.getRotationY(); // Hier wird der Y-Gyro-Wert verwendet, aber andere Achsen könnten ebenfalls verwendet werden
  angle += elapsedTime * (gyroRate - angle) + Q_gyro * elapsedTime;

  // Integration von Beschleunigung zu Geschwindigkeit
  velX += accX * elapsedTime;
  velY += accY * elapsedTime;
  velZ += accZ * elapsedTime;

  // Integration von Geschwindigkeit zu Position
  posX += velX * elapsedTime;
  posY += velY * elapsedTime;
  posZ += velZ * elapsedTime;

  // Kalman-Filter zur Korrektur der Positionsschätzung verwenden
  float y = posX - angle; // Messung (Residuum)
  float S = R_angle + elapsedTime * Q_angle * elapsedTime; // Residuum-Kovarianz
  float K = S / (S + R_angle); // Kalman-Filter-Gewicht
  angle += K * y; // Geschätzten Winkel aktualisieren
  posX += K * y;  // Geschätzte Position aktualisieren

  // Ausgabe der Position
  Serial.print("Position: ");
  Serial.print("X = "); Serial.print(posX);
  Serial.print(" | Y = "); Serial.print(posY);
  Serial.print(" | Z = "); Serial.println(posZ);

  // Wartezeit für die nächste Iteration
  delay(100); // Kleine Pause für stabilere Schätzungen
  previousTime = currentTime;
}
