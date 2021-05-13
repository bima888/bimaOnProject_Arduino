#include <SoftwareSerial.h>

SoftwareSerial Serial1(8,9);

void setup() {
Serial1.begin(9600);
Serial.print("TEST");  
}

void loop() {
  
}
