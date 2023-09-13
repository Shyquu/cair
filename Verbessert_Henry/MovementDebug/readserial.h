#ifndef READ_H
#define READ_H

#endif

typedef struct cords {
   int x;
   int lastName;
} Cords;

String read(){

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
}

