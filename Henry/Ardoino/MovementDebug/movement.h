#include "Arduino.h"
#include "HardwareSerial.h"
#ifndef MOVEMENT_H
#define MOVEMENT_H

#endif

//#include "readserial.h"

class motor{

    private:
  
    public:

    int Pin1, Pin2, Tag, Direktion;
    
    motor(int P1 = 0, int P2 = 0, int tag = 0){
        this->Pin1 = P1;
        this->Pin2 = P2;
        this->Tag = tag;
        this->Direktion = 0;

        pinMode(this->Pin1,OUTPUT);
        pinMode(this->Pin2,OUTPUT);

        digitalWrite(this->Pin1, LOW);
        digitalWrite(this->Pin2, LOW);
    }

    void Forward(){
        this->Direktion = 1;
        digitalWrite(this->Pin2, HIGH);
        digitalWrite(this->Pin1, LOW);
    }
    void Backward(){
        this->Direktion = -1;
        digitalWrite(this->Pin1, HIGH);
        digitalWrite(this->Pin2, LOW);
    }

    void Stop(){
        this->Direktion = 0;
        digitalWrite(this->Pin2, LOW);
        digitalWrite(this->Pin1, LOW);
    }

    void Start(int Direktion = 0){
        if (Direktion == 1){
            this->Forward();
        }else if (Direktion == -1){
            this->Backward();
        }else if (Direktion == 0){
            this->Stop();
        }
    }
    
};

class Side{

  public:

    motor M1, M2;
    int SPin;
    int speed;
    int speedP;

    Side(motor MF = motor(), motor MB = motor(), int Speed_Pin = 0){
      this->M1 = MF;
      this->M2 = MB;

      this->speed = 50;
      this->speedP = 50;
      this->SPin = Speed_Pin;

      pinMode(this->SPin, OUTPUT);
    }

    int prozent(int prozent){
      //Serial.println("P:"+ String(prozent));
      //Serial.println("Speed:"+ String((float(255)/float(100))*float(prozent)));
      return ((float(255)/float(100))*float(prozent));
    }

    void Motors(int direktion){
      this->SpeedTo(100);
      this->M1.Start(direktion);
      this->M2.Start(direktion);
    }

    void SpeedTo(int speed){
      this->speedP = speed;
      this->SpeedUpdate();
    }

    void SpeedUpdate(){
      this->speed = prozent(this->speedP);
      analogWrite(this->SPin, this->speed);
      //Serial.print(this->speed); Serial.print("=");
      //Serial.println(analogRead(this->SPin));
    }

    void startDirektion(int direktion){
      this->Motors(direktion);
    }

    void start(int speed = NULL){
      int direktion = 0;
      if(speed > 0){
        direktion = 1;
      }else if(speed < 0){
        direktion = -1;
        speed = -speed;
      }
      
      this->SpeedTo(speed);

      this->Motors(direktion);
    }

    void Stop(){
      this->M1.Stop();
      this->M2.Stop();
    }

};

class movement{
private:
public:

    Side Left, Right;
    readserial BT;
    bool OptimComands = false;
    //Datem empfang
    //int incomingByte = 1; //char
    //char Buff[5];
    //int BuffI = 0;
    
    movement() {
        this->Left = Side(motor(2,3,1), motor(5,4,2), 10);
        this->Right = Side(motor(7,6,3), motor(8,9,4), 11);
        Serial.println("Movment Ready");
    }

    void setupMotor(int M1, int M2, int tag){
      if(tag = 1){
        this->Left.M1 = motor(M1, M2, tag);
      }else if(tag = 2){
        this->Left.M2 = motor(M1, M2, tag);
      }else if(tag = 3){
        this->Right.M1 = motor(M1, M2, tag);
      }else if(tag = 1){
        this->Right.M2 = motor(M1, M2, tag);
      }
    }

    motor FindMotor(int Tag){
      if(this->Left.M1.Tag == Tag){
        return this->Left.M1;
      }else if(this->Left.M2.Tag == Tag){
        return this->Left.M2;
      }else if(this->Right.M1.Tag == Tag){
        return this->Right.M1;
      }else if(this->Right.M2.Tag == Tag){
        return this->Right.M2;
      }
    }

    void Start(int Direktion = 1){
      this->Left.Motors(Direktion);
      this->Right.Motors(Direktion);
    }

    void Stop(){
      this->Left.Stop();
      this->Right.Stop();
    }

    void ReadJoyStick(){
      // IP = j
      Cords Joy = this->BT.readCords();
      int R = Joy.y + Joy.x;
      int L = Joy.y - Joy.x;
      Serial.println(L,R);
      this->Left.start(L);
      this->Right.start(R);
    }

    void ReadOptimisedDirektionComands(){
      /*
        IP = o
        W F O   A (Acalerate)
        L D R
        T B P   S (Desalerate)
      */
      char Command = this->BT.readChar();
      String Comand = "";
      if (Command != 20){
        //Serial.print("o ");
        //Serial.print(String(Command));
        //Serial.println(String(Command) == "F");
        Comand = String(char(Command));}
      else{
        return;
      }
      //Serial.print("test ");
      if(Comand == "N"){
        //Serial.println("N");
        return;}
      else if(Comand == "o"){this->OptimComands=false; /*Serial.println("De");*/ return;}
      else if(Comand == "D"){this->Stop(); /*Serial.println("St");*/}
      else if(Comand == "F"){this->Start(1); /*Serial.println("FF");*/}
      else if(Comand == "B"){this->Start(-1); /*Serial.println("BB");*/}
      else if(Comand == "R"){
        this->Right.startDirektion(-1);
        this->Left.startDirektion(1);}
      else if(Comand == "L"){
        this->Right.startDirektion(1);
        this->Left.startDirektion(-1);}
      else if(Comand == "W"){
        this->Right.startDirektion(1);
        this->Left.startDirektion(0);}
      else if(Comand == "O"){
        this->Right.startDirektion(0);
        this->Left.startDirektion(1);}
      else if(Comand == "T"){
        this->Right.startDirektion(-1);
        this->Left.startDirektion(0);}
      else if(Comand == "P"){
        this->Right.startDirektion(0);
        this->Left.startDirektion(-1);
        }
      else{
        Serial.println("KP");
      }
    }

    void ReadComands(){
      // PI = c
      String Buff = this->BT.read();
      if (Buff != ""){
      Serial.print(Buff + ">> ");
        if(String(char(Buff[0])) == "s"){
          if(String(char(Buff[1])) == "+"){
            this->Right.speedP += 10;
            this->Left.speedP += 10;
            Serial.println("+Speed " + String(this->Left.speedP));
          }else if(String(Buff[1]) == "-"){
            this->Right.speedP -= 10;
            this->Left.speedP -= 10;
            Serial.println("+Speed " + String(this->Left.speedP));
            }
          this->Right.SpeedUpdate();
          this->Left.SpeedUpdate();
        }else{
          int Direkt= 0;
        if(String(char(Buff[1])) == "0"){
          Direkt = 0;
          Serial.print("D0 ");
        }else if(String(char(Buff[1])) == "1"){
          Direkt = 1;
          Serial.print("D1 ");
        }else if(String(char(Buff[1])) == "2"){
          Direkt = -1;
          Serial.print("D-1 ");
        }

        if(String(char(Buff[0])) == "1"){
          this->FindMotor(1).Start(Direkt);
          Serial.println("m1");
        }else if(String(Buff[0]) == "2"){
          this->FindMotor(2).Start(Direkt);
          Serial.println("m2");
        }else if(String(Buff[0]) == "3"){
          this->FindMotor(3).Start(Direkt);
          Serial.println("m3");
        }else if(String(Buff[0]) == "4"){
          this->FindMotor(4).Start(Direkt);
          Serial.println("m4");
        }else if(String(Buff[0]) == "*"){
          this->Start(Direkt);
          Serial.println("m*");
        }else if(String(Buff[0]) == "r"){
          this->Right.startDirektion(Direkt);
          Serial.println("mR");
        }else if(String(Buff[0]) == "l"){
          this->Left.startDirektion(Direkt);
          Serial.println("mL");
        }
      } 
    }
    }
};


/*void move (Movement movement, Motor motor, int motor_speed) {
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
}*/