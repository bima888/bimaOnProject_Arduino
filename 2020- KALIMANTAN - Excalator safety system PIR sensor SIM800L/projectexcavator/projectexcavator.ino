#include<SoftwareSerial.h>
SoftwareSerial SIM800L(10,11);
int PIR = 2;              
int data = 0;     

void setup() {

  pinMode(PIR, INPUT);    
  Serial.begin(9600);    
  SIM800L.begin(9600);
}

void loop(){
  data = digitalRead(PIR);
  if (digitalRead(PIR) == HIGH) { 
    Serial.println("Motion detected!"); 
    delay(100);       
    SIM800L.println("AT+CNMI=2,2,0,0,0");
    SIM800L.println("AT+CMGS=1");   
    delay(5000);  
    SIM800L.println("AT+CMGS=\"082335684250\"\r");//nomor telepon tujuan SMS yang diganti
    delay(1000);
    SIM800L.println("ada yang gerak nih");
    delay(100);
    SIM800L.println((char)26);
    delay(1000);
  } 
  
  else {
    Serial.println("Motion stopped!");
    delay(100);                   
}
