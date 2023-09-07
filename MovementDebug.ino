

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

//Datem empfang
int incomingByte = 1; //char
char Buff[5];
int BuffI;

//Bluetooth
#include <SoftwareSerial.h>
#define tx 13
#define rx 12 

char result[7]; // temporary variable used in convert function

enum Movement {
  forwards, backwards, stopIt, z0, z1, z2
};
enum Motor {
  one, two, three, four, m1, m2, m3, m4
};

Movement M = Movement(z0);
Motor Mo = Motor(m1);

char* toStr(int16_t character) { // converts int16 to string and formatting
  sprintf(result, "%6d", character);
  return result;
}

class GyrusWerte{
  private:
  
  public:

  void init() {

  }
  
  long GX;
  long GY;
  long GZ;
  float Temp;
  long AX;
  long AY;
  long AZ;

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
  void quer(GyrusWerte D){
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
  void add(GyrusWerte D){
    this->GX = this->GX + D.GX;
    this->GY = (this->GY + D.GY);
    this->GZ = (this->GZ + D.GZ);
    this->Temp = (this->Temp + D.Temp);
    this->AX = (this->AX + D.AX);
    this->AY = (this->AY + D.AY);
    this->AZ = (this->AZ + D.AZ);
    
  }
  void weg(GyrusWerte D){
    this->GX = D.GX*100/2;
    this->GY = D.GY*100/2;
    this->GZ = D.GZ*100/2;
    this->Temp = (this->Temp + D.Temp);
    this->AX = D.AX*100/2;
    this->AY = D.AY*100/2;
    this->AZ = D.AZ*100/2;
    
  }
};

GyrusWerte Gyruskop;
GyrusWerte Durchschnit;
GyrusWerte Opti;
GyrusWerte D2;
GyrusWerte D1;
GyrusWerte Werte;

void Opt(){
   Opti.AX = Gyruskop.AX - Durchschnit.AX;
   Opti.AY = Gyruskop.AY - Durchschnit.AY;
   Opti.AZ = Gyruskop.AZ - Durchschnit.AZ;
   Opti.Temp = Gyruskop.Temp - Durchschnit.Temp;
   Opti.GX = Gyruskop.GX - Durchschnit.GX;
   Opti.GY = Gyruskop.GY - Durchschnit.GY;
   Opti.GZ = Gyruskop.GZ - Durchschnit.GZ; 
}


void move (Movement movement, Motor motor, int motor_speed) {
  //Übertragen der pins des Gewählten Motors

  //Motor1 Forne Links
  if(motor == Motor(one) or motor == Motor(m1)) {
  motor_pin1 = motor1_pin1;
  motor_pin2 = motor1_pin2;
  speed_pin = motorL_speed;
  } else
  //Motor2 Hinten Links
  if(motor == Motor(two)or motor == Motor(m2)) {
  motor_pin1 = motor2_pin1;
  motor_pin2 = motor2_pin2;
  speed_pin = motorL_speed;
  } else
  //Motor3 Forne Rechts
  if(motor == Motor(three)or motor == Motor(m3)) {
  motor_pin1 = motor3_pin1;
  motor_pin2 = motor3_pin2;
  speed_pin = motorR_speed;
  } else
  //Motor4 Hinten Rechts
  if(motor == Motor(four)or motor == Motor(m4)) {
  motor_pin1 = motor4_pin1;
  motor_pin2 = motor4_pin2;
  speed_pin = motorR_speed;
  }

  if(movement == Movement(forwards) or movement == Movement(z1)) {
      analogWrite(speed_pin, motor_speed);
      digitalWrite(motor_pin2, LOW);
      digitalWrite(motor_pin1, HIGH);
      //digitalWrite(motor_pin1, HIGH);
      //digitalWrite(motor_pin2, LOW);
      //analogWrite(speed_pin, motor_speed);
    } else 
    if(movement == Movement(backwards) or movement == Movement(z2)) {
      analogWrite(speed_pin, motor_speed); 
      digitalWrite(motor_pin1, LOW);
      digitalWrite(motor_pin2, HIGH);
      //digitalWrite(motor_pin1, LOW);
      //analogWrite(speed_pin, motor_speed);
      
    } else 
    if(movement == Movement(stopIt) or movement == Movement(z0)) {
      //analogWrite(speed_pin, motor_speed);
      analogWrite(speed_pin, motor_speed);
      digitalWrite(motor_pin1, LOW);
      digitalWrite(motor_pin2, LOW);
      
    }
    
}
SoftwareSerial configBt(rx, tx); // RX, TX

void setup(){

  /*Wire.begin();
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // wake up!
  Wire.endTransmission(true);*/
  pinMode(0,INPUT);
  pinMode(1,OUTPUT);


  Serial.begin(9600);
  //setupGyro();
  
  // Motor 1
  pinMode(motor1_pin1,OUTPUT);
  pinMode(motor1_pin2,OUTPUT);
  // Motor 2
  pinMode(motor2_pin1,OUTPUT);
  pinMode(motor2_pin2,OUTPUT);
  // Motor 3
  pinMode(motor3_pin1,OUTPUT);
  pinMode(motor3_pin2,OUTPUT);
  // Motor 4
  pinMode(motor4_pin1,OUTPUT);
  pinMode(motor4_pin2,OUTPUT);

  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  
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

BuffI = 0;


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
  //Serial.begin(9600);
  //GReconekt();
  //getdataGyro();
  

  //Durchschnit.quer(Gyruskop);
  //Gyruskop.flatten();
  //Gyruskop.Draw();
  //Durchschnit.quer(Gyruskop);
  //Durchschnit.Draw('D');
  //Opt();
  //Opti.flatten();
  //Opti.Draw();
  //Opti.flatten();
  //Opti.Draw();
  
  //Werte.weg(Opti);
  //Werte.flatten();
  //Werte.Draw();
  //D1.add(Werte);
  //D1.Draw();
  //D2.add(D1);
  //D2.Draw();

  //move(Movement(z0), Motor(m1));

  if (Serial.available() > 0) {
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
     
  }
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
