#ifndef MOVEMENT_H
#define MOVEMENT_H

#endif

class motor{

    private:
  
    public:

    int Pin1, Pin2, Tag;
    
    motor(int P1 = 0, int P2 = 0, int tag = 0){
        this->Pin1 = P1;
        this->Pin2 = P2;
        this->Tag = tag;

        pinMode(this->Pin1,OUTPUT);
        pinMode(this->Pin2,OUTPUT);

        digitalWrite(this->Pin1, LOW);
        digitalWrite(this->Pin2, LOW);
    }

    void Forward(){
        digitalWrite(this->Pin2, LOW);
        digitalWrite(this->Pin1, HIGH);
    }
    void Backward(){
        digitalWrite(this->Pin1, LOW);
        digitalWrite(this->Pin2, HIGH);
    }

    void Stop(){
        digitalWrite(this->Pin2, LOW);
        digitalWrite(this->Pin1, LOW);
    }

    void Start(int Direktion = 1){
        if (Direktion = 1){
            this->Forward();
        }else if (Direktion = -1){
            this->Backward();
        }else if (Direktion = 0){
            this->Stop();
        }
    }
    
};

class Side{

  public:

    motor M1, M2;
    int SPin;
    float speed;

    Side(motor MF = motor(), motor MB = motor(), int Speed_Pin = 0){
      this->M1 = MF;
      this->M2 = MB;

      this->speed = 50;
      this->SPin = Speed_Pin;

      pinMode(this->SPin, OUTPUT);
    }

    float prozent(int prozent){
      return (255/100)*prozent;
    }

    void Motors(int direktion){
      this->M1.Start(direktion);
      this->M2.Start(direktion);
    }

    void start(int speed){
      int direktion = 0;
      if(speed > 0){
        direktion = 1;
      }else if(speed <0){
        direktion = -1;
        speed = speed*(-1);
      }
      this->speed = prozent(speed);
      analogWrite(this->SPin, this->speed);

      Motors(direktion);
    }
};

class movement{
private:
public:

    Side Left, Right;

    //Datem empfang
    int incomingByte = 1; //char
    char Buff[5];
    int BuffI = 0;
    
    movement() {
        this->Left = Side(motor(2,3,1), motor(5,4,2), 10);
        this->Right = Side(motor(7,6,3), motor(8,9,4), 11);

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

    void ReadComands(){
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
          //M = Movement(z0);
          Serial.println("z0");
        }else if(String(Buff[0]) == "1"){
          //M = Movement(z1);
          Serial.println("z1");
        }else if(String(Buff[0]) == "2"){
          //M = Movement(z2);
          Serial.println("z2");
        }

        if(String(Buff[1]) == "1"){
          //Mo = Motor(m1);
          Serial.println("m1");
        }else if(String(Buff[1]) == "2"){
          //Mo = Motor(m2);
          Serial.println("m2");
        }else if(String(Buff[1]) == "3"){
          //Mo = Motor(m3);
          Serial.println("m3");
        }else if(String(Buff[1]) == "4"){
          //Mo = Motor(m4);
          Serial.println("m4");
        }else if(String(Buff[1]) == "*"){
          //move(M,Motor(m1),motor_speed);
          //move(M,Motor(m2),motor_speed);
          //move(M,Motor(m3),motor_speed);
          //Mo = Motor(m4);
          Serial.println("m*");
        }else if(String(Buff[1]) == "r"){
          //move(M,Motor(m1),motor_speed);
          //Mo = Motor(m2);
          Serial.println("mR");
        }else if(String(Buff[1]) == "l"){
          //move(M,Motor(m3),motor_speed);
          //Mo = Motor(m4);
          Serial.println("mL");
        }
        //move(M,Mo,motor_speed);
      }
        BuffI = 0;
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