#include <SoftwareSerial.h>

SoftwareSerial sim(10,11);
char c;
String halo;
void setup() {
  Serial.begin(9600);
  sim.begin(9600);
}

void loop() {

    while(Serial.available())
    {
      delay(10);
      c = Serial.read();
      halo = String(c);
      if (halo == "SMS TERKIRIM")
      {
        sim.println("AT+CMGF=1\r");
        delay(1000);
        //SIM800L.println("AT+CNMI=2,2,0,0,0"); //enable instruksi SMS
        //SIM800L.println("AT+CMGF=1");   //enable instruksi SMS
        delay(1000);  
        sim.print("AT+CMGS=\"089515480619\"\r");//nomor telepon tujuan SMS yang diganti
        delay(1000);  
        sim.print("LAT");
        sim.println(latitude, 6);
        sim.write("LONG");
        sim.println(longitude, 6);
        delay(100);
        sim.write((char)26);
        //SIM800L.println(0x1A);
        delay(1000);
       }
    }
}
