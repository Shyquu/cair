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
        delay(5);
        
      }
        //Serial.println();
        //Serial.println(ret + ">\n");
        return ret;
    }
  }


  Cords readCords(){
    Cords ret;
    ret.x=0;
    ret.y=0;
    if (Serial.available() > 0){
      String tmp = "";
      char t = "";
      while (Serial.available() > 0) {
        t = Serial.read();
        if(String(t) == " "){
          ret.x=String(tmp).toInt();
          tmp = "";
          continue;
        }
        tmp = tmp + String(t);
        //Serial.println("LL");
        //ret.x = Serial.parseInt();
        //ret.y = Serial.parseInt();

        delay(10);

      }
      ret.y = String(tmp).toInt();
      Serial.println("X"+ String(ret.x) + " Y"+String(ret.y));
      return ret;
    }
  }

  char readChar(){
    if (Serial.available() > 0){
        //this->TimeOutCount = 0;
        //delay(5);
        char tmp = Serial.read();
        if(String(tmp) == "#"){
          resetFunc();
        }
        return tmp;
    }
    /*if(TimeOutCount >= 100){
      return "N";
    }
    this->TimeOutCount += 1;
    return "N";*/
    return "";

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

