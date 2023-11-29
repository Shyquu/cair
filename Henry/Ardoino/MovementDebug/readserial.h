#ifndef READSERIAL_H
#define READSERIAL_H

#endif

#include "HardwareSerial.h"
#include "strukturs.h"


//#include "readserial.h"
//SoftwareSerial configBt(rx, tx);
//configBt.begin(9600);//38400);


//Bluetooth
//#include <SoftwareSerial.h>
class readserial{

  public:
  int TimeOutCount = 0;
  bool TimeOut = false;
  reader(){
    Serial.println("Lissen");
  }

  String GetIP(){
    char temp = "";
    if (Serial.available() > 0){
      temp = Serial.read();
      if(String(temp) == "#"){
          resetFunc();
        }
      return String(temp);
    }else{
      return "";
    }
  }

  String read(){
    String ret = "";
    char temp = "";
    if (Serial.available() > 0){
        //Serial.println("[");
    }
    if (Serial.available() > 0){
      while ((Serial.available() > 0)) {
        
        temp = Serial.read();
        //Serial.print("In:");
        //Serial.println(temp);
        if(String(temp) == "#"){
          resetFunc();
        }
        ret = ret + String(temp);
        
      }
        //Serial.println();
        //Serial.println(ret + ">\n");
        return ret;
    }
  }


  Cords readCords(){
    Cords ret;
    if (Serial.available() > 0){
      while (Serial.available() > 0) {

        ret.x = Serial.parseInt();
        ret.y = Serial.parseInt();

      }
      Serial.println("x"+ String(ret.x) + " Y"+String(ret.y));
      return ret;
    }
  }

  char readChar(){
    if (Serial.available() > 0){
        this->TimeOutCount = 0;
        return Serial.read();
    }
    if(TimeOutCount >= 100){
      return "0";
    }
    this->TimeOutCount += 1;
    return "N";

  }
  

};



/*class ReadComands : reader{

  

}*/


/*String readCords(){

  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    int X = Serial.parseInt();
    // do it again:
    int Y = Serial.parseInt();
    // do it again:

    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') {
    }
  }
}*/

