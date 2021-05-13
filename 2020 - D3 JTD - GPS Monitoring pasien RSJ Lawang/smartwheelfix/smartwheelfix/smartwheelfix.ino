#include <ArduinoJson.h>
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

#define FIREBASE_HOST "wheel-c097a.firebaseio.com"              //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "WwFRAezGKnlE4gs8WYuL4p65RdNAMP2M1HypuPDC"       //Your Firebase Database Secret goes here
#define WIFI_SSID "diki"                                     //your WiFi SSID for which yout NodeMCU connects
#define WIFI_PASSWORD "misquinn"                                      //Password of your wifi network 

#include <TinyGPS++.h>
TinyGPSPlus gps;

int R ; // variabel untuk seleksi kondisi relay
int metode ; // untuk switch case
float latitude ;
float longitude ;
String lati, longi ; //variabel untuk di kirim ke firebase

#define relay 4
#define buzzer 5
#define ijo 23
#define merah 22
#define putih 21

void setup() {
  Serial.begin(9600) ;
  pinMode(ijo,OUTPUT) ; //Led Hijau Indikator Kursi Roda Jalan/nyala// Relay OFF
  pinMode(merah,OUTPUT) ; //Led Merah Indikator Kursi Roda Mati/waktu habis// Relay ON
  pinMode(buzzer,OUTPUT) ;  //Buzzer nyala indikator kursi mau habis // Relay ON
  pinMode(putih,OUTPUT) ; //Led Putih nyala indikator Standby// Relay OFF
  pinMode(relay,OUTPUT) ; //pin Relay
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected:");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
}

void loop() {
    while (Serial.available() > 0){
    gps.encode(Serial.read());
    if (gps.location.isUpdated()){
      latitude =  gps.location.lat() ; //mendapatkan nilai latitude
      longitude = gps.location.lng() ; //mendapatkan nilai longitude
      lati = String(latitude,6) ;
      longi = String (longitude,6) ;
      Serial.print("Latitude= "); 
      Serial.print (latitude,6); //Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println (longitude,6); //Serial.println(gps.location.lng(), 6);
      Firebase.setString("latitude",lati);
      Firebase.setString("longitude",longi);
      
   R=Firebase.getString("durasi").toInt();   //Pembacaan di Firebase
   Serial.print(R) ;
   if(R==0)
   metode = 1 ;
   else if(R==1 || R==2 || R==3)
   metode = 2 ;
   else if (R==5)
   metode = 3 ;
   else if (R==9)
   metode = 4 ;
switch (metode){
  case 1 : mati() ;
  break ;
  case 2 : nyala() ;
  break ;
  case 3 : standby() ;
  break ;
  case 4 : buzer() ;
  break ;
}
    }
    }
     R=Firebase.getString("durasi").toInt();   //Pembacaan di Firebase
     Serial.print(R) ;
     if(R==0)
     metode = 1 ;
     else if(R==1 || R==2 || R==3)
     metode = 2 ;
     else if (R==5)
     metode = 3 ;
     else if (R==9)
     metode = 4 ;
     
switch (metode){
  case 1 : mati() ;
  break ;
  case 2 : nyala() ;
  break ;
  case 3 : standby() ;
  break ;
  case 4 : buzer() ;
  break ;
}
}
void mati(){
  Serial.println(" Kursi Roda Ngunci") ; //Relay ON led merah nyala
  digitalWrite(ijo,LOW) ;
  digitalWrite(putih,LOW) ;
  digitalWrite(buzzer,LOW);
  digitalWrite(merah,HIGH) ;
  digitalWrite(relay,HIGH); //lampu di relay mati
}
void nyala() {
  Serial.println(" Kursi Roda Aktif Bisa Jalan") ; // Relay OFF led hijau nyala
  digitalWrite(ijo,HIGH) ;
  digitalWrite(merah,LOW) ;
  digitalWrite(buzzer,LOW) ;
  digitalWrite(putih,LOW) ;
  digitalWrite(relay,LOW); //lampu di relay nyala
}
void standby() { 
  Serial.println(" Kursi Roda Standby") ; //Relay OFF led putih nyala
  digitalWrite(putih,HIGH) ;
  digitalWrite(merah,LOW) ;
  digitalWrite(buzzer,LOW) ;
  digitalWrite(ijo,LOW) ;
  digitalWrite(relay,LOW); //lampu di relay nyala
}
void buzer(){
  Serial.println(" Kursi Roda Mau Habis") ; //Relay OFF led hijau nyala
  digitalWrite(ijo,HIGH) ;
  digitalWrite(buzzer,HIGH);
  digitalWrite(relay,LOW);
  delay(200) ;
  digitalWrite(ijo,HIGH) ;
  digitalWrite(buzzer,LOW);
  digitalWrite(relay,LOW);
  delay(200) ;
}
