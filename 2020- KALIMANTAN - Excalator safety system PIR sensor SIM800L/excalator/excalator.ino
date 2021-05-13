#include <SoftwareSerial.h>

SoftwareSerial sim(10,11);
String nomor = "+6282335684250";// ini adalah nomor telepon tujuan yang akan dikirimi sms oleh alat.. jangan lupa untuk tetap menuliskan kode negara +62 didepan sebagai pengganti 0
int PIR = 2;
int statusPIR = 0;
int data = 0;     

void setup()
{
  Serial.begin(9600);
  pinMode(PIR, INPUT);    // inisial input
  sim.begin(9600);
  delay(200);
  
  }

void loop(){
  data = digitalRead(PIR);
  
  if (data == HIGH) { // check if the sensor is HIGH
   if (statusPIR == LOW){
   statusPIR= HIGH;
   Serial.println("Motion detected!"); 
   delay(10);                   // delay 100 milliseconds 
   /*Send SMS*/
   sim.println("AT+CMGF=1");
   delay(10);
   sim.println("AT+CMGS=\"" + nomor + "\"\r");
   delay(10);
   String SMS = "Motion Detected!"; //isi SMS
   sim.println(SMS);
   delay(100);
   sim.println((char)26);
   delay(10);
   /*Send SMS*/
   
   
   }
  } 
  else {
    if (data == HIGH) { // check if the sensor is LOW
    Serial.println("Motion stopped!");
    delay(100);                   // delay 100 milliseconds 
    statusPIR = LOW; 
    }   
   }
    
  }

  
