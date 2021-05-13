#include <SoftwareSerial.h>

SoftwareSerial SIM800L(10,11);

int trigger = 11;
int echo = 12;
int duration, cm, inches;

void setup ()
{
  Serial.begin(115200);
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  }

void loop () 
{
  digitalWrite(trigger, LOW);
  delayMicroseconds (5);
  digitalWrite(trigger, HIGH);
  delayMicroseconds (10);
  digitalWrite(trigger, LOW);


  pinMode(echo, INPUT);
  duration  = pulseIn(echo, HIGH);


  cm = (duration/2) / 29.1;
  inches = (duration/2) / 74;

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm ");
  Serial.println();
  
  delay(250); 

 if (cm >= 7)
 {
  Serial.print("Keranjang sampah belum penuh");
  delay(200);
    }
  else if (cm)
  {
   Serial.print("Keranjang sampah penuh");
   delay(200);
   SIM800L.write("AT+CMGF=1\r\n");
   delay(500);
   SIM800L.write("AT+CMGS=\"082335684250\"\r\n");
   delay(500);
   SIM800L.write("Keranjang sampah penuh");
   delay(250);
   SIM800L.write((char)26);
   delay(300);
   
    }
  }
