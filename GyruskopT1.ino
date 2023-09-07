#include "Wire.h" 

#define MPU6050_ADDR 0x68 // Alternatively set AD0 to HIGH  --> Address = 0x69

int16_t accX, accY, accZ, gyroX, gyroY, gyroZ, tRaw; // Raw register values (accelaration, gyroscope, temperature)

//Wire.



void GReconekt(){
  Wire.begin();
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // wake up!

  Wire.endTransmission(true);}

void setupGyro() {
  //Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // wake up!

  Wire.endTransmission(true);
}

void getdataGyro() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. 
                               // As a result, the connection is kept active.
  Wire.requestFrom(MPU6050_ADDR, 14, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same int16_t variable
  accX = Wire.read()<<8 | Wire.read(); Gyruskop.AX = accX; // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accY = Wire.read()<<8 | Wire.read(); Gyruskop.AY = accY; // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accZ = Wire.read()<<8 | Wire.read(); Gyruskop.AZ = accZ; // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  tRaw = Wire.read()<<8 | Wire.read(); Gyruskop.Temp = (tRaw + 12412.0) / 340.0; // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyroX = Wire.read()<<8 | Wire.read(); Gyruskop.GX = gyroX; // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyroY = Wire.read()<<8 | Wire.read(); Gyruskop.GY = gyroY; // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyroZ = Wire.read()<<8 | Wire.read(); Gyruskop.GZ = gyroZ; // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  
  /*Serial.print("AcX = "); Serial.print(toStr(accX)); 
  Serial.print(" | AcY = "); Serial.print(toStr(accY)); 
  Serial.print(" | AcZ = "); Serial.print(toStr(accZ)); 
  // from data sheet:
  Serial.print(" | tmp = "); Serial.print((tRaw + 12412.0) / 340.0);
  Serial.print(" | GyX = "); Serial.print(toStr(gyroX)); 
  Serial.print(" | GyY = "); Serial.print(toStr(gyroY)); 
  Serial.print(" | GyZ = "); Serial.print(toStr(gyroZ)); 
  Serial.println();*/
}
