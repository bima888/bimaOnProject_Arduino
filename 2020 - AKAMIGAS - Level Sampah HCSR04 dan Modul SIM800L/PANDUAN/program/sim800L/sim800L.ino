#include <SoftwareSerial.h>

SoftwareSerial SIM800L(10,11);
String nomor = "+6281238740744";// ini adalah nomor telepon tujuan yang akan dikirimi sms oleh alat.. jangan lupa untuk tetap menuliskan kode negara +62 didepan sebagai pengganti 0
int trigger = 4;
int echo = 3;
int duration, cm, inches;

void setup ()
{
  SIM800L.begin(9600);
  delay(2000);
  Serial.begin(9600);
  delay(2000);
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

 if (cm >= 20)
 {
  Serial.print("Keranjang sampah belum penuh");
  delay(200);
    }
  else if (cm <= 18)
  {
   Serial.print("Keranjang sampah penuh");
   delay(200);
   SIM800L.println("AT+CMGF=1");
   delay(500);
   SIM800L.println("AT+CMGS=\""+nomor+"\"\r");
   delay(500);
   String sms = "Keranjang sampah penuh";
   SIM800L.println(sms);
   delay(1000);
   SIM800L.println((char)26);
   delay(300);
   
    }
  }
