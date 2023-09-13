#ifndef GYRUSKOP_H
#define GYRUSKOP_H

#endif

#include "Wire.h"

#define MPU6050_ADDR 0x68 // Alternatively set AD0 to HIGH  --> Address = 0x69

int16_t accX, accY, accZ, gyroX, gyroY, gyroZ, tRaw; // Raw register values (accelaration, gyroscope, temperature)

//Wire.

class gyruskop{
  private:
  
  public:

  void init() {
    GyroConekt();
    Serial.print("Startet Gyro");

  }
  
  long GX;
  long GY;
  long GZ;
  float Temp;
  long AX;
  long AY;
  long AZ;

  void Update(){
    getdataGyro();
  }

  char* toStr(int16_t character) { // converts int16 to string and formatting
    char result[7];
    sprintf(result, "%6d", character);
    return result;
  }

  void Draw(/*char I*/){
    //Serial.begin(9600);
    Serial.println();
    //Serial.print(I);
    /*Serial.print(" AXD = ");*/ Serial.print(toStr(this->AX));
    /*Serial.print(" | AYD = ");*/ Serial.print(toStr(this->AY)); //toStr()
    /*Serial.print(" | AZD = ");*/ Serial.print(toStr(this->AZ));
    /*Serial.print(" | tmpD= "); Serial.print(this->Temp);*/
    /*Serial.print(" | GXD = ");*/ Serial.print(toStr(this->GX));
    /*Serial.print(" | GYD = ");*/ Serial.print(toStr(this->GY));
    /*Serial.print(" | GZD = ");*/ Serial.print(toStr(this->GZ));
  }
  void quer(gyruskop D){
    this->GX = (this->GX + D.GX)/2;
    this->GY = (this->GY + D.GY)/2;
    this->GZ = (this->GZ + D.GZ)/2;
    this->Temp = (this->Temp + D.Temp)/2;
    this->AX = (this->AX + D.AX)/2;
    this->AY = (this->AY + D.AY)/2;
    this->AZ = (this->AZ + D.AZ)/2;
    
  }
  void flatten(){
    this->GX = this->GX/150;
    this->GY = this->GY/150;
    this->GZ = this->GZ/150;
    this->Temp = this->Temp/100;
    this->AX = this->AX/100;
    this->AY = this->AY/100;
    this->AZ = this->AZ/100; 
  }
  void add(gyruskop D){
    this->GX = this->GX + D.GX;
    this->GY = (this->GY + D.GY);
    this->GZ = (this->GZ + D.GZ);
    this->Temp = (this->Temp + D.Temp);
    this->AX = (this->AX + D.AX);
    this->AY = (this->AY + D.AY);
    this->AZ = (this->AZ + D.AZ);
    
  }
  void weg(gyruskop D){
    this->GX = D.GX*100/2;
    this->GY = D.GY*100/2;
    this->GZ = D.GZ*100/2;
    this->Temp = (this->Temp + D.Temp);
    this->AX = D.AX*100/2;
    this->AY = D.AY*100/2;
    this->AZ = D.AZ*100/2;
    
  }


  void GyroConekt(){
    Wire.begin();
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0); // wake up!
    Wire.endTransmission(true);
  }

  void getdataGyro() {
    this->GyroConekt();
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. 
                                  // As a result, the connection is kept active.
    Wire.requestFrom(MPU6050_ADDR, 14, true); // request a total of 7*2=14 registers

    // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same int16_t variable
    accX = Wire.read()<<8 | Wire.read(); this->AX = accX; // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
    accY = Wire.read()<<8 | Wire.read(); this->AY = accY; // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
    accZ = Wire.read()<<8 | Wire.read(); this->AZ = accZ; // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
    tRaw = Wire.read()<<8 | Wire.read(); this->Temp = (tRaw + 12412.0) / 340.0; // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
    gyroX = Wire.read()<<8 | Wire.read(); this->GX = gyroX; // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
    gyroY = Wire.read()<<8 | Wire.read(); this->GY = gyroY; // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
    gyroZ = Wire.read()<<8 | Wire.read(); this->GZ = gyroZ; // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  }
};