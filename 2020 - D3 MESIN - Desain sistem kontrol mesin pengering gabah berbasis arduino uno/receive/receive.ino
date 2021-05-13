#include <SoftwareSerial.h>

SoftwareSerial Serial2(0,1);
String data;

const int IN1 = 5;
const int IN2 = 6;
const int ENA = 11;

void setup() {
Serial2.begin(9600);
pinMode (IN1, OUTPUT);
pinMode (IN2, OUTPUT);
pinMode (ENA, OUTPUT);    
}

void loop() {

  while(Serial.available()>0)
  {
    delay(10);
    char c = Serial.read();
    data+=c;    
    }
    if (data.length()>0)
    {
      Serial.println(data);
      data="";
      motordc1();
      }
  
}

void motordc1()
{
     analogWrite(ENA, 255);//set PWM kecepatan Torsi Motor
     digitalWrite(IN1, HIGH);
     digitalWrite(IN2, LOW);
     Serial.print("Motor DC 1 ON");
     delay(3000);
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, LOW);
     delay(3000);
     digitalWrite(IN1, LOW);
     digitalWrite(IN2, HIGH);
     //lcd.clear();
     //lcd.setCursor(0,0);
     //lcd.print("Motor DC 1 ON");     
     //delay(2000);//delay 2 detik ,orde satuan milisecond
     //digitalWrite(IN1, LOW);
     //digitalWrite(IN2, LOW);
  }
